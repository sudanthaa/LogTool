#pragma once


class LTBitmapBuffer;
// LTIndefProgressBar

class LTIndefProgressBar : public CWnd
{
	DECLARE_DYNAMIC(LTIndefProgressBar)

public:
	LTIndefProgressBar();
	virtual ~LTIndefProgressBar();

	int i_Progress;
	HTHEME h_ThemeBar;

	void	DrawProgress(CDC* pDC, bool bFull = false);
	void	VerifyBuffers( CDC* pDC );

	LTBitmapBuffer*		p_BarBack;
	LTBitmapBuffer*		p_BarSlot;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


