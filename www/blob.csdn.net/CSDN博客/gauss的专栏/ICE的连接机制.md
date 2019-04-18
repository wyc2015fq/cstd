# ICE的连接机制 - gauss的专栏 - CSDN博客
2014年07月11日 23:21:08[gauss](https://me.csdn.net/mathlmx)阅读数：558
                
1、当使用ICE的proxy进行方法调用时，ICE运行环境会建立一个到服务器的连接。当proxy提供了多个endpoint时
   默认的ICE运行环境选择endpoint的行为为random，可以通过ice_endpointSelection设置选择endpoint的方式为order
   也可以通过设置属性：Ice.Default.EndpointSelection、name.EndpointSelection来改变endpoint的选择策略
2、ICE的连接在失败时会重试，重试的规则取决于属性Ice.RetryIntervals，默认取值为0，即立即重试一次
   可以配置成如下形式：Ice.RetryIntervals=0 10 50 100
   可以配置Ice.Trace.Retry来跟踪连接的重试
3、ICE支持给每个连接一个唯一的ID标示，来让应用控制连接的使用行为，通过调用proxy的ice_connectionId方法
   来给其建立的连接命名。当然使用相同连接ID的代理意味着共享了相同的连接
4、ICE会对proxy上的建立的连接进行cache，以确保下次方法调用时尽可能的使用已经建立的连接，有时如果应用针对
   某个proxy有多个endpoint，并且希望在这些endpoint之间保持负载均衡的方法调用，那么可以通过设置属性
   ice_connectionCached(false)来关闭连接cache，当然这也会为频繁的连接建立带来足够的开销
5、ICE的每个连接都有两个超时时间：ice_timeout、ice_connectiontimeout，分别对应消息的超时时间和连接建立
   的超时时间，可以通过在代理上调用上述方法来设置超时，也可以通过属性Ice.Override.Timeout、Ice.Override.ConnectTimeout
   来强制改变超时时间
6、ICE运行环境针对每个communicator可以设置连接ACM，分别为Ice.ACM.Client和Ice.ACM.Server，默认取值为60s
   意味着如果连接存在60s的idle时间，那么连接将会自动关闭。可以设置取值为0来禁用IDLE检测机制。那么连接建立后
   则不会关闭，直到communicator关闭为止。ICE检测空闲连接的时间间隔为Ice.MonitorConnections，取值在5s到5m之间。
   如果你使用了glacier2来进行防火墙的策略调度，那么你最好关闭ACM，否则可能会导致过早的关闭客户端连接而导致
   回调无法成功
7、程序中获取连接:客户端可以使用proxy->ice_getConnection()，服务器端可以使用current.conn
8、连接对象提供的接口方法如下：
   local interface Connection 
   {
       void close(bool force);                    // 关闭连接，基本上很少显示调用
       Object* createProxy(Identity id);          // 在使用双向的防火墙策略时，常在客户端使用来创建回调对象
       void setAdapter(ObjectAdapter adapter);    // 设置代理的对象适配器，也常用于双向连接
       ObjectAdapter getAdapter();                // 获取adapter对象
       Endpoint getEndpoint();                    // 获取endpoint端点信息
       void flushBatchRequests();                 // 刷新批量请求
       string type();                             // 返回协议类型，类似如：tcp、udp、ssl
       int timeout();                             // 返回超时时间
       string toString();                         // 串化
       ConnectionInfo getInfo();                  // 获取连接的相关连接信息，类似如tcp端口、udp信息等
    };
注意getEndpoint和getInfo返回的是基类信息，需要根据type方法的返回值来进行向下转型获取详情
9、客户端双向连接的使用：
Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("");  // 创建默认adapter，无名称，无端点
    Ice::Identity ident;
    ident.name = IceUtil::generateUUID();
    ident.category = "";
    CallbackPtr cb = new CallbackI;                                         // 创建回调对象
    adapter->add(cb, ident);                                                // 添加回调对象为servant
    adapter->activate();                                                    // 激活adapter
    proxy->ice_getConnection()->setAdapter(adapter);                        // 设置代理对象的连接adapter为当前adapter
    proxy->addClient(ident);                                                // 传递callback的ID给服务器
10、服务器端双向连接的使用：
    CallbackPrx client = CallbackPrx::uncheckedCast(curr.con->createProxy(ident)); // 在连接上使用callback的id来创建代理
    client->notify();                                                              // 回调代理
注意，该处创建的callbackprx代理不能修改其相关属性：类似timeout、twoway、datagram等，同时需要关闭ACM机制来防止
不合事宜的连接关闭
11、客户端的双向调用接收callback的回调是在客户端线程池中，但是此时可能客户端线程池正在阻塞在客户端请求上，那么
    你最好配置客户端线程池的线程个数大于1，否则这样的嵌套调用会导致线程的死锁等待
