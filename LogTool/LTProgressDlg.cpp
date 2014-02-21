// LTProgressDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTProgressDlg.h"

// LTProgressDlg dialog

IMPLEMENT_DYNAMIC(LTProgressDlg, CDialog)

LTProgressDlg::LTProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTProgressDlg::IDD, pParent)
{

}

LTProgressDlg::~LTProgressDlg()
{
}

void LTProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LTProgressDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// LTProgressDlg message handlers

BOOL LTProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CWnd* pAreaFrame = GetDlgItem(IDC_STATIC_PROGRESS_FRAME);
	if (pAreaFrame)
	{
		CRect rWnd;
		pAreaFrame->GetWindowRect(rWnd);
		ScreenToClient(rWnd);
		o_Bar.MoveWindow(rWnd);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int LTProgressDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	o_Bar.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 101000);

	// TODO:  Add your specialized creation code here

	return 0;
}
