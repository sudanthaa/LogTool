// LTProcThread.cpp : implementation file
//

#include "LTPch.h"
#include "LTProcThread.h"
#include "LTProgressDlg.h"
#include "LTUploadTask.h"


// LTProcThread


LTProcThread::LTProcThread()
{
	p_Thread = NULL;
	b_TermSignalled = false;
}

LTProcThread::~LTProcThread()
{
//	if (p_Thread)
//		Terminate();

	ExitThread();
}

UINT __cdecl LTProcThread::_ThreadFunc( LPVOID pParam )
{
	LTProcThread* pThread = (LTProcThread*)pParam;
	pThread->OnThreadRun();
	pThread->ExitThread();
	return 0;
}

void LTProcThread::RunThread()
{
	p_Thread = 
		AfxBeginThread(LTProcThread::_ThreadFunc, (LPVOID)this, 0);
}

void LTProcThread::WaitForCompletion()
{
	if (p_Thread)
		::WaitForSingleObject(p_Thread->m_hThread, INFINITE);
}

void LTProcThread::ExitThread()
{
	//::PostMessage(HWND_BROADCAST, WM_TERMINATE_THREAD, 0, 0);
	b_TermSignalled = true;
	//WaitForCompletion();
}

void LTProcThread::DoTask( LTUploadTask* pTask )
{
	p_Task = pTask;
	RunThread();
}

void LTProcThread::OnThreadRun()
{
	if (p_Task)
		p_Task->b_Success = LTUploadTask::DoTask(p_Task);

	p_Task = NULL;
}


// LTProcThread message handlers
