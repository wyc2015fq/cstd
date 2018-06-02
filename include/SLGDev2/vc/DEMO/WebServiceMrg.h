#if !defined(AFX_WEBSERVICEMRG_H__390ACC37_04AB_437A_B93B_72D6A8D5A526__INCLUDED_)
#define AFX_WEBSERVICEMRG_H__390ACC37_04AB_437A_B93B_72D6A8D5A526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebServiceMrg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWebServiceMrg dialog

#define DEV_WEB_LOADUSER    0x0001   //加载用户
#define DEV_WEB_WRITEUSER   0x0002   //写用户
#define DEV_WEB_WRITEREC    0x0004   //写记录
#define DEV_WEB_SYNCTIME    0x0008   //同步时间
#define DEV_WEB_JUDGE       0x10   //识别成功后，由服务端权限验证
#define DEV_WEB_RECORD_TXT  0x20   //记录自动上传时，只上传文字信息
#define DEV_WEB_ENABLE      0x80   // 
CString dwIP2csIP( DWORD  ip )  ;
DWORD csIP2dwIP( TCHAR* ip )  ;
class CWebServiceMrg : public CDialog
{
// Construction
public:
	CWebServiceMrg(const char* ip,CWnd* pParent = NULL);   // standard constructor
public:
	void SetInfo(CString strIP, CString strGate,CString strSubMask,CString strMuiltIP,CString strDNS, CString strURL1,int nFlag1,CString strURL2,int nFlag2)
	{
		DWORD dIP = 0;
		m_strWebURL1 = strURL1;
		m_strWebURL2 = strURL2;

		GetDlgItem(IDC_IPAdr)->SetWindowText(strIP);

		dIP = csIP2dwIP(  (TCHAR*) strMuiltIP.GetBuffer(0));
		m_ctrMulitIP.SetAddress(dIP);
		
		dIP = csIP2dwIP(  (TCHAR*) strSubMask.GetBuffer(0));
		m_ctrSubMask.SetAddress(dIP);
		
	    dIP = csIP2dwIP(  (TCHAR*) strGate.GetBuffer(0));
		m_ctrGateWay.SetAddress(dIP);
		
		GetDlgItem(IDC_DNS)->SetWindowText(strDNS);

		if ( (nFlag1 & DEV_WEB_LOADUSER) == DEV_WEB_LOADUSER )
			m_bLoad = TRUE;
		else
		   m_bLoad = FALSE;
		
		if ( (nFlag1 & DEV_WEB_WRITEUSER) == DEV_WEB_WRITEUSER )
			m_bWUser = TRUE;
		else
			m_bWUser = FALSE;
		
		if ( (nFlag1 & DEV_WEB_WRITEREC) == DEV_WEB_WRITEREC )
			m_bWRecord = TRUE;
		else
			m_bWRecord = FALSE;
		
		if ( (nFlag1 &  DEV_WEB_SYNCTIME) ==  DEV_WEB_SYNCTIME )
			m_bSyncT1 = TRUE;
		else
			m_bSyncT1 = FALSE;

		if ( (nFlag1 &  DEV_WEB_JUDGE) ==  DEV_WEB_JUDGE )
			m_bServiceVerify = TRUE;
		else
			m_bServiceVerify = FALSE;

		if ( (nFlag1 &  DEV_WEB_RECORD_TXT) ==  DEV_WEB_RECORD_TXT )
			m_bOnlyTxt = TRUE;
		else
			m_bOnlyTxt = FALSE;

		if ( (nFlag1 &  DEV_WEB_ENABLE) ==  DEV_WEB_ENABLE )
			m_bEnableWebService = TRUE;
		else
			m_bEnableWebService = FALSE;
		
		if ( (nFlag2 & DEV_WEB_LOADUSER) == DEV_WEB_LOADUSER )
			m_bLoad2 = TRUE;
		else
		   m_bLoad2 = FALSE;
		
		if ( (nFlag2 & DEV_WEB_WRITEUSER) == DEV_WEB_WRITEUSER )
			m_bWUser2 = TRUE;
		else
			m_bWUser2 = FALSE;
		
		if ( (nFlag2 & DEV_WEB_WRITEREC) == DEV_WEB_WRITEREC )
			m_bWRecord2 = TRUE;
		else
			m_bWRecord2 = FALSE;
		
		if ( (nFlag2 &  DEV_WEB_SYNCTIME) ==  DEV_WEB_SYNCTIME )
			m_bSyncTime2 = TRUE;
		else
			m_bSyncTime2 = FALSE;

		if ( (nFlag2 &  DEV_WEB_JUDGE) ==  DEV_WEB_JUDGE )
			m_bServiceVerify2 = TRUE;
		else
			m_bServiceVerify2 = FALSE;

		if ( (nFlag2 &  DEV_WEB_RECORD_TXT) ==  DEV_WEB_RECORD_TXT )
			m_bOnlyTxt2 = TRUE;
		else
			m_bOnlyTxt2 = FALSE;

		if ( (nFlag2 &  DEV_WEB_ENABLE) ==  DEV_WEB_ENABLE )
			m_bEnableWebService2 = TRUE;
		else
			m_bEnableWebService2 = FALSE;
	}

	void GetInfo(CString& strIP, CString& strGate,CString& strSubMask,CString& strMuiltIP,CString& strDNS, CString& strURL1,int& nFlag1,CString& strURL2,int& nFlag2)
	{
		DWORD dIP = 0;

      
	    GetDlgItem(IDC_IPAdr)->GetWindowText(strIP);
		

		m_ctrMulitIP.GetAddress(dIP);
		strMuiltIP = dwIP2csIP(dIP);

		m_ctrSubMask.GetAddress(dIP);
		strSubMask = dwIP2csIP(dIP);
	
		m_ctrGateWay.GetAddress(dIP);
		strGate = dwIP2csIP(dIP);

		GetDlgItem(IDC_DNS)->GetWindowText(strDNS);

		m_strWebURL1.TrimLeft();
		m_strWebURL1.TrimRight();
		m_strWebURL2.TrimLeft();
		m_strWebURL2.TrimRight();

		CString   strURL ;
		int i = 0;

		strURL = m_strWebURL1;
		if ( ( i = strURL.Find("http:",0) ) >=0)
			m_strWebURL1 = strURL.Right( strURL.GetLength() - i );

		
		strURL = m_strWebURL2;
		if ( (i = strURL.Find("http:",0) ) >= 0 )
			m_strWebURL2 = strURL.Right( strURL.GetLength() - i );

		strURL1 = m_strWebURL1;
		strURL2 = m_strWebURL2;

		int nPro = 0;
		if ( m_bLoad )
			nPro = nPro + DEV_WEB_LOADUSER;
		if ( m_bWUser )
			nPro = nPro + DEV_WEB_WRITEUSER;
		if ( m_bWRecord )
			nPro = nPro + DEV_WEB_WRITEREC;
		if( m_bSyncT1 )
			nPro = nPro +  DEV_WEB_SYNCTIME;
		if( m_bServiceVerify )
			nPro = nPro +  DEV_WEB_JUDGE;
		if( m_bOnlyTxt )
			nPro = nPro +  DEV_WEB_RECORD_TXT;
		if( m_bEnableWebService )
			nPro = nPro +  DEV_WEB_ENABLE;
		nFlag1 = nPro;
		
		nPro = 0;
		if ( m_bLoad2 )
			nPro = nPro + DEV_WEB_LOADUSER;
		if ( m_bWUser2 )
			nPro = nPro + DEV_WEB_WRITEUSER;
		if ( m_bWRecord2 )
			nPro = nPro + DEV_WEB_WRITEREC;
		if( m_bSyncTime2 )
			nPro = nPro +  DEV_WEB_SYNCTIME;
		if( m_bServiceVerify2 )
			nPro = nPro +  DEV_WEB_JUDGE;
		if( m_bOnlyTxt2 )
			nPro = nPro +  DEV_WEB_RECORD_TXT;
		if( m_bEnableWebService2 )
			nPro = nPro +  DEV_WEB_ENABLE;
		nFlag2 = nPro;

	}
// Dialog Data
	//{{AFX_DATA(CWebServiceMrg)
	enum { IDD = IDD_WebServiceMrg };
	CButton	m_ctrEnableWebService2;
	CButton	m_ctrEnableWebService;
	CButton	m_ctrServiceVerify2;
	CButton	m_ctrServiceVerify;
	CButton	m_ctrOnlyTxt2;
	CButton	m_ctrOnlyTxt;
	CIPAddressCtrl	m_ctrSubMask;
	CIPAddressCtrl	m_ctrMulitIP;
	CIPAddressCtrl	m_ctrGateWay;
	CButton	m_ctrSyncTimeChk;
	CButton	m_ctrLoadChk2;
	CButton	m_ctrLoadChk;
	CButton	m_ctrWUserChk2;
	CButton	m_ctrWUserChk;
	CButton	m_ctrWRecord2;
	CButton	m_ctrWRecord;
	CButton	m_ctrSyncTimeChk2;
	CString	m_strWebURL1;
	CString	m_strWebURL2;
	BOOL	m_bLoad2;
	BOOL	m_bLoad;
	BOOL	m_bSyncT1;
	BOOL	m_bSyncTime2;
	BOOL	m_bWRecord;
	BOOL	m_bWRecord2;
	BOOL	m_bWUser;
	BOOL	m_bWUser2;
	BOOL	m_bOnlyTxt;
	BOOL	m_bOnlyTxt2;
	BOOL	m_bServiceVerify;
	BOOL	m_bServiceVerify2;
	BOOL	m_bEnableWebService;
	BOOL	m_bEnableWebService2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebServiceMrg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWebServiceMrg)
	afx_msg void OnOkBnt();
	afx_msg void OnCancelBnt();
	afx_msg LRESULT OnUpdate(WPARAM wparam, LPARAM lpparam);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  m_sURL1;
	int		 m_iFlag1;
public:
	//static void DevOperResult( const char* server_addr, const char* dev_addr, NET_OPER_CMD oper_type, NET_OPER_RESULT eRt, void* oper_content, int serial_num, void* param );
    static BOOL CALLBACK DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext );
protected:
	void DevNetWebConfigInfoNotify( const char* dev_addr, const DEV_NETPRO& dev_att );
private:
	void InitCtrl();
	char m_IP[64];
	char m_ServerIP[16];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBSERVICEMRG_H__390ACC37_04AB_437A_B93B_72D6A8D5A526__INCLUDED_)
