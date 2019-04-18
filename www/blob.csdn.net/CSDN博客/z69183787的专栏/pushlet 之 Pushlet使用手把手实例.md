# pushlet 之 Pushlet使用手把手实例 - z69183787的专栏 - CSDN博客
2014年03月22日 13:30:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2179
Pushlet（一种comet 架构的实现）是基于Servlet 机制，数据从server端的Java 对象直接推送（push）到客户端浏览器的（动态）HTML 页面，而无需任何Java applet 或者插件的帮助。
- 在MyEclipse中创建工程。附件包含了从官方下载的Pushlet包已经本次的源码包，直接导入到开发工具即可。
- 在创建的工程中增加pushlet.jar，log4j.jar。拷贝pushlet.properties，sources.properties到WEB-INF下。拷贝ajax-pushlet-client.js到WebRoot下。
- 配置web.xml
Xml代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE web-app  
-     PUBLIC "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"  
-     "http://java.sun.com/dtd/web-app_2_3.dtd">
- <!-- $Id: web.xml,v 1.7 2005/02/21 17:21:49 justb Exp $ -->
- <web-app>
- <!-- Define the pushlet servlet -->
- <servlet>
- <servlet-name>pushlet</servlet-name>
- <servlet-class>nl.justobjects.pushlet.servlet.Pushlet</servlet-class>
- <load-on-startup>3</load-on-startup>
- </servlet>
- <!-- Define the Servlet Mappings. -->
- <!-- The pushlet -->
- <servlet-mapping>
- <servlet-name>pushlet</servlet-name>
- <url-pattern>/pushlet.srv</url-pattern>
- </servlet-mapping>
- </web-app>
- 后台代码HelloWorldPlushlet.java，该类包含一个内部类
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- package com;  
- import nl.justobjects.pushlet.core.Event;  
- import nl.justobjects.pushlet.core.EventPullSource;  
- publicclass HelloWorldPlushlet {     
- staticpublicclass HwPlushlet extends EventPullSource {  
- // 休眠五秒
- @Override
- protectedlong getSleepTime() {  
- return5000;  
-         }  
- @Override
- protected Event pullEvent() {  
-             Event event = Event.createDataEvent("/cuige/he");  
-             event.setField("mess", "hello,world!Plushlet!");  
- return event;  
-         }  
-     }  
- }  
-  sources.properties清理其他，定义自己发布的内容
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- source1=com.HelloWorldPlushlet$HwPlushlet  
-  在JSP中调用
Jsp代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>  
- <html>  
-     <head>  
-         <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />  
-         <meta http-equiv="Pragma" content="no-cache" />  
-         <script type="text/javascript" src="ajax-pushlet-client.js"></script>         
-         <script type="text/javascript">  
-             PL._init();   
-             PL.joinListen('/cuige/he');  
-             function onData(event) {   
-                 alert(event.get("mess"));   
-                 // 离开  
-                 // PL.leave();  
-             }  
-         </script>  
-     </head>  
-     <body>  
-         <center>  
-         <h1>  
-             my first pushlet!  
-         </h1>  
-         </center>  
-     </body>  
- </html>  
-  将工程部署到Tomcat后访问，该页面以五秒钟一个间隔，收到后台传来的数据！
工程部署后如下图展示
![](http://dl.iteye.com/upload/picture/pic/107328/3c590519-20e0-3730-874c-e249fe5a9c6f.png)
 每次访问时，后台都会打印增加到管理中的用户信息，当关闭浏览器后，服务器会在超时后自动清理！
