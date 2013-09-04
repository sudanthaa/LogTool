#pragma once


// LTScreenshotCtrl

class LTScreenshotCtrl : public CWnd
{
	DECLARE_DYNAMIC(LTScreenshotCtrl)

public:
	LTScreenshotCtrl();
	virtual ~LTScreenshotCtrl();

	BOOL CreateScreenshotCtrl(CWnd* pParent, CRect rArea, int iID);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnNcPaint();
};


