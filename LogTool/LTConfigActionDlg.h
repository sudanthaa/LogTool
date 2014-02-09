#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "LTConfig.h"

// LTConfigActionDlg dialog
class LTDlg;

class LTConfigActionDlg : public CDialog
{
	DECLARE_DYNAMIC(LTConfigActionDlg)

public:
	LTConfigActionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTConfigActionDlg();

	void	SetDlg(LTDlg* pDlg) { p_Dlg = pDlg; };
	void	SetEditMode(LTConfig::CustomAction* pAction);
	LTConfig::CustomAction* p_InitAction;
	LTDlg*  p_Dlg;

// Dialog Data
	enum { IDD = IDD_ADD_CONFIGURED_ACTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl o_RichTextDescription;
	virtual BOOL OnInitDialog();
	CEdit o_EditCommand;
	CEdit o_EditActionName;
protected:
	virtual void OnOK();
public:
	afx_msg void OnStnClickedStaticConfiguredCommand();
};
