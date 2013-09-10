#pragma once

#include <vector>
#include "LTVirtualButton.h"

// LTScreenshotCtrl

class LTScreenshotCtrl : public CWnd, public LTVirtualButtonOwner
{
	DECLARE_DYNAMIC(LTScreenshotCtrl)

public:
	LTScreenshotCtrl();
	virtual ~LTScreenshotCtrl();

	BOOL	CreateScreenshotCtrl(CWnd* pParent, CRect rArea, int iID);
	void	Layout(CRect rClient);
	void	PaintCtrl(CDC* pDC);

	// LTVirtualButtonOwner overrides
	CWnd*	GetCWnd();
	void	OnPress(LTVirtualButton* pButton);

	class Screenshot
	{
	public:
		Screenshot(LTScreenshotCtrl* pCtrl, HTHEME* pBtnTheme);
		Screenshot(LTScreenshotCtrl* pCtrl, HICON ahCloseBtnIcon[4], HICON ahEditBtnIcon[4]);
		~Screenshot();

		void	OnMouseMove(CPoint point, CDC* pDC);
		void	OnMouseDown(CPoint point, CDC* pDC);
		void	OnMouseUp(CPoint point, CDC* pDC);
		void	OnMouseLeave(CDC* pDC);
		void	OnPaint(CDC* pDC);
		void	Layout(CRect rContainer, int iIndex);
		int		GetLeftEdge(){ return r_Rect.left; };
		int		GetRightEdge(){ return r_Rect.right; };
		const char* GetName();
		void	SetName(const char* zName);

		int		i_BackColor;
		LTScreenshotCtrl* p_Ctrl;
		LTVirtualButton* p_CloseButton;
		LTVirtualButton* p_EditButton;
		CRect	r_Clip;
		CRect	r_Rect;
		CRect	r_Image;
		CString	 s_Name;
	};

	int i_SreenshotOffset;

	HTHEME h_thmEdit;
	HTHEME h_thmArrow;
	HTHEME h_thmWindow;

	CRect r_Main;
	bool  b_LeaveTracking;

	LTVirtualButton* p_PrevButton;
	LTVirtualButton* p_NextButton;

	std::vector<Screenshot*> a_ScreenShots;

	void	OnArrowButtonPress(LTVirtualButton* pButton);
	void	OnArrowNext();
	void	OnArrowPrevious();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


