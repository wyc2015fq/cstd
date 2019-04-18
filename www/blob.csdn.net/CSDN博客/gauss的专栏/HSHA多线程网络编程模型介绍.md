# HSHA多线程网络编程模型介绍 - gauss的专栏 - CSDN博客
2014年04月27日 10:48:27[gauss](https://me.csdn.net/mathlmx)阅读数：571
个人分类：[多线程](https://blog.csdn.net/mathlmx/article/category/954554)
我在上一篇的分享《Leader/Follower多线程网络模型介绍》中详细介绍了LF多线程网络模型的原理和代码分析。它的思路是减少线程上下文切换和数据拷贝时间，通过角色转换，来提高处理效率，尤其适用于处理短暂的、原子的并且反复的动作中的事件。可能大家都比较熟悉这种常用的思路了，那就是分层。它的核心思想：通过把异步和同步分开，用异步的方式来处理底层网络问题，用同步的方式来简化应用服务流程，通过合理的配合，达到提升效率的目的。
**1、 引言**
      大家知道，通常高并发软件的服务处理，包括我们的旺旺，都是异步和同步同时存在的。异步用于高效地处理底层系统服务，同步则用于简化应用服务处理。要想从两种编程模型中均获益，高效地协调异步和同步服务处理是非常重要的。异步和同步服务处理通常是相关联的。例如，Web服务器的I/O层往往使用异步读操作来取得HTTP
 Get请求。而在CGI层对GET请求的处理则同步地运行于独立的控制线程。在I/O层异步到达的请求必须通过一种方法与CGI层对请求的同步处理集成在一起。换一个角度，我们再来看看Web客户端，AJAX可以使用异步JavaScript和XML来提高Web客户端的可感知响应速度。通常，异步和同步服务应该相互协作、取长补短。
**2、 基本思想**
     本文介绍的HSHA方法，即HALF-SYNC/HALF-ASYNC模式，将并发软件的服务分解为异步和同步两层，然后增加一个队列层来处理二者直接的交互。使用单独的线程或者进程以同步的方式处理高层服务，比如领域功能、数据查询或文件传输，而底层系统服务则应该采用异步方式。
HALF-SYNC/HALF-ASYNC模式对这三层进行了严格的划分，这使得并发软件更容易理解、调试和改进。而且，异步和同步服务各自的缺陷也不会出现相互传染：异步服务的性能不会因为同步服务被阻塞而降低，同步服务的编程简单性也不会受到异步复杂性的影响。最后，使用排队层可以避免对异步和同步服务层之间的依赖进行硬编码，同时也可以简化消息处理的优先级排序。异步层和同步层的严格解耦合要求这两层之间的通信必须：
1）使用COPIED VALUES——这样就会带来性能上的损失和资源管理上的消耗，因为有更多的数据需要传送；
2）使用IMMUTABLE VALUES——这种做法相对来说属于轻量级的方式，但是其构造上可能更为复杂。
　　总的来说，HALF-SYNC/HALF-ASYNC通过使用层的概念来保证三个不同层的执行模型和通信模型的独立性和封装性。
同步层服务，比如数据库查询、文件传输或者领域功能通常运行在自己的线程中，以便多个服务可以同时执行。
异步层的服务可以通过异步中断或者支持异步I/O的操作系统API实现，后者包括Windows overlapped I/O和I/O完成端口(I/O
 completion ports)，或者POSIX异步I/O系统调用。如果我们将HALF-SYNC/HALF-ASYNC设计成与PROACTOR或者REACTOR事件处理基础设施联合使用，这个事件处理基础设施便是所谓的异步层。虽然REACTOR并非是真正的异步，但如果它的服务实现的是短暂(short-duration)操作，而不是长时间的阻塞，你就会发现它其实具有异步的关键属性。
　　排队层通常是由同步层和异步层所有服务共享的消息队列组成。复杂的排队层可以提供多个消息队列，比如为每个消息优先级或者通信端提供一个消息队列。
**3、 代码分析**
和上一篇一样,我们还是来看一下开源代码实现spserver。
再次介绍一下:spserver是一个实现了半同步/半异步(Half-Sync/Half-Async)和领导者/追随者(Leader/Follower) 模式的服务器框架，能够简化 TCP server 的开发工作。spserver
 使用 c++ 实现，目前实现了以下功能：
Ø  封装了 TCP server 中接受连接的功能
Ø  使用非阻塞型I/O和事件驱动模型，由主线程负责处理所有 TCP 连接上的数据读取和发送，因此连接数不受线程数的限制
Ø  主线程读取到的数据放入队列，由一个线程池处理实际的业务
Ø  一个 http 服务器框架，即嵌入式 web 服务器
Spserver的每个版本都有一定的修改。而基于HSHA的实现则很早的版本就已经有了。在V0.7之后引入了IOCP完成端口。简单地说，iocp就是事件io操作由操作系统完成，完成后才由线程接收处理事件。代码可以在这里看到：
[http://spserver.googlecode.com/svn/trunk/spserver/spiocpserver.cpp](http://spserver.googlecode.com/svn/trunk/spserver/spiocpserver.cpp)
在文件最后找到start函数：
int SP_IocpServer :: start()
{
    ret = SP_IOUtils::tcpListen( mBindIP, mPort, &listenFD, 0 );
if( 0 == ret ) {
acceptArg.mListenSocket = (HANDLE)listenFD;
        ret = sp_thread_create( &thread, NULL, acceptThread, &acceptArg );
        SP_Executor actExecutor( 1, 
"act" );
        SP_Executor workerExecutor( mMaxThreads,"work" );
/* Start the event loop. */
while( 0 == mIsShutdown ) {
            SP_IocpEventCallback::**eventLoop**( &eventArg, &acceptArg );
for( ; NULL != eventArg.getInputResultQueue()->top(); ) {
                SP_Task * task = (SP_Task*)**eventArg.getInputResultQueue()->pop();**
**workerExecutor.execute( task );**
            }
for( ; NULL != eventArg.getOutputResultQueue()->top(); ) {
                SP_Message * msg = (SP_Message*)**eventArg.getOutputResultQueue()->pop();**
void ** arg = ( void** )malloc( 
sizeof( void * ) * 2 );
**actExecutor.execute( outputCompleted, arg );**
            }
        }
delete completionHandler;
        sp_close( listenFD );
    }
return ret;
}
由于篇幅的关系，我只留下了关键几句代码，它的处理流程大致如下：
1）首先，侦听端口，然后通过sp_thread_create创建线程，把侦听的任务交给这个线程。
2）接下来便是循环SP_IocpEventCallback::eventLoop()，所有的 recv/send 都在 eventloop 这个函数调用上完成的。这个层就是属于异步层。
3）eventloop 在 recv 的时候，会调用 MsgDecoder.decode 函数，如果 decode 返回 OK ，说明完整地读入数据了，那么就把对应的数据放入eventArg.mInputResultQueue 里面。在 send 的时候，如果把一个 Message 完整地发送了，那么就把这个 Message 放入 eventArg.mOutputResultQueue。这两个就是队列，队列里面保存的数据一般称为完成事件。
4）workerExecutor 和 actExecutor 是两个线程池，前者用于所请求的业务处理，后者用于输出完成后的后续逻辑。由于完成事件的处理可能会涉及很复杂的业务，可能会使用到数据库或者其他，因此不能直接使用 event_loop 线程，而是使用线程池。这个就是同步层。
接下来看一下eventLoop的处理：
BOOL SP_IocpEventCallback :: eventLoop( SP_IocpEventArg * eventArg, SP_IocpAcceptArg_t * acceptArg )
{
    HANDLE completionPort = eventArg->getCompletionPort();
    BOOL isSuccess = GetQueuedCompletionStatus( completionPort, &bytesTransferred,
            &completionKey, &overlapped, timeout );
if( ! isSuccess ) {
        SP_IocpEventHelper::doClose( iocpSession->mSession );
if( lastError == WAIT_TIMEOUT ) {
**onTimeout**( eventArg );
        } 
return FALSE;
    }
if( eKeyAccept == completionKey ) {
return**onAccept**( acceptArg );
    } elseif( eKeyMsgQueue == completionKey ) {
        SP_IocpMsgQueue * msgQueue = (SP_IocpMsgQueue*)overlapped;
**msgQueue->process();**
return TRUE;
    } elseif( eKeyFree == completionKey ) {
delete iocpSession->mSession;
**free**( iocpSession );
return TRUE;
    } else {
if( SP_IocpEvent_t::eEventRecv == iocpEvent->mType ) {
**onRecv**( iocpSession );
return TRUE;
        }
if( SP_IocpEvent_t::eEventSend == iocpEvent->mType ) {
**onSend**( iocpSession );
return TRUE;
        }
    }
return TRUE;
}
上面也同样省掉了大量代码。大家可能到这里看源码：
[http://spserver.googlecode.com/svn/trunk/spserver/spwin32iocp.cpp](http://spserver.googlecode.com/svn/trunk/spserver/spwin32iocp.cpp)
1）、首先是检查完成端口的完成情况。如果失败了，会进行错误处理，比如超时等。如果标记是消息队列中的消息，则还会调用消息的process。
2）、然后判断完成事件的类型，分别进行onAccept，free，onRecv，onSend的处理。
**4、模式分析**
**HSHA模式也可以从 生产者/消费者 的角度来描述。在 spserver 中，同步层和异步层轮流扮演这两个角色。**异步层从 socket 读入数据，然后放到同步层的队列中，这个时候异步层是生产者，同步层是消费者。同步层处理完之后，把输出数据放到异步层的队列中，这个时候同步层是生产者，异步层是消费者。事实上，这种方式也在大多数操作系统采用的方式，下图是Unix的消息处理示意图。
![](https://img-my.csdn.net/uploads/201206/30/1341045730_1005.jpg)
**异步层**的关键是要响应快速，所以它的关键字应该是**被动**。实现一个结构清晰的异步I/O框架，有多种方式。
- 使用Reactor或Proactor反应堆模式实现多路事件的处理，反应堆模式使用一个单线程的处理循环，把多路的事件派发给多个处理者。这个模式组合了单线程处理循环的简单性和面向对象编程提供的可扩展性。反应堆模式在一个线程（或进程）中进行顺序的消息处理，常用来消除多线程同步和加锁处理的复杂性。一个反应堆可以使用在同步或者异步的事件源上。但是它支持的事件处理者的行为要求是异步的。也就是说，为了不影响其它事件源的响应效率，事件处理者是不能阻塞的。
- 实现一个多级的中断处理机构。这种机构下，当更高级别的任务（如硬中断）要求处理的时候，当前的进程可以被中断。为了防止共享的状态访问时被破坏，异步层使用的数据结构必须被保护（例如提升处理器级别或使用信号量）。例如，在一个操作系统内核中，硬件中断的服务时间很大程度上决定了对多级中断处理机构的需要。如果这个时间能够显著的减少，可以把所有的处理放到硬中断的层次上，这样就可以避免软中断的过度资源消耗。TCP/IP的实现中，就减少输入包的协议处理时间化费，让所有的包处理过程可以在两级中断机构实现。
**同步层的关键字是主动**，主动方式可以让程序更加简单可靠。系统中经常会有时间长的任务，如执行大量流数据传输，或者是等待服务器响应的数据查询。使用主动对象模型（活动对象）实现这些长时任务。活动对象拥有自己的运行栈和寄存器状态，在执行同步I/O的时候可以被阻塞。实现活动对象结构，要求一个控制线程的切换机制，需要一个地方存放和恢复线程的状态（如寄存器的值，栈指针），这些功能足够实现一个非抢占的，没有内存保护的线程机构。任何操作系统的用户线程都可以做到这一点。
**队列层的关键字则是安全高效。**一般需要考虑以下几个问题：
- 并行控制。如果同步任务和异步任务的执行是并行的（如论使用多CPU还是硬件中断），为了避免争用，共享的队列的状态变化必须是连续的。因此，实现队列层的时候，经常使用并行控制机制，如信号量，互斥体和条件变量等。当消息在队列中插入或删除的时候，并行控制保证队列的内部数据结构不被破坏。
- 层到层之间的流量控制。在队列层缓存消息，系统不能提供无限的资源。因此，必须控制同步和异步层之间传输的数据量。例如，在层到层的流控制下，避免同步层的数据量超过网络接口能够传输的极限。同步任务可以被阻塞。因此，可以使用下面的策略：如果其排队超过一定的量，可以让任务阻塞。当异步任务层把排队数降低到一定的水平，就可以重新唤醒同步任务继续执行。相对地，异步层地任务不能被阻塞，当处理过量地数据时，队列层要根据策略丢弃消息。这种情况下，如果使用了一个可靠的，面向连接的网络协议，发送者最终会发现传输超时，要求重新传输数据。
- 数据拷贝消耗。有些系统把队列层放到了用户和内核之间的保护边界上。为了分离不同的保护域，一般使用拷贝数据的方法。然而，这增加了系统总线和内存的负担。当大数据量的消息传输的时候，这可能会降低很大的性能。一种减少数据拷贝的方式：分配一个专用的内存区，这个内存区被同步任务层和异步任务层共享。这样，两层之间可以高效率的交换数据，不需要拷贝。
顺便说一下，这类东西大师级作品还是得靠老外啊。这里有大师级的HSHA介绍，本文从中翻译了一点，其它自己看吧：
[http://www.cs.wustl.edu/~schmidt/PDF/HS-HA.pdf](http://www.cs.wustl.edu/~schmidt/PDF/HS-HA.pdf)
