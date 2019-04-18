# 如何正确的关闭 MFC 线程 - 文章 - 伯乐在线
原文出处： [lwbeyond](http://blog.csdn.net/lwbeyond/article/details/5686165)
**前言：**
近日在网上看到很多人问及如何关闭一下线程，但是我看网上给出的并不详细，而且有些方法还是错误的。小弟在此拙作一篇，不谈别的，只谈及如何正确的关闭MFC的线程，至于Win32和C RunTime的线程暂不涉及。
**一.关于MFC的线程**
１.MFC的线程有两种，一种称为Work线程，一种称为UI线程。一般情况下Work线程与UI线程的区别主要在于UI线程有消息队列（并不是有没有界面，这点要注意，UI线程也是可以没有界面的）。
２.创建这两种线程的区别也不大，可以从创建函数看出。


```
// Work线程  
CWinThread* AfxBeginThread(  
      AFX_THREADPROC pfnThreadProc,  
      LPVOID pParam,  
      int nPriority = THREAD_PRIORITY_NORMAL,  
      UINT nStackSize = 0,  
      DWORD dwCreateFlags = 0,  
      LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL  
     );  
  
// UI线程  
CWinThread* AfxBeginThread(  
      CRuntimeClass* pThreadClass,  
      int nPriority = THREAD_PRIORITY_NORMAL,  
      UINT nStackSize = 0,  
      DWORD dwCreateFlags = 0,  
      LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL  
     );
```
关于函数的具体使用，请查阅MSDN，这里不涉及。
**二. 结束线程前的注意事项**
在结束一个线程前，只有一点要注意，那就是m_bAutoDelete 的状态。（什么？不知道m_bAutoDelete ？！！快去查阅MSDN吧）。


```
m_bAutoDelete = FALSE;　// 表示你自己管理　CWind　对象，包括它的清理  
  
m_bAutoDelete = TRUE;  // 默认值，　系统会自己清理　CWind 对象
```
m_bAutoDelete = TRUE;　系统自己清理CWind对象，当然还包括CloseHandle()，ExitInstance()等等一堆函数的调用。
m_bAutoDelete = FALSE;　那么就一定要记得自己在用完后调用delete删除创建线程的对象，这一点极为重要，因为不调用delete一定会有内存泄漏问题。
总之m_bAutoDelete 的值对结束工作是很重要的，这点一定要注意。
**三.正确的结束一个Work线程**
因为Work线程是一个全局函数，或者是一个Static函数，所以它的运行完成也就是它的正常退出了。（什么？不明白，示例代码如下）
１.情况一：


```
UINT WorkFunc(LPVOID pParam)  
{  
　　// 工作  
　　......  
   return 0;  // 就算正常退出了，简单吧  
}
```
２.情况二：
Work线程是个死循环或一时半会儿出不来，这时要主线程要发个消息给Work线程，让他退出。


```
UINT WorkFunc(LPVOID pParam)  
{  
  for(;;)  
  {  
   // ...  
　　if( WAIT_OBJECT_0 == WaitForSingleObject(m_hThread, INFINITE)} // 收到激发态的消息  
    {  
      return 0;//正常退出  
     }  
  
　}//end for  
　　  
　return 0;  
}
```
关于主线程发一个激发态的消息给Work线程，有多种方法，如在主线程里调用SetEvent()等等，你想用什么都行，但是最好不要在Work线程里用Busy loop的方法。至于为什么，请参阅《Win32多线程程序设计》上面的论述。
**四.正确结束一个UI线程**
因为UI线程有消息队列，所以结束一个UI线程最好的方法是发一个WM_QUIT消息给消息队列，方法很多如：PostQuitMessage(),PostThreadMessage()等等。但是发出消息后最好等待看UI线程是否已经退出（很多人都没有提及这一点，但是实际工作中发现，加上这一点是多么的重要）。


```
// 主线程结束UI线程的代码  
if(pThread)   
{  
  // 1. 发一个WM_QUIT　消息结　UI　线程  
  pThread->PostThreadMessage(WM_QUIT, NULL, NULL);  
                  
 // 2. 等待　UI　线程正常退出  
 if (WAIT_OBJECT_0 == WaitForSingleObject(pThread->m_hThread, INFINITE))  
　{  
    // 3. 删除 UI 线程对象，只有当你设置了m_bAutoDelete = FALSE;　时才调用  
　　delete   pThread;   
　}  
  
}
```
**五.关于几个问题的解答**
**１.问：为什么我的UI线程没有调用ExitInstance()？**
答：最大的可能是你的WM_QUIT消息没有通知到UI线程。为了保险期间最好调用PostThreadMessage()，这样可以指定线程的ID。当然如果你对消息比较熟悉的话，也可以抛一个消息到最顶层。
**２.问：为什么我的UI线程没有调用析构函数？**
答：检查看你的m_bAutoDelete = FALSE，如果是的话，那么看你的线程对象是否已经delete了。一般情况下调用delete会调用析构函数。
**３.问：在UI线程中没有调用WaitForSingleObject()，会怎么样？**
答：我们知道在PostMessage()之后，函数会马上返回，如查没有wait…()，那么紧接着就调用了delete，很有可能对象做的退出操作过程还没有完成时，又把对象delete掉了，结果还是没有正常结束。（注：WM_QUIT消息之后会触发一堆函数，这个时间是不定的，所以最好Wait…才是正道。）
**六.最后不愿提的函数**
几乎每本讲线程的书都会提到下面的函数：
void AfxEndThread(UINT nExitCode);
TerminateThread();
……还有其它的一些极端的函数
**我的观点是：最好不要使用，除非你知道要发生什么！！**
