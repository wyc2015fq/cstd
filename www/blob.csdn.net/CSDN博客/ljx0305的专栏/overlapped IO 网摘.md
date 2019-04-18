# overlapped IO 网摘 - ljx0305的专栏 - CSDN博客
2010年01月18日 23:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：896标签：[io																[socket																[null																[winapi																[扩展																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)
个人分类：[C++																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
[http://dev.csdn.net/article/74/74807.shtm](http://dev.csdn.net/article/74/74807.shtm)
Overlapped模型与CompletionPort模型，摘自《Visual C++网络游戏建模与实现》（苏羽、王媛媛编著）   
Win32重叠I/O(Overloapped I/O)机制允许发起一个操作，然后在操作完成之后接受
到信息。对于那种需要很长时间才能完成的操作来说，重叠I/O机制尤其有用，因为发起
重叠操作的线程在重叠请求发出后就可以自由地做别的事情了。
    在Windows NT/2000上，提供真正可扩展的I/O模型就是使用完成端口（Completion 
Port)的重叠I/O。
    ……
    可以把完成端口看成系统维护的一个队列，操作系统把重叠I/O操作完成的事件通知
放到该队列里，由于是暴露“操作完成”的事件通知，所以命名为“完成端口”（Completion
Ports)。一个Socket被创建后，可以在任何时刻和一个完成端口联系起来。
    一般来说，一个应用程序可以创建多个工作线程来处理完成端口上的通知事件。工作
线程的数量依赖于程序的具体需要。但是在理想的情况下，应该对应一个CPU创建一个线
程。因为在完成端口理想模型中，每个线程都可以从系统获得一个“原子”性的时间片，轮
番运行并检查完成端口，线程的切换是额外的开销。在实际开发的时候，还要考虑这些线
程是否牵涉到其他堵塞操作的情况。如果某线程进行堵塞操作，系统则将其挂起，让别的
线程获得运行时间。因此，如果有这样的情况，可以多创建几个线程来尽量利用时间。
    应用完成端口分两步走：
    1. 创建完成端口句柄：
    HANDLE hIocp;
    hIocp=CreateIoCompletionPort(
    INVALID_HANDLE_VALUE,
    NULL,
    (ULONG_PTR)0,
    0);
    if(hIocp==NULL) {
    //如果错误
      ……
    }
    注意在第1个参数(FileHandle)传入INVALID_FILE_HANDLE，第2个参数(ExistingCompletionPort)
传入NULL，系统将创建一个新的完成端口句柄，没有任何I/O句柄与其关联。
    2. 完成端口创建成功后，在Socket和完成端口之间建立关联。再次调用CreateIoCompletionPort
函数，这一次在第1个参数FileHandle传入创建的Socket句柄，参数ExistingCompletionPort
为已经创建的完成端口句柄。
    以下代码创建了一个Socket并把它和完成端口联系起来。
    SOCKET s;
    s=Socket(AF_INET,SOCK_STREAM,0);
    if(s==INVALID_SOCKET) {
    if(CreateIoCompletionPort((HANDLE)s,
    hIocp,
    (ULONG_PTR)0,
    0)==NULL)
    {
    //如果创建失败
      ……
    }
    }
    到此为止，Socket已经成功和完成端口相关联。在此Socket进行的重叠I/O操作结果均
使用完成端口发出通知。
    注意：CreateIoCompletionPort函数的第3个参数允许开发人员传入一个类型为ULONG_PTR
的数据成员，我们把它称为完成键(Completion Key)，此数据成员可以设计为指向包含Socket
信息的一个结构体的一个指针，用来把相关的环境信息和Socket联系起来，每次完成通知来
到的同时，该环境信息也随着通知一起返回给开发人员。
    完成端口创建以及与Socket关联之后，就要创建一个或多个工作线程来处理完成通知，
每个线程都可以循环地调用GetQueuedCompletionStatus函数，检查完成端口上的通知事件。
    在举例说明一个典型的工作线程之前，我们先讨论一下重叠I/O的过程。到一个重叠I/O
被发起，一个Overlapped结构体的指针就要作为参数传递给系统。当操作完成时，
GetQueueCompletionStatus就可以返回指向同一个Overlapped结构的指针。为了辨认和定位
这个已完成的操作，开发人员最好定义自己的OVERLAPPED结构，以包含一些自己定义的关于
操作本身的额外信息。比如：
    typedef struct _OVERLAPPELUS {
      OVERLAPPED ol;
      SOCKET s, sclient;
      int OpCode;
      WSABUF wbuf;
      DWORD dwBytes, dwFlags;
    } OVERLAPPELUS;
    此结构的第1个成员为默认的OVERLAPPED结构，第2和第3个为本地服务Socket和与该
操作相关的客户socket，第4个成员为操作类型，对于Socket，现在定义的有以下3种：
    #define OP_READ 0
    #define OP_WRITE 1
    #define OP_ACCEPT 2
    然后还有应用程序的Socket缓冲区，操作数据量，标志位以及其他开发人员认为有用
的信息。
    当进行重叠I/O操作，把OVERLAPPELUS结构作为重叠I/O的参数lpOverlapp传递（如
WSASend，WASRecv，等函数的lpOverlapped参数，要求传入一个OVERLAPP结构的指针）。
    当操作完成后，GetQueuedCompletionStatus函数返回一个LPOVERLAPPED类型的指针，
这个指针其实是指向开发人员定义的扩展OVERLAPPELUS结构，包含着开发人员早先传入的
全部信息。
    注意：OVERLAPPED成员不一定要求是OVERLAPPELUS扩展结构的一个成员，在获得
OVERLAPPED指针之后，可以用CONTAINING_RECORD宏获得相应的扩展结构的指针。
    典型的Worker Thread结构：
    DWORD WINAPI WorkerThread(LPVOID lpParam)
    {
      ULONG_PTR *PerHandleKey;
      OVERLAPPED *Overlap;
      OVERLAPPELUS *OverlapPlus, *newolp;
      DWORD dwBytesXfered;
      while(1)
      {
        ret=GetQueuedCompletionStatus(
        hIocp,
        &dwBytesXfered,
        (PULONG_PTR)&PerHandleKey,
        &Overlap,
        INFINITE);
        if(ret==0)
        {
          //如果操作失败
          continue;
        }
        OverlapPlus=CONTATING_RECORD(Overlap, OVERLAPPELUS, ol);
        switch(OverlapPlus->OpCode)
        {
          case OP_ACCEPT:
            CreateIoCompletionPort(
                (HANDLE)OverlapPlus->sclient,
                hIocp,
                (ULONG_PTR)0,
                0);
            newolp=AllocateOverlappelus();
            newolp->s=OverlapPlus->sclient;
            newolp->OpCode=OP_READ;
            PrepareSendBuffer(&newolp->wbuf);
            ret=WSASend(
              newolp->s,
              &newolp->wbuf,
              1,
              &newolp->dwBytes,
              0,
              &newolp.ol,
              NULL);
            if(ret==SOCKET_ERROR)
            {
              if(WSAGetLastError()!=WSA_IO_PENDING)
              {
                //进行错误处理
                ……
              }
            }
            FreeOverlappelus(OverlapPlus);
            SetEvent(hAcceptThread);
            break;
          case OP_READ:
            memset(&OverlapPlus->ol,0,sizeof(OVERLAPPED));
            ret=WSARecv(
                OverlapPlus->s,
                &OverlapPlus->wbuf,
                1,
                &OverlapPlus->dwBytes,
                &OverlapPlus->dwFlags,
                &OverlapPlus->ol,
                NULL);
            if(ret==SOCKET_ERROR)
            {
              if(WSAGetLastError()!=WSA_IO_PENDING)
              {
                //错误处理
                ……
              }
            }
            break;
          case OP_WRITE:
            break;
        }/*switch结束*/
      }/*while结束*/
    }/*WorkerThread结束*/
    注意：如果Overlapped操作立刻失败（比如，返回SOCKET_ERROR或其他非
    WSA_IO_PENDING的错误），则没有任何完成通知事件会被放到完成端口队列里。反之，
    则一定有相应的通知事件被放到端口队列。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/guitao_w/archive/2010/01/05/5138220.aspx](http://blog.csdn.net/guitao_w/archive/2010/01/05/5138220.aspx)
