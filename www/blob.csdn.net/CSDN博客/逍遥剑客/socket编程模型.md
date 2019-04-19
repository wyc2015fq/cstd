# socket编程模型 - 逍遥剑客 - CSDN博客
2007年03月19日 23:00:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2340
最先出现的是阻塞模型，这个模型是讲解计算机网络时被作为例子介绍的，也是最简单的。其基本原理是：首先建立一个socket连接，然后对其进行操作，比如，从该socket读数据。因为网络传输是要一定的时间的，即使网络通畅的情况下，接受数据的操作也要花费时间。对于一个简单的单线程程序，接收数据的过程是无法处理其他操作的。比如一个窗口程序，当你接收数据时，点击按钮或关闭窗口操作都不会有效。它的缺点显而易见，一个线程你只能处理一个socket，用来教课还行，实际使用效果就不行了。
为了处理多个socket连接，聪明的人们发明了select模型。该模型以集合来管理socket连接，每次去查询集合中的socket状态，从而达到处理多连接的能力，其函数原型是int select(int nfds, fd_set FAR * readfds, fd_set FAR * writefds, fd_set FAR * exceptfds, const struct timeval FAR * timeout)。比如我们判断某个socket是否有数据可读，我们首先将一个fdread集合置空，然后将socket加入到该集合，调用select(0,&fdread,NULL,NULL,NULL)，之后我们判断socket是否还在fdread中，如果还在，则说明有数据可读。数据的读取和阻塞模型相同，调用recv函数。但是每个集合容量都有一个限值，默认情况下是64个，当然你可以重新定义它的大小，但还是有一个最上限，自己设置也不能超过该值，一般情况下是1024。尽管select模型可以处理多连接，但集合的管理多少让人感到繁琐。
熟悉windows操作系统的都知道，其窗口处理是基于消息的。人们又发明了一种新的网络模型——WSAAsyncSelect模型，即异步选择模型。该模型为每个socket绑定一个消息，当socket上出现事先设置的socket事件时，操作系统就会给应用程序发送这个消息，从而对该socket事件进行处理，其函数原型是int WSAAsynSelect(SOCKET s, HWND hWnd, unsigned int wMsg, long lEvent)。hWnd指明接收消息的句柄，wMsg指定消息ID，lEvent按位设置感兴趣的网络事件，入WSAAsyncSelect(s,hwnd,WM_SOCKET, FD_CONNECT | FD_READ | FD_CLOSE)。该模型的优点是在系统开销不大的情况下同时处理许多连接，也不需要什么集合管理。缺点很明显，即使你的程序不需要窗口，也要专门为WSAAsyncSelect模型定义一个窗口。另外，让单个窗口去处理成千上万的socket操作事件，很可能成为性能瓶颈。
 与WSAAsynSelect模型类似，人们还发明了WSAEventSelect模型，即事件选择模型。看名字就可以猜测出来，它是基于事件的。WSAAsynSelect模型在出现感兴趣的socket事件时，系统会发一个相应的消息。而WSAEventSelect模型在出现感兴趣的socket事件时，系统会将相应WSAEVENT事件设为传信。可能你现在对sokect事件和普通WSAEVENT事件还不是很清楚。socket事件是与socket操作相关的一些事件，如FD_READ,FD_WRITE,FD_ACCEPT等。而WSAEVENT事件是传统的事件，该事件有两种状态，传信（signaled)和未传信(non-signaled）。所谓传信，就是事件发生了，未传信就是还没有发生。我们每次建立一个连接，都为其绑定一个事件，等到该连接变化时，事件就会变为传信状态。那么，谁去接受这个事件变化呢？我们通过一个WSAWaitForMultipleEvents（...）函数来等待事件发生，传入参数中的事件数组中，只有有一个事件发生，该函数就会返回（也可以设置为所有事件发生才返回，在这里没用），返回值为事件的数组序号，这样我们就知道了哪个事件发生了，也就是该事件对应的socket有了socket操作事件。该模型比起WSAAsynSelect模型的优势很明显，不需要窗口。唯一缺点是，该模型每次只能等待64个事件，这一限制使得在处理多socket时，有必要组织一个线程池，伸缩性不如后面要讲的重叠模型。
重叠I/O（Overlapped I/O）模型使应用程序达到更佳的系统性能。重叠模型的基本设计原理是让应用程序使用重叠数据结构，一次投递一个或多个Winsock I/O请求。重叠模型到底是什么东西呢？可以与WSAEventSelect模型做类比（其实不恰当，后面再说），事件选择模型为每个socket连接绑定了一个事件，而重叠模型为每个socket连接绑定了一个重叠。当连接上发生socket事件时，对应的重叠就会被更新。其实重叠的高明之处在于，它在更新重叠的同时，还把网络数据传到了实现指定的缓存区中。我们知道，前面的网络模型都要用户自己通过recv函数来接受数据，这样就降低了效率。我们打个比方，WSAEventSelect模型就像邮局的包裹通知，用户收到通知后要自己去邮局取包裹。而重叠模型就像送货上门，邮递员发给你通知时，也把包裹放到了你事先指定的仓库中。
    重叠模型又分为事件通知和完成例程两种模式。在分析这两种模式之前，我们还是来看看重叠数据结构：
    typedef struct WSAOVERLAPPED
    {
       DWORD Internal;
       DWORD InternalHigh;
       DWORD Offset;
       DWORD OffsetHigh;
       WSAEVENT hEvent;
    }WSAOVERLAPPED, FAR * LPWSAOVERLAPPED;
    该数据结构中，Internal、InternalHigh、Offset、OffsetHigh都是系统使用的，用户不用去管，唯一关注的就是hEvent。如果使用事件通知模式，那么hEvent就指向相应的事件句柄。如果是完成例程模式，hEvent设为NULL。我们现在来看事件通知模式，首先创建一个事件hEvent，并创建一个重叠结构AcceptOverlapped，并设置AcceptOverlapped.hEvent = hEvent，DataBuf是我们事先设置的数据缓存区。调用WSARecv(AcceptSocket,&DataBuf,1,&RecvBytes,&Flags,&AcceptOverlapped,NULL)，则将AcceptSocket与AcceptOverlapped重叠绑定在了一起。当接收到数据以后，hEvent就会设为传信，而数据就会放到DataBuf中。我们再通过WSAWaitForMultipleEvents（...）接收到该事件通知。这里我们要注意，既然是基于事件通知的，那它就有一个事件处理上限，一般为64。
    完成例程和事件通知模式的区别在于，当相应的socket事件出现时，系统会调用用户事先指定的回调函数，而不是设置事件。其实就是将WSARecv的最后一个参数设为函数指针。该回调函数的原型如下：
    void CALLBACK CompletionROUTINE(
        DWORD dwError,
        DWORD cbTransferred,
        LPWSAOVERLAPPED lpOverlapped,
        DWORD dwFlags
    );
    其中，cbTransferred表示传输的字节数，lpOverlapped是发生socket事件的重叠指针。我们调用WSARecv(AcceptSocket,&DataBuf,1,&RecvBytes,&Flags,&AcceptOverlapped,WorkerRoutine)将AcceptSocket与WorkRoutine例程绑定。这里有一点小提示，当我们创建多个socket的连接时，最好把重叠与相应的数据缓存区用一个大的数据结构放到一块，这样，我们在例程中通过lpOverlapped指针就可以直接找到相应的数据缓存区。这里要注意，不能将多个重叠使用同一个数据缓存区，这样在多个重叠都在处理时，就会出现数据混乱。
下面我们来介绍专门用于处理为数众多socket连接的网络模型——完成端口。因为需要做出大量的工作以便将socket添加到一个完成端口，而其他方法的初始化步骤则省事多了，所以对新手来说，完成端口模型好像过于复杂了。然而，一旦弄明白是怎么回事，就会发现步骤其实并非那么复杂。所谓完成端口，实际是Windows采用的一种I/O构造机制，除套接字句柄之外，还可以接受其他东西。使用这种模式之前，首先要创建一个I/O完成端口对象，该函数定义如下：
    HANDLE CreateIoCompletionPort(
       HANDLE FileHandle,
       HANDLE ExistingCompletionPort,
       DWORD CompletionKey,
       DWORD NumberOfConcurrentThreads
    );
    该函数用于两个截然不同的目的：1）用于创建一个完成端口对象。2）将一个句柄同完成端口关联到一起。
    通过参数NumberOfConcurrentThreads，我们可以指定同时运行的线程数。理想状态下，我们希望每个处理器各自负责一个线程的运行，为完成端口提供服务，避免过于频繁的线程任务切换。对于一个socket连接，我们通过CreateIoCompletionPort((HANDLE)Accept,CompletionPort,(DWORD)PerHandleData,0)将Accept连接与CompletionPort完成端口绑定到一起，CompetionPort对应的那些线程不断通过GetQueuedCompletionStatus来查询与其关联的socket连接是否有I/O操作完成，如果有，则做相应的数据处理，然后通过WSARecv将该socket连接再次投递，继续工作。完成端口在性能和伸缩性方面表现都很好，相关联的socket连接数目没有限制。
    上面介绍了6种socket编程模型，好像现在已经有新的模型出现。
