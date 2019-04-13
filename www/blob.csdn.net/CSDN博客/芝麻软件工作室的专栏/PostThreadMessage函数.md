
# PostThreadMessage函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 10:55:52[seven-soft](https://me.csdn.net/softn)阅读数：314


在写服务器程序里，很多地方都需要使用到线程池。特别现在多处理器的CPU越来越普及，使用多个线程池是明显提高服务器程序的性能。在以消息为基础的Windows系统里，使用消息来处理是最简单的线程池办法，不但使用起来简单，而且理解起来也很简单的方法。创建多个线程后，就可以根据线程的ID来向不同的线程发送消息，每个线程都处理自己的消息。而发送消息给线程的函数是PostThreadMessage函数。下面来演示怎么样使用这个函数。
函数PostThreadMessage声明如下：
WINUSERAPI
BOOL
WINAPI
PostThreadMessageA(
__in DWORD idThread,
__in UINT Msg,
__in WPARAM wParam,
__in LPARAM lParam);
WINUSERAPI
BOOL
WINAPI
PostThreadMessageW(
__in DWORD idThread,
__in UINT Msg,
__in WPARAM wParam,
__in LPARAM lParam);
\#ifdef UNICODE
\#define PostThreadMessage PostThreadMessageW
\#else
\#define PostThreadMessage PostThreadMessageA
\#endif // !UNICODE
**idThread**是线程ID。
**Msg**是发送的消息ID。
**wParam**是消息参数。
**lParam**是消息参数。
调用函数的例子如下：
\#001 //线程运行函数。
\#002 //在这里可以使用类里的成员，也可以让派生类实现更强大的功能。
\#003 //
\#004 DWORD CThreadMsg::Run(void)
\#005 {
\#006  //创建线程消息队列。
\#007  MSG msg;
\#008  PeekMessage(&msg, NULL, WM_USER, WM_USER+1000, PM_NOREMOVE);
\#009
\#010  //
\#011  for (;;)
\#012  {
\#013         //查找是否有线程消息处理。
\#014**BOOL bRes = PeekMessage(&msg, NULL, WM_USER, WM_USER+1000, PM_REMOVE);**
**\#015        if (bRes)**
**\#016        {**
**\#017              //****在这里处理本线程的消息。**
**\#018              ::OutputDebugString(_T("CThreadMsg::Run()****有消息处理****/r/n"));**
**\#019        }**
\#020         else
\#021         {
\#022               //等线程退出事件。
\#023               DWORD dwRet = WaitForSingleObject(m_hEventExit,0);
\#024               if (dwRet == WAIT_TIMEOUT)
\#025               {
\#026                    //目前没有做什么事情，就让线程释放一下CPU。
\#027                    Sleep(10);
\#028               }
\#029               else
\#030               {
\#031                    //退出线程。
\#032                    ::OutputDebugString(_T("CThreadMsg::Run()退出线程/r/n"));
\#033                    break;
\#034               }
\#035         }
\#036  }
\#037
\#038  //
\#039  return 0;
\#040 }
上面实现线程的消息处理。
\#001 class CThreadMsg :
\#002  public CThread
\#003 {
\#004 public:
\#005  CThreadMsg(void);
\#006  virtual ~CThreadMsg(void);
\#007
\#008  //发送一条消息给线程处理。
\#009  //
\#010  void PostMessage(void)
\#011  {
\#012         //
**\#013        ::PostThreadMessage(m_dwThreadID,WM_USER+100,0,0);**
\#014  }
\#015 protected:
\#016  virtual DWORD Run(void);
\#017 };
上面是调用函数PostThreadMessage发送消息给线程处理。


