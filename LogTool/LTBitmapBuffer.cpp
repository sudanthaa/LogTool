#include "LTPch.h"
#include "LTBitmapBuffer.h"

LTBitmapBuffer::LTBitmapBuffer()
{
	p_DC = NULL;
	p_Bitmap = NULL;
	p_OldBitmap = NULL;
}

LTBitmapBuffer::~LTBitmapBuffer(void)
{
	Destory();
}

LTBitmapBuffer* LTBitmapBuffer::Create( CDC* pRefeDC, int iWidth, int iHeight )
{
	LTBitmapBuffer* pBuffer = new LTBitmapBuffer;
	pBuffer->p_DC = new CDC;
	pBuffer->p_Bitmap = new CBitmap;

	pBuffer->p_DC->CreateCompatibleDC(pRefeDC);
	pBuffer->p_Bitmap->CreateCompatibleBitmap(pRefeDC, iWidth, iHeight);
	pBuffer->p_OldBitmap = pBuffer->p_DC->SelectObject(pBuffer->p_Bitmap);
	pBuffer->sz_Size.cx = iWidth;
	pBuffer->sz_Size.cy = iHeight;

	return pBuffer;
}

void LTBitmapBuffer::Destory()
{
	if (p_DC)
		p_DC->SelectObject(p_OldBitmap);

	if (p_Bitmap)
		p_Bitmap->DeleteObject();

	if (p_DC)
		p_DC->DeleteDC();

	delete p_DC;
	delete p_Bitmap;

	p_OldBitmap = NULL;
	p_Bitmap = NULL;
	p_DC = NULL;
}

CBitmap* LTBitmapBuffer::DetachBitmap()
{
	CBitmap* pBitmap = p_Bitmap;
	p_Bitmap = NULL;
	return pBitmap;
}

LTBitmapBuffer* LTBitmapBuffer::Clone()
{
	LTBitmapBuffer* pBuffer = LTBitmapBuffer::Create(p_DC, Width(), Height());
	pBuffer->GetDC()->BitBlt(0, 0, Width(), Height(), p_DC, 0, 0, SRCCOPY);
	return pBuffer;
}
