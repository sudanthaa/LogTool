// LTAddLogEnvDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTAddLogEnvDlg.h"
#include "LTDlg.h"
#include "LTUtils.h"

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
	DDX_Control(pDX, IDC_EDIT_ADD_LOG_ENV_BASE, o_EditBaseLocation);
	DDX_Control(pDX, IDC_ADD_LOG_ENV_ENV, o_ComboEnv);
}


BEGIN_MESSAGE_MAP(LTAddLogEnvDlg, CDialog)
END_MESSAGE_MAP()


// LTAddLogEnvDlg message handlers

void LTAddLogEnvDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CString sEnvName = "";
	CString sBaseLoc = "";

	o_ComboEnv.GetLBText(o_ComboEnv.GetCurSel(), sEnvName);
	o_EditBaseLocation.GetWindowText(sBaseLoc);

	if (b_EditMode)
		p_Dlg->EditLogEnv(sEnvName, sBaseLoc);
	else 
		p_Dlg->AddLogEnv(sEnvName, sBaseLoc);

	CDialog::OnOK();
}

BOOL LTAddLogEnvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (b_EditMode)
	{
		CString sEnvName,sPath;

		LTUtils::DecodePathStringEx(s_EnvString, sEnvName, sPath);
		o_EditBaseLocation.SetWindowText(sPath);

		CString sTitle;
		sTitle.Format("Edit Log Environment - (%s)", s_EnvString);
		SetWindowText(sTitle);

		o_ComboEnv.AddString(sEnvName);
		o_ComboEnv.SetCurSel(0);

		o_ComboEnv.EnableWindow(FALSE);
	}
	else
	{
		VEC_ENV_ITR itr = LTEnv::vec_Env.begin();
		for (; itr != LTEnv::vec_Env.end(); itr++)
		{
			LTEnv* pEnv = *itr;
			o_ComboEnv.AddString(pEnv->s_Name);
		}

		if (LTEnv::vec_Env.size() > 0)
			o_ComboEnv.SetCurSel(0);
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
