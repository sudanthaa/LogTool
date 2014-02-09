// LTConfigActionDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTConfigActionDlg.h"
#include "LTDlg.h"


// LTConfigActionDlg dialog

IMPLEMENT_DYNAMIC(LTConfigActionDlg, CDialog)

LTConfigActionDlg::LTConfigActionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTConfigActionDlg::IDD, pParent)
{
	p_InitAction = NULL;
	p_Dlg = NULL;
}

LTConfigActionDlg::~LTConfigActionDlg()
{
}

void LTConfigActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_CFGCOMMAND_DESC, o_RichTextDescription);
	DDX_Control(pDX, IDC_EDIT_CONFIGURED_ACTION, o_EditCommand);
	DDX_Control(pDX, IDC_EDIT_CONFIGURED_ACTION_NAME, o_EditActionName);
}


BEGIN_MESSAGE_MAP(LTConfigActionDlg, CDialog)
END_MESSAGE_MAP()


// LTConfigActionDlg message handlers

BOOL LTConfigActionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	COLORREF cr = GetSysColor(COLOR_BTNFACE);
	o_RichTextDescription.SetBackgroundColor(FALSE, cr);
	// TODO:  Add extra initialization here
	o_RichTextDescription.SetWindowText("{\\rtf1\\ansi\\deff0"
	"{\\colortbl;\\red100\\green100\\blue100;\\red0\\green0\\blue0;}"
	"\\cf1"
	"Input the shell command to execute the desired action. May use any \\line "
	"command that is available in your environment. The shell variables \\line"
	"\\cf2\\b $logenvuser $logenvip $logenvpath $logenvsshcon \\b0\\cf1 are available \\line "
	"for use. To copy anything into log environment, use scp. \\line"
	"\\i eg:\\i0 \\line\\b  scp nora.tar.gz $logenvsshcon:$logenvpath \\b0\\line\\line"
	" Multiple commands has to be separate them with a semi-colon. This string \\line"
	" will be executed as it is through ssh. So any command that works in \\line"
	" environment will work.}");

	if (p_InitAction)
	{
		o_EditActionName.SetWindowText(p_InitAction->s_Name);
		o_EditCommand.SetWindowText(p_InitAction->s_Command);
		//o_EditActionName.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LTConfigActionDlg::SetEditMode( LTConfig::CustomAction* pAction )
{
	p_InitAction = pAction;
}

void LTConfigActionDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	if (p_InitAction)
	{
		o_EditActionName.GetWindowText(p_InitAction->s_Name);
		o_EditCommand.GetWindowText(p_InitAction->s_Command);
		p_Dlg->UpdateCustomAction(p_InitAction);
	}
	else
	{
		CString sName;
		CString sCommand;
		o_EditActionName.GetWindowText(sName);
		o_EditCommand.GetWindowText(sCommand);
		p_Dlg->AddCustomAction(sName, sCommand);
	}

	CDialog::OnOK();
}
