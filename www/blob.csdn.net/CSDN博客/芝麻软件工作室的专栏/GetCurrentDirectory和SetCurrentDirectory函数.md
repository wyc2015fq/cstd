
# GetCurrentDirectory和SetCurrentDirectory函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:41:37[seven-soft](https://me.csdn.net/softn)阅读数：326


在开发软件里，常常碰到要读取当前目录下的配置参数文件，或者打开当前目录下别的程序来运行，那么就需要获取当前进程的目录位置，这就需要使用函数GetCurrentDirectory获取当前进程所有在的目录。同时也可以使用SetCurrentDirectory函数来改变进程的当前目录。
函数GetCurrentDirectory和SetCurrentDirectory声明如下：
WINBASEAPI
DWORD
WINAPI
GetCurrentDirectoryA(
__in DWORD nBufferLength,
__out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer
);
WINBASEAPI
DWORD
WINAPI
GetCurrentDirectoryW(
__in DWORD nBufferLength,
__out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer
);
\#ifdef UNICODE
\#define GetCurrentDirectory GetCurrentDirectoryW
\#else
\#define GetCurrentDirectory GetCurrentDirectoryA
\#endif // !UNICODE
WINBASEAPI
BOOL
WINAPI
SetCurrentDirectoryA(
__in LPCSTR lpPathName
);
WINBASEAPI
BOOL
WINAPI
SetCurrentDirectoryW(
__in LPCWSTR lpPathName
);
\#ifdef UNICODE
\#define SetCurrentDirectory SetCurrentDirectoryW
\#else
\#define SetCurrentDirectory SetCurrentDirectoryA
\#endif // !UNICODE
**nBufferLength**是缓冲区的大小。
**lpBuffer**是接收目录的缓冲区指针。
**lpPathName**是设置的目录。
调用函数的例子如下：
\#001  //获取或者改变当前目录路径。
\#002  //
\#003  void GetCurDir(void)
\#004  {
\#005         //
\#006         TCHAR szBuf[MAX_PATH];
\#007         ZeroMemory(szBuf,MAX_PATH);
\#008         if (GetCurrentDirectory(MAX_PATH,szBuf) > 0)
\#009         {
\#010               //获取进程目录成功。
\#011               OutputDebugString(szBuf);
\#012         }
\#013         else
\#014         {
\#015               //改变当前目录位置。
\#016               SetCurrentDirectory(_T("C://"));
\#017         }
\#018
\#019         OutputDebugString(_T("/r/n"));
\#020  }
\#021


