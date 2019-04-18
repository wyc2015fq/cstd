# 窗口消息——Windows核心编程学习手札之二十六 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月14日 15:27:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2324标签：[windows																[编程																[数据结构																[timer																[struct																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)





# 窗口消息

## ——Windows核心编程学习手札之二十六

Windows允许一个进程至多建立10000个不同类型的用户对象（user object）：图符、光标、窗口类、菜单、加速键表等，当一个线程调用一个函数来建立某个对象时，则该对象就归属这个线程的进程所拥有。这种线程拥有关系的概念对窗口有重要意义：建立窗口的线程必须为窗口处理所有消息。一个线程创建了一个窗口，系统为其分配一个消息队列，用于窗口消息的派送（dispatch），为了使窗口接收这些消息，线程必须有自己的消息循环。一旦线程调用一个与图形界面有关的函数（如检查它的消息队列或建立一个窗口）系统就会为该线程分配一些另外的资源，以便它能够执行与用户界面有关的任务，特别的是，系统分配一个THREADINFO结构，并将这个数据结构和线程联系起来。

THREADINFO结构是一个内部的、未公开的数据结构，用来指定线程的登记消息队列（posted-message queue）、发送消息队列（send-message queue）、应答消息队列（reply-message queue）、虚拟输入队列（virtualized-input queue）、唤醒标志（wake flag）、以及用来描述线程局部输入状态的若干变量。当线程有了与之相联系的THREADINFO结构时，线程就有了自己的消息队列集合。

### 将消息发送到线程的消息队列中

发送到线程的消息被放置在接收线程的登记消息队列中，发送消息通过函数PostMessage完成：

BOOL PostMessage(HWND hwnd,

UINT uMsg,

WPARAM wParam,

LPARAM lParam);

也可以通过调用PostThreadMessage将消息放置在接收线程的登记消息队列中：

BOOL PostThreadMessage(DWORD dwThreadID,

UINT uMsg,

WPARAM wParam,

LPARAM lParam);

可以通过调用GetWindowsThreadPorcessID来确定是哪个线程建立了一个窗口：

DWORD GetWindowThreadProcessID（HWND hwnd,

PDWORD pdwProcessID);

传递窗口DWORD地址返回拥有该线程的进程ID。

为线程编写主消息循环以便在GetMessage或PeekMessage取出一个消息时，主消息循环代码检查hwnd是否为null，并检查MSG结构的msg成员来执行特殊的处理。如线程确定该消息不被指派给一个窗口，则不调用DispatchMessage，消息循环进行下一个消息。

终止线程的消息循环，可以调用函数PostQuitMessage：

VOID PostQuitMessage(int nExitCode);

类似于调用PostThreadMessage(GetCurrentThreadId(),WM_QUIT,nExitCode,0);

不过，PostQuitMessage并不实际登记一个消息到任何一个THREADINFO结构的队列，只是在内部，PostQuitMessage设定QS_QUIT唤醒标志，并设置THREADINFO结构的nExitCode成员。

### 向窗口发送消息

使用SendMessage函数可将窗口消息直接发送给一个窗口过程：

LRESULT SendMessage(HWND hwnd,

UINT uMsg,

WPARAM wParam,

LPARAM lParam);

窗口过程将处理这个消息，只有当消息被处理之后，SendMessage才能返回到调用程序。由于具有这个同步特性，比之PostMessage或PostThreadMessage，SendMessage函数用得频繁，调用这个函数的线程在下一行代码执行之前就知道窗口消息已经被完全处理。

如果调用SendMessage的线程向自己创建的窗口发送一个消息：调用指定窗口的窗口过程，将其作为一个子例程，当窗口过程完成对消息的处理，向SendMessage返回一个值，SendMessage再将这个值返回给调用线程。

如果一个线程通过SendMessage向其他线程创建的窗口发送消息：发送线程挂起，由另外线程处理消息并返回。由于可能造成线程挂起（hang）：接收线程的bug导致发送线程挂起，可利用4个函数：SendMessageTimeout、SendMessageCallback、SendNotifyMessage、ReplyMessage编写保护性代码防止出现这个情况：

LRESULT SendMessageTimeout(

HWND hwnd,

UINT uMsg,

WPARAM wParam,

LPARAM lParam,

UINT fuFlags,

UINT uTimeout,

PDWORD_PTR pdwResult);

这个函数可以设置等待其他线程返回消息的时间最大值。

线程间发送消息的第二个函数：

BOOL SendMessageCallback(

HWND hwnd,

UINT uMsg,

WPARAM wParam,

LPARAM lParam,

SENDASYNCPROC pfnResultCallBack,

ULONG_PTR dwData);

当发送消息的线程调用该函数时发送消息到接收线程的发送消息队列，并立即返回使发送线程可以继续执行。当接收线程完成对消息的处理时，一个消息被登记到发送线程的应答消息队列中，然后系统通过调用一个函数将这个应答通知给发送线程。

线程发送消息的第三个函数是：

BOOL SendNotifyMessage(

HWND hwnd,

UINT uMsg,

WPARAM wParam,

LPARAM lParam);

该函数将一个消息置于接收线程的发送消息队列中，并立即返回到发送线程，与PostMessage一样，但有两点不同：SendNotifyMessage是向另外的线程建立的窗口发送消息，发送的消息比起接收线程消息队列中存放的登记消息有更高的优先级；当向发送线程自己创建的窗口发送消息时，SendNotifyMessage和SendMessage函数一样，在消息被处理完后才能返回。

用于线程发送消息的第四个函数：

BOOL ReplyMessage(LRESULT lResult);

当发送线程调用ReplyMessage，是告诉系统：消息结果应该包装起来并登记到发送线程的应答消息队列中，唤醒发送线程获得结果并继续执行。

### 唤醒一个线程

当一个线程调用GetMessage或WaitMessage，但没有对这个线程或这个线程所建立窗口的消息时，系统可以挂起这个线程，这样就不再分配给它CPU时间。当有一个消息登记或发送到这个线程，系统要设置一个唤醒标志，指出现在要给这个线程分配CPU时间，以便处理消息。正常情况下，如果用户不按键或移动鼠标，就没有消息发送给任何窗口。

当一个线程正在运行时，可以通过调用GetQueueStatus函数来查询队列的状态：

DWORD GetQueueStatus(UINT fuFlags);

参数fuFlags是一个标志或一组由OR连接起来的标志，可用来测试特定的唤醒位。

当一个线程调用GetMessage或PeekMessage时，系统必须检查线程的队列状态标志的情况，并确定应该处理哪个消息：

1）如果QS_SENDMESSAGE标志被设置，系统向相应的窗口发送消息；

2）如果消息在线程的登记消息队列中，函数GetMessage或PeekMessage填充传递给它们的MSG结构；

3）如果QS_QUIT标志被设置，GetMessage或PeekMessage返回一个WM_QUIT消息，并复位QS_QUIT标志；

4）如果消息在线程的虚拟输入队列，函数GetMessage或PeekMessage返回硬件输入消息；

5）如果QS_PAINT标志被设置，GetMessage或PeekMessage为相应的窗口返回一个WM_PAINT消息；

6）如果QS_TIMER标志被设置，GetMessage或PeekMessage返回一个WM_TIMER消息；

GetMessage或PeekMessage函数导致一个线程睡眠，也可以利用内核对象或队列状态标志唤醒线程，函数：

DWORD MsgWaitForMultipleObjects(

DWORD nCount,

PHANDLE phObjects,

BOOL fWaitAll,

DWORD dwMilliseconds,

DWORD dwWakeMask);

DWORD MsgWaitForMultipleObjectsEx(

DWORD nCount,

PHANDLE phObjects,

DWORD dwMilliseconds,

DWORD dwWakeMask);

### 通过消息发送数据

特殊窗口消息WM_COPYDATA可以通过消息在进程间发送和接收数据。

例子：

COPYDATASTRUCT cds;

SendMessage(hwndReceiver,WM_COPYDATA,(WPARAM)hwndSender,(LPARAM)&cds);

其中COPYDATASTRUCT是一个结构，定义在WinUser.h文件中：

typedef struct tagCOPYDATASTRUCT{

ULONG_PTR dwData;

DWORD cbData;

PVOID lpData;

}COPYDATASTRUCT;

当SendMessage看到要发送一个WM_COPYDATA消息时，它建立一个内存映像文件，大小是cbData字节，并从发送进程的地址空间向这个内存映像文件复制数据，然后向目的窗口发送消息。](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)




