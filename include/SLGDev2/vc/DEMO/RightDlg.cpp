// RightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "RightDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRightDlg dialog


CRightDlg::CRightDlg(CHCADOConnection& ado,CWnd* pParent /*=NULL*/)
	: m_ado(ado),CDialog(CRightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRightDlg)
	m_name = _T("");

	//}}AFX_DATA_INIT
}


void CRightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRightDlg)
	DDX_Control(pDX, IDC_CHECK_HOLIDY_VALIDATE, m_checkHolidy);
	DDX_Control(pDX, IDC_CHECK_MANEGT, m_checkLock);
	DDX_Control(pDX, IDC_CHECK_OUT, m_checkOutPut);
	DDX_Control(pDX, IDC_LIST_TIMEGROUP, m_listTime);
	DDX_Control(pDX, IDC_TREE_RIGHT, m_treeRight);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRightDlg, CDialog)
	//{{AFX_MSG_MAP(CRightDlg)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_RIGHT, OnSelRightTree)
 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightDlg message handlers

BOOL CRightDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	HICON hIcon = NULL;
	m_treeImage.Detach();
	m_treeImage.Create ( 16, 16, ILC_COLOR16|ILC_MASK, 4, 1 );
	hIcon = AfxGetApp()->LoadIcon( IDI_RIGHT );			
	m_treeImage.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon( IDI_RIGHTSELECT);
	m_treeImage.Add(hIcon);
	m_treeRight.SetImageList( &m_treeImage, TVSIL_NORMAL );
	m_hRootItem = NULL;
	m_hCurItem = NULL;


	m_listTime.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP|LVS_EX_CHECKBOXES );

	RECT rt;
	m_listTime.GetClientRect( &rt );
	int width = (rt.right-rt.left);

	m_listTime.InsertColumn( 0, _T("Select"),   LVCFMT_CENTER, (width / 8)  );
	m_listTime.InsertColumn( 1, _T("Time group"), LVCFMT_CENTER, width - (width / 8)  );
	
	
	LoadRight();
	LoadTimeGroup();
//	LoadUser();
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	
	Clear();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRightDlg::LoadRight()
{
	m_treeRight.DeleteAllItems();
	m_hRootItem = m_treeRight.InsertItem(_T("Author set"),0,1);
	CString sql(_T("select [RightID], [Name] from [right]"));
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();
		HTREEITEM hTemp = NULL;
		CString name ;
		int     ID = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext() )
		{
			name = tmp->GetCString( _T("Name") );
			ID   = tmp->GetLong( _T("RightID") );

			hTemp = m_treeRight.InsertItem(name,0,1,m_hRootItem);			
			m_treeRight.SetItemData(hTemp,ID);
		}
	}
}

void CRightDlg::LoadTimeGroup()
{
	m_listTime.DeleteAllItems();
	
	CString sql(_T("select [ID], [name] from [timegroup]"));
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();
		HTREEITEM hTemp = NULL;
		CString name ;
		int     ID = 0;
		int n = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext(), ++n )
		{
			name = tmp->GetCString( _T("name") );
			ID   = tmp->GetLong( _T("ID") );
			n = m_listTime.InsertItem(n,"");
			m_listTime.SetItemText( n, 1, name );
			m_listTime.SetItemData( n, ID );
		}
	}
}


void CRightDlg::Clear()
{
	int count = m_listTime.GetItemCount();
	for ( int i = 0; i < count; ++i )
	{
		m_listTime.SetCheck(i, FALSE);
	}
	m_name = _T("");
	m_checkHolidy.SetCheck(FALSE);
	m_checkLock.SetCheck(FALSE);
	m_checkOutPut.SetCheck(FALSE);
// 	m_listSelUser.DeleteAllItems();
// 	m_listUser.DeleteAllItems();
	UpdateData(FALSE);
}

void CRightDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here

	Clear();
	CString sql = _T("select max([RightID]) as [mID] from [right]");
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	int ID = 1;
	if ( !tmp->IsEmpty() )
	{
		ID = tmp->GetLong( _T("mID") ) + 1;
	}
	
	sql.Format( _T("insert into[right] ([Name]) values( \'%s\')"), _T("Author") );
	m_ado.ExecuteNoSelectSQL( sql );
	m_hCurItem = m_treeRight.InsertItem( _T("Author"), 0, 1, m_hRootItem );
	m_treeRight.SetItemData( m_hCurItem,ID );
	m_treeRight.SelectItem( m_hCurItem );

}

void CRightDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	int ID =  m_treeRight.GetItemData( m_hCurItem );
	CString sql ;
	sql.Format( _T("delete from [right] where ([RightID]=%d)"), ID);
	m_ado.ExecuteNoSelectSQL( sql );
	
	m_treeRight.SelectItem(m_hRootItem);
	NMTREEVIEW vi; 
	memset( (void*)&vi, 0, sizeof(NMTREEVIEW) ); 
	vi.hdr.hwndFrom   =   m_treeRight.m_hWnd; 
	vi.hdr.idFrom   =   ::GetWindowLong(m_treeRight.m_hWnd, GWL_ID); 
	vi.hdr.code   =   TVN_SELCHANGED; 
	vi.action   =   TVN_SELCHANGED; 
	
	::SendMessage( m_treeRight.m_hWnd, WM_NOTIFY, (WPARAM)vi.hdr.idFrom, (LPARAM)&vi );
	
	m_treeRight.UpdateData(FALSE);

	Clear();

}

void CRightDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	
	int R_ID =  m_treeRight.GetItemData( m_hCurItem );
	int T_ID[8] = {-1};
	m_treeRight.SetItemText(m_hCurItem, m_name);
	int items = m_listTime.GetItemCount();
	int index = 0;
	for ( int i = 0; i < items && index < 8; ++i )
	{
		if (  m_listTime.GetCheck( i ) )
		{
			T_ID[index++] = m_listTime.GetItemData(i);
		}
	}

	CString sql ;  
	sql.Format( _T("update [right] set [Name]=\'%s\',\
		[TimeGID_0]=%d, [TimeGID_1]=%d, [TimeGID_2]=%d, [TimeGID_3]=%d, [TimeGID_4]=%d, [TimeGID_5]=%d, [TimeGID_6]=%d, [TimeGID_7]=%d, \
		[HolidyValid]=%d, [ActionLock]=%d, [ActionOutPut]=%d where ([RightID]=%d)"), \
		m_name,   
		T_ID[0], T_ID[1], T_ID[2], T_ID[3], T_ID[4], T_ID[5], T_ID[6], T_ID[7], 
		m_checkHolidy.GetCheck(), m_checkLock.GetCheck(), m_checkOutPut.GetCheck(), R_ID);	
			 	
	m_ado.ExecuteNoSelectSQL(sql);
	UpdateData( FALSE );
	AfxMessageBox( _T("Save successfully") );
	
}

void CRightDlg::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	
}

void CRightDlg::OnSelRightTree( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM  hSelect = m_treeRight.GetSelectedItem();
	if (hSelect == NULL)
	{
		m_hCurItem = NULL;
		*pResult = 0;
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
		Clear();
		return;
	}
	if(hSelect == m_hRootItem)	
	{
		m_hCurItem = NULL;
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
		*pResult = 0;
		Clear();
		return;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
	}
	m_hCurItem = hSelect;
	ShowRight( m_treeRight.GetItemData(m_hCurItem)  );
	*pResult = 0;
}

void CRightDlg::ShowRight( int ID )
{
	CString sql ;
	sql.Format( _T("select * from [right] where ([RightID]=%d)"), ID );
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )
	{
		m_name = tmp->GetCString( _T("Name") );

		int items = m_listTime.GetItemCount();
		int id = 0;
		CString key;
		for ( int i = 0 ; i < 8; ++i )
		{
			key.Format( _T("TimeGID_%d"), i );
			id = tmp->GetLong( key );
			for ( int j = 0; j < items; ++j )
			{
				if ( id == m_listTime.GetItemData(j) ) 
				{
					m_listTime.SetCheck( j, TRUE );
					break;
				}
			}
		}
 
		m_checkHolidy.SetCheck( tmp->GetLong( _T("HolidyValid") ) );
		m_checkLock.SetCheck( tmp->GetLong( _T("ActionLock") ) );
		m_checkOutPut.SetCheck( tmp->GetLong( _T("HolidyValid") ) );
	}
	else
	{
		Clear();
	}
	UpdateData(FALSE);
}
