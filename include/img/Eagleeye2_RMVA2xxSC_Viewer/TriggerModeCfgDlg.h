#if !defined(AFX_TRIGGERMODECFGDLG_H__D5B29481_D263_4F34_8F43_00F15A84D893__INCLUDED_)
#define AFX_TRIGGERMODECFGDLG_H__D5B29481_D263_4F34_8F43_00F15A84D893__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TriggerModeCfgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTriggerModeCfgDlg dialog

class CTriggerModeCfgDlg : public CDialog
{
// Construction
public:
	CTriggerModeCfgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTriggerModeCfgDlg)
	enum { IDD = IDD_TRGMODECONFIG };
	CButton	m_ctrlEnableADIL;
	CButton	m_ctrlSingalCoin;
	CButton	m_ctrlShutPgaManualCtrl;
	CButton	m_ctrlShutPgaAutoCtrl;
	CButton	m_ctrlDoubleCoin;
	UINT	m_uiTrgShutterTime;
	int		m_iECol;
	int		m_iERow;
	float	m_fInterval;
	CString	m_strLocation;
	float	m_fPGALimit;
	int		m_iSCol;
	int		m_iShutLimit;
	int		m_iSRow;
	float	m_fTrgPga;
	int		m_iWayIndex;
	BYTE	m_byYMean;
	int		m_iADLIP;
	int		m_iCurShut;
	float	m_fCurPga;
	//}}AFX_DATA
public:
	BOOL	m_bIsSingalCoin;
	BOOL	m_bIsManualCtrl;
	BOOL	m_bEnableADLI;
	BOOL	m_bSaveParam;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTriggerModeCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTriggerModeCfgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDoublecoin();
	afx_msg void OnSingalcoin();
	afx_msg void OnShutpgamanualctrl();
	afx_msg void OnShutpgaautoctrl();
	afx_msg void OnEnableadli();
	afx_msg void OnSaveparam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIGGERMODECFGDLG_H__D5B29481_D263_4F34_8F43_00F15A84D893__INCLUDED_)
