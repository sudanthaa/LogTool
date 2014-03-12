#include "LTPch.h"

#include <libssh2.h>
#include <curl/curl.h>

#include "LTUploadTask.h"
#include "LTEnv.h"
#include "LTJiraCredentials.h"
#include "LTScreenshot.h"
#include "LTSshSession.h"
#include "LTConfig.h"
#include "LTUtils.h"
#include "LTDlg.h"

#define SI __FILE__, __LINE__

#define  curl_easy_setopt_VA_fix_call    curl_easy_setopt
#define LOGIN_ERR_MSG  "If Login is failed once due to authentication failure, " \
	"jira may have temporarily halted the REST-API connection for your account. \n" \
	"Login once to jira via web will revert this. " \
	"Check whether you have entered the correct username/password in LogTool before continuing"

LTUploadTask::LTUploadTask(void)
{
	p_Env = NULL;
	p_JiraCred = NULL;
	p_LogEnv = NULL;
	p_EnvSession = NULL;
	e_TaskType = TT_UPLOAD;
	b_CreateNew = false;
	b_DoJiraComment = false;
	b_ScreenshotAttach = false;
	b_CustomCommands = false;
	b_Success = false;
}

LTUploadTask::~LTUploadTask(void)
{
}

bool LTUploadTask::DoTask(LTUploadTask* pTask )
{
	if (pTask->e_TaskType == TT_UPLOAD)
		return DoUploadTask(pTask);
	else if (pTask->e_TaskType == TT_FILE_QUERY)
		return DoFileQuery(pTask);

	return false;
}

bool LTUploadTask::DoFileQuery(LTUploadTask* pTask)
{
	LTSshSession* pSession = *pTask->p_EnvSession;
	
	if (!pSession)
	{
		pSession = new LTSshSession;
		if (pSession->Connect(pTask->p_Env, pTask->s_Error))
		{
			*pTask->p_EnvSession = pSession;
		}
		else
		{
			delete pSession;
			pSession = NULL;
		}
	}

	if (pSession)
	{
		pTask->b_Success = 
			pSession->Execute("ls -1 logs/*:*:* corefiles/*", &pTask->lst_Output, &pTask->s_Error);
	}

	return pTask->b_Success;
}

bool LTUploadTask::DoUploadTask( LTUploadTask* pTask )
{
	LTSshSession* pSessionEnv = *pTask->p_EnvSession;
	LTEnv* pLogEnv = pTask->p_LogEnv;

	// Taking the public key from environment
	//////////////////////////////////////////////////////////////////////////
	std::list<CString> lst;
	if (!pSessionEnv->Execute("cat .ssh/id_rsa.pub", &lst, &pTask->s_Error))
	{
		pTask->s_Error = "Key upload failed";
		return false;
	}

	if (lst.size() == 0)
	{
		pTask->s_Error  = "Public key not found in environment";
		return false;
	}

	CString sKey = lst.front();


	// Connect to Log environement.
	LTSshSession* pLogEnvSession = new LTSshSession;
	if (!pLogEnvSession->Connect(pLogEnv, pTask->s_Error))
	{
		delete pLogEnvSession;
		return false;
	}

	// Putting your environments public key as a trusted public key in log environment
	//////////////////////////////////////////////////////////////////////////
	CString sCmd;
	sCmd.Format("cur_key=`grep \"%s\" .ssh/authorized_keys ` ; if [ -z \"$cur_key\"  ]; then echo adding key to authorized list; mkdir -p .ssh; echo \"%s\" >> .ssh/authorized_keys; else echo key already found authorized; fi"
		, sKey, sKey);

	std::list<CString> lstOut;
	pLogEnvSession->Execute(sCmd, &lstOut);



	// Collecting logs
	//////////////////////////////////////////////////////////////////////////
	CString sTempDir;
	sTempDir.Format("/tmp/%I64u", time(NULL));

	CString sMkTmpDir;
	sMkTmpDir.Format("mkdir -p %s", sTempDir);

	lstOut.clear();
	pSessionEnv->Execute(sMkTmpDir, &lstOut);



	CString sFileList = "";
	std::list<CString>::iterator itr = pTask->lst_UploadFiles.begin();

	// Compression log files to export.
	for (;itr != pTask->lst_UploadFiles.end(); itr++)
	{
		CString sSel = *itr;
		CString sZipCmd;
		CString sFile;
		int iLO =  sSel.ReverseFind('/');
		CString sRelFileName = iLO > -1 ? sSel.Right(sSel.GetLength() - iLO - 1) : sSel; 

		sFile.Format("%s/%s.gz",  sTempDir, sRelFileName);
		sZipCmd.Format("gzip -c %s > %s", sSel, sFile);

		sFileList += " ";
		sFileList += sFile;

		pSessionEnv->Execute(sZipCmd, &lstOut);
	}

	// Creating JiraID, create jira ticket if necessary
	if (pTask->b_CreateNew)
	{
		CString sNewJiraTicketID;
		if (!CreateJiraTicket(pTask->s_JiraProject, pTask->s_JiraIssueType, 
			sNewJiraTicketID, pTask->s_Error, pTask->p_JiraCred, 
			pTask->s_JiraIssueSummary, pTask->s_JiraIssueDescription))
			return false;

		pTask->s_JiratFullTicketID = sNewJiraTicketID;
	}

	CString sJiraTicketID = pTask->s_JiratFullTicketID;


	// Make log location
	//////////////////////////////////////////////////////////////////////////
	CString sTicketPath;
	if (pTask->s_EnvPath.GetLength() > 0)
		sTicketPath.Format("%s/%s", pTask->s_EnvPath, sJiraTicketID);
	else
		sTicketPath = sJiraTicketID;

	CString sTicketDirMkCmd;
	sTicketDirMkCmd.Format("mkdir -p %s", sTicketPath);

	pLogEnvSession->Execute(sTicketDirMkCmd);


	delete pLogEnvSession;
	pLogEnvSession = NULL;


	// Copy logs
	//////////////////////////////////////////////////////////////////////////
	CString sCopyCmd;
	sCopyCmd.Format("scp %s %s@%s:%s", sFileList, pLogEnv->s_EnvUser, pLogEnv->s_IP, sTicketPath);

	pSessionEnv->Execute(sCopyCmd);

	// Delete temp directory
	//////////////////////////////////////////////////////////////////////////
	CString sCleanDirCmd;
	sCleanDirCmd.Format("rm -rf %s", sTempDir);
	lstOut.clear();

	pSessionEnv->Execute(sCleanDirCmd);


	// Execute custom commands.
	//////////////////////////////////////////////////////////////////////////

	CString sSSHConnStr;
	sSSHConnStr.Format("%s@%s", pLogEnv->s_EnvUser, pLogEnv->s_IP);

	for (LST_CA::iterator itr = pTask->lst_CustomActions.begin(); 
				itr !=  pTask->lst_CustomActions.end(); itr++)
	{
		LTConfig::CustomAction* pAction = *itr;
		CString sFullCommand;
		sFullCommand.Format("logenvuser=%s;"
			"logenvip=%s;"
			"logenvpath=%s;"
			"logenvsshcon=%s;"
			"%s", pLogEnv->s_EnvUser, pLogEnv->s_IP,
			sTicketPath, sSSHConnStr, pAction->s_Command);

		std::list<CString> lst;
		CString sErr;
		pSessionEnv->Execute(sFullCommand,  &lst, &sErr);
	}


	// Add Comment on jira
	//////////////////////////////////////////////////////////////////////////
	if (pTask->b_DoJiraComment)
	{
		if (!PutJiraComment(sJiraTicketID, pTask->p_Env, pLogEnv, sTicketPath,
			pTask->s_Error, pTask->p_JiraCred, pTask->lst_Screenshots))
			return false;
	}

	return true;
}

//**************************************************************************************************
bool LTUploadTask::PutJiraComment(const char* zTiceketID, LTEnv* pDevEnv, LTEnv* pLogEnv, 
							const char* zTicketPath,CString& sErr, LTJiraCredentials* pCred,
							std::list<LTScreenshot*>& lstScreenshots)
{
	if (!pCred)
	{
		sErr = "Jira credentials is null";
		return false;
	}

	CString sAttachmentURL;
	sAttachmentURL.Format("%s/rest/api/2/issue/%s/comment",  pCred->s_URL, zTiceketID);

	CString sAuth;
	pCred->GetAuthCode(sAuth);

	CString sXShellFile;
	sXShellFile.Format("%s_logs.xsh", zTiceketID);
	CString sVersion = LTUtils::GetProductVersionX();

	bool bSuccess = false;

	CURL* pCurlComment = NULL;
	pCurlComment = curl_easy_init();
	LTCurlRetunBuffer oBuff;
	LTCurlRetunBuffer oHeaderBuff;

	if (pCurlComment) 
	{ 
		struct curl_slist *chunk = NULL;
		chunk = curl_slist_append(chunk, "Content-Type: application/json");

		CString sComment;
		sComment.Format("{ \"body\" : \"Logs uploaded to below location.\\r\\n"
			"\\r\\n"
			" _Location_: %s@%s:%s \\r\\n"
			" _Password_: %s \\r\\n"
			" _XShell-Link_: [^%s] \\r\\n"
			" _Uploaded from_: %s@%s\\r\\n"
			" {color:gray}~_Uploaded via LogTool v%s_~{color} \"\n }",
			pLogEnv->s_EnvUser, pLogEnv->s_IP, zTicketPath,
			pLogEnv->s_Password,
			sXShellFile,
			pDevEnv->s_EnvUser, pDevEnv->s_IP,
			sVersion);

		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_URL, sAttachmentURL.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_POST, 1);	
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_USERPWD, sAuth.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_POSTFIELDS, sComment.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_POSTFIELDSIZE, sComment.GetLength());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_HTTPHEADER, chunk);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_WRITEDATA, &oBuff);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_WRITEFUNCTION, LTCurlRetunBuffer::_WritFunc);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_HEADERDATA,  &oHeaderBuff);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_HEADERFUNCTION, LTCurlRetunBuffer::_WritFunc);

		CURLcode res = curl_easy_perform(pCurlComment);
		/* Check for errors */ 
		if (res == CURLE_OK)
		{
			bSuccess = true;

			LTHTTPResponse* pResponse = LTHTTPResponse::Create(oHeaderBuff.GetFullOutput());
			if (pResponse)
			{
				LTHTTPResponse::ReturnCodeType eCode = pResponse->GetReturnCodeType();
				if (eCode != LTHTTPResponse::RCT_OK)
				{
					bSuccess = false;
					if (eCode == LTHTTPResponse::RCT_ERROR)
					{
						CString sLoginReason = pResponse->GetAttribute("X-Seraph-LoginReason");
						CString sLoginDeniedReason  = pResponse->GetAttribute("X-Authentication-Denied-Reason");
						sErr.Format("%s:%d: %s:%s\n%s", pResponse->s_Msg, pResponse->i_ReturnCode, 
							sLoginReason, sLoginDeniedReason, LOGIN_ERR_MSG);
					}
				}
			}
		}
		else
		{
			sErr.Format("[%s:%d] curl_easy_perform() failed: %s",
				SI , curl_easy_strerror(res));
		}

		curl_easy_cleanup(pCurlComment);
		curl_slist_free_all(chunk);
	}

	CString sXShell;
	const char* zXShellFmt = 
		"[CONNECTION]\n"
		"Protocol=SSH\n"
		"Host=%s\n"
		"[CONNECTION:AUTHENTICATION]\n"
		"TelnetLoginPrompt=ogin:\n"
		"TelnetPasswordPrompt=assword:\n"
		"UserName=%s\n"
		//"Method=2\n"
		"Method=1\n"
		"UserKey=survlog_trustkey\n"
		"UseExpectSend=1\n"
		"ExpectSend_Count=1\n"
		"ExpectSend_Send_0=cd %s\n"
		"ExpectSend_Expect_0=>\n"
		"ExpectSend_Hide_0=0\n"
		"[TERMINAL:WINDOW]\n"
		"FontSize=9\n"
		"FontFace=Consolas\n";

	sXShell.Format(zXShellFmt, pLogEnv->s_IP, pLogEnv->s_EnvUser, zTicketPath);
	AttachFileToJira(sXShell.GetBuffer(), sXShell.GetLength(), sXShellFile, pCred, sErr);

	std::list<LTScreenshot*>::iterator itr = lstScreenshots.begin();
	for (;itr != lstScreenshots.end(); itr++)
	{
		LTScreenshot* pScreenshot = *itr;
		CString sTmpFileName;
		sTmpFileName.Format("%s\\ss_%lu.jpg", LTUtils::GetTempPath(), time(NULL));

		CString sFileName;
		sFileName.Format("%s.jpg", pScreenshot->GetName());

		pScreenshot->Save(sTmpFileName);
		AttachFileToJira(sTmpFileName, sFileName, pCred, sErr);
	}

	return bSuccess;
}


//**************************************************************************************************
bool LTUploadTask::CreateJiraTicket( const char* zProject, const char* zIssueType, CString& sTicket,
							 CString& sErr, LTJiraCredentials* pCred,
							 const char* zSummary, const char* zDescription)
{
	if (!pCred)
	{
		sErr = "Jira credentials is empty";
		return false;
	}

	CString sAttachmentURL;
	sAttachmentURL.Format("%s/rest/api/2/issue/", pCred->s_URL);

	CString sAuth;
	pCred->GetAuthCode(sAuth);

	bool bSuccess = false;

	CURL* pCurlComment = NULL;
	pCurlComment = curl_easy_init();
	if (pCurlComment) 
	{ 
		LTCurlRetunBuffer oBuff;
		LTCurlRetunBuffer oHeaderBuff;

		struct curl_slist *chunk = NULL;
		chunk = curl_slist_append(chunk, "Content-Type: application/json");

		CString sComment;
		sComment.Format("{\n"
			"  \"fields\": {\n"
			"    \"project\": {\n"
			"      \"key\": \"%s\"\n"
			"    },\n"
			"    \"summary\": \"%s\",\n"
			"    \"description\": \"%s\",\n"
			"    \"issuetype\": {\n"
			"      \"name\": \"%s\"\n"
			"    }\n"
			"  }\n"
			"}\n"
			,zProject, zSummary, zDescription, zIssueType);

		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_URL, sAttachmentURL.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_POST, 1);	
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_USERPWD, sAuth.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_POSTFIELDS, sComment.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_POSTFIELDSIZE, sComment.GetLength());
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_HTTPHEADER, chunk);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_WRITEDATA, &oBuff);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_WRITEFUNCTION, LTCurlRetunBuffer::_WritFunc);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_HEADERDATA,  &oHeaderBuff);
		curl_easy_setopt_VA_fix_call(pCurlComment, CURLOPT_HEADERFUNCTION, LTCurlRetunBuffer::_WritFunc);

		CURLcode res = curl_easy_perform(pCurlComment);
		/* Check for errors */ 
		if (res == CURLE_OK)
		{

			LTHTTPResponse* pResponse = LTHTTPResponse::Create(oHeaderBuff.GetFullOutput());
			if (pResponse)
			{
				LTHTTPResponse::ReturnCodeType eCode = pResponse->GetReturnCodeType();
				if (eCode != LTHTTPResponse::RCT_OK)
				{
					bSuccess = false;
					if (eCode == LTHTTPResponse::RCT_ERROR)
					{
						CString sLoginReason = pResponse->GetAttribute("X-Seraph-LoginReason");
						CString sLoginDeniedReason  = pResponse->GetAttribute("X-Authentication-Denied-Reason");
						sErr.Format("%s:%d: %s:%s\n%s", pResponse->s_Msg, pResponse->i_ReturnCode, 
							sLoginReason, sLoginDeniedReason, LOGIN_ERR_MSG);
					}
				}
				else
				{
					oBuff.GetTicketID(sTicket);
					if (sTicket.IsEmpty())
					{
						bSuccess = false;
						sErr.Format("Ticket creation problem. Ticket-ID is empty");
					}
					else
					{
						bSuccess = true;
					}
				}
			}
		}
		else
		{
			bSuccess = false;
			sErr.Format("[%s:%d] curl_easy_perform() failed: %s",
				SI , curl_easy_strerror(res));
		}

		curl_easy_cleanup(pCurlComment);
		curl_slist_free_all(chunk);
	}

	return bSuccess;
}


//**************************************************************************************************
bool LTUploadTask::AttachFileToJira( const char* zFile, const char* zFileName,
							 LTJiraCredentials* pCred, CString& sErr)
{
	CFile oFile;
	BOOL bRes = oFile.Open(zFile, CFile::modeRead);
	bool bSuccess = false;
	if (bRes)
	{
		UINT uiFileSize = (UINT) oFile.GetLength();
		char* pBuff = new char[uiFileSize];

		oFile.Read(pBuff, uiFileSize);
		bSuccess = AttachFileToJira(pBuff, uiFileSize, zFileName, pCred, sErr);
		delete [] pBuff;
	}
	else
	{
		sErr.Format("File open failed [%s]", zFile);
	}

	return bSuccess;
}

//**************************************************************************************************
bool LTUploadTask::AttachFileToJira(char* zBuffer, int iBufferSize, const char* zFileName, 
							 LTJiraCredentials* pCred, CString& sErr)
{
	if (!pCred)
	{
		sErr = "Jira credentials is empty";
		return false;
	}

	CString sAttachmentURL;
	sAttachmentURL.Format("%s/rest/api/2/issue/%s-%s/attachments", pCred->s_URL, pCred->s_Project, pCred->s_ID);

	CString sAuth;
	pCred->GetAuthCode(sAuth);

	bool bSuccess = false;

	CURL* pCurlAttachment = NULL;
	pCurlAttachment = curl_easy_init();
	LTCurlRetunBuffer oBuff;
	LTCurlRetunBuffer oHeaderBuff;
	if (pCurlAttachment) 
	{ 
		struct curl_slist *chunk = NULL;
		struct curl_httppost *formpost = NULL;
		struct curl_httppost *lastptr = NULL;

		chunk = curl_slist_append(chunk, "X-Atlassian-Token: nocheck");

		curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "file",
			CURLFORM_BUFFER, zFileName,
			CURLFORM_BUFFERPTR, zBuffer,
			CURLFORM_BUFFERLENGTH, iBufferSize,
			CURLFORM_END);

		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_URL, sAttachmentURL.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_POST, 1);	
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_USERPWD, sAuth.GetBuffer());
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_HTTPHEADER, chunk);
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_WRITEDATA, &oBuff);
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_WRITEFUNCTION, LTCurlRetunBuffer::_WritFunc);
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_HEADERDATA, &oHeaderBuff);
		curl_easy_setopt_VA_fix_call(pCurlAttachment, CURLOPT_HEADERFUNCTION, LTCurlRetunBuffer::_WritFunc);

		CURLcode res = curl_easy_perform(pCurlAttachment);
		/* Check for errors */ 
		if (res == CURLE_OK)
		{
			bSuccess = true;
			TRACE("%s\n", oBuff.GetFullOutput());
			TRACE("\n\n%s\n", oHeaderBuff.GetFullOutput());

			LTHTTPResponse* pResponse = LTHTTPResponse::Create(oHeaderBuff.GetFullOutput());
			if (pResponse)
			{
				LTHTTPResponse::ReturnCodeType eCode = pResponse->GetReturnCodeType();
				if (eCode != LTHTTPResponse::RCT_OK)
				{
					bSuccess = false;
					if (eCode == LTHTTPResponse::RCT_ERROR)
					{
						CString sLoginReason = pResponse->GetAttribute("X-Seraph-LoginReason");
						CString sLoginDeniedReason  = pResponse->GetAttribute("X-Authentication-Denied-Reason");
						sErr.Format("%s:%d: %s:%s\n%s", pResponse->s_Msg, pResponse->i_ReturnCode, 
							sLoginReason, sLoginDeniedReason, LOGIN_ERR_MSG);
					}
				}
			}
		}
		else
		{
			sErr.Format("[%s:%d] curl_easy_perform() failed: %s",
				SI , curl_easy_strerror(res));
		}

		curl_easy_cleanup(pCurlAttachment);
		curl_slist_free_all(chunk);
	}

	return bSuccess;
}
