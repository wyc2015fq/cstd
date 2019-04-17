# Mina APR传输与串口传输讲解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月04日 00:49:24[boonya](https://me.csdn.net/boonya)阅读数：3117
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina Transports(Apache Mina user guide Chapter6 Transport)](http://mina.apache.org/mina-project/userguide/ch6-transports/ch6-transports.html)



### 1.1.1. **APR传输**

#### 1.1.1.1. **介绍**



[APR (Apache Portable Runtime)](http://apr.apache.org/) Apache便携式运行时库提供卓越的可扩展性、性能和能够更好地与本地服务器技术集成。Mina支持APR传输。在这部分，我没们将在Mina的基础之上使用APR传输。我们将使用Time
 Server的例子。

#### 1.1.1.2. **先决条件**

**APR传输依赖以下组件：**

- APR 程序包- 从[http://www.apache.org/dist/tomcat/tomcat-connectors/native/](http://www.apache.org/dist/tomcat/tomcat-connectors/native/)下载/安装适当的平台jar包。
- JNI wrapper (tomcat-apr-5.5.23.jar)发布版本自带这个包。



将本地包加入到PATH路径下。

#### 1.1.1.3. **使用APR传输**

参考[Time Server](http://mina.apache.org/mina-project/xref/org/apache/mina/example/gettingstarted/timeserver/)完整示例程序。让我们来看出基于NIO的Time
 Server实现是什么样的：

```java
IoAcceptor acceptor = new NioSocketAcceptor();

acceptor.getFilterChain().addLast( "logger", new LoggingFilter() );acceptor.getFilterChain().addLast( "codec", new ProtocolCodecFilter( new TextLineCodecFactory( Charset.forName( "UTF-8" ))));

acceptor.setHandler(  new TimeServerHandler() );

acceptor.getSessionConfig().setReadBufferSize( 2048 );acceptor.getSessionConfig().setIdleTime( IdleStatus.BOTH_IDLE, 10 );

acceptor.bind( new InetSocketAddress(PORT) );
```

让我们来看怎么使用APR传输：

```java
IoAcceptor acceptor = new AprSocketAcceptor();

acceptor.getFilterChain().addLast( "logger", new LoggingFilter() );acceptor.getFilterChain().addLast( "codec", new ProtocolCodecFilter( new TextLineCodecFactory( Charset.forName( "UTF-8" ))));

acceptor.setHandler(  new TimeServerHandler() );

acceptor.getSessionConfig().setReadBufferSize( 2048 );acceptor.getSessionConfig().setIdleTime( IdleStatus.BOTH_IDLE, 10 );

acceptor.bind( new InetSocketAddress(PORT) );
```

我们只是将NioSocketAcceptor变为AprSocketAcceptor了。就是这样，现在我们的Time
 Server将会使用APR传输。

其他完整的流程仍然是相同的。

### 1.1.2. **Serial传输**

通过Mina2.0你可以连接到串行端口通过使用连接到Mina的TCP / IP端口。

#### 1.1.2.1. **了解Mina2.0**

你可以下载最新的构建版本(目前最新的是2.0.13)。如果你喜欢从仓库构建代码，如需要帮助请查阅开发者指南。

#### 1.1.2.2. **先决条件**

**有用的信息：**

在访问串行端口之前Java程序需要一个本地程序库（根据你的操作系统使用.DLL或.so文件)。Mina使用来自于[ftp://ftp.qbang.org/pub/rxtx/rxtx-2.1-7-bins-r2.zip](ftp://ftp.qbang.org/pub/rxtx/rxtx-2.1-7-bins-r2.zip)的RXTX.org包。

只需要将你完好的.DLL或.so文件放入你的JDK/JRE里面的jre/lib/i386/路径，或使用-Djava.library.path=定义你的本地程序包路径。

**有用的信息：**

mina-transport-serial.jar不包括在完整的分发包。你可以从[这里](http://repo1.maven.org/maven2/org/apache/mina/mina-transport-serial/2.0.2/)下载。

#### 1.1.2.3.  **连接一个串行端口**

串行通信Mina只提供一个IoConnector，由于其本身是点对点通信媒体。在这个时候，你应该已经读了Mina教程。现在连接到串行端口你需要SerialConnector:

```java
// create your connector

IoConnector connector = new SerialConnector()connector.setHandler( ... here your buisness logic IoHandler ... );
```

没有什么与SocketConnector截然不同，让我们创建一个地址连接到串行端口：



```java
SerialAddress portAddress=new SerialAddress( "/dev/ttyS0", 38400, 8, StopBits.BITS_1, Parity.NONE, FlowControl.NONE );
```



第一个参数是你的端口标识。Windows电脑的串口被称为“COM1”、“COM2”等……Linux和其他一些Unix被称为:“/ dev / ttyS0”、“/
 dev / ttyS1”、“/ dev / ttyUSB0”。

其余的参数是根据设备的驱动和所谓的通信特点。
- · the baud rate
- · the data bits
- · the parity
- · the flow control mecanism



一旦完成,将连接器连接到地址:

```java
ConnectFuture future = connector.connect( portAddress );

future.await();

IoSession sessin = future.getSession();
```

看吧！其他的都是像往常一样,你可以插入过滤器和编解码器。为了学习更多关于RS232的请参看：[http://en.wikipedia.org/wiki/RS232](http://en.wikipedia.org/wiki/RS232)






