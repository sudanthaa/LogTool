#pragma once

#include "resource.h"

// LTScreenCaptureDlg dialog

class LTScreenCaptureDlg : public CDialog
{
	DECLARE_DYNAMIC(LTScreenCaptureDlg)

public:
	LTScreenCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTScreenCaptureDlg();

	CRect r_Rect; 
	CDC* pdc_VirScreenCopy;
	CBitmap* pbmp_VirScreenCopy;

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

	enum State 
	{
		MOUSE_STATE_FREE,
		MOUSE_STATE_FIRST_POINT_SELECT,
		MOUSE_STATE_SECOND_POINT_SELECT,
	};

	void	PointToCrossLines(CPoint pt, CRect& rHor, CRect& rVir);
	void	DrawCross(CDC* pDC, CRect& rHor, CRect& rVir);
	void	DrawRect( CDC* pDC, CRect rRect );

	
	State	e_State;
	CRect	r_LastHor;
	CRect	r_LastVir;
	CRect	r_Last;

// Dialog Data
	enum { IDD = IDD_SCREEN_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
