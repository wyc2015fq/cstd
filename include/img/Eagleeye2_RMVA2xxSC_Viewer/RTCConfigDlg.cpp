// RTCConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "RTCConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTCConfigDlg dialog


CRTCConfigDlg::CRTCConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRTCConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRTCConfigDlg)
	m_iDay = 22;
	m_iHour = 0;
	m_iMinute = 0;
	m_iMonth = 1;
	m_iSecond = 0;
	m_iWeek = 1;
	m_iYear = 2007;
	//}}AFX_DATA_INIT
}


void CRTCConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRTCConfigDlg)
	DDX_Text(pDX, IDC_RTCDAY, m_iDay);
	DDV_MinMaxUInt(pDX, m_iDay, 1, 31);
	DDX_Text(pDX, IDC_RTCHOUR, m_iHour);
	DDV_MinMaxUInt(pDX, m_iHour, 0, 24);
	DDX_Text(pDX, IDC_RTCMINUTE, m_iMinute);
	DDV_MinMaxUInt(pDX, m_iMinute, 0, 59);
	DDX_Text(pDX, IDC_RTCMONTH, m_iMonth);
	DDV_MinMaxUInt(pDX, m_iMonth, 1, 12);
	DDX_Text(pDX, IDC_RTCSECOND, m_iSecond);
	DDV_MinMaxUInt(pDX, m_iSecond, 0, 59);
	DDX_Text(pDX, IDC_RTCWEEK, m_iWeek);
	DDV_MinMaxUInt(pDX, m_iWeek, 1, 7);
	DDX_Text(pDX, IDC_RTCYEAR, m_iYear);
	DDV_MinMaxUInt(pDX, m_iYear, 0, 9999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRTCConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CRTCConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTCConfigDlg message handlers

BOOL CRTCConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	m_iYear = systime.wYear;
	m_iMonth = systime.wMonth;
	m_iDay = systime.wDay;
	m_iWeek = systime.wDayOfWeek;
	m_iHour = systime.wHour;
	m_iMinute = systime.wMinute;
	m_iSecond = systime.wSecond;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRTCConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
