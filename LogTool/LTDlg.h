// LTDlg.h : header file
//

#pragma once
#include <vector>
#include "afxcmn.h"
#include "afxwin.h"
#include "LTFieldListener.h"
#include "LTComboBox.h"
#include "LTConfig.h"
#include "LTEnv.h"
#include "LTResizeMan.h"
#include "LTThumbnailsCtrl.h"

class LTSshSession;
class LTJiraCredentials;

// LTDlg dialog
class LTDlg : public CDialog, public LTFieldListener, public LTThumbnailsCtrlListener
{
// Construction
public:
	LTDlg(CWnd* pParent = NULL);	// standard constructor
	void	AddEnv(const char* zName, const char* zUser, const char* zIP, const char* zPassword);
	void	EditEnv(LTEnv* pEnv);
	void	AddLogEnv(const char* zEnvName, const char* zBaseLocation = "");
	void	EditLogEnv(const char* zCurrentStr, const char* zEnvName, const char* zBaseLocation = "");
	LTEnv*  GetSelectedEnv();

// Dialog Data
	enum { IDD = IDD_LOGTOOL };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	void	OnChangeCombo(LTComboBox* pComboBox, const char* zValue);
	void	OnEditScreenshot(LTThumbnailsCtrl::Screenshot* pScreenShot);
	void	SpawnNewJiraDlg();
	void	GetJiraTicketID( CString &sJiraTicketID );

// Implementation
protected:

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool	LoadEnvFromXShellConfig(void);
	void	InsertEnvToList( LTEnv* pEnv );
	bool	AttachFileToJira(const char* zFile, const char* zFileName,
						LTJiraCredentials* pCred, CString& sErr);
	bool	AttachFileToJira(char* zBuffer, int iBufferSize, const char* zFileName,
					LTJiraCredentials* pCred, CString& sErr);
	bool	CreateJiraTicket(const char* zProject, const char* zIssueType, CString& sID, 
				CString& sErr, LTJiraCredentials* pCred,
				const char* zSummary = "", const char* zDescription = "");
	bool	PutJiraComment(const char* zTiceketID, LTEnv* pDevEnv, LTEnv* pLogEnv, const char* zTicketPath,
				CString& sErr, LTJiraCredentials* pCred = NULL);
	void	OnPressEnterKey();
	void	InitResizes();
	void	GetAllFiles( CString sXShellSessionFolder, CString sSubFolder, VEC_ENV& rvecEvs);
	void	PopulateComboFromCfg(CComboBox* pCombo, LTConfig::StringSet* pStrSet);
	bool	ProvideJiraCred(LTJiraCredentials* pJiCred, CString& sErr,
				bool bWithID = true);
	bool	ProvideWinJiraCred(const char* zURL, CString& sUser, CString& sPassword, bool bUpdateAnyway = false);
	bool	UploadLogs(CString& sErr);
	void	AddCustomAction(const char* zName, const char* zCommand);
	void	UpdateCustomAction(LTConfig::CustomAction* pAction);

	LTSshSession* p_ConnectedSession;
	LTResizeMan o_Resizer;
	LTThumbnailsCtrl o_ThumbnailsCtrl;
	bool	b_ChangeSkip;

	CString		s_JiraDescription;
	CString		s_JiraSummary;
	CString		s_JiraIssueType;

protected:
	virtual void OnOK();
public:
	afx_msg void OnLvnItemchangedListEnv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonEnvAdd();
//	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnCbnKillfocusComboJiraProject();
	afx_msg void OnCbnKillfocusComboIncludeFilter();
	afx_msg void OnCbnKillfocusComboExcludeFilter();
	afx_msg void OnEnKillfocusEditTicketId();
	afx_msg void OnBnClickedCheckJiraCreateTicket();
	afx_msg void OnBnClickedCheckCommentOnJira();
	afx_msg void OnCbnKillfocusComboSelection();
	afx_msg void OnBnClickedButtonFileSelectionAll();
	afx_msg void OnBnClickedButtonFileSelectionClear();
	afx_msg void OnBnClickedCheckFilterSelectAll();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonLogmacNew();
	afx_msg void OnBnClickedButtonLogmacEdit();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonEnvEdit();
	afx_msg void OnBnClickedButtonEnvDelete();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonScreenshotNew();
	afx_msg void OnBnClickedButtonEnvRefresh();
	afx_msg void OnNMRClickListEnv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpload();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeEditTicketId();
	afx_msg void OnCbnSelchangeComboJiraProject();
	afx_msg void OnBnClickedButtonJiraCredentials();
	afx_msg void OnBnClickedButtonJiraTickeInfo();


	CListCtrl o_ListEnv;
	CStatic o_StaticLogEnv;
	CListCtrl o_ListSelection;
	CEdit o_EditJiraURL;
	CButton o_CheckJiraCreateNew;
	CButton o_CheckJiraComment;
	CEdit o_EditJiraTicket;
	CComboBox o_ComboLogMachines;
	CComboBox o_ComboIncludeFilters;
	CComboBox o_ComboExcludeFilters;
	CComboBox o_ComboJiraProject;
	CStatic o_StaticFrmFiles;
	CStatic o_StaticFrmJira;
	CStatic o_StaticFrmEnvionment;
	CStatic o_StaticFrmLogMac;
	CButton o_ButtonLogMacNew;
	CButton o_ButtonLogMacEdit;
	CButton o_ButtonLogMacDelete;
	CButton o_ButtonEnvNew;
	CButton o_ButtonEnvEdit;
	CButton o_ButtonEnvDelete;
	CButton o_ButtonCancel;
	CStatic o_StaticJiraURL;
	CStatic o_StaticJiraTicket;
	CStatic o_StaticJiraTicketSep;
	CStatic o_StaticScreenshotBoundary;
	CStatic o_StaticFrmScreenshots;
	CButton o_ButtonEnvRefresh;
	CButton o_ButtonUpload;
	CButton o_ButtonScreenshotNew;
	CButton o_ButtonScreenshotClear;
	CButton o_ButtonJiraCredentials;
	CButton o_ButtonJiraTicketInfo;

	CStatic o_StaticFrmConfiguredFileUpload;
	CListCtrl o_ListConfiguredUploads;
	afx_msg void OnBnClickedButtonAttachAlone();
	afx_msg void OnBnClickedButtonCfgactionNew();
	afx_msg void OnBnClickedButtonCfgactionEdit();
};


class LTCurlRetunBuffer
{
public:
	LTCurlRetunBuffer(int iLimit = 10000)
	{
		i_Len = 0;
		i_Limit = iLimit;
		p_Data = new char[i_Limit];
		*p_Data = 0;
	}

	~LTCurlRetunBuffer()
	{
		delete [] p_Data;
		p_Data = NULL;
	}

	bool	GetTicketID(CString& sOut);
	void	Append(int iLen, char* pData);
	bool	HasMoreSpace(int iLen);
	static size_t _WritFunc(void *ptr, size_t size, size_t nmemb, LTCurlRetunBuffer* s);

protected:
	char* p_Data;
	int i_Len;
	int i_Limit;
};