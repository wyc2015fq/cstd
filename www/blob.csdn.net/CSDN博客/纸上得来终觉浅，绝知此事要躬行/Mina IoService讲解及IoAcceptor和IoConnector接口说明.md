# Mina IoService讲解及IoAcceptor和IoConnector接口说明 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月02日 17:07:09[boonya](https://me.csdn.net/boonya)阅读数：3643
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina IoService介绍（Apache Mina user guide Chapter 3 service）](http://mina.apache.org/mina-project/userguide/ch3-service/ch3-service.html)


### 1.1.1. **IoService**

IoService所处的结构示意图可以从[应用架构](http://mina.apache.org/mina-project/userguide/ch2-basics/application-architecture.html)中查看。基本类支持所有IO 服务操作，无论是服务端还是客户端。它将处理所有与应用程序的交互、与远程对等、发送和接收消息，管理session会话、连接等。

它是一个提供服务端实现IoAcceptor和客户端实现IOConnector的接口。在以下的章节，我们将分析这些接口。

#### 1.1.1.1. **IoService介绍**

在Mina中[IoService](http://mina.apache.org/mina-project/xref/org/apache/mina/core/service/IoService.html)提供了基本的IO服务以及管理IO session会话,它是Mina架构中的重要组成部分之一。IoService接口及其子接口的实现类,大多数是低水平的I
 / O处理操作。

#### 1.1.1.2. **IoService思维导图**

让我们来看看IoService负责哪些功能，及[AbstractIoService](http://mina.apache.org/mina-project/xref/org/apache/mina/core/service/AbstractIoService.html)的实现类。让我们采取一种稍微不同的方式首先使用[思维导图](http://en.wikipedia.org/wiki/Mind_map),然后跳进其内部工作（原理）。思维导图是通过[XMind](http://www.xmind.net/)创建的。

![](https://img-blog.csdn.net/20160602170257372)


#### 1.1.1.3. **负责功能**

如前面的图像所示，IoService具有多种负责功能。
- · sessions 管理: 创建和删除sessions, 侦查空闲sessions.
- · filter chain 管理 : 处理过滤器链, 允许用户改变过滤器链
- · handler 调用: 当接收到新的消息时调用处理 
- · 统计管理: 更新消息的发送数量如：, bytes 和其他类型
- · 监听器管理: 以超级用户身份管理监听器
- · 通信管理: 处理两端（客户端和服务端）的数据传输

所有这些方面都会在接下来的章节中讲到。

#### 1.1.1.4. **接口详情**

IoService是一个为IoConnector和IoAcceptor提供IO服务和session会话管理的基本接口。接口的相关的所有功能需要执行I / O操作，让我们深入一点来了解接口的一系列方法：
- · getTransportMetadata()
- · addListener()
- · removeListener()
- · isDisposing()
- · isDisposed()
- · dispose()
- · getHandler()
- · setHandler()
- · getManagedSessions()
- · getManagedSessionCount()
- · getSessionConfig()
- · getFilterChainBuilder()
- · setFilterChainBuilder()
- · getFilterChain()
- · isActive()
- · getActivationTime()
- · broadcast()
- · setSessionDataStructureFactory()
- · getScheduledWriteBytes()
- · getScheduledWriteMessages()
- · getStatistics()

##### 1.1.1.4.1. **getTransportMetadata()**

该方法返回IoAcceptor或IoConnector正在运行传输的元数据。典型的详情包括供应商名称(nio,apr,rxtx),连接类型(无连接/面向连接的)等。

##### 1.1.1.4.2. **addListener()**

允许添加IoServiceListener监听IoService相关的特定事件。

##### 1.1.1.4.3. **removeListener()**

允许删除IoService指定IoServiceListener 附加。

##### 1.1.1.4.4. **isDisposing()**

这个方法用于辨别如果服务当前正在处理，可能需要一段时间，它是了解服务的当前状态很有用的方法。

##### 1.1.1.4.5. **isDisposed()**

这个方法用于辨别如果服务已经被处理，服务将被视为处理，只有当所有已分配的资源都被释放了。

##### 1.1.1.4.6. **dispose()**

这个方法会释放所有资源分配的服务。因为它可能需要一段时间,用户应该检查服务状态使用isDisposing()和isDisposed()知道服务是现在处理完全。

##### 1.1.1.4.7. **getHandler()**

返回服务关联的*IoHandler。*

##### 1.1.1.4.8. **setHandler()**

设置的IoHandler将负责处理所有的事件服务。处理程序包含应用程序逻辑!

##### 1.1.1.4.9. **getManagedSessions()**

返回目前由该服务管理的所有session会话的Map对象。一个被管理的session会话添加到服务监听。它将用于处理闲置会话,会话和其他方面,根据听众的一个用户添加到服务。

##### 1.1.1.4.10. **getManagedSessionCount()**

返回所有目前由该服务管理session会话的数量。

##### 1.1.1.4.11. **getSessionConfig()**

返回session配置。

##### 1.1.1.4.12. **getFilterChainBuilder()**

返回过滤器链构建器。如果想添加一些新的过滤器注入时,将创建session会话，这是有用的。

##### 1.1.1.4.13. **setFilterChainBuilder()**

通过服务定义过滤器链构造器。

##### 1.1.1.4.14. **getFilterChain()**

返回服务默认的过滤器链。

##### 1.1.1.4.15. **isActive()**

辨别服务是否活跃。

##### 1.1.1.4.16. **getActivationTime()**

返回时间当这个服务被激活时。如果服务不再活跃，它返回最后一次当这个服务被激活的时间。

##### 1.1.1.4.17. **broadcast()**

写消息给所有的session会话管理。

##### 1.1.1.4.18. **setSessionDataStructureFactory()**

设置IoSessionDataStructureFactory提供相关数据结构创建一个新会话为这项服务。

##### 1.1.1.4.19. **getScheduledWriteBytes()**

返回将写入的字节数(即字节存储在内存等待插座准备写)。

##### 1.1.1.4.20. **getScheduledWriteMessages()**

返回的消息数量将书面(即消息存储在内存中等待插座准备写)。

##### 1.1.1.4.21. **getStatistics()**

为这个服务返回IoServiceStatistics对象。

### 1.1.2. **IoService详情**

IoService是Mina中两个最重要的实现类的接口：
- · IoAcceptor
- · IoConnector

为了建立一个服务器,你需要选择IoAcceptor接口的一个实现。对于客户端应用程序,您需要实现IoConnector接口的一个实现。

#### 1.1.2.1. **IoAcceptor**

基本上,这个接口命名是因为accept()方法,负责创建新的客户端和服务器之间的连接。服务器接受传入的连接请求。在某种程度上,我们可以叫这个接口的服务器(这是在未来Mina3.0的新名称)。我们可能处理超过一种运输(TCP
 / UDP /…),我们有不止一个实现这个接口。很可能你需要实现一个新的。

我们有很多的实现类：
- · **NioSocketAcceptor** : the non-blocking Socket transport*IoAcceptor*
- · **NioDatagramAcceptor** : the non-blocking UDP transport*IoAcceptor*
- · **AprSocketAcceptor** : the blocking Socket transport*IoAcceptor*, based on APR
- · **VmPipeSocketAcceptor** : the in-VM*IoAcceptor*

只选择需要一个适合您的。这是IoAcceptor接口和类的类图:

![](https://img-blog.csdn.net/20160602170431420)

#### 1.1.2.2. **IoConnector**

作为服务器,我们必须使用一个IoAcceptor你必须为客户端实现IoConnector。同样,我们有很多实现类:
- · **NioSocketConnector** : the non-blocking Socket transport*IoConnector*
- · **NioDatagramConnector** : the non-blocking UDP transport*IoConnector*
- · **AprSocketConnector** : the blocking Socket transport*IoConnector*, based on APR
- · **ProxyConnector** : a *IoConnector* providing proxy support
- · **SerialConnector** : a *IoConnector* for a serial transport
- · **VmPipeConnector** : the in-VM *IoConnector*



只选择需要一个适合您的。这是IoConnector接口和类的类图:

![](https://img-blog.csdn.net/20160602170403607)

### 1.1.3. **Acceptor**

为了建立一个服务器,你需要选择IoAcceptor接口的一个实现。

#### 1.1.3.1. **IoAcceptor**

如上：IoService详情>IoAcceptor所述。

#### 1.1.3.2. **创建**

你首先要选择你想实例化的IoAcceptor类型。这是一个早于你做出处理之前的选择,它都可以归结为您将使用的网络协议。让我们看看它是如何工作的一个例子:

```java
<span style="font-size:12px;">public TcpServer() throws IOException {

    // Create a TCP acceptor
    IoAcceptor acceptor = new NioSocketAcceptor();

    // Associate the acceptor to an IoHandler instance (your application)
    acceptor.setHandler(this);

    // Bind : this will start the server...
    acceptor.bind(new InetSocketAddress(PORT));

    System.out.println("Server started...");

}</span>
```

就是这样!您已经创建了一个TCP服务器。如果你想开始一个UDP服务器,只需替换的第一行代码:

```java
<span style="font-size:12px;">...// Create an UDP acceptor

IoAcceptor acceptor = new NioDatagramAcceptor();...</span>
```

#### 1.1.3.3. **废弃处理**

服务可以通过调用dispose()方法停止。服务将不再只有当所有的等待会话处理:


```java
<span style="font-size:12px;">// Stop the service, waiting for the pending sessions to be inactive
acceptor.dispose();</span>
```

你也可以等待每个线程正在执行正确完成一个布尔参数传递给这个方法:

```java
<span style="font-size:12px;">// Stop the service, waiting for the processing session to be properly completed
acceptor.dispose( true );</span>
```

#### 1.1.3.4. **服务状态**

你可以通过调用以下方法之一得到IoService状态:
- · *isActive()* : true if the service can accept incoming requests
- · *isDisposing()* : true if the *dispose()* method has been called. It does not tell if the service is actually stopped (some sessions might be processed)
- · *isDisposed()* : true if the *dispose(boolean)* method has been called, and the executing threads have been completed.

#### 1.1.3.5. **管理IoHandler**

您可以添加或获得相关IoHandler实例化服务。你必须调用setHandler(IoHandler)或getHandler()方法。

#### 1.1.3.6. **管理过滤器链**

如果你想管理过滤器链,你需要调用getFilterChain()方法。这里有一个例子:


```java
<span style="font-size:12px;">// Add a logger filter

DefaultIoFilterChainBuilder chain = acceptor.getFilterChain();

chain.addLast("logger", new LoggingFilter());

您还可以创建过滤器链在设置到到服务之前：

// Add a logger filter

DefaultIoFilterChainBuilder chain = new DefaultIoFilterChainBuilder();

chain.addLast("logger", new LoggingFilter());

// And inject the created chain builder in the service

acceptor.setFilterChainBuilder(chain);</span>
```

### 1.1.4. Connector

对于客户端应用程序,您需要编写IoConnector接口的一个实现。

#### 1.1.4.1. **IoConnector**

如上：IoService详情>IoConnector所述。






