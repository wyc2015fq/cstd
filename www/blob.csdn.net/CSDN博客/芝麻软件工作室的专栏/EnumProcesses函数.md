
# EnumProcesses函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 11:18:55[seven-soft](https://me.csdn.net/softn)阅读数：205标签：[email																](https://so.csdn.net/so/search/s.do?q=email&t=blog)[报表																](https://so.csdn.net/so/search/s.do?q=报表&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=email&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



当你开发的软件在用户那里运行出错了，想怎么办呢？当然是希望把出错时候的运行环境信息生成报表，然后再Email回来查看了。这里就介绍一个函数可以把当时运行环境的进程全部找到，然后可以输出每个进程的信息。当然，这个函数也可以使用到杀病毒软件里，用来查看可疑的进程信息。
函数EnumProcesses声明如下：
BOOL
WINAPI
EnumProcesses (
DWORD * lpidProcess,
DWORD   cb,
DWORD * cbNeeded
);
**lpidProcess**是保存进程ID的数组。
**cb**是进程组数的大小。
**cbNeeded**是返回进程数组的大小。
调用函数的例子如下：
\#001 //获取系统所有进程。
\#002  //
\#003  void TestEnumProcesses(void)
\#004  {
\#005         //
\#006         const int nBufSize = 512;
\#007         TCHAR chBuf[nBufSize];
\#008         ZeroMemory(chBuf,nBufSize);
\#009
\#010         //
\#011         DWORD dwProcs[1024*2];
\#012         DWORD dwNeeded;
\#013
\#014         //枚举所有进程ID。
**\#015        if ( !EnumProcesses( dwProcs, sizeof(dwProcs), &dwNeeded ) )**
\#016         {
\#017               //输出出错信息。
\#018               wsprintf(chBuf,_T("EnumProcesses failed (%d)./n"), GetLastError() );
\#019               OutputDebugString(chBuf);
\#020
\#021               return;
\#022         }
\#023
\#024         //计算有多少个进程ID。
\#025         DWORD dwProcCount = dwNeeded / sizeof(DWORD);
\#026
\#027         wsprintf(chBuf,_T("EnumProcesses Count(%d)./n"), dwProcCount );
\#028         OutputDebugString(chBuf);
\#029
\#030  }
\#031


