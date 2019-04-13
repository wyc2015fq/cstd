
# GetComputerName函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:32:16[seven-soft](https://me.csdn.net/softn)阅读数：208标签：[网络																](https://so.csdn.net/so/search/s.do?q=网络&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



当你在一个大公司里面，当一个网络管理员时，发现成千上万个电脑需要你去管理时，怎么去区分这些电脑呢？那肯定是通过计算机的名称。而这个网络管理员又需要你开发一套软件，它可以把所有电脑的名称自动地上报给他。面对这样的需求，就可以使用下面的函数GetComputerName来获取计算机的名称，并通过网络传送给管理员。
函数GetComputerName声明如下：
WINBASEAPI
BOOL
WINAPI
GetComputerNameA (
__out_ecount_part(*nSize, *nSize + 1) LPSTR lpBuffer,
__inout LPDWORD nSize
);
WINBASEAPI
BOOL
WINAPI
GetComputerNameW (
__out_ecount_part(*nSize, *nSize + 1) LPWSTR lpBuffer,
__inout LPDWORD nSize
);
\#ifdef UNICODE
\#define GetComputerName GetComputerNameW
\#else
\#define GetComputerName GetComputerNameA
\#endif // !UNICODE
**lpBuffer**是获取电脑名称的缓冲区。
**nSize**是输入缓冲区的大小和输出电脑名称的大小。
调用函数的例子如下：
\#001 //
\#002  //获取当前计算机的名称。
\#003  //
\#004  void GetPCName(void)
\#005  {
\#006         //
\#007         const int nBufSize = MAX_COMPUTERNAME_LENGTH + 1;
\#008         TCHAR chBuf[nBufSize];
\#009         ZeroMemory(chBuf,nBufSize);
\#010
\#011         //获取当前计算机的名称
\#012         DWORD dwRet = nBufSize;
**\#013        if (GetComputerName(chBuf,&dwRet))**
\#014         {
\#015               //
\#016               OutputDebugString(chBuf);
\#017         }
\#018         else
\#019         {
\#020               OutputDebugString(_T("获取计算名称出错!"));
\#021         }
\#022
\#023         OutputDebugString(_T("/r/n"));
\#024
\#025  }
\#026
\#027


