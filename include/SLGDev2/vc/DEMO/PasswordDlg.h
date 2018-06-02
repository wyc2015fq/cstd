#if !defined(AFX_PASSWORDDLG_H__26220768_0E42_479D_A128_7E84B8BBCFC2__INCLUDED_)
#define AFX_PASSWORDDLG_H__26220768_0E42_479D_A128_7E84B8BBCFC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog

class CPasswordDlg : public CDialog
{
// Construction
public:
	CPasswordDlg(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL IsVerify();

// Dialog Data
	//{{AFX_DATA(CPasswordDlg)
	enum { IDD = IDD_PassWordDlg };
	UINT	m_nIntVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswordDlg)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDDLG_H__26220768_0E42_479D_A128_7E84B8BBCFC2__INCLUDED_)
