// LTComboBox.cpp : implementation file
//

#include "LTPch.h"
#include "LTComboBox.h"
#include "LTFieldListener.h"

// LTComboBox

IMPLEMENT_DYNAMIC(LTComboBox, CComboBox)

LTComboBox::LTComboBox(LTFieldListener* pListenser)
{
	p_Listener = pListenser;
}

LTComboBox::~LTComboBox()
{
}


BEGIN_MESSAGE_MAP(LTComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, &LTComboBox::OnCbnKillfocus)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// LTComboBox message handlers



void LTComboBox::OnCbnKillfocus()
{
	// TODO: Add your control notification handler code here
	CString sVal;
	GetWindowText(sVal);

	if (sVal != s_Value)
	{
		s_Value = sVal;
		p_Listener->OnChangeCombo(this, s_Value);
	}
}

void LTComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_RETURN)
	{
		CString sVal;
		GetWindowText(sVal);
		
		if (sVal != s_Value)
		{
			s_Value = sVal;
			p_Listener->OnChangeCombo(this, s_Value);
		}
	}

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void LTComboBox::SetText( const char* zValue )
{
	s_Value = zValue;
	SetWindowText(s_Value);
}

void LTComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}
