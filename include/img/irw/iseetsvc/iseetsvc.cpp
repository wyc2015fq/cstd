/********************************************************************

	iseetsvc.cpp

	----------------------------------------------------------------
    软件许可证 － GPL
	版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
	可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
	据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
	目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
	GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	或发信到：

			isee##vip.163.com
	----------------------------------------------------------------
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件框架
					类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30318
	最后修改于：	2003-3-18

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-3		增加高精度计时器类
		2003-1		第一个测试版发布

********************************************************************/

#include "stdafx.h"
#include "iseetsvc.h"

#include "shellpidl.h"
#include "shelllist.h"
#include "shelltree.h"
#include "shellpath.h"

#include "Display.h"
#include "LeftView.h"
#include "RightView.h"

#include "MainFrm.h"
#include "iseetsvcDoc.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTimeCounter::CTimeCounter()
{
	m_liStart.QuadPart = (LONGLONG)0;
	m_liEnd.QuadPart   = (LONGLONG)0;
	m_dwSpace          = 0UL;

	// 获取计时器频率
	::QueryPerformanceFrequency(&m_liFrequ);

	// 转换为毫秒单位的频率
	m_liFrequ.QuadPart = m_liFrequ.QuadPart / 1000;
}

CTimeCounter::~CTimeCounter()
{
}

// 开始计时
void CTimeCounter::BeginCount(DWORD space/* 间隔时间（毫秒单位）*/)
{
	m_dwSpace = space;

	::QueryPerformanceCounter(&m_liStart);
}

// 判断是否到时
BOOL CTimeCounter::IsEnd()
{
	::QueryPerformanceCounter(&m_liEnd);

	if (((m_liEnd.QuadPart-m_liStart.QuadPart)/m_liFrequ.QuadPart) >= (LONGLONG)m_dwSpace)
	{
		return TRUE;									// 已超出设定时间
	}

	return FALSE;										// 未到时间
}

// 获取从开始计时起，到当前所流逝的时间（毫秒单位）
DWORD CTimeCounter::GetTime()
{
	::QueryPerformanceCounter(&m_liEnd);

	return (DWORD)((m_liEnd.QuadPart-m_liStart.QuadPart)/m_liFrequ.QuadPart);
}




char	CLogRecorder::clr_welcome[] = "\r\n\t\t\tISee调试用操作记录文件\r\n=======================================================================\r\n\r\n";
char	CLogRecorder::clr_spek[]    = "-------------------------------------------------------------------\r\n";


CLogRecorder::CLogRecorder() : CFile()
{
}


CLogRecorder::~CLogRecorder()
{
}


int	CLogRecorder::OpenRecorder(LPCSTR logname)
{
	ASSERT(m_hFile == hFileNull);						// 不能重复打开记录文件

	TCHAR	szBuff[MAX_PATH];
	int		iLen;
	
	// 取得应用程序的全路径
	::GetModuleFileName(NULL, (LPTSTR)szBuff, MAX_PATH);
	
	iLen = lstrlen((char*)szBuff);
	
	// 去除尾部的应用程序名
	while ((szBuff[--iLen] != '\\')&&(iLen > 0))
	{
		szBuff[iLen] = '\0';
	}
	
	// 合成记录文件名
	lstrcat((LPSTR)szBuff, logname);

	// 创建记录文件
	if (Open((LPCTSTR)szBuff, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate) == 0)
	{
		return -1;										// 创建记录文件失败
	}

	SeekToEnd();

	Write((const void *)clr_welcome, lstrlen((LPCSTR)clr_welcome));

	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[开始记录时间：%Y-%m-%d  %H:%M:%S]\r\n");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	Write((const void *)clr_spek, lstrlen((LPCSTR)clr_spek));

	return 0;
}


int	CLogRecorder::CloseRecorder()
{
	if (m_hFile == CFile::hFileNull)
	{
		return -1;										// 记录文件并没有被创建
	}

	Write((const void *)"\r\n", 2);
	Write((const void *)clr_spek, lstrlen((LPCSTR)clr_spek));

	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[结束记录时间：%Y-%m-%d  %H:%M:%S]\r\n");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	Write((const void *)"\r\n\r\n\r\n\r\n", 8);

	Flush();
	Close();

	return 0;
}


//
int CLogRecorder::Record(LPCSTR str, int cont, int ipar)
{
	if (m_hFile == CFile::hFileNull)
	{
		return -1;										// 记录文件并没有被创建
	}

	static int	flcnt = 0;

	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[%H:%M:%S] - ");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	
	while (cont--)
	{
		Write((const void *)"\t", 1);
	}

	CString	stFS;

	stFS.Format(str, (int)ipar);

	Write((const void *)(LPCSTR)stFS, stFS.GetLength());
	Write((const void *)"\r\n", 2);

	if (!(flcnt % 10))
	{
		Flush();
	}

	return 0;
}


// 
int CLogRecorder::Record(LPCSTR str, int cont, LPCSTR ppar)
{
	if (m_hFile == CFile::hFileNull)
	{
		return -1;										// 记录文件并没有被创建
	}
	
	static int	flcnt = 0;
	
	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[%H:%M:%S] - ");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	
	while (cont--)
	{
		Write((const void *)"\t", 1);
	}
	
	CString	stFS;
	
	stFS.Format(str, ppar);
	
	Write((const void *)(LPCSTR)stFS, stFS.GetLength());
	Write((const void *)"\r\n", 2);
	
	if (!(flcnt % 10))
	{
		Flush();
	}
	
	return 0;
}


// 
CLogRecorder & ISeeTsVCDebugStrRecorder()
{
	return ((CIseetsvcApp*)AfxGetApp())->m_clRecord;
}

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp

BEGIN_MESSAGE_MAP(CIseetsvcApp, CWinApp)
	//{{AFX_MSG_MAP(CIseetsvcApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp construction

CIseetsvcApp::CIseetsvcApp()
{
}



/////////////////////////////////////////////////////////////////////////////
// The one and only CIseetsvcApp object

CIseetsvcApp theApp;


#define PACKVERSION(major,minor) MAKELONG(minor,major)

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp initialization

BOOL CIseetsvcApp::InitInstance()
{
	// 初始化COM库
	if (CoInitialize(NULL) != S_OK)
	{
		return FALSE;
	}

	// 创建调试用操作记录文件
	m_clRecord.OpenRecorder("iseetsvc.log");
	
	m_clRecord.Record("软件开始运行。", 0, 0);

	// 限定用户的操作系统
	if(GetDllVersion(TEXT("comctl32.dll")) < PACKVERSION(4,71))
	{
		m_clRecord.Record("IE的版本太低，软件未能运行。", 0, 0);
		::AfxMessageBox("本软件需要Win98+IE4.0以上的操作系统，您的系统不符合此最低要求，软件即将退出。");
		CoUninitialize();
		return FALSE;
	}
	
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	SetRegistryKey(_T("ISee Workgroup"));

	LoadStdProfileSettings();

	TCHAR	szBuff[MAX_PATH];
	int		iLen;
	
	// 取得应用程序的全路径（因为插件DLL也放在这个目录中）
	::GetModuleFileName(NULL, (LPTSTR)szBuff, MAX_PATH);
	
	iLen = lstrlen((char*)szBuff);
	
	// 去除尾部的应用程序名
	while ((szBuff[--iLen] != '\\')&&(iLen > 0))
	{
		szBuff[iLen] = '\0';
	}
	
	// 初始化ISee图象读写引擎
	if (isirw_init((char*)(LPCSTR)szBuff, 0))
	{
		m_clRecord.Record("图象读写引擎创建失败，软件退出。", 0, 0);
		::AfxMessageBox("ISee图象读写引擎创建失败！程序即将退出。");
		CoUninitialize();
		return FALSE;
	}

	// 开启引擎的象素缓冲区
	isirw_enbuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);

	m_clRecord.Record("图象读写引擎创建成功。", 0, 0);
	m_clRecord.Record("当前图象读写引擎支持的图象格式扩展名：%s", 0, (LPCSTR)isirw_get_shellstr());

	CSingleDocTemplate* pDocTemplate;

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIseetsvcDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));

	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 开启欢迎画面功能
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}



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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CIseetsvcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp message handlers


int CIseetsvcApp::ExitInstance() 
{
	m_clRecord.Record("软件正在退出...", 0, 0);

	// 关闭调试用记录文件
	m_clRecord.CloseRecorder();

	// 关闭引擎的象素缓冲区
	isirw_dibuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);

	// 关闭ISee图象读写引擎
	isirw_close();

	CoUninitialize();

	return CWinApp::ExitInstance();
}


// 获取DLL文件的版本（通用型，截取自MSDN）
DWORD CIseetsvcApp::GetDllVersion(LPCTSTR lpszDllName)
{
	HINSTANCE	hinstDll;
	DWORD		dwVersion = 0;

	// 读入DLL文件
	hinstDll = LoadLibrary(lpszDllName);

	if(hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;

		// 取得DllGetVersion通用接口函数的地址
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

		// 在IE3.0之前的DLL中可能不存在此接口函数，所以需要判断
		if(pDllGetVersion)
		{
			DLLVERSIONINFO	dvi;
			HRESULT			hr;

			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);

			// 获取DLL信息
			hr = (*pDllGetVersion)(&dvi);

			if(SUCCEEDED(hr))
			{
				// 将主版本号与子版本号压缩到一个DWORD变量中
				dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
			}
		}

		// 释放DLL
		FreeLibrary(hinstDll);
	}

	// 返回版本号，如果失败返回0
	return dwVersion;
}


BOOL CIseetsvcApp::OnIdle(LONG lCount) 
{
	// 优先处理MFC内部的空闲处理
	BOOL bRet = CWinApp::OnIdle(lCount);

	CMainFrame	*pmf = (CMainFrame*)AfxGetMainWnd();

	// 向文档类及框架类提供空闲处理的机会
	if (pmf&&pmf->GetSafeHwnd())
	{
		bRet |= pmf->OnIdleProc(lCount);

		CIseetsvcDoc *pid = (CIseetsvcDoc*)pmf->GetActiveDocument();

		if (pid)
		{
			bRet |= pid->OnIdleProc(lCount);
		}
	}
	
	return bRet;
}

BOOL CIseetsvcApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}
