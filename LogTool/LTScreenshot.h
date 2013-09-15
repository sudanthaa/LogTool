#pragma once

#include <list>
#include <vector>

class LTScreenshot;
class LTBitmapBuffer;

#define  LT_MARKTYPE_PEN	0
#define  LT_MARKTYPE_RECT	1
#define  LT_MARKTYPE_ARROW	2

class LTScreenshotMarking
{
public:
	LTScreenshotMarking(int iType, LTScreenshot* pScreenshot);
	virtual ~LTScreenshotMarking();

	virtual void	OnMouseMove(CDC* pDC, CPoint pt, CPoint ptOffset){};
	virtual void	OnMouseDown(CDC* pDC,CPoint pt, CPoint ptOffset){};
	virtual void	OnMouseUp(CDC* pDC,CPoint pt, CPoint ptOffset){};

	int		i_Type;
	LTScreenshot*	p_Screenshot;
};

class LTPenMarking: public LTScreenshotMarking
{
public:
	LTPenMarking(LTScreenshot* pScreenshot, COLORREF iColor = RGB(255,0,0), int iWidth = 1);
	~LTPenMarking();

	void	OnMouseMove(CDC* pDC, CPoint pt, CPoint ptOffset);
	void	OnMouseDown(CDC* pDC, CPoint pt, CPoint ptOffset);
	void	OnMouseUp(CDC* pDC, CPoint pt, CPoint ptOffset);

protected:
	COLORREF cr_Line;
	int		i_Width;
	std::list<CPoint> lst_Points;
};

class LTRectMarking: public LTScreenshotMarking
{
public:
	LTRectMarking(LTScreenshot* pScreenshot, COLORREF iColor = RGB(255,0,0), int iWidth = 1);
	~LTRectMarking();

	void	OnMouseMove(CDC* pDC, CPoint pt, CPoint ptOffset);
	void	OnMouseDown(CDC* pDC, CPoint pt, CPoint ptOffset);
	void	OnMouseUp(CDC* pDC, CPoint pt, CPoint ptOffset);
	void	DrawRect( CDC* pDC, CRect& rRect );

protected:
	COLORREF cr_Line;
	CRect	r_Rect;
	int		i_Width;
};

class LTArrowMarking: public LTScreenshotMarking
{
public:
	LTArrowMarking(LTScreenshot* pScreenshot, COLORREF iColor = RGB(255,0,0), int iWidth = 2);
	~LTArrowMarking();

	void	OnMouseMove(CDC* pDC, CPoint pt, CPoint ptOffset);
	void	OnMouseDown(CDC* pDC, CPoint pt, CPoint ptOffset);
	void	OnMouseUp(CDC* pDC, CPoint pt, CPoint ptOffset);

protected:
	COLORREF cr_Arrow;
	CRect	r_Rect;
	int		i_Width;
};

class LTScreenshot
{
	LTScreenshot(void);
public:
	~LTScreenshot(void);

	static LTScreenshot* Create(LTBitmapBuffer* pBuffer);

	LTBitmapBuffer* GetOrigBuffer()	{	return p_OriginalBitmap; };
	LTBitmapBuffer* GetBuffer()	{	return p_Bitmap; };
	CDC*	GetOrigDC();
	CDC*	GetDC();
	int		Width();
	int		Height();
	void	Add(LTScreenshotMarking* pMarking);
	void	SetName(const char* zName)	{	s_Name = zName; };
	const char*		GetName() {	return s_Name; };

protected:
	CString		s_Name;
	std::vector<LTScreenshotMarking*> a_Markings;
	LTBitmapBuffer* p_OriginalBitmap;
	LTBitmapBuffer* p_Bitmap;
};
