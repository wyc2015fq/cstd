# Netty-WebSocket长连接推送服务 - z69183787的专栏 - CSDN博客
2016年09月11日 18:38:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8374
转自：[http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256)
November 25, 2015
**推送服务 推送服务**
几种消息推送技术比较
**AJAX轮询** 轮询：缺点，糟糕的用户体验；对服务器压力很大，并造成带宽的极大浪费。
**Comet**：长连接机制，同样由浏览器端主动发起，但Server端以一种很慢的方式给出回应，优点是实时性好，缺点，长时间占用链接，丧失了无状态高并发的缺 点
**Flash XML Socket**，需客户端安装java虚拟机，Chrome浏览器新版本已经默认屏蔽Flash
**DWR**开源框架,将java代码转为js文件，引入js文件路径不能更改，原理也是轮询，
**WebSocket**，HTML5定义，不支持html5的可以使用socket.io，高效节能，简单易用，配合netty可以达到百万级的链接推送。
NIO，java1.4版本，非阻塞IO
**netty**，基于原生NIO实现的高并发框架，配合websocket实现消息推送，netty会单独开一个websocket端口处理请求，并不会占用中间件的连接数，而且一 个线程可以处理几万个链接 
**NIO是什么 **
NIO 是New IO 的简称，在jdk1.4 里提供的新api 。Sun 官方标榜的特性如下：
为所有的原始类型提供 为所有的原始类型提供(Buffer)缓存支持 缓存支持。字符集编码解码解决方案。
**Channel **：一个新的原始I/O 抽象。 支持锁和内存映射文件的文件访问接口。 提供多路(non-bloking) 非阻塞式的高伸缩性网络I/O 。
Channel 通道、Buffer 缓冲区、Selector 选择器
其中Channel对应以前的流 对应以前的流，Buffer不是什么新东西，Selector是因为nio可以使用异步的非堵塞模式才加入的东西。以前的流总是堵塞的，一个线程只要 对它进行操作，其它操作就会被堵塞，也就相当于水管没有阀门，你伸手接水的时候，不管水到了没有，你就都只能耗在接水（流）上。nio的Channel的加 入，相当于增加了水龙头（有阀门），虽然一个时刻也只能接一个水管的水，但依赖轮换策略，在水量不大的时候，各个水管里流出来的水，都可以得到妥善接 纳，这个关键之处就是增加了一个接水工，也就是Selector，他负责协调，也就是看哪根水管有水了的话，在当前水管的水接到一定程度的时候，就切换一
 下：临时关上当前水龙头，试着打开另一个水龙头（看看有没有水）。当其他人需要用水的时候，不是直接去接水，而是事前提了一个水桶给接水工，这个水桶 就是Buffer。也就是，其他人虽然也可能要等，但不会在现场等，而是回家等，可以做其它事去，水接满了，接水工会通知他们 Java NIO 实现百万连接（ 这 段 代 码 只 会 接 受 连 过 来 的 连 接 ， 不 做 任 何 操 作 ） 
**[java]**[view
 plain](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)[copy](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)
- ServerSocketChannel ssc = ServerSocketChannel.open();  
- Selector sel = Selector.open();   
- ssc.configureBlocking(false);  
- ssc.socket().bind(new InetSocketAddress(8080));  
- SelectionKey key = ssc.register(sel, SelectionKey.OP_ACCEPT);   
- while(true) {  
-     sel.select();  
-     Iterator it = sel.selectedKeys().iterator();  
- while(it.hasNext()) {  
-         SelectionKey skey = (SelectionKey)it.next();  
-         it.remove();  
- if(skey.isAcceptable()) {  
-             ch = ssc.accept();  
-         }  
-     }  
- }  
**Netty是什么**
Netty: http://netty.io/ Netty is an asynchronous event-driven network application framework for rapid development of maintainable high performance protocol servers & clients 
官方解释已经够清楚的了，其中最吸引人的就是高性能 高性能了。
 1，普通的服务器10000个连接需要10000个线程，服务器可能就直接卡住了，但对于netty服务器可能几个线程就够了
2，netty是一套在 是一套在java NIO的基础上封装的便于用户开发网络应用程序的 的基础上封装的便于用户开发网络应用程序的api. 应用场景很多,诸如阿里的消息队列(RocketMQ),分布式rpc(Dubbo)通 信层都使用到了netty(dubbo可以用服务发现自由选择通信层)
3，netty是非阻塞事件驱动框架 是非阻塞事件驱动框架, 并结合线程组(group)的概念,可以很好的支持高并发,慢连接的场景
4，编程接口非常容易,并且也较好的解决了TCP粘包/拆包的问题.netty提供了自己的ByteBuf和channel,相比于jdk的ByteBuffer和channel来说更简便灵 活操作, 并提供了pipeline的概念,并针对每个contextHandler都可以由用户定义, 方便直接.
5，有一些web框架已经开始直接使用netty做为底层通信服务,诸如play. 这样play就不用依赖于容器去进行部署,在没有nginx做反向代理的情况下也能支持 高并发.编解码器可以随意扩展,今天是个web，明天就可以是一个ftp或email服务器,个人觉得比较灵活。
6，相对于Tomcat这种Web Server（顾名思义主要是提供Web协议相关的服务的），Netty是一个 是一个Network Server，是处于Web Server更下层的网络框 架，也就是说你可以使用Netty模仿Tomcat做一个提供HTTP服务的Web容器。简而言之，Netty通过使用NIO的很多新特性，对TCP/UDP编程进行了简化和封 装，提供了更容易使用的网络编程接口，让你可以根据自己的需要封装独特的HTTP
 Server活着FTP Server等.
Netty更多的链接 更多的链接
非阻塞IO
其实无论是用Java NIO 还是用Netty，达到百万链接都没有任何难度，因为他们都是非阻塞的IO，不需要为每个链接创建一个线程了。
预知详情，可以搜索一下BIO,NIO,AIO的相关知识。
Netty 实现百万连接（ 这 段 代 码 只 会 接 受 连 过 来 的 连 接 ） 
**[java]**[view
 plain](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)[copy](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)
- NioEventLoopGroup bossGroup =  new NioEventLoopGroup();  
- NioEventLoopGroup workerGroup= new NioEventLoopGroup();  
- ServerBootstrap bootstrap = new ServerBootstrap();  
- bootstrap.group(bossGroup, workerGroup);   
- bootstrap.channel( NioServerSocketChannel.class);   
- bootstrap.childHandler(new ChannelInitializer<SocketChannel>() {  
- @Overrideprotectedvoid initChannel(SocketChannel ch) throws Exception {  
-         ChannelPipeline pipeline = ch.pipeline();  
- //todo: add handler
-     }});  
- bootstrap.bind(8080).sync();   
**WebSocket是什么**
WebSocket是HTML5出的东西（协议），也就是说HTTP协议没有变化，或者说没关系，但HTTP是不支持持久连接的（长连接，循环连接的不算）
首先，Websocket是一个持久化的协议，相对于HTTP这种非持久的协议来说。简单的举个例子吧，用目前应用比较广泛的PHP生命周期来解释。
1) HTTP的生命周期通过Request来界定，也就是一个Request 一个Response，那么在HTTP1.0中，这次HTTP请求就结束了。在HTTP1.1中进行了改进，使 得有一个keep-alive，也就是说，在一个HTTP连接中，可以发送多个Request，接收多个Response。但是请记住 Request = Response ， 在HTTP中永 远是这样，也就是说一个request只能有一个response。而且这个response也是被动的，不能主动发起。
Websocket的作用 在讲Websocket之前，我就顺带着讲下 long poll 和 ajax轮询 的原理。 
首先是 ajax轮询 ，ajax轮询 的原理非常简单，让浏览器隔个几秒就发送一次请求，询问服务器是否有新信息。 
场景再现： 客户端：啦啦啦，有没有新信息(Request) 
服务端：没有（Response） 
客户端：啦啦啦，有没有新信息(Request) 
服务端：没有。。（Response） 
客户端：啦啦啦，有没有新信息(Request) 
服务端：你好烦啊，没有啊。。（Response）
 客户端：啦啦啦，有没有新消息（Request） 
服务端：好啦好啦，有啦给你。（Response） 
客户端：啦啦啦，有没有新消息（Request） 
服务端：。。。。。没。。。。没。。。没有（Response） ---- loop
long poll long poll 其实原理跟 ajax轮询 差不多，都是采用轮询的方式，不过采取的是阻塞模型（一直打电话，没收到就不挂电话），也就是说，客户端发起连接 后，如果没消息，就一直不返回Response给客户端。直到有消息才返回，返回完之后，客户端再次建立连接，周而复始。 
场景再现 客户端：啦啦啦，有没有新信息，没有的话就等有了才返回给我吧（Request） 
服务端：额。。 等待到有消息的时候。。来 给你（Response） 
客户端：啦啦啦，有没有新信息，没有的话就等有了才返回给我吧（Request） -loop
从上面可以看出其实这两种方式，都是在不断地建立HTTP连接，然后等待服务端处理，可以体现HTTP协议的另外一个特点，被动性。 何为被动性呢，其实就是，服务端不能主动联系客户端，只能有客户端发起。 简单地说就是，服务器是一个很懒的冰箱（这是个梗）（不会、不能主动发起连接），但是上司有命令，如果有客户来，不管多么累都要好好接待。
说完这个，我们再来说一说上面的缺陷 从上面很容易看出来，不管怎么样，上面这两种都是非常消耗资源的。 
ajax轮询 需要服务器有很快的处理速度和资源。（速度） long poll 需要有很高的并发，也就是说同时接待客户的能力。（场地大小） 所以ajax轮询 和long poll 都有可能发生这种情况。
客户端：啦啦啦啦，有新信息么？ 
客户端：啦啦啦啦，有新信息么？ 
服务端：月线正忙，请稍后再试（ 服务端：月线正忙，请稍后再试（503 Server Unavailable） ） 
客户端：。。。。好吧，啦啦啦，有新信息么？ 
客户端：。。。。好吧，啦啦啦，有新信息么？ 
服务端：月线正忙，请稍后再试（ 服务端：月线正忙，请稍后再试（503 Server Unavailable） ）
通过上面这个例子，我们可以看出，这两种方式都不是最好的方式，需要很多资源。 一种需要更快的速度，一种需要更多的'电话'。这两种都会导致'电话'的需求越来越高。 哦对了，忘记说了HTTP还是一个状态协议。 通俗的说就是，服务器因为每天要接待太多客户了，是个健忘鬼 健忘鬼，你一挂电话，他就把你的东西全忘光了，把你的东西全丢掉了。你第二次还得再告诉服务 器一遍。
所以在这种情况下出现了，Websocket出现了。 他解决了HTTP的这几个难题。 首先，被动性 被动性，当服务器完成协议升级后（HTTP->Websocket），服务端就可以主动推送信息给客户端啦。 所以上面的情景可以做如下修改。 
客户端：啦啦啦，我要建立Websocket协议，需要的服务：chat，Websocket协议版本：17（HTTP Request） 
服务端：ok，确认，已升级为Websocket协议（HTTP Protocols Switched） 
客户端：麻烦你有信息的时候推送给我噢。。 
服务端：ok，有的时候会告诉你的。 
服务端：balabalabalabala 
服务端：balabalabalabala 
服务端：哈哈哈哈哈啊哈哈哈哈 
服务端：笑死我了哈哈哈哈哈哈哈 
浏览器打开 浏览器打开websocket代码 代码
**[javascript]**[view
 plain](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)[copy](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)
- var socket;  
- if(!window.WebSocket){  
-   window.WebSocket = window.MozWebSocket;  
- }   
- if(window.WebSocket){  
-   socket = new WebSocket("ws://10.0.53.219:7397/websocket");  
-   socket.onmessage = function(event){  
- var ta = document.getElementById('responseText');  
-                    ta.value += event.data+"\r\n";  
-   };  
-   socket.onopen = function(event){  
- var ta = document.getElementById('responseText');  
-                    ta.value = "这里显示服务器推送信息"+"\r\n";  
- 
-   };  
- socket.onclose = function(event){  
- var ta = document.getElementById('responseText');  
-                    ta.value = "";  
-                    ta.value = "WebSocket 关闭"+"\r\n";  
-   };  
- }else{  
-          alert("您的浏览器不支持WebSocket协议！");  
- }   
**不支持websocket的浏览器处理方法**
[点击打开链接](https://github.com/gimite/web-socket-js/) https://github.com/gimite/web-socket-js/
复制 swfobject.js, web_socket.js, WebSocketMain.swf到项目中
**[javascript]**[view
 plain](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)[copy](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)
- <span style="font-size:10px;"><script type="text/javascript" src="swfobject.js"></script>  
- <script type="text/javascript" src="web_socket.js"></script>  
- 
- <script type="text/javascript">  
- 
- // Let the library know where WebSocketMain.swf is:
-   WEB_SOCKET_SWF_LOCATION = "WebSocketMain.swf";  
- 
- // Write your code in the same way as for native WebSocket:
- var ws = new WebSocket("ws://example.com:10081/");  
-   ws.onopen = function() {  
-     ws.send("Hello");  // Sends a message.
-   };  
-   ws.onmessage = function(e) {  
- // Receives a message.
-     alert(e.data);  
-   };  
-   ws.onclose = function() {  
-     alert("closed");  
-   };  
- 
- </script></span>  
附录： 附录：netty+websocket即时聊天代码 即时聊天代码
Global.java
**[java]**[view
 plain](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)[copy](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)
- package com.fesco.netty.websocket.common;   
- import io.netty.channel.group.ChannelGroup;  
- import io.netty.channel.group.DefaultChannelGroup;  
- import io.netty.util.concurrent.GlobalEventExecutor;  
- publicfinalclass Global {  
- 
- publicstatic ChannelGroup group = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);  
- 
- }   
- ChildChannelHandler.java   
- package com.fesco.netty.websocket;   
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.socket.SocketChannel;  
- import io.netty.handler.codec.http.HttpObjectAggregator;  
- import io.netty.handler.codec.http.HttpServerCodec;  
- import io.netty.handler.stream.ChunkedWriteHandler;   
- publicclass ChildChannelHandler extends ChannelInitializer<SocketChannel> {   
- @Override
- protectedvoid initChannel(SocketChannel ch) throws Exception {  
-                    ch.pipeline().addLast("http-codec", new HttpServerCodec());  
-                    ch.pipeline().addLast("aggregator",new HttpObjectAggregator(65535));  
-                    ch.pipeline().addLast("http-chunked",new ChunkedWriteHandler());  
-                    ch.pipeline().addLast("handler",new MyWebSocketServerHandler());  
-          }   
- }     
- MyWebSocketServerHandler.java   
- package com.fesco.netty.websocket;   
- import io.netty.buffer.ByteBuf;  
- import io.netty.buffer.Unpooled;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelFutureListener;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.SimpleChannelInboundHandler;  
- import io.netty.handler.codec.http.DefaultFullHttpResponse;  
- import io.netty.handler.codec.http.FullHttpRequest;  
- import io.netty.handler.codec.http.HttpResponseStatus;  
- import io.netty.handler.codec.http.HttpVersion;  
- import io.netty.handler.codec.http.websocketx.CloseWebSocketFrame;  
- import io.netty.handler.codec.http.websocketx.PingWebSocketFrame;  
- import io.netty.handler.codec.http.websocketx.PongWebSocketFrame;  
- import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;  
- import io.netty.handler.codec.http.websocketx.WebSocketFrame;  
- import io.netty.handler.codec.http.websocketx.WebSocketServerHandshaker;  
- import io.netty.handler.codec.http.websocketx.WebSocketServerHandshakerFactory;  
- import io.netty.util.CharsetUtil;   
- import java.util.Calendar;  
- import java.util.GregorianCalendar;  
- import java.util.logging.Level;  
- import java.util.logging.Logger;   
- import com.fesco.netty.websocket.common.Global;   
- publicclass MyWebSocketServerHandler extends SimpleChannelInboundHandler<Object>{   
- privatestaticfinal Logger logger = Logger.getLogger(WebSocketServerHandshaker.class.getName());  
- private WebSocketServerHandshaker handshaker;  
- 
- 
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) throws Exception {  
-                    Global.group.add(ctx.channel());  
-                    System.out.println("客户端与服务器段开启");  
-          }   
- @Override
- publicvoid channelInactive(ChannelHandlerContext ctx) throws Exception {  
-                    Global.group.remove(ctx.channel());  
-                    System.out.println("客户端与服务器链接关闭！");  
-          }   
- @Override
- protectedvoid messageReceived(ChannelHandlerContext ctx, Object msg)  
- throws Exception {  
- if(msg instanceof FullHttpRequest){  
-                             handleHttpRequest(ctx, ((FullHttpRequest) msg));  
-                    }elseif(msg instanceof WebSocketFrame){  
-                             handlerWebSocketFrame(ctx, (WebSocketFrame) msg);  
-                    }  
-          }   
- @Override
- publicvoid channelReadComplete(ChannelHandlerContext ctx) throws Exception {  
-                    ctx.flush();  
-          }   
- privatevoid handlerWebSocketFrame(ChannelHandlerContext ctx,  
-                             WebSocketFrame frame) {  
- 
- if(frame instanceof CloseWebSocketFrame){  
-                             handshaker.close(ctx.channel(), (CloseWebSocketFrame)frame.retain());  
-                    }  
- if(frame instanceof PingWebSocketFrame){  
-                             ctx.channel().write(new PongWebSocketFrame(frame.content().retain()));  
-                    }  
- 
- if(!(frame instanceof TextWebSocketFrame)){  
-                             System.err.println("本例仅支持文本！");  
- thrownew UnsupportedOperationException(String.format(  
- "%s frame types not supported", frame.getClass().getName()));  
-                    }  
- 
-                    String request = ((TextWebSocketFrame)frame).text();  
-                    System.out.println("服务器收到:" + request);  
- if (logger.isLoggable(Level.FINE)) {  
-                             logger.fine(String.format("%s received %s", ctx.channel(),request));  
-                    }  
- 
- 
-                    TextWebSocketFrame tws = new TextWebSocketFrame(getDateTime()  
-                                      + "(" +ctx.channel().remoteAddress() + ") ：" + request);  
- // 群发
-                    Global.group.writeAndFlush(tws);  
- // 返回【谁发的发给谁】
- // ctx.channel().writeAndFlush(tws);
-          }   
- privatevoid handleHttpRequest(ChannelHandlerContext ctx,FullHttpRequest req) {  
- 
- if(!req.getDecoderResult().isSuccess() ||  
-                                      !("websocket".equals(req.headers().get("Upgrade")))){  
-                             sendHttpResponse(ctx, req, new DefaultFullHttpResponse(  
-                                                HttpVersion.HTTP_1_1, HttpResponseStatus.BAD_REQUEST));  
- return;  
-                    }  
- 
-                    WebSocketServerHandshakerFactory wsFactory = new WebSocketServerHandshakerFactory("ws://localhost:7397/websocket", null, false);  
-                    handshaker = wsFactory.newHandshaker(req);  
- if(null == handshaker){  
-                             WebSocketServerHandshakerFactory.sendUnsupportedWebSocketVersionResponse(ctx.channel());  
-                    }else{  
-                             handshaker.handshake(ctx.channel(), req);  
-                    }  
- 
-          }   
- privatevoid sendHttpResponse(ChannelHandlerContext ctx,  
-                             FullHttpRequest req, DefaultFullHttpResponse res) {  
- if(res.getStatus().code() != 200){  
-                             ByteBuf buf = Unpooled.copiedBuffer(res.getStatus().toString(),  
-                                                CharsetUtil.UTF_8);  
-                             res.content().writeBytes(buf);  
-                             buf.release();  
-                    }  
- 
-                    ChannelFuture future = ctx.channel().writeAndFlush(res);  
- if (!isKeepAlive(req) || res.getStatus().code() != 200) {  
-                             future.addListener(ChannelFutureListener.CLOSE);  
-                    }  
- 
-          }  
- 
- privatestaticboolean isKeepAlive(FullHttpRequest req){  
- returnfalse;  
-          }  
- 
- @Override
- publicvoid exceptionCaught(ChannelHandlerContext ctx, Throwable cause)  
- throws Exception {  
-                    cause.printStackTrace();  
-                    ctx.close();  
-          }    
- private String getDateTime() {  
- // Calendar calendar = Calendar.getInstance();
-                    Calendar calendar = new GregorianCalendar();  
-                    java.util.Date date = new java.util.Date();  
-                    calendar.setTime(date);  
-                    String sHour = null;  
-                    String sMinute = null;  
-                    String sSecond = null;  
-                    String sYear = null;  
-                    String sMonth = null;  
-                    String sDay = null;  
- int year = calendar.get(Calendar.YEAR);  
- int month = calendar.get(Calendar.MONTH) + 1;  
- int day = calendar.get(Calendar.DATE);  
- int hour = calendar.get(Calendar.HOUR_OF_DAY);  
- int minute = calendar.get(Calendar.MINUTE);  
- int second = calendar.get(Calendar.SECOND);  
- int milliSecond = calendar.get(Calendar.MILLISECOND);   
-                    sYear = String.valueOf(year);  
- if (month < 10) {  
-                             sMonth = "0" + month;  
-                    } else
-                             sMonth = String.valueOf(month);  
- if (day < 10) {  
-                             sDay = "0" + day;  
-                    } else
-                             sDay = String.valueOf(day);   
- if (hour < 10) {  
-                             sHour = "0" + hour;  
-                    } else {  
-                             sHour = String.valueOf(hour);  
-                    }   
- if (minute < 10) {  
-                             sMinute = "0" + minute;  
-                    } else {  
-                             sMinute = String.valueOf(minute);  
-                    }   
- if (second < 10) {  
-                             sSecond = "0" + second;  
-                    } else {  
-                             sSecond = String.valueOf(second);  
-                    }   
- return sYear + "-" + sMonth + "-" + sDay + " " + sHour + ":" + sMinute + ":" + sSecond;  
-          }    
- }   
NettyServer.java 
**[java]**[view
 plain](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)[copy](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)
- package com.fesco.netty.websocket;   
- import java.net.InetSocketAddress;   
- import io.netty.bootstrap.ServerBootstrap;  
- import io.netty.channel.Channel;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.nio.NioServerSocketChannel;   
- publicclass NettyServer {  
- publicvoid run(){  
-                    EventLoopGroup boosGroup = new NioEventLoopGroup();  
-                    EventLoopGroup workGroup = new NioEventLoopGroup();  
- 
- try {  
-                             ServerBootstrap bootstrap = new ServerBootstrap();  
-                             bootstrap.group(boosGroup, workGroup);  
-                             bootstrap.channel(NioServerSocketChannel.class);  
-                             bootstrap.childHandler(new ChildChannelHandler());  
-                             System.err.println("服务器开启待客户端链接.....");  
-                             Channel ch = bootstrap.bind(new InetSocketAddress("10.0.53.219", 7397)).sync().channel();  
-                             ch.closeFuture().sync();  
-                    } catch (Exception e) {  
-                             e.printStackTrace();  
-                    }finally{  
-                             boosGroup.shutdownGracefully();  
-                             workGroup.shutdownGracefully();  
-                    }  
-          }  
- 
- publicstaticvoid main(String[] args){  
- new NettyServer().run();  
-          }  
- 
- }   
ClinetHtml.html 
**[html]**[view
 plain](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)[copy](http://blog.csdn.net/sinosoft_fesco_12138/article/details/50380256#)
- <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1transitional.dtd">
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=utf-8"/>
- <title>无标题文档</title>
- <scripttype="text/javascript">
-   var socket;  
-   if(!window.WebSocket){  
- window.WebSocket = window.MozWebSocket;  
-   }   
-   if(window.WebSocket){  
- socket = new WebSocket("ws://10.0.53.219:7397/websocket");  
- socket.onmessage = function(event){  
-                             var ta = document.getElementById('responseText');  
-                             ta.value += event.data+"\r\n";  
-            };  
- socket.onopen = function(event){  
-                             var ta = document.getElementById('responseText');  
- ta.value = "这里显示服务器推送信息"+"\r\n";  
- 
-            };   
- socket.onclose = function(event){  
-                             var ta = document.getElementById('responseText');  
- ta.value = "";  
- ta.value = "WebSocket 关闭"+"\r\n";  
-            };  
-   }else{  
-                    alert("您的浏览器不支持WebSocket协议！");  
-   }   
-   function send(message){  
-          if(!window.WebSocket){return;}  
-          if(socket.readyState == WebSocket.OPEN){  
-                    socket.send(message);  
-          }else{  
-                    alert("WebSocket 连接没有建立成功！");  
-          }  
- 
-   }  
- 
- </script>
- </head>
- <body>
- <formonSubmit="return false;">
- <inputtype="text"name="message"value="这里输入消息"/><br/>
- <br/><inputtype="button"value="发送 WebSocket 请求消息"
- onClick="send(this.form.message.value)"/>
- <hrcolor="blue"/>
- <h3>服务端返回的应答消息</h3>
- <textareaid="responseText"style="width: 1024px;height: 300px;"></textarea>
- </form>
- </body>
- </html>
- 
