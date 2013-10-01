// LTScreenshotEditDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTScreenshotEditDlg.h"
#include "LTScreenCaptureDlg.h"

#define  AUTO_SPAWN_INITIATE_TIMER_ID  20020
// LTScreenshotEditDlg dialog

IMPLEMENT_DYNAMIC(LTScreenshotEditDlg, CDialog)

LTScreenshotEditDlg::LTScreenshotEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTScreenshotEditDlg::IDD, pParent)
{
	p_Screenshot = NULL;
}

LTScreenshotEditDlg::~LTScreenshotEditDlg()
{
	delete p_Screenshot;
	p_Screenshot = NULL;
}

void LTScreenshotEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, o_BtnOK);
	DDX_Control(pDX, IDCANCEL, o_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_TAKE, o_BtnTake);
	DDX_Control(pDX, IDC_EDIT_SCREENSHOT_EDIT_NAME, o_EditName);
}


BEGIN_MESSAGE_MAP(LTScreenshotEditDlg, CDialog)
	ON_BN_CLICKED(IDOK, &LTScreenshotEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TAKE, &LTScreenshotEditDlg::OnBnClickedButtonTake)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SCREENSHOT_EDIT_PEN, &LTScreenshotEditDlg::OnBnClickedButtonScreenshotEditPen)
	ON_BN_CLICKED(IDC_BUTTON_SCREENSHOT_EDIT_RECT, &LTScreenshotEditDlg::OnBnClickedButtonScreenshotEditRect)
END_MESSAGE_MAP()


// LTScreenshotEditDlg message handlers

void LTScreenshotEditDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	p_Screenshot = o_ScreenshotEditCtrl.DetachScreenshot();
	CString sName;
	o_EditName.GetWindowText(sName);
	p_Screenshot->SetName(sName);

	p_Screenshot->Save("test.jpg");

	EndDialog(IDOK);
	//OnOK();
}

BOOL LTScreenshotEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetTimer(AUTO_SPAWN_INITIATE_TIMER_ID, 100, NULL);


	CRect rSSEdit;
	CWnd* pStaticSSEdit = GetDlgItem(IDC_STATIC_SCREENSHOT_EDIT_CTRL);
	pStaticSSEdit->GetWindowRect(rSSEdit);
	ScreenToClient(rSSEdit);
	o_ScreenshotEditCtrl.MoveWindow(rSSEdit);

	static int iNameSeed = 0;
	iNameSeed++;
	CString sName;
	sName.Format("ss-%03d", iNameSeed);
	o_EditName.SetWindowText(sName);

	o_Resizer.Attach(&o_ScreenshotEditCtrl, LT_RM_BOTTMRIGHT);
	o_Resizer.Attach(&o_BtnOK, LT_RM_ALL);
	o_Resizer.Attach(&o_BtnCancel, LT_RM_ALL);
	o_Resizer.Attach(&o_BtnTake, LT_RM_HORIZONTAL);
	o_Resizer.Attach(&o_EditName, LT_RM_HORIZONTAL);
	o_Resizer.Originate(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LTScreenshotEditDlg::OnBnClickedButtonTake()
{
	// TODO: Add your control notification handler code here

	TakeScreenshot();

	//BeginSelect();
}

void LTScreenshotEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnMouseMove(nFlags, point);
}

void LTScreenshotEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}

void LTScreenshotEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonUp(nFlags, point);
}

void LTScreenshotEditDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);


	// TODO: Add your message handler code here
}

void LTScreenshotEditDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == AUTO_SPAWN_INITIATE_TIMER_ID)
	{
		KillTimer(AUTO_SPAWN_INITIATE_TIMER_ID);
		TakeScreenshot();

	}
	
	CDialog::OnTimer(nIDEvent);
}

int LTScreenshotEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rClient(0,0,0,0);
	o_ScreenshotEditCtrl.CreateScreenshotEditCtrl(this, rClient);
	// TODO:  Add your specialized creation code here

	return 0;
}

void LTScreenshotEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	o_Resizer.Resize(cx, cy);
	// TODO: Add your message handler code here
}

void LTScreenshotEditDlg::TakeScreenshot()
{
	LTScreenCaptureDlg oDlg;
	int iRes = oDlg.DoModal();
	if (iRes == IDOK)
	{
		LTBitmapBuffer* pImage = oDlg.DetachOutput();
		o_ScreenshotEditCtrl.SetImage(pImage);
	}
}

void LTScreenshotEditDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	p_Screenshot = o_ScreenshotEditCtrl.DetachScreenshot();

	CString sName;
	o_EditName.GetWindowText(sName);
	p_Screenshot->SetName(sName);
	CDialog::OnOK();
}

void LTScreenshotEditDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

void LTScreenshotEditDlg::OnBnClickedButtonScreenshotEditPen()
{
	// TODO: Add your control notification handler code here

	o_ScreenshotEditCtrl.PenStart();
}

void LTScreenshotEditDlg::OnBnClickedButtonScreenshotEditRect()
{
	o_ScreenshotEditCtrl.RectStart();
	// TODO: Add your control notification handler code here
}

LTScreenshot* LTScreenshotEditDlg::DetachScreenshot()
{
	LTScreenshot* pScreenshot = p_Screenshot;
	p_Screenshot = NULL;
	return pScreenshot;
}
