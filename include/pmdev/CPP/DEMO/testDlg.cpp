// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "Picture.h"
#include "GroupTimeDlg.h"
#include "RightDlg.h"
#include "UDDlg.h"
#include "AuthDlg.h"
#include "UserGroupDlg.h"
#include <assert.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#include "WebServiceMrg.h"
#include "CreateDynamicDlg.h"
#include "PasswordDlg.h"

#include "VideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
#define BASE_MSG WM_USER+0x1001
#define USER_MSG BASE_MSG+1
#define REC_MSG USER_MSG+1
#define LOG_MSG REC_MSG+1
#define AUTH_MSG LOG_MSG+1
#define IMAGE_MSG AUTH_MSG+1
#define RLOAD_MSG IMAGE_MSG+1
#define USER_NUMS_MSG RLOAD_MSG+1
#define LOG_NUMS_MSG USER_NUMS_MSG+1
#define REC_NUMS_MSG LOG_NUMS_MSG+1
#define CIMAGE_NUMS_MSG REC_NUMS_MSG+1
#define ALARMS_NUMS_MSG REC_NUMS_MSG+1

//return( 8 * ( s->p - s->p_start ) + 8 - s->i_left );

#include ".\skin\test_CSuperGridCtrl.h"
#pragma comment(lib, "skin\\Skindll.lib")
//#pragma comment(lib, "//192.168.1.3/pub/cstd/lib/Skindll.lib")

#define SERCH_EVENT_TIME 0x01
#define HERAT_PACK_TIME 0x02
#define CLEARLCLICKTIME 0x03

BOOL CALLBACK DevConnectNotify(const char* cszDevAddr, DEV_CONNECT_STATUS eCNStatus, void* pvContext)
{
	CTestDlg* pthis = (CTestDlg*)pvContext;
	assert( pthis );
	pthis->DevConnectNotify(cszDevAddr, eCNStatus);
	return TRUE;
}

//void DevOperResult( const char* cszServerAddr, const char* cszDevAddr, NET_OPER_CMD eCMD, NET_OPER_RESULT eRt, void* pvContent, int nSN, void* param )
BOOL CALLBACK DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext )
{
	CTestDlg* pthis = (CTestDlg*)pvContext;
	pthis->DevOperResult( cszDevAddr, eType, eFlag , pvContent, nSeq , eResult );
	return TRUE;
}

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_ETimeRec = 0;
	m_ETimeUser = 0;
	m_STimeRec = 0;
	m_STimeUser = 0;
	m_nFeatMax = 20;
	m_nPicMax = 20; 
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_BASEINFO_OPER, &CTestDlg::DevBaseInfoOper) );
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_AUTH_OPER, &CTestDlg::DevAuthOper) );
	//m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(CMD_AUTH_SET, &CTestDlg::DevAuthOper) );
	
	//    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(CMD_ADD_DEV, &CTestDlg::DevAddOper) );             
    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_FORMAT_OPER, &CTestDlg::DevFormatOper) );
	//用户
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_USER_OPER, &CTestDlg::DevUserInfoOper) );
    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_USER_EX_OPER, &CTestDlg::DevUserExOper) );
	
	// 	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(CMD_RTCAP_USER, &CTestDlg::DevUserInfoOper) );
	// 	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(CMD_RTUPDATE_USER, &CTestDlg::DevUserInfoOper) );
	// 	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(CMD_GET_USER_COUNT	, &CTestDlg::DevUserInfoOper) );
	
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_USER_RANGE_OPER, &CTestDlg::DevUserNumsOper) );
    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_USER_RT_OPER, &CTestDlg::DevRealLoadUser) );
	
	//记录
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_RECORD_OPER, &CTestDlg::DevRecordInfoOper) );
    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_ALARM_OPER, &CTestDlg::DevRecordInfoOper) );
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_RECORD_RANGE_OPER, &CTestDlg::DevRecNums) );
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_ALARM_RANGE_OPER, &CTestDlg::DevRecNums) );
	
	
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_LOG_OPER, &CTestDlg::DevLogInfoOper) );
    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_LOG_EX_OPER, &CTestDlg::DevLogExOper) );
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_LOG_RANGE_OPER, &CTestDlg::DevLogNumsOper) );
	
    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_CIMAGE_EX_OPER, &CTestDlg::DevCImageExOper) );
	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_CHANGEIMAGE_OPER, &CTestDlg::DevChangeImageOper) );
    m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_CIMAGE_RANGE_OPER, &CTestDlg::DevChangeImageNumsOper) );


	
	// 	m_map.insert( std::pair<DEV_OPER_TYPE,OperResult>(DEV_USER_RT_OPER, &CTestDlg::DevRealLoadUser) );
	
    memset(m_serverIP,0,sizeof(m_serverIP));
	memset(m_IP, 0x0, sizeof(m_IP));
	memcpy(m_IP, "224.0.1.200", strlen("224.0.1.200"));
	
	m_nImageItems = 0;
	
	unsigned int pieces = 207384 / (64*1024) ;
	unsigned int arith = 207384 % (64*1024) ;
	m_nLClickTimes = 0;
}

CTestDlg::~CTestDlg()
{
	CPM_InitSys(FALSE);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_IPADDRESS_ADDR2, m_IPDev);
	DDX_Control(pDX, IDC_IPADDRESS_ADDR, m_IPCtrl);
	DDX_Control(pDX, IDC_IPADDRESS_S, m_IPServer);	
	
	DDX_Control(pDX, IDC_COMBO_CONDITION, m_cmbCondition);
	DDX_Control(pDX, IDC_COMBO_RECORD, m_cmbRecordCondition);
	DDX_Control(pDX, IDC_STATIC_PIC, m_static);
	DDX_Control(pDX, IDC_STATIC_RECORD_PIC, m_staticRecord);
	DDX_Control(pDX, IDC_LIST_USER, m_user);
	DDX_Control(pDX, IDC_LIST_DEV, m_dev);
	DDX_Control(pDX, IDC_LIST_RECORD, m_record);
	DDX_Control(pDX, IDC_LIST_LOG, m_log);
	DDX_Control(pDX, IDC_LIST_IMAGE, m_image);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_END_REC, m_ETimeRec);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_END_USER, m_ETimeUser);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_START_REC, m_STimeRec);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_START_USER, m_STimeUser);
	
	
	DDX_Text( pDX, IDC_EDIT_FEAT, m_nFeatMax );
	DDX_Text( pDX, IDC_EDIT_PIC, m_nPicMax );
	//}}AFX_DATA_MAP
}


#ifndef WM_UPDATE_IMAGE
#define WM_UPDATE_IMAGE WM_USER+0x1011
#endif

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
//{{AFX_MSG_MAP(CTestDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_SERCH, OnButtonSerch)
ON_NOTIFY(NM_RCLICK, IDC_LIST_DEV, OnRButtonDown)
ON_COMMAND_RANGE( IDM_DEVSTATIS, IDM_DEVSTATIS+20, OnCommandMenu )
ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
ON_NOTIFY(NM_CLICK, IDC_LIST_USER, OnLClickUserList )
ON_NOTIFY(NM_CLICK, IDC_LIST_RECORD, OnLClickRecordList )
ON_NOTIFY(NM_CLICK, IDC_LIST_IMAGE, OnLClickCImageList )
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
ON_BN_CLICKED(IDC_BUTTON_PIC, OnButtonPic)
ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
ON_BN_CLICKED(IDC_BUTTON_DLRECORD, OnButtonDlrecord)
ON_BN_CLICKED(IDC_BUTTON_DELRECORD, OnButtonDelrecord)
ON_WM_CLOSE()
ON_WM_LBUTTONUP()
ON_BN_CLICKED(IDC_BUTTON_TIMEGROUP, OnButtonTimegroup)
ON_BN_CLICKED(IDC_BUTTON_RRIGHT, OnButtonRright)
ON_BN_CLICKED(IDC_BUTTON_UG, OnButtonUG)
ON_BN_CLICKED(IDC_BUTTON_OPEN_DOOR, OnButtonOpenDoor)
ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
ON_BN_CLICKED(IDC_BUTTON_SETADDR, OnButtonSetaddr)
ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
ON_BN_CLICKED(IDC_BUTTON_CN, OnButtonCNServer)
ON_BN_CLICKED(IDC_BUTTON_DCN, OnButtonDCNServer)
ON_BN_CLICKED(IDC_BUTTON_FILE, OnFile)
ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnUpdate)
ON_BN_CLICKED(IDC_BUTTON_DL_LOG, OnButtonDLLog )
ON_BN_CLICKED(IDC_BUTTON_DEL_LOG, OnButtonDelLog )
ON_BN_CLICKED(IDC_BUTTON_DL_IMAGE, OnButtonDLImage )
ON_BN_CLICKED(IDC_BUTTON_DEL_IMAGE, OnButtonDelImage )
ON_COMMAND_RANGE( IDC_RADIO_NET, IDC_RADIO_UDISK, OnCmdNetOrUDisk )
ON_BN_CLICKED(IDC_BUTTON_UPATH, OnBtnUDiskScan)
ON_BN_CLICKED(IDC_BUTTON_CN_UDISK, OnBtnUDiskCN)
ON_MESSAGE(WM_UPDATE_IMAGE, OnUpdateImage)
ON_BN_CLICKED(IDC_BUTTON_CANCLE, OnButtonCancelOpenDoor)
ON_BN_CLICKED(IDC_BUTTON_REMOTE, OnButtonROpenDoor)
ON_BN_CLICKED(IDC_BUTTON_NOMAL, OnButtonECloseDoor)
ON_BN_CLICKED(IDC_BUTTON_R_CLOSE, OnButtonRCloseDoor)
ON_MESSAGE( BASE_MSG, DevBaseMsg )
ON_MESSAGE( USER_MSG, DevUserMsg )
ON_MESSAGE( REC_MSG, DevRecMsg )
ON_MESSAGE( LOG_MSG, DevLogMsg )
ON_MESSAGE( IMAGE_MSG, DevImageMsg )
ON_MESSAGE( LOG_NUMS_MSG, DevLogNumsMsg )
ON_MESSAGE( USER_NUMS_MSG, DevUserNumsMsg )
ON_MESSAGE( CIMAGE_NUMS_MSG, DevCImageNumsMsg )
ON_MESSAGE( REC_NUMS_MSG, DevRecNumsMsg )
ON_MESSAGE( ALARMS_NUMS_MSG, DevAlarmNumsMsg )
ON_BN_CLICKED( IDC_BUTTON_VIDEO, OnBtnStartVideo )
ON_BN_CLICKED( IDC_BUTTON_CAP, OnBtnCap )
ON_BN_CLICKED( IDC_BUTTON_REBOOT, OnBtnReboot )


//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers


unsigned long DotIpToLong( const char* ip )
{
	if ( NULL == ip ) return 0;
	unsigned long a = 0 ;
	unsigned long b = 0;
	unsigned long c = 0;
	unsigned long d = 0;
	sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
	return (a << 24) | (b<<16) | (c<<8) | d;
}

void LongToDotIp( char ip[], unsigned long l )
{
	unsigned long a = (l & 0xFF000000) >> 24 ;
	unsigned long b = (l & 0x00FF0000) >> 16 ;
	unsigned long c = (l & 0x0000FF00) >> 8 ;
	unsigned long d = (l & 0x000000FF)   ;
	sprintf( ip, "%d.%d.%d.%d", a, b, c, d);
}

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	int ddd = (1 << 31)-1;
	int cb = sizeof(DEV_DATE);
	((CButton*)GetDlgItem(IDC_RADIO_NET))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_UDISK))->SetCheck(0);
	m_bNet = TRUE;
	
	m_ado.Open( _T("user.mdb") );
	
	m_menu.CreatePopMenu( IDR_MENU );
	m_menu.InserMenuItem( IDM_DEVSTATIS, _T("统计信息") );
	m_menu.InserMenuItem( IDM_DEVSET, _T("工作属性") );
	m_menu.InserMenuItem( IDM_NETWORKSET, _T("网络设置") );
	m_menu.InserMenuItem( IDR_MENU_SYN_TIME, _T("时间同步") );
	m_menu.InserMenuItem( IDR_TIMEGROUP, _T("时间组") );
	m_menu.InserMenuItem( IDR_RIGHT, _T("权限") );
	m_menu.InserMenuItem( IDR_UGROUP, _T("用户组") );
	m_menu.InserMenuItem( IDR_DEV_FORMAT, _T("格式化") );
	m_menu.InserMenuItem( IDR_DEVAUTH, _T("设置验证") );
	m_menu.InserMenuItem( IDR_DEVCUT, _T("断开连接") );
//	m_menu.InserMenuItem( IDR_DMYPWD, _T("获取动态密码") );

	
	
	((CButton*)GetDlgItem(IDC_CHECK_PIC))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_FEAT))->SetCheck(1);
	
	((CButton*)GetDlgItem(IDC_CHECK_RECORD_PIC))->SetCheck(1);
	
	m_IPCtrl.SetAddress(224, 0, 1, 200);
	InitDev();
	InitUser();
	InitRecord();
	InitLog();
	InitCImage();
	m_cmbCondition.SetCurSel(0);
	m_cmbRecordCondition.AddString( _T("所有记录") );
	m_cmbRecordCondition.AddString( _T("最新识别记录") );
	m_cmbRecordCondition.AddString( _T("所有报警记录") );
	m_cmbRecordCondition.AddString( _T("时间段识别记录") );
	m_cmbRecordCondition.AddString( _T("最新报警记录") );
	m_cmbRecordCondition.AddString( _T("时间段报警记录") );
	m_cmbRecordCondition.AddString( _T("区间段识别记录") );
	m_cmbRecordCondition.AddString( _T("区间段报警记录") );
	m_cmbRecordCondition.SetCurSel(0);
	
	((CButton*)(GetDlgItem(IDC_RADIO_ALL)))->SetCheck(TRUE);
	
	
	CPM_InitSys(TRUE);
	CPM_RegDevConnectStatusCB( ::DevConnectNotify, this);
	CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	
	
	GetDlgItem( IDC_EDIT_FEAT )->SetWindowText( _T("20") );
	GetDlgItem( IDC_EDIT_PIC )->SetWindowText( _T("20") );

	SetTimer( CLEARLCLICKTIME, 1200, NULL );
	
	UpdateData(FALSE);
	
	
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CTestDlg::InitDev()
{
	RECT eRt;
	m_dev.GetClientRect( &eRt );
	int width = (eRt.right-eRt.left)/3;
	m_dev.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP  );
	
	m_dev.InsertColumn(0, "IP", LVCFMT_LEFT, width );
	m_dev.InsertColumn(1, "设备类型", LVCFMT_LEFT, width );
	m_dev.InsertColumn(2, "人数上线", LVCFMT_LEFT, width);
}

void CTestDlg::InitUser()
{
	RECT eRt;
	m_user.GetClientRect( &eRt );
	int width = (eRt.right-eRt.left)/7;
	// 	m_user.SetBkColor( RGB(231, 246, 253) );
	// 	m_user.ModifyStyle( 0, LBS_NOTIFY | LVS_OWNERDRAWFIXED );
	m_user.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP  );
	
	m_user.InsertColumn(0, "ID", LVCFMT_LEFT, width );
	m_user.InsertColumn(1, "卡号", LVCFMT_LEFT, width );
	m_user.InsertColumn(2, "姓名", LVCFMT_LEFT, width);
	m_user.InsertColumn(3, "权限", LVCFMT_LEFT, width);
	m_user.InsertColumn(4, "类型", LVCFMT_LEFT, width);
	m_user.InsertColumn(5, "黑名单", LVCFMT_LEFT, width);
	m_user.InsertColumn(6, "失效时间", LVCFMT_LEFT, width);
	
	
	CString sql = _T("select * from [user]");
	CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
	if ( ptr->IsEmpty() ) return;
	ptr->MoveFirst();
	int i = 0;
	int n = 0;
	char values[5] ={0};
	for (; !(ptr->IsEOF()); ptr->MoveNext(), ++i )
	{
		n = m_user.InsertItem( i, ptr->GetCString( _T("UserID") ) );
		m_user.SetItemText( n, 0, ptr->GetCString( _T("UserID") ) );
		m_user.SetItemText( n, 1, ptr->GetCString( _T("AppendID") ) );
		m_user.SetItemText( n, 2, ptr->GetCString( _T("UserName") ) );
		sprintf( values, "%d", ptr->GetLong( _T("RightID") ));	
		m_user.SetItemText( n, 3, values );
		sprintf( values, "%d", ptr->GetLong( _T("UserType") ));	
		m_user.SetItemText( n, 4, values );
		sprintf( values, "%d", ptr->GetLong( _T("UserFlag") ));
		m_user.SetItemText( n, 5, values );
		m_user.SetItemText( n, 6, ptr->GetCString( _T("InvalidTime") ) );
	}
}

void CTestDlg::InitRecord()
{
	UI_Init(m_record.GetSafeHwnd(), &aa, SW_CSuperGridCtrl);
	aa.m_crCellRGB = -1;
	//HIMAGELIST hIml = CImageList_Create( IDB_FOLDERS, 16, 1, RGB( 0, 255, 255 ) );
	//CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
	HIMAGELIST pImageList = CListCtrl_GetImageList( aa.m_hWnd, LVSIL_SMALL );
	if ( pImageList ) 
		ImageList_GetIconSize( pImageList, &aa.m_cxImage, &aa.m_cyImage );
	//CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
	CListCtrl_SetExtendedStyle( aa.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	LV_COLUMN lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	RECT eRt;
	m_record.GetClientRect( &eRt );
	int width = (eRt.right-eRt.left)/5;
	lvColumn.cx = width;
	
	
	lvColumn.pszText = "记录ID";
    CListCtrl_InsertColumn( aa.m_hWnd, 0, &lvColumn );
	lvColumn.pszText = "用户ID";
	CListCtrl_InsertColumn( aa.m_hWnd, 1, &lvColumn );
	lvColumn.pszText = "记录类型";
	CListCtrl_InsertColumn( aa.m_hWnd, 2, &lvColumn );
	lvColumn.pszText = "验证模式";
	CListCtrl_InsertColumn( aa.m_hWnd, 3, &lvColumn );
	lvColumn.pszText = "记录来源";
	CListCtrl_InsertColumn( aa.m_hWnd, 4, &lvColumn );
	
}

void CTestDlg::InitLog()
{
	RECT eRt;
	m_log.GetClientRect( &eRt );
	int width = (eRt.right-eRt.left)/5;
	m_log.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP  );
	
	m_log.InsertColumn(0, "日志ID", LVCFMT_LEFT, width );
	m_log.InsertColumn(1, "类型", LVCFMT_LEFT, width );
	m_log.InsertColumn(2, "操作员", LVCFMT_LEFT, width);
	m_log.InsertColumn(3, "被操作员", LVCFMT_LEFT, width);
	m_log.InsertColumn(4, "时间", LVCFMT_LEFT, width);
	
	//	m_log.InsertItem(0, "test");
}

void CTestDlg::InitCImage()
{
	UI_Init(m_image.GetSafeHwnd(), &bb, SW_CSuperGridCtrl);
	bb.m_crCellRGB = -1;
	//HIMAGELIST hIml = CImageList_Create( IDB_FOLDERS, 16, 1, RGB( 0, 255, 255 ) );
	//CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
	HIMAGELIST pImageList = CListCtrl_GetImageList( bb.m_hWnd, LVSIL_SMALL );
	if ( pImageList ) 
		ImageList_GetIconSize( pImageList, &bb.m_cxImage, &bb.m_cyImage );
	//CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
	CListCtrl_SetExtendedStyle( bb.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	LV_COLUMN lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	RECT eRt;
	m_image.GetClientRect( &eRt );
	int width = (eRt.right-eRt.left)/3;
	lvColumn.cx = width;
	
	
	lvColumn.pszText = "流水号ID";
    CListCtrl_InsertColumn( bb.m_hWnd, 0, &lvColumn );
	lvColumn.pszText = "用户ID";
	CListCtrl_InsertColumn( bb.m_hWnd, 1, &lvColumn );
	lvColumn.pszText = "时间";
	CListCtrl_InsertColumn( bb.m_hWnd, 2, &lvColumn );
	// 	lvColumn.pszText = "验证模式";
	// 	CListCtrl_InsertColumn( aa.m_hWnd, 3, &lvColumn );
	// 	lvColumn.pszText = "记录来源";
	// 	CListCtrl_InsertColumn( aa.m_hWnd, 4, &lvColumn );
	// 
	// 	m_image.InsertColumn(0, "流水号ID", LVCFMT_LEFT, width );
	// 	m_image.InsertColumn(1, "用户ID", LVCFMT_LEFT, width );
	// 	m_image.InsertColumn(2, "时间", LVCFMT_LEFT, width);
	
}

//搜索设备
void CTestDlg::OnButtonSerch() 
{
	static BOOL serch = TRUE;
	
	UpdateData(TRUE);
	BYTE field0 = 0, field1=0, field2=0, field3=0;
	m_IPCtrl.GetAddress(field0, field1, field2, field3);
	KillTimer(SERCH_EVENT_TIME);
	memset(m_IP, 0x0, sizeof(m_IP));
	sprintf(m_IP, "%d.%d.%d.%d", field0, field1, field2, field3);
	
	
	char ip[16] = {0};
	//	BYTE field0 = 0, field1=0, field2=0, field3=0;
	m_IPServer.GetAddress(field0, field1, field2, field3);
	sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
	
	//	CPM_SerchDev( ip/*, m_IP*/);
	//	serch ? SetTimer(SERCH_EVENT_TIME, 1000*10, NULL) : KillTimer( SERCH_EVENT_TIME );
	//	serch = FALSE;
}

void CTestDlg::OnButtonSetaddr() 
{
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	
	UpdateData(TRUE);
	BYTE field0 = 0, field1=0, field2=0, field3=0;
	m_IPCtrl.GetAddress(field0, field1, field2, field3);
	
	char ip[16] = {0};
	
	sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
	
	//	CPM_ULDevRegionAddr(str_ip.GetBuffer(0), ip);
}

void CTestDlg::OnRButtonDown( NMHDR * pNotifyStruct, LRESULT* result  )
{
	*result = CDRF_DODEFAULT;
	NMCLICK* pclick = (NMCLICK*)pNotifyStruct;
	
	if ( NULL == pclick ) return ;
	if ( (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) >= m_dev.GetItemCount() ) return ;
	
	CPoint pt;
	::GetCursorPos(&pt);
	::SetForegroundWindow( this->GetSafeHwnd() );
	::TrackPopupMenu(m_menu.m_hMenu, 0, pt.x, pt.y, 0, this->GetSafeHwnd(), NULL);
	
	return ;
}

void CTestDlg::OnCommandMenu(UINT nID)
{
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL ) return;
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	
	if ( IDM_DEVSTATIS == nID  )
	{
		DevStatisDlg dlg(str_ip.GetBuffer(0));
		dlg.DoModal();
		CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	}
	else if ( IDM_DEVSET == nID )
	{
		DevSetDlg dlg(m_ado,str_ip.GetBuffer(0));
		dlg.DoModal();
		CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	}
	else if ( IDM_NETWORKSET == nID )
	{
		CWebServiceMrg dlg(str_ip.GetBuffer(0));
		dlg.DoModal();
		CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	}
	else if ( IDR_MENU_SYN_TIME == nID )
	{
		DEV_DATETIME dev_time;
		sftime st;
		dev_time.m_Date.m_Year = st.year();
		dev_time.m_Date.m_Month = st.month();
		dev_time.m_Date.m_Day = st.day();
		
		dev_time.m_Time.m_Hour = st.hour();
		dev_time.m_Time.m_Minute = st.minute();
		dev_time.m_Time.m_Second = st.second();
		dev_time.m_Time.m_Msec = st.millis();
		
		CPM_ULDevTime( str_ip.GetBuffer(0), dev_time );
	}
	else if ( IDR_TIMEGROUP == nID )
	{
		CUDDlg dlg(m_ado, str_ip.GetBuffer(0), OPER_TIMEGROUP);
		dlg.DoModal();
		CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	}
	else if ( IDR_RIGHT == nID )
	{
		CUDDlg dlg(m_ado, str_ip.GetBuffer(0), OPER_RIGHT);
		dlg.DoModal();
		CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	}
	else if ( IDR_DEVCUT == nID )
	{
		CPM_DCNDev(str_ip.GetBuffer(0));
		m_dev.DeleteItem(nIndex);
	}
	else if ( IDR_DEV_FORMAT == nID )
	{
		BOOL ret = CPM_ULFormat( str_ip.GetBuffer(0) );
	}
	else if ( IDR_DEVAUTH == nID )
	{
		AuthDlg dlg(str_ip.GetBuffer(0));
		dlg.DoModal();
	}
	else if (IDR_UGROUP == nID)
	{
		CUDDlg dlg(m_ado, str_ip.GetBuffer(0), OPER_USERGROUP);
		dlg.DoModal();
		CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	}else if( IDR_DMYPWD == nID )
	{
		CCreateDynamicDlg dlg( str_ip.GetBuffer(0),this);
		dlg.DoModal();
		CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	}
}

//上传数据
void CTestDlg::OnButtonUp() 
{
	if( ((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck() )
	{
		UpUserEx();
		return;
	}
	
	POSITION pos = m_user.GetFirstSelectedItemPosition(); 
	CString str_id;
	int nIndex = 0;
	while ( pos )
	{
		nIndex = m_user.GetNextSelectedItem(pos); 
		str_id = str_id + _T("\'") + m_user.GetItemText(nIndex, 0) + _T("\',");
	}
	str_id = str_id.Left( str_id.GetLength()-1 );
	CString str_sql;
	str_sql.Format( _T("select * from [user] where [UserID] in (%s) order by [UserID]"), str_id );
	CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL( str_sql );
	if ( ptr->IsEmpty() ) return;
	uutime t_t;
	uutime t_test;
	POSITION pos1 = m_dev.GetFirstSelectedItemPosition(); 
	CString str_ip;
	
	while ( pos1 )
	{
		nIndex = m_dev.GetNextSelectedItem( pos1 ); 
		str_ip = m_dev.GetItemText(nIndex, 0);
		
		ptr->MoveFirst();
		for (; !(ptr->IsEOF()) ; ptr->MoveNext())
		{
			DEV_USER user = {0};
			memcpy( user.m_UserID.m_ID, ptr->GetCString( _T("UserID") ).GetBuffer(0), ptr->GetCString( _T("UserID") ).GetLength() );         
			memcpy( user.m_AppendID.m_ID, ptr->GetCString( _T("AppendID") ).GetBuffer(0), ptr->GetCString( _T("AppendID") ).GetLength() );     
			memcpy( user.m_ImageSID.m_ID, ptr->GetCString( _T("ChangeID") ).GetBuffer(0), ptr->GetCString( _T("ChangeID") ).GetLength() );
			memcpy( user.m_UserName.m_ID, ptr->GetCString( _T("UserName") ).GetBuffer(0), ptr->GetCString( _T("UserName") ).GetLength() );       
			memcpy( user.m_Department.m_ID, ptr->GetCString( _T("Department") ).GetBuffer(0), ptr->GetCString( _T("Department") ).GetLength() );      
			memcpy( user.m_Admin.m_ID, ptr->GetCString( _T("RegAdmin") ).GetBuffer(0), ptr->GetCString( _T("RegAdmin") ).GetLength() );             			       
			user.m_RightID = ptr->GetLong( _T("RightID") ); 
			user.m_UserFlag = ptr->GetLong( _T("UserFlag") ); 
			user.m_UserType = DEV_USER_TYPE(ptr->GetLong(_T("UserType")));
			
			COleDateTime time;
			if ( time.ParseDateTime( ptr->GetCString( _T("InvalidTime") ) ) )
			{
				user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
				user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
				user.m_DeadLineTime.m_Date.m_Day = time.GetDay();		
				user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
				user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
				user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
			}
			
			if ( time.ParseDateTime( ptr->GetCString( _T("RegistTime") ) ) )
			{
				user.m_RegistTime.m_Date.m_Year = time.GetYear();
				user.m_RegistTime.m_Date.m_Month = time.GetMonth();
				user.m_RegistTime.m_Date.m_Day = time.GetDay();		
				user.m_RegistTime.m_Time.m_Hour = time.GetHour();
				user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
				user.m_RegistTime.m_Time.m_Second = time.GetSecond();
			}
			
			if ( time.ParseDateTime( ptr->GetCString( _T("LastUpdTime") ) ) )
			{
				user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
				user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
				user.m_LastUpdTime.m_Date.m_Day = time.GetDay();		
				user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
				user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
				user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
			}
			
			if ( time.ParseDateTime( ptr->GetCString( _T("TypeUpdTime") ) ) )
			{
				user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
				user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
				user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();		
				user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
				user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
				user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
			}
			
			char buf[130*1024] = {0};
			ptr->GetChunk( _T("Block"), (BYTE*)buf, ptr->GetLong(_T("FeatBytes"))+ptr->GetLong(_T("PicBytes") ) );
			
			user.m_FeatLen = ptr->GetLong(_T("FeatBytes"));
			user.m_PicLen = ptr->GetLong(_T("PicBytes") );
			user.m_FeatData = buf;
			user.m_PicData = buf + user.m_FeatLen;
			t_test.restart();
			if ( !CPM_ULUser( str_ip.GetBuffer(0), user) )
			{			
				DEBUG_INFO1( _T("%s 数据上传失败"), user.m_UserID.m_ID );
			}
			else
			{
				double dd = t_test.elapsed();		
				///		DEBUG_INFO2( _T("%s 数据上传成功 %0.2f"), user.m_UserID.m_ID, dd );
				
			}
			//		WaitForSingleObject(m_hEvent, 60*1000);
			///	Sleep(1000);
		}
		DEBUG_INFO2( _T("%s %0.2f"), _T("数据上传完成"), t_t.elapsed() );
		
	}
}

#define USER_UP_PIC_BIN_MAX 10
#define USER_UP_FEAT_BIN_MAX 20
#define USER_UP_TEXT_MAX 2000
void CTestDlg::UpUserEx()
{
	UpdateData( TRUE );
	int nIndex = 0;
	
	POSITION pos1 = m_dev.GetFirstSelectedItemPosition(); 
	CString str_ip;
	if ( pos1 == NULL ) return ;
	nIndex = m_dev.GetNextSelectedItem( pos1 ); 
	str_ip = m_dev.GetItemText(nIndex, 0);
	
	POSITION pos = m_user.GetFirstSelectedItemPosition(); 
	CString str_id;
	
	while ( pos )
	{
		nIndex = m_user.GetNextSelectedItem(pos); 
		str_id = str_id + _T("\'") + m_user.GetItemText(nIndex, 0) + _T("\',");
	}
	str_id = str_id.Left( str_id.GetLength()-1 );
	CString str_sql;
	str_sql.Format( _T("select * from [user] where [UserID] in (%s) order by [UserID]"), str_id );
	CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL( str_sql );
	if ( ptr->IsEmpty() ) return;
	
	int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_PIC))->GetCheck();
	int feat_select = ((CButton*)GetDlgItem(IDC_CHECK_FEAT))->GetCheck();
	int text_select =   ((CButton*)GetDlgItem(IDC_CHECK_TEXT))->GetCheck();
	
	int flag[3] = {0};
	if ( text_select )
		flag[0] = DL_DEV_USER_TEXT;
	if ( feat_select )
		flag[1] = DL_DEV_USER_FEAT;
	if ( pic_select )
		flag[2] = DL_DEV_PIC;
	
	// 	int MAX =  (flag == DL_DEV_USER_TEXT) ? USER_UP_TEXT_MAX : (flag == DL_DEV_USER_FEAT) ? USER_UP_FEAT_BIN_MAX : USER_UP_PIC_BIN_MAX;
	
	DEV_BATCH_USER users;
	users.m_nCount = m_user.GetSelectedCount();
	users.m_pUserArray = new DEV_USER[ users.m_nCount+1 ];
	DEV_USER* pCurUser = users.m_pUserArray;
	for ( int j = 0; j < users.m_nCount+1; ++j )
	{
		pCurUser->m_FeatData = NULL;
		pCurUser->m_PicData = NULL;
		++pCurUser;	
	}
	
	pCurUser = users.m_pUserArray;
	for ( int i = 0; /*(flag == DL_DEV_USER_FEAT || flag == DL_DEV_PIC ) && */i < USER_UP_FEAT_BIN_MAX && i < users.m_nCount; ++i )
	{
		pCurUser->m_FeatData = new char[512*1024];
		++pCurUser;
	}
	
	users.m_nCount = 0;
	
	int MAX =  USER_UP_TEXT_MAX;
	if ( flag[1] == DL_DEV_USER_FEAT || flag[2] == DL_DEV_PIC ) MAX = USER_UP_PIC_BIN_MAX;
	
	uutime total_time ;
	
	for ( /*int*/ i = 0; i < 3; ++i )
	{
		if ( flag[i] == 0 ) continue;
		
		DEBUG_INFO1( _T("开始上传%s"), (flag[i] == DL_DEV_USER_TEXT) ? _T("用户文字信息") : (flag[i] == DL_DEV_USER_FEAT) ? _T("用户特征信息") : _T("用户注册照信息") );
		
		
		int tmp_flag = flag[i];
		if ( flag[i] == DL_DEV_USER_TEXT )
		{
			tmp_flag = DL_DEV_USER_TEXT;
			tmp_flag = tmp_flag | ((flag[1] == DL_DEV_USER_FEAT) ? DL_DEV_USER_FEAT : 0);
			tmp_flag = tmp_flag | ((flag[2] == DL_DEV_PIC) ? DL_DEV_PIC : 0);
		}
		
		pCurUser = users.m_pUserArray;
		users.m_nCount = 0;
		uutime t_t;
		for (ptr->MoveFirst(); !(ptr->IsEOF()) ; ptr->MoveNext())
		{		
			DEV_USER& user = *pCurUser;
			char* buf = user.m_FeatData;
			memset( pCurUser, 0x0, sizeof(DEV_USER) );
			user.m_FeatData = buf;
			memcpy( user.m_UserID.m_ID, ptr->GetCString( _T("UserID") ).GetBuffer(0), ptr->GetCString( _T("UserID") ).GetLength() );         
			memcpy( user.m_AppendID.m_ID, ptr->GetCString( _T("AppendID") ).GetBuffer(0), ptr->GetCString( _T("AppendID") ).GetLength() );     
			memcpy( user.m_ImageSID.m_ID, ptr->GetCString( _T("ChangeID") ).GetBuffer(0), ptr->GetCString( _T("ChangeID") ).GetLength() );
			memcpy( user.m_UserName.m_ID, ptr->GetCString( _T("UserName") ).GetBuffer(0), ptr->GetCString( _T("UserName") ).GetLength() );       
			memcpy( user.m_Department.m_ID, ptr->GetCString( _T("Department") ).GetBuffer(0), ptr->GetCString( _T("Department") ).GetLength() );      
			memcpy( user.m_Admin.m_ID, ptr->GetCString( _T("RegAdmin") ).GetBuffer(0), ptr->GetCString( _T("RegAdmin") ).GetLength() );             			       
			user.m_RightID = ptr->GetLong( _T("RightID") ); 
			user.m_UserFlag = ptr->GetLong( _T("UserFlag") ); 
			user.m_UserType = DEV_USER_TYPE(ptr->GetLong(_T("UserType")));
			
			COleDateTime time;
			if ( time.ParseDateTime( ptr->GetCString( _T("InvalidTime") ) ) )
			{
				user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
				user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
				user.m_DeadLineTime.m_Date.m_Day = time.GetDay();		
				user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
				user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
				user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
			}
			
			if ( time.ParseDateTime( ptr->GetCString( _T("RegistTime") ) ) )
			{
				user.m_RegistTime.m_Date.m_Year = time.GetYear();
				user.m_RegistTime.m_Date.m_Month = time.GetMonth();
				user.m_RegistTime.m_Date.m_Day = time.GetDay();		
				user.m_RegistTime.m_Time.m_Hour = time.GetHour();
				user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
				user.m_RegistTime.m_Time.m_Second = time.GetSecond();
			}
			
			if ( time.ParseDateTime( ptr->GetCString( _T("LastUpdTime") ) ) )
			{
				user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
				user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
				user.m_LastUpdTime.m_Date.m_Day = time.GetDay();		
				user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
				user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
				user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
			}
			
			if ( time.ParseDateTime( ptr->GetCString( _T("TypeUpdTime") ) ) )
			{
				user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
				user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
				user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();		
				user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
				user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
				user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
			}
			
			
			if ( flag[1] == DL_DEV_USER_FEAT || flag[2] == DL_DEV_PIC  )
			{
				ptr->GetChunk( _T("Block"), (BYTE*)buf, ptr->GetLong(_T("FeatBytes"))+ptr->GetLong(_T("PicBytes") ) );			
				user.m_FeatLen = ptr->GetLong(_T("FeatBytes"));
				user.m_PicLen = ptr->GetLong(_T("PicBytes") );			
				user.m_PicData = buf + user.m_FeatLen;
				ASSERT( (user.m_FeatLen+user.m_PicLen) < 258*1024 );
			}
			
			++users.m_nCount;
			++pCurUser;
			
			if ( users.m_nCount == MAX )
			{
				///DEBUG_INFO1( _T("%s "), _T("数据上传---") );
				if ( ! CPM_ULUserEx( str_ip.GetBuffer(0), users, tmp_flag ) )
				{			
					DEBUG_INFO1( _T("%s "), _T("数据上传失败") );
				}
				
				users.m_nCount = 0;
				pCurUser = users.m_pUserArray;
				WaitForSingleObject(m_hEvent, INFINITE/*10*1000*/);
			}
			
		}	
		SetEvent( m_hEvent );
		if ( users.m_nCount  )
		{	
			DEBUG_INFO1( _T("%s "), _T("数据上传---") );
			if ( ! CPM_ULUserEx( str_ip.GetBuffer(0), users, tmp_flag ) )
			{DEBUG_INFO1( _T("%s "), _T("数据上传失败") );}
			
		}	
		//	WaitForSingleObject(m_hEvent, INFINITE);
		DEBUG_INFO1( _T("数据上传完成%0.2f"),t_t.elapsed() );
		
		if ( flag[i] == DL_DEV_USER_TEXT ) break;
	}
	//	return;
	DEBUG_INFO2( _T("%s%0.2f"), _T("数据上传完成---"), total_time.elapsed() );
	pCurUser = users.m_pUserArray;
	for ( int m = 0; /*(flag == DL_DEV_USER_FEAT || flag == DL_DEV_PIC ) &&*/ m < USER_UP_FEAT_BIN_MAX && m < users.m_nCount ; ++m )
	{
		delete [] pCurUser->m_FeatData ;
		pCurUser->m_FeatData = NULL;
		++pCurUser;
	}
	delete [] users.m_pUserArray;
}

//下载数据
void CTestDlg::OnButtonDown() 
{
	if( ((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck() )
	{
		DownUserEx();
		return;
	}
	CString str_ip;
	if ( m_bNet )
	{
		POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
		if ( pos == NULL )
		{
			AfxMessageBox( _T("请选择一个设备") );
			return;
		}
		int nIndex = m_dev.GetNextSelectedItem(pos); 
		str_ip = m_dev.GetItemText(nIndex, 0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
		if ( str_ip.IsEmpty() )
		{
			AfxMessageBox( _T("请选择一个U盘数据库") );
			return;
		}
	}
	
	
	int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_PIC))->GetCheck();
	int feat_select = ((CButton*)GetDlgItem(IDC_CHECK_FEAT))->GetCheck();
	if ( 1 != pic_select && 1 != feat_select )
	{
		AfxMessageBox( _T("请选择需下载的数据") );
		return;
	}
	int flag = 0x00;
	flag |= pic_select ? DL_DEV_PIC : 0x00;
	flag |= feat_select ? DL_DEV_USER_FEAT : 0x00 ;
	
	int Index = m_cmbCondition.GetCurSel();
	BOOL ret = FALSE;
	
	//	m_hTestEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	switch (Index)
	{
	case 0:
		ret = CPM_DLAllUser( str_ip.GetBuffer(0), 11 );
		break;
	default:
		break;
	}
	
	if ( !ret )	
	{	DEBUG_INFO1( _T("%s"), _T("用户数据请求下载发送失败") );}
	else 
    {	DEBUG_INFO1( _T("%s"), _T("用户数据请求下载发送成功") );}
}

void CTestDlg::DownUserEx()
{
	CString str_ip;
	{
		POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
		if ( pos == NULL )
		{
			AfxMessageBox( _T("请选择一个设备") );
			return;
		}
		int nIndex = m_dev.GetNextSelectedItem(pos); 
		str_ip = m_dev.GetItemText(nIndex, 0);
	}
	
	
	
	int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_PIC))->GetCheck();
	int feat_select = ((CButton*)GetDlgItem(IDC_CHECK_FEAT))->GetCheck();
	int text_select =   ((CButton*)GetDlgItem(IDC_CHECK_TEXT))->GetCheck();
	
	
	if ( 1 != pic_select && 1 != feat_select && 1 != text_select)
	{
		AfxMessageBox( _T("请选择需下载的数据") );
		return;
	}
	int flag = 0x00;
	if ( text_select )
	{
		flag = DL_DEV_USER_TEXT;
		if ( feat_select )
			flag |= DL_DEV_USER_FEAT;
		if ( pic_select )
			flag |= DL_DEV_PIC;
	}
	else
	{
		if ( feat_select )
			flag = DL_DEV_USER_FEAT;
		else if ( pic_select )
			flag = DL_DEV_PIC;
	}
	
	BOOL ret = FALSE;
	
	
	int nCount = m_user.GetSelectedCount();
	DEV_CID* pIDArray = new DEV_CID[nCount+1];
	DEV_CID* pTmp = pIDArray; 
	
	{
		POSITION pos = m_user.GetFirstSelectedItemPosition(); 
		int nIndex = 0;
		while ( pos )
		{
			memset( pTmp, 0x0, sizeof(DEV_CID) );
			nIndex = m_user.GetNextSelectedItem(pos); 
			strcpy( pTmp->m_ID, m_user.GetItemText(nIndex, 0) );
			++pTmp;
		}
	}
	uutime total;
	ret = CPM_DLUserEx( str_ip.GetBuffer(0), pIDArray, nCount, flag );
	if ( !ret )	
	{	
        DEBUG_INFO1( _T("%s"), _T("用户数据请求下载发送失败") );
    }
	else 
    {
		WaitForSingleObject( m_hEvent , INFINITE );
		DEBUG_INFO2( _T("%s -- %0.2f"), _T("用户数据下载完成"),  total.elapsed() );
	}
	delete [] pIDArray;
}


//增加用户
void CTestDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	AddUserDlg dlg(m_ado);
	if ( IDOK == dlg.DoModal() )
	{
		char values[5] ={0};
		int items = m_user.GetItemCount();
		int n = m_user.InsertItem( items, dlg.m_ID );
		
		m_user.SetItemText( n, 0, dlg.m_ID );
		m_user.SetItemText( n, 1, dlg.m_Card );
		m_user.SetItemText( n, 2, dlg.m_Name );
		
		int R_ID = dlg.m_Right;
		sprintf( values, "%d", R_ID );
		m_user.SetItemText( n, 3, values );
		
		sprintf( values, "%d", dlg.m_UserType );	
		m_user.SetItemText( n, 4, values );
		sprintf( values, "%d", dlg.m_userFlag);
		m_user.SetItemText( n, 5, values );
		
		m_user.SetItemText( n, 6, dlg.m_invalideTime );
		
		DEV_CID imageID = {0};
		
		//>>Liush =================
		//		sftime st;
		//		sprintf( imageID.m_ID, _T("%d%d%d%d%d%d%d"), st.year(), st.month(),st.day(),st.hour(),st.minute(),st.second(),st.millis());
		//<<========================
		CString sql ;
		sql.Format(_T("insert into [user]([UserID], [AppendID], [UserName], [Department], [RightID], [UserType], [UserFlag], [InvalidTime], [ChangeID]) \
			values (\'%s\', \'%s\', \'%s\', \'%s\', %d, %d, %d, \'%s\', \'%s\')"), 
			dlg.m_ID, dlg.m_Card, dlg.m_Name, dlg.m_Dep, R_ID, dlg.m_UserType, dlg.m_userFlag , dlg.m_invalideTime, imageID.m_ID);
		m_ado.ExecuteNoSelectSQL(sql);
		
		UpdateData( FALSE );
	}
}
//删除用户
void CTestDlg::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	if( ((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck() )
	{
		DelUserEx();
		return;
	}
	
	int local_select = ((CButton*)GetDlgItem(IDC_CHECK_LOCAL))->GetCheck();
	int dsp_select = ((CButton*)GetDlgItem(IDC_CHECK_DSP))->GetCheck();
	if ( local_select != 1 && dsp_select != 1 ) return;
	POSITION pos = m_user.GetFirstSelectedItemPosition();
	if ( pos == NULL ) return;
	
	int i = 0 ;
	while ( pos )
	{
		int nIndex = m_user.GetNextSelectedItem(pos);
		CString str_id = m_user.GetItemText(nIndex, 0);
		
		if ( local_select )
		{
			CString str_sql;
			str_sql.Format( _T("delete from [user] where ( [UserID] = \'%s\')"), str_id.GetBuffer(0) );
			m_ado.ExecuteNoSelectSQL(str_sql); 
			m_user.DeleteItem(nIndex);
			int items = m_user.GetItemCount();
			if ( items > 0 )
			{
				m_dev.SetItemState( items-1, LVIS_SELECTED, LVIS_SELECTED );
				ShowImage(items-1);
			}
			pos = m_user.GetFirstSelectedItemPosition(); 
		}
		if ( dsp_select )
		{
			CString str_ip;
			if ( m_bNet )
			{
				POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
				if ( pos == NULL ) continue;
				int nIndex = m_dev.GetNextSelectedItem(pos); 
				str_ip = m_dev.GetItemText(nIndex, 0);
			}
			else
			{
				GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
				if ( str_ip.IsEmpty() )
				{
					continue;
				}
			}
			DEV_CID user_id = {0};
			memcpy( user_id.m_ID, str_id.GetBuffer(0), str_id.GetLength() );
			if ( CPM_DELUser( str_ip.GetBuffer(0), user_id) )
			{
				DEBUG_INFO1( _T("%s 删除请求发送成功"), str_id.GetBuffer(0) );
			}
			else
			{
				DEBUG_INFO1( _T("%s 删除请求发送失败"), str_id.GetBuffer(0) );
			}
		}
		++i;
	}
	DEBUG_INFO1( _T("删除请求发送完成 %d"), i );
}

void CTestDlg::DelUserEx() 
{
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL ) return;
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	
	int nCount = m_user.GetSelectedCount();
	DEV_CID* pIDArray = new DEV_CID[nCount+1];
	DEV_CID* pTmp = pIDArray; 
	
	{
		POSITION pos = m_user.GetFirstSelectedItemPosition(); 
		int nIndex = 0;
		while ( pos )
		{
			memset( pTmp, 0x0, sizeof(DEV_CID) );
			nIndex = m_user.GetNextSelectedItem(pos); 
			strcpy( pTmp->m_ID, m_user.GetItemText(nIndex, 0) );
			++pTmp;
		}
	}
    int res = CPM_DELUserEx( str_ip.GetBuffer(0), pIDArray, nCount);
	DEBUG_INFO1( _T("删除请求发送完成 %d"), res );
	delete [] pIDArray;
}


void CTestDlg::OnLClickUserList( NMHDR * pNotifyStruct, LRESULT* result  )
{
	*result = CDRF_DODEFAULT;
	NMCLICK* pclick = (NMCLICK*)pNotifyStruct;
	if ( NULL == pclick || (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) > m_user.GetItemCount() ) return;
	int nIndex = pclick->dwItemSpec; 
	ShowImage(nIndex);
}

void CTestDlg::OnLClickRecordList( NMHDR * pNotifyStruct, LRESULT* result )
{
	*result = CDRF_DODEFAULT;
	NMCLICK* pclick = (NMCLICK*)pNotifyStruct;
	if ( NULL == pclick || (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) > m_record.GetItemCount() ) return;
	int nIndex = pclick->dwItemSpec; 
	CString str_id = m_record.GetItemText(nIndex, 0);
	ShowRecordImage(nIndex, str_id);
}

void CTestDlg::OnLClickCImageList( NMHDR * pNotifyStruct, LRESULT* result )
{
	*result = CDRF_DODEFAULT;
	NMCLICK* pclick = (NMCLICK*)pNotifyStruct;
	if ( NULL == pclick || (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) > m_image.GetItemCount() ) return;
	int nIndex = pclick->dwItemSpec; 
	CString str_id = m_image.GetItemText(nIndex, 0);
	ShowRecordImage(nIndex, str_id);
}



void CTestDlg::ShowImage( int index )
{
	CString str_id = m_user.GetItemText(index, 0);
	
	CString str_sql;
	str_sql.Format( _T("select [FeatBytes], [PicBytes], [Block] from [user] where ([UserId] = \'%s\')"), str_id );
	CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL( str_sql );
	if ( ptr->IsEmpty() ) return;
	int FeatBytes = ptr->GetLong( _T("FeatBytes") );
	int PicBytes = ptr->GetLong( _T("PicBytes") );
	
	unsigned int len = FeatBytes+PicBytes;
	if ( PicBytes <= 0 ) return;
	char* buf = new char[FeatBytes+PicBytes];
	memset(buf, 0x0, FeatBytes+PicBytes );
	ptr->GetChunk( _T("Block"), (BYTE*)buf, len );
	
	
	CDC *pDC = m_static.GetDC(); 
	RECT rect; 
	m_static.GetClientRect(&rect);
	CPicture pic;
	pic.LoadPictureData( (BYTE*)(buf+FeatBytes), PicBytes );
	pic.UpdateSizeOnDC(pDC);
	pic.Show( pDC, rect );
	pic.FreePictureData();
	
	delete [] buf;
}

void CTestDlg::ShowRecordImage( int index, const CString& str_id )
{
	
	CString str_sql;
	str_sql.Format( _T("select [PicBytes], [Block] from [record] where ([SerialID] = \'%s\')"), str_id );
	CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL( str_sql );
	if ( ptr->IsEmpty() ) return;
	unsigned int len = ptr->GetLong( _T("PicBytes") );
	if ( len <= 0 ) return;
	
	char* buf = new char[len];
	memset(buf, 0x0, len);
	ptr->GetChunk( _T("Block"), (BYTE*)buf, len );
	
	
	
	CDC *pDC = m_staticRecord.GetDC(); 
	RECT rect; 
	m_staticRecord.GetClientRect(&rect);
	CPicture pic;
	pic.LoadPictureData( (BYTE*)(buf), len );
	pic.UpdateSizeOnDC(pDC);
	pic.Show( pDC, rect );
	pic.FreePictureData();
	
	delete [] buf;
}

void CTestDlg::OnTimer( UINT nIDEvent )
{
	if ( nIDEvent == SERCH_EVENT_TIME )
	{
		//	BOOL ret = CPM_StartDevSerch(TRUE, m_IP);//SERCH_DEVICE(true);
		//	DEBUG_INFO1( _T("搜索服务器 %d"), ret );
	}

	if ( nIDEvent == CLEARLCLICKTIME )
		m_nLClickTimes = 0;
}


void CTestDlg::OnButtonPic() 
{
	// TODO: Add your control notification handler code here
	CString str_ip;
	POSITION pos;
	int nIndex;
	if (m_bNet)
	{
		pos = m_dev.GetFirstSelectedItemPosition(); 
		if ( pos == NULL )
		{
			AfxMessageBox( _T("请选择一个设备") );
			return;
		}
		nIndex = m_dev.GetNextSelectedItem(pos); 
		str_ip = m_dev.GetItemText(nIndex, 0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
		if ( str_ip.IsEmpty() )
		{
			AfxMessageBox( _T("请选择一个U盘数据库") );
			return;
		}
	}
	
	pos = m_user.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一条记录") );
		return;
	}
	nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_id = m_user.GetItemText(nIndex, 0);
	
	CFileDialog dlg(TRUE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "jpg Files (*.jpg)|*.jpg||" );
	if ( dlg.DoModal() != IDOK ) return;
	
	
	CString str_sql;
	str_sql.Format( _T("select * from [user] where ([UserId] = \'%s\')"), str_id );
	CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL( str_sql );
	if ( ptr->IsEmpty() ) return;
	
	DEV_USER user = {0};
	memcpy( user.m_UserID.m_ID, ptr->GetCString( _T("UserID") ).GetBuffer(0), ptr->GetCString( _T("UserID") ).GetLength() );         
	memcpy( user.m_AppendID.m_ID, ptr->GetCString( _T("AppendID") ).GetBuffer(0), ptr->GetCString( _T("AppendID") ).GetLength() ); 
	
	//>>Liush 2011-04-03管理端上传用户，带上了注册照， 
	//	1.如果是自己生成的注册照，必须使用此图片生成的ImageSID---ChangeID
	//  2.如果此照片是从DSP端down下来，
	//  其实也就相当于每一张注册照都必须对应唯一的SID，如果是管理端生成注册照，则管理端生成SID
	//  DSP端生成的注册照，则由DSP端生成SID
	memcpy( user.m_ImageSID.m_ID, ptr->GetCString( _T("ChangeID") ).GetBuffer(0), ptr->GetCString( _T("ChangeID") ).GetLength() );   
	memcpy( user.m_UserName.m_ID, ptr->GetCString( _T("UserName") ).GetBuffer(0), ptr->GetCString( _T("UserName") ).GetLength() );       
	memcpy( user.m_Department.m_ID, ptr->GetCString( _T("Department") ).GetBuffer(0), ptr->GetCString( _T("Department") ).GetLength() );      
	memcpy( user.m_Admin.m_ID, ptr->GetCString( _T("RegAdmin") ).GetBuffer(0), ptr->GetCString( _T("RegAdmin") ).GetLength() );           
	
	user.m_UserType = DEV_USER_TYPE(ptr->GetLong(_T("UserType"))); 
	user.m_RightID = ptr->GetLong(_T("RightID"));  
	
	user.m_UserFlag |= ptr->GetLong( _T("UserFlag") ); 
	
	
	COleDateTime time;
	time.ParseDateTime( ptr->GetCString( _T("InvalidTime") ) );
	user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
	user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
	user.m_DeadLineTime.m_Date.m_Day = time.GetDay();		
	user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
	user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
	user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
	
	time.ParseDateTime( ptr->GetCString( _T("RegistTime") ) );
	user.m_RegistTime.m_Date.m_Year = time.GetYear();
	user.m_RegistTime.m_Date.m_Month = time.GetMonth();
	user.m_RegistTime.m_Date.m_Day = time.GetDay();		
	user.m_RegistTime.m_Time.m_Hour = time.GetHour();
	user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
	user.m_RegistTime.m_Time.m_Second = time.GetSecond();
	
	time.ParseDateTime( ptr->GetCString( _T("LastUpdTime") ) );
	user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
	user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
	user.m_LastUpdTime.m_Date.m_Day = time.GetDay();		
	user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
	user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
	user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
	
	time.ParseDateTime( ptr->GetCString( _T("TypeUpdTime") ) );
	user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
	user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
	user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();		
	user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
	user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
	user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
	
	char block[192*1024] = {0};
	ptr->GetChunk( _T("Block"), (BYTE*)block, ptr->GetLong(_T("FeatBytes"))+ptr->GetLong( _T("PicBytes") ) );
	
	
	int PicBytes = 0;
	
	CString str_path = dlg.GetPathName();
	FILE* pfile = fopen( str_path.GetBuffer(0), "rb" );
	if ( pfile )
	{
		fseek(pfile, 0, SEEK_END );
		PicBytes = ftell(pfile);
		if ( PicBytes > 50*1024 ) 
		{
			fclose(pfile);
			return ;
		}
		fseek(pfile, 0, SEEK_SET );
		fread( block+ptr->GetLong(_T("FeatBytes")), 1, PicBytes, pfile);		
		fclose(pfile);
		ptr->SetLong( _T("PicBytes") , PicBytes );
		//>>Liush 用户自己上传注册照时，修改在源数据表中的ChangeID字段以及，将要被发送出去的user.m_ImageSID.m_ID 内容		
		sftime st;
		sprintf( user.m_ImageSID.m_ID, _T("%d%d%d%d%d%d%d"), st.year(), st.month(),st.day(),st.hour(),st.minute(),st.second(),st.millis());
		CString newChangeID = user.m_ImageSID.m_ID;
		ptr->SetCString( _T("ChangeID") , newChangeID);
		ptr->AppendChunk( _T("Block"), (BYTE*)block, ptr->GetLong(_T("FeatBytes"))+PicBytes );
		ptr->Update();
	}	
	
	user.m_FeatLen = ptr->GetLong(_T("FeatBytes"));
	user.m_PicLen = ptr->GetLong(_T("PicBytes") );
	user.m_FeatData = block;
	user.m_PicData = block + user.m_FeatLen;
	
	if ( !CPM_ULUser( str_ip.GetBuffer(0), user) )
	{DEBUG_INFO1( _T("%s 数据上传失败"), str_id.GetBuffer(0) );}
	else
	{DEBUG_INFO1( _T("%s 数据上传成功"), str_id.GetBuffer(0) );}
}	

void CTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
	
#define FEATURE_LEN 4330
	typedef struct
	{
		char          m_Feature[ FEATURE_LEN ];
		DEV_CID           m_PID;
		unsigned char m_Flag;
	}SPFeatInfo;  
	
	
	
	CString str_sql = _T("select [UserName],[RegistTime], [RegAdmin],[FeatBytes],[PicBytes],[Block] from [user] where ([UserId] = \'3005\')") ;
	CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL( str_sql );
	if ( ptr->IsEmpty() ) return;
	unsigned int FeatBytes = ptr->GetLong( _T("FeatBytes") );
	unsigned int PicBytes = ptr->GetLong( _T("PicBytes") );
	CString str_name = ptr->GetCString( _T("UserName") );
	CString str_RegTime = ptr->GetCString( _T("RegistTime") );
	CString str_RegAdmin = ptr->GetCString( _T("RegAdmin") );
	
	char* buf = new char[FeatBytes+PicBytes+1024];
	memset(buf, 0x0, FeatBytes+PicBytes+1024 );
	ptr->GetChunk( _T("Block"), (BYTE*)buf, FeatBytes+PicBytes );
	str_sql = _T("select [UserId], [UserName],[RegistTime],[FeatBytes],[RegAdmin],[PicBytes],[Block] from [user] where ([UserId] >= \'7001\')") ;
	ptr = m_ado.ExecuteSelectSQL( str_sql );
	if ( ptr->IsEmpty() ) return;
	ptr->MoveFirst();
	CString str_id;
	for ( ; !(ptr->IsEOF()); ptr->MoveNext() )
	{
		str_id =  ptr->GetCString( _T("UserId") );
		ptr->SetLong( _T("FeatBytes"), FeatBytes );
		ptr->SetLong( _T("PicBytes"), PicBytes );
		ptr->SetCString( _T("RegistTime"), str_RegTime ); 
		ptr->SetCString( _T("RegAdmin"), str_RegAdmin ); 
		SPFeatInfo* p = (SPFeatInfo*)buf;
		for ( int i = 0 ; i < 20; ++i )
		{
			memset( &(p->m_PID), 0x0, sizeof(DEV_CID) );
			sprintf( p->m_PID.m_ID, "%s", str_id );
			p++;
		}
		ptr->AppendChunk( _T("Block"), (BYTE*)buf, FeatBytes+PicBytes);
		ptr->Update();
	}
	DEBUG_INFO1( _T("%s"), _T("更改完成") );
	
}

void CTestDlg::OnButtonDlrecord() 
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	CString str_ip;
	if ( m_bNet )
	{
		pos = m_dev.GetFirstSelectedItemPosition(); 
		if ( pos == NULL )
		{
			AfxMessageBox( _T("请选择一个设备") );
			return;
		}
		int n = m_dev.GetNextSelectedItem(pos); 
		str_ip = m_dev.GetItemText(n, 0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
		if ( str_ip.IsEmpty() )
		{
			AfxMessageBox( _T("请选择一个U盘数据库") );
			return;
		}
	}
	
	
	int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_RECORD_PIC))->GetCheck();	
	int flag = 0x00;
	flag |= pic_select;
	int nIndex = m_cmbRecordCondition.GetCurSel();
	
	
	
	BOOL ret = FALSE;
	switch (nIndex)
	{
	case 0:
		ret = CPM_DLAllIdentifyRecord( str_ip.GetBuffer(0) );
		break;
	case 1:
		//	ret = CPM_DLNewIdentifyRecord( str_ip.GetBuffer(0) );
		break;
	case 2:
		ret = CPM_DLAllAlarmRecord( str_ip.GetBuffer(0)  );
		break;
	case 3:
		{
			UpdateData(TRUE);
			DEV_DATE start = {m_STimeRec.GetYear(), m_STimeRec.GetMonth(), m_STimeRec.GetDay()};
			DEV_DATE end = {m_ETimeRec.GetYear(), m_ETimeRec.GetMonth(), m_ETimeRec.GetDay()};
			///	ret = CPM_DLSegTimeIdentifyRecord( str_ip.GetBuffer(0), start, end);
		}
		break;
	case 4:
		//	ret = CPM_DLNewAlarmRecord( str_ip.GetBuffer(0)  );
		break;
	case 5:
		{
			UpdateData(TRUE);
			DEV_DATE start = {m_STimeRec.GetYear(), m_STimeRec.GetMonth(), m_STimeRec.GetDay()};
			DEV_DATE end = {m_ETimeRec.GetYear(), m_ETimeRec.GetMonth(), m_ETimeRec.GetDay()};
			//		ret = CPM_DLSegTimeAlarmRecord( str_ip.GetBuffer(0), start, end  );
		}
		break;
	case 6:
		{
			UpdateData(TRUE);
			DEV_DL_RECORD_RANGE range = {0};
			CString count ;
			GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
			
			range.m_Count = atoi( count.GetBuffer(0) );
			CString id ;
			GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
			memcpy( range.m_SID.m_ID, id.GetBuffer(0), id.GetLength() );
			
			//ret = CPM_GetRangeIdentifyRecCount( m_serverIP, str_ip.GetBuffer(0), range, flag );
            ret = CPM_DLRangeRec ( str_ip.GetBuffer(0), range, flag );
		}
		break;
	case 7:
		{
			UpdateData(TRUE);
			DEV_DL_RECORD_RANGE range = {0};
			CString count ;
			GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
			
			range.m_Count = atoi( count.GetBuffer(0) );
			CString id ;
			GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
			memcpy( range.m_SID.m_ID, id.GetBuffer(0), id.GetLength() );
			
			//ret = CPM_GetRangeAlarmRecCount( m_serverIP, str_ip.GetBuffer(0), range, flag );
            ret = CPM_DLRangeAlarm( str_ip.GetBuffer(0), range, flag );
		}
		break;
	default:
		break;
	}
	
	if ( !ret )	
	{	DEBUG_INFO1( _T("%s"), _T("记录请求下载发送失败") );}
	else 
	{	DEBUG_INFO1( _T("%s"), _T("记录请求下载发送成功") );}
	
}

void CTestDlg::OnButtonDelrecord() 
{
	// TODO: Add your control notification handler code here
	
	CString str_sql( _T("delete from [record]") );
	m_ado.ExecuteNoSelectSQL(str_sql); 
	
	CTreeCtrl_DeleteAllItems( aa.m_hWnd );
	
	
}

#define USERNAME ("admin")
#define PASSWORD ("201031")
void CTestDlg::DevConnectNotify(const char* dev_addr, DEV_CONNECT_STATUS flag )
{
	if ( !m_bNet ) return;
	char ip[16] = {0};
	BYTE field0 = 0, field1=0, field2=0, field3=0;
	m_IPServer.GetAddress(field0, field1, field2, field3);
	sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
	
	switch ( flag )
	{
	case DEV_CONNECT_NONE:
		{
			CPM_CNDev(dev_addr);
		}
		break;
	case DEV_CONNECT_FAILUE:
		DEBUG_INFO1( _T("%s 连接失败"), dev_addr );
		break;
	case DEV_CONNECT_SUCCESS:
		{
			DEBUG_INFO1( _T("%s 建立连接"), dev_addr );
            DEV_CID user={0};
            DEV_CID pswd={0};
            memcpy(user.m_ID , USERNAME , sizeof(USERNAME));
            memcpy(pswd.m_ID , PASSWORD , sizeof(PASSWORD));
            CPM_ULDevAuth(dev_addr,user,pswd,1);
		}
		break;
	case DEV_CONNECT_CUT:
		{
			CPM_DCNDev(dev_addr);
			int items = m_dev.GetItemCount();
			CString str_ip( dev_addr );
			CString temp ;
			for( int i = 0; i < items; ++i )
			{
				temp = m_dev.GetItemText(i, 0);
				if ( str_ip == temp )
				{
					m_dev.DeleteItem(i);
					break;
				}
			}
			DEBUG_INFO1( _T("%s 断开连接"), dev_addr );
		}
		break;
	default: 
		break;
	}
}

void CTestDlg::DevOperResult(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
	CMDPROCESSOR_MAP::iterator it = m_map.find(eType);
	if ( it != m_map.end() )
	{
		OperResult cmd_process = it->second;
		(this->*cmd_process)(cszDevAddr, eType , eFlag ,  pvContent, nSeq , eResult);
	}
}

void CTestDlg::DevBaseInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	if ( OPER_SUCCESS==eResult && pvContent )
	{
		DEV_BASEINFO* pInfo = new DEV_BASEINFO;
		*pInfo = *((DEV_BASEINFO*)pvContent);
		this->PostMessage(BASE_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pInfo );		
	}
}

LRESULT  CTestDlg::DevBaseMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_BASEINFO* pvContent = (DEV_BASEINFO*)lpparam;
	char cszDevAddr[20] = {0};
	LongToDotIp( cszDevAddr, wparam );
	
	DEV_BASEINFO& dev_info = *((DEV_BASEINFO*)pvContent);
	int items = m_dev.GetItemCount();
	m_dev.InsertItem( items, cszDevAddr );
	m_dev.SetItemText(items, 0, cszDevAddr );
	char values[5] = {0};
	sprintf( values, "%d", dev_info.m_DevType );
	m_dev.SetItemText(items, 1, values );
	sprintf( values, "%d", dev_info.m_LimitUser );
	m_dev.SetItemText(items, 2, values);
	m_dev.SetItemState( items, LVIS_SELECTED, LVIS_SELECTED );
	DEBUG_INFO1( _T("%s 获取设备信息成功"), cszDevAddr );
	
	delete pvContent;
	
	return 0;
}

void CTestDlg::DevUserInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	SetEvent( m_hEvent );    
    if ( OPER_SUCCESS==eResult && pvContent && (DEV_USER_OPER == eType ) )
	{
        if ( DEV_OPER_DELETE == eFlag )
        {
            DEV_USER& dev_user = *((DEV_USER*)pvContent);
            DEBUG_INFO2( _T("eResult == %d  , 用户删除 ID == %s"),eResult,dev_user.m_UserID.m_ID );
        }
		
        if ( DEV_OPER_DOWNLOAD == eFlag )
        {
			DEV_USER& dev_user = *((DEV_USER*)pvContent);
			
			//		DEBUG_INFO2( _T("%s 用户信息下载完成%d"), dev_user.m_UserID.m_ID, dev_user.m_bRTUser );
			//	return ;
			CString sql;
			sql.Format( _T("select * from [user] where ([UserID]=\'%s\')"), dev_user.m_UserID.m_ID );
			CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
			if ( ptr->IsEmpty() ) 
			{
				ptr->NewRecord();
				ptr->SetCString( _T("UserID"), dev_user.m_UserID.m_ID );		
				
				DEV_USER* pUser = new DEV_USER;
				*pUser = *((DEV_USER*)pvContent);
				this->PostMessage(USER_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pUser );	
			}
			if ( !(ptr->IsEmpty()) )
			{
				//#if 0
				ptr->SetCString( _T("AppendID"), dev_user.m_AppendID.m_ID );
				ptr->SetCString( _T("UserName"), dev_user.m_UserName.m_ID );
				ptr->SetCString( _T("Department"), dev_user.m_Department.m_ID );
				ptr->SetLong( _T("RightID"), dev_user.m_RightID  );		
				ptr->SetCString( _T("ChangeID"),dev_user.m_ImageSID.m_ID);
				ptr->SetCString( _T("RegAdmin"), dev_user.m_Admin.m_ID );
				ptr->SetLong( _T("UserType") , dev_user.m_UserType  );
				
				ptr->SetLong( _T("UserFlag"), dev_user.m_UserFlag );
				//#endif
				
				
				ptr->SetLong( _T("FeatBytes"), dev_user.m_FeatLen );
				ptr->SetLong( _T("PicBytes") , dev_user.m_PicLen );	
				
				//#if 0
				char values[100] = {0};
				sprintf( values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user.m_RegistTime.m_Date.m_Month,
					dev_user.m_RegistTime.m_Date.m_Day,
					dev_user.m_RegistTime.m_Date.m_Year,
					dev_user.m_RegistTime.m_Time.m_Hour,
					dev_user.m_RegistTime.m_Time.m_Minute,
					dev_user.m_RegistTime.m_Time.m_Second
					);
				ptr->SetCString( _T("RegistTime"), values );
				sprintf( values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user.m_LastUpdTime.m_Date.m_Month,
					dev_user.m_LastUpdTime.m_Date.m_Day,
					dev_user.m_LastUpdTime.m_Date.m_Year,
					dev_user.m_LastUpdTime.m_Time.m_Hour,
					dev_user.m_LastUpdTime.m_Time.m_Minute,
					dev_user.m_LastUpdTime.m_Time.m_Second
					);
				ptr->SetCString( _T("LastUpdTime"), values );
				sprintf( values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user.m_DeadLineTime.m_Date.m_Month,
					dev_user.m_DeadLineTime.m_Date.m_Day,
					dev_user.m_DeadLineTime.m_Date.m_Year,
					dev_user.m_DeadLineTime.m_Time.m_Hour,
					dev_user.m_DeadLineTime.m_Time.m_Minute,
					dev_user.m_DeadLineTime.m_Time.m_Second
					);
				ptr->SetCString( _T("InvalidTime"), values );
				
				sprintf( values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user.m_TypeUpdTime.m_Date.m_Month,
					dev_user.m_TypeUpdTime.m_Date.m_Day,
					dev_user.m_TypeUpdTime.m_Date.m_Year,
					dev_user.m_TypeUpdTime.m_Time.m_Hour,
					dev_user.m_TypeUpdTime.m_Time.m_Minute,
					dev_user.m_TypeUpdTime.m_Time.m_Second
					);
				ptr->SetCString( _T("TypeUpdTime"), values );
				
				char block[192*1024] = {0};
				memcpy( block, dev_user.m_FeatData, dev_user.m_FeatLen );
				memcpy( block+dev_user.m_FeatLen, dev_user.m_PicData, dev_user.m_PicLen );
				ptr->AppendChunk( _T("Block"), (BYTE*)block, dev_user.m_FeatLen+dev_user.m_PicLen );
				ptr->Update();
			}
        }
	}
}

LRESULT CTestDlg::DevUserMsg( WPARAM wparam, LPARAM lpparam )
{
	
	DEV_USER* pvContent = (DEV_USER*)lpparam;
	char cszDevAddr[20] = {0};
	LongToDotIp( cszDevAddr, wparam );
	
	DEV_USER& dev_user = *((DEV_USER*)pvContent);
	
	
	char values[100] = {0};
	int items = m_user.GetItemCount();
	int n = m_user.InsertItem( items, dev_user.m_UserID.m_ID );
	
	
	m_user.SetItemText( n, 0, dev_user.m_UserID.m_ID );
	m_user.SetItemText( n, 1, dev_user.m_AppendID.m_ID );
	m_user.SetItemText( n, 2, dev_user.m_UserName.m_ID );
	
	int R_ID = dev_user.m_RightID;
	sprintf( values, "%d", R_ID );
	m_user.SetItemText( n, 3, values );
	
	sprintf( values, "%d", dev_user.m_UserType );	
	m_user.SetItemText( n, 4, values );
	sprintf( values, "%d", dev_user.m_UserFlag);
	m_user.SetItemText( n, 5, values );
	
	sprintf( values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user.m_DeadLineTime.m_Date.m_Month,
		dev_user.m_DeadLineTime.m_Date.m_Day,
		dev_user.m_DeadLineTime.m_Date.m_Year,
		dev_user.m_DeadLineTime.m_Time.m_Hour,
		dev_user.m_DeadLineTime.m_Time.m_Minute,
		dev_user.m_DeadLineTime.m_Time.m_Second
		);
	m_user.SetItemText( n, 6, values ); 			
	m_user.SetItemState( n, LVIS_SELECTED, LVIS_SELECTED );	
	
	delete pvContent;
	
	return 0;
}

void CTestDlg::DevUserExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	SetEvent(m_hEvent);
	if ( (OPER_SUCCESS==eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent )
	{
		switch ( eFlag )
		{
		case DEV_OPER_UPLOAD:
			{				
				DEBUG_INFO1( _T("%s "), _T("数据上传完成") );
				DEV_BATCH_USER* pUsers = (DEV_BATCH_USER*)pvContent;
				DEV_USER* pUser = pUsers->m_pUserArray;
				int i = 0;
				for ( i = 0; i < pUsers->m_nCount; ++i )
				{
					DEBUG_INFO1( _T("用户%s上传完成"), pUser->m_UserID.m_ID  );
					
					++pUser;
				}
			}
			break;
		case DEV_OPER_DOWNLOAD:
			{
				DEV_BATCH_USER* pUsers = (DEV_BATCH_USER*)pvContent;
				DEBUG_INFO2( _T("%s%d"), _T("数据下载完成xxx"), pUsers->m_nCount );
				if ( eResult == OPER_BATCH_DL_COMPLETE  ) SetEvent(m_hEvent);
				// 				return;
				
				DEV_USER* pUser = pUsers->m_pUserArray;
				int i = 0;
				for ( i = 0; i < pUsers->m_nCount; ++i )
				{
					DEBUG_INFO3( _T("用户%s, pic=%d, feat=%d下载完成"), pUser->m_UserID.m_ID, pUser->m_PicLen, pUser->m_FeatLen );
					DevUserInfoOper( cszDevAddr, DEV_USER_OPER ,DEV_OPER_DOWNLOAD, pUser, nSeq , OPER_SUCCESS);
					++pUser;
				}
			}
			break;
		case DEV_OPER_DELETE:
			{
				DEV_BATCH_USER* pUsers = (DEV_BATCH_USER*)pvContent;
				DEV_USER* pUser = pUsers->m_pUserArray;
				int i = 0;
				for ( i = 0; i < pUsers->m_nCount; ++i )
				{
					DEBUG_INFO1( _T("用户%s删除完成"), pUser->m_UserID.m_ID  );
					++pUser;
				}
			}
		}
	}
}

void CTestDlg::DevLogExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	if ( (OPER_SUCCESS==eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent )
	{
		DEV_BATCH_LOG* pRecords = (DEV_BATCH_LOG*)pvContent;
		DEV_LOG* pRecord = pRecords->m_pLogArray;
		for ( int i = 0; i < pRecords->m_nCount; ++i )
		{
			DevLogInfoOper( cszDevAddr, eType , eFlag ,pRecord, nSeq, OPER_SUCCESS );
			++pRecord;
		}
	}
}

void CTestDlg::DevRecExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	if ( (OPER_SUCCESS==eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent )
	{
		DEV_BATCH_RECORD* pRecords = (DEV_BATCH_RECORD*)pvContent;
		DEV_RECORD* pRecord = pRecords->m_pRecordArray;
		for ( int i = 0; i < pRecords->m_nCount; ++i )
		{
			DevRecordInfoOper( cszDevAddr, eType , DEV_OPER_DOWNLOAD , pRecord, nSeq, OPER_SUCCESS );
			++pRecord;
		}
	}
}

void CTestDlg::DevCImageExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	if ( (OPER_SUCCESS==eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent )
	{
		DEV_BATCH_CIMAGE* pRecords = (DEV_BATCH_CIMAGE*)pvContent;
		DEV_CHANGEIMAGE* pRecord = pRecords->m_pCImageArray;
		for ( int i = 0; i < pRecords->m_nCount; ++i )
		{
            DevChangeImageOper( cszDevAddr, eType, eFlag, pRecord, nSeq , OPER_SUCCESS );
			++pRecord;
		}
	}
}

LONG count = 0;
void CTestDlg::DevRecordInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
    if ( OPER_SUCCESS==eResult && pvContent && (eType == DEV_RECORD_OPER || eType == DEV_ALARM_OPER ) )
	{
		//	return ;
		sftime time;	
		DEBUG_INFO5( _T("recv Record %02d:%02d:%02d:%02d--%d"), time.hour(), time.minute(), time.second(), time.millis(), InterlockedIncrement( &count ) );
		//	return;
		//	if ( count > 1 ) return;
		DEV_RECORD& dev_rec = *((DEV_RECORD*)pvContent) ;
		
		CString strSerialID  ;	
		strSerialID.Format( "%s", dev_rec.m_SerialID.m_ID );		 			 
		CString tmp_sql;
		tmp_sql.Format( _T("select * from [record] where ([SerialID]=\'%s\')"), strSerialID );
		CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(tmp_sql);
		if ( tmp->IsEmpty() ) 
		{		
			CString sql;
			sql.Format( _T("insert into [record] ([SerialID], [PicBytes])values ( \'%s\', %d)"), strSerialID, dev_rec.m_PicLen);			
			m_ado.ExecuteNoSelectSQL(sql);
			sql.Format( _T("select [Block] from [record] where ([SerialID]=\'%s\')"), strSerialID );
			CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);			
			ptr->AppendChunk( _T("Block"),  (BYTE*)(dev_rec.m_PicData), dev_rec.m_PicLen );
			ptr->Update();	
        }          
		DEV_RECORD* pVal = new DEV_RECORD;
		*pVal = *((DEV_RECORD*)pvContent);
		this->PostMessage(REC_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pVal );	
	}
}

LRESULT CTestDlg::DevRecMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_RECORD* pvContent = (DEV_RECORD*)lpparam;	
	DEV_RECORD& dev_rec = *((DEV_RECORD*)pvContent) ;
	DEBUG_INFO1( _T("%s 记录信息下载完成"), dev_rec.m_SerialID.m_ID );
	
	char* rec_type = NULL;
	COLORREF color = -1;
	CString strSerialID  ;	
	strSerialID.Format( "%s", dev_rec.m_SerialID.m_ID );
	switch ( dev_rec.m_RecType )
	{
	case DEV_VERIFY_SUCC  :     //
		rec_type = "验证成功记录";
		break;
	case DEV_VERIFY_FAIL  :    //
		rec_type = "验证失败记录";
		break;
	case DEV_ADMIN_SUCC:
		rec_type = "管理员验证成功记录";
		break;
	case DEV_EMER_OPEN:
		rec_type = "紧急开门记录";
		break;
	case DEV_RIGHT_OPEN  :     //
		rec_type = "权限开门";
		break;
	case DEV_GROUP_OPEN  :     //
		rec_type = "组合开门";
		break;
	case DEV_BUTTON_OPEN:
		rec_type = "按钮开门";
		break;
	case DEV_ALARM_HD_MANGET_TIMEOUT:     //
		rec_type = "门磁超时";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_MANGET_ILLOPEN:      //
		rec_type = "门磁非法开门";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_OFFLINE :				//
		rec_type = "前端掉线报警";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_BREAK :  //
		rec_type = "防拆报警";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_SHOCK  :   //
		rec_type = "震动报警";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_FPOWR:
		rec_type = "前端供电异常报警";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_UPS_ON:
		rec_type = "UPS备用电池开启";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_UPS_OFF:
		rec_type = "UPS备用电池关闭";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_HD_ASSIST:
		rec_type = "辅助输入触发";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_SF_BACKLIST :   //
		rec_type = "黑名单验证报警";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_SF_ILLCARD :
		rec_type = "无效卡(挂失)";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_SF_ILLTIME:
		rec_type = "非法时间识别";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_SF_DEADLINE :
		rec_type = "失效时间";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_SF_DANGER_OPEN :
		rec_type = "胁迫开门";
		color = RGB(255, 0, 0);
		break;
	case DEV_ALARM_SF_SUPER_OPEN :
		rec_type = "超级密码开门";
		color = RGB(255, 0, 0);
		break;
	default:
		break;
	}
	
	
	TLCONTROLINFO info[] = {
		{TLCT_TEXT, color, ""},
		{TLCT_TEXT, color, ""},
		{TLCT_TEXT, color, ""},
		{TLCT_TEXT, color, ""},
		{TLCT_TEXT, color, ""}
	};	
	
	HTREEITEM fa = TVI_ROOT;
	LPARAM lp = (LPARAM)info;
	int items = ListView_GetItemCount(aa.m_hWnd);		
	CTreeCtrl_InsertItem( aa.m_hWnd, strSerialID, fa, TVI_LAST, lp );
	CTreeList_SetItem( aa.m_hWnd, items, 1, dev_rec.m_UserID.m_ID, lp );
	CTreeList_SetItem( aa.m_hWnd, items, 2, rec_type, lp );
	
	
	CString values;
	values.Format( _T("%d"), dev_rec.m_VerifyMode );
	CTreeList_SetItem( aa.m_hWnd, items, 3, values,lp );
	
	switch ( dev_rec.m_Source )
	{
	case DEV_DOOR_SOURCE :		//门相关来源
		{
			values.Format( _T("门点%d的%02X号设备"), (dev_rec.m_ConcretSource >> 4) & 0x0f , dev_rec.m_ConcretSource & 0x0f );
		}
		break;
	case DEV_SIGNAL_SOURCE:			//信号输入输出相关来源
		{
			values.Format( _T("辅助输入输出端子%d"), dev_rec.m_ConcretSource );
		}
		break;
	case DEV_NO_NUM_DEV_SOURCE:		//无编号设备来源 
		{
			values = _T("无编号设备来源");
		}
		break;
	default:
		break;
	}
	CTreeList_SetItem( aa.m_hWnd, items, 4, values, lp );
	
	delete pvContent;
	return 0;
}


void CTestDlg::DevLogInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	if ( OPER_SUCCESS==eResult && pvContent && DEV_LOG_OPER == eType )
	{
		DEV_LOG* pVal = new DEV_LOG;
		*pVal = *((DEV_LOG*)pvContent);
		this->PostMessage(LOG_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pVal );	
	}
}

LRESULT  CTestDlg::DevLogMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_LOG* pvContent = (DEV_LOG*)lpparam;
	DEV_LOG& log = *((DEV_LOG*)pvContent);
	int items = m_log.GetItemCount();
	int n = m_log.InsertItem( items, log.m_SerialID.m_ID );
	char* rec_type = NULL;
	switch ( log.m_LogType )
	{
	case DEV_LOG_ADDUSER  :     //
		rec_type = "增加用户";
		break;
	case DEV_LOG_DELUSER  :    //
		rec_type = "删除用户";
		break;
	case DEV_LOG_REREGIST:
		rec_type = "重新采集";
		break;
	case DEV_LOG_CAHNGETYPE:
		rec_type = "改变用户类型";
		break;
	case DEV_UDISK_ULUSER  :     //
		rec_type = "U盘上传用户";
		break;
	case DEV_UDISK_DLUSER  :     //
		rec_type = "U盘下载用户";
		break;
	case DEV_UDISK_DLRECORD :  //
		rec_type = "U盘下载记录";
		break;
	case DEV_UDISK_UPDATEAPP  :   //
		rec_type = "U盘更新程序";
		break;
	default:
		break;
	}		
	m_log.SetItemText( n, 1, rec_type );
	
	m_log.SetItemText( n, 2, log.m_Admin.m_ID );
	m_log.SetItemText( n, 3, log.m_BeOptUser.m_ID ); 	
	
	char time[20] = {0};
	sprintf( time, "%02d/%02d/%04d %02d:%02d:%02d",log.m_RecTime.m_Date.m_Month,
		log.m_RecTime.m_Date.m_Day, 
		log.m_RecTime.m_Date.m_Year,
		log.m_RecTime.m_Time.m_Hour,
		log.m_RecTime.m_Time.m_Minute,
		log.m_RecTime.m_Time.m_Second
		);
	
	m_log.SetItemText( n, 4, time );
	delete pvContent;
	return 0;
}

void CTestDlg::DevAuthOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	if ( OPER_SUCCESS == eResult )
	{
		CPM_DLDevBaseInfo(cszDevAddr);
		DEBUG_INFO1( _T("%s"), _T("请求获取设备信息") );
	}
	else
	{
		CPM_DCNDev(cszDevAddr);
		DEBUG_INFO1( _T("%s"), _T("验证失败") );	
	}
}

void CTestDlg::DevAddOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
    CPM_DLDevBaseInfo( cszDevAddr , nSeq );
}

void CTestDlg::DevFormatOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
    DEBUG_INFO2( _T("%s%d") , ("格式化res = "), eResult);
}

void CTestDlg::DevChangeImageOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	// 	DEBUG_INFO1( _T("%s"), _T("DevChangeImageOper 成功"));
	// 	return;
	if ( OPER_SUCCESS == eResult )
	{
		DEV_CHANGEIMAGE* pCImage = (DEV_CHANGEIMAGE*)pvContent;	
		CString strSerialID  ;	
		strSerialID.Format( "%s", pCImage->m_SerialID.m_ID );	
		CString tmp_sql;
		tmp_sql.Format( _T("select * from [record] where ([SerialID]=\'%s\')"), strSerialID );
		CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(tmp_sql);
		if ( tmp->IsEmpty() ) 
		{			
			CString sql;
			sql.Format( _T("insert into [record] ([SerialID], [PicBytes])values ( \'%s\', %d)"), strSerialID, pCImage->m_PicBytes);
			
			m_ado.ExecuteNoSelectSQL(sql);
			sql.Format( _T("select [Block] from [record] where ([SerialID]=\'%s\')"), strSerialID );
			CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
			
			ptr->AppendChunk( _T("Block"),  (BYTE*)(pCImage->m_PicData), pCImage->m_PicBytes );
			ptr->Update();
		}
		
		DEV_CHANGEIMAGE* pVal = new DEV_CHANGEIMAGE;
		*pVal = *((DEV_CHANGEIMAGE*)pvContent);
		this->PostMessage(IMAGE_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pVal );	
	}
}

LRESULT CTestDlg::DevImageMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_CHANGEIMAGE* pCImage = (DEV_CHANGEIMAGE*)lpparam;
	
	//	return 0;
	
	DEBUG_INFO1( _T("接受到变更注册照 %s"), pCImage->m_SerialID.m_ID );
	
	COLORREF color = -1;
	TLCONTROLINFO info[] = {
		{TLCT_TEXT, color, ""},
		{TLCT_TEXT, color, ""},
		{TLCT_TEXT, color, ""}
	};	
	
	HTREEITEM fa = TVI_ROOT;
	LPARAM lp = (LPARAM)info;
	int items = ListView_GetItemCount(bb.m_hWnd);		
	CTreeCtrl_InsertItem( bb.m_hWnd, pCImage->m_SerialID.m_ID, fa, TVI_LAST, lp );
	CTreeList_SetItem( bb.m_hWnd, items, 1, pCImage->m_UserID.m_ID, lp );
	
	CString str_value;
	str_value.Format( _T("%02d/%02d/%04d %02d:%02d:%02d"),pCImage->m_RecTime.m_Date.m_Month,
		pCImage->m_RecTime.m_Date.m_Day, 
		pCImage->m_RecTime.m_Date.m_Year,
		pCImage->m_RecTime.m_Time.m_Hour,
		pCImage->m_RecTime.m_Time.m_Minute,
		pCImage->m_RecTime.m_Time.m_Second
		);
	
	CTreeList_SetItem( bb.m_hWnd, items, 2, str_value, lp );
	
	delete pCImage;
	return 0;
}

void CTestDlg::DevRealLoadUser( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	if ( pvContent )
	{
		DEV_USER_REAL_LOAD& load_mode = *((DEV_USER_REAL_LOAD*)pvContent);
		CString str_sql;
		if ( !load_mode.m_bApeendID )
			str_sql.Format( _T("select * from [user] where ([UserId] = \'%s\')"), load_mode.m_ID.m_ID );
		else 
			str_sql.Format( _T("select * from [user] where ([AppendID] = \'%s\')"), load_mode.m_ID.m_ID );
		CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL( str_sql );
		
		if ( ptr->IsEmpty() )
		{
			DEV_USER user = {0};
			if ( !load_mode.m_bApeendID )
				memcpy( user.m_UserID.m_ID, load_mode.m_ID.m_ID, sizeof(DEV_CID) ); 
			else 
				memcpy( user.m_AppendID.m_ID, load_mode.m_ID.m_ID, sizeof(DEV_CID) ); 
			
			//CPM_AddUser(cszDevAddr, user, load_mode);
		}
		else
		{
			DEV_USER user = {0};
			memcpy( user.m_UserID.m_ID, ptr->GetCString( _T("UserID") ).GetBuffer(0), ptr->GetCString( _T("UserID") ).GetLength() );    
			memcpy( user.m_ImageSID.m_ID, ptr->GetCString( _T("ChangeID") ).GetBuffer(0), ptr->GetCString( _T("ChangeID") ).GetLength() );
			memcpy( user.m_AppendID.m_ID, ptr->GetCString( _T("AppendID") ).GetBuffer(0), ptr->GetCString( _T("AppendID") ).GetLength() );      
			memcpy( user.m_UserName.m_ID, ptr->GetCString( _T("UserName") ).GetBuffer(0), ptr->GetCString( _T("UserName") ).GetLength() );       
			memcpy( user.m_Department.m_ID, ptr->GetCString( _T("Department") ).GetBuffer(0), ptr->GetCString( _T("Department") ).GetLength() );      
			memcpy( user.m_Admin.m_ID, ptr->GetCString( _T("RegAdmin") ).GetBuffer(0), ptr->GetCString( _T("RegAdmin") ).GetLength() );           
			user.m_UserFlag = ptr->GetLong( _T("UserFlag") );          
			user.m_UserType = DEV_USER_TYPE(ptr->GetLong(_T("UserType")));    
			user.m_RightID = ptr->GetLong(_T("RightID"));   
			
			COleDateTime time;
			time.ParseDateTime( ptr->GetCString( _T("InvalidTime") ) );
			user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
			user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
			user.m_DeadLineTime.m_Date.m_Day = time.GetDay();		
			user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
			user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
			user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
			
			time.ParseDateTime( ptr->GetCString( _T("RegistTime") ) );
			user.m_RegistTime.m_Date.m_Year = time.GetYear();
			user.m_RegistTime.m_Date.m_Month = time.GetMonth();
			user.m_RegistTime.m_Date.m_Day = time.GetDay();		
			user.m_RegistTime.m_Time.m_Hour = time.GetHour();
			user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
			user.m_RegistTime.m_Time.m_Second = time.GetSecond();
			
			time.ParseDateTime( ptr->GetCString( _T("LastUpdTime") ) );
			user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
			user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
			user.m_LastUpdTime.m_Date.m_Day = time.GetDay();		
			user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
			user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
			user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
			
			time.ParseDateTime( ptr->GetCString( _T("TypeUpdTime") ) );
			user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
			user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
			user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();		
			user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
			user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
			user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
			
			char buf[150*1024] = {0};
			
			if ( (load_mode.m_LoadFlag & DL_DEV_PIC) || (load_mode.m_LoadFlag & DL_DEV_USER_FEAT) )
			{
				ptr->GetChunk( _T("Block"), (BYTE*)buf, ptr->GetLong(_T("FeatBytes"))+ptr->GetLong(_T("PicBytes") ) );
				
				user.m_FeatLen  = (load_mode.m_LoadFlag & DL_DEV_USER_FEAT) ? ptr->GetLong(_T("FeatBytes")) : 0;
				user.m_PicLen   = (load_mode.m_LoadFlag & DL_DEV_PIC) ? ptr->GetLong(_T("PicBytes") ) : 0;
				user.m_FeatData = (load_mode.m_LoadFlag & DL_DEV_USER_FEAT) ? buf : NULL;
				user.m_PicData  = (load_mode.m_LoadFlag & DL_DEV_PIC) ? (buf + user.m_FeatLen) : NULL;
				
			}
			
			CPM_ULRealTimeUser(cszDevAddr, user, load_mode) ;
			delete[] buf;
		}
		
		DEBUG_INFO1(_T("%s 请求实时用户加载"), cszDevAddr );
	}
}

void CTestDlg::DevLogNumsOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
	if ( pRange->m_Count <= 0 ) 
	{
		DEBUG_INFO1( _T("%s"), _T("操作日志下载失败") );
	}
	else
	{
		DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY;
		pArray->m_Count = pRange->m_Count;
		pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
		memcpy( pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);
		
		this->PostMessage(LOG_NUMS_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pArray );
	}
}

LRESULT CTestDlg::DevLogNumsMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)lpparam;
	char IP[50] = {0};
	if ( wparam == 0 )
	{
		CString str;
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
		strcpy( IP, str.GetBuffer(0) );
	}
	else
	{
		LongToDotIp( IP, wparam );
	}
	int i = 0;
	for ( i = 0; i < pRange->m_Count; ++i )
	{
		pRange->m_pRange[i].m_bOpenRange = FALSE;
		CPM_DLRangeLog( IP,  pRange->m_pRange[i], 0x01, TRUE);
	}
	
	delete [] pRange->m_pRange;
	pRange->m_pRange = NULL;
	delete pRange;
	return 0;
}

void CTestDlg::DevUserNumsOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	DEV_DL_USER_RANGE* pRange = (DEV_DL_USER_RANGE*)pvContent;	
	if ( pRange->m_Count <= 0 )
	{
		DEBUG_INFO1( _T("%s"), _T("用户下载失败") );
	}
	else
	{
		DEBUG_INFO1( _T("%s"), _T("开始用户下载") );
		DEV_DL_USER_RANGE* pArray = new DEV_DL_USER_RANGE;
		pArray->m_Count = pRange->m_Count;
		pArray->m_pArray = new DEV_VUSER[pArray->m_Count];
		memcpy( pArray->m_pArray, pRange->m_pArray, sizeof(DEV_VUSER)*pRange->m_Count);		
		this->PostMessage(USER_NUMS_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pArray );
	}
}

LRESULT  CTestDlg::DevUserNumsMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_DL_USER_RANGE* pRange = (DEV_DL_USER_RANGE*)lpparam;
	char IP[50] = {0};
	if ( wparam == 0 )
	{
		CString str;
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
		strcpy( IP, str.GetBuffer(0) );
	}
	else
	{
		LongToDotIp( IP, wparam );
	}
	int flag = DL_DEV_PIC | DL_DEV_USER_FEAT;
	int i = 0;
	BOOL ret = TRUE;
	DEV_VUSER* pTmp = (DEV_VUSER*)( pRange->m_pArray );
	for ( ; i < pRange->m_Count; ++i )
	{
		do
		{
			if ( CPM_DLSingleUser( IP, pTmp[i].m_UserID , flag) ) break;
			Sleep(100);
		}while ( TRUE );
		WaitForSingleObject( m_hEvent, INFINITE );
	}
	delete [] pRange->m_pArray;
	pRange->m_pArray = NULL;
	delete pRange;
	return 0;
}


void CTestDlg::DevChangeImageNumsOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
	if ( pRange->m_Count <= 0 ) 
	{
		DEBUG_INFO1( _T("%s"), _T("变更注册照下载失败") );
	}
	else
	{
		DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY; 
		pArray->m_Count = pRange->m_Count;
		pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
		memcpy( pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);
		
		this->PostMessage(CIMAGE_NUMS_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pArray );
	}
}

LRESULT CTestDlg::DevCImageNumsMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)lpparam;
	char IP[50] = {0};
	if ( wparam == 0 )
	{
		CString str;
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
		strcpy( IP, str.GetBuffer(0) );
	}
	else
	{
		LongToDotIp( IP, wparam );
	}
	int i = 0;
	for ( i = 0; i < pRange->m_Count; ++i )
	{
		pRange->m_pRange[i].m_bOpenRange = FALSE;
		if( ((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck() ) 
		{
			CPM_DLRecEx( IP, pRange->m_pRange[i] );
			continue;
		}
		CPM_DLRangeCImage( IP, pRange->m_pRange[i], 0x01, TRUE, 2);
	}	
	delete [] pRange->m_pRange;
	pRange->m_pRange = NULL;
	delete pRange;
	return 0;
}

void CTestDlg::DevRecNums( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
	if ( pRange->m_Count <= 0 ) 
	{
		DEBUG_INFO1( _T("%s"), _T("记录下载失败") );
	}
	else
	{
		DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY;
		pArray->m_Count = pRange->m_Count;
		pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
		memcpy( pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);	
		this->PostMessage(REC_NUMS_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pArray );
	}
}

LRESULT CTestDlg::DevRecNumsMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)lpparam;
	char IP[50] = {0};
	if ( wparam == 0 )
	{
		CString str;
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
		strcpy( IP, str.GetBuffer(0) );
	}
	else
	{
		LongToDotIp( IP, wparam );
	}
	int ntotal = 0;
	int i = 0;
	DEV_DL_RECORD_RANGE* pTemp = (DEV_DL_RECORD_RANGE*)( pRange->m_pRange );
	for ( i = 0; i < pRange->m_Count; ++i )
	{
		pTemp->m_bOpenRange = FALSE;
		if( ((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck() ) 
		{
			//	cpm_( IP, pRange->m_pRange[i] );
			continue;
		}
		if ( !CPM_DLRangeRec( IP, pTemp[i] , 0x01 , true) ) 
		{
			break;
		}
	}	
	delete [] pRange->m_pRange;
	pRange->m_pRange = NULL;
	delete pRange;
	return 0;
}

void CTestDlg::DevAlarmNums( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
	if ( pRange->m_Count <= 0 ) 
	{
		DEBUG_INFO1( _T("%s"), _T("记录下载失败") );
	}
	else
	{
		DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY;
		pArray->m_Count = pRange->m_Count;
		pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
		memcpy( pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);
		
		this->PostMessage(ALARMS_NUMS_MSG, DotIpToLong( cszDevAddr ), (LPARAM)pArray );
	}
}

LRESULT CTestDlg::DevAlarmNumsMsg( WPARAM wparam, LPARAM lpparam )
{
	DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)lpparam;
	char IP[50] = {0};
	if ( wparam == 0 )
	{
		CString str;
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
		strcpy( IP, str.GetBuffer(0) );
	}
	else
	{
		LongToDotIp( IP, wparam );
	}	int i = 0;
	for ( i = 0; i < pRange->m_Count; ++i )
	{
		pRange->m_pRange[i].m_bOpenRange = FALSE;
		if( ((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck() ) 
		{
			CPM_DLRecEx( IP, pRange->m_pRange[i] );
			continue;
		}
		CPM_DLRangeAlarm( IP, pRange->m_pRange[i], 0x01, TRUE);
	}	
	delete [] pRange->m_pRange;
	pRange->m_pRange = NULL;
	delete pRange;
	return 0;
}

void CTestDlg::OnClose( )
{
	KillTimer(SERCH_EVENT_TIME);
	CPM_InitSys(FALSE);
	// 	return;
	CDialog::OnClose();
}

void CTestDlg::OnButtonTimegroup() 
{
	// TODO: Add your control notification handler code here
	CGroupTimeDlg dlg(m_ado);
	dlg.DoModal();
}

void CTestDlg::OnButtonRright() 
{
	// TODO: Add your control notification handler code here
	CRightDlg dlg(m_ado);
	dlg.DoModal();
}

void CTestDlg::OnButtonUG() 
{
	// TODO: Add your control notification handler code here
	UserGroupDlg dlg(m_ado);
	dlg.DoModal();
}

void CTestDlg::OnButtonOpenDoor() 
{
	// TODO: Add your control notification handler code here
	static BOOL bOpen = TRUE;
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CPM_ULOpenDoorState( str_ip.GetBuffer(0), DEV_DOOR_OPEN  ) ;
	GetDlgItem( IDC_BUTTON_OPEN_DOOR )->SetWindowText( bOpen ? _T("紧急关门") : _T("紧急开门") );
	bOpen = !bOpen;
	UpdateData(TRUE);
}

void CTestDlg::OnButtonCancelOpenDoor() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	
	CPM_ULOpenDoorState( str_ip.GetBuffer(0), DEV_DOOR_CLOSE  ) ;
	
	CPM_DLOpenDoorState( str_ip.GetBuffer(0) );
}

void CTestDlg::OnButtonROpenDoor() 
{
	// TODO: Add your control notification handler code here
	static BOOL bOpen = TRUE;
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CPM_ULRemoteOpenDoor( str_ip.GetBuffer(0), bOpen ) ;
	
	GetDlgItem( IDC_BUTTON_REMOTE )->SetWindowText( bOpen ? _T("远程关门") : _T("远程开门") );
	bOpen = !bOpen;
	UpdateData(TRUE);
}

void CTestDlg::OnButtonECloseDoor()
{
	static BOOL bOpen = FALSE;
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CPM_ULOpenDoorState( str_ip.GetBuffer(0), DEV_DOOR_NOMAL  ) ;
}

void CTestDlg::OnButtonRCloseDoor()
{
	static BOOL bOpen = FALSE;
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CPM_ULRemoteOpenDoor( str_ip.GetBuffer(0), bOpen ) ;
}

void CTestDlg::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_user.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一条记录") );
		return;
	}
	int n = m_user.GetNextSelectedItem(pos); 
	CString str_id = m_user.GetItemText(n, 0);
	AddUserDlg dlg( m_ado, str_id.GetBuffer(0) );
	if ( IDOK == dlg.DoModal() )
	{
		char values[5] ={0};
		int items = m_user.GetItemCount();
		int count = m_user.GetItemCount();
		CString id(dlg.m_ID);
		CString tmp;
		
		m_user.SetItemText( n, 0, dlg.m_ID );
		m_user.SetItemText( n, 1, dlg.m_Card );
		m_user.SetItemText( n, 2, dlg.m_Name );
		
		
		
		int R_ID = dlg.m_Right;
		sprintf( values, "%d", R_ID );
		m_user.SetItemText( n, 3, values );
		
		sprintf( values, "%d", dlg.m_UserType );	
		m_user.SetItemText( n, 4, values );
		sprintf( values, "%d", dlg.m_userFlag);
		m_user.SetItemText( n, 5, values );
		
		
		m_user.SetItemText( n, 6, dlg.m_invalideTime );
		
		
		CString sql ;
		sql.Format(_T("update [user] set [AppendID]=\'%s\', [UserName]=\'%s\', [Department]=\'%s\', \
			[RightID]=%d, [UserType]=%d, [UserFlag]=%d, [InvalidTime]=\'%s\' where [UserID]=\'%s\'") ,
			dlg.m_Card, dlg.m_Name, dlg.m_Dep, R_ID, dlg.m_UserType, dlg.m_userFlag , dlg.m_invalideTime, dlg.m_ID
			);
		m_ado.ExecuteNoSelectSQL(sql);
		
		UpdateData(FALSE);
		
		m_user.SetItemState( n, LVIS_SELECTED, LVIS_SELECTED );
		
	}
}



void CTestDlg::OnButtonConnect() 
{
	UpdateData(TRUE);
	char ip[16] = {0};
	BYTE field0 = 0, field1=0, field2=0, field3=0;
	m_IPDev.GetAddress(field0, field1, field2, field3);
	sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
	
    if ( strlen(ip) == 0 )
    {
        DEBUG_INFO1(_T("%s"), _T("设备IP为空"));
        return ;
    }
	
	CPM_CNDev( ip);
}

void CTestDlg::OnButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CPM_DCNDev( str_ip.GetBuffer(0) );
}

void CTestDlg::OnUpdate()
{
	UpdateData( TRUE );
	
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CFileDialog dlg(TRUE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "(*.*)|*.*||" );
	if ( dlg.DoModal() != IDOK ) return;
	CString file = dlg.GetPathName();
	// 	GetDlgItem(IDC_EDIT_FILE)->GetWindowText( file );
	FILE* pfile = fopen( dlg.GetPathName(), "rb" );
	if ( pfile )
	{
		DEV_APPUPDATE app = {0};
		fseek(pfile, 0, SEEK_END );
		app.m_Bytes = ftell(pfile);
		fseek(pfile, 0, SEEK_SET);
		app.m_Buf = new char[app.m_Bytes];
		memset( app.m_Buf, 0x0, app.m_Bytes );
		fread( app.m_Buf, 1, app.m_Bytes, pfile );
		app.m_FileIndex = 1;
		app.m_TotalFiles = 1;
		
		int nCount = m_dev.GetItemCount();
		for ( int i = 0; i < nCount; ++i )
		{
			CString str_ip = m_dev.GetItemText(i, 0);
			CPM_ULUpdate( str_ip.GetBuffer(0), app );
		}
		delete [] app.m_Buf;
		fclose(pfile);
	}
}

void CTestDlg::OnFile()
{
// 	CFileDialog dlg(TRUE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "(*.*)|*.*||" );
// 	if ( dlg.DoModal() != IDOK ) return;
// 	GetDlgItem(IDC_EDIT_FILE)->SetWindowText( dlg.GetPathName() );
// 	UpdateData( FALSE );
	CPasswordDlg dg;
	DWORD dw = dg.DoModal();
	if (IDOK != dw )
		return;

	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	
	int nIndex = 0;
	m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CCreateDynamicDlg dlg( str_ip.GetBuffer(0),this);
	dlg.DoModal();
	CPM_RegOperResultNotifyCB( ::DevOperResult, this );
	GetDlgItem(IDC_BUTTON_FILE)->ShowWindow(SW_HIDE);
}

void CTestDlg::OnButtonDLLog()
{
	POSITION pos;
	int nIndex;
	CString str_ip;
	if (m_bNet)
	{
		pos = m_dev.GetFirstSelectedItemPosition(); 
		if ( pos == NULL )
		{
			AfxMessageBox( _T("请选择一个设备") );
			return;
		}
		nIndex = m_dev.GetNextSelectedItem(pos); 
		str_ip = m_dev.GetItemText(nIndex, 0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
		if ( str_ip.IsEmpty() )
		{
			AfxMessageBox( _T("请选择一个U盘数据库") );
			return;
		}
	}
	
	
	UpdateData(TRUE);
	DEV_DL_RECORD_RANGE range = {0};
	CString count ;
	GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
	
	range.m_Count = atoi( count.GetBuffer(0) );
	CString id ;
	GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
	memcpy( range.m_SID.m_ID, id.GetBuffer(0), id.GetLength() );
	
	
	BOOL flag = 1;
	if ( ((CButton*)(GetDlgItem(IDC_RADIO_ALL)))->GetCheck() )
	{
		flag = true;
	}
	else if ( ((CButton*)(GetDlgItem(IDC_RADIO_LOG_ID)))->GetCheck() )
	{
		flag = false;
	}
	BOOL ret = flag ? CPM_DLLog( str_ip ) : CPM_DLRangeLog(str_ip, range);	
	if ( !ret )	
	{	DEBUG_INFO1( _T("%s"), _T("请求下载操作日志具体数据") );}
	else 
	{	DEBUG_INFO1( _T("%s"), _T("请求下载操作日志ID集合") );}	
}

void CTestDlg::OnButtonDelLog()
{
	m_log.DeleteAllItems();
}

void CTestDlg::OnButtonDLImage()
{
	POSITION pos;
	int nIndex;
	CString str_ip;
	if ( m_bNet )
	{
		pos = m_dev.GetFirstSelectedItemPosition(); 
		if ( pos == NULL )
		{
			AfxMessageBox( _T("请选择一个设备") );
			return;
		}
		nIndex = m_dev.GetNextSelectedItem(pos); 
		str_ip = m_dev.GetItemText(nIndex, 0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
		if ( str_ip.IsEmpty() )
		{
			AfxMessageBox( _T("请选择一个U盘数据库") );
			return;
		}
	}
	
	UpdateData(TRUE);
	DEV_DL_RECORD_RANGE range = {0};
	CString count ;
	GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
	
	range.m_Count = atoi( count.GetBuffer(0) );
	CString id ;
	GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
	memcpy( range.m_SID.m_ID, id.GetBuffer(0), id.GetLength() );
	
	
	BOOL flag = 1;
	if ( ((CButton*)(GetDlgItem(IDC_RADIO_ALL_IMAGE)))->GetCheck() )
	{
		flag = true;
	}
	else if ( ((CButton*)(GetDlgItem(IDC_RADIO_IMAGE_ID)))->GetCheck() )
	{
		flag = false;
	}
	
	BOOL ret = flag ? CPM_DLChangeImage( str_ip, 2 ) : CPM_DLRangeCImage(str_ip, range, 0x01);	
	if ( !ret )	
	{	DEBUG_INFO1( _T("%s"), _T("记录请求下载发送失败") );}
	else 
	{	DEBUG_INFO1( _T("%s"), _T("记录请求下载发送成功") );}
}

void CTestDlg::OnButtonDelImage()
{
	CString str_sql( _T("delete from [record]") );
	m_ado.ExecuteNoSelectSQL(str_sql); 
	m_nImageItems = 0;
	m_image.DeleteAllItems();
}


LRESULT CTestDlg::OnUpdateImage(WPARAM wparam, LPARAM lpparam)
{
	int i = 0;
	int count = m_pImageList.size();
	for (; i < count; ++i)
	{
		DEV_CHANGEIMAGE* pCImage = (DEV_CHANGEIMAGE*)m_pImageList.front();
		COLORREF color = -1;
		TLCONTROLINFO info[] = {
			{TLCT_TEXT, color, ""},
			{TLCT_TEXT, color, ""},
			{TLCT_TEXT, color, ""}
		};	
		
		HTREEITEM fa = TVI_ROOT;
		LPARAM lp = (LPARAM)info;
		int items = ListView_GetItemCount(bb.m_hWnd);		
		CTreeCtrl_InsertItem( bb.m_hWnd, pCImage->m_SerialID.m_ID, fa, TVI_LAST, lp );
		CTreeList_SetItem( bb.m_hWnd, items, 1, pCImage->m_UserID.m_ID, lp );
		
		CString str_value;
		str_value.Format( _T("%02d/%02d/%04d %02d:%02d:%02d"),pCImage->m_RecTime.m_Date.m_Month,
			pCImage->m_RecTime.m_Date.m_Day, 
			pCImage->m_RecTime.m_Date.m_Year,
			pCImage->m_RecTime.m_Time.m_Hour,
			pCImage->m_RecTime.m_Time.m_Minute,
			pCImage->m_RecTime.m_Time.m_Second
			);
		
		CTreeList_SetItem( bb.m_hWnd, items, 2, str_value, lp );
		
		CString strSerialID  ;
		
		strSerialID.Format( "%s", pCImage->m_SerialID.m_ID );
		
		CString tmp_sql;
		tmp_sql.Format( _T("select * from [record] where ([SerialID]=\'%s\')"), strSerialID );
		CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(tmp_sql);
		if ( tmp->IsEmpty() ) 
		{			
			CString sql;
			sql.Format( _T("insert into [record] ([SerialID], [PicBytes])values ( \'%s\', %d)"), strSerialID, pCImage->m_PicBytes);
			
			m_ado.ExecuteNoSelectSQL(sql);
			sql.Format( _T("select [Block] from [record] where ([SerialID]=\'%s\')"), strSerialID );
			CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
			
			ptr->AppendChunk( _T("Block"),  (BYTE*)(pCImage->m_PicData), pCImage->m_PicBytes );
			ptr->Update();
		}
		
		m_pImageList.pop_front();
		count--;
		delete [] pCImage->m_PicData;
		pCImage->m_PicData = NULL;
		delete pCImage;
	}
	
	
	return 0;
}

void CTestDlg::OnButtonCNServer()
{
	UpdateData(TRUE);
	char ip[16] = {0};
	BYTE field0 = 0, field1=0, field2=0, field3=0;
	m_IPServer.GetAddress(field0, field1, field2, field3);
	sprintf(m_serverIP, "%d.%d.%d.%d", field0, field1, field2, field3);
    if ( strlen(m_serverIP) == 0 )
    {
        DEBUG_INFO1( _T("%s") ,_T("中转服务器IP为空"));
        return ;
    }
	//CPM_CNServer(m_serverIP);
}

void CTestDlg::OnButtonDCNServer()
{
	UpdateData(TRUE);
	char ip[16] = {0};
	BYTE field0 = 0, field1=0, field2=0, field3=0;
	m_IPServer.GetAddress(field0, field1, field2, field3);
	sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
	CPM_DCNDev(ip);
}

void CTestDlg::OnCmdNetOrUDisk( UINT nID )
{
	if (IDC_RADIO_NET == nID)
	{
		m_bNet = TRUE;
	}
	else
	{
		m_bNet = FALSE;
	}
}

void CTestDlg::OnBtnUDiskScan()
{
	CFileDialog dlg(TRUE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "db Files (*.db)|*.db||" );
	if ( dlg.DoModal() != IDOK ) return;
	GetDlgItem(IDC_EDIT_UPATH)->SetWindowText(dlg.GetPathName());
	UpdateData(FALSE);
}

void CTestDlg::OnBtnUDiskCN()
{
	UpdateData(TRUE);
    CString str;
	GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
	CPM_CNDev(str.GetBuffer(0));	
}


void CTestDlg::OnBtnStartVideo()
{
	VideoDlg* pDlg = new VideoDlg;
	CStringArray IPArray;
	int nCount = m_dev.GetItemCount();
	int i = 0;
	for ( i=0; i < nCount; ++i)
	{
		IPArray.Add( m_dev.GetItemText(i, 0) );
	}
	pDlg->Init( IPArray );
	pDlg->Create( IDD_DIALOG_VIDEO );
	pDlg->ShowWindow( SW_SHOW );
	// 	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	// 	if ( pos == NULL )
	// 	{
	// 		return;
	// 	}
	// 	int nIndex = m_dev.GetNextSelectedItem(pos); 
	// 	CString str_ip = m_dev.GetItemText(nIndex, 0);
	// 	CPM_StartVideo( str_ip.GetBuffer(0), (long)( GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd() ) );
}

void CTestDlg::OnBtnCap()
{
	UpdateData( TRUE );
	CString str;
	GetDlgItem( IDC_EDIT_ID_USER )->GetWindowText(str);
	
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		return;
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	DEV_USER user = { 0 };
	strcpy( user.m_UserID.m_ID , str.GetBuffer(0) );
	CPM_ULRealTimeUserCap( str_ip.GetBuffer(0), user , -1 );
}

void CTestDlg::OnBtnReboot()
{
	POSITION pos = m_dev.GetFirstSelectedItemPosition(); 
	if ( pos == NULL )
	{
		AfxMessageBox( _T("请选择一个设备") );
	}
	int nIndex = m_dev.GetNextSelectedItem(pos); 
	CString str_ip = m_dev.GetItemText(nIndex, 0);
	CPM_RebootDev( str_ip.GetBuffer(0) );
}


void CTestDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( IsClickDmyButton(point) )
	{
		m_nLClickTimes++;
		if ( m_nLClickTimes == 6 )
		{
			GetDlgItem(IDC_BUTTON_FILE)->ShowWindow(SW_SHOW);
			m_nLClickTimes = 0;
		}
	}
	else
		m_nLClickTimes = 0;
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CTestDlg::IsClickDmyButton(CPoint pt)
{
	CRect rc;
	GetDlgItem(IDC_BUTTON_FILE)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	if ( rc.PtInRect( pt ) )
		return TRUE;
	return false;
}

