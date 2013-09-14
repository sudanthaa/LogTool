#pragma once

#include <vector>

#define  LT_RM_LEFT		0x000F
#define  LT_RM_TOP		0x00F0
#define  LT_RM_RIGHT	0x0F00
#define  LT_RM_BOTTOM   0xF000

#define  LT_RM_BOTTMRIGHT	(LT_RM_BOTTOM | LT_RM_RIGHT)
#define  LT_RM_VIRTICAL		(LT_RM_BOTTOM | LT_RM_TOP)
#define  LT_RM_HORIZONTAL	(LT_RM_LEFT | LT_RM_RIGHT)
#define  LT_RM_ALL			(LT_RM_LEFT | LT_RM_RIGHT | LT_RM_TOP | LT_RM_BOTTOM )

#define  LT_RM_LEN_EXPAND	-1
#define  LT_RM_LEN_RESOURCE	-2

class LTResizeMan
{
public:
	LTResizeMan(void);
	~LTResizeMan(void);

	void	Attach(CWnd* pWnd, bool bLeft, bool bTop, bool bRight, bool bBottom, 
					int iWidth = LT_RM_LEN_EXPAND,  int iHeight = LT_RM_LEN_EXPAND);
	void	Attach(CWnd* pWnd, DWORD dwSpec, int iMinWidth = LT_RM_LEN_EXPAND,  int iMinHeight = LT_RM_LEN_EXPAND);
	void	Originate(CWnd* pParent);
	void	Resize(int iCX, int iCY);

	class ResizeEntry
	{
	public:
		CWnd*	p_Wnd;
		DWORD	dw_Spec;
		CRect	r_Original;
		int		i_MinWidth;
		int		i_MinHeight;
	};

	CRect	r_Original;
	std::vector<ResizeEntry*>	a_Resizes;
};
