#if !defined(AFX_NEWIPMACDLG_H__802337A4_B88E_453A_9062_B52A2538AED8__INCLUDED_)
#define AFX_NEWIPMACDLG_H__802337A4_B88E_453A_9062_B52A2538AED8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewIPMACDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewIPMACDlg dialog

class CNewIPMACDlg : public CDialog
{
// Construction
public:
	CNewIPMACDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewIPMACDlg)
	enum { IDD = IDD_NEWIPMACSET };
	CIPAddressCtrl	m_ctrlNewIPAddress;
	//}}AFX_DATA

public:
	BYTE	m_byIPAdd1, m_byIPAdd2, m_byIPAdd3, m_byIPAdd4;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewIPMACDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewIPMACDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWIPMACDLG_H__802337A4_B88E_453A_9062_B52A2538AED8__INCLUDED_)
