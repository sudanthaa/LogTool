#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

class LTDlg;
class LTEnv;

// LTAddEnvDlg dialog

class LTAddEnvDlg : public CDialog
{
	DECLARE_DYNAMIC(LTAddEnvDlg)

public:
	LTAddEnvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTAddEnvDlg();

	void	SetDlg(LTDlg* pDlg)	{ p_Dlg = pDlg; };
	void	SetEditMode(LTEnv* pEnv) { p_Env = pEnv; };

	LTEnv* p_Env;
	LTDlg* p_Dlg;

// Dialog Data
	enum { IDD = IDD_ADD_ENV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CEdit o_EditUser;
	CIPAddressCtrl o_IP;
	virtual BOOL OnInitDialog();
	CEdit o_EditPassword;
	CEdit o_EditName;
};
