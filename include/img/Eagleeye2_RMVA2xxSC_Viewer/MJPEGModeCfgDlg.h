#if !defined(AFX_MJPEGMODECFGDLG_H__70C62CB3_8794_4C05_9D02_04F0027C545D__INCLUDED_)
#define AFX_MJPEGMODECFGDLG_H__70C62CB3_8794_4C05_9D02_04F0027C545D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MJPEGModeCfgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMJPEGModeCfgDlg dialog

class CMJPEGModeCfgDlg : public CDialog
{
// Construction
public:
	CMJPEGModeCfgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMJPEGModeCfgDlg)
	enum { IDD = IDD_MJPEGCONFIG };
	CButton	m_ctrlMJPEGManual;
	CButton	m_ctrlMJPEGAuto;
	UINT	m_uiAutoShutterUpLimit;
	UINT	m_uiMJPEGShutterTime;
	UINT	m_uiBrightnessTH;
	float	m_fPGAUpLimit;
	//}}AFX_DATA

public:
	BOOL	m_bIsManual;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMJPEGModeCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMJPEGModeCfgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAuto();
	afx_msg void OnManual();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MJPEGMODECFGDLG_H__70C62CB3_8794_4C05_9D02_04F0027C545D__INCLUDED_)
