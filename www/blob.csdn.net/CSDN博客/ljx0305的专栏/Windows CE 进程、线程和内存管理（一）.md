# Windows CE 进程、线程和内存管理（一） - ljx0305的专栏 - CSDN博客
2008年03月11日 22:23:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：649
**Windows CE 进程、线程和内存管理（一）**
作者：[付林林](mailto:windowsce@tom.com)
　　进程、线程、内存管理是一个内核最基本的服务，也是一个内核最主要的组成部分。这几方面的知识是一个软件开发人员必须掌握的基础知识。虽然一个人不懂这些知识也能编写简单的程序，但这样的程序只能算是皮毛。掌握了进程、线程和内存管理方面的知识，就能够充分利用操作系统内核提供的服务，提高你编写的软件的执行效率、更节省资源、更健壮。顺便说一下，在Windows CE.net下可以运行用Visual Studio.net开发的.net平台上运行的软件，但这样的软件是最上层的软件，离操作系统内核太远了。不但执行效率相对较低，而且还要把.net 框架加到内核中。所以在大多数情况下，EVC仍然是第一选择。
![](http://www.vckbase.com/document/journal/vckbase32/images/fllprothr1img1.JPG)
图一 CE内核结构
**一、进程和线程**
1、概念：
　　Windows CE.NET是一个抢占多任务操作系统，抢占多任务又被称为调度。在调度过程中，内核的调度系统包含一个当前所有进程中线程的优先级列表，并对所有的线程按优先级排列顺序。当中断发生时，调度系统重新安排所有线程的排列顺序。
　　一个进程是一个正运行的应用程序的实例。它由两个部分组成：一个是操作系统用来管理这个进程的内核对象。另一个是这个进程拥有的地址空间。这个地址空间包含应用程序的代码段、静态数据段、堆、栈，非XIP（Execute In Place）DLL。从执行角度方面看，一个进程由一个或多个线程组成。一个线程是一个执行单元，它控制CPU执行进程中某一段代码段。一个线程可以访问这个进程中所有的地址空间和资源。一个进程最少包括一个线程来执行代码，这个线程又叫做主线程。
2、进程：
　　Windows CE.NET最多支持32个进程同时运行。这是由整个系统分配给所有进程的总地址空间决定的。低于Windows CE 4.0版本（也就是低于.NET的版本）的CE操作系统，总进程空间从0x0000 0000到0x4200 0000 ，每32MB地址空间为一个槽（Slot），共33个槽。当一个进程启动时，内核选择一个没有被占用的槽作为这个进程的地址空间。其中0x0000 0000到0x01FF FFFF这个槽称为Slot 0。每个进程在即将得到CPU控制权时，将整个地址映射到Slot 0。这个进程在帮助文档中称为当前运行进程（currently running process）。分配一个槽后，内核在这个槽内按由低地址到高地址顺序为代码段、静态数据段分配足够的地址空间，然后是堆、栈，栈之后的空间为所有 DLL保留，包括XIP和非XIP DLL。注意Slot 0最底部64KB是永远保留的。从Slot 1 到 Slot32 为进程使用。前几个槽一般为系统程序使用。如filesys.exe、device.exe、gwes.exe等。
　　Windows CE.NET与低版本操作系统略有不同。这一点是从MSDN 的"Technical Articles"和"Knowledge Base"的文章中找到的，这的确让我费了一番功夫。在Windows CE.NET的帮助文件中只能找到和早期版本相同的说法，而"Technical Articles"和"Knowledge Base"中有几篇文章清楚的说明了Windows CE.NET 和低版本操作系统的不同。在低版本操作系统中，的确如上所说分为33个槽，Slot 0用于当前运行进程，共支持32个进程同时运行。而且所有DLL都加载到进程的地址空间。但Windows CE.NET下 Slot 1也用于当前进程（Slot 1只用于加载所有XIP DLL）。那么一个进程就不是占有32MB地址空间了，而是64MB。在讲解内存管理时我会具体讲解。
创建一个进程的API函数如下： 
BOOL  CreateProcess(LPCTSTR  lpApplicationName, LPTSTR  lpCommandLine,
		LPSECURITY_ATTRIBUTES  lpProcessAttributes, 
		LPSECURITY_ATTRIBUTES  lpThreadAttributes, 
		BOOL  bInheritHandles, 
		DWORD  dwCreationFlags, 
		LPVOID  lpEnvironment,
		LPCTSTR  lpCurrentDirectory, 
		LPSTARTUPINFO  lpStartupInfo, 
		LPPROCESS_INFORMATION  lpProcessInformation );
　　Windows CE.NET不支持安全性、当前目录、继承性，所以这个函数很多参数都必须设为0或FALSE。具体第3、4、7、8、9设为0，第5设为FALSE。第1参数为应用程序名称，这个参数不能为NULL。如果只传递应用程序名称而没有指定路径，那么系统将先搜索/Windows目录，接着搜索OEM指定的搜索路径。第2参数用于传递启动参数，必须为UNICODE码。第6参数为创建标志。可以为0（创建一个常规进程）、CREATE_SUSPENDED（启动后挂起）、DEBUG_PROCESS（用于创建这个进程的父进程调试用）、DEBUG_ONLY_THIS_PROCESS（不调试子进程）、CREATE_NEW_CONSOLE（控制台进程）。第10参数传递给它一个PROCESS_INFORMATION结构变量的地址。返回进程和主线程的句柄和ID。
　　终止一个进程最好是由WinMain函数返回。在主线程中调用ExitThread函数也可以。在当前进程终止另一个进程使用TerminateProcess函数。CE下的TerminateProcess函数要比其他Windows下TerminateProcess函数功能强大。CE下的TerminateProcess函数在使进程退出时，会通知每个加载的DLL并做出进程退出时该做的所有处理工作。
3、线程：
　　线程除了能够访问进程的资源外，每个线程还拥有自己的栈。栈的大小是可以调整的，最小为1KB或4KB（也就是一个内存页。内存页的大小取决于CPU），一般默认为64KB，但栈顶端永远保留2KB为防止溢出。如果要改变栈初始时大小，在EVC"Project"－"Settings"－"Link"链接选项"/STACK"后的参数中指定大小。其中参数1为默认大小，参数2为一个内存页大小，都用十六进制表示。如果将栈的初始值设置太小，很容易导致系统访问非法并立即终止进程。
　　线程有五中状态，分别为运行、挂起、睡眠、阻塞、终止。当所有线程全部处于阻塞状态时，内核处于空闲模式（Idle mode），这时对CPU的电力供应将减小。
创建一个线程的API函数如下：
HANDLE  CreateThread(LPSECURITY_ATTRIBUTES  lpThreadAttributes,
		 DWORD  dwStackSize, 
		 LPTHREAD_START_ROUTINE  lpStartAddress,
		 LPVOID  lpParameter, 
		 DWORD  dwCreationFlags,  
		 LPDWORD  lpThreadId );      
Windows CE.NET 不支持安全性所以参数1必须设置为0。如果参数5为STACK_SIZE_PARAM_IS_A_RESERVATION，那么参数2可以指定栈的大小，内核将按照参数2的数值来为此线程拥有的栈保留地址空间。如果参数5不为STACK_SIZE_PARAM_IS_A_RESERVATION，那么参数2必须设置为0。参数3为执行路径的首地址，也就是函数的地址。参数4用来向线程中传递一个参数。参数5除了上面说明外，还可以为0、CREATE_SUSPENDED。CREATE_SUSPENDED表示这个线程在创建后一直处于挂起状态，直到用ResumeThread函数来恢复。最后一个参数保存函数返回的创建的线程ID。
　　退出一个线程同退出一个进程有类似的方法。最好是由函数返回，在线程中调用ExitThead函数也可以。在当前线程中终止另一个线程使用TerminateThread函数。此函数在使一个线程退出时，会通知这个线程加载的所有DLL。这样DLL就可以做结束工作了。
　　Windows CE.NET不像其他Windows操作系统将进程分为不同的优先级类，Windows CE.NET只将线程分为256个优先级。0优先级最高，255最低，0到248优先级属于实时性优先级。0到247优先级一般分配给实时性应用程序、驱动程序、系统程序。249到255优先级中，251优先级（THREAD_PRIORITY_NORMAL）是正常优先级。255优先级（THREAD_PRIORITY_IDLE）为空闲优先级。249优先级（THREAD_PRIORITY_HIGHEST）是高优先级。248到255优先级一般分配给普通应用程序线程使用。具体分段见下表：
|**优先级范围**|**分配对象**|
|----|----|
|0-96|高于驱动程序的程序|
|97-152|基于Windows CE的驱动程序|
|153-247|低于驱动程序的程序|
|248-255|普通的应用程序|
　　Windows CE.NET操作系统具有实时性，所以调度系统必须保证高优先级线程先运行，低优先级线程在高优先级线程终止后或者阻塞时才能得到CPU时间片。而且一旦发生中断，内核会暂停低优先级线程的运行，让高优先级线程继续运行，直到终止或者阻塞。具有相同优先级的线程平均占有CPU时间片，当一个线程使用完了CPU时间片或在时间片内阻塞、睡眠，那么其他相同优先级的线程会占有时间片。这里提到的CPU时间片是指内核限制线程占有CPU的时间，默认为100ms。OEM可以更改这个值，甚至设置为0。如果为0，当前线程将一直占有CPU，直到更高优先级线程要求占有CPU。这个调度算法好像是很有效、很完美，但却存在着一种情况，当这种情况发生时程序会死锁。举例来说：一个应用程序包含两个线程，线程1是高优先级，线程2是低优先级，当线程1运行过程中处于阻塞时，线程2得到时间片，线程2这次进入了一个临界区，我们都知道临界区内的资源是不会被其它线程访问的，当线程2正运行时，线程1已经从阻塞状态转变为运行状态，而这次线程1却要访问线程2的资源，这个资源却被临界区锁定，那么线程1只能等待，等待线程2从临界区中运行结束并释放资源的独占权。但是线程2却永远不会得到时间片，因为CE保证高优先级线程会先运行。这时程序就会处于死锁状态。当然系统不会死锁，因为还有更高优先级的线程、驱动程序在运行。对于这种情况，CE采取优先级转换的办法来解决。就是当发生这种情况时，内核将线程2的优先级提高到线程1的优先级水平。这样线程2就可以执行完临界区代码了，线程1也就能够访问资源了。然后内核再恢复线程2原来的优先级。
　　挂起一个线程使用SuspendThread函数。参数只有一个――线程的句柄。要说明的是如果要挂起的线程正调用一个内核功能，这时执行此函数可能会失败。需要多次调用此函数直到函数返回值不为0xFFFFFFFF，说明挂起成功。恢复线程使用ResumeThread函数。参数也只有一个――线程的句柄。
　　关于线程本地存储器和纤程，实际用到的时候非常少，这部分知识可以参考《Windows核心编程》。
写作时间：2004-06-01 
未经本文作者同意，不准擅自转载本篇文章。联系作者请邮至 fllsoft@sina.com 或windowsce@tom.com
 引用[http://www.vckbase.com/document/viewdoc/?id=1154](http://www.vckbase.com/document/viewdoc/?id=1154)
