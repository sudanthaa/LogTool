#pragma once

#include <vector>

#define  LT_RM_LEFT		0x000F
#define  LT_RM_TOP		0x00F0
#define  LT_RM_RIGHT	0x0F00
#define  LT_RM_BOTTOM   0xF000

#define  LT_RM_BOTTMRIGHT  (LT_RM_BOTTOM | LT_RM_RIGHT)
#define  LT_RM_ALL		( LT_RM_LEFT | LT_RM_RIGHT | LT_RM_TOP | LT_RM_BOTTOM )

class LTResizeMan
{
public:
	LTResizeMan(void);
	~LTResizeMan(void);

	void	AttachForResize(CWnd* pWnd, bool bLeft, bool bTop, bool bRight, bool bBottom);
	void	AttachForResize(CWnd* pWnd, DWORD dwSpec);
	void	Originate(CWnd* pParent);
	void	Resize(int iCX, int iCY);

	class ResizeEntry
	{
	public:
		CWnd*	p_Wnd;
		DWORD	dw_Spec;
		CRect	r_Original;
	};

	CRect	r_Original;
	std::vector<ResizeEntry*>	a_Resizes;
};
