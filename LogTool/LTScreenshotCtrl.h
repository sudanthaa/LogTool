#pragma once

#include <vector>

// LTScreenshotCtrl

class LTScreenshotCtrl : public CWnd
{
	DECLARE_DYNAMIC(LTScreenshotCtrl)

public:
	LTScreenshotCtrl();
	virtual ~LTScreenshotCtrl();

	BOOL	CreateScreenshotCtrl(CWnd* pParent, CRect rArea, int iID);
	void	Layout(CRect rClient);
	bool	IsTrackingMouseLeave()	{	return b_LeaveTracking; };
	bool	TrackMouseLeave();

	class ArrowButton
	{
	public:
		ArrowButton(LTScreenshotCtrl* pCtrl, HTHEME* pTheme, int iPart, int StateOffset = 0);
		~ArrowButton();

		void	SetTheme(HTHEME* pTheme) { ph_Theme = pTheme; };
		void	SetRect(CRect rRect) { r_Area = rRect; };
		void	OnMouseMove(CPoint point, CDC* pDC);
		void	OnMouseDown(CPoint point, CDC* pDC);
		void	OnMouseUp(CPoint point, CDC* pDC);
		void	OnMouseLeave(CDC* pDC);
		void	OnPaint(CDC* pDC);

		LTScreenshotCtrl* p_Ctrl;
		int		i_StateOffset;
		int		i_State;
		CRect	r_Area;
		int		i_Part;
		HTHEME* ph_Theme;
	};

	class Screenshot
	{
	public:
		Screenshot(LTScreenshotCtrl* pCtrl, HTHEME* pTheme);
		~Screenshot();

		void	OnMouseMove(CPoint point, CDC* pDC);
		void	OnMouseDown(CPoint point, CDC* pDC);
		void	OnMouseUp(CPoint point, CDC* pDC);
		void	OnMouseLeave(CDC* pDC);
		void	OnPaint(CDC* pDC);
		void	Layout(CRect rContainer, int iIndex);

		LTScreenshotCtrl* p_Ctrl;
		ArrowButton* p_CloseButton;
		ArrowButton* p_EditButton;
		CRect	r_Rect;
		CRect	r_Image;

		HTHEME* ph_Theme;
	};

	HTHEME h_thmEdit;
	HTHEME h_thmArrow;
	HTHEME h_thmWindow;

	CRect r_Main;
	bool  b_LeaveTracking;

	ArrowButton* p_PrevButton;
	ArrowButton* p_NextButton;

	std::vector<Screenshot*> a_ScreenShots;

	void	OnArrowButtonPress(ArrowButton* pButton);
	void	OnArrowNext();
	void	OnArrowPrevious();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnNcPaint();
//	afx_msg void OnNcPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


