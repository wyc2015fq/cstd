# ICE运行时环境备忘三 - gauss的专栏 - CSDN博客
2014年04月25日 22:31:24[gauss](https://me.csdn.net/mathlmx)阅读数：365
1、ICE是一个多线程的运行环境，每个communicator通信器中都有一个Leader-Follower的线程池负责处理客户端的服务请求，
   ICE的线程池有一系列的属性用以来控制线程池的运行，主要包含如下内容：
   ThreadPoolName.Size          = 3              # 线程池初始线程个数3个，注意默认大小1个
   ThreadPoolName.SizeMax       = 10             # 线程池最大线程个数10个，注意默认大小1个，大于等于初始大小
   ThreadPoolName.sizeWarn      = 8              # 线程池告警线程个数8个，默认取值为0不告警
   ThreadPoolName.StackSize     = 4MB            # 线程池中线程的堆栈大小4MB，默认使用系统配置大小
   ThreadPoolName.Serialize     = 1              # 线程池的线程消息序列化标记，默认取值0不序列化
   ThreadPoolName.ThreadIdleTime= 30             # 线程池中空闲线程回收时间，单位秒，默认取值60秒
   该处的ThreadPoolName可以替换成Ice.ThreadPool.Client、Ice.ThreadPool.Server、AdapterName.ThreadPool，分别用以
   控制客户端线程池、服务器端线程池、ObjectAdapter线程池的属性
   注意：如果没有显示的配置objectadapter的线程池属性，那么它继承使用归属的communicator的线程池属性
2、在强事物要求的处理系统中，可能需要客户端请求处理的时序性有严格保证，那么要达到该种效果，可以从两个方面解决：
   A、使用单线程的服务器运行环境，即Ice.ThreadPool.Server=1
   B、采用序列化的线程池属性，即Ice.ThreadPool.Server.Serialize = 1
   采用B方案看上去很优雅，但是他比单线程的线程池需要额外的系统负担来确保消息的序列化分发和处理。
   如果客户端请求能够保证不会再多线程中发送消息、不会使用多个连接发送消息、仅仅采用双向的同步调用，那么消息的发送
   和处理肯定是时序的，就不用上述方式来进行属性限制了
3、在ICE运行环境，提供了一个线程创建和通知的回调接口给应用，当ICE运行环境动态的创建和回收线程时，会回调对应的接口：
   class MyThreadHook : public virtual Ice::ThreadNotification
   {
   public:
       void start() {cout<<"Thread creat with id "<<ThreadControl().id()<<endl;}
  void stop() {cout<<"Thread destory with id "<<ThreadControl().id()<<endl;}
   };
   typedef IceUtil::Handle<MyThreadHook> MyTHreadHookPtr;
   int main()
   {
       Ice::InitializationData id;
  id.threadHook = new MyThreadHook;
  communicator = Ice::initialize(argc, argv, id);
   }
4、在ICE的运行环境中，所有的线程池默认采用LEADER-FOLLOWER的线程模型，按照默认的规则进行消息分发和处理，有的时候需要
   应用自行来控制特殊消息的分发，或者事物处理。ICE运行环境支持内嵌一个消息分发器，具体操作方式如下：
   class MyDispatcher : public virtaul Ice::Dispatcher
   {
   public:
       void dispatch(const DispatcherCallPtr& call, const Ice::ConnectionPtr& connection)
  {
       // 完成方法分派，但是不要试图在该方法中进行阻塞式的方法调用
// 可以再该处将消息分发给自己创建的指定线程
// 并在指定的线程中中调用call->run();
  }
   };
   typedef IceUtil::Handle<MyDispatcher> MyDispatcherPtr;
   int main()
   {
       Ice::InitializationData id;
  id.dispatcher = new MyDispatcher;
  communicator = Ice::initialize(argc, argv, id);
   }
   dispather 在如下有被调用:
   Ice::ConnectionI::message(ThreadPoolCurrent& current)
   Ice::ConnectionI::finished(ThreadPoolCurrent& current)
   IceInternal::DispatchWorkItem::execute(ThreadPoolCurrent& current)
