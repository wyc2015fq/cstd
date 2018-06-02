#if !defined(AFX_CREATEDYNAMICDLG_H__1C441CBD_57FD_4B08_B669_A90629873426__INCLUDED_)
#define AFX_CREATEDYNAMICDLG_H__1C441CBD_57FD_4B08_B669_A90629873426__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateDynamicDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateDynamicDlg dialog
#define DMCMDSHOW    WM_USER+2000
class CCreateDynamicDlg : public CDialog
{
// Construction
public:
	CCreateDynamicDlg( CString strIP, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateDynamicDlg)
	enum { IDD = IDD_CreateDynamicDlg };
	CDateTimeCtrl	m_ctrDPT;
	CButton	m_ctrCheck;
	CString	m_strDevID;
	CTime	m_time;
	BOOL	m_check;
	CTime	m_date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateDynamicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
		//static void DevOperResult( const char* server_addr, const char* dev_addr, NET_OPER_CMD oper_type, NET_OPER_RESULT eRt, void* oper_content, int serial_num, void* param );
	static BOOL CALLBACK DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext );
protected:
	void DevBaseInfoNotify( const char* dev_addr, const DEV_BASEINFO& dev_att );
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateDynamicDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	afx_msg LRESULT  DevShowCMD( WPARAM wparam, LPARAM lpparam );
	DECLARE_MESSAGE_MAP()
private:
	CString  m_strIP;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEDYNAMICDLG_H__1C441CBD_57FD_4B08_B669_A90629873426__INCLUDED_)
