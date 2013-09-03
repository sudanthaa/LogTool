#include "LTPch.h"
#include "LTResizeMan.h"

LTResizeMan::LTResizeMan(void)
{
}

LTResizeMan::~LTResizeMan(void)
{
}

void LTResizeMan::Attach( CWnd* pWnd, bool bLeft, bool bTop, bool bRight, bool bBottom 
						 , int iMinWidth,  int iMinHeight)
{
	DWORD dwSpec = 
		(bLeft ? LT_RM_LEFT : 0) | (bRight ? LT_RM_RIGHT : 0) | (bTop ? LT_RM_TOP : 0)| (bBottom ? LT_RM_BOTTOM : 0);
	
	Attach(pWnd, dwSpec, iMinWidth, iMinHeight);
}

void LTResizeMan::Attach( CWnd* pWnd, DWORD dwSpec, int iMinWidth,  int iMinHeight)
{
	ResizeEntry* pEntry = new ResizeEntry;
	pEntry->dw_Spec = dwSpec;
	pEntry->p_Wnd  = pWnd;
	pEntry->i_MinHeight = iMinHeight;
	pEntry->i_MinWidth = iMinWidth;
	a_Resizes.push_back(pEntry);
}

void LTResizeMan::Originate( CWnd* pParent )
{
	for (UINT ui = 0; ui < a_Resizes.size(); ui++)
	{
		ResizeEntry* pEntry = a_Resizes[ui];
		pEntry->p_Wnd->GetWindowRect(pEntry->r_Original);
		pParent->ScreenToClient(pEntry->r_Original);
		int i = 0;
		i++;
	}

	pParent->GetClientRect(r_Original);
}

void LTResizeMan::Resize( int iCX, int iCY )
{
	for (UINT ui = 0; ui < a_Resizes.size(); ui++)
	{
		ResizeEntry* pEntry = a_Resizes[ui];
		CRect rNew = pEntry->r_Original;

		if (pEntry->dw_Spec & LT_RM_LEFT)
			rNew.left = iCX - (r_Original.Width() - pEntry->r_Original.left);

		if (pEntry->dw_Spec & LT_RM_TOP)
			rNew.top = iCY - (r_Original.Height() - pEntry->r_Original.top);

		if (pEntry->dw_Spec & LT_RM_RIGHT)
		{
			rNew.right = iCX - (r_Original.Width() - pEntry->r_Original.right);

			if (pEntry->i_MinWidth == LT_RM_LEN_RESOURCE)
				rNew.right = rNew.left + pEntry->r_Original.Width();
			else if  (pEntry->i_MinWidth != LT_RM_LEN_EXPAND) 
				rNew.right = rNew.left + pEntry->i_MinWidth;
		}

		if (pEntry->dw_Spec & LT_RM_BOTTOM)
		{
			rNew.bottom = iCY - (r_Original.Height() - pEntry->r_Original.bottom);

			if (pEntry->i_MinHeight == LT_RM_LEN_RESOURCE)
				rNew.right = rNew.left + pEntry->r_Original.Width();
			else if  (pEntry->i_MinWidth != LT_RM_LEN_EXPAND) 
				rNew.right = rNew.left + pEntry->i_MinWidth;
		}

		pEntry->p_Wnd->MoveWindow(rNew, TRUE);
	}

	for (UINT ui = 0; ui < a_Resizes.size(); ui++)
	{
		ResizeEntry* pEntry = a_Resizes[ui];
		pEntry->p_Wnd->Invalidate(FALSE);
	}
}
