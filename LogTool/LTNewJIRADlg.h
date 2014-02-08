#pragma once
#include "afxwin.h"

#include "resource.h"

// LTNewJIRADlg dialog

class LTNewJIRADlg : public CDialog
{
	DECLARE_DYNAMIC(LTNewJIRADlg)

public:
	LTNewJIRADlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTNewJIRADlg();

	CString s_Summary;
	CString s_Description;
	CString s_Type;

// Dialog Data
	enum { IDD = IDD_NEW_JIRA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	CEdit o_EditSummary;
	CEdit o_EditDescription;
	CComboBox o_ComboIssyeType;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
