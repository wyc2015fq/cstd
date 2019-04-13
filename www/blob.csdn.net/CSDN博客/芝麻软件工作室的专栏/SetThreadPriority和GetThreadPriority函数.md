
# SetThreadPriority和GetThreadPriority函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 10:54:39[seven-soft](https://me.csdn.net/softn)阅读数：282标签：[thread																](https://so.csdn.net/so/search/s.do?q=thread&t=blog)[任务																](https://so.csdn.net/so/search/s.do?q=任务&t=blog)[音乐																](https://so.csdn.net/so/search/s.do?q=音乐&t=blog)[网络																](https://so.csdn.net/so/search/s.do?q=网络&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=网络&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=音乐&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=音乐&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=任务&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=任务&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=thread&t=blog)

Windows是抢先式执行任务的操作系统，无论进程还是线程都具有优先级的选择执行方式，这样就可以让用户更加方便处理多任务。比如当你一边听着音乐，一边上网时，这时就可以把音乐的任务执行级别高一点，这样不让音乐听起来断断续续。当你编写网络程序时，一个线程从网络接收数据，一个线程写数据到硬盘，这时也可以把网络接收线程的优先级设置高一点，因为可以把接收到的数据写到内存里去，然后缓存起来再写到硬盘里。还有一种任务，当你写的程序需要在空闲时才去执行，这时就需设置线程的优先级。这样就使用到SetThreadPriority和GetThreadPriority函数来设置线程的优先级和获取线程的优先级。
函数SetThreadPriority和GetThreadPriority声明如下：
BOOL
WINAPI
SetThreadPriority(
__in HANDLE hThread,
__in int nPriority
);
**hThread**是线程的句柄。
**nPriority**是线程的优先级。

WINBASEAPI
int
WINAPI
GetThreadPriority(
__in HANDLE hThread
);
**hThread**是线程的句柄。
**返回值**是线程的优先级。
调用函数的例子如下：
\#001 //设置线程的优先级和获取线程的优先级。
\#002  //
\#003  void ThreadPriority(void)
\#004  {
\#005         //
**\#006        ::SetThreadPriority(m_hThread,THREAD_PRIORITY_HIGHEST);**
\#007
\#008         //
**\#009        if (::GetThreadPriority(m_hThread) == THREAD_PRIORITY_HIGHEST)**
\#010         {
\#011               //
\#012               OutputDebugString(_T("THREAD_PRIORITY_HIGHEST/r/n"));
\#013         }
\#014
\#015  }


