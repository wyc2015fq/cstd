# 一起学Netty（十四）之 Netty生产级的心跳和重连机制 - z69183787的专栏 - CSDN博客
2016年10月31日 11:15:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1848
sigh，写这篇博客的时候老脸还是红了一下，心里还是有些唏嘘的，应该算是剽窃吧，每个人的代码功力的确是有差距的，好在文章的标题是“一起学”，而不是开涛大神的“跟我学”系列的文章，我们还是多花点时间学习吧，感叹无用~
最近工作比较忙，但闲暇之余还是看了阿里的冯家春（fengjiachun）的github上的开源代码Jupiter，写的RPC框架让我感叹人外有人，废话不多说，下面的代码全部截取自Jupiter，写了一个比较完整的例子，供大家一起学习分享，再次对@Luca抱拳，Jupiter的Github地址：
https://github.com/fengjiachun/Jupiter
今天研究的是，心跳和重连，虽然这次是大神写的代码，但是万变不离其宗，我们先回顾一下Netty应用心跳和重连的整个过程：
1）客户端连接服务端
2）在客户端的的ChannelPipeline中加入一个比较特殊的IdleStateHandler，设置一下客户端的写空闲时间，例如5s
3）当客户端的所有ChannelHandler中4s内没有write事件，则会触发userEventTriggered方法（上文介绍过）
4）我们在客户端的userEventTriggered中对应的触发事件下发送一个心跳包给服务端，检测服务端是否还存活，防止服务端已经宕机，客户端还不知道
5）同样，服务端要对心跳包做出响应，其实给客户端最好的回复就是“不回复”，这样可以服务端的压力，假如有10w个空闲Idle的连接，那么服务端光发送心跳回复，则也是费事的事情，那么怎么才能告诉客户端它还活着呢，其实很简单，因为5s服务端都会收到来自客户端的心跳信息，那么如果10秒内收不到，服务端可以认为客户端挂了，可以close链路
6）加入服务端因为什么因素导致宕机的话，就会关闭所有的链路链接，所以作为客户端要做的事情就是短线重连
以上描述的就是整个心跳和重连的整个过程，虽然很简单，上一篇blog也写了一个Demo,简单地做了一下上述功能
要写工业级的Netty心跳重连的代码，需要解决一下几个问题：
1）ChannelPipeline中的ChannelHandlers的维护，首次连接和重连都需要对ChannelHandlers进行管理
2）重连对象的管理，也就是bootstrap对象的管理
3）重连机制编写
完整的代码：https://github.com/BazingaLyn/netty-study/tree/master/src/main/[Java](http://lib.csdn.net/base/javaee)/com/lyncc/netty/idle
下面我们就看大神是如何解决这些问题的，首先先定义一个接口ChannelHandlerHolder，用来保管ChannelPipeline中的Handlers的
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.channel.ChannelHandler;  
- 
- /**
-  * 
-  * 客户端的ChannelHandler集合，由子类实现，这样做的好处：
-  * 继承这个接口的所有子类可以很方便地获取ChannelPipeline中的Handlers
-  * 获取到handlers之后方便ChannelPipeline中的handler的初始化和在重连的时候也能很方便
-  * 地获取所有的handlers
-  */
- publicinterface ChannelHandlerHolder {  
- 
-     ChannelHandler[] handlers();  
- }  
我们再来编写我们熟悉的服务端的ServerBootstrap的编写：
HeartBeatServer.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.bootstrap.ServerBootstrap;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.ChannelOption;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.SocketChannel;  
- import io.netty.channel.socket.nio.NioServerSocketChannel;  
- import io.netty.handler.codec.string.StringDecoder;  
- import io.netty.handler.codec.string.StringEncoder;  
- import io.netty.handler.logging.LogLevel;  
- import io.netty.handler.logging.LoggingHandler;  
- import io.netty.handler.timeout.IdleStateHandler;  
- 
- import java.net.InetSocketAddress;  
- import java.util.concurrent.TimeUnit;  
- 
- publicclass HeartBeatServer {  
- 
- privatefinal AcceptorIdleStateTrigger idleStateTrigger = new AcceptorIdleStateTrigger();  
- 
- privateint port;  
- 
- public HeartBeatServer(int port) {  
- this.port = port;  
-     }  
- 
- publicvoid start() {  
-         EventLoopGroup bossGroup = new NioEventLoopGroup(1);  
-         EventLoopGroup workerGroup = new NioEventLoopGroup();  
- try {  
-             ServerBootstrap sbs = new ServerBootstrap().group(bossGroup, workerGroup)  
-                     .channel(NioServerSocketChannel.class).handler(new LoggingHandler(LogLevel.INFO))  
-                     .localAddress(new InetSocketAddress(port)).childHandler(new ChannelInitializer<SocketChannel>() {  
- protectedvoid initChannel(SocketChannel ch) throws Exception {  
-                             ch.pipeline().addLast(new IdleStateHandler(5, 0, 0, TimeUnit.SECONDS));  
-                             ch.pipeline().addLast(idleStateTrigger);  
-                             ch.pipeline().addLast("decoder", new StringDecoder());  
-                             ch.pipeline().addLast("encoder", new StringEncoder());  
-                             ch.pipeline().addLast(new HeartBeatServerHandler());  
-                         };  
- 
-                     }).option(ChannelOption.SO_BACKLOG, 128).childOption(ChannelOption.SO_KEEPALIVE, true);  
- // 绑定端口，开始接收进来的连接
-             ChannelFuture future = sbs.bind(port).sync();  
- 
-             System.out.println("Server start listen at " + port);  
-             future.channel().closeFuture().sync();  
-         } catch (Exception e) {  
-             bossGroup.shutdownGracefully();  
-             workerGroup.shutdownGracefully();  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- int port;  
- if (args.length > 0) {  
-             port = Integer.parseInt(args[0]);  
-         } else {  
-             port = 8080;  
-         }  
- new HeartBeatServer(port).start();  
-     }  
- 
- }  
单独写一个AcceptorIdleStateTrigger，其实也是继承ChannelInboundHandlerAdapter，重写userEventTriggered方法，因为客户端是write，那么服务端自然是read，设置的状态就是IdleState.READER_IDLE，源码如下：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.channel.ChannelHandler;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- import io.netty.handler.timeout.IdleState;  
- import io.netty.handler.timeout.IdleStateEvent;  
- 
- 
- @ChannelHandler.Sharable  
- publicclass AcceptorIdleStateTrigger extends ChannelInboundHandlerAdapter {  
- 
- @Override
- publicvoid userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {  
- if (evt instanceof IdleStateEvent) {  
-             IdleState state = ((IdleStateEvent) evt).state();  
- if (state == IdleState.READER_IDLE) {  
- thrownew Exception("idle exception");  
-             }  
-         } else {  
- super.userEventTriggered(ctx, evt);  
-         }  
-     }  
- }  
HeartBeatServerHandler就是一个很简单的自定义的Handler，不是重点：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- publicclass HeartBeatServerHandler extends ChannelInboundHandlerAdapter {  
- 
- 
- @Override
- publicvoid channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {  
-         System.out.println("server channelRead..");  
-         System.out.println(ctx.channel().remoteAddress() + "->Server :" + msg.toString());  
-     }  
- 
- @Override
- publicvoid exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {  
-         cause.printStackTrace();  
-         ctx.close();  
-     }  
- 
- }  
接下来就是重点，我们需要写一个类，这个类可以去观察链路是否断了，如果断了，进行循环的断线重连操作，ConnectionWatchdog，顾名思义，链路检测狗，我们先看完整代码：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.bootstrap.Bootstrap;  
- import io.netty.channel.Channel;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelFutureListener;  
- import io.netty.channel.ChannelHandler.Sharable;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.util.Timeout;  
- import io.netty.util.Timer;  
- import io.netty.util.TimerTask;  
- 
- import java.util.concurrent.TimeUnit;  
- 
- /**
-  * 
-  * 重连检测狗，当发现当前的链路不稳定关闭之后，进行12次重连
-  */
- @Sharable
- publicabstractclass ConnectionWatchdog extends ChannelInboundHandlerAdapter implements TimerTask ,ChannelHandlerHolder{  
- 
- 
- 
- privatefinal Bootstrap bootstrap;  
- privatefinal Timer timer;  
- privatefinalint port;  
- 
- privatefinal String host;  
- 
- privatevolatileboolean reconnect = true;  
- privateint attempts;  
- 
- 
- public ConnectionWatchdog(Bootstrap bootstrap, Timer timer, int port,String host, boolean reconnect) {  
- this.bootstrap = bootstrap;  
- this.timer = timer;  
- this.port = port;  
- this.host = host;  
- this.reconnect = reconnect;  
-     }  
- 
- /**
-      * channel链路每次active的时候，将其连接的次数重新☞ 0
-      */
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) throws Exception {  
- 
-         System.out.println("当前链路已经激活了，重连尝试次数重新置为0");  
- 
-         attempts = 0;  
-         ctx.fireChannelActive();  
-     }  
- 
- @Override
- publicvoid channelInactive(ChannelHandlerContext ctx) throws Exception {  
-         System.out.println("链接关闭");  
- if(reconnect){  
-             System.out.println("链接关闭，将进行重连");  
- if (attempts < 12) {  
-                 attempts++;  
-             }  
-         <pre name="code"class="java">             //重连的间隔时间会越来越长
- int timeout = 2 << attempts;  
-              timer.newTimeout(this, timeout, TimeUnit.MILLISECONDS);  
  } ctx.fireChannelInactive(); } public void run(Timeout timeout) throws Exception { ChannelFuture future; //bootstrap已经初始化好了，只需要将handler填入就可以了 synchronized (bootstrap) { bootstrap.handler(new
 ChannelInitializer<Channel>() { @Override protected void initChannel(Channel ch) throws Exception { ch.pipeline().addLast(handlers()); } }); future = bootstrap.connect(host,port); } //future对象 future.addListener(new ChannelFutureListener() { public void operationComplete(ChannelFuture
 f) throws Exception { boolean succeed = f.isSuccess(); //如果重连失败，则调用ChannelInactive方法，再次出发重连事件，一直尝试12次，如果失败则不再重连 if (!succeed) { System.out.println("重连失败"); f.channel().pipeline().fireChannelInactive(); }else{ System.out.println("重连成功"); } } }); }}稍微分析一下：
1）继承了ChannelInboundHandlerAdapter，说明它也是Handler，也对，作为一个检测对象，肯定会放在链路中，否则怎么检测
2）实现了2个接口，TimeTask，ChannelHandlerHolder
   ①TimeTask，我们就要写run方法，这应该是一个定时任务，这个定时任务做的事情应该是重连的工作
   ②ChannelHandlerHolder的接口，这个接口我们刚才说过是维护的所有的Handlers，因为在重连的时候需要获取Handlers
3）bootstrap对象，重连的时候依旧需要这个对象
4）当链路断开的时候会触发channelInactive这个方法，也就说触发重连的导火索是从这边开始的
好了，我们这边再写次核心的HeartBeatsClient的代码：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.bootstrap.Bootstrap;  
- import io.netty.channel.Channel;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelHandler;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.nio.NioSocketChannel;  
- import io.netty.handler.codec.string.StringDecoder;  
- import io.netty.handler.codec.string.StringEncoder;  
- import io.netty.handler.logging.LogLevel;  
- import io.netty.handler.logging.LoggingHandler;  
- import io.netty.handler.timeout.IdleStateHandler;  
- import io.netty.util.HashedWheelTimer;  
- 
- import java.util.concurrent.TimeUnit;  
- 
- publicclass HeartBeatsClient {  
- 
- protectedfinal HashedWheelTimer timer = new HashedWheelTimer();  
- 
- private Bootstrap boot;  
- 
- privatefinal ConnectorIdleStateTrigger idleStateTrigger = new ConnectorIdleStateTrigger();  
- 
- publicvoid connect(int port, String host) throws Exception {  
- 
-         EventLoopGroup group = new NioEventLoopGroup();    
- 
-         boot = new Bootstrap();  
-         boot.group(group).channel(NioSocketChannel.class).handler(new LoggingHandler(LogLevel.INFO));  
- 
- final ConnectionWatchdog watchdog = new ConnectionWatchdog(boot, timer, port,host, true) {  
- 
- public ChannelHandler[] handlers() {  
- returnnew ChannelHandler[] {  
- this,  
- new IdleStateHandler(0, 4, 0, TimeUnit.SECONDS),  
-                             idleStateTrigger,  
- new StringDecoder(),  
- new StringEncoder(),  
- new HeartBeatClientHandler()  
-                     };  
-                 }  
-             };  
- 
-             ChannelFuture future;  
- //进行连接
- try {  
- synchronized (boot) {  
-                     boot.handler(new ChannelInitializer<Channel>() {  
- 
- //初始化channel
- @Override
- protectedvoid initChannel(Channel ch) throws Exception {  
-                             ch.pipeline().addLast(watchdog.handlers());  
-                         }  
-                     });  
- 
-                     future = boot.connect(host,port);  
-                 }  
- 
- // 以下代码在synchronized同步块外面是安全的
-                 future.sync();  
-             } catch (Throwable t) {  
- thrownew Exception("connects to  fails", t);  
-             }  
-     }  
- 
- /**
-      * @param args
-      * @throws Exception
-      */
- publicstaticvoid main(String[] args) throws Exception {  
- int port = 8080;  
- if (args != null && args.length > 0) {  
- try {  
-                 port = Integer.valueOf(args[0]);  
-             } catch (NumberFormatException e) {  
- // 采用默认值
-             }  
-         }  
- new HeartBeatsClient().connect(port, "127.0.0.1");  
-     }  
- 
- }  
也稍微说明一下：
1）创建了ConnectionWatchdog对象，自然要实现handlers方法
2）初始化好bootstrap对象
3）4秒内没有写操作，进行心跳触发，也就是IdleStateHandler这个方法
最后ConnectorIdleStateTrigger这个类
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.buffer.ByteBuf;  
- import io.netty.buffer.Unpooled;  
- import io.netty.channel.ChannelHandler.Sharable;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- import io.netty.handler.timeout.IdleState;  
- import io.netty.handler.timeout.IdleStateEvent;  
- import io.netty.util.CharsetUtil;  
- 
- @Sharable
- publicclass ConnectorIdleStateTrigger extends ChannelInboundHandlerAdapter {  
- 
- privatestaticfinal ByteBuf HEARTBEAT_SEQUENCE = Unpooled.unreleasableBuffer(Unpooled.copiedBuffer("Heartbeat",  
-             CharsetUtil.UTF_8));  
- 
- @Override
- publicvoid userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {  
- if (evt instanceof IdleStateEvent) {  
-             IdleState state = ((IdleStateEvent) evt).state();  
- if (state == IdleState.WRITER_IDLE) {  
- // write heartbeat to server
-                 ctx.writeAndFlush(HEARTBEAT_SEQUENCE.duplicate());  
-             }  
-         } else {  
- super.userEventTriggered(ctx, evt);  
-         }  
-     }  
- }  
HeartBeatClientHandler.java(不是重点)
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51509847#)[copy](http://blog.csdn.net/linuu/article/details/51509847#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698578/fork)
- package com.lyncc.netty.idle;  
- 
- import io.netty.channel.ChannelHandler.Sharable;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- import io.netty.util.ReferenceCountUtil;  
- 
- import java.util.Date;  
- 
- @Sharable
- publicclass HeartBeatClientHandler extends ChannelInboundHandlerAdapter {  
- 
- 
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) throws Exception {  
-         System.out.println("激活时间是："+new Date());  
-         System.out.println("HeartBeatClientHandler channelActive");  
-         ctx.fireChannelActive();  
-     }  
- 
- @Override
- publicvoid channelInactive(ChannelHandlerContext ctx) throws Exception {  
-         System.out.println("停止时间是："+new Date());  
-         System.out.println("HeartBeatClientHandler channelInactive");  
-     }  
- 
- 
- @Override
- publicvoid channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {  
-         String message = (String) msg;  
-         System.out.println(message);  
- if (message.equals("Heartbeat")) {  
-             ctx.write("has read message from server");  
-             ctx.flush();  
-         }  
-         ReferenceCountUtil.release(msg);  
-     }  
- }  
好了，到此为止，所有的代码都贴完了，我们做一个简单的测试，按照常理，如果不出任何状况的话，客户端4秒发送心跳，服务端5秒才验证是不会断连的，所以我们在启动之后，关闭服务端，然后再次重启服务端
首先启动服务端，控制台如下：
![](https://img-blog.csdn.net/20160526200020778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
启动客户端，控制台如下：
![](https://img-blog.csdn.net/20160526200045669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
客户端启动之后，服务端的控制台：
![](https://img-blog.csdn.net/20160526200338530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
关闭服务端后，客户端控制台：
![](https://img-blog.csdn.net/20160526200634765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
重启启动服务端：
![](https://img-blog.csdn.net/20160526200701401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
重连成功~

