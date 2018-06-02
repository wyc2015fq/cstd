// AuthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "AuthDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AuthDlg dialog


AuthDlg::AuthDlg(const char* dev_addr, CWnd* pParent /*=NULL*/)
	: CDialog(AuthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AuthDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset( m_IP, 0x0, sizeof(m_IP) );
	memcpy( m_IP, dev_addr, strlen(dev_addr) );
}


void AuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AuthDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AuthDlg, CDialog)
	//{{AFX_MSG_MAP(AuthDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AuthDlg message handlers

void AuthDlg::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData( FALSE );
	CString name;
	GetDlgItem(IDC_EDIT_USER)->GetWindowText(name);
	CString pws ;
	GetDlgItem(IDC_EDIT_PWS)->GetWindowText(pws);
	DEV_CID c_name = {0};
	memcpy( c_name.m_ID, name.GetBuffer(0), name.GetLength() );
	DEV_CID c_pws = {0};
	memcpy( c_pws.m_ID, pws.GetBuffer(0), pws.GetLength() );

//	CPM_ULDevAuth( m_IP, c_name, c_pws, 2 );
	CDialog::OnOK();
}
