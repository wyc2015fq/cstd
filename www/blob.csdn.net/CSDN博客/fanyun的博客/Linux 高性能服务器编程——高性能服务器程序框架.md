# Linux 高性能服务器编程——高性能服务器程序框架 - fanyun的博客 - CSDN博客
2018年04月15日 20:54:26[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1906
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
问题聚焦：
    核心章节。
    服务器一般分为如下三个主要模块：I/O处理单元（四种I/O模型，两种高效事件处理模块），逻辑单元（两种高效并发模式，有效状态机）和存储单元（不讨论）。
服务器模型
C/S模型
结构：
![](https://img-blog.csdn.net/20140224092200734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
特点：
逻辑简单。
工作流程：
![](https://img-blog.csdn.net/20140224092220812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
I/O复用技术：select，同时监听多个客户请求。
优点：适合资源相对集中的场合。
缺点：当访问量过大，可能所有客户都将得到很慢的相应。
P2P模型
结构：两种结构
![](https://img-blog.csdn.net/20140224092237234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结构b比结构a增加了发现服务器，用于主机之间的互相发现，尽快找到自己需要的资源。
特点：
摒弃了服务器为中心的格局，让网络上所有主机处于对等的地位。
每台机器在消耗服务的同时也给别人提供服务
缺点：当用户之间传输的请求过多时，网络的负载将加重
服务器编程框架
基本框架：
![](https://img-blog.csdn.net/20140224092253984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
            模块说明：
|**模块**|**单个服务器程序**|**服务器集群**|
|----|----|----|
|I/O处理单元|处理客户连接，读写网络数据|作为接入服务器，实现负载均衡|
|逻辑单元|业务进程或线程|逻辑服务器|
|网络存储单元|本地数据库、文件或缓存|数据库服务器|
|请求队列|各单元之间的通信方式|各服务器之间的永久TCP连接|
I/O模型
阻塞I/O
       socket在创建的时候是阻塞的。我们可以给socket系统调用的第2个参数传递SOCK_NONBLOCK 标志，或者通过fcntl 系统调用的F_SETFL 命令，将其设置为非阻塞。
阻塞模型和非阻塞模型：
阻塞I/O：阻塞的文件描述符，系统调用可能因为无法立即完成而被操作系统挂起。
    例如：客户端connect发起连接，服务器相应之前的这段时间，connect调用将被挂起，直到确认报文段到达将之唤起。
**可能被阻塞的系统调用包括accept，send，recv和connect 。**
非阻塞I/O：非阻塞的文件描述符，总是立即返回，不管时间是否发生。
    如果事件没有立即发生，这些系统调用返回-1，这时我们就要确认是延迟还是出错，此时我们必须根据errno来区分这两种情况。
    对accept.send和recv而言，事件未发生时errno通常被设置成EAGAIN（再来一次）或者EWOULDBLOCK（期望阻塞）；对connect而言，errno则被设置成EINPROGRESS（在处理中）。
注意：通常情况下，非阻塞I/O要和其他I/O通知机制一起使用才能提高程序的效率。
I/O复用
常用：I/O通知机制
描述：应用程序通过I/O复用函数向内核注册一组事件，内核通过I/O复用函数把其中就绪的事件通知应用程序。
I/O复用函数：select、poll和epoll_wait，后面的章节会讨论这些函数。
注意：**I/O复用函数本身是阻塞的，它们能提高程序效率的原因在于它们具有同时监听多个I/O事件的能力。**
SIGIO信号
作用：报告I/O事件
描述：我们可以为一个目标文件描述符指定宿主进程，那么指定的宿主进程将捕获到SIGIO信号。这样，当目标文件描述符上有事件发生时，SIGIO信号的信号处理函数将被触发，我们也就可以在该信号处理函数中对目标文件描述符执行非阻塞I/O操作了。
异步I/O模型
上面讨论的三种模型都属于同步I/O模型
同步I/O模型和异步I/O模型的区别
同步：I/O的读写操作发生在I/O事件之后，由应用程序（用户代码）来完成。
异步：异步I/O的读写操作总是立即返回的，不论I/O事件是否被阻塞，因为真正的读写操作被内核接管，即内核来执行I/O操作，具体表现为数据在内核缓冲区和用户缓冲区之间的移动。
可以认为，**同步I/O向应用程序通知I/O就绪事件，异步I/O向应用程序通知I/O完成事件**（可能并没有真正的完成）。
                                                I/O模型对比如下：
|**I/O模型**|**读写操作和阻塞阶段**|
|----|----|
|阻塞I/O|程序阻塞于读写函数|
|I/O复用|程序阻塞于I/O复用系统调用，但可同时监听多个I/O事件，对I/O本身的读写操作是非阻塞的|
|SIGIO信号|信号触发读写就绪事件，用户程序执行读写操作，程序没有阻塞阶段|
|异步I/O|内核执行读写操作并触发读写完成事件，程序没有阻塞阶段|
两种高效的事件处理模式
服务器程序通常需要处理三类事件：I/O事件，信号和定时事件。后面会一次介绍。
这一节先介绍两种高效的事件处理模式：Reactor（同步I/O模型）和Proactor（异步I/O模型）。
Reactor模式
描述：
- 它要求主线程只负责监听文件描述上是否有事件发生，有的话就立即将该事件通知工作线程。
- 除此之外，主线程不做任何其他实质性的工作。
- 工作线程负责读写数据，接受新的连接，以及处理客户请求。
流程：
    使用同步I/O模型（以epoll_wait为例）实现的Reactor模式的工作流程是：
- 主线程往epoll内核事件表中注册socket上的读就绪事件。
- 主线程调用epoll_wait等待socket上有数据可读。
- 当socket上有数据可读时，epoll_wait通知主线程，主线程则将socket可读事件放入请求队列。
- 睡眠在请求队列上的某个工作线程被唤醒，它从socket读取数据，并处理客户请求，然后往epoll内核事件表中注册该socket上的写就绪事件。
- 当socket可写时，epoll_wait通知主线程，主线程将socket可写事件放入请求队列。
- 睡眠在请求队列上的某个工作线程被唤醒，它往socket上写入服务器处理客户请求的结果。
流程图如下：
![](https://img-blog.csdn.net/20140224092317468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Proactor模式
描述：将所有I/O操作都交给主线程和内核来处理，工作线程仅仅负责业务逻辑。更符合之前提到的服务器编程框架。    
流程：使用异步I/O模型（以aio_read和aio_write为例）实现Proactor模式的工作流程是：
- 主线程调用aio_read函数向内核注册socket上的读写完成事件，并告诉内核用户读缓冲区的位置，以及读操作完成后如何通知应用程序。
- 主线程继续处理其他逻辑。
- 当socket上的数据被读入用户缓冲区后，内核将向应用程序发送一个 信号，以通知应用程序数据可用。
- 应用程序预先定义好的信号处理函数选择一个工作线程来处理客户请求。工作线程处理完客户请求之后，调用aio_write函数想内核注册socket的写完成事件，并告诉内核用户写缓冲区的位置，以及写操作完成时如何通知应用程序。
- 主线程继续处理其他逻辑。
- 当用户缓冲区的数据被写入socket之后，内核将向应用程序发送一个信号，以通知应用程序数据已经发送完毕。
- 应用程序预先定义好的信号处理函数选择一个工作线程来做善后处理，比如决定是否关闭socket。
流程图如下：
![](https://img-blog.csdn.net/20140224092338234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
       连接socket上的读写事件是通过aio_read/aio_write 向内核注册的，因此内核将通过信号来向应用程序报告连接socket上的读写事件。所以，主线程中的epoll_wait调用仅能用来检测监听socket 上的连接请求事件，而不能用来检测连接socket 上的读写事件。

同步I/O方式模拟Proactor模式
原理：主线程执行数据读写操作，读写完成之后，主线程向工作线程通知这一“完成事件”，工作线程处理后续逻辑。
流程：
- 主线程往epoll内核事件表中注册socket上的读就绪事件。
- 主线程调用epoll_wait等待socket上有数据可读。
- 当socket上有数据可读时，epoll_wait通知主线程。主线程从socket循环读取数据，直到没有更多数据可读，然后将读取到的数据封装成一个请求对象并插入请求队列。
- 睡眠在请求队列上的某个工作线程被唤醒，它获得请求对象并处理客户请求，然后往epoll内核事件表中注册socket上的写就绪事件。
- 主线程调用epoll_wait等待socket可写。
- 当socket可写时，epoll_wait通知主线程。主线程往socket上写入服务器处理客户请求的结果。
流程图如下：
![](https://img-blog.csdn.net/20140224092409312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
两种高效的并发模式
并发模式适合：I/O密集型任务
方式：多进程和多线程（后面讨论）
描述：并发模式是指I/O处理单元和多个逻辑单元之间协调完成任务的方法。
服务器主要有两种并发编程模式：
- 半同步/半异步模式
- 领导者/追随者模式
半同步/半异步模式
解释：这里的“同步”和“异步”
同步：程序完全按照代码序列的顺序执行
异步：程序的执行需要由系统事件来驱动，这里的系统事件包括中断、信号等。
![](https://img-blog.csdn.net/20140224092446468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同步线程：按照同步方式运行的线程称为同步线程
异步线程：按照异步方式运行的线程称为异步线程
半同步/半异步模式：同步线程用于处理客户逻辑，异步线程用于处理I/O事件。
半同步/半反应堆模式
结合考虑两种事件处理模式（Reactor和Proactor）和几种I/O模型（阻塞I/O，I/O复用，SIGIO信号，异步I/O），则半同步/半异步就存在多种变体
半同步/半反应堆模式就是其中的一种。
如下图所示：
![](https://img-blog.csdn.net/20140224092506984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
特点：
- 异步线程只有一个，由主线程来充当，负责监听所有socket上的事件。
- 如果有新的连接请求，主线程就接受之，以得到新的连接socket
- 在epoll内核事件表中注册该socket上的读写事件 
- 如果连接socket上有读写事件发生，即有新的客户请求到来或有数据要发送到客户端，主线程就将该连接socket插入请求队列。
- 所有工作线程都睡眠在请求队列上，当有任务到来时，它们将通过竞争获得任务的接管权。
领导者/追随者模式
描述：多个工作线程轮流获得事件源集合，轮流监听、分发并处理事件的一种模式。
关键：领导者的变换和I/O事件的处理
实现：在任意时间点，程序都仅有一个领导者线程， 它负责监听I/O事件，而其他线程都是追随者，它们休眠在进程池等待成为新的领导者。当前领导者如果检测到I/O事件，首先要从线程池中推选出新的领导者线程，然后处理I/O事件。
结构：
![](https://img-blog.csdn.net/20140224092529312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
说明：
句柄集：表示I/O资源，在Linux下通常就是一个文件描述符。
线程集：所有工作线程的管理者。负责各线程之间的同步和新领导者线程的推选。
事件处理器及其子类： 用回调函数的方式处理某事件发生时对应的业务。
工作流程：
![](https://img-blog.csdn.net/20140224092601953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenM2MzQxMzQ1Nzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
To be continued：后面的专题将介绍有限状态机和提高服务器性能的一些建议
小结：
这篇主要介绍了服务器方面的核心框架和设计模式，是这个系列的核心。后续的篇幅都是实现这些模型的技术相关的介绍。
服务器编程的路很深，但技术方面也是稳定的，不像前端技术那样技术革新很频繁和有趣。
参考资料：
《Linux高性能服务器编程》
from: [http://blog.csdn.net/zs634134578/article/details/19806429](http://blog.csdn.net/zs634134578/article/details/19806429)
