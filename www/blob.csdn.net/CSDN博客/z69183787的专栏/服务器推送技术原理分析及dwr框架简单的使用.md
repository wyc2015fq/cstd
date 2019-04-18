# 服务器推送技术原理分析及dwr框架简单的使用 - z69183787的专栏 - CSDN博客
2014年03月23日 13:04:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：915
个人分类：[服务端推-Dwr](https://blog.csdn.net/z69183787/article/category/2275861)
**1 背景**
“服务器推送技术”（ ServerPushing）是最近 Web技术中最热门的一个流行术语。它是继“ Ajax ”之后又一个倍受追捧的 Web技术。“服务器推送技术”最近的流行跟“ Ajax ”有着密切的关系。
随着 Ajax技术的兴起，让广大开发人员又一次看到了使用浏览器来替代桌面应用的机会，并且这次机会非常大。Ajax将整个页面的刷新变成页面局部的刷新，并且数据的传送是以异步方式进行，这使得网络延迟带来的视觉差异将会消失。
但是，在浏览器中的 Ajax应用中存在一个致命的缺陷无法满足传统桌面系统的需求。那就是“服务器发起的消息传递”（ Server-Initiated Message Delivery）。在很多的应用当中，服务器软件需要向客户端主动发送消息或信息。因为服务器掌握着系统的主要资源，能够最先获得系统的状态变化和事件的发生。当这些变化发生的时候，服务器需要主动的向客户端实时的发送消息。例如股票的变化。在传统的桌面系统这种需求没有任何问题，因为客户端和服务器之间通常存在着持久的连接，这个连接可以双向传递各种数据。而基于 HTTP协议的 Web应用却不行。
**2 客户端得到通知的方式**
图1 传统web 访问机制
我们知道， Web的访问机制天生是设计用来 pull数据的，如图 1，也就是只允许 Browser端主动发起请求，server是被动的响应，不允许 Server向 Browser发出一个 connection请求，也就是说没有为 server向 Browser
 push数据提供设计实现 .虽然没有直接的实现方法 ,却可以使用一些变通的方式完成类似的功能。
**2.1 传统轮询**
在 Web早期，这一点常使用 meta刷新实现。这将自动指示浏览器在指定秒数之后重新装载页面，从而支持简陋的轮询（ polling）。例如在 HTML文件中加入
 <META HTTP-RQUIV="Refresh" CONTENT=12> ，实际上就是 HTTP 头标告知浏览器每 12 秒更新一次文档。
**优点 **：不需要服务器端的配置。
**缺点 **：
a)糟糕的用户体验
b)对服务器的压力很大，并且造成带宽的极大浪费。
**2.2 Ajax 轮询**
Ajax隔一段时间（通常使用 JavaScript的 setTimeout函数）就去服务器查询是否有改变，从而进行增量式的更新。但是间隔多长时间去查询成了问题，因为性能和即时性造成了严重的反比关系。间隔太短，连续不断的请求会冲垮服务器，间隔太长，务器上的新数据就需要越多的时间才能到达客户机。
**优点：**
a)不需要太多服务器端的配置。
b)降低带宽的负荷（因为服务器返回的不是完整页面）。
**缺点：**
a)对服务器的压力并不会有明显的减少。
b)实时性差，有一定的延迟。
**应用： **这是一项非常常见的技术，例如，大多数 webmail应用程序就是通过这种技术在电子邮件到达时显示电子邮件的。
**2.3 Comet**
Comet方式通俗的说就是一种长连接机制 (long lived http)。同样是由 Browser端主动发起请求，但是 Server端以一种似乎非常慢的响应方式给出回答。这样在这个期间内，服务器端可以使用同一个 connection把要更新的数据主动发送给 Browser。因此请求可能等待较长的时间，期间没有任何数据返回，但是一旦有了新的数据，它将立即被发送到客户机。 Comet又有很多种实现方式，但是总的来说对 Server端的负载都会有增加 .虽然对于单位操作来说，每次只需要建议一次 connection,但是由于 connection是保持较长时间的 ,对于 server端的资源的占用要有所增加。
**优点： **实时性好（消息延时小）；性能好（能支持大量用户）
**缺点： **长期占用连接，丧失了无状态高并发的特点。
**应用： **股票系统、实时通讯。
**2.4 Flash XML Socket**
这种方案实现的基础是：一、 Flash提供了 XMLSocket类。二、 JavaScript 和 Flash的紧密结合：在 JavaScript可以直接调用 Flash程序提供的接口。
**缺点：**
a)因为XMLSocket没有HTTP隧道功能，XMLSocket类不能自动穿过防火墙；
b)因为是使用套接口，需要设置一个通信端口，防火墙、代理服务器也可能对非HTTP 通道端口进行限制；
**应用： **网络聊天室，网络互动游戏。
**2.5 Java Applet 套接口**
在客户端使用 Java Applet，通过 java.net.Socket或 java.net.DatagramSocket或 java.net.MulticastSocket 建立与服务器端的套接口连接，从而实现 “服务器推送 ”。
**缺点： **需要客户端安装 JAVA虚拟机。
**3 Comet 介绍**
Comet 有时也称反向 Ajax或服务器端推技术（ server-side push）。其思想很简单：将数据直接从服务器推到浏览器，而不必等到浏览器请求数据。听起来简单，但是如果熟悉 Web 应用程序，尤其是 HTTP协议，那么您就会知道，这绝不简单。实现 Comet风格的 Web应用程序，同时保证在浏览器和服务器上的可伸缩性，这只是在最近几年才成为可能。目前一些主流网站都有类似的原理，例如： webQQ、开心网、校内等等，它们中消息动态都是采用类似的技术，只是具体实现方式不一样。
COMET的精髓就在于用服务器与 javascript来维持浏览器的长连接，同时完成服务器端事件的浏览器端响应。这样的事件广播机制是跨网络的，同时也是实时的。
采用了 Comet技术的 [服务器](http://server.zdnet.com.cn/files/list-0-0-184747-1-1.htm)在客户机做出一个请求后，和客户机建立一个永久的连接，然后 [服务器](http://server.zdnet.com.cn/files/list-0-0-184747-1-1.htm)会根据客户机的请求不断把数据包推向客户，这个推的过程是不间断的。由 [服务器](http://server.zdnet.com.cn/files/list-0-0-184747-1-1.htm)推向客户机的数据在客户机的浏览器上会不断产生新的内容，而且不会产生 Client
 pull那样的 HTML文档头，从而大大减少了延迟的时间，向（ [服务器](http://server.zdnet.com.cn/files/list-0-0-184747-1-1.htm)响应 --客户机请求）同步迈进了一步。
服务器推送通常效率要比客户端拖曳效率高，因为它不必为后续数据建立新的连接。由于始终保持连接，即使没有数据传输时也是这样，因此服务器必须愿意分配这些 TCP/IP端口，对于 TCP/IP端口数有限的服务器这将是一个严重的问题。
客户端拖曳效率低，因为这必须每次为传送数据建立新的连接。但是它不必始终保持连接。在实际情况中，建立HTTP连接通常需要花费相当多的时间，多达一秒甚至更多。因此从性能上考虑，服务器推送对于最终用户更有吸引力，特别是对于需要经常更新信息的情况下。
服务器推送相对客户端拖曳的另一点优势是，服务器推送相对比较容易控制。例如，服务器每一次推送时都保持一个连接，但它又随时可以关闭其中的任何连接，而不需要在服务器上设置特殊的算法。而客户端拖曳在同样的情况下要麻烦许多，它每次要与服务器建立连接，服务器为了处理将客户端拖曳请求与特定的最终用户匹配等情况，需要使用相当麻烦的算法。
如上所述，在服务器推送中，多个响应中连接始终保持，使服务器可在任何时间发送更多的数据。一个明显的好处是服务器完全能够控制更新数据的时间和频率。另外，这种方法效率高，因为始终保持连接。缺点是保持连接状态会浪费服务器端的资源。服务器推送还比较容易中断。
**4 Comet 实现（Java语言）**
**4.1 死循环法**
最简单的自然是死循环法，如果使用观察者模式则可以进一步提高性能。
但是这种做法的缺点在于客户端请求了这个 servlet 后， web 服务器会开启一个线程执行 servlet 的代码，而 servlet 由迟迟不肯结束，造成该线程也无法被释放。于是乎，一个客户端一个线程，当客户端数量增加时，服务器依然会承受很大的负担。
4.2 **改写**web **服务器**
目前的趋势是从 web 服务器内部入手，用 nio （ JDK 1.4 提出的 java.nio 包）改写 request/response 的实现，再利用线程池增强服务器的资源利用率，从而解决这个问题，目前支持这一非 J2EE 官方技术的服务器有 Glassfish 和 Jetty 。
JDK 1.4 版本 ( 包括之后的版本 ) 最显著的新特性就是增加了 NIO(New IO) ，能够以非阻塞的方式处理网络的请求，这就使得在 Java 中只需要少量的线程就能处理大量的并发请求了。
Jetty 6设计来处理大量并发连接，它使用 Java语言的不堵塞 I/O(java.nio)库并且使用优化的输出缓冲架构。 Jetty也有一个处理长连接的杀手锏 :一个称为 Continuations的特性。
Grizzly 作为 GlassFish 中非常重要的一个项目，就是用 NIO 的技术来实现应用服务器中的高性能纯 Java 的 HTTP 引擎。 Grizzly 还是一个独立于 GlassFish 的框架结构，可以单独用来扩展和构建自己的服务器软件。
**特点： **使用 NIO 不是一件简单的技术，它的一些特点使得编程的模型比原来阻塞的方式更为复杂。
4.3 **使用框架**
基于 Java 的成熟的服务器推送框架有 DWR 。
DWR是一个开放源码的使用 Apache许可协议的解决方案，它包含服务器端 Java库、一个 DWR
 servlet以及JavaScript库。虽然 DWR不是 Java平台上唯一可用的 Ajax-RPC 工具包，但是它是最成熟的，而且提供了许多有用的功能。从最简单的角度来说， DWR是一个引擎，可以把服务器端 Java对象的方法公开给 JavaScript 代码。使用 DWR 可以有效地从应用程序代码中把 Ajax的全部请求 -响应循环消除掉。这意味着客户端代码再也不需要直接处理 XMLHttpRequest对象或者服务器的响应。不再需要编写对象的序列化代码或者使用第三方工具才能把对象变成 XML。甚至不再需要编写 servlet代码把 Ajax请求调整成对 Java域对象的调用。
DWR 从 2.0 开始增加了 push 功能 , 也就是在异步传输的情况下可以从 Web-Server 端发送数据到 Browser
一个简单的dwr推送程序
第一步 将dwr相关的jar包导入到工程
第二步 配置web.xml文件
- **<?xml**version="1.0"encoding="UTF-8"**?>**
- **<web-app**version="2.5"xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee    
-  http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"**>**
- **<servlet>**
- **<servlet-name>**dwr-invoker**</servlet-name>**
- **<servlet-class>**org.directwebremoting.servlet.DwrServlet**</servlet-class>**
-     <!-- 设置是否允许使用dwr推送技术 -->  
- **<init-param>**
- **<param-name>**activeReverseAjaxEnabled**</param-name>**
- **<param-value>**true**</param-value>**
- **</init-param>**
- **<init-param>**
- **<param-name>**maxWaitAfterWrite**</param-name>**
- **<param-value>**-1**</param-value>**
- **</init-param>**
- **<load-on-startup>**1**</load-on-startup>**
- **</servlet>**
- **<servlet-mapping>**
- **<servlet-name>**dwr-invoker**</servlet-name>**
- **<url-pattern>**/dwr/***</url-pattern>**
- **</servlet-mapping>**
- **</web-app>**
第三步编写com.im.service.SendPushService类
- public class SendPushService {   
-     //发送消息   
-     public void send(String msg) {   
-         System.out.println("==========调用了send方法==========");   
-         ScriptBuffer scriptBuffer = new ScriptBuffer(); //构造js脚本   
-         WebContext webContext=WebContextFactory.get();   
-         ScriptSession myScSession = webContext.getScriptSession();   
-         scriptBuffer.appendScript("dwrtest(");   
-         scriptBuffer.appendData(msg);   
-         scriptBuffer.appendScript(")");   
-         Util util = new Util(myScSession);   
-         util.addScript(scriptBuffer); //向客户端推送消息   
-     }   
- } 
第四步在dwr.xml文件定义向外暴露的接口
- **<allow>**
- **<create**creator="new"javascript="SendPushService"**>**
- **<param**name="class"value="com.im.service.SendPushService"**/>**
- **</create>**
- **</allow>**
第五步: 编写jsp文件.
- **<**%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%**>**
- <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" " http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"**>**
- **<html**xmlns=" http://www.w3.org/1999/xhtml"**>**
- **<head>**
- **<base**href="${basePath }"**/>**
- **<script**type='text/javascript'src='${basePath}dwr/engine.js'**></script>**
- **<script**type='text/javascript'src='${basePath}dwr/util.js'**></script>**
- **<script**type='text/javascript'src='${basePath}dwr/interface/SendPushService.js'**></script>**
- **<script**type="text/javascript"**>**
-         function hello(){   
-             SendPushService.send("第一个dwr推程序");   
-         }   
-         /**由dwr在后台调用这个方法**/   
-         function dwrtest(data){   
-             alert(data);   
-         }   
- **</script>**
- **<title>**第一个dwr推程序**</title>**
- **</head>**
- **<body**onload="dwr.engine.setActiveReverseAjax(true);"**>**
- **<input**type="button"value="点击我"onclick="hello();"**/>**
- **</body>**
- **</html>**
