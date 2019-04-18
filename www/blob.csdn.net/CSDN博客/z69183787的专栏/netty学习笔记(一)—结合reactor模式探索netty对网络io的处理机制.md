# netty学习笔记(一)—结合reactor模式探索netty对网络io的处理机制 - z69183787的专栏 - CSDN博客
2016年09月12日 13:02:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：640
## Reactor与Proactor简介
reactor、proactor常见的翻译是反应器(堆)、前摄器，这名字听着总让人一头雾水的，抓不着本质。后来看看对应形容词的英文释义，再结合技术角度的描述，总算有了基础的认识：
reactive: reacting to events or situations rather than starting or doing new things yourself.
proactive: making things happen or change rather than reacting to events.
很明显，这对词意思相对，reactive 被动地响应事件，而proactive则主动促使事情发生或改变。
Reactor 架构：由Dispatcher中的event loop循环选择有已就绪操作的通道，触发对应的事件，分派给相应的handler去处理。其中的 io read/write 操作都是 non-blocking 的。同步的事件分派模型。
Proactor 架构：其本身只需要向操作系统发起异步操作请求，由OS代表Proactor框架执行相应操作，并在操作完成时添加相应的完成事件至Completeion Dispatcher(本质上就是一个队列)中，Proactor框架在Completeion Dispatcher的event loop中获取完成事件，分派给相应的handler处理。其中的 io read/write 操作都是 asynchronous 的。异步的事件分派模型。
网上看了不少文章到此可能就转入下一部分了，但实际上我们还是没有完全弄明白Reactor和Proactor的区别。前者采用的是 non-blocking io，而后者是 asynchronous io，我觉得这才是导致它们有不同的根本原因。non-blocking io需要用户态(即应用)线程轮询(polling)检查是否有数据到达内核(kernel)空间，如果有，就发起read操作将数据从内核空间拷贝到应用空间，进而处理数据；write操作亦如此。而asynchronous io只需要用户态线程发起read操作，然后就可以去做其他工作(即处理其他请求，而非挂起等待)，操作系统会在完成read操作即已将数据从内核拷贝到应用空间时通知应用去处理数据，在此期间应用完全不需要等待read操作返回结果。显然，服务同样多的客户端，前者需要消耗较多的用户态线程资源，而后者只需要较少的线程资源即可满足需求，这直接导致它们采用的线程策略、并发策略也不同。这两种I/O模型的不同也导致它们采用的事件分派模型不同。有关Reactor和Proactor更详细的阐述请查阅参考资源。
有关blocking, nonblocking, multiplexing, signal driven(SIGIO), asynchronous 五种I/O模型的区别以及对Synchronous、Asynchronous I/O的解释请参考《UNIX Network Programming(Volume 1)》6.2 I/O Models，讲解的非常棒，简洁明了，通俗易懂。
## 支持多个reactor的Reactor模型
![Reactor模式图解](https://img-blog.csdn.net/20140318221600281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTA5MDQ5NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
mainReactor 负责响应连接请求，调用acceptor接受请求，建立新连接，并将新连接交给一个subReactor维护，以后就由这个subReactor为此连接提供服务直至连接关闭。subReactor 负责响应读写请求，循环检查哪些通道有已准备就绪的操作，触发相应事件，并分派给具体的handler处理。
## 探索netty的启动过程
其实netty就是一个Reactor模式的实现，下面以netty自带的示例EchoServer为入口，开启netty内部实现的探索之旅。先来看看netty server的启动，乍一看就三步：
1. 为ServerBootstrap配置channel factory
2. 为ServerBootstrap配置pipeline factory **(1)**
3. 绑定端口
但实际上，在这简单的外表下隐藏了许多蛮复杂的工作，主要集中在第一、三步。
第一步中以bossExecutor、workerExecutor两个线程池为参数创建了一个NioServerSocketChannelFactory，在此对象的构造过程中，完成了NioServerBossPool、NioWorkerPool的创建和初始化，池大小分别为1、n(>1)。初始化pool时，创建了相应数量的NioServerBoss、NioWorker，并提交给bossExecutor、workerExecutor，让它们在各自的I/O线程中运行起来了。这点很重要哦，因为Boss和Worker的线程体就是一个event
 loop，它们无限轮询着以即时响应已准备就绪的I/O操作——触发相应的事件，分派给handler处理。
NioServerBoss和NioWorker都通过继承AbstractNioSelector实现了Runnable接口，都持有且维护着一个Selector对象。AbstractNioSelector类是netty nio部分的核心，其中的逻辑很复杂，包括Boss和Worker的启动运行、rebuildSelector以绕过jdk epoll(..) bug等。为了便于理解，对其做了精简：
**[java]**[view
 plain](http://blog.csdn.net/u011090495/article/details/21470679#)[copy](http://blog.csdn.net/u011090495/article/details/21470679#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243934/fork)
- abstractclass AbstractNioSelector implements NioSelector {  
- 
- // nio Selector
- protectedvolatile Selector selector;  
- 
- // 当前对象需要处理的任务队列
- privatefinal Queue taskQueue = new ConcurrentLinkedQueue();  
- 
- publicvoid register(Channel channel, ChannelFuture future) {  
- // omitted
-     }  
- 
- // 无线循环，这就是event loop
- publicvoid run() {  
- for (;;) {  
- try {  
- // 处理任务队列
-                 processTaskQueue();  
- // 处理selector，以响应已就绪的I/O操作
-                 process(selector);  
-             } catch (Throwable t) {  
- try {  
-                     Thread.sleep(1000);  
-                 } catch (InterruptedException e) {  
- // Ignore.
-                 }  
-             }  
-         }  
-     }  
- 
- privatevoid processTaskQueue() {  
- for (;;) {  
- final Runnable task = taskQueue.poll();  
- if (task == null) {  
- break;  
-             }  
-             task.run();  
-         }  
-     }  
- 
- protectedabstractvoid process(Selector selector) throws IOException;  
- }  
第三步是明修栈道，暗度陈仓。说是绑定端口，实则先从NioServerSocketChannelFactory创建一个NioServerSocketChannel，在该Channel的构造函数中打开、配置socket，触发上行的channel open事件，以让与当前channel关联的pipeline中的Binder事件处理器处理此事件【特别提醒：此pipeline并非由**(1)**处配给的pipeline
 factory创建，具体请参考ServerBootstrap.bindAsync(localAddress)。server与client间每个新建立的channel关联的pipeline都由**(1)**处配给的pipeline factory创建，每个pipeline中装着netty用户自定义的事件处理器】。ServerBootstrap.Binder中处理channel open事件的逻辑：
1. 为刚创建的NioServerSocketchannel配置pipelineFactory，此pipelineFactory就是**(1)**处设置给ServerBootstrap的那个
2. 为该channel配置其他选项
3. 继续传播channel open事件
4. 发送下行的端口绑定事件，最终由NioServerSocketPipelineSink处理该事件，其调用boss.bind(...)将端口绑定任务添加至boss的任务队列，由boss在运行期间择机执行实际的端口绑定操作。
NioServerSocketChannel有几个很重要的关联对象，了解这些对象之间的关系对从整体把握理解netty代码很重要，其精简代码如下：
**[java]**[view
 plain](http://blog.csdn.net/u011090495/article/details/21470679#)[copy](http://blog.csdn.net/u011090495/article/details/21470679#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243934/fork)
- class NioServerSocketChannel ... {  
- 
- final ServerSocketChannel socket;  
- final Boss boss;  
- final WorkerPool<NioWorker> workerPool;  
- 
- privatefinal ServerSocketChannelConfig config;  
- 
-     NioServerSocketChannel(  
-             ChannelFactory factory,  
-             ChannelPipeline bossPipeline,  
-             ChannelSink sink, Boss boss, WorkerPool<NioWorker> workerPool) {  
- super(factory,bossPipeline, sink);  
- this.boss = boss;  
- this.workerPool = workerPool;  
- 
- // Ignore ..
-           socket = ServerSocketChannel.open();  
- // Ignore ..
-           socket.configureBlocking(false);  
- // Ignore ..
- 
-         config = new DefaultServerSocketChannelConfig(socket.socket());  
-         fireChannelOpen(this);  
-     }  
- }  
至此，netty server已经启动完成了。不过前面配置pipeline factory的逻辑看起来蛮让人困惑的，待我们从整体理清netty各部分的职责，以及相互之间的协作后再来拾掇它。
## 结合Reactor模式，探索netty对网络I/O的处理机制
NioServerBoss负责监听、接受client的连接请求，创建代表新连接的NioAcceptedSocketChannel对象，并将其分派给workerPool中的一个NioWorker，由该worker为其服务；然后将新channel的注册任务添加至指派的NioWorker的任务队列中(调用worker.register(...))。回头看看第二部分的Reactor模型，会发现NioServerBoss扮演的就是mainReactor角色。注意哦，由于workerPool中NioWorker数量有限，因此就必然需要一个NioWorker服务于多个channel，即多个channel都在同一个NioWorke持有的selector处注册，这样一个io
 thread就可以服务多个channel了，这就是I/O多路复用(i/o multiplexing)。
NioWorker负责响应所服务的通道中已准备就绪的I/O读写操作，触发相应的事件以使匹配的事件处理器处理此事件。同样地，你会发现NioWorker扮演的就是subReactor角色。NilWorker继承自AbstractNioWorker，该类中包含了read/write的几乎所有逻辑，有点复杂，有空再细看。
现在能看清netty的整体脉络了，是时候回头捋捋配置pipeline factory的逻辑了。channel与pipeline是一对一的关系，每个channel都由一个pipeline对象与之关联，这个pipeline中装着处理该channel上已准备就绪的I/O操作的handlers。NioServerBoss为NioServerSocketChannel服务，该channel上可能发生的就是accept操作，Boss负责接受、创建新的连接对象NioAcceptedSocketChannel。NioAcceptedSocketChannel上可能发生的是read/write操作，而这正是netty用户感兴趣的操作，用户需要自定义handler来处理它们。NioServerSocketChannel只有一个，而NioAcceptedSocketChannel则会有许多，前者只需要一个pipeline与之关联就够了，而对后者来说则需要有一个工厂为它们中的每一个创建一个pipeline。而netty刚启动完成时，还没有client来连接，当然就不存在NioAcceptedSocketChannel了。但是这个pipeline
 factory总得先保存到某个地方，待需要时能直接取用才行吧。NioServerSocketChannel刚好又是NioAcceptedSocketChannel的parent channel，那么把pipeline factory作为其配置对象中的一部分保存起来自然是个不错的选择。从这个工厂中创建出来的每个pipeline都装着用户自定义的handlers。自然而然地，pipeline factory需要由用户在启动netty server时指定(就在**(1)**处)，此配置经由ServerBootstrap传递给由其创建的NioServerSocketChannel。而NioServerSocketChannel关联的pipeline中要装进包含什么逻辑的handler则是固定不变的，所以由netty自己创建pipeline，自己实现好handler，即ServerBootstrap.Binder。
喷了这么多口水，上张图做个总结吧。
![netty server的启动](https://img-blog.csdn.net/20140319001530578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTA5MDQ5NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
丰满了不少细节后，我们再回头看看整体的脉络。
![netty类图](https://img-blog.csdn.net/20140319001924875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTA5MDQ5NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**注意哦，本系列笔记是以netty-3.6.6-final源代码和文档作为学习材料的，netty的迭代很快，总是不断有新特性出现，特别是netty4和5，变化比较大，但是基本机制和模式没有变，变的是概念模型，以及性能优化。**
## 参考资源
- [Proactor模式(中文版)](http://docs.huihoo.com/ace_tao/ACE-2002-12/Part-One/Chapter-8.htm)、[Proactor模式(英文版)](http://www.cs.wustl.edu/~schmidt/PDF/proactor.pdf)
- [Doug Lea分享Reactor in Java：Scalable I/O in Java](http://gee.cs.oswego.edu/dl/cpjslides/nio.pdf)
- [Reactor – An Object Behavioral Pattern for Demultiplexing and Dispatching Handles for
 Synchronous Events](http://www.cs.wustl.edu/~schmidt/PDF/reactor-siemens.pdf)
- [维基百科 Reactor Pattern](http://en.wikipedia.org/wiki/Reactor_pattern)
- [[翻译]两种高性能I/O设计模式(Reactor/Proactor)的比较](http://www.cppblog.com/pansunyou/archive/2013/11/19/139343.html)
- [Proactor和Reactor模式_继续并发系统设计的扫盲](http://www.cppblog.com/kevinlynx/archive/2008/06/06/52356.HTML)
- blocking, nonblocking, multiplexing, signal driven(SIGIO), asynchronous 五种I/O模型的区别以及对Synchronous、Asynchronous I/O的解释：《UNIX Network Programming(Volume 1)》6.2 I/O Models
- [I/O–同步，异步，阻塞，非阻塞(亡羊补牢篇)](http://blog.csdn.net/historyasamirror/article/details/5778378)。注：此资源基本就是前一资源的翻译，可以看看大家在下面的讨论。
- [netty实现原理浅析](http://www.kafka0102.com/2010/06/167.html)
- [Netty与Reactor模式](http://ifeve.com/netty-reactor-4/)
- [Netty5.0架构剖析和源码解读](http://vdisk.weibo.com/s/C9LV9iVqH13rW/1391437855)
- [高性能网络编程6–reactor反应堆与定时器管理](http://blog.csdn.net/russell_tao/article/details/17452997)
- [ACE前摄器Proactor模式](http://www.cnblogs.com/TianFang/archive/2006/12/31/608952.html)
- [ACE Proactor前摄器模式的服务器端代码](http://blog.csdn.net/hangmwll/article/details/2495193)
