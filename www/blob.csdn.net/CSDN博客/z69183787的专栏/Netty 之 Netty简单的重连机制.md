# Netty 之 Netty简单的重连机制 - z69183787的专栏 - CSDN博客
2016年09月22日 16:08:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6799
其实重连机制并不是多么多高深的技术，其实就是一个在客户端做一个简单的判断，如果连接断了，那么就重新调用连接服务端的代码
当然，我们重连的动作肯定是发生在断连之后发生的，我们可以在上篇的心跳机制的基础上，简单地修改一下客户端的启动代码就可以了：
我们在连接断了之后，我们一般会在finally的方法中去释放资源，这边我们应该不去释放资源，我们在finally里面进行重连：
![](https://img-blog.csdn.net/20160514100939148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
整个客户端的代码如下：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51404430#)[copy](http://blog.csdn.net/linuu/article/details/51404430#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1682887/fork)
- package com.lyncc.netty.heartbeats;  
- 
- import java.util.concurrent.TimeUnit;  
- 
- import io.netty.bootstrap.Bootstrap;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.ChannelOption;  
- import io.netty.channel.ChannelPipeline;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.SocketChannel;  
- import io.netty.channel.socket.nio.NioSocketChannel;  
- import io.netty.handler.codec.string.StringDecoder;  
- import io.netty.handler.codec.string.StringEncoder;  
- import io.netty.handler.logging.LogLevel;  
- import io.netty.handler.logging.LoggingHandler;  
- import io.netty.handler.timeout.IdleStateHandler;  
- 
- publicclass HeartBeatsClient {  
- 
- publicvoid connect(int port, String host) throws Exception {  
- // Configure the client.
-         EventLoopGroup group = new NioEventLoopGroup();  
-         ChannelFuture future = null;  
- try {  
-             Bootstrap b = new Bootstrap();  
-             b.group(group)  
-              .channel(NioSocketChannel.class)  
-              .option(ChannelOption.TCP_NODELAY, true)  
-              .handler(new LoggingHandler(LogLevel.INFO))  
-              .handler(new ChannelInitializer<SocketChannel>() {  
- @Override
- publicvoid initChannel(SocketChannel ch) throws Exception {  
-                      ChannelPipeline p = ch.pipeline();  
-                      p.addLast("ping", new IdleStateHandler(0, 4, 0, TimeUnit.SECONDS));  
-                      p.addLast("decoder", new StringDecoder());  
-                      p.addLast("encoder", new StringEncoder());  
-                      p.addLast(new HeartBeatClientHandler());  
-                  }  
-              });  
- 
-             future = b.connect(host, port).sync();  
-             future.channel().closeFuture().sync();  
-         } finally {  
- //          group.shutdownGracefully();
- if (null != future) {  
- if (future.channel() != null && future.channel().isOpen()) {  
-                   future.channel().close();  
-               }  
-           }  
-           System.out.println("准备重连");  
-           connect(port, host);  
-           System.out.println("重连成功");  
-         }  
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
我们再看看服务器端和客户端启动之后的控制台打印信息：
服务器控制台：
![](https://img-blog.csdn.net/20160514101246489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
客户端：
![](https://img-blog.csdn.net/20160514101306270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
好了，这样就可以重连~这只是一个简单的Demo，真实的生产场景用法可能并不是这样的
