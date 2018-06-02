// CreateDynamicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "CreateDynamicDlg.h"
#include "dynamicPassword.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateDynamicDlg dialog
#define CTIME_MIN	CTime( 2001, 1, 1, 0, 0, 00 )
#define CTIME_MAX	CTime( 2030, 12, 31, 23, 59, 59 )

CCreateDynamicDlg::CCreateDynamicDlg( CString sIP, CWnd* pParent /*=NULL*/)
	: CDialog( CCreateDynamicDlg::IDD, pParent)

{
	//{{AFX_DATA_INIT(CCreateDynamicDlg)
	m_strDevID = _T("");
	m_time = CTime::GetCurrentTime();
	m_check = FALSE;
	m_date =  CTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_strIP =sIP;
}


void CCreateDynamicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateDynamicDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_ctrDPT);
	DDX_Control(pDX, IDC_CHECK1, m_ctrCheck);
	DDX_Text(pDX, IDC_TIMEPWD, m_strDevID);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time);
	DDX_Check(pDX, IDC_CHECK1, m_check);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_date);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateDynamicDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateDynamicDlg)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
	ON_MESSAGE( DMCMDSHOW, DevShowCMD )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateDynamicDlg message handlers

void CCreateDynamicDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if ( m_strDevID.IsEmpty() )
	{
		AfxMessageBox(_T("Please enter the Device No.!"));
		return;
	}

	CTime objCurTime = CTime::GetCurrentTime();
	dpalg_t dp[1] = { objCurTime.GetYear(), objCurTime.GetMonth(), objCurTime.GetDay(), objCurTime.GetHour(), objCurTime.GetMinute(), objCurTime.GetSecond() };
	memset( &dp, 0, sizeof(dpalg_t) );
	objCurTime =   CTime::GetCurrentTime();
	if ( !m_ctrCheck.GetCheck() )
	{
		dp[0].year  =   (unsigned int) m_date.GetYear();
		dp[0].month  =  (unsigned int) m_date.GetMonth();
		dp[0].day    =  (unsigned int) m_date.GetDay();
		dp[0].hour   =  (unsigned int)  m_time.GetHour();
		dp[0].minute =  (unsigned int)  m_time.GetMinute();
		dp[0].second =  (unsigned int) m_time.GetSecond();
	}
	else
	{	
		objCurTime   =  CTime::GetCurrentTime();
		dp[0].year   =  (unsigned int) objCurTime.GetYear();
		dp[0].month  =  (unsigned int) objCurTime.GetMonth();
		dp[0].day    =  (unsigned int) objCurTime.GetDay();
		dp[0].hour   =  (unsigned int) objCurTime.GetHour();
		dp[0].minute =  (unsigned int) objCurTime.GetMinute();
		dp[0].second =  (unsigned int) objCurTime.GetSecond();
	}
	
	int codelen = m_strDevID.GetLength();
	unsigned int tt = 0;
	const char* cP = m_strDevID;
// 	CString strDD;
// 	strDD.Format(_T("%u-%u-%u %u:%u:%u"),dp[0].year , dp[0].month , dp[0].day,  dp[0].hour , dp[0].minute,  dp[0].second);
// 	AfxMessageBox(strDD);
	int nIn = dp_encode(dp, (const uchar*) cP, codelen, &tt);
	CString str;
	str.Format("%010u", tt);
	GetDlgItem(IDC_EDIT2)->SetWindowText(" ");
	GetDlgItem(IDC_EDIT2)->SetWindowText(str);
	UpdateData(FALSE);
}

void CCreateDynamicDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCreateDynamicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CTime TimeMax = CTIME_MAX;
	CTime TimeMin = CTIME_MIN;
	GetDlgItem(IDC_EDIT2)->SetWindowText(" ");
	m_ctrDPT.SetRange( &TimeMin,  &TimeMax );
	
	CPM_RegOperResultNotifyCB( &CCreateDynamicDlg::DevOperResult, this );

	if ( !m_strIP.IsEmpty() )
		CPM_DLDevBaseInfo(m_strIP);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCreateDynamicDlg::DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext )
{

    if ( eType == DEV_BASEINFO_OPER )
    {
        assert(eFlag == DEV_OPER_DOWNLOAD);
        CCreateDynamicDlg* pthis = (CCreateDynamicDlg*)pvContext;
        if ( OPER_SUCCESS==eResult && pvContent )
        {
			pthis->DevBaseInfoNotify(cszDevAddr, *((DEV_BASEINFO*)pvContent));
		}
    }
	return TRUE;
}

void CCreateDynamicDlg::DevBaseInfoNotify( const char* dev_addr, const DEV_BASEINFO& dev_att )
{
	m_strDevID = dev_att.m_SN.m_ID;
	PostMessage(DMCMDSHOW,0,0);
}
LRESULT  CCreateDynamicDlg::DevShowCMD( WPARAM wparam, LPARAM lpparam )
{
	UpdateData(FALSE);
	return 0;
}

void CCreateDynamicDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow( !m_ctrCheck.GetCheck() );
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow( !m_ctrCheck.GetCheck() );
	UpdateData(FALSE);
}
