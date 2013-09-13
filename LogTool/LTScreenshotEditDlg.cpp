// LTScreenshotEditDlg.cpp : implementation file
//

#include "LTPch.h"
#include "LTScreenshotEditDlg.h"
#include "LTScreenCaptureDlg.h"


// LTScreenshotEditDlg dialog

IMPLEMENT_DYNAMIC(LTScreenshotEditDlg, CDialog)

LTScreenshotEditDlg::LTScreenshotEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LTScreenshotEditDlg::IDD, pParent)
{
	e_CaputeState = CS_FREE;
}

LTScreenshotEditDlg::~LTScreenshotEditDlg()
{
}

void LTScreenshotEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LTScreenshotEditDlg, CDialog)
	ON_BN_CLICKED(IDOK, &LTScreenshotEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TAKE, &LTScreenshotEditDlg::OnBnClickedButtonTake)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// LTScreenshotEditDlg message handlers

void LTScreenshotEditDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here


	//OnOK();
}

BOOL LTScreenshotEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LTScreenshotEditDlg::OnBnClickedButtonTake()
{
	// TODO: Add your control notification handler code here


	LTScreenCaptureDlg oDlg;
	oDlg.DoModal();


	return;

	CRect rDesktop;
	CWnd::GetDesktopWindow()->GetWindowRect(rDesktop);
	CRect rRect;
	rRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rRect.right = rRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rRect.bottom = rRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	MoveWindow(rDesktop);

	//BeginSelect();
}

void LTScreenshotEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rWindow;
	AfxGetApp()->GetMainWnd()->GetWindowRect(rWindow);
	CPoint ptNew = point;
	ptNew.y = point.y + rWindow.top;
	ptNew.x = point.x+ rWindow.left;

	if (e_CaputeState == CS_ON_SET_FIRST_POINT)
	{
		CWindowDC dc(CWnd::GetDesktopWindow());
		XORPenContext ctx(&dc);


		dc.MoveTo(r_LastVir.TopLeft());
		dc.LineTo(r_LastVir.BottomRight());
		dc.MoveTo(r_LastHor.TopLeft());
		dc.LineTo(r_LastHor.BottomRight());

		PointToCrossLines(ptNew, r_LastHor, r_LastVir);

		dc.MoveTo(r_LastVir.TopLeft());
		dc.LineTo(r_LastVir.BottomRight());
		dc.MoveTo(r_LastHor.TopLeft());
		dc.LineTo(r_LastHor.BottomRight());
	}
	else if (e_CaputeState == CS_ON_SET_SECOND_POINT)
	{
		CWindowDC dc(CWnd::GetDesktopWindow());
		XORPenContext ctx(&dc);

		DrawRect(&dc, r_Last);

		r_Last.right = ptNew.x;
		r_Last.bottom = ptNew.y;

		DrawRect(&dc, r_Last);
	}

	CDialog::OnMouseMove(nFlags, point);
}


void LTScreenshotEditDlg::DrawRect( CDC* pDC, CRect rRect )
{
	pDC->MoveTo(rRect.left, rRect.top);
	pDC->LineTo(rRect.right, rRect.top);
	pDC->LineTo(rRect.right, rRect.bottom);
	pDC->LineTo(rRect.left, rRect.bottom);
	pDC->LineTo(rRect.left, rRect.top);
}


void LTScreenshotEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (e_CaputeState == CS_ON_SET_FIRST_POINT)
	{
		e_CaputeState = CS_ON_SET_SECOND_POINT;
		
		CWindowDC dc(CWnd::GetDesktopWindow());
		XORPenContext ctx(&dc);

		dc.MoveTo(r_LastVir.TopLeft());
		dc.LineTo(r_LastVir.BottomRight());
		dc.MoveTo(r_LastHor.TopLeft());
		dc.LineTo(r_LastHor.BottomRight());

		r_Last.left = point.x;
		r_Last.right = point.x;
		r_Last.top = point.y;
		r_Last.bottom = point.y;
		dc.Rectangle(r_Last);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void LTScreenshotEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (e_CaputeState == CS_ON_SET_SECOND_POINT)
	{
		e_CaputeState = CS_FREE;

		CWindowDC dc(CWnd::GetDesktopWindow());
		XORPenContext ctx(&dc);

		DrawRect(&dc, r_Last);

		ReleaseCapture();
		ShowWindow(SW_SHOW);
		GetParent()->ShowWindow(SW_SHOW);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void LTScreenshotEditDlg::BeginSelect()
{
	if (e_CaputeState != CS_FREE)
		return;

	e_CaputeState = CS_ON_SET_FIRST_POINT;
	SetCapture();

	//ShowWindow(SW_HIDE);
	//GetParent()->ShowWindow(SW_HIDE);

	//http://msdn.microsoft.com/en-us/library/windows/desktop/dd162729%28v=vs.85%29.aspx
	//SM_XVIRTUALSCREEN and SM_YVIRTUALSCREEN identify the upper-left corner of the virtual screen, SM_CXVIRTUALSCREEN and SM_CYVIRTUALSCREEN are the vertical and horizontal
	// SM_CMONITORS is the number of monitors attached to the desktop.

	
	CPoint ptCur(0,0);
	PointToCrossLines(ptCur, r_LastHor, r_LastVir);

	CWindowDC dc(CWnd::GetDesktopWindow());
	XORPenContext ctx(&dc);

	dc.MoveTo(r_LastVir.TopLeft());
	dc.LineTo(r_LastVir.BottomRight());
	dc.MoveTo(r_LastHor.TopLeft());
	dc.LineTo(r_LastHor.BottomRight());

	
	//GetDesktopWindow();
}

void LTScreenshotEditDlg::EndSelect()
{

}

void LTScreenshotEditDlg::PointToCrossLines( CPoint pt, CRect& rHor, CRect& rVir )
{
	rVir.top = pt.y - 50;
	rVir.left = pt.x;
	rVir.bottom = rVir.top + 100;
	rVir.right = pt.x;

	rHor.top = pt.y;
	rHor.left =  pt.x - 50;
	rHor.bottom = pt.y; 
	rHor.right = rHor.left + 100;
}



LTScreenshotEditDlg::XORPenContext::XORPenContext( CDC* pDC )
{
	pen_XOR.CreatePen(PS_DOT, 1, RGB(0,0,0)) ;
	i_LastROP = pDC->GetROP2();
	pDC->SetROP2(R2_XORPEN);
	p_LastPen = pDC->SelectObject(&pen_XOR);
	p_DC = pDC;
}

LTScreenshotEditDlg::XORPenContext::~XORPenContext()
{
	p_DC->SelectObject(p_LastPen);
	p_DC->SetROP2(i_LastROP);
	pen_XOR.DeleteObject();
}
