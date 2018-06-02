// Zheng_DFADlg.h : header file
//

#pragma once
#include "TransformExpr.h"

// CZheng_DFADlg dialog
class CZheng_DFADlg : public CDialog
{
// Construction
public:
	CZheng_DFADlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ZHENG_DFA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CString m_Input_expr;
public:
	CString m_Postfix_expr;

public:
	TransformExpr dfa;                                          // ÉùÃ÷¶ÔÏó
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnShowExample();
};
