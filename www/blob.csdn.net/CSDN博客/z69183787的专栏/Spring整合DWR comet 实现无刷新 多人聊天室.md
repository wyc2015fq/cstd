# Spring整合DWR comet 实现无刷新 多人聊天室 - z69183787的专栏 - CSDN博客
2014年03月17日 13:29:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1653
用dwr的comet(推)来实现简单的无刷新多人聊天室，comet是长连接的一种。通常我们要实现无刷新，一般会使用到Ajax。Ajax 应用程序可以使用两种基本的方法解决这一问题：一种方法是浏览器每隔若干秒时间向服务器发出轮询以进行更新，另一种方法是服务器始终打开与浏览器的连接并在数据可用时发送给浏览器。第一种方法一般利用setTimeout或是setInterval定时请求，并返回最新数据，这无疑增加了服务器的负担，浪费了大量的资源。而第二种方法也会浪费服务器资源，长期的建立连接;而相对第一种来说，第二种方式会更优于第一种方法;这里有一个一对多和多对一的关系，而comet向多个客户端推送数据就是一对多的关系。而具体使用哪种方式，要看你当前的需求而定，没有绝对的。
为什么使用Comet?
轮询方法的主要缺点是：当扩展到更多客户机时，将生成大量的通信量。每个客户机必须定期访问服务器以检查更新，这为服务器资源添加了更多负荷。最坏的一种情况是对不频繁发生更新的应用程序使用轮询，例如一种 Ajax 邮件 Inbox。在这种情况下，相当数量的客户机轮询是没有必要的，服务器对这些轮询的回答只会是 “没有产生新数据”。虽然可以通过增加轮询的时间间隔来减轻服务器负荷，但是这种方法会产生不良后果，即延迟客户机对服务器事件的感知。当然，很多应用程序可以实现某种权衡，从而获得可接受的轮询方法。
尽管如此，吸引人们使用 Comet 策略的其中一个优点是其显而易见的高效性。客户机不会像使用轮询方法那样生成烦人的通信量，并且事件发生后可立即发布给客户机。但是保持长期连接处于打开状态也会消耗服务器资源。当等待状态的 servlet 持有一个持久性请求时，该 servlet 会独占一个线程。这将限制 Comet 对传统 servlet 引擎的可伸缩性，因为客户机的数量会很快超过服务器栈能有效处理的线程数量。
如果本示例结合Jetty应用服务器效果会更好。
开发环境：
System：Windows
WebBrowser：IE6+、Firefox3+
JavaEE Server：tomcat5.0.2.8、tomcat6
IDE：eclipse、MyEclipse 8
开发依赖库：
JavaEE5、Spring 3.0.5、dwr 3
Email：hoojo_@126.com
Blog：http://blog.csdn.net/IBM_hoojo or http://hoojo.cnblogs.com/
一、准备工作
1、 下载dwr的相关jar包
https://java.net/downloads/dwr/Development%20Builds/Build%20116/dwr.jar
程序中还需要spring的相关jar包
http://ebr.springsource.com/repository/app/library/version/detail?name=org.springframework.spring&version=3.0.5.RELEASE
需要的jar包如下
![](http://images.51cto.com/files/uploadimg/20110609/1557220.jpg)
2、 建立一个WebProject，名称DWRComet
在web.xml中添加dwr、spring配置如下：
- <-- 加载Spring容器配置 -->
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- 
- <-- 设置Spring容器加载配置文件路径 -->
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath*:applicationContext-*.xml</param-value>
- </context-param>
- 
- <listener>
- <listener-class>org.directwebremoting.servlet.DwrListener</listener-class>
- </listener>
- 
- <servlet>
- <servlet-name>dwr-invoker</servlet-name>
- <servlet-class>org.directwebremoting.servlet.DwrServlet</servlet-class>
- <init-param>
- <param-name>debug</param-name>
- <param-value>true</param-value>
- </init-param>
- 
- <-- dwr的comet控制 -->
- <init-param>
- <param-name>pollAndCometEnabled</param-name>
- <param-value>true</param-value>
- </init-param>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>dwr-invoker</servlet-name>
- <url-pattern>/dwr/*</url-pattern>
- </servlet-mapping>
3、 在src目录加入applicationContext-beans.xml配置，这个配置专门配置bean对象，用来配置需要注入的对象。
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:util="http://www.springframework.org/schema/util"
- xmlns:context="http://www.springframework.org/schema/context"
- xsi:schemaLocation="http://www.springframework.org/schema/beans   
-     http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-     http://www.springframework.org/schema/aop   
-     http://www.springframework.org/schema/aop/spring-aop-3.0.xsd  
-     http://www.springframework.org/schema/tx   
-     http://www.springframework.org/schema/tx/spring-tx-3.0.xsd  
-     http://www.springframework.org/schema/util   
-     http://www.springframework.org/schema/util/spring-util-3.0.xsd  
-     http://www.springframework.org/schema/context   
-     http://www.springframework.org/schema/context/spring-context-3.0.xsd">
- </beans>
4、 在WEB-INF目录添加dwr.xml文件，基本代码如下
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE dwr PUBLIC "-//GetAhead Limited//DTD Direct Web Remoting 3.0//EN" "http://getahead.org/dwr/dwr30.dtd">
- <dwr>
- </dwr>
以上的准备基本完毕，下面来完成无刷新聊天室代码
#### 二、聊天室相关业务实现
1、 聊天实体类Model
- package com.hoo.entity;  
- 
- import java.util.Date;  
- 
- /** 
-  * <b>function:</b> 
-  * @author hoojo 
-  * @createDate 2011-6-3 下午06:40:07 
-  * @file Message.java 
-  * @package com.hoo.entity 
-  * @project DWRComet 
-  * @blog http://blog.csdn.net/IBM_hoojo 
-  * @email hoojo_@126.com 
-  * @version 1.0 
-  */
- publicclass Message {  
- privateint id;  
- private String msg;  
- private Date time;  
- //getter、setter 
- } 
2、 编写聊天信息的事件
- package com.hoo.chat;  
- 
- import org.springframework.context.ApplicationEvent;  
- 
- /** 
-  * <b>function:</b>发送聊天信息事件 
-  * @author hoojo 
-  * @createDate 2011-6-7 上午11:24:21 
-  * @file MessageEvent.java 
-  * @package com.hoo.util 
-  * @project DWRComet 
-  * @blog http://blog.csdn.net/IBM_hoojo 
-  * @email hoojo_@126.com 
-  * @version 1.0 
-  */
- publicclass ChatMessageEvent extends ApplicationEvent {  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- 
- public ChatMessageEvent(Object source) {  
- super(source);  
-     }  
- } 
继承ApplicationEvent，构造参数用于传递发送过来的消息。这个事件需要一个监听器监听，一旦触发了这个事件，我们就可以向客户端发送消息。
3、 发送消息服务类，用户客户端发送消息。dwr需要暴露这个类里面的发送消息的方法
- package com.hoo.chat;  
- 
- import org.springframework.beans.BeansException;  
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.ApplicationContextAware;  
- import com.hoo.entity.Message;  
- 
- /** 
-  * <b>function:</b>客户端发消息服务类业务 
-  * @author hoojo 
-  * @createDate 2011-6-7 下午02:12:47 
-  * @file ChatService.java 
-  * @package com.hoo.chat 
-  * @project DWRComet 
-  * @blog http://blog.csdn.net/IBM_hoojo 
-  * @email hoojo_@126.com 
-  * @version 1.0 
-  */
- publicclass ChatService implements ApplicationContextAware {  
- private ApplicationContext ctx;  
- publicvoid setApplicationContext(ApplicationContext ctx) throws BeansException {  
- this.ctx = ctx;  
-     }  
- 
- /** 
-      * <b>function:</b> 向服务器发送信息，服务器端监听ChatMessageEvent事件，当有事件触发就向所有客户端发送信息 
-      * @author hoojo 
-      * @createDate 2011-6-8 下午12:37:24 
-      * @param msg 
-      */
- publicvoid sendMessage(Message msg) {  
- //发布事件 
-         ctx.publishEvent(new ChatMessageEvent(msg));  
-     }  
- } 
上面的sendMessage需要浏览器客户端调用此方法完成消息的发布，传递一个Message对象，并且是触发ChatMessageEvent事件。
4、 编写监听器监听客户端是否触发ChatMessageEvent
- package com.hoo.chat;  
- 
- import java.util.Collection;  
- import java.util.Date;  
- import javax.servlet.ServletContext;  
- import org.directwebremoting.ScriptBuffer;  
- import org.directwebremoting.ScriptSession;  
- import org.directwebremoting.ServerContext;  
- import org.directwebremoting.ServerContextFactory;  
- import org.springframework.context.ApplicationEvent;  
- import org.springframework.context.ApplicationListener;  
- import org.springframework.web.context.ServletContextAware;  
- import com.hoo.entity.Message;  
- 
- /** 
-  * <b>function:</b>监听客户端事件，想客户端推出消息 
-  * @author hoojo 
-  * @createDate 2011-6-7 上午11:33:08 
-  * @file SendMessageClient.java 
-  * @package com.hoo.util 
-  * @project DWRComet 
-  * @blog http://blog.csdn.net/IBM_hoojo 
-  * @email hoojo_@126.com 
-  * @version 1.0 
-  */
- @SuppressWarnings("unchecked")  
- publicclass ChatMessageClient implements ApplicationListener, ServletContextAware {  
- 
- private ServletContext ctx;  
- publicvoid setServletContext(ServletContext ctx) {  
- this.ctx = ctx;  
-     }  
- 
- @SuppressWarnings("deprecation")  
- publicvoid onApplicationEvent(ApplicationEvent event) {  
- //如果事件类型是ChatMessageEvent就执行下面操作 
- if (event instanceof ChatMessageEvent) {  
-             Message msg = (Message) event.getSource();  
-             ServerContext context = ServerContextFactory.get();  
- //获得客户端所有chat页面script session连接数 
- 
-             Collection<ScriptSession> sessions = context.getScriptSessionsByPage(ctx.getContextPath() + "/chat.jsp");  
- for (ScriptSession session : sessions) {  
-                 ScriptBuffer sb = new ScriptBuffer();  
-                 Date time = msg.getTime();  
-                 String s = time.getYear() + "-" + (time.getMonth() + 1) + "-" +  time.getDate() + " "
-                         +  time.getHours() + ":" + time.getMinutes() + ":" + time.getSeconds();  
- //执行setMessage方法 
- 
-                 sb.appendScript("showMessage({msg: '")  
-                 .appendScript(msg.getMsg())  
-                 .appendScript("', time: '")  
-                 .appendScript(s)  
-                 .appendScript("'})");  
-                 System.out.println(sb.toString());  
- //执行客户端script session方法，相当于浏览器执行JavaScript代码 
- //上面就会执行客户端浏览器中的showMessage方法，并且传递一个对象过去 
- 
-                 session.addScript(sb);  
-             }  
-         }  
-     }  
- } 
上面的代码主要是监听客户端的事件，一旦客户端有触发ApplicationEvent事件或是其子类，就会执行onApplicationEvent方法。代码中通过instanceof判断对象实例，然后再执行。如果有触发ChatMessageEvent事件，就获取所有连接chat.jsp这个页面的ScriptSession。然后像所有的ScriptSession中添加script。这样被添加的ScriptSession就会在有连接chat.jsp的页面中执行。
所以这就是客户端为什么会执行服务器端的JavaScript代码。但前提是需要在web.xml中添加dwrComet配置以及在chat页面添加ajax反转。
5、 下面开始在bean容器和dwr的配置中添加我们的配置
applicationContext-beans.xml配置
- <beanid="chatService"class="com.hoo.chat.ChatService"/>
- <beanid="chatMessageClient"class="com.hoo.chat.ChatMessageClient"/>
上面的chatService会在dwr配置中用到
dwr.xml配置
- <allow>
- <convertmatch="com.hoo.entity.Message"converter="bean">
- <paramname="include"value="msg,time"/>
- </convert>
- 
- <createcreator="spring"javascript="ChatService">
- <paramname="beanName"value="chatService"/>
- </create>
- </allow>
charService的sendMessage方法传递的是Message对象，所以要配置Message对象的convert配置。
上面的create的creator是spring，表示在spring容器中拿chatService对象。里面的参数的beanName表示在spring容器中找name等于charService的bean对象。
6、 客户端chat.jsp页面代码
- <%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%>
- <%  
- String path = request.getContextPath();  
- String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";  
- %>
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
- <html>
- <head>
- <basehref="<%=basePath%>">
- 
- <title>Chat</title>
- 
- <metahttp-equiv="pragma"content="no-cache">
- <metahttp-equiv="cache-control"content="no-cache">
- <metahttp-equiv="expires"content="0">
- <scripttype="text/javascript"src="${pageContext.request.contextPath }/dwr/engine.js"></script>
- <scripttype="text/javascript"src="${pageContext.request.contextPath }/dwr/util.js"></script>
- <scripttype="text/javascript"src="${pageContext.request.contextPath }/dwr/interface/ChatService.js"></script>
- <scripttype="text/javascript">
-         function send() {  
-             var time = new Date();  
-             var content = dwr.util.getValue("content");  
-             var name = dwr.util.getValue("userName");  
-             var info = encodeURI(encodeURI(name + " say:\n" + content));  
-             var msg = {"msg": info, "time": time};  
-             dwr.util.setValue("content", "");  
-             if (!!content) {  
-                 ChatService.sendMessage(msg);  
-             } else {  
-                 alert("发送的内容不能为空！");  
-             }  
-         }  
- 
-         function showMessage(data) {  
-             var message = decodeURI(decodeURI(data.msg));  
-             var text = dwr.util.getValue("info");  
-             if (!!text) {    
-                 dwr.util.setValue("info", text + "\n" + data.time + "  " + message);  
-             } else {  
-                 dwr.util.setValue("info", data.time + "  " + message);  
-             }  
-         }  
- </script>
- </head>
- 
- <bodyonload="dwr.engine.setActiveReverseAjax(true);">
- <textarearows="20"cols="60"id="info"readonly="readonly"></textarea>
- <hr/>
-       昵称：<inputtype="text"id="userName"/><br/>
-       消息：<textarearows="5"cols="30"id="content"></textarea>
- <inputtype="button"value=" Send "onclick="send()"style="height: 85px; width: 85px;"/>
- </body>
- </html>
首先，你需要导入dwr的engine.js文件，这个很重要，是dwr的引擎文件。其次你使用的那个类的方法，也需要在导入进来。一般是interface下的，并且在dwr.xml中配置过的create。
上面的js中调用的charService类中的sendMessage方法，所以在jsp页面中导入的是ChatService.js。
在body的onload事件中，需要设置反转Ajax，这个很重要。
showMessage是ChatMessageClient的onApplicationEvent方法中的appendScript中需要执行的方法。data参数也是在那里传递过来的。
每当发送sendMessage方法后就会触发ChatMessageEvent事件，然后监听的地方就会执行onApplicationEvent方法，在这个方法中又会执行浏览器中的showMessage方法。
![](http://images.51cto.com/files/uploadimg/20110609/1557221.jpg)
原文链接：http://www.cnblogs.com/hoojo/archive/2011/06/08/2075201.html
