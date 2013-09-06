#pragma once


#define  BTN_STATE_NORMAL	1
#define  BTN_STATE_HOT	2
#define  BTN_STATE_PRESSED	3
#define  BTN_STATE_DISABLED	4

class LTVirtualButtonOwner
{
public:
	LTVirtualButtonOwner();
	virtual CWnd* GetCWnd(){ return NULL; };
	void	TrackLeave();
	void	OnMouseLeave();

protected:
	bool	b_LeaveTracking;
};

class LTVirtualButton
{
public:
	LTVirtualButton(LTVirtualButtonOwner* pOwner);
	~LTVirtualButton(void);

	void	SetRect(CRect rRect) { r_Area = rRect; };
	void	SetClipRect(CRect rRect);
	virtual	void	OnMouseMove(CPoint point, CDC* pDC);
	virtual	void	OnMouseDown(CPoint point, CDC* pDC);
	virtual void	OnMouseUp(CPoint point, CDC* pDC);
	virtual void	OnMouseLeave(CDC* pDC);
	virtual	void	OnPaint(CDC* pDC);
	virtual void	OnPaintButtonState(CDC* pDC);

protected:
	CRect	r_Area;
	CRect*	pr_Clip;
	int		i_State;
	LTVirtualButtonOwner* p_Owner;
};


class LTThemeButton: public LTVirtualButton
{
public:
	LTThemeButton(LTVirtualButtonOwner* pCtrl, HTHEME* pTheme, int iPart, int StateOffset = 0);
	~LTThemeButton();

	void	OnPaintButtonState(CDC* pDC);

protected:
	HTHEME* ph_Theme;
	int		i_Part;
	int		i_StateOffset;
};