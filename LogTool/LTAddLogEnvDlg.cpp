// LTAddLogEnvDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTAddLogEnvDlg.h"
#include "LTDlg.h"

// LTAddLogEnvDlg dialog

IMPLEMENT_DYNAMIC(LTAddLogEnvDlg, CDialog)

LTAddLogEnvDlg::LTAddLogEnvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTAddLogEnvDlg::IDD, pParent)
{
	p_Dlg= NULL;
}

LTAddLogEnvDlg::~LTAddLogEnvDlg()
{
}

void LTAddLogEnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ADD_LOG_ENV_USER, o_EditUser);
	DDX_Control(pDX, IDC_IPADDRESS_ADD_LOG_ENV_IP, o_IPIP);
	DDX_Control(pDX, IDC_EDIT_ADD_LOG_ENV_BASE, o_EditBaseLocation);
}


BEGIN_MESSAGE_MAP(LTAddLogEnvDlg, CDialog)
END_MESSAGE_MAP()


// LTAddLogEnvDlg message handlers

void LTAddLogEnvDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CString sUser = "";
	CString sIP = "";
	CString sBaseLoc = "";

	o_EditBaseLocation.GetWindowText(sBaseLoc);
	o_EditUser.GetWindowText(sUser);
	o_IPIP.GetWindowText(sIP);

	p_Dlg->AddLogEnv(sUser, sIP, sBaseLoc);

	CDialog::OnOK();
}
