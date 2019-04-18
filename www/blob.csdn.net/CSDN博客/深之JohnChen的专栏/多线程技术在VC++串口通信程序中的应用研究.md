# 多线程技术在VC++串口通信程序中的应用研究 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:13:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2887标签：[vc++																[多线程																[file																[mfc																[windows																[任务](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)
个人分类：[VC](https://blog.csdn.net/byxdaz/article/category/7211574)


多线程技术在VC++串口通信程序中的应用研究

1 概述

　　在现代的各种实时监控系统和通信系统中，在Windows 9X/NT下利用VC++对RS-232串口编程是常用的手段。Windows 9X/NT是抢先式的多任务操作系统，程序对CPU的占用时间由系统决定。多任务指的是系统可以同时运行多个进程，每个进程又可以同时执行多个线程。进程是应用程序的运行实例，拥有自己的地址空间。每个进程拥有一个主线程， 同时还可以建立其他的线程。线程是操作系统分配CPU时间的基本实体，每个线程占用的CPU时间由系统分配，系统不停的在线程之间切换。进程中的线程共享进程的虚拟地址空间，可以访问进程的资源，处于并行执行状态，这就是多线程的基本概念。 

　　2 VC++对多线程的支持

　　使用MFC开发是较普遍的VC++编程方法。在VC++6.0下，MFC应用程序的线程由CWinThread对象表示。VC++把线程分为两种：用户界面线程和工作者线程。用户界面线程能够提供界面和用户交互，通常用于处理用户输入并相应各种事件和消息；而工作者线程主要用来处理程序的后台任务。

　　程序一般不需要直接创建CWinThread对象，通过调用AfxBeginThread()函数就会自动创建一个CWinThread对象，从而开始一个进程。创建上述的两种线程都利用这个函数。

　　线程的终止取决于下列事件之一：线程函数返回；线程调用ExitThread()退出；异常情况下用线程的句柄调用TerminateThread()退出；线程所属的进程被终止。

3 多线程在串口通信中的应用

　　3.1 串口通信对线程同步的要求

　　因为同一进程的所有线程共享进程的虚拟地址空间，而在Windows 9X/NT系统下线程是汇编级中断，所以有可能多个线程同时访问同一个对象。这些对象可能是全局变量，MFC的对象，MFC的API等。串口通信的几个特点决定了必须采用措施来同步线程的执行。

　　串口通信中，对于每个串口对象，只有一个缓冲区，发送和接收都要用到，必须建立起同步机制，使得在一个时候只能进行一种操作，否则通信就会出错。

　　进行串口通信处理的不同线程之间需要协调运行。如果一个线程必须等待另一个线程结束才能运行，则应该挂起该线程以减少对CPU资源的占用，通过另一进程完成后发出的信号(线程间通信)来激活。

　　VC++提供了同步对象来协调多线程的并行，常用的有以下几种：

　　　CSemaphore：信号灯对象，允许一定数目的线程访问某个共享资源，常用来控制访问共享资源的线程数量。

　　　Cmutex：互斥量对象，一个时刻至多只允许一个线程访问某资源，未被占用时处于有信号状态，可以实现对共享资源的互斥访问。

　　　CEvent：事件对象，用于使一个线程通知其他线程某一事件的发生，所以也可以用来封锁对某一资源的访问，直到线程释放资源使其成为有信号状态。适用于某一线程等待某事件发生才能执行的场合。

　　　CCriticalSection：临界区对象，将一段代码置入临界区，只允许最多一个线程进入执行这段代码。一个临界区仅在创建它的进程中有效。

　　3.2 等待函数

　　Win32 API提供了能使线程阻塞其自身执行的等待函数，等待其监视的对象产生一定的信号才停止阻塞，继续线程的执行。其意义是通过暂时挂起线程减少对CPU资源的占用。在某些大型监控系统中，串口通信只是其中事务处理的一部分，所以必须考虑程序执行效率问题，当串口初始化完毕后，就使其处于等待通信事件的状态，减少消耗的CPU时间，提高程序运行效率。

　　常用的等待函数是WaitForSingleObject()和WaitForMultipleObjects()，前者可监测单个同步对象，后者可同时监测多个同步对象。

　　3.3 串口通信的重叠I/O方式

　　MFC对于串口作为文件设备处理，用CreateFile()打开串口，获得一个串口句柄。打开后SetCommState()进行端口配置，包括缓冲区设置，超时设置和数据格式等。成功后就可以调用函数ReadFile()和WriteFile()进行数据的读写，用WaitCommEvent()监视通信事件。CloseHandle()用于关闭串口。

　　在ReadFile()和WriteFile()读写串口时，可以采取同步执行方式，也可以采取重叠I/O方式。同步执行时，函数直到执行完毕才返回，因而同步执行的其他线程会被阻塞，效率下降；而在重叠方式下，调用的读写函数会立即返回，I/O操作在后台进行，这样线程就可以处理其他事务。这样，线程可以在同一串口句柄上实现读写操作，实现"重叠"。

　　使用重叠I/O方式时，线程要创建OVERLAPPED结构供读写函数使用，该结构最重要的成员是hEvent事件句柄。它将作为线程的同步对象使用，读写函数完成时hEvent处于有信号状态，表示可进行读写操作；读写函数未完成时，hEvent被置为无信号。

4 程序关键代码的实现

　　程序专门建立了一个串口通信类，下面给出关键成员函数的核心代码。

BOOL InitComm file://串口初始化，这里只给出关键步骤的代码，下同
{
　HANDLE m_hComm;
　COMMTIMEOUTS m_CommTimeouts;
　m_hComm = CreateFile("COM1", file://在这里只使用串口1
　　GENERIC_READ | GENERIC_WRITE, file://打开类型为可读写
　　0, file://以独占模式打开串口
　　NULL, file://不设置安全属性
　　OPEN_EXISTING,
　　FILE_FLAG_OVERLAPPED, file://重叠I/O方式
　　0);
　if (m_hComm == INVALID_HANDLE_VALUE) file://打开不成功
　　{return FALSE;}
　m_CommTimeouts.ReadIntervalTimeout = 1000; 
　file://进行超时设置，读者应根据自己的实际需要设置
　m_CommTimeouts.ReadTotalTimeoutMultiplier = 500;
　m_CommTimeouts.ReadTotalTimeoutConstant = 5000;
　m_CommTimeouts.WriteTotalTimeoutMultiplier = 500;
　m_CommTimeouts.WriteTotalTimeoutConstant = 5000;
　if (!SetCommTimeouts(m_hComm, &m_CommTimeouts))
　　{CloseHandle(m_hComm);
　　　return FALSE;}
　PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); file://清缓冲
　return TRUE;
} 

以上是专门针对COM1的初始化，如果要利用同一函数对不同串口初始化，则要在初始化前先进入代码临界区，以保证在某一时刻只进行一个串口的初始化。

　　在串口初始化成功后，就可以建立监控线程处理串口通信事件。下面是该线程的关键代码。

UINT CommThread(LPVOID pParam) file://用于监控串口的工作者线程
{
　BOOL bResult = FALSE;
　if (m_hComm) file://查看端口是否打开，这里m_hComm同上，作者在这里做了简化
　　PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | 　　　PURGE_TXABORT);
　　for (;;) file://只要线程运行，就处于监视端口行为的无限循环
　　{
　　　bResult = WaitCommEvent(m_hComm, &Event, &m_ov); 
　　　file://m_ov是OVERLAPPED类型的成员变量
　　　if (!bResult)
　　　　{ file://进行出错处理}
　　　else
　　　{
　　　　Event = WaitForMultipleObjects(4, m_hEvent, FALSE, INFINITE);
　　　　file://无限等待设定的事件发生，数组m_hEvent根据需要定义了须响应的接收，发送，关闭端口事件和OVERLAPPED类型的hEvent事件
　　　　switch (Event)
　　　　{ file://读写事件的响应处理过程，在此略}
　　　　}
　　　　return 0;
　} 

这样监控主程序就可以使用AfxBeginThread()函数来产生CommThread串口监控线程。如果要实现对所有端口的同时监控，可以分别对端口建立监控线程。

　　5 小结

　　作为一个机房监控系统的组成部分，本串口通信程序在VC++6.0下编译通过，在使用windows 98/NT的局域网里运行良好。


