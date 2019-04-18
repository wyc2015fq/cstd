# ACE学习笔记 - 深之JohnChen的专栏 - CSDN博客

2010年04月08日 12:41:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：8082


**ACE****介绍**

ACE自适配通信[**环境**](javascript:;)（ADAPTIVE Communication Environment）是可以自由[**使用**](javascript:;)、开放源码的面向对象（OO）[**框架**](javascript:;)（Framework），在其中实现了许多用于并发通信软件的核心[**模式**](javascript:;)。ACE提供了一组丰富的可复用C++ Wrapper Facade（[**包装**](javascript:;)外观）和框架组件，可跨越多种平台完成通用的通信软件[**任务**](javascript:;)，其中包括：事件多路分离和事件处理器分派、信号处理、[**服务**](javascript:;)初始化、[**进程**](javascript:;)间通信、共享[**内存管理**](javascript:;)、消息路由、分布式服务动态（重）配置、并发执行和[**同步**](javascript:;)，等等。

ACE的目标用户是[**高性能**](javascript:;)和实时[**通信服务**](javascript:;)和应用的[**开发**](javascript:;)者。它简化了使用进程间通信、事件多路分离、显式动态链接和并发的OO网络应用和服务的开发。此外，通过服务在运行时与应用的动态链接，ACE还使系统的配置和重配置得以自动化。

ACE正在进行持续的改进。Riverace公司（[http://www.riverace.com](http://www.riverace.com/)）采用开放源码商业模式对ACE进行商业支持。此外，ACE开发组的许多成员目前正在进行The ACE [**ORB**](javascript:;)（TAO，[http://www.cs.wustl.edu/~schmidt/TAO.html](http://www.cs.wustl.edu/~schmidt/TAO.html)）的开发工作。

**二、使用ACE****的好处**

使用ACE的好处有：

l**增强可移植性**：在ACE组件的帮助下，很容易在一种OS平台上编写并发网络应用，然后快速地将它们移植到各种其他的OS平台上。而且，因为ACE是开放源码的自由软件，你无需担心被锁定在特定的操作系统平台或编译器上。

l**更好的软件质量：**ACE的设计使用了许多可提高软件质量的关键模式，这些质量因素包括通信软件灵活性、[**可扩展**](javascript:;)性、可复用性和模块性。

l**更高的效率和可预测性：**ACE经仔细设计，支持广泛的应用服务质量（QoS）需求，包括延迟敏感应用的低响应等待时间、高带宽应用的高性能，以及实时应用的可预测性。

l**更容易转换到标准的高级[**中间件**](javascript:;)：**TAO使用了ACE提供的可复用组件和模式。它是CORBA的开发源码、遵循标准的实现，并为高性能和实时系统作了优化。为此，ACE和TAO被设计为能良好地协同工作，以提供全面的中间件解决方案。

**三、ACE****的结构和功能**

下图显示了ACE中的关键组件以及它们的层次关系：

![](http://hi.csdn.net/attachment/201004/8/0_1270701998F10y.gif)

图中的结构和各层的组成部分描述如下。

**四、ACE OS****适配层**

该层直接位于用C写成的本地OS API之上。它提供轻型的类POSIX OS适配层，将ACE中的其他层及组件和以下与OS API相关联的平台专有特性屏蔽开来：

l**并发和同步**：ACE的适配层[**封装**](javascript:;)了用于多线程、多进程和同步的OS API。

l**进程间通信（[**IPC**](javascript:;)****）和共享内存：**ACE的适配层封装了用于本地和远地IPC、以及共享内存的OS API。

l**事件多路分离机制：**ACE的适配层封装了用于对基于I/O、定时器、信号和同步的事件进行同步和异步多路分离的OS API。

l**显式动态链接：**ACE的适配层封装了用于显式动态链接的OS API。显式动态链接允许在安装时或运行时对应用服务进行配置。

l**文件系统机制：**ACE的适配层封装了用于操作文件和目录的OS文件系统API。

ACE OS适配层的可移植性使得ACE可运行在许多操作系统上。ACE已在广泛的OS平台上进行了移植和测试，包括Win32（也就是，在Intel和Alpha平台，使用MSVC++、Borland C++ Builder和IBM Visual Age的WinNT 3.5.x、4.x、2000、Win95/98和WinCE）、Mac OS X、大多数版本的UNIX（例如，SPARC和Intel上的Solaris 1.x和2.x、SGI IRIX 5.x和6.x、DG/UX、HP-UX 9.x、10.x和11.x、DEC/Compaq UNIX 3.x和4.x、AIX 3.x和4.x、UnixWare、SCO，以及可自由使用的UNIX实现，比如Debian Linux 2.x、RedHat Linux 5.2、6.x和7.x、FreeBSD和NetBSD）、实时操作系统（比如，LynxOS、VxWorks、Chorus ClassiX 4.0、QnX Neutrino、RTEMS和PSoS）、MVS OpenEdition和CRAY UNICOS。

由于ACE的OS适配层所提供的抽象，所有这些平台使用同一棵代码树。这样的设计极大地增强了ACE的可移植性和可维护性。此外，还有Java版本的ACE可用（[http://www.cs.wustl.edu/~eea1/JACE.html](http://www.cs.wustl.edu/~eea1/JACE.html)）。

**五、OS****接口的C++Wrapper Facade**

可以直接在ACE OS适配层之上编写高度可移植的C++应用。但是，大多数ACE开发者使用的是上图中所示的C++Wrapper Facade层。通过提供类型安全的C++接口（这些接口封装并增强本地的OS并发、通信、内存管理、事件多路分离、动态链接和文件系统API），ACEWrapper Facade简化了应用的开发。应用可以通过有选择地继承、聚合和/或[**实例**](javascript:;)化下面的组件来组合和使用这些包装：

l**并发和同步组件：**ACE对像互斥体和信号量这样的本地OS多线程和多进程机制进行抽象，以创建高级的OO并发抽象，像[**主动对象**](javascript:;)（[**Active**](javascript:;)[**Object**](javascript:;)）和多态期货（Polymorphic Future）。

l**IPC****和文件系统组件：**ACE C++包装对本地和/或远地IPC机制进行封装，比如socket、TLI、UNIX FIFO和[**STREAM**](javascript:;)管道，以及Win32命名管道。此外，ACE C++包装还封装了OS文件系统API。

l**内存管理组件：**ACE内存管理组件为管理进程间共享内存和进程内堆内存的动态分配和释放提供了灵活和可扩展的抽象。

ACE C++包装提供了许多与ACE OS适配层一样的特性。但是，这些特性是采用C++类和对象、而不是独立的C函数来构造的。这样的OO包装有助于减少正确地学习和使用ACE所需的努力。

例如，C++的使用提高了应用的健壮性，因为C++包装是强类型的。所以，编译器可在编译时、而不是运行时检测类型系统违例。相反，不到运行时，不可能检测像socket或文件系统I/O这样的C一级OS API的类型系统违例。

ACE采用了许多[**技术**](javascript:;)来降低或消除额外的性能开销。例如，ACE大量地使用C++内联来消除额外的方法调用开销；这样的开销可由OS适配层和C++包装所提供的额外的类型安全和抽象层次带来。此外，对于性能要求很高的包装，比如socket和文件I/O的send/recv方法，ACE会避免使用虚函数。

**六、框架**

ACE还含有一个高级的[**网络编程**](javascript:;)框架，集成并增强了较低层次的C++Wrapper Facade。该框架支持将并发分布式服务[**动态配置**](javascript:;)进应用。ACE的框架部分包含以下组件：

l**事件多路分离组件：**ACE [**Reactor**](javascript:;)(反应器)和[**Proactor**](javascript:;)（[**前摄器**](javascript:;)）是可扩展的面向对象多路分离器，它们分派应用特有的处理器，以响应多种类型的基于I/O、定时器、信号和同步的事件。

l**服务初始化组件：**ACE [**Acceptor**](javascript:;)（[**接受器**](javascript:;)）和[**Connector**](javascript:;)（[**连接器**](javascript:;)）组件分别使主动和被动的初始化任务与初始化一旦完成后通信服务所执行的应用特有的任务去耦合。

l**服务配置组件：**ACE [**Service**](javascript:;)[**Configurator**](javascript:;)（服务配置器）支持应用的配置，这些应用的服务可在安装时和/或运行时动态装配。

l**分层的流组件：**ACE Stream组件简化了像用户级协议栈这样的由分层服务组成的通信软件应用的开发。

l**ORB****适配器组件：**通过ORB适配器，ACE可以与单线程和多线程CORBA实现进行无缝集成。

ACE框架组件便利了通信软件的开发，它们无需修改、重编译、重链接，或频繁地重启运行中的应用，就可被更新和扩展。在ACE中，这样的灵活性是通过结合以下要素来获得的：（1）C++语言特性，比如模板、继承和动态绑定，（2）设计模式，比如抽象工厂、策略和服务配置器，以及（3）OS机制，比如显式动态链接和多线程。

**七、分布式服务和组件**

除了OS适配层、C++Wrapper Facade和框架组件，ACE还提供了包装成自包含组件的标准分布式服务库。尽管这些服务组件并不是ACE框架库的严格组成部分，它们在ACE中扮演了两种角色：

1.**分解出可复用分布式应用的“积木”：**这些服务组件提供通用的分布式应用任务的可复用实现，比如名字服务、事件路由、日志、时间同步和网络锁定。

2.**演示ACE****组件的常见用例：**这些分布式服务还演示了怎样用像Reactor、Service Configurator、Acceptor和Connector、Active Object，以及IPC包装这样的ACE组件来有效地开发灵活、高效和可靠的通信软件。

**八、高级分布式计算中间件组件**

即使使用像ACE这样的通信框架，开发健壮、可扩展和高效的通信应用仍富有挑战性。特别是，开发者必须掌握许多复杂的OS和通信的概念，比如：

l网络寻址和服务标识。

l表示转换，比如加密、压缩和在异种终端系统间的字节序转换。

l进程和线程的创建和同步。

l本地和远地进程间通信（IPC）机制的系统调用和库例程。

通过采用像CORBA、DCOM或Java RMI这样的高级分布式计算中间件，可以降低开发通信应用的复杂性。高级分布式计算中间件驻留在客户端和服务器之间，可自动完成分布式应用开发的许多麻烦而易错的方面，包括：

l认证、授权和数据安全。

l服务定位和绑定。

l服务注册和启用。

l事件多路分离和分派。

l在像TCP这样的面向字节流的通信协议之上实现消息帧。

l涉及网络字节序和参数整编（marshaling）的表示转换问题。

为给通信软件的开发者提供这些特性，在ACE中绑定了下面的高级中间件应用：

1.**The ACE ORB****（TAO****）：**TAO是使用ACE提供的框架组件和模式构建的CORBA实时实现，包含有网络接口、OS、通信协议和CORBA中间件组件等特性。TAO基于标准的OMG CORBA参考模型，并进行了增强的设计，以克服传统的用于高性能和实时应用的ORB的缺点。TAO像ACE一样，也是可自由使用的开放源码软件。

2.**JAWS****：**JAWS是高性能、自适配的Web服务器，使用ACE提供的框架组件和模式构建。JAWS被构造成“框架的框架”。JAWS的总体框架含有以下组件和框架：事件多路分派器、并发策略、I/O策略、协议管道、协议处理器和缓存虚拟文件系统。每个框架都被构造成一组协作对象，通过组合和扩展ACE中的组件来实现。JAWS也是可自由使用的开放源码软件。

**九、主页**

ACE的主页为：[http://www.cs.wustl.edu/~schmidt/ACE.html](http://www.cs.wustl.edu/~schmidt/ACE.html)，在这里可获得最新版本的ACE以及其他相关资源。

十、ICE和ACE的区别
同为网络通信中间件，ICE和ACE的区别在于，前者是对通信对象的封装，
例如如客户端的ICE核心和服务器端的ICE核心的不同，
而后者是对通信本身的封装，例如对套接字的封装，对同步异步通信方式的封装。
后者比前者更加底层，更加抽象。ACE可以认为是通用网络编程框架软件，而ICE则是一个跨平台的RPC软件。

**ACE****框架**

ACE的库分为4个层次：

OS适配层该层将ACE的较高层和与OS机制相关联的平台特有的依赖屏蔽开来。

OO包装层封装并增强在像Win32和UNIX这样的现代操作系统上可用的并发、进程间通信（IPC）、以及虚拟内存机制。应用可以通过有选择地继承、聚合（aggregating）、和/或实例化ACE包装类属来合并和编写这些组件

框架包括反应器，服务配置器，流。

ACE 的通讯模式包括接受器－连接器，前摄器两种主要的通讯模式。前摄器理解可以理解为象Windws的Overlapper形式的一种利用操作系统的挂钩进行快速异步处理IO通讯的一种方式。它在某种程度上类似于一种软中断。用户只负责编写并注册相应的挂钩, 操作系统负责监测事件发生，并调用相应的挂钩。接受器－连接器模式是我们经常使用的通讯模式。相对于连接器，接收器模式是服务器处理程序经常重复编写的救世主。程序员在编写服务器处理程序时，无论是采用异步通讯还是阻塞通讯，单个线程还是多个线程，都可以采用接收器方式，由此可见接受器－连接器模式的强大。接受器－连接器模式的服务器端用接收器，客户端使用连接器连接服务器（当然可以采用其他方式连接到采用接收器的服务器上），相对于连接器，接受器简化了服务端编程的复杂度，使程序员从大量重复的工作中解脱出来，并且编写出成熟稳定的服务处理程序，对比以前只有少数具有丰富的通讯程序编写经验的人才能写出稳定健壮的服务处理程序（如web服务器），ACE的接收器可以称之为改写这一历史的巨人。

接受器模式是ACE中最闪耀之处，是通讯程序编写史上的分水岭，他的强大足以使我们震惊。

在ACE文档方面，尽管有马维达翻译的Douglas C.Schmidt和Huston编写的《C++网络编程》卷1，卷2，还有《ACE技术论文集》，《ACE程序员教程》《ACE应用实例》，但是领悟ACE的接收器不是一件容易的事情。原因也许归于开源项目的一个通病--文档比较生僻难懂，或者不全面。所以开源项目领悟的最好方法是结合文档读源代码。

接收器主要有ACE_Acceptor, ACE_Svc_Handler, ACE_Reactor 3个主要类组成。ACE_Reactor是分发器(Dispatcher), ACE_Acceptor 创建出ACE_Svc_Handler.

处理顺序是：

1.ACE_Acceptor的open将自身帮定到ACE_Reactor上，并向其注册：当在PEER_ADDR上发生ACCEPT事件，调用handle_input成员挂钩函数。

2.主程序调用ACE_Reactor的handle_events（）时，检测到ACCEPT，调用ACE_Acceptor的handle_input()。在Handle_Input中继续调用虚函数make_svc_handle()构造出ACE_Svc_Handler类（可以新建，则每客户一个Handler,也可使用单例，则多个客户共用一个服务处理器）。接着调用accept_ svc_handle(),将具体的参数传给ACE_Svc_Handler。最后调用active_ svc_handle()，一般调用ACE_Svc_Handler的open函数。在Open函数中注册反应器事件，如必要调用active（）创建出线程。我们把创建接收器的线程称为主线程，把运行Ace_Reactor的handle_events（）的线程取名为事件分发线程。把运行ACE_Svc_Handler的svc()的线程叫做服务线程。这些线程根据实现不同会有以下几种组合。l

主线程，事件分发线程, 服务线程三者合一在ACE_Svc_Handler的open函数中不调用active(),则服务不创建新的线程。主线程，事件分发线程合一，服务线程运行在ACE_Svc_Handler的open函数中调用active(),则服务线程创建，线程运行ACE_Svc_Handler的svc()。

主线程运行，事件分发线程和服务线程合一。后叙述

主线程，事件分发线程，服务线程都运行在ACE_Svc_Handler的open函数中调用active()；另创建一个线程，循环运行ACE_Reactor的handle_events() 或者run_event_loop();l主线程, 服务线程合一,事件分发线程运行。另创建一个线程，循环运行ACE_Reactor的handle_events() 或者run_event_loop();在ACE_Svc_Handler的open函数中不调用active()。三个线程都运行时，下表显示类的成员函数被线程调用的关系.当3个线程都运行时，缺省状态下ACE_Acceptor和ACE_Svc_Handler使用一个反应器，而反应器监测事件到达时，调用ACE_Event_Handler类（ACE_Acceptor和ACE_Svc_Handler 都继承了ACE_Event_Handler）的handle_*钩子函数。因此，handle_*函数实际上运行在事件分发线程上。一般在ACE_Svc_Handler的handle_input钩子函数中读取数据，也就是说读数据在事件分发线程内执行了，这样读处理没有并行化，可能ACE的设计者认为操作系统的socket尽管有多个，实际的IO处理比如读写是串行的，因此缺省的Connector-Acceptor架构被设计成这样。Svc()运行在服务线程内，对读取的数据进行处理，并用peer()返回的Stream发送结果，这样一来，事件分发线程和服务线程会公用一个Handler,所以应当采用互斥方式访问IO。传统的Soket服务编程一般是主线程在监听soket等待一个客户端的连接，然后产生一个用于和客户端通讯的数据soket,同时创建一个线程或进程，该线程用此socket从客户端接收数据，进行数据处理，然后发送结果到客户端。这就是每客户每处理的通讯服务程序方式。如何用Connector-Acceptor架构实现这一过程呢？如上表格所示，ACE_Svc_Handler的handle_input()应当从事件分发线程线程移到服务线程运行，因此，实现多线程同步IO的方法是将用于ACE_Acceptor的反应器和ACE_Svc_Handler的反应器分开，并且做到每个数据Soket有一个反应器，这样就可以同步访问，这种方式就是前面列出的组合的第3种组合，不过事件分发线程被分为两部分，一部分是反应器分发监听socket的事件函数即run_reactor_event_loop()函数。另一部分是反应器分发数据socket的事件函数即run_reactor_event_loop()函数。具体实现步骤如下：1．在ACE_Svc_Handler 的open函数中, 重新为此 ACE_Svc_Handler实例分配一个新的反应器,并用此实例作为参数注册读写，关闭事件。2．接着调用active(),会创建出线程。3．在ACE_Svc_Handler的svc()中循环调用此反应器的handle_events()或run_reactor_event_loop()函数。4．在handle_input函数中读取数据，然后进行数据分析。发送写消息到反应器。会调用handle_outpu().5．在handle_outpu()发送结果到客户端。经过这样改写，就可以用Connector-Acceptor架构实现每客户每处理的通讯方式。ACE的功能强大，配置灵活多样，以至于很多新手在实现一个程序时面对很多实现方式，一时不能决定究竟如何选择。而现有的关于ACE的书本和文档的例子都很不完整，这些都为初学ACE的程序员增加了难度。我的经验是尽量优先使用模式，然后框架，最后是类层次的复用。所以使用ACE,一定先选择Connector-Acceptor和Proactor,只有当这两者不能满足要求时再考虑使用其它的类。

**ACE****知识点**

1、ace框架与平台选择

先看一下你的模块运行的平台，是windows/linux/unix还是其它。平台不同，使用的ACE框架还是有所差别的。比如windows下面的服务器端，一般都用Proactor框架，配合各种异步操作，如ACE_Asynch_Acceptor/ACE_Asynch_Connector，因为Proactor内部实现是完成端口，在windows平台上，公认可以取得最好的性能。
如果你用Linux，服务器端推荐你使用Reactor框架+Dev_Poll_Reactor实现，这个实现使用了Epoll机制，性能很棒。
客户端，一般为了兼容性考虑，都用Reactor，当然，如果是在windows上面运行，默认实现是WFMO_Reactor。

2、任务和主动对象（Active Object）

在主动对象持有的线程中，如果调用的是被动对象的方法（常规对象），调用会阻塞

（同步的）；而另一方面，如果调用的是主动对象的方法，调用不会阻塞（异步的）。ACE_Task 是 ACE 中的任务或主动对象“处理结构”的基类。 ACE_Task 可用作：

1)更高级的线程（我们称之为任务）。

2)主动对象模式中的主动对象。

3、ACE Reactor框架类，用于检测事件的发生，并随即将这些事件多路分离和分派给它们的事件处理器。就会保证在此事件发生时，自动回调在适当的事件处理器对象中的适当的”handle”方法。

使用 ACE_Reactor基本上有三个步骤：

1、创建 ACE_Event_Handler 的子类，并在其中实现适当的“handle_”方法，以处理你想要此事件处理器为之服务的事件类型。

2、通过调用反应器对象的 register_handler()，将你的事件处理器登记到反应器。

3、在事件发生时，反应器将自动回调相应的事件处理器对象的适当的“handle_”方法。

ACE_Event_Handler是实现向Reactor注册I/O, Signal, Timeout等时间并处理的类, 通过回调函数来进行处理.handle_input, handle_output, handle_timeout, handle_signal等就是分别注册的回调函数, 我们称为handle_*回调函数,注意不包括handle_close.另外需要注意就是handle_close函数, Reactor对于handle_*回调函数的返回值是由规定的,如果返回0就表示处理正常,Reactor会继续保持事件并在发生时派发, 但是如果返回了-1, 就代表了出错, 需要删除注册对应的handle_*回调函数. handle_close()在handle_*回调函数返回-1或者我们显示调用了remove_hadler函数后被调(remove_hadler有个参数, 可以指定是否调用handle_close 指定DONT_CALL就会不调用handle_close,主要用于我们已经在handle_close函数中调用了remove_handle函数,接着操作不需要重复调用handle_close). 

: handle_close()在handle_*回调函数返回-1或者我们显示调用了remove_hadler函数后(无DONT_CALL)被调用, 另Reactor会忽略hanle_close的返回值, 其他的cancel_timer默认的不调用handle_close, 但也可以强制调用, 移除事件处理器使之不再处理信号时,也不调用handle_close()清理工作: 所有的清除操作都集中写在handle_close函数中,清晰不容易出错.

4、MessageQueue消息队列

high_water_mark/low_water_mark设置消息队列的水位标（参数是所有消息块的数据大小，而不是消息块的数量）,水位标用于在消息队列中指示何时在其中的数据已过多（消息队列到达了高水位标），或何时在其中的数据的数量不足（消息队列到达了低水位标）。

ACE_Message_Block ，表示消息块。每个 ACE_Message_Block 都有两个底层指针：rd_ptr 和 wr_ptr，用于在消息块中读写数据。它们可以通过调用 rd_ptr()和 wr_ptr()方法来直接访问。rd_ptr指向下一次读取数据的位置，而 wr_ptr指向下一次写入数据的位置。

5、ACE_Event_Handler，表示事件处理类

ACE_Task，表示主动对象任务

ACE_Svc_Handler，表示服务处理类，它同时继承自 ACE_Task 和 ACE_Event_Handler，并且增加了一个私有数据流。这种结合使得 ACE_Svc_Handler对象能够用作这样的任务：它能够处理事件、并与远地主机的任务间发送和接收数据。

6、ACE的动态库服务编写要点

1），需要定义宏ACE_BUILD_SVC_DLL

2），需要包含ACE/svc_export.h头文件

3），用户类必须直接或间接的派生自ACE_Service_Object

4），声明服务工厂，ACE_SVC_FACTORY_DECLARE(CLS_NAME)，实际上是声明_make_CLS_NAME接口

5），实现类中必须的接口，如init、fini

6），定义服务工厂，ACE_SVC_FACTORY_DEFINE(CLS_NAME)，缺少这一步，将会在调用ace_yyparse后出错，不能正常启用动态库中的服务。

实例：http://egeho123.blogbus.com/logs/20242944.html

7**、**进程间[通信服务](javascript:;)访问点包装

socket、TLI、[STREAM](javascript:;)管道和FIFO为访问局部和全局IPC机制提供广泛的接口。但是，有许多问题与这些不统一的接口有关联。比如类型安全的缺乏和多维度的复杂性会导致成问题的和易错的编程。

[ACE](javascript:;)的IPC SAP类属提供了统一的层次类属，对那些麻烦而易错的接口进行封装。在保持[高性能](javascript:;)的同时，IPC SAP被设计用于改善通信软件的正确性、易学性、可移植性和可复用性。

具体参考：[http://www.acejoy.com/space/html/51/n-51.html](http://www.acejoy.com/space/html/51/n-51.html)

8、内存管理

ACE含有两组不同的类用于内存管理。

第一组是那些基于ACE_Allocator的类。这组类使用动态绑定和策略模式来提供灵活性和可扩展性。它们只能用于局部的动态内存分配。

第二组类基于ACE_Malloc模板类。这组类使用C++模板和外部多态性（External Polymorphism）来为内存分配机制提供灵活性。在这组类中的类不仅包括了用于局部动态内存管理的类，也包括了管理进程间共享内存的类。这些共享内存类使用底层OS（OS）共享内存接口。

为什么使用一组类而不是另外一组呢？这是由在性能和灵活性之间所作的权衡决定的。因为实际的分配器对象可以在运行时改变，ACE_Allocator类更为灵活。这是通过动态绑定（这在C++里需要使用虚函数）来完成的，因此，这样的灵活性并非不需要代价。虚函数调用带来的间接性使得这一方案成了更为昂贵的选择。

另一方面，ACE_Malloc类有着更好的性能。在编译时，malloc类通过它将要使用的内存分配器进行配置。这样的编译时配置被称为“外部多态性”。基于ACE_Malloc的分配器不能在运行时进行配置。尽管ACE_Malloc效率更高，它不像ACE_Allocator那样灵活。

具体参考：http://www.acejoy.com/space/html/50/n-50.html

9、线程管理

线程管理使用ACE_Thread和ACE_Thread_Manager提供的功能，线程之间同步就使用ACE同步类。

ACE_Thread提供了对OS的线程调用的简单包装，这些调用处理线程创建、挂起、取消和删除等问题。它提供给应用[**程序员**](javascript:;)一个简单易用的接口，可以在不同的线程API间移植。ACE_Thread是非常“瘦”的包装，有着很少的开销。其大多数方法都是内联的，因而等价于对底层OS专有线程接口的直接调用。ACE_Thread中的所有方法都是静态的，而且该类一般不进行[**实例**](javascript:;)化。

ACE_Thread_Manager提供了ACE_Thread中的功能的超集。特别地，它增加了管理功能，以使启动、取消、挂起和恢复一组相关线程变得更为容易。它用于创建和销毁成组的线程和任务（ACE_Task是一种比线程更高级的构造，可在ACE中用于进行多线程编程。我们将在后面再来讨论任务）。它还提供了这样的功能：发送信号给一组线程，或是在一组线程上等待，而不是像我们在前面的例子中所看到的那样，以一种不可移植的方式来调用join()。

ACE有若干可用于同步目的的类。这些类可划分为以下范畴：

·ACE Lock类属

·ACE Guard类属

·ACE Condition类属 

·杂项ACE Synchronization类

10、接受器/连接器模式

接受器/连接器模式设计用于降低连接建立与连接建立后所执行的服务之间的耦合。例如，在WWW浏览器中，所执行的服务或“实际工作”是解析和显示客户浏览器接收到的HTML页面。连接建立是次要的，可能通过BSD socket或其他一些等价的IPC机制来完成。[使用](javascript:;)这些模式允许**程序员**专注于“实际工作”，而最少限度地去关心怎样在服务器和客户之间建立连接。而另外一方面，程序员也可以独立于他所编写的、或将要编写的服务例程，去调谐连接建立的策略。

　　因为该模式降低了服务和连接建立方法之间的耦合，非常容易改动其中一个，而不影响另外一个，从而也就可以复用以前编写的连接建立机制和服务例程的代码。在同样的例子中，使用这些模式的浏览器程序员一开始可以构造他的系统、使用特定的连接建立机制来运行它和测试它；然后，如果先前的连接机制被证明不适合他所构造的系统，他可以决定说他希望将底层连接机制改变为多线程的（或许使用线程池策略）。因为此模式提供了严格的去耦合，只需要极少的努力就可以实现这样的变动。

11、Proactor和Reactor都是并发编程中的设计模式。在我看来，他们都是用于派发/分离IO操作事件的。这里所谓的IO事件也就是诸如read/write的IO操作。"派发/分离"就是将单独的IO事件通知到上层模块。两个模式不同的地方在于：Proactor用于异步IO，而Reactor用于同步IO。



**ACE****中日志系统**

以前曾介绍过一个开源日志系统log4cplus，ACE也有自己的Logging Facility，与log4cplus相似，ACE日志系统
也具有线程安全、灵活、分级显示等特点，可以面向程序调试、运行、测试、和维护等全生命周期，可以选择将
信息输出到屏幕、文件、系统日志（如Windows下的Event log）、甚至是远程服务器。除此之外，ACE日志系统
支持回调函数以及运行时刻动态配置（Runtime Configuration ），本文主要参考了《ACE Programmer's Guide,
 The: Practical Design Patterns for Network and Systems Programming》。
ACE日志系统的基本使用首先通过一个引例介绍如何调用ACE Log Macro：#include < ace/Log_Msg.h >int ACE_TMAIN (int, ACE_TCHAR *[])

{

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Hi ACE Logging Facility!/n")));

  return 0;

}程序需要包含头文件：Log_Msg.h，该文件定义了各种有用的日志输出宏，宏ACE_TMAIN 是ACE定义的
wide-character-enabled entry point ，ACE日志系统缺省输出到stderr，所以对于Console类型的应用程序，
会直接输出到屏幕。运行该程序时屏幕上会显示：
 Hi ACE Logging Facility!可以看出ACE日志系统的宏调用非常简单：
 ACE_XXX((severity, formatting-args));其中，ACE_XXX表示日志输出宏（ACE Logging Macros），包括：
ACE_ERROR((level, string, ...))

ACE_DEBUG((level, string, ...))

ACE_TRACE(string)

ACE_ASSERT(test)

ACE_HEX_DUMP((level, buffer, size [,text]))

ACE_RETURN(value)

ACE_ERROR_RETURN((level, string, ...), value)

ACE_ERROR_INIT( value, flags )

ACE_ERROR_BREAK((level, string, ...))severity即输出的严重等级（Severity Level），包括：
LM_TRACE  Messages indicating function-calling sequence

LM_DEBUG  Debugging information

LM_INFO   Messages that contain information normally of use only when debugging a program

LM_NOTICE  Conditions that are not error conditions but that may require special handling

LM_WARNING  Warning messages

LM_ERROR  Error messages

LM_CRITICAL  Critical conditions, such as hard device errors

LM_ALERT  A condition that should be corrected immediately, such as a corrupted database

LM_EMERGENCY     A panic condition, normally broadcast to all usersLM_TRACE Messages indicating

                 function-calling sequenceformatting-args是要输出的内容，格式类似于printf函数的输出格式：Code        Argument Type   DisplaysA           ACE_timer_t     浮点数
a           —              导致程序终止（Abort）
c           char            单个字符
C           char*           字符串（narrow characters）
i,d         int             10进制整数
I           —              缩进
e,E,f,F,g,G double          双精度浮点数
l           —              行号
M           —              severity level的名称
m           —              错误号（errorno）
N           —              文件名
n           —              ACE_Log_Msg::open()指定的程序名
o           int             八进制整数
P           —              当前进程ID

p           ACE_TCHAR*      字符串，后接错误号，同perror

Q           ACE_UINT64      64位无符号十进制整数
r           void (*)()      函数调用
R           int             十进制整数
S           int             数字对应的信号名称
s           ACE_TCHAR*      ACE_TCHAR类型的字符串
T           —              当前时间（hour:minute:sec.usec）
D           —              时戳（month/day/year/hour:minute:sec.usec）
t           —              当前线程ID

u           int             无符号十进制整数
w           wchar_t         Single wide character

W           wchar_t*        Wide-character string

x,X         int             十六进制数
@           void*           指针（十六进制）
%           N/A             %

关于ACE日志系统请参考：

**[http://masterdog.bokee.com/984024.html](http://masterdog.bokee.com/984024.html)**

**[http://masterdog.bokee.com/1035113.html](http://masterdog.bokee.com/1035113.html)**

**ACE****的环境搭建**
Windows下ACE的构建(以VC++6.0环境为例)
1. 将ACE-5.5.zip解压到所需的安装目录，此处以E:/为例，解压后形成ACE_wrappers文件夹，因此ACE将会存在于ACE_wrappers/ace目录中。ACE_ROOT=E:/ACE_wrappers。
2. 在系统中新建ACE_ROOT环境变量，值设为 E:/ACE_wrappers。具体设置为：我的电脑->属性->高级->环境变量->新建
3. 在E:/ACE_wrappers/ace目录中创建一个头文件，命名为config.h，其内容为：
#include "ace/config-win32.h"（必选项）
#define ACE_HAS_STANDARD_CPP_LIBRARY 1 （MSVC自带的标准C++头文件）
#define ACE_HAS_MFC 1（与MFC一起使用）
#define ACE_NO_INLINE（取消函数内联从而减小静态库和运行码的大小）
#define ACE_AS_STATIC_LIBS（构建ACE的静态版本）
#define ACE_HAS_WINNT4 0（在Windows 9x/me上构建所需，在ACE中移掉一些WinNT/Win2K中特有的代码）
　　以上只需要添加前两项就可以了。
4. 在VC++中加载ACE的工程文件（ACE_ROOT /ace/ace.dsw），在release和debug两种编译方式下进行编译，得到相应的库文件(ACE.dll、ACE.lib 和 ACEd.dll、ACEd.lib)，其中ACE.dll 、ACE.lib是由release版生成的，ACEd.dll、ACEd.lib是由debug版生成的。具体操作方法：运行VC++，选择Build ->Batch Build…，将ACE-Win32 Debug和ACE-Win32 Release全部选中，Rebuild All即可。如下图所示：

　　按照上面的方法可能只编译出ACE.dll 、ACE.lib库文件，具体原因还不详。按照下面的操作方法进行编译可以成功生成全部库文件。具体操作方法：Build->Set Active Configuration…分别选中ACE-Win32 Release进行编译即可。如下图所示

　　同样打开ace_static.dsw编译生成ACEs.lib、 ACEsd.lib、ACE_RMCasts.lib、ACE_RMCastsd.lib、TMCasts.lib、TMCastsd.lib

5. 配置ACE环境。打开VC++6.0，选择Tools->Options->Directories 加入以下内容：
1) Executable File：E:/ACE_wrappers/bin
2) Include File：E:/ACE_wrappers
3) Library Files：E:/ACE_wrappers/lib
4) Source Files：E:/ACE_wrappers
　　在编写程序时要在Project->Settings菜单中进行如下设置：
C/C++选项卡：
Code Generation：选择多线程版运行时库（Multithreaded DLL）。
Preprocessor：在“Additional include directories”域中加入$ACE_ROOT。　　
Link选项卡：
Input：在“Object/library modules”域中包含适当的ACE库文件，在“Additional library 
path”域中包含ACE库链接文件的路径。这通常是ACE_ROOT/ace。
其中 DLL debug对应ACEd.lib
 DLL release对应ACE.lib
 Static library debug对应ACEsd.lib
 Static library release对应ACEs.lib
 MFC DLL debug对应ACEmfcd.lib
 MFC DLL release对应ACEmfc.lib
主要用到以上两个库。

6. 在系统环境的变量 path中加入E:/ACE_wrappers/bin，否则在运行程序的时候碰到寻找ace.dll或者aced.dll的问题。
7. 在MFC程序中使用时，应该在InitInstance()中添加：
WSDATA wsData;
WORD VerRequested = MAKEWORD (2, 2);
WSAStarup (VerRequested, &wsData)
　　注：可以在头文件中加入如下代码就不用再手动填加lib库文件
#ifdef _DEBUG
#pragma comment (lib,"aced.lib")
#else
#pragma comment (lib,"ace.lib")
#endif
8. 在windows下release时有可能报错，错误如下：
error C2039: 'TryEnterCriticalSection' : is not a member of '`global
error C2065: 'TryEnterCriticalSection' : undeclared identifier
　　解决办法：
　　工程 Project-->setting-->C/C++ 写入:_WIN32_WINNT=0x0500

Linux 下ACE的构建
1. 设置环境变量ACE_ROOT，命令如下：
“vi /etc/profile” ，在“export PATH USER….”后加入4行
l ACE_ROOT=/opt/ACE 
l export ACE_ROOT
l LD_LIBRARY_PATH=$ACE_ROOT/aceLD_LIBRARY_PATH
l export LD_LIBRARY_PATH
　完成后将/etc/profile执行一次，命令：“chmod 555 /etc/profile”，“/etc/profile”。这样我们的ACE_ROOT就设置好了，可以用如下命令查看ACE_ROOT是否设置好了： “echo $ACE_ROOT” ，这个时候最好reboot启动一次linux。
2. 解压ACE.tar.gz包：
　不要自己手工将ACE.tar.gz包完全解开，特别强调不要完全解开，后面可以看到安装脚本会给我们解开它的；如果你手工解开全部，安装脚本将会将你的解压缩目录删除的！我们要做的事就只是将ACE.tar.gz包中的ACE-install.sh安装脚本单独解出来，并对其修改。我们只要这一个文件就可以。用mkdir建目录，如/home/my/ACE，然后将你的ACE.tar.gz放在此目录下（用cp命令拷贝或mv命令移动到此目录）。注意这个目录将是你的一个存放ACE.tar.gz的目录，安装脚本会来这个目录找这个文件的，这个目录和ACE_ROOT是不一样的，且ACE- install.sh也提示不要将这两个目录设置成同一个目录。ACE_ROOT将是我们的安装目标目录。
3. 从ACE.tar.gz解压出单个文件ACE-install.sh，命令如下：
“tar –zxvf ACE+TAO+CIAO.tar.gz ACE_wrappers/ACE-install.sh”
　解好后，会出来一个新的子目录“ACE_wrappers”。此时可以先将ACE-install.sh移到外面的目录来：“mv /ACE_wrappers/ACE-install.sh .” “.”代表当前目录。当然ACE-install.sh在什么路径下是没什么影响的，因为我们要对ACE-install.sh的内容进行修改的，里面有很多路径要修改。现在在/home/my/ACE就有了ACE-install.sh，我们现在用vi对其进行修改，修改前自己可以先备份一个。命令： “cp ACE-install.sh ACE-install.sh.bak”，“vi ACE-install.sh”先找到“MY_ACEDIR=${HOME}/aceconfig”，将其改成“MY_ACEDIR= /home/my/ACE”，就是改成我们自己建的、放有ACE.tar.gz文件的目录，之后去掉前面的注释“#”号。接着在下面一点，找到 “MY_ACE_CONFIG=config-sunos5.4-sunc++-4.x.h”，将其修改成“MY_ACE_CONFIG=config- linux.h”，即改成linux下的配置文件。继续改，找到“MY_ACE_GNU_MACROS=platform_sunos5_sunc++. GNU”，将其改成“MY_ACE_GNU_MACROS=platform_linux.GNU”。这个是各种操作系统平台下的GNU宏文件设置。文件修改完毕，保存退出“:wq”。
4. 安装ACE：
　　此时应该是在/home/my/ACE目录下，我们执行： h./ACE-install.sh 
　此后将是较长时间的解包、编译等待时间。这个过程将不会出现任何错误，因为安装脚本将为我们做很多事情，如：生成config.h，生成 platform_macros.GNU。我们不需要按ACE-install.html中说的手工改东西：不用做连接“ln -s config-sunos5-sunc++-4.x.h config.h”，安装脚本会给我们做好；(题外话：Windows中没有文件连接这一说，所以要自己生成一个config.h文件，再在它的里面加一句话“#include "ace/config-win32.h"”。) 
　　如果编译中出现错误，如“../apps/gperf”等，请编辑$ACE_ROOT/apps下的Makefile，将其中的有关“gperf”的部分注释掉，这个东西只是一个例子，可以不去编译它。
　　强调一下，用ACE-install.sh来安装，会编译ACE、tests、apps等。最后会生成libACE.so及tests、apps下也会出来很多经过编译的文件。
　　当然如果你只是下载了ACE.tar.gz这样的单个包来安装，你可以手工改文件，再make，因为只编译ACE，工作就少了很多，手工做还可以接受。

**参考资料**

1. ACE 程序员指南:网络与系统编程的实用设计模式网络与系统编程的实用设计模式 ACE Programmer's Guide practical Design Patterns for Network and Systems Programming
(美)Stephen D. Huston,(美)James CE Johnson,(美)Umar Syyid著
　　马维达译中国电力出版社
2. C++网络编程卷1：运用ACE和模式消除复杂性
C++ Network Programming Volume 1 Mastering Complexity with ACE and Patterns 
[美]Douglas C.Schmidt,Stephen D.Huston /著 [美]Douglas C.Schmidt，Stephen D.Huston 著，於春景译华中科技大学出版社
3. C++网络编程,卷2 基于ACE和框架的系统化复用
C++ network Programming, Volume 2 Systematic Reuse with ACE and Frameworks 
Douglas C.Schmidt, Stephn D.Huston /著 | 马维达译电子工业出版社
4. ACE的官方网站 http://www.cs.wustl.edu/~schmidt/ACE.html 
5. 马维达的主页 http://www.flyingdonkey.com/
6. RIVERACE公司 http://www.riverace.com
7. 灰狐 http://www.huihoo.com/ 
8. ACE类库 http://dev.csdn.net/article/40/40700.shtm
　　9.ACE开发者[http://www.acejoy.com/space/html/index.html](http://www.acejoy.com/space/html/index.html)

**常见问题**

问题1：
“Cannot open include file: ace/SOCK_Acceptor.h: No such file or directory”
　　解决方法：
Project->settings->C/C++: Preprocessor的Additional include directories中加入$ACE_ROOT，如E:/ACE_wrappers 
　　问题2：
“You must link against multi-threaded libraries when using ACE (check your project settings)”
　　解决方法：
Project->settings->C/C++: Code Generation 的Use run-time library : Debug Multithreaded Dll
　　问题3：
“error C2065: ACE_ERROR : undeclared identifier”
“error C2065: ACE_DEBUG : undeclared identifier”
　　解决方法：
　　在原马中加入#include ace/Log_Msg.h
　　问题4：“error C4716: ace_main_i : must return a value”
　　解决方法：
　　在main中加入return 0;
　　问题5：
“error LNK2001: unresolved external symbol __declspec(dllimport) int __cdecl”
　　解决方法：
Project->settings->Link->Input: Object/library Modules加入aced.lib ，Additional Library Path中加入$ACE_ROOT/ace，如E:/ACE_wrappers/ace 
　　编译client中遇到的问题:
　　问题6：
error C2039: sprintf : is not a member of ACE_OS
　　解决方法：#include ace/OS_NS_stdio.h
　　问题7：error C2039: strlen : is not a member of ACE_OS
　　解决方法：#include ace/OS_NS_string.h
　　问题8：无法找到动态链接库aced.dll于指定的路径
　　解决方法：为系统变量PATH加上D:/ACE_wrappers/bin 
　　静态包含：
工程-设置-C/C++-预处理程序定义中加入 ,ACE_AS_STATIC_LIBS

　　例１server编译后生成
 server.exe 16K serverd.exe 97K servers.exe 124K serversd.exe 669K
（servers.exe较适合发布）

　　例2 client编译后生成
 client.exe 16K clientd.exe 97K clients.exe 124K clientsd.exe 669K
（clients.exe较适合发布）

测试 clients 127.0.0.1 1919
 servers 1919

　　问题9：如果编译器支持64位，通过什么宏定义来区分64位还是32位阿？
　　解决方法：#define ver ((2>>31)>0?64:32)
　　问题10：UNICODE问题

解决方法：支持UNICODE时，int_tmain(intargc, _TCHAR* argv[])里面需要增加ACE::init()/ACE::fini()函数，否则会有网络未初始化错误。或者使用intACE_TMAIN ( intargc, ACE_TCHAR* argv[])替换入口函数。

问题11：ace框架与平台选择

先看一下你的模块运行的平台，是windows/linux/unix还是其它。平台不同，使用的ACE框架还是有所差别的。比如windows下面的服务器端，一般都用Proactor框架，配合各种异步操作，如ACE_Asynch_Acceptor/ACE_Asynch_Connector，因为Proactor内部实现是完成端口，在windows平台上，公认可以取得最好的性能。
如果你用Linux，服务器端推荐你使用Reactor框架+Dev_Poll_Reactor实现，这个实现使用了Epoll机制，性能很棒。
客户端，一般为了兼容性考虑，都用Reactor，当然，如果是在windows上面运行，默认实现是WFMO_Reactor。


