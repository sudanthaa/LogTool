// LTScreenCaptureDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTScreenCaptureDlg.h"


// LTScreenCaptureDlg dialog

IMPLEMENT_DYNAMIC(LTScreenCaptureDlg, CDialog)

LTScreenCaptureDlg::LTScreenCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTScreenCaptureDlg::IDD, pParent)
{
	pbmp_VirScreenCopy = NULL;
	pdc_VirScreenCopy = NULL;
	e_State = MOUSE_STATE_FREE;
}

LTScreenCaptureDlg::~LTScreenCaptureDlg()
{
}

void LTScreenCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LTScreenCaptureDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// LTScreenCaptureDlg message handlers

void LTScreenCaptureDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	if (pdc_VirScreenCopy && pbmp_VirScreenCopy)
	{
		CBitmap* pbmpOld = pdc_VirScreenCopy->SelectObject(pbmp_VirScreenCopy);
		dc.BitBlt(0, 0, r_Rect.Width(), r_Rect.Height(), pdc_VirScreenCopy, 0, 0, SRCCOPY);
		pdc_VirScreenCopy->SelectObject(pbmpOld);
	}
}

BOOL LTScreenCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// Show Current application window before taking the screenshot
	ShowWindow(SW_HIDE);
	GetParent()->ShowWindow(SW_HIDE);
	GetParent()->GetParent()->ShowWindow(SW_HIDE);

	CClientDC dc(this);

	// Virtual screen rect
	r_Rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	r_Rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	r_Rect.right = r_Rect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	r_Rect.bottom = r_Rect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);

	pbmp_VirScreenCopy = new CBitmap;
	pbmp_VirScreenCopy->CreateCompatibleBitmap(&dc, r_Rect.Width(), r_Rect.Height());
	pdc_VirScreenCopy = new CDC;
	pdc_VirScreenCopy->CreateCompatibleDC(&dc);
	CBitmap* pbmpOld = pdc_VirScreenCopy->SelectObject(pbmp_VirScreenCopy);

	// Taking virtual screen dc.
	HDC hDC = ::GetDC(NULL);
	CDC* pdcVirtScreen = CDC::FromHandle(hDC);
	pdc_VirScreenCopy->BitBlt(0, 0, r_Rect.Width(), r_Rect.Height(), pdcVirtScreen, r_Rect.left, r_Rect.top, SRCCOPY);
	::ReleaseDC(NULL, hDC);

	pdc_VirScreenCopy->SelectObject(pbmpOld);

	// Show back the current application after taking screenshot
	
	GetParent()->GetParent()->ShowWindow(SW_SHOW);
	GetParent()->ShowWindow(SW_SHOW);
	ShowWindow(SW_SHOW);

	MoveWindow(r_Rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int LTScreenCaptureDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void LTScreenCaptureDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void LTScreenCaptureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (e_State == MOUSE_STATE_FREE)
	{

	}
	else if (e_State == MOUSE_STATE_FIRST_POINT_SELECT)
	{
		CClientDC dc(this);
		XORPenContext oCtx(&dc);

		DrawCross(&dc, r_LastHor, r_LastVir);

		PointToCrossLines(point, r_LastHor, r_LastVir);
		DrawCross(&dc, r_LastHor, r_LastVir);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void LTScreenCaptureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (e_State == MOUSE_STATE_FREE)
		e_State = MOUSE_STATE_FIRST_POINT_SELECT


	CDialog::OnLButtonDown(nFlags, point);
}

void LTScreenCaptureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonUp(nFlags, point);
}

void LTScreenCaptureDlg::PointToCrossLines( CPoint pt, CRect& rHor, CRect& rVir )
{
	rVir.top = pt.y - 50;
	rVir.left = pt.x;
	rVir.bottom = rVir.top + 100;
	rVir.right = pt.x;

	rHor.top = pt.y;
	rHor.left =  pt.x - 50;
	rHor.bottom = pt.y; 
	rHor.right = rHor.left + 100;
}

void LTScreenCaptureDlg::DrawCross(CDC* pDC, CRect& rHor, CRect& rVir )
{
	pDC->MoveTo(rVir.TopLeft());
	pDC->LineTo(rVir.BottomRight());
	pDC->MoveTo(rHor.TopLeft());
	pDC->LineTo(rHor.BottomRight());
}

void LTScreenCaptureDlg::DrawRect( CDC* pDC, CRect rRect )
{
	pDC->MoveTo(rRect.left, rRect.top);
	pDC->LineTo(rRect.right, rRect.top);
	pDC->LineTo(rRect.right, rRect.bottom);
	pDC->LineTo(rRect.left, rRect.bottom);
	pDC->LineTo(rRect.left, rRect.top);
}

LTScreenCaptureDlg::XORPenContext::XORPenContext( CDC* pDC )
{

}

LTScreenCaptureDlg::XORPenContext::~XORPenContext()
{

}
