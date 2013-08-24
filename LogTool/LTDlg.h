// LTDlg.h : header file
//

#pragma once
#include <vector>
#include "afxcmn.h"
#include "afxwin.h"
#include "LTFieldListener.h"
#include "LTComboBox.h"

class LTEnv;

typedef std::vector<LTEnv*>  VEC_ENV;
typedef VEC_ENV::iterator	  VEC_ENV_ITR;

// LTDlg dialog
class LTDlg : public CDialog, public LTFieldListener
{
// Construction
public:
	LTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void	AddEnv(const char* zUser, const char* zIP);
	void	AddLogEnv(const char* zUser, const char* zIP, const char* zBaseLocation = "");
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
	CListCtrl o_ListEnv;
	bool LoadEnvFromXShellConfig(void);
	int	 TestCall();
	void OnPressEnterKey();

	void GetAllFiles( CString sXShellSessionFolder, CString sSubFolder, VEC_ENV& rvecEvs);

	afx_msg void OnLvnItemchangedListEnv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickListEnv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTest();
	CListCtrl o_ListSelection;
	afx_msg void OnBnClickedButtonEnvAdd();
	CEdit o_EditJiraURL;
	CEdit o_EditJiraUser;
	CComboBox o_ComboLogMachines;
protected:
	virtual void OnOK();
public:
	CComboBox o_ComboJiraProject;
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
};
