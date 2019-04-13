
# GetUserName函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:30:57[seven-soft](https://me.csdn.net/softn)阅读数：206标签：[磁盘																](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



随着系统安全性的加强，每个系统里越来越多不同的帐号登录。假如你正在开发一个上网安全软件，让不同的用户有不同的上网权限，这样就需要识别当前的用户是什么帐号登录了，然后再作出权限分配。还有出错时，也需要对当前帐号进行记录下来，因为不同的帐号有不同的权限，有些磁盘是不允许操作的。面对这些需求，就需要使用函数GetUserName。
函数GetUserName声明如下：
WINADVAPI
BOOL
WINAPI
GetUserNameA (
__out_ecount_part(*pcbBuffer, *pcbBuffer) LPSTR lpBuffer,
__inout LPDWORD pcbBuffer
);
WINADVAPI
BOOL
WINAPI
GetUserNameW (
__out_ecount_part(*pcbBuffer, *pcbBuffer) LPWSTR lpBuffer,
__inout LPDWORD pcbBuffer
);
\#ifdef UNICODE
\#define GetUserName GetUserNameW
\#else
\#define GetUserName GetUserNameA
\#endif // !UNICODE
**lpBuffer**是获取名称缓冲区。
**pcbBuffer**是缓冲区的大小和返回帐号的大小。
调用函数的例子如下：
\#001 //
\#002  //获取当前登录用户的名称。
\#003  //
\#004  void GetUserName(void)
\#005  {
\#006         //
\#007         const int nBufSize = UNLEN + 1;
\#008         TCHAR chBuf[nBufSize];
\#009         ZeroMemory(chBuf,nBufSize);
\#010
\#011         //获取当前登录用户的名称
\#012         DWORD dwRet = nBufSize;
**\#013        if (::GetUserName(chBuf,&dwRet))**
\#014         {
\#015               //
\#016               OutputDebugString(chBuf);
\#017         }
\#018         else
\#019         {
\#020               OutputDebugString(_T("获取登录用户名称出错!"));
\#021         }
\#022
\#023         OutputDebugString(_T("/r/n"));
\#024
\#025  }


