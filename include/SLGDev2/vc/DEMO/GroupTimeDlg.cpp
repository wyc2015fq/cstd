// GroupTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "GroupTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupTimeDlg dialog


CGroupTimeDlg::CGroupTimeDlg(CHCADOConnection& ado,CWnd* pParent /*=NULL*/)
: m_ado(ado),CDialog( CGroupTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupTimeDlg)
	m_start = 0;
	m_end = 0;
	m_name = _T("");
	m_s_hour = 0;
	m_s_mini = 0;
	m_s_second = 0;
	m_e_hour = 0;
	m_e_mini = 0;
	m_e_second = 0;
	//}}AFX_DATA_INIT
	m_hRootItem = NULL;
	m_hCurItem =  NULL;
	m_hWeekFirst = NULL;
}


void CGroupTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupTimeDlg)
	DDX_Control(pDX, IDC_TREE_WEEK, m_treeWeek);
	DDX_Control(pDX, IDC_TREE_TIMEGROUP, m_treeTG);
	DDX_Control(pDX, IDC_CHECK_WEEK, m_checkW);
	DDX_Control(pDX, IDC_CHECK_YEAR, m_checkY);
	DDX_Control(pDX, IDC_CHECK_MONTH, m_checkM);
	DDX_Control(pDX, IDC_CHECK_DAY, m_checkD);
	DDX_Control(pDX, IDC_CHECK_TIME, m_checkT);
	DDX_Control(pDX, IDC_CHECK_HOLIDY, m_checkH);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START, m_start);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END, m_end);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_S_HOUR, m_s_hour);
	DDX_Text(pDX, IDC_EDIT_S_MINI, m_s_mini);
	DDX_Text(pDX, IDC_EDIT_S_SECOND, m_s_second);
	DDX_Text(pDX, IDC_EDIT_E_HOUR, m_e_hour);
	DDX_Text(pDX, IDC_EDIT_E_MINI, m_e_mini);
	DDX_Text(pDX, IDC_EDIT_E_SECOND, m_e_second);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupTimeDlg)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TIMEGROUP, OnSelTimeGroupTree)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK_WEEK, OnWeekSelChk)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, OnButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupTimeDlg message handlers

BOOL CGroupTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//

	HICON hIcon = NULL;
	m_ImageTimeGroupTree.Detach();
	m_ImageTimeGroupTree.Create ( 16, 16, ILC_COLOR16|ILC_MASK, 4, 1 );
	//
	hIcon = AfxGetApp()->LoadIcon( IDI_TIMEGROUP );			
	m_ImageTimeGroupTree.Add(hIcon);
	//
	hIcon = AfxGetApp()->LoadIcon( IDI_TIMEGROUPSELECT);			
	m_ImageTimeGroupTree.Add(hIcon);
	m_treeTG.SetImageList( &m_ImageTimeGroupTree, TVSIL_NORMAL );
	
	m_ImageWeekCheckTree.Detach();
	m_ImageWeekCheckTree.Create ( 16, 16, ILC_COLOR16|ILC_MASK, 4, 1 );
	//
	hIcon = AfxGetApp()->LoadIcon( IDI_WEEKCHECK );			
	m_ImageWeekCheckTree.Add(hIcon);
	//
	hIcon = AfxGetApp()->LoadIcon( IDI_WEEKCHECKSELECT );			
	m_ImageWeekCheckTree.Add(hIcon);
	m_treeWeek.SetImageList( &m_ImageWeekCheckTree, TVSIL_NORMAL );

	LoadTimeGroup();
	LoadWeekDay();

	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);

	ClearSkin();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGroupTimeDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		int key = int( pMsg->wParam );
		if( key == VK_ESCAPE )
			return TRUE;
		
		if( key == VK_RETURN )
			return TRUE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CGroupTimeDlg::LoadTimeGroup()
{
	m_treeTG.DeleteAllItems();
	m_hRootItem = m_treeTG.InsertItem(_T("All of the time groups"),0,1);
	CString sql(_T("select [ID], [name] from [timegroup]"));
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();
		HTREEITEM hTemp = NULL;
		CString name ;
		int     ID = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext() )
		{
			name = tmp->GetCString( _T("name") );
			ID   = tmp->GetLong( _T("ID") );
			hTemp = m_treeTG.InsertItem(name,0,1,m_hRootItem);			
			m_treeTG.SetItemData(hTemp,ID);
		}
	}
}

void CGroupTimeDlg::LoadWeekDay()
{
	m_treeWeek.DeleteAllItems();
	HTREEITEM	hItem;
	hItem = m_treeWeek.InsertItem(_T("Sunday"),0,1);
	m_hWeekFirst = hItem;
	m_treeWeek.SetItemData(hItem,(DWORD)64);
	hItem = m_treeWeek.InsertItem(_T("Monday"),0,1);
	m_treeWeek.SetItemData(hItem,(DWORD)1);
	hItem = m_treeWeek.InsertItem(_T("Tuesday"),0,1);
	m_treeWeek.SetItemData(hItem,(DWORD)2);
	hItem = m_treeWeek.InsertItem(_T("Wednesday"),0,1);
	m_treeWeek.SetItemData(hItem,(DWORD)4);
	hItem = m_treeWeek.InsertItem(_T("Thursday"),0,1);
	m_treeWeek.SetItemData(hItem,(DWORD)8);
	hItem = m_treeWeek.InsertItem(_T("Friday"),0,1);
	m_treeWeek.SetItemData(hItem,(DWORD)16);
	hItem = m_treeWeek.InsertItem(_T("Saturday"),0,1);
	m_treeWeek.SetItemData(hItem,(DWORD)32);
}

void CGroupTimeDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	CString sql = _T("select max([ID]) as [mID] from [timegroup]");
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	int ID = 1;
	if ( !tmp->IsEmpty() )
	{
		ID = tmp->GetLong( _T("mID") ) + 1;
	}

	sql.Format( _T("insert into[timegroup] ([checkflag],[name]) values( %d, \'%s\')"), 0x08, _T("Time group") );
	m_ado.ExecuteNoSelectSQL(sql);
	m_hCurItem = m_treeTG.InsertItem(_T("Time group"),0,1,m_hRootItem);
	m_treeTG.SetItemData(m_hCurItem,ID);
	m_treeTG.SelectItem(m_hCurItem);

}

void CGroupTimeDlg::OnSelTimeGroupTree( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM  hSelect = m_treeTG.GetSelectedItem();
	if (hSelect == NULL)
	{
		m_hCurItem = NULL;
		*pResult = 0;
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);
		ClearSkin();
		return;
	}
	if(hSelect == m_hRootItem)	
	{
		m_hCurItem = NULL;
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);
		*pResult = 0;
		ClearSkin();
		return;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(TRUE);
	}
	m_hCurItem = hSelect;
	ShowTimeGroup( m_treeTG.GetItemData(m_hCurItem)  );
	*pResult = 0;
}

void CGroupTimeDlg::ClearSkin()
{
	
	m_treeWeek.EnableWindow(FALSE);
	HTREEITEM hItem = m_hWeekFirst;
	while(hItem)
	{
		m_treeWeek.SetCheck(hItem,FALSE);
		hItem = m_treeWeek.GetNextSiblingItem(hItem);
	}	
	m_name = _T("");
	CTime  global_e_time(CTime::GetCurrentTime());
	m_start   = global_e_time;
	CTime  global_s_time(CTime::GetCurrentTime());
	m_end = global_s_time;
	m_checkY.SetCheck(FALSE);
	m_checkM.SetCheck(FALSE);
	m_checkD.SetCheck(FALSE);
	m_checkT.SetCheck(FALSE);
	m_checkW.SetCheck(FALSE);
	m_checkH.SetCheck(FALSE);
	UpdateData(FALSE);
}

void CGroupTimeDlg::ShowTimeGroup(int ID)
{
	CString sql ;
	sql.Format( _T("select * from [timegroup] where ([ID]=%d)"), ID );
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )
	{
		CString name = tmp->GetCString( _T("name") );
		CString start = tmp->GetCString( _T("start") );
		CString end = tmp->GetCString( _T("end") );
		int checkdate = tmp->GetLong( _T("checkflag") );
		int checkweek = tmp->GetLong( _T("weekflag") );
		
		m_name = name;
		COleDateTime time;
		if ( time.ParseDateTime(start) ) 
		{
			CTime c_s( time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			m_s_hour = time.GetHour();
			m_s_mini = time.GetMinute();
			m_s_second = time.GetSecond();
			m_start = c_s;
		}
		if ( time.ParseDateTime(end) )
		{
			CTime c_e( time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			m_e_hour = time.GetHour();
			m_e_mini = time.GetMinute();
			m_e_second = time.GetSecond();
			m_end   = c_e;
		}

		m_checkY.SetCheck( checkdate & 0x10 );
		m_checkM.SetCheck( checkdate & 0x08 );
		m_checkD.SetCheck( checkdate & 0x04 );
		m_checkT.SetCheck( checkdate & 0x01 );

		m_checkH.SetCheck( (tmp->GetLong( _T("type") ) == DEV_HOLIDY) );

		if ( checkdate & 0x02 )
		{
			m_checkW.SetCheck( TRUE );
			m_treeWeek.EnableWindow(TRUE);
			HTREEITEM hItem = m_hWeekFirst;			 
			while( hItem )
			{			 
				m_treeWeek.SetCheck( hItem, m_treeWeek.GetItemData( hItem ) &  checkweek );
				hItem = m_treeWeek.GetNextSiblingItem(hItem);
			}
		}
		else
		{
			m_checkW.SetCheck( FALSE );
			m_treeWeek.EnableWindow(FALSE);
			HTREEITEM hItem = m_hWeekFirst;
			while(hItem)
			{
				m_treeWeek.SetCheck( hItem, FALSE );
				hItem = m_treeWeek.GetNextSiblingItem( hItem );
			}	
		}

	}
	else
	{
		ClearSkin();
	}
	UpdateData(FALSE);
}

void CGroupTimeDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	int ID =  m_treeTG.GetItemData( m_hCurItem );
	m_treeTG.SetItemText(m_hCurItem, m_name);
	CString name = m_name;
	CString start  ;
	start.Format( _T("%02d/%02d/%04d %02d:%02d:%02d"), m_start.GetMonth(), m_start.GetDay(), m_start.GetYear(), m_s_hour, m_s_mini, m_s_second );
	CString end  ;
	end.Format( _T("%02d/%02d/%04d %02d:%02d:%02d"), m_end.GetMonth(), m_end.GetDay(), m_end.GetYear(), m_e_hour, m_e_mini, m_e_second );

	int checkdate = 0;
	checkdate |= ((m_checkY.GetCheck()==1) ? 1 : 0) << 4;
	checkdate |= ((m_checkM.GetCheck()==1) ? 1 : 0) << 3;
	checkdate |= ((m_checkD.GetCheck()==1) ? 1 : 0) << 2;
	checkdate |= ((m_checkW.GetCheck()==1) ? 1 : 0) << 1;
	checkdate |= (m_checkT.GetCheck()==1) ? 1 : 0;

	int checkweek = 0;
	if ( m_checkW.GetCheck() )
	{
		//	
		HTREEITEM hItem = m_hWeekFirst;
		while(hItem)
		{
			checkweek |= (m_treeWeek.GetCheck(hItem)==1) ? m_treeWeek.GetItemData(hItem) : 0  ;
			hItem = m_treeWeek.GetNextSiblingItem(hItem);
		}	 
	}

	int type = m_checkH.GetCheck() ? 0x02 : 0x01;

	CString sql ;  
	sql.Format( _T("update [timegroup] set [name]=\'%s\', [start]=\'%s\', [end]=\'%s\', [type]=%d, [checkflag]=%d,[weekflag]=%d where ([ID]=%d)"), 
				name,start,end,type, checkdate,checkweek,ID);		
	m_ado.ExecuteNoSelectSQL(sql);
	UpdateData( FALSE );
	AfxMessageBox( _T("Save successfully") );
	
}

void CGroupTimeDlg::OnWeekSelChk()
{
	m_treeWeek.EnableWindow( m_checkW.GetCheck() ? TRUE : FALSE );
}

void CGroupTimeDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	int ID =  m_treeTG.GetItemData( m_hCurItem );
	CString sql ;
	sql.Format( _T("delete from [timegroup] where ([ID]=%d)"), ID);
	m_ado.ExecuteNoSelectSQL( sql );

	m_treeTG.SelectItem(m_hRootItem);
	NMTREEVIEW vi; 
	memset( (void*)&vi, 0, sizeof(NMTREEVIEW) ); 
	vi.hdr.hwndFrom   =   m_treeTG.m_hWnd; 
	vi.hdr.idFrom   =   ::GetWindowLong(m_treeTG.m_hWnd, GWL_ID); 
	vi.hdr.code   =   TVN_SELCHANGED; 
	vi.action   =   TVN_SELCHANGED; 
 
	::SendMessage( m_treeTG.m_hWnd, WM_NOTIFY, (WPARAM)vi.hdr.idFrom, (LPARAM)&vi );
     
	m_treeTG.UpdateData(FALSE);

}

void CGroupTimeDlg::OnButtonUpload() 
{
	// TODO: Add your control notification handler code here

// 
// 	int ID =  m_treeTG.GetItemData( m_hCurItem );
// 
// 	CString sql ;
// 	sql.Format( _T("select * from [timegroup] where ([ID]=%d)"), ID );
// 	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
// 	if ( !tmp->IsEmpty() )
// 	{
// 		 
// 		DEV_TIMEGROUP dev_tg = {0};
// 		dev_tg.m_TGID = ID;
// 
// 		dev_tg.m_TGType = tmp->GetLong( _T("type") );
// 		dev_tg.m_CheckFlag = tmp->GetLong( _T("checkflag") );
// 		dev_tg.m_WeekFlag = tmp->GetLong( _T("weekflag") );
// 
// 		CString start = tmp->GetCString( _T("start") );
// 		CString end = tmp->GetCString( _T("end") );
// 	 
// 		COleDateTime time;
// 		if ( time.ParseDateTime(start) ) 
// 		{
// 			dev_tg.m_Start.m_Date.m_Year = time.GetYear();
// 			dev_tg.m_Start.m_Date.m_Month = time.GetMonth();
// 			dev_tg.m_Start.m_Date.m_Day = time.GetDay();
// 		}
// 		if ( time.ParseDateTime(end) )
// 		{
// 			dev_tg.m_End.m_Date.m_Year = time.GetYear();
// 			dev_tg.m_End.m_Date.m_Month = time.GetMonth();
// 			dev_tg.m_End.m_Date.m_Day = time.GetDay();
// 		}
// 		CPM_SetTimeGroup()
// 	}

}

void CGroupTimeDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	NMHDR hr = {0};
	hr.hwndFrom = m_checkY.GetSafeHwnd();
	hr.idFrom = m_checkY.GetDlgCtrlID();
	hr.code = NM_CLICK;
	m_checkY.SendMessage(WM_NOTIFY, 0, (LPARAM)&hr);
}
