#pragma once

#include "LTBitmapBuffer.h"

// LTScreenshotEditCtrl

class LTScreenshotEditCtrl : public CWnd
{
	DECLARE_DYNAMIC(LTScreenshotEditCtrl)

public:
	LTScreenshotEditCtrl();
	virtual ~LTScreenshotEditCtrl();

	BOOL	CreateScreenshotEditCtrl(CWnd* pParent, CRect rRect);
	void	SetImage(LTBitmapBuffer* pBitmap);
	void	AdjustScroolBars(int cx, int cy);
	void	DrawCtrl(CDC* pDC);

	LTBitmapBuffer* p_Bitmap;
	LTBitmapBuffer* p_OriginalBitmap;
	CPoint  pt_Offset;

	enum State
	{
		STATE_FREE,
		STATE_DRAW_LINE,
		STATE_DRAW_BOX_START,
	};

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
};


