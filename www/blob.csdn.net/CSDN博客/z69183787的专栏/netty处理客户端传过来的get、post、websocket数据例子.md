# netty处理客户端传过来的get、post、websocket数据例子 - z69183787的专栏 - CSDN博客
2016年09月11日 18:08:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4331
用netty作为http服务器，在服务器端读取客户端传过来的get和post参数和websocket数据例子，netty版本3.6.6
```java
package com.penngo.http;
import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;
public class HttpServer {
    private final int port;
    public HttpServer(int port) {
        this.port = port;
    }
    public void run() {
        ServerBootstrap bootstrap = new ServerBootstrap(new NioServerSocketChannelFactory(
                Executors.newCachedThreadPool(), Executors.newCachedThreadPool()));
        bootstrap.setPipelineFactory(new HttpServerPipelineFactory());
        bootstrap.bind(new InetSocketAddress(port));
        System.out.println("Web socket server started at port " + port + '.');
        System.out.println("Open your browser and navigate to http://localhost:" + port + '/');
    }
    public static void main(String[] args) {
        int port;
        if (args.length > 0) {
            port = Integer.parseInt(args[0]);
        } else {
            port = 8080;
        }
        new HttpServer(port).run();
    }
}
```
```java
package com.penngo.http;
import java.util.List;
import java.util.Map;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelFutureListener;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.handler.codec.http.DefaultHttpResponse;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.jboss.netty.handler.codec.http.HttpResponse;
import org.jboss.netty.handler.codec.http.QueryStringDecoder;
import org.jboss.netty.handler.codec.http.multipart.Attribute;
import org.jboss.netty.handler.codec.http.multipart.DefaultHttpDataFactory;
import org.jboss.netty.handler.codec.http.multipart.HttpPostRequestDecoder;
import org.jboss.netty.handler.codec.http.multipart.InterfaceHttpData;
import org.jboss.netty.handler.codec.http.multipart.InterfaceHttpData.HttpDataType;
import org.jboss.netty.handler.codec.http.websocketx.CloseWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.PingWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.PongWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.WebSocketFrame;
import org.jboss.netty.handler.codec.http.websocketx.WebSocketServerHandshaker;
import org.jboss.netty.handler.codec.http.websocketx.WebSocketServerHandshakerFactory;
import org.jboss.netty.util.CharsetUtil;
import static org.jboss.netty.handler.codec.http.HttpHeaders.Names.*;
import static org.jboss.netty.handler.codec.http.HttpHeaders.*;
import static org.jboss.netty.handler.codec.http.HttpMethod.*;
import static org.jboss.netty.handler.codec.http.HttpResponseStatus.*;
import static org.jboss.netty.handler.codec.http.HttpVersion.*;
public class HttpServerHandler extends SimpleChannelUpstreamHandler {
	private WebSocketServerHandshaker handshaker;
	private static final String WEBSOCKET_PATH = "/websocket";
	@Override
	public void messageReceived(ChannelHandlerContext ctx, MessageEvent e)
			throws Exception {
		Object msg = e.getMessage();
		if (msg instanceof HttpRequest) {
			handleHttpRequest(ctx, (HttpRequest) msg);
		} else if (msg instanceof WebSocketFrame) {
			handleWebSocketFrame(ctx, (WebSocketFrame) msg);
		}
	}
	private void handleHttpRequest(ChannelHandlerContext ctx, HttpRequest req)
			throws Exception {
		System.out.println("handleHttpRequest method==========" + req.getMethod());
		System.out.println("handleHttpRequest uri==========" + req.getUri());
		if (req.getMethod() == GET) { // 处理get请求
			if(req.getUri().equals("/websocket")){
				  // Handshake
		        WebSocketServerHandshakerFactory wsFactory = new WebSocketServerHandshakerFactory(
		                getWebSocketLocation(req), null, false);
		        handshaker = wsFactory.newHandshaker(req);
		        if (handshaker == null) {
		            wsFactory.sendUnsupportedWebSocketVersionResponse(ctx.getChannel());
		        } else {
		            handshaker.handshake(ctx.getChannel(), req).addListener(WebSocketServerHandshaker.HANDSHAKE_LISTENER);
		        }
			}
			else{
				QueryStringDecoder decoder = new QueryStringDecoder(req.getUri());
				Map<String, List<String>> parame = decoder.getParameters();
				List<String> q = parame.get("q"); // 读取从客户端传过来的参数
				String question = q.get(0);
				if (question != null && !question.equals("")) {
					System.out.println("r :" + question);
					HttpResponse res = new DefaultHttpResponse(HTTP_1_1, OK);
					String data = "<html><body>你好，GET</body><html>";
					ChannelBuffer content = ChannelBuffers.copiedBuffer(data,
							CharsetUtil.UTF_8);
					res.setHeader(CONTENT_TYPE, "text/html; charset=UTF-8");
					res.setHeader(ACCESS_CONTROL_ALLOW_ORIGIN, "*");
					setContentLength(res, content.readableBytes());
					res.setContent(content);
					sendHttpResponse(ctx, req, res);
				}
			}
		}
		if (req.getMethod() == POST) { // 处理POST请求
			HttpPostRequestDecoder decoder = new HttpPostRequestDecoder(
					new DefaultHttpDataFactory(false), req);
			InterfaceHttpData postData = decoder.getBodyHttpData("q"); // //
																		// 读取从客户端传过来的参数
			String question = "";
			if (postData.getHttpDataType() == HttpDataType.Attribute) {
				Attribute attribute = (Attribute) postData;
				question = attribute.getValue();
				System.out.println("q:" + question);
			}
			if (question != null && !question.equals("")) {
				HttpResponse res = new DefaultHttpResponse(HTTP_1_1, OK);
				String data = "<html><body>你好，POST</body><html>";
				ChannelBuffer content = ChannelBuffers.copiedBuffer(data,
						CharsetUtil.UTF_8);
				res.setHeader(CONTENT_TYPE, "text/html; charset=UTF-8");
				res.setHeader(ACCESS_CONTROL_ALLOW_ORIGIN, "*");
				setContentLength(res, content.readableBytes());
				res.setContent(content);
				sendHttpResponse(ctx, req, res);
			}
			return;
		}
	}
	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e)
			throws Exception {
		e.getCause().printStackTrace();
		e.getChannel().close();
	}
	   private void handleWebSocketFrame(ChannelHandlerContext ctx, WebSocketFrame frame) {
	        if (frame instanceof CloseWebSocketFrame) {
	            handshaker.close(ctx.getChannel(), (CloseWebSocketFrame) frame);
	            return;
	        }
	        if (frame instanceof PingWebSocketFrame) {
	            ctx.getChannel().write(new PongWebSocketFrame(frame.getBinaryData()));
	            return;
	        }
	        if (!(frame instanceof TextWebSocketFrame)) {
	            throw new UnsupportedOperationException(String.format("%s frame types not supported", frame.getClass()
	                    .getName()));
	        }
	        String request = ((TextWebSocketFrame) frame).getText();
	        System.out.println("收到socket msg=" + request);
	        request = "这是来自服务器端的数据";
	        ctx.getChannel().write(new TextWebSocketFrame(request.toUpperCase()));
	    }
	private static void sendHttpResponse(ChannelHandlerContext ctx,
			HttpRequest req, HttpResponse res) {
		if (res.getStatus().getCode() != 200) {
			res.setContent(ChannelBuffers.copiedBuffer(res.getStatus()
					.toString(), CharsetUtil.UTF_8));
			setContentLength(res, res.getContent().readableBytes());
		}
		ChannelFuture f = ctx.getChannel().write(res);
		if (!isKeepAlive(req) || res.getStatus().getCode() != 200) {
			f.addListener(ChannelFutureListener.CLOSE);
		}
	}
	 private static String getWebSocketLocation(HttpRequest req) {
	        return "ws://" + req.getHeader(HOST) + WEBSOCKET_PATH;
	    }
}
```
```java
package com.penngo.http;
import static org.jboss.netty.channel.Channels.*;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.handler.codec.http.HttpChunkAggregator;
import org.jboss.netty.handler.codec.http.HttpRequestDecoder;
import org.jboss.netty.handler.codec.http.HttpResponseEncoder;
public class HttpServerPipelineFactory implements ChannelPipelineFactory {
    public ChannelPipeline getPipeline() throws Exception {
        // Create a default pipeline implementation.
        ChannelPipeline pipeline = pipeline();
        pipeline.addLast("decoder", new HttpRequestDecoder());
//        pipeline.addLast("aggregator", new HttpChunkAggregator(65536));
        pipeline.addLast("encoder", new HttpResponseEncoder());
        pipeline.addLast("handler", new HttpServerHandler());
        return pipeline;
    }
}
```
测试代码
```
<span style="font-size:14px;"><!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>Web Socket Example</title>
        <script src="http://ajax.aspnetcdn.com/ajax/jQuery/jquery-1.8.3.min.js" type="text/javascript"></script>
        <style>
        textarea {
    width: 500px;
    height: 100px;
}
        </style>
        <script>
        $(function(){
            var Sock = function() {
         var socket;
         if (!window.WebSocket) {
             window.WebSocket = window.MozWebSocket;
         }
         if (window.WebSocket) {
             socket = new WebSocket("ws://localhost:8080/websocket");
             socket.onopen = onopen;
             socket.onmessage = onmessage;
             socket.onclose = onclose;
         } else {
             alert("Your browser does not support Web Socket.");
         }
         function onopen(event) {
             getTextAreaElement().value = "Web Socket opened!";
         }
         function onmessage(event) {
             appendTextArea(event.data);
         }
         function onclose(event) {
             appendTextArea("Web Socket closed");
         }
         function appendTextArea(newData) {
             var el = getTextAreaElement();
             el.value = el.value + '\n' + newData;
         }
         function getTextAreaElement() {
             return document.getElementById('responseText');
         }
         function send(event) {
             event.preventDefault();
             if (window.WebSocket) {
         if (socket.readyState == WebSocket.OPEN) {
             socket.send(event.target.message.value);
         } else {
             alert("The socket is not open.");
         }
             }
         }
         document.forms.inputform.addEventListener('submit', send, false);
            }
            window.addEventListener('load', function() {
         new Sock();
            }, false);
            
            $("#btnGet").click( function () {
             $.get("http://localhost:8080/", { q: "John"},
             function(data){
           $("#responseTextGet").val($("#responseTextGet").val() + data)
             });
            });
$("#btnPost").click( function () {
$.post("http://localhost:8080/", { q: "John"},
  function(data){
$("#responseTextPost").val($("#responseTextGet").val() + data)
  });
            });
        });
       
        </script>
    </head>
    <body>
        <h3>输入内容</h3>
        <form name="inputform">
            <input type="text" name="message" id="message" placeholder="Enter text to be sent" autofocus>
            <input type="submit" value="发送WebSocket数据">
        </form>
        <h3>服务端返回</h3>
        <textarea id="responseText"></textarea>
        <h3>输入内容</h3>
        <form name="inputform">
            <input type="text" name="message" id="messageGet" placeholder="Enter text to be sent" autofocus>
            <input type="button" value="Get数据" id="btnGet">
        </form>
        <h3>服务端返回</h3>
        <textarea id="responseTextGet"></textarea>
        <h3>输入内容</h3>
        <form name="inputform">
            <input type="text" name="message" id="messagePost" placeholder="Enter text to be sent" autofocus>
            <input type="button" value="Post数据" id="btnPost">
        </form>
        <h3>服务端返回</h3>
        <textarea id="responseTextPost"></textarea>
    </body>
</html>
</span>
```
