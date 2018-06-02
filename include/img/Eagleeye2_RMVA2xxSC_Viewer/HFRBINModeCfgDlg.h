#if !defined(AFX_HFRBINMODECFGDLG_H__BB523711_33F1_4F2A_8DA0_40C7B06EBC88__INCLUDED_)
#define AFX_HFRBINMODECFGDLG_H__BB523711_33F1_4F2A_8DA0_40C7B06EBC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HFRBINModeCfgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHFRBINModeCfgDlg dialog

class CHFRBINModeCfgDlg : public CDialog
{
// Construction
public:
	CHFRBINModeCfgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHFRBINModeCfgDlg)
	enum { IDD = IDD_HFRBINMODECONFIG };
	CButton	m_ctrlHFRBINManual;
	CButton	m_ctrlHFRBINAuto;
	UINT	m_uiAutoShutterUpLimit;
	UINT	m_uiHFRBINShutterTime;
	UINT	m_uiBrightnessTH;
	float	m_fPGAUpLimit;
	//}}AFX_DATA

public:
	BOOL	m_bIsManual;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHFRBINModeCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHFRBINModeCfgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnManual();
	afx_msg void OnAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HFRBINMODECFGDLG_H__BB523711_33F1_4F2A_8DA0_40C7B06EBC88__INCLUDED_)
