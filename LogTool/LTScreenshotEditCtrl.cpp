// LTScreenshotEditCtrl.cpp : implementation file
//

#include "LTPch.h"

#include "LTScreenshotEditCtrl.h"

// LTScreenshotEditCtrl

IMPLEMENT_DYNAMIC(LTScreenshotEditCtrl, CWnd)

LTScreenshotEditCtrl::LTScreenshotEditCtrl()
{
	p_Screenshot = NULL;
	pt_Offset.SetPoint(0,0);
	e_State = STATE_FREE;
	h_Theme = 0;
	cr_Color = RGB(255, 0,0);
	i_With = 1;
}

LTScreenshotEditCtrl::~LTScreenshotEditCtrl()
{
	delete p_Screenshot;
	p_Screenshot = NULL;
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
	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// LTScreenshotEditCtrl message handlers



void LTScreenshotEditCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	SetClipForSmall(&dc);

	DrawCtrl(&dc);
}

void LTScreenshotEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	SetClipForSmall(&dc);

	if (e_State == STATE_PEN_START)
	{
		e_State = STATE_PEN_DRAW;
		p_ActiveMarking = new LTPenMarking(p_Screenshot, cr_Color, i_With);
		p_ActiveMarking->OnMouseDown(&dc, point, pt_Offset);
		SetCapture();
	}
	else if (e_State == STATE_RECT_START)
	{
		e_State = STATE_RECT_DRAW;
		p_ActiveMarking = new LTRectMarking(p_Screenshot, cr_Color, i_With);
		p_ActiveMarking->OnMouseDown(&dc, point, pt_Offset);
		SetCapture();
	}
	else if (e_State == STATE_ARROW_START)
	{
		e_State = STATE_ARROW_DRAW;
		p_ActiveMarking = new LTArrowMarking(p_Screenshot, cr_Color, i_With);
		p_ActiveMarking->OnMouseDown(&dc, point, pt_Offset);
		SetCapture();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void LTScreenshotEditCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	SetClipForSmall(&dc);

	if (e_State == STATE_PEN_DRAW)
	{
		e_State = STATE_PEN_START;
		p_ActiveMarking->OnMouseUp(&dc, point, pt_Offset);
		p_Screenshot->Add(p_ActiveMarking);
		p_ActiveMarking = NULL;
		ReleaseCapture();
	}
	else if (e_State == STATE_RECT_DRAW)
	{
		e_State = STATE_RECT_START;
		ReleaseCapture();
		p_ActiveMarking->OnMouseUp(&dc, point, pt_Offset);
		p_Screenshot->Add(p_ActiveMarking);
		p_ActiveMarking = NULL;
	}
	else if (e_State == STATE_ARROW_DRAW)
	{
		e_State = STATE_ARROW_START;
		ReleaseCapture();
		p_ActiveMarking->OnMouseUp(&dc, point, pt_Offset);
		p_Screenshot->Add(p_ActiveMarking);
		p_ActiveMarking = NULL;
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void LTScreenshotEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	SetClipForSmall(&dc);


	if (e_State == STATE_PEN_DRAW)
	{
		p_ActiveMarking->OnMouseMove(&dc, point, pt_Offset);
	}
	else if (e_State == STATE_RECT_DRAW)
	{
		p_ActiveMarking->OnMouseMove(&dc, point, pt_Offset);
	}
	else if (e_State == STATE_ARROW_DRAW)
	{
		p_ActiveMarking->OnMouseMove(&dc, point, pt_Offset);
	}

	CWnd::OnMouseMove(nFlags, point);
}

int LTScreenshotEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	h_Theme = OpenThemeData(m_hWnd, L"EDIT");
	pen_Line.CreatePen(PS_SOLID, 2, RGB(255,0,0));
	// TODO:  Add your specialized creation code here

	return 0;
}

void LTScreenshotEditCtrl::OnDestroy()
{
	CWnd::OnDestroy();
	CloseThemeData(h_Theme);
	pen_Line.DeleteObject();
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
	if (p_Screenshot && p_Screenshot->Width() > cx)
	{
		si.nMin = 0;
		si.nMax = p_Screenshot->Width() - 1;
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
	if (p_Screenshot && p_Screenshot->Height() > cy)
	{
		si.nMin = 0;
		si.nMax = p_Screenshot->Height() -1;
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
	SetScreenshot(LTScreenshot::Create(pBitmap));
}


void LTScreenshotEditCtrl::SetScreenshot( LTScreenshot* pScreenShot )
{
	delete p_Screenshot;
	p_Screenshot = pScreenShot;

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

	if (p_Screenshot)
	{
		pDC->BitBlt(0, 0, rRect.Width(), rRect.Height(), 
			p_Screenshot->GetDC(), pt_Offset.x, pt_Offset.y, SRCCOPY);
	}
	else
		pDC->FillSolidRect(rRect, RGB(200,200,200));
}

void LTScreenshotEditCtrl::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CWnd::OnNcPaint() for painting messages
	Default();

	CRect rWnd;
	GetWindowRect(rWnd);
	CWindowDC dc(this);
	int iXWidth = GetSystemMetrics(SM_CXBORDER);
	int iYWidth = GetSystemMetrics(SM_CYBORDER);
	COLORREF cr;
	GetThemeColor(h_Theme, EP_EDITTEXT, ETS_NORMAL, TMT_BORDERCOLOR, &cr);

	CRect rWndEx(0,0,0,0);
	rWndEx.right = rWnd.Width();
	rWndEx.bottom = rWnd.Height();

	CRect rBorder;

	rBorder = rWndEx;
	rBorder.bottom = rBorder.top + iYWidth;
	dc.FillSolidRect(rBorder, cr);

	rBorder = rWndEx;
	rBorder.top = rBorder.bottom - iYWidth;
	dc.FillSolidRect(rBorder, cr);

	rBorder = rWndEx;
	rBorder.left = rBorder.right - iXWidth;
	dc.FillSolidRect(rBorder, cr);

	rBorder = rWndEx;
	rBorder.right = rBorder.left + iXWidth;
	dc.FillSolidRect(rBorder, cr);
}

void LTScreenshotEditCtrl::PenStart()
{
	if (e_State == STATE_FREE)
		e_State = STATE_PEN_START;
	else if (e_State == STATE_PEN_START)
		e_State = STATE_FREE;
}

void LTScreenshotEditCtrl::RectStart()
{
	if (e_State == STATE_FREE)
		e_State = STATE_RECT_START;
	else if (e_State == STATE_RECT_START)
		e_State = STATE_FREE;
}

void LTScreenshotEditCtrl::ArrowStart()
{
	if (e_State == STATE_FREE)
		e_State = STATE_ARROW_START;
	else if (e_State == STATE_ARROW_START)
		e_State = STATE_FREE;
}


LTScreenshot* LTScreenshotEditCtrl::GetScreenshot()
{
	return p_Screenshot;
}

LTScreenshot* LTScreenshotEditCtrl::DetachScreenshot()
{
	LTScreenshot* pSS = p_Screenshot;
	p_Screenshot = NULL;
	return pSS;
}

void LTScreenshotEditCtrl::SetClipForSmall( CDC* pDC )
{
	CRect rClient;
	GetClientRect(rClient);
	if (p_Screenshot && (p_Screenshot->Height() < rClient.Height() ||
		p_Screenshot->Width() < rClient.Width()))
	{
		rClient.right = min(rClient.right, p_Screenshot->Width());
		rClient.bottom = min(rClient.bottom, p_Screenshot->Height());


		CRgn rgn;
		rgn.CreateRectRgnIndirect(rClient);
		pDC->SelectClipRgn(&rgn);
	}
}


