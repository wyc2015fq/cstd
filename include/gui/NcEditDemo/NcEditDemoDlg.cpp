// NcEditDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NcEditDemo.h"
#include "NcEditDemoDlg.h"
#include ".\nceditdemodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CNcEditDemoDlg 对话框



CNcEditDemoDlg::CNcEditDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNcEditDemoDlg::IDD, pParent)
	,m_pNcEditWnd(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNcEditDemoDlg::~CNcEditDemoDlg()
{
	if (m_pNcEditWnd!=NULL){
		delete m_pNcEditWnd;
	}
}

void CNcEditDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNcEditDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_OPEN, OnMenuOpen)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CNcEditDemoDlg 消息处理程序

BOOL CNcEditDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CNcEditDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNcEditDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CNcEditDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CNcEditDemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rcWin;
	GetClientRect(&rcWin);
	m_pNcEditWnd=new CNcEditWnd();
	m_pNcEditWnd->Create(WS_EX_CLIENTEDGE,rcWin,this);
	m_pNcEditWnd->InitNcEdit(12,"Fixedsys");
	m_pNcEditWnd->ShowWindow(SW_SHOW);

	return 0;
}

void CNcEditDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rcWin;
	GetClientRect(&rcWin);
	m_pNcEditWnd->MoveWindow(&rcWin);
	// TODO: 在此处添加消息处理程序代码
}

void CNcEditDemoDlg::OnMenuOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog fDlg(TRUE);
	if (IDOK==fDlg.DoModal())
	{
		m_pNcEditWnd->OpenFile(fDlg.GetPathName());
	}
}

BOOL CNcEditDemoDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	struct SCNotification* scn = (struct SCNotification*)lParam;
	LPNMHDR pnmh = (LPNMHDR)lParam;
	switch(pnmh->code)
	{
	case SCN_MODIFIED://修改了文件
	case SCN_ZOOM://放大，缩小
		m_pNcEditWnd->UpdateStyle();
		break;
	case SCN_UPDATEUI://界面更新(单击鼠标，按下箭头等)
		int n=m_pNcEditWnd->GetCurrentLine();
		//当前行
		break;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CNcEditDemoDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szBufferName[MAX_PATH];
	DragQueryFile(hDropInfo,0,szBufferName,MAX_PATH);
	m_pNcEditWnd->OpenFile(szBufferName);
	CDialog::OnDropFiles(hDropInfo);
}
