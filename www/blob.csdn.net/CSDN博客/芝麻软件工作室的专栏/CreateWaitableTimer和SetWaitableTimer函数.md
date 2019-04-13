
# CreateWaitableTimer和SetWaitableTimer函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:25:49[seven-soft](https://me.csdn.net/softn)阅读数：166标签：[attributes																](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)[integer																](https://so.csdn.net/so/search/s.do?q=integer&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[object																](https://so.csdn.net/so/search/s.do?q=object&t=blog)[游戏																](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=object&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=api&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=api&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=integer&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)
用户感觉到软件的好用，就是可以定时地做一些工作，而不需要人参与进去。比如每天定时地升级病毒库，定时地下载电影，定时地更新游戏里的人物。要想实现这些功能，就可以使用定时器的API函数CreateWaitableTimer和SetWaitableTimer来实现了，这对API函数创建的时钟是比较精确的，可以达到100倍的10亿分之一秒。
函数CreateWaitableTimer和SetWaitableTimer声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
CreateWaitableTimerA(
__in_opt LPSECURITY_ATTRIBUTES lpTimerAttributes,
__in     BOOL bManualReset,
__in_opt LPCSTR lpTimerName
);
WINBASEAPI
__out
HANDLE
WINAPI
CreateWaitableTimerW(
__in_opt LPSECURITY_ATTRIBUTES lpTimerAttributes,
__in     BOOL bManualReset,
__in_opt LPCWSTR lpTimerName
);
\#ifdef UNICODE
\#define CreateWaitableTimer CreateWaitableTimerW
\#else
\#define CreateWaitableTimer CreateWaitableTimerA
\#endif // !UNICODE

WINBASEAPI
BOOL
WINAPI
SetWaitableTimer(
__in     HANDLE hTimer,
__in     const LARGE_INTEGER *lpDueTime,
__in     LONG lPeriod,
__in_opt PTIMERAPCROUTINE pfnCompletionRoutine,
__in_opt LPVOID lpArgToCompletionRoutine,
__in     BOOL fResume
);
**lpTimerAttributes**是设置定时器的属性。
**bManualReset**是是否手动复位。
**lpTimerName**是定时器的名称。
**hTimer**是定时器的句柄。
**lpDueTime**是设置定时器时间间隔，当设置为正值是绝对时间；当设置为负数是相对时间。
**lPeriod**是周期。
**pfnCompletionRoutine**是设置回调函数。
**lpArgToCompletionRoutine**是传送给回调函数的参数。
**fResume**是设置系统是否自动恢复。
调用函数的例子如下：
\#001 //创建定时器
\#002  //
\#003  int CreateTestTimer(void)
\#004  {
\#005         HANDLE hTimer = NULL;
\#006         LARGE_INTEGER liDueTime;
\#007
\#008         //设置相对时间为10秒。
\#009         liDueTime.QuadPart = -100000000;
\#010
\#011         //创建定时器。
**\#012        hTimer = CreateWaitableTimer(NULL, TRUE, _T("TestWaitableTimer"));**
\#013         if (!hTimer)
\#014         {
\#015               return 1;
\#016         }
\#017
\#018         OutputDebugString(_T("10秒定时器/r/n"));
\#019
\#020         //设置10秒钟。
**\#021        if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))**
\#022         {
\#023               //
\#024               CloseHandle(hTimer);
\#025               return 2;
\#026         }
\#027
\#028         //等定时器有信号。
\#029         if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
\#030         {
\#031               OutputDebugString(_T("10秒定时器出错了/r/n"));
\#032               //
\#033               CloseHandle(hTimer);
\#034               return 3;
\#035         }
\#036         else
\#037         {
\#038               //10秒钟到达。
\#039               OutputDebugString(_T("10秒定时器到了/r/n"));
\#040         }
\#041
\#042         //
\#043         CloseHandle(hTimer);
\#044         return 0;
\#045  }


