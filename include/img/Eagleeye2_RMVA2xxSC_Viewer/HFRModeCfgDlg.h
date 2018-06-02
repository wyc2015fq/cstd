#if !defined(AFX_HFRMODECFGDLG_H__D781DE00_66AA_43F2_B062_D29F12C58E6C__INCLUDED_)
#define AFX_HFRMODECFGDLG_H__D781DE00_66AA_43F2_B062_D29F12C58E6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HFRModeCfgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHFRModeCfgDlg dialog

class CHFRModeCfgDlg : public CDialog
{
// Construction
public:
	CHFRModeCfgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHFRModeCfgDlg)
	enum { IDD = IDD_HFRMODECONFIG };
	CButton	m_ctrlHFRManual;
	CButton	m_ctrlHFRAuto;
	UINT	m_uiHFRShutterTime;
	UINT	m_uiAutoShutterUpLimit;
	float	m_fPGAUpLimit;
	UINT	m_uiBrightnessTH;
	//}}AFX_DATA

public:
	BOOL	m_bIsManual;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHFRModeCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHFRModeCfgDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAuto();
	afx_msg void OnManual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HFRMODECFGDLG_H__D781DE00_66AA_43F2_B062_D29F12C58E6C__INCLUDED_)
