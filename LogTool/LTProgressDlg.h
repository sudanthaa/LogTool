#pragma once

#include <resource.h>
#include "LTIndefProgressBar.h"

#define  WM_TERMINATE_THREAD     WM_USER  +  1

class LTProcThread;
// LTProgressDlg dialog

class LTProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(LTProgressDlg)

public:
	LTProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTProgressDlg();

	LTIndefProgressBar o_Bar;
	LTProcThread* p_Thread;
	int i_Seq;

	void	SetThread(LTProcThread* pThread){ p_Thread = pThread; }
	

	CStatic o_StaticLog;
// Dialog Data
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTermRequest(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
