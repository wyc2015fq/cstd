# Pushlet之消息通知- 实现服务器端推送技术 - z69183787的专栏 - CSDN博客
2014年03月26日 10:51:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2694

Pushlet 是一个开源的 Comet 框架，在设计上有很多值得借鉴的地方，对于开发轻量级的 Comet 应用很有参考价值。
观察者模型
Pushlet 使用了观察者模型：客户端发送请求，订阅感兴趣的事件；服务器端为每个客户端分配一个会话 ID 作为标记，事件源会把新产生的事件以多播的方式发送到订阅者的事件队列里。
客户端 JavaScript 库
pushlet 提供了基于 AJAX 的 JavaScript 库文件用于实现长轮询方式的“服务器推”；还提供了基于 iframe 的 JavaScript 库文件用于实现流方式的“服务器推”。
JavaScript 库做了很多封装工作：
- 
定义客户端的通信状态：`STATE_ERROR`、`STATE_ABORT`、`STATE_NULL`、`STATE_READY`、`STATE_JOINED`、`STATE_LISTENING`；
- 
保存服务器分配的会话 ID，在建立连接之后的每次请求中会附上会话 ID 表明身份；
- 
提供了 `join()`、`leave()`、`subscribe()`、 `unsubsribe()`、`listen()` 等
 API 供页面调用；
- 
提供了处理响应的 JavaScript 函数接口 `onData()`、`onEvent()`…
网页可以很方便地使用这两个 JavaScript 库文件封装的 API 与服务器进行通信。
客户端与服务器端通信信息格式
pushlet 定义了一套客户与服务器通信的信息格式，使用 XML 格式。定义了客户端发送请求的类型：`join`、`leave`、`subscribe`、`unsubscribe`、`listen`、`refresh`；以及响应的事件类型：`data`、`join_ack`、`listen_ack`、`refresh`、`heartbeat`、`error`、`abort`、`subscribe_ack`、`unsubscribe_ack`。
服务器端事件队列管理
pushlet 在服务器端使用 Java Servlet 实现，其数据结构的设计框架仍可适用于 PHP、C 编写的后台客户端。
Pushlet 支持客户端自己选择使用流、拉（长轮询）、轮询方式。服务器端根据客户选择的方式在读取事件队列（fetchEvents）时进行不同的处理。“轮询”模式下 `fetchEvents()` 会马上返回。”流“和”拉“模式使用阻塞的方式读事件，如果超时，会发给客户端发送一个没有新信息收到的“heartbeat“事件，如果是“拉”模式，会把“heartbeat”与“refresh”事件一起传给客户端，通知客户端重新发出请求、建立连接。
客户服务器之间的会话管理
服务端在客户端发送 `join` 请求时，会为客户端分配一个会话 ID， 并传给客户端，然后客户端就通过此会话 ID 标明身份发出`subscribe` 和 `listen` 请求。服务器端会为每个会话维护一个订阅的主题集合、事件队列。
服务器端的事件源会把新产生的事件以多播的方式发送到每个会话（即订阅者）的事件队列里。
官网： [http://www.pushlets.com/](http://www.pushlets.com/)
根据需要设计了一个简单的消息通知系统，使用pushlet实现。如有错误，欢迎指正，非常感谢。 
第一步
将pushlet.jar放到工程的lib目录中，将pushlet-2.0.4\webapps\pushlet\lib\js-pushlet-client.js放到网页能访问的目录，将pushlet-2.0.4\webapps\pushlet\WEB-INF\classes\pushlet.properties和pushlet-2.0.4\webapps\pushlet\WEB-INF\classes\sources.properties拷贝到工程目录的classes目录下。 
第二步
使用pushlets在网页客户端中配置非常简单 
在网中页面引用如下代码 
Java代码 
- <script type="text/javascript" src="../../lib/ajax-pushlet-client.js">  
- </script>  
- 
- <script type="text/javascript">  
- //可以设置debug为true，会在新标签页中打印debug信息，调试的时候非常有用。
-     PL.setDebug(**false**);  
- 
- // Pushlet Event Callback from Server
- //回调
-     function onEvent(event) {  
-         pr(event.toString());  
-     }  
- 
- //订阅topic
-     function joinListen() {  
-         pr('joinListen...');  
-         PL.joinListen('/pushlet/ping');  
-     }  
- //取消订阅
-     function leave() {  
-         pr('leaving...');  
-         PL.leave();  
-     }  
- 
- // 打印信息
-     function pr(aString) {  
-         document.disp.event.value = aString;  
-     }  
- 
- //-->
-     </script>  
第三步
在工程的web.xml中添加配置如下 
Java代码 
- <!-- Define the pushlet servlet -->  
-     <servlet>  
-         <servlet-name>pushlet</servlet-name>  
-         <servlet-**class**>nl.justobjects.pushlet.servlet.Pushlet</servlet-**class**>  
-         <load-on-startup>1</load-on-startup>  
-     </servlet>  
- 
-     <!-- Define the Servlet Mappings. -->  
- 
-     <!-- The pushlet -->  
-     <servlet-mapping>  
-         <servlet-name>pushlet</servlet-name>  
-         <url-pattern>/js/pushlet.srv</url-pattern>  
-     </servlet-mapping>  
其中url-pattern跟前面ajax-pushlet-client.js放置的目录有关，需要根据自己的项目路径调整。 
第四步
服务器端实现pushlets有两种方式，第一种使用eventpullsource，需要实现包含一个继承了EventPullSource的内部静态类，如下面代码中的EncodeJobQuery。 
Java代码 
- **public****class** EncodeJobQuery {  
- 
- **public****static****class** EncodeJobEventPullSource **extends** EventPullSource {  
-         Logger log = Logger.getLogger(getClass());  
- @Autowired
-         PvmsEncodeMessageManager encodeMessageManager;  
- 
- @Override
- **protected****long** getSleepTime() {  
- **return** 5000l;  
-         }  
- 
- @Override
- **protected** Event pullEvent() {  
- //参数是subject，与js中的subject相同
-             Event event = Event.createDataEvent("/pushlet/ping");  
-             event.setField("msg", "encoding....");  
- **return** event;  
-         }  
-     }  
- }  
同时要在classes/sources.properties文件中增加一个行。 
Java代码 
- source7=cn.com.people.tv.pvms.encode.EncodeJobQuery$EncodeJobEventPullSource  
第二种方法是在程序中调用Dispatcher.getInstance().multicast(event)，除了multicast还有其他两种方法用来发送消息。 
Java代码 
- String subject = "/pushlet/ping"
- Event event = Event.createDataEvent(subject);  
-         event.setField("msg", "ping.....");  
- PvmsEncodeMessage.MSG_MESSAGE_COUNT);  
-         Dispatcher.getInstance().multicast(event);  
总结： 
在使用的过程中遇到了若干问题 
1.怎么实现根据用户来推送消息 
我在实现的时候为不同的用户定义不同的topic，根据用户id来区分。看了文档介绍，说可以绑定pushlet的session，没研究明白具体怎么操作。我的实现方式缺点是，如果服务器重启或者消息发送失败导致pushlet的session关闭，那么后续的消息就没办法发送给用户。 
2.发送离线消息 
我把消息存在服务器，只用pushlet来发送当前的消息数量。比如在用户登录后，发送给用户一条消息告诉用户有多少条未读消息，用户可以手动标记消息为已读或者删除消息。
