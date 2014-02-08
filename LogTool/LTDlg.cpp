// LTDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTApp.h"
#include "LTDlg.h"
#include "LTUtils.h"
#include "LTEnv.h"
#include "LTAddEnvDlg.h"
#include "LTAddLogEnvDlg.h"
#include "LTConfig.h"
#include "LTSshSession.h"
#include "LTScreenshotEditDlg.h"
#include "LTJiraCredentials.h"
#include "LTNewJIRADlg.h"

#include <WinCred.h>

#include <libssh2.h>
#include <curl/curl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Due to a recursive macro call in curl_easy_setopt(please refer libcurl), having it directly called
// disturbs VisualAssistXs symbol identification.
#define  curl_easy_setopt_VA_fix_call    curl_easy_setopt


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()




LTDlg::LTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	p_ConnectedSession = NULL;
	b_ChangeSkip = false;
}

void LTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ENV, o_ListEnv);
	DDX_Control(pDX, IDC_LIST_SELECTION, o_ListSelection);
	DDX_Control(pDX, IDC_EDIT_JIRA_URL, o_EditJiraURL);
	DDX_Control(pDX, IDC_COMBO_LOG_MACHINES, o_ComboLogMachines);
	DDX_Control(pDX, IDC_COMBO_JIRA_PROJECT, o_ComboJiraProject);
	DDX_Control(pDX, IDC_STATIC_LOGMAC_IP_VALUE, o_StaticLogEnv);
	DDX_Control(pDX, IDC_CHECK_JIRA_CREATE_TICKET, o_CheckJiraCreateNew);
	DDX_Control(pDX, IDC_CHECK_COMMENT_ON_JIRA, o_CheckJiraComment);
	DDX_Control(pDX, IDC_EDIT_TICKET_ID, o_EditJiraTicket);
	DDX_Control(pDX, IDC_COMBO_INCLUDE_FILTER, o_ComboIncludeFilters);
	DDX_Control(pDX, IDC_COMBO_EXCLUDE_FILTER, o_ComboExcludeFilters);
	DDX_Control(pDX, IDC_STATIC_FRAME_FILE_LIST, o_StaticFrmFiles);
	DDX_Control(pDX, IDC_STATIC_FRM_JIRA, o_StaticFrmJira);
	DDX_Control(pDX, IDC_STATIC_FRAME_ENVIRONMENT, o_StaticFrmEnvionment);
	DDX_Control(pDX, IDC_STATIC_FRAME_LOG_MACHINE, o_StaticFrmLogMac);
	DDX_Control(pDX, IDC_BUTTON_LOGMAC_NEW, o_ButtonLogMacNew);
	DDX_Control(pDX, IDC_BUTTON_LOGMAC_EDIT, o_ButtonLogMacEdit);
	DDX_Control(pDX, IDC_BUTTON_LOGMAC_DELETE, o_ButtonLogMacDelete);
	DDX_Control(pDX, IDC_BUTTON_ENV_ADD, o_ButtonEnvNew);
	DDX_Control(pDX, IDC_BUTTON_ENV_EDIT, o_ButtonEnvEdit);
	DDX_Control(pDX, IDC_BUTTON_ENV_DELETE, o_ButtonEnvDelete);
	DDX_Control(pDX, IDCANCEL, o_ButtonCancel);
	DDX_Control(pDX, IDC_STATIC_JIRA_URL, o_StaticJiraURL);
	DDX_Control(pDX, IDC_STATIC_JIRA_PROJECT, o_StaticJiraTicket);
	DDX_Control(pDX, IDC_STATIC_JIRA_ID_DEVIDER, o_StaticJiraTicketSep);
	DDX_Control(pDX, IDC_STATIC_SCREENSHOT_AREA, o_StaticScreenshotBoundary);
	DDX_Control(pDX, IDC_STATIC_FRM_SCREENSHOTS, o_StaticFrmScreenshots);
	DDX_Control(pDX, IDC_BUTTON_ENV_REFRESH, o_ButtonEnvRefresh);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, o_ButtonUpload);
	DDX_Control(pDX, IDC_BUTTON_SCREENSHOT_NEW, o_ButtonScreenshotNew);
	DDX_Control(pDX, IDC_BUTTON_SCREENSHOT_CLEAR, o_ButtonScreenshotClear);
	DDX_Control(pDX, IDC_BUTTON_JIRA_CREDENTIALS, o_ButtonJiraCredentials);
	DDX_Control(pDX, IDC_BUTTON_JIRA_TICKE_INFO, o_ButtonJiraTicketInfo);
}

BEGIN_MESSAGE_MAP(LTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ENV, &LTDlg::OnLvnItemchangedListEnv)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &LTDlg::OnBnClickedButtonTest)	
	ON_BN_CLICKED(IDC_BUTTON_ENV_ADD, &LTDlg::OnBnClickedButtonEnvAdd)
	ON_CBN_KILLFOCUS(IDC_COMBO_JIRA_PROJECT, &LTDlg::OnCbnKillfocusComboJiraProject)
	ON_CBN_KILLFOCUS(IDC_COMBO_INCLUDE_FILTER, &LTDlg::OnCbnKillfocusComboIncludeFilter)
	ON_CBN_KILLFOCUS(IDC_COMBO_EXCLUDE_FILTER, &LTDlg::OnCbnKillfocusComboExcludeFilter)
	ON_EN_KILLFOCUS(IDC_EDIT_TICKET_ID, &LTDlg::OnEnKillfocusEditTicketId)
	ON_BN_CLICKED(IDC_CHECK_JIRA_CREATE_TICKET, &LTDlg::OnBnClickedCheckJiraCreateTicket)
	ON_BN_CLICKED(IDC_CHECK_COMMENT_ON_JIRA, &LTDlg::OnBnClickedCheckCommentOnJira)
	ON_CBN_KILLFOCUS(IDC_COMBO_SELECTION, &LTDlg::OnCbnKillfocusComboSelection)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SELECTION_ALL, &LTDlg::OnBnClickedButtonFileSelectionAll)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SELECTION_CLEAR, &LTDlg::OnBnClickedButtonFileSelectionClear)
	ON_BN_CLICKED(IDC_CHECK_FILTER_SELECT_ALL, &LTDlg::OnBnClickedCheckFilterSelectAll)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_LOGMAC_NEW, &LTDlg::OnBnClickedButtonLogmacNew)
	ON_BN_CLICKED(IDC_BUTTON_LOGMAC_EDIT, &LTDlg::OnBnClickedButtonLogmacEdit)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ENV_EDIT, &LTDlg::OnBnClickedButtonEnvEdit)
	ON_BN_CLICKED(IDC_BUTTON_ENV_DELETE, &LTDlg::OnBnClickedButtonEnvDelete)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_SCREENSHOT_NEW, &LTDlg::OnBnClickedButtonScreenshotNew)
	ON_BN_CLICKED(IDC_BUTTON_ENV_REFRESH, &LTDlg::OnBnClickedButtonEnvRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ENV, &LTDlg::OnNMRClickListEnv)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &LTDlg::OnBnClickedButtonUpload)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_TICKET_ID, &LTDlg::OnEnChangeEditTicketId)
	ON_CBN_SELCHANGE(IDC_COMBO_JIRA_PROJECT, &LTDlg::OnCbnSelchangeComboJiraProject)
	ON_BN_CLICKED(IDC_BUTTON_JIRA_CREDENTIALS, &LTDlg::OnBnClickedButtonJiraCredentials)
	ON_BN_CLICKED(IDC_BUTTON_JIRA_TICKE_INFO, &LTDlg::OnBnClickedButtonJiraTickeInfo)
END_MESSAGE_MAP()


// LTDlg message handlers

BOOL LTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	o_ComboJiraProject.AddString("SLSESURV");
	o_ComboJiraProject.AddString("SURV");
	o_ComboJiraProject.AddString("JSESURV");
	o_ComboJiraProject.AddString("LMEXSURV");
	o_ComboJiraProject.AddString("BMESSURV");
	int iIndex = o_ComboJiraProject.FindString(-1, LTConfig::o_Inst.GetJiraProjectSet()->Get());
	if (iIndex < 0)
		iIndex = 0;
	o_ComboJiraProject.SetCurSel(iIndex);


	// TODO: Add extra initialization here
	o_ListEnv.SetExtendedStyle(o_ListEnv.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	o_ListEnv.InsertColumn(0,CString("Env"), LVCFMT_LEFT,75);
	o_ListEnv.InsertColumn(1,CString("User"), LVCFMT_LEFT,75);
	o_ListEnv.InsertColumn(2,CString("IP"),LVCFMT_LEFT, 90);
	o_ListEnv.InsertColumn(3,CString("Folder"),LVCFMT_LEFT, 50);
	LoadEnvFromXShellConfig();
	
	CRect rScreenShot;
	o_StaticScreenshotBoundary.GetWindowRect(rScreenShot);
	ScreenToClient(rScreenShot);
	o_ThumbnailsCtrl.MoveWindow(rScreenShot);

	o_ListSelection.SetExtendedStyle(o_ListSelection.GetExtendedStyle() | LVS_EX_CHECKBOXES);
	o_ListSelection.InsertColumn(0, CString("Selection"), LVCFMT_LEFT,75);

	o_EditJiraURL.SetWindowText(LTConfig::o_Inst.s_JiraURL);
	o_EditJiraTicket.SetWindowText(LTConfig::o_Inst.s_JiraTicket);

	o_CheckJiraCreateNew.SetCheck(LTConfig::o_Inst.b_JiraCreateNew);
	o_CheckJiraComment.SetCheck(LTConfig::o_Inst.b_JiraDoComment);

	o_EditJiraTicket.EnableWindow(!LTConfig::o_Inst.b_JiraCreateNew);

	PopulateComboFromCfg(&o_ComboLogMachines, LTConfig::o_Inst.GetLogMacSet());
	PopulateComboFromCfg(&o_ComboIncludeFilters, LTConfig::o_Inst.GetIncludeFilterSet());
	PopulateComboFromCfg(&o_ComboExcludeFilters, LTConfig::o_Inst.GetExcludeFilterSet());
	CString sDefaultLogMac = LTConfig::o_Inst.GetLogMacSet()->Get();
	o_StaticLogEnv.SetWindowText(sDefaultLogMac);


	//o_ButtonEnvNew.SetIcon(::LoadIcon(NULL, IDI_ERROR));
	o_ButtonEnvDelete.EnableWindow(FALSE);
	o_ButtonEnvEdit.EnableWindow(FALSE);
	o_ButtonEnvRefresh.EnableWindow(FALSE);
	//o_ListSelection.SetItemText(0, 0, "Selection");

	o_Resizer.Attach(&o_ListSelection, LT_RM_BOTTMRIGHT);
	o_Resizer.Attach(&o_StaticFrmFiles, LT_RM_BOTTMRIGHT);
	o_Resizer.Attach(&o_ButtonCancel, LT_RM_ALL);
	o_Resizer.Attach(&o_ButtonUpload, LT_RM_ALL);

	o_Resizer.Attach(&o_StaticFrmJira, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_CheckJiraComment, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_CheckJiraCreateNew, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_EditJiraURL, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_EditJiraTicket, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ComboJiraProject, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraTicket, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraTicketSep, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraURL, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonJiraCredentials, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonJiraTicketInfo, LT_RM_VIRTICAL);

	o_Resizer.Attach(&o_ComboLogMachines, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonLogMacDelete, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonLogMacNew, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonLogMacEdit, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticFrmLogMac, LT_RM_VIRTICAL);

	o_Resizer.Attach(&o_ListEnv, LT_RM_BOTTOM);
	o_Resizer.Attach(&o_ButtonEnvDelete, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonEnvEdit, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonEnvNew, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonEnvRefresh, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticFrmEnvionment, LT_RM_BOTTOM);

	o_Resizer.Attach(&o_StaticFrmScreenshots, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ThumbnailsCtrl, LT_RM_VIRTICAL);

	o_Resizer.Attach(&o_ButtonScreenshotNew, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ButtonScreenshotClear, LT_RM_VIRTICAL);

	o_Resizer.Originate(this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void LTDlg::PopulateComboFromCfg( CComboBox* pCombo, LTConfig::StringSet* pStrSet )
{
	for (int i = 0; i < pStrSet->GetCount(); i++)
	{
		CString sVal = pStrSet->GetAt(i);
		pCombo->AddString(sVal);
		if (i == 0)
			pCombo->SetCurSel(0);
	}
}


void LTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void LTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR LTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool LTDlg::LoadEnvFromXShellConfig(void)
{
	CString sXShellSessionFolder("");
	if (!LTUtils::GetXShellSeesionFolder(sXShellSessionFolder))
		return false;

	VEC_ENV aEnvs;
	GetAllFiles(sXShellSessionFolder, "", aEnvs);

	for (UINT ui = 0 ; ui < aEnvs.size(); ui++)
	{
		LTEnv* pEnv = aEnvs[ui];
		LTEnv::vec_Env.push_back(pEnv);
		InsertEnvToList(pEnv);
	}

	return false;
}

void LTDlg::GetAllFiles( CString sBaseFolder, CString sSubDir, VEC_ENV& rvecEvs)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString sFindFileExp;
	if (sSubDir.GetLength() > 0)
		sFindFileExp.Format("%s\\%s\\*",  sBaseFolder, sSubDir);
	else
		sFindFileExp.Format("%s\\*", sBaseFolder);

	hFind = FindFirstFile(sFindFileExp, &FindFileData);

	while (hFind != (HANDLE)ERROR_FILE_NOT_FOUND &&
		hFind != (HANDLE)INVALID_HANDLE_VALUE )
	{
		CString sFoundFile = FindFileData.cFileName;
		bool bIsDirectory = (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (bIsDirectory && 
			(strcmp(FindFileData.cFileName, ".") != 0) && 
			(strcmp(FindFileData.cFileName, "..") != 0))
		{
			CString sNewSubDir = "";
			if (sSubDir.GetLength() > 0)
				sNewSubDir.Format("%s\\%s", sSubDir, FindFileData.cFileName);
			else
				sNewSubDir = FindFileData.cFileName;

			GetAllFiles(sBaseFolder, sNewSubDir, rvecEvs);
		}
		else if (sFoundFile.Find(".xsh") > -1)
		{
			LTEnv* pEnv = LTEnv::CreateEnv(sBaseFolder, sSubDir, FindFileData.cFileName);
			if (pEnv)
			{
				rvecEvs.push_back(pEnv);
			}
			//Process file
		}

		//TRACE("The first file found is %s\\%s\\%s\n", sBaseFolder, sSubDir, FindFileData.cFileName);
		
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}

	FindClose(hFind);
}

void LTDlg::OnLvnItemchangedListEnv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ((pNMLV->uChanged & LVIF_STATE) 
		&& (pNMLV->uNewState & LVNI_SELECTED))
	{
		LTEnv* pEnv = (LTEnv*)o_ListEnv.GetItemData(pNMLV->iItem);

		o_ButtonEnvDelete.EnableWindow(TRUE);
		o_ButtonEnvEdit.EnableWindow(TRUE);
		o_ButtonEnvRefresh.EnableWindow(TRUE);

		o_ListSelection.DeleteAllItems();
	}
}


void LTDlg::OnBnClickedButtonTest()
{
	CString s;
	LTCurlRetunBuffer oBuf;
	oBuf.GetTicketID(s);
	return;
	TestCurl();
}

int LTDlg::TestCurl()
{
	LTJiraCredentials oCred;
	CString sErr;
	if (!ProvideJiraCred(&oCred, sErr))
		return 1;

	LTEnv* pDevEnv = GetSelectedEnv();
	if (!pDevEnv)
		return 1;

	CString sEnvName;
	CString sEnvPath;
	CString sConnStr;
	o_ComboLogMachines.GetLBText(o_ComboLogMachines.GetCurSel(), sConnStr);

	if (!LTUtils::DecodePathStringEx(sConnStr, sEnvName, sEnvPath))
		return 1;

	LTEnv* pLogEnv = LTEnv::FindEnvFQ(sEnvName);
	if (!pLogEnv)
		return 1;

	CString sTicketPath;
	CString sTicketID;
	oCred.GetFullTicketID(sTicketID);
	if (sEnvPath.IsEmpty())
		sTicketPath = sTicketID;
	else
		sTicketPath.Format("%s/%s", sEnvPath, oCred.GetFullTicketID());

	CString sAttachmentURL;
	sAttachmentURL.Format("%s/rest/api/2/issue/%s/comment",  oCred.s_URL, sTicketID);

	CString sAuth;
	sAuth.Format("%s:%s",  oCred.s_User,  oCred.s_Password);

	CString sXShellFile = sTicketID + "_logs.xsh";
	CString sVersion = LTUtils::GetProductVersionX();

	CURL* pCurlComment = NULL;
	pCurlComment = curl_easy_init();
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
			pLogEnv->s_EnvUser, pLogEnv->s_IP, sTicketPath,
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

		CURLcode res = curl_easy_perform(pCurlComment);
		/* Check for errors */ 
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));


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
		"Method=2\n"
		"UseExpectSend=1\n"
		"ExpectSend_Count=1\n"
		"ExpectSend_Send_0=cd %s\n"
		"ExpectSend_Expect_0=>\n"
		"ExpectSend_Hide_0=0\n"
		"[TERMINAL:WINDOW]\n"
		"FontSize=9\n"
		"FontFace=Consolas\n";

	//UserKey=id_rsa_1024
	//Method=1

	sXShell.Format(zXShellFmt, pLogEnv->s_IP, pLogEnv->s_EnvUser, sTicketPath);

	AttachFileToJira(sXShell.GetBuffer(), sXShell.GetLength(), sXShellFile);

	for (unsigned int ui = 0; ui < o_ThumbnailsCtrl.a_ScreenShots.size(); ui++)
	{
		LTScreenshot* pScreenshot = o_ThumbnailsCtrl.a_ScreenShots[ui]->p_Screenshot;
		CString sTmpFileName;
		sTmpFileName.Format("ss_%lu.jpg", time(NULL));

		CString sFileName;
		sFileName.Format("%s.jpg", pScreenshot->GetName());

		pScreenshot->Save(sTmpFileName);
		AttachFileToJira(sTmpFileName, sFileName);
	}

	

	return 0;
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonEnvAdd()
{
	// TODO: Add your control notification handler code here
	LTAddEnvDlg oDlg;
	oDlg.SetDlg(this);
	oDlg.DoModal();
}
//**************************************************************************************************
void LTDlg::AddLogEnv(const char* zUser, const char* zBaseLocation )
{
	CString sNew;
	sNew.Format("%s:%s", zUser, zBaseLocation);

	int iSel = o_ComboLogMachines.AddString(sNew);
	o_ComboLogMachines.SetCurSel(iSel);

	LTConfig::o_Inst.GetLogMacSet()->Set(sNew);
}

//**************************************************************************************************
void LTDlg::EditLogEnv(const char* zCurrentStr, const char* zUser, const char* zBaseLocation /*= ""*/ )
{
	CString sNew;
	sNew.Format("%s:%s", zUser, zBaseLocation);

	int iCurSel = o_ComboLogMachines.FindString(-1, zCurrentStr);

	if (iCurSel > -1)
		o_ComboLogMachines.DeleteString(iCurSel);

	int iSel = o_ComboLogMachines.AddString(sNew);
	o_ComboLogMachines.SetCurSel(iSel);

	LTConfig::o_Inst.GetLogMacSet()->Set(sNew);
}

//**************************************************************************************************
void LTDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}

//**************************************************************************************************
void LTDlg::OnChangeCombo( LTComboBox* pComboBox, const char* zValue )
{
	CString s = zValue;
}

//**************************************************************************************************
void LTDlg::OnCbnKillfocusComboJiraProject()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnCbnKillfocusComboIncludeFilter()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnCbnKillfocusComboExcludeFilter()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnEnKillfocusEditTicketId()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnBnClickedCheckJiraCreateTicket()
{
	// TODO: Add your control notification handler code here
	int iCheck = o_CheckJiraCreateNew.GetCheck();
	LTConfig::o_Inst.b_JiraCreateNew = (iCheck != 0);

	o_EditJiraTicket.EnableWindow(!LTConfig::o_Inst.b_JiraCreateNew);
}

//**************************************************************************************************
void LTDlg::OnBnClickedCheckCommentOnJira()
{
	// TODO: Add your control notification handler code here
	int iCheck = o_CheckJiraComment.GetCheck();
	LTConfig::o_Inst.b_JiraDoComment = (iCheck != 0);
}

//**************************************************************************************************
void LTDlg::OnCbnKillfocusComboSelection()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonFileSelectionAll()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonFileSelectionClear()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnBnClickedCheckFilterSelectAll()
{
	// TODO: Add your control notification handler code here
}

//**************************************************************************************************
void LTDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	__super::OnKeyUp(nChar, nRepCnt, nFlags);
}

//**************************************************************************************************
void LTDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

//**************************************************************************************************
BOOL LTDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && 
			pMsg->wParam == VK_RETURN ) 
	{   // verify ENTER key is pressed

		UINT id = ::GetDlgCtrlID(pMsg->hwnd);
		UINT iid = o_ComboJiraProject.GetDlgCtrlID();
		OnPressEnterKey();
	}
	return __super::PreTranslateMessage(pMsg);
}

//**************************************************************************************************
void LTDlg::OnPressEnterKey()
{
	
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonLogmacNew()
{
	// TODO: Add your control notification handler code here
	LTAddLogEnvDlg oDlg;
	oDlg.SetDlg(this);
	oDlg.DoModal();
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonLogmacEdit()
{
	// TODO: Add your control notification handler code here
	CString sLogEnv = "";

	int iSel = o_ComboLogMachines.GetCurSel();
	o_ComboLogMachines.GetLBText(iSel, sLogEnv);

	LTAddLogEnvDlg oDlg;
	oDlg.SetEditMode(sLogEnv);
	oDlg.SetDlg(this);
	oDlg.DoModal();
}


//**************************************************************************************************
void LTDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	LTConfig::o_Inst.Save();
	__super::OnClose();
}

//**************************************************************************************************
void LTDlg::AddEnv(const char* zName, const char* zUser, const char* zIP, const char* zPassword )
{
	LTEnv* pEnvCur = LTEnv::FindEnv(zUser);
	if (pEnvCur)
	{
		CString sMsg;
		sMsg.Format("Environment for \"%s\" is already found", zUser);
		AfxMessageBox(sMsg);
		return;
	}

	CString sXShellSessionFolder("");
	if (!LTUtils::GetXShellSeesionFolder(sXShellSessionFolder))
		return;

	LTEnv* pNew = new LTEnv;
	pNew->s_Name = zName;
	pNew->s_EnvUser = zUser;
	pNew->s_IP = zIP;
	pNew->s_Password = zPassword;
	pNew->s_Folder = "";
	pNew->s_FullFile = sXShellSessionFolder + "\\" + pNew->s_EnvUser  + ".xsh";

	pNew->Save();
	LTEnv::vec_Env.push_back(pNew);

	InsertEnvToList(pNew);
}

//**************************************************************************************************
void LTDlg::EditEnv( LTEnv* pEnvEdited )
{
	int i = -1;

	do
	{
		i = o_ListEnv.GetNextItem(i, LVNI_ALL);
		if  (i == -1)
			break;

		LTEnv* pEnv = (LTEnv*)o_ListEnv.GetItemData(i);

		if (pEnvEdited == pEnv)
		{
			o_ListEnv.SetItemText(i, 0, pEnv->s_Name);
			o_ListEnv.SetItemText(i, 1, pEnv->s_EnvUser);
			o_ListEnv.SetItemText(i, 2, pEnv->s_IP);
			o_ListEnv.SetItemText(i, 3, pEnv->s_Folder);
		}
	}
	while(true);
}

//**************************************************************************************************
void LTDlg::InsertEnvToList( LTEnv* pEnv )
{
	int iCurCount = o_ListEnv.GetItemCount();
	o_ListEnv.InsertItem(LVIF_TEXT | LVIF_STATE, iCurCount, pEnv->s_Name, 0, LVIS_SELECTED, 0, 0);

	o_ListEnv.SetItemText(iCurCount, 0, pEnv->s_Name);
	o_ListEnv.SetItemText(iCurCount, 1, pEnv->s_EnvUser);
	o_ListEnv.SetItemText(iCurCount, 2, pEnv->s_IP);
	o_ListEnv.SetItemText(iCurCount, 3, pEnv->s_Folder);
	o_ListEnv.SetItemData(iCurCount, (DWORD_PTR)pEnv);
}

//**************************************************************************************************
void LTDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	o_Resizer.Resize(cx, cy);
	// TODO: Add your message handler code here
}

//void LTDlg::OnHdnItemchangedListEnv(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonEnvEdit()
{
	// TODO: Add your control notification handler code here
	LTEnv* pEnv = GetSelectedEnv();
	if (pEnv)
	{
		LTAddEnvDlg oDlg;
		oDlg.SetDlg(this);
		oDlg.SetEditMode(pEnv);
		oDlg.DoModal();
	}
	//returns -1 if not selected;
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonEnvDelete()
{
	// TODO: Add your control notification handler code here
	POSITION pos = o_ListEnv.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = o_ListEnv.GetNextSelectedItem(pos);
		if (nItem > -1)
		{
			LTEnv* pEnv = (LTEnv*)o_ListEnv.GetItemData(nItem);
			if (pEnv)
			{
				LTEnv::Remove(pEnv);
				o_ListEnv.DeleteItem(nItem);
				
				o_ButtonEnvDelete.EnableWindow(FALSE);
				o_ButtonEnvEdit.EnableWindow(FALSE);
				o_ButtonEnvRefresh.EnableWindow(FALSE);

				o_ListSelection.DeleteAllItems();
			}
		}
	}
}

//**************************************************************************************************
int LTDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	o_ThumbnailsCtrl.CreateScreenshotCtrl(this, CRect(0,0,0,0), 300300, this);

	// TODO:  Add your specialized creation code here

	return 0;
}

//void LTDlg::OnHdnItemdblclickListEnv(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//
//	int i = 0;
//	i++;
//}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonScreenshotNew()
{
	// TODO: Add your control notification handler code here
	LTScreenshotEditDlg oDlg;
	BOOL bRes = oDlg.DoModal();
	if (bRes == IDOK)
	{
		LTScreenshot* pScreenshot = oDlg.DetachScreenshot();
		if (pScreenshot)
			o_ThumbnailsCtrl.AddScreenshot(pScreenshot);
	}
}

//**************************************************************************************************
void LTDlg::OnEditScreenshot(LTThumbnailsCtrl::Screenshot* pScreenShot)
{
	LTScreenshotEditDlg oDlg;
	BOOL bRes = oDlg.DoModalEx(pScreenShot->p_Screenshot);
	if (bRes == IDOK)
	{
		LTScreenshot* pLtScreenshot = oDlg.DetachScreenshot();
		ASSERT(pScreenShot->p_Screenshot == pLtScreenshot);
		o_ThumbnailsCtrl.Invalidate();
	//	if (pScreenshot)
	//		o_ThumbnailsCtrl.AddScreenshot(pScreenshot);
	}
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonEnvRefresh()
{
	// TODO: Add your control notification handler code here

	POSITION pos = o_ListEnv.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = o_ListEnv.GetNextSelectedItem(pos);
		if (nItem > -1)
		{
			LTEnv* pEnv = (LTEnv*)o_ListEnv.GetItemData(nItem);
			if (pEnv)
			{
				if (p_ConnectedSession && !p_ConnectedSession->GetEnv()->IsSame(pEnv))
				{
					delete p_ConnectedSession;
					p_ConnectedSession = NULL;
				}

				LTSshSession* pSession = p_ConnectedSession;
				
				if (!pSession)
				{
					pSession = new LTSshSession;
					CString sErr = "";
					if (pSession->Connect(pEnv, sErr))
					{
						p_ConnectedSession = pSession;
					}
					else
					{
						AfxMessageBox(sErr);
						delete pSession;
						pSession = NULL;
					}
				}

				if (pSession)
				{
					std::list<CString> lst;

					if (pSession->Execute("ls -1 logs/*:*:* corefiles/*", &lst))
					{
						TRACE("===========================\n");
						std::list<CString>::iterator itr = lst.begin();
						for (; itr != lst.end(); itr++)
						{
							o_ListSelection.InsertItem(LVIF_TEXT | LVIF_STATE, 0, 
								*itr, 0, LVIS_SELECTED, 0, 0);
						}
					}
				}
			}
		}
	}
}

//**************************************************************************************************
void LTDlg::OnNMRClickListEnv(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	// display a menu created using CreateMenu()
	HMENU hMenu = ::CreatePopupMenu();
	if (NULL != hMenu)
	{
		// add a few test items
		::AppendMenu(hMenu, MF_STRING, 1, "Item 1");
		::AppendMenu(hMenu, MF_STRING, 2, "Item 2");
		::AppendMenu(hMenu, MF_STRING, 3, "Item 3");

		CPoint point(50,10);
		o_ListEnv.ClientToScreen(&point);

		int sel = ::TrackPopupMenuEx(hMenu, 
			TPM_CENTERALIGN | TPM_RETURNCMD,
			point.x,
			point.y,
			m_hWnd,
			NULL);
		::DestroyMenu(hMenu);
	}
}

//**************************************************************************************************
void LTDlg::OnBnClickedButtonUpload()
{

	CString sCreated;
	CreateJiraTicket("SURVRD","CC-System", sCreated, "Test Summary", "Test Description");
	return;
	// TODO: Add your control notification handler code here

	/*
	mykey=`cat ~/.ssh/id_rsa.pub`
	ssh $env_conn 'cur_key=`grep "'$mykey'" .ssh/authorized_keys ` ; if [ -z "$cur_key" ]; then echo adding key to authorized list; mkdir -p .ssh; echo "'$mykey'" >> .ssh/authorized_keys; else echo key already found authorized; fi'
	*/

	// Taking the public key from environment
	//////////////////////////////////////////////////////////////////////////
	LTEnv* pEnv = GetSelectedEnv();
	if (!pEnv)
	{
		AfxMessageBox("No environment selected");
		return;
	}

	LTSshSession* pSessionEnv = p_ConnectedSession;
	CString sErr = "";

	//if (!pSessionEnv->Connect(pEnv, sErr))
	//{
	//	delete pSessionEnv;
	//	return;
	//}

	if (!pSessionEnv)
		return;

	std::list<CString> lst;
	if (!pSessionEnv->Execute("cat .ssh/id_rsa.pub", &lst))
	{
		delete p_ConnectedSession;
		p_ConnectedSession = NULL;
		return;
	}

	if (lst.size() == 0)
		return;

	CString sKey = lst.front();


	// Putting the public key of environment in authorized list of log-machine
	//////////////////////////////////////////////////////////////////////////
	CString sEnvName;
	CString sEnvPath;
	CString sConnStr;
	o_ComboLogMachines.GetLBText(o_ComboLogMachines.GetCurSel(), sConnStr);
	
	if (!LTUtils::DecodePathStringEx(sConnStr, sEnvName, sEnvPath))
		return;

	LTEnv* pLogEnv = LTEnv::FindEnvFQ(sEnvName);
	if (!pLogEnv)
		return;

	LTSshSession* pSession = new LTSshSession;

	if (!pSession->Connect(pLogEnv, sErr))
	{
		delete pSession;
		return;
	}

	CString sCmd;
	sCmd.Format("cur_key=`grep \"%s\" .ssh/authorized_keys ` ; if [ -z \"$cur_key\"  ]; then echo adding key to authorized list; mkdir -p .ssh; echo \"%s\" >> .ssh/authorized_keys; else echo key already found authorized; fi"
		, sKey, sKey);

	std::list<CString> lstOut;
	pSession->Execute(sCmd, &lstOut);

	



	// Collecting logs
	//////////////////////////////////////////////////////////////////////////
	CString sTempDir;
	sTempDir.Format("/tmp/%I64u", time(NULL));

	CString sMkTmpDir;
	sMkTmpDir.Format("mkdir -p %s", sTempDir);

	lstOut.clear();
	pSessionEnv->Execute(sMkTmpDir, &lstOut);

	CString sFileList = "";

	// Getting selections;
	//////////////////////////////////////////////////////////////////////////
	int i = -1;
	do
	{
		i = o_ListSelection.GetNextItem(i, LVNI_ALL);
		if  (i == -1)
			break;

		if (o_ListSelection.GetCheck(i) )
		{
			CString sSel = o_ListSelection.GetItemText(i, 0);
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
	}
	while(true);


	// Make log location
	//////////////////////////////////////////////////////////////////////////
	CString sProject;
	o_ComboJiraProject.GetLBText(o_ComboJiraProject.GetCurSel(), sProject);
	CString sJiraNo;
	o_EditJiraTicket.GetWindowText(sJiraNo);
	CString sJiraID;
	sJiraID.Format("%s-%s", sProject, sJiraNo);


	CString sTicketPath;
	if (sEnvPath.GetLength() > 0)
		sTicketPath.Format("%s/%s", sEnvPath, sJiraID);
	else
		sTicketPath = sJiraID;

	CString sTicketDirMkCmd;
	sTicketDirMkCmd.Format("mkdir -p %s", sTicketPath);

	pSession->Execute(sTicketDirMkCmd);


	delete pSession;
	pSession = NULL;


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


	// Add Comment on jira
	//////////////////////////////////////////////////////////////////////////
}

//**************************************************************************************************
LTEnv* LTDlg::GetSelectedEnv()
{
	POSITION pos = o_ListEnv.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = o_ListEnv.GetNextSelectedItem(pos);
		if (nItem > -1)
		{
			LTEnv* pEnv = (LTEnv*)o_ListEnv.GetItemData(nItem);
			return pEnv;
		}
	}

	return NULL;
}

//**************************************************************************************************
void LTDlg::OnDestroy()
{
	__super::OnDestroy();

	delete p_ConnectedSession;
	p_ConnectedSession = NULL;
	// TODO: Add your message handler code here
}

//**************************************************************************************************
void LTDlg::AttachFileToJira( const char* zFile, const char* zFileName )
{
	CFile oFile;
	BOOL bRes = oFile.Open(zFile, CFile::modeRead);
	if (bRes)
	{
		UINT uiFileSize = (UINT) oFile.GetLength();
		char* pBuff = new char[uiFileSize];

		oFile.Read(pBuff, uiFileSize);
		AttachFileToJira(pBuff, uiFileSize, zFileName);

		delete [] pBuff;
	}
}

//**************************************************************************************************
void LTDlg::AttachFileToJira(char* zBuffer, int iBufferSize, const char* zFileName )
{
	LTJiraCredentials oJiCred;
	CString sErr;
	if (!ProvideJiraCred(&oJiCred, sErr))
		return;

	CString sAttachmentURL;
	sAttachmentURL.Format("%s/rest/api/2/issue/%s-%s/attachments", oJiCred.s_URL, oJiCred.s_Project, oJiCred.s_ID);

	CString sAuth;
	sAuth.Format("%s:%s", oJiCred.s_User, oJiCred.s_Password);

	CURL* pCurlAttachment = NULL;
	pCurlAttachment = curl_easy_init();
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

		CURLcode res = curl_easy_perform(pCurlAttachment);
		/* Check for errors */ 
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		curl_easy_cleanup(pCurlAttachment);
		curl_slist_free_all(chunk);
	}
}

//**************************************************************************************************
bool LTDlg::CreateJiraTicket( const char* zProject, const char* zIssueType, CString& sTicket, 
							 const char* zSummary, const char* zDescription)
{
	LTJiraCredentials oCred;
	CString sErr;
	if (!ProvideJiraCred(&oCred, sErr, false))
		return false;

	CString sAttachmentURL;
	sAttachmentURL.Format("%s/rest/api/2/issue/", oCred.s_URL);

	CString sAuth;
	oCred.GetAuthCode(sAuth);

	bool bSuccess = false;

	CURL* pCurlComment = NULL;
	pCurlComment = curl_easy_init();
	if (pCurlComment) 
	{ 
		LTCurlRetunBuffer oBuff;

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

		CURLcode res = curl_easy_perform(pCurlComment);
		/* Check for errors */ 
		if (res != CURLE_OK)
		{
			bSuccess = false;
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}

		oBuff.GetTicketID(sTicket);
		bSuccess &= !sTicket.IsEmpty();

		curl_easy_cleanup(pCurlComment);
		curl_slist_free_all(chunk);
	}

	return bSuccess;
}





//**************************************************************************************************
bool LTDlg::ProvideJiraCred(LTJiraCredentials* pJiCred, CString& sErr, bool bWithID)
{
	o_EditJiraURL.GetWindowText(pJiCred->s_URL);
	o_EditJiraTicket.GetWindowText(pJiCred->s_ID);

	if (!ProvideWinJiraCred(pJiCred->s_URL, pJiCred->s_User, pJiCred->s_Password))
	{
		AfxMessageBox("Failed to aquire JIRA credentials");
		return false;
	}


	int iSel = o_ComboJiraProject.GetCurSel();
	o_ComboJiraProject.GetLBText(iSel, pJiCred->s_Project);

	if (pJiCred->s_User.IsEmpty())
	{
		AfxMessageBox("JIRA user name is empty.");
		return false;
	}

	if (pJiCred->s_Password.IsEmpty())
	{
		AfxMessageBox("JIRA password is empty.");
		return false;
	}

	if (pJiCred->s_URL.IsEmpty())
	{
		AfxMessageBox("JIRA URL is empty.");
		return false;
	}

	if (pJiCred->s_Project.IsEmpty())
	{
		AfxMessageBox("JIRA project is empty.");
		return false;
	}

	if (bWithID)
	{
		if (pJiCred->s_ID.IsEmpty())
		{
			AfxMessageBox("JIRA project/ticket-id is not specified");
			return false;
		}
	}

	return true;
}

//**************************************************************************************************
bool SkipChar(CString& sVal)
{
	const char* zStr = sVal;
	const char* zItr = zStr;
	char zBuff[10];
	int iBufIdx = 0;
	bool bChanged = false;

	while (*zItr != 0)
	{
		char cVal = *zItr;
		if ((cVal >= '0') && (cVal <= '9'))
			zBuff[iBufIdx++] = cVal;
		else
			bChanged = true;

		zItr++;
	}

	if (bChanged)
	{
		zBuff[iBufIdx] = 0;
		sVal = zBuff;
	}
	return bChanged;
}

//**************************************************************************************************
void LTDlg::OnEnChangeEditTicketId()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString sTicketID;
	o_EditJiraTicket.GetWindowText(sTicketID);

	LTConfig::o_Inst.s_JiraTicket = sTicketID;

	if (b_ChangeSkip)
	{
		b_ChangeSkip = false;
		return;
	}

	int iChange = sTicketID.Find('-');
	if (iChange > -1)
	{
		CString sProj = sTicketID.Left(iChange);
		CString sID = sTicketID.Mid(iChange + 1);
		SkipChar(sID);

		int iCurSel = o_ComboJiraProject.FindString(-1, sProj);
		if (iCurSel > -1)
		{
			o_ComboJiraProject.SetCurSel(iCurSel);
			LTConfig::o_Inst.GetJiraProjectSet()->Set(sProj);
		}

		b_ChangeSkip = true;
		o_EditJiraTicket.SetWindowText(sID);
	}
	else
	{
		//if (SkipChar(sTicketID))
		//{
		//	b_ChangeSkip = true;
		//	o_EditJiraTicket.SetWindowText(sTicketID);
		//}
	}
}

//**************************************************************************************************
void LTDlg::OnCbnSelchangeComboJiraProject()
{
	// TODO: Add your control notification handler code here
	CString sProject;
	o_ComboJiraProject.GetLBText(o_ComboJiraProject.GetCurSel(), sProject);
	LTConfig::o_Inst.GetJiraProjectSet()->Set(sProject);
}

//**************************************************************************************************
void LTCurlRetunBuffer::Append( int iLen, char* pData)
{
	if (!HasMoreSpace(iLen))
		return;

	memcpy(p_Data + i_Len, pData, iLen);
	i_Len += iLen;	
	p_Data[(i_Len < i_Limit) ? i_Len : i_Limit - 1] = 0;
}

//**************************************************************************************************
size_t LTCurlRetunBuffer::_WritFunc( void* ptr, size_t size, size_t nmemb, LTCurlRetunBuffer* pBuff )
{
	int iNewLen = size * nmemb;
	pBuff->Append(iNewLen, (char*)ptr);
	return iNewLen;
}

//**************************************************************************************************
bool LTCurlRetunBuffer::HasMoreSpace( int iLen )
{
	return( i_Len + iLen) <= i_Limit;
}

#define KEY_GET_TAG  "\"key\":\""
//**************************************************************************************************
bool LTCurlRetunBuffer::GetTicketID( CString& sOut )
{
	/*
	{
	"id":"39000",
	"key":"TEST-101",
	"self":"http://localhost:8090/rest/api/2/issue/39000"
	}
	*/

	char* pzStart = strstr(p_Data, KEY_GET_TAG);
	if (!pzStart)
		return false;

	char* pzEnd =  strstr(pzStart + sizeof(KEY_GET_TAG), "\"");
	if (!pzEnd)
		return false;

	char zBuff[30];
	*zBuff = 0;

	strncpy_s(zBuff, 30, pzStart + sizeof(KEY_GET_TAG) - 1,  
		(pzEnd - pzStart - sizeof(KEY_GET_TAG) + 1));

	sOut = zBuff;
	return true;
}
//**************************************************************************************************
void LTDlg::OnBnClickedButtonJiraCredentials()
{
	// TODO: Add your control notification handler code here
	CString sUser;
	CString sPassword;
	ProvideWinJiraCred(LTConfig::o_Inst.s_JiraURL, sUser, sPassword, true);
}

//**************************************************************************************************
bool LTDlg::ProvideWinJiraCred( const char* zURL, CString& sUser, CString& sPassword, bool bUpdateAnyway)
{
	WCHAR wcURL[200];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, zURL, -1, wcURL, 200);

	DWORD	dwUser = MAX_PATH;
	DWORD	dwPassword = MAX_PATH;
	DWORD	dwDomain = MAX_PATH;
	WCHAR   zwUserName[MAX_PATH] = {0};
	WCHAR   zwPassword[MAX_PATH] = {0};
	WCHAR   zwDomain[MAX_PATH] = {0};

	// TODO: Add your control notification handler code here
	PCREDENTIALW cr = {0};
	bool bHas =  (0 != 	::CredReadW(wcURL, CRED_TYPE_GENERIC, 0, &cr));
	if (bHas && !bUpdateAnyway)
	{
		CredUnPackAuthenticationBufferW(0, cr->CredentialBlob, cr->CredentialBlobSize, 
			zwUserName, &dwUser, zwDomain, &dwDomain, zwPassword, &dwPassword);
	}
	else 
	{
		CREDUI_INFOW ci = {0}; 
		ULONG ulAuthPkg = 0; 
		void *pAuthBuff = NULL; 
		ULONG cbAuthBuff = 0; 
		void *pOutAuthBuff = NULL; 
		ULONG cbOutAuthBuff = 0; 
		BOOL fSave = FALSE;
		DWORD dwFlags = CREDUIWIN_GENERIC|CREDUIWIN_CHECKBOX|CREDUIWIN_ENUMERATE_CURRENT_USER; 
		ci.cbSize = sizeof(ci);	
		ci.hbmBanner = NULL; 
		ci.hwndParent = NULL; 
		ci.pszCaptionText = L"Writing Secure Code for Windows Vista"; 
		ci.pszMessageText = L"Please enter your JIRA username & password";

		if (bHas)
		{
			fSave = (cr->Persist != 0);
			pAuthBuff = cr->CredentialBlob;
			cbAuthBuff = cr->CredentialBlobSize;
		}

		DWORD dwErr = CredUIPromptForWindowsCredentialsW( 
			&ci, 0, &ulAuthPkg, pAuthBuff, cbAuthBuff, 
			&pOutAuthBuff, &cbOutAuthBuff, &fSave, dwFlags); 

		CredUnPackAuthenticationBufferW(0, pOutAuthBuff, cbOutAuthBuff, 
			zwUserName, &dwUser, zwDomain, &dwDomain, zwPassword, &dwPassword);

		if (fSave)
		{
			CREDENTIALW cred = {0};
			cred.Type = CRED_TYPE_GENERIC;
			cred.TargetName = wcURL;
			cred.CredentialBlobSize = cbOutAuthBuff;
			cred.CredentialBlob = (LPBYTE) pOutAuthBuff;
			cred.Persist = CRED_PERSIST_ENTERPRISE;
			cred.UserName = zwUserName;
			cred.TargetAlias = wcURL;

			BOOL ok = ::CredWriteW(&cred, 0);
		}
	}

	char zUserName[MAX_PATH];
	char zPassword[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, zwUserName, -1, zUserName, MAX_PATH, NULL, NULL);
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, zwPassword, -1, zPassword, MAX_PATH, NULL, NULL);
	sUser = zUserName;
	sPassword = zPassword;
	return true;
}

void LTDlg::OnBnClickedButtonJiraTickeInfo()
{
	// TODO: Add your control notification handler code here

	LTNewJIRADlg oDlg;
	oDlg.s_Description = s_JiraDescription;
	oDlg.s_Summary = s_JiraSummary;
	oDlg.s_Type = s_JiraIssueType;

	if (IDOK == oDlg.DoModal())
	{
		s_JiraDescription = oDlg.s_Description;
		s_JiraSummary = oDlg.s_Summary;
		s_JiraIssueType = oDlg.s_Type;
	}
}
