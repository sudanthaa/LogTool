// LTIndefProgressBar.cpp : implementation file
//

#include "LTPch.h"
#include "LTIndefProgressBar.h"
#include "LTBitmapBuffer.h"

#define  PROGRESS_TIMER  101010

#define  SLOT_WIDTH  120
#define  SLOT_ADVANCE 3

// LTIndefProgressBar

IMPLEMENT_DYNAMIC(LTIndefProgressBar, CWnd)

LTIndefProgressBar::LTIndefProgressBar()
{
	p_BarBack = NULL;
	p_BarSlot = NULL;
	i_Progress = 0;
}

LTIndefProgressBar::~LTIndefProgressBar()
{
}


BEGIN_MESSAGE_MAP(LTIndefProgressBar, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// LTIndefProgressBar message handlers



void LTIndefProgressBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	DrawProgress(&dc, true);
}



void LTIndefProgressBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == PROGRESS_TIMER)
	{
		i_Progress += SLOT_ADVANCE;
		CClientDC dc(this);
		DrawProgress(&dc);
	}

	CWnd::OnTimer(nIDEvent);
}

int LTIndefProgressBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(PROGRESS_TIMER, 30, NULL);
	h_ThemeBar = OpenThemeData(m_hWnd, L"PROGRESS");
	// TODO:  Add your specialized creation code here

	return 0;
}

void LTIndefProgressBar::OnDestroy()
{
	KillTimer(PROGRESS_TIMER);
	CWnd::OnDestroy();

	CloseThemeData(h_ThemeBar);
	h_ThemeBar = 0;

	delete p_BarBack;
	delete p_BarSlot;
	p_BarSlot = NULL;
	p_BarBack = NULL;

	// TODO: Add your message handler code here
}



void LTIndefProgressBar::DrawProgress( CDC* pDC, bool bFull)
{
	CRect rClient;
	GetClientRect(rClient);

	VerifyBuffers(pDC);

	int iProg = (i_Progress % (rClient.Width() + SLOT_WIDTH + 4)) - SLOT_WIDTH;

	if (bFull)
		pDC->BitBlt(0,0, p_BarBack->Width(), p_BarBack->Height(), 
			p_BarBack->GetDC(), 0, 0, SRCCOPY);
	else
		pDC->BitBlt(iProg - SLOT_ADVANCE,0, SLOT_ADVANCE, 
			p_BarBack->Height(), p_BarBack->GetDC(), iProg - SLOT_ADVANCE, 0, SRCCOPY);


	pDC->BitBlt(iProg,0, p_BarSlot->Width(), p_BarSlot->Height(), 
			p_BarSlot->GetDC(), 0, 0, SRCCOPY);
}

void LTIndefProgressBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void LTIndefProgressBar::VerifyBuffers( CDC* pDC )
{
	CRect rClientEx;
	GetClientRect(rClientEx);
	if (!p_BarBack)
	{

		p_BarBack = LTBitmapBuffer::Create(pDC, rClientEx.Width(),rClientEx.Height());
		DrawThemeBackground(h_ThemeBar, p_BarBack->GetDC()->m_hDC, PP_BAR, 0, rClientEx, rClientEx);
	}

	if (!p_BarSlot)
	{
		CRect rSlot = rClientEx;
		rSlot.right = rSlot.left + SLOT_WIDTH;

		p_BarSlot = LTBitmapBuffer::Create(pDC, rSlot.Width(), rClientEx.Height());
		DrawThemeBackground(h_ThemeBar, p_BarSlot->GetDC()->m_hDC, PP_FILL, PBFS_PARTIAL, rSlot, rSlot);
	}
}
