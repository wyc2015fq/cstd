#if !defined(AFX_TARGETIPADDRESSDLG_H__2ACE5673_7738_44C8_A432_791A6C25A94C__INCLUDED_)
#define AFX_TARGETIPADDRESSDLG_H__2ACE5673_7738_44C8_A432_791A6C25A94C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TargetIPAddressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTargetIPAddressDlg dialog

class CTargetIPAddressDlg : public CDialog
{
// Construction
public:
	CTargetIPAddressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTargetIPAddressDlg)
	enum { IDD = IDD_TARGETIPADDRESS };
	CIPAddressCtrl	m_ctrlTargetIP;
	//}}AFX_DATA

public:
	BYTE	m_byIPAdd1, m_byIPAdd2, m_byIPAdd3, m_byIPAdd4;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetIPAddressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTargetIPAddressDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARGETIPADDRESSDLG_H__2ACE5673_7738_44C8_A432_791A6C25A94C__INCLUDED_)
