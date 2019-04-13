
# QueryPerformanceCounter函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:22:41[seven-soft](https://me.csdn.net/softn)阅读数：274


精确的时间计时，有时候是非常必要的。比如播放多媒体时视频与音频的时间同步，还有在测试代码的性能时，也需要使用到非常精确的时间计时。还有测试硬件的性能时，也需要精确的时间计时。这时就需要使用QueryPerformanceCounter来查询定时器的计数值，如果硬件里有定时器，它就会启动这个定时器，并且不断获取定时器的值，这样的定时器精度，就跟硬件时钟的晶振一样精确的。
函数QueryPerformanceCounter和QueryPerformanceFrequency声明如下：
WINBASEAPI
BOOL
WINAPI
QueryPerformanceCounter(
__out LARGE_INTEGER *lpPerformanceCount
);
WINBASEAPI
BOOL
WINAPI
QueryPerformanceFrequency(
__out LARGE_INTEGER *lpFrequency
);
**lpPerformanceCount**是返回定时器当前计数值。
**QueryPerformanceFrequency**是返回定时器的频率。
调用函数的例子如下：
\#001 //精确时钟查询。
\#002  //
\#003  void TestHighTimer(void)
\#004  {
\#005         //
\#006         LARGE_INTEGER nFreq;
\#007         LARGE_INTEGER nLastTime1;
\#008         LARGE_INTEGER nLastTime2;
\#009
\#010         //获取是否支持精确定时器。
**\#011        if (QueryPerformanceFrequency(&nFreq))**
\#012         {
\#013               //
\#014               const int nBufSize = 256;
\#015               TCHAR chBuf[nBufSize];
\#016
\#017               //显示定时器的频率。
\#018               wsprintf(chBuf,_T("LastTime=%I64d/r/n"),nFreq);
\#019               OutputDebugString(chBuf);
\#020
\#021               //获取定时器的值。
**\#022              QueryPerformanceCounter(&nLastTime1);**
\#023               wsprintf(chBuf,_T("LastTime=%I64d/r/n"),nLastTime1);
\#024               OutputDebugString(chBuf);
\#025
\#026               Sleep(0);
\#027
\#028               //获取定时器的值。
**\#029              QueryPerformanceCounter(&nLastTime2);**
\#030               wsprintf(chBuf,_T("LastTime=%I64d/r/n"),nLastTime2);
\#031               OutputDebugString(chBuf);
\#032
\#033
\#034               //计算时间是花费多少秒。
\#035               float fInterval = nLastTime2.QuadPart - nLastTime1.QuadPart;
\#036               swprintf(chBuf,nBufSize,_T("花费:%f/r/n"),fInterval/(float)nFreq.QuadPart);
\#037               OutputDebugString(chBuf);
\#038         }
\#039
\#040  }


