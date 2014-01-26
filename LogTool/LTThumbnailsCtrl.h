#pragma once

#include <vector>
#include "LTVirtualButton.h"

// LTScreenshotCtrl
class LTScreenshot;
class LTThumbnailsCtrlListener;


class LTThumbnailsCtrl : public CWnd, public LTVirtualButtonOwner
{
	DECLARE_DYNAMIC(LTThumbnailsCtrl)

public:
	LTThumbnailsCtrl();
	virtual ~LTThumbnailsCtrl();

	BOOL	CreateScreenshotCtrl(CWnd* pParent, CRect rArea, int iID, LTThumbnailsCtrlListener* pListener = NULL);
	void	Layout(CRect rClient);
	void	PaintCtrl(CDC* pDC);
	void	AddScreenshot(LTScreenshot* pScreenshot);

	// LTVirtualButtonOwner overrides
	CWnd*	GetCWnd();
	void	OnPress(LTVirtualButton* pButton);
	bool	PaintBack(LTVirtualButton* pButton, CDC* pDC, CRect rRect);

	class Screenshot
	{
	public:
		Screenshot(LTThumbnailsCtrl* pCtrl, HTHEME* pBtnTheme, LTScreenshot* pScreenshot);
		Screenshot(LTThumbnailsCtrl* pCtrl, HICON ahCloseBtnIcon[4], HICON ahEditBtnIcon[4], LTScreenshot* pScreenshot);
		~Screenshot();

		void	OnMouseMove(CPoint point, CDC* pDC);
		void	OnMouseDown(CPoint point, CDC* pDC);
		void	OnMouseUp(CPoint point, CDC* pDC);
		void	OnMouseLeave(CDC* pDC);
		void	OnPaint(CDC* pDC);
		void	PaintThumbnailImage(CDC* pDC, CRect& rRect);

		void	Layout(CRect rContainer, int iIndex);
		int		GetLeftEdge(){ return r_Rect.left; };
		int		GetRightEdge(){ return r_Rect.right; };
		const char* GetName();
		void	SetName(const char* zName);

		int		i_BackColor;
		LTScreenshot* p_Screenshot;
		LTThumbnailsCtrl* p_Ctrl;
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

	LTThumbnailsCtrlListener* p_CtrlListener;
	LTVirtualButton* p_PrevButton;
	LTVirtualButton* p_NextButton;

	HICON a_hCloseIcons[4];
	HICON a_hEditIcons[4];

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
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


class LTThumbnailsCtrlListener
{
public:
	virtual void OnEditScreenshot(LTThumbnailsCtrl::Screenshot* pScreenShot){};
};

