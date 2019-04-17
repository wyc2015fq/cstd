# Netty通信框架Java实现小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月29日 16:14:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4266








                
1、Netty介绍

   Netty通俗地说就是一套Socket通信框架，提供异步的、事件驱动的网络应用程序框架和工具，可快速开发高性能、高可靠性的网络服务器和客户端程序


2、Netty的特性

   1）设计

      统一的API，适用于不同的协议（阻塞和非阻塞）

      基于灵活、可扩展的事件驱动模型

      高度可定制的线程模型

      可靠的无连接数据Socket支持（UDP）

   2）性能

      更好的吞吐量，低延迟

      更省资源

      尽量减少不必要的内存拷贝

   3）安全

      完整的SSL/TLS和STARTTLS的支持

      能在Applet与Android的限制环境运行良好

   4）健壮性

      不再因过快、过慢或超负载连接导致OutOfMemoryError

      不再有在高速网络环境下NIO读写频率不一致的问题

   5）易用

      完善的JavaDoc，用户指南和样例

      简洁简单

3、下载Jar包

    http://netty.io/downloads.html 下载

    引入netty-all-4.1.4.Final.jar包到工程



4、Server端代码示例：

```java
package cn.netty;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;

public class NettyServer {
	
	private int port;
	public NettyServer(int port) {
	    this.port = port;
	    bind();
	}
	
	private void bind() {
		
		EventLoopGroup boss = new NioEventLoopGroup();
	    EventLoopGroup worker = new NioEventLoopGroup();

	    try {
	    	
	    	ServerBootstrap bootstrap = new ServerBootstrap();
	    	bootstrap.group(boss, worker);
	    	bootstrap.channel(NioServerSocketChannel.class);
	    	bootstrap.option(ChannelOption.SO_BACKLOG, 1024); //连接数
	    	bootstrap.option(ChannelOption.TCP_NODELAY, true);  //不延迟，消息立即发送
	    	bootstrap.childOption(ChannelOption.SO_KEEPALIVE, true); //长连接
	    	bootstrap.childHandler(new ChannelInitializer<SocketChannel>() {
	        @Override
	        protected void initChannel(SocketChannel socketChannel) throws Exception {
	        	ChannelPipeline p = socketChannel.pipeline();
	        	p.addLast(new NettyServerHandler());
	        } });
	    	
	    	ChannelFuture f = bootstrap.bind(port).sync();
	    	if (f.isSuccess()) {
	    		System.out.println("启动Netty服务成功，端口号：" + this.port);
	    	}
	    	//Wait until the server socket is closed.
	    	f.channel().closeFuture().sync();
	    } catch (Exception e) {
	    	System.out.println("启动Netty服务异常，异常信息：" + e.getMessage());
	    	e.printStackTrace();
	    } finally {
	    	boss.shutdownGracefully();
	    	worker.shutdownGracefully();
	    }
	}

	public static void main(String[] args) throws InterruptedException {
	    
		NettyServer server= new NettyServer(9999);

	  }
}
```

```java
package cn.netty;

import java.io.UnsupportedEncodingException;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

public class NettyServerHandler extends ChannelInboundHandlerAdapter {

	  @Override
	  public void channelRead(ChannelHandlerContext ctx, Object msg) {
	    ByteBuf buf = (ByteBuf) msg;	    
	    String recieved = getMessage(buf);
	    System.out.println("服务器接收到消息：" + recieved);	    
	    try {
	    	ctx.writeAndFlush(getSendByteBuf("Message"));
	    } catch (UnsupportedEncodingException e) {
	    	e.printStackTrace();
	    }
	  }

	  /*
	   * 从ByteBuf中获取信息 使用UTF-8编码返回
	   */
	  private String getMessage(ByteBuf buf) {

	    byte[] con = new byte[buf.readableBytes()];
	    buf.readBytes(con);
	    try {
	    	return new String(con, "UTF-8");
	    } catch (UnsupportedEncodingException e) {
	    	e.printStackTrace();
	    	return null;
	    }
	  }
	  
	  private ByteBuf getSendByteBuf(String message) throws UnsupportedEncodingException {
	    byte[] req = message.getBytes("UTF-8");
	    ByteBuf pingMessage = Unpooled.buffer();
	    pingMessage.writeBytes(req);
	    return pingMessage;
	  }
}
```
5、Client端代码示例：



```java
package cn.netty;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;

public class NettyClient {
	private int port;//服务器端口号
	private String host;//服务器IP
	
	public NettyClient(int port, String host) throws InterruptedException {
		this.port = port;
	    this.host = host;
	    start();
	}
	
	private void start() throws InterruptedException {
		
		EventLoopGroup eventLoopGroup = new NioEventLoopGroup();
		try {
			Bootstrap bootstrap = new Bootstrap();
			bootstrap.channel(NioSocketChannel.class);
			bootstrap.option(ChannelOption.SO_KEEPALIVE, true);
			bootstrap.group(eventLoopGroup);
			bootstrap.remoteAddress(host, port);
			bootstrap.handler(new ChannelInitializer<SocketChannel>() {
	        @Override
	        protected void initChannel(SocketChannel socketChannel) throws Exception {		
	        	socketChannel.pipeline().addLast(new NettyClientHandler());
	        }});
	      ChannelFuture future = bootstrap.connect(host, port).sync();
	      if (future.isSuccess()) {
	    	  //SocketChannel socketChannel = (SocketChannel) future.channel();
	    	  System.out.println("----------------connect server success----------------");
	      }
	      // Wait until the connection is closed.
	      future.channel().closeFuture().sync();
	   } finally {
		   eventLoopGroup.shutdownGracefully();
	   }
	}

	  public static void main(String[] args) throws InterruptedException {
	    
	    NettyClient client = new NettyClient(9999,"localhost");

	  }
}
```


```java
package cn.netty;

import java.io.UnsupportedEncodingException;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

public class NettyClientHandler extends ChannelInboundHandlerAdapter  {
	private  ByteBuf firstMessage;
	
	@Override
	public void channelActive(ChannelHandlerContext ctx) throws Exception {
		byte[] data = "服务器，给我一个Message".getBytes();
	    firstMessage=Unpooled.buffer();
	    firstMessage.writeBytes(data);
	    ctx.writeAndFlush(firstMessage);
    }
	
	@Override
	public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
		ByteBuf buf = (ByteBuf) msg;
		String rev = getMessage(buf);
		System.out.println("客户端收到服务器数据:" + rev);
	}
	
	private String getMessage(ByteBuf buf) {
		
		byte[] con = new byte[buf.readableBytes()];
	    buf.readBytes(con);
	    try {
	    	return new String(con, "UTF-8");
	    } catch (UnsupportedEncodingException e) {
	    	e.printStackTrace();
	    	return null;
	    }
	}
	
}
```


6、总结：server和client端主要是建立连接，数据接收和发送主要是继承ChannelInboundHandlerAdapter类，重载channelRead等函数。
            


