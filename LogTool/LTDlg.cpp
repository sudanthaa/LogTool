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

#include <WinCred.h>

#include <libssh2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
}

void LTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ENV, o_ListEnv);
	DDX_Control(pDX, IDC_LIST_SELECTION, o_ListSelection);
	DDX_Control(pDX, IDC_EDIT_JIRA_URL, o_EditJiraURL);
	DDX_Control(pDX, IDC_EDIT_JIRA_USER_ID, o_EditJiraUser);
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
	DDX_Control(pDX, IDC_EDIT_JIRA_PASSWORD, o_EditJiraPassword);
	DDX_Control(pDX, IDC_STATIC_JIRA_URL, o_StaticJiraURL);
	DDX_Control(pDX, IDC_STATIC_JIRA_PROJECT, o_StaticJiraTicket);
	DDX_Control(pDX, IDC_STATIC_JIRA_USER_ID, o_StaticJiraUserID);
	DDX_Control(pDX, IDC_STATIC_JIRA_PASSWORD, o_StaticJiraPassword);
	DDX_Control(pDX, IDC_STATIC_JIRA_ID_DEVIDER, o_StaticJiraTicketSep);
	DDX_Control(pDX, IDC_STATIC_SCREENSHOT_AREA, o_StaticScreenshotBoundary);
	DDX_Control(pDX, IDC_STATIC_FRM_SCREENSHOTS, o_StaticFrmScreenshots);
	DDX_Control(pDX, IDC_BUTTON_ENV_REFRESH, o_ButtonEnvRefresh);
}

BEGIN_MESSAGE_MAP(LTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ENV, &LTDlg::OnLvnItemchangedListEnv)
//	ON_NOTIFY(HDN_ITEMCLICK, 0, &LTDlg::OnHdnItemclickListEnv)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &LTDlg::OnBnClickedButtonTest)	
	ON_BN_CLICKED(IDC_BUTTON_ENV_ADD, &LTDlg::OnBnClickedButtonEnvAdd)
	ON_CBN_KILLFOCUS(IDC_COMBO_JIRA_PROJECT, &LTDlg::OnCbnKillfocusComboJiraProject)
	ON_CBN_KILLFOCUS(IDC_COMBO_INCLUDE_FILTER, &LTDlg::OnCbnKillfocusComboIncludeFilter)
	ON_CBN_KILLFOCUS(IDC_COMBO_EXCLUDE_FILTER, &LTDlg::OnCbnKillfocusComboExcludeFilter)
	ON_EN_KILLFOCUS(IDC_EDIT_TICKET_ID, &LTDlg::OnEnKillfocusEditTicketId)
	ON_BN_CLICKED(IDC_CHECK_JIRA_CREATE_TICKET, &LTDlg::OnBnClickedCheckJiraCreateTicket)
	ON_EN_CHANGE(IDC_EDIT_JIRA_USER_ID, &LTDlg::OnEnChangeEditJiraUserId)
	ON_EN_CHANGE(IDC_EDIT_JIRA_PASSWORD, &LTDlg::OnEnChangeEditJiraPassword)
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
//	ON_NOTIFY(HDN_ITEMCHANGED, 0, &LTDlg::OnHdnItemchangedListEnv)
	ON_BN_CLICKED(IDC_BUTTON_ENV_EDIT, &LTDlg::OnBnClickedButtonEnvEdit)
	ON_BN_CLICKED(IDC_BUTTON_ENV_DELETE, &LTDlg::OnBnClickedButtonEnvDelete)
	ON_WM_CREATE()
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &LTDlg::OnHdnItemdblclickListEnv)
ON_BN_CLICKED(IDC_BUTTON_SCREENSHOT_NEW, &LTDlg::OnBnClickedButtonScreenshotNew)
ON_BN_CLICKED(IDC_BUTTON_ENV_REFRESH, &LTDlg::OnBnClickedButtonEnvRefresh)
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
	o_EditJiraUser.SetWindowText(LTConfig::o_Inst.s_JiraUser);

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

	o_Resizer.Attach(&o_StaticFrmJira, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_CheckJiraComment, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_CheckJiraCreateNew, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_EditJiraURL, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_EditJiraUser, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_EditJiraTicket, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_EditJiraPassword, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_ComboJiraProject, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraPassword, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraTicket, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraTicketSep, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraURL, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraUserID, LT_RM_VIRTICAL);
	o_Resizer.Attach(&o_StaticJiraPassword, LT_RM_VIRTICAL);

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
	sFindFileExp.Format("%s/%s\\*",  sBaseFolder, sSubDir);

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
			CString sNewSubDir;
			sNewSubDir.Format("%s\\%s", sSubDir, FindFileData.cFileName);
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
	}
}

//void LTDlg::OnHdnItemclickListEnv(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void LTDlg::OnBnClickedButtonTest()
{
	TestCall();
}

int LTDlg::TestCall()
{
	CREDUI_INFO info = { sizeof (CREDUI_INFO) };
	info.hwndParent = GetSafeHwnd();

	info.pszCaptionText = "Title";
	info.pszMessageText = "Message";

	//CBitmap bitmap;
	//VERIFY(bitmap.LoadBitmap(IDB_PROMPT_BITMAP));
	//info.hbmBanner = bitmap;
	info.hbmBanner = 0;

	CString target = "https://jira.millenniumit.com";
	const DWORD errorCode = 0;

	char userName[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };

	strcpy_s(userName,
		_countof(userName),
		"sudantha");

	char password[CREDUI_MAX_PASSWORD_LENGTH + 1] = { 0 };

	BOOL saveChecked = false;

	DWORD flags = CREDUI_FLAGS_PERSIST | CREDUI_FLAGS_KEEP_USERNAME;

	DWORD result = ::CredUIPromptForCredentials(&info, target, 0, // reserved
		errorCode, userName, _countof(userName), password, _countof(password),
		&saveChecked, flags);

	switch (result)
	{
	case NO_ERROR:
		{
			// User chose OK...
			break;
		}
	case ERROR_CANCELLED:
		{
			// User chose Cancel...
			break;
		}
	default:
		{
			// Handle all other errors...
		}
	}

	::SecureZeroMemory(password,
		sizeof (password));

	return 0;


	LTSshSession* pSession = new LTSshSession;
	LTEnv* pEnv = LTEnv::FindEnv("survdev11");

	if (!pEnv)
		return 0;

	CString sErr = "";
	pSession->Connect(pEnv, sErr);
	pSession->Execute("ls -1 logs/Surv* corefiles/*");
	TRACE("===========================\n");

	return 0;
}


void LTDlg::OnBnClickedButtonEnvAdd()
{
	// TODO: Add your control notification handler code here
	LTAddEnvDlg oDlg;
	oDlg.SetDlg(this);
	oDlg.DoModal();
}


void LTDlg::AddLogEnv( const char* zUser, const char* zIP, const char* zBaseLocation /*= ""*/ )
{
	CString sNew;
	sNew.Format("%s@%s:%s", zUser, zIP, zBaseLocation);

	o_ComboLogMachines.AddString(sNew);
	o_StaticLogEnv.SetWindowText(sNew);


	LTConfig::o_Inst.GetLogMacSet()->Set(sNew);
}

void LTDlg::EditLogEnv( const char* zUser, const char* zIP, const char* zBaseLocation /*= ""*/ )
{
	CString sNew;
	sNew.Format("%s@%s:%s", zUser, zIP, zBaseLocation);

	o_ComboLogMachines.SetWindowText(sNew);
	o_StaticLogEnv.SetWindowText(sNew);

	LTConfig::o_Inst.GetLogMacSet()->Set(sNew);
}

void LTDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}

void LTDlg::OnChangeCombo( LTComboBox* pComboBox, const char* zValue )
{
	CString s = zValue;
}

void LTDlg::OnCbnKillfocusComboJiraProject()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnCbnKillfocusComboIncludeFilter()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnCbnKillfocusComboExcludeFilter()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnEnKillfocusEditTicketId()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnBnClickedCheckJiraCreateTicket()
{
	// TODO: Add your control notification handler code here
	int iCheck = o_CheckJiraCreateNew.GetCheck();
	LTConfig::o_Inst.b_JiraCreateNew = (iCheck != 0);

	o_EditJiraTicket.EnableWindow(!LTConfig::o_Inst.b_JiraCreateNew);
}

void LTDlg::OnEnChangeEditJiraUserId()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	o_EditJiraUser.GetWindowText(LTConfig::o_Inst.s_JiraUser);
}

void LTDlg::OnEnChangeEditJiraPassword()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void LTDlg::OnBnClickedCheckCommentOnJira()
{
	// TODO: Add your control notification handler code here
	int iCheck = o_CheckJiraComment.GetCheck();
	LTConfig::o_Inst.b_JiraDoComment = (iCheck != 0);
}

void LTDlg::OnCbnKillfocusComboSelection()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnBnClickedButtonFileSelectionAll()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnBnClickedButtonFileSelectionClear()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnBnClickedCheckFilterSelectAll()
{
	// TODO: Add your control notification handler code here
}

void LTDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	__super::OnKeyUp(nChar, nRepCnt, nFlags);
}

void LTDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

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

void LTDlg::OnPressEnterKey()
{
	
}

void LTDlg::OnBnClickedButtonLogmacNew()
{
	// TODO: Add your control notification handler code here
	LTAddLogEnvDlg oDlg;
	oDlg.SetDlg(this);
	oDlg.DoModal();
}

void LTDlg::OnBnClickedButtonLogmacEdit()
{
	// TODO: Add your control notification handler code here
	CString sLogEnv = "";
	o_StaticLogEnv.GetWindowText(sLogEnv);

	LTAddLogEnvDlg oDlg;
	oDlg.SetEditMode(sLogEnv);
	oDlg.SetDlg(this);
	oDlg.DoModal();
}



void LTDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	LTConfig::o_Inst.Save();
	__super::OnClose();
}

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

void LTDlg::OnBnClickedButtonEnvEdit()
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
				LTAddEnvDlg oDlg;
				oDlg.SetDlg(this);
				oDlg.SetEditMode(pEnv);
				oDlg.DoModal();
			}
		}
	}
	//returns -1 if not selected;
}

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
			}
		}
	}
}

int LTDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	o_ThumbnailsCtrl.CreateScreenshotCtrl(this, CRect(0,0,0,0), 300300);

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
				LTSshSession* pSession = new LTSshSession;

				std::list<CString> lst;

				CString sErr = "";
				pSession->Connect(pEnv, sErr);
				pSession->Execute("ls -1 logs/Surv* corefiles/*", &lst);
				TRACE("===========================\n");


				std::list<CString>::iterator itr = lst.begin();
				for (; itr != lst.end(); itr++)
				{
					o_ListSelection.InsertItem(LVIF_TEXT | LVIF_STATE, 0, *itr, 0, LVIS_SELECTED, 0, 0);
				}
			}
		}
	}
}
