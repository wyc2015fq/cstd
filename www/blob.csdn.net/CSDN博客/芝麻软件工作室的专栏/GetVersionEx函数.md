
# GetVersionEx函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:30:00[seven-soft](https://me.csdn.net/softn)阅读数：235


Windows发展还是非常快速的，从Win95，到Win98，再到Win2000和XP系统。每个系统的功能也有所不同，要想在程序里区别不同的系统，就需要获取系统的版本信息。比如XP里有防火墙，而其它以前的系统里没有带有的。但有时编写了一个服务器程序，或者编写BT程序，又需要设置一个端口对外面接收连接，因此在XP系统里就需要设置防火墙的端口，而在XP以前的系统里就没有必要设置端口了。这样的需求，就可以使用函数GetVersionEx来区分不同的系统。
函数GetVersionEx声明如下：
WINBASEAPI
BOOL
WINAPI
GetVersionExA(
__inout LPOSVERSIONINFOA lpVersionInformation
);
WINBASEAPI
BOOL
WINAPI
GetVersionExW(
__inout LPOSVERSIONINFOW lpVersionInformation
);
\#ifdef UNICODE
\#define GetVersionEx GetVersionExW
\#else
\#define GetVersionEx GetVersionExA
\#endif // !UNICODE
**lpVersionInformation**是返回系统版本的信息。

调用函数的例子如下：
\#001 //
\#002  //获取当前登录用户的名称。
\#003  //
\#004  void GetWinVersion(void)
\#005  {
\#006         //
\#007         OSVERSIONINFO osvi;
\#008         osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
\#009
\#010         //获取系统的版本信息。
\#011         ::GetVersionEx(&osvi);
\#012         bool bIsWindowsXPorLater = (osvi.dwMajorVersion > 5) ||
\#013               ( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1) );
\#014
\#015         //显示当前的版本。
\#016         if (bIsWindowsXPorLater)
\#017         {
\#018               OutputDebugString(_T("Windows XP或更新版本!/r/n"));
\#019         }
\#020         else
\#021         {
\#022               OutputDebugString(_T("Windows XP以前版本!/r/n"));
\#023         }
\#024  }


