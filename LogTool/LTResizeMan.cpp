#include "LTPch.h"
#include "LTResizeMan.h"

LTResizeMan::LTResizeMan(void)
{
}

LTResizeMan::~LTResizeMan(void)
{
}

void LTResizeMan::AttachForResize( CWnd* pWnd, bool bLeft, bool bTop, bool bRight, bool bBottom )
{
	DWORD dwSpec = 
		(bLeft ? LT_RM_LEFT : 0) | (bRight ? LT_RM_RIGHT : 0) | (bTop ? LT_RM_TOP : 0)| (bBottom ? LT_RM_BOTTOM : 0);
	
	AttachForResize(pWnd, dwSpec);
}

void LTResizeMan::AttachForResize( CWnd* pWnd, DWORD dwSpec )
{
	ResizeEntry* pEntry = new ResizeEntry;
	pEntry->dw_Spec = dwSpec;
	pEntry->p_Wnd  = pWnd;
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

		if (pEntry->dw_Spec & LT_RM_RIGHT)
			rNew.right = iCX - (r_Original.Width() - pEntry->r_Original.right);

		if (pEntry->dw_Spec & LT_RM_BOTTOM)
			rNew.bottom = iCY - (r_Original.Height() - pEntry->r_Original.bottom);

		if (pEntry->dw_Spec & LT_RM_LEFT)
			rNew.left = iCX - (r_Original.Width() - pEntry->r_Original.left);

		if (pEntry->dw_Spec & LT_RM_TOP)
			rNew.top = iCY - (r_Original.Height() - pEntry->r_Original.top);

		pEntry->p_Wnd->MoveWindow(rNew);
	}
}
