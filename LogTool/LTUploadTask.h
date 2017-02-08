#pragma once

#include <list>
#include "LTConfig.h"

typedef std::list<LTConfig::CustomAction*>    LST_CA;

class LTEnv;
class LTJiraCredentials;
class LTScreenshot;
class LTSshSession;

class LTUploadTask
{
public:
	LTUploadTask(void);
	~LTUploadTask(void);

	enum TaskType
	{
		TT_UPLOAD,
		TT_FILE_QUERY
	};

	static bool	DoTask(LTUploadTask* pTask);
	static bool DoUploadTask(LTUploadTask* pTask);
	static bool DoFileQuery(LTUploadTask* pTask);
	static bool PutJiraComment(const char* zTiceketID, LTEnv* pDevEnv, LTEnv* pLogEnv, 
					const char* zTicketPath,CString& sErr, 
					LTJiraCredentials* pCred, std::list<LTScreenshot*>& lstScreenshots);
	static bool CreateJiraTicket( const char* zProject, const char* zIssueType, 
					CString& sTicket, CString& sErr, LTJiraCredentials* pCred, 
					const char* zSummary, const char* zDescription);
	static bool AttachFileToJira( const char* zFile, const char* zFileName, 
					LTJiraCredentials* pCred, CString& sErr, const char* zTicketID);
	static bool AttachFileToJira(char* zBuffer, int iBufferSize, 
					const char* zFileName, LTJiraCredentials* pCred, CString& sErr, const char* zTicketID);

	bool	b_CustomCommands;
	bool	b_DoJiraComment;
	bool	b_CreateNew;
	bool	b_ScreenshotAttach;

	LTJiraCredentials*	p_JiraCred;
	LTEnv*	p_LogEnv;
	LTEnv*	p_Env;
	LTSshSession**	p_EnvSession;

	std::list<CString>	lst_UploadFiles;
	std::list<LTScreenshot*> lst_Screenshots;
	LST_CA	lst_CustomActions;

	std::list<CString>	lst_Output;

	CString s_JiraIssueType;
	CString s_JiraIssueSummary;
	CString s_JiraIssueDescription;
	CString s_JiraProject;

	CString s_JiratFullTicketID;
	CString s_EnvPath;

	CString s_Error;
	bool	b_Success;
	TaskType e_TaskType;
};
