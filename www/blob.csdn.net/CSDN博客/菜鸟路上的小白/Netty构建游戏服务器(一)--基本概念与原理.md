# Netty构建游戏服务器(一)--基本概念与原理 - 菜鸟路上的小白 - CSDN博客





2016年10月26日 20:07:13[somnus_小凯](https://me.csdn.net/u012486840)阅读数：8113
个人分类：[Netty](https://blog.csdn.net/u012486840/article/category/6483866)









# 一，Netty是什么

1，Netty是由[JBOSS](http://baike.baidu.com/view/309533.htm)提供的一个[java开源](http://baike.baidu.com/view/629119.htm)框架。

2，Netty是JAR包，一般使用ALL-IN-ONE的JAR包就可以开发了。

3，Netty不需要运行在Tomcat这类服务器中，他是单独构建一个服务器。

4，Netty可以构建HTTP服务器，socket服务器，websocket服务器等。

5，Netty其实是对JDK1.4以后提供的NIO的封装，NIO就是new i/o，JDK1.7推出了NIO2。

# 二，Netty服务架构图

以下官方图展示了Netty基本提供的服务：

![](http://static.oschina.net/uploads/img/201609/10175538_0V1k.jpg)

主要提供的就是HTTP服务器，socket服务器，websocket服务器。

# 三，Netty原理架构图

从下面的原创图，可以基本看出Netty构建服务器的原理（开源中国博客不能全屏了，图变得很小，没有办法）：

![](http://static.oschina.net/uploads/space/2016/0910/175226_SIe6_1476692.jpg)

## 1，组件名词解释

(1),Bootstrap / ServerBootstrap（建立连接）：

Netty引导组件，简化NIO的开发步骤，是一个Netty程序的开始，作用是配置和串联各个组件。

(2)EventLoopGroup（事件循环组）：

是EventLoop组合，可以包含多个EventLoop。创建一个EventLoopGroup的时候，内部包含的方法就会创建一个子对象EventLoop。

(3)EventLoop（事件循环）：

循环服务Channel，可以包含多个Channel。

(4)Channel（通道）：

代表一个Scoket连接，或者其他IO操作组件。

(5)ChannelInitializer（初始化连接）：

主要提供了一个传输通道ChannelPipeline。

(6)ChannelPipeline（传输通道）：

主要是管理各种ChannelHandler业务控制器，提供一个链式管理模式。

(7)ChannelHandler（业务控制器）：

主要业务写入的地方，由开发人员写入，Netty也提供了很多写好的控制器和适配器，可以直接引用。

(8)ChannelInboundHandler（通道传入控制器）：

继承至ChannelHandler，在传输通道中对传入事件进行控制。

(9)ChannelOutboundHandler（通道传出控制器）：

继承至ChannelHandler，在传输通道中对传出事件进行控制。

(10)Decoder（解码）：

网络传输都是byte传输，所以Netty首先接收到的是byte，需要进行解码，编程JAVA对象。

Netty提供了很多解码器，包括服务架构图（最上）显示的Google Protobuf编码，这是Google提供的跨平台的小体积编码方式，在Netty中可以直接解码。

(11)Encoder（编码）：

和解码类似，在传出服务器的时候，需要编码成byte传输给客户端。

(12)Future / ChannelFuture(消息返回，图上没有)：

Netty提供的返回结果，类似回调函数，告知你执行结果是什么。

## 2，构建服务器基本方法

从原理架构图可以看出，构建一个Netty服务器基本需要3个步骤：

1），ServerBootstrap（建立连接）：构建一个Scoket或者其他连接，通过事件循环建立通道。

2），ChannelInitializer（初始化连接）：构建传输通道，用于管理控制器。

3），ChannelHandler（业务控制器）：构建业务控制器，最基本的是解码，编码，信息传入/传出。

同样，构建一个客户端，同样也是这些步骤，不过建立连接是使用Bootstrap，并且使用一个事件循环，而服务器一般使用两个事件循环。

具体代码可以看一下节内容：构建一个最简单的socket服务器与客户端。

# 四，Netty构建游戏服务器有什么优势

1，游戏服务器是一个复杂的组成，基本要求是低延迟和高并发。游戏服务器经常需要各种通信，例如地图服务器、网管服务器、聊天服务器之间的通信。Netty的异步NIO框架可以保证高性能的通信能力。

2，灵活的编码解码定制能力，满足不同游戏场景下的多协议和私有协议编解码。

3，可配置的线程池，TCP参数，提供差异化定制能力。

4，黑白名单过滤（IP过滤），SSL安全，可以用于登录认证环节。

5，Netty心跳检测（链路有效性检测），用于检测客户端是否处于活跃状态。心跳检测是指，服务器定期向客户端发送指定内容，通过反馈来判断链路是否可用，客户端是否活着，是否可以正常接收和发送消息。

6，流量整形，一种主动调整流量输出速率的措施。作用是

1），防止由于上下游网元性能不均衡导致下游网元被压垮，业务流程中断；

2），防止由于通信模块接收消息过快，后端业务线程处理不及时导致的“撑死”问题。

流行整形原理是将废弃报文先缓存，放入队列中，当具有足够令牌的时候在发送缓存的报文。这种做法可以缓解压力，但是有一定的延迟。

7，日志统计能力，提供游戏服务可服务性。

8，基于内存池的对象重用技术，降低GC频度，使得玩家不卡。



