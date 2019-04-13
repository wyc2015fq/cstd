
# GetSystemInfo函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:29:07[seven-soft](https://me.csdn.net/softn)阅读数：236标签：[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[x86																](https://so.csdn.net/so/search/s.do?q=x86&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=system&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=system&t=blog)


有一次，我正在开发一个视频压缩程序，而压缩算法是需要非常高效的，也就是需要使用到CPU的多媒体指令。在X86的领域里，目前主要有两家CPU，就是INTEL和AMD。它们的多媒体指令是不一样的。为了区分这种不同的指令，就需要调用函数GetSystemInfo来获取CPU的信息，然后再调用不同的动态连接库来进行多媒体数据压缩。
函数GetSystemInfo声明如下：
WINBASEAPI
VOID
WINAPI
GetSystemInfo(
__out LPSYSTEM_INFO lpSystemInfo
);
**lpSystemInfo**是返回硬件信息的结构。
调用函数的例子如下：
\#001 //
\#002  //获取当前系统的硬件信息。
\#003  //
\#004  void GetHardInfo(void)
\#005  {
\#006         //
\#007         SYSTEM_INFO sysInfo;
\#008
\#009         //获取系统的信息。
\#010         ::GetSystemInfo(&sysInfo);
\#011
\#012         //显示当前系统的信息。
\#013         //
\#014         const int nBufSize = 512;
\#015         TCHAR chBuf[nBufSize];
\#016         ZeroMemory(chBuf,nBufSize);
\#017
\#018         wsprintf(chBuf,_T("OEM ID: %u/n"),sysInfo.dwOemId);
\#019         OutputDebugString(chBuf);
\#020
\#021         wsprintf(chBuf,_T("CPU个数: %u/n"),sysInfo.dwNumberOfProcessors);
\#022         OutputDebugString(chBuf);
\#023
\#024         wsprintf(chBuf,_T("内存分页大小: %u/n"),sysInfo.dwPageSize);
\#025         OutputDebugString(chBuf);
\#026
\#027         wsprintf(chBuf,_T("CPU类型: %u/n"),sysInfo.dwProcessorType);
\#028         OutputDebugString(chBuf);
\#029
\#030         wsprintf(chBuf,_T("CPU架构: %u/n"),sysInfo.wProcessorArchitecture);
\#031         OutputDebugString(chBuf);
\#032
\#033         wsprintf(chBuf,_T("CPU的级别: %u/n"),sysInfo.wProcessorLevel);
\#034         OutputDebugString(chBuf);
\#035
\#036         wsprintf(chBuf,_T("CPU的版本: %u/n"),sysInfo.wProcessorRevision);
\#037         OutputDebugString(chBuf);
\#038
\#039  }
\#040


