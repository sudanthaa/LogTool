#pragma once

#include <resource.h>
#include "afxwin.h"
#include "afxcmn.h"

class LTDlg;

// LTAddLogEnvDlg dialog

class LTAddLogEnvDlg : public CDialog
{
	DECLARE_DYNAMIC(LTAddLogEnvDlg)

public:
	LTAddLogEnvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTAddLogEnvDlg();

	void	SetDlg(LTDlg* pDlg) { p_Dlg = pDlg; };
	LTDlg* p_Dlg;

// Dialog Data
	enum { IDD = IDD_ADD_LOG_ENV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CEdit o_EditUser;
	CIPAddressCtrl o_IPIP;
	CEdit o_EditBaseLocation;
};
