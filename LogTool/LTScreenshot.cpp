#include "LTPch.h"
#include "LTScreenshot.h"
#include "LTBitmapBuffer.h"
#include "LTUtils.h"

#include <math.h>

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

wchar_t* AnsiCharToWChar(const char* zText)
{
	size_t newsize = strlen(zText) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, zText, _TRUNCATE);	
	return wcstring;
}

int GetEncoderClsid(const char* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	wchar_t* wcstring = AnsiCharToWChar(format);

	for(UINT j = 0; j < num; ++j)
	{
		size_t convertedChars = 0;


		if (wcscmp(pImageCodecInfo[j].MimeType, wcstring) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	delete [] wcstring;

	free(pImageCodecInfo);
	return -1;  // Failure
}

bool LTScreenshot::Save( const char* zFileName, int iQuality)
{
	CLSID             encoderClsid;
	Gdiplus::EncoderParameters encoderParameters;
	ULONG             quality;
	Gdiplus::Status            stat;

	// Get an image from the disk.
	//Gdiplus::Image* image = new Gdiplus::Image(L"Shapes.bmp");

	Gdiplus::Bitmap image(*(GetBuffer()->GetBitmap()), NULL);

	// Get the CLSID of the JPEG encoder.
	GetEncoderClsid("image/jpeg", &encoderClsid);

	// Before we call Image::Save, we must initialize an
	// EncoderParameters object. The EncoderParameters object
	// has an array of EncoderParameter objects. In this
	// case, there is only one EncoderParameter object in the array.
	// The one EncoderParameter object has an array of values.
	// In this case, there is only one value (of type ULONG)
	// in the array. We will let this value vary from 0 to 100.

	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	wchar_t* filename = AnsiCharToWChar(zFileName);

	// Save the image as a JPEG with quality level 0.
	quality = iQuality;
	encoderParameters.Parameter[0].Value = &quality;
	stat = image.Save(filename, &encoderClsid, &encoderParameters);
	delete [] filename;

	return (stat == Gdiplus::Ok);
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

	CRect rRect = r_Rect;
	rRect.OffsetRect(ptOffset);
	DrawRect(pDC, rRect);

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

#define  MATH_PI 3.14159

void LTArrowMarking::OnMouseMove( CDC* pdcAct, CPoint pt, CPoint ptOffset )
{
	CRect rErase = r_Rect;
	rErase.NormalizeRect();
	rErase.InflateRect(i_Width * 3, i_Width * 3);

	CDC* pDC = p_Screenshot->GetDC();
	int i = pdcAct->SaveDC();

	// Erase early ones
	pdcAct->BitBlt(rErase.left, rErase.top, rErase.Width(), rErase.Height(), pDC,
		rErase.left + ptOffset.x, rErase.top + ptOffset.y, SRCCOPY);

	r_Rect.SetRect(r_Rect.TopLeft(), pt);

	DrawArrow(pdcAct, r_Rect);

	pdcAct->RestoreDC(i);
}

void LTArrowMarking::OnMouseDown( CDC* pDC, CPoint pt, CPoint ptOffset )
{
	r_Rect.SetRect(pt, pt);
}

void LTArrowMarking::OnMouseUp( CDC* pdcAct, CPoint pt, CPoint ptOffset )
{
	CDC* pDC = p_Screenshot->GetDC();

	CRect rRect = r_Rect;
	rRect.OffsetRect(ptOffset);
	DrawArrow(pDC, rRect);
}

void LTArrowMarking::DrawArrow( CDC* pdcAct, CRect& rRect)
{
	double dAngle = atan2((float)(rRect.top - rRect.bottom), (float)(rRect.left - rRect.right));
	float fRadiusLineEnd = i_Width * 2.0; 
		// use this radius to stop line from reaching the end. if it reaches the end the point will get jeopardized 
	float fRadiusArrowEdge = i_Width * 4.0;
	float fAnLeft = dAngle + (MATH_PI / 5.0);
	float fAnRight = dAngle - (MATH_PI / 5.0);
	float fAnEdge = dAngle + (MATH_PI);

	Gdiplus::PointF pointA(rRect.left, rRect.top);
	Gdiplus::PointF pointB(((float)rRect.right) + (cos(dAngle) * fRadiusLineEnd), 
		((float)rRect.bottom) + (sin(dAngle) * fRadiusLineEnd));

	Gdiplus::Graphics graphics(pdcAct->m_hDC);
	Gdiplus::Pen      pen(Gdiplus::Color(255, RGB_PARAMS(cr_Arrow)), i_Width);
	graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality);
	graphics.DrawLine(&pen, pointA, pointB);

	Gdiplus::SolidBrush brush(Gdiplus::Color(255, RGB_PARAMS(cr_Arrow)));
	Gdiplus::PointF point1(rRect.right + (cos(fAnLeft) * fRadiusArrowEdge), rRect.bottom + (sin(fAnLeft) * fRadiusArrowEdge));
	Gdiplus::PointF point2(rRect.right + (cos(fAnRight) * fRadiusArrowEdge), rRect.bottom + (sin(fAnRight) * fRadiusArrowEdge));
	Gdiplus::PointF point3(rRect.right, rRect.bottom);
	Gdiplus::PointF points[3] = {point1, point2, point3};
	graphics.FillPolygon(&brush, points, 3);
}
