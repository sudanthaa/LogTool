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
	p_DrawToolbar = NULL;
}

LTScreenshotEditDlg::~LTScreenshotEditDlg()
{
	delete p_Screenshot;
	p_Screenshot = NULL;
	delete p_DrawToolbar;
	p_DrawToolbar = NULL;
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


	CRect rDTEdit;
	CWnd* pStaticDrawToobar = GetDlgItem(IDC_STATIC_DRAW_TOOLBAR);
	pStaticDrawToobar->GetWindowRect(rDTEdit);
	ScreenToClient(rDTEdit);
	p_DrawToolbar->MoveWindow(rDTEdit);


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
	o_Resizer.Attach(p_DrawToolbar, LT_RM_ALL);
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

	p_DrawToolbar = new LTDrawToolBar;
	p_DrawToolbar->SetListener(this);
	p_DrawToolbar->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rClient, this, 10001);
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

void LTScreenshotEditDlg::OnToolPen( bool bPress )
{
	o_ScreenshotEditCtrl.PenStart();
}

void LTScreenshotEditDlg::OnToolRect( bool bPress )
{
	o_ScreenshotEditCtrl.RectStart();
}

void LTScreenshotEditDlg::OnToolArrow( bool bPress )
{
	o_ScreenshotEditCtrl.ArrowStart();
}

// LTDrawToolBar

IMPLEMENT_DYNAMIC(LTDrawToolBar, CWnd)

LTDrawToolBar::LTDrawToolBar()
{
	p_PenButton = new LTPaintFnButton(this, PaintToolButtonState, this);
	p_RectButton = new LTPaintFnButton(this, PaintToolButtonState, this);
	p_ArrowButton = new LTPaintFnButton(this, PaintToolButtonState, this);
	p_Listener = NULL;

	SetType(LTVirtualButtonOwner::CHECK_OPTIONAL_BUTTON);

	p_ActiveIcons = new Gdiplus::Image(L"draw-tools-active.png");
	p_DisabledIcons = new Gdiplus::Image(L"draw-tools-disabled.png");
}

LTDrawToolBar::~LTDrawToolBar()
{
}


BEGIN_MESSAGE_MAP(LTDrawToolBar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// LTDrawToolBar message handlers



int LTDrawToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	h_Theme = OpenThemeData(m_hWnd, L"BUTTON");

	// TODO:  Add your specialized creation code here

	return 0;
}

void LTDrawToolBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	p_PenButton->OnPaint(&dc);
	p_RectButton->OnPaint(&dc);
	p_ArrowButton->OnPaint(&dc);
}

void LTDrawToolBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rRect(0, 0, 0, 0);
	rRect.right = cx;
	rRect.bottom = cy;
	Layout(rRect);

	// TODO: Add your message handler code here
}

CWnd* LTDrawToolBar::GetCWnd()
{
	return this;
}

void LTDrawToolBar::OnPress( LTVirtualButton* pButton )
{
	//TRACE("PRESS..-%p:\n", pButton);
	if (pButton == p_PenButton)
		p_Listener->OnToolPen(true);
	else if (pButton == p_RectButton)
		p_Listener->OnToolRect(true);
	else if (pButton == p_ArrowButton)
		p_Listener->OnToolArrow(true);
}

void LTDrawToolBar::OnRelease( LTVirtualButton* pButton )
{
	//TRACE("RELEASE-%p:\n", pButton);
	if (pButton == p_PenButton)
		p_Listener->OnToolPen(false);
	else if (pButton == p_RectButton)
		p_Listener->OnToolRect(false);
	else if (pButton == p_ArrowButton)
		p_Listener->OnToolArrow(false);
}

void LTDrawToolBar::Layout( CRect rRect )
{
	CRect rPenButton = rRect;
	CRect rRectButton = rRect;
	CRect rArrowButton = rRect;

	int iWidth = rRect.Width() / 3;

	rPenButton.right = iWidth;
	rRectButton.left = iWidth;
	rRectButton.right = iWidth * 2;
	rArrowButton.left = iWidth * 2;
	rArrowButton.right = rRect.right;

	p_PenButton->SetRect(rPenButton);
	p_RectButton->SetRect(rRectButton);
	p_ArrowButton->SetRect(rArrowButton);
}

void LTDrawToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	p_PenButton->OnMouseMove(point,&dc);
	p_RectButton->OnMouseMove(point,&dc);
	p_ArrowButton->OnMouseMove(point,&dc);

	__super::OnMouseMove(nFlags, point);
}

//**************************************************************************************************
void LTDrawToolBar::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	MouseLeave();

	CClientDC dc(this);
	p_PenButton->OnMouseLeave(&dc);
	p_RectButton->OnMouseLeave(&dc);
	p_ArrowButton->OnMouseLeave(&dc);

	__super::OnMouseLeave();
}

//**************************************************************************************************
void LTDrawToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	p_PenButton->OnMouseDown(point, &dc);
	p_RectButton->OnMouseDown(point, &dc);
	p_ArrowButton->OnMouseDown(point, &dc);

	__super::OnLButtonDown(nFlags, point);
}

//**************************************************************************************************
void LTDrawToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	p_PenButton->OnMouseUp(point,&dc);
	p_RectButton->OnMouseUp(point,&dc);
	p_ArrowButton->OnMouseUp(point,&dc);

	__super::OnLButtonUp(nFlags, point);
}

#define  ICON_PADDING 2.0

//**************************************************************************************************
void LTDrawToolBar::PaintToolButtonState( int iState, CDC* pDC, CRect rArea, 
											LTVirtualButton* pButton, void* pContext )
{
	int iColor;
	int iIndex = 0;

	LTDrawToolBar* pDrawToobalr = (LTDrawToolBar*) pContext;

	if		(iState == BTN_STATE_NORMAL)		iColor = RGB(200,200,200);
	else if (iState == BTN_STATE_HOT)		iColor = RGB(220,200,200);
	else if (iState == BTN_STATE_PRESSED)		iColor = RGB(200,200,200);
	else if (iState == BTN_STATE_DISABLED)		iColor = RGB(200,200,200);

	if		(pButton == pDrawToobalr->p_PenButton)		iIndex = 0;
	else if	(pButton == pDrawToobalr->p_RectButton)		iIndex = 1;
	else if	(pButton == pDrawToobalr->p_ArrowButton)	iIndex = 2;

	int iHeight =  pDrawToobalr->p_ActiveIcons->GetHeight();

	Gdiplus::Image* pImage = pDrawToobalr->p_DisabledIcons;
	if (iState == BTN_STATE_PRESSED)
		pImage =  pDrawToobalr->p_ActiveIcons;

	Gdiplus::RectF rSource(0 * iIndex, 0, iHeight, iHeight);
	Gdiplus::RectF rTarget((rArea.left + (rArea.Width() - iHeight) / 2) , 
		(rArea.top + (rArea.Height() - iHeight) / 2), 
		iHeight, iHeight);


	//pDC->FillSolidRect(rArea, iColor);


	Gdiplus::Image imgBack(L"tool-tile.png");
	Gdiplus::Graphics oGraphics(pDC->m_hDC);


	oGraphics.DrawImage(&imgBack, rTarget, 0, 0, 
		iHeight, iHeight, Gdiplus::UnitPixel);

	//CRect rAreaEx = rArea;
	//rAreaEx.right += 1;
	//DrawThemeParentBackground(pDrawToobalr->m_hWnd, pDC->m_hDC, rAreaEx);
	//DrawThemeBackground(pDrawToobalr->h_Theme, pDC->m_hDC, BP_PUSHBUTTON, iState, rAreaEx, NULL);

	
	oGraphics.DrawImage(pImage, rTarget, iHeight * iIndex, 0, 
		iHeight, iHeight, Gdiplus::UnitPixel);
}

//**************************************************************************************************
bool LTDrawToolBar::PaintBack( LTVirtualButton* pButton, CDC* pDC, CRect rRect )
{
	return false;
}


//**************************************************************************************************
