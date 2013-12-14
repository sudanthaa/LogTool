#pragma once


#define  BTN_STATE_NORMAL	1
#define  BTN_STATE_HOT	2
#define  BTN_STATE_PRESSED	3
#define  BTN_STATE_DISABLED	4

#define  WM_THMBNAIL_CLOSE		(WM_USER + 1)

class LTVirtualButton;

class LTVirtualButtonOwner
{
public:
	LTVirtualButtonOwner();
	virtual CWnd* GetCWnd(){ return NULL; };
	virtual void	OnPress(LTVirtualButton* pButton){};
	virtual void	OnRelease(LTVirtualButton* pButton){};
	virtual bool	PaintBack(LTVirtualButton* pButton, CDC* pDC, CRect rRect){ return false; };

	void	TrackLeave();
	void	OnMouseLeave();

	enum ButtonType
	{
		PUSH_BUTTON = 0,
		CHECK_SINGLE = 1,
		CHECK_SINGLE_OR_NOCHECK = 2
	};

protected:
	bool	b_LeaveTracking;
	ButtonType  e_ButtonType;
	LTVirtualButton* p_Pressed;
};

class LTVirtualButton
{
public:
	LTVirtualButton(LTVirtualButtonOwner* pOwner);
	virtual ~LTVirtualButton(void);

	void	SetRect(CRect rRect) { r_Area = rRect; };
	void	SetClipRect(CRect rRect);
	void	SetBackColor(int iColor);
	int		GetRightEdge() {	return r_Area.right; };
	int		GetLeftEdge(){	return r_Area.left; };;
	virtual	bool	OnMouseMove(CPoint point, CDC* pDC);
	virtual	bool	OnMouseDown(CPoint point, CDC* pDC);
	virtual bool	OnMouseUp(CPoint point, CDC* pDC);
	virtual void	OnMouseLeave(CDC* pDC);
	virtual	void	OnPaint(CDC* pDC);
	virtual void	OnPaintButtonState(CDC* pDC);

protected:
	bool	IsOwnedArea(CPoint point);

	CRect	r_Area;
	CRect*	pr_Clip;
	int		i_State;
	int*	pi_BackColor;
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

class LTIconButton: public LTVirtualButton
{
public:
	LTIconButton(LTVirtualButtonOwner* pOwner, HICON hIcon[4]);

	void	OnPaintButtonState(CDC* pDC);
protected:
	HICON	ah_Icon[4];
};