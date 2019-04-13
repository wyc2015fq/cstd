
# GlobalMemoryStatusEx函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:24:27[seven-soft](https://me.csdn.net/softn)阅读数：257个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



在开发软件的过程中，经常会碰到不同用户的PC系统配置不一样。比如有些用户的系统内存配置比较差，这样处理大量数据时，就不能把大量的数据读取到内存里处理了。而又有一些用户的内存比较多，或者是机器比较新，那么就可以加载大量的数据到内存里处理，这样可以随着系统的更新，软件的处理能力能大幅地提高性能。这样就需要了解系统的配置信息了，最重要的资源之一内存，就是最需要了解的，需要调用函数GlobalMemoryStatusEx来了解内存的分配情况。

函数GlobalMemoryStatusEx声明如下：
WINBASEAPI
BOOL
WINAPI
GlobalMemoryStatusEx(
__out LPMEMORYSTATUSEX lpBuffer
);
**lpBuffer**是接收内存信息的结构。
调用函数的例子如下：
\#001  //
\#002  //当前系统内存信息。
\#003  //
\#004  void MemInfo(void)
\#005  {
\#006         //获取内存信息。
\#007         MEMORYSTATUSEX memStatex;
\#008          memStatex.dwLength = sizeof(memStatex);
\#009
**\#010        if (GlobalMemoryStatusEx(&memStatex))**
\#011         {
\#012               //
\#013               const int nBufSize = 512;
\#014               TCHAR chBuf[nBufSize];
\#015               ZeroMemory(chBuf,nBufSize);
\#016
\#017               //内存使用率。
\#018               wsprintf(chBuf,_T("内存使用率: %u%%/n"),memStatex.dwMemoryLoad);
\#019               OutputDebugString(chBuf);
\#020
\#021               //总共物理内存。
\#022               wsprintf(chBuf,_T("总共物理内存: %u/n"),memStatex.ullTotalPhys );
\#023               OutputDebugString(chBuf);
\#024
\#025               //可用物理内存。
\#026               wsprintf(chBuf,_T("可用物理内存: %u/n"),memStatex.ullAvailPhys );
\#027               OutputDebugString(chBuf);
\#028
\#029               //全部内存。
\#030               wsprintf(chBuf,_T("全部内存: %u/n"),memStatex.ullTotalPageFile );
\#031               OutputDebugString(chBuf);
\#032
\#033               //全部可用的内存。
\#034               wsprintf(chBuf,_T("全部可用的内存: %u/n"),memStatex.ullAvailPageFile);
\#035               OutputDebugString(chBuf);
\#036
\#037               //全部的虚拟内存。
\#038               wsprintf(chBuf,_T("全部的内存: %u/n"),memStatex.ullTotalVirtual);
\#039               OutputDebugString(chBuf);
\#040
\#041         }
\#042  }


