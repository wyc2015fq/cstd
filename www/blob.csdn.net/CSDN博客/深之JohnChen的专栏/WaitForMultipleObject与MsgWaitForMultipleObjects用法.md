# WaitForMultipleObject与MsgWaitForMultipleObjects用法 - 深之JohnChen的专栏 - CSDN博客

2010年06月01日 10:24:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6972


用户模式的线程同步机制效率高，如果需要考虑线程同步问题，应该首先考虑用户模式的线程同步方法。但是，用户模式的线程同步有限制，对于多个进程之间的线程同步，用户模式的线程同步方法无能为力。这时，只能考虑使用内核模式。

用户模式与内核模式线程同步机制比较：
||用户模式|内核模式|
|----|----|----|
|优点|线程同步机制速度快|支持多个进程之间的线程同步防止死锁|
|缺点|容易陷入死锁状态多个进程之间的线程同步会出现问题。（比如竞争资源、死锁）|线程同步机制速度慢线程必须从用户模式转为内核模式。这个转换需要很大的代价：往返一次需要占用x 8 6平台上的大约1 0 0 0个C P U周期|

Windows提供了许多内核对象来实现线程的同步。对于线程同步而言，这些内核对象有两个非常重要的状态：“已通知”状态，“未通知”状态（也有翻译为：受信状态，未受信状态）。Windows提供了几种内核对象可以处于已通知状态和未通知状态：进程、线程、作业、文件、控制台输入/输出/错误流、事件、等待定时器、信号量、互斥对象。

![](http://hi.csdn.net/attachment/201006/1/0_1275358971Bb1v.gif)

你可以通知一个内核对象，使之处于“已通知状态”，然后让其他等待在该内核对象上的线程继续执行。你可以使用Windows提供的API函数，等待函数来等待某一个或某些内核对象变为已通知状态。

一、WaitForSingleObject、WaitForMulitpleObjects

函数功能: 等待一个内核对象变为已通知状态

可以使用WaitForSingleObject函数来等待一个内核对象变为已通知状态：

DWORD WaitForSingleObject(

HANDLE hObject, //指明一个内核对象的句柄

DWORD dwMilliseconds); //等待时间

该函数需要传递一个内核对象句柄，该句柄标识一个内核对象，如果该内核对象处于未通知状态，则该函数导致线程进入阻塞状态；如果该内核对象处于已通知状态，则该函数立即返回WAIT_OBJECT_0。第二个参数指明了需要等待的时间（毫秒），可以传递INFINITE指明要无限期等待下去，如果第二个参数为0，那么函数就测试同步对象的状态并立即返回。如果等待超时，该函数返回WAIT_TIMEOUT。如果该函数失败，返回WAIT_FAILED。可以通过下面的代码来判断：

DWORD dw = WaitForSingleObject(hProcess, 5000); //等待一个进程结束

switch (dw)

{

case WAIT_OBJECT_0:

// hProcess所代表的进程在5秒内结束

break;

case WAIT_TIMEOUT:

// 等待时间超过5秒

break;

case WAIT_FAILED:

// 函数调用失败，比如传递了一个无效的句柄

break;

}

还可以使用WaitForMulitpleObjects函数来等待多个内核对象变为已通知状态：

DWORD WaitForMultipleObjects(

DWORD dwCount, //等待的内核对象个数

CONST HANDLE* phObjects, //一个存放被等待的内核对象句柄的数组

BOOL bWaitAll, //是否等到所有内核对象为已通知状态后才返回

DWORD dwMilliseconds); //等待时间

该函数的第一个参数指明等待的内核对象的个数，可以是0到MAXIMUM_WAIT_OBJECTS（64）中的一个值。phObjects参数是一个存放等待的内核对象句柄的数组。bWaitAll参数如果为TRUE，则只有当等待的所有内核对象为已通知状态时函数才返回，如果为FALSE，则只要一个内核对象为已通知状态，则该函数返回。第四个参数和WaitForSingleObject中的dwMilliseconds参数类似。

该函数失败，返回WAIT_FAILED；如果超时，返回WAIT_TIMEOUT；如果bWaitAll参数为TRUE，函数成功则返回WAIT_OBJECT_0，如果bWaitAll为FALSE，函数成功则返回值指明是哪个内核对象收到通知。

可以如下使用该函数：

HANDLE h[3]; //句柄数组

//三个进程句柄

h[0] = hProcess1;

h[1] = hProcess2;

h[2] = hProcess3;

DWORD dw = WaitForMultipleObjects(3, h, FALSE, 5000); //等待3个进程结束

switch (dw)

{

case WAIT_FAILED:

// 函数呼叫失败

break;

case WAIT_TIMEOUT:

// 超时

break;

case WAIT_OBJECT_0 + 0:

// h[0]（hProcess1）所代表的进程结束

break;

case WAIT_OBJECT_0 + 1:

// h[1]（hProcess2）所代表的进程结束

break;

case WAIT_OBJECT_0 + 2:

// h[2]（hProcess3）所代表的进程结束

break;

}

你也可以同时通知一个内核对象，同时等待另一个内核对象，这两个操作以原子的方式进行：

DWORD SignalObjectAndWait(

HANDLE hObjectToSignal, //通知的内核对象

HANDLE hObjectToWaitOn, //等待的内核对象

DWORD dwMilliseconds, //等待的时间

BOOL bAlertable); //与IO完成端口有关的参数，暂不讨论

该函数在内部使得hObjectToSignal参数所指明的内核对象变成已通知状态，同时等待hObjectToWaitOn参数所代表的内核对象。dwMilliseconds参数的用法与WaitForSingleObject函数类似。

该函数返回如下：WAIT_OBJECT_0，WAIT_TIMEOUT，WAIT_FAILED，WAIT_IO_COMPLETION。

等你需要通知一个互斥内核对象并等待一个事件内核对象的时候，可以这么写：

ReleaseMutex(hMutex);

WaitForSingleObject(hEvent, INFINITE);

可是，这样的代码不是以原子的方式来操纵这两个内核对象。因此，可以更改如下：

SignalObjectAndWait(hMutex, hEvent, INFINITE, FALSE);

二、MsgWaitForMultipleObjects 

函数功能:阻塞时仍可以响应消息

MsgWaitForMultipleObjects()函数类似WaitForMultipleObjects()，但它会在“对象被激发”或“消息到达队列”时被唤醒而返回。MsgWaitForMultipleObjects()多接收一个参数，允许指定哪些消息是观察对象。

DWORD MsgWaitForMultipleObjects(

DWORD nCount,// 表示pHandles所指的handles数组的元素个数，最大容量是MAXIMUM_WAIT_OBJECTS

LPHANDLE pHandles,// 指向一个由对象handles组成的数组，这些handles的类型不需要相同

BOOL fWaitAll,// 是否等待所有的handles被激发才返回

DWORD dwMilliseconds,// 超时时间

DWORD dwWakeMask// 欲观察的用户输入消息类型

);

参数

dwWakeMask 

欲观察的用户输入消息类型: Value Meaning 

QS_ALLEVENTS An input, WM_TIMER, WM_PAINT, WM_HOTKEY, or posted message is in the queue. 

QS_ALLINPUT Any message is in the queue. 

QS_ALLPOSTMESSAGE A posted message (other than those listed here) is in the queue.

QS_HOTKEY A WM_HOTKEY message is in the queue. 

QS_INPUT An input message is in the queue. 

QS_KEY A WM_KEYUP, WM_KEYDOWN, WM_SYSKEYUP, or WM_SYSKEYDOWN message is in the queue. 

QS_MOUSE A WM_MOUSEMOVE message or mouse-button message (WM_LBUTTONUP, WM_RBUTTONDOWN, and so on). 

QS_MOUSEBUTTON A mouse-button message (WM_LBUTTONUP, WM_RBUTTONDOWN, and so on). 

QS_MOUSEMOVE A WM_MOUSEMOVE message is in the queue. 

QS_PAINT A WM_PAINT message is in the queue. 

QS_POSTMESSAGE A posted message (other than those just listed) is in the queue. 

QS_SENDMESSAGE A message sent by another thread or application is in the queue. 

QS_TIMER A WM_TIMER message is in the queue 

返回值

WAIT_TIMEOUT ：因时间终了而返回

WAIT_OBJECT_0 ：当bWaitAll是TRUE

WAIT_OBJECT_0 to (WAIT_OBJECT_0 + nCount – 1) ：bWaitAll是FALSE，将返回值减去WAIT_OBJECT_0，就表示数组中哪一个handle被激发了

WAIT_ABANDONED_0 to (WAIT_ABANDONED_0 + nCount – 1) ：等待的对象中有任何mutexes

WAIT_FAILED ：函数失败时返回该值，可以使用GetLastError()找出失败的原因

WAIT_OBJECT_0 + nCount ：消息到达队列

MsgWaitForMultipleObjects()的正确使用方式是改写主消息循环，使得激发状态的handles得以像消息一样被对待。通常程序中只会有一个地方调用MsgWaitForMultipleObjects()，而这个调用存在于消息循环中。

注意：

1. 在收到WM_QUIT之后，Windows仍然会传送消息给你，如果要在收到WM_QUIT之后等待所有线程结束，必须继续处理你的消息，否则窗口会变得反应迟钝，而且没有重绘能力。

2.MsgWaitForMultipleObjects()不允许handles数组中有缝隙产生。所以当某个handle被激发了时，应该在下一次调用MsgWaitForMultipleObjects之前先把handles数组做个整理、紧压，不要只是把数组中的handle设为NULL

3.如果有另一个线程改变了对象数组，而那是你正在等待的，那么需要一种方法，可以强迫MsgWaitForMultipleObjects返回，并重新开始，以包含新的handle

三、MsgWaitForMultipleObjectsEx

函数功能:阻塞时仍可以响应消息

函数原型

DWORD MsgWaitForMultipleObjectsEx( 

DWORD nCount, // 句柄数组中句柄数目

LPHANDLE pHandles, // 指向句柄数组的指针

DWORD dwMilliseconds, // 以毫秒计的超时值

DWORD dwWakeMask, // 要等待的输入事件类型

DWORD dwFlags // 等待标志

); 

参数

nCount，指定pHandles指向的数组中的对象句柄数目。最大对象数目是MAXIMUM_WAIT_OBJECTS-1 

pHandles ，指向一个对象句柄数组。要得到可以使用的对象句柄类型清单，请查看备注部分。数组中可以包含多种对象类型。 Windows NT: 数组中句柄必须拥有SYNCHRONIZE访问权。要得到更多相关信息，请查阅MSDN中Standard Access Rights。

dwMilliseconds ，指定以毫秒计的超时值。即使参数dwWakeMask与dwFlags中指定的条件未满足，超时后函数仍然返回。如果dwMilliseconds值为0，函数测试指定的对象状态并立即返回。如果dwMilliseconds值为INFINITE，函数超时周期为无穷大。

dwWakeMask ，指定被加到对象句柄数组中的输入事件对象句柄的对象类型。这个参数可以是下面列出值的任意组合：

值含义

QS_ALLEVENTS 

WM_TIMER, WM_PAINT, WM_HOTKEY输入消息或登记消息(posted message)在消息队列中

QS_ALLINPUT 

任何消息在消息队列中

QS_ALLPOSTMESSAGE

登记消息(在此处列出的除外)在消息队列中

QS_HOTKEY 

WM_HOTKEY消息在消息队列中

QS_INPUT

输入消息在消息队列中

QS_KEY 

WM_KEYUP,WM_KEYDOWN,WM_SYSKEYUP或WM_SYSKEYDOWN消息在消息队列中

QS_MOUSE 

WM_MOUSEMOVE消息或鼠标点击消息(WM_LBUTTONUP,WM_RBUTTONDOWN等)在消息队列中

QS_MOUSEBUTTON 

鼠标点击消息(WM_LBUTTONUP,WM_RBUTTONDOWN等)在消息队列中

QS_MOUSEMOVE 

WM_MOUSEMOVE消息在消息队列中

QS_PAINT 

WM_PAINT消息在消息队列中

QS_POSTMESSAGE

登记消息(在此处列出的除外)在消息队列中

QS_SENDMESSAGE 

由另一个线程或应用发送的消息在消息队列中

QS_TIMER

WM_TIMER消息在消息队列中

dwFlags ，指定等待类型。这个参数可以是下面列出值的任意组合：

值含义

0

当对象中任意一个变为有信号状态则函数返回。返回值指出是哪个对象状态的改变导致函数返回。

MWMO_WAITALL 

只有当pHandles数组中所有对象有信号时函数返回

MWMO_ALERTABLE 

调用QueueUserAPC加入一个APC将导致函数返回

MWMO_INPUTAVAILABLE 

只适用于Windows 98, Windows NT 5.0及其以后版本: 消息队列中存在输入函数将返回，甚至于输入已经被另一个函数检测过了，如PeekMessage函数

返回值

假如函数成功，返回值表明引起函数返回的事件。成功的函数值是下面中的一个：

值含义

WAIT_OBJECT_0 到(WAIT_OBJECT_0 + nCount - 1)

假如MWMO_WAITALL标志置位，返回值指明所有指定的对象处于有信号状态。返回值减去WAIT_OBJECT_0就是pHandles数组中引起函数返回的对象的索引

WAIT_OBJECT_0 + nCount

有新的在dwWakeMask参数中指定的输入类型存在于输入队列中。函数如：PeekMessage,GetMessage,GetQueueStatus与WaitMessage将队列中的消息标记为旧的。因此，当你在这些函数之后调用MsgWaitForMultipleObjectsEx，函数将不会返回，除非有新的被指定的输入到达。当一个需要该线程活动的系统事件发生时也将返回该值，例如前台活动。因此即使没有相应的输入发生或dwWaitMask置0，MsgWaitForMultipleObjectsEx也可以返回。如果发生这种情况，那么在再次调用MsgWaitForMultipleObjectsEx之前要调用PeekMessage或GetMessage处理系统事件。

WAIT_ABANDONED_0 到(WAIT_ABANDONED_0 + nCount - 1) 

假如MWMO_WAITALL标志置位，返回值指明所有指定的对象处于有信号状态并且至少其中的一个是一个被舍弃的(abandoned)互斥对象。另外，返回值减去WAIT_ABANDONED_0即是pHandles数组中引起函数返回的被舍弃的互斥对象的索引

WAIT_IO_COMPLETION

等待被一加入队列中的用户模式异步过程调用(user-mode asynchronous procedure call (APC))所终止

WAIT_TIMEOUT 

超时，但dwFlags与dwWakeMask参数中条件未满足

假如函数调用失败，返回值是0xFFFFFFFF。若想获得更多的错误信息，请调用GetLastError函数。

备注

MsgWaitForMultipleObjectsEx函数检测是否dwWakeMask与dwFlags参数中指定的条件满足。假如条件未满足，调用线程进入高效的等待状态。线程在等待条件之一满足或超时时只用很少的处理器时间。

返回前，等待函数会修改某些异步对象的状态。修改只会针对那些置信号状态后会导致函数返回的对象，例如系统将信号对象(semaphore)的引用计数减一。当dwFlags为零并且多个对象处于信号状态时，函数选择对象中的一个来确保等待；未被选中的对象的状态不受影响。

MsgWaitForMultipleObjectsEx函数可以在pHandles数组中指定下列的对象类型：

改变通知(Change notification) 

控制台输入

事件

作业(job) 

互斥

进程

信号

线程

等待计时器

要获取更多信息，请参阅Synchronization Objects

QS_ALLPOSTMESSAGE与QS_POSTMESSAGE标志被消除时是有区别的。QS_POSTMESSAGE在你调用GetMessage或PeekMessage时被消除，而不管你是否正在过滤消息。QS_ALLPOSTMESSAGE在你调用不过滤消息(wMsgFilterMin与wMsgFilterMax皆为零)的GetMessage或PeekMessage时被消除。这在你调用PeekMessage多次以获得不同区域的消息时会很有用。

Windows CE:Windows CE不支持QS_HOTKEY赋予dwWakeMask参数，也不支持MWMO_WAITALL和MWMO_ALERTABLE标志赋予dwFlags参数。

MsgWaitForMultipleObjectsEx复制句柄表，将消息队列事件加入其中，然后调用WaitForMultipleObjects 

示例代码段

//一段代码，等待线程与事件对象及消息，超时值为2000毫秒

CWinThread* pThread = AfxBeginThread((AFX_THREADPROC)YourThreadFun, NULL);

HANDLE hThreadAndEvent[ 2 ];

hThreadAndEvent[ 0 ] = pThread->m_hThread;

hThreadAndEvent[ 1 ] = ::CreateEvent( NULL, FALSE, FALSE, NULL );

DWORD dwReturn = ::MsgWaitForMultipleObjectsEx(2,

hThreadAndEvent,

2000,//2秒醒来一次

QS_ALLEVENTS,

MWMO_INPUTAVAILABLE

);

if ( dwReturn==WAIT_OBJECT_0 )

{

//线程对象通知

}

if ( dwReturn==WAIT_OBJECT_0+1 )

{

//事件对象通知

}

if ( dwReturn == WAIT_OBJECT_0+2 )

{

//消息

}

if ( dwReturn == WAIT_TIMEOUT )

{

//超时

}

