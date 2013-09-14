// LTScreenCaptureDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTScreenCaptureDlg.h"

#define  CROSS_SIZE		50
// LTScreenCaptureDlg dialog

IMPLEMENT_DYNAMIC(LTScreenCaptureDlg, CDialog)

LTScreenCaptureDlg::LTScreenCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTScreenCaptureDlg::IDD, pParent)
{
	p_Bitmap = NULL;
	e_State = MOUSE_STATE_FREE;
	p_OutputBitmap = NULL;
}

LTScreenCaptureDlg::~LTScreenCaptureDlg()
{
}

void LTScreenCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LTScreenCaptureDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// LTScreenCaptureDlg message handlers

void LTScreenCaptureDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	if (p_Bitmap)
		dc.BitBlt(0, 0, r_Rect.Width(), r_Rect.Height(), p_Bitmap->GetDC(), 0, 0, SRCCOPY);
}

BOOL LTScreenCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// Show Current application window before taking the screenshot
	ShowWindow(SW_HIDE);
	GetParent()->ShowWindow(SW_HIDE);
	GetParent()->GetParent()->ShowWindow(SW_HIDE);

	// Leaving sometime for the desktop dc to full paint. Othewise the application UI may appear in the screen shot.
	Sleep(500);

	// Virtual screen rect
	r_Rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	r_Rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	r_Rect.right = r_Rect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	r_Rect.bottom = r_Rect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);

	CClientDC dc(this);
	p_Bitmap = LTBitmapBuffer::Create(&dc, r_Rect.Width(), r_Rect.Height());

	// Taking virtual screen dc. and a screen shot from it.
	HDC hDC = ::GetDC(NULL);
	CDC* pdcVirtScreen = CDC::FromHandle(hDC);
	p_Bitmap->GetDC()->BitBlt(0, 0, r_Rect.Width(), r_Rect.Height(), pdcVirtScreen, r_Rect.left, r_Rect.top, SRCCOPY);
	::ReleaseDC(NULL, hDC);

	// Show back the current application after taking screen-shot
	GetParent()->GetParent()->ShowWindow(SW_SHOW);
	GetParent()->ShowWindow(SW_SHOW);
	ShowWindow(SW_SHOW);

	MoveWindow(r_Rect);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int LTScreenCaptureDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	pen_Line.CreatePen(PS_SOLID, 1, RGB(255,0,0));

	// TODO:  Add your specialized creation code here

	return 0;
}

void LTScreenCaptureDlg::OnDestroy()
{
	CDialog::OnDestroy();
	pen_Line.DeleteObject();
	if (p_Bitmap)
		p_Bitmap->Destory();
	delete p_Bitmap;
	p_Bitmap = NULL;

	// TODO: Add your message handler code here
}

void LTScreenCaptureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (e_State == MOUSE_STATE_FREE)
	{
		e_State = MOUSE_STATE_FIRST_POINT_SELECT;
		CClientDC dc(this);
		int i = dc.SaveDC();

		dc.SelectObject(&pen_Line);
		DrawCross(&dc, point);
		pt_Cross = point;

		dc.RestoreDC(i);
	}
	else if (e_State == MOUSE_STATE_FIRST_POINT_SELECT)
	{
		CClientDC dc(this);
		int i = dc.SaveDC();

		dc.SelectObject(&pen_Line);
		ClearCross(&dc, pt_Cross, p_Bitmap->GetDC());
		DrawCross(&dc, point);
		pt_Cross = point;

		dc.RestoreDC(i);
	}
	else if (e_State == MOUSE_STATE_SECOND_POINT_SELECT)
	{
		CClientDC dc(this);
		int i = dc.SaveDC();

		dc.SelectObject(&pen_Line);
		ClearRect(&dc, r_Select, p_Bitmap->GetDC());
		r_Select.SetRect(r_Select.TopLeft(), point);
		DrawRect(&dc, r_Select);
		

		dc.RestoreDC(i);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void LTScreenCaptureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (e_State == MOUSE_STATE_FREE)
	{
		e_State = MOUSE_STATE_FIRST_POINT_SELECT;
		CClientDC dc(this);
		int i = dc.SaveDC();

		dc.SelectObject(&pen_Line);
		DrawCross(&dc, point);

		dc.RestoreDC(i);
	}
	else if (e_State == MOUSE_STATE_FIRST_POINT_SELECT)
	{
		e_State = MOUSE_STATE_SECOND_POINT_SELECT;
		CClientDC dc(this);
		int i = dc.SaveDC();

		dc.SelectObject(&pen_Line);
		ClearCross(&dc, pt_Cross, p_Bitmap->GetDC());
		r_Select.SetRect(point, point);
		DrawRect(&dc, r_Select);

		dc.RestoreDC(i);
	}
	

	CDialog::OnLButtonDown(nFlags, point);
}

void LTScreenCaptureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (e_State == MOUSE_STATE_SECOND_POINT_SELECT)
	{
		e_State = MOUSE_STATE_FREE;
		CClientDC dc(this);
		ClearRect(&dc, r_Select, p_Bitmap->GetDC());

		LTBitmapBuffer* pCropped = LTBitmapBuffer::Create(&dc, 
			r_Select.Width(), r_Select.Height());
		pCropped->GetDC()->BitBlt(0, 0, r_Select.Width(), r_Select.Height(), 
			p_Bitmap->GetDC(), r_Select.left, r_Select.top, SRCCOPY);

		SetOutput(pCropped);
		EndDialog(IDOK);
	}

	CDialog::OnLButtonUp(nFlags, point);
}


void LTScreenCaptureDlg::DrawCross( CDC* pDC, CPoint& pt )
{
	pDC->MoveTo(pt.x, pt.y - CROSS_SIZE);
	pDC->LineTo(pt.x, pt.y + CROSS_SIZE);
	pDC->MoveTo(pt.x - CROSS_SIZE, pt.y);
	pDC->LineTo(pt.x + CROSS_SIZE, pt.y);
}

void LTScreenCaptureDlg::ClearCross( CDC* pDC, CPoint& pt, CDC* pDCBack )
{
	CRect rArea;
	rArea.top = pt.y - CROSS_SIZE - 1;
	rArea.bottom = pt.y + CROSS_SIZE + 1;
	rArea.left = pt.x- CROSS_SIZE - 1;
	rArea.right = pt.x + CROSS_SIZE + 1;

	pDC->BitBlt(rArea.left, rArea.top, rArea.Width(), rArea.Height(), 
		pDCBack, rArea.left, rArea.top, SRCCOPY);
}


void LTScreenCaptureDlg::DrawRect( CDC* pDC, CRect& rRect )
{
	CRect rNormalized = rRect;
	rNormalized.NormalizeRect();

	pDC->MoveTo(rNormalized.left, rNormalized.top);
	pDC->LineTo(rNormalized.right, rNormalized.top);
	pDC->LineTo(rNormalized.right, rNormalized.bottom);
	pDC->LineTo(rNormalized.left, rNormalized.bottom);
	pDC->LineTo(rNormalized.left, rNormalized.top);
}


void LTScreenCaptureDlg::ClearRect( CDC* pDC, CRect& rRect, CDC* pDCBack )
{
	CRect rNormalized = rRect;
	rNormalized.NormalizeRect();

	rNormalized.bottom ++;
	rNormalized.right ++;
	rNormalized.top --;
	rNormalized.left --;

	pDC->BitBlt(rNormalized.left, rNormalized.top, rNormalized.Width(), rNormalized.Height(), 
		pDCBack, rNormalized.left, rNormalized.top, SRCCOPY);
}

LTBitmapBuffer* LTScreenCaptureDlg::DetachOutput()
{
	LTBitmapBuffer* pBitmap = p_OutputBitmap;
	p_OutputBitmap = NULL;
	return pBitmap;
}


void LTScreenCaptureDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
}

void LTScreenCaptureDlg::SetOutput( LTBitmapBuffer* pBuffer )
{
	p_OutputBitmap = pBuffer;
}
