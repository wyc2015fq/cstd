# 用 VC++建立 Windows 服务程序 - ljx0305的专栏 - CSDN博客
2013年01月08日 11:36:09[ljx0305](https://me.csdn.net/ljx0305)阅读数：483
个人分类：[C																[C++																[MFC																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/394717)
本文主要介绍了 OpenSCManager、CreateService、OpenService、ControlService、DeleteService、RegisterServiceCtrlHandler、SetServiceStatus、StartServiceCtrlDispatcher等操作服务程序的主要几个API的用法，具体的函数参数大家可以查阅MSDN。
为什么要使用服务应该程序呢？服务程序就像系统的一些服务一样，能够自动地启动，并执行相应的操作；而且因为服务程序的在层次上和一般的应用程序不同，其能够在系统启动时就自动地运行，而不像一般的应用程序那样一定要在登陆后才能运行，这些就是服务的一些好处了，如果你也想你的程序具有这样的功能，那么你就可以建立一个服务应用程序了。下面就跟着我一步一步地教你怎么去创建一个服务应用程序吧。
一、建立 Win32 Application 应用程序（当然你也可以建立其它的应用程序，但服务一般是没有用户界面的），并命名为 ServiceTest。 
![](https://img-my.csdn.net/uploads/201301/08/1357616479_2268.gif)
二、定义全局函数变量。这里主要是设置服务句柄和状态。 
```cpp
BOOL IsInstalled(); 
BOOL Install(); 
BOOL Uninstall(); 
void LogEvent(LPCTSTR pszFormat, ...); 
void WINAPI ServiceMain(); 
void WINAPI ServiceStrl(DWORD dwOpcode); 
 
TCHAR szServiceName[] = _T("ServiceTest"); 
BOOL bInstall; 
SERVICE_STATUS_HANDLE hServiceStatus; 
SERVICE_STATUS status; 
DWORD dwThreadID;
```
三、添加Init初始化函数。 
```cpp
void Init() 
{ 
    hServiceStatus = NULL; 
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    status.dwCurrentState = SERVICE_STOPPED; 
    tatus.dwControlsAccepted = SERVICE_ACCEPT_STOP; 
    status.dwWin32ExitCode = 0; 
    status.dwServiceSpecificExitCode = 0; 
    status.dwCheckPoint = 0; 
    status.dwWaitHint = 0; 
}
```
四、添加安装和删除服务函数。这里主要是用到了四个函数 OpenSCManager 和 CreateService。OpenSCManager 用于打开服务控制管理器；CreateService 用于创建服务；OpenService用于打开已有的服务，返回该服务的句柄；ControlService则用于控制已打开的服务状态，这里是让服务停止后才删除；DeleteService 用于删除指定服务。
```cpp
BOOL Install(); 
{ 
    //这里列出主要的两个函数，其它的可以在代码里找。 
  
    //打开服务控制管理器 
     OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);  
    //创建服务 
    SC_HANDLE hService = ::CreateService( 
        hSCM, szServiceName, szServiceName,
```
```cpp
SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, 
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
        szFilePath, NULL, NULL, _T(""), NULL, NULL); 
  
    ::CloseServiceHandle(hService); 
    ::CloseServiceHandle(hSCM); 
} 
  
  
BOOL Uninstall(); 
{ 
   //这里列出主要的两个函数，其它的可以在代码里找。  
   //打开服务控制管理器 
   OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
   //打开服务 
   OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE); 
   //停止服务 
   ControlService(hService, SERVICE_CONTROL_STOP, &status);   
  //删除服务 
   DeleteService(hService); 
    … 
}
```
五、添加服务主线程函数和控制函数。这里调用 RegisterServiceCtrlHandler 来注册服务的控制函数，这里要设置status.dwControlsAccepted 为 SERVICE_ACCEPT_STOP，否则你不能控制这个服务的状态。
``
```cpp
void WINAPI ServiceMain() 
{ 
    // Register the control request handler 
    status.dwCurrentState = SERVICE_START_PENDING; 
    status.dwControlsAccepted = SERVICE_ACCEPT_STOP;//这个要使用，否则你不能控制 
 
    //注册服务控制 
    hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl); 
    if (hServiceStatus == NULL) 
    { 
        LogEvent(_T("Handler not installed")); 
        return; 
    } 
    SetServiceStatus(hServiceStatus, &status); 
  
    status.dwWin32ExitCode = S_OK; 
    status.dwCheckPoint = 0; 
    status.dwWaitHint = 0; 
    status.dwCurrentState = SERVICE_RUNNING; 
    SetServiceStatus(hServiceStatus, &status);  
  
    //模拟服务的运行，10后自动退出。应用时将主要任务放于此即可 
    int i = 0; 
    while (i < 10) 
    { 
           Sleep(1000); 
           i++; 
    } 
    // 
  
    status.dwCurrentState = SERVICE_STOPPED; 
    SetServiceStatus(hServiceStatus, &status); 
    LogEvent(_T("Service stopped")); 
}
```
六、在主线程函数里注册控制函数和程序执行主体。这里主要是说明这就是程序的执行体。 
```cpp
void WINAPI ServiceMain() 
{ 
    … 
  
    //模拟服务的运行，10后自动退出。应用时将主要任务放于此即可 
    int i = 0; 
   while (i < 10) 
    { 
           Sleep(1000); 
  
           i++; 
    } 
  
    … 
}
```
七、最后，要在main函数里注册添加安装、删除、注册主函数。 
```cpp
int APIENTRY WinMain(HINSTANCE hInstance, 
                     HINSTANCE hPrevInstance, 
                     LPSTR     lpCmdLine, 
                     int       nCmdShow) 
{ 
    Init(); 
    dwThreadID = ::GetCurrentThreadId(); 
    SERVICE_TABLE_ENTRY st[] = 
    {
```
```cpp
{ szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain }, 
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
    else
    { 
        if (!::StartServiceCtrlDispatcher(st)) 
        { 
            LogEvent(_T("Register Service Main Function Error!")); 
        } 
    } 
    return 0; 
}
```
八、总结。其实做一个服务程序并不难，主要是懂得程序的执行体放于哪里？和注册程序的主函数和注册控制函数，如果这两个没有注册的话，你就程序就不知道如何去控制了。status.dwControlsAccepted = SERVICE_ACCEPT_STOP;这个也重要，如果你没有设置的话，那么服务就不会受你控制了。
引用:[http://www.vckbase.com/index.php/wv/1391](http://www.vckbase.com/index.php/wv/1391)
