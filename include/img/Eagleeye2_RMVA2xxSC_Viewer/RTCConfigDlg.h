#if !defined(AFX_RTCCONFIGDLG_H__F755DD51_91C8_4BFF_8287_29968C716A6F__INCLUDED_)
#define AFX_RTCCONFIGDLG_H__F755DD51_91C8_4BFF_8287_29968C716A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RTCConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRTCConfigDlg dialog

class CRTCConfigDlg : public CDialog
{
// Construction
public:
	CRTCConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRTCConfigDlg)
	enum { IDD = IDD_RTCDLG };
	UINT	m_iDay;
	UINT	m_iHour;
	UINT	m_iMinute;
	UINT	m_iMonth;
	UINT	m_iSecond;
	UINT	m_iWeek;
	UINT	m_iYear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTCConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRTCConfigDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTCCONFIGDLG_H__F755DD51_91C8_4BFF_8287_29968C716A6F__INCLUDED_)
