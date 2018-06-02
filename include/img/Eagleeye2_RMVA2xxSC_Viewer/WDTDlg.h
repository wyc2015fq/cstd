#if !defined(AFX_WDTDLG_H__C98517BA_2831_402B_ADE0_1B0CA0E121DC__INCLUDED_)
#define AFX_WDTDLG_H__C98517BA_2831_402B_ADE0_1B0CA0E121DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WDTDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWDTDlg dialog

class CWDTDlg : public CDialog
{
// Construction
public:
	CWDTDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWDTDlg)
	enum { IDD = IDD_WDTDLG };
	CButton	m_ctrlWDTEnable;
	UINT	m_iWDT;
	//}}AFX_DATA
public:
	BOOL	m_bIsWDTEnable;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWDTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWDTDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnWdtenable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDTDLG_H__C98517BA_2831_402B_ADE0_1B0CA0E121DC__INCLUDED_)
