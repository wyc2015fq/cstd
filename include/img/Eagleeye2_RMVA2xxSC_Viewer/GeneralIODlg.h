#if !defined(AFX_GENERALIODLG_H__7C5363E5_16CE_4EFB_A99B_4253A3BB23FB__INCLUDED_)
#define AFX_GENERALIODLG_H__7C5363E5_16CE_4EFB_A99B_4253A3BB23FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralIODlg.h : header file
//

//struct for PIO 
struct PIO_indicator{
	unsigned pio1 : 1;
	unsigned pio2 : 1;
	unsigned pio3 : 1;
	unsigned pio4 : 1;
	unsigned pio5 : 1;
	unsigned pio6 : 1;
	unsigned pio7 : 1;
	unsigned pio8 : 1;
	unsigned pio9 : 1;
	unsigned pio10 : 1;
	unsigned pio1outvalue : 1;
	unsigned pio2outvalue : 1;
	unsigned pio3outvalue : 1;
	unsigned pio4outvalue : 1;
	unsigned pio5outvalue : 1;
	unsigned pio6outvalue : 1;
	unsigned pio7outvalue : 1;
	unsigned pio8outvalue: 1;
	unsigned pio9outvalue : 1;
	unsigned pio10outvalue : 1;
	unsigned reserved : 12;
};

/////////////////////////////////////////////////////////////////////////////
// CGeneralIODlg dialog

class CGeneralIODlg : public CDialog
{
// Construction
public:
	CGeneralIODlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGeneralIODlg)
	enum { IDD = IDD_GIODLG };
	CButton	m_ctrlFlashSynEnable;
	CButton	m_ctrlFlashSynDisable;
	CButton	m_ctrlPort9OUT;
	CButton	m_ctrlPort9IN;
	CButton	m_ctrlPort8OUT;
	CButton	m_ctrlPort8IN;
	CButton	m_ctrlPort7OUT;
	CButton	m_ctrlPort7IN;
	CButton	m_ctrlPort6OUT;
	CButton	m_ctrlPort6IN;
	CButton	m_ctrlPort5OUT;
	CButton	m_ctrlPort5IN;
	CButton	m_ctrlPort4OUT;
	CButton	m_ctrlPort4IN;
	CButton	m_ctrlPort3OUT;
	CButton	m_ctrlPort3IN;
	CButton	m_ctrlPort2OUT;
	CButton	m_ctrlPort2IN;
	CButton	m_ctrlPort1OUT;
	CButton	m_ctrlPort1IN;
	CButton	m_ctrlPort10OUT;
	CButton	m_ctrlPort10IN;
	BOOL	m_bPort10OUTValue;
	BOOL	m_bPort1OUTValue;
	BOOL	m_bPort2OUTValue;
	BOOL	m_bPort3OUTValue;
	BOOL	m_bPort4OUTValue;
	BOOL	m_bPort5OUTValue;
	BOOL	m_bPort6OUTValue;
	BOOL	m_bPort7OUTValue;
	BOOL	m_bPort8OUTValue;
	BOOL	m_bPort9OUTValue;
	//}}AFX_DATA

public:
	BOOL	m_bIsPort1IN;
	BOOL	m_bIsPort2IN;
	BOOL	m_bIsPort3IN;
	BOOL	m_bIsPort4IN;
	BOOL	m_bIsPort5IN;
	BOOL	m_bIsPort6IN;
	BOOL	m_bIsPort7IN;
	BOOL	m_bIsPort8IN;
	BOOL	m_bIsPort9IN;
	BOOL	m_bIsPort10IN;
	BOOL	m_bIsFlashSynEnable;
	struct PIO_indicator	m_pioIndicator;

	BOOL	m_bIsPortDirAdj;
	BOOL	m_bIsPortOutAdj;
	BOOL	m_bIsFlashSynAdj;

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneralIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPort1in();
	afx_msg void OnPort1out();
	afx_msg void OnFlashsyndisable();
	afx_msg void OnFlashsynenable();
	afx_msg void OnPort2in();
	afx_msg void OnPort2out();
	afx_msg void OnPort3in();
	afx_msg void OnPort3out();
	afx_msg void OnPort4in();
	afx_msg void OnPort4out();
	afx_msg void OnPort5in();
	afx_msg void OnPort5out();
	afx_msg void OnPort6in();
	afx_msg void OnPort6out();
	afx_msg void OnPort7in();
	afx_msg void OnPort7out();
	afx_msg void OnPort8in();
	afx_msg void OnPort8out();
	afx_msg void OnPort9in();
	afx_msg void OnPort9out();
	afx_msg void OnPort10in();
	afx_msg void OnPort10out();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALIODLG_H__7C5363E5_16CE_4EFB_A99B_4253A3BB23FB__INCLUDED_)
