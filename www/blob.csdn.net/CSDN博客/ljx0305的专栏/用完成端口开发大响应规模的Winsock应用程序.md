# 用完成端口开发大响应规模的Winsock应用程序 - ljx0305的专栏 - CSDN博客
2008年05月15日 20:44:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：453标签：[windows																[服务器																[socket																[api																[structure																[resources](https://so.csdn.net/so/search/s.do?q=resources&t=blog)](https://so.csdn.net/so/search/s.do?q=structure&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
通常要开发网络应用程序并不是一件轻松的事情，不过，实际上只要掌握几个关键的原则也就可以了——**创建和连接一个套接字，尝试进行连接，然后收发数据。真正难的是要写出一个可以接纳少则一个，多则数千个连接的网络应用程序。**本文将讨论如何通过Winsock2在Windows NT? 和 Windows 2000上开发高扩展能力的Winsock应用程序。文章主要的焦点在客户机/服务器模型的服务器这一方，当然，其中的许多要点对模型的双方都适用。
**API与响应规模**
通过Win32的重叠I/O机制，应用程序可以提请一项I/O操作，重叠的操作请求在后台完成，而同一时间提请操作的线程去做其他的事情。等重叠操作完成后线程收到有关的通知。这种机制对那些耗时的操作而言特别有用。不过，像Windows 3.1上的WSAAsyncSelect()及Unix下的select()那样的函数虽然易于使用，但是它们不能满足响应规模的需要。而完成端口机制是针对操作系统内部进行了优化，在Windows NT 和 Windows 2000上，使用了完成端口的重叠I/O机制才能够真正扩大系统的响应规模。
**完成端口**
一个完成端口其实就是一个通知队列，由操作系统把已经完成的重叠I/O请求的通知放入其中。当某项I/O操作一旦完成，某个可以对该操作结果进行处理的工作者线程就会收到一则通知。而套接字在被创建后，可以在任何时候与某个完成端口进行关联。
通常情况下，我们会在应用程序中创建一定数量的工作者线程来处理这些通知。线程数量取决于应用程序的特定需要。理想的情况是，线程数量等于处理器的数量，不过这也要求任何线程都不应该执行诸如同步读写、等待事件通知等阻塞型的操作，以免线程阻塞。每个线程都将分到一定的CPU时间，在此期间该线程可以运行，然后另一个线程将分到一个时间片并开始执行。如果某个线程执行了阻塞型的操作，操作系统将剥夺其未使用的剩余时间片并让其它线程开始执行。也就是说，前一个线程没有充分使用其时间片，当发生这样的情况时，应用程序应该准备其它线程来充分利用这些时间片。
完成端口的使用分为两步。首先创建完成端口，如以下代码所示：
HANDLE    hIocp;
hIocp = CreateIoCompletionPort(
    INVALID_HANDLE_VALUE,
    NULL,
    (ULONG_PTR)0,
    0);
if (hIocp == NULL) {
    // Error
}
完成端口创建后，要把将使用该完成端口的套接字与之关联起来。方法是再次调用CreateIoCompletionPort ()函数，第一个参数FileHandle设为套接字的句柄，第二个参数ExistingCompletionPort 设为刚刚创建的那个完成端口的句柄。
以下代码创建了一个套接字，并把它和前面创建的完成端口关联起来：
SOCKET    s;
s = socket(AF_INET, SOCK_STREAM, 0);
if (s == INVALID_SOCKET) {
    // Error
if (CreateIoCompletionPort((HANDLE)s,
                           hIocp,
                           (ULONG_PTR)0,
                           0) == NULL)
{
// Error
}
???
}
这时就完成了套接字与完成端口的关联操作。在这个套接字上进行的任何重叠操作都将通过完成端口发出完成通知。注意，CreateIoCompletionPort()函数中的第三个参数用来设置一个与该套接字相关的“完成键(completion key)”(译者注：完成键可以是任何数据类型)。每当完成通知到来时，应用程序可以读取相应的完成键，因此，完成键可用来给套接字传递一些背景信息。
在创建了完成端口、将一个或多个套接字与之相关联之后，我们就要创建若干个线程来处理完成通知。这些线程不断循环调用GetQueuedCompletionStatus ()函数并返回完成通知。
下面，我们先来看看应用程序如何跟踪这些重叠操作。当应用程序调用一个重叠操作函数时，要把指向一个overlapped结构的指针包括在其参数中。当操作完成后，我们可以通过GetQueuedCompletionStatus()函数中拿回这个指针。不过，单是根据这个指针所指向的overlapped结构，应用程序并不能分辨究竟完成的是哪个操作。要实现对操作的跟踪，你可以自己定义一个OVERLAPPED结构，在其中加入所需的跟踪信息。
无论何时调用重叠操作函数时，总是会通过其lpOverlapped参数传递一个OVERLAPPEDPLUS结构(例如WSASend、 WSARecv等函数)。这就允许你为每一个重叠调用操作设置某些操作状态信息，当操作结束后，你可以通过GetQueuedCompletionStatus()函数获得你自定义结构的指针。注意OVERLAPPED字段不要求一定是这个扩展后的结构的第一个字段。当得到了指向OVERLAPPED结构的指针以后，可以用CONTAINING_RECORD宏取出其中指向扩展结构的指针(译者注：以上两小段一会是OVERLAPPEDPLUS结构，一会是OVERLAPPED结构，本人也看不太懂，请高手赐教)。
OVERLAPPED 结构的定义如下：
typedef struct _OVERLAPPEDPLUS {
    OVERLAPPED        ol;
    SOCKET            s, sclient;
    int               OpCode;
    WSABUF            wbuf;
    DWORD             dwBytes, dwFlags;
    // other useful information
} OVERLAPPEDPLUS;
#define OP_READ     0
#define OP_WRITE    1
#define OP_ACCEPT   2
下面让我们来看看Figure2里工作者线程的情况。
**Figure 2 Worker Thread**
DWORD WINAPI WorkerThread(LPVOID lpParam)
{    
    ULONG_PTR       *PerHandleKey;
    OVERLAPPED      *Overlap;
    OVERLAPPEDPLUS  *OverlapPlus,
                    *newolp;
    DWORD           dwBytesXfered;
    while (1)
    {
        ret = GetQueuedCompletionStatus(
            hIocp,
            &dwBytesXfered,
            (PULONG_PTR)&PerHandleKey,
            &Overlap,
            INFINITE);
        if (ret == 0)
        {
            // Operation failed
            continue;
        }
        OverlapPlus = CONTAINING_RECORD(Overlap, OVERLAPPEDPLUS, ol);
    switch (OverlapPlus->OpCode)
    {
    case OP_ACCEPT:
        // Client socket is contained in OverlapPlus.sclient
        // Add client to completion port
            CreateIoCompletionPort(
                (HANDLE)OverlapPlus->sclient,
                hIocp,
                (ULONG_PTR)0,
                0);
        //  Need a new OVERLAPPEDPLUS structure
        //  for the newly accepted socket. Perhaps
        //  keep a look aside list of free structures.
        newolp = AllocateOverlappedPlus();
        if (!newolp)
        {
            // Error
        }
        newolp->s = OverlapPlus->sclient;
        newolp->OpCode = OP_READ;
        // This function prepares the data to be sent
        PrepareSendBuffer(&newolp->wbuf);
        ret = WSASend(
                newolp->s,
                &newolp->wbuf,
                1,
                &newolp->dwBytes,
                0,
                &newolp.ol,
                NULL);
        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
            // Error
            }
        }
        // Put structure in look aside list for later use
        FreeOverlappedPlus(OverlapPlus);
        // Signal accept thread to issue another AcceptEx
        SetEvent(hAcceptThread);
        break;
    case OP_READ:
        // Process the data read    
        // •••
        // Repost the read if necessary, reusing the same
        // receive buffer as before
        memset(&OverlapPlus->ol, 0, sizeof(OVERLAPPED));
        ret = WSARecv(
              OverlapPlus->s,
              &OverlapPlus->wbuf,
              1,
              &OverlapPlus->dwBytes,
              &OverlapPlus->dwFlags,
              &OverlapPlus->ol,
              NULL);
        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                // Error
            }
        }
        break;
    case OP_WRITE:
        // Process the data sent, etc.
        break;
    } // switch
    } // while
}  // WorkerThread
其中每句柄键(PerHandleKey)变量的内容，是在把完成端口与套接字进行关联时所设置的完成键参数；Overlap参数返回的是一个指向发出重叠操作时所使用的那个OVERLAPPEDPLUS结构的指针。
要记住，如果重叠操作调用失败时(也就是说，返回值是SOCKET_ERROR，并且错误原因不是WSA_IO_PENDING)，那么完成端口将不会收到任何完成通知。如果重叠操作调用成功，或者发生原因是WSA_IO_PENDING的错误时，完成端口将总是能够收到完成通知。(未完待续)
**Windows NT和Windows 2000的套接字架构**
对于开发大响应规模的Winsock应用程序而言，对Windows NT和Windows 2000的套接字架构有基本的了解是很有帮助的。
与其它类型操作系统不同，Windows NT和Windows 2000的传输协议没有一种风格像套接字那样的、可以和应用程序直接交谈的界面，而是采用了一种更为底层的API，叫做传输驱动程序界面(Transport Driver Interface,TDI)。Winsock的核心模式驱动程序负责连接和缓冲区管理，以便向应用程序提供套接字仿真(在AFD.SYS文件中实现)，同时负责与底层传输驱动程序对话。
**谁来负责管理缓冲区？**
正如上面所说的，应用程序通过Winsock来和传输协议驱动程序交谈，而AFD.SYS负责为应用程序进行缓冲区管理。也就是说，当应用程序调用send()或WSASend()函数来发送数据时，AFD.SYS将把数据拷贝进它自己的内部缓冲区(取决于SO_SNDBUF设定值)，然后send()或WSASend()函数立即返回。也可以这么说，AFD.SYS在后台负责把数据发送出去。不过，如果应用程序要求发出的数据超过了SO_SNDBUF设定的缓冲区大小，那么WSASend()函数会阻塞，直至所有数据发送完毕。
从远程客户端接收数据的情况也类似。只要不用从应用程序那里接收大量的数据，而且没有超出SO_RCVBUF设定的值，AFD.SYS将把数据先拷贝到其内部缓冲区中。当应用程序调用recv()或WSARecv()函数时，数据将从内部缓冲拷贝到应用程序提供的缓冲区。
多数情况下，这样的架构运行良好，特别在是应用程序采用传统的套接字下非重叠的send()和receive()模式编写的时候。不过程序员要小心的是，尽管可以通过setsockopt()这个API来把SO_SNDBUF和SO_RCVBUF选项值设成0(关闭内部缓冲区)，但是程序员必须十分清楚把AFD.SYS的内部缓冲区关掉会造成什么后果，避免收发数据时有关的缓冲区拷贝可能引起的系统崩溃。
举例来说，一个应用程序通过设定SO_SNDBUF为0把缓冲区关闭，然后发出一个阻塞send()调用。在这样的情况下，系统内核会把应用程序的缓冲区锁定，直到接收方确认收到了整个缓冲区后send()调用才返回。似乎这是一种判定你的数据是否已经为对方全部收到的简洁的方法，实际上却并非如此。想想看，即使远端TCP通知数据已经收到，其实也根本不代表数据已经成功送给客户端应用程序，比如对方可能发生资源不足的情况，导致AFD.SYS不能把数据拷贝给应用程序。另一个更要紧的问题是，在每个线程中每次只能进行一次发送调用，效率极其低下。
把SO_RCVBUF设为0，关闭AFD.SYS的接收缓冲区也不能让性能得到提升，这只会迫使接收到的数据在比Winsock更低的层次进行缓冲，当你发出receive调用时，同样要进行缓冲区拷贝，因此你本来想避免缓冲区拷贝的阴谋不会得逞。
现在我们应该清楚了，关闭缓冲区对于多数应用程序而言并不是什么好主意。只要要应用程序注意随时在某个连接上保持几个WSARecvs重叠调用，那么通常没有必要关闭接收缓冲区。如果AFD.SYS总是有由应用程序提供的缓冲区可用，那么它将没有必要使用内部缓冲区。
高性能的服务器应用程序可以关闭发送缓冲区，同时不会损失性能。不过，这样的应用程序必须十分小心，保证它总是发出多个重叠发送调用，而不是等待某个重叠发送结束了才发出下一个。如果应用程序是按一个发完再发下一个的顺序来操作，那浪费掉两次发送中间的空档时间，总之是要保证传输驱动程序在发送完一个缓冲区后，立刻可以转向另一个缓冲区。
**资源的限制条件**
在设计任何服务器应用程序时，其强健性是主要的目标。也就是说，
你的应用程序要能够应对任何突发的问题，例如并发客户请求数达到峰值、可用内存临时出现不足、以及其它短时间的现象。这就要求程序的设计者注意Windows NT和2000系统下的资源限制条件的问题，从容地处理突发性事件。
你可以直接控制的、最基本的资源就是网络带宽。通常，使用用户数据报协议(UDP)的应用程序都可能会比较注意带宽方面的限制，以最大限度地减少包的丢失。然而，在使用TCP连接时，服务器必须十分小心地控制好，防止网络带宽过载超过一定的时间，否则将需要重发大量的包或造成大量连接中断。关于带宽管理的方法应根据不同的应用程序而定，这超出了本文讨论的范围。
虚拟内存的使用也必须很小心地管理。通过谨慎地申请和释放内存，或者应用lookaside lists(一种高速缓存)技术来重新使用已分配的内存，将有助于控制服务器应用程序的内存开销(原文为“让服务器应用程序留下的脚印小一点”)，避免操作系统频繁地将应用程序申请的物理内存交换到虚拟内存中(原文为“让操作系统能够总是把更多的应用程序地址空间更多地保留在内存中”)。你也可以通过SetWorkingSetSize()这个Win32 API让操作系统分配给你的应用程序更多的物理内存。
在使用Winsock时还可能碰到另外两个非直接的资源不足情况。一个是被锁定的内存页面的极限。如果你把AFD.SYS的缓冲关闭，当应用程序收发数据时，应用程序缓冲区的所有页面将被锁定到物理内存中。这是因为内核驱动程序需要访问这些内存，在此期间这些页面不能交换出去。如果操作系统需要给其它应用程序分配一些可分页的物理内存，而又没有足够的内存时就会发生问题。我们的目标是要防止写出一个病态的、锁定所有物理内存、让系统崩溃的程序。也就是说，你的程序锁定内存时，不要超出系统规定的内存分页极限。
在Windows NT和2000系统上，所有应用程序总共可以锁定的内存大约是物理内存的1/8(不过这只是一个大概的估计，不是你计算内存的依据)。如果你的应用程序不注意这一点，当你的发出太多的重叠收发调用，而且I/O没来得及完成时，就可能偶尔发生ERROR_INSUFFICIENT_RESOURCES的错误。在这种情况下你要避免过度锁定内存。同时要注意，系统会锁定包含你的缓冲区所在的整个内存页面，因此缓冲区靠近页边界时是有代价的(译者理解，缓冲区如果正好超过页面边界，那怕是1个字节，超出的这个字节所在的页面也会被锁定)。
另外一个限制是你的程序可能会遇到系统未分页池资源不足的情况。所谓未分页池是一块永远不被交换出去的内存区域，这块内存用来存储一些供各种内核组件访问的数据，其中有的内核组件是不能访问那些被交换出去的页面空间的。Windows NT和2000的驱动程序能够从这个特定的未分页池分配内存。
当应用程序创建一个套接字(或者是类似的打开某个文件)时，内核会从未分页池中分配一定数量的内存，而且在绑定、连接套接字时，内核又会从未分页池中再分配一些内存。当你注意观察这种行为时你将发现，如果你发出某些I/O请求时(例如收发数据)，你会从未分页池里再分配多一些内存(比如要追踪某个待决的I/O操作，你可能需要给这个操作添加一个自定义结构，如前文所提及的)。最后这就可能会造成一定的问题，操作系统会限制未分页内存的用量。
在Windows NT和2000这两种操作系统上，给每个连接分配的未分页内存的具体数量是不同的，未来版本的Windows很可能也不同。为了使应用程序的生命期更长，你就不应该计算对未分页池内存的具体需求量。
你的程序必须防止消耗到未分页池的极限。当系统中未分页池剩余空间太小时，某些与你的应用程序毫无关系的内核驱动就会发疯，甚至造成系统崩溃，特别是当系统中有第三方设备或驱动程序时，更容易发生这样的惨剧(而且无法预测)。同时你还要记住，同一台电脑上还可能运行有其它同样消耗未分页池的其它应用程序，因此在设计你的应用程序时，对资源量的预估要特别保守和谨慎。
处理资源不足的问题是十分复杂的，因为发生上述情况时你不会收到特别的错误代码，通常你只能收到一般性的WSAENOBUFS或者ERROR_INSUFFICIENT_RESOURCES 错误。要处理这些错误，首先，把你的应用程序工作配置调整到合理的最大值(译者注：所谓工作配置，是指应用程序各部分运行中所需的内存用量，请参考 [http://msdn.microsoft.com/msdnmag/issues/1000/Bugslayer/Bugslayer1000.asp](http://msdn.microsoft.com/msdnmag/issues/1000/Bugslayer/Bugslayer1000.asp) ，关于内存优化，译者另有译文)，如果错误继续出现，那么注意检查是否是网络带宽不足的问题。之后，请确认你没有同时发出太多的收发调用。最后，如果还是收到资源不足的错误，那就很可能是遇到了未分页内存池不足的问题了。要释放未分页内存池空间，请关闭应用程序中相当部分的连接，等待系统自行渡过和修正这个瞬时的错误。
**接受连接请求**
服务器要做的最普通的事情之一就是接受来自客户端的连接请求。在套接字上使用重叠I/O接受连接的惟一API就是AcceptEx()函数。有趣的是，通常的同步接受函数accept()的返回值是一个新的套接字，而AcceptEx()函数则需要另外一个套接字作为它的参数之一。这是因为AcceptEx()是一个重叠操作，所以你需要事先创建一个套接字(但不要绑定或连接它)，并把这个套接字通过参数传给AcceptEx()。以下是一小段典型的使用AcceptEx()的伪代码：
do {
    -等待上一个 AcceptEx 完成
    -创建一个新套接字并与完成端口进行关联
    -设置背景结构等等
    -发出一个 AcceptEx 请求
}while(TRUE);
作为一个高响应能力的服务器，它必须发出足够的AcceptEx调用，守候着，一旦出现客户端连接请求就立刻响应。至于发出多少个AcceptEx才够，就取决于你的服务器程序所期待的通信交通类型。比如，如果进入连接率高的情况(因为连接持续时间较短，或者出现交通高峰)，那么所需要守候的AcceptEx当然要比那些偶尔进入的客户端连接的情况要多。聪明的做法是，由应用程序来分析交通状况，并调整AcceptEx守候的数量，而不是固定在某个数量上。
对于Windows2000，Winsock提供了一些机制，帮助你判定AcceptEx的数量是否足够。这就是，在创建监听套接字时创建一个事件，通过WSAEventSelect()这个API并注册FD_ACCEPT事件通知来把套接字和这个事件关联起来。一旦系统收到一个连接请求，如果系统中没有AcceptEx()正在等待接受连接，那么上面的事件将收到一个信号。通过这个事件，你就可以判断你有没有发出足够的AcceptEx()，或者检测出一个非正常的客户请求(下文述)。这种机制对Windows NT 4.0不适用。
使用AcceptEx()的一大好处是，你可以通过一次调用就完成接受客户端连接请求和接受数据(通过传送lpOutputBuffer参数)两件事情。也就是说，如果客户端在发出连接的同时传输数据，你的AcceptEx()调用在连接创建并接收了客户端数据后就可以立刻返回。这样可能是很有用的，但是也可能会引发问题，因为AcceptEx()必须等全部客户端数据都收到了才返回。具体来说，如果你在发出AcceptEx()调用的同时传递了lpOutputBuffer参数，那么AcceptEx()不再是一项原子型的操作，而是分成了两步：接受客户连接，等待接收数据。当缺少一种机制来通知你的应用程序所发生的这种情况：“连接已经建立了，正在等待客户端数据”，这将意味着有可能出现客户端只发出连接请求，但是不发送数据。如果你的服务器收到太多这种类型的连接时，它将拒绝连接更多的合法客户端请求。这就是黑客进行“拒绝服务”攻击的常见手法。
要预防此类攻击，接受连接的线程应该不时地通过调用getsockopt()函数(选项参数为SO_CONNECT_TIME)来检查AcceptEx()里守候的套接字。getsockopt()函数的选项值将被设置为套接字被连接的时间，或者设置为-1(代表套接字尚未建立连接)。这时，WSAEventSelect()的特性就可以很好地利用来做这种检查。如果发现连接已经建立，但是很久都没有收到数据的情况，那么就应该终止连接，方法就是关闭作为参数提供给AcceptEx()的那个套接字。注意，在多数非紧急情况下，如果套接字已经传递给AcceptEx()并开始守候，但还未建立连接，那么你的应用程序不应该关闭它们。这是因为即使关闭了这些套接字，出于提高系统性能的考虑，在连接进入之前，或者监听套接字自身被关闭之前，相应的内核模式的数据结构也不会被干净地清除。
发出AcceptEx()调用的线程，似乎与那个进行完成端口关联操作、处理其它I/O完成通知的线程是同一个，但是，别忘记线程里应该尽力避免执行阻塞型的操作。Winsock2分层结构的一个副作用是调用socket()或WSASocket() API的上层架构可能很重要(译者不太明白原文意思，抱歉)。每个AcceptEx()调用都需要创建一个新套接字，所以最好有一个独立的线程专门调用AcceptEx()，而不参与其它I/O处理。你也可以利用这个线程来执行其它任务，比如事件记录。
有关AcceptEx()的最后一个注意事项：要实现这些API，并不需要其它提供商提供的Winsock2实现。这一点对微软特有的其它API也同样适用，比如TransmitFile()和GetAcceptExSockAddrs()，以及其它可能会被加入到新版Windows的API. 在Windows NT和2000上，这些API是在微软的底层提供者DLL(mswsock.dll)中实现的，可通过与mswsock.lib编译连接进行调用，或者通过WSAIoctl() (选项参数为SIO_GET_EXTENSION_FUNCTION_POINTER)动态获得函数的指针。
如果在没有事先获得函数指针的情况下直接调用函数(也就是说，编译时静态连接mswsock.lib，在程序中直接调用函数)，那么性能将很受影响。因为AcceptEx()被置于Winsock2架构之外，每次调用时它都被迫通过WSAIoctl()取得函数指针。要避免这种性能损失，需要使用这些API的应用程序应该通过调用WSAIoctl()直接从底层的提供者那里取得函数的指针。
**TransmitFile 和 TransmitPackets**
Winsock 提供两个专门为文件和内存数据传输进行了优化的函数。其中TransmitFile()这个API函数在Windows NT 4.0 和 Windows 2000上都可以使用，而TransmitPackets()则将在未来版本的Windows中实现。
TransmitFile()用来把文件内容通过Winsock进行传输。通常发送文件的做法是，先调用CreateFile()打开一个文件，然后不断循环调用ReadFile() 和WSASend ()直至数据发送完毕。但是这种方法很没有效率，因为每次调用ReadFile() 和 WSASend ()都会涉及一次从用户模式到内核模式的转换。如果换成TransmitFile()，那么只需要给它一个已打开文件的句柄和要发送的字节数，而所涉及的模式转换操作将只在调用CreateFile()打开文件时发生一次，然后TransmitFile()时再发生一次。这样效率就高多了。
TransmitPackets()比TransmitFile()更进一步，它允许用户只调用一次就可以发送指定的多个文件和内存缓冲区。函数原型如下：
BOOL TransmitPackets(
  SOCKET hSocket,                             
  LPTRANSMIT_PACKET_ELEMENT lpPacketArray,
  DWORD nElementCount,                
  DWORD nSendSize,                
  LPOVERLAPPED lpOverlapped,                  
  DWORD dwFlags                               
);
其中，lpPacketArray是一个结构的数组，其中的每个元素既可以是一个文件句柄或者内存缓冲区，该结构定义如下：
typedef struct _TRANSMIT_PACKETS_ELEMENT { 
    DWORD dwElFlags; 
    DWORD cLength; 
    union {
        struct {
            LARGE_INTEGER     nFileOffset;
            HANDLE            hFile;
            };
            PVOID             pBuffer;
    };
} TRANSMIT_FILE_BUFFERS;
其中各字段是自描述型的(self explanatory)。
**dwElFlags字段：**指定当前元素是一个文件句柄还是内存缓冲区(分别通过常量TF_ELEMENT_FILE 和TF_ELEMENT_MEMORY指定)；
**cLength字段：**指定将从数据源发送的字节数(如果是文件，这个字段值为0表示发送整个文件)；
**结构中的无名联合体：**包含文件句柄的内存缓冲区(以及可能的偏移量)。
使用这两个API的另一个好处，是可以通过指定TF_REUSE_SOCKET和TF_DISCONNECT标志来重用套接字句柄。每当API完成数据的传输工作后，就会在传输层级别断开连接，这样这个套接字就又可以重新提供给AcceptEx()使用。采用这种优化的方法编程，将减轻那个专门做接受操作的线程创建套接字的压力(前文述及)。
这两个API也都有一个共同的弱点：Windows NT Workstation 或 Windows 2000 专业版中，函数每次只能处理两个调用请求，只有在Windows NT、Windows 2000服务器版、Windows 2000高级服务器版或 Windows 2000 Data Center中才获得完全支持。
**放在一起看看**
以上各节中，我们讨论了开发高性能的、大响应规模的应用程序所需的函数、方法和可能遇到的资源瓶颈问题。这些对你意味着什么呢？其实，这取决于你如何构造你的服务器和客户端。当你能够在服务器和客户端设计上进行更好地控制时，那么你越能够避开瓶颈问题。
来看一个示范的环境。我们要设计一个服务器来响应客户端的连接、发送请求、接收数据以及断开连接。那么，服务器将需要创建一个监听套接字，把它与某个完成端口进行关联，为每颗CPU创建一个工作线程。再创建一个线程专门用来发出AcceptEx()。我们知道客户端会在发出连接请求后立刻传送数据，所以如果我们准备好接收缓冲区会使事情变得更为容易。当然，不要忘记不时地轮询AcceptEx()调用中使用的套接字(使用SO_CONNECT_TIME选项参数)来确保没有恶意超时的连接。
该设计中有一个重要的问题要考虑，我们应该允许多少个AcceptEx()进行守候。这是因为，每发出一个AcceptEx()时我们都同时需要为它提供一个接收缓冲区，那么内存中将会出现很多被锁定的页面(前文说过了，每个重叠操作都会消耗一小部分未分页内存池，同时还会锁定所有涉及的缓冲区)。这个问题很难回答，没有一个确切的答案。最好的方法是把这个值做成可以调整的，通过反复做性能测试，你就可以得出在典型应用环境中最佳的值。
好了，当你测算清楚后，下面就是发送数据的问题了，考虑的重点是你希望服务器同时处理多少个并发的连接。通常情况下，服务器应该限制并发连接的数量以及等候处理的发送调用。因为并发连接数量越多，所消耗的未分页内存池也越多；等候处理的发送调用越多，被锁定的内存页面也越多(小心别超过了极限)。这同样也需要反复测试才知道答案。
对于上述环境，通常不需要关闭单个套接字的缓冲区，因为只在AcceptEx()中有一次接收数据的操作，而要保证给每个到来的连接提供接收缓冲区并不是太难的事情。但是，如果客户机与服务器交互的方式变一变，客户机在发送了一次数据之后，还需要发送更多的数据，在这种情况下关闭接收缓冲就不太妙了，除非你想办法保证在每个连接上都发出了重叠接收调用来接收更多的数据。
**结论**
开发大响应规模的Winsock服务器并不是很可怕，其实也就是设置一个监听套接字、接受连接请求和进行重叠收发调用。通过设置合理的进行守候的重叠调用的数量，防止出现未分页内存池被耗尽，这才是最主要的挑战。按照我们前面讨论的一些原则，你就可以开发出大响应规模的服务器应用程序。
[http://blog.csdn.net/wang_54439463/archive/2008/05/15/2447738.aspx](http://blog.csdn.net/wang_54439463/archive/2008/05/15/2447738.aspx)
