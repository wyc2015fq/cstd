# Windows CE 进程、线程和内存管理（二） - ljx0305的专栏 - CSDN博客
2008年03月11日 22:22:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：921
**Windows CE 进程、线程和内存管理（二）**
作者：[付林林](mailto:windowsce@tom.com)
**二、同步**
　　在多数情况下，线程之间难免要相互通信、相互协调才能完成任务。比如，当有多个线程共同访问同一个资源时，就必须保证一个线程正读取这个资源数据的时候，其它线程不能够修改它。这就需要线程之间相互通信，了解对方的行为。再有当一个线程要准备执行下一个任务之前，它必须等待另一个线程终止才能运行，这也需要彼此相互通信。实际开发过程中，线程间需要同步的情况非常多。Windows CE.NET给我们提供了很多的同步机制，熟练的掌握这些机制并合理运用会使线程之间的同步更合理、更高效。进程间的通信机制在下一篇文章中讲解。
　　Windows CE.NET具有两种运行模式：用户模式和内核模式。并且允许一个运行于用户模式的应用程序随时切换为内核模式，或切换回来。线程同步的有些解决办法运行在用户模式，有些运行在内核模式。《Windows核心编程》上说从用户模式切换到内核模式再切换回来至少要1000个CPU周期。我查看过CE下API函数SetKMode的源码，这个函数用于在两种模式间切换，改变模式只需修改一些标志，至于需要多少个CPU周期很难确定。但至少可以肯定来回切换是需要一定时间的。所以在选择同步机制上应该优先考虑运行在用户模式的同步解决办法。
1、互锁函数
　　互锁函数运行在用户模式。它能保证当一个线程访问一个变量时，其它线程无法访问此变量，以确保变量值的唯一性。这种访问方式被称为原子访问。互锁函数及其功能见如下列表：
|**函数**|**参数和功能**|
|----|----|
|InterlockedIncrement|参数为PLONG类型。此函数使一个LONG变量增1|
|InterlockedDecrement|参数为PLONG类型。此函数使一个LONG变量减1|
|InterlockedExchangeAdd|参数1为PLONG类型，参数2为LONG类型。此函数将参数2赋给参数1指向的值|
|InterlockedExchange|参数1为PLONG类型，参数2为LONG类型。此函数将参数2的值赋给参数1指向的值|
|InterlockedExchangePointer|参数为PVOID* 类型，参数2为PVOID类型。此函数功能同上。具体参见帮助|
|InterlockedCompareExchange|参数1为PLONG类型，参数2为LONG类型，参数3为LONG类型。此函数将参数1指向的值与参数3比较，相同则把参数2的值赋给参数1指向的值。不相同则不变|
|InterlockedCompareExchangePointer|参数1为PVOID* 类型，参数2为PVOID类型，参数3为PVOID。此函数功能同上。具体参见帮助|
2、临界区
　　临界区对象运行在用户模式。它能保证在临界区内所有被访问的资源不被其它线程访问，直到当前线程执行完临界区代码。除了API外，MFC也对临界区函数进行了封装。临界区相关函数： 
void  InitializeCriticalSection ( LPCRITICAL_SECTION );
void  EnterCriticalSection ( LPCRITICAL_SECTION );
void  LeaveCriticalSection ( LPCRITICAL_SECTION );
void  DeleteCriticalSection ( LPCRITICAL_SECTION );      
举例如下：
void CriticalSectionExample (void)
{
    CRITICAL_SECTION  csMyCriticalSection;
    InitializeCriticalSection (&csMyCriticalSection);  ///初始化临界区变量
    __try 
    {
       EnterCriticalSection (&csMyCriticalSection);   ///开始保护机制
       ///此处编写代码
    }
    __finally   ///异常处理，无论是否异常都执行此段代码
    {
       LeaveCriticalSection (&csMyCriticalSection);  ///撤销保护机制
    }
} 
      
MFC类使用更简单：
CCriticalSection  cs;
cs.Lock();
///编写代码
cs.Unlock();      
　　使用临界区要注意的是避免死锁。当有两个线程，每个线程都有临界区，而且临界区保护的资源有相同的时候，这时就要在编写代码时多加考虑。 
3、事件对象 
　　事件对象运行在内核模式。与用户模式不同，内核模式下线程利用等待函数来等待所需要的事件、信号，这个等待过程由操作系统内核来完成，而线程处于睡眠状态，当接收到信号后，内核恢复线程的运行。内核模式的优点是线程在等待过程中并不浪费CPU时间，缺点是从用户模式切换到内核模式需要一定的时间，而且还要切换回来。在讲解事件对象前应该先谈谈等待函数。等待函数有四个。具体参数和功能见下表：
|**函数**|**参数和功能**|
|----|----|
|WaitForSingleObject|参数1为HANDLE类型，参数2为DWORD类型。此函数等待参数1标识的事件，等待时间为参数2的值，单位ms。如果不超时，当事件成为有信号状态时，线程唤醒继续运行。|
|WaitForMultipleObjects|参数1为DWORD类型，参数2为HANDLE * 类型，参数3为BOOL类型，参数4为DWORD类型。此函数等待参数2指向的数组中包含的所有事件。如果不超时，当参数3为FALSE时，只要有一个事件处于有信号状态，函数就返回这个事件的索引。参数3为TRUE时，等待所有事件都处于有信号状态时才返回。|
|MsgWaitForMultipleObjects|参数1为DWORD类型，参数2为LPHANDLE类型，参数3为BOOL类型，参数4为DWORD类型，参数5为DWORD类型。此函数功能上同WaitForMultipleObjects函数相似，只是多了一个唤醒掩码。唤醒掩码都是和消息有关的。此函数不但能够为事件等待，还能为特定的消息等待。其实这个函数就是专为等待消息而定义的。|
|MsgWaitForMultipleObjectsEx|参数1为DWORD类型，参数2为LPHANDLE类型，参数3为DWORD类型，参数4为DWORD类型，参数5为DWORD类型。此函数是MsgWaitForMultipleObjects函数的扩展。将原来函数的参数3除掉，添加参数5为标志。标志有两个值：0或MWMO_INPUTAVAILABLE。|
　　如果一个线程既要执行大量任务同时又要响应用户的按键消息，这两个专用于等待消息的函数将非常有用。
和事件有关的函数有： 
HANDLE  CreateEvent(LPSECURITY_ATTRIBUTES  lpEventAttributes, 
					BOOL bManualReset, 
					BOOL  bInitialState, 
					LPTSTR  lpName);
BOOL  SetEvent(HANDLE  hEvent );
BOOL  PulseEvent(HANDLE  hEvent);
BOOL  ResetEvent(HANDLE  hEvent);
HANDLE  OpenEvent(DWORD  dwDesiredAccess, 
				  BOOL  bInheritHandle, 
				  LPCTSTR  lpName );
　　事件对象是最常用的内核模式同步方法。它包含一个使用计数和两个BOOL变量。其中一个BOOL变量指定这个事件对象是自动重置还是手工重置。另一个BOOL变量指定当前事件对象处于有信号状态还是无信号状态。
　　函数CreateEvent创建一个事件对象，参数1必须为NULL，参数2指定是否手工重新设置事件对象的状态。如果为FALSE，当等待函数接到信号并返回后此事件对象被自动置为无信号状态。这时等待此事件对象的其它线程就不会被唤醒，因为事件对象已经被置为无信号状态。如果参数2设置为TRUE，当等待函数接到信号并返回后事件对象不会被自动置于无信号状态，其它等待此事件对象的线程都能够被唤醒。用ResetEvent函数可以手工将事件对象置为无信号状态。相反SetEvent函数将事件对象置为有信号状态。PulseEvent函数将事件对象置为有信号状态，然后立即置为无信号状态，在实际开发中这个函数很少使用。OpenEvent函数打开已经创建的事件对象，一般用于不同进程内的线程同步。在调用CreateEvent创建一个事件对象时，传递一个名字给参数4，这样在其它进程中的线程就可以调用OpenEvent函数并指定事件对象的名字，来访问这个事件对象。
4、互斥对象
　　互斥对象运行在内核模式。它的行为特性同临界区非常相似，在一个线程访问某个共享资源时，它能够保证其它线程不能访问这个资源。不同的是，互斥对象运行在内核模式，从时间上比临界区要慢。由于内核对象具有全局性，不同的进程都能够访问，这样利用互斥对象就可以让不同的进程中的线程互斥访问一个共享资源。而临界区只能在一个进程内有效。
和互斥相关的函数有：
HANDLE CreateMutex(LPSECURITY_ATTRIBUTES  lpMutexAttributes, 
				   BOOL  bInitialOwner, 
				   LPCTSTR  lpName);
BOOL  ReleaseMutex(HANDLE  hMutex);      
　　互斥对象包含一个引用计数，一个线程ID和一个递归计数。引用计数是所有内核对象都含有的。线程ID表示哪个线程正在使用互斥资源，当ID为0时，互斥对象发出信号。递归计数用于一个线程多次等待同一个互斥对象。函数CreateMutex创建一个互斥对象，参数1必须设置为NULL，参数2如果设置为FALSE，表示当前线程并不占有互斥资源，互斥对象的线程ID和递归计数都被设置为0，互斥对象处于有信号状态。如果设置为TRUE，表示当前线程将占有互斥资源，互斥对象的线程ID被设置为当前线程ID，递归计数被设置为1，互斥对象处于无信号状态。当调用等待函数时，等待函数检验互斥对象的线程ID是否为0，如果为0，说明当前没有线程访问互斥资源，内核将线程唤醒，并且将互斥对象的递归计数加1。当一个线程被唤醒后，必须调用函数ReleaseMutex将互斥对象的递归计数减1。如果一个线程多次调用等待函数，就必须以同样的次数调用ReleaseMutex函数。与其它Windows不同的是，和互斥相关的函数中没有OpenMutex函数。要在不同进程中访问同一互斥对象，调用CreateMutex函数，参数传递互斥对象的名称，返回这个互斥对象的句柄。
5、信标对象
　　信标对象，也叫信号灯，用于限制资源访问数量，他包含一个引用计数，一个当前可用资源数，一个最大可用资源数。如果当前可用资源数大于0，信标对象处于有信号状态。当可用资源数等于0，信标对象处于无信号状态。
和信标对象相关的函数：
HANDLE  CreateSemaphore(LPSECURITY_ATTRIBUTES  lpSemaphoreAttributes, 
		LONG  lInitialCount, 
		LONG  lMaximumCount, 
		LPCTSTR  lpName);
BOOL  ReleaseSemaphore(HANDLE  hSemaphore, 
		LONG  lReleaseCount, 
		LPLONG  lpPreviousCount);
　　函数CreateSemaphore的参数1为NULL，参数2为当前可用资源初始值，参数3为最大可用资源数，参数4为名字。当参数2的值等于0时，信标对象处于无信号状态，这时内核将调用等待函数的线程置于睡眠状态，如果参数2的值大于0，信标对象处于有信号状态，这时内核将调用等待函数的线程置于运行状态，并将信标对象的当前可用资源数减1。函数ReleaseSemaphore的参数1为信标对象的句柄，参数2为要释放的资源数，参数3返回原来可用资源数，调用此函数将当前可用资源数加上参数2的值。当一个线程访问完可用资源后，应该调用ReleaseSemaphore函数使当前可用资源数递增。要在不同进程中访问同一信标对象，调用CreateSemaphore函数并传递信标对象的名称，得到已经在其它进程创建的信标对象的句柄。CE下没有OpenSemaphore函数。另外我还要说明一点，等待函数默认将信标对象的当前可用资源数减1，但线程可能一次使用多个资源，这就可能出现问题了。为避免问题出现，应该遵守一个线程只使用一个资源的原则。
6、消息队列
　　Windows CE.NET允许一个应用程序或驱动程序创建自己的消息队列。消息队列既可以作为在线程之间传递数据的工具，也可以作为线程之间同步的工具。它的优点是需要很小的内存，一般只用于点到点的通信。
和消息队列相关的函数：
HANDLE  WINAPI  CreateMsgQueue(LPCWSTR  lpszName, 
			   LPMSGQUEUEOPTIONS  lpOptions);
BOOL  WINAPI  CloseMsgQueue(HANDLE  hMsgQ);
BOOL  GetMsgQueueInfo(HANDLE  hMsgQ, 
		  LPMSGQUEUEINFO  lpInfo);
HANDLE  WINAPI  OpenMsgQueue(HANDLE  hSrcProc, 
			 HANDLE  hMsgQ, 
			 LPMSGQUEUEOPTIONS  lpOptions);
BOOL  ReadMsgQueue(HANDLE  hMsgQ, 
	   LPVOID  lpBuffer, 
	   DWORD  cbBufferSize, 
	   LPDWORD  lpNumberOfBytesRead, 
	   DWORD  dwTimeout,  
	   DWORD  *pdwFlags);
BOOL  WINAPI  WriteMsgQueue(HANDLE  hMsgQ, 
			LPVOID  lpBuffer, 
			DWORD  cbDataSize,  
			DWORD  dwTimeout, 
			DWORD  dwFlags);      
　　使用CreateMsgQueue函数创建一个消息队列，传递一个MSGQUEUEOPTIONS结构指针。在这个结构中设置标志（允许队列缓冲区动态改变大小，允许直接读或者写操作而不管之前是否有过写操作或读操作）、队列允许的最大消息数、队列属性（只读或者只写）。使用WriteMsgQueue函数把一个消息写入到消息队列中。传递一个消息队列的缓冲区、消息数据的大小、写入缓冲区的超时值、标志。使用ReadMsgQueue函数把一个消息从消息队列中读出。使用CloseMsgQueue函数关闭消息队列缓冲区。使用OpenMsgQueue函数能够打开其它进程中创建的消息队列。另外可以用等待函数等待消息队列的变化。当消息队列由没有消息到有消息时，或由满消息到不满消息时唤醒调用等待函数的线程。关于消息队列我并没有实验过，MSDN上有几个简单的例子。
写作时间：2004-06-04 
未经本文作者同意，不准擅自转载本篇文章。联系作者请邮至 fllsoft@sina.com 或[windowsce@tom.com](mailto:windowsce@tom.com)
引用[http://www.vckbase.com/document/viewdoc/?id=1155](http://www.vckbase.com/document/viewdoc/?id=1155)
