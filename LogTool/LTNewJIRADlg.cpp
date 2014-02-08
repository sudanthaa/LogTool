// LTNewJIRADlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTNewJIRADlg.h"


// LTNewJIRADlg dialog

IMPLEMENT_DYNAMIC(LTNewJIRADlg, CDialog)

LTNewJIRADlg::LTNewJIRADlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTNewJIRADlg::IDD, pParent)
{

}

LTNewJIRADlg::~LTNewJIRADlg()
{
}

void LTNewJIRADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NEW_JIRA_SUMMARY, o_EditSummary);
	DDX_Control(pDX, IDC_EDIT_NEW_JIRA_DESCRIPTION, o_EditDescription);
	DDX_Control(pDX, IDC_COMBO_NEW_JIRA_TICKET_TYPE, o_ComboIssyeType);
}


BEGIN_MESSAGE_MAP(LTNewJIRADlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// LTNewJIRADlg message handlers

void LTNewJIRADlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	o_EditDescription.GetWindowText(s_Description);
	o_EditSummary.GetWindowText(s_Summary);
	CDialog::OnClose();
}

BOOL LTNewJIRADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	o_ComboIssyeType.AddString("Delivery-CR");
	o_ComboIssyeType.AddString("Delivery-ICR");
	o_ComboIssyeType.AddString("Fault-Bug");
	o_ComboIssyeType.AddString("Fault-Configuration");


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}