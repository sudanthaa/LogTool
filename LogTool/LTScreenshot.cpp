#include "LTPch.h"
#include "LTScreenshot.h"
#include "LTBitmapBuffer.h"
#include "LTUtils.h"

LTScreenshot::LTScreenshot(void)
{
}

LTScreenshot::~LTScreenshot(void)
{
}

CDC* LTScreenshot::GetOrigDC()
{
	return p_OriginalBitmap->GetDC();
}

CDC* LTScreenshot::GetDC()
{
	return p_Bitmap->GetDC();
}

int LTScreenshot::Width()
{
	return p_Bitmap->Width();
}

int LTScreenshot::Height()
{
	return p_Bitmap->Height();
}

void LTScreenshot::Add( LTScreenshotMarking* pMarking )
{
	a_Markings.push_back(pMarking);
}

LTScreenshot* LTScreenshot::Create( LTBitmapBuffer* pBuffer )
{
	LTScreenshot* pScreenshot = new LTScreenshot;
	pScreenshot->p_OriginalBitmap = pBuffer;
	pScreenshot->p_Bitmap = pBuffer->Clone();
	return pScreenshot;
}

void LTRectMarking::DrawRect( CDC* pDC, CRect& rRect )
{
	CRect rNormalRect = rRect;
	rNormalRect.NormalizeRect();

	Gdiplus::Graphics graphics(pDC->m_hDC);
	Gdiplus::Pen      pen(Gdiplus::Color(255, RGB_PARAMS(cr_Line)), i_Width);
	graphics.DrawRectangle(&pen, rNormalRect.left, rNormalRect.top, rNormalRect.Width(), rNormalRect.Height());
}

void LTRectMarking::OnMouseMove( CDC* pdcAct, CPoint pt, CPoint ptOffset )
{
	CRect rErase = r_Rect;
	rErase.NormalizeRect();
	rErase.InflateRect(i_Width, i_Width);

	CDC* pDC = p_Screenshot->GetDC();
	int i = pdcAct->SaveDC();

	// Erase early ones
	pdcAct->BitBlt(rErase.left, rErase.top, rErase.Width(), rErase.Height(), pDC,
		rErase.left + ptOffset.x, rErase.top + ptOffset.y, SRCCOPY);

	r_Rect.SetRect(r_Rect.TopLeft(), pt);

	DrawRect(pdcAct, r_Rect);

	pdcAct->RestoreDC(i);
	// Buffer to current dc
}

void LTRectMarking::OnMouseDown( CDC* pdcAct, CPoint pt, CPoint ptOffset )
{
	r_Rect.SetRect(pt, pt);
}

void LTRectMarking::OnMouseUp( CDC* pdcAct, CPoint pt, CPoint ptOffset )
{
	CDC* pDC = p_Screenshot->GetDC();
	int i = pDC->SaveDC();

	DrawRect(pDC, r_Rect);

	pDC->RestoreDC(i);
}

LTRectMarking::LTRectMarking(LTScreenshot* pScreenshot, COLORREF iColor /*= RGB(255,0,0)*/, int iWidth /*= 1*/ )
:LTScreenshotMarking(LT_MARKTYPE_PEN, pScreenshot)
{
	cr_Line = iColor;
	i_Width = iWidth;
}

LTRectMarking::~LTRectMarking()
{

}

void LTPenMarking::OnMouseMove( CDC* pdcAct, CPoint pt, CPoint ptOffset )
{
	CPoint ptStart = lst_Points.back();
	CPoint ptEnd = pt;
	lst_Points.push_back(pt);

	// Prepare invalidation area on client rect
	CRect rInv;
	rInv.SetRect(ptStart, ptEnd);
	rInv.NormalizeRect();
	rInv.InflateRect(i_Width, i_Width);

	// Paint on buffer
	ptStart.Offset(ptOffset);
	ptEnd.Offset(ptOffset);

	CDC* pDC = p_Screenshot->GetDC();
	int i = pDC->SaveDC();

	//GDI+ draw
	Gdiplus::Graphics graphics(pDC->m_hDC);
	Gdiplus::Pen      pen(Gdiplus::Color(255, RGB_PARAMS(cr_Line)), i_Width);
	//graphics.SetCompositingMode(Gdiplus::CompositingMode::CompositingModeSourceOver);
	graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality);
	graphics.DrawLine(&pen, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);


	pDC->RestoreDC(i);
	// Copy from buffer to client rect		
	pdcAct->BitBlt(rInv.left, rInv.top, rInv.Width(), rInv.Height(), pDC, 
		ptOffset.x + rInv.left, ptOffset.y + rInv.top, SRCCOPY);
}

void LTPenMarking::OnMouseDown( CDC* pDC, CPoint pt, CPoint ptOffset )
{
	lst_Points.push_back(pt);
}

void LTPenMarking::OnMouseUp( CDC* pDC, CPoint pt, CPoint ptOffset )
{

}

LTPenMarking::LTPenMarking(LTScreenshot* pScreenshot, COLORREF iColor /*= RGB(255,0,0)*/, int iWidth /*= 1*/ )
:LTScreenshotMarking(LT_MARKTYPE_PEN, pScreenshot)
{
	cr_Line = iColor;
	i_Width = iWidth;
}

LTPenMarking::~LTPenMarking()
{
	lst_Points.clear();
}

LTScreenshotMarking::LTScreenshotMarking( int iType, LTScreenshot* pScreenshot )
{
	p_Screenshot = pScreenshot;
	i_Type = iType;
}

LTScreenshotMarking::~LTScreenshotMarking()
{

}

LTArrowMarking::LTArrowMarking( LTScreenshot* pScreenshot, COLORREF iColor /*= RGB(255,0,0)*/, int iWidth /*= 2*/ )
:LTScreenshotMarking(LT_MARKTYPE_ARROW, pScreenshot)
{
	cr_Arrow = iColor;
	i_Width = iWidth;
}

LTArrowMarking::~LTArrowMarking()
{

}

void LTArrowMarking::OnMouseMove( CDC* pdcAct, CPoint pt, CPoint ptOffset )
{
	CRect rErase = r_Rect;
	rErase.NormalizeRect();
	rErase.InflateRect(i_Width + 3, i_Width + 3);

	CDC* pDC = p_Screenshot->GetDC();
	int i = pdcAct->SaveDC();

	// Erase early ones
	pdcAct->BitBlt(rErase.left, rErase.top, rErase.Width(), rErase.Height(), pDC,
		rErase.left + ptOffset.x, rErase.top + ptOffset.y, SRCCOPY);

	r_Rect.SetRect(r_Rect.TopLeft(), pt);
	Gdiplus::Graphics graphics(pdcAct->m_hDC);
	Gdiplus::Pen      pen(Gdiplus::Color(255, RGB_PARAMS(cr_Arrow)), i_Width);
	graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality);
	graphics.DrawLine(&pen, r_Rect.left, r_Rect.top, r_Rect.right, r_Rect.bottom);



	pdcAct->RestoreDC(i);
}

void LTArrowMarking::OnMouseDown( CDC* pDC, CPoint pt, CPoint ptOffset )
{
	r_Rect.SetRect(pt, pt);
}

void LTArrowMarking::OnMouseUp( CDC* pDC, CPoint pt, CPoint ptOffset )
{

}
