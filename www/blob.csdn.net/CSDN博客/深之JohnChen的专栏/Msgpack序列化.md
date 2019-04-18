# Msgpack序列化 - 深之JohnChen的专栏 - CSDN博客

2017年12月02日 11:45:30[byxdaz](https://me.csdn.net/byxdaz)阅读数：6870


﻿﻿

Msgpack是一个基于二进制高效的对象序列化Library用于跨语言通信。它可以像JSON那样，在许多种语言之间交换结构对象；但是它比JSON更快速也更轻巧。 支持Python、Ruby、Java、C/C++、Javascript等众多语言。

由于某个POJO对象里含有Map<Integer, Object>类型的成员，在使用msgpack进行序列化和反序列化时，出现了一些奇怪的现象，比如反序列化出来的Map在进行get时无法表现出正常的操作，虽然Debug时看到内存里确实有数据。后来简单分析了一下发现，在没有指定模板类型时，msgpack反序列化出来的Map实际上是其Jar包内自己封装的一个Map实现类，所以就会出现上述的情况，要解决这个问题，就需要在反序列化解析时，给msgpack明确指定模板类型。

msgpack的内置提供了很多模板，其中就有MapTemplate，这是一个泛型类，在构造时需要指定key和val的模板类型，而msgpack并没有内置Object的模板，所以当如果需要序列化和反序列化一个Value是Object的Map时，就会出现问题。因此我们需要自己来实现一个Object的模板，代码如下:

public class ObjectTemplate extends AbstractTemplate<Object> {

private ObjectTemplate() {

}

static public ObjectTemplate getInstance() {

    return instance;

}

static final ObjectTemplate instance = new ObjectTemplate();

@Override

public void write(Packer pk, Object v, boolean required) throws IOException {

    if (v == null) {

        if (required) {

            throw new MessageTypeException("Attempted to write null");

        }

        pk.writeNil();

        return;

    }

    pk.write(v);

}

@Override

public Object read(Unpacker u, Object to, boolean required) throws IOException {

    if (!required && u.trySkipNil()) {

        return null;

    }

    return toObject(u.readValue());

}

private static Object toObject(Value value) throws IOException {

    Converter conv = new Converter(value);

    if (value.isNilValue()) { // null

        return null;

    } else if (value.isRawValue()) { // byte[] or String or maybe Date?

        // deserialize value to String object

        RawValue v = value.asRawValue();

        return conv.read(Templates.TString);

    } else if (value.isBooleanValue()) { // boolean

        return conv.read(Templates.TBoolean);

    } else if (value.isIntegerValue()) { // int or long or BigInteger

        // deserialize value to int

        IntegerValue v = value.asIntegerValue();

        return conv.read(Templates.TInteger);

    } else if (value.isFloatValue()) { // float or double

        // deserialize value to double

        FloatValue v = value.asFloatValue();

        return conv.read(Templates.TDouble);

    } else if (value.isArrayValue()) { // List or Set

        // deserialize value to List object

        ArrayValue v = value.asArrayValue();

        List<Object> ret = new ArrayList<Object>(v.size());

        for (Value elementValue : v) {

            ret.add(toObject(elementValue));

        }

        return ret;

    } else if (value.isMapValue()) { // Map

        MapValue v = value.asMapValue();

        Map map = new HashMap<>(v.size());

        for (Map.Entry<Value, Value> entry : v.entrySet()) {

            Value key = entry.getKey();

            Value val = entry.getValue();

            map.put(toObject(key), toObject(val));

        }

        return map;

    } else {

        throw new RuntimeException("fatal error");

    }

}

}

netty中使用msgpack序列化实例

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

    ch.pipeline().addLast(new LengthFieldBasedFrameDecoder(65535,0,4,0,4));

    ch.pipeline().addLast(new MsgPackDecoder());

    ch.pipeline().addLast(new LengthFieldPrepender(4,false));

    ch.pipeline().addLast(new MsgPackEncoder());

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

import java.util.ArrayList;

import java.util.Iterator;

import java.util.List;

import org.msgpack.MessagePack;

import org.msgpack.template.Template;

import org.msgpack.template.Templates;

import org.msgpack.type.ArrayValue;

import org.msgpack.type.Value;

import io.netty.buffer.ByteBuf;

import io.netty.buffer.Unpooled;

import io.netty.channel.ChannelHandlerContext;

import io.netty.channel.ChannelInboundHandlerAdapter;

import io.netty.util.ReferenceCountUtil;

public class EchoServerHandler extends ChannelInboundHandlerAdapter {

 private int counter = 0;

 @Override

 public void channelRead(ChannelHandlerContext ctx, Object msg)

   throws Exception {

  @SuppressWarnings("unchecked")

  List<Value> u = (List<Value>)msg;

  System.out.println(u);

  @SuppressWarnings("deprecation")

  UserInfo t = MessagePack.unpack(MessagePack.pack(msg), UserInfo.class);

  System.out.println("this is "+ ++ counter +" times receive client:name[" + t.getName() +"]" +" age["+ t.getAge() +"]");

  UserInfo usInfo = new UserInfo();

  usInfo.setName("bbbbbbbbbb");

  usInfo.setAge(89);

  ctx.writeAndFlush(usInfo);

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

import java.util.List;

import org.msgpack.MessagePack;

import org.msgpack.template.Templates;

import org.msgpack.type.Value;

import io.netty.buffer.ByteBuf;

import io.netty.channel.ChannelHandlerContext;

import io.netty.handler.codec.MessageToMessageDecoder;

public class MsgPackDecoder extends MessageToMessageDecoder<ByteBuf> {

 @Override

 protected void decode(ChannelHandlerContext arg0, ByteBuf arg1,

   List<Object> arg2) throws Exception {

  final byte[] array;

  int length = arg1.readableBytes();

  array = new byte[length];

  arg1.getBytes(arg1.readerIndex(),array,0,length);

  MessagePack mpPack = new MessagePack();

  Value v = mpPack.read(array,Templates.TValue);

  arg2.add(v);

 }

}

package cc.zychen.com;

import org.msgpack.MessagePack;

import io.netty.buffer.ByteBuf;

import io.netty.channel.ChannelHandlerContext;

import io.netty.handler.codec.MessageToByteEncoder;

public class MsgPackEncoder extends MessageToByteEncoder<Object> {

 @Override

 protected void encode(ChannelHandlerContext arg0, Object obj, ByteBuf buf)

   throws Exception {

  //序列化

  MessagePack mpPack = new MessagePack();

  byte [] raw = mpPack.write(obj);

  buf.writeBytes(raw);

 }

}

package cc.zychen.com;

import org.msgpack.annotation.Message;

@Message

public class UserInfo {

 protected String name;

 protected int age;

public String getName() {

 return name;

}

public void setName(String name) {

 this.name = name;

}

public int getAge() {

 return age;

}

public void setAge(int age) {

 this.age = age;

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

import io.netty.handler.codec.LengthFieldPrepender;

import io.netty.handler.codec.string.StringDecoder;

public class EchoClient {

 public void connect(String ip,int port) throws InterruptedException{

  EventLoopGroup group = new NioEventLoopGroup();

  try{

  Bootstrap b = new Bootstrap();

  b.group(group)

  //.option(ChannelOption.TCP_NODELAY, true)

  .channel(NioSocketChannel.class)

  .handler(new ChannelInitializer<SocketChannel>() {

   @Override

   protected void initChannel(SocketChannel ch) throws Exception {

    ch.pipeline().addLast(new LengthFieldBasedFrameDecoder(65535,0,4,0,4));

    ch.pipeline().addLast(new MsgPackDecoder());

    ch.pipeline().addLast(new LengthFieldPrepender(4,false));

    ch.pipeline().addLast(new MsgPackEncoder());

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

import java.io.ByteArrayInputStream;

import java.util.List;

import org.msgpack.MessagePack;

import org.msgpack.template.TemplateRegistry;

import org.msgpack.template.Templates;

import org.msgpack.type.Value;

import io.netty.channel.ChannelHandlerContext;

import io.netty.channel.ChannelInboundHandlerAdapter;

public class EchoClientHandler extends ChannelInboundHandlerAdapter {

 private int counter = 0;

 @Override

 public void channelRead(ChannelHandlerContext ctx, Object msg)

   throws Exception {

  @SuppressWarnings("unchecked")

  List<UserInfo> u = (List<UserInfo>)msg;

  System.out.println(u);

//@SuppressWarnings("deprecation")

  UserInfo t = MessagePack.unpack(MessagePack.pack(msg),UserInfo.class);

  System.out.println("this is "+ ++ counter +" times receive server:name[" + t.getName() +"]" +" age["+ t.getAge() +"]");

 }

 @Override

 public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause)

   throws Exception {

  cause.printStackTrace();

  ctx.close();

 }

 @Override

 public void channelActive(ChannelHandlerContext ctx) throws Exception {

  UserInfo usInfo = null;

  for(int n=0;n<10;n++){

   usInfo = new UserInfo();

   usInfo.setName("aaaaaaa");

   usInfo.setAge(88);

   ctx.write(usInfo);

  }

  ctx.flush();

 }

 @Override

 public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {

  ctx.flush();

 }

}

package cc.zychen.com;

import java.util.List;

import org.msgpack.MessagePack;

import io.netty.buffer.ByteBuf;

import io.netty.channel.ChannelHandlerContext;

import io.netty.handler.codec.MessageToMessageDecoder;

public class MsgPackDecoder extends MessageToMessageDecoder<ByteBuf> {

 @Override

 protected void decode(ChannelHandlerContext arg0, ByteBuf arg1,

   List<Object> arg2) throws Exception {

  final byte[] array;

  int length = arg1.readableBytes();

  array = new byte[length];

  arg1.getBytes(arg1.readerIndex(),array,0,length);

  MessagePack mpPack = new MessagePack();

  arg2.add(mpPack.read(array));

 }

}

package cc.zychen.com;

import org.msgpack.MessagePack;

import io.netty.buffer.ByteBuf;

import io.netty.channel.ChannelHandlerContext;

import io.netty.handler.codec.MessageToByteEncoder;

public class MsgPackEncoder extends MessageToByteEncoder<Object> {

 @Override

 protected void encode(ChannelHandlerContext arg0, Object arg1, ByteBuf arg2)

   throws Exception {

  //序列化

  MessagePack mpPack = new MessagePack();

  byte [] raw = mpPack.write(arg1);

  arg2.writeBytes(raw);

 }

}

package cc.zychen.com;

import org.msgpack.annotation.Message;

@Message

public class UserInfo {

 protected String name;

 protected int age;

public String getName() {

 return name;

}

public void setName(String name) {

 this.name = name;

}

public int getAge() {

 return age;

}

public void setAge(int age) {

 this.age = age;

}

}

完整代码下载：[http://download.csdn.net/download/byxdaz/10141725](http://download.csdn.net/download/byxdaz/10141725)

