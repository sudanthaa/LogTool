#pragma  once

class LTComboBox;

class LTFieldListener
{
public:
	virtual void OnChangeCombo(LTComboBox* pComboBox, const char* zValue){};
};

