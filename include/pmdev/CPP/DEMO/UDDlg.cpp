// UDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "UDDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "\\192.168.1.3\uictr\skinimpl.h"
/////////////////////////////////////////////////////////////////////////////
// CUDDlg dialog

// void ResultNotify( const char* dev_addr, DEV_OPER_TYPE oper_type, DEV_OPER_FLAG oper_flag, void* oper_content, int serial_num, DEV_OPER_RESULT rt, void* param )
// {
// 	CUDDlg* pDlg = (CUDDlg*)param;
// 	pDlg->Recv( dev_addr, oper_type, oper_flag, oper_content, serial_num, rt );
// }


CUDDlg::CUDDlg(CHCADOConnection& ado,const char* dev_addr, int flag,CWnd* pParent /*=NULL*/)
	: m_ado(ado),m_flag(flag), CDialog(CUDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset( m_IP, 0x0, sizeof(m_IP) );
	memcpy( m_IP, dev_addr, strlen(dev_addr) );
	m_nItems = 0;
}


void CUDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUDDlg)
	DDX_Control(pDX, IDC_LIST_TG, m_listTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUDDlg, CDialog)
	//{{AFX_MSG_MAP(CUDDlg)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, OnButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDDlg message handlers

BOOL CUDDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listTree.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP|LVS_EX_CHECKBOXES );
	RECT rt;
	m_listTree.GetClientRect( &rt );
	int width = (rt.right-rt.left);
	m_listTree.InsertColumn( 0, _T("选择"), LVCFMT_CENTER, (width / 3) / 2 );
	m_listTree.InsertColumn( 1, _T("ID"), LVCFMT_CENTER, (width / 3) / 2 );
	if ( m_flag == OPER_TIMEGROUP )
	{		
		m_listTree.InsertColumn( 2, _T("时间组"), LVCFMT_CENTER, width - (width / 3)  );
		LoadTimegroup();
	}
	else if ( m_flag == OPER_RIGHT )
	{
		m_listTree.InsertColumn( 2, _T("权限"), LVCFMT_CENTER, width - (width / 3)  );
		LoadRight();
	}
	else
	{
		m_listTree.InsertColumn( 2, _T("用户组"), LVCFMT_CENTER, width - (width / 3)  );
		LoadUserGroup();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUDDlg::LoadTimegroup()
{
	m_listTree.DeleteAllItems();
 
	CString sql(_T("select [ID], [name] from [timegroup]"));
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();
		CString name ;
		int     ID = 0;
		int n = 0;
		m_nItems = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext(), ++n )
		{
			name = tmp->GetCString( _T("name") );
			ID   = tmp->GetLong( _T("ID") );
			n = m_listTree.InsertItem(n,"");
			CString values ;
			values.Format( _T("%d"), ID );
			m_listTree.SetItemText( n, 1, values );
			m_listTree.SetItemText( n, 2, name );
			m_listTree.SetItemData( n, ID );
		}
		m_nItems = n;
	}
}

void CUDDlg::LoadRight()
{
	m_listTree.DeleteAllItems();	
	CString sql(_T("select [RightID], [Name] from [right]"));
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();
		CString name ;
		int     ID = 0;
		int n = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext(), ++n )
		{
			name = tmp->GetCString( _T("Name") );
			ID   = tmp->GetLong( _T("RightID") );
			n = m_listTree.InsertItem(n,"");
			CString values ;
			values.Format( _T("%d"), ID );
			m_listTree.SetItemText( n, 1, values );
			m_listTree.SetItemText( n, 2, name );
			m_listTree.SetItemData( n, ID );
		}
		m_nItems = n;
	}
}

void CUDDlg::LoadUserGroup()
{
	m_listTree.DeleteAllItems();	
	CString sql(_T("select [GroupID], [Name] from [ugroup]"));
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();
		CString name ;
		int     ID = 0;
		int n = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext(), ++n )
		{
			name = tmp->GetCString( _T("Name") );
			ID   = tmp->GetLong( _T("GroupID") );
			n = m_listTree.InsertItem(n,"");
			CString values ;
			values.Format( _T("%d"), ID );
			m_listTree.SetItemText( n, 1, values );
			m_listTree.SetItemText( n, 2, name );
			m_listTree.SetItemData( n, ID );
		}
		m_nItems = n;
	}
}

void CUDDlg::UpLoadTimeGroup()
{
	if ( m_nItems <= 0 ) return;
	DEV_TIMEGROUP* pGroups = new DEV_TIMEGROUP[m_nItems];
	memset( pGroups, 0x0, sizeof(DEV_TIMEGROUP)*m_nItems );
	int items = m_listTree.GetItemCount();
	int ID = -1;
	CString sql ;
	int index = 0;
	for ( int i = 0; i < items; ++i )
	{
		if (  m_listTree.GetCheck(i) == 1 )
		{
			ID =  m_listTree.GetItemData( i );
			sql.Format( _T("select * from [timegroup] where ([ID]=%d)"), ID );
			CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
			if ( !tmp->IsEmpty() )
			{				 
				(pGroups+index)->m_TGID = ID;
				
				(pGroups+index)->m_TGType = DEV_TIMEGROUP_TYPE(tmp->GetLong( _T("type") ));
				(pGroups+index)->m_CheckFlag = tmp->GetLong( _T("checkflag") );
				(pGroups+index)->m_WeekFlag = tmp->GetLong( _T("weekflag") );
				
				CString start = tmp->GetCString( _T("start") );
				CString end = tmp->GetCString( _T("end") );
				
				COleDateTime time;
				if ( time.ParseDateTime(start) ) 
				{
					(pGroups+index)->m_Start.m_Date.m_Year = time.GetYear();
					(pGroups+index)->m_Start.m_Date.m_Month = time.GetMonth();
					(pGroups+index)->m_Start.m_Date.m_Day = time.GetDay();

					(pGroups+index)->m_Start.m_Time.m_Hour = time.GetHour();
					(pGroups+index)->m_Start.m_Time.m_Minute = time.GetMinute();
					(pGroups+index)->m_Start.m_Time.m_Second = time.GetSecond();

				}
				if ( time.ParseDateTime(end) )
				{
					(pGroups+index)->m_End.m_Date.m_Year = time.GetYear();
					(pGroups+index)->m_End.m_Date.m_Month = time.GetMonth();
					(pGroups+index)->m_End.m_Date.m_Day = time.GetDay();

					(pGroups+index)->m_End.m_Time.m_Hour = time.GetHour();
					(pGroups+index)->m_End.m_Time.m_Minute = time.GetMinute();
					(pGroups+index)->m_End.m_Time.m_Second = time.GetSecond();

				}
				index++;
			}
		}
	}
	if ( index > 0 )
	{
// 		DEV_TIMEGROUP_ARRAY tmp = {0};
// 		tmp.m_nCount = index;
// 		tmp.m_pTGArray = pGroups;
// 		CPM_ULTimeGroup( m_IP, tmp );
		DEBUG_INFO1( _T("%s 时间组上传完成"), m_IP );
	}
	delete [] pGroups;
}

void CUDDlg::UpLoadRight()
{
	if ( m_nItems <= 0 ) return;
	DEV_RIGHT* pRight = new DEV_RIGHT[m_nItems];
	memset( pRight, 0x0, sizeof(DEV_RIGHT)*m_nItems );
	int items = m_listTree.GetItemCount();
	int ID = -1;
	CString sql ;
	int index = 0;
	for ( int i = 0; i < items; ++i )
	{
		if (  m_listTree.GetCheck(i) == 1 )
		{
			ID =  m_listTree.GetItemData( i );
			sql.Format( _T("select * from [right] where ([RightID]=%d)"), ID );
			CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
			if ( !tmp->IsEmpty() )
			{				 

				(pRight+index)->m_RightID = ID;    
				
				(pRight+index)->m_TimeGID[0] = tmp->GetLong( _T("TimeGID_0") );  
				(pRight+index)->m_TimeGID[1] = tmp->GetLong( _T("TimeGID_1") ); 
				(pRight+index)->m_TimeGID[2] = tmp->GetLong( _T("TimeGID_2") ); 
				(pRight+index)->m_TimeGID[3] = tmp->GetLong( _T("TimeGID_3") ); 
				(pRight+index)->m_TimeGID[4] = tmp->GetLong( _T("TimeGID_4") ); 
				(pRight+index)->m_TimeGID[5] = tmp->GetLong( _T("TimeGID_5") ); 
				(pRight+index)->m_TimeGID[6] = tmp->GetLong( _T("TimeGID_6") ); 
				(pRight+index)->m_TimeGID[7] = tmp->GetLong( _T("TimeGID_7") ); 


				(pRight+index)->m_bHolidyValid =   tmp->GetLong( _T("HolidyValid") ) ;
				(pRight+index)->m_bActionLock =	   tmp->GetLong( _T("ActionLock") )  ;
				(pRight+index)->m_bActionOutPut =  tmp->GetLong( _T("ActionOutPut") )  ;
				
				index++;
			}
		}
	}
	if ( index > 0 )
	{
// 		DEV_RIGHT_ARRAY tmp = {pRight, index};
// 		CPM_ULRight(m_IP, tmp);
		DEBUG_INFO1( _T("%s 权限上传完成"), m_IP );
	}
	delete [] pRight;
}

void CUDDlg::UpUserGroup()
{
	if ( m_nItems <= 0 ) return;
	DEV_USERGROUP* pRight = new DEV_USERGROUP[m_nItems];
	memset( pRight, 0x0, sizeof(DEV_USERGROUP)*m_nItems );
	int items = m_listTree.GetItemCount();
	int ID = -1;
	CString sql ;
	int index = 0;
	for ( int i = 0; i < items; ++i )
	{
		if (  m_listTree.GetCheck(i) == 1 )
		{
			ID =  m_listTree.GetItemData( i );
			sql.Format( _T("select * from [ugroup] where ([GroupID]=%d)"), ID );
			CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
			if ( !tmp->IsEmpty() )
			{				 
				
				(pRight+index)->m_GroupID = ID;    
				(pRight+index)->m_NormalValid =  tmp->GetLong( _T("NormalValid") ); 
				(pRight+index)->m_ForceValid =  tmp->GetLong( _T("ForceValid") ); 
				(pRight+index)->m_bGroupOrder=  tmp->GetLong( _T("GroupOrder") );
				
				strcpy( (pRight+index)->m_NormalUsers[0].m_ID, tmp->GetCString( _T("NormalUsers_0")).GetBuffer(0) ); 
				strcpy( (pRight+index)->m_NormalUsers[1].m_ID, tmp->GetCString( _T("NormalUsers_1")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_NormalUsers[2].m_ID, tmp->GetCString( _T("NormalUsers_2")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_NormalUsers[3].m_ID, tmp->GetCString( _T("NormalUsers_3")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_NormalUsers[4].m_ID, tmp->GetCString( _T("NormalUsers_4")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_NormalUsers[5].m_ID, tmp->GetCString( _T("NormalUsers_5")).GetBuffer(0) );  
				
				strcpy( (pRight+index)->m_ForceUsers[0].m_ID, tmp->GetCString( _T("ForceUsers_0")).GetBuffer(0) ); 
				strcpy( (pRight+index)->m_ForceUsers[1].m_ID, tmp->GetCString( _T("ForceUsers_1")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_ForceUsers[2].m_ID, tmp->GetCString( _T("ForceUsers_2")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_ForceUsers[3].m_ID, tmp->GetCString( _T("ForceUsers_3")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_ForceUsers[4].m_ID, tmp->GetCString( _T("ForceUsers_4")).GetBuffer(0) );  
				strcpy( (pRight+index)->m_ForceUsers[5].m_ID, tmp->GetCString( _T("ForceUsers_5")).GetBuffer(0) );  					
				index++;
			}
		}
	}
	if ( index > 0 )
	{
// 		DEV_USERGROUP_ARRAY tmp = {pRight, index};
// 		CPM_ULUserGroup(m_IP, tmp);
		DEBUG_INFO1( _T("%s 权限上传完成"), m_IP );
	}
	delete [] pRight;
}

void CUDDlg::DownLoadTimeGroup()
{
// 	CPM_RegOperResultNotifyCB( ::ResultNotify, this );
// 	DEV_TIMEGROUP_ARRAY tga = {0};
// 	
// 	CPM_DLTimeGroup( m_IP, tga, DEV_ANY_TIME );
	DEBUG_INFO1( _T("%s 请求下载时间组"), m_IP );
}

void CUDDlg::DownLoadRight()
{
// 	CPM_RegOperResultNotifyCB( ::ResultNotify, this );
// 	DEV_RIGHT_ARRAY ra = {0};
// 	CPM_DLRight( m_IP, ra );
	DEBUG_INFO1( _T("%s 请求下载权限"), m_IP );
}

void CUDDlg::DownUserGroup()
{
// 	CPM_RegOperResultNotifyCB( ::ResultNotify, this );
// 	DEV_USERGROUP_ARRAY ua = {0};
// 	CPM_DLUserGroup( m_IP, ua );
	DEBUG_INFO1( _T("%s 请求下载权限"), m_IP );
}

#if 0
void CUDDlg::Recv( const char* dev_addr, DEV_OPER_TYPE oper_type, DEV_OPER_FLAG oper_flag, void* oper_content, int serial_num, DEV_OPER_RESULT rt )
{
	if ( OPER_SUCCESS != rt || oper_flag != DEV_OPER_DOWNLOAD ) return;

	if ( m_flag == OPER_TIMEGROUP )
	{
		if ( oper_type == DEV_TIME_GROUP_OPER )
			TimeGroupInfo( dev_addr,  *((DEV_TIMEGROUP_ARRAY*)oper_content) );
	}
	else if ( m_flag == OPER_RIGHT )
	{
		if ( oper_type == DEV_RIGHT_OPER )
			RightInfo( dev_addr,  *((DEV_RIGHT_ARRAY*)oper_content) );
	}
	else
	{
		if ( oper_type == DEV_USERGROUP_OPER )
			UserGroupInfo( dev_addr,  *((DEV_USERGROUP_ARRAY*)oper_content) );
	}
}


void CUDDlg::TimeGroupInfo( const char* dev_addr, const DEV_TIMEGROUP_ARRAY& time_group )
{
	CString sql;
	DEV_TIMEGROUP* pTmp = time_group.m_pTGArray;
	for (int i = 0; i < time_group.m_nCount; ++i)
	{
		sql.Format( _T("select * from [timegroup] where ([ID]=%d)"), pTmp->m_TGID );
		CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
		if ( tmp->IsEmpty() ) tmp->NewRecord();
		if ( !tmp->IsEmpty() )
		{
			CString time ;
			time.Format( _T("%02d/%02d/%04d"), pTmp->m_Start.m_Date.m_Month, pTmp->m_Start.m_Date.m_Day, pTmp->m_Start.m_Date.m_Year);
			tmp->SetCString( _T("start"), time );
			time.Format( _T("%02d/%02d/%04d"), pTmp->m_End.m_Date.m_Month, pTmp->m_End.m_Date.m_Day, pTmp->m_End.m_Date.m_Year);
			tmp->SetCString( _T("end"), time );
			tmp->SetLong( _T("type"), pTmp->m_TGType );
			tmp->SetLong( _T("checkflag"), pTmp->m_CheckFlag );
			tmp->SetLong( _T("weekflag"), pTmp->m_WeekFlag );
			tmp->Update();
		}
		DEBUG_INFO1( _T("时间组 %d 下载完成"), pTmp->m_TGID );
		pTmp++;
	}
    
}

void CUDDlg::RightInfo( const char* dev_addr, const DEV_RIGHT_ARRAY& right )
{
	CString sql;
	DEV_RIGHT* pTmp = right.m_pRtArray;
	for ( int i = 0; i < right.m_nCount; ++i )
	{
		sql.Format( _T("select * from [right] where ([RightID]=%d)"), pTmp->m_RightID );
		CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
		if ( tmp->IsEmpty() ) tmp->NewRecord();
		if ( !tmp->IsEmpty() )
		{
			
			tmp->SetLong( _T("TimeGID_0"),  pTmp->m_TimeGID[0] );  
			tmp->SetLong( _T("TimeGID_1"),  pTmp->m_TimeGID[1] ); 
			tmp->SetLong( _T("TimeGID_2"),  pTmp->m_TimeGID[2] ); 
			tmp->SetLong( _T("TimeGID_3"),  pTmp->m_TimeGID[3] ); 
			tmp->SetLong( _T("TimeGID_4"),  pTmp->m_TimeGID[4] ); 
			tmp->SetLong( _T("TimeGID_5"),  pTmp->m_TimeGID[5] ); 
			tmp->SetLong( _T("TimeGID_6"),  pTmp->m_TimeGID[6] ); 
			tmp->SetLong( _T("TimeGID_7"),  pTmp->m_TimeGID[7] ); 
			
// 			tmp->SetLong( _T("nvalid"), pTmp->m_NormalValid ); 
// 			tmp->SetLong( _T("fvalid"), pTmp->m_ForceValid ); 
// 			
// 			tmp->SetCString( _T("nuserid1"), pTmp->m_NormalUsers[0].m_ID ) ; 
// 			tmp->SetCString( _T("nuserid2"), pTmp->m_NormalUsers[1].m_ID ) ;  
// 			tmp->SetCString( _T("nuserid3"), pTmp->m_NormalUsers[2].m_ID ) ;  
// 			tmp->SetCString( _T("nuserid4"), pTmp->m_NormalUsers[3].m_ID ) ;   
// 			tmp->SetCString( _T("nuserid5"), pTmp->m_NormalUsers[4].m_ID ) ;  
// 			tmp->SetCString( _T("nuserid6"), pTmp->m_NormalUsers[5].m_ID ) ;  
// 			
// 			tmp->SetCString( _T("fuserid1"), pTmp->m_ForceUsers[0].m_ID ) ; 
// 			tmp->SetCString( _T("fuserid1"), pTmp->m_ForceUsers[1].m_ID ) ;  
// 			tmp->SetCString( _T("fuserid1"), pTmp->m_ForceUsers[2].m_ID ) ;  
// 			tmp->SetCString( _T("fuserid1"), pTmp->m_ForceUsers[3].m_ID ) ;   
// 			tmp->SetCString( _T("fuserid1"), pTmp->m_ForceUsers[4].m_ID ) ;  
// 			tmp->SetCString( _T("fuserid1"), pTmp->m_ForceUsers[5].m_ID ) ;  
			
// 			int mode = 0;
// 			mode |= pTmp->m_bHolidyValid << 5;
// 			mode |= pTmp->m_bCombOpenDoor << 6;
// 			mode |= pTmp->m_bSeqOpenDoor << 7;
			tmp->SetLong( _T("HolidyValid"), pTmp->m_bHolidyValid ); 
			tmp->SetLong( _T("ActionLock"), pTmp->m_bActionLock ); 
			tmp->SetLong( _T("ActionOutPut"), pTmp->m_bActionOutPut ); 

			tmp->Update();
		}
		DEBUG_INFO1( _T("权限 %d 下载完成"), pTmp->m_RightID );
		pTmp++;
	}
}

void CUDDlg::UserGroupInfo( const char* dev_addr, const DEV_USERGROUP_ARRAY& right )
{
	CString sql;
	DEV_USERGROUP* pTmp = right.m_pUGArray;
	for ( int i = 0; i < right.m_nCount; ++i )
	{
		sql.Format( _T("select * from [ugroup] where ([GroupID]=%d)"), pTmp->m_GroupID );
		CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
		if ( tmp->IsEmpty() ) tmp->NewRecord();
		if ( !tmp->IsEmpty() )
		{	
			tmp->SetLong( _T("NormalValid"), pTmp->m_NormalValid ); 
			tmp->SetLong( _T("ForceValid"), pTmp->m_ForceValid ); 
			
			tmp->SetCString( _T("NormalUsers_0"), pTmp->m_NormalUsers[0].m_ID ) ; 
			tmp->SetCString( _T("NormalUsers_1"), pTmp->m_NormalUsers[1].m_ID ) ;  
			tmp->SetCString( _T("NormalUsers_2"), pTmp->m_NormalUsers[2].m_ID ) ;  
			tmp->SetCString( _T("NormalUsers_3"), pTmp->m_NormalUsers[3].m_ID ) ;   
			tmp->SetCString( _T("NormalUsers_4"), pTmp->m_NormalUsers[4].m_ID ) ;  
			tmp->SetCString( _T("NormalUsers_5"), pTmp->m_NormalUsers[5].m_ID ) ;  
			
			tmp->SetCString( _T("ForceUsers_0"), pTmp->m_ForceUsers[0].m_ID ) ; 
			tmp->SetCString( _T("ForceUsers_1"), pTmp->m_ForceUsers[1].m_ID ) ;  
			tmp->SetCString( _T("ForceUsers_2"), pTmp->m_ForceUsers[2].m_ID ) ;  
			tmp->SetCString( _T("ForceUsers_3"), pTmp->m_ForceUsers[3].m_ID ) ;   
			tmp->SetCString( _T("ForceUsers_4"), pTmp->m_ForceUsers[4].m_ID ) ;  
			tmp->SetCString( _T("ForceUsers_5"), pTmp->m_ForceUsers[5].m_ID ) ;  
			
		 
			tmp->SetLong( _T("GroupOrder"), pTmp->m_bGroupOrder ); 
		 
			
			tmp->Update();
		}
//		DEBUG_INFO1( _T("权限 %d 下载完成"), pTmp->m_RightID );
		pTmp++;
	}
}
#endif

void CUDDlg::DownDelTimeGroup()
{
	if ( m_nItems <= 0 ) return;

	DEV_TIMEGROUP* pGroups = new DEV_TIMEGROUP[m_nItems];
	memset( pGroups, 0x0, sizeof(DEV_TIMEGROUP)*m_nItems );

	int items = m_listTree.GetItemCount();
	int ID = -1;
	CString sql ;
	int index = 0;
	for ( int i = 0; i < items; ++i )
	{
		if (  m_listTree.GetCheck(i) == 1 )
		{
			pGroups[index++].m_TGID =  m_listTree.GetItemData( i );		    
		}
	}
	if ( index > 0 )
	{
// 		DEV_TIMEGROUP_ARRAY tmp = {pGroups,index};
// 		CPM_DELTimeGroup(m_IP, tmp, DEV_ANY_TIME);
		DEBUG_INFO1( _T("%s 时间组删除完成"), m_IP );
	}
	delete [] pGroups;
}

void CUDDlg::DownDelRight()
{
	if ( m_nItems <= 0 ) return;

	DEV_RIGHT* pRight = new DEV_RIGHT[m_nItems];
	memset( pRight, 0x0, sizeof(DEV_RIGHT)*m_nItems );

	int items = m_listTree.GetItemCount();

	CString sql ;
	int index = 0;
	for ( int i = 0; i < items; ++i )
	{
		if (  m_listTree.GetCheck(i) == 1 )
		{
			pRight[index++].m_RightID =  m_listTree.GetItemData( i );		    
		}
	}
	if ( index > 0 )
	{
// 		DEV_RIGHT_ARRAY tmp = {pRight, index};
// 		CPM_DELRight(m_IP, tmp);
		DEBUG_INFO1( _T("%s 权限删除完成"), m_IP );
	}
	delete [] pRight;
}

void CUDDlg::DelUserGroup()
{
	if ( m_nItems <= 0 ) return;
	
	DEV_USERGROUP* pRight = new DEV_USERGROUP[m_nItems];
	memset( pRight, 0x0, sizeof(DEV_USERGROUP)*m_nItems );
	
	int items = m_listTree.GetItemCount();
	
	CString sql ;
	int index = 0;
	for ( int i = 0; i < items; ++i )
	{
		if (  m_listTree.GetCheck(i) == 1 )
		{
			pRight[index++].m_GroupID =  m_listTree.GetItemData( i );		    
		}
	}
	if ( index > 0 )
	{
// 		DEV_USERGROUP_ARRAY tmp = {pRight, index};
// 		CPM_DELUserGroup(m_IP, tmp);
		DEBUG_INFO1( _T("%s 权限删除完成"), m_IP );
	}
	delete [] pRight;
}

void CUDDlg::OnButtonUpload() 
{
	// TODO: Add your control notification handler code here
	if ( m_flag == OPER_TIMEGROUP )
	{		
		UpLoadTimeGroup();
	}
	else if ( m_flag == OPER_RIGHT )
	{
		UpLoadRight();
	}
	else
	{
		UpUserGroup();
	}

}

void CUDDlg::OnButtonDownload() 
{
	// TODO: Add your control notification handler code here

	if ( m_flag == OPER_TIMEGROUP )
	{		
		DownLoadTimeGroup();
	}
	else if ( m_flag == OPER_RIGHT )
	{
		DownLoadRight();
	}
	else
	{
		DownUserGroup();
	}
}

void CUDDlg::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	if ( m_flag == OPER_TIMEGROUP )
	{		
		DownDelTimeGroup();
	}
	else if ( m_flag == OPER_RIGHT )
	{
		DownDelRight();
	}
	else
	{
		DelUserGroup();
	}
}
