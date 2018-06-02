#if !defined(AFX_AUTHDLG_H__F590D85A_4F46_45DF_BA30_51911392D3F5__INCLUDED_)
#define AFX_AUTHDLG_H__F590D85A_4F46_45DF_BA30_51911392D3F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AuthDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AuthDlg dialog

class AuthDlg : public CDialog
{
// Construction
public:
	AuthDlg(const char* dev_addr, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AuthDlg)
	enum { IDD = IDD_DIALOG_DSP_AUTH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AuthDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AuthDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	char m_IP[64];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTHDLG_H__F590D85A_4F46_45DF_BA30_51911392D3F5__INCLUDED_)
