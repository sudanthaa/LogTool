#pragma once

#include "resource.h"
#include "LTScreenshotEditCtrl.h"
#include "LTResizeMan.h"
#include "afxwin.h"

// LTScreenshotEditDlg dialog

class LTScreenshotEditDlg : public CDialog
{
	DECLARE_DYNAMIC(LTScreenshotEditDlg)

public:
	LTScreenshotEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTScreenshotEditDlg();
	
	void	TakeScreenshot();
	LTScreenshot*	DetachScreenshot();

	LTScreenshotEditCtrl o_ScreenshotEditCtrl;
	LTResizeMan o_Resizer;
	LTScreenshot*	p_Screenshot;

// Dialog Data
	enum { IDD = IDD_SCREENSHOT_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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
};
