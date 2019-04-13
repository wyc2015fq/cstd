
# CreateProcess函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 11:22:11[seven-soft](https://me.csdn.net/softn)阅读数：238标签：[attributes																](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[报表																](https://so.csdn.net/so/search/s.do?q=报表&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)


人们需要处理的信息越来越复杂，往往在一个应用程序里是处理不完的，因此，就出现多个应用程序协同处理同一件事情。当然多个应用程序分开处理，也是比较容易开发，并且让应用程序复杂难度迅速降低。比如在开发一个银行的交易系统，有一个报表生成的主程序，然后还有很多小的，不同的报表生成程序。这样就需要从主程序里创建小报表程序进行运行。创建进程运行，需要使用函数CreateProcess来实现。
函数CreateProcess声明如下：

WINBASEAPI
BOOL
WINAPI
CreateProcessA(
__in_opt    LPCSTR lpApplicationName,
__inout_opt LPSTR lpCommandLine,
__in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes,
__in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes,
__in        BOOL bInheritHandles,
__in        DWORD dwCreationFlags,
__in_opt    LPVOID lpEnvironment,
__in_opt    LPCSTR lpCurrentDirectory,
__in        LPSTARTUPINFOA lpStartupInfo,
__out       LPPROCESS_INFORMATION lpProcessInformation
);
WINBASEAPI
BOOL
WINAPI
CreateProcessW(
__in_opt    LPCWSTR lpApplicationName,
__inout_opt LPWSTR lpCommandLine,
__in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes,
__in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes,
__in        BOOL bInheritHandles,
__in        DWORD dwCreationFlags,
__in_opt    LPVOID lpEnvironment,
__in_opt    LPCWSTR lpCurrentDirectory,
__in        LPSTARTUPINFOW lpStartupInfo,
__out       LPPROCESS_INFORMATION lpProcessInformation
);
\#ifdef UNICODE
\#define CreateProcess CreateProcessW
\#else
\#define CreateProcess CreateProcessA
\#endif // !UNICODE
**lpApplicationName**是应用程序的名称。
**lpCommandLine**是命令行参数。
**lpProcessAttributes**是进程的属性。
**lpThreadAttributes**是线程的属性。
**bInheritHandles**是否继承父进程的属性。
**dwCreationFlags**是创建标志。
**lpEnvironment**是环境变量。
**lpCurrentDirectory**是当前目录。
**lpStartupInfo**是传给新进程的信息。
**lpProcessInformation**是进程返回的信息。
调用函数的例子如下：
\#001 //创建进程。
\#002  //
\#003  void TestCreateProcess(void)
\#004  {
\#005         //清空结构。
\#006         STARTUPINFO sInfo;
\#007         PROCESS_INFORMATION pInfo;
\#008
\#009         ZeroMemory( &sInfo, sizeof(sInfo) );
\#010         sInfo.cb = sizeof(sInfo);
\#011         sInfo.dwFlags = STARTF_USESHOWWINDOW;
\#012         sInfo.wShowWindow = SW_SHOWNORMAL;
\#013
\#014         ZeroMemory( &pInfo, sizeof(pInfo) );
\#015
\#016         //创建一个进程。
**\#017        if( !::CreateProcess( _T("WinCpp.exe"),**
**\#018              NULL,**
**\#019              NULL,**
**\#020              NULL,**
**\#021              FALSE,**
**\#022              0,**
**\#023              NULL,**
**\#024              NULL,**
**\#025              &sInfo,**
**\#026              &pInfo )**
\#027               )
\#028         {
\#029               //输出出错信息。
\#030               const int nBufSize = 512;
\#031               TCHAR chBuf[nBufSize];
\#032               ZeroMemory(chBuf,nBufSize);
\#033
\#034               wsprintf(chBuf,_T("CreateProcess failed (%d)./n"), GetLastError() );
\#035               OutputDebugString(chBuf);
\#036               return;
\#037         }
\#038
\#039
\#040         //等进程关闭。
\#041         WaitForSingleObject( pInfo.hProcess, INFINITE );
\#042
\#043         //关闭进程和线程的句柄。
\#044         CloseHandle( pInfo.hProcess );
\#045         CloseHandle( pInfo.hThread );
\#046
\#047  }


