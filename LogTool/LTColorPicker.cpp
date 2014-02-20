// LTColorPicker.cpp : implementation file
//

#include "LTPch.h"
#include "LTColorPicker.h"


// LTColorPicker

IMPLEMENT_DYNAMIC(LTColorPicker, CWnd)

LTColorPicker::LTColorPicker()
{
	cr_Val = RGB(255,0,0);
	p_CB = NULL;
}

LTColorPicker::~LTColorPicker()
{
}


BEGIN_MESSAGE_MAP(LTColorPicker, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// LTColorPicker message handlers



void LTColorPicker::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	CRect rClient;
	GetClientRect(rClient);

	dc.FillSolidRect(rClient, cr_Val);
}

void LTColorPicker::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CColorDialog oDlg(cr_Val, CC_FULLOPEN, NULL);
	if (oDlg.DoModal() == IDOK)
	{
		cr_Val = oDlg.GetColor();
		Invalidate();

		if (p_CB)
			p_CB->OnColorChange(cr_Val);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void LTColorPicker::SetColor( COLORREF cr )
{
	cr_Val = cr;
	Invalidate();
}
