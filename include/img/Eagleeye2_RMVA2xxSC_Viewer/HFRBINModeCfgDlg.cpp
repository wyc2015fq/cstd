// HFRBINModeCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "HFRBINModeCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHFRBINModeCfgDlg dialog


CHFRBINModeCfgDlg::CHFRBINModeCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHFRBINModeCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHFRBINModeCfgDlg)
	m_uiAutoShutterUpLimit = 20000;
	m_uiHFRBINShutterTime = 20000;
	m_uiBrightnessTH = 90;
	m_fPGAUpLimit = 15.0f;
	m_bIsManual = FALSE;
	//}}AFX_DATA_INIT
}


void CHFRBINModeCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHFRBINModeCfgDlg)
	DDX_Control(pDX, IDC_MANUAL, m_ctrlHFRBINManual);
	DDX_Control(pDX, IDC_AUTO, m_ctrlHFRBINAuto);
	DDX_Text(pDX, IDC_AUTOSHUTTERUPLIMIT, m_uiAutoShutterUpLimit);
	DDV_MinMaxUInt(pDX, m_uiAutoShutterUpLimit, 1, 60000);
	DDX_Text(pDX, IDC_HFRBINSHUTTERTIME, m_uiHFRBINShutterTime);
	DDX_Text(pDX, IDC_MEANBRIGHTNESS, m_uiBrightnessTH);
	DDV_MinMaxUInt(pDX, m_uiBrightnessTH, 30, 250);
	DDX_Text(pDX, IDC_PGAUPLIMIT, m_fPGAUpLimit);
	DDV_MinMaxFloat(pDX, m_fPGAUpLimit, 0.f, 22.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHFRBINModeCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CHFRBINModeCfgDlg)
	ON_BN_CLICKED(IDC_MANUAL, OnManual)
	ON_BN_CLICKED(IDC_AUTO, OnAuto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHFRBINModeCfgDlg message handlers

BOOL CHFRBINModeCfgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_bIsManual){
		m_ctrlHFRBINManual.SetCheck(TRUE);
		m_ctrlHFRBINAuto.SetCheck(FALSE);
		GetDlgItem(IDC_HFRBINSHUTTERTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(FALSE);
	}
	else{
		m_ctrlHFRBINManual.SetCheck(FALSE);
		m_ctrlHFRBINAuto.SetCheck(TRUE);
		GetDlgItem(IDC_HFRBINSHUTTERTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHFRBINModeCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CHFRBINModeCfgDlg::OnManual() 
{
	// TODO: Add your control notification handler code here
	m_bIsManual = TRUE;
	m_ctrlHFRBINManual.SetCheck(TRUE);
	m_ctrlHFRBINAuto.SetCheck(FALSE);
	GetDlgItem(IDC_HFRBINSHUTTERTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(FALSE);
}

void CHFRBINModeCfgDlg::OnAuto() 
{
	// TODO: Add your control notification handler code here
	m_bIsManual = FALSE;
	m_ctrlHFRBINAuto.SetCheck(TRUE);
	m_ctrlHFRBINManual.SetCheck(FALSE);
	GetDlgItem(IDC_HFRBINSHUTTERTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTOSHUTTERUPLIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PGAUPLIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_MEANBRIGHTNESS)->EnableWindow(TRUE);
}
