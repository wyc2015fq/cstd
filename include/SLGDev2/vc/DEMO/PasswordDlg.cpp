// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "PasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog


CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordDlg)
	m_nIntVal = 0;
	//}}AFX_DATA_INIT
}


void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordDlg)
	DDX_Text(pDX, IDC_EDIT1, m_nIntVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	//{{AFX_MSG_MAP(CPasswordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers

void CPasswordDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CPasswordDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if ( IsVerify() )
	{
		CDialog::OnOK();
	}

}
BOOL CPasswordDlg::IsVerify()
{
   CTime t = 	CTime::GetCurrentTime();
   UINT Value = t.GetMonth()*1000+t.GetDay()*100+t.GetHour()*10+t.GetMinute();
   if ( abs( Value - m_nIntVal ) <= 5  )
	   return true;
   else
	   return false;
}
