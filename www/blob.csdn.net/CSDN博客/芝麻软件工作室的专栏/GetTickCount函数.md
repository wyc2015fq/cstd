
# GetTickCount函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:21:36[seven-soft](https://me.csdn.net/softn)阅读数：310个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



时间计时，也不是越精确越好，有时只需要有一个计时就行了。这样就可以使用毫秒级别的计时函数GetTickCount。这个函数是记录了系统启动以来的时间毫秒，当超过49.7天，这个值变为从0开始，也就是说49.7天是一个周期。当不同的两次函数调时，就返回两次时间差值。
函数GetTickCount声明如下：
WINBASEAPI
DWORD
WINAPI
GetTickCount(
VOID
);
调用函数的例子如下：
\#001 //一般的时钟计时。
\#002  //
\#003  void TestTickCount(void)
\#004  {
\#005         //获取第一次计时值。
\#006         DWORD dwStart = ::GetTickCount();
\#007         for (int i = 0; i < 10; i++)
\#008         {
\#009               //计算时间间隔。
\#010               DWORD dwInterval = ::GetTickCount() - dwStart;
\#011
\#012               Sleep(100);
\#013
\#014               //显示时间的间隔。
\#015               const int nBufSize = 256;
\#016               TCHAR chBuf[nBufSize];
\#017               wsprintf(chBuf,_T("dwInterval=%d/r/n"),dwInterval);
\#018               OutputDebugString(chBuf);
\#019         }
\#020
\#021  }


