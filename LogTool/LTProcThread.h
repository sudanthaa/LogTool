#pragma once



class LTUploadTask;
// LTProcThread

class LTProcThread
{
public:
	LTProcThread();           // protected constructor used by dynamic creation
	virtual ~LTProcThread();

	void	DoTask(LTUploadTask* pTask);
	void	RunThread();
	void	WaitForCompletion();
	void	ExitThread();
	bool	IsTermSignalled(){	return b_TermSignalled; };

protected:
	LTUploadTask* p_Task;

	void	OnThreadRun();
	static UINT __cdecl _ThreadFunc( LPVOID pParam );
	CWinThread*		p_Thread;
	bool	b_TermSignalled;
};


