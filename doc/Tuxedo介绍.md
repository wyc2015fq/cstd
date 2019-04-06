# Tuxedo介绍

2016年04月02日 21:23:02

waterxcfg304

阅读数：10222

 								标签： 																[tuxedo																](http://so.csdn.net/so/search/s.do?q=tuxedo&t=blog)[middleware																](http://so.csdn.net/so/search/s.do?q=middleware&t=blog)[weblogic																](http://so.csdn.net/so/search/s.do?q=weblogic&t=blog) 							更多

 								个人分类： 																[Middleware																](https://blog.csdn.net/waterxcfg304/article/category/6164477) 							

 									



 **1、Tuxedo介绍**                                                                                  

 **Tuxedo** **是什么**？

 　　Tuxedo是BEA公司（现已被Oracle公司收购）的一个客户机/服务器的“中间件”产品，它在客户机和服务器之间进行调节，以保证正确地处理事务。它用C语言技术开发的并且有很高性能。

 　　TUXEDO是在、Internet 这样的分布式运算环境中开发和管理三层结构的客户/服务器型关键任务应用系统的强有力工具。它具备分布式事务处理和应用通信功能，并提供完善的各种服务来建立、运行和管理关键任务应用系统。开发人员能够用它建立跨多个[硬件](http://syue.com/Digital/)平台、数据库和操作系统的可互操作的应用系统。 

  **Tuxedo 的主要作用是**：

 　　屏蔽分布式环境中各种通信协议、硬件体系结构、操作系统、数据库和其它应用服务等方面的差异，使分布于网络节点上的应用程序的各个单元部件之间能够进行互操作，并协调操作的一致性和完整性，最大限度地节省系统资源，提高系统性能。

 　　* Tuxedo 已经广泛地应用于金融、电信、制造业等各行各业的核心业务系统。

![img](https://img-blog.csdn.net/20160402211220236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 **三层架构**

  从左边往右依次为：客户端层（表现层），中间件服务层（业务逻辑层），数据库服务器层（数据层）。这种典型的三层架构应用非常广泛。对于应用weblogic中间件的系统一般采用的B/S架构，绝大部分采用HTTP协议，少量的系统用java编写的客户端，使用的是RMI  协议，或J2EE里的其它协议。

 对于tuxedo中间件使用的是tuxedo协议，前端开发工具可以是各式各样，VC++ 、java 、Delphi 、VB 等。

 Tuxedo 的通讯过程 

![img](https://img-blog.csdn.net/20160402211010188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



Tuxedo 服务器处理请求的方式与apache有本质的区别。


 　　Apache服务器处理请求，由客户端发出请求到服务器，由服务器对请求进行处理后将数据返回给客户端。


 　　Tuxedo  服务器一次请求需要两次进行两次交互，Tuxedo有两个负责通讯的进程，一个为WSL，WSL的数量可以进行配置，典型的配置一般两、三个；WSH可以有N多个。客户端通过IP地址和端口号与WSL建立连接，由WSL认证请求是否合法，在WSL的响应中包含了另外一个IP地址和端口号；然后，客户端通过拿到的新的IP地址和端口号去请求WSH  。


 　　客户端程序由GUI 与 Tuxeo通讯两部分组成，GUI部分主要由开发人员关心如何设计，通讯部分可能设计成几个函数供开发人员调用。对于性能测试人员可能更关心客户端与服务器之间的通讯过程。



2、tuxedo相关概念



IPC: Inter-Process Communication  进程间通信: 管道、信号量（semaphore）、共享内存（shared memory）、消息队列（Message Queue）。

管道是UNIX系统IPC的最古老形式，数据只能单向流动。

Tuxedo在客户机和服务器通信中大量使用UNIX系统的消息队列。
   SSSO(Single Server Single Queue)模式：每个客户机都有一个响应队列来接受客户端请求。
   MSSO(Multiple Server Single Queue)模式：多个服务器共享同一个请求队列。

信号量包含一个计数器，表示某个资源正在被访问和访问的次数，用来控制多线程对共享数据的访问。

Tuxedo使用共享内存存储公告牌，用来公告进程状态信息和需要在进程间共享或传递的数据。

\-------------------------------------------------------------

Tuxedo的配置文件称为UBBCONFIG或ubb，包含了域（Domain）、逻辑机器(Machine)、服务器组(Group)、服务进程(Server)、服务(Service)的定义。运行前，需要把UBBCONFIG装载成二进制文件，称为TUXCONFIG。

Tuxedo服务启动时，执行tpsvrinit()函数，可以打开一些如数据库之类的资源供以后使用
 Tuxedo服务停止时，执行tpsvrdown()函数，关闭资源
 服务程序调用tpreturn()函数来结束服务请求，并返回一个缓冲区，必要时，将它传给客户程序。

\--------------------------------------------------------

ATMI环境支持的C/S通信方式：请求/应答式通信、回话通信、队列通信、事件代理通信、消息通知
     请求/应答式通信：同步调用(tpcall)、异步调用(tpacall)、嵌套调用、转发调用(tpforward)
                        转发调用和嵌套调用类似，不同的是最里层的嵌套服务可以直接给客户程序一个响应，而不必按照调用栈 逐级返回。


    回话方式：tpsend()/tprecv()  基于事件，分通告和代理
               void (**p)(): 定义了一个指向函数指针的指针p
               tpsetunsol(p) : 将p指向的函数func设置为客户机的事件处理器。
               tpchkunsol(): 检查意外事件

​    事件代理： tppost()/tpsubscribe()  消息发布/订阅
​                 Tuxedo提供了两个事件代理器(TMUSREVT  TMSYSEVT)来处理订阅请求。

​    队列存储： tpenqueue() / tpdequeue()
​               Tuxedo/Q用到了Tuxedo提供的两个服务器：消息队列服务器(TMQUEUE)和消息转发服务器(TMQFORWARD)

\---------------------------

多系统多机之间通信需要每台机器上都有一个Bridge进程，通过TCP/IP通信，Bridge进程维持一个长连接，一旦建立不会断掉。



TUXEDO应用系统的客户端访问TUXEDO服务器上的服务的过程图:







说明：
 WS（Workstation Extension Product）用于指TUXEDO产品的客户端部分
 WSC Workstation Client
 WSL（Workstation Listener） TUXEDO系统自带的一个SERVER，它侦听一个指定的端口，WSC最初与该SERVER建立连接
 WSH（Workstation Handler）TUXEDO系统自带的一个SERVER，由它处理WSC与TUXEDO SERVER之间的通讯。
 Bulletin Board（公告板）TUXEDO把系统的配置保存在一个共享内存中，该共享内存称为公告板（BB）
 BBL TUXEDO的管理进程，主要对公告板等进行管理

 Workstation Client与TUXEDO SERVER建立连接的过程为：
 1．    WSC 调用tpinit()或tpchkauth()
 2．    WSC采用在WSNADDR中指定的IP地址与服务端的WSL建立连接
 3．    WSL为该WSC指定一个WSH，并把该WSH的侦听端口返回给WSC
 4．    WSC采用返回的端口与指定的WSH建立连接，并与WSL断开连接，这之后WSC与TUXEDO SERVER之间的通讯通过WSH进行处理，与WSL无关。
 5．    tpinit()或tpchkauth()调用返回。

\----------------------------------------------------------

单域模式Single-Domain Model。单机模式 Single Host Model, 多机模式Multi-Processor Model
 多域模式Multi-Domain Model