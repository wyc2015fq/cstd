# netty DelimiterBaseFrameDecoder 使用实例 - 深之JohnChen的专栏 - CSDN博客

2017年12月02日 11:10:23[byxdaz](https://me.csdn.net/byxdaz)阅读数：874


﻿﻿       netty中，通过DelimiterBasedFrameDecoder 特殊分隔符解码器来解决以特殊符号作为消息结束符的TCP黏包问题。DelimiterBasedFrameDecoder是自定义的分隔符解码，构造函数的第一个参数表示单个消息的最大长度，当达到该长度后仍然没有查到分隔符，就抛出TooLongFrameException异常，防止由于异常码流缺失分隔符导致的内存溢出。

服务端代码：

package cc.zychen.com;

import io.netty.bootstrap.ServerBootstrap;

import io.netty.buffer.ByteBuf;

import io.netty.buffer.Unpooled;

import io.netty.channel.ChannelFuture;

import io.netty.channel.ChannelInitializer;

import io.netty.channel.ChannelOption;

import io.netty.channel.EventLoopGroup;

import io.netty.channel.nio.NioEventLoopGroup;

import io.netty.channel.socket.SocketChannel;

import io.netty.channel.socket.nio.NioServerSocketChannel;

import io.netty.handler.codec.DelimiterBasedFrameDecoder;

import io.netty.handler.codec.string.StringDecoder;

import io.netty.handler.logging.LogLevel;

import io.netty.handler.logging.LoggingHandler;

public class EchoServer {

 public void bind(int port) throws Exception {

  EventLoopGroup bossGroup = new NioEventLoopGroup();

  EventLoopGroup workerGroup = new NioEventLoopGroup();

  try{

  ServerBootstrap b = new ServerBootstrap();

  b.group(bossGroup, workerGroup)

  .channel(NioServerSocketChannel.class)

  .option(ChannelOption.SO_BACKLOG, 100)

  .handler(new LoggingHandler(LogLevel.INFO))

  .childHandler(new ChannelInitializer<SocketChannel>() {

   @Override

   protected void initChannel(SocketChannel ch) throws Exception {

    // TODO Auto-generated method stub

    ByteBuf delimiter = Unpooled.copiedBuffer("@_".getBytes());

    ch.pipeline().addLast(new DelimiterBasedFrameDecoder(1024, delimiter));

    ch.pipeline().addLast(new StringDecoder());

    ch.pipeline().addLast(new EchoServerHandler());

   }

  });

  //bind port

  ChannelFuture f = b.bind(port).sync();

  //wait

  f.channel().closeFuture().sync();

  }finally{

   bossGroup.shutdownGracefully();

   workerGroup.shutdownGracefully();

  }

 }

 /**

  * @param args

  */

 public static void main(String[] args) {

  int port = 8080;

  try {

   if(args != null && args.length > 0){

    port = Integer.parseInt(args[0]);

   }

   new EchoServer().bind(port);

  } catch (Exception e) {

   // TODO Auto-generated catch block

   e.printStackTrace();

  }

 }

}

客户端代码：

package cc.zychen.com;

import io.netty.bootstrap.Bootstrap;

import io.netty.buffer.ByteBuf;

import io.netty.buffer.Unpooled;

import io.netty.channel.ChannelFuture;

import io.netty.channel.ChannelInitializer;

import io.netty.channel.ChannelOption;

import io.netty.channel.EventLoopGroup;

import io.netty.channel.nio.NioEventLoopGroup;

import io.netty.channel.socket.SocketChannel;

import io.netty.channel.socket.nio.NioSocketChannel;

import io.netty.channel.udt.nio.NioUdtProvider;

import io.netty.handler.codec.DelimiterBasedFrameDecoder;

import io.netty.handler.codec.string.StringDecoder;

public class EchoClient {

 public void connect(String ip,int port) throws InterruptedException{

  EventLoopGroup group = new NioEventLoopGroup();

  try{

  Bootstrap b = new Bootstrap();

  b.group(group)

  .option(ChannelOption.TCP_NODELAY, true)

  .channel(NioSocketChannel.class)

  .handler(new ChannelInitializer<SocketChannel>() {

   @Override

   protected void initChannel(SocketChannel ch) throws Exception {

    ByteBuf delimiter = Unpooled.copiedBuffer("@_".getBytes());

    ch.pipeline().addLast(new DelimiterBasedFrameDecoder(1024, delimiter));

    ch.pipeline().addLast(new StringDecoder());

    ch.pipeline().addLast(new EchoClientHandler());

   }

  });

  ChannelFuture f = b.connect(ip, port).sync();

  f.channel().closeFuture().sync();

  } finally{

   group.shutdownGracefully();

  }

 }

 /**

  * @param args

  */

 public static void main(String[] args) {

  int port = 8080;

  try {

   if(args != null && args.length > 0){

    port = Integer.parseInt(args[0]);

   }

   new EchoClient().connect("127.0.0.1", port);

  } catch (Exception e) {

   // TODO Auto-generated catch block

   e.printStackTrace();

  }

 }

}

完整代码下载：[http://download.csdn.net/download/byxdaz/10141627](http://download.csdn.net/download/byxdaz/10141627)

﻿﻿

﻿﻿

