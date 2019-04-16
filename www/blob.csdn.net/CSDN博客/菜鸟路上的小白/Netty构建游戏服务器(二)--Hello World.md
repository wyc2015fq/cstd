# Netty构建游戏服务器(二)--Hello World - 菜鸟路上的小白 - CSDN博客





2016年10月26日 20:06:20[somnus_小凯](https://me.csdn.net/u012486840)阅读数：2940








# 一，准备工作

1，netty-all-4.1.5.Final.jar（官网下载）

2，eclipse

# 二，步骤概要

## 1，服务器开发

### （1），创建Server类

该类是程序的主入口，有main方法，服务器开启也是在此执行。

该类主要是提供了channel链接，绑定了端口。

该类需要new一个Initalizer类来完成服务器开启。

### （2），创建Initalizer类

该类是初始化类，主要是创建了传输通道ChannelPipeline，然后在通道中加入了一些列的Handler，其中解码和编码Handler是Netty自带的辅助类，在最后假如了自定义业务控制器类。

### （3），创建Handler类

该类是自定义的业务控制器，需要的逻辑都在这里实现，例如收到信息如何处理，断开连接如何发送消息等。

## 2，客户端开发

和服务器端开发类似，不同在于：

（1），client类（对应服务器的server类）只需要一个EventLoopGroup，而服务器类需要两个。

（2），编码解码器要和服务器一致。

# 三，具体实现

## 1，项目树形图

![](http://static.oschina.net/uploads/space/2016/0912/043652_UoIw_1476692.png)

服务器端和客户端分别有三个类，HelloClient是客户端主入口，HelloServer是服务器端主入口。

## 2，服务器代码

### （1），HelloServer

服务器主入口类

```java
package org.example.hello;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

public class HelloServer {
    
    /**
     * 服务端监听的端口地址
     */
    private static final int portNumber = 7878;
    
    public static void main(String[] args) throws InterruptedException {
        //开启两个事件循环组，事件循环组会自动构建EventLoop，服务器一般开启两个，提高效率
        EventLoopGroup bossGroup = new NioEventLoopGroup();
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
           //Netty的引导类，用于简化开发
            ServerBootstrap b = new ServerBootstrap();
           //把事件循环组加入引导程序
            b.group(bossGroup, workerGroup);
           //开启socket
            b.channel(NioServerSocketChannel.class);
           //加入业务控制器，这里是加入一个初始化类，其中包含了很多业务控制器
            b.childHandler(new HelloServerInitializer());

            // 服务器绑定端口监听
            ChannelFuture f = b.bind(portNumber).sync();
            // 监听服务器关闭监听
            f.channel().closeFuture().sync();

            // 可以简写为
            /* b.bind(portNumber).sync().channel().closeFuture().sync(); */
        } finally {
           //Netty优雅退出
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
        }
    }
}
```

### （1），HelloServerInitializer

初始化传输通道

```java
package org.example.hello;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;
import io.netty.handler.codec.Delimiters;
import io.netty.handler.codec.string.StringDecoder;
import io.netty.handler.codec.string.StringEncoder;

//继承Netty提供的初始化类，只要复写其中的方法就可以了
public class HelloServerInitializer extends ChannelInitializer<SocketChannel> {

    @Override
    protected void initChannel(SocketChannel ch) throws Exception {
       //开启传输通道，这个通道的作用就是管理控制器，形成一个责任链式管理
        ChannelPipeline pipeline = ch.pipeline();

        // 以("\n")为结尾分割的 解码器
        pipeline.addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));

        // 字符串解码 和 编码
        pipeline.addLast("decoder", new StringDecoder()); 
        pipeline.addLast("encoder", new StringEncoder());

        // 加入自定义的Handler
        pipeline.addLast("handler", new HelloServerHandler());
        //初始化类一般都是先加入编码解码器来解读传输来的消息，然后加入自定义类来处理业务逻辑
    }
}
```

### （1），HelloServerHandler

定义业务逻辑

```java
```java
package org.example.hello;

import java.net.InetAddress;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.channel.group.ChannelGroup;
import io.netty.channel.group.DefaultChannelGroup;
import io.netty.util.concurrent.GlobalEventExecutor;

//继承Netty提供的通道传入处理器类，只要复写方法就可以了，简化开发
public class HelloServerHandler extends SimpleChannelInboundHandler<String> {
	
	//获取现有通道，一个通道channel就是一个socket链接在这里
	public static ChannelGroup channels = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);

    //有新链接加入，对外发布消息
    @Override
    public void handlerAdded(ChannelHandlerContext ctx) throws Exception {  // (2)
        Channel incoming = ctx.channel();
        for (Channel channel : channels) {
            channel.writeAndFlush("[SERVER] - " + incoming.remoteAddress() + " 加入\n");
        }
        channels.add(ctx.channel());
    } 
    
    //有链接断开，对外发布消息
    @Override
    public void handlerRemoved(ChannelHandlerContext ctx) throws Exception {  // (3)
        Channel incoming = ctx.channel();
        for (Channel channel : channels) {
            channel.writeAndFlush("[SERVER] - " + incoming.remoteAddress() + " 离开\n");
        }
        channels.remove(ctx.channel());
    }
    
    //消息读取有两个方法，channelRead和channelRead0，其中channelRead0可以读取泛型，常用  
    //收到消息打印出来，并返还客户端消息
    @Override
    protected void channelRead0(ChannelHandlerContext ctx, String msg) throws Exception {
        // 收到消息直接打印输出
        System.out.println(ctx.channel().remoteAddress() + " Say : " + msg);
        
        // 返回客户端消息 - 我已经接收到了你的消息
        ctx.writeAndFlush("Received your message !\n");
    }
    
    /*
     * 
     * 覆盖 channelActive 方法 在channel被启用的时候触发 (在建立连接的时候)
     * 
     * channelActive 和 channelInActive 在后面的内容中讲述，这里先不做详细的描述
     * */
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        
        System.out.println("RamoteAddress : " + ctx.channel().remoteAddress() + " active !");
        
        ctx.writeAndFlush( "Welcome to " + InetAddress.getLocalHost().getHostName() + " service!\n");
        
        super.channelActive(ctx);
    }
}
```
```

## 3，客户端代码

（1），HelloClient

```java
package org.example.hello;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioSocketChannel;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class HelloClient {
    
    public static String host = "127.0.0.1";
    public static int port = 7878;

    /**
     * @param args
     * @throws InterruptedException 
     * @throws IOException 
     */
    public static void main(String[] args) throws InterruptedException, IOException {
        EventLoopGroup group = new NioEventLoopGroup();
        try {
            Bootstrap b = new Bootstrap();
            b.group(group)
            .channel(NioSocketChannel.class)
            .handler(new HelloClientInitializer());

            // 连接服务端
            Channel ch = b.connect(host, port).sync().channel();
            
            // 控制台输入
            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
            //也可以用while循环
            for (;;) {
                String line = in.readLine();
                if (line == null) {
                    continue;
                }
                /*
                 * 向服务端发送在控制台输入的文本 并用"\r\n"结尾
                 * 之所以用\r\n结尾 是因为我们在handler中添加了 DelimiterBasedFrameDecoder 帧解码。
                 * 这个解码器是一个根据\n符号位分隔符的解码器。所以每条消息的最后必须加上\n否则无法识别和解码
                 * */
                ch.writeAndFlush(line + "\r\n");
            }
        } finally {
            // The connection is closed automatically on shutdown.
            group.shutdownGracefully();
        }
    }
}
```

（2），HelloClientInitializer

```java
package org.example.hello;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;
import io.netty.handler.codec.Delimiters;
import io.netty.handler.codec.string.StringDecoder;
import io.netty.handler.codec.string.StringEncoder;

public class HelloClientInitializer extends ChannelInitializer<SocketChannel> {

    @Override
    protected void initChannel(SocketChannel ch) throws Exception {
        ChannelPipeline pipeline = ch.pipeline();

        /*
         * 这个地方的 必须和服务端对应上。否则无法正常解码和编码
         * 
         * 解码和编码 我将会在下一张为大家详细的讲解。再次暂时不做详细的描述
         * 
         * */
        pipeline.addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));
        pipeline.addLast("decoder", new StringDecoder());
        pipeline.addLast("encoder", new StringEncoder());
        
        // 客户端的逻辑
        pipeline.addLast("handler", new HelloClientHandler());
    }
}
```

（3），HellClientHandler

```java
```java
package org.example.hello;

import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;

public class HelloClientHandler extends SimpleChannelInboundHandler<String> {

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, String msg) throws Exception {
        
        System.out.println("Server say : " + msg);
    }
    
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("Client active ");
        super.channelActive(ctx);
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("Client close ");
        super.channelInactive(ctx);
    }
}
```
```

# 四，运行测试

服务器右键运行程序

![](http://static.oschina.net/uploads/space/2016/0912/050431_GZEA_1476692.png)

客户端右键运行程序

![](http://static.oschina.net/uploads/space/2016/0912/050506_5g4o_1476692.png)

服务器端console：

![](http://static.oschina.net/uploads/space/2016/0912/051019_f4yB_1476692.png)

客户端console：

![](http://static.oschina.net/uploads/space/2016/0912/050952_EA2j_1476692.png)

切换console:

![](http://static.oschina.net/uploads/space/2016/0912/051135_lr4B_1476692.png)



