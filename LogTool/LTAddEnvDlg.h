#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

class LTDlg;

// LTAddEnvDlg dialog

class LTAddEnvDlg : public CDialog
{
	DECLARE_DYNAMIC(LTAddEnvDlg)

public:
	LTAddEnvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTAddEnvDlg();

	void	SetDlg(LTDlg* pDlg)	{ p_Dlg = pDlg; };

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
};
