
# SetTimer和KillTimer函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:24:18[seven-soft](https://me.csdn.net/softn)阅读数：179标签：[timer																](https://so.csdn.net/so/search/s.do?q=timer&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=api&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=api&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=timer&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=timer&t=blog)

在前面介绍了一对定时器的API函数使用，现在又介绍另外一对API函数的使用。它使用起来比前的函数要简单一些，但它一般是使用到有窗口的程序里，并且它的精度也没有前面的API函数高，对于一些要求不高的场合还是非常合适的。它是采用消息通知的方式，每当定时到了就会收到一条消息。
函数SetTimer和KillTimer声明如下：
WINAPI
SetTimer(
__in_opt HWND hWnd,
__in UINT_PTR nIDEvent,
__in UINT uElapse,
__in_opt TIMERPROC lpTimerFunc);
WINUSERAPI
BOOL
WINAPI
KillTimer(
__in_opt HWND hWnd,
__in UINT_PTR uIDEvent);
**hWnd**是窗口接收定时器的句柄。
**nIDEvent**是定时器事件标识号。
**uElapse**是定时器的毫秒值。
**lpTimerFunc**是定时到达回调函数。
调用函数的例子如下：
\#001 //设置定时器。
**\#002              ::SetTimer(m_hWnd,             //****指向窗口的句柄。**
**\#003                   IDT_TIMER1,            //****定时器标识。**
**\#004                   1000,                 // 1****秒**
**\#005                   (TIMERPROC) NULL);     //****不使用回调函**数。
接收**WM_TIMER**消息并关闭定时器：
**\#001 case WM_TIMER:**
\#002         {
\#003               if (IDT_TIMER1 == wParam)
\#004               {
\#005                    OutputDebugString(_T("定时器测试消息关闭/r/n"));
**\#006                   ::KillTimer(m_hWnd,IDT_TIMER1);**
\#007               }
\#008
\#009         }
\#010         break;


