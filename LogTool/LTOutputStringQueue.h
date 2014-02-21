#pragma once

class LTOutputStringQueue
{
	LTOutputStringQueue(void);
	~LTOutputStringQueue(void);

	CMutex o_Lock;
	UINT ui_Seq;
	UINT ui_Limit;

	std::list<CString>  lst_strings;

public:
	void PushString(const char* zStr);
	static LTOutputStringQueue o_Inst;
};
