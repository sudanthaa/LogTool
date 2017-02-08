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

	CDialog::OnClose();
}

BOOL LTNewJIRADlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	o_ComboIssyeType.AddString("Bug");
	o_ComboIssyeType.AddString("CC-System");
	o_ComboIssyeType.AddString("Delivery-CR");
	o_ComboIssyeType.AddString("Delivery-ICR");
	o_ComboIssyeType.AddString("Fault-Bug");
	o_ComboIssyeType.AddString("Fault-Configuration");
	o_ComboIssyeType.SetCurSel(0);

	
	o_EditDescription.SetWindowText(s_Description);
	o_EditSummary.SetWindowText(s_Summary);


	for (int i = 0; i < o_ComboIssyeType.GetCount(); i++)
	{
		CString s;
		o_ComboIssyeType.GetLBText(i, s);
		if (s == s_Type)
		{
			o_ComboIssyeType.SetCurSel(i);
			break;
		}
	}

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LTNewJIRADlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	o_EditDescription.GetWindowText(s_Description);
	o_EditSummary.GetWindowText(s_Summary);
	int iSel = o_ComboIssyeType.GetCurSel();
	if (iSel > -1)
		o_ComboIssyeType.GetLBText(iSel, s_Type);

	CDialog::OnOK();
}
