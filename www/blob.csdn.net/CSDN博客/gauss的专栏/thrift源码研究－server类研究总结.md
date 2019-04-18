# thrift源码研究－server类研究总结 - gauss的专栏 - CSDN博客
2015年05月03日 23:35:19[gauss](https://me.csdn.net/mathlmx)阅读数：274
　server类处于thrift架构的最高层，用于服务器。
　下面将对thrift的各种server类进行介绍。
1、所有server类都是继承TServer；
2、所有server类处理请求的流程基本相同:
（1）首先获取连接
client = serverTransport_->accept();
（2）然后获取thransport和protocol
inputTransport = inputTransportFactory_->getTransport(client);
outputTransport = outputTransportFactory_->getTransport(client);
inputProtocol = inputProtocolFactory_->getProtocol(inputTransport);
outputProtocol = outputProtocolFactory_->getProtocol(outputTransport);
（3）接着获取processor
shared_ptr<TProcessor> processor = getProcessor(inputProtocol,
                                                    outputProtocol, client);
（4）最后处理请求
processor->process(inputProtocol, outputProtocol,
                                connectionContext);
3、TSimpleServer是单线程处理请求；
4、TThreadedServer是多线程处理请求，来一个连接请求就开启一个线程处理它，处理完就退出线程。处理线程有2种类型：PthreadThread和BoostThread，前者是基posix函数，后者基于boost线程类，它们均通过对应的工厂类创建；还有个Task类，继承于Runnable，主要的处理请求流程在此实现。
5、TNonblockingServer
（1）采用libevent库的事件驱动框架
注册两件事件：（其中一个事件是用于threadPoolProcessing mode）
  // Register the server event
  event_set(&serverEvent_,
            serverSocket_,
            EV_READ | EV_PERSIST,
            TNonblockingServer::eventHandler,
            this);
  event_base_set(eventBase_, &serverEvent_);
  event_add(&serverEvent_, 0);
  // Create an event to be notified when a task finishes
  event_set(¬ificationEvent_,
            getNotificationRecvFD(),
            EV_READ | EV_PERSIST,
            TConnection::taskHandler,
            this);
  event_base_set(eventBase_, ¬ificationEvent_);
  event_add(¬ificationEvent_, 0);
启动事件循环：
  // Run libevent engine, invokes calls to eventHandler
  // Only returns if stop() is called.
  event_base_loop(eventBase_, 0);
停止事件循环：
  event_base_loopbreak(eventBase_);
  // event_base_loopbreak() only causes the loop to exit the next time it wakes
  // up.  We need to force it to wake up, in case there are no real events
  // it needs to process.
  //
  // Attempt to connect to the server socket.  If anything fails,
  // we'll just have to wait until libevent wakes up on its own.
  //
  // First create a socket
  int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fd < 0) {
    return;
  }
  // Set up the address
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
  addr.sin_port = htons(port_);
  // Finally do the connect().
  // We don't care about the return value;
  // we're just going to close the socket either way.
  connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
  close(fd);
（2）当为threadPoolProcessing mode时，主线程+工作线程架构，主线程负责接收新连接和监听网络数据，当有数据到达时，生成一个task放入队列，工作线程负责取出task，并执行task。这里还用到unix socket来通知主线程任务已完成和使用TMemoryBuffer来缓存读写数据。
（3）connectionStack_是保存空闲TConnection对象的栈，有新连接就从栈里取出一个TConnection对象并初始化它，栈为空时才new一个TConnection对象。连接被close掉时会将TConnection对象交还给connectionStack_，即入栈，但connectionStack_的大小达到connectionStackLimit_时delete掉该TConnection对象。
（4）当numActiveProcessors_ > maxActiveProcessors_ 或 activeConnections > maxConnections_时,server达到overloaded状态，其中numActiveProcessors_是指目前尚未处理的task数；
（5）server达到overloaded状态时，可能采取以下三种措施：
/// Overload condition actions.
enum TOverloadAction {
  T_OVERLOAD_NO_ACTION,        ///< Don't handle overload */
  T_OVERLOAD_CLOSE_ON_ACCEPT,  ///< Drop new connections immediately */
  T_OVERLOAD_DRAIN_TASK_QUEUE  ///< Drop one tasks from head of task queue */
};
有新连接被accept时，会先检查是否达到overloaded，但是会进行如下处理Overload的代码：
// If we're overloaded, take action here
if (overloadAction_ != T_OVERLOAD_NO_ACTION && serverOverloaded()) {
  nConnectionsDropped_++;
  nTotalConnectionsDropped_++;
  if (overloadAction_ == T_OVERLOAD_CLOSE_ON_ACCEPT) {
    close(clientSocket);
    return;
  } else if (overloadAction_ == T_OVERLOAD_DRAIN_TASK_QUEUE) {
    if (!drainPendingTask()) {
    // Nothing left to discard, so we drop connection instead.
    close(clientSocket);
    return;
    }
  }
}
drainPendingTask()函数：
bool TNonblockingServer::drainPendingTask() {
  if (threadManager_) {
    boost::shared_ptr<Runnable> task = threadManager_->removeNextPending();
    if (task) {
      TConnection* connection =
        static_cast<TConnection::Task*>(task.get())->getTConnection();
      assert(connection && connection->getServer()
             && connection->getState() == APP_WAIT_TASK);
      connection->forceClose();
      return true;
    }
  }
  return false;
} //可见，只drop one task
6、TThreadPoolServer跟TNonblockingServer一样，都采用了线程池方案，只是它采用同步I/O方式，而TNonblockingServer采用异步I/O方式。
以上四个server中，TNonblockingServer实现最复杂，但性能最好，一般应用会采用TNonblockingServer。
            
