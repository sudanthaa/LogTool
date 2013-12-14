#include "LTPch.h"
#include "LTVirtualButton.h"

LTVirtualButton::LTVirtualButton(LTVirtualButtonOwner* pOwner)
{
	pi_BackColor = NULL;
	p_Owner = pOwner;
	pr_Clip = NULL;
	i_State = BTN_STATE_NORMAL;
}

LTVirtualButton::~LTVirtualButton(void)
{
	delete pr_Clip;
	delete pi_BackColor;
}

bool LTVirtualButton::OnMouseMove( CPoint point, CDC* pDC )
{
	int iOldState = i_State;

	bool bOwnedArea = IsOwnedArea(point);

	if (bOwnedArea)
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

	return bOwnedArea;
}

bool LTVirtualButton::OnMouseDown( CPoint point, CDC* pDC )
{
	int iOldState = i_State;

	bool bOwnedArea = IsOwnedArea(point);

	if (bOwnedArea)
		i_State = BTN_STATE_PRESSED;
	else
		i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
	{
		OnPaintButtonState(pDC);
		if (i_State != BTN_STATE_NORMAL)
			p_Owner->TrackLeave();
	}

	return bOwnedArea;
}

bool LTVirtualButton::OnMouseUp( CPoint point, CDC* pDC )
{
	int iOldState = i_State;

	bool bOwnedArea = IsOwnedArea(point);

	if (bOwnedArea)
	{
		if (i_State == BTN_STATE_PRESSED)
		{
			p_Owner->OnPress(this);
			//p_Owner->GetCWnd()->PostMessage();
		}

		i_State = BTN_STATE_HOT;
	}
	else
		i_State = BTN_STATE_NORMAL;

	if (i_State != iOldState)
		OnPaintButtonState(pDC);

	return bOwnedArea;
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

bool LTVirtualButton::IsOwnedArea( CPoint point )
{
	bool bIsClippedOut = pr_Clip ? (!(pr_Clip->PtInRect(point))) : false;
	return ((!bIsClippedOut) && r_Area.PtInRect(point));
}

void LTVirtualButton::SetBackColor( int iColor )
{
	delete pi_BackColor;
	pi_BackColor = new int;
	*pi_BackColor = iColor;
}


LTThemeButton::LTThemeButton( LTVirtualButtonOwner* pCtrl, HTHEME* pTheme, int iPart, int iStateOffset /*= 0*/ )
:LTVirtualButton(pCtrl)
{
	ph_Theme = pTheme;
	i_Part = iPart;
	i_StateOffset = iStateOffset;
}

LTThemeButton::~LTThemeButton()
{

}

void LTThemeButton::OnPaintButtonState( CDC* pDC )
{
	if (!p_Owner->PaintBack(this, pDC, r_Area))
	{
		if (pi_BackColor)
			pDC->FillSolidRect(r_Area, *pi_BackColor);
	}

	DrawThemeBackground(*ph_Theme, pDC->m_hDC, i_Part, i_State + i_StateOffset, r_Area, *pr_Clip);
	//LTVirtualButton::OnPaintButtonState(pDC);
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
	b_LeaveTracking = false;
}

LTVirtualButtonOwner::LTVirtualButtonOwner()
{
	b_LeaveTracking = false;
	p_Pressed = NULL;
	e_ButtonType = PUSH_BUTTON;
}

LTIconButton::LTIconButton( LTVirtualButtonOwner* pOwner, HICON hIcon[4])
:LTVirtualButton(pOwner)
{
	for (int i = 0; i < 4; i++)
		ah_Icon[i] = hIcon[i];
}

void LTIconButton::OnPaintButtonState( CDC* pDC )
{
	if (pr_Clip)
	{
		CRgn rgn;
		rgn.CreateRectRgn(pr_Clip->left, pr_Clip->top, pr_Clip->right, pr_Clip->bottom);
		pDC->SelectClipRgn(&rgn);
	}

	if (!p_Owner->PaintBack(this, pDC, r_Area))
	{
		if (pi_BackColor)
			pDC->FillSolidRect(r_Area, *pi_BackColor);
	}

	pDC->DrawIcon(r_Area.left, r_Area.top, ah_Icon[i_State - 1]);
}
