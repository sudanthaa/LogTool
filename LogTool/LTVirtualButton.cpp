#include "LTPch.h"
#include "LTVirtualButton.h"

//**************************************************************************************************
LTVirtualButton::LTVirtualButton(LTVirtualButtonOwner* pOwner)
{
	pi_BackColor = NULL;
	p_Owner = pOwner;
	pr_Clip = NULL;
	i_State = BTN_STATE_NORMAL;
}

//**************************************************************************************************
LTVirtualButton::~LTVirtualButton(void)
{
	delete pr_Clip;
	delete pi_BackColor;
}

//**************************************************************************************************
bool LTVirtualButton::OnMouseMove( CPoint point, CDC* pDC )
{
	int iOldState = i_State;

	bool bIsSelfArea = IsSelfArea(point);

	LTButtonStateEngine* pStateEng = p_Owner->GetStateEngine();
	pStateEng->OnMouseMove(this, bIsSelfArea);

	if (i_State != iOldState)
	{
		OnPaintButtonState(pDC);
		if (pStateEng->TrackRequired(i_State))
			p_Owner->TrackLeave();
	}

	return bIsSelfArea;
}

//**************************************************************************************************
bool LTVirtualButton::OnMouseDown( CPoint point, CDC* pDC )
{
	int iOldState = i_State;
	bool bIsSelfArea = IsSelfArea(point);

	LTButtonStateEngine* pStateEng = p_Owner->GetStateEngine();
	pStateEng->OnMouseDown(this, bIsSelfArea);

	if (i_State != iOldState)
	{
		OnPaintButtonState(pDC);
		if (pStateEng->TrackRequired(i_State))
			p_Owner->TrackLeave();
	}

	return bIsSelfArea;
}

//**************************************************************************************************
bool LTVirtualButton::OnMouseUp( CPoint point, CDC* pDC )
{
	int iOldState = i_State;
	bool bIsSelfArea = IsSelfArea(point);

	LTButtonStateEngine* pStateEng = p_Owner->GetStateEngine();
	pStateEng->OnMouseUp(this, bIsSelfArea);

	if (i_State != iOldState)
		OnPaintButtonState(pDC);

	return bIsSelfArea;
}

//**************************************************************************************************
void LTVirtualButton::OnMouseLeave( CDC* pDC )
{
	int iOldState = i_State;
	
	LTButtonStateEngine* pStateEng = p_Owner->GetStateEngine();
	pStateEng->OnMouseLeave(this);

	if (i_State != iOldState)
		OnPaintButtonState(pDC);
}

//**************************************************************************************************
void LTVirtualButton::OnPaint( CDC* pDC )
{
	OnPaintButtonState(pDC);
}

//**************************************************************************************************
void LTVirtualButton::OnPaintButtonState( CDC* pDC )
{
	int iC = i_State * 50;
	iC = 255 - iC;

	pDC->FillSolidRect(r_Area, RGB(iC,iC,iC));
}

//**************************************************************************************************
void LTVirtualButton::SetClipRect( CRect rRect )
{
	delete pr_Clip;
	pr_Clip = new CRect;
	*pr_Clip = rRect;
}

//**************************************************************************************************
bool LTVirtualButton::IsSelfArea( CPoint point )
{
	bool bIsClippedOut = pr_Clip ? (!(pr_Clip->PtInRect(point))) : false;
	return ((!bIsClippedOut) && r_Area.PtInRect(point));
}

//**************************************************************************************************
void LTVirtualButton::SetBackColor( int iColor )
{
	delete pi_BackColor;
	pi_BackColor = new int;
	*pi_BackColor = iColor;
}

//**************************************************************************************************
LTThemeButton::LTThemeButton( LTVirtualButtonOwner* pCtrl, HTHEME* pTheme, int iPart, int iStateOffset /*= 0*/ )
:LTVirtualButton(pCtrl)
{
	ph_Theme = pTheme;
	i_Part = iPart;
	i_StateOffset = iStateOffset;
}

//**************************************************************************************************
LTThemeButton::~LTThemeButton()
{

}

//**************************************************************************************************
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

//**************************************************************************************************
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

//**************************************************************************************************
void LTVirtualButtonOwner::MouseLeave()
{
	b_LeaveTracking = false;
}

//**************************************************************************************************
LTVirtualButtonOwner::LTVirtualButtonOwner()
{
	b_LeaveTracking = false;
	p_Pressed = NULL;
	SetType(PUSH_BUTTON);
}

//**************************************************************************************************
void LTVirtualButtonOwner::SetType( ButtonType eType )
{
	e_ButtonType = eType;
	if (eType == PUSH_BUTTON)
	{
		static LTPushButtonState oPushButtonEng;
		p_StateEngine = &oPushButtonEng;
	}
	else if (eType == CHECK_BUTTON)
	{
		static LTCheckButtonState oPushButtonEng;
		p_StateEngine = &oPushButtonEng;
	}
	else if (eType == CHECK_OPTIONAL_BUTTON)
	{
		static LTCheckOptionButtonState oPushButtonEng;
		p_StateEngine = &oPushButtonEng;
	}
}

//**************************************************************************************************
LTIconButton::LTIconButton( LTVirtualButtonOwner* pOwner, HICON hIcon[4])
:LTVirtualButton(pOwner)
{
	for (int i = 0; i < 4; i++)
		ah_Icon[i] = hIcon[i];
}

//**************************************************************************************************
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

//**************************************************************************************************
void LTPushButtonState::OnMouseMove( LTVirtualButton* pButton, bool bSelfArea )
{
	int iState = pButton->GetState();
	if (bSelfArea)
	{
		if (iState == BTN_STATE_NORMAL)
			pButton->SetState(BTN_STATE_HOT);
		else if (iState == BTN_STATE_PRESSED)
			pButton->SetState(BTN_STATE_PRESSED);
	}
	else
	{
		pButton->SetState(BTN_STATE_NORMAL);
	}
}

//**************************************************************************************************
void LTPushButtonState::OnMouseDown( LTVirtualButton* pButton, bool bSelfArea )
{
	if (bSelfArea)
		pButton->SetState(BTN_STATE_PRESSED);
	else
		pButton->SetState(BTN_STATE_NORMAL);
}

//**************************************************************************************************
void LTPushButtonState::OnMouseUp( LTVirtualButton* pButton, bool bSelfArea )
{
	int iState = pButton->GetState();

	if (bSelfArea)
	{
		if (iState == BTN_STATE_PRESSED)
			pButton->GetOwner()->OnPress(pButton);

		pButton->SetState(BTN_STATE_HOT);
	}
	else
		pButton->SetState(BTN_STATE_NORMAL);
}

//**************************************************************************************************
bool LTPushButtonState::TrackRequired( int iState )
{
	if (iState != BTN_STATE_NORMAL)
		return true;

	return false;
}

//**************************************************************************************************
void LTPushButtonState::OnMouseLeave( LTVirtualButton* pButton)
{
	pButton->SetState(BTN_STATE_NORMAL);
}

//**************************************************************************************************
void LTCheckButtonState::OnMouseMove( LTVirtualButton* pButton, bool bSelfArea )
{
	int iState = pButton->GetState();
	if (bSelfArea)
	{
		if (iState == BTN_STATE_NORMAL)
			pButton->SetState(BTN_STATE_HOT);
		else if (iState == BTN_STATE_PRESSED)
			pButton->SetState(BTN_STATE_PRESSED);
	}
	else
	{
		if (iState == BTN_STATE_HOT)
			pButton->SetState(BTN_STATE_NORMAL);
	}
}

//**************************************************************************************************
void LTCheckButtonState::OnMouseDown( LTVirtualButton* pButton, bool bSelfArea )
{
	if (bSelfArea)
	{
		pButton->SetState(BTN_STATE_PRESSED);
	}
	else
	{
		pButton->SetState(BTN_STATE_NORMAL);
	}
}

//**************************************************************************************************
void LTCheckButtonState::OnMouseUp( LTVirtualButton* pButton, bool bSelfArea )
{
	int iState = pButton->GetState();

	if (bSelfArea)
	{
		if (iState == BTN_STATE_PRESSED)
		{
			LTVirtualButtonOwner* pOwener = pButton->GetOwner();
			LTVirtualButton* pPressed = pOwener->GetPressed();
			if (pPressed)
				pOwener->OnRelease(pPressed);

			pOwener->OnPress(pButton);
			pOwener->SetPressed(pButton);
		}

		pButton->SetState(BTN_STATE_PRESSED);
	}
	else
		pButton->SetState(BTN_STATE_NORMAL);

}

//**************************************************************************************************
bool LTCheckButtonState::TrackRequired( int iState )
{
	if (iState != BTN_STATE_NORMAL)
		return true;

	return false;
}

//**************************************************************************************************
void LTCheckButtonState::OnMouseLeave( LTVirtualButton* pButton )
{

}

//**************************************************************************************************
void LTCheckOptionButtonState::OnMouseMove( LTVirtualButton* pButton, bool bSelfArea )
{
	int iState = pButton->GetState();
	if (bSelfArea)
	{
		if (iState == BTN_STATE_NORMAL)
			pButton->SetState(BTN_STATE_HOT);
		else if (iState == BTN_STATE_PRESSED)
			pButton->SetState(BTN_STATE_PRESSED);
	}
	else
	{
		if (iState == BTN_STATE_HOT)
			pButton->SetState(BTN_STATE_NORMAL);
	}
}

//**************************************************************************************************
void LTCheckOptionButtonState::OnMouseDown( LTVirtualButton* pButton, bool bSelfArea )
{
	int iState = pButton->GetState();

	if (bSelfArea)
	{
		if (iState == BTN_STATE_HOT)
			pButton->SetState(BTN_STATE_PRESSED);
	}
	else
	{
		pButton->SetState(BTN_STATE_NORMAL);
	}
}

//**************************************************************************************************
void LTCheckOptionButtonState::OnMouseUp( LTVirtualButton* pButton, bool bSelfArea )
{
	int iState = pButton->GetState();

	if (bSelfArea)
	{
		if (iState == BTN_STATE_PRESSED)
		{
			LTVirtualButtonOwner* pOwener = pButton->GetOwner();
			LTVirtualButton* pPressed = pOwener->GetPressed();
			if (pPressed)
			{
				if (pPressed == pButton)
				{
					pOwener->OnRelease(pButton);
					pOwener->SetPressed(NULL);
					pButton->SetState(BTN_STATE_HOT);
				}
				else 
				{
					pOwener->OnRelease(pPressed);
					pOwener->OnPress(pButton);
					pOwener->SetPressed(pButton);
				}
			}
			else
			{
				pOwener->OnPress(pButton);
				pOwener->SetPressed(pButton);
			}
		}
	}
	else
		pButton->SetState(BTN_STATE_NORMAL);
}

//**************************************************************************************************
bool LTCheckOptionButtonState::TrackRequired( int iState )
{
	if (iState != BTN_STATE_NORMAL)
		return true;

	return false;
}

//**************************************************************************************************
void LTCheckOptionButtonState::OnMouseLeave( LTVirtualButton* pButton)
{
	//LTVirtualButtonOwner* pOwner = pButton->GetOwner();
	//if (pButton != pOwner->GetPressed())
	//	pButton->SetState(BTN_STATE_NORMAL);

	int iState = pButton->GetState();

	if (iState == BTN_STATE_PRESSED)
	{
		//LTVirtualButtonOwner* pOwener = pButton->GetOwner();
		//LTVirtualButton* pPressed = pOwener->GetPressed();
		//if (pPressed)
		//{
		//	if (pPressed == pButton)
		//	{
		//		pOwener->OnRelease(pButton);
		//		pOwener->SetPressed(NULL);
		//		pButton->SetState(BTN_STATE_NORMAL);
		//	}
		//	else 
		//	{
		//		pOwener->OnRelease(pPressed);
		//		pOwener->OnPress(pButton);
		//		pOwener->SetPressed(pButton);
		//	}
		//}
		//else
		//{
		//	pOwener->OnPress(pButton);
		//	pOwener->SetPressed(pButton);
		//}
	}
	else if (iState == BTN_STATE_HOT)
	{
		pButton->SetState(BTN_STATE_NORMAL);
	}
}

//**************************************************************************************************
LTPaintFnButton::LTPaintFnButton( LTVirtualButtonOwner* pOwner, PainterFn pPainter, void* pContext)
:LTVirtualButton(pOwner)
{
	p_Painter = pPainter;
	p_Context = pContext; 
}

//**************************************************************************************************
void LTPaintFnButton::OnPaintButtonState( CDC* pDC )
{
	p_Painter(i_State, pDC, r_Area, this, p_Context);
}

//**************************************************************************************************

