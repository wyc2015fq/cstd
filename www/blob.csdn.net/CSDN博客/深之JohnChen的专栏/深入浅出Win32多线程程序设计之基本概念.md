# 深入浅出Win32多线程程序设计之基本概念 - 深之JohnChen的专栏 - CSDN博客

2006年03月31日 10:38:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3180标签：[多线程																[任务																[thread																[semaphore																[mfc																[exchange](https://so.csdn.net/so/search/s.do?q=exchange&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程编程](https://blog.csdn.net/byxdaz/article/category/560198)


**深入浅出Win32多线程程序设计**

作者：宋宝华出处：天极开发责任编辑：[方舟](http://comments.yesky.com/t/%B7%BD%D6%DB/6,324/2234286.shtml)[ 2005-12-14 09:48 ]

        从单进程单线程到多进程多线程是操作系统发展的一种必然趋势，当年的DOS系统属于单任务操作系统，最优秀的程序员也只能通过驻留内存的方式实现所谓的"多任务"，而如今的Win32操作系统却可以一边听音乐，一边编程，一边打印文档。
　　理解多线程及其同步、互斥等通信方式是理解现代操作系统的关键一环，当我们精通了Win32多线程程序设计后，理解和学习其它操作系统的多任务控制也非常容易。因此，学习Win32多线程不仅对理解Win32本身有重要意义，而且对学习和领会其它操作系统也有触类旁通的作用。

**深入浅出Win32多线程程序设计之基本概念**

**引言**　　从单进程单线程到多进程多线程是操作系统发展的一种必然趋势，当年的DOS系统属于单任务操作系统，最优秀的程序员也只能通过驻留内存的方式实现所谓的"[多任务](http://www.yesky.com/key/2628/12628.html)"，而如今的Win32操作系统却可以一边听音乐，一边编程，一边打印文档。

　　理解多线程及其同步、互斥等通信方式是理解现代操作系统的关键一环，当我们精通了Win32多线程程序设计后，理解和学习其它操作系统的多任务控制也非常容易。许多程序员从来没有学习过嵌入式系统领域著名的操作系统VxWorks，但是立马就能在上面做开发，大概要归功于平时在Win32多线程上下的功夫。

　　因此，学习Win32多线程不仅对理解Win32本身有重要意义，而且对学习和领会其它操作系统也有触类旁通的作用。

**进程与线程**

　　先阐述一下进程和线程的概念和区别，这是一个许多大学老师也讲不清楚的问题。

　　进程（Process）是具有一定独立功能的程序关于某个数据集合上的一次运行活动，是系统进行资源分配和调度的一个独立单位。程序只是一组指令的有序集合，它本身没有任何运行的含义，只是一个静态实体。而进程则不同，它是程序在某个数据集上的执行，是一个动态实体。它因创建而产生，因调度而运行，因等待资源或事件而被处于等待状态，因完成任务而被撤消，反映了一个程序在一定的数据集上运行的全部动态过程。

　　线程（Thread）是进程的一个实体，是CPU调度和分派的基本单位。线程不能够独立执行，必须依存在应用程序中，由应用程序提供多个线程执行控制。

　　线程和进程的关系是：线程是属于进程的，线程运行在进程空间内，同一进程所产生的线程共享同一内存空间，当进程退出时该进程所产生的线程都会被强制退出并清除。线程可与属于同一进程的其它线程共享进程所拥有的全部资源，但是其本身基本上不拥有系统资源，只拥有一点在运行中必不可少的信息(如程序计数器、一组寄存器和栈)。

　　根据进程与线程的设置，操作系统大致分为如下类型： 

　　（1）单进程、单线程，MS-DOS大致是这种操作系统；

　　（2）多进程、单线程，多数UNIX（及类UNIX的LINUX）是这种操作系统；

　　（3）多进程、多线程，Win32（Windows NT/2000/XP等）、Solaris 2.x和OS/2都是这种操作系统；

　　（4）单进程、多线程，VxWorks是这种操作系统。

　　在操作系统中引入线程带来的主要好处是：

　　（1）在进程内创建、终止线程比创建、终止进程要快；

　　（2）同一进程内的线程间切换比进程间的切换要快，尤其是用户级线程间的切换。另外，线程的出现还因为以下几个原因：

　　（1）并发程序的并发执行，在多处理环境下更为有效。一个并发程序可以建立一个进程,而这个并发程序中的若干并发程序段就可以分别建立若干线程,使这些线程在不同的处理机上执行。

　　（2）每个进程具有独立的地址空间，而该进程内的所有线程共享该地址空间。这样可以解决父子进程模型中，子进程必须复制父进程地址空间的问题。

　　（3）线程对解决客户/服务器模型非常有效。

**Win32进程**

　　1、进程间通信（[IPC](http://www.yesky.com/key/2278/2278.html)）

　　Win32进程间通信的方式主要有：

　　（1）剪贴板(Clip Board)；

　　（2）动态数据交换(Dynamic Data Exchange)；

　　（3）部件对象模型(Component [Object](http://www.yesky.com/key/3163/8163.html) Model)；

　　（4）文件映射(File Mapping)；

　　（5）邮件槽(Mail Slots)；

　　（6）管道(Pipes)；

　　（7）Win32套接字([Socket](http://www.yesky.com/key/80/10080.html))；

　　（8）远程过程调用(Remote Procedure Call)；

　　（9）WM_COPYDATA消息(WM_COPYDATA Message)。

　　2、获取进程信息

　　在WIN32中，可使用在PSAPI .DLL中提供的Process status Helper函数帮助我们获取进程信息。

　　（1）EnumProcesses()函数可以获取进程的ID，其原型为：

BOOL EnumProcesses(DWORD * lpidProcess, DWORD cb, DWORD*cbNeeded);

　　参数lpidProcess：一个足够大的DWORD类型的数组，用于存放进程的ID值；

　　参数cb：存放进程ID值的数组的最大长度，是一个DWORD类型的数据；

　　参数cbNeeded：指向一个DWORD类型数据的指针，用于返回进程的数目；

　　函数返回值：如果调用成功，返回TRUE，同时将所有进程的ID值存放在lpidProcess参数所指向的数组中，进程个数存放在cbNeeded参数所指向的变量中；如果调用失败，返回FALSE。

　　（2）GetModuleFileNameExA()函数可以实现通过进程句柄获取进程文件名，其原型为：

DWORD GetModuleFileNameExA(HANDLE hProcess, HMODULE hModule,LPTSTR lpstrFileName, DWORD nsize);

　　参数hProcess：接受进程句柄的参数，是HANDLE类型的变量；

　　参数hModule：指针型参数，在本文的程序中取值为NULL；

　　参数lpstrFileName：LPTSTR类型的指针，用于接受主调函数传递来的用于存放进程名的字符数组指针；

　　参数nsize：lpstrFileName所指数组的长度；

　　函数返回值：如果调用成功，返回一个大于0的DWORD类型的数据，同时将hProcess所对应的进程名存放在lpstrFileName参数所指向的数组中；加果调用失败，则返回0。

　　通过下列代码就可以遍历系统中的进程，获得进程列表：

//获取当前进程总数
EnumProcesses(process_[ids](http://www.yesky.com/key/1756/1756.html), sizeof(process_ids), &num_processes);
//遍历进程
for (int i = 0; i < num_processes; i++)
{
　//根据进程ID获取句柄 
　process[i] = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0,
　process_ids[i]);
　//通过句柄获取进程文件名
　if (GetModuleFileNameExA(process[i], NULL, File_name, sizeof(fileName)))
　　cout << fileName << endl;
}
**Win32线程**　　WIN32靠线程的优先级（达到抢占式多任务的目的）及分配给线程的CPU时间来调度线程。WIN32本身的许多应用程序也利用了多线程的特性，如任务管理器等。

　　本质而言，一个处理器同一时刻只能执行一个线程（"微观串行"）。WIN32多任务机制使得CPU好像在同时处理多个任务一样，实现了"宏观并行"。其多线程调度的机制为：

　　（1）运行一个线程，直到被中断或线程必须等待到某个资源可用；

　　（2）保存当前执行线程的描述表(上下文)；

　　（3）装入下一执行线程的描述表(上下文)；

　　（4）若存在等待被执行的线程，则重复上述过程。

　　WIN32下的线程可能具有不同的优先级，优先级的范围为0～31，共32级，其中31表示最高优先级，优先级0为系统保留。它们可以分成两类，即实时优先级和可变优先级：

　　（1）实时优先级从16到31，是实时程序所用的高优先级线程，如许多监控类应用程序；

　　（2）可变优先级从1到15，绝大多数程序的优先级都在这个范围内。。WIN32调度器为了优化系统响应时间，在它们执行过程中可动态调整它们的优先级。

　　多线程确实给应用开发带来了许多好处，但并非任何情况下都要使用多线程，一定要根据应用程序的具体情况来综合考虑。一般来说，在以下情况下可以考虑使用多线程：

　　（1）应用程序中的各任务相对独立；

　　（2）某些任务耗时较多；

　　（3）各任务需要有不同的优先级。

　　另外，对于一些实时系统应用，应考虑多线程。

**Win32核心对象**

　　WIN32核心对象包括进程、线程、文件、事件、信号量、互斥体和管道，核心对象可能有不只一个拥有者，甚至可以跨进程。有一组WIN32 API与核心对象息息相关：

　　（1）WaitForSingleObject，用于等待对象的"激活"，其函数原型为：

DWORD WaitForSingleObject(
　HANDLE hHandle, // 等待对象的句柄
　DWORD dwMilliseconds // 等待毫秒数，INFINITE表示无限等待
);
　　可以作为WaitForSingleObject第一个参数的对象包括：Change notification、Console input、Event、Job、Memory resource notification、Mutex、Process、Semaphore、Thread和Waitable timer。

　　如果等待的对象不可用，那么线程就会挂起，直到对象可用线程才会被唤醒。对不同的对象，WaitForSingleObject表现为不同的含义。例如，使用WaitForSingleObject(hThread,…)可以判断一个线程是否结束；使用WaitForSingleObject(hMutex,…)可以判断是否能够进入临界区；而WaitForSingleObject (hProcess,… )则表现为等待一个进程的结束。

　　与WaitForSingleObject对应还有一个WaitForMultipleObjects函数，可以用于等待多个对象，其原型为：

DWORD WaitForMultipleObjects(DWORD nCount,const HANDLE* pHandles,BOOL bWaitAll,DWORD dwMilliseconds);
　　（2）CloseHandle，用于关闭对象，其函数原型为：

BOOL CloseHandle(HANDLE hObject);
　　如果函数执行成功，则返回TRUE；否则返回FALSE，我们可以通过GetLastError函数进一步可以获得错误原因。

**C运行时库**

　　在VC++6.0中，有两种多线程编程方法：一是使用C运行时库及WIN32 API函数，另一种方法是使用MFC，MFC对多线程开发有强大的支持。
标准C运行时库是1970年问世的，当时还没有多线程的概念。因此，C运行时库早期的设计者们不可能考虑到让其支持多线程应用程序。
Visual C++提供了两种版本的C运行时库，-个版本供单线程应用程序调用，另一个版本供多线程应用程序调用。多线程运行时库与单线程运行时库有两个重大差别：

　　（1）类似errno的全局变量，每个线程单独设置一个；

　　这样从每个线程中可以获取正确的错误信息。

　　（2）多线程库中的数据结构以同步机制加以保护。

　　这样可以避免访问时候的冲突。

　　Visual C++提供的多线程运行时库又分为静态链接库和动态链接库两类，而每一类运行时库又可再分为debug版和release版，因此Visual C++共提供了6个运行时库。如下表：

|C运行时库|库文件|
|----|----|
|Single thread(static link)|libc.lib|
|Debug single thread(static link)|Libcd.lib|
|MultiThread(static link)|libcmt.lib|
|Debug multiThread(static link)|libcmtd.lib|
|MultiThread(dynamic link)|msvert.lib|
|Debug multiThread(dynamic link)|msvertd.lib|

　　如果不使用VC多线程C运行时库来生成多线程程序，必须执行下列操作：

　　（1）使用标准 C 库（基于单线程）并且只允许可重入函数集进行库调用；

　　（2）使用 Win32 API 线程管理函数，如 CreateThread；

　　（3）通过使用 Win32 服务（如信号量和 EnterCriticalSection 及 LeaveCriticalSection 函数），为不可重入的函数提供自己的同步。

　　如果使用标准 C 库而调用VC运行时库函数，则在程序的link阶段会提示如下错误：

error LNK2001: unresolved external symbol __endthreadex
error LNK2001: unresolved external symbol __beginthreadex

