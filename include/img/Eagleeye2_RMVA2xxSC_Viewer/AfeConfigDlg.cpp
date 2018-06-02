// AfeConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "AfeConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAfeConfigDlg dialog


CAfeConfigDlg::CAfeConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAfeConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAfeConfigDlg)
	m_fAfeVGAGain = 0.0f;
	//}}AFX_DATA_INIT
}


void CAfeConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAfeConfigDlg)
	DDX_Text(pDX, IDC_AFEVGAGAIN, m_fAfeVGAGain);
	DDV_MinMaxFloat(pDX, m_fAfeVGAGain, 0.f, 36.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAfeConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CAfeConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAfeConfigDlg message handlers

BOOL CAfeConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAfeConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CDialog::OnOK();
}
