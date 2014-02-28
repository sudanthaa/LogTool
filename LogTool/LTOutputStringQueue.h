#pragma once

#include <afxmt.h>
#include <list>
#include <deque>

class LTOutputStringQueue
{
	LTOutputStringQueue(void);
	~LTOutputStringQueue(void);

	void _PushString(const char* zStr);
	int _GetSeq();
	bool _GetStringSet(int uiSeq, int& uiLastSeq, int uiAmount, std::list<CString>* plstString);
	bool _GetLastStringSet(int& uiSeq, int iLines, CString& sString);

	CMutex o_Lock;
	int i_Seq;
	int i_Limit;

	std::deque<CString>  dq_strings;
	static LTOutputStringQueue o_Inst;

public:
	static void	PushString(const char* zVal)	{	o_Inst._PushString(zVal); };
	static int	GetSeq()	{	return o_Inst._GetSeq(); };
	static bool GetStringSet(int uiSeq, int& uiLastSeq, int uiAmount, std::list<CString>* plstString)
					{return o_Inst._GetStringSet(uiSeq, uiLastSeq, uiAmount, plstString); };
	static bool GetLastStringSet(int& uiSeq, int iLines, CString& sString)
					{ return o_Inst._GetLastStringSet(uiSeq, iLines, sString); }

};
