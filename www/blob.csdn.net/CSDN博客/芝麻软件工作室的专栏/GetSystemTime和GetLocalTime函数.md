
# GetSystemTime和GetLocalTime函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:20:11[seven-soft](https://me.csdn.net/softn)阅读数：262


时间是一个非常重要的信息，比如写LOG时，就需要把时间输出来，跟踪程序是什么时候出错的。或者当你开发一个银行交易系统时，就要记录当前交易的时间，以便后面可以输出报表，打印给信用卡用户。根据不同的需求，可能需要使用不同的时间，目前有UTC和本地时间。UTC是格林威治时间，也就是全球标准时间。本地时间就是相对于UTC而言的，比如中国北京是在东8区，相对于UTC就多了8个小时。一般使用到的时间都是使用本地时间，也就是调用函数GetLocalTime。
函数GetSystemTime和GetLocalTime声明如下：
WINBASEAPI
VOID
WINAPI
GetSystemTime(
__out LPSYSTEMTIME lpSystemTime
);
WINBASEAPI
VOID
WINAPI
GetLocalTime(
__out LPSYSTEMTIME lpSystemTime
);
**lpSystemTime**是获取系统时间的结构。
调用函数的例子如下：
\#001
\#002  //获取系统时间。
\#003  //
\#004  void TestSystem(void)
\#005  {
\#006         //获取系统的UTC时间。
\#007         SYSTEMTIME stUTC;
**\#008        ::GetSystemTime(&stUTC);**
\#009
\#010         //显示时间的间隔。
\#011         const int nBufSize = 256;
\#012         TCHAR chBuf[nBufSize];
\#013         wsprintf(chBuf,_T("UTC: %u/%u/%u %u:%u:%u:%u %d/r/n"),
\#014               stUTC.wYear, stUTC.wMonth, stUTC.wDay,
\#015               stUTC.wHour, stUTC.wMinute, stUTC.wSecond,
\#016               stUTC.wMilliseconds,stUTC.wDayOfWeek);
\#017         OutputDebugString(chBuf);
\#018
\#019
\#020         //获取当地的时间。
\#021         SYSTEMTIME stLocal;
**\#022        ::GetLocalTime(&stLocal);**
\#023
\#024         //显示时间的间隔。
\#025         wsprintf(chBuf,_T("Local: %u/%u/%u %u:%u:%u:%u %d/r/n"),
\#026               stLocal.wYear, stLocal.wMonth, stLocal.wDay,
\#027               stLocal.wHour, stLocal.wMinute, stLocal.wSecond,
\#028               stLocal.wMilliseconds,stLocal.wDayOfWeek);
\#029         OutputDebugString(chBuf);
\#030
\#031  }
\#032
上面两个函数在我测试时输出的结果，如下：
UTC: 2007/11/11 1:53:1:46 0
Local: 2007/11/11 9:53:1:46 0


