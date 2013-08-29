// LTAddEnvDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTAddEnvDlg.h"
#include "LTDlg.h"

// LTAddEnvDlg dialog

IMPLEMENT_DYNAMIC(LTAddEnvDlg, CDialog)

LTAddEnvDlg::LTAddEnvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTAddEnvDlg::IDD, pParent)
{
	p_Dlg = NULL;
	p_Env = NULL;
}

LTAddEnvDlg::~LTAddEnvDlg()
{
}

void LTAddEnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ADD_ENV_USER, o_EditUser);
	DDX_Control(pDX, IDC_IPADDRESS_ADD_ENV_IP, o_IP);
	DDX_Control(pDX, IDC_EDIT_ADD_ENV_PASSWORD, o_EditPassword);
}


BEGIN_MESSAGE_MAP(LTAddEnvDlg, CDialog)
END_MESSAGE_MAP()


// LTAddEnvDlg message handlers

void LTAddEnvDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if (p_Env) // Edit Mode;
	{
		o_EditPassword.GetWindowText(p_Env->s_Password);
		o_EditPassword.GetWindowText(p_Env->s_IP);

		p_Dlg->EditEnv(p_Env);
	}
	else
	{
		CString sUser;
		CString sPassword;
		CString sIP;
		o_EditPassword.GetWindowText(sPassword);
		o_EditUser.GetWindowText(sUser);
		o_IP.GetWindowText(sIP);

		p_Dlg->AddEnv(sUser, sIP, sPassword);
	}

	CDialog::OnOK();
}

BOOL LTAddEnvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (p_Env) // Edit Mode;
	{
		o_EditUser.SetWindowText(p_Env->s_EnvUser);
		o_EditUser.EnableWindow(FALSE);

		o_EditPassword.SetWindowText(p_Env->s_Password);
		o_IP.SetWindowText(p_Env->s_IP);
	}
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
