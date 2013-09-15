// LTScreenshotCtrl.cpp : implementation file
//

#include "LTPch.h"
#include "LTThumbnailsCtrl.h"

#include <resource.h>


#define  SIDE_BUTTON_WIDTH   16

#define  BTN_STATE_NORMAL	1
#define  BTN_STATE_HOT	2
#define  BTN_STATE_PRESSED	3
#define  BTN_STATE_DISABLED	4

#define  CLOSE_BTN_WIDTH	11
#define  CLOSE_BTN_HEIGHT	11
#define  BUTTON_PADDING		2
#define  THUMBNAIL_GAP		2
#define  THUMBNAIL_CONTAINER_PADDING	3
#define  THUMBNAIL_TILE_HEIGHT_TO_WIDTH_RATIO	1.5

// LTScreenshotCtrl

IMPLEMENT_DYNAMIC(LTThumbnailsCtrl, CWnd)

LTThumbnailsCtrl::LTThumbnailsCtrl()
{
	p_PrevButton = NULL;
	p_NextButton = NULL;
	b_LeaveTracking = false;
	i_SreenshotOffset = 0;
}

LTThumbnailsCtrl::~LTThumbnailsCtrl()
{
	delete p_PrevButton;
	delete p_NextButton;
	CloseThemeData(h_thmEdit);
	CloseThemeData(h_thmArrow);
	CloseThemeData(h_thmWindow);
}


BEGIN_MESSAGE_MAP(LTThumbnailsCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL LTThumbnailsCtrl::CreateScreenshotCtrl( CWnd* pParent, CRect rArea, int iID )
{
	return Create(NULL, NULL, WS_VISIBLE | WS_CHILD /*| WS_BORDER*/, rArea, pParent, iID);
}

// LTScreenshotCtrl message handlers



void LTThumbnailsCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	PaintCtrl(&dc);
}

void LTThumbnailsCtrl::PaintCtrl( CDC* pDC )
{
	CRect rClient;
	GetClientRect(rClient);

	DrawThemeParentBackground(m_hWnd, pDC->m_hDC, rClient);
	DrawThemeBackground(h_thmEdit, pDC->m_hDC, EP_EDITBORDER_NOSCROLL, EPSN_NORMAL, r_Main, NULL);

	p_NextButton->OnPaint(pDC);
	p_PrevButton->OnPaint(pDC);

	for (UINT i = 0; i < a_ScreenShots.size(); i++)
	{
		Screenshot* pScreenShot = a_ScreenShots[i];
		pScreenShot->OnPaint(pDC);
	}
}

void LTThumbnailsCtrl::Layout( CRect rClient )
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

	for (int i = 0; i < (int)a_ScreenShots.size(); i++)
	{
		Screenshot* pScreenShot = a_ScreenShots[i];
		pScreenShot->Layout(r_Main, i + i_SreenshotOffset);
	}
}

void LTThumbnailsCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rRect(0, 0, cx, cy);
	Layout(rRect);
	// TODO: Add your message handler code here
}

int LTThumbnailsCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	h_thmEdit = OpenThemeData(m_hWnd, L"EDIT");;
	h_thmArrow = OpenThemeData(m_hWnd, L"SCROLLBAR");
	h_thmWindow = OpenThemeData(m_hWnd, L"WINDOW");

	p_PrevButton = new LTThemeButton(this, &h_thmArrow, SBP_ARROWBTN, 8);
	p_NextButton = new LTThemeButton(this, &h_thmArrow, SBP_ARROWBTN, 12);

	a_hCloseIcons[0] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_CLOSE_NORMAL);
	a_hCloseIcons[1] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_CLOSE_HOT);
	a_hCloseIcons[2] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_CLOSE_PRESSED);
	a_hCloseIcons[3] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_CLOSE_NORMAL);
	a_hEditIcons[0] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_EDIT_NORMAL);
	a_hEditIcons[1] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_EDIT_HOT);
	a_hEditIcons[2] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_EDIT_PRESSED);
	a_hEditIcons[3] = AfxGetApp()->LoadIcon(IDI_ICON_SMALL_EDIT_NORMAL);

// 	Screenshot* pSS = new Screenshot(this, ahCloseIcons, ahEditIcons);
// 	pSS->SetName("Cat");
// 	a_ScreenShots.push_back(pSS);
// 
// 	pSS = new Screenshot(this, ahCloseIcons, ahEditIcons);
// 	pSS->SetName("Mouse");
// 	a_ScreenShots.push_back(pSS);
// 
// 	pSS = new Screenshot(this, ahCloseIcons, ahEditIcons);
// 	pSS->SetName("Gecko");
// 	a_ScreenShots.push_back(pSS);
// 
// 	pSS = new Screenshot(this, ahCloseIcons, ahEditIcons);
// 	pSS->SetName("MarketReplayServer");
// 	a_ScreenShots.push_back(pSS);

	// TODO:  Add your specialized creation code here

	return 0;
}

void LTThumbnailsCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);

	p_NextButton->OnMouseMove(point, &dc);
	p_PrevButton->OnMouseMove(point, &dc);

	if (r_Main.PtInRect(point))
	{
		for (UINT i = 0; i < a_ScreenShots.size(); i++)
		{
			Screenshot* pScreenShot = a_ScreenShots[i];
			pScreenShot->OnMouseMove(point, &dc);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void LTThumbnailsCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	p_NextButton->OnMouseDown(point, &dc);
	p_PrevButton->OnMouseDown(point, &dc);

	if (r_Main.PtInRect(point))
	{
		for (UINT i = 0; i < a_ScreenShots.size(); i++)
		{
			Screenshot* pScreenShot = a_ScreenShots[i];
			pScreenShot->OnMouseDown(point, &dc);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void LTThumbnailsCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	p_NextButton->OnMouseUp(point, &dc);
	p_PrevButton->OnMouseUp(point, &dc);

	if (r_Main.PtInRect(point))
	{
		for (UINT i = 0; i < a_ScreenShots.size(); i++)
		{
			Screenshot* pScreenShot = a_ScreenShots[i];
			pScreenShot->OnMouseUp(point, &dc);
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void LTThumbnailsCtrl::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	LTVirtualButtonOwner::OnMouseLeave();

	CClientDC dc(this);
	p_NextButton->OnMouseLeave(&dc);
	p_PrevButton->OnMouseLeave(&dc);

	CWnd::OnMouseLeave();
}

CWnd* LTThumbnailsCtrl::GetCWnd()
{
	return this;
}

void LTThumbnailsCtrl::OnPress( LTVirtualButton* pButton )
{
	if (pButton == p_NextButton)
	{
		bool bValidMove = false;
		for (int i = 0; i < (int)a_ScreenShots.size(); i++)
		{
			Screenshot* pScreeshot = a_ScreenShots[i];
			int iRightEdge = pScreeshot->GetRightEdge();
			if (iRightEdge > r_Main.right)
				bValidMove = true;
		}

		if (bValidMove)
		{
			i_SreenshotOffset--;
			CRect rClient;
			GetClientRect(rClient);
			Layout(rClient);

			CClientDC dc(this);
			PaintCtrl(&dc);
		}
	}
	else if (pButton == p_PrevButton)
	{
		bool bValidMove = false;
		for (int ui = 0; ui < (int)a_ScreenShots.size(); ui++)
		{
			Screenshot* pScreeshot = a_ScreenShots[ui];
			int iLeftEdge = pScreeshot->GetLeftEdge();
			if (iLeftEdge < r_Main.left)
				bValidMove = true;
		}

		if (bValidMove)
		{
			i_SreenshotOffset++;
			CRect rClient;
			GetClientRect(rClient);
			Layout(rClient);

			CClientDC dc(this);
			PaintCtrl(&dc);
		}
	}
}

void LTThumbnailsCtrl::AddScreenshot( LTScreenshot* pScreenshot )
{
	Screenshot* pSS = new Screenshot(this, a_hCloseIcons, a_hEditIcons, pScreenshot);
	// 	pSS->SetName("Cat");
	a_ScreenShots.push_back(pSS);

	CRect rClient;
	GetClientRect(rClient);
	Layout(rClient);
	Invalidate();
}



LTThumbnailsCtrl::Screenshot::Screenshot( LTThumbnailsCtrl* pCtrl, HTHEME* pBtnTheme, LTScreenshot* pScreenshot)
{
	p_Screenshot = pScreenshot;
	i_BackColor =  RGB(230,230,240);
	p_Ctrl = pCtrl;
	p_CloseButton = new LTThemeButton(pCtrl, pBtnTheme, WP_SMALLCLOSEBUTTON, 0);
	p_EditButton = new LTThemeButton(pCtrl, pBtnTheme, WP_MAXBUTTON, 0);

	p_CloseButton->SetBackColor(i_BackColor);
	p_EditButton->SetBackColor(i_BackColor);
}

LTThumbnailsCtrl::Screenshot::Screenshot(LTThumbnailsCtrl* pCtrl, HICON ahCloseBtnIcon[4], HICON ahEditBtnIcon[4], LTScreenshot* pScreenshot)
{
	p_Screenshot = pScreenshot;
	i_BackColor =  RGB(230,230,240);
	p_Ctrl = pCtrl;
	p_CloseButton = new LTIconButton(pCtrl, ahCloseBtnIcon);
	p_EditButton = new LTIconButton(pCtrl, ahEditBtnIcon);

	p_CloseButton->SetBackColor(i_BackColor);
	p_EditButton->SetBackColor(i_BackColor);
}

LTThumbnailsCtrl::Screenshot::~Screenshot()
{
	delete p_CloseButton;
	delete p_EditButton;
}

void LTThumbnailsCtrl::Screenshot::Layout( CRect rContainer, int iIndex)
{
	r_Rect = rContainer;
	r_Rect.top += THUMBNAIL_CONTAINER_PADDING;
	r_Rect.left += THUMBNAIL_CONTAINER_PADDING;
	r_Rect.bottom -= THUMBNAIL_CONTAINER_PADDING;
	r_Rect.right -= THUMBNAIL_CONTAINER_PADDING;

	r_Clip = r_Rect;

	int iWidth = (r_Rect.Height() * THUMBNAIL_TILE_HEIGHT_TO_WIDTH_RATIO);
	
	r_Rect.left += iIndex * (iWidth + THUMBNAIL_GAP);
	r_Rect.right = r_Rect.left + iWidth;

	CRect rClose;
	CRect rEdit;

	rClose.right = r_Rect.right - BUTTON_PADDING;
	rClose.top = r_Rect.top + BUTTON_PADDING;
	rClose.left = rClose.right - CLOSE_BTN_WIDTH;
	rClose.bottom = rClose.top + CLOSE_BTN_HEIGHT;

	rEdit = rClose;
	rEdit.top += CLOSE_BTN_HEIGHT + BUTTON_PADDING;
	rEdit.bottom += CLOSE_BTN_HEIGHT + BUTTON_PADDING;

	p_EditButton->SetRect(rEdit);
	p_CloseButton->SetRect(rClose);

	p_EditButton->SetClipRect(r_Clip);
	p_CloseButton->SetClipRect(r_Clip);
}

void LTThumbnailsCtrl::Screenshot::OnMouseMove( CPoint point, CDC* pDC )
{
	p_EditButton->OnMouseMove(point, pDC);
	p_CloseButton->OnMouseMove(point, pDC);
}

void LTThumbnailsCtrl::Screenshot::OnMouseDown( CPoint point, CDC* pDC )
{
	p_EditButton->OnMouseDown(point, pDC);
	p_CloseButton->OnMouseDown(point, pDC);
}

void LTThumbnailsCtrl::Screenshot::OnMouseUp( CPoint point, CDC* pDC )
{
	p_EditButton->OnMouseUp(point, pDC);
	p_CloseButton->OnMouseUp(point, pDC);
}

void LTThumbnailsCtrl::Screenshot::OnMouseLeave( CDC* pDC )
{
	p_EditButton->OnMouseLeave(pDC);
	p_CloseButton->OnMouseLeave(pDC);
}
#define  TEXT_PADDING  2

void LTThumbnailsCtrl::Screenshot::OnPaint( CDC* pDC )
{
	CRgn rgn;
	rgn.CreateRectRgn(r_Clip.left, r_Clip.top, r_Clip.right, r_Clip.bottom);
	pDC->SelectClipRgn(&rgn);
	 
	pDC->FillSolidRect(r_Rect, i_BackColor);

	// Printing screen-shot name
	CRect rText = r_Rect;
	rText.right -= TEXT_PADDING;
	CFont* pOld = pDC->SelectObject(p_Ctrl->GetParent()->GetFont());
	CSize szText = pDC->GetTextExtent(GetName());
	pDC->ExtTextOut(r_Rect.left + TEXT_PADDING, r_Rect.bottom - szText.cy - TEXT_PADDING, ETO_CLIPPED, 
			&rText, GetName(), strlen(GetName()), NULL );
	pDC->SelectObject(pOld);

	p_EditButton->OnPaint( pDC);
	p_CloseButton->OnPaint( pDC);
}

void LTThumbnailsCtrl::Screenshot::SetName( const char* zName )
{
	s_Name = zName;
}

const char* LTThumbnailsCtrl::Screenshot::GetName()
{
	return s_Name;
}

