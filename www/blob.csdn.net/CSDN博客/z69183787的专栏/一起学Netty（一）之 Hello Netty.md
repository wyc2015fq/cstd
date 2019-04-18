# 一起学Netty（一）之 Hello Netty - z69183787的专栏 - CSDN博客
2016年10月31日 10:21:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1604
案例说明：客户端发送一个信息到服务器端，服务器端将信息原样返回
maven的依赖，我们如下：
**[html]**[view
 plain](http://blog.csdn.net/linuu/article/details/51306480#)[copy](http://blog.csdn.net/linuu/article/details/51306480#)
- <dependencies>
- <dependency>
- <groupId>junit</groupId>
- <artifactId>junit</artifactId>
- <version>4.10</version>
- <scope>test</scope>
- </dependency>
- <dependency>
- <groupId>io.netty</groupId>
- <artifactId>netty-all</artifactId>
- <version>4.0.21.Final</version>
- </dependency>
- </dependencies>

我们先写Netty器端的ChannelHandler，我们命名Netty的服务端的处理器为：HelloWorldServerHandler，继承于ChannelInboundHandlerAdapter，《Netty in Action》中介绍过，我们不需要使每一个inboundChannel继承于ChannelInboundHandler，这样会需要我们实现ChannelInboundHandler中的所有接口，在一般的channel中我们没有必要这样做，这样只会增加我们的额外的工作量，我们只需要继承ChannelInboundHandlerAdapter，继承它的适配就可以了，我们需要实现几个特别重要的方法，例如读取的方法channelRead和异常处理的方法exceptionCaught，源码如下：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51306480#)[copy](http://blog.csdn.net/linuu/article/details/51306480#)
- package com.lyncc.netty.hello;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- publicclass HelloWorldServerHandler extends ChannelInboundHandlerAdapter{  
- 
- 
- @Override
- publicvoid channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {  
-         System.out.println("server channelRead..");  
-         System.out.println(ctx.channel().remoteAddress()+"->Server :"+ msg.toString());  
-         ctx.write("server write"+msg);  
-         ctx.flush();  
-     }  
- 
- @Override
- publicvoid exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {  
-         cause.printStackTrace();  
-         ctx.close();  
-     }  
- 
- }  
接下来我们就要写Netty的服务器端了，书中写的很清楚，我们需要写一个ServerBootstrap，ServerBootstrap的写法是现在比较流行的流式编程法，我们需要指定它的transports，是NIO还是OIO,还需要指定端口号，最最重要的是安装server端的处理器，也就是我们之前写的HelloWorldServerHandler，还有一些Option的配置，这里只是用到，以后我们一起分析这些Option的作用，写server端还有一点需要注意，就是需要关闭连接，释放线程资源，源码如下：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51306480#)[copy](http://blog.csdn.net/linuu/article/details/51306480#)
- package com.lyncc.netty.hello;  
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
- 
- import java.net.InetSocketAddress;  
- 
- publicclass HelloWorldServer {  
- 
- privateint port;  
- 
- public HelloWorldServer(int port) {  
- this.port = port;  
-     }  
- 
- publicvoid start(){  
-         EventLoopGroup bossGroup = new NioEventLoopGroup(1);  
-         EventLoopGroup workerGroup = new NioEventLoopGroup();  
- try {  
-             ServerBootstrap sbs = new ServerBootstrap().group(bossGroup,workerGroup).channel(NioServerSocketChannel.class).localAddress(new InetSocketAddress(port))  
-                     .childHandler(new ChannelInitializer<SocketChannel>() {  
- 
- protectedvoid initChannel(SocketChannel ch) throws Exception {  
- //                            ch.pipeline().addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));
-                             ch.pipeline().addLast("decoder", new StringDecoder());  
-                             ch.pipeline().addLast("encoder", new StringEncoder());  
-                             ch.pipeline().addLast(new HelloWorldServerHandler());  
-                         };  
- 
-                     }).option(ChannelOption.SO_BACKLOG, 128)     
-                     .childOption(ChannelOption.SO_KEEPALIVE, true);  
- // 绑定端口，开始接收进来的连接
-              ChannelFuture future = sbs.bind(port).sync();    
- 
-              System.out.println("Server start listen at " + port );  
-              future.channel().closeFuture().sync();  
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
- new HelloWorldServer(port).start();  
-     }  
- }  
如法炮制，客户端的handler，我们依旧命名为HelloWorldClientHandler，也是继承于ChannelInboundHandlerAdapter
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51306480#)[copy](http://blog.csdn.net/linuu/article/details/51306480#)
- package com.lyncc.netty.hello;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- publicclass HelloWorldClientHandler extends ChannelInboundHandlerAdapter{  
- 
- 
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) {  
-           System.out.println("HelloWorldClientHandler Active");  
-       }  
- 
- @Override
- publicvoid channelRead(ChannelHandlerContext ctx, Object msg) {  
-          System.out.println("HelloWorldClientHandler read Message:"+msg);  
-       }  
- 
- 
- @Override
- publicvoid exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {  
-          cause.printStackTrace();  
-          ctx.close();  
-       }  
- 
- }  
客户端我们需要写一个Bootstrap，也需要指定客户端的处理器，我们这里在客户端和服务器端都加了编码和解码两个String类型的译码器，这样的例子也算比较完整的hello world的例子了，完整的代码如下：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51306480#)[copy](http://blog.csdn.net/linuu/article/details/51306480#)
- package com.lyncc.netty.hello;  
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
- 
- publicclass HelloWorldClient {  
- 
- staticfinal String HOST = System.getProperty("host", "127.0.0.1");  
- staticfinalint PORT = Integer.parseInt(System.getProperty("port", "8080"));  
- staticfinalint SIZE = Integer.parseInt(System.getProperty("size", "256"));  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- 
- // Configure the client.
-         EventLoopGroup group = new NioEventLoopGroup();  
- try {  
-             Bootstrap b = new Bootstrap();  
-             b.group(group)  
-              .channel(NioSocketChannel.class)  
-              .option(ChannelOption.TCP_NODELAY, true)  
-              .handler(new ChannelInitializer<SocketChannel>() {  
- @Override
- publicvoid initChannel(SocketChannel ch) throws Exception {  
-                      ChannelPipeline p = ch.pipeline();  
-                      p.addLast("decoder", new StringDecoder());  
-                      p.addLast("encoder", new StringEncoder());  
-                      p.addLast(new HelloWorldClientHandler());  
-                  }  
-              });  
- 
-             ChannelFuture future = b.connect(HOST, PORT).sync();  
-             future.channel().writeAndFlush("Hello Netty Server ,I am a common client");  
-             future.channel().closeFuture().sync();  
-         } finally {  
-             group.shutdownGracefully();  
-         }  
-     }  
- 
- }  
我们打开HelloWorldServer的代码，运行main函数，eclipse的控制台打印信息如下：
![](https://img-blog.csdn.net/20160503190213302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
说明服务器已经成功运行，在8080端口监听了
我们再打开HelloWorldClient代码，同样运行main函数。控制台打印信息如下：
![](https://img-blog.csdn.net/20160503190319943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第一行是HelloWorldClientHandler该处理器的channelActive的方法打印的输出，第二行是channelRead方法的输出，这个输出是服务器端写入的，返回给客户端的
我们再回头看服务器端，此时服务器的HelloWorldServerHandler的channelRead的方法应该打印从客户端收到的信息了
![](https://img-blog.csdn.net/20160503190540288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
好了，到此为止，Netty的服务器端和客户端就完全搭建完毕了，我们做到了客户端发送信息，服务器端接收且把收到的信息原样返回给客户端了
