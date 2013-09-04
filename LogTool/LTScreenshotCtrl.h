#pragma once


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

	};

	HTHEME h_thmEdit;
	HTHEME h_thmArrow;

	CRect r_Main;
	bool  b_LeaveTracking;

	ArrowButton* p_PrevButton;
	ArrowButton* p_NextButton;

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


