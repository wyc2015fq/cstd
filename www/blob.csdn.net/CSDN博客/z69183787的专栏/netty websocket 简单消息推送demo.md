# netty websocket 简单消息推送demo - z69183787的专栏 - CSDN博客
2016年09月11日 18:25:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4627
                
这篇是基于"netty与websocket通信demo"。
错误想法：大量客户请求，共用一个worker，来实现推送。
正确作法：应该是对Channel对应的ChannelGroup进行操作，来实现推送。
一个Channel可以划分到多个ChannelGroup中。
PushServerChannelHandler和DynMessage这两个类最重要，其实类基本没变
```java
package org.sl.demo.chatserver;
import java.util.List;
import java.util.Map;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelHandler;
import org.jboss.netty.channel.group.ChannelGroup;
import org.jboss.netty.channel.group.DefaultChannelGroup;
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
public class PushServerChannelHandler extends SimpleChannelHandler {
	static boolean debug = true;
	
	@Override
	public void channelOpen(ChannelHandlerContext ctx, ChannelStateEvent e){
		if(debug){
			System.out.println("channelOpen");
		}
		DynMessage.addAudience(e.getChannel());
	}
	
	@Override
    public void messageReceived(ChannelHandlerContext ctx, MessageEvent e) throws Exception{
		Channel ch = e.getChannel();
		Object msg = e.getMessage();
		
		if(debug){
			System.out.println("---------------");
			System.out.println("message: "+msg.getClass());
		}
		try{
			if(msg instanceof HttpRequest){
				processHttpRequest(ch, (HttpRequest)msg);
			}else if(msg instanceof WebSocketFrame){
				processWebsocketRequest(ch,(WebSocketFrame)msg);
			}else{
				//未处理的请求类型
			}
		}catch(Exception ex){
			ch.close().sync();
		}
		super.messageReceived(ctx, e);
	}
	
	@Override
	public void channelClosed(ChannelHandlerContext ctx, ChannelStateEvent e){
		if(debug){
			System.out.println("channelClosed");
		}
		if(e instanceof MessageEvent){
			MessageEvent me = (MessageEvent) e;			
		}
		DynMessage.removeAudience(e.getChannel());
		e.getChannel().close();
	}
	
	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e){
		if(debug){
			System.out.println("channelClosed");
		}
		DynMessage.removeAudience(e.getChannel());
		e.getCause().printStackTrace();
		e.getChannel().close();
		try {
			super.exceptionCaught(ctx, e);
		} catch (Exception e1) {		
			e1.printStackTrace();
		}
	}
	
	void processHttpRequest(Channel channel,HttpRequest request){
		HttpHeaders headers = request.headers();
		if(debug){
			List<Map.Entry<String,String>> ls = headers.entries();
			for(Map.Entry<String,String> i: ls){
				System.out.println("header  "+i.getKey()+":"+i.getValue());
			}
		}	
		
		//non-get request
		if(!HttpMethod.GET.equals(request.getMethod())){
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
	
	void processWebsocketRequest(Channel channel, WebSocketFrame request) throws Exception{		
		if(request instanceof CloseWebSocketFrame){
			DynMessage.removeAudience(channel);
			channel.close().sync();
		}else if(request instanceof PingWebSocketFrame){			
			channel.write(new PongWebSocketFrame(request.getBinaryData()));  
		}else if(request instanceof TextWebSocketFrame){
			//这个地方 可以根据需求，加上一些业务逻辑
			TextWebSocketFrame txtReq = (TextWebSocketFrame) request;		
			if(debug){ System.out.println("txtReq:"+txtReq.getText());}
			if("disconnect".equalsIgnoreCase(txtReq.getText())){
				DynMessage.removeAudience(channel);
				channel.close().sync();
				return;
			}
			//把符合条件的channel添加到DynMessage的channelGroup中
			DynMessage.addAudience(channel);
		}else{
			//WebSocketFrame还有一些
		}
	}
}
```
```java
package org.sl.demo.chatserver;
import java.util.Random;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.group.ChannelGroup;
import org.jboss.netty.channel.group.DefaultChannelGroup;
import org.jboss.netty.handler.codec.http.websocketx.TextWebSocketFrame;
/**
*动态产生消息，并向Channel组推送。
*/
public class DynMessage implements Runnable{
	public static ChannelGroup audiences = new DefaultChannelGroup("msg-group");
	
	static public void addAudience(Channel ch){		
		audiences.add(ch);
	}
	
	static public void removeAudience(Channel ch){
		audiences.remove(ch);
	}
	
	static String[] names = {
		"Tom", "Jerry",
		"Terry", "Looney",
		"Merrie", "William",
		"Joseph", "Hanna",
		"Speike", "Tyke",
		"Tuffy", "Lightning",
	};
	static String message = "";
	
	public static String getMessage(){
		StringBuffer sb = new StringBuffer();
		sb.append("hello,my name is ");
		sb.append(names[new Random().nextInt(names.length)]);
		sb.append(".");		
		return sb.toString();
//		return message;
	}
	@Override
	public void run() {		
		System.out.println("DynMessage start");
		for(;;){
			String msg = getMessage();			
			radiate(msg);
			try{Thread.sleep(1000); }catch(Exception ex){}
		}
	}
	
	void radiate(String msg){
		audiences.write(new TextWebSocketFrame(msg));
	}
}
```
```
<html>
<head>
<script src="jquery-1.9.1.js"></script>
<script src="messagepush.js"></script>
<script >
function doStop(){
	stopMsgPush();
}
function doWsStart(){
	var  r6 = generateMixed(6);
	$("#txtReq").val(r6);
	var  params = $("#txtReq").val();
	doStop();
	
	wsMsgPush('127.0.0.1',params,
		function(data){
			$("#txtResp").val(data);			
		},
		function(){
			$("#txtResp").val("ws close...");
		} ,
		function(){
			$("#txtResp").val("ws error...");
		} );		
}
</script>
</head>
<body>
 
<br/>
<br/><br/>
send: <input id="txtReq" readonly="readonly" type="text" value="" />
<input type="button" value="start" onclick="doWsStart()">
<input type="button" value="stop" onclick="doStop()"/> 
<br/>
recv: <input id="txtResp" type="text" value=""  size="50"/>
</body>
</html>
```
```
var _mp_ws = null;
var _mp_ajax_it = null;
function msgPush(url, params,onmessage,onclose,onerror){
	wsMsgPush(url,params,onmessage,onclose,onerror);
	if(!_mp_ws){
		ajaxMsgPush(url,params,10000,onmessage,onclose,onerror);
	}
}
function old_wsMsgPush(url, params,onmessage,onclose,onerror){	
	var ws = new WebSocket("ws://"+url); 
	ws.onopen = function(){ws.send('1111')};
	ws.onmessage = function(evt){ onmessage(evt.data);};
}
function wsMsgPush(url, params,onmessage,onclose,onerror){	
	_mp_ws = new WebSocket("ws://"+url); 
	if(!_mp_ws){ return; }
		
	_mp_ws.onopen = function(){ 
		_mp_ws.send(params); 
	};
	if(onmessage) _mp_ws.onmessage = function(evt){ onmessage(evt.data); }
	if(onerror) _mp_ws.onerror = function (evt){ onerror(); }
	if(onclose) _mp_ws.onclose = function (evt){ onclose(); }	
}
function ajaxMsgPush(url, params,interval,onmessage,onclose,onerror){	
	function __getmsg(){
		$.ajax({
			url:				url,
			data:			params,
			cache:			true,
			type:			"get",
			dataType:		"text",		
			success:		function(data, textStatus, jqXHR){ 
				if(onmessage) onmessage(data);
			},
			error:			function(jqXHR, textStatus, errorThrown){
				if(onerror) onerror();
			},
			complete:		function(jqXHR, textStatus){
				if(onclose) onclose();
			}
		});
	}	
	
	_mp_ajax_it = setInterval("__getmsg()",interval);
}
function stopMsgPush(){
	if(_mp_ws){
		_mp_ws.send("disconnect");
		_mp_ws.close();
	}
	if(_mp_ajax_it){
		clearInterval(_mp_ajax_it);
	}
}
var chars = ['0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'];
function generateMixed(n) {
     var res = "";
     for(var i = 0; i < n ; i ++) {
         var id = Math.ceil(Math.random()*35);
         res += chars[id];
     }
     return res;
}
```
```java
package org.sl.demo.chatserver;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.handler.codec.http.HttpRequestDecoder;
import org.jboss.netty.handler.codec.http.HttpResponseEncoder;
import org.jboss.netty.handler.timeout.WriteTimeoutHandler;
import org.jboss.netty.util.HashedWheelTimer;
public class PushServerChannelPiplelineFactory  implements ChannelPipelineFactory{
	@Override
	public ChannelPipeline getPipeline() throws Exception {
		ChannelPipeline cp = Channels.pipeline();
		cp.addLast("decoder", new HttpRequestDecoder());
		cp.addLast("encoder", new HttpResponseEncoder());
		cp.addLast("writeTimeout", new WriteTimeoutHandler(new HashedWheelTimer(),10));
		cp.addLast("handler", new PushServerChannelHandler());
		return cp;
	}
}
```
```java
package org.sl.demo.chatserver;
import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;
public class PushServer implements Runnable{
	int port = 80;
	
	public PushServer(int port){
		this.port = port;
	}
	@Override
	public void run() {
		System.out.println("ChatServer "+port);
		
		ServerBootstrap b = new ServerBootstrap(
				new NioServerSocketChannelFactory(
						Executors.newCachedThreadPool(),
						Executors.newCachedThreadPool()));
		b.setOption("child.tcpNoDelay", true);  
		b.setOption("child.keepAlive", true);
		b.setPipelineFactory(new PushServerChannelPiplelineFactory());
		b.bind(new InetSocketAddress(port));
	}
	
	public static void main(String[] args){
		Thread t = new Thread(new DynMessage(),"DynMessage");
		t.start();
		new PushServer(80).run();
	}
}
```
