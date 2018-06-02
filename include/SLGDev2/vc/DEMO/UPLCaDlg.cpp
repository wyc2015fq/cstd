// UPLCaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "UPLCaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUPLCaDlg dialog


CUPLCaDlg::CUPLCaDlg(CString strIP, CWnd* pParent /*=NULL*/)
	: CDialog(CUPLCaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUPLCaDlg)
	m_strPWDEDT = _T("");
	//}}AFX_DATA_INIT
   m_strIP = strIP;
}


void CUPLCaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUPLCaDlg)
	DDX_Text(pDX, IDC_PSWEDT, m_strPWDEDT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUPLCaDlg, CDialog)
	//{{AFX_MSG_MAP(CUPLCaDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPLCaDlg message handlers

void CUPLCaDlg::OnFileOpenBnt() 
{
	// TODO: Add your control notification handler code here
	
}

void CUPLCaDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( TRUE );

	CFileDialog dlg(TRUE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "(*.*)|*.*||" );
	if ( dlg.DoModal() != IDOK ) return;
	CString file = dlg.GetPathName();
	FILE* pfile = fopen( dlg.GetPathName(), "rb" );
	if ( pfile )
	{
		DEV_APPUPDATE app = {0};
		fseek(pfile, 0, SEEK_END );
		app.m_Bytes = ftell(pfile);
		fseek(pfile, 0, SEEK_SET);
	    memcpy( app.m_CtxInfo.m_ID, m_strPWDEDT.GetBuffer(0), m_strPWDEDT.GetLength() );
		app.m_Buf = new char[app.m_Bytes];
		memset( app.m_Buf, 0x0, app.m_Bytes );
		fread( app.m_Buf, 1, app.m_Bytes, pfile );
		app.m_Type = 2 ;
		int index = file.ReverseFind('\\');
		file = file.Right( file.GetLength() - index - 1 );
		memcpy( app.m_FileName.m_ID, file.GetBuffer(0), file.GetLength() );
		CPM_ULUpdate( m_strIP.GetBuffer(0), app );
		delete [] app.m_Buf;
		fclose(pfile);
	}
	CDialog::OnOK();
}

void CUPLCaDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CUPLCaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AfxMessageBox(m_strIP);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
