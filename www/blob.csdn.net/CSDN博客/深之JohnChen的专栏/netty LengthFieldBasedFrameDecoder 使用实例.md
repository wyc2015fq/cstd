# netty LengthFieldBasedFrameDecoder 使用实例 - 深之JohnChen的专栏 - CSDN博客

2017年12月02日 11:23:13[byxdaz](https://me.csdn.net/byxdaz)阅读数：1391


﻿﻿       netty中，通过LengthFieldBasedFrameDecoder 自定义长度解码器解决TCP黏包问题。大多数的协议在协议头中都会携带长度字段，用于标识消息体或则整包消息的长度。LengthFieldBasedFrameDecoder通过指定长度来标识整包消息，这样就可以自动的处理黏包和半包消息，只要传入正确的参数，就可以轻松解决“读半包”的问题。

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

import io.netty.handler.codec.LengthFieldBasedFrameDecoder;

import io.netty.handler.codec.LengthFieldPrepender;

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

    ch.pipeline().addLast(new LengthFieldBasedFrameDecoder(65535,2,4,0,2));

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

package cc.zychen.com;

import java.nio.charset.Charset;

import io.netty.buffer.ByteBuf;

import io.netty.buffer.Unpooled;

import io.netty.channel.ChannelHandlerContext;

import io.netty.channel.ChannelInboundHandlerAdapter;

public class EchoServerHandler extends ChannelInboundHandlerAdapter {

 private int counter = 0;

 @Override

 public void channelRead(ChannelHandlerContext ctx, Object msg)

   throws Exception {

  ByteBuf body = (ByteBuf)msg;

  if(body.readableBytes() <= 0){

   ctx.fireChannelRead(msg);

  }

  CCMessageHeader recHd = new CCMessageHeader();

  int nLength = body.readInt();

  int nType = body.readInt();

  int nDataSize = body.readableBytes();

  byte [] aa = new byte[nDataSize];

  body.readBytes(aa);

  String myMsg = new String(aa,Charset.forName("utf-8"));

  System.out.println("this is "+ ++ counter +" times receive client:[" + myMsg +"]" +" Type["+ nType +"]");

  CCMessageHeader hd = new CCMessageHeader();

  hd.setType(2);

  hd.setData("server data...");

  ByteBuf echo = Unpooled.directBuffer();

  echo.writeBytes(hd.getMessageFlag());

  echo.writeInt(hd.getLength());

  echo.writeInt(hd.getType());

  echo.writeCharSequence(hd.getData(), Charset.forName("utf-8"));

  ctx.writeAndFlush(echo);

 }

 @Override

 public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {

  ctx.flush();

 }

 @Override

 public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause)

   throws Exception {

  cause.printStackTrace();

  ctx.close();

 }

}

package cc.zychen.com;

public class CCMessageHeader {

 protected byte[] messageFlag = new byte[2];

 protected int length;

 protected int type;

 protected String data;

 CCMessageHeader(){

  messageFlag[0] = (byte) 0xaa;

  messageFlag[1] = (byte) 0xbb;

 }

 public byte[] getMessageFlag() {

  return messageFlag;

 }

 public void setMessageFlag(byte[] messageFlag) {

  this.messageFlag = messageFlag;

 }

 public int getType() {

  return type;

 }

 public void setType(int type) {

  this.type = type;

 }

 public int getLength() {

  return length;

 }

 public void setLength(int length) {

  this.length = length;

 }

 public String getData() {

  return data;

 }

 public void setData(String data) {

  this.data = data;

  length = data.length() + 4;

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

import io.netty.handler.codec.DelimiterBasedFrameDecoder;

import io.netty.handler.codec.LengthFieldBasedFrameDecoder;

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

    ch.pipeline().addLast(new LengthFieldBasedFrameDecoder(65535,2,4,0,2));

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

package cc.zychen.com;

import java.nio.charset.Charset;

import io.netty.buffer.ByteBuf;

import io.netty.buffer.Unpooled;

import io.netty.channel.ChannelHandlerContext;

import io.netty.channel.ChannelInboundHandlerAdapter;

public class EchoClientHandler extends ChannelInboundHandlerAdapter {

 private int counter = 0;

 static final String ECHO_REQ = "Hi,liujunliang. Welcome to Netty.@_";

 @Override

 public void channelRead(ChannelHandlerContext ctx, Object msg)

   throws Exception {

  ByteBuf body = (ByteBuf)msg;

  CCMessageHeader recHd = new CCMessageHeader();

  int nLength = body.readInt();

  int nType = body.readInt();

  int nDataSize = body.readableBytes();

  byte [] aa = new byte[nDataSize];

  body.readBytes(aa);

  String myMsg = new String(aa,Charset.forName("utf-8"));

  System.out.println("this is "+ ++ counter +" times receive server:[" + myMsg +"]" +" Type["+ nType +"]");

 }

 @Override

 public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause)

   throws Exception {

  cause.printStackTrace();

  ctx.close();

 }

 @Override

 public void channelActive(ChannelHandlerContext ctx) throws Exception {

  CCMessageHeader hd = new CCMessageHeader();

  hd.setType(1);

  for(int n=0; n<10; n++){

   String strData = String.format("client data%d...", n+1);

   hd.setData(strData);

   ByteBuf echo = Unpooled.directBuffer();

   echo.writeBytes(hd.getMessageFlag());

   echo.writeInt(hd.getLength());

   echo.writeInt(hd.getType());

   echo.writeCharSequence(hd.getData(), Charset.forName("utf-8"));

   ctx.write(echo);

  }

  ctx.flush();

 }

 @Override

 public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {

  ctx.flush();

 }

}

package cc.zychen.com;

public class CCMessageHeader {

 protected byte[] messageFlag = new byte[2];

 protected int length;

 protected int type;

 protected String data;

 CCMessageHeader(){

  messageFlag[0] = (byte) 0xaa;

  messageFlag[1] = (byte) 0xbb;

 }

 public byte[] getMessageFlag() {

  return messageFlag;

 }

 public void setMessageFlag(byte[] messageFlag) {

  this.messageFlag = messageFlag;

 }

 public int getType() {

  return type;

 }

 public void setType(int type) {

  this.type = type;

 }

 public int getLength() {

  return length;

 }

 public void setLength(int length) {

  this.length = length;

 }

 public String getData() {

  return data;

 }

 public void setData(String data) {

  this.data = data;

  length = data.length() + 4;

 }

}

完整代码下载：[http://download.csdn.net/download/byxdaz/10141694](http://download.csdn.net/download/byxdaz/10141694)

﻿﻿

