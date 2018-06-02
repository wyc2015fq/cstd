// TargetIPAddressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "TargetIPAddressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTargetIPAddressDlg dialog


CTargetIPAddressDlg::CTargetIPAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTargetIPAddressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTargetIPAddressDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_byIPAdd1 = 192;
	m_byIPAdd2 = 168;
	m_byIPAdd3 = 1;
	m_byIPAdd4 = 218;
	//}}AFX_DATA_INIT
}


void CTargetIPAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetIPAddressDlg)
	DDX_Control(pDX, IDC_TARGETIPADDRESS, m_ctrlTargetIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTargetIPAddressDlg, CDialog)
	//{{AFX_MSG_MAP(CTargetIPAddressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetIPAddressDlg message handlers

BOOL CTargetIPAddressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlTargetIP.SetAddress(m_byIPAdd1, m_byIPAdd2, m_byIPAdd3, m_byIPAdd4);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTargetIPAddressDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_ctrlTargetIP.GetAddress(m_byIPAdd1, m_byIPAdd2, m_byIPAdd3, m_byIPAdd4);	
	CDialog::OnOK();
}
