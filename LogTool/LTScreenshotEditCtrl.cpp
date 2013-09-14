// LTScreenshotEditCtrl.cpp : implementation file
//

#include "LTPch.h"
#include "LTScreenshotEditCtrl.h"

// LTScreenshotEditCtrl

IMPLEMENT_DYNAMIC(LTScreenshotEditCtrl, CWnd)

LTScreenshotEditCtrl::LTScreenshotEditCtrl()
{
	p_Bitmap = NULL;
	pt_Offset.SetPoint(0,0);
}

LTScreenshotEditCtrl::~LTScreenshotEditCtrl()
{
}


BEGIN_MESSAGE_MAP(LTScreenshotEditCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// LTScreenshotEditCtrl message handlers



void LTScreenshotEditCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	DrawCtrl(&dc);
}

void LTScreenshotEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDown(nFlags, point);
}

void LTScreenshotEditCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonUp(nFlags, point);
}

void LTScreenshotEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);
}

int LTScreenshotEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void LTScreenshotEditCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL LTScreenshotEditCtrl::CreateScreenshotEditCtrl( CWnd* pParent, CRect rRect )
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_BORDER ;
	BOOL bRes = Create(NULL, NULL, dwStyle, rRect, pParent, 202021);
	return bRes;
}

void LTScreenshotEditCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	AdjustScroolBars(cx, cy);

	pt_Offset.SetPoint(0, 0);
	// TODO: Add your message handler code here
}

void LTScreenshotEditCtrl::AdjustScroolBars( int cx, int cy ) 
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;

	// Set horizontal 
	if (p_Bitmap && p_Bitmap->Width() > cx)
	{
		si.nMin = 0;
		si.nMax = p_Bitmap->Width() - 1;
		si.nPage = cx;
		//TRACE("HORSET: min=%d max=%d page=%d\n", si.nMin, si.nMax, si.nPage);
	}
	else
	{
		si.nMin = 0;
		si.nMax = 0;
		si.nPage = 0;
	}
	SetScrollInfo(SB_HORZ, &si);


	// Set vertical
	if (p_Bitmap && p_Bitmap->Height() > cy)
	{
		si.nMin = 0;
		si.nMax = p_Bitmap->Height() -1;
		si.nPage = cy;
		//TRACE("VERSET: min=%d max=%d page=%d\n", si.nMin, si.nMax, si.nPage);
	}
	else
	{
		si.nMin = 0;
		si.nMax = 0;
		si.nPage = 0;
	}

	SetScrollInfo(SB_VERT, &si);
}

void LTScreenshotEditCtrl::SetImage( LTBitmapBuffer* pBitmap )
{
	p_Bitmap = pBitmap;

	CRect rClient;
	GetClientRect(rClient);
	AdjustScroolBars(rClient.Width(), rClient.Height());
	Invalidate();
}

void LTScreenshotEditCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	UINT uiCurPos = GetScrollPos(SB_VERT);
	UINT uiLimit = GetScrollLimit(SB_VERT);

	CRect rClient;
	GetClientRect(rClient);

	switch (nSBCode)
	{
	case SB_TOP:            uiCurPos = 0;	break;
	case SB_BOTTOM:         uiCurPos = uiCurPos;	break;
	case SB_LINEUP:         uiCurPos = max(uiCurPos - 1, 0);	break;
	case SB_LINEDOWN:       uiCurPos = min(uiCurPos + 1, uiLimit);	break;
	case SB_PAGEUP:         uiCurPos = max(uiCurPos - rClient.Height(), 0);	break;
	case SB_PAGEDOWN:       uiCurPos = min(uiCurPos + rClient.Height(), uiLimit);	break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:  uiCurPos = nPos ;        break;
	}

	pt_Offset.y = (int)uiCurPos;

	CClientDC dc(this);
	DrawCtrl(&dc);

	//TRACE("OnVScroll: %d-%d-%d\n", nPos, uiCurPos, uiLimit);
	SetScrollPos(SB_VERT, uiCurPos);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void LTScreenshotEditCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	UINT uiCurPos = GetScrollPos(SB_HORZ);
	UINT uiLimit = GetScrollLimit(SB_HORZ);

	CRect rClient;
	GetClientRect(rClient);

	switch (nSBCode)
	{
	case SB_TOP:            uiCurPos = 0;	break;
	case SB_BOTTOM:         uiCurPos = uiCurPos;	break;
	case SB_LINEUP:         uiCurPos = max(uiCurPos - 1, 0);	break;
	case SB_LINEDOWN:       uiCurPos = min(uiCurPos + 1, uiLimit);	break;
	case SB_PAGEUP:         uiCurPos = max(uiCurPos - rClient.Height(), 0);	break;
	case SB_PAGEDOWN:       uiCurPos = min(uiCurPos + rClient.Height(), uiLimit);	break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:  uiCurPos = nPos ;        break;
	}

	pt_Offset.x = (int)uiCurPos;

	CClientDC dc(this);
	DrawCtrl(&dc);

	//TRACE("OnHScroll: %d-%d-%d\n", nPos, uiCurPos, uiLimit);
	SetScrollPos(SB_HORZ, uiCurPos);
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void LTScreenshotEditCtrl::DrawCtrl(CDC* pDC)
{
	CRect rRect;
	GetClientRect(rRect);

	if (p_Bitmap)
	{
		pDC->BitBlt(0, 0, rRect.Width(), rRect.Height(), 
			p_Bitmap->GetDC(), pt_Offset.x, pt_Offset.y, SRCCOPY);
	}
	else
		pDC->FillSolidRect(rRect, RGB(200,200,200));
}
