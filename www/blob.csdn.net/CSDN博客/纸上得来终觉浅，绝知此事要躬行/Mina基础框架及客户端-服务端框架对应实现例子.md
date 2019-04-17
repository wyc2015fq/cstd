# Mina基础框架及客户端-服务端框架对应实现例子 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月01日 18:18:12[boonya](https://me.csdn.net/boonya)阅读数：4915
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina基础（Apache user guide Chapter2 basics）](http://mina.apache.org/mina-project/userguide/ch2-basics/ch2-basics.html)


### 1.1.1. **Mina应用架构**

      问的最多的问题就是，“Mina的基础应用看起来是什么样的呢？”。通过这篇文章让我们来看Mina基础应用是什么样子。已经尝试从演示收集Mina基础的信息。

鸟瞰图如下：

![](https://img-blog.csdn.net/20160601181418125)


        从这里，我们可以看到Mina是介于应用层与底层网络层之间的，底层网络层基于TCP、UDP、in-VM通信，甚至作为RS-232C串口通信的客户端。你只需在Mina上层设计你的应用，而无需处理所有复杂的网络层。

       让我们深入去了解详情吧。下面的图片是展示Mina内部各个组件允许的示意图：

![](https://img-blog.csdn.net/20160601181449363)


上面的图片来自于[MINA in real life (ApacheCon EU 2009)](http://mina.apache.org/staticresources/pdfs/Mina_in_real_life_ASEU-2009.pdf)演示。

**显而易见，Mina的基础应用分为三层：**
- · I/O Service -执行真实 I/O
- · I/O Filter Chain -过滤或转化bytes为期望数据结构，正反互逆转换
- · I/O Handler -真实的逻辑处理

所以，为了创建一个Mina基础应用你必须：

1. Create an I/O service - Choose from already available Services (*Acceptor) or create your own

2. Create a Filter Chain - Choose from already existing Filters or create a custom Filter for transforming request/response

3. Create an I/O Handler - Write business logic, on handling different messages

**这就差不多了，你可以阅读深一点的这里有两篇介绍：**
- · [Server Architecture](http://mina.apache.org/mina-project/userguide/ch2-basics/server-architecture.html)（服务端架构）
- · [Client Architecture](http://mina.apache.org/mina-project/userguide/ch2-basics/client-architecture.html)（客户端架构）

当然,Mina不仅仅提供这些,你将很可能不得不照顾许多方面,如消息编码/解码、网络配置如何扩大,等等……我们将有一个进一步的看那些方面在接下来的章节。

#### 1.1.1.1. **服务端架构**

在之前的部分，我们已经剖析了Mina的架构。让我们来聚焦服务端的架构吧。通常，服务端为进来的请求设置监听端口，处理请求并回应请求端。它也会为每个客户端创建并处理一个session（无论是我们的TCP还是UDP基本协议中）。这个会在后面[第四章](http://mina.apache.org/mina-project/userguide/ch4-session/ch4-session.html)进行讲解。

![](https://img-blog.csdn.net/20160601181542099)
- · IOAcceptor 监听网络进来的连接或数据包
- · 通过IP地址或端口为新连接或新session创建一个有序请求，并组合处理session
- · I**O Filters**所有数据包接收会话,遍历图中指定的过滤器链，过滤器可以用来修改数据包的内容（如对象转换、添加或移除信息等），从原始字节转换成/高级对象,PacketEncoder
 /Decoder尤其有用
- · 最后包转换对象IOHandler. IOHandlers 能够用于业务需求

**创建session：**

每当Mina服务器上的客户端连接,我们将创建一个新的会话存储持久数据。即使协议是没有连接,也会创建会话。下面的模式显示了Mina如何处理传入的连接: (Incoming
 connections handling-图片不见了，需要在网上搜寻补充)

**输入报文消息处理：**

· 现在我们将解释Mina处理传入消息。假设创建一个会话,任何新传入消息都会导致选择器被唤醒。

#### 1.1.1.2. **客户端架构**

我们来简单的看下在Mina服务器架构的基础上,可以看到客户的样子。客户端需要连接到服务器,发送信息和过程的响应。

![](https://img-blog.csdn.net/20160601181619860)

- · 客户端首先创建一个 IOConnector (MINA 构造Socket连接), 与服务器发起绑定访问
- · 一旦连接创建，同时一个session也会被创建并关联到这个连接
- · 应用或客户端写入session, 致使数据发送的服务器端，之后在过滤器链中遍历
- · 所有从服务端接收到的相应或消息都是通过过滤器连对应的 IOHandler进行处理

### 1.1.2. **TCP服务端示例**



本教程将会带领你处理一个基本的Mina程序，这里会构建一个time server.下面是本教程的先决条件：
- 
· MINA 2.x Core核心包

· JDK 1.5 或更高版本

· SLF4J 1.3.0 或更高版本





o **Log4J 1.2** users: slf4j-api.jar, slf4j-log4j12.jar, and [Log4J](http://logging.apache.org/log4j/1.2/) 1.2.x

o **Log4J 1.3** users: slf4j-api.jar, slf4j-log4j13.jar, and [Log4J](http://logging.apache.org/log4j/1.2/) 1.3.x

o **java.util.logging** users: slf4j-api.jar and slf4j-jdk14.jar

o **IMPORTANT**: Please make sure you are using the right slf4j-*.jar that matches to your logging framework.

例如，slf4j-log4j12.jar 和log4j-1.3.x.jar不能同时使用会产生冲突。

我们已经测试了这个程序在Windows©2000专业和linux。让这个项目工作如果你有任何问题，请不要犹豫与我们或跟Mina开发人员联系。此外,本教程一直试图保持独立的开发环境(IDE、编辑.
 .等等)。本教程将使用任何你熟悉的环境。编译命令和执行步骤简洁的程序已被移除。如果你需要帮助学习如何编译或执行java程序,请参阅[java教程](http://java.sun.com/docs/books/tutorial/)。



#### 1.1.2.1. 编写Mina时间服务器






首先我们创建MinaTimeServer.java文件，并初始化如下代码：



```java
public class MinaTimeServer {
    public static void main(String[] args) {

        // code will go here next
    }
}
```



这段代码应该是简单的。我们只是简单地定义一个主方法用于启动程序。在这一点上,我们将开始添加代码实现我们的服务器。首先,我们需要一个用于侦听传入的连接对象。因为这个项目将基于TCP
 / IP的,我们将添加一个SocketAcceptor到我们的程序。



```java
import org.apache.mina.transport.socket.nio.NioSocketAcceptor;
public class MinaTimeServer{
    public static void main( String[] args )
    {
        IoAcceptor acceptor = new NioSocketAcceptor();
    }
}
```



使用NioSocketAcceptor类,我们可以继续定义处理程序类NioSocketAcceptor并绑定到端口。



```java
import java.net.InetSocketAddress;

import org.apache.mina.core.service.IoAcceptor;import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

public class MinaTimeServer{
    private static final int PORT = 9123;

    public static void main( String[] args ) throws IOException
    {
        IoAcceptor acceptor = new NioSocketAcceptor();
        acceptor.bind( new InetSocketAddress(PORT) );
    }
}
```



正如你所见，这里有一个acceptor.setLocalAddress( new InetSocketAddress(PORT) );调用。该方法定义了这个服务器将监听主机和端口。最后一个方法是调用IoAcceptor.bind()。该方法将绑定到指定的港口并开始处理远程客户端。

接下来,我们添加一个过滤器,配置。这个过滤器将记录所有信息,如新创建的session，接收到的message,发送的message、session关闭。下一个是ProtocolCodecFilter过滤器，这个过滤器将二进制或协议特定的数据转化为消息对象,反之亦然。我们使用现有的TextLine工厂,因为它将为您处理文本（text）基础信息(您不必编写编解码器部分)。



```java
import java.io.IOException;import java.net.InetSocketAddress;import java.nio.charset.Charset;

import org.apache.mina.core.service.IoAcceptor;import org.apache.mina.filter.codec.ProtocolCodecFilter;import org.apache.mina.filter.codec.textline.TextLineCodecFactory;import org.apache.mina.filter.logging.LoggingFilter;import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

public class MinaTimeServer{

    public static void main( String[] args )
    {

        IoAcceptor acceptor = new NioSocketAcceptor();

        acceptor.getFilterChain().addLast( "logger", new LoggingFilter() );

        acceptor.getFilterChain().addLast( "codec", new ProtocolCodecFilter( new TextLineCodecFactory( Charset.forName( "UTF-8" ))));

        acceptor.bind( new InetSocketAddress(PORT) );

    }}
```



在这一点上，我们将定义被用于服务客户端连接请求处理程序，以获取当前时间。处理器类是一个实现IoHandler接口的类。几乎所有的程序都在使用Mina,这成为该计划的主力,因为它能从客户提供传入的请求服务。对于这个教程，其类集成自IoHandlerAdapter。此类追随[adapter
 design pattern](http://en.wikipedia.org/wiki/Adapter_pattern)使用大量代码写入有序的特性需求的处理实现通过IoHandler接口。



```java
import java.net.InetSocketAddress;import java.nio.charset.Charset;

import org.apache.mina.core.service.IoAcceptor;import org.apache.mina.filter.codec.ProtocolCodecFilter;import org.apache.mina.filter.codec.textline.TextLineCodecFactory;import org.apache.mina.filter.logging.LoggingFilter;import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

public class MinaTimeServer{

    public static void main( String[] args ) throws IOException
    {
        IoAcceptor acceptor = new NioSocketAcceptor();
        acceptor.getFilterChain().addLast( "logger", new LoggingFilter() );
        acceptor.getFilterChain().addLast( "codec", new ProtocolCodecFilter( new TextLineCodecFactory( Charset.forName( "UTF-8" ))));
        acceptor.setHandler(  new TimeServerHandler() );
        acceptor.bind( new InetSocketAddress(PORT) );

    }}
```



我们现在添加NioSocketAcceptor配置。它接受来自客户端的特定设置的Socket请求。



```java
import java.net.InetSocketAddress;import java.nio.charset.Charset;

import org.apache.mina.core.session.IdleStatus;import org.apache.mina.core.service.IoAcceptor;import org.apache.mina.filter.codec.ProtocolCodecFilter;import org.apache.mina.filter.codec.textline.TextLineCodecFactory;import org.apache.mina.filter.logging.LoggingFilter;import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

public class MinaTimeServer{

    public static void main( String[] args ) throws IOException
    {
        IoAcceptor acceptor = new NioSocketAcceptor();
        acceptor.getFilterChain().addLast( "logger", new LoggingFilter() );
        acceptor.getFilterChain().addLast( "codec", new ProtocolCodecFilter( new TextLineCodecFactory( Charset.forName( "UTF-8" ))));
        acceptor.setHandler(  new TimeServerHandler() );
        acceptor.getSessionConfig().setReadBufferSize( 2048 );
        acceptor.getSessionConfig().setIdleTime( IdleStatus.BOTH_IDLE, 10 );
        acceptor.bind( new InetSocketAddress(PORT) );
    }}
```



这里有两个新行MinaTimeServer类。这些方法设置设置IoHandler,输入缓冲区大小和空闲session的属性。为了指定的缓冲区大小将告诉底层操作系统为传入的数据分配多少空间。第二行指定当检查闲置会话。调用setIdleTime,第一个参数定义了什么行动来检查确定会话空闲时,第二个参数定义了在几秒钟的时间,必须发生在一个会话被认为是闲置。Handler对应的代码如下：



```java
import java.util.Date;

import org.apache.mina.core.session.IdleStatus;import org.apache.mina.core.service.IoHandlerAdapter;import org.apache.mina.core.session.IoSession;

public class TimeServerHandler extends IoHandlerAdapter{

    @Override

    public void exceptionCaught( IoSession session, Throwable cause ) throws Exception

    {

        cause.printStackTrace();

    }

    @Override

    public void messageReceived( IoSession session, Object message ) throws Exception

    {

        String str = message.toString();

        if( str.trim().equalsIgnoreCase("quit") ) {

            session.close();

            return;

        }

        Date date = new Date();

        session.write( date.toString() );

        System.out.println("Message written...");

    }

    @Override

    public void sessionIdle( IoSession session, IdleStatus status ) throws Exception

    {

        System.out.println( "IDLE " + session.getIdleCount( status ));

    }

}
```



      这个类的方法被用于exceptionCaught,messageReceived sessionIdle。exceptionCaught来定义一个正常和异常处理程序来处理提高了处理远程连接。如果没有定义这个方法,异常可能不会得到正确的报告出来。

     exceptionCaught方法将直接打印错误的堆栈跟踪并关闭session会话。对于大多数项目,这是标准做法,除非异常条件的处理程序是可以恢复的。

     messageReceived方法将接收来自客户机的数据，并写回客户机当前时间。如果从客户端接收到的消息是“quit”这个词,那么session会话将被关闭。这种方法也将打印当前时间到客户机。这取决于您所使用的协议编解码器，该对象(第二个参数)被传递给这个方法会有所不同,以及你传入的对象session.write(Object)方法。如果你不指定协议编解码器,您很可能会收到一个IoBuffer对象,并被要求写一份IoBuffer对象。

    sessionIdle方法将调用一次session会话保持指令规定的空闲时间。如：acceptor.getSessionConfig().setIdleTime(
 IdleStatus.BOTH_IDLE, 10 );.

剩下要做的就是定义服务器server端监听Socket地址,实际上调用时将启动服务器。代码如下所示:



```java
<span style="font-size:12px;">import java.io.IOException;import java.net.InetSocketAddress;import java.nio.charset.Charset;

import org.apache.mina.core.service.IoAcceptor;import org.apache.mina.core.session.IdleStatus;import org.apache.mina.filter.codec.ProtocolCodecFilter;import org.apache.mina.filter.codec.textline.TextLineCodecFactory;import org.apache.mina.filter.logging.LoggingFilter;import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

public class MinaTimeServer{

    private static final int PORT = 9123;

    public static void main( String[] args ) throws IOException

    {

        IoAcceptor acceptor = new NioSocketAcceptor();

        acceptor.getFilterChain().addLast( "logger", new LoggingFilter() );

        acceptor.getFilterChain().addLast( "codec", new ProtocolCodecFilter( new TextLineCodecFactory( Charset.forName( "UTF-8" ))));

        acceptor.setHandler( new TimeServerHandler() );

        acceptor.getSessionConfig().setReadBufferSize( 2048 );

        acceptor.getSessionConfig().setIdleTime( IdleStatus.BOTH_IDLE, 10 );

        acceptor.bind( new InetSocketAddress(PORT) );

    }}</span>
```



#### 1.1.2.2. 试图退出时间服务器



在这一点上,我们可以继续并编译程序。一旦编译程序，可以运行程序来测试会发生什么。以最简单的方法来启动程序,使用telnet客户端程序:
|**Client Output**|**Server Output**|
|----|----|
|user@myhost:~> telnet 127.0.0.1 9123 Trying 127.0.0.1... Connected to 127.0.0.1. Escape character is '^]'. hello Wed Oct 17 23:23:36 EDT 2007 quit Connection closed by foreign host. user@myhost:~>|MINA Time server started. Message written...|

#### 1.1.2.3. 接下来干什么



请访问我们的文档页面找到更多资源。你也可以继续阅读其他教程。


### 1.1.3. **TCP客户端示例**

我们已经看过客户端的架构了，让我们来探讨一个客户端的实现示例。我们引用[Sumup
 Client](http://mina.apache.org/mina-project/xref/org/apache/mina/example/sumup/Client.html)的一个客户端实现来讲解。

我们将移除不必要的部分而集中关注它的主要构造。下面是客户端的代码：

```java
<span style="font-size:12px;"><span style="font-size:12px;">public static void main(String[] args) throws Throwable {

    NioSocketConnector connector = new NioSocketConnector();

    connector.setConnectTimeoutMillis(CONNECT_TIMEOUT);


    if (USE_CUSTOM_CODEC) {

    connector.getFilterChain().addLast("codec",

        new ProtocolCodecFilter(new SumUpProtocolCodecFactory(false)));

    } else {

        connector.getFilterChain().addLast("codec",

            new ProtocolCodecFilter(new ObjectSerializationCodecFactory()));

    }
 

    connector.getFilterChain().addLast("logger", new LoggingFilter());

    connector.setHandler(new ClientSessionHandler(values));

    IoSession session;
 

    for (;;) {

        try {

            ConnectFuture future = connector.connect(new InetSocketAddress(HOSTNAME, PORT));

            future.awaitUninterruptibly();

            session = future.getSession();

            break;

        } catch (RuntimeIoException e) {

            System.err.println("Failed to connect.");

            e.printStackTrace();

            Thread.sleep(5000);

        }

    }


    // wait until the summation is done

    session.getCloseFuture().awaitUninterruptibly();

    connector.dispose();}</span></span>
```

**构造一个客户端我们需要做如下步骤：**
- · 创建一个 Connector
- · 创建一个 Filter Chain
- · 创建一个 IOHandler 并添加到Connector
- · 绑定到Server访问

让我们检查每一个细节。

#### 1.1.3.1. **创建一个Connector**

```java
<span style="font-size:12px;"><span style="font-size:12px;">NioSocketConnector connector = new NioSocketConnector();</span></span>
```

这里我们创建了一个NIO Socket连接。

#### 1.1.3.2. **创建一个Filter chain**

```java
<span style="font-size:12px;"><span style="font-size:12px;">if (USE_CUSTOM_CODEC) {

    connector.getFilterChain().addLast("codec",

        new ProtocolCodecFilter(new SumUpProtocolCodecFactory(false)));} else {

    connector.getFilterChain().addLast("codec",

        new ProtocolCodecFilter(new ObjectSerializationCodecFactory()));}</span></span>
```

添加Filter chain到Connector,这里我们已经添加了ProtocolCodec到过滤器链。

#### 1.1.3.3. **创建一个IOHandler**

```java
<span style="font-size:12px;"><span style="font-size:12px;">connector.setHandler(new ClientSessionHandler(values));</span></span>
```

我们创建了一个[ClientSessionHandler](http://mina.apache.org/mina-project/xref/org/apache/mina/example/sumup/ClientSessionHandler.html)并将它设置到了Connector。

#### 1.1.3.4. **绑定Server**

```java
<span style="font-size:12px;"><span style="font-size:12px;">IoSession session;

for (;;) {

    try {

        ConnectFuture future = connector.connect(new InetSocketAddress(HOSTNAME, PORT));

        future.awaitUninterruptibly();

        session = future.getSession();

        break;

    } catch (RuntimeIoException e) {

        System.err.println("Failed to connect.");

        e.printStackTrace();

        Thread.sleep(5000);

    }}</span></span>
```

这是最重要的东西，通过以上代码我们连接到远程服务器。连接是一个异步任务,我们使用ConnectFuture类知道当连接完成。连接完成后,我们得到了IoSession的关联。向服务器发送任何消息,我们必须写入session会话。所有响应/消息从服务器过滤器链遍历最后在IoHandler被处理。

### 1.1.4. **UDP服务端示例**

我们将从[org.apache.mina.example.udp](http://mina.apache.org/mina-project/xref/org/apache/mina/example/udp/package-summary.html)包查看代码。为了生动简单，我们只关注Mina的相关构造。

**为构造server端我们需要做如下步骤：**
- 创建一个Datagram Socket监听进来的客户端请求 (参见 [MemoryMonitor.java](http://mina.apache.org/mina-project/xref/org/apache/mina/example/udp/MemoryMonitor.html))
- 创建一个IoHandler处理Mina框架生成的事件 (参见[MemoryMonitorHandler.java](http://mina.apache.org/mina-project/xref/org/apache/mina/example/udp/MemoryMonitorHandler.html))

这里是第一个程序小片段：

```java
<span style="font-size:12px;"><span style="font-size:12px;">NioDatagramAcceptor acceptor = new NioDatagramAcceptor();acceptor.setHandler(new MemoryMonitorHandler(this));</span></span>
```

在这里,我们创建一个NioDatagramAcceptor侦听传入的客户端请求,并设置IoHandler。变量“PORT”只是一个int。下一步是添加日志过滤器到过滤器链将被DatagramAcceptor所使用。LoggingFilter是一个查看Mina运行行为好的方法。它在不同阶段生成日志语句,通过它能够观察Mina是如何工作的。

```java
<span style="font-size:12px;"><span style="font-size:12px;">DefaultIoFilterChainBuilder chain = acceptor.getFilterChain();chain.addLast("logger", new LoggingFilter());</span></span>
```

接下来我们进入一些更具体的UDP传输代码。我们将设置Connector重用地址:

```java
<span style="font-size:12px;"><span style="font-size:12px;">DatagramSessionConfig dcfg = acceptor.getSessionConfig();dcfg.setReuseAddress(true);

acceptor.bind(new InetSocketAddress(PORT));</span></span>
```

当然最后一件事就是调用bind()方法。

#### 1.1.4.1. **IOHandler 实现**

有三个主要的事件为我们的服务器实现：
- · Session 创建
- · Message 接收
- · Session 关闭

我们来看一下每一个事件的详情。

#### 1.1.4.2. **Session创建事件**

```java
<span style="font-size:12px;"><span style="font-size:12px;">@Overridepublic void sessionCreated(IoSession session) throws Exception {

    SocketAddress remoteAddress = session.getRemoteAddress();

server.addClient(remoteAddress);

}</span></span>
```

在Session中创建事件,我们就调用内部增加了一个Tab界面的addClient()函数

#### 1.1.4.3. **Message接收事件**

```java
<span style="font-size:12px;"><span style="font-size:12px;">@Overridepublic void messageReceived(IoSession session, Object message) throws Exception {

    if (message instanceof IoBuffer) {

        IoBuffer buffer = (IoBuffer) message;

        SocketAddress remoteAddress = session.getRemoteAddress();

        server.recvUpdate(remoteAddress, buffer.getLong());

    }

 }</span></span>
```

在消息接收事件中,我们只备份接收到的消息数据。应用需要发送响应,应用程序可以处理消息并编写，通过这个函数响应给session。

#### 1.1.4.4. **Session关闭事件**

```java
<span style="font-size:12px;"><span style="font-size:12px;">@Overridepublic void sessionClosed(IoSession session) throws Exception {

    System.out.println("Session closed...");

    SocketAddress remoteAddress = session.getRemoteAddress();

server.removeClient(remoteAddress);

}</span></span>
```

在会话关闭时,事件中我们只从UI选项卡删除客户端Session。

### 1.1.5. **UDP客户端示例**

让我们看看为UDP服务器实现的客户端代码。实现客户端我们需要做如下步骤：
- · 创建Socket 并连接到 Server
- · 设置IoHandler
- · 收集空闲内存
- · 发送数据到Server

我们从查看[MemMonClient.java](http://mina.apache.org/mina-project/xref/org/apache/mina/example/udp/client/MemMonClient.html)文件开始，找到org.apache.mina.example.udp.client对应的Java包。可以看到，前几行代码简单明了。

```java
<span style="font-size:12px;"><span style="font-size:12px;">connector = new NioDatagramConnector();

connector.setHandler( this );

ConnectFuture connFuture = connector.connect( new InetSocketAddress("localhost", MemoryMonitor.PORT ));</span></span>
```

在这里,我们创建一个NioDatagramConnector,设置Handler并连接到server。一个问题是,您必须设置主机InetSocketAddress，否则就不会工作。这个例子主要是编写和测试在Windows
 XP机器上,所以事情可能会有所不同。接下来,我们将等待确认,客户端连接到服务器。一旦我们知道已经建立连接,就可以开始写数据到服务器。下面是这段代码:

```java
<span style="font-size:12px;"><span style="font-size:12px;">connFuture.addListener( new IoFutureListener(){

            public void operationComplete(IoFuture future) {

                ConnectFuture connFuture = (ConnectFuture)future;

                if( connFuture.isConnected() ){

                    session = future.getSession();

                    try {

                        sendData();

                    } catch (InterruptedException e) {

                        e.printStackTrace();

                    }

                } else {

                    log.error("Not connected...exiting");

                }

            }

        });</span></span>
```

在这里,我们将一个监听器添加到ConnectFutur对象，当我们接收一个回调客户端连接,我们将开始写数据。由一个sendData（）方法写数据到服务器。这种方法如下所示:

```java
<span style="font-size:12px;"><span style="font-size:12px;">private void sendData() throws InterruptedException {

    for (int i = 0; i < 30; i++) {

        long free = Runtime.getRuntime().freeMemory();

        IoBuffer buffer = IoBuffer.allocate(8);

        buffer.putLong(free);

        buffer.flip();

        session.write(buffer);

        try {

            Thread.sleep(1000);

        } catch (InterruptedException e) {

            e.printStackTrace();

            throw new InterruptedException(e.getMessage());

        }

    }}</span></span>
```

该方法将编写服务器空闲内存的数量一次30秒。在这里你可以看到我们分配一个IoBuffer足够容纳一个长时间的变量,然后空闲出把大量的内存缓冲区。然后这个缓冲区翻转并写入到服务器。我们的UDP客户端实现就完成了。

### 1.1.6. **总结**

在这一章,我们了解了Mina建立应用程序架构,客户端以及服务器。我们还实现了示例的TCP服务器/客户端以及UDP服务器和客户端。   在接下来的章节我们将讨论Mina核心结构和高级主题。



