
# CreateThread函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:02:35[seven-soft](https://me.csdn.net/softn)阅读数：314标签：[attributes																](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[网络游戏																](https://so.csdn.net/so/search/s.do?q=网络游戏&t=blog)[多线程																](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)[button																](https://so.csdn.net/so/search/s.do?q=button&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=button&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=网络游戏&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=网络游戏&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)
随着时代的发展，计算机技术发展得很快。CPU已经从单核心到多核心的转变，也就是一个CPU里具备了同时做多件事情的能力，而不是过去的分时复用了，而是实实在在地做多件事情。因此，开发软件也进入了一个新时代，就是多线程软件的开发时代。如何合理地分配多个线程同时运行，是提高软件效率的关键因素了。比如像网络游戏的客户端里，就可以使用一个线程不断地更新游戏的界面，分配另外一个线程不断地发送和接收网络的数据，这样CPU的两个核心都在不断地工作。如果还像以前那样使用一个线程的话，就会发现只有一个内核在做事情。当然使用线程的编程模型，也会大大地简化软件的复杂性。下面就来使用线程的API函数。
函数CreateThread声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
CreateThread(
__in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes,
__in      SIZE_T dwStackSize,
__in      LPTHREAD_START_ROUTINE lpStartAddress,
__in_opt LPVOID lpParameter,
__in     DWORD dwCreationFlags,
__out_opt LPDWORD lpThreadId
);
**lpThreadAttributes**是线程的属性。
**dwStackSize**是线程的栈大小。
**lpStartAddress**是线程函数的开始地址。
**lpParameter**是传送给线程函数的参数。
**dwCreationFlags**是创建线程标志，比如挂起线程。
**lpThreadId**是标识这个线程的ID。
调用这个函数的例子如下：
\#001 //
\#002 //线程运行函数。
\#003 //
\#004 //
\#005 static DWORD WINAPI ThreadProc(
\#006                               LPVOID lpParameter
\#007                               )
\#008 {
\#009  //输出到调试窗口。
\#010  OutputDebugString(_T("ThreadProc线程函数运行/r/n"));
\#011
\#012  //线程返回码。
\#013  return 0;
\#014 }
\#015
\#016
\#017
\#018 //
\#019 //响应命令.
\#020 //
\#021 //
\#022 LRESULT CCaiWinMsg::OnCommand(int nID,int nEvent)
\#023 {
\#024  //菜单选项命令响应:
\#025  switch (nID)
\#026  {
\#027  case IDC_CREATEBTN:
\#028         //显示一个按钮。
\#029         if (!m_hBtn)
\#030         {
\#031                m_hBtn = CreateWindow(_T("BUTTON"),_T("按钮"),
\#032                    WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,
\#033                    50,50,100,32,
\#034                     m_hWnd,(HMENU)IDC_BTN,m_hInstance,NULL);
\#035         }
\#036         break;
\#037  case IDC_BTN:
\#038         {
\#039               //传给线程的简单参数。
\#040               int nParam = 110;
\#041
\#042               //保存线程的ID。
\#043               DWORD dwThreadID = 0;
\#044
\#045               //创建线程。
\#046               HANDLE hThread = CreateThread(
\#047                    NULL,                    //安全属性使用缺省。
\#048                    0,                         //线程的堆栈大小。
\#049                    ThreadProc,                 //线程运行函数地址。
\#050                    &nParam,               //传给线程函数的参数。
\#051                    0,                         //创建标志。
\#052                    &dwThreadID);       //成功创建后的线程标识码。
\#053
\#054               //等待线程结束。
\#055               WaitForSingleObject(hThread,INFINITE);
\#056
\#057               //删除的线程资源。
\#058               CloseHandle(hThread);
\#059
\#060               //
\#061               OutputDebugString(_T("按钮按下/r/n"));
\#062
\#063         }
\#064         break;
\#065  default:
\#066         return CCaiWin::OnCommand(nID,nEvent);
\#067  }
\#068
\#069  return 1;
\#070 }


