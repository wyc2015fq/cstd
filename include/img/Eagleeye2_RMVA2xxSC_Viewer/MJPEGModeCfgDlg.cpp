// MJPEGModeCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "MJPEGModeCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMJPEGModeCfgDlg dialog


CMJPEGModeCfgDlg::CMJPEGModeCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMJPEGModeCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMJPEGModeCfgDlg)
	m_uiAutoShutterUpLimit = 20000;
	m_uiMJPEGShutterTime = 20000;
	m_uiBrightnessTH = 90;
	m_bIsManual = FALSE;
	m_fPGAUpLimit = 10.0f;
	//}}AFX_DATA_INIT
}


void CMJPEGModeCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMJPEGModeCfgDlg)
	DDX_Control(pDX, IDC_MANUAL, m_ctrlMJPEGManual);
	DDX_Control(pDX, IDC_AUTO, m_ctrlMJPEGAuto);
	DDX_Text(pDX, IDC_AUTOSHUTTERUPLIMIT, m_uiAutoShutterUpLimit);
	DDV_MinMaxUInt(pDX, m_uiAutoShutterUpLimit, 1, 120000);
	DDX_Text(pDX, IDC_MJPEGSHUTTERTIME, m_uiMJPEGShutterTime);
	DDV_MinMaxUInt(pDX, m_uiMJPEGShutterTime, 1, 120000);
	DDX_Text(pDX, IDC_MEANBRIGHTNESS, m_uiBrightnessTH);
	DDV_MinMaxUInt(pDX, m_uiBrightnessTH, 30, 250);
	DDX_Text(pDX, IDC_PGAUPLIMIT, m_fPGAUpLimit);
	DDV_MinMaxFloat(pDX, m_fPGAUpLimit, 0.f, 22.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMJPEGModeCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CMJPEGModeCfgDlg)
	ON_BN_CLICKED(IDC_AUTO, OnAuto)
	ON_BN_CLICKED(IDC_MANUAL, OnManual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMJPEGModeCfgDlg message handlers

BOOL CMJPEGModeCfgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_bIsManual){
		m_ctrlMJPEGManual.SetCheck(TRUE);
		m_ctrlMJPEGAuto.SetCheck(FALSE);
		GetDlgItem(IDC_MJPEGSHUTTERTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(FALSE);
	}
	else{
		m_ctrlMJPEGAuto.SetCheck(TRUE);
		m_ctrlMJPEGManual.SetCheck(FALSE);
		GetDlgItem(IDC_MJPEGSHUTTERTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMJPEGModeCfgDlg::OnAuto() 
{
	// TODO: Add your control notification handler code here
	m_bIsManual = FALSE;
	m_ctrlMJPEGAuto.SetCheck(TRUE);
	m_ctrlMJPEGManual.SetCheck(FALSE);
	GetDlgItem(IDC_MJPEGSHUTTERTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(TRUE);
}

void CMJPEGModeCfgDlg::OnManual() 
{
	// TODO: Add your control notification handler code here
	m_bIsManual = TRUE;
	m_ctrlMJPEGManual.SetCheck(TRUE);
	m_ctrlMJPEGAuto.SetCheck(FALSE);
	GetDlgItem(IDC_MJPEGSHUTTERTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(FALSE);
}

void CMJPEGModeCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
