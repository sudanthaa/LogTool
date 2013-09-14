#pragma once

class LTBitmapBuffer
{
	LTBitmapBuffer();

public:
	~LTBitmapBuffer(void);
	static LTBitmapBuffer* Create(CDC* pRefer, int iWidth, int iHeight);

	CBitmap* GetBitmap()	{	return p_Bitmap; };
	CDC*	GetDC()	{	return p_DC; };
	void	Destory();
	CBitmap* DetachBitmap();
	CSize	 GetSize()	{	return sz_Size; };
	int		Width()	{	return sz_Size.cx; };
	int		Height(){	return sz_Size.cy; };

protected:
	CSize	sz_Size;
	CDC*	p_DC;
	CBitmap* p_Bitmap;
	CBitmap* p_OldBitmap;
};
