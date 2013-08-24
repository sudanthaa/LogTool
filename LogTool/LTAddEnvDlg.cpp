// LTAddEnvDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTAddEnvDlg.h"


// LTAddEnvDlg dialog

IMPLEMENT_DYNAMIC(LTAddEnvDlg, CDialog)

LTAddEnvDlg::LTAddEnvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTAddEnvDlg::IDD, pParent)
{
	p_Dlg = NULL;
}

LTAddEnvDlg::~LTAddEnvDlg()
{
}

void LTAddEnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ADD_ENV_USER, o_EditUser);
	DDX_Control(pDX, IDC_IPADDRESS_ADD_ENV_IP, o_IP);
}


BEGIN_MESSAGE_MAP(LTAddEnvDlg, CDialog)
END_MESSAGE_MAP()


// LTAddEnvDlg message handlers

void LTAddEnvDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnOK();
}
