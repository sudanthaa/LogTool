#pragma once


// LTColorPicker

class LTColorPickerCallback
{
public:
	virtual void OnColorChange(COLORREF cr){};
};

class LTColorPicker : public CWnd
{
	DECLARE_DYNAMIC(LTColorPicker)

public:
	LTColorPicker();
	virtual ~LTColorPicker();

	void	SetCallback(LTColorPickerCallback* pCB) { p_CB = pCB;};

	COLORREF	cr_Val;
	LTColorPickerCallback* p_CB;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


