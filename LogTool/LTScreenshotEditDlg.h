#pragma once

#include "resource.h"

// LTScreenshotEditDlg dialog

class LTScreenshotEditDlg : public CDialog
{
	DECLARE_DYNAMIC(LTScreenshotEditDlg)

public:
	LTScreenshotEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTScreenshotEditDlg();

	enum State
	{
		CS_FREE,
		CS_ON_SET_FIRST_POINT,
		CS_ON_SET_SECOND_POINT
	};

	class XORPenContext
	{
	public:
		XORPenContext(CDC* pDC);
		~XORPenContext();

		CPen pen_XOR;
		CPen* p_LastPen;
		int i_LastROP;
		CDC* p_DC;
	};

	State	e_CaputeState;
	CRect	r_LastHor;
	CRect	r_LastVir;
	CRect	r_Last;

	void	BeginSelect();
	void	EndSelect();
	void	PointToCrossLines(CPoint pt, CRect& rHor, CRect& rVir);
	void	DrawRect(CDC* pDC, CRect rRect);

// Dialog Data
	enum { IDD = IDD_DIALOG_SCREENSHOT_EDIT };

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
};
