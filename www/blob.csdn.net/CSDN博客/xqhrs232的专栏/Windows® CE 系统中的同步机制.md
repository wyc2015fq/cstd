# Windows® CE 系统中的同步机制 - xqhrs232的专栏 - CSDN博客
2010年02月24日 16:20:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：647标签：[wince																[thread																[semaphore																[object																[uml																[reference](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=uml&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
Windows® CE 系统中的同步机制
田海立
2006-1-17
摘要
Windows® CE 是微软系列嵌入式平台所采用的操作系统内核。本文讨论了 WinCE 进程/线程之间的同步机制，给出了它们的典型应用场景。这些同步机制包括临界区、互斥体、信号量、事件、互锁函数和消息队列等。
关键字：同步，临界区，互斥体，信号量，事件，消息队列
Keywords: Synchronization, Critical Section, Mutex, Semaphore, Event, Message Queue
适用范围：本文全部内容适用于WinCE.net 4.0及以上版本，以及以这样的WinCE做内核的Windows® Mobile系列平台。各个章节中对每种同步机制的适用版本还有说明。
知识准备：操作系统，UML，etc.
目录
摘要... 1
目录... 1
一、WinCE进程/线程模型概览... 1
二、临界区（Critical Section）... 2
三、互斥体（Mutex）... 3
四、信号量（Semaphore）... 4
五、事件（Event）... 5
六、消息队列（MsgQueue P2P）... 6
七、互锁函数（Interlocked Function）... 8
八、Wait函数... 8
总结... 10
参考资料以及进一步阅读... 10
关于作者... 10
一、WinCE进程/线程模型概览
WinCE操作系统实现了进程/线程两级管理模型。连同内核进程和系统进程，以及应用进程一起，WinCE共支持32个进程。进程还可以有自己的线程，进程默认有一个主线程，线程分为256个优先级别，WinCE调度程序按照线程优先级高低来调度。进程是WinCE中最小资源分配的单元，线程是WinCE的最小调度单元。
本文讲述的同步机制有些只适用于线程间同步，有些既能用于线程间同步又能用于进程间同步，下面讨论到某一种机制的时候，再具体详述其适用场景。
二、临界区（Critical Section）
（本节内容适用于WinCE 1.0及以上版本）
WinCE实现了操作系统理论里的临界区管理。临界区内含有对临街资源的访问。通过对临界区进行有效管理，使得某一时刻最多只能有一个线程进入临界区，实现对临界资源的保护。
考虑下面用临界区实现两个线程对临界资源互斥访问的情形。The 1st Thread和The 2nd Thread都要调用Func_CriticalSection()函数，而Func_CriticalSection()内部会对某一临界资源进行操作，为了保护这一临界资源，我们用一个WinCE的CriticalSection来实现。
图一是该解决方案的一个场景。The 1st Thread和The 2nd Thread进入临界区之前已经创建（new）并初始化（InitializeCriticalSection()）了一个临界区。试图进入该临界区的线程首先必须获得进入该临界区（通过EnterCriticalSection() / TryEnterCriticalSection()）的资格，如果临界区内没有线程，它就能进入，否则必须被挂起等待。进入临界区的线程可以对临界资源进行操作（OpOnSharedResources()）。操作完成之后退出临界区（LeaveCriticalSection()），以允许其它线程进入。图一中第一个线程进入临界区还未退出之前，第二个线程因执行EnterCriticalSection()而一直在被挂起等待，第一个线程退出临界区之后，第二个线程从等待中被唤醒，按照相应的调度机制重新竞争获得CPU，从而继续执行，完成临界区内的操作。
图一、应用临界区（CriticalSection）实现同步
利用临界区可以实现对临界资源的互斥操作，WinCE的临界区应用在同一进程内，亦即实现的是同一进程内的线程间同步，不能应用在进程之间。
三、互斥体（Mutex）
（本节内容适用于WinCE 1.01及以上版本）
互斥体（Mutex）顾名思义就是实现对共享资源实现互斥访问的。WinCE中的互斥体的使用规则如下（按线程之间的同步为例）：
◇  互斥体可以是匿名互斥体也可以是命名互斥体；
◇  线程创建互斥体的时候可以指定创建完毕它是否就立即拥有该互斥体；
◇  某一时刻最多只有一个线程拥有给定的互斥体；
◇  拥有互斥体的线程可多次获得该互斥体；
◇  线程可用CreateMutex或wait函数来获得互斥体。
看下面应用互斥体的情景。Thread1创建并拥有了一个互斥体g_hMutex[序列1&2]。互斥体g_hMutex是定义的全局量，thread2可访问到，Thread2用WaitForSingleObject()试图获得该互斥体，因为此时g_hMutex是被Thread1拥有的，所以Thread2被挂起[序列3]。Thread1执行了一些操作之后，又用wait函数试图再次获得了该互斥体，因为此时g_hMutex的拥有者还是Thread1，所以Thread1立即再次获得了该互斥体[序列4-6]。Thread1对互斥体g_hMutex保护的共享资源操作完毕，释放该互斥体[序列7]，但是因为Thread1两次获得了g_hMutex，所以g_hMutex的拥有权并没有交出。等到Thread1再次释放互斥体g_hMutex[序列8]之后，Thread1才失去了g_hMutex的拥有权，Thread2可竞争g_hMutex的拥有权，如能成功拥有，就可从等待状态被唤醒，完成对共享资源的访问操作。
图二、应用互斥体（Mutex）实现同步
不知道从上面的描述，读者有又没有看出互斥体与临界区之间的区别。使用上，它们都实现的对共享资源的互斥访问，但是临界区是多个线程对同一段程序的执行，这段程序会访问到临界资源，所以它们是同一个进程内的多个线程；而互斥体的应用情景是在线程之间独立执行，可以不是程序上的重叠，只是一个线程执行到共享资源的时候，有可能别的线程也要访问该共享资源，所以要用互斥体来保护该共享资源。
由于互斥体上述的应用范围，它不但能应用在同一进程内的线程之间，也能应用在进程之间。进程之间可以通过命名互斥体来实现。一个进程通过为CreateMutex()指定一个名字做参数来获得已经存在的互斥体的句柄，处理过程如下面程序所示。
  HANDLE  hMutex; 
  hMutex = CreateMutex (
                NULL,                       // 
                FALSE,                      // Mutex object NOT initially owned
                TEXT("NameOfMutexObject")); // Muetx Name
  if (NULL == hMutex) 
  {
    // Something wrong, deal with it here. 
  }
  else
  {
    if ( ERROR_ALREADY_EXISTS == GetLastError () ) 
    {
      // CreateMutex() opened existing mutex." 
      // ... 
    }
    else
    {
      // CreateMutex() created new mutex." 
      // ... 
    }
  }
进程获得已经存在的互斥体的句柄之后，就可以如线程之间同步规则那样来实现进程之间的互斥体使用。
四、信号量（Semaphore）
（本节内容适用于WinCE 3.0及以上版本）
信号量实体有一个数值指示当前该信号量使用情况，当前值的大小处于零和最大值之间。用下列操作原语实现信号量的同步操作（用线程间同步来说明）：
◇ P（S, num）：如果信号量当前值减去num大于零，执行该操作的线程获得信号量，可继续执行，同时信号量的当前值减小num；否则访问线程被挂起等待
◇ V（S, num）：信号量的当前值增加num（增加之后仍不大于最大值），如果有等待该信号量的线程被挂起，唤醒等待线程并按照相应的调度机制参与调度。
信号量一般用来控制某类共享资源，最大值标识该类资源的数目，执行P操作是申请一定数目这类资源，V操作是释放一定数目的这类资源。在WinCE的信号量实现中，并未实现OpenSemaphore，P操作是用wait函数来实现的，而V操作由ReleaseSemaphore来实现。
看下面用信号量来控制数量为2的某类共享资源的使用情景。
图三、用信号量（Semaphore）实现同步
Thread1创建一个控制2个共享资源的信号量[序列1&2]，并且自己用WaitForSingleObject()来申请一个资源，因为当前可用的这类资源有2个，所以它就获得了其中的一个[序列3&4]。同样地，Thread2获得了另外一个资源[序列5&6]。但是当Thread3也申请这类资源的时候，因为此时已经没有这类资源，信号量的值为零，它就被挂起[序列7]。拥有这类资源的线程释放掉一个资源[序列8&9]，并且满足能满足Thread3申请资源数目的要求，Thread3竞争获得了该资源[序列10]。
信号量是实现同步的基本方法，在几乎所有的多任务操作系统里面都做了信号量的实现，其它一些同步机制其实可以通过信号量来实现。如果把信号量的最大值和初始值均设置为1，那么它就可实现互斥体，即保证对共享资源互斥访问的保护。如果把信号量的初始值设置为0，等待别的线程ReleaseSemaphore来唤醒它，那么它就可实现事件（Event）机制。
信号量机制可以用在同一进程内的线程之间同步，也可以用在进程之间的同步。进程间同步的实现方法如同互斥体的此类实现。
五、事件（Event）
（本节内容适用于WinCE 1.0及以上版本）
WinCE系统中广泛用到事件（Event）机制来实现线程之间的协调工作，具体表现在：
◇ 通知一个线程什么时候去执行它的特定的任务
◇ 标识事件的发生
WinCE中的线程操作原语有CreateEvent()，SetEvent()/PulseEvent()，ResetEvent()等。创建Event的时候在CreateEvent()的参数中指定Event的初始状态（触发的/未触发的），还要指定事件是否手动复位（手动复位是只有用ResetEvent()才能把事件状态显式地设置为未触发的，自动复位是等待该事件的线程等待事件到来之后，系统自动把该事件的状态复位为未触发的）。线程等待事件仍然用wait函数。
下面是使用Event同步的简单情况：
图四、用事件（Event）实现同步
线程Thread1执行过程中，要等到某个条件满足（事件触发），所以它创建了一个事件Event（参数设置为：手动复位，初始条件为未触发的），用WaitForSingleObject()来等待这个事件。线程Thread2执行了一些操作之后，满足了Thread1的条件，用SetEvent来触发该事件。
       除了可以用SetEvent()来触发事件之外，也可以用PulseEvent()来触发，区别是PulseEvent()触发该事件之后把它又复位。
       另外，也可以把命名事件用于进程之间的同步。实现方法同互斥体中的描述。
六、消息队列（MsgQueue P2P）
（本节内容适用于WinCE.net 4.0及以上版本）
消息队列通信机制如同建立了一个管道，管道的双方通过分别建立到管道的两端，与管道的读端口建立连接的进程可以从该端口读取消息（Message），与管道的写端口建立连接的进程可以写入消息（Message）到管道。管道内消息组成了一个FIFO（First In First Out）的队列，从读端口读取消息是读取队列的头，写入消息到写端口是在队列尾部追加一个消息。
WinCE中关于MsgQueue的操作函数主要有：
◇ CreateMsgQueue()创建一个消息队列。在该函数的参数中指定消息队列的名字，消息队列的最大数目，每个消息的最大长度，对该消息队列可进行读还是写操作等。因为调用一次CreateMsgQueue函数，只能指定读或者写这样的二选一的消息队列，所以一般需要用相同的消息队列名字做参数两次调用该函数，分别创建读消息队列和写消息队列，它们的返回值分别被读进程和写进程用OpenMsgQueue()打开用于读取消息和写入消息。
◇ OpenMsgQueue()打开消息队列并建立与相应端口的连接。进程与读端口建立连接之后，可用返回的句柄从消息队列中读取消息；进程与写端口建立连接之后，可用返回的句柄写入消息到消息队列中。
◇ CloseMsgQueue()断开与消息队列相应的端口之间的连接，并关闭由CreateMsgQueue()或OpenMsgQueue()创建或打开的消息队列。
◇ ReadMsgQueue()如同从普通文件中读取数据一样，用于从消息队列中读取消息。可以指定读取消息时，如果消息队列为空，读进程是被挂起还是直接返回。
◇ WriteMsgQueue()如同写数据到普通文件中一样，用于写消息到消息队列中。可以指定写入消息时，如果消息队列已满，写进程是被挂起还是直接返回。
下图是MsgQueue应用的典型场景。
图五、用消息队列（MsgQueue）实现同步
这种场景下的执行过程为：
◇ 主进程MainProcess创建了名为“Reader/Writer MsgQueue”的读和写的消息队列，并分别返回hMsgQ_r_m和hMsgQ_w_m[序列1-4]。
◇读进程ReaderProcess以主进程的ProcessId和hMsgQ_r_m为参数，通过OpenMsgQueue()与MainProcess消息队列的读端口建立连接[序列5&6]。
◇ ReaderProcess与消息队列建立连接之后，用WaitForSingleOnject(hMsg_r)看消息队列中是否有消息，因为此时消息队列为空，所以ReaderProcess被挂起[序列7]。
◇写进程WriterProcess以主进程的ProcessId和hMsgQ_w_m为参数，通过OpenMsgQueue()与MainProcess消息队列的写端口建立连接[序列8&9]。
◇ WriterProcess与消息队列建立连接之后，用WaitForSingleOnject(hMsg_w)看消息队列中消息是否满，因为此时消息队列为空，未满，所以WriterProcess不会被挂起[序列10&11]。
◇ WriterProcess写消息到消息队列中[序列12&13]。
◇ 因为消息队列中已经有了消息，ReaderProcess从挂起状态被唤醒[序列14]。
◇ ReaderProcess继续执行，从消息队列中读取WriterProcess刚才写入的消息。
消息队列除可用于同步之外，主要用于进程之间的数据传递，另外消息队列也可以用于同一进程中的线程之间同步，但是既然线程之间能直接传递数据，又何必那么麻烦呢。
七、互锁函数（Interlocked Function）
（本节内容适用于WinCE 1.0及以上版本）
除了上面各节的同步方法之外，WinCE还提供了一些用于原子操作的互锁函数，这些函数在执行过程中，不会因为线程的调度引起的当前线程被抢占而打断函数内的操作。
这些函数主要有：
InterlockedIncrement 
InterlockedDecrement 
InterlockedExchange 
InterlockedTestExchange 
InterlockedCompareExchange 
InterlockedCompareExchangePointer 
InterlockedExchangePointer 
InterlockedExchangeAdd
八、Wait函数
（本节内容适用于WinCE 1.0及以上版本）
Wait函数不是特指的某一个函数，而是指wait的系列函数。wait函数并不是WinCE同步机制中的一种，但是WinCE的很多同步机制要用到wait函数，这些在前面讲述各个同步方法的时候也已有论述。
一般地，执行wait函数时，如果等待的同步对象条件不满足，那么执行wait函数的进程/线程会被挂起，当然也可以给它们设置等待的超时时间，超过给定时间，不管条件是否满足，它们会自动从等待状态苏醒。等待既可以等待某一个条件，也可以等待多个条件中的一个，WinCE不支持等待多个条件同时满足，如果有这种需要，要自己实现。
Wait函数原型如下：
DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds ); 
DWORD WaitForMultipleObjects(
  DWORD nCount,              // No. of object handles in the array. 
  CONST HANDLE* lpHandles,   // Pointer to an array of object handles. 
  BOOL fWaitAll,             // MUST be FALSE in WinCE
  DWORD dwMilliseconds       // Timeout (0, mills, or INFINITE)
); 
DWORD MsgWaitForMultipleObjects(
  DWORD nCount,             // No. of object handles in the array. 
  LPHANDLE pHandles,        // Pointer to an array of object handles. 
  BOOL fWaitAll,            // MUST be FALSE in WinCE
  DWORD dwMilliseconds,     // Timeout (0, mills, or INFINITE) 
  DWORD dwWakeMask          // Input types for which an input event object handle
);
前面讲述各种同步机制的时候都是以WaitForSingleObject()来说明的，这里就不再赘述它了。
WaitForMultipleObjects()和MsgWaitForMultipleObjects()可以用来等多个同步对象，它们之间的区别就是MsgWaitForMultipleObjects()还等待dwWakeMask参数中指定的输入事件，即这些事件发生时，等待的进程/线程也能被唤醒。
用WaitForMultipleObjects()等待的多个同步对象的句柄放在参数lpHandles数组中，同步对象的句柄的数目放在参数nCount中。dwMilliseconds指定了等待的超时参数：如果指定为0，该函数等待每个同步对象之后，不管触发与否都直接返回；如果指定为INFINITE ，该函数等待每个同步对象，直到有一个同步对象被触发，否则执行该函数的运行实体将一直被挂起；如果指定为非0，非INFINITE的一个数值，那么不管等待的同步对象是否被触发，到了指定的时间，执行该函数而被挂起的运行实体也会被唤醒。因哪个同步对象被触发而返回还是因超时而返回，可以从返回值中来判定，返回值为WAIT_TIMEOUT，是因为超时；返回值为WAIT_OBJECT_0到WAIT_OBJECT_0
 + nCount -1之间的数时，可以按顺序找到具体那个同步对象被触发。
下面是WaitForMultipleObjects的典型应用。
  HANDLE hSynchObjects[EVENT_COUNT]; 
  DWORD dwEvent; 
  /* Put event handles in hEvents */
  // ... 
  dwEvent = WaitForMultipleObjects (
                      EVENT_COUNT,        // Number of objects in an array
                      hSynchObjects,      // Array of objects
                      FALSE,              // MUST be FALSE
                      500);               // timeout, 0.5s
  switch (dwEvent) 
  {
    case WAIT_TIMEOUT: 
      // Handle for timeout
      break; 
    case WAIT_OBJECT_0 + 0: 
      // Handle the 1st event
      break; 
    case WAIT_OBJECT_0 + 1: 
      // Handle the 2nd one
      break; 
    ... 
    case WAIT_OBJECT_0 + EVENT_COUNT -1: 
      // Handle the final one
      break; 
    default: 
      // Error: Not an anticipant one, handle it. 
      break; 
  }
总结
本文探讨了WinCE中的各种同步机制的用法，并给出了它们的典型应用场景。关于它们进一步的高级话题，将在后续文章中探讨。
参考资料以及进一步阅读
1) MSDN
2) UML Reference Manual, 2nd Edition
3) Abraham Silberschatz, Peter Baer Galvin, Greg Gagne. Operating System Concepts, 6th Edition. John Wiley & Sons, Inc/高等教育出版社影印, 2002.5
4) David R. Butenhof/于磊，曾刚. Programming with POSIX Threads. Addison Wesley/中国电力出版社, 2003
关于作者
田海立，硕士，国家系统分析师，中国系统分析员协会顾问团专业顾问。您可以通过 [haili.tian@csai.cn](mailto:haili.tian@csai.cn) 或
[tianhaili@nju.org.cn](mailto:tianhaili@nju.org.cn) 与他联系，到 [http://blog.csdn.net/thl789/](http://blog.csdn.net/thl789/) 看他最新的文章。
版权声明：
◇ 本文为作者原创作品，版权归作者所有。 
◇ 为了学习和研究，可转载本文，但必须与原文的内容和格式保持一致，并给出原文的链接！[http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx](http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx](http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx](http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx](http://blog.csdn.net/thl789/archive/2006/01/17/582246.aspx)
