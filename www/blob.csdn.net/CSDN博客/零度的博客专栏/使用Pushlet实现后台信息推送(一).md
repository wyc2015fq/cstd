# 使用Pushlet实现后台信息推送(一) - 零度的博客专栏 - CSDN博客
2016年06月02日 15:55:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：2292
Pushlet是使用较多的后台向前台推送信息的工具。前台订阅某个感兴趣的事件joinListen，触发后台的Pushlet的servlet，为该请求会话建立session，默认这个sessionID是随机的java.rmi.server.UID（后面会介绍pushlet二次开发，改掉这个UID，以便可以自由控制向某一个特点前台推送信息）。之后前台会向后台发送refresh请求，后台返回给前台下一次发送refresh的时间间隔，如此循环，以便维持session会话，宏观上就实现了前后台之间的长连接。
这里首先列出最简单的后台利用PullSource周期性地向订阅了某事件的所有网页客户端推送信息。
下载最新版本的Pushlet。MyEclipse下建立一个web工程，我这里叫PushletTest，将pushlet.jar加入引用路径，pushlet.properties和sources.properties放入WebRoot/WEB-INF下，工程结构如下图：
![](http://dl.iteye.com/upload/attachment/0062/8888/0b739cb2-136f-3a74-bab8-9923724914f3.bmp)
在web.xml中添加pushlet的servlet的声明，让这个servlet在服务器启动时就启动（load-on-startup为正数），web.xml代码如下：
Xml代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- <?xmlversion="1.0"encoding="UTF-8"?>
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
- <servlet-mapping>
- <servlet-name>pushlet</servlet-name>
- <url-pattern>/pushlet.srv</url-pattern>
- </servlet-mapping>
- 
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- </web-app>
<?xml version="1.0" encoding="UTF-8"?>
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
	<servlet-mapping>
	   <servlet-name>pushlet</servlet-name>
	   <url-pattern>/pushlet.srv</url-pattern>
	</servlet-mapping>	
	
	<welcome-file-list>
	   <welcome-file>index.jsp</welcome-file>
	</welcome-file-list>
</web-app>
 注意触发pushlet这个servlet的条件是请求pushlet.srv。
接下来就是自定义推送源，MyPushSource.java代码如下：
Java代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- package com.mysrc;  
- 
- import java.io.Serializable;  
- import nl.justobjects.pushlet.core.Event;  
- import nl.justobjects.pushlet.core.EventPullSource;  
- 
- publicclass MyPushSource implements Serializable {  
- publicstaticclass MySource1 extends EventPullSource {   
- @Override
- protectedlong getSleepTime() {   
- return1000;     
-         }   
- @Override
- protected Event pullEvent() {   
-             Event event =Event.createDataEvent("myevent1");   
-             event.setField("key1","my_value1");   
- return event;    
-         }   
-      }   
- }  
```java
package com.mysrc;
import java.io.Serializable;
import nl.justobjects.pushlet.core.Event;
import nl.justobjects.pushlet.core.EventPullSource;
public class MyPushSource implements Serializable {
	 public static class MySource1 extends EventPullSource { 
        @Override 
        protected long getSleepTime() { 
            return 1000;   
	    } 
        @Override 
        protected Event pullEvent() { 
            Event event =Event.createDataEvent("myevent1"); 
            event.setField("key1","my_value1"); 
            return event;  
        } 
	 } 
}
```
 每隔一秒钟就向订阅了myevent1的所有前端页面推送信息，这里是一个键值对。
然后需要在sources.properties声明该源（该文件现在只有这一行内容）：
source1=com.mysrc.MyPushSource$MySource1
前台页面test.html代码如下：
Html代码 ![复制代码](http://tonl.iteye.com/images/icon_copy.gif)![收藏代码](http://tonl.iteye.com/images/icon_star.png)![](http://tonl.iteye.com/images/spinner.gif)
- <html>
- <head>
- <scripttype="text/javascript"src="ajax-pushlet-client.js"></script>
- <scripttype="text/javascript">
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
 PL是在ajax-pushlet-client.js中定义的。好了，将工程部署到Tomcat，启动，请求test.hmtl，就能看到my_value1的alert的显示了。
需要说明的是，也可以将test.html和ajax-pushlet-client.js放在本地，不用请求服务器上的，因为这儿的关键是触发服务器上的pushlet那个servlet，之后能进行心跳同步。打开ajax-pushlet-client.js，找到PL.pushletURL = PL._getWebRoot() + 'pushlet.srv';这一行，这儿PL._getWebRoot() 是通过ajax-pushlet-client.js在服务器上的路径得到url的。这儿咋们自己换成服务器的地址就可以了。我的为：PL.pushletURL
 = "[http://192.168.1.154:7087/PushletTest/](http://192.168.1.154:7087/PushletTest/)" + 'pushlet.srv';
OK，然后用浏览器打开这个本地的test.html页面就可以了。
