# Visual C++编程讲义一： Windows消息队列 - ljx0305的专栏 - CSDN博客
2009年09月20日 17:20:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：952标签：[windows																[编程																[c++																[null																[api																[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
一、      消息队列
序：消息队列牵扯到win32和mfc编程的基础，意义非常重大！:)所以学Visual C++之前有必要先了解一下消息队列！
这篇文章（包括整个系列）不会从基础讲起，写出来主要是像我给我这样已经了解，但隔两天就闹不清楚的同学看的，
1 消息队列介绍
大概是为了多任务的需要，Windows接管了所有的资源，以便于进行统一处理。
其中最××的就是接管了输入，并推出了消息队列的概念。
Windows为每个线程（包括主、辅、UI线程）维护一个消息队列，我们再从线程所属的消息队列中读取消息。这个消息队列什么时候创建我不知道，但当线程收到消息时，它是肯定会存在的！
2 消息队列的读取
Windows为我们提供了两个从当前线程的消息队列中读取消息的API：GetMessage、PeekMessage
两者通过指定hWnd读取指定窗口消息，hWnd=0读取所有消息
区别是GetMessage会一直等到消息出现，而PeekMessage在没有消息的情况下会直接返回0
这两个API的区别有点像以前Basic的Input和Inkey。
3 消息队列的写入
大多数消息由Windows维护，不过Windows提供了两个向消息队列写入的API：PostMessage、PostThreadMessage
PostMessage：向hWnd指定的窗口发信息,即向窗口所在的线程的消息队列写入消息。当hWnd=HWND_BROADCAST时向所有顶层窗体发信息，hWnd=NULL时向当前线程的消息队列写消息
PostThreadMessage：当然是向指定的线程的消息队列写入消息，这些消息可以用GetMessage、PeekMessage读取(指定hWnd参数=NULL可以读取所有消息包括线程消息，PeekMessage还可以用hWnd=-1读取线程消息)
4 其它概念
有了上面4个API就可以构成消息队列的主体了，不过大概是为了减轻大家的负担！Windows又额外提供了些东东
SendMessage：向窗口发信息，调用指定窗口过程并等待返回。窗口过程是Win32下的东西，大概是因为不能面向对象，所以为各个窗体各写一个消息处理函数达到代码隔离。简单的说就是一堆case语句
DispatchMessage：自动分发消息。将当前信息（用GetMessage、PeekMessage读取的）分发到各个窗口过程,我觉得和SendMessage类似
TranslateMessage、TranslateAccelerator：消息转换API，大概是把扫描码转换成相应的ASCII码或者控制命令
消息循环: 一般来说大家都会在主程序中写出下面这样类似的读取消息的循环结构，大家都叫它消息循环
  while(GetMessage   (&msg,   NULL,   0,   0))   
    {   
      TranslateMessage   (&msg)   ;       
       DispatchMessage   (&msg)   ;   
     }
　　return msg.wParam ;
.....
5 一个简单的MFC消息收发示例
UINT MyThreadProc( LPVOID pParam )   //包含消息处理的线程函数
{
       MSG msg;
       while(!::PeekMessage(&msg,0,0,0,PM_REMOVE))
…..; //….，收到信息就退出
       return 0;
}
CWinThread* wt;
wt = AfxBeginThread(MyThreadProc, NULL);启动线程
……
::PostThreadMessage(wt->m_nThreadID, WM_USER+1, 0, 0); //发消息结束线程
这段代码的效果几乎和下面一样
UINT MyThreadProc( LPVOID pParam )   //包含消息处理的线程函数
{
       while(:WaitForSingleObject(g_e,0)!=WAIT_OBJECT_0) //收到信号就退出
…..; 
       return 0;
}
   CEvent g_e;
   AfxBeginThread(MyThreadProc, NULL); //启动线程
   ……
   g_e.SetEvent; //发信号结束线程
6 深入的了解消息队列
  以上是消息队列的基本内容，可以大概了解消息队列。
 不过如果你想解决更深的问题,仅凭上面这些就不行了,比如：
 SendMessage、DispatchMessage都是向窗口发信息，凭直觉可以猜测它们是通过直接调用窗口过程来实现，那么可不可以把程序的消息循环改成下面这样：
  while(GetMessage   (&msg,   NULL,   0,   0))   
    {   
      TranslateMessage   (&msg)   ;       
       //DispatchMessage(&msg);
       SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);  
     }
这个问题好像有点莫名奇妙，但要给出一个肯定的回答就超出MSDN的范畴了，大家可以查查看
这个问题的答案是可以!
 因为SendMessage向同一线程内部的窗体发送信息时，实际上就是直接调用该窗口的窗口过程，DispatchMessage也就是做这样的工作。
 区别是DispatchMessage无法调用其它进程的窗口过程，因为不在同一地址空间。SendMessage也同样不能直接调用，不过对于向不同线程的窗口发信息，SendeMessage提供了另外的实现。所以可以用SendeMessage替换DispatchMessage，反过来就不一定行！
关于消息队列的更多内容可参考MSDN、《Windows核心编程》 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/flyingscv/archive/2008/03/24/2214798.aspx](http://blog.csdn.net/flyingscv/archive/2008/03/24/2214798.aspx)
