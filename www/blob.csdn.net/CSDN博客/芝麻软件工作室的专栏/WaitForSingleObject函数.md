
# WaitForSingleObject函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:01:29[seven-soft](https://me.csdn.net/softn)阅读数：305


上面已经介绍怎么样创建一个简单的线程，在那里就需要使用函数WaitForSingleObject，它是用来做什么的呢？其实它是用来实现等待线程结束的，它的机理是这样的，通知Windows操作系统，现在我进入睡眼状态，当我关注的对象条件是否满足，如果满足了就吵醒我。在那里关注的对象是线程是否退出，这是一个条件测试。如果不等待线程关闭，就去删除线程的资源，就会出错的。使用前面的线程是简单一些，但它不合适复用，现在就使用C＋＋的类来封装一个比较好用的类，这样就可以很方便地创建很多线程来使用，当然还可以继承它，实现更复杂的功能，下面就来学习这个例子。
函数WaitForSingleObject声明如下：
WINBASEAPI
DWORD
WINAPI
WaitForSingleObject(
__in HANDLE hHandle,
__in DWORD dwMilliseconds
);
**hHandle**是等待对象的句柄。
**dwMilliseconds**是等待的时间条件，可以永远等待下去。
调用这个函数的例子如下：
\#001 \#pragma once
\#002
\#003 //线程类。
\#004 //
\#005 class CThread
\#006 {
\#007 public:
\#008
\#009  CThread(void)
\#010  {
\#011          m_hThread = NULL;
\#012  }
\#013
\#014  virtual ~CThread(void)
\#015  {
\#016         if (m_hThread)
\#017         {
\#018               //删除的线程资源。
\#019               ::CloseHandle(m_hThread);
\#020         }
\#021
\#022  }
\#023
\#024  //创建线程
\#025  HANDLE CreateThread(void)
\#026  {
\#027         //创建线程。
\#028          m_hThread = ::CreateThread(
\#029               NULL,                    //安全属性使用缺省。
\#030               0,                         //线程的堆栈大小。
\#031               ThreadProc,                 //线程运行函数地址。
\#032               this,                      //传给线程函数的参数。
\#033               0,                         //创建标志。
\#034               &m_dwThreadID);        //成功创建后的线程标识码。
\#035
\#036         return m_hThread;
\#037  }
\#038
\#039  //等待线程结束。
\#040  void WaitFor(DWORD dwMilliseconds = INFINITE)
\#041  {
\#042         //等待线程结束。
**\#043        ::WaitForSingleObject(m_hThread,dwMilliseconds);**
\#044  }
\#045
\#046 protected:
\#047  //
\#048  //线程运行函数。
\#049  //
\#050  //
\#051  static DWORD WINAPI ThreadProc(LPVOID lpParameter)
\#052  {
\#053         //输出到调试窗口。
\#054         ::OutputDebugString(_T("ThreadProc线程函数运行/r/n"));
\#055
\#056         //线程返回码。
\#057         return 0;
\#058  }
\#059
\#060 protected:
\#061  HANDLE m_hThread;         //线程句柄。
\#062  DWORD m_dwThreadID;          //线程ID。
\#063
\#064 };
\#065
使用这个线程：
\#001 //
\#002 //响应命令.
\#003 //
\#004 //
\#005 LRESULT CCaiWinMsg::OnCommand(int nID,int nEvent)
\#006 {
\#007  //菜单选项命令响应:
\#008  switch (nID)
\#009  {
\#010  case IDC_CREATEBTN:
\#011         //显示一个按钮。
\#012         if (!m_hBtn)
\#013         {
\#014                m_hBtn = CreateWindow(_T("BUTTON"),_T("按钮"),
\#015                    WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,
\#016                    50,50,100,32,
\#017                     m_hWnd,(HMENU)IDC_BTN,m_hInstance,NULL);
\#018         }
\#019         break;
\#020  case IDC_BTN:
\#021         {
**\#022              CThread threadDemo;**
**\#023              threadDemo.CreateThread();**
**\#024              threadDemo.WaitFor();**
\#025
\#026               //
\#027               OutputDebugString(_T("按钮按下/r/n"));
\#028         }
\#029         break;
\#030  default:
\#031         return CCaiWin::OnCommand(nID,nEvent);
\#032  }
\#033
\#034  return 1;
\#035 }


