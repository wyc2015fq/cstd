#if !defined(AFX_UPLCADLG_H__45546749_7384_4BA8_8203_882184791C75__INCLUDED_)
#define AFX_UPLCADLG_H__45546749_7384_4BA8_8203_882184791C75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UPLCaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUPLCaDlg dialog

class CUPLCaDlg : public CDialog
{
// Construction
public:
	CUPLCaDlg(CString strIP,CWnd* pParent = NULL);   // standard constructor
public:
     CString m_strIP;

// Dialog Data
	//{{AFX_DATA(CUPLCaDlg)
	enum { IDD = IDD_UPLCADLG };
	CString	m_strPWDEDT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUPLCaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUPLCaDlg)
	afx_msg void OnFileOpenBnt();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPLCADLG_H__45546749_7384_4BA8_8203_882184791C75__INCLUDED_)
