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
	p_Dlg = NULL;
	b_EditMode = false;
	s_EnvString = "";
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

	if (b_EditMode)
		p_Dlg->EditLogEnv(sUser, sIP, sBaseLoc);
	else 
		p_Dlg->AddLogEnv(sUser, sIP, sBaseLoc);

	CDialog::OnOK();
}

BOOL LTAddLogEnvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (b_EditMode)
	{
		int iPosEUEnd = s_EnvString.Find('@');
		int iPosIPEnd = s_EnvString.Find(':');
		CString sEnvUser = s_EnvString.Left(iPosEUEnd);
		CString sIP = "";
		CString sBase = "";

		if (iPosIPEnd == -1)
			sIP = s_EnvString.Right(s_EnvString.GetLength() - iPosEUEnd - 1);
		else 
		{
			sIP = s_EnvString.Mid(iPosEUEnd + 1, iPosIPEnd - iPosEUEnd - 1);
			sBase = s_EnvString.Right(s_EnvString.GetLength() - iPosIPEnd - 1);
		}

		o_EditUser.EnableWindow(FALSE);
		o_EditUser.SetWindowText(sEnvUser);
		o_IPIP.SetWindowText(sIP);
		o_EditBaseLocation.SetWindowText(sBase);
	}

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LTAddLogEnvDlg::SetEditMode( const char* zEnvString )
{
	s_EnvString = zEnvString;
	b_EditMode = true;
}
