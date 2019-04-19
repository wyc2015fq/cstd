# 使用Pushlet实现后台信息推送(二) - 零度的博客专栏 - CSDN博客
2016年06月02日 16:00:04[零度anngle](https://me.csdn.net/zmx729618)阅读数：2441
上一篇日志利用推送源周期性地向订阅了某一事件的所有网页端推送信息，但怎么实现向特定的某一个用户推送信息呢，想象一个网络聊天室，怎么向单独的一个好友私聊呢。问题的关键就是那个SessionID，Pushlet默认是用java.rmi.server.UID参数的随机字符串作为会话标志，这就造成我们没法跟一个特点的用户对应上，也就是说客户必须在joinListen的时候提供一个他的ID，就像登陆qq时，输入用户名一样。
好了，这里就列出怎么修改相关代码，实现上述功能。
同样，首先用MyEclipse新建一个web工程，我这里命名为PushletTest2。不同的是，我们不引用pushlet.jar包了，因为我们要更改源码，在下载pushlet的时候里面就包含源代码了，最外层是nl包，加入我的工程的src里。好了，加进去之后会报错，将log4j.jar加入引用就好了。。之后，同样别忘了把pushlet.properties和sources.properties加入WebRoot/WEN-INF下。工程结构如下：
![](http://dl.iteye.com/upload/attachment/0062/8998/1b9ac2e9-3dc2-3e5b-8e79-6b4d55ecff97.bmp)
FirPush.java是自己写的一个触发推送动作的类，它是一个servlet，请求该servlet就触发向特定的用户推送信息的动作（当然，怎么触发推送动作还有很多种形式）。
web.xml代码如下：
Xml代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- <spanstyle="font-size: x-small;"><?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.5"
- xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee   
-     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
- <servlet>
- <servlet-name>pushlet</servlet-name>
- <servlet-class>nl.justobjects.pushlet.servlet.Pushlet</servlet-class>
- <load-on-startup>3</load-on-startup>
- </servlet>
- <servlet>
- <servlet-name>FirePush</servlet-name>
- <servlet-class>com.mysrc.FirePush</servlet-class>
- </servlet>
- <servlet-mapping>
- <servlet-name>pushlet</servlet-name>
- <url-pattern>/pushlet.srv</url-pattern>
- </servlet-mapping>
- <servlet-mapping>
- <servlet-name>FirePush</servlet-name>
- <url-pattern>/FirePush</url-pattern>
- </servlet-mapping>
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- </web-app></span>
<span style="font-size:10px;"><?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5" 
	xmlns="http://java.sun.com/xml/ns/javaee" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
	http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
	<servlet>
	   <servlet-name>pushlet</servlet-name>
	   <servlet-class>nl.justobjects.pushlet.servlet.Pushlet</servlet-class>
	   <load-on-startup>3</load-on-startup>
	</servlet>
	<servlet> 
    <servlet-name>FirePush</servlet-name>
  		<servlet-class>com.mysrc.FirePush</servlet-class>
    </servlet>	
	<servlet-mapping>
	   <servlet-name>pushlet</servlet-name>
	   <url-pattern>/pushlet.srv</url-pattern>
	</servlet-mapping>	
	<servlet-mapping>
  	   <servlet-name>FirePush</servlet-name>
  	   <url-pattern>/FirePush</url-pattern>
    </servlet-mapping>	
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
</web-app></span>
 Ok，接下来就是要进行修改了，首先打开ajax-pushlet-client.js。PL对象，在它的最开始添加一个属性userId:'',  ，因为分析ajax-pushlet-client.js之后会发现，所有的请求都是get方式，也就是说是将请求参数添加到url里的，我们要做的是在进行join请求的时候将userId加到url上。找到PL的_doRequest，它是一个函数，所有的请求都是通过调用它发出的，找到var
 url = PL.pushletURL + '?p_event=' + anEvent;这一行，这儿就是在构造请求参数，在它的后面加入代码：
Js代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- if (anEvent == 'join' || anEvent == 'join-listen') {    url = url + '&userId=' + PL.userId;   
-         }  
if (anEvent == 'join' || anEvent == 'join-listen') { 	url = url + '&userId=' + PL.userId; 
		}
 好了，ajax-pushlet-client.js的修改就完成了。
接下来是test.html:
Html代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- <html>
- <head>
- <scripttype="text/javascript"src="ajax-pushlet-client.js"></script>
- <scripttype="text/javascript">
- PL.userId='piero';  
-          PL._init();    
-          PL.joinListen('myevent1');   
-          function onData(event) {   
-             alert(event.get("key1"));    
-          }  
- </script>
- </head>
- <body>
-     This is pushlet test page....<br>
- </body>
- </html>
```
<html>
  <head>
  <script type="text/javascript" src="ajax-pushlet-client.js"></script> 
  <script type="text/javascript">  
  		 PL.userId='piero';
         PL._init();  
         PL.joinListen('myevent1'); 
         function onData(event) { 
            alert(event.get("key1"));  
         }
  </script> 
  </head>
  <body>
    This is pushlet test page....<br>
  </body>
</html>
```
然后就是修改pushlet的java源代码，找到nl.justobjects.pushlet.core包下的SessionManager类，将它的createSession方法改成：
Java代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- public Session createSession(Event anEvent) throws PushletException {  
- // Trivial
- //return Session.create(createSessionId());
- return Session.create(anEvent.getField("userId", "visitor"));  
-     }  
```java
public Session createSession(Event anEvent) throws PushletException {
		// Trivial
		//return Session.create(createSessionId());
		return Session.create(anEvent.getField("userId", "visitor"));
	}
```
 这儿就使得建立的session的id是发送过来的userId，而不是一个随机字符串。getField的第二个参数是当得不到请求参数userId的值的时候设定的默认值。
好了，所有的准备工作都完成了，接下来就是通过userId给特点的用户推送信息了。
FirePush.java代码如下：
Java代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- package com.mysrc;  
- 
- import java.io.IOException;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import nl.justobjects.pushlet.core.Dispatcher;  
- import nl.justobjects.pushlet.core.Event;  
- import nl.justobjects.pushlet.core.SessionManager;  
- 
- publicclass FirePush extends HttpServlet {  
- 
- @Override
- protectedvoid doGet(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
-         myUnicast();  
- //myMulticast();
- //myBroadcast();
-     }  
- privatevoid myUnicast()  
-     {  
- if(SessionManager.getInstance().hasSession("piero")){  
-             Event  event = Event.createDataEvent("myevent1");   
-             event.setField("key1", "houhou_yesttttt....");            
-             Dispatcher.getInstance().unicast(event,"piero"); //向ID为piero的用户推送
- 
-             System.out.println("success....");  
-         }  
- else {  
-             System.out.println("piero do not login....%%%%%%%%%%%%");  
-         }  
-     }  
- privatevoid myMulticast()  
-     {  
-         Event  event = Event.createDataEvent("myevent1");   
-         event.setField("key1", "houhou....");    
-         Dispatcher.getInstance().multicast(event);  //向所有和myevent1名称匹配的事件推送
- 
-         System.out.println("wa success....");  
-     }  
- privatevoid myBroadcast()  
-     {  
-         Event  event = Event.createDataEvent("myevent1"); //向所有的事件推送，不要求和这儿的myevent1名称匹配
-         event.setField("key1", "dig hole....");      
-         Dispatcher.getInstance().broadcast(event);  
- 
-         System.out.println("asw success....");  
-     }  
- }  
```java
package com.mysrc;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import nl.justobjects.pushlet.core.Dispatcher;
import nl.justobjects.pushlet.core.Event;
import nl.justobjects.pushlet.core.SessionManager;
public class FirePush extends HttpServlet {
	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		myUnicast();
		//myMulticast();
		//myBroadcast();
	}
    private void myUnicast()
    {
    	if(SessionManager.getInstance().hasSession("piero")){
    		Event  event = Event.createDataEvent("myevent1"); 
			event.setField("key1", "houhou_yesttttt....");  		
			Dispatcher.getInstance().unicast(event,"piero"); //向ID为piero的用户推送
			  
			System.out.println("success....");
    	}
    	else {
    		System.out.println("piero do not login....%%%%%%%%%%%%");
    	}
    }
    private void myMulticast()
    {
    	Event  event = Event.createDataEvent("myevent1"); 
		event.setField("key1", "houhou....");  
		Dispatcher.getInstance().multicast(event);  //向所有和myevent1名称匹配的事件推送
		
		System.out.println("wa success....");
    }
    private void myBroadcast()
    {
    	Event  event = Event.createDataEvent("myevent1"); //向所有的事件推送，不要求和这儿的myevent1名称匹配
		event.setField("key1", "dig hole....");    
		Dispatcher.getInstance().broadcast(event);
		
		System.out.println("asw success....");
    }
}
```
 这儿主要利用了dispatcher的unicat/multicat/broadcast方法。
好了，值得一提的是在ajax-pushlet-client.js有publish方法，也就是上面提到的触发信息推送动作，其实完全可以在前端调用这个js的publish函数向订阅了某事件的所有网页推送信息。test2.html代码如下：
Html代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- <html>
- <head>
- <scripttype="text/javascript"src="ajax-pushlet-client.js"></script>
- <scripttype="text/javascript">
- PL.userId='juvenh';  
-          PL._init();   
- 
-          PL.joinListen('event223');   
-          function onData(event) {   
-                alert(event.get("eventVal"));    
-          }  
- 
-          function sendnews(){   
-             //PL.publish("myevent1","key1=dlut&ba=ls");   
-             p_publish('myevent1', 'key1', 'dlut', 'ba', 'ls');  
-         }  
- </script>
- </head>
- 
- <body>
-     This is my JSP page. <br>
- <inputtype = "button"value="发消息"onclick="sendnews()"/>
- </body>
- </html>
```
<html>
  <head>
  <script type="text/javascript" src="ajax-pushlet-client.js"></script> 
  <script type="text/javascript"> 
  		 PL.userId='juvenh';
         PL._init(); 
              
         PL.joinListen('event223'); 
         function onData(event) { 
               alert(event.get("eventVal"));  
         }
         
         function sendnews(){ 
 			//PL.publish("myevent1","key1=dlut&ba=ls"); 
 			p_publish('myevent1', 'key1', 'dlut', 'ba', 'ls');
		}
  </script> 
  </head>
  
  <body>
    This is my JSP page. <br>
    <input type = "button" value="发消息" onclick="sendnews()"/> 
  </body>
</html>
```
 上面代码中的PL.publish和p_publish完全等效，查看p_publish的代码，其实也是构造请求url。这儿的publish方法只能向订阅某一事件的所有网页推送消息，也即等同于multicast方法，无法向某一个特定的用户推送，解决方法还是向上面那样修改pushlet的java源代码，在nl.justobjects.pushlet.core包下的Controller.java中的protected void doPublish(Command aCommand);函数，看一下就会发现，这儿就是调用了dispacher的multicast函数，这儿改成unicast，前提是上面的js的publish里提供了userId。
