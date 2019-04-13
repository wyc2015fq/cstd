
# VC定时器SetTimer函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年06月21日 18:29:03[seven-soft](https://me.csdn.net/softn)阅读数：320标签：[timer																](https://so.csdn.net/so/search/s.do?q=timer&t=blog)[callback																](https://so.csdn.net/so/search/s.do?q=callback&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[application																](https://so.csdn.net/so/search/s.do?q=application&t=blog)[function																](https://so.csdn.net/so/search/s.do?q=function&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=function&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=application&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=application&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=callback&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=timer&t=blog)
一、SetTimer表示的是定义个定时器。根据定义指定的窗口，在指定的窗口（CWnd）中实现OnTimer事件，这样，就可以相应事件了。
SetTimer有两个函数。
①一个是全局的函数::SetTimer()
UINT SetTimer(
HWND hWnd, // handle of window for timer messages
UINT nIDEvent, // timer identifier
UINT uElapse, // time-out value
TIMERPROC lpTimerFunc // address of timer procedure
);
其中hWnd 是指向CWnd的指针，即处理Timer事件的窗口类。因此继承CWnd的子类均可以定义SetTimer事件。
②SetTimer() 在CWnd中也有定义，即SetTimer()是CWnd的一个成员函数。CWnd的子类可以调用该函数，来设置触发器。
UINT SetTimer(UINT nIDEvent, UINT nElapse, void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT, DWORD) );
参数含义：
nIDEvent:是指设置这个定时器的iD，即身份标志，这样在OnTimer()事件中，才能根据不同的定时器，来做不同的事件响应。这个ID是一个无符号的整型。
nElapse
是指时间延迟。单位是毫秒。这意味着，每隔nElapse毫秒系统调用一次Ontimer()。
void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT, DWORD)
Specifies the address of the application-supplied TimerProc callback function that processes the WM_TIMER messages. If this parameter is NULL, the WM_TIMER messages are placed in the application’s message queue and handled by the CWnd object。
意思是，指定应用程序提供的TimerProc回调函数的地址，来处里这个Timer事件。如果是NULL,系统将交由OnTimer()来处理这个Timer事件。
所以，一般情况下，我们将这个值设为NULL，有设置该定时器的对象中的OnTimer()函数来处理这个事件。
例：SetTimer(1,1000,NULL);
如果我要加入两个或者两个以上的 timer怎么办? 继续用SetTimer函数吧，上次的timer的ID是1，这次可以是2，3，4。
SetTimer(2,1000,NULL);
SetTimer(3,500,NULL);
WINDOWS会协调他们的。当然onTimer函数体也要发生变化，要在函数体内添加每一个timer的处理代码：
onTimer(nIDEvent)
{
switch(nIDEvent)
{
case 1:........;
break;
case 2:.......;
break;
case 3:......;
break;
}
}
二、我们再看看KillTimer()和OnTimer()的定义:
KillTimer同SetTimer（）一样，他也有两个，一个是全局的::KillTimer(),另一个是CWnd的一个函数。他的声明如下：
//全局函数
BOOL KillTimer(
HWND hWnd, // handle of window that installed timer
UINT uIDEvent // timer identifier
);
//CWnd函数
BOOL KillTimer(int nIDEvent );
这两个函数表示的意思是将iD为nIDEVENT的定时器移走。使其不再作用。其用法如同SetTimer（）一样。
再看看OnTimer()
CWnd::OnTimer
afx_msg void OnTimer(UINT nIDEvent);
ontimer() 是响应CWnd对象产生的WM_Timer消息。nIDEvent表示要响应TIMER事件的ID。

