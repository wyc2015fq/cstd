# 聊天室 - Netty WebSocket初试 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2019年03月20日 11:38:44[dutong0321](https://me.csdn.net/dutong0321)阅读数：24标签：[netty																[websocket																[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)](https://so.csdn.net/so/search/s.do?q=websocket&t=blog)](https://so.csdn.net/so/search/s.do?q=netty&t=blog)
个人分类：[编程世界](https://blog.csdn.net/dutong0321/article/category/6127295)
### 背景
最近项目上面需要用到聊天室，在Ajax轮询和WebSocket之间考虑了下，决定还是采用WebSocket来实现这个项目。采用WebSocket实现，那么就必须进行服务器的技术选型，主要考虑的有Java(Netty/Jetty)、node.js([socket.io](http://socket.io))、PHP(swoole/workerman)。
但是PHP语言的服务器就全部放弃了，论速度估计是比不上前面两者（没测试，但有人好像测试过，不过PHP的一般情况下貌似的确速度方面不如其他语言，而且workerman貌似不太快，swoole安装还挺麻烦的，相对来讲），最关键的是我感觉PHP的一个非常重要的就是可以在虚拟主机中运行，但是PHP的WebSocket基本上必须得虚拟服务器，那我要他何用！！！
在Java和node.js中进行选择的主要原因是：服务器中已经配好Java环境了，就懒得搞node.js了。
Netty和Jetty对比，好像是Netty更好一些，我从网上看到的，说错了，别打我！！！
好了，那就Netty了，顺便还可以学点新的玩意。以下内容其实大部分来自慕课网的netty websocket课程，主讲人是济癫，其中有个地方好像有BUG，随手改了，另外Netty5被废了，于是我用的包其实是Netty4的。
PS:网络上绝大部分内容应该都是来自慕课网的课程或者是《Netty权威指南》，其实慕课网的这些个应该是来自《Netty权威指南》。
### 服务器代码
其实没有什么好说的，直接上代码吧！
```
import io.netty.channel.group.ChannelGroup;
import io.netty.channel.group.DefaultChannelGroup;
import io.netty.util.concurrent.GlobalEventExecutor;
public class NettyConfig {
	
	/*
	 *   存储每个客户端接入的配置量
	 */
	public static ChannelGroup cg = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);
}
```
这个没有任何改动，都是慕课网原装的！
```
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.DefaultFullHttpResponse;
import io.netty.handler.codec.http.FullHttpRequest;
import io.netty.handler.codec.http.HttpResponseStatus;
import io.netty.handler.codec.http.HttpVersion;
import io.netty.handler.codec.http.websocketx.CloseWebSocketFrame;
import io.netty.handler.codec.http.websocketx.PingWebSocketFrame;
import io.netty.handler.codec.http.websocketx.PongWebSocketFrame;
import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketServerHandshaker;
import io.netty.handler.codec.http.websocketx.WebSocketServerHandshakerFactory;
import io.netty.util.CharsetUtil;
// 接收/处理/响应客户端WebSocket请求的核心业务处理类
public class MwWebSocketHandler extends SimpleChannelInboundHandler<Object> {
	private WebSocketServerHandshaker handshaker;
	private static final String WEB_SOCKET_URL = "ws://127.0.0.1:8888/websocket";
	// 服务端处理客户端WebSocket请求的核心方法
	@Override
	protected void channelRead0(ChannelHandlerContext arg0, Object arg1) throws Exception {
		// TODO Auto-generated method stub
		if(arg1 instanceof FullHttpRequest) {
			handHttpRequest(arg0, (FullHttpRequest)arg1);
		} else if (arg1 instanceof WebSocketFrame) {
			handWebsocketFrame(arg0, (WebSocketFrame)arg1);
		}
		
	}
	
	// 处理客户端与服务端之间的WebSocket业务
	private void handWebsocketFrame(ChannelHandlerContext ctx, WebSocketFrame frame) {
		// 判断是否是关闭WebSocket的命令
		if(frame instanceof CloseWebSocketFrame) {
			handshaker.close(ctx.channel(), (CloseWebSocketFrame)frame.retain());
			return;
		}
		// 判断是否是ping命令 
		if (frame instanceof PingWebSocketFrame) {
			ctx.channel().write(new PongWebSocketFrame(frame.content().retain()));
			return;
		}
		// 判断是否是二进制消息
		if (!(frame instanceof TextWebSocketFrame)) {
			System.out.println("该例子不处理二进制消息！");
			throw new RuntimeException("【 " + this.getClass().getName() + "】不支持消息！");
		}
		// 返回应答消息
		// 获取客户端向服务端发送的消息
		String request = ((TextWebSocketFrame)frame).text();
		System.out.println("服务端收到客户端的消息====>>>" + request);
		TextWebSocketFrame tws = new TextWebSocketFrame((ctx.channel().id() + "===>>>" + request);
		NettyConfig.cg.writeAndFlush(tws);
	}
	
	// 处理客户端向服务端发起HTTP握手请求的业务
	@SuppressWarnings("deprecation")
	private void handHttpRequest(ChannelHandlerContext ctx, FullHttpRequest req) {
		if(!req.getDecoderResult().isSuccess() || !("websocket".equals(req.headers().get("Upgrade")))) {
			sendHttpRequest(ctx, req, new DefaultFullHttpResponse(HttpVersion.HTTP_1_1, HttpResponseStatus.BAD_REQUEST));
			return;
		}
		WebSocketServerHandshakerFactory wsFactory = new WebSocketServerHandshakerFactory(WEB_SOCKET_URL, null, false);
		handshaker = wsFactory.newHandshaker(req);
		if(handshaker == null) {
			WebSocketServerHandshakerFactory.sendUnsupportedWebSocketVersionResponse(ctx.channel());
		} else {
			handshaker.handshake(ctx.channel(), req);
		}
	}
	
	// 服务端向客户端响应消息
	@SuppressWarnings("deprecation")
	private void sendHttpRequest(ChannelHandlerContext ctx, FullHttpRequest req, DefaultFullHttpResponse res) {
		if(res.getStatus().code() != 200) {
			ByteBuf buf = Unpooled.copiedBuffer(res.getStatus().toString(), CharsetUtil.UTF_8);
			res.content().writeBytes(buf);
			buf.release();
		}
		// 服务端向客户端发送数据
		ChannelFuture cf = ctx.channel().writeAndFlush(res);
		if(res.getStatus().code() != 200) {
			cf.addListener(ChannelFutureListener.CLOSE);
		}
	}
	
	// 客户端与服务端创建连接时调用
	@Override
	public void channelActive(ChannelHandlerContext ctx) throws Exception {
		// TODO Auto-generated method stub
		NettyConfig.cg.add(ctx.channel());
		System.out.println("客户端与服务端连接开启...");
	}
	// 客户端与服务端断开连接时调用
	@Override
	public void channelInactive(ChannelHandlerContext ctx) throws Exception {
		// TODO Auto-generated method stub
		NettyConfig.cg.remove(ctx.channel());
		System.out.println("客户端与服务端连接关闭。");
	}
	
	// 服务端接收客户端发送过来结束时调用
	@Override
	public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {
		// TODO Auto-generated method stub
		ctx.flush();
	}
	// 工程出现异常时调用
	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
		// TODO Auto-generated method stub
		cause.printStackTrace();
		ctx.close();
	}
}
```
把返回消息时会增加一个时间的地方去掉了，另外Nettt4中的处理 “服务端处理客户端WebSocket请求的核心方法”是channelRead0，不再是 messageReceived了，有几个函数被标记为deprecation了，以上这些就是使用netty4的差异吧。另外有一个BUG是：如果客户端断开请求时，服务器会弹出上面的那个自己定义的二进制异常，包括刷新、浏览器关闭和websocket主动关闭。其实原因在于在判断frame是否为CloseWebSocketFrame后，如果是的话，除了执行代码块后的代码，还会继续往下执行，然后是判断PingWebSocketFrame，再然后判断是否是TextWebScoketFrame，如果不是的话，就是二进制代码，但是上面说了这个frame其实是CloseWebSocketFrame，所以自然会抛出异常。那么解决方案其实很简单，在判断是否为CloseWebSocketFrame时，如果是的话执行完close后，那就return出去！
```
import io.netty.channel.ChannelInitializer;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.stream.ChunkedWriteHandler;
// 初始化连接时的各个组件
public class MwWebSocketChannelHandler extends ChannelInitializer<SocketChannel> {
	@Override
	protected void initChannel(SocketChannel arg0) throws Exception {
		// TODO Auto-generated method stub
		arg0.pipeline().addLast("http-codec", new HttpServerCodec());
		arg0.pipeline().addLast("aggregator", new HttpObjectAggregator(65536));
		arg0.pipeline().addLast("http-chunked", new ChunkedWriteHandler());
		arg0.pipeline().addLast("handler", new MwWebSocketHandler());
	}
}
```
这个没啥说的，继续！
```
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.Channel;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;
// 程序的入口，负责启动应用
public class Main {
	public static void main(String[] args) {
		EventLoopGroup bossGroup = new NioEventLoopGroup();
		EventLoopGroup workGroup = new NioEventLoopGroup();
		try {
			ServerBootstrap b = new ServerBootstrap();
			b.group(bossGroup, workGroup);
			b.channel(NioServerSocketChannel.class);
			b.childHandler(new MwWebSocketChannelHandler());
			System.out.println("服务端开启，等待客户端连接...");
			Channel ch = b.bind(8888).sync().channel();
			ch.closeFuture().sync();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		} finally {
			// TODO: handle finally clause
			bossGroup.shutdownGracefully();
			workGroup.shutdownGracefully();
		}
	}
}
```
同上
### 客户端代码
客户端代码我增加了一个关闭按钮，其实关闭、刷新浏览器也会自动关闭websocket的。
```
<html> 
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>WebSocket客户端</title>
	</head>
	<body>
		<form onSubmit="return false;">
			<input type="text" name="message" value="" />
			<br/><br/>
			<input type="button" value="发送WebSocket消息" onclick="send(this.form.message.value)" />
			<input type="button" value="关闭" onclick="close_socket()" />
			<hr color="red" />
			<h2>客户端接收到服务端返回的应答消息</h2>
			<textarea id="responseContent" style="width:100%;height:300px"></textarea>
		</form>
		<script type="text/javascript">
			var socket;
			if(!window.WebSocket) {
				window.WebSocket = window.MozWebSocket;
			}
			if(window.WebSocket) {
				console.log("debug2");
				socket = new WebSocket("ws://127.0.0.1:8888/websocket");
				socket.onmessage = function(event) {
					var ta = document.getElementById("responseContent");
					ta.value += event.data + "\r\n";
				}
				socket.onopen = function(event) {
					var ta = document.getElementById("responseContent");
					ta.value = "您的浏览器支持WebSocket，已连接服务器...\r\n";
				}
				socket.onclose = function(event) {
					var ta = document.getElementById("responseContent");
					ta.value = "WebSocket 连接关闭！\r\n";
				}
			} else {
				alert("您的浏览器不支持WebSocket");
			}
			function send(message) {
				console.log(message);
				if (!window.WebSocket) {
					return;
				}
				if (socket.readyState = WebSocket.OPEN) {
					socket.send(message);
				} else {
					alert("脚本没有连接成功");
				}
			}
			function close_socket() {
				socket.close();
			}
		</script>
	</body>
</html>
```
OK啦，对于一个简单的聊天室是可以实现基本的功能了，不过这玩意如果上交给领导，那我就该另投简历了，所以之后还会有一些别的东西加入，例如：
1.用户的识别！
2.用户的分组，不可能所有的用户都在一起聊吧！！
3.心跳机制，就算没有什么意外，难免会被防火墙/网关去中断！！
4.前端的美化，现在都9102年了，还用这个界面！！！
5.以及其他功能的实现！
之后有空的时候还会更新的！！！
注意：是有空的时候！！！！！
