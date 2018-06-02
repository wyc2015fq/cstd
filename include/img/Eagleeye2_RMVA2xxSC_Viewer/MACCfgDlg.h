#if !defined(AFX_MACCFGDLG_H__F3733371_B109_4874_9B01_577AD9EC2FAA__INCLUDED_)
#define AFX_MACCFGDLG_H__F3733371_B109_4874_9B01_577AD9EC2FAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MACCfgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMACCfgDlg dialog

class CMACCfgDlg : public CDialog
{
// Construction
public:
	CMACCfgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMACCfgDlg)
	enum { IDD = IDD_MACCFGDLG };
	CString	m_strMacAdd1;
	CString	m_strMacAdd2;
	CString	m_strMacAdd3;
	CString	m_strMacAdd4;
	CString	m_strMacAdd5;
	CString	m_strMacAdd6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMACCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMACCfgDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACCFGDLG_H__F3733371_B109_4874_9B01_577AD9EC2FAA__INCLUDED_)
