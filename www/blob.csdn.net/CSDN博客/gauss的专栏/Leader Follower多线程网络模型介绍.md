# Leader/Follower多线程网络模型介绍 - gauss的专栏 - CSDN博客
2014年04月27日 10:41:40[gauss](https://me.csdn.net/mathlmx)阅读数：473
个人分类：[多线程](https://blog.csdn.net/mathlmx/article/category/954554)
[线程](http://www.csdn.net/tag/%E5%A4%9A%E7%BA%BF%E7%A8%8B)[网络](http://www.csdn.net/tag/%E7%BD%91%E7%BB%9C)[代码分析](http://www.csdn.net/tag/%E4%BB%A3%E7%A0%81%E5%88%86%E6%9E%90)[server](http://www.csdn.net/tag/server)[null](http://www.csdn.net/tag/null)[服务器](http://www.csdn.net/tag/%E6%9C%8D%E5%8A%A1%E5%99%A8)
     之前分享过《轻量级web server Tornado代码分析》，介绍了目前我们采用nginx + tornado的方式搭建升级、配管、数据中心等各类服务组建客户端迭代体系。最近注意到，淘宝目前公开了其网络服务器源代码Tengine。根据官方介绍，Tengine是由淘宝网发起的Web服务器项目。它在Nginx的基础上，针对大访问量网站的需求，添加了很多高级功能和特性。Tengine的性能和稳定性已经在大型的网站如淘宝网，天猫商城等得到了很好的检验。它的最终目标是打造一个高效、稳定、安全、易用的Web平台。它们都采用了多线程非阻塞模式，并使用了LF模型。我最近整理了一下LF的相关资料，和大家分享一下。对淘宝开源的Tengine有兴趣的同学可以到这里checkout代码研究：[http://code.taobao.org/svn/tengine/trunk](http://code.taobao.org/svn/tengine/trunk)
**1、 引言**
大家知道，多线程网络服务最简单的方式就是一个连接一个线程，这种模型当客户端连接数快速增长是就会出现性能瓶颈。当然，这时候，我们理所当然会考虑使用线程池，而任何池的使用，都会带来一个管理和切换的问题。 在java 1.4中引入了NIO编程模型，它采用了Reactor模式，或者说观察者模式，由于它的读写操作都是无阻塞的，使得我们能够只用一个线程处理所有的IO事件，这种处理方式是同步的。为了提高性能，当一个线程收到事件后，会考虑启动一个新的线程去处理，而自己继续等待下一个请求。这里可能会有性能问题，就是把工作交给别一个线程的时候的上下文切换，包括数据拷贝。今天向大家介绍一种Leader-Follower模型。
![](https://img-my.csdn.net/uploads/201206/30/1341045549_7751.jpg)
**2、 基本思想**
所有线程会有三种身份中的一种：leader和follower，以及一个干活中的状态：proccesser。它的基本原则就是，永远最多只有一个leader。而所有follower都在等待成为leader。线程池启动时会自动产生一个Leader负责等待网络IO事件，当有一个事件产生时，Leader线程首先通知一个Follower线程将其提拔为新的Leader，然后自己就去干活了，去处理这个网络事件，处理完毕后加入Follower线程等待队列，等待下次成为Leader。这种方法可以增强CPU高速缓存相似性，及消除动态内存分配和线程间的数据交换。
![](https://img-my.csdn.net/uploads/201206/30/1341045554_8293.jpg)
**3、 原理分析**
显然地，通过预先分配一个线程池，Leader/Follower设计避免了动态线程创建和销毁的额外开销。将线程放在一个自组织的池中，而且无需交换数据，这种方式将上下文切换、同步、数据移动和动态内存管理的开销都降到了最低。
不过，这种模式在处理短暂的、原子的、反复的和基于事件的动作上可以取得明显的性能提升，比如接收和分发网络事件或者向数据库存储大量数据记录。事件处理程序所提供的服务越多，其体积也就越大，而处理一个请求所需的时间越长，池中的线程占用的资源也就越多，同时也需要更多的线程。相应的，应用程序中其它功能可用的资源也就越少，从而影响到应用程序的总体性能、吞吐量、可扩展性和可用性。
在大多数LEADER/FOLLOWERS设计中共享的事件源封装在一个分配器组件中。如果在一个设计中联合使用了LEADER/FOLLOWERS和REACTOR事件处理基础设施，由reactor组件进行分发。封装事件源将事件分离和分派机制与事件处理程序隔离开来。每个线程有两个方法：一个是join方法，使用这个方法可以把新初始化的线程加入到池中。新加入的线程将自己的执行挂起到线程池监听者条件(monitor condition)上，并开始等待被提升为新的Leader。在它变成一个Leader之后，它便可以访问共享的事件源，等待执行下一个到来的事件。另一个是promote_new_leader方法，当前的Leader线程使用这个方法可以提升新的Leader，其做法是通过线程池监听者条件通知休眠的Follower。收到通知的Follower继续执行(resume)线程池的join方法，访问共享事件源，并等待下一个事件的到来。
4、 代码演示
首先用一段简单的代码演示一下整个角色转换的过程。由于同一时刻只有一个leader，用一个互斥量就可以解决了。每个线程一直在做如下4个步骤循环：
publicclassWorkThread
    {
publicstaticMutex mutex =newMutex();
publicvoid start()
        {
while (true)
            {
// 等待成为leader
                waitToLeader();
// 用select或epoll等方式等待消息处理
                simulateReactor();
// 产生下一个leader
                promoteNewLeader();
// 处理消息
                simulateDojob();
            }
        }
privatevoid simulateDojob()
        {
…
        }
privatevoid promoteNewLeader()
        {
Console.WriteLine(Thread.CurrentThread.Name +": Release leadership to others..");
            mutex.ReleaseMutex();
        }
privatevoid simulateReactor()
        {
…
        }
privatevoid waitToLeader()
        {
Console.WriteLine(Thread.CurrentThread.Name +": Waiting to be Leader..");
            mutex.WaitOne();
        }
    }
详细的代码可以参见附件。
5、 代码分析
接下来我们来看一下一个典型的开源代码实现：spserver。抄段官网的话，spserver是一个实现了半同步/半异步(Half-Sync/Half-Async)和领导者/追随者(Leader/Follower) 模式的服务器框架，能够简化
 TCP server 的开发工作。spserver 使用 c++ 实现，目前实现了以下功能：
Ø  封装了 TCP server 中接受连接的功能
Ø  使用非阻塞型I/O和事件驱动模型，由主线程负责处理所有 TCP 连接上的数据读取和发送，因此连接数不受线程数的限制
Ø  主线程读取到的数据放入队列，由一个线程池处理实际的业务
Ø  一个 http 服务器框架，即嵌入式 web 服务器
Spserver的每个版本都有一定的修改。早先版本V0.5还没有引入Leader/Follower模式，在V0.8版本中已经有了sp_lfserver。在V0.9版本中将其改为了sp_iocplfserver，引入了iocp完成端口的名字，但事实上之前版本已经使用了完成端口的技术。简单地说，iocp就是事件io操作由操作系统完成，完成后才由线程接收处理事件。先看一下代码,server启动以后开始监听，并将线程池启动起来。线程入口函数lfHandler一直在循环执行handleOneEvent：
int SP_LFServer :: run()
{
int ret = 0;
int listenFD = -1;
     ret = SP_IOUtils::tcpListen( mBindIP, mPort, &listenFD, 0 );
if( 0 == ret ) {
         mThreadPool = 
new SP_ThreadPool( mMaxThreads );
for(int i = 0; i < mMaxThreads; i++ ) {
              mThreadPool->dispatch( lfHandler,this );
         }
     }
return ret;
}
void SP_LFServer :: lfHandler(void * arg )
{
     SP_LFServer * server = (SP_LFServer*)arg;
for( ; 0 == server->mIsShutdown; ) {
         server->handleOneEvent();
     }
}
接下来看一下handleOneEvent的处理，和上面的演示程序一样，先mutexlock争取leader权，然后去等待读、写事件，最后释放leadership给其它人，自己执行读完成事件处理函数task->run()或写事件的完成端口事件completionMessage，这个completionMessage会做一些清理工作，例如delete msg：
void SP_LFServer :: handleOneEvent()
{
     SP_Task * task = NULL;
     SP_Message * msg = NULL;
     pthread_mutex_lock( &mMutex );
for( ; 0 == mIsShutdown && NULL == task && NULL == msg; ) {
if( mEventArg->getInputResultQueue()->getLength() > 0 ) {
              task = (SP_Task*)mEventArg->getInputResultQueue()->pop();
         } 
elseif( mEventArg->getOutputResultQueue()->getLength() > 0 ) {
              msg = (SP_Message*)mEventArg->getOutputResultQueue()->pop();
         }
if( NULL == task && NULL == msg ) {
              event_base_loop( mEventArg->getEventBase(), EVLOOP_ONCE );
         }
     }
     pthread_mutex_unlock( &mMutex );
if( NULL != task ) task->run();
if( NULL != msg ) mCompletionHandler->completionMessage( msg );
}
6、 框架使用
和之前介绍的框架一样，采用spserver构建server非常快捷,如下，只要把SP_TestHandler里的几个处理事件实现即可。
class SP_TestHandler :public SP_Handler {
public:
     SP_ TestHandler (){}
virtual ~SP_ TestHandler (){}
virtualint start( SP_Request * request, SP_Response * response ) {}
virtualint handle( SP_Request * request, SP_Response * response ) {}
virtualvoid error( SP_Response * response ) {}
virtualvoid timeout( SP_Response * response ) {}
virtualvoid close() {}
};
class SP_TestHandlerFactory :public SP_HandlerFactory {
public:
     SP_ TestHandlerFactory () {}
virtual ~SP_ TestHandlerFactory () {}
virtual SP_Handler * create()const {
returnnew SP_TestHandler();
     }
};
int main(int argc,
char * argv[] )
{
int port = 3333, maxThreads = 4, maxConnections = 20000;
int timeout = 120, reqQueueSize = 10000;
constchar * serverType =
"lf";
     SP_IocpLFServer server( 
"", port, new SP_TestHandlerFactory() );
     server.setTimeout( timeout );
     server.setMaxThreads( maxThreads );
     server.setMaxConnections( maxConnections );
     server.runForever();
return 0;
}
    Spserver的代码可以在这里看到：[http://spserver.googlecode.com/svn/trunk/spserver/。spserver同时实现了一个与leader/follower](http://spserver.googlecode.com/svn/trunk/spserver/%E3%80%82spserver%E5%90%8C%E6%97%B6%E5%AE%9E%E7%8E%B0%E4%BA%86%E4%B8%80%E4%B8%AA%E4%B8%8Eleader/follower)齐名的网络编程模型：HAHS，翻译为半异步半同步模型。本文暂不作介绍。
