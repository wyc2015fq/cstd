
# CreateIoCompletionPort和GetQueuedCompletionStatus函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:32:25[seven-soft](https://me.csdn.net/softn)阅读数：245


在Windows系统里，使用完成端口是高性能的方法之一，比如把完成端口使用到线程池和网络服务器里。现在就通过线程池的方法来介绍怎么样使用完成端口，高性能的服务器以后再仔细地介绍怎么样构造它。其实完成端口是一个队列，所有的线程都在等消息出现，如果队列里有消息，就每个线程去获取一个消息执行它。先用函数CreateIoCompletionPort来创建一个消息队列，然后使用GetQueuedCompletionStatus函数来从队列获取消息，使用函数PostQueuedCompletionStatus来向队列里发送消息。通过这三个函数就实现完成端口的消息循环处理。
函数CreateIoCompletionPort、GetQueuedCompletionStatus、PostQueuedCompletionStatus声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
CreateIoCompletionPort(
__in     HANDLE FileHandle,
__in_opt HANDLE ExistingCompletionPort,
__in     ULONG_PTR CompletionKey,
__in     DWORD NumberOfConcurrentThreads
);
WINBASEAPI
BOOL
WINAPI
GetQueuedCompletionStatus(
__in HANDLE CompletionPort,
__out LPDWORD lpNumberOfBytesTransferred,
__out PULONG_PTR lpCompletionKey,
__out LPOVERLAPPED *lpOverlapped,
__in DWORD dwMilliseconds
);
WINBASEAPI
BOOL
WINAPI
PostQueuedCompletionStatus(
__in     HANDLE CompletionPort,
__in     DWORD dwNumberOfBytesTransferred,
__in     ULONG_PTR dwCompletionKey,
__in_opt LPOVERLAPPED lpOverlapped
);
**FileHandle**是关联的文件句柄。
**ExistingCompletionPort**是已经存在的完成端口。
**CompletionKey**是传送给处理函数的参数。
**NumberOfConcurrentThreads**是有多少个线程在访问这个消息队列。
**CompletionPort**是已经存在的完成端口。
**lpCompletionKey**是传送给处理函数的参数。
**lpOverlapped**是传送给处理函数的参数。
**dwMilliseconds**是等待时间。
**dwNumberOfBytesTransferred**是传送了多少个字节。
调用函数的例子如下：
\#001 \#pragma once
\#002
\#003 \#include "Thread.h"
\#004
\#005
\#006 //使用IOCP实现线程池。
\#007 //
\#008 class CThreadPools
\#009 {
\#010 public:
\#011
\#012  CThreadPools(void)
\#013  {
\#014          m_nThreadCount = 2;
\#015   }
\#016
\#017  ~CThreadPools(void)
\#018  {
\#019  }
\#020
\#021  bool Init(void)
\#022  {
\#023         //创建一个IOCP。
**\#024         m_hQueue = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_nThreadCount);**
\#025         if (m_hQueue == NULL)
\#026         {
\#027               //创建IOCP失败。
\#028               return false;
\#029         }
\#030  }
\#031
\#032  int GetThreadCount(void) const
\#033  {
\#034         return m_nThreadCount;
\#035  }
\#036
\#037  //线程池处理的内容。
\#038  DWORD Run(void)
\#039  {
\#040         DWORD dwBytesTransfered;
\#041         ULONG_PTR dwCompletionKey;
\#042
\#043         OVERLAPPED* pOverlapped;
\#044
\#045         //等一个IOCP的消息。
**\#046        while (GetQueuedCompletionStatus(m_hQueue, &dwBytesTransfered, &dwCompletionKey, &pOverlapped, INFINITE))**
\#047         {
\#048               if (pOverlapped == ((OVERLAPPED*) ((__int64) -1)) )
\#049               {
\#050                    //退出。
\#051                    OutputDebugString(_T("退出/r/n"));
\#052                    break;
\#053               }
\#054               else
\#055
\#056               {
\#057                    WPARAM request = (WPARAM) dwCompletionKey;
\#058
\#059                    //处理消息。
\#060                    OutputDebugString(_T("GetQueuedCompletionStatus /r/n"));
\#061               }
\#062         }
\#063
\#064         return 0;
\#065  }
\#066
\#067  //发送处理的消息。
\#068  bool QueueRequest(WPARAM wParam)
\#069  {
\#070         //往IOCP里发送一条消息。
**\#071        if (!PostQueuedCompletionStatus(m_hQueue, 0, (ULONG_PTR) wParam, NULL))**
\#072         {
\#073               return false;
\#074         }
\#075
\#076         return true;
\#077  }
\#078
\#079  //关闭所有线程。
\#080  void Close(void)
\#081  {
\#082         for (int i = 0; i < m_nThreadCount; i++)
\#083         {
\#084               PostQueuedCompletionStatus(m_hQueue, 0, 0, (OVERLAPPED*) ((__int64) -1) );
\#085         }
\#086  }
\#087
\#088 protected:
\#089  //接收消息处理的队列。
\#090  HANDLE m_hQueue;
\#091
\#092  //线程个数。
\#093  int m_nThreadCount;
\#094 };
\#095
\#096 //////////////////////////////////////////////////////////////////////////
\#097 class CThreads :
\#098  public CThread
\#099 {
\#100 public:
\#101  CThreads(CThreadPools* pPool)
\#102  {
\#103          m_pPool = pPool;
\#104  }
\#105  virtual ~CThreads(void)
\#106  {
\#107
\#108  }
\#109
\#110
\#111 protected:
\#112  //
\#113  //线程运行函数。
\#114  //在这里可以使用类里的成员，也可以让派生类实现更强大的功能。
\#115  //蔡军生2007/10/29
\#116  virtual DWORD Run(void)
\#117  {
\#118         //
\#119         if (m_pPool)
\#120         {
\#121               return m_pPool->Run();
\#122         }
\#123
\#124         return -1;
\#125  }
\#126
\#127 protected:
\#128  CThreadPools* m_pPool;
\#129
\#130 };


