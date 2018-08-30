// testA20Dlg.h : header file
//

#if !defined(AFX_TESTA20DLG_H__DC98E124_7501_41E1_B4D9_20E1EBBFEF44__INCLUDED_)
#define AFX_TESTA20DLG_H__DC98E124_7501_41E1_B4D9_20E1EBBFEF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestA20Dlg dialog

class CTestA20Dlg : public CDialog
{
// Construction
public:
	CTestA20Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestA20Dlg)
	enum { IDD = IDD_TESTA20_DIALOG };
	CListBox	m_EditLogger;
	CButton	m_ctrlReceiveHex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestA20Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestA20Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
	afx_msg void OnAddId();
	afx_msg void OnDelIdAll();
	afx_msg void OnAddIdX();
	afx_msg void OnUpCharAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTA20DLG_H__DC98E124_7501_41E1_B4D9_20E1EBBFEF44__INCLUDED_)
