#if !defined(AFX_AFECONFIGDLG_H__FAE54E23_9D70_4149_A99A_5FD110C90152__INCLUDED_)
#define AFX_AFECONFIGDLG_H__FAE54E23_9D70_4149_A99A_5FD110C90152__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AfeConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAfeConfigDlg dialog

class CAfeConfigDlg : public CDialog
{
// Construction
public:
	CAfeConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAfeConfigDlg)
	enum { IDD = IDD_AFECONFIGDLG };
	float	m_fAfeVGAGain;
	//}}AFX_DATA
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAfeConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAfeConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AFECONFIGDLG_H__FAE54E23_9D70_4149_A99A_5FD110C90152__INCLUDED_)
