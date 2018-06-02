// HFRModeCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "HFRModeCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHFRModeCfgDlg dialog


CHFRModeCfgDlg::CHFRModeCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHFRModeCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHFRModeCfgDlg)
	m_uiHFRShutterTime = 20000;
	m_uiAutoShutterUpLimit = 20000;
	m_fPGAUpLimit = 10.0f;
	m_uiBrightnessTH = 90;
	m_bIsManual = FALSE;
	//}}AFX_DATA_INIT
}


void CHFRModeCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHFRModeCfgDlg)
	DDX_Control(pDX, IDC_MANUAL, m_ctrlHFRManual);
	DDX_Control(pDX, IDC_AUTO, m_ctrlHFRAuto);
	DDX_Text(pDX, IDC_HFRSHUTTERTIME, m_uiHFRShutterTime);
	DDV_MinMaxUInt(pDX, m_uiHFRShutterTime, 1, 60000);
	DDX_Text(pDX, IDC_AUTOSHUTTERUPLIMIT, m_uiAutoShutterUpLimit);
	DDV_MinMaxUInt(pDX, m_uiAutoShutterUpLimit, 1, 60000);
	DDX_Text(pDX, IDC_PGAUPLIMIT, m_fPGAUpLimit);
	DDV_MinMaxFloat(pDX, m_fPGAUpLimit, 0.f, 22.f);
	DDX_Text(pDX, IDC_MEANBRIGHTNESS, m_uiBrightnessTH);
	DDV_MinMaxUInt(pDX, m_uiBrightnessTH, 30, 250);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHFRModeCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CHFRModeCfgDlg)
	ON_BN_CLICKED(IDC_AUTO, OnAuto)
	ON_BN_CLICKED(IDC_MANUAL, OnManual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHFRModeCfgDlg message handlers

void CHFRModeCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CHFRModeCfgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_bIsManual){
		m_ctrlHFRManual.SetCheck(TRUE);
		m_ctrlHFRAuto.SetCheck(FALSE);
		GetDlgItem(IDC_HFRSHUTTERTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(FALSE);
	}
	else{
		m_ctrlHFRManual.SetCheck(FALSE);
		m_ctrlHFRAuto.SetCheck(TRUE);
		GetDlgItem(IDC_HFRSHUTTERTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(TRUE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHFRModeCfgDlg::OnAuto() 
{
	// TODO: Add your control notification handler code here
	m_bIsManual = FALSE;
	m_ctrlHFRAuto.SetCheck(TRUE);
	m_ctrlHFRManual.SetCheck(FALSE);
	GetDlgItem(IDC_HFRSHUTTERTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(TRUE);
}

void CHFRModeCfgDlg::OnManual() 
{
	// TODO: Add your control notification handler code here
	m_bIsManual = TRUE;
	m_ctrlHFRManual.SetCheck(TRUE);
	m_ctrlHFRAuto.SetCheck(FALSE);
	GetDlgItem(IDC_HFRSHUTTERTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(FALSE);
}
