// WhiteBalanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "WhiteBalanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhiteBalanceDlg dialog


CWhiteBalanceDlg::CWhiteBalanceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWhiteBalanceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWhiteBalanceDlg)
	m_uiWBType = 0; //0: daylight; 1: fluorescence; 2: without whitebalance
	//}}AFX_DATA_INIT
}


void CWhiteBalanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhiteBalanceDlg)
	DDX_Control(pDX, IDC_WBFLUORESCENCE, m_ctrlFluorescence);
	DDX_Control(pDX, IDC_WBDAYLIGHT, m_ctrlDaylight);
	DDX_Control(pDX, IDC_ORGDATA, m_ctrlOrgData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhiteBalanceDlg, CDialog)
	//{{AFX_MSG_MAP(CWhiteBalanceDlg)
	ON_BN_CLICKED(IDC_WBDAYLIGHT, OnWbdaylight)
	ON_BN_CLICKED(IDC_WBFLUORESCENCE, OnWbfluorescence)
	ON_BN_CLICKED(IDC_ORGDATA, OnOrgdata)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhiteBalanceDlg message handlers

BOOL CWhiteBalanceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_uiWBType) {
	case 0:
		m_ctrlDaylight.SetCheck(TRUE);
		m_ctrlFluorescence.SetCheck(FALSE);
		m_ctrlOrgData.SetCheck(FALSE);
		break;
	case 1:
		m_ctrlFluorescence.SetCheck(TRUE);
		m_ctrlDaylight.SetCheck(FALSE);
		m_ctrlOrgData.SetCheck(FALSE);
		break;
	case 2:
		m_ctrlOrgData.SetCheck(TRUE);
		m_ctrlDaylight.SetCheck(FALSE);
		m_ctrlFluorescence.SetCheck(FALSE);
	default:
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWhiteBalanceDlg::OnWbdaylight() 
{
	// TODO: Add your control notification handler code here
	m_uiWBType = 0;
	m_ctrlDaylight.SetCheck(TRUE);
	m_ctrlFluorescence.SetCheck(FALSE);
	m_ctrlOrgData.SetCheck(FALSE);
}

void CWhiteBalanceDlg::OnWbfluorescence() 
{
	// TODO: Add your control notification handler code here
	m_uiWBType = 1;
	m_ctrlFluorescence.SetCheck(TRUE);
	m_ctrlDaylight.SetCheck(FALSE);
	m_ctrlOrgData.SetCheck(FALSE);
}

void CWhiteBalanceDlg::OnOrgdata() 
{
	// TODO: Add your control notification handler code here
	m_uiWBType = 2;
	m_ctrlOrgData.SetCheck(TRUE);
	m_ctrlDaylight.SetCheck(FALSE);
	m_ctrlFluorescence.SetCheck(FALSE);
}
