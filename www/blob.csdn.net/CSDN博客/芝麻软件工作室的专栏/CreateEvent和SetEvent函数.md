
# CreateEvent和SetEvent函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 10:59:02[seven-soft](https://me.csdn.net/softn)阅读数：244


当你创建一个线程时，其实那个线程是一个循环，不像上面那样只运行一次的。这样就带来了一个问题，在那个死循环里要找到合适的条件退出那个死循环，那么是怎么样实现它的呢？在Windows里往往是采用事件的方式，当然还可以采用其它的方式。在这里先介绍采用事件的方式来通知从线程运行函数退出来，它的实现原理是这样，在那个死循环里不断地使用WaitForSingleObject函数来检查事件是否满足，如果满足就退出线程，不满足就继续运行。当在线程里运行阻塞的函数时，就需要在退出线程时，先要把阻塞状态变成非阻塞状态，比如使用一个线程去接收网络数据，同时使用阻塞的SOCKET时，那么要先关闭SOCKET，再发送事件信号，才可以退出线程的。下面就来演示怎么样使用事件来通知线程退出来。
函数CreateEvent声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
CreateEventA(
__in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
__in     BOOL bManualReset,
__in     BOOL bInitialState,
__in_opt LPCSTR lpName
);
WINBASEAPI
__out
HANDLE
WINAPI
CreateEventW(
__in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
__in     BOOL bManualReset,
__in     BOOL bInitialState,
__in_opt LPCWSTR lpName
);
\#ifdef UNICODE
\#define CreateEvent CreateEventW
\#else
\#define CreateEvent CreateEventA
\#endif // !UNICODE
**lpEventAttributes**是事件的属性。
**bManualReset**是指事件手动复位，还是自动复位状态。
**bInitialState**是初始化的状态是否处于有信号的状态。
**lpName**是事件的名称，如果有名称，可以跨进程共享事件状态。
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
\#012          m_hEventExit = NULL;
\#013  }
\#014
\#015  virtual ~CThread(void)
\#016  {
\#017         if (m_hThread)
\#018         {
\#019               //删除的线程资源。
\#020               ::CloseHandle(m_hThread);
\#021         }
\#022
\#023         if (m_hEventExit)
\#024         {
\#025               //删除事件。
\#026               ::CloseHandle(m_hEventExit);
\#027         }
\#028
\#029  }
\#030
\#031  //创建线程
\#032  HANDLE CreateThread(void)
\#033  {
\#034         //创建退出事件。
**\#035         m_hEventExit = ::CreateEvent(NULL,TRUE,FALSE,NULL);**
\#036         if (!m_hEventExit)
\#037          {
\#038               //创建事件失败。
\#039               return NULL;
\#040         }
\#041
\#042         //创建线程。
\#043          m_hThread = ::CreateThread(
\#044               NULL,                    //安全属性使用缺省。
\#045               0,                         //线程的堆栈大小。
\#046               ThreadProc,                 //线程运行函数地址。
\#047               this,                      //传给线程函数的参数。
\#048               0,                         //创建标志。
\#049               &m_dwThreadID);        //成功创建后的线程标识码。
\#050
\#051         return m_hThread;
\#052  }
\#053
\#054  //等待线程结束。
\#055  void WaitFor(DWORD dwMilliseconds = INFINITE)
\#056  {
\#057         //发送退出线程信号。
**\#058        ::SetEvent(m_hEventExit);**
\#059
\#060         //等待线程结束。
\#061         ::WaitForSingleObject(m_hThread,dwMilliseconds);
\#062  }
\#063
\#064 protected:
\#065  //
\#066  //线程运行函数。
\#067  //
\#068  //
\#069  static DWORD WINAPI ThreadProc(LPVOID lpParameter)
\#070  {
\#071         //转换传送入来的参数。
\#072         CThread* pThread = reinterpret_cast<CThread *>(lpParameter);
\#073         if (pThread)
\#074         {
\#075               //线程返回码。
\#076               //调用类的线程处理函数。
\#077               return pThread->Run();
\#078         }
\#079
\#080         //
\#081         return -1;
\#082  }
\#083
\#084  //线程运行函数。
\#085  //在这里可以使用类里的成员，也可以让派生类实现更强大的功能。
\#086  //
\#087  virtual DWORD Run(void)
\#088  {
\#089         //输出到调试窗口。
\#090         ::OutputDebugString(_T("Run()线程函数运行/r/n"));
\#091
\#092         //线程循环。
\#093         for (;;)
\#094         {
**\#095              DWORD dwRet = WaitForSingleObject(m_hEventExit,0);**
\#096               if (dwRet == WAIT_TIMEOUT)
\#097               {
\#098                    //可以继续运行。
\#099                    TCHAR chTemp[128];
\#100                    wsprintf(chTemp,_T("ThreadID=%d/r/n"),m_dwThreadID);
\#101                    ::OutputDebugString(chTemp);
\#102
\#103                     //目前没有做什么事情，就让线程释放一下CPU。
\#104                    Sleep(10);
\#105               }
\#106               else if (dwRet == WAIT_OBJECT_0)
\#107               {
\#108                    //退出线程。
\#109                    ::OutputDebugString(_T("Run()退出线程/r/n"));
\#110                    break;
\#111               }
\#112               else if (dwRet == WAIT_ABANDONED)
\#113               {
\#114                    //出错。
\#115                    ::OutputDebugString(_T("Run()线程出错/r/n"));
\#116                    return -1;
\#117               }
\#118         }
\#119
\#120         return 0;
\#121  }
\#122
\#123 protected:
\#124  HANDLE m_hThread;         //线程句柄。
\#125  DWORD m_dwThreadID;          //线程ID。
\#126
\#127  HANDLE m_hEventExit;    //线程退出事件。
\#128 };
\#129


