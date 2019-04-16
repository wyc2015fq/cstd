# Java后端WebSocket的Tomcat实现 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Java后端WebSocket的Tomcat实现](https://www.cnblogs.com/xdp-gacl/p/5193279.html)



## 一.WebSocket简单介绍

　　随着互联网的发展，传统的HTTP协议已经很难满足Web应用日益复杂的需求了。近年来，随着HTML5的诞生，WebSocket协议被提出，它实现了浏览器与服务器的全双工通信，扩展了浏览器与服务端的通信功能，使服务端也能主动向客户端发送数据。

　　我们知道，传统的HTTP协议是无状态的，每次请求（request）都要由客户端（如 浏览器）主动发起，服务端进行处理后返回response结果，而服务端很难主动向客户端发送数据；这种客户端是主动方，服务端是被动方的传统Web模式 对于信息变化不频繁的Web应用来说造成的麻烦较小，而对于涉及实时信息的Web应用却带来了很大的不便，如带有即时通信、实时数据、订阅推送等功能的应 用。在WebSocket规范提出之前，开发人员若要实现这些实时性较强的功能，经常会使用折衷的解决方法：**轮询（polling）**和**Comet**技术。其实后者本质上也是一种轮询，只不过有所改进。


**　　轮询**是最原始的实现实时Web应用的解决方案。轮询技术要求客户端以设定的时间间隔周期性地向服务端发送请求，频繁地查询是否有新的数据改动。明显地，这种方法会导致过多不必要的请求，浪费流量和服务器资源。

**　　Comet技术**又可以分为**长轮询**和**流技术**。**长轮询**改进了上述的轮询技术，减小了无用的请求。它会为某些数据设定过期时间，当数据过期后才会向服务端发送请求；这种机制适合数据的改动不是特别频繁的情况。**流技术**通常是指客户端使用一个隐藏的窗口与服务端建立一个HTTP长连接，服务端会不断更新连接状态以保持HTTP长连接存活；这样的话，服务端就可以通过这条长连接主动将数据发送给客户端；流技术在大并发环境下，可能会考验到服务端的性能。


　　这两种技术都是基于请求-应答模式，都不算是真正意义上的实时技术；它们的每一次请求、应答，都浪费了一定流量在相同的头部信息上，并且开发复杂度也较大。

　　伴随着HTML5推出的WebSocket，真正实现了Web的实时通信，使B/S模式具备了C/S模式的实时通信能力。WebSocket的工作流程是这
样的：浏览器通过JavaScript向服务端发出建立WebSocket连接的请求，在WebSocket连接建立成功后，客户端和服务端就可以通过
TCP连接传输数据。因为WebSocket连接本质上是TCP连接，不需要每次传输都带上重复的头部数据，所以它的数据传输量比轮询和Comet技术小
了很多。本文不详细地介绍WebSocket规范，主要介绍下WebSocket在Java Web中的实现。

　　JavaEE
 7中出了JSR-356:Java API for 
WebSocket规范。不少Web容器，如Tomcat,Nginx,Jetty等都支持WebSocket。Tomcat从7.0.27开始支持
WebSocket，从7.0.47开始支持JSR-356，下面的Demo代码也是需要部署在**Tomcat7.0.47**以上的版本才能运行。

## 二.WebSocket示例

### 2.1.新建JavaWeb测试项目

![](data:image/png;base64,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)

　　在pom.xml中添加Jar包依赖

```
1 <dependency>
2         <groupId>javax</groupId>
3         <artifactId>javaee-api</artifactId>
4         <version>7.0</version>
5         <scope>provided</scope>
6 </dependency>
```

客户端（Web主页）代码：

```
1 <%@ page language="java" pageEncoding="UTF-8" %>
 2 <!DOCTYPE html>
 3 <html>
 4 <head>
 5     <title>Java后端WebSocket的Tomcat实现</title>
 6 </head>
 7 <body>
 8     Welcome<br/><input id="text" type="text"/>
 9     <button onclick="send()">发送消息</button>
10     <hr/>
11     <button onclick="closeWebSocket()">关闭WebSocket连接</button>
12     <hr/>
13     <div id="message"></div>
14 </body>
15 
16 <script type="text/javascript">
17     var websocket = null;
18     //判断当前浏览器是否支持WebSocket
19     if ('WebSocket' in window) {
20         websocket = new WebSocket("ws://localhost:8080/websocket");
21     }
22     else {
23         alert('当前浏览器 Not support websocket')
24     }
25 
26     //连接发生错误的回调方法
27     websocket.onerror = function () {
28         setMessageInnerHTML("WebSocket连接发生错误");
29     };
30 
31     //连接成功建立的回调方法
32     websocket.onopen = function () {
33         setMessageInnerHTML("WebSocket连接成功");
34     }
35 
36     //接收到消息的回调方法
37     websocket.onmessage = function (event) {
38         setMessageInnerHTML(event.data);
39     }
40 
41     //连接关闭的回调方法
42     websocket.onclose = function () {
43         setMessageInnerHTML("WebSocket连接关闭");
44     }
45 
46     //监听窗口关闭事件，当窗口关闭时，主动去关闭websocket连接，防止连接还没断开就关闭窗口，server端会抛异常。
47     window.onbeforeunload = function () {
48         closeWebSocket();
49     }
50 
51     //将消息显示在网页上
52     function setMessageInnerHTML(innerHTML) {
53         document.getElementById('message').innerHTML += innerHTML + '<br/>';
54     }
55 
56     //关闭WebSocket连接
57     function closeWebSocket() {
58         websocket.close();
59     }
60 
61     //发送消息
62     function send() {
63         var message = document.getElementById('text').value;
64         websocket.send(message);
65     }
66 </script>
67 </html>
```

Java Web后端代码

```
1 package me.gacl.websocket;
 2 
 3 import java.io.IOException;
 4 import java.util.concurrent.CopyOnWriteArraySet;
 5 
 6 import javax.websocket.*;
 7 import javax.websocket.server.ServerEndpoint;
 8 
 9 /**
10  * @ServerEndpoint 注解是一个类层次的注解，它的功能主要是将目前的类定义成一个websocket服务器端,
11  * 注解的值将被用于监听用户连接的终端访问URL地址,客户端可以通过这个URL来连接到WebSocket服务器端
12  */
13 @ServerEndpoint("/websocket")
14 public class WebSocketTest {
15     //静态变量，用来记录当前在线连接数。应该把它设计成线程安全的。
16     private static int onlineCount = 0;
17 
18     //concurrent包的线程安全Set，用来存放每个客户端对应的MyWebSocket对象。若要实现服务端与单一客户端通信的话，可以使用Map来存放，其中Key可以为用户标识
19     private static CopyOnWriteArraySet<WebSocketTest> webSocketSet = new CopyOnWriteArraySet<WebSocketTest>();
20 
21     //与某个客户端的连接会话，需要通过它来给客户端发送数据
22     private Session session;
23 
24     /**
25      * 连接建立成功调用的方法
26      * @param session  可选的参数。session为与某个客户端的连接会话，需要通过它来给客户端发送数据
27      */
28     @OnOpen
29     public void onOpen(Session session){
30         this.session = session;
31         webSocketSet.add(this);     //加入set中
32         addOnlineCount();           //在线数加1
33         System.out.println("有新连接加入！当前在线人数为" + getOnlineCount());
34     }
35 
36     /**
37      * 连接关闭调用的方法
38      */
39     @OnClose
40     public void onClose(){
41         webSocketSet.remove(this);  //从set中删除
42         subOnlineCount();           //在线数减1
43         System.out.println("有一连接关闭！当前在线人数为" + getOnlineCount());
44     }
45 
46     /**
47      * 收到客户端消息后调用的方法
48      * @param message 客户端发送过来的消息
49      * @param session 可选的参数
50      */
51     @OnMessage
52     public void onMessage(String message, Session session) {
53         System.out.println("来自客户端的消息:" + message);
54         //群发消息
55         for(WebSocketTest item: webSocketSet){
56             try {
57                 item.sendMessage(message);
58             } catch (IOException e) {
59                 e.printStackTrace();
60                 continue;
61             }
62         }
63     }
64 
65     /**
66      * 发生错误时调用
67      * @param session
68      * @param error
69      */
70     @OnError
71     public void onError(Session session, Throwable error){
72         System.out.println("发生错误");
73         error.printStackTrace();
74     }
75 
76     /**
77      * 这个方法与上面几个方法不一样。没有用注解，是根据自己需要添加的方法。
78      * @param message
79      * @throws IOException
80      */
81     public void sendMessage(String message) throws IOException{
82         this.session.getBasicRemote().sendText(message);
83         //this.session.getAsyncRemote().sendText(message);
84     }
85 
86     public static synchronized int getOnlineCount() {
87         return onlineCount;
88     }
89 
90     public static synchronized void addOnlineCount() {
91         WebSocketTest.onlineCount++;
92     }
93 
94     public static synchronized void subOnlineCount() {
95         WebSocketTest.onlineCount--;
96     }
97 }
```

###  1.2.运行效果

　　同时打开Google浏览器和火狐浏览器进行多客户端模拟测试,运行效果如下:

![](https://images2015.cnblogs.com/blog/289233/201602/289233-20160216164521970-2049326717.gif)

**　　该Demo在Jdk1.7+Tomcat7.0.65下环境测试过,**[示例项目代码下载](http://pan.baidu.com/s/1hrx1GFy)

　　本篇博客的大部分内容转载自http://blog.chenzuhuang.com/archive/28.html,然后在此基础上进行完善,在此对作者表示感谢.









