// LTProgressDlg.cpp : implementation file
//

#include "LTPch.h"

#include <list>
#include "LTProgressDlg.h"
#include "LTProcThread.h"
#include "LTOutputStringQueue.h"

// LTProgressDlg dialog

#define  EXIT_TIMER  100012


IMPLEMENT_DYNAMIC(LTProgressDlg, CDialog)

LTProgressDlg::LTProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTProgressDlg::IDD, pParent)
{
	p_Thread = NULL;
	i_Seq = 0;
}

LTProgressDlg::~LTProgressDlg()
{
}

void LTProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOG, o_StaticLog);
}


BEGIN_MESSAGE_MAP(LTProgressDlg, CDialog)
	ON_WM_CREATE()
	ON_MESSAGE(WM_TERMINATE_THREAD, &LTProgressDlg::OnTermRequest)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// LTProgressDlg message handlers

BOOL LTProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pAreaFrame = GetDlgItem(IDC_STATIC_PROGRESS_FRAME);
	if (pAreaFrame)
	{
		CRect rWnd;
		pAreaFrame->GetWindowRect(rWnd);
		ScreenToClient(rWnd);
		o_Bar.MoveWindow(rWnd);
	}

	SetTimer(EXIT_TIMER, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT LTProgressDlg::OnTermRequest( WPARAM, LPARAM )
{
	EndDialog(IDCANCEL);
	return 0;
}

int LTProgressDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	o_Bar.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 101000);

	// TODO:  Add your specialized creation code here

	return 0;
}



void LTProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == EXIT_TIMER)
	{
		CString sString = "";
		if (LTOutputStringQueue::GetLastStringSet(i_Seq, 4, sString))
			o_StaticLog.SetWindowText(sString);

		if (p_Thread && p_Thread->IsTermSignalled())
		{
			KillTimer(EXIT_TIMER);
			EndDialog(IDCANCEL);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
