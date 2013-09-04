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
#include "LTScreenshotCtrl.h"

// LTDlg dialog
class LTDlg : public CDialog, public LTFieldListener
{
// Construction
public:
	LTDlg(CWnd* pParent = NULL);	// standard constructor
	void	AddEnv(const char* zName, const char* zUser, const char* zIP, const char* zPassword);
	void	EditEnv(LTEnv* pEnv);
	void	AddLogEnv(const char* zUser, const char* zIP, const char* zBaseLocation = "");
	void	EditLogEnv(const char* zUser, const char* zIP, const char* zBaseLocation = "");

// Dialog Data
	enum { IDD = IDD_LOGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	void	OnChangeCombo(LTComboBox* pComboBox, const char* zValue);

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
	int		TestCall();
	void	OnPressEnterKey();
	void	InitResizes();
	void	GetAllFiles( CString sXShellSessionFolder, CString sSubFolder, VEC_ENV& rvecEvs);
	void	PopulateComboFromCfg(CComboBox* pCombo, LTConfig::StringSet* pStrSet);

	LTResizeMan o_Resizer;
	LTScreenshotCtrl o_Screenshot;

protected:
	virtual void OnOK();
public:
	CComboBox o_ComboJiraProject;
	afx_msg void OnLvnItemchangedListEnv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickListEnv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonEnvAdd();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnCbnKillfocusComboJiraProject();
	afx_msg void OnCbnKillfocusComboIncludeFilter();
	afx_msg void OnCbnKillfocusComboExcludeFilter();
	afx_msg void OnEnKillfocusEditTicketId();
	afx_msg void OnBnClickedCheckJiraCreateTicket();
	afx_msg void OnEnChangeEditJiraUserId();
	afx_msg void OnEnChangeEditJiraPassword();
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

	CListCtrl o_ListEnv;
	CStatic o_StaticLogEnv;
	CListCtrl o_ListSelection;
	CEdit o_EditJiraURL;
	CEdit o_EditJiraUser;
	CButton o_CheckJiraCreateNew;
	CButton o_CheckJiraComment;
	CEdit o_EditJiraTicket;
	CComboBox o_ComboLogMachines;
	CComboBox o_ComboIncludeFilters;
	CComboBox o_ComboExcludeFilters;
	afx_msg void OnSize(UINT nType, int cx, int cy);
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
	CEdit o_EditJiraPassword;
	CStatic o_StaticJiraURL;
	CStatic o_StaticJiraTicket;
	CStatic o_StaticJiraUserID;
	CStatic o_StaticJiraPassword;
	CStatic o_StaticJiraTicketSep;
	afx_msg void OnHdnItemchangedListEnv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonEnvEdit();
	afx_msg void OnBnClickedButtonEnvDelete();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CStatic o_StaticScreenshotBoundary;
	CStatic o_StaticFrmScreenshots;
};
