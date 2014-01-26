#pragma once

#include <list>
#include "LTBitmapBuffer.h"
#include "LTScreenshot.h"
// LTScreenshotEditCtrl

class LTScreenshotEditCtrl : public CWnd
{
	DECLARE_DYNAMIC(LTScreenshotEditCtrl)

public:
	LTScreenshotEditCtrl();
	virtual ~LTScreenshotEditCtrl();

	BOOL	CreateScreenshotEditCtrl(CWnd* pParent, CRect rRect);
	void	SetImage(LTBitmapBuffer* pBitmap);
	void	SetScreenshot(LTScreenshot* pScreenShot);
	void	AdjustScroolBars(int cx, int cy);
	void	DrawCtrl(CDC* pDC);

	void	PenStart();
	void	RectStart();
	void	ArrowStart();

	LTScreenshot* GetScreenshot();
	LTScreenshot* DetachScreenshot();

protected:
	LTScreenshot* p_Screenshot;
	LTScreenshotMarking* p_ActiveMarking;
	CPoint  pt_Offset;
	CPen	pen_Line;

	enum State
	{
		STATE_FREE,
		STATE_PEN_START,
		STATE_PEN_DRAW,
		STATE_RECT_START,
		STATE_RECT_DRAW,
		STATE_ARROW_START,
		STATE_ARROW_DRAW,
	};

	State	e_State;
	HTHEME	h_Theme;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNcPaint();
};


