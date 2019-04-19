# Windows 临界区，内核事件，互斥量，信号量 - 三少GG - CSDN博客
2012年06月04日 22:37:12[三少GG](https://me.csdn.net/scut1135)阅读数：1945
# [Windows 临界区，内核事件，互斥量，信号量。](http://www.cnblogs.com/vieri122/archive/2009/09/10/1564104.html)
**临界区，内核事件，互斥量，信号量，都能完成线程的同步，在这里把他们各自的函数调用，结构定义，以及适用情况做一个总结。**
**临界区:**
**适用范围：它只能同步一个进程中的线程，不能跨进程同步。一般用它来做单个进程内的代码快同步,效率比较高。**
相关结构：CRITICAL_SECTION _critical
相关方法：
/*初始化，最先调用的函数。没什么好说的，一般windows编程都有类似初始化的方法*/
InitializeCriticalSection(& _critical)
/*释放资源，确定不使用_critical时调用，一般在程序退出的时候调用。如果以后还要用_critical，则要重新调用InitializeCriticalSection
*/
DeleteCriticalSection(& _critical)
/*
把代码保护起来。调用此函数后，他以后的资源其他线程就不能访问了。
*/
EnterCriticalSection（& _critical）
/*
离开临界区，表示其他线程能够进来了。注意EnterCritical和LeaveCrticalSection必须是成对出现的!当然除非你是想故意死锁！
*/
LeaveCriticalSection(& _critical)
代码Demo
```
#include "stdafx.h"
int thread_count = 0;
/*Mutex mutex1;*/
**CRITICAL_SECTION g_cs;**
DWORD CALLBACK thread_proc(LPVOID params)
{
for(int i = 0;
 i < 10; ++i)
{
//synchronized(mutex1)
**EnterCriticalSection(****&****g_cs);**
{
for(char c = 'A';
 c <= 'Z'; ++c)
{
printf("%c",c);
}
printf("\n");
}
**LeaveCriticalSection(****&****g_cs);**
}
thread_count--;
return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
**InitializeCriticalSection(****&****g_cs);**
thread_count = 4;
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
while (thread_count) 
Sleep(0);
getchar();
**DeleteCriticalSection(****&****g_cs);**
return 0;
```
**内核事件：**
**适用范围：多用于线程间的通信，可以跨进程同步。**
相关结构: HANDLE hEvent;
相关方法：
/*
初始化方法，创建一个事件对象，第一个参数表示安全属性，一般情况下，遇到这类型的参数直接给空就行了，第二个参数是否是人工重置。（内核时间有两种工作模式：人工重置和自动重置。其区别会在下面提到。）。第三个参数是初始状态，第四个参数事件名称。
*/
hEvent = CreateEvent（NULL,FALSE,FALSE,NULL);
/*
等待单个事件置位，即线程会在这个函数阻塞直到事件被置位，SetEvent。
如果是自动重置事件，则在此函数返返回后系统会自动调用ResetEvent（hEvnet），重置事件，保证其他线程不能访问。
如果是人工重置事件，则在此函数返回以后，系统的其他线程能继续访问。
第二个参数说明等待事件，INIFINET表示一直等待。
*/
WatiForSingleObject(hEvent,INIFINET)
/*
置位事件，只要使事件置位线程才能进去访问。即WatiForSingleObject(hEvent,INIFINET)才返回
*/
SerEvent(hEvent);
/*
重置事件,使得WatiForSingleObject（）不返回
*/
ResetEvent(hEvent)
/*
等待多个事件对象。参数nCount指定了要等待的内核对象的数目，存放这些内核对象的数组由lpHandles来指向。fWaitAll对指定的这nCount个内核对 象的两种等待方式进行了指定，为TRUE时当所有对象都被通知时函数才会返回，为FALSE则只要其中任何一个得到通知就可以返回。 dwMilliseconds在这里的作用与在WaitForSingleObject（）中的作用是完全一致的。如果等待超时，函数将返回 WAIT_TIMEOUT。如果返回WAIT_OBJECT_0到WAIT_OBJECT_0+nCount-1中的某个值，则说明所有指定对象的状态均
 为已通知状态（当fWaitAll为TRUE时）或是用以减去WAIT_OBJECT_0而得到发生通知的对象的索引（当fWaitAll为FALSE 时）
*/
WaitForMultiObjects(DWORD nCount, // 等待句柄数
CONST HANDLE *lpHandles, // 句柄数组首地址
BOOL fWaitAll, // 等待标志
　DWORD dwMilliseconds // 等待时间间隔)
/*
打开一个命名的事件对象，可以用来跨进程同步
*/
HANDLE OpenEvent(
DWORD dwDesiredAccess, // 访问标志
BOOL bInheritHandle, // 继承标志
LPCTSTR lpName // 指向事件对象名的指针
);
测试代码
```
#include "stdafx.h"
/*#include "Mutex.h"*/
int thread_count = 0;
/*Mutex mutex1;*/
/*CRITICAL_SECTION g_cs;*/
HANDLE hEvent;
DWORD CALLBACK thread_proc(LPVOID params)
{
for(int i = 0;
 i < 10; ++i)
{
//synchronized(mutex1)
//EnterCriticalSection(&g_cs);
WaitForSingleObject(hEvent,INFINITE);
{ 
for(char c = 'A';
 c <= 'Z'; ++c)
{
printf("%c",c);
Sleep(1);
}
printf("\n");
}
SetEvent(hEvent);
//LeaveCriticalSection(&g_cs);
}
thread_count--;
return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
//InitializeCriticalSection(&g_cs);
hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
SetEvent(hEvent);
thread_count = 4;
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
while (thread_count) 
Sleep(0);
getchar();
//DeleteCriticalSection(&g_cs);
return 0;
}
```
**互斥量：**
**适用范围：可以跨进程同步，还可以用来保证程序只有一个实例运行（创建命名互斥量）,也可以用来做线程间的同步**
相关结构：HANDLE hMutex;
相关方法：
/*
创建互斥量，初始化的工作
参数一为安全选项，一般为空
参数二表示当前互斥量是否属于某个线程，一般为空
参数三互斥量的名称，如果需要跨进程同步或者需要保证程序只有一个实例运行，则需要设置，其他情况一般为空。
*/
CreateMutex(NULL,FALSE,NULL)
WaitForSingleObject(hMutex,INIFINET);//同事件对象
/*
释放互斥量，以使得其他线程可以访问。
*/
ReleaseMutex(hMutex)
/*
在互斥对象通知引 起调用等待函数返回时，等待函数的返回值不再是通常的WAIT_OBJECT_0（对于WaitForSingleObject（）函数）或是在 WAIT_OBJECT_0到WAIT_OBJECT_0+nCount-1之间的一个值（对于WaitForMultipleObjects（）函 数），而是将返回一个WAIT_ABANDONED_0（对于WaitForSingleObject（）函数）或是在WAIT_ABANDONED_0 到WAIT_ABANDONED_0+nCount-1之间的一个值（对于WaitForMultipleObjects（）函数）。
*/
WaitForMultiObjects(DWORD nCount, // 等待句柄数
CONST HANDLE *lpHandles, // 句柄数组首地址
BOOL fWaitAll, // 等待标志
　DWORD dwMilliseconds // 等待时间间隔)
/*
打开一个已经创建好了的命名互斥量，用于跨进程同步
*/
HANDLE OpenMutex(
DWORD dwDesiredAccess, // 访问标志
BOOL bInheritHandle, // 继承标志
LPCTSTR lpName // 互斥对象名
);
测试demo
```
[](http://www.cnblogs.com/vieri122/archive/2009/09/10/1564104.html)
#include "stdafx.h"
/*#include "Mutex.h"*/
int thread_count = 0;
/*Mutex mutex1;*/
/*CRITICAL_SECTION g_cs;*/
//HANDLE hEvent;
HANDLE hMutex;
DWORD CALLBACK thread_proc(LPVOID params)
{
for(int i = 0;
 i < 10; ++i)
{
//synchronized(mutex1)
//EnterCriticalSection(&g_cs);
WaitForSingleObject(hMutex,INFINITE);
//WaitForSingleObject(hEvent,INFINITE);
//{ 
for(char c = 'A';
 c <= 'Z'; ++c)
{
printf("%c",c);
Sleep(1);
}
printf("\n");
//}
//SetEvent(hEvent);
ReleaseMutex(hMutex);
//LeaveCriticalSection(&g_cs);
}
thread_count--;
return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
//InitializeCriticalSection(&g_cs);
//hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
//SetEvent(hEvent);
hMutex = CreateMutex(NULL,FALSE,NULL);
thread_count = 4;
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
CreateThread(0, 0, thread_proc, 0, 0, 0);
while (thread_count) 
Sleep(0);
getchar();
//DeleteCriticalSection(&g_cs);
return 0;
}
[](http://www.cnblogs.com/vieri122/archive/2009/09/10/1564104.html)
```
**注意事项**：所有的同步操作的必须成对存在，即锁一对象，一定要释放一个对象。但是如果在保护的代码快中发生异常，程序流程发生意外跳转而没有释放锁对象，导致程序进入死锁。所以在程序中必要的异常处理是必须的，但是C++中没有finally这样的关键字来保证不管是否发生异常都会执行的代码快。那怎么办呢？这就需要对C++的异常加一些小技巧来处理了......
.....................................................
下班了，改天另写一篇**C++异常**处理来解决上述问题。
希望天一直是蓝的，真的很美......
++++++++++++++++++++++++++++++++++++++++++++++
[Visual C++线程同步技术剖析:临界区，时间，信号量，互斥量](http://www.cppblog.com/killsound/archive/2006/12/08/16147.html)
信号量（Semaphore）内核对象对线程的同步方式与前面几种方法不同，它允许多个线程在同一时刻访问同一资源，但是需要限制在同一时刻访问此资源的最大线程数目。在用CreateSemaphore（）创建信号量时即要同时指出允许的最大资源计数和当前可用资源计数。一般是将当前可用资源计数设置为最大资源计数，每增加一个线程对共享资源的访问，当前可用资源计数就会减1，只要当前可用资源计数是大于0的，就可以发出信号量信号。但是当前可用计数减小到0时则说明当前占用资源的线程数已经达到了所允许的最大数目，不能在允许其他线程的进入，此时的信号量信号将无法发出。线程在处理完共享资源后，应在离开的同时通过ReleaseSemaphore（）函数将当前可用资源计数加1。在任何时候当前可用资源计数决不可能大于最大资源计数。
![windows信号量使用 - 幽绿丛林 - I choose](http://www.yesky.com/image20010518/110095.jpg)
图3 使用信号量对象控制资源
下面结合图例3来演示信号量对象对资源的控制。在图3中，以箭头和白色箭头表示共享资源所允许的最大资源计数和当前可用资源计数。初始如图（a）所示，最大资源计数和当前可用资源计数均为4，此后每增加一个对资源进行访问的线程（用黑色箭头表示）当前资源计数就会相应减1，图（b）即表示的在3个线程对共享资源进行访问时的状态。当进入线程数达到4个时，将如图（c）所示，此时已达到最大资源计数，而当前可用资源计数也已减到0，其他线程无法对共享资源进行访问。在当前占有资源的线程处理完毕而退出后，将会释放出空间，图（d）已有两个线程退出对资源的占有，当前可用计数为2，可以再允许2个线程进入到对资源的处理。可以看出，信号量是通过计数来对线程访问资源进行控制的，而实际上信号量确实也被称作Dijkstra计数器。
　　使用信号量内核对象进行线程同步主要会用到CreateSemaphore（）、OpenSemaphore（）、ReleaseSemaphore（）、WaitForSingleObject（）和WaitForMultipleObjects（）等函数。其中，CreateSemaphore（）用来创建一个信号量内核对象，其函数原型为：
HANDLE CreateSemaphore(
　LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, // 安全属性指针
　LONG lInitialCount, // 初始计数
　LONG lMaximumCount, // 最大计数
　LPCTSTR lpName // 对象名指针
);
　　参数lMaximumCount是一个有符号32位值，定义了允许的最大资源计数，最大取值不能超过4294967295。lpName参数可以为创建的信号量定义一个名字，由于其创建的是一个内核对象，因此在其他进程中可以通过该名字而得到此信号量。OpenSemaphore（）函数即可用来根据信号量名打开在其他进程中创建的信号量，函数原型如下：
HANDLE OpenSemaphore(
　DWORD dwDesiredAccess, // 访问标志
　BOOL bInheritHandle, // 继承标志
　LPCTSTR lpName // 信号量名
);
　　在线程离开对共享资源的处理时，必须通过ReleaseSemaphore（）来增加当前可用资源计数。否则将会出现当前正在处理共享资源的实际线程数并没有达到要限制的数值，而其他线程却因为当前可用资源计数为0而仍无法进入的情况。ReleaseSemaphore（）的函数原型为：
BOOL ReleaseSemaphore(
　HANDLE hSemaphore, // 信号量句柄
　LONG lReleaseCount, // 计数递增数量
　LPLONG lpPreviousCount // 先前计数
);
　　该函数将lReleaseCount中的值添加给信号量的当前资源计数，一般将lReleaseCount设置为1，如果需要也可以设置其他的值。WaitForSingleObject（）和WaitForMultipleObjects（）主要用在试图进入共享资源的线程函数入口处，主要用来判断信号量的当前可用资源计数是否允许本线程的进入。只有在当前可用资源计数值大于0时，被监视的信号量内核对象才会得到通知。
　　信号量的使用特点使其更适用于对Socket（套接字）程序中线程的同步。例如，网络上的HTTP服务器要对同一时间内访问同一页面的用户数加以限制，这时可以为没一个用户对服务器的页面请求设置一个线程，而页面则是待保护的共享资源，通过使用信号量对线程的同步作用可以确保在任一时刻无论有多少用户对某一页面进行访问，只有不大于设定的最大用户数目的线程能够进行访问，而其他的访问企图则被挂起，只有在有用户退出对此页面的访问后才有可能进入。下面给出的示例代码即展示了类似的处理过程：
// 信号量对象句柄
HANDLE hSemaphore;
UINT ThreadProc15(LPVOID pParam)
{ 
　// 试图进入信号量关口
　WaitForSingleObject(hSemaphore, INFINITE);
　// 线程任务处理
　AfxMessageBox("线程一正在执行!");
　// 释放信号量计数
　ReleaseSemaphore(hSemaphore, 1, NULL);
　return 0;
}
UINT ThreadProc16(LPVOID pParam)
{ 
　// 试图进入信号量关口
　WaitForSingleObject(hSemaphore, INFINITE);
　// 线程任务处理
　AfxMessageBox("线程二正在执行!");
　// 释放信号量计数
　ReleaseSemaphore(hSemaphore, 1, NULL);
　return 0;
}
UINT ThreadProc17(LPVOID pParam)
{ 
　// 试图进入信号量关口
　WaitForSingleObject(hSemaphore, INFINITE);
　// 线程任务处理
　AfxMessageBox("线程三正在执行!");
　// 释放信号量计数
　ReleaseSemaphore(hSemaphore, 1, NULL);
　return 0;
}
……
void CSample08View::OnSemaphore() 
{
　// 创建信号量对象
　hSemaphore = CreateSemaphore(NULL, 2, 2, NULL);
　// 开启线程
　AfxBeginThread(ThreadProc15, NULL);
　AfxBeginThread(ThreadProc16, NULL);
　AfxBeginThread(ThreadProc17, NULL);
}
![windows信号量使用 - 幽绿丛林 - I choose](http://www.yesky.com/image20010518/110096.jpg)
图4 开始进入的两个线程
![windows信号量使用 - 幽绿丛林 - I choose](http://www.yesky.com/image20010518/110097.jpg)
图5 线程二退出后线程三才得以进入
上述代码在开启线程前首先创建了一个初始计数和最大资源计数均为2的信号量对象hSemaphore。即在同一时刻只允许2个线程进入由hSemaphore保护的共享资源。随后开启的三个线程均试图访问此共享资源，在前两个线程试图访问共享资源时，由于hSemaphore的当前可用资源计数分别为2和1，此时的hSemaphore是可以得到通知的，也就是说位于线程入口处的WaitForSingleObject（）将立即返回，而在前两个线程进入到保护区域后，hSemaphore的当前资源计数减少到0，hSemaphore将不再得到通知，WaitForSingleObject（）将线程挂起。直到此前进入到保护区的线程退出后才能得以进入。图4和图5为上述代脉的运行结果。从实验结果可以看出，信号量始终保持了同一时刻不超过2个线程的进入。
　　在MFC中，通过CSemaphore类对信号量作了表述。该类只具有一个构造函数，可以构造一个信号量对象，并对初始资源计数、最大资源计数、对象名和安全属性等进行初始化，其原型如下：
CSemaphore( LONG lInitialCount = 1, LONG lMaxCount = 1, LPCTSTR pstrName = NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL );
　　在构造了CSemaphore类对象后，任何一个访问受保护共享资源的线程都必须通过CSemaphore从父类CSyncObject类继承得到的Lock（）和UnLock（）成员函数来访问或释放CSemaphore对象。与前面介绍的几种通过MFC类保持线程同步的方法类似，通过CSemaphore类也可以将前面的线程同步代码进行改写，这两种使用信号量的线程同步方法无论是在实现原理上还是从实现结果上都是完全一致的。下面给出经MFC改写后的信号量线程同步代码：
// MFC信号量类对象
CSemaphore g_clsSemaphore(2, 2);
UINT ThreadProc24(LPVOID pParam)
{ 
　// 试图进入信号量关口
　g_clsSemaphore.Lock();
　// 线程任务处理
　AfxMessageBox("线程一正在执行!");
　// 释放信号量计数
　g_clsSemaphore.Unlock();
　return 0;
}
UINT ThreadProc25(LPVOID pParam)
{
　// 试图进入信号量关口
　g_clsSemaphore.Lock();
　// 线程任务处理
　AfxMessageBox("线程二正在执行!");
　// 释放信号量计数
　g_clsSemaphore.Unlock();
　return 0;
}
UINT ThreadProc26(LPVOID pParam)
{
　// 试图进入信号量关口
　g_clsSemaphore.Lock();
　// 线程任务处理
　AfxMessageBox("线程三正在执行!");
　// 释放信号量计数
　g_clsSemaphore.Unlock();
　return 0;
}
……
void CSample08View::OnSemaphoreMfc() 
{
　// 开启线程
　AfxBeginThread(ThreadProc24, NULL);
　AfxBeginThread(ThreadProc25, NULL);
　AfxBeginThread(ThreadProc26, NULL);
}
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [信号量浅析](http://blog.csdn.net/java05/article/details/3275314)
  什么是信号量?
     信号量(Semaphore)是在多线程环境下使用的一种设施, 它负责协调各个线程, 以保证它们能够正确、合理的使用公共资源。.
我们来看看一个停车场是怎样运作的。为了简单起见，假设停车场只有三个车位，一开始三个车位都是空的。这是如果同时来了五辆车，看门人允许其中三辆不受阻碍的进入，然后放下车拦，剩下的车则必须在入口等待，此后来的车也都不得不在入口处等待。这时，有一辆车离开停车场，看门人得知后，打开车拦，放入一辆，如果又离开两辆，则又可以放入两辆，如此往复。
在这个停车场系统中，车位是公共资源，每辆车好比一个线程，看门人起的就是信号量的作用。
更进一步，信号量的特性如下：信号量是一个非负整数（车位数），所有通过它的线程（车辆）都会将该整数减一（通过它当然是为了使用资源），当该整数值为零时，所有试图通过它的线程都将处于等待状态。在信号量上我们定义两种操作： acquire（获取许可） 和 Release（释放许可）。 当一个线程调用Wait等待）操作时，它要么通过然后将信号量减一，要么一自等下去，直到信号量大于一或超时。Release（释放）实际上是在信号量上执行加操作，对应于车辆离开停车场，该操作之所以叫做“释放”是应为加操作实际上是释放了由信号量守护的资源
下面是一个用信号量实现同一时刻只有两个线程方法某一方法的例子
package com.sr178.test;
import java.util.concurrent.Semaphore;
public class Test {
    private final Semaphore available = new Semaphore(2);
    public void methodA() {
        try {
            available.acquire();
            methodB();
            available.release();
        } catch (InterruptedException ex) {
        }
    }
    public void methodB() {
        System.out.println(Thread.currentThread().getName() + ": " + "Entering methodB ...");
        try {
            Thread.sleep(3000);
        } catch (InterruptedException ex) {
        }
        System.out.println(Thread.currentThread().getName() + ": " + "Leaving methodB ...");
    }
    public static void main(String[] args) {
        final Test t = new Test();
        for (int i = 0; i < 10; i++) {
            new Thread() {
                @Override
                public void run() {
                    t.methodA();
                }
            }.start();
        }
    }
}
