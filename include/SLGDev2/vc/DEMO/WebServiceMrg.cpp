// WebServiceMrg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "WebServiceMrg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebServiceMrg dialog


CWebServiceMrg::CWebServiceMrg(const char* ip,CWnd* pParent /*=NULL*/)
	: CDialog(CWebServiceMrg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebServiceMrg)
	m_strWebURL1 = _T("");
	m_strWebURL2 = _T("");
	m_bLoad2 = FALSE;
	m_bLoad = FALSE;
	m_bSyncT1 = FALSE;
	m_bSyncTime2 = FALSE;
	m_bWRecord = FALSE;
	m_bWRecord2 = FALSE;
	m_bWUser = FALSE;
	m_bWUser2 = FALSE;
	m_bOnlyTxt = FALSE;
	m_bOnlyTxt2 = FALSE;
	m_bServiceVerify = FALSE;
	m_bServiceVerify2 = FALSE;
	m_bEnableWebService = FALSE;
	m_bEnableWebService2 = FALSE;
	//}}AFX_DATA_INIT

	memset(m_IP, 0x0, sizeof(m_IP));
	memcpy(m_IP, ip, sizeof(m_IP));
}


void CWebServiceMrg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebServiceMrg)
	DDX_Control(pDX, IDC_EnaleWebServiceChk2, m_ctrEnableWebService2);
	DDX_Control(pDX, IDC_EnaleWebServiceChk, m_ctrEnableWebService);
	DDX_Control(pDX, IDC_ServiceVerifyChk2, m_ctrServiceVerify2);
	DDX_Control(pDX, IDC_ServiceVerifyChk, m_ctrServiceVerify);
	DDX_Control(pDX, IDC_OnlyUpLoadTextChk2, m_ctrOnlyTxt2);
	DDX_Control(pDX, IDC_OnlyUpLoadTextChk, m_ctrOnlyTxt);
	DDX_Control(pDX, IDC_SubMask, m_ctrSubMask);
	DDX_Control(pDX, IDC_MulitIP, m_ctrMulitIP);
	DDX_Control(pDX, IDC_GateWay, m_ctrGateWay);
	DDX_Control(pDX, IDC_SyncTimeChk, m_ctrSyncTimeChk);
	DDX_Control(pDX, IDC_LoadChk2, m_ctrLoadChk2);
	DDX_Control(pDX, IDC_LoadChk, m_ctrLoadChk);
	DDX_Control(pDX, IDC_WriteUserChk2, m_ctrWUserChk2);
	DDX_Control(pDX, IDC_WriteUserChk, m_ctrWUserChk);
	DDX_Control(pDX, IDC_WriteRecord2, m_ctrWRecord2);
	DDX_Control(pDX, IDC_WriteRecord, m_ctrWRecord);
	DDX_Control(pDX, IDC_SyncTimeChk2, m_ctrSyncTimeChk2);
	DDX_Text(pDX, IDC_WebURL1Edt, m_strWebURL1);
	DDX_Text(pDX, IDC_WebURL1Edt2, m_strWebURL2);
	DDX_Check(pDX, IDC_LoadChk2, m_bLoad2);
	DDX_Check(pDX, IDC_LoadChk, m_bLoad);
	DDX_Check(pDX, IDC_SyncTimeChk, m_bSyncT1);
	DDX_Check(pDX, IDC_SyncTimeChk2, m_bSyncTime2);
	DDX_Check(pDX, IDC_WriteRecord, m_bWRecord);
	DDX_Check(pDX, IDC_WriteRecord2, m_bWRecord2);
	DDX_Check(pDX, IDC_WriteUserChk, m_bWUser);
	DDX_Check(pDX, IDC_WriteUserChk2, m_bWUser2);
	DDX_Check(pDX, IDC_OnlyUpLoadTextChk, m_bOnlyTxt);
	DDX_Check(pDX, IDC_OnlyUpLoadTextChk2, m_bOnlyTxt2);
	DDX_Check(pDX, IDC_ServiceVerifyChk, m_bServiceVerify);
	DDX_Check(pDX, IDC_ServiceVerifyChk2, m_bServiceVerify2);
	DDX_Check(pDX, IDC_EnaleWebServiceChk, m_bEnableWebService);
	DDX_Check(pDX, IDC_EnaleWebServiceChk2, m_bEnableWebService2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebServiceMrg, CDialog)
	//{{AFX_MSG_MAP(CWebServiceMrg)
	ON_BN_CLICKED(IDC_OkBnt, OnOkBnt)
	ON_BN_CLICKED(IDC_CancelBnt, OnCancelBnt)
	ON_MESSAGE(WM_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebServiceMrg message handlers
//IP控件转字符串
CString dwIP2csIP( DWORD  ip )   
{   
	BYTE		field[4] = {0};  
	CString		csIP = _T("");   
	
	for( int i=0; i<4; i++ )    
	{   
		field[i]=(BYTE)(0x000000FF&ip>>i*8);   
	}   
	csIP.Format(_T("%d.%d.%d.%d"),field[3],field[2],field[1],field[0]);   
	return   csIP;   
}

//字符串转IP对象
DWORD csIP2dwIP( TCHAR* ip )   
{   
	int len=_tcslen( ip );   
	if(len<6||len>15)   return 0;   
	DWORD   dwip=0;   
	TCHAR* p = NULL; 
	BYTE dd = 0x00; 
	TCHAR field[4] = {0};   
	int   i=0,j=3;   
	for( p=(TCHAR*)ip; *p!=NULL; p++ )   
	{   
		if(*p>='0'   &&   *p<='9')   field[i++]=*p;   
		if( *(p+1) =='.' || (*(p+1) == '\0') )   
		{ 
			field[i] = 0; 
			i = 0;   
			dd = (BYTE)::_ttoi(field);
			dwip = dwip|((DWORD)dd<<(j--)*8);   
		}   
		
	}   
	if(j!=-1)   return 0;    
	return   dwip;   
    
}
void CWebServiceMrg::OnOkBnt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strIP;
	CString strGate;
	CString strSubMask;
	CString strMuiltIP;
	CString strDNS;
	CString strURL1;
	int nFlag1=0;
	CString strURL2;
	int nFlag2=0;
    DEV_NETPRO dev_att;

	memset( &dev_att, 0, sizeof(DEV_NETPRO));
	
	GetInfo( strIP, strGate, strSubMask, strMuiltIP, strDNS, strURL1, nFlag1, strURL2, nFlag2);

	memcpy( dev_att.m_NetMask, strSubMask.GetBuffer(0), strlen( strSubMask.GetBuffer(0) ) );
	memcpy( dev_att.m_MIPAdr, strMuiltIP.GetBuffer(0), strlen( strMuiltIP.GetBuffer(0) ) );
	memcpy( dev_att.m_Dns, strDNS.GetBuffer(0), strlen( strDNS.GetBuffer(0) ) );
	memcpy( dev_att.m_IPAdr, strIP.GetBuffer(0), strlen( strIP.GetBuffer(0) ) );
    memcpy( dev_att.m_GateWay, strGate.GetBuffer(0), strlen( strGate.GetBuffer(0) ) );
	memcpy( dev_att.m_WebUrl2, strURL2.GetBuffer(0), strlen( strURL2.GetBuffer(0) ) );
    memcpy( dev_att.m_WebUrl1, strURL1.GetBuffer(0), strlen( strURL1.GetBuffer(0) ) );
	dev_att.m_WebPro1 = (char) nFlag1;
	dev_att.m_WebPro2 = (char) nFlag2;

	if ( CPM_NetPro( m_IP, &dev_att) )
		AfxMessageBox( _T("Data uploaded successfully") );
	else
		AfxMessageBox( _T("Data Upload Failed") );

}

void CWebServiceMrg::OnCancelBnt() 
{
	// TODO: Add your control notification handler code here
	CPM_RegOperResultNotifyCB( &CWebServiceMrg::DevOperResult, this );
	if ( !(CPM_NetPro(m_IP, NULL)) )
	{
		AfxMessageBox( _T("Data transmission request failed") );
	}
}

BOOL CWebServiceMrg::DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext )
{
    if ( eType == DEV_NETPRO_OPER )
    {
        /*assert(eFlag == DEV_OPER_DOWNLOAD);*/
        CWebServiceMrg* pthis = (CWebServiceMrg*)pvContext;
        if ( OPER_SUCCESS==eResult && pvContent/* &&  eType == DEV_OPER_DOWNLOAD*/ )
			pthis->DevNetWebConfigInfoNotify(cszDevAddr, *((DEV_NETPRO*)pvContent));
    }
	return TRUE;
}

void CWebServiceMrg::DevNetWebConfigInfoNotify( const char* dev_addr, const DEV_NETPRO& dev_att )
{
    SetInfo( dev_att.m_IPAdr, dev_att.m_GateWay, dev_att.m_NetMask, dev_att.m_MIPAdr, dev_att.m_Dns, dev_att.m_WebUrl1, dev_att.m_WebPro1 , dev_att.m_WebUrl2, dev_att.m_WebPro2 );
    ::PostMessage(this->GetSafeHwnd(), WM_UPDATE, 0, 0);
}

LRESULT CWebServiceMrg::OnUpdate(WPARAM wparam, LPARAM lpparam)
{
	UpdateData(lpparam);
	return 0;
}

BOOL CWebServiceMrg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CPM_RegOperResultNotifyCB( &CWebServiceMrg::DevOperResult, this );
	CPM_NetPro(m_IP, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
