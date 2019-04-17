# Netty分隔符解码器和定长解码器的应用方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月21日 18:11:21[boonya](https://me.csdn.net/boonya)阅读数：287








文章转自：[http://blog.csdn.net/u010853261/article/details/54799389](http://blog.csdn.net/u010853261/article/details/54799389)

**TCP以流的方式进行数据传输，上层应用协议为了对消息进行区分，往往采用如下4种方式:**
（1）消息长度固定：累计读取到固定长度为LENGTH之后就认为读取到了一个完整的消息。然后将计数器复位，重新开始读下一个数据报文。

（2）回车换行符作为消息结束符：在文本协议中应用比较广泛。

（3）将特殊的分隔符作为消息的结束标志，回车换行符就是一种特殊的结束分隔符。

（4）通过在消息头中定义长度字段来标示消息的总长度。

netty中针对这四种场景均有对应的解码器作为解决方案。本文我们继续介绍DelimiterBasedFrameDecoder和FixedLengthFrameDecoder分别来解决以特殊分隔符作为消息的结束标志的解码和定长消息的解码。他们均能解决TCP导致的黏包读半包问题。

## DelimiterBasedFrameDecoder分隔符解码器应用开发

我们以Echo服务器来演示DelimiterBasedFrameDecoder自动完成以分隔符作为码流结束标识消息结束的解码。EchoServer收到EchoClient的请求消息后，将其打印出来，然后将原始消息返回客户端。消息以”$_” 作为分隔符。

### 服务端Server

服务端主要加了DelimiterBasedFrameDecoder，并在构造器中配置消息的最大长度是1024，并设置行分隔符是”$_”。

```java
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.phei.netty.frame.delimiter;

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

/**
 * @author lilinfeng
 * @date 2014年2月14日
 * @version 1.0
 */
public class EchoServer {
	public void bind(int port) throws Exception {
		// 配置服务端的NIO线程组
		EventLoopGroup bossGroup = new NioEventLoopGroup();
		EventLoopGroup workerGroup = new NioEventLoopGroup();
		try {
			ServerBootstrap b = new ServerBootstrap();
			b.group(bossGroup, workerGroup).channel(NioServerSocketChannel.class).option(ChannelOption.SO_BACKLOG, 100)
					.handler(new LoggingHandler(LogLevel.INFO)).childHandler(new ChannelInitializer<SocketChannel>() {
						@Override
						public void initChannel(SocketChannel ch) throws Exception {
							ByteBuf delimiter = Unpooled.copiedBuffer("$_".getBytes());
							ch.pipeline().addLast(new DelimiterBasedFrameDecoder(1024, delimiter));
							ch.pipeline().addLast(new StringDecoder());
							ch.pipeline().addLast(new EchoServerHandler());
						}
					});

			// 绑定端口，同步等待成功
			ChannelFuture f = b.bind(port).sync();

			// 等待服务端监听端口关闭
			f.channel().closeFuture().sync();
		} finally {
			// 优雅退出，释放线程池资源
			bossGroup.shutdownGracefully();
			workerGroup.shutdownGracefully();
		}
	}

	public static void main(String[] args) throws Exception {
		int port = 8080;
		if (args != null && args.length > 0) {
			try {
				port = Integer.valueOf(args[0]);
			} catch (NumberFormatException e) {
				// 采用默认值
			}
		}
		new EchoServer().bind(port);
	}
}
```


### 服务端handler

handler 中直接把收到的消息echo回客户端，并在服务端做了一个技术，用来验证TCP的黏包是否被解决。




```java
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.phei.netty.frame.delimiter;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerAdapter;
import io.netty.channel.ChannelHandlerContext;

/**
 * @author lilinfeng
 * @date 2014年2月14日
 * @version 1.0
 */
@Sharable
public class EchoServerHandler extends ChannelHandlerAdapter {

	int counter = 0;

	@Override
	public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
		String body = (String) msg;
		System.out.println("This is " + ++counter + " times receive client : [" + body + "]");
		body += "$_";
		ByteBuf echo = Unpooled.copiedBuffer(body.getBytes());
		ctx.writeAndFlush(echo);
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
		cause.printStackTrace();
		ctx.close();// 发生异常，关闭链路
	}
}
```




### 客户端Client



```java
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.phei.netty.frame.delimiter;

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
import io.netty.handler.codec.string.StringDecoder;

/**
 * @author lilinfeng
 * @date 2014年2月14日
 * @version 1.0
 */
public class EchoClient {

	public void connect(int port, String host) throws Exception {
		// 配置客户端NIO线程组
		EventLoopGroup group = new NioEventLoopGroup();
		try {
			Bootstrap b = new Bootstrap();
			b.group(group).channel(NioSocketChannel.class).option(ChannelOption.TCP_NODELAY, true)
					.handler(new ChannelInitializer<SocketChannel>() {
						@Override
						public void initChannel(SocketChannel ch) throws Exception {
							ByteBuf delimiter = Unpooled.copiedBuffer("$_".getBytes());
							ch.pipeline().addLast(new DelimiterBasedFrameDecoder(1024, delimiter));
							ch.pipeline().addLast(new StringDecoder());
							ch.pipeline().addLast(new EchoClientHandler());
						}
					});

			// 发起异步连接操作
			ChannelFuture f = b.connect(host, port).sync();

			// 当代客户端链路关闭
			f.channel().closeFuture().sync();
		} finally {
			// 优雅退出，释放NIO线程组
			group.shutdownGracefully();
		}
	}

	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		int port = 8080;
		if (args != null && args.length > 0) {
			try {
				port = Integer.valueOf(args[0]);
			} catch (NumberFormatException e) {
				// 采用默认值
			}
		}
		new EchoClient().connect(port, "127.0.0.1");
	}
}
```





### 客户端Handler

客户端的handler中，在建立链路时发送10个请求给服务端




```java
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.phei.netty.frame.delimiter;

import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerAdapter;
import io.netty.channel.ChannelHandlerContext;

/**
 * @author lilinfeng
 * @date 2014年2月14日
 * @version 1.0
 */
public class EchoClientHandler extends ChannelHandlerAdapter {

	private int counter;

	static final String ECHO_REQ = "Hi, Lilinfeng. Welcome to Netty.$_";

	/**
	 * Creates a client-side handler.
	 */
	public EchoClientHandler() {
	}

	@Override
	public void channelActive(ChannelHandlerContext ctx) {
		// ByteBuf buf = UnpooledByteBufAllocator.DEFAULT.buffer(ECHO_REQ
		// .getBytes().length);
		// buf.writeBytes(ECHO_REQ.getBytes());
		for (int i = 0; i < 10; i++) {
			ctx.writeAndFlush(Unpooled.copiedBuffer(ECHO_REQ.getBytes()));
		}
	}

	@Override
	public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
		System.out.println("This is " + ++counter + " times receive server : [" + msg + "]");
	}

	@Override
	public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {
		ctx.flush();
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
		cause.printStackTrace();
		ctx.close();
	}
}
```



### 分隔符测试效果



服务端：

```java
this is:1 time. Server received: LOUYYUTING netty. 
this is:2 time. Server received: LOUYYUTING netty. 
this is:3 time. Server received: LOUYYUTING netty. 
this is:4 time. Server received: LOUYYUTING netty. 
this is:5 time. Server received: LOUYYUTING netty. 
this is:6 time. Server received: LOUYYUTING netty. 
this is:7 time. Server received: LOUYYUTING netty. 
this is:8 time. Server received: LOUYYUTING netty. 
this is:9 time. Server received: LOUYYUTING netty. 
this is:10 time. Server received: LOUYYUTING netty.
```

服务端成功获取到10次请求。

客户端：

```java
client received: counter:1  msg:LOUYYUTING netty. 
client received: counter:2  msg:LOUYYUTING netty. 
client received: counter:3  msg:LOUYYUTING netty. 
client received: counter:4  msg:LOUYYUTING netty. 
client received: counter:5  msg:LOUYYUTING netty. 
client received: counter:6  msg:LOUYYUTING netty. 
client received: counter:7  msg:LOUYYUTING netty. 
client received: counter:8  msg:LOUYYUTING netty. 
client received: counter:9  msg:LOUYYUTING netty. 
client received: counter:10  msg:LOUYYUTING netty.
```

客户端也成功接收到了10次服务器的回应。

## FixedLengthFrameDecoder定长解码器应用开发


FixedLengthFrameDecoder是一个固定长度解码器，它可以按照指定的长度对消息进行解码，开发者不需要关心TCP的黏包和拆包问题，非常实用，下面也是通过实例讲解：

在第一节中的Echo服务器中做一点小小改动来实现：仅仅只把解码器DelimiterBasedFrameDecoder换成FixedLengthFrameDecoder。


### 服务端Server



```java
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.phei.netty.frame.fixedLen;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.FixedLengthFrameDecoder;
import io.netty.handler.codec.string.StringDecoder;
import io.netty.handler.logging.LogLevel;
import io.netty.handler.logging.LoggingHandler;

/**
 * @author lilinfeng
 * @date 2014年2月14日
 * @version 1.0
 */
public class EchoServer {
	public void bind(int port) throws Exception {
		// 配置服务端的NIO线程组
		EventLoopGroup bossGroup = new NioEventLoopGroup();
		EventLoopGroup workerGroup = new NioEventLoopGroup();
		try {
			ServerBootstrap b = new ServerBootstrap();
			b.group(bossGroup, workerGroup).channel(NioServerSocketChannel.class).option(ChannelOption.SO_BACKLOG, 100)
					.handler(new LoggingHandler(LogLevel.INFO)).childHandler(new ChannelInitializer<SocketChannel>() {
						@Override
						public void initChannel(SocketChannel ch) throws Exception {
							ch.pipeline().addLast(new FixedLengthFrameDecoder(20));
							ch.pipeline().addLast(new StringDecoder());
							ch.pipeline().addLast(new EchoServerHandler());
						}
					});

			// 绑定端口，同步等待成功
			ChannelFuture f = b.bind(port).sync();

			// 等待服务端监听端口关闭
			f.channel().closeFuture().sync();
		} finally {
			// 优雅退出，释放线程池资源
			bossGroup.shutdownGracefully();
			workerGroup.shutdownGracefully();
		}
	}

	public static void main(String[] args) throws Exception {
		int port = 8080;
		if (args != null && args.length > 0) {
			try {
				port = Integer.valueOf(args[0]);
			} catch (NumberFormatException e) {
				// 采用默认值
			}
		}
		new EchoServer().bind(port);
	}
}
```





### 服务端Handler



```java
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.phei.netty.frame.fixedLen;

import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerAdapter;
import io.netty.channel.ChannelHandlerContext;

/**
 * @author lilinfeng
 * @date 2014年2月14日
 * @version 1.0
 */
@Sharable
public class EchoServerHandler extends ChannelHandlerAdapter {

	@Override
	public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
		System.out.println("Receive client : [" + msg + "]");
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
		cause.printStackTrace();
		ctx.close();// 发生异常，关闭链路
	}
}
```





### 客户端Client

与分隔符编码器客户端类似...........


### 客户端Handler

与分隔符编码器客户端Handler类似...........


### 定长解码器运行效果



server端:

```
this is:1 time. Server received: LOUYUTING.
this is:2 time. Server received: LOUYUTING.
this is:3 time. Server received: LOUYUTING.
this is:4 time. Server received: LOUYUTING.
this is:5 time. Server received: LOUYUTING.
this is:6 time. Server received: LOUYUTING.
this is:7 time. Server received: LOUYUTING.
this is:8 time. Server received: LOUYUTING.
this is:9 time. Server received: LOUYUTING.
this is:10 time. Server received: LOUYUTING.
```

client端：

```java
client received: counter:1  msg:LOUYUTING.
client received: counter:2  msg:LOUYUTING.
client received: counter:3  msg:LOUYUTING.
client received: counter:4  msg:LOUYUTING.
client received: counter:5  msg:LOUYUTING.
client received: counter:6  msg:LOUYUTING.
client received: counter:7  msg:LOUYUTING.
client received: counter:8  msg:LOUYUTING.
client received: counter:9  msg:LOUYUTING.
client received: counter:10  msg:LOUYUTING.
```

可知我们上面定义的定长是10个字节，所以每次发送的都是以10个字节为一个完整的数据包。

所以FixedLengthFrameDecoder解码器无论一次接收到多少字节的数据包，都会按照构造函数中设定的字节数进行解码。如果数据字节数不到指定的定长（半 包），FixedLengthFrameDecoder会缓存半包消息等待下个包到达后进行拼包。








