# 《win32多线程程序设计》学习笔记四 - 一世豁然的专栏 - CSDN博客





2017年03月23日 22:27:15[一世豁然](https://me.csdn.net/Explorer_day)阅读数：656








一、Critical Sections

1、win32中关于进程和线程的协调工作是由同步机制来完成的。




2、win32中最容易使用的一个同步机制是Critical Sections。

 1）、此一次只能有一个线程获准进入Critical Sections中（相对的也就是说资源收到了保护），实施的方式是在程序中加上“进入”和“离开”的Critical Sections的操作。如果有一个线程已经“进入Critical Sections中，另一个线程就绝不能进入同一个Critical Sections中。

 2）、Critical Sections并不是核心对象。




3、将一个类型为CRITICAL_SECTION的局部变量初始化，方法是调用以下函数：

![](https://img-blog.csdn.net/20170317205240897?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3、当你用毕Critical Sections的时候，必须调用以下函数清除：

![](https://img-blog.csdn.net/20170317205413831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4、一旦Critical Sections被初始化，调用以下函数进入：

![](https://img-blog.csdn.net/20170317205636075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





5、当线程准备好离开Critical Sections时，调用以下函数：

![](https://img-blog.csdn.net/20170317205801546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





6、不要长时间锁住一份资源。如果你一直让资源被锁定，就会阻止其它线程的执行，并把整个程序带到一个完全停止的状态。




7、避免Dangling Critical Sections。







二、死锁




三、互斥器

1、Mutex和Critical section的区别

 1 ）、锁住一个未被使用mutex，比锁住一个未被使用的Critical section，需要花费几乎100倍的时间。

 2）、Mutexs可以跨进程使用。Critical section只能在同一个进程使用。

 3）、等待一个mutex时，你可以指定“结束等待”的时间长度。但是Critical section不行。




2、Mutex和Critical section对象函数比较

![](https://img-blog.csdn.net/20170320194527957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3、为了能够跨进程使用同一个Mutex，你可以在产生mutex时指定其名称。

      注：其他进程也可能使用这个同步机制，所以mutex名称对整个系统是全局性的。




4、产生一个互斥器

![](https://img-blog.csdn.net/20170320195032370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170320195038105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、如果成功，则传回一个handle，否则传回NULL。

 2）、当你不再需要一个mutex时，你可以调用Closehandle（）将它关闭。




5、打开一个互斥器

 1）、可以使用OpenMutex（）打开（而非产生）一个原已存在mutex




6、锁住一个互斥器

 1）、欲获得一个mutex的拥有权，请使用Win32的Wait..()函数。

 2）、一旦没有任何线程拥有mutex，这个线程便处于激发状态。反过来说，当线程拥有mutex时，它便不处于激发状态。如果有某个线程正在等待一个未被激发mutex，它便出于“阻塞”状态。

 注：“mutex激发状态”是指当没有任何线程拥有该mutex而且有一个线程正在以函数Wait...（）等待该mutex，该mutex就会短暂地出现激发状态，使Wait...()得以返回。




7、释放mutex

![](https://img-blog.csdn.net/20170320200747691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、如果成功，返回TRUE，失败，返回FALSE。




8、Mutex拥有权

 1）、Mutex的拥有权并非属于哪个产生它的线程，而是那个最后对此mutex进行Waut...()操作并且尚未进行ReleaseMutex操作的线程。一个只能有一个mutex拥有该mutex。

 2）、Mutex的被摧毁和其拥有权没有什么关系。




9、处理被舍弃的互斥器

 1）、如果线程拥有一个mutex而在结束前没有调用ReleaseMutex()，mutex不会被摧毁。取而代之的是，该mutex会被视为“未被拥有“以及“未被激发”，而下一个等待中的线程会被以WAIT_ABANDDONED_0通知。不论线程是因为ExitThread()而结束，或是因当掉而结束，这种情况都存在。







四、信号量

1、产生信号量

![](https://img-blog.csdn.net/20170320202723353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170320202731775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、Semaphore的现值代表的意义是目前可用的资源数。

 2）、每当一个锁定动作成功，Semaphore的现值就会减一。你可以要求任何一个Wait...()要求锁定一个Semaphore。

     i、如果Semaphore的现值不为0，Wait...()函数会立刻返回。

 3）、一旦Semaphore的现值降到0，就表示资源已经耗尽。此时，任何线程调用Wait...()函数，必然要等待，知道某个锁定被解除位置。




2、解除锁定

![](https://img-blog.csdn.net/20170323211646568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、ReleaseSemaphore（）对于semapore所造成得现值的添加，绝对不会超过CreateSemaphore()所指定的1MaximumCount。

 2）、请记住，lpPreviousCount所传回的是一个瞬间值。不可以把1ReleaseCount加上*lpPreviousCount，就当作是semaphore的现值，因为其他线程可能已经改变了semaphore的值。

 3）、与mutex不同的是，调用ReleaseSemaphore（）的那个线程，并不一定就得是调用wait...()的那个线程。任何线程都可以在任何时候调用ReleaseSemaphore（），解除被任何线程锁定的semapore。




3、semaphore必须要有一个初值。







五、事件

1、Event对象是一种核心对象，他的为唯一目的就是称为激发或未激发在状态。event的状态不会因为调用Wait...()而变化。可以精确的告诉一个event对象做什么事，以及何时去做。




2、为了产生一个event对象，你必须调用CreateEvent()

![](https://img-blog.csdn.net/20170323213704111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170323213709469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








六、interlocked Variables

1、同步机制最简单类型是使用interlocked函数，对着标准的32位变量进行操作。这些函数并没有提供“等待”功能，它们只是保证对某个特定变量的存取操作是“一个一个按顺序来”。




2、interlocked函数共有两个

 1）、interlockedIncrement()

 2）、interlockedDecrement()

  注：这两个函数都只能和0做比较，不能和任何其他数值作比较。




3、interlockedIncrement()函数

![](https://img-blog.csdn.net/20170323220221316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4、interlockedExchange()可以设置一个新值并传回旧值。

![](https://img-blog.csdn.net/20170323220613559?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








七、同步机制简要

1、Critical Section

  Critical Section是来实现“排他性占有”。适用范围是单一进程的各线程之间。他是

 1）、一个局部性对象，不是一个核心对象

 2）、快速而有效率。

 3）、不能够同时有一个以上的Critical Section被等待。

 4）、无法侦测是否已被某个线程放弃。




2、Mutex

 Mutex是一个核心对象，可以在不同线程之间实现“排他性占有”，设置甚至即使那些线程分属不同进程。他是

 1）、一个核心对象。

 2）、如果拥有mutex的那个线程结束，则会产生一个“abandoned”错误信息。

 3）、可以使用Wait...()等待一个mutex。

 4）、可以具名，因此可以被其他进程开启。

 5）、只能被拥有它的那个线程释放。




3、Semaphore

 Semphore被用来追踪有限的资源。他是

 1）、一个核心对象。

 2）、没有拥有者。

 3）、可以具名，因此可以被其他进程开启。

 4）、可以被任何一个线程释放。




4、Event Object

 Event Object通常使用于Overlapped，或用来设计某些自定义的同步对象。他是

 1）、一个核心对象。

 2）、完全在程序控制之下。

 3）、适用于设计新的同步对象。

 4）、“要求苏醒”的请求并不会被存储起来，可能遗失掉。

 5）、可以具名，因此可以被其他进程开启。




5、interlocked Variables

 1）、如果interlocked...()函数使用所谓的spin-lock，那么它们只是一种同步机制。

 2）、除此之外，interlocked Variables主要用于引用计数：

     I、允许4字节的数值有些基本的同步操作，不需动用到Critical Section和Mutex之类。

     II、在SMP系统中亦可有效运行。



