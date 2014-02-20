#pragma once

#include "resource.h"
#include "LTScreenshotEditCtrl.h"
#include "LTResizeMan.h"
#include "LTVirtualButton.h"
#include "LTColorPicker.h"
#include "afxwin.h"

class LTDrawToolBarListener
{
public:
	virtual void OnToolPen(bool bPress){};
	virtual void OnToolRect(bool bPress){};
	virtual void OnToolArrow(bool bPress){};
};

// LTDrawToolBar

class LTDrawToolBar : public CWnd, public LTVirtualButtonOwner
{
	DECLARE_DYNAMIC(LTDrawToolBar)

public:
	LTDrawToolBar();
	virtual ~LTDrawToolBar();

	void	SetListener(LTDrawToolBarListener* pListener) { p_Listener = pListener; };

protected:
	DECLARE_MESSAGE_MAP()

	LTVirtualButton* p_PenButton;
	LTVirtualButton* p_RectButton;
	LTVirtualButton* p_ArrowButton;
	LTDrawToolBarListener* p_Listener;

	Gdiplus::Image*		p_ActiveIcons;
	Gdiplus::Image*		p_DisabledIcons;

	HTHEME	h_Theme;
	CWnd*	GetCWnd();
	void	Layout(CRect rRect);
	void	OnPress(LTVirtualButton* pButton);
	void	OnRelease(LTVirtualButton* pButton);
	bool	PaintBack(LTVirtualButton* pButton, CDC* pDC, CRect rRect);
	static void PaintToolButtonState(int iState, CDC* pDC, CRect rArea, LTVirtualButton* pButton, void* pContext);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

// LTScreenshotEditDlg dialog
class LTScreenshotEditDlg : public CDialog, 
								public LTDrawToolBarListener, 
								public LTColorPickerCallback
{
	DECLARE_DYNAMIC(LTScreenshotEditDlg)

public:
	LTScreenshotEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTScreenshotEditDlg();
	
	void	TakeScreenshot();
	LTScreenshot*	DetachScreenshot();
	int		DoModalEx(LTScreenshot* pScreenshot);

	enum Tool
	{
		TOOL_PEN,
		TOOL_RECT,
		TOOL_ARROW,
		TOOL_NONE
	};

	int		i_PenWidth;
	int		i_RectWidth;
	int		i_ArrowWidth;
	COLORREF cr_Pen;
	COLORREF cr_Rect;
	COLORREF cr_Arrow;
	Tool e_Tool;

	LTColorPicker o_ColorPicker;
	LTScreenshotEditCtrl o_ScreenshotEditCtrl;
	LTResizeMan o_Resizer;
	LTScreenshot*	p_OutputScreenshot;
	LTDrawToolBar*	p_DrawToolbar;

// Dialog Data
	enum { IDD = IDD_SCREENSHOT_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void	OnToolPen(bool bPress);
	void	OnToolRect(bool bPress);
	void	OnToolArrow(bool bPress);
	void	OnColorChange(COLORREF cr);
	LTScreenshot*	p_PreLoadScreenshot;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTake();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton o_BtnOK;
	CButton o_BtnCancel;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	CButton o_BtnTake;
	afx_msg void OnBnClickedButtonScreenshotEditPen();
	afx_msg void OnBnClickedButtonScreenshotEditRect();
	CEdit o_EditName;
	afx_msg void OnDeltaposSpinPenWidth(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic o_StaticPenWidth;
};





