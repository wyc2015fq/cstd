# Netty 之 TCP粘包拆包基本解决方案 - z69183787的专栏 - CSDN博客
2016年09月20日 14:49:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1304
上个小节我们浅析了在Netty的使用的时候TCP的粘包和拆包的现象，Netty对此问题提供了相对比较丰富的解决方案
Netty提供了几个常用的解码器，帮助我们解决这些问题，其实上述的粘包和拆包的问题，归根结底的解决方案就是发送端给远程端一个标记，告诉远程端，每个信息的结束标志是什么，这样，远程端获取到数据后，根据跟发送端约束的标志，将接收的信息分切或者合并成我们需要的信息，这样我们就可以获取到正确的信息了
例如，我们刚才的例子中，我们可以在发送的信息中，加一个结束标志，例如两个远程端规定以行来切分数据，那么发送端，就需要在每个信息体的末尾加上行结束的标志，部分代码如下：
修改BaseClientHandler的req的构造：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51338538#)[copy](http://blog.csdn.net/linuu/article/details/51338538#)
- public BaseClientHandler() {  
- //        req = ("BazingaLyncc is learner").getBytes();
-         req = ("In this chapter you general, we recommend Java Concurrency in Practice by Brian Goetz. His book w"
-                 + "ill give We’ve reached an exciting point—in the next chapter we’ll discuss bootstrapping, the process "
-                 + "of configuring and connecting all of Netty’s components to bring your learned about threading models in ge"
-                 + "neral and Netty’s threading model in particular, whose performance and consistency advantages we discuss"
-                 + "ed in detail In this chapter you general, we recommend Java Concurrency in Practice by Brian Goetz. Hi"
-                 + "s book will give We’ve reached an exciting point—in the next chapter we’ll discuss bootstrapping, the"
-                 + " process of configuring and connecting all of Netty’s components to bring your learned about threading "
-                 + "models in general and Netty’s threading model in particular, whose performance and consistency advantag"
-                 + "es we discussed in detailIn this chapter you general, we recommend Java Concurrency in Practice by Bri"
-                 + "an Goetz. His book will give We’ve reached an exciting point—in the next chapter;the counter is: 1 2222"
-                 + "sdsa ddasd asdsadas dsadasdas" + System.getProperty("line.separator")).getBytes();  
-     }  
我们在我们巨长的req中末尾加了System.getProperty("line.separator")，这样相当于给req打了一个标记
打完标记，其实我们这个示例中的server中还不知道是以行为结尾的，所以我们需要修改server的handler链，在inbound链中加一个额外的处理链，判断一下，获取的信息按照行来切分，我们很庆幸，这样枯燥的代码Netty已经帮我们完美地完成了，Netty提供了一个LineBasedFrameDecoder这个类，顾名思义，这个类名字中有decoder，说明是一个解码器，我们再看看它的详细声明：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51338538#)[copy](http://blog.csdn.net/linuu/article/details/51338538#)
- /**
-  * A decoder that splits the received {@link ByteBuf}s on line endings.
-  * <p>
-  * Both {@code "\n"} and {@code "\r\n"} are handled.
-  * For a more general delimiter-based decoder, see {@link DelimiterBasedFrameDecoder}.
-  */
- publicclass LineBasedFrameDecoder extends ByteToMessageDecoder {  
- 
- /** Maximum length of a frame we're willing to decode.  */
- privatefinalint maxLength;  
- /** Whether or not to throw an exception as soon as we exceed maxLength. */
- privatefinalboolean failFast;  
- privatefinalboolean stripDelimiter;  
- 
- /** True if we're discarding input because we're already over maxLength.  */
- privateboolean discarding;  
- privateint discardedBytes;  
它是继承ByteToMessageDecoder的，是将byte类型转化成Message的，所以我们应该将这个解码器放在inbound处理器链的第一个，所以我们修改一下Server端的启动代码：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51338538#)[copy](http://blog.csdn.net/linuu/article/details/51338538#)
- package com.lyncc.netty.stickpackage.myself;  
- 
- import io.netty.bootstrap.ServerBootstrap;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.ChannelOption;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.SocketChannel;  
- import io.netty.channel.socket.nio.NioServerSocketChannel;  
- import io.netty.handler.codec.LineBasedFrameDecoder;  
- import io.netty.handler.codec.string.StringDecoder;  
- 
- import java.net.InetSocketAddress;  
- 
- publicclass BaseServer {  
- 
- privateint port;  
- 
- public BaseServer(int port) {  
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
-                             ch.pipeline().addLast(new LineBasedFrameDecoder(2048));  
-                             ch.pipeline().addLast(new StringDecoder());  
-                             ch.pipeline().addLast(new BaseServerHandler());  
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
- new BaseServer(port).start();  
-     }  
- }  
这样，我们只是在initChannel方法中增加了一个LineBasedFrameDecoder这个类，其中2048是规定一行数据最大的字节数
我们再次运行，我们再看看效果：
![](https://img-blog.csdn.net/20160507164727052?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到客户端发送的两次msg，被服务器端成功地两次接收了，我们要的效果达到了
我们将LineBasedFrameDecoder中的2048参数，缩小一半，变成1024，我们再看看效果：
![](https://img-blog.csdn.net/20160507164936205?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
出现了异常，这个异常时TooLongFrameException,这个异常在Netty in Action中介绍过，帧的大小太大，在我们这个场景中，就是我们发送的一行信息大小是1076，大于了我们规定的1024所以报错了
我们再解决另一个粘包的问题，我们可以看到上节中介绍的那个粘包案例中，我们发送了100次的信息“BazingaLyncc is learner”，这个案例很特殊，这个信息是一个特长的数据，字节长度是23，所以我们可以使用Netty为我们提供的FixedLengthFrameDecoder这个解码器，看到这个名字就明白了大半，定长数据帧的解码器，所以我们修改一下代码：
BaseClientHandler：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51338538#)[copy](http://blog.csdn.net/linuu/article/details/51338538#)
- package com.lyncc.netty.stickpackage.myself;  
- 
- import io.netty.buffer.ByteBuf;  
- import io.netty.buffer.Unpooled;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- publicclass BaseClientHandler extends ChannelInboundHandlerAdapter{  
- 
- privatebyte[] req;  
- 
- public BaseClientHandler() {  
-         req = ("BazingaLyncc is learner").getBytes();  
- //        req = ("In this chapter you general, we recommend Java Concurrency in Practice by Brian Goetz. His book w"
- //                + "ill give We’ve reached an exciting point—in the next chapter we’ll discuss bootstrapping, the process "
- //                + "of configuring and connecting all of Netty’s components to bring your learned about threading models in ge"
- //                + "neral and Netty’s threading model in particular, whose performance and consistency advantages we discuss"
- //                + "ed in detail In this chapter you general, we recommend Java Concurrency in Practice by Brian Goetz. Hi"
- //                + "s book will give We’ve reached an exciting point—in the next chapter we’ll discuss bootstrapping, the"
- //                + " process of configuring and connecting all of Netty’s components to bring your learned about threading "
- //                + "models in general and Netty’s threading model in particular, whose performance and consistency advantag"
- //                + "es we discussed in detailIn this chapter you general, we recommend Java Concurrency in Practice by Bri"
- //                + "an Goetz. His book will give We’ve reached an exciting point—in the next chapter;the counter is: 1 2222"
- //                + "sdsa ddasd asdsadas dsadasdas" + System.getProperty("line.separator")).getBytes();
-     }  
- 
- 
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) throws Exception {  
-         ByteBuf message = null;  
- for (int i = 0; i < 100; i++) {  
-             message = Unpooled.buffer(req.length);  
-             message.writeBytes(req);  
-             ctx.writeAndFlush(message);  
-         }  
- //        message = Unpooled.buffer(req.length);
- //        message.writeBytes(req);
- //        ctx.writeAndFlush(message);
- //        message = Unpooled.buffer(req.length);
- //        message.writeBytes(req);
- //        ctx.writeAndFlush(message);
-     }  
- 
- @Override
- publicvoid exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {  
-         ctx.close();  
-     }  
- 
- }  
BaseServer：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51338538#)[copy](http://blog.csdn.net/linuu/article/details/51338538#)
- package com.lyncc.netty.stickpackage.myself;  
- 
- import io.netty.bootstrap.ServerBootstrap;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.ChannelOption;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.SocketChannel;  
- import io.netty.channel.socket.nio.NioServerSocketChannel;  
- import io.netty.handler.codec.FixedLengthFrameDecoder;  
- import io.netty.handler.codec.string.StringDecoder;  
- 
- import java.net.InetSocketAddress;  
- 
- publicclass BaseServer {  
- 
- privateint port;  
- 
- public BaseServer(int port) {  
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
-                             ch.pipeline().addLast(new FixedLengthFrameDecoder(23));  
-                             ch.pipeline().addLast(new StringDecoder());  
-                             ch.pipeline().addLast(new BaseServerHandler());  
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
- new BaseServer(port).start();  
-     }  
- }  
我们就是在channelhandler链中，加入了FixedLengthFrameDecoder，且参数是23，告诉Netty，获取的帧数据有23个字节就切分一次
运行结果：
![](https://img-blog.csdn.net/20160507165814984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160507165826338?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看见，我们获取到了我们想要的效果
当然Netty还提供了一些其他的解码器，有他们自己的使用场景，例如有按照某个固定字符切分的DelimiterBasedFrameDecoder的解码器
我们再次修改代码：
BaseClientHandler.java
**[html]**[view
 plain](http://blog.csdn.net/linuu/article/details/51338538#)[copy](http://blog.csdn.net/linuu/article/details/51338538#)
- package com.lyncc.netty.stickpackage.myself;  
- 
- import io.netty.buffer.ByteBuf;  
- import io.netty.buffer.Unpooled;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- public class BaseClientHandler extends ChannelInboundHandlerAdapter{  
- 
-     private byte[] req;  
- 
-     public BaseClientHandler() {  
- //        req = ("BazingaLyncc is learner").getBytes();  
- req = ("In this chapter you general, we recommend Java Concurrency in Practice by Brian Goetz. $$__ His book w"  
-                 + "ill give We’ve reached an exciting point—in the next chapter we’ll $$__ discuss bootstrapping, the process "  
-                 + "of configuring and connecting all of Netty’s components to bring $$__ your learned about threading models in ge"  
-                 + "neral and Netty’s threading model in particular, whose performance $$__ and consistency advantages we discuss"  
-                 + "ed in detail In this chapter you general, we recommend Java  $$__Concurrency in Practice by Brian Goetz. Hi"  
-                 + "s book will give We’ve reached an exciting point—in the next $$__ chapter we’ll discuss bootstrapping, the"  
-                 + " process of configuring and connecting all of Netty’s components $$__ to bring your learned about threading "  
-                 + "models in general and Netty’s threading model in particular, $$__ whose performance and consistency advantag"  
-                 + "es we discussed in detailIn this chapter you general, $$__ we recommend Java Concurrency in Practice by Bri"  
-                 + "an Goetz. His book will give We’ve reached an exciting $$__ point—in the next chapter;the counter is: 1 2222"  
-                 + "sdsa ddasd asdsadas dsadasdas" + System.getProperty("line.separator")).getBytes();  
-     }  
- 
- 
-     @Override  
-     public void channelActive(ChannelHandlerContext ctx) throws Exception {  
-         ByteBuf message = null;  
- //        for (int i = 0; i <100; i++) {  
- //            message = Unpooled.buffer(req.length);  
- //            message.writeBytes(req);  
- //            ctx.writeAndFlush(message);  
- //        }  
- message = Unpooled.buffer(req.length);  
-         message.writeBytes(req);  
-         ctx.writeAndFlush(message);  
- message = Unpooled.buffer(req.length);  
-         message.writeBytes(req);  
-         ctx.writeAndFlush(message);  
-     }  
- 
-     @Override  
-     public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {  
-         ctx.close();  
-     }  
- 
- }  
我们在req的字符串中增加了“$$__”这样的切割符，然后再Server中照例增加一个DelimiterBasedFrameDecoder，来切割字符串：
**[html]**[view
 plain](http://blog.csdn.net/linuu/article/details/51338538#)[copy](http://blog.csdn.net/linuu/article/details/51338538#)
- ServerBootstrap sbs = new ServerBootstrap().group(bossGroup,workerGroup).channel(NioServerSocketChannel.class).localAddress(new InetSocketAddress(port))  
-                     .childHandler(new ChannelInitializer<SocketChannel>() {  
- 
-                         protected void initChannel(SocketChannel ch) throws Exception {  
-                             ch.pipeline().addLast(new DelimiterBasedFrameDecoder(1024,Unpooled.copiedBuffer("$$__".getBytes())));  
-                             ch.pipeline().addLast(new StringDecoder());  
-                             ch.pipeline().addLast(new BaseServerHandler());  
-                         };  
- 
-                     }).option(ChannelOption.SO_BACKLOG, 128)     
-                     .childOption(ChannelOption.SO_KEEPALIVE, true);  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们在initChannel中第一个inbound中增加了DelimiterBasedFrameDecoder，且规定切割符就是“$$__”，这样就能正常切割了，我们看看运行效果：
![](https://img-blog.csdn.net/20160507170753263?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到被分了20次读取，我们可以这样理解，客户端发送了2次req字节，每个req中有10个“$$__”，这样就是第11次切割的时候其实发送了粘包，第一个req中末尾部分和第二次的头部粘在了一起，作为第11部分的内容
而最后一部分的内容因为没有"$$__"切割，所以没有打印在控制台上~
其实这类的Handler还是相对比较简单的，真实的生产环境这些decoder只是作为比较基本的切分类，但是这些decoder还是很好用的~
希望讲的对您有所帮助~END~
