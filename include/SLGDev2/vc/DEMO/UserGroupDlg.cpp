// UserGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "UserGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UserGroupDlg dialog


UserGroupDlg::UserGroupDlg(CHCADOConnection& ado, CWnd* pParent /*=NULL*/)
	: m_ado(ado), CDialog(UserGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(UserGroupDlg)
	m_nNormal = 0;
	m_nForce = 0;
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void UserGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(UserGroupDlg)
	DDX_Control(pDX, IDC_LIST_SEL_USER, m_listSelUser);
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
	DDX_Control(pDX, IDC_TREE_LIST, m_treeUG);
	DDX_Text(pDX, IDC_EDIT_N, m_nNormal);
	DDX_Text(pDX, IDC_EDIT_F, m_nForce);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(UserGroupDlg, CDialog)
	//{{AFX_MSG_MAP(UserGroupDlg)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_R, OnButtonR)
	ON_BN_CLICKED(IDC_BUTTON_L, OnButtonL)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LIST, OnSelRightTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UserGroupDlg message handlers
BOOL UserGroupDlg::OnInitDialog() 
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
	m_treeUG.SetImageList( &m_treeImage, TVSIL_NORMAL );
	m_hRootItem = NULL;
	m_hCurItem = NULL;

	RECT rt;
	m_listUser.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP  );
	m_listUser.GetClientRect( &rt );
	int width = (rt.right-rt.left);
	m_listUser.InsertColumn(0, _T("ID"), LVCFMT_CENTER, width/2);
	m_listUser.InsertColumn(1, _T("Name"), LVCFMT_CENTER, width/2);
	
	m_listSelUser.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_CHECKBOXES );
	m_listSelUser.GetClientRect( &rt );
	width = (rt.right-rt.left);
	m_listSelUser.InsertColumn(0, _T("validation"), LVCFMT_CENTER, width/3);
	m_listSelUser.InsertColumn(1, _T("ID"), LVCFMT_CENTER, width/3);
	m_listSelUser.InsertColumn(2, _T("Name"), LVCFMT_CENTER, width/3);

	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(FALSE);

	Clear();
	LoadGroup();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void UserGroupDlg::Clear()
{

	m_nNormal = 0;
	m_nForce = 0;
	m_Name = _T("");

	((CButton*)GetDlgItem(IDC_CHECK_ORDER))->SetCheck( FALSE ); 

	m_listSelUser.DeleteAllItems();
	m_listUser.DeleteAllItems();
	UpdateData(FALSE);
}

void UserGroupDlg::LoadGroup()
{
	m_treeUG.DeleteAllItems();
	m_hRootItem = m_treeUG.InsertItem(_T("User group"),0,1);
	CString sql(_T("select [GroupID], [Name] from [ugroup]"));
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
			ID   = tmp->GetLong( _T("GroupID") );
			
			hTemp = m_treeUG.InsertItem(name,0,1,m_hRootItem);			
			m_treeUG.SetItemData(hTemp,ID);
		}
	}
}

void UserGroupDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	
	Clear();
	CString sql = _T("select max([GroupID]) as [mID] from [ugroup]");
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	int ID = 1;
	if ( !tmp->IsEmpty() )
	{
		ID = tmp->GetLong( _T("mID") ) + 1;
	}
	
	sql.Format( _T("insert into[ugroup] ([Name]) values( \'%s\')"), _T("User group") );
	m_ado.ExecuteNoSelectSQL( sql );
	m_hCurItem = m_treeUG.InsertItem( _T("User group"), 0, 1, m_hRootItem );
	m_treeUG.SetItemData( m_hCurItem,ID );
	m_treeUG.SelectItem( m_hCurItem );

	sql =  _T("select * from [user]");
 	LoadUser(sql);

}

void UserGroupDlg::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	int ID =  m_treeUG.GetItemData( m_hCurItem );
	CString sql ;
	sql.Format( _T("delete from [ugroup] where ([GroupID]=%d)"), ID);
	m_ado.ExecuteNoSelectSQL( sql );
	
	m_treeUG.SelectItem(m_hRootItem);
	NMTREEVIEW vi; 
	memset( (void*)&vi, 0, sizeof(NMTREEVIEW) ); 
	vi.hdr.hwndFrom   =   m_treeUG.m_hWnd; 
	vi.hdr.idFrom   =   ::GetWindowLong(m_treeUG.m_hWnd, GWL_ID); 
	vi.hdr.code   =   TVN_SELCHANGED; 
	vi.action   =   TVN_SELCHANGED; 
	
	::SendMessage( m_treeUG.m_hWnd, WM_NOTIFY, (WPARAM)vi.hdr.idFrom, (LPARAM)&vi );
	
	m_treeUG.UpdateData(FALSE);
	
	Clear();
}

void UserGroupDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	
	int U_ID =  m_treeUG.GetItemData( m_hCurItem );

	int users = m_listSelUser.GetItemCount();
	
	int nNums = 0;
	int fNums = 0;
	CString strNID[6];
	CString strFID[6];
	for ( int j = 0; j < users; ++j )
	{
		if ( m_listSelUser.GetCheck(j) )
		{
			strFID[fNums] = m_listSelUser.GetItemText(j, 1);
			fNums++;
		}
		else
		{
			strNID[nNums] = m_listSelUser.GetItemText(j, 1);
			nNums++;
		}
	}


	CString sql ;  
	sql.Format( _T("update [ugroup] set [Name]=\'%s\', [NormalValid]=%d, [ForceValid]=%d, [GroupOrder]=%d,\
		[NormalUsers_0]=\'%s\', [NormalUsers_1]=\'%s\', [NormalUsers_2]=\'%s\', [NormalUsers_3]=\'%s\', [NormalUsers_4]=\'%s\', [NormalUsers_5]=\'%s\',  \
		[ForceUsers_0]=\'%s\', [ForceUsers_1]=\'%s\', [ForceUsers_2]=\'%s\', [ForceUsers_3]=\'%s\', [ForceUsers_4]=\'%s\', [ForceUsers_5]=\'%s\' where ([GroupID]=%d)"),  \
		m_Name,  m_nNormal, m_nForce, ((CButton*)GetDlgItem(IDC_CHECK_ORDER))->GetCheck() ,
		strNID[0], strNID[1], strNID[2], strNID[3], strNID[4], strNID[5], 
		strFID[0], strFID[1], strFID[2], strFID[3], strFID[4], strFID[5],
		U_ID
		);
	
	
	m_ado.ExecuteNoSelectSQL(sql);
	UpdateData( FALSE );
	AfxMessageBox( _T("Save successfully") );
}

void UserGroupDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	
}

void UserGroupDlg::OnButtonR() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listUser.GetFirstSelectedItemPosition(); 
	if ( pos == NULL ) return;
	int items = m_listSelUser.GetItemCount();
	if (items>=12) return;
	int n = m_listUser.GetNextSelectedItem(pos); 
	CString str_id = m_listUser.GetItemText(n, 0);
	CString str_name = m_listUser.GetItemText(n, 1);
	m_listSelUser.InsertItem(items, "");
	m_listSelUser.SetItemText(items, 1, str_id);
	m_listSelUser.SetItemText(items, 2, str_name);
	m_listUser.DeleteItem(n);
}

void UserGroupDlg::OnButtonL() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listSelUser.GetFirstSelectedItemPosition(); 
	if ( pos == NULL ) return;
	
	int n = m_listSelUser.GetNextSelectedItem(pos); 
	CString str_id = m_listSelUser.GetItemText(n, 1);
	CString str_name = m_listSelUser.GetItemText(n, 2);
	
	int items = m_listUser.GetItemCount();
	m_listUser.InsertItem(items, str_id);
	m_listUser.SetItemText(items, 1, str_name);
	
	m_listSelUser.DeleteItem(n);
}

void UserGroupDlg::OnSelRightTree( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM  hSelect = m_treeUG.GetSelectedItem();
	if (hSelect == NULL)
	{
		m_hCurItem = NULL;
		*pResult = 0;
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(FALSE);
		Clear();
		return;
	}
	if(hSelect == m_hRootItem)	
	{
		m_hCurItem = NULL;
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(FALSE);
		*pResult = 0;
		Clear();
		return;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(TRUE);
	}
	m_hCurItem = hSelect;
	ShowUG( m_treeUG.GetItemData(m_hCurItem)  );
	*pResult = 0;
}

void UserGroupDlg::ShowUG( int ID )
{
	CString sql ;
	sql.Format( _T("select * from [ugroup] where ([GroupID]=%d)"), ID );
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )
	{
		m_Name = tmp->GetCString( _T("Name") );
		
		
		((CButton*)GetDlgItem(IDC_CHECK_ORDER))->SetCheck( tmp->GetLong( _T("GroupOrder") ) );
		
		
		m_nNormal = tmp->GetLong( _T("NormalValid") );
		m_nForce = tmp->GetLong( _T("ForceValid") );
		
		m_listSelUser.DeleteAllItems();
		
		CString exit_user;
		CString key;
		int index = 0;
		for ( int j = 0; j < 6; ++j)
		{
			key.Format( _T("NormalUsers_%d"), j );
			if ( tmp->GetCString( key ).IsEmpty() ) continue;
			exit_user += _T('\'') + tmp->GetCString( key )+ _T("\',");
			
			m_listSelUser.InsertItem(index, "");
			m_listSelUser.SetItemText(index, 1, tmp->GetCString( key ));
			++index;
		}
		for ( int ii = 0; ii < 6; ++ii )
		{
			key.Format( _T("ForceUsers_%d"), ii );
			if ( tmp->GetCString( key ).IsEmpty() ) continue;
			exit_user += _T('\'') + tmp->GetCString( key )+ _T("\',");
			
			m_listSelUser.InsertItem(index, "");
			m_listSelUser.SetItemText(index, 1, tmp->GetCString( key ));
			m_listSelUser.SetCheck(index,TRUE);
			++index;
		}
		
		if ( !exit_user.IsEmpty() )
		{
			exit_user = exit_user.Left( exit_user.GetLength()-1 );
			sql.Format( _T("select * from [user] where [UserID] not in (%s) "), exit_user );
			
		}
		else
		{
			sql = _T("select * from [user]");
		}
		LoadUser(sql);
	}
	else
	{
		Clear();
	}
	UpdateData(FALSE);
}

void UserGroupDlg::LoadUser( const CString& sql )
{
	m_listUser.DeleteAllItems();

	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();

		CString name ;
		CString ID;
		int n = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext(), ++n )
		{
			name = tmp->GetCString( _T("UserName") );
			ID   = tmp->GetCString( _T("UserID") );
			n = m_listUser.InsertItem(n,ID);		
			m_listUser.SetItemText( n, 1, name );
		}
	}
}



