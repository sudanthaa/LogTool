#include "LTPch.h"
#include "LTOutputStringQueue.h"

LTOutputStringQueue LTOutputStringQueue::o_Inst;

LTOutputStringQueue::LTOutputStringQueue(void)
{
	i_Seq = 0;
	i_Limit = 200;
}

LTOutputStringQueue::~LTOutputStringQueue(void)
{
}


void LTOutputStringQueue::_PushString( const char* zStr )
{
	o_Lock.Lock();
	dq_strings.push_back(zStr);
	if (dq_strings.size() > i_Limit)
		dq_strings.pop_front();

	i_Seq++;
	o_Lock.Unlock();
}

int LTOutputStringQueue::_GetSeq()
{
	int ui = 0;
	o_Lock.Lock();
	ui = i_Seq;
	o_Lock.Unlock();
	return ui;
}

bool LTOutputStringQueue::_GetStringSet( int iSeq, int& iLastSeq, int iAmount, std::list<CString>* plstString )
{
	o_Lock.Lock();

	if (iSeq >= i_Seq)
	{
		o_Lock.Unlock();
		return false;
	}

	int iAbsStart = 0;
	int iAbsEnd = 0;

	if (i_Seq >= i_Limit)
	{
		iAbsStart = max(i_Limit - 1 - (i_Seq - iSeq),i_Limit - iAmount - 1);
		iAbsEnd = i_Limit - 1;
	}
	else
	{
		iAbsStart = max(i_Seq - iAmount - 1,  iSeq - 1);
		iAbsEnd = i_Seq - 1;
	}

	iLastSeq = i_Seq;

	for (int i = iAbsStart; i <= iAbsEnd; i++)
		if (i < (int)dq_strings.size())
			plstString->push_back(dq_strings[i]);

	o_Lock.Unlock();

	return true;
}

bool LTOutputStringQueue::_GetLastStringSet( int& iSeq, int iLines, CString& sString )
{
	o_Lock.Lock();

	if (iSeq >= i_Seq)
	{
		o_Lock.Unlock();
		return false;
	}

	int iStart = max(0, (int)dq_strings.size() - iLines);
	for (; iStart < dq_strings.size(); iStart ++)
	{
		sString += dq_strings[iStart];
		sString += "\n";
	}

	iSeq = i_Seq;

	o_Lock.Unlock();

	return true;
}
