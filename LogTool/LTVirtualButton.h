#pragma once


#define  BTN_STATE_NORMAL	1
#define  BTN_STATE_HOT	2
#define  BTN_STATE_PRESSED	3
#define  BTN_STATE_DISABLED	4

#define  WM_THMBNAIL_CLOSE		(WM_USER + 1)

class LTVirtualButton;


class LTButtonStateEngine
{
public:
	virtual void OnMouseMove(LTVirtualButton* pButton, bool bSelfArea) = 0;
	virtual void OnMouseDown(LTVirtualButton* pButton, bool bSelfArea) = 0;
	virtual void OnMouseUp(LTVirtualButton* pButton, bool bSelfArea) = 0;
	virtual void OnMouseLeave(LTVirtualButton* pButton) = 0;
	virtual bool TrackRequired(int iState) = 0;
};

class LTPushButtonState: public LTButtonStateEngine
{
public:
	LTPushButtonState(){};
	void	OnMouseMove(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseDown(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseUp(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseLeave(LTVirtualButton* pButton);
	bool	TrackRequired(int iState);
};

class LTCheckButtonState: public LTButtonStateEngine
{
public:
	LTCheckButtonState(){};
	void	OnMouseMove(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseDown(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseUp(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseLeave(LTVirtualButton* pButton);
	bool	TrackRequired(int iState);
};

class LTCheckOptionButtonState: public LTButtonStateEngine
{
public:
	LTCheckOptionButtonState(){};
	void	OnMouseMove(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseDown(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseUp(LTVirtualButton* pButton, bool bSelfArea);
	void	OnMouseLeave(LTVirtualButton* pButton);
	bool	TrackRequired(int iState);
};


class LTVirtualButtonOwner
{
public:
	enum ButtonType
	{
		PUSH_BUTTON = 0,
		CHECK_BUTTON = 1,
		CHECK_OPTIONAL_BUTTON = 2
	};

	LTVirtualButtonOwner();
	virtual CWnd* GetCWnd(){ return NULL; };
	virtual void	OnPress(LTVirtualButton* pButton){};
	virtual void	OnRelease(LTVirtualButton* pButton){};
	virtual bool	PaintBack(LTVirtualButton* pButton, CDC* pDC, CRect rRect){ return false; };
	ButtonType GetType()	{	return e_ButtonType; };
	void	SetType(ButtonType eType);

	void	TrackLeave();
	void	MouseLeave();
	LTButtonStateEngine* GetStateEngine()	{	return p_StateEngine; };
	LTVirtualButton* GetPressed()	{ return p_Pressed; };
	void	SetPressed(LTVirtualButton* pButton){ p_Pressed = pButton; };

protected:
	bool	b_LeaveTracking;
	ButtonType  e_ButtonType;
	LTVirtualButton* p_Pressed;
	LTButtonStateEngine* p_StateEngine;
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
	int		GetState()	{return i_State;};
	void		SetState(int iState)	{i_State = iState; };
	LTVirtualButtonOwner* GetOwner() { return p_Owner; };

protected:
	bool	IsSelfArea(CPoint point);

	CRect	r_Area;
	CRect*	pr_Clip;
	int		i_State;
	int*	pi_BackColor;
	LTVirtualButtonOwner* p_Owner;
};


class LTThemeButton: public LTVirtualButton
{
public:
	LTThemeButton(LTVirtualButtonOwner* pCtrl, HTHEME* pTheme, int iPart, int iStateOffset = 0);
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


class LTPaintFnButton: public LTVirtualButton
{
public:
	typedef void (*PainterFn)(int iState, CDC* pDC, CRect rArea, LTVirtualButton* pButton, void* pContext);
	LTPaintFnButton(LTVirtualButtonOwner* pOwner, PainterFn pPainter, void* pContext = NULL);

	void	OnPaintButtonState(CDC* pDC);

protected:
	void*	p_Context;
	PainterFn p_Painter;
};