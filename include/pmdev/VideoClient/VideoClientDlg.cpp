// VideoClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoClient.h"
#include "VideoClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoClientDlg dialog

CVideoClientDlg::CVideoClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideoClientDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoClientDlg message handlers

BOOL CVideoClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	HWND hWnd = GetDlgItem( IDC_VIDE )->m_hWnd;

	/*
	  整个流程如下:
	  1、初始化，初始化网络以及视频图像处理相关东西。
	  2、连接设备，并创建接收TCP数据的线程，用于接收控制设备，设备的响应消息。
	  3、发心跳响应包，保持本次连接。
	  4、再发登陆设备的身份权限验证。
	  5、创建UDP接收视频数据包的套接字，启动接收视频数据包的线程，向设备发起传送视频数据的控制命令，
      6、组包：由于一帧视频数据过大，一般为200多K，需要分包发送，接收方需要用一缓存缓冲当次视频帧的数据，即接收方要组包。
	  7、组包完毕后播放，传过来的视频图像数据是YUV需要转换成RGB，转换算法，采用的是查表法，故在1是要对转换表初始化。
	  8、向设备发起停止发送视频数据的命令。
	  9、关闭TCP/UDP套接字以及相应数据接收线程。

      注意事项：
	  1、网络包由：包头+包数据组成。并对包头进行了校验。
	  2、包数据由：命令头+命令数据组成。
	  3、整个网络包要求四字节对齐。
	  4、接收一次UDP包的数据缓存长度必须4字节对齐，且不少于要接收的数据的长度。
	  5、要初始化YUV转RGB的表。
	  6、UDP端口是随机获取的可用端口号。
	*/

	//初始化
	m_objVideoSDK.SDKInit(  hWnd, "192.168.1.121", "192.168.1.3", 13333 );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoClientDlg::OnPaint() 
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
HCURSOR CVideoClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVideoClientDlg::OnOK() 
{
	// TODO: Add extra validation here

	//建立连接
	m_objVideoSDK.SDKConnect();
    
	//开启视频
	m_objVideoSDK.SDKStartVideo();

}

void CVideoClientDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	//关闭视频
	m_objVideoSDK.SDKStopVideo();

    //断开连接
	m_objVideoSDK.SDKDisConnect();

	::InvalidateRect(GetDlgItem(IDC_VIDE)->m_hWnd,NULL,TRUE);
}

void CVideoClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_objVideoSDK.SDKUInit();
	CDialog::OnClose();
}
