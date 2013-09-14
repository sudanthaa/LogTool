#pragma once

#include "resource.h"
#include "LTBitmapBuffer.h"

// LTScreenCaptureDlg dialog

class LTScreenCaptureDlg : public CDialog
{
	DECLARE_DYNAMIC(LTScreenCaptureDlg)

public:
	LTScreenCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTScreenCaptureDlg();

	LTBitmapBuffer*  DetachOutput();
	void	SetOutput(LTBitmapBuffer* pBuffer);

protected:
	CRect r_Rect; 
	LTBitmapBuffer* p_Bitmap;

	enum State 
	{
		MOUSE_STATE_FREE,
		MOUSE_STATE_FIRST_POINT_SELECT,
		MOUSE_STATE_SECOND_POINT_SELECT,
	};

	void	DrawCross(CDC* pDC, CPoint& pt);
	void	ClearCross(CDC* pDC, CPoint& pt, CDC* pDCBack);
	void	DrawRect( CDC* pDC, CRect& rRect );
	void	ClearRect( CDC* pDC, CRect& rRect, CDC* pDCBack);

	State	e_State;
	CPoint	pt_Cross;
	CRect	r_Select;

	CPen	pen_Line;
	LTBitmapBuffer*	p_OutputBitmap;

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
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
