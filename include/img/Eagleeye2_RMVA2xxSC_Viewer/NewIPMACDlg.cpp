// NewIPMACDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "NewIPMACDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewIPMACDlg dialog


CNewIPMACDlg::CNewIPMACDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewIPMACDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewIPMACDlg)
	//}}AFX_DATA_INIT
}


void CNewIPMACDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewIPMACDlg)
	DDX_Control(pDX, IDC_NEWIPADDRESS, m_ctrlNewIPAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewIPMACDlg, CDialog)
	//{{AFX_MSG_MAP(CNewIPMACDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewIPMACDlg message handlers

void CNewIPMACDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_ctrlNewIPAddress.GetAddress(m_byIPAdd1, m_byIPAdd2, m_byIPAdd3, m_byIPAdd4);

	CDialog::OnOK();
}
