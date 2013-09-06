#include "LTPch.h"
#include "LTVirtualButton.h"

LTVirtualButton::LTVirtualButton(LTVirtualButtonOwner* pOwner)
{
	p_Owner = pOwner;
	pr_Clip = NULL;
}

LTVirtualButton::~LTVirtualButton(void)
{
	delete pr_Clip;
}

void LTVirtualButton::OnMouseMove( CPoint point, CDC* pDC )
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
		OnPaintButtonState(pDC);
		if (i_State != BTN_STATE_NORMAL)
			p_Owner->TrackLeave();
	}
}

void LTVirtualButton::OnMouseDown( CPoint point, CDC* pDC )
{
	int iOldState = i_State;

	if (r_Area.PtInRect(point))
		i_State = BTN_STATE_PRESSED;
	else
		i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
	{
		OnPaintButtonState(pDC);
		if (i_State != BTN_STATE_NORMAL)
			p_Owner->TrackLeave();
	}
}

void LTVirtualButton::OnMouseUp( CPoint point, CDC* pDC )
{
	int iOldState = i_State;

	if (r_Area.PtInRect(point))
		i_State = BTN_STATE_HOT;
	else
		i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
		OnPaintButtonState(pDC);
}

void LTVirtualButton::OnMouseLeave( CDC* pDC )
{
	int iOldState = i_State;
	i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
		OnPaintButtonState(pDC);
}

void LTVirtualButton::OnPaint( CDC* pDC )
{
	OnPaintButtonState(pDC);
}

void LTVirtualButton::OnPaintButtonState( CDC* pDC )
{
	int iC = i_State * 50;
	iC = 255 - iC;

	pDC->FillSolidRect(r_Area, RGB(iC,iC,iC));
}

void LTVirtualButton::SetClipRect( CRect rRect )
{
	delete pr_Clip;
	pr_Clip = new CRect;
	*pr_Clip = rRect;
}

LTThemeButton::LTThemeButton( LTVirtualButtonOwner* pCtrl, HTHEME* pTheme, int iPart, int StateOffset /*= 0*/ )
:LTVirtualButton(pCtrl)
{

}

LTThemeButton::~LTThemeButton()
{

}

void LTThemeButton::OnPaintButtonState( CDC* pDC )
{
	LTVirtualButton::OnPaintButtonState(pDC);
}

void LTVirtualButtonOwner::TrackLeave()
{
	if (b_LeaveTracking)
		return;

	TRACKMOUSEEVENT tTrkMouse;
	tTrkMouse.cbSize = sizeof(TRACKMOUSEEVENT);
	tTrkMouse.dwFlags = TME_LEAVE;
	tTrkMouse.hwndTrack = GetCWnd()->m_hWnd;
	TrackMouseEvent(&tTrkMouse);

	b_LeaveTracking = true;
}

void LTVirtualButtonOwner::OnMouseLeave()
{

}
