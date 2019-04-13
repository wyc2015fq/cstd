
# CreateSemaphore和ReleaseSemaphore函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 10:56:53[seven-soft](https://me.csdn.net/softn)阅读数：565标签：[attributes																](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)[semaphore																](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[object																](https://so.csdn.net/so/search/s.do?q=object&t=blog)[多线程																](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)[通讯																](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=object&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=object&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)
在开发软件的过程中，多线程的程序往往需要实现相互通讯，比如几个线程添加一个消息到队列里，而另一个线程在睡眠时，就需要唤醒那个线程来处理事情。在这其中，就需要使用到信号量来进行同步。CreateSemaphore是创建信号量，ReleaseSemaphore是增加信号量。
函数CreateSemaphore和ReleaseSemaphore声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
CreateSemaphoreA(
__in_opt LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
__in     LONG lInitialCount,
__in     LONG lMaximumCount,
__in_opt LPCSTR lpName
);
WINBASEAPI
__out
HANDLE
WINAPI
CreateSemaphoreW(
__in_opt LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
__in     LONG lInitialCount,
__in     LONG lMaximumCount,
__in_opt LPCWSTR lpName
);
\#ifdef UNICODE
\#define CreateSemaphore CreateSemaphoreW
\#else
\#define CreateSemaphore CreateSemaphoreA
\#endif // !UNICODE
**lpSemaphoreAttributes**是信号量的安全属性。
**lInitialCount**是初始化的信号量。
**lMaximumCount**是允许信号量增加到最大值。
**lpName**是信号量的名称。
WINAPI
ReleaseSemaphore(
__in      HANDLE hSemaphore,
__in      LONG lReleaseCount,
__out_opt LPLONG lpPreviousCount
);
**hSemaphore**是要增加的信号量句柄。
**lReleaseCount**是增加的计数。
**lpPreviousCount**是增加前的数值返回。
调用函数的例子如下：
\#001 //线程运行函数。
\#002 //在这里可以使用类里的成员，也可以让派生类实现更强大的功能。
\#003 //
\#004 DWORD CThreadSemaphore::Run(void)
\#005 {
\#006  //输出到调试窗口。
\#007  ::OutputDebugString(_T("Run()线程函数运行/r/n"));
\#008
\#009  //
\#010  const LONG cMax = 10;
**\#011   m_hSemaphore = CreateSemaphore(**
**\#012        NULL,   //****缺省的安全属性。**
**\#013        0,   //****初始化为****0****个信号量。**
**\#014        cMax,   //****最大为****10****个信号量。**
**\#015        NULL); //****不命名。**
\#016
\#017  if (m_hSemaphore == NULL)
\#018  {
\#019         return -1;
\#020  }
\#021
\#022  //
\#023  const int nMaxObjs = 2;
\#024  HANDLE hWaitObjects[nMaxObjs] = {m_hEventExit,m_hSemaphore};
\#025
\#026  //线程循环。
\#027  for (;;)
\#028  {
\#029         DWORD dwRet = WaitForMultipleObjects(nMaxObjs,hWaitObjects,FALSE,INFINITE);
\#030         if (dwRet == WAIT_TIMEOUT)
\#031         {
\#032               //可以继续运行。
\#033               TCHAR chTemp[128];
\#034               wsprintf(chTemp,_T("CThreadSemaphore::Run() ThreadID=%d/r/n"),m_dwThreadID);
\#035               ::OutputDebugString(chTemp);
\#036
\#037               //目前没有做什么事情，就让线程释放一下CPU。
\#038               Sleep(10);
\#039         }
\#040         else if (dwRet == WAIT_OBJECT_0)
\#041         {
\#042               //退出线程。
\#043               ::OutputDebugString(_T("Run()退出线程/r/n"));
\#044               break;
\#045         }
\#046         else if (dwRet == WAIT_OBJECT_0+1)
\#047         {
\#048               //可以继续运行。
\#049               TCHAR chTemp[128];
\#050               wsprintf(chTemp,_T("CThreadSemaphore::Run() Semaphore,ThreadID=%d/r/n"),m_dwThreadID);
\#051               ::OutputDebugString(chTemp);
\#052
\#053               //
\#054
\#055         }
\#056         else if (dwRet == WAIT_ABANDONED)
\#057         {
\#058               //出错。
\#059               ::OutputDebugString(_T("Run()线程出错/r/n"));
\#060               return -1;
\#061         }
\#062  }
\#063
\#064  //
\#065  if (m_hSemaphore)
\#066  {
\#067         CloseHandle(m_hSemaphore);
\#068          m_hSemaphore = NULL;
\#069  }
\#070
\#071  return 0;
\#072 }
\#073
第11行就是创建信号量。
第29行等信号量事件和退出事件。

\#001
\#002  //
\#003  //增加信号量
\#004  //
\#005  //
\#006  void IncSemaphore(void)
\#007  {
\#008         if (m_hSemaphore)
\#009         {
**\#010              if (!ReleaseSemaphore(**
**\#011                    m_hSemaphore, //****要增加的信号量。**
**\#012                   1,           //****增加****1.**
**\#013                   NULL) )      //****不想返回前一次信号量。**
\#014               {
\#015
\#016               }
\#017         }
\#018  }
\#019


