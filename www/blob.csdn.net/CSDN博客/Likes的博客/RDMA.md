# RDMA - Likes的博客 - CSDN博客
2018年10月19日 11:35:34[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：319
来源： https://zcopy.wordpress.com/ 说明： 本文不是对原文的逐字逐句翻译，而是摘取核心部分以介绍RDMA Send操作(后面凡是提到RDMA send, 都对应于IBA里的send操作)。文中给出的例子非常浅显易懂，很值得一读。
**1. What is RDMA | 什么是RDMA**
![](https://images2018.cnblogs.com/blog/1264595/201711/1264595-20171129111107175-701954478.jpg)
**RDMA** is **R**emote **D**irect **M**emory **A**ccess which is a way of moving buffers between two applications across a network. RDMA differs from traditional network interfaces because it bypasses the operating system. This allows programs that implement RDMA to have:
- **The absolute lowest latency**
- **The highest throughput**
- **Smallest CPU footprint**
RDMA指的是远程直接内存访问，这是一种通过网络在两个应用程序之间搬运缓冲区里的数据的方法。RDMA与传统的网络接口不同，因为它绕过了操作系统。这允许实现了RDMA的程序具有如下特点：
- **绝对的最低时延**
- **最高的吞吐量**
- **最小的CPU足迹 （也就是说，需要CPU参与的地方被最小化）**
**2. How Can We Use It | 怎么使用RDMA**
To make use of RDMA we need to have a network interface card that implements an RDMA engine.
使用RDMA, 我们需要有一张实现了RDMA引擎的网卡。
We call this an HCA (Host Channel Adapter). The adapter creates a channel from it’s RDMA engine though the PCI Express bus to the application memory. A good HCA will implement in hardware all the logic needed to execute RDMA protocol over the wire. This includes segmentation and reassembly as well as flow control and reliability. So from the application perspective we deal with whole buffers.
我们把这种卡称之为HCA(主机通道适配器)。 适配器创建一个贯穿PCIe总线的从RDMA引擎到应用程序内存的通道。一个好的HCA将在导线上执行的RDMA协议所需要的全部逻辑都在硬件上予以实现。这包括分组，重组以及流量控制和可靠性保证。因此，从应用程序的角度看，只负责处理所有缓冲区即可。
![](https://images2018.cnblogs.com/blog/1264595/201711/1264595-20171129112503347-1921631622.jpg)
In RDMA we setup data channels using a kernel driver. We call this the command channel. We use the command channel to establish data channels which will allow us to move data bypassing the kernel entirely.  Once we have established these data channels we can read and write buffers directly.
在RDMA中我们使用内核态驱动建立一个数据通道。我们称之为命令通道。使用命令通道，我们能够建立一个数据通道，该通道允许我们在搬运数据的时候完全绕过内核。一旦建立了这种数据通道，我们就能直接读写数据缓冲区。
The API to establish these the data channels are provided by an API called "verbs". The verbs API is a maintained in an open source linux project called the Open Fabrics Enterprise Distribution (OFED). (www.openfabrics.org). There is an equivalent project for Windows WinOF located at the same site.
建立数据通道的API是一种称之为"verbs"的API。"verbs" API是由一个叫做OFED的Linux开源项目维护的。在站点www.openfabrics.org上，为Windows WinOF提供了一个等价的项目。
The verbs API is different from the sockets programming API you might be used to. But once you learn some concepts it is actually a lot easier to use and much simpler to design your programs.
"verbs" API跟你用过的socket编程API是不一样的。但是，一旦你掌握了一些概念后，就会变得非常容易，而且在设计你的程序的时候更简单。
**3. Queue Pairs | 队列对**
RDMA operations start by "pinning" memory. When you pin memory you are telling the kernel that this memory is owned by the application. Now we tell the HCA to address the memory and prepare a channel from the card to the memory. We refer to this as registering a **Memory Region**. We can now use the memory that has been registered in any of the RDMA operations we want to perform. The diagram below show the registered region and buffers within that region in use by the communication queues.
RDMA操作开始于“搞”内存。当你在“搞”内存的时候，就是告诉内核这段内存名花有主了，主人就是你的应用程序。于是，你告诉HCA，就在这段内存上寻址，赶紧准备开辟一条从HCA卡到这段内存的通道。我们将这一动作称之为注册一个内存区域(MR)。一旦MR注册完毕，我们就可以使用这段内存来做任何RDMA操作。在下面的图中，我们可以看到注册的内存区域(**MR**)和被通信队列所使用的位于内存区域之内的缓冲区(buffer)。
![](https://images2018.cnblogs.com/blog/1264595/201711/1264595-20171129114135190-597667307.jpg)
RDMA communication is based on a set of three queues. The send queue and receive queue are responsible for scheduling work. They are always created in pairs. They are referred to as a **Queue Pair(QP)**. A Completion Queue (CQ) is used to notify us when the instructions placed on the work queues have been completed.
RDMA通信基于三条队列(SQ, RQ和CQ)组成的集合。 其中， **发送队列(SQ)**和**接收队列(RQ)**负责调度工作，他们总是成对被创建，称之为**队列对(QP)**。当放置在工作队列上的指令被完成的时候，**完成队列(CQ)**用来发送通知。
A user places instructions on it’s work queues that tells the HCA what buffers it wants to send or receive. These instructions are small structs called **work requests** or **Work Queue Elements (WQE)**. WQE is pronounced "WOOKIE" like the creature from starwars. A WQE primarily contains a pointer to a buffer. A WQE placed on the send queue contains a pointer to the message to be sent. A pointer in the WQE on the receive queue contains a pointer to a buffer where an incoming message from the wire can be placed.
当用户把指令放置到工作队列的时候，就意味着告诉HCA那些缓冲区需要被发送或者用来接受数据。这些指令是一些小的结构体，称之为**工作请求(WR)**或者**工作队列元素(WQE)**。 WQE的发音为"WOOKIE"，就像星球大战里的猛兽。一个WQE主要包含一个指向某个缓冲区的指针。一个放置在发送队列（SQ）里的WQE中包含一个指向待发送的消息的指针。一个放置在接受队列里的WQE里的指针指向一段缓冲区，该缓冲区用来存放待接受的消息。
RDMA is an asynchronous transport mechanism. So we can queue a number of send or receive WQEs at a time. The HCA will process these WQE in order as fast as it can. When the WQE is processed the data is moved. Once the transaction completes a **Completion Queue Element (CQE)** is created and placed on the Completion Queue (CQ). We call a CQE a "COOKIE".
RDMA是一种异步传输机制。因此我们可以一次性在工作队列里放置好多个发送或接收WQE。HCA将尽可能快地按顺序处理这些WQE。当一个WQE被处理了，那么数据就被搬运了。 一旦传输完成，HCA就创建一个**完成队列元素(CQE)**并放置到完成队列(CQ)中去。 相应地，CQE的发音为"COOKIE"。
**4. A Simple Example | 举个简单的例子**
Lets look at a simple example. In this example we will move a buffer from the memory of system A to the memory of system B. This is what we call Message Passing semantics. The operation is a SEND, this is the most basic form of RDMA.
让我们看个简单的例子。在这个例子中，我们将把一个缓冲区里的数据从系统A的内存中搬到系统B的内存中去。这就是我们所说的消息传递语义学。接下来我们要讲的一种操作为SEND，是RDMA中最基础的操作类型。
**Step 1** System A and B have created their QP's Completion Queue's and registered a regions in memory for RDMA to take place. System A identifies a buffer that it will want to move to System B. System B has an empty buffer allocated for the data to be placed.
**第1步：**系统A和B都创建了他们各自的QP的完成队列(CQ), 并为即将进行的RDMA传输注册了相应的内存区域(MR)。 系统A识别了一段缓冲区，该缓冲区的数据将被搬运到系统B上。系统B分配了一段空的缓冲区，用来存放来自系统A发送的数据。
![](https://images2018.cnblogs.com/blog/1264595/201711/1264595-20171129124034378-1190665631.jpg)
**Step 2** System B creates a WQE "WOOKIE" and places in on the Receive Queue. This WQE contains a pointer to the memory buffer where the data will be placed. System A also creates a WQE which points to the buffer in it's memory that will be transmitted.
**第2步：**系统B创建一个WQE并放置到它的接收队列(RQ)中。这个WQE包含了一个指针，该指针指向的内存缓冲区用来存放接收到的数据。系统A也创建一个WQE并放置到它的发送队列(SQ)中去，该WQE中的指针执行一段内存缓冲区，该缓冲区的数据将要被传送。
![](https://images2018.cnblogs.com/blog/1264595/201711/1264595-20171129124051128-1627513745.jpg)
**Step 3** The HCA is always working in hardware looking for WQE's on the send queue. The HCA will consume the WQE from System A and begin streaming the data from the memory region to system B.  When data begins arriving at System B the HCA will consume the WQE in the receive queue to learn where it should place the data. The data streams over a high speed channel bypassing the kernel.
**第3步：**系统A上的HCA总是在硬件上干活，看看发送队列里有没有WQE。HCA将消费掉来自系统A的WQE, 然后将内存区域里的数据变成数据流发送给系统B。当数据流开始到达系统B的时候，系统B上的HCA就消费来自系统B的WQE，然后将数据放到该放的缓冲区上去。在高速通道上传输的数据流完全绕过了操作系统内核。
         注：WQE上的箭头表示指向用户空间内存的指针（地址）。receive/send模式下，通信双方需要事先准备自己的WQE（WorkQueue），HCA完成后会写CQ
![](https://images2018.cnblogs.com/blog/1264595/201711/1264595-20171129124312112-711976107.jpg)
**Step 4** When the data movement completes the HCA will create a CQE "COOKIE". This is placed in the Completion Queue and indicates that the transaction has completed. For every WQE consumed a CQE is generated. So a CQE is created on System A's CQ indicating that the operation completed and also on System B's CQ. A CQE is always generated even if there was an error. The CQE will contain field indicating the status of the transaction.
**第4步：**当数据搬运完成的时候，HCA会创建一个CQE。 这个CQE被放置到完成队列(CQ)中，表明数据传输已经完成。HCA每消费掉一个WQE, 都会生成一个CQE。因此，在系统A的完成队列中放置一个CQE,意味着对应的WQE的发送操作已经完成。同理，在系统B的完成队列中也会放置一个CQE，表明对应的WQE的接收操作已经完成。如果发生错误，HCA依然会创建一个CQE。在CQE中，包含了一个用来记录传输状态的字段。
![](https://images2018.cnblogs.com/blog/1264595/201711/1264595-20171129124303472-47005786.jpg)
The transaction we just demonstrated is an RDMA SEND operation. On Infiniband or RoCE the total time for a relatively small buffer would be about 1.3 µs. By creating many WQE's at once we could move millions of buffers every second.
我们刚刚举例说明的是一个RDMA Send操作。在IB或RoCE中，传送一个小缓冲区里的数据耗费的总时间大约在1.3µs。通过同时创建很多WQE, 就能在1秒内传输存放在数百万个缓冲区里的数据。
**5. Summary | 总结**
This lesson showed you how and where to get the software so you can start using the RDMA verbs API. It also introduced the Queuing concept that is the basis of the RDMA programming paradigm. Finally we showed how a buffer is moved between two systems, demonstrating an RDMA SEND operation.
在这一课中，我们学习了如何使用RDMA verbs API。同时也介绍了队列的概念，而队列概念是RDMA编程的基础。最后，我们演示了RDMA send操作，展现了缓冲区的数据是如何在从一个系统搬运到另一个系统上去的。
### 本文关于RADMA send/receive的论述已经很充分，但是没有讲read/write模式，故补充如下
    传统的TCP/IP通信，发送和接收数据的过程中，都是在源端应用层数据从上向下逐层拷贝封装，目的端从下向上拷贝和解封装，所以比较慢，而且需要CPU参与的次数很多。RDMA通信过程中，发送和接收，读/写操作中，都是RNIC直接和参与数据传输的已经注册过的内存区域直接进行数据传输，速度快，不需要CPU参与，RDMA网卡接替了CPU的工作，节省下来的资源可以进行其它运算和服务。由此可以看出，RDMA可以提供低延迟、高吞吐量、低CPU占用率，适用于高性能计算。
    两种通信方式中都有send、write方法，对应的也有receive、read方法。在传统的TCP/IP通信过程中，SEND/RECEIVE和READ/WRITE操作除了参数不同之外，没有本质的区别，都是进行数据的发送和接收，都是双边操作，即C/S都需要参与，这个过程也需要CPU的参与，并且需要内存拷贝，带来很大的网络延迟。但是在RDMA传输过程中，SEND和WRITE是完全不同的概念，同样，RECEIVE和READ也是完全不同的概念。
    在RDMA传输中，SEND/RECEIVE是双边操作，即需要通信双方的参与，并且RECEIVE要先于SEND执行，这样对方才能发送数据，当然如果对方不需要发送数据，可以不执行RECEIVE操作，因此该过程和传统通信相似，区别在于RDMA的零拷贝网络技术和内核旁路，延迟低，多用于传输短的控制消息。
    WRITE/READ是单边操作，顾名思义，读/写操作是一方在执行，在实际的通信过程中，WRITE/READ操作是由active即客户端来执行的，而passive即服务器不需要执行任何操作。RDMA WRITE操作中，由客户端把数据从本地buffer中直接push到远程QP的虚拟空间的连续内存块中（物理内存不一定连续），因此需要知道目的地址（remote_addr）和访问权限（remote_key）。RDMA READ操作中，是客户端直接到远程的QP的虚拟空间的连续内存块中获取数据poll到本地目的buffer中，因此需要远程QP的内存地址和访问权限。单边操作多用于批量数据传输。
    可以看出，在单边操作过程中，客户端需要知道远程QP的remote_addr（要读取或写入的地址）和remote_key，而这两个信息是通过SEND/REVEIVE操作来交换的，RDMA通信过程的大致流程如下：（注：read/write模式最后返回给另外一端的确认信息并不会放到CQ中，CQ仅仅用云receive/send模式）
1）初始化context，注册内存域
2）建立RDMA连接
3）通过SEND/RECEIVE操作，C/S交换包含RDMA memory region key的MSG_MR消息（一般是客户端先发送）
4）通过WRITE/READ操作，进行数据传输（单边操作）
5）发送MSG_DONE消息，关闭连接
--------------------- 
作者：upupday19 
来源：CSDN 
原文：https://blog.csdn.net/upupday19/article/details/79377984 
版权声明：本文为博主原创文章，转载请附上博文链接！
[https://blog.csdn.net/qq_21125183/article/details/80563463](https://blog.csdn.net/qq_21125183/article/details/80563463)
# 3.7 RDMA操作细节
 RDMA提供了基于消息队列的点对点通信，每个应用都可以直接获取自己的消息，无需操作系统和协议栈的介入。
       消息服务建立在通信双方本端和远端应用之间创建的Channel-IO连接之上。当应用需要通信时，就会创建一条Channel连接，每条Channel的首尾端点是两对Queue Pairs（QP）。每对QP由Send Queue（SQ）和Receive Queue（RQ）构成，这些队列中管理着各种类型的消息。QP会被映射到应用的虚拟地址空间，使得应用直接通过它访问RNIC网卡。除了QP描述的两种基本队列之外，RDMA还提供一种队列Complete Queue（CQ），CQ用来知会用户WQ上的消息已经被处理完。
       RDMA提供了一套软件传输接口，方便用户创建传输请求Work Request(WR），WR中描述了应用希望传输到Channel对端的消息内容，WR通知QP中的某个队列Work Queue(WQ)。在WQ中，用户的WR被转化为Work Queue Element（WQE）的格式，等待RNIC的异步调度解析，并从WQE指向的Buffer中拿到真正的消息发送到Channel对端。
3.7.1 RDAM单边操作 (RDMA READ)![](https://img-blog.csdn.net/20180604113309716?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
READ和WRITE是单边操作，只需要本端明确信息的源和目的地址，远端应用不必感知此次通信，数据的读或写都通过RDMA在RNIC与应用Buffer之间完成，再由远端RNIC封装成消息返回到本端。
对于单边操作，以存储网络环境下的存储为例，数据的流程如下：
1.   首先A、B建立连接，QP已经创建并且初始化。
2.   数据被存档在B的buffer地址VB，注意VB应该提前注册到B的RNIC (并且它是一个Memory Region) ，并拿到返回的local key，相当于RDMA操作这块buffer的权限。
3.   B把数据地址VB，key封装到专用的报文传送到A，这相当于B把数据buffer的操作权交给了A。同时B在它的WQ中注册进一个WR，以用于接收数据传输的A返回的状态。
4.   A在收到B的送过来的数据VB和R_key后，RNIC会把它们连同自身存储地址VA到封装RDMA READ请求，将这个消息请求发送给B，这个过程A、B两端不需要任何软件参与，就可以将B的数据存储到A的VA虚拟地址。
5.   A在存储完成后，会向B返回整个数据传输的状态信息。
单边操作传输方式是RDMA与传统网络传输的最大不同，只需提供直接访问远程的虚拟地址，无须远程应用的参与其中，这种方式适用于批量数据传输。
### 3.7.2 RDMA 单边操作 (RDMA WRITE)
对于单边操作，以存储网络环境下的存储为例，数据的流程如下：
1.   首先A、B建立连接，QP已经创建并且初始化。
2.   数据remote目标存储buffer地址VB，注意VB应该提前注册到B的RNIC(并且它是一个Memory Region)，并拿到返回的local key，相当于RDMA操作这块buffer的权限。
3.   B把数据地址VB，key封装到专用的报文传送到A，这相当于B把数据buffer的操作权交给了A。同时B在它的WQ中注册进一个WR，以用于接收数据传输的A返回的状态。
4.   A在收到B的送过来的数据VB和R_key后，RNIC会把它们连同自身发送地址VA到封装RDMA WRITE请求，这个过程A、B两端不需要任何软件参与，就可以将A的数据发送到B的VB虚拟地址。
5.   A在发送数据完成后，会向B返回整个数据传输的状态信息。
单边操作传输方式是RDMA与传统网络传输的最大不同，只需提供直接访问远程的虚拟地址，无须远程应用的参与其中，这种方式适用于批量数据传输。
###  3.7.3 RDMA 双边操作 (RDMA SEND/RECEIVE)
RDMA中SEND/RECEIVE是双边操作，即必须要远端的应用感知参与才能完成收发。在实际中，SEND/RECEIVE多用于连接控制类报文，而数据报文多是通过READ/WRITE来完成的。
对于双边操作为例，主机A向主机B(下面简称A、B)发送数据的流程如下：
1.   首先，A和B都要创建并初始化好各自的QP，CQ
2.   A和B分别向自己的WQ中注册WQE，对于A，WQ=SQ，WQE描述指向一个等到被发送的数据；对于B，WQ=RQ，WQE描述指向一块用于存储数据的Buffer。
3.   A的RNIC异步调度轮到A的WQE，解析到这是一个SEND消息，从Buffer中直接向B发出数据。数据流到达B的RNIC后，B的WQE被消耗，并把数据直接存储到WQE指向的存储位置。
4.  AB通信完成后，A的CQ中会产生一个完成消息CQE表示发送完成。与此同时，B的CQ中也会产生一个完成消息表示接收完成。每个WQ中WQE的处理完成都会产生一个CQE。
双边操作与传统网络的底层Buffer Pool类似，收发双方的参与过程并无差别，区别在零拷贝、Kernel Bypass，实际上对于RDMA，这是一种复杂的消息传输模式，多用于传输短的控制消息。
--------------------- 
作者：MasterT-J 
来源：CSDN 
原文：https://blog.csdn.net/qq_21125183/article/details/80563463 
版权声明：本文为博主原创文章，转载请附上博文链接！
