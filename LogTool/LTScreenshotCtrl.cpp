// LTScreenshotCtrl.cpp : implementation file
//

#include "LTPch.h"
#include "LTScreenshotCtrl.h"


#define  SIDE_BUTTON_WIDTH   15

#define  BTN_STATE_NORMAL	1
#define  BTN_STATE_HOT	2
#define  BTN_STATE_PRESSED	3
#define  BTN_STATE_DISABLED	4

// LTScreenshotCtrl

IMPLEMENT_DYNAMIC(LTScreenshotCtrl, CWnd)

LTScreenshotCtrl::LTScreenshotCtrl()
{
	p_PrevButton = NULL;
	p_NextButton = NULL;
	b_LeaveTracking = false;
}

LTScreenshotCtrl::~LTScreenshotCtrl()
{
	delete p_PrevButton;
	delete p_NextButton;
	CloseThemeData(h_thmEdit);
	CloseThemeData(h_thmArrow);
}


BEGIN_MESSAGE_MAP(LTScreenshotCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL LTScreenshotCtrl::CreateScreenshotCtrl( CWnd* pParent, CRect rArea, int iID )
{
	p_PrevButton = new ArrowButton(this, &h_thmArrow, SPNP_DOWNHORZ);
	p_NextButton = new ArrowButton(this, &h_thmArrow, SPNP_UPHORZ);
	//p_PrevButton = new ArrowButton(this, &h_thmArrow, SBP_ARROWBTN, 8);
	//p_NextButton = new ArrowButton(this, &h_thmArrow, SBP_ARROWBTN, 12);

	Layout(rArea);
	return Create(NULL, NULL, WS_VISIBLE | WS_CHILD /*| WS_BORDER*/, rArea, pParent, iID);
}



// LTScreenshotCtrl message handlers



void LTScreenshotCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	CRect rClient;
	GetClientRect(rClient);

	DrawThemeParentBackground(m_hWnd, dc.m_hDC, rClient);
	DrawThemeBackground(h_thmEdit, dc.m_hDC, EP_EDITBORDER_NOSCROLL, EPSN_NORMAL, r_Main, NULL);

	p_NextButton->OnPaint(&dc);
	p_PrevButton->OnPaint(&dc);
}

void LTScreenshotCtrl::Layout( CRect rClient )
{
	r_Main = rClient;
	CRect rPrevButton = rClient;
	CRect rNextButton = rClient;

	r_Main.left += SIDE_BUTTON_WIDTH;
	r_Main.right -= SIDE_BUTTON_WIDTH;
	rPrevButton.right = r_Main.left - 1;
	rNextButton.left = r_Main.right + 1;

	rNextButton.bottom--;
	rPrevButton.bottom--;

	p_NextButton->SetRect(rNextButton);
	p_PrevButton->SetRect(rPrevButton);
}

void LTScreenshotCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rRect(0, 0, cx, cy);
	Layout(rRect);
	// TODO: Add your message handler code here
}

int LTScreenshotCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	h_thmEdit = OpenThemeData(m_hWnd, L"EDIT");
	h_thmArrow = OpenThemeData(m_hWnd, L"SPIN");
	//h_thmArrow = OpenThemeData(m_hWnd, L"SCROLLBAR");

	p_NextButton->SetTheme(&h_thmArrow);
	p_PrevButton->SetTheme(&h_thmArrow);
	// TODO:  Add your specialized creation code here

	return 0;
}



//void LTScreenshotCtrl::OnNcPaint()
//{
//	// TODO: Add your message handler code here
//	// Do not call CWnd::OnNcPaint() for painting messages
//}

//void LTScreenshotCtrl::OnNcPaint()
//{
//	// TODO: Add your message handler code here
//	// Do not call CWnd::OnNcPaint() for painting messages
//
//	Default();
//	CWindowDC dc(this);
//	CRect invRect;
//	dc.GetClipBox (&invRect);
//	//dc.FillSolidRect (&invRect,RGB (248,2,25));
//
//	if (IsThemeBackgroundPartiallyTransparent(h_thmEdit, EP_EDITBORDER_NOSCROLL, EPSN_NORMAL))
//		DrawThemeParentBackground(m_hWnd, dc.m_hDC, invRect);
//	DrawThemeBackground(h_thmEdit, dc.m_hDC, EP_EDITBORDER_NOSCROLL, EPSN_NORMAL, invRect, NULL);
//}

void LTScreenshotCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);

	p_NextButton->OnMouseMove(point, &dc);
	p_PrevButton->OnMouseMove(point, &dc);

	CWnd::OnMouseMove(nFlags, point);
}

void LTScreenshotCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	p_NextButton->OnMouseDown(point, &dc);
	p_PrevButton->OnMouseDown(point, &dc);

	CWnd::OnLButtonDown(nFlags, point);
}

void LTScreenshotCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	p_NextButton->OnMouseUp(point, &dc);
	p_PrevButton->OnMouseUp(point, &dc);

	CWnd::OnLButtonUp(nFlags, point);
}


void LTScreenshotCtrl::ArrowButton::OnMouseMove( CPoint point, CDC* pDC)
{
	int iOldState = i_State;

	if (r_Area.PtInRect(point))
	{
		if (i_State == BTN_STATE_NORMAL)
			i_State = BTN_STATE_HOT;
		else if (i_State == BTN_STATE_PRESSED)
			i_State = BTN_STATE_PRESSED; // No change
	}
	else
	{
		i_State = BTN_STATE_NORMAL;
	}

	if (i_State != iOldState)
	{
		DrawThemeBackground(*ph_Theme, pDC->m_hDC, i_Part, i_State + i_StateOffset, r_Area, NULL);
		if (i_State != BTN_STATE_NORMAL)
			p_Ctrl->TrackMouseLeave();
	}
}

void LTScreenshotCtrl::ArrowButton::OnMouseDown( CPoint point, CDC* pDC)
{
	int iOldState = i_State;

	if (r_Area.PtInRect(point))
		i_State = BTN_STATE_PRESSED;
	else
		i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
	{
		DrawThemeBackground(*ph_Theme, pDC->m_hDC, i_Part, i_State + i_StateOffset, r_Area, NULL);
		if (i_State != BTN_STATE_NORMAL)
			p_Ctrl->TrackMouseLeave();
	}
}

void LTScreenshotCtrl::ArrowButton::OnMouseUp( CPoint point, CDC* pDC)
{
	int iOldState = i_State;

	if (r_Area.PtInRect(point))
		i_State = BTN_STATE_HOT;
	else
		i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
		DrawThemeBackground(*ph_Theme, pDC->m_hDC, i_Part, i_State + i_StateOffset, r_Area, NULL);
}

void LTScreenshotCtrl::ArrowButton::OnMouseLeave( CDC* pDC )
{
	int iOldState = i_State;
	i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
		DrawThemeBackground(*ph_Theme, pDC->m_hDC, i_Part, i_State + i_StateOffset, r_Area, NULL);
}


void LTScreenshotCtrl::ArrowButton::OnPaint( CDC* pDC )
{
	DrawThemeBackground(*ph_Theme, pDC->m_hDC, i_Part, i_State + i_StateOffset, r_Area, NULL);
}


LTScreenshotCtrl::ArrowButton::ArrowButton(LTScreenshotCtrl* pCtrl, HTHEME* pTheme, int iPart, int iStateOffset)
{
	i_State = BTN_STATE_NORMAL;
	p_Ctrl = pCtrl;
	ph_Theme = pTheme;
	i_Part = iPart;
	i_StateOffset = iStateOffset;
}

LTScreenshotCtrl::ArrowButton::~ArrowButton()
{

}



void LTScreenshotCtrl::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	b_LeaveTracking = false;

	CClientDC dc(this);
	p_NextButton->OnMouseLeave(&dc);
	p_PrevButton->OnMouseLeave(&dc);

	CWnd::OnMouseLeave();
}

bool LTScreenshotCtrl::TrackMouseLeave()
{
	if (b_LeaveTracking)
		return false;

	TRACKMOUSEEVENT tTrkMouse;
	tTrkMouse.cbSize = sizeof(TRACKMOUSEEVENT);
	tTrkMouse.dwFlags = TME_LEAVE;
	tTrkMouse.hwndTrack = m_hWnd;
	TrackMouseEvent(&tTrkMouse);

	b_LeaveTracking = true;

	return false;
}

