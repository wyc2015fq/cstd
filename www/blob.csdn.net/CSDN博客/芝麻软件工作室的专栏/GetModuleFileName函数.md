
# GetModuleFileName函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:20:53[seven-soft](https://me.csdn.net/softn)阅读数：209标签：[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



在开发软件的过程里，经常需要把数据保存到当前执行文件路径下面，或者读取当前执行文件路径下的一些配置信息。这时就需要从当前模块里获取所在的目录路径，以便进行固定的位置操作文件。要解决这个需求，就需要调用API函数GetModuleFileName来获取模块所在的路径。
函数GetModuleFileName声明如下：
WINBASEAPI
DWORD
WINAPI
GetModuleFileNameA(
__in_opt HMODULE hModule,
__out_ecount_part(nSize, return + 1) LPCH lpFilename,
__in     DWORD nSize
);
WINBASEAPI
DWORD
WINAPI
GetModuleFileNameW(
__in_opt HMODULE hModule,
__out_ecount_part(nSize, return + 1) LPWCH lpFilename,
__in     DWORD nSize
);
\#ifdef UNICODE
\#define GetModuleFileName GetModuleFileNameW
\#else
\#define GetModuleFileName GetModuleFileNameA
\#endif // !UNICODE
**hModule**是模块的句柄，或者设置为NULL表示当前模块。
**lpFilename**是保存路径的缓冲区。
**nSize**是缓冲区的大小。
调用函数的例子如下：
\#001 //获取当前程序所在路径。
\#002  //
\#003  void TestGetExePath(void)
\#004  {
\#005         //
\#006         const int nBufSize = 512;
\#007         TCHAR chBuf[nBufSize];
\#008         ZeroMemory(chBuf,nBufSize);
\#009
\#010         //获取当前执行文件的路径。
**\#011        if (GetModuleFileName(NULL,chBuf,nBufSize))**
\#012         {
\#013               //输出带文件名称路径。
\#014               OutputDebugString(chBuf);
\#015               OutputDebugString(_T("/r/n"));
\#016
\#017               //获取文件路径。
\#018               TCHAR* lpStrPath = chBuf;
\#019               PathRemoveFileSpec(lpStrPath);
\#020               OutputDebugString(lpStrPath);
\#021               OutputDebugString(_T("/r/n"));
\#022         }
\#023
\#024  }
输出的结果如下：
g:/work/windows_api/wincpp2/debug/WinCpp.exe
g:/work/windows_api/wincpp2/debug


