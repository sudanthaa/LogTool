#pragma once

#include <resource.h>
#include "LTIndefProgressBar.h"

// LTProgressDlg dialog

class LTProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(LTProgressDlg)

public:
	LTProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LTProgressDlg();

	LTIndefProgressBar o_Bar;

// Dialog Data
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
