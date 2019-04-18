# Winsock 完成端口模型简介 - 深之JohnChen的专栏 - CSDN博客

2005年12月19日 15:49:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2059


摘自《Networking Programming for Microsoft Windows》第八章

“完成端口”模型是迄今为止最为复杂的一种I/O模型。然而，假若一个应用程序同时需要管理为数众多的套接字，那么采用这种模型，往往可以达到最佳的系统性能！

从本质上说，完成端口模型要求我们创建一个Win32完成端口对象，通过指定数量的线程，对重叠I/O请求进行管理，以便为已经完成的重叠I/O请求提供服务。

使用这种模型之前，首先要创建一个I/O完成端口对象，用它面向任意数量的套接字句柄，管理多个I/O请求。要做到这一点，需要调用CreateCompletionPort函数。
该函数定义如下：

HANDLE CreateIoCompletionPort(
    HANDLE FileHandle,
    HANDLE ExistingCompletionPort,
    ULONG_PTR CompletionKey,
    DWORD NumberOfConcurrentThreads
);

在我们深入探讨其中的各个参数之前，首先要注意该函数实际用于两个明显有别的目的：
1. 用于创建一个完成端口对象。
2. 将一个句柄同完成端口关联到一起。 

最开始创建一个完成端口时，唯一感兴趣的参数便是NumberOfConcurrentThreads（并发线程的数量）；前面三个参数都会被忽略。NumberOfConcurrentThreads参数的特殊之处在于，它定义了在一个完成端口上，同时允许执行的线程数量。理想情况下，我们希望每个处理器各自负责一个线程的运行，为完成端口提供服务，避免过于频繁的线程“场景”切换。若将该参数设为0，表明系统内安装了多少个处理器，便允许同时运行多少个线程！可用下述代码创建一个I/O完成端口：

hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

该语句的作用是返回一个句柄，在为完成端口分配了一个套接字句柄后，用来对那个端口进行标定（引用）。 

一、工作者线程与完成端口
成功创建一个完成端口后，便可开始将套接字句柄与对象关联到一起。但在关联套接字之前，首先必须创建一个或多个“工作者线程”，以便在I/O请求投递给完成端口对象后，为完成端口提供服务。在这个时候，大家或许会觉得奇怪，到底应创建多少个线程，以便为完成端口提供服务呢？这实际正是完成端口模型显得颇为“复杂”的一个方面，因为服务I/O请求所需的数量取决于应用程序的总体设计情况。在此要记住的一个重点在于，在我们调用CreateIoCompletionPort时指定的并发线程数量，与打算创建的工作者线程数量相比，它们代表的并非同一件事情。早些时候，我们曾建议大家用CreateIoCompletionPort函数为每个处理器
都指定一个线程（处理器的数量有多少，便指定多少线程）以避免由于频繁的线程“场景”交换活动，从而影响系统的整体性能。CreateIoCompletionPort函数的NumberOfConcurrentThreads参数明确指示系统：在一个完成端口上，一次只允许n个工作者线程运行。假如在完成端口上创建的工作者线程数量超出n个，那么在同一时刻，最多只允许n个线程运行。但实际上，在一段较短的时间内，系统有可能超过这个值，但很快便会把它减少至事先在CreateIoCompletionPort函数中设定的值。那么，为何实际创建的工作者线程数量有时要比CreateIoCompletionPort函数设定的多一些呢？这样做有必要吗？如先前所述，这主要取决于
应用程序的总体设计情况。假定我们的某个工作者线程调用了一个函数，比如Sleep或WaitForSingleObject，但却进入了暂停（锁定或挂起）状态，那么允许另一个线程代替它的位置。换言之，我们希望随时都能执行尽可能多的线程；当然，最大的线程数量是事先在CreateIoCompletionPort调用里设定好的。这样一来，假如事先预计到自己的线程有可能暂时处于停顿状态，那么最好能够创建比CreateIoCompletionPort的NumberOfConcurrentThreads参数的值多的线程，以便到时候充分发挥系统的潜力。一旦在完成端口上拥有足够多的工作者线程来为I/O请求提供服务，便可着手将套接字句柄同完成端口关联到一起。这要求我们在一个现有的完成端口上，调用CreateIoCompletionPort函数，同时为前三个参数——FileHandle，ExistingCompletionPort和CompletionKey——提供套接字的信息。其中， FileHandle参数指定一个要同完成端口关联在一起的套接字句柄。ExistingCompletionPort参数指定的是一个现有的完成端口。CompletionKey（完成键）参数则指定要与某个特定套接字句柄关联在一起的“单句柄数据”；在这个参数中，应用程序可保存与一个套接字对应的任意类型的信息。之所以把它叫作“单句柄数据”，是由于它只对
应着与那个套接字句柄关联在一起的数据。可将其作为指向一个数据结构的指针，来保存套接字句柄；在那个结构中，同时包含了套接字的句柄，以及与那个套接字有关的其他信息。

根据我们到目前为止学到的东西，首先来构建一个基本的应用程序框架。下面阐述了如何使用完成端口模型，来开发一个ECHO服务器应用。在这个程序中，我们基本上按下述步骤行事：

1) 创建一个完成端口。第四个参数保持为0，指定在完成端口上，每个处理器一次只允许执行一个工作者线程。
2) 判断系统内到底安装了多少个处理器。
3) 创建工作者线程，根据步骤2)得到的处理器信息，在完成端口上，为已完成的I/O请求提供服务。
4) 准备好一个监听套接字，在端口5150上监听进入的连接请求。
5) 使用accept函数，接受进入的连接请求。
6) 创建一个数据结构，用于容纳“单句柄数据”，同时在结构中存入接受的套接字句柄。
7) 调用CreateIoCompletionPort，将自accept返回的新套接字句柄同完成端口关联到一起。通过完成键（CompletionKey）参数，将单句柄数据结构传递给CreateIoCompletionPort。
8) 开始在已接受的连接上进行I/O操作。在此，我们希望通过重叠I/O机制，在新建的套接字上投递一个或多个异步WSARecv或WSASend请求。这些I/O请求完成后，一个工作者线程会为I/O请求提供服务，同时继续处理未来的I/O请求，稍后便会在步骤3 )指定的工作者例程中，体验到这一点。
9) 重复步骤5 ) ~ 8 )，直至服务器中止。

二、完成端口和重叠I/O
将套接字句柄与一个完成端口关联在一起后，便可以套接字句柄为基础，投递发送与接收请求，开始对I/O请求的处理。接下来，可开始依赖完成端口，来接收有关I/O操作完成情况的通知。从本质上说，完成端口模型利用了Win32重叠I/O机制。在这种机制中，象WSASend和WSARecv这样的Winsock API调用会立即返回。此时，需要由我们的应用程序负责在以后的某个时间，通过一个OVERLAPPED结构，来接收调用的结果。在完成端口模型中，要想做到这一点，需要使用GetQueuedCompletionStatus（获取排队完成状态）函数，让一个或多个工作者线程在完成端口上等待。该函数的定义如下：

BOOL GetQueuedCompletionStatus(
    HANDLE CompletionPort,
    LPDWORD lpNumberOfBytes,
    PULONG_PTR lpCompletionKey,
    LPOVERLAPPED* lpOverlapped,
    DWORD dwMilliseconds
);

其中，CompletionPort参数对应于要在上面等待的完成端口。lpNumberOfBytes参数负责在完成了一次I/O操作后（如WSASend或WSARecv），接收实际传输的字节数。lpCompletionKey参数为原先传递进入CreateIoCompletionPort函数的套接字返回“单句柄数据”。如我们早先所述，大家最好将套接字句柄保存在这个“键”（Key）中。lpOverlapped参数用于接收完成的I/O操作的重叠结果。这实际是一个相当重要的参数，因为可用它获取每个I/O操作的数据。而最后一个参数，dwMilliseconds，用于指定调用者希望等待一个完成数据包在完成端口上出现的时间。假如将其设为INFINITE，调用会无休止地等待下去。 

三、单句柄数据和单I/O操作数据
一个工作者线程从GetQueuedCompletionStatus这个API调用接收到I/O完成通知后，在lpCompletionKey和lpOverlapped参数中，会包含一些必要的套接字信息。利用这些信息，可通过完成端口，继续在一个套接字上的I/O处理。通过这些参数，可获得两方面重要的套接字数据：单句柄数据，以及单I/O操作数据。其中，lpCompletionKey参数包含了“单句柄数据”，因为在一个套接字首次与完成端口关联到一起的时候，那些数据便与一个特定的套接字句柄对应起来了。这些数据正是我们在进行CreateIoCompletionPort API调用的时候，通过CompletionKey参数传递的。如早先所述，应用程序可通过该参数传递任意类型的数据。通常情况下，应用程序会将与I/O请求有关的套接字句柄保存在这里。lpOverlapped参数则包含了一个OVERLAPPED结构，在它后面跟随“单I/O操作数据”。我们的工作者线程处理一个完成数据包时（将数据原封不动打转回去，接受连接，投递另一个线程，等等），这些信息是它必须要知道的。单I/O操作数据可以是追加到一个OVERLAPPED结构末尾的、任意数量的字节。假如一个函数要求用到一个OVERLAPPED结构，我们便必须将这样的一个结构传递进去，以满足它的要求。要想做到这一点，一个简单的方法是定义一个结构，然后将OVERLAPPED结构作为新结构的第一个元素使用。举个例子来说，可定义下述数据结构，实现对单I/O操作数据的管理：

typedef struct
{
    OVERLAPPED Overlapped;
    WSABUF     DataBuf;
    CHAR       Buffer[DATA_BUFSIZE];
    BOOL       OperationType;
}PER_IO_OPERATION_DATA

该结构演示了通常要与I/O操作关联在一起的某些重要数据元素，比如刚才完成的那个I/O操作的类型（发送或接收请求）。在这个结构中，我们认为用于已完成I/O操作的数据缓冲区是非常有用的。要想调用一个Winsock API函数，同时为其分配一个OVERLAPPED结构，既可将自己的结构“造型”为一个OVERLAPPED指针，亦可简单地撤消对结构中的OVERLAPPED元素的引用。如下例所示：

PER_IO_OPERATION_DATA PerIoData;
// 可像下面这样调用一个函数
  WSARecv(socket, ..., (OVERLAPPED *)&PerIoData);
// 或像这样
  WSARecv(socket, ..., &(PerIoData.Overlapped));

在工作线程的后面部分，等GetQueuedCompletionStatus函数返回了一个重叠结构（和完成键）后，便可通过撤消对OperationType成员的引用，调查到底是哪个操作投递到了这个句柄之上（只需将返回的重叠结构造型为自己的PER_IO_OPERATION_DATA结构）。对单I/O操作数据来说，它最大的一个优点便是允许我们在同一个句柄上，同时管理多个I/O操作（读/写，多个读，多个写，等等）。大家此时或许会产生这样的疑问：在同一个套接字上，真的有必要同时投递多个I/O操作吗？答案在于系统的“伸缩性”，或者说“扩展能力”。例如，假定我们的机器安装了多个中央处理器，每个处理器都在运行一个工作者线程，那么在同一个时
候，完全可能有几个不同的处理器在同一个套接字上，进行数据的收发操作。 

最后要注意的一处细节是如何正确地关闭I/O完成端口—特别是同时运行了一个或多个线程，在几个不同的套接字上执行I/O操作的时候。要避免的一个重要问题是在进行重叠I/O操作的同时，强行释放一个OVERLAPPED结构。要想避免出现这种情况，最好的办法是针对每个套接字句柄，调用closesocket函数，任何尚未进行的重叠I/O操作都会完成。一旦所有套接字句柄都已关闭，便需在完成端口上，终止所有工作者线程的运行。要想做到这一点， 需要使用PostQueuedCompletionStatus函数，向每个工作者线程都发送一个特殊的完成数据包。该函数会指示每个线程都“立即结束并退出”。下面是PostQueuedCompletionStatus函数的定义：

BOOL PostQueuedCompletionStatus(
    HANDLE CompletionPort,
    DWORD dwNumberOfBytesTransferred,
    ULONG_PTR dwCompletionKey,
    LPOVERLAPPED lpOverlapped
);

其中，CompletionPort参数指定想向其发送一个完成数据包的完成端口对象。而就dwNumberOfBytesTransferred、dwCompletionKey和lpOverlapped这三个参数来说，每一个都允许我们指定一个值，直接传递给GetQueuedCompletionStatus函数中对应的参数。这样一来，一个工作者线程收到传递过来的三个GetQueuedCompletionStatus函数参数后，便可根据由这三个参数的某一个设置的特殊值，决定何时应该退出。例如，可用dwCompletionPort参数传递0值，而一个工作者线程会将其解释成中止指令。一旦所有工作者线程都已关闭，便可使用CloseHandle函数，关闭完成端口，最终安全退出程序。

注：CreateIoCompletionPort ，PostQueuedCompletionStatus ，GetQueuedCompletionStatus 等函数的用法说明。

Platform SDK: Storage 


I/O Completion Ports

I/O completion ports are the mechanism by which an application uses a pool of threads that was created when the application was started to process asynchronous I/O requests. These threads are created for the sole purpose of processing I/O requests. Applications that process many concurrent asynchronous I/O requests can do so more quickly and efficiently by using I/O completion ports than by using creating threads at the time of the I/O request. 





I/O完成端口(s)是一种机制，通过这个机制，应用程序在启动时会首先创建一个线程池，然后该应用程序使用线程池处理异步I/O请求。这些线程被创建的唯一目的就是用于处理I/O请求。对于处理大量并发异步I/O请求的应用程序来说，相比于在I/O请求发生时创建线程来说，使用完成端口(s)它就可以做的更快且更有效率。 





The [CreateIoCompletionPort](http://www.csdn.net/editor/Editor.htm#_CreateIoCompletionPort) function associates an I/O completion port with one or more file handles. When an asynchronous I/O operation started on a file handle associated with a completion port is completed, an I/O completion packet is queued to the port. This can be used to combine the synchronization point for multiple file handles into a single object.





[CreateIoCompletionPort](http://www.csdn.net/editor/Editor.htm#CreateIoCompletionPort)函数会使一个I/O完成端口与一个或多个文件句柄发生关联。当与一个完成端口相关的文件句柄上启动的异步I/O操作完成时，一个I/O完成包就会进入到该完成端口的队列中。对于多个文件句柄来说，就可以把这些多个文件句柄合并成一个单独的对象，这个可以被用来结合同步点？ 





A thread uses the GetQueuedCompletionStatus function to wait for a completion packet to be queued to the completion port, rather than waiting directly for the asynchronous I/O to complete. Threads that block their execution on a completion port are released in last-in-first-out (LIFO) order. This means that when a completion packet is queued to the completion port, the system releases the last thread to block its execution on the port. 


调用GetQueuedCompletionStatus函数，某个线程就会等待一个完成包进入到完成端口的队列中，而不是直接等待异步I/O请求完成。线程（们）就会阻塞于它们的运行在完成端口（按照后进先出队列顺序的被释放）。这就意味着当一个完成包进入到完成端口的队列中时，系统会释放最近被阻塞在该完成端口的线程。





When a thread calls GetQueuedCompletionStatus, it is associated with the specified completion port until it exits, specifies a different completion port, or frees the completion port. A thread can be associated with at most one completion port. 


调用GetQueuedCompletionStatus，线程就会将会与某个指定的完成端口建立联系，一直延续其该线程的存在周期，或被指定了不同的完成端口，或者释放了与完成端口的联系。一个线程只能与最多不超过一个的完成端口发生联系。 





The most important property of a completion port is the concurrency value. The concurrency value of a completion port is specified when the completion port is created. This value limits the number of runnable threads associated with the completion port. When the total number of runnable threads associated with the completion port reaches the concurrency value, the system blocks the execution of any subsequent threads that specify the completion port until the number of runnable threads associated with the completion port drops below the concurrency value. The most efficient scenario occurs when there are completion packets waiting in the queue, but no waits can be satisfied because the port has reached its concurrency limit. In this case, when a running thread calls GetQueuedCompletionStatus, it will immediately pick up the queued completion packet. No context switches will occur, because the running thread is continually picking up completion packets and the other threads are unable to run. 


完成端口最重要的特性就是并发量。完成端口的并发量可以在创建该完成端口时指定。该并发量限制了与该完成端口相关联的可运行线程的数目。当与该完成端口相关联的可运行线程的总数目达到了该并发量，系统就会阻塞任何与该完成端口相关联的后续线程的执行，直到与该完成端口相关联的可运行线程数目下降到小于该并发量为止。最有效的假想是发生在有完成包在队列中等待，而没有等待被满足，因为此时完成端口达到了其并发量的极限。此时，一个正在运行中的线程调用GetQueuedCompletionStatus时，它就会立刻从队列中取走该完成包。这样就不存在着环境的切换，因为该处于运行中的线程就会连续不断地从队列中取走完成包，而其他的线程就不能运行了。 





The best value to pick for the concurrency value is the number of CPUs on the machine. If your transaction required a lengthy computation, a larger concurrency value will allow more threads to run. Each transaction will take longer to complete, but more transactions will be processed at the same time. It is easy to experiment with the concurrency value to achieve the best effect for your application. 


对于并发量最好的挑选值就是您计算机中cpu的数目。如果您的事务处理需要一个漫长的计算时间，一个比较大的并发量可以允许更多线程来运行。虽然完成每个事务处理需要花费更长的时间，但更多的事务可以同时被处理。对于应用程序来说，很容易通过测试并发量来获得最好的效果。 





The PostQueuedCompletionStatus function allows an application to queue its own special-purpose I/O completion packets to the completion port without starting an asynchronous I/O operation. This is useful for notifying worker threads of external events. 


PostQueuedCompletionStatus函数允许应用程序可以针对自定义的专用I/O完成包进行排队，而无需启动一个异步I/O操作。这点对于通知外部事件的工作者线程来说很有用。 





The completion port is freed when there are no more references to it. The completion port handle and every file handle associated with the completion port reference the completion port. All the handles must be closed to free the completion port. To close the port handle, call the CloseHandle function. 


在没有更多的引用针对某个完成端口时，需要释放该完成端口。该完成端口句柄以及与该完成端口相关联的所有文件句柄都需要被释放。调用CloseHandle可以释放完成端口的句柄。


