# Windows服务框架 - 深之JohnChen的专栏 - CSDN博客

2017年04月19日 16:52:20[byxdaz](https://me.csdn.net/byxdaz)阅读数：1405


为了分离服务下的业务处理，将业务代码封装成dll，服务中动态调用dll中的函数。为了调试方便在服务中增加对话框进程模式，可以把其他程序(dll)作为服务加载运行，为了dll调试方便，增加了窗口程序运行模式(使用参数/debug)。在程序目录下CenterService.ini 用于服务名称、配置业务模块dll路径名、业务模块配置文件名。为支持服务可以自动化的调用各个服务器功能，这个服务器必须做成dll，接口包含以下7个。

 //-------------------------------------------------------------------------------------

 // 功能：初始化

 // 参数：无

 // 返回：错误代码

 //-------------------------------------------------------------------------------------  

 int YES_Initialize();

 //-------------------------------------------------------------------------------------

 // 功能：反初始化

 // 参数：无

 // 返回：错误代码

 //-------------------------------------------------------------------------------------

 int YES_UnInitialize();

 //-------------------------------------------------------------------------------------

 // 功能：设置工作参数

 // 参数：pParameter 工作参数(配置文件名称);nParameterLen 文件长度；nRervere 保留值，默认为0

 // 返回：错误代码

 //-------------------------------------------------------------------------------------

 int YES_SetWorkParamter(char *pParameter,int nParameterLen,int nRervere);

 //-------------------------------------------------------------------------------------

 // 功能：开始工作

 // 参数：无

 // 返回：错误代码

 //-------------------------------------------------------------------------------------

 int YES_StartWork();

 //-------------------------------------------------------------------------------------

 // 功能：停止工作

 // 参数：无

 // 返回：错误代码

 //-------------------------------------------------------------------------------------

 int YES_StopWork();

 //-------------------------------------------------------------------------------------

 // 功能：程序是否自动退出

 // 参数：无

 // 返回：bool

 //-------------------------------------------------------------------------------------

 bool YES_IsAutoStopRun();
 //-------------------------------------------------------------------------------------

 // 功能：写日志

 // 参数：日志描述文本

 // 返回：无

 //-------------------------------------------------------------------------------------

 void YES_WriteLog(char *pLogInfo);

流程图：

![](https://img-blog.csdn.net/20170419162155718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

实例代码：

```cpp
// CenterServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BusniessImplement.h"
#include "resource.h"
#include <process.h>

#define	 MAX_LOADSTRING			256

//定义全局函数变量
DWORD	   g_dwServiceMainThreadID = 0;							//服务线程ID
HANDLE     g_hServiceMainThread = NULL;							//服务线程句柄
BOOL	   g_bDebug = FALSE;									//是否调试模式
HWND	   g_hMain = NULL;										//主窗口
HINSTANCE hInst;												// 当前实例
TCHAR szTitle[MAX_LOADSTRING] = "ServiceMain";					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING] = "ServiceMain";			// 主窗口类名
TCHAR szServiceName[] = _T("ServiceMain");
BOOL bInstall = FALSE;
SERVICE_STATUS_HANDLE hServiceStatus = NULL;
SERVICE_STATUS status = {0};
DWORD dwThreadID = 0;

void Init();
BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();
BOOL ReadConfig();							//读取配置信息
BOOL StartServerWork();
void LogEvent(LPCTSTR pszFormat, ...);
void WINAPI ServiceMain();
void WINAPI ServiceStrl(DWORD dwOpcode);
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

TCHAR	   g_szCurrentPath[MAX_PATH] = {0};				//当前程序目录
TCHAR	   g_szConfigFileName[MAX_PATH] = {0};			//配置文件名称
TCHAR	   g_szWorkConfigFileName[MAX_PATH] = {0};		//工作主模块配置文件路径名
TCHAR	   g_szWorkMainDllName[MAX_PATH] = {0};			//工作主模块业务dll路径名
HANDLE	   g_hWorkThread	= NULL;						//工作线程句柄
BOOL	   g_bIsExistWorkThread = FALSE;				//是否退出工作线程

//业务处理
int BusinessProcess();
DWORD WINAPI ServiceMainThread(LPVOID lpParam)
{
	BusinessProcess();
	return 0;
}

//工作线程函数
unsigned int __stdcall WorkThreadFun(PVOID pParam)
{
	//业务处理
	BusinessProcess();
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	Init();

	dwThreadID = ::GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{ szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
		{ NULL, NULL }
	};

	if (stricmp(lpCmdLine, "/install") == 0)
	{
		Install();
	}
	else if (stricmp(lpCmdLine, "/uninstall") == 0)
	{
		Uninstall();
	}
	else if (stricmp(lpCmdLine, "/debug") == 0)
	{
		//调试模式
		g_bDebug = TRUE;
		status.dwCurrentState = SERVICE_START_PENDING;
		MyRegisterClass(hInstance);

		// 执行应用程序初始化
		if (!InitInstance(hInstance, nCmdShow))
		{
			return FALSE;
		}

		g_hServiceMainThread = ::CreateThread(NULL,0,ServiceMainThread,NULL,0, &g_dwServiceMainThreadID);
	}
	else
	{
		g_bIsExistWorkThread = FALSE;
		BOOL bRet = FALSE;
		bRet = ::StartServiceCtrlDispatcher(st);
		if (!bRet)
		{
			DWORD dd = GetLastError();
			LogEvent("%s %s",szServiceName,"Register Service Main Function Error!");
		}

		//停止服务
		LogEvent("%s %s",szServiceName,"Server stopped!");
	}

	if(g_bDebug)
	{
		MSG msg;
		// 主消息循环:
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		g_bIsExistWorkThread = TRUE;
		status.dwCurrentState = SERVICE_STOP_PENDING;
		if(g_hServiceMainThread != NULL)
		{
			WaitForSingleObject(g_hServiceMainThread,INFINITE);
		}
	}

	return 0;
}

/*
功能说明：初始化
*/
void Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_STOPPED;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;

	//获取当前程序目录			
	memset(g_szCurrentPath,0,sizeof(g_szCurrentPath));
	GetModuleFileName(NULL,g_szCurrentPath,MAX_PATH);
	char* pLastFlag = ::strrchr(g_szCurrentPath,'\\');
	if(pLastFlag != NULL)
	{
		int nFlagPosition = ::strlen(g_szCurrentPath) - ::strlen(pLastFlag);
		g_szCurrentPath[nFlagPosition] = 0;
	}

	//获取配置信息
	ReadConfig();
}

/*
功能说明：服务主函数，这在里进行控制对服务控制的注册
*/
void WINAPI ServiceMain()
{
	// Register the control request handler
	status.dwCurrentState = SERVICE_START_PENDING;//SERVICE_RUNNING;//
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	//注册服务控制
	hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);
	if (hServiceStatus == NULL)
	{
		LogEvent("%s %s",szServiceName,"Handler not installed!");
		return;
	}
	SetServiceStatus(hServiceStatus, &status);

	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);

	//应用时将主要任务放于此即可
	LogEvent("%s %s",szServiceName,"ServerDetect started!");

	//创建运行线程
	g_hWorkThread = (HANDLE)_beginthreadex(NULL, 0, WorkThreadFun, NULL, 0, NULL);
	if(g_hWorkThread != NULL)
	{
		WaitForSingleObject(g_hWorkThread,INFINITE);
		CloseHandle(g_hWorkThread);
	}
	//停止服务
	LogEvent("%s %s",szServiceName,"Server stopped!");

	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
}

/*
功能说明：	服务控制主函数，这里实现对服务的控制，当在服务管理器上停止或其它操作时，将会运行此处代码
参数说明:	dwOpcode，控制服务的状态
返回值：无
*/
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		{
			g_bIsExistWorkThread = TRUE;
			status.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(hServiceStatus, &status);
			Sleep(500);
			/*
			if(dwThreadID != 0)
			{
				PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
			}
			*/
		}
		break;
	case SERVICE_CONTROL_PAUSE:
		status.dwCurrentState = SERVICE_PAUSED;
		SetServiceStatus(hServiceStatus, &status);
		break;
	case SERVICE_CONTROL_CONTINUE:
		{
			g_bIsExistWorkThread = FALSE;
			//获取配置信息
			ReadConfig();
		}
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		g_bIsExistWorkThread = TRUE;
		status.dwCurrentState = SERVICE_STOP;
		SetServiceStatus(hServiceStatus, &status);
		Sleep(500);
		break;
	default:
		LogEvent("%s %s",szServiceName,"Bad service request!");
		break;
	}
}

/*
功能说明:判断服务是否已经被安装
参数说明：无
返回值：BOOL类型
*/
BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务
		SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

/*
功能说明:安装服务函数
参数说明：无
返回值：BOOL类型
*/
BOOL Install()
{
	if (IsInstalled())
		return TRUE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务
	SC_HANDLE hService = ::CreateService(
		hSCM, szServiceName, szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS/*|SERVICE_INTERACTIVE_PROCESS*/,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), szServiceName, MB_OK);
		return FALSE;
	}

	//ChangeServiceConfig(hService,SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, NULL, NULL,NULL,NULL,NULL,NULL,NULL);

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	return TRUE;
}

/*
功能说明:删除服务函数
参数说明：无
返回值：BOOL类型
*/
BOOL Uninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	LogEvent("%s %s",szServiceName,"Service could not be deleted!");
	return FALSE;
}

BOOL StartServerWork()
{
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), szServiceName, MB_OK);
		return FALSE;
	}
	//打开服务
	if(!::StartService(hService,0,NULL))
	{
		DWORD dd = GetLastError();
		MessageBox(NULL, _T("Couldn't start service"), szServiceName, MB_OK);
		CloseServiceHandle(hService); 
		CloseServiceHandle(hSCM);
		return FALSE; 
	}

	return TRUE;
}

//读取配置信息
BOOL ReadConfig()
{
	sprintf_s(g_szConfigFileName,MAX_PATH,"%s\\CenterServer.ini",g_szCurrentPath); 
	//读取服务名
	GetPrivateProfileString("service","name","ServiceName",szServiceName,MAX_PATH,g_szConfigFileName);
	//读取主模块配置
	TCHAR szWorkDllNameTmp[MAX_PATH]	=	_T("");			//主模块dll名称
	TCHAR szWorkConfigFileNameTmp[MAX_PATH]	=	_T("");		//主模块配置文件名称
	GetPrivateProfileString("main","configfile","config.xml",szWorkConfigFileNameTmp,MAX_PATH,g_szConfigFileName);
	if(strchr(szWorkConfigFileNameTmp,'\\') == NULL)
	{
		//构建全路径名称
		sprintf_s(g_szWorkConfigFileName,MAX_PATH,"%s\\%s",g_szCurrentPath,szWorkConfigFileNameTmp);
	}
	else
	{
		strcpy_s(g_szWorkConfigFileName,szWorkConfigFileNameTmp);
	}
	GetPrivateProfileString("main","dll","",szWorkDllNameTmp,MAX_PATH,g_szConfigFileName);
	if(strchr(szWorkDllNameTmp,'\\') == NULL)
	{
		//构建全路径名称
		sprintf_s(g_szWorkMainDllName,MAX_PATH,"%s\\%s",g_szCurrentPath,szWorkDllNameTmp);
	}
	else
	{
		strcpy_s(g_szWorkMainDllName,MAX_PATH,szWorkDllNameTmp);
	}

	if(strlen(g_szWorkConfigFileName) == 0 || strlen(g_szWorkMainDllName) == 0)
	{
		//参数错误
		LogEvent("%s %s",szServiceName,"read config fail!");
		return FALSE;
	}

	return TRUE;
}

/*
功能说明:记录服务事件(window日志，通过“事件查看器”查看)
参数说明：无
返回值：无
*/
void LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256] = {0};
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, szServiceName);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 400001, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

//业务处理
int BusinessProcess()
{
	BusniessBase *pBusiness = NULL;			   //业务处理
	pBusiness = new BusniessImplement(g_szWorkMainDllName);
	if(NULL == pBusiness)
	{
		LogEvent("new BusniessImplement fail!");
		return 1;
	}
	if(!((BusniessImplement *)pBusiness)->GetDllLoader())
	{
		LogEvent("%s %s",szServiceName,"DllLoader fail!");
		return 1;
	}

	//初始化
	int nRet = pBusiness->Initialize();
	if(nRet != 0)
	{
		LogEvent("%s %s",szServiceName,"BusniessImplement Initialize fail!");
		return 1;
	}
	//设置参数
	pBusiness->SetWorkParamter(g_szWorkConfigFileName,strlen(g_szWorkConfigFileName),0);
	//开启工作
	pBusiness->WriteLog("开启工作--pBusiness->StartWork()");
	pBusiness->StartWork();
	while(true)
	{
		if(g_bIsExistWorkThread)
		{
			//退出工作线程
			pBusiness->WriteLog("退出工作线程");
			break;
		}
		if(SERVICE_PAUSED == status.dwCurrentState)
		{
			//服务暂停中
			Sleep(200);
			pBusiness->WriteLog("服务暂停中");
			continue;
		}
		if(SERVICE_STOP_PENDING == status.dwCurrentState || SERVICE_STOP == status.dwCurrentState)
		{
			break;
		}
		if(pBusiness->IsAutoStopRun())
		{
			LogEvent("%s %s",szServiceName,"IsAutoStopRun true!");
			if(g_bDebug)
			{
				//退出窗口
				PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
				PostMessage(g_hMain,WM_CLOSE,0,0);
			}
			else
			{
				//程序自动退出
				status.dwCurrentState = SERVICE_STOP_PENDING;
				SetServiceStatus(hServiceStatus, &status);
				/*
				if(dwThreadID != 0)
				{
					PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
				}
				*/
			}
			break;
		}
		Sleep(200);
	}
	//停止工作
	pBusiness->StopWork();
	pBusiness->WriteLog("停止工作--pBusiness->StopWork()");
	//反初始化
	pBusiness->UnInitialize();
	if(pBusiness != NULL)
	{
		delete pBusiness;
	}
	LogEvent("%s %s",szServiceName,"BusinessProcess end...");
	return 0;

	/*
	int	nDetectInterval = 5*1000;//检测间隔(默认5秒)
	while (true)
	{
		if(status.dwCurrentState == SERVICE_PAUSED || status.dwCurrentState == SERVICE_PAUSE_PENDING)
		{
			Sleep(500);
			continue;
		}
		else if(status.dwCurrentState == SERVICE_STOPPED || status.dwCurrentState == SERVICE_STOP_PENDING)
		{
			break;
		}

		//业务处理

		//等待处理
		int nCellInterval = 200;
		int nIntervalLength = nDetectInterval/nCellInterval;
		for(int i=0;i<nIntervalLength;i++)
		{
			if(status.dwCurrentState == SERVICE_STOPPED || status.dwCurrentState == SERVICE_STOP_PENDING)
			{
				break;
			}
			Sleep(nCellInterval);
		}
	}

	return 0;
	*/
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	g_hMain = hWnd;
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```

```cpp
//DLL函数加载基础类

#include <Windows.h>

#ifndef  __DllLoaderBase_h__
#define  __DllLoaderBase_h__

#pragma once

class DllLoaderBase
{
public:
	DllLoaderBase();
	virtual ~DllLoaderBase();

public:
	//------------------------------------------------------------
	//功能：加载DLL
	//参数：pFileName:文件名
	//返回：成功与否
	//------------------------------------------------------------
	virtual bool	Initialize(char* pFileName);

	//------------------------------------------------------------
	//功能：加载需要用到的函数指针
	//参数：hModule:模块
	//返回：成功与否
	//------------------------------------------------------------
	virtual bool	LoadFunction(HMODULE hModule)=0;

	//------------------------------------------------------------
	//功能：释放DLL
	//参数：无
	//返回：无
	//------------------------------------------------------------
	virtual void	Uninitialize();

	//------------------------------------------------------------
	//功能：是否已经加载
	//参数：无
	//返回：加载与否
	//------------------------------------------------------------
	virtual bool	IsLoaded();

private:
	HMODULE			m_hModule;			//模块
};

#endif
```

```cpp
//业务控制dll加载类

#ifndef  __BusinessControlDllLoader_h__
#define  __BusinessControlDllLoader_h__

#pragma once
#include "dllloaderbase.h"

typedef int (__cdecl *pbns_Initialize)();//初始化
typedef int (__cdecl *pbns_UnInitialize)();//反初始化
typedef int (__cdecl *pbns_SetWorkParamter)(char *pParameter,int nParameterLen,int nRervere);//设置工作参数
typedef int (__cdecl *pbns_StartWork)();//开始工作
typedef int (__cdecl *pbns_StopWork)();//停止工作
typedef bool (__cdecl *pbns_IsAutoStopRun)();//程序是否自动停止运行
typedef void (__cdecl *pbns_WriteLog)(char *pLogInfo);//写日志

class BusinessControlDllLoader :
	public DllLoaderBase
{
public:
	BusinessControlDllLoader(void);
	virtual~BusinessControlDllLoader(void);

public:
	//------------------------------------------------------------
	//功能：加载需要用到的函数指针
	//参数：hModule:模块
	//返回：成功与否
	//------------------------------------------------------------
	virtual bool	LoadFunction(HMODULE hModule);

public:
	//初始化
	pbns_Initialize			m_bns_Initialize;
	//反初始化
	pbns_UnInitialize		m_bns_UnInitialize;
	//设置工作参数
	pbns_SetWorkParamter	 m_bns_SetWorkParamter;
	//开始工作
	pbns_StartWork			m_bns_StartWork;
	//停止工作
	pbns_StopWork			m_bns_StopWork;
	//是否自动停止运行
	pbns_IsAutoStopRun		m_bns_IsAutoStopRun;
	//写日志
	pbns_WriteLog			m_bns_WriteLog;
};

#endif
```

```cpp
#include "stdafx.h"
#include "BusinessControlDllLoader.h"
BusinessControlDllLoader::BusinessControlDllLoader(void)
{
 m_bns_Initialize = NULL;
 m_bns_UnInitialize = NULL;
  m_bns_SetWorkParamter = NULL;
 m_bns_StartWork = NULL;
 m_bns_StopWork = NULL;
 m_bns_IsAutoStopRun = NULL;
}
BusinessControlDllLoader::~BusinessControlDllLoader(void)
{
}
//------------------------------------------------------------
//功能：加载需要用到的函数指针
//参数：hModule:模块
//返回：成功与否
//------------------------------------------------------------
bool BusinessControlDllLoader::LoadFunction(HMODULE hModule)
{
 if(NULL == hModule)
  return false;
 /********************************SDK接口函数声明*********************************/
 m_bns_Initialize   =(pbns_Initialize)::GetProcAddress(hModule, "YES_Initialize");
 m_bns_UnInitialize   =(pbns_UnInitialize)::GetProcAddress(hModule, "YES_UnInitialize");
 m_bns_SetWorkParamter  =(pbns_SetWorkParamter)::GetProcAddress(hModule, "YES_SetWorkParamter");
 m_bns_StartWork    =(pbns_StartWork)::GetProcAddress(hModule, "YES_StartWork");
 m_bns_StopWork    =(pbns_StopWork)::GetProcAddress(hModule, "YES_StopWork");
 m_bns_IsAutoStopRun   =(pbns_IsAutoStopRun)::GetProcAddress(hModule,"YES_IsAutoStopRun");
 m_bns_WriteLog    =(pbns_WriteLog)::GetProcAddress(hModule,"YES_WriteLog");
 return true;
}
```



```cpp
//业务基类

#pragma once

class BusniessBase
{
public:
	BusniessBase(void);
	virtual~BusniessBase(void);

	//-------------------------------------------------------------------------------------
	// 功能：初始化
	// 参数：无
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		Initialize() = 0;

	//-------------------------------------------------------------------------------------
	// 功能：反初始化
	// 参数：无
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		UnInitialize() = 0;

	//-------------------------------------------------------------------------------------
	// 功能：设置工作参数
	// 参数：pParameter 工作参数(配置文件名称)
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		SetWorkParamter(char *pParameter,int nParameterLen,int nRervere) = 0;	

	//-------------------------------------------------------------------------------------
	// 功能：开始工作
	// 参数：无
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		StartWork()=0;

	//-------------------------------------------------------------------------------------
	// 功能：停止工作
	// 参数：无
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		StopWork()=0;

	//-------------------------------------------------------------------------------------
	// 功能：程序是否自动退出
	// 参数：无
	// 返回：bool
	//-------------------------------------------------------------------------------------
	virtual bool	IsAutoStopRun()=0;

	//写日志
	virtual void	WriteLog(char *pLogInfo)=0;
};
```

```cpp
#include "stdafx.h"
#include "BusniessBase.h"

BusniessBase::BusniessBase(void)
{
}

BusniessBase::~BusniessBase(void)
{
}
```

```cpp
//业务实现类

#pragma once
#include "busniessbase.h"
#include "BusinessControlDllLoader.h"

class BusniessImplement :public BusniessBase
{
public:
	BusniessImplement(char* pFileName);
	virtual~BusniessImplement(void);

public:
	//-------------------------------------------------------------------------------------
	// 功能：初始化
	// 参数：无
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		Initialize();

	//-------------------------------------------------------------------------------------
	// 功能：反初始化
	// 参数：无
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		UnInitialize();

	//-------------------------------------------------------------------------------------
	// 功能：设置工作参数
	// 参数：pParameter 工作参数
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		SetWorkParamter(char *pParameter,int nParameterLen,int nRervere);	

	//-------------------------------------------------------------------------------------
	// 功能：开始工作
	// 参数：pParameter:侦听参数
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		StartWork();

	//-------------------------------------------------------------------------------------
	// 功能：停止工作
	// 参数：无
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		StopWork();

	//-------------------------------------------------------------------------------------
	// 功能：程序是否自动退出
	// 参数：无
	// 返回：bool
	//-------------------------------------------------------------------------------------
	virtual bool	IsAutoStopRun();

	//写日志
	virtual void	WriteLog(char *pLogInfo);

	//获取dll加载成功与否
	bool			GetDllLoader();
protected:
	bool	m_bDllLoader;						//dll是否加载成功
	BusinessControlDllLoader m_bcDllLoader;		//服务dll
};
```

```cpp
#include "stdafx.h"
#include "BusniessImplement.h"

BusniessImplement::BusniessImplement(char* pFileName)
{
	m_bDllLoader = m_bcDllLoader.Initialize(pFileName);
}

BusniessImplement::~BusniessImplement(void)
{
	m_bcDllLoader.Uninitialize();
}

//-------------------------------------------------------------------------------------
// 功能：初始化
// 参数：无
// 返回：错误代码
//-------------------------------------------------------------------------------------
int		BusniessImplement::Initialize()
{
	if(!m_bDllLoader)
		return 0;

	return m_bcDllLoader.m_bns_Initialize();
}

//-------------------------------------------------------------------------------------
// 功能：反初始化
// 参数：无
// 返回：错误代码
//-------------------------------------------------------------------------------------
int		BusniessImplement::UnInitialize()
{
	if(!m_bDllLoader)
		return 0;

	return m_bcDllLoader.m_bns_UnInitialize();
}

//-------------------------------------------------------------------------------------
// 功能：设置工作参数
// 参数：pParameter 工作参数
// 返回：错误代码
//-------------------------------------------------------------------------------------
int		BusniessImplement::SetWorkParamter(char *pParameter,int nParameterLen,int nRervere)
{
	if(!m_bDllLoader)
		return 0;

	return m_bcDllLoader.m_bns_SetWorkParamter(pParameter,nParameterLen,nRervere);
}

//-------------------------------------------------------------------------------------
// 功能：开始工作
// 参数：pParameter:侦听参数
// 返回：错误代码
//-------------------------------------------------------------------------------------
int		BusniessImplement::StartWork()
{
	if(!m_bDllLoader)
		return 0;

	return m_bcDllLoader.m_bns_StartWork();
}

//-------------------------------------------------------------------------------------
// 功能：停止工作
// 参数：无
// 返回：错误代码
//-------------------------------------------------------------------------------------
int		BusniessImplement::StopWork()
{
	if(!m_bDllLoader)
		return 0;

	return m_bcDllLoader.m_bns_StopWork();
}

//获取dll加载成功与否
bool	BusniessImplement::GetDllLoader()
{
	return m_bDllLoader;
}

//-------------------------------------------------------------------------------------
// 功能：程序是否自动退出
// 参数：无
// 返回：bool
//-------------------------------------------------------------------------------------
bool	BusniessImplement::IsAutoStopRun()
{
	if(!m_bDllLoader)
		return false;

	if(m_bcDllLoader.m_bns_IsAutoStopRun == NULL)
		return false;

	return m_bcDllLoader.m_bns_IsAutoStopRun();
}

//写日志
void	BusniessImplement::WriteLog(char *pLogInfo)
{
	if(!m_bDllLoader)
		return;

	if(m_bcDllLoader.m_bns_WriteLog == NULL)
		return;

	return m_bcDllLoader.m_bns_WriteLog(pLogInfo);
}
```


说明，服务最好不要直接编译执行，虽然是exe，最好的方式是安装。

安装命令（DOS命令）

cd/d %~dp0

CenterServer.exe /install

卸载命令（DOS命令）

cd/d %~dp0

CenterServer.exe /uninstall

大多的时候，你直接运行exe都会报1063错误，就是因为你把服务以控制台的方式运行了。使用管理员方式运行exe就不会报错。

