// test_venaMFCDlg.h : header file
//

#if !defined(AFX_TEST_VENAMFCDLG_H__BCBC179E_A923_4DBA_9A16_DE98F4DD4002__INCLUDED_)
#define AFX_TEST_VENAMFCDLG_H__BCBC179E_A923_4DBA_9A16_DE98F4DD4002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTest_venaMFCDlg dialog

class CTest_venaMFCDlg : public CDialog
{
// Construction
public:
	CTest_venaMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTest_venaMFCDlg)
	enum { IDD = IDD_TEST_VENAMFC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_venaMFCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTest_venaMFCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_VENAMFCDLG_H__BCBC179E_A923_4DBA_9A16_DE98F4DD4002__INCLUDED_)
