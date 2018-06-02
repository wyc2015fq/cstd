#if !defined(AFX_WHITEBALANCEDLG_H__70E3F5CA_3FE9_40C7_B017_F56E9FF84409__INCLUDED_)
#define AFX_WHITEBALANCEDLG_H__70E3F5CA_3FE9_40C7_B017_F56E9FF84409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhiteBalanceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWhiteBalanceDlg dialog

class CWhiteBalanceDlg : public CDialog
{
// Construction
public:
	CWhiteBalanceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWhiteBalanceDlg)
	enum { IDD = IDD_WHITEBALANCEDLG };
	CButton	m_ctrlFluorescence;
	CButton	m_ctrlDaylight;
	CButton	m_ctrlOrgData;
	//}}AFX_DATA

public:
	UINT	m_uiWBType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhiteBalanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWhiteBalanceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWbdaylight();
	afx_msg void OnWbfluorescence();
	afx_msg void OnOrgdata();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHITEBALANCEDLG_H__70E3F5CA_3FE9_40C7_B017_F56E9FF84409__INCLUDED_)
