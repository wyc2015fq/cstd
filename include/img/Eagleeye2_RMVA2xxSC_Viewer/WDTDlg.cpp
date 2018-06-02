// WDTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "WDTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWDTDlg dialog


CWDTDlg::CWDTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWDTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWDTDlg)
	m_iWDT = 35;
	//}}AFX_DATA_INIT
}


void CWDTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWDTDlg)
	DDX_Control(pDX, IDC_WDTENABLE, m_ctrlWDTEnable);
	DDX_Text(pDX, IDC_WDT, m_iWDT);
	DDV_MinMaxUInt(pDX, m_iWDT, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWDTDlg, CDialog)
	//{{AFX_MSG_MAP(CWDTDlg)
	ON_BN_CLICKED(IDC_WDTENABLE, OnWdtenable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWDTDlg message handlers

void CWDTDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CWDTDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_bIsWDTEnable){
		m_ctrlWDTEnable.SetCheck(TRUE);
		GetDlgItem(IDC_WDT)->EnableWindow(TRUE);
	}
	else{
		m_ctrlWDTEnable.SetCheck(FALSE);
		GetDlgItem(IDC_WDT)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWDTDlg::OnWdtenable() 
{
	// TODO: Add your control notification handler code here
	if( m_bIsWDTEnable ){
		m_bIsWDTEnable = FALSE;
		m_ctrlWDTEnable.SetCheck(FALSE);
		GetDlgItem(IDC_WDT)->EnableWindow(FALSE);
	}
	else{
		m_bIsWDTEnable = TRUE;
		m_ctrlWDTEnable.SetCheck(TRUE);
		GetDlgItem(IDC_WDT)->EnableWindow(TRUE);
	}
	
}
