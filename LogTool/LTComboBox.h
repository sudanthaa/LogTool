#pragma once

class LTFieldListener;

// LTComboBox

class LTComboBox : public CComboBox
{
	DECLARE_DYNAMIC(LTComboBox)

public:
	LTComboBox(LTFieldListener* pListenser = NULL);
	virtual ~LTComboBox();

	void	SetListener(LTFieldListener* pListener) { p_Listener = pListener; };
	void	SetText(const char* zValue);

	LTFieldListener* p_Listener;
	CString s_Value;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnKillfocus();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


