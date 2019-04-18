# netty与websocket通信demo - z69183787的专栏 - CSDN博客
2016年09月11日 18:17:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2284
                
netty v3.9.4
websocket连接建立前，客户端需要与服务器进行握手（http协议） 确认websocket连接，也就是说在处理websocket请求前，必需要处理一些http请求。
websocket到现在为止，已经有多个版本，netty有相应的对应类，这部分处理一般不需要人工干预。
如果运行正常的话，会在页面的文本框中显示1-20记数。
可以通过firefox或chrome的开发人员工具，显看浏览器与服务器的交互。
主要是HttpServerChannelHandler2，加了些注释和少量debug代码。
```java
package org.sl.demo.httpserver1;
import java.util.List;
import java.util.Map;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelHandler;
import org.jboss.netty.handler.codec.http.DefaultHttpResponse;
import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.jboss.netty.handler.codec.http.HttpMethod;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.jboss.netty.handler.codec.http.HttpResponseStatus;
import org.jboss.netty.handler.codec.http.HttpVersion;
import org.jboss.netty.handler.codec.http.websocketx.CloseWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.PingWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.PongWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.WebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.WebSocketServerHandshaker;
import org.jboss.netty.handler.codec.http.websocketx.WebSocketServerHandshakerFactory;
public class HttpServerChannelHandler2 extends SimpleChannelHandler{
	public static boolean debug = true;
	
	@Override
	public void messageReceived(ChannelHandlerContext ctx, MessageEvent e) {
		Channel ch = e.getChannel();
		Object msg = e.getMessage();
		
		if(debug){
			System.out.println("---------------");
			System.out.println("message: "+msg.getClass());
		}
		//虽然是websocket，但在建立websocket连接前，先进行http握手,所以，这时也要处理http请求
		//在http握手完成后，才是websocket下的通信
		if(msg instanceof HttpRequest){
			processHttpRequest(ch, (HttpRequest)msg);
		}else if(msg instanceof WebSocketFrame){
			processWebsocketRequest(ch,(WebSocketFrame)msg);
		}else{
			//未处理的请求类型
		}
	}
	
	//这个方法：
	//1.完成websocket前的http握手
	//2.屏蔽掉非websocket握手请求
	void processHttpRequest(Channel channel,HttpRequest request){
		HttpHeaders headers = request.headers();
		if(debug){
			List<Map.Entry<String,String>> ls = headers.entries();
			for(Map.Entry<String,String> i: ls){
				System.out.println("header  "+i.getKey()+":"+i.getValue());
			}
		}		
		
		//屏蔽掉非websocket握手请求
		//只接受http GET和headers['Upgrade']为'websocket'的http请求
		if(!HttpMethod.GET.equals(request.getMethod())
				|| !"websocket".equalsIgnoreCase(headers.get("Upgrade"))){
			DefaultHttpResponse resp = new DefaultHttpResponse(
					HttpVersion.HTTP_1_1,
					HttpResponseStatus.BAD_REQUEST);
			channel.write(resp);			
			channel.close();
			return;
		}
		
		WebSocketServerHandshakerFactory wsShakerFactory = new WebSocketServerHandshakerFactory(
				"ws://"+request.headers().get(HttpHeaders.Names.HOST),
				null,false );
		WebSocketServerHandshaker wsShakerHandler = wsShakerFactory.newHandshaker(request);
		if(null==wsShakerHandler){
			//无法处理的websocket版本
			wsShakerFactory.sendUnsupportedWebSocketVersionResponse(channel);
		}else{
			//向客户端发送websocket握手,完成握手
			//客户端收到的状态是101 sitching protocol
			wsShakerHandler.handshake(channel, request);
		}		
	}
	
	//websocket通信
	void processWebsocketRequest(Channel channel, WebSocketFrame request){		
		if(request instanceof CloseWebSocketFrame){
			channel.close();
		}else if(request instanceof PingWebSocketFrame){			
			channel.write(new PongWebSocketFrame(request.getBinaryData()));  
		}else if(request instanceof TextWebSocketFrame){
			//这个地方 可以根据需求，加上一些业务逻辑
			TextWebSocketFrame txtReq = (TextWebSocketFrame) request;
			if(debug){ System.out.println("txtReq:"+txtReq.getText());}
			
			//向ws客户端发送多个响应
			for(int i=1; i<=20; i++){
				channel.write(new TextWebSocketFrame(""+i));
				try{Thread.sleep(300);}catch(Exception ex){}
			}			
		}else{
			//WebSocketFrame还有一些
		}
	}
}
```
其他类跟网上的差不多： http://blog.csdn.net/shagoo/article/details/8028813
```java
package org.sl.demo.httpserver1;
import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;
public class HttpServer implements Runnable{
	int port = 80;
	
	public HttpServer(int port){
		this.port = port;
	}
	@Override
	public void run() {
		ServerBootstrap b = new ServerBootstrap(
				new NioServerSocketChannelFactory(
						Executors.newCachedThreadPool(),
						Executors.newCachedThreadPool()));
		b.setPipelineFactory(new HttpServerChannelPipelineFactory());
		b.setOption("child.tcpNoDelay", true);  
		b.setOption("child.keepAlive", true);
		b.bind(new InetSocketAddress(port));
	}
	
	public static void main(String[] args){
		new HttpServer(80).run();
	}
}
```
```java
package org.sl.demo.httpserver1;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.handler.codec.http.HttpRequestDecoder;
import org.jboss.netty.handler.codec.http.HttpResponseEncoder;
import org.jboss.netty.handler.codec.http.websocketx.WebSocket00FrameDecoder;
import org.jboss.netty.handler.codec.http.websocketx.WebSocket08FrameDecoder;
public class HttpServerChannelPipelineFactory  implements ChannelPipelineFactory {
	@Override
	public ChannelPipeline getPipeline() throws Exception {
		ChannelPipeline cp = Channels.pipeline();
		
		cp.addLast("decoder", new HttpRequestDecoder());
//		cp.addLast("decoder", new WebSocket00FrameDecoder());
		cp.addLast("encoder", new HttpResponseEncoder());
//		cp.addLast("downhandler", new HttpServerDownstreamHandler());
//		cp.addLast("uphandler", new HttpServerUpstreamHandler());
		cp.addLast("handler", new HttpServerChannelHandler2());
		
		return cp;
	}
}
```
测试页面：
```
<html>
<head>
<script >
function connect1(){
	alert('connect1');
	
	var ta = document.getElementById('responseText');
	var socket = new WebSocket('ws://127.0.0.1/websocket'); 
	if (window.WebSocket) {
	
	}else{
		alert('Your browser does not support Web Socket.');
		return;
	}
	socket.onopen = function(event) { 
		ta.value = "Web Socket opened!";
	}
	
	socket.onmessage = function(event) { 
		ta.value = event.data;
	}; 
	
	socket.onclose = function(event) { 
		ta.value = "Web Socket closed"; 
	}; 
}
function connect() {
	alert('connect');
	var socket;
	if (!window.WebSocket) {
  		window.WebSocket = window.MozWebSocket;
	}
	if (window.WebSocket) {
  		socket = new WebSocket("ws://127.0.0.1/websocket");
  		socket.onmessage = function(event) {
    		var ta = document.getElementById('responseText');
    		ta.value = event.data;
  		};
  
	  	socket.onopen = function(event) {
	    	var ta = document.getElementById('responseText');
	    	ta.value = "Web Socket opened!";
			socket.send('hello');
	  	};
	  
	  	socket.onclose = function(event) {
	    	var ta = document.getElementById('responseText');
	    	ta.value = "Web Socket closed"; 
	  	};
		
		socket.onerror = function(event){			
		};  
 	} else {
 		alert("Your browser does not support Web Socket.");
	}
}
</script>
</head>
<body>
<input type="button" onclick="connect1()" value="ws connect1"/> <br/><br/>
<input type="button" onclick="connect()" value="ws connect"/> <br/><br/>
<input id="responseText" type="text" value="123456"/>
</body>
</html>
```
