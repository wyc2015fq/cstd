// VideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "VideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// VideoDlg dialog


VideoDlg::VideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(VideoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(VideoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void VideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(VideoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control( pDX, IDC_LIST_IP, m_IPList );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(VideoDlg, CDialog)
	//{{AFX_MSG_MAP(VideoDlg)
		// NOTE: the ClassWizard will add message map macros here
		ON_BN_CLICKED( IDC_BUTTON_VIDEO1, OnBtnVideo1 )
		ON_BN_CLICKED( IDC_BUTTON_VIDEO2, OnBtnVideo2 )
		ON_BN_CLICKED( IDC_BUTTON_VIDEO3, OnBtnVideo3 )
		ON_BN_CLICKED( IDC_BUTTON_VIDEO4, OnBtnVideo4 )
		ON_BN_CLICKED( IDC_BUTTON_VIDEO5, OnBtnVideo5 )
		ON_BN_CLICKED( IDC_BUTTON_VIDEO6, OnBtnVideo6 )
		ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VideoDlg message handlers

void VideoDlg::OnBtnVideo1()
{
	int nItem = m_IPList.GetCurSel();
	if (  nItem >= 0 )
	{
		CString str_ip ;
		m_IPList.GetText(nItem, str_ip);
		CPM_StartVideo( str_ip.GetBuffer(0), (void*)( GetDlgItem(IDC_STATIC_VIDEO1)->GetSafeHwnd()), NULL );
	}
}

void VideoDlg::OnBtnVideo2()
{
	int nItem = m_IPList.GetCurSel();
	if (  nItem > 0 )
	{
		CString str_ip ;
		m_IPList.GetText(nItem, str_ip);
//		CPM_StartVideo( str_ip.GetBuffer(0), (long)( GetDlgItem(IDC_STATIC_VIDEO2)->GetSafeHwnd() ) );
	}
}

void VideoDlg::OnBtnVideo3()
{
	int nItem = m_IPList.GetCurSel();
	if (  nItem > 0 )
	{
		CString str_ip ;
		m_IPList.GetText(nItem, str_ip);
//		CPM_StartVideo( str_ip.GetBuffer(0), (long)( GetDlgItem(IDC_STATIC_VIDEO3)->GetSafeHwnd() ) );
	}
}

void VideoDlg::OnBtnVideo4()
{
	int nItem = m_IPList.GetCurSel();
	if (  nItem > 0 )
	{
		CString str_ip ;
		m_IPList.GetText(nItem, str_ip);
//		CPM_StartVideo( str_ip.GetBuffer(0), (long)( GetDlgItem(IDC_STATIC_VIDEO4)->GetSafeHwnd() ) );
	}
}

void VideoDlg::OnBtnVideo5()
{
	int nItem = m_IPList.GetCurSel();
	if (  nItem > 0 )
	{
		CString str_ip ;
		m_IPList.GetText(nItem, str_ip);
//		CPM_StartVideo( str_ip.GetBuffer(0), (long)( GetDlgItem(IDC_STATIC_VIDEO5)->GetSafeHwnd() ) );
	}
}

void VideoDlg::OnBtnVideo6()
{
	int nItem = m_IPList.GetCurSel();
	if (  nItem > 0 )
	{
		CString str_ip ;
		m_IPList.GetText(nItem, str_ip);
//		CPM_StartVideo( str_ip.GetBuffer(0), (long)( GetDlgItem(IDC_STATIC_VIDEO6)->GetSafeHwnd() ) );
	}
}

BOOL VideoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int nCount = m_IPArray.GetSize();
	int i = 0;
	for ( i=0; i < nCount; ++i )
	{
		m_IPList.InsertString( i, m_IPArray.GetAt(i) );
	}
	return TRUE;
}

void VideoDlg::Init( const CStringArray& IPArray )
{
	int nCount = IPArray.GetSize();
	int i = 0;
	for ( i=0; i < nCount; ++i )
	{
		m_IPArray.Add( IPArray.GetAt(i) );
	}
}

void VideoDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

void VideoDlg::OnClose()
{
	int count = m_IPList.GetCount();
	for( int index = 0; index < count; ++index )
	{
		CString str_ip ;
		m_IPList.GetText( index , str_ip);
		CPM_StopVideo( str_ip.GetBuffer(0) );
	}
	DestroyWindow();
}