# Windows下对进程的一些操作 - yixianfeng41的专栏 - CSDN博客
2016年08月09日 13:14:44[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1749标签：[windows下c++打开指定exe进程																[windows下c++关闭指定进程																[windows判断进程是否在运行](https://so.csdn.net/so/search/s.do?q=windows判断进程是否在运行&t=blog)](https://so.csdn.net/so/search/s.do?q=windows下c++关闭指定进程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows下c++打开指定exe进程&t=blog)
个人分类：[Windows](https://blog.csdn.net/yixianfeng41/article/category/6324100)
## 一、Woindows Api介绍
### 1、STRTUPINFO介绍
这是用于指定新进程主窗口特性的一个结构。
```cpp
typedef struct _STARTUPINFO {
    DWORD  cb;   //应用程序必须将cb初始化为sizeof(STARTUPINFO)
    LPTSTR lpReserved;  //保留,必须初始化为NULL
    LPTSTR lpDesktop;   //用于标识启动应用程序所在的桌面的名字.
    LPTSTR lpTitle;  //用于设定控制台窗口的名称。
    //如果dwFlags是STARTF_USEPOSITION,接下来四个属性分别是程序窗口位置和大小
    DWORD  dwX; 
    DWORD  dwY;
    DWORD  dwXSize;
    DWORD  dwYSize;
    //如果dwFlags是 STARTF_USECOUNTCHARS，用于设置子控制台窗口的宽和高度
    DWORD  dwXCountChars;
    DWORD  dwYCountChars;
    //dwFlags是 STARTF_USEFILLATTRIBUTE时，用于设置文本和背景颜色
    DWORD  dwFillAttribute;  
    DWORD  dwFlags;
    WORD   wShowWindow;
    WORD   cbReserved2;  //Reserved for use by the C Run-time; must be zero.
    LPBYTE lpReserved2; //Reserved for use by the C Run-time; must be NULL.
    //If dwFlags specifies STARTF_USESTDHANDLES, this member is the standard input handle for the process.
    //If STARTF_USESTDHANDLES is not specified, the default for standard input is the keyboard buffer.
    HANDLE hStdInput;
    //If dwFlags specifies STARTF_USESTDHANDLES, this member is the standard output handle for the process. 
    //Otherwise, this member is ignored and the default for standard output is the console window's buffer.
    HANDLE hStdOutput;
    // If dwFlags specifies STARTF_USESTDHANDLES, this member is the standard error handle for the process.
    //Otherwise, this member is ignored and the default for standard error is the console window's buffer.
    HANDLE hStdError;
} STARTUPINFO, *LPSTARTUPINFO;
```
### 2、PROCESS_INFORMATION
该结构返回有关新进程及其主线程的信息。
```cpp
typedef struct _PROCESS_INFORMATION { // pi 
    HANDLE hProcess;  //返回新进程句柄
    HANDLE hThread;   //返回主线程句柄
    DWORD dwProcessId; //返回一个全局进程标识符。该标识符用于标识一个进程。从进程被创建到终止，该值始终有效。
    DWORD dwThreadId; //：返回一个全局线程标识符。该标识符用于标识一个线程。从线程被创建到终止，该值始终有效。
} PROCESS_INFORMATION;
```
### 3、CreateProcess
WIN32API函数CreateProcess用来创建一个新的进程和它的主线程，这个新进程运行指定的可执行文件。
```cpp
BOOL WINAPI CreateProcess(
    _In_opt_    LPCTSTR               lpApplicationName,  
    _Inout_opt_ LPTSTR                lpCommandLine,  //参数指定的字符串当做要运行的程序的命令行。
    _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes, //指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承
    _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,  //同lpProcessAttribute,不过这个参数决定的是线程是否被继承.通常置为NULL.
    _In_        BOOL                  bInheritHandles, //指示新进程是否从调用进程处继承了句柄。
    _In_        DWORD                 dwCreationFlags, //指定附加的、用来控制优先类和进程的创建的标志。
    _In_opt_    LPVOID                lpEnvironment,  //指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境
    _In_opt_    LPCTSTR               lpCurrentDirectory, //这个字符串用来指定子进程的工作路径。这个字符串必须是一个包含驱动器名的绝对路径。如果这个参数为空，新进程将使用与调用进程相同的驱动器和目录。
    _In_        LPSTARTUPINFO         lpStartupInfo, //指向一个用于决定新进程的主窗体如何显示的STARTUPINFO结构体。
    _Out_       LPPROCESS_INFORMATION lpProcessInformation //指向一个用来接收新进程的识别信息的PROCESS_INFORMATION结构体。
    );
```
### 4、EnumProcesses
检索进程中的每一个进程标识符.
```cpp
BOOL WINAPI EnumProcesses(
    _Out_ DWORD *pProcessIds, //接收进程标识符的数组
    _In_  DWORD cb, //数组的大小
    _Out_ DWORD *pBytesReturned //数组返回的字节数
    );
```
### 5、EnumProcessModules
该函数枚举获取一个进程的所有模块。
```cpp
BOOL WINAPI EnumProcessModules(
    _In_  HANDLE  hProcess,  //要枚举的一个进程的句柄 
    _Out_ HMODULE *lphModule, //该进程所包含的模块句柄的数组，我们可以定义一个数组来接受该进程所含有的模块句柄 
    _In_  DWORD   cb,  //这个进程大概有多少个模块的数量
    _Out_ LPDWORD lpcbNeeded  //该进程实际的模块的数量
    );
```
### 6、GetModuleBaseName
获取进程模块名字。
```cpp
DWORD WINAPI GetModuleBaseName(
    __in     HANDLE hProcess,  //进程句柄
    __in_opt HMODULE hModule,  //进程模块的句柄
    __out    LPTSTR lpBaseName,  //进程名字
    __in     DWORD nSize  //名字大小
    );
```
## 二、封装的一些操作方法
### 1、判断进程是否在运行
先枚举所有正在运行的进程，然后看指定进程是否在其中，来判断进程是否在运行。
```cpp
bool isRunning( const TCHAR* dstProcessName )
{
    TCHAR chBuf[MAX_BUF_SIZE];
    ZeroMemory( chBuf , MAX_BUF_SIZE );
    DWORD dwProcs[1024*2];
    DWORD dwNeeded;
    if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
    {
        //输出出错信息
        wsprintf( chBuf , TEXT("EnumProcesses Failed (%d).\n") , GetLastError() ) ;
        OutputDebugString(chBuf);
        return true; // 假定进程存在，防止意外重启
    }
    //计算有多少个进程ID
    DWORD dwProcCount =  dwNeeded / sizeof( DWORD );
    HMODULE hMod;
    DWORD arraySize;
    TCHAR processName[512];
    for( int i=0; i<dwProcCount ; i++ )
    {
        DWORD m_processid = dwProcs[i];
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
        if(hProcess)
        {
            if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
            {
                GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
                if( _tcscmp(processName , dstProcessName )==0 )
                {
                    return true;
                }
            }
        }
    }
    return false;
}
```
### 2、运行指定exe进程
```cpp
//此方法用于将数据格式抓环为LLPWSTR
LPWSTR ConvertCharToLPWSTR(const char * szString)
{
    int dwLen = strlen(szString) + 1;
    int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);//算出合适的长度
    LPWSTR lpszPath = new WCHAR[dwLen];
    MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
    return lpszPath;
}
bool openExe( const string& name , string dir , string param) //指定目录Dir下的exe或其他文件name,命令行参数param
{
    STARTUPINFO si; 
    PROCESS_INFORMATION pi;  
    ZeroMemory(&si, sizeof(si));   
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    ZeroMemory( &pi, sizeof(pi) );
    std::string cmd = dir + name + " " + param;
#ifdef UNICODE
    LPWSTR cmdLine =ConvertCharToLPWSTR(cmd.c_str());
#else();
    LPTSTR cmdLine = s2ws(cmd).c_str();
#endif
 
    DWORD flag = HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE; 
    // Start the child process. 
    bool res=CreateProcess( NULL,   // No module name (use command line)
        cmdLine,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        flag,           // creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi );           // Pointer to PROCESS_INFORMATION structure
    return res;
}
```
### 3、停止某进程
跟判断某进程是否在运行原理一样，只是找到进程后，执行一个停止进程的方法。
```cpp
bool killProcess( const TCHAR* dwProcessName )
{
    TCHAR chBuf[MAX_BUF_SIZE];
    ZeroMemory( chBuf , MAX_BUF_SIZE );
    DWORD dwProcs[1024*2];
    DWORD dwNeeded;
    if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
    {
        //输出出错信息
        wsprintf( chBuf , TEXT("EnumProcesses Failed (%d).\n") , GetLastError() ) ;
        OutputDebugString(chBuf);
        return false;
    }
    //计算有多少个进程ID
    DWORD dwProcCount =  dwNeeded / sizeof( DWORD );
    HMODULE hMod;
    DWORD arraySize;
    TCHAR processName[512];
    for( int i=0; i<dwProcCount ; i++ )
    {
        DWORD m_processid = dwProcs[i];
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
        if(hProcess)
        {
            if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
            {
                GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
                if( _tcscmp(processName , dwProcessName )==0 )
                {
                    return ::TerminateProcess( hProcess,4);
                }
            }
        }
    }
    //尝试发送关闭动作
    HWND wnd = ::FindWindow( NULL , dwProcessName );
    ::SendMessage( wnd , WM_CLOSE , 0 , 0 );
    return false;
}
```

