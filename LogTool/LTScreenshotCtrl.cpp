// LTScreenshotCtrl.cpp : implementation file
//

#include "LTPch.h"
#include "LTScreenshotCtrl.h"


// LTScreenshotCtrl

IMPLEMENT_DYNAMIC(LTScreenshotCtrl, CWnd)

LTScreenshotCtrl::LTScreenshotCtrl()
{

}

LTScreenshotCtrl::~LTScreenshotCtrl()
{
}


BEGIN_MESSAGE_MAP(LTScreenshotCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
//	ON_WM_NCPAINT()
END_MESSAGE_MAP()

BOOL LTScreenshotCtrl::CreateScreenshotCtrl( CWnd* pParent, CRect rArea, int iID )
{
	return Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, rArea, pParent, iID);
}



// LTScreenshotCtrl message handlers



void LTScreenshotCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	CRect rClient;
	GetClientRect(rClient);

	dc.FillSolidRect(rClient, RGB(255,255,255));
}

void LTScreenshotCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

int LTScreenshotCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

//void LTScreenshotCtrl::OnNcPaint()
//{
//	// TODO: Add your message handler code here
//	// Do not call CWnd::OnNcPaint() for painting messages
//}
