# HttpSessionListener, HttpSessionAttributeListener的用法 - z69183787的专栏 - CSDN博客
2014年03月23日 17:52:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1244
HttpSessionListener有2个接口需要实现
sessionCreated//新建一个会话时候触发也可以说是客户端第一次和服务器交互时候触发
sessionDestroyed//销毁会话的时候  一般来说只有某个按钮触发进行销毁 或者配置定时销毁 （ 很多文献中提到说浏览器关闭时候会销毁 但是楼主通过各种现行主流浏览器测试效果不尽如人意）
HttpSessionAttributeListener有3个接口需要实现
attributeAdded//在session中添加对象时触发此操作 笼统的说就是调用setAttribute这个方法时候会触发的
attributeRemoved//修改、删除session中添加对象时触发此操作  笼统的说就是调用 removeAttribute这个方法时候会触发的
attributeReplaced//在Session属性被重新设置时
以下是一个统计在线会话数的功能，并且让超时的自动销毁
web.xml
**[html]**[view
 plain](http://blog.csdn.net/yzsy0818/article/details/7670619#)[copy](http://blog.csdn.net/yzsy0818/article/details/7670619#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.4"xmlns="http://java.sun.com/xml/ns/j2ee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee   
-     http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
- <listener>
- <listener-class>
-             org.xiosu.listener.onlineListener  
- </listener-class>
- </listener>
- 
- 
- <!--默认的会话超时时间间隔，以分钟为单位  -->
- <session-config>
- <session-timeout>1</session-timeout>
- </session-config>
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- 
- </web-app>
**[html]**[view
 plain](http://blog.csdn.net/yzsy0818/article/details/7670619#)[copy](http://blog.csdn.net/yzsy0818/article/details/7670619#)
- 
onlineListener.java
**[java]**[view
 plain](http://blog.csdn.net/yzsy0818/article/details/7670619#)[copy](http://blog.csdn.net/yzsy0818/article/details/7670619#)
- package org.xiosu.listener;  
- 
- import java.util.ArrayList;  
- import javax.servlet.ServletContext;  
- import javax.servlet.http.HttpSessionAttributeListener;  
- import javax.servlet.http.HttpSessionBindingEvent;  
- import javax.servlet.http.HttpSessionEvent;  
- import javax.servlet.http.HttpSessionListener;  
- 
- publicclass onlineListener implements HttpSessionListener,  
-         HttpSessionAttributeListener {  
- // 参数
-     ServletContext sc;  
- 
-     ArrayList list = new ArrayList();  
- 
- // 新建一个session时触发此操作
- publicvoid sessionCreated(HttpSessionEvent se) {  
-         sc = se.getSession().getServletContext();  
-         System.out.println("新建一个session");  
-     }  
- 
- // 销毁一个session时触发此操作
- publicvoid sessionDestroyed(HttpSessionEvent se) {  
-         System.out.println("销毁一个session");  
- if (!list.isEmpty()) {  
-             list.remove((String) se.getSession().getAttribute("userName"));  
-             sc.setAttribute("list", list);  
-         }  
-     }  
- 
- // 在session中添加对象时触发此操作，在list中添加一个对象
- publicvoid attributeAdded(HttpSessionBindingEvent sbe) {  
-         list.add((String) sbe.getValue());  
-         System.out.println(sbe.getValue());  
-         sc.setAttribute("list", list);  
-     }  
- 
- // 修改、删除session中添加对象时触发此操作
- publicvoid attributeRemoved(HttpSessionBindingEvent arg0) {  
- 
-         System.out.println("5555555");  
-     }  
- 
- publicvoid attributeReplaced(HttpSessionBindingEvent arg0) {  
-         System.out.println("77777777");  
-     }  
- }  
index.jsp
**[html]**[view
 plain](http://blog.csdn.net/yzsy0818/article/details/7670619#)[copy](http://blog.csdn.net/yzsy0818/article/details/7670619#)
- <%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%>
- <%  
-     String path = request.getContextPath();  
-     String basePath = request.getScheme() + "://"  
-             + request.getServerName() + ":" + request.getServerPort()  
-             + path + "/";  
- %>
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
- <html>
- <head>
- <basehref="<%=basePath%>">
- 
- <title>My JSP 'index.jsp' starting page</title>
- <metahttp-equiv="pragma"content="no-cache">
- <metahttp-equiv="cache-control"content="no-cache">
- <metahttp-equiv="expires"content="0">
- <metahttp-equiv="keywords"content="keyword1,keyword2,keyword3">
- <metahttp-equiv="description"content="This is my page">
- <!--
-     <link rel="stylesheet" type="text/css" href="styles.css">
-     -->
- </head>
- 
- <body>
- <%  
- session = request.getSession(false);  
- 
-             if (session != null)  
-                 session.invalidate();  
-         %>
- <formaction="isOnline.jsp"method="post">
-             用户名：  
- <inputtype="text"name="uName"/>
- <inputtype="submit"value="上线">
- </body>
- </html>
isOnline.jsp
**[html]**[view
 plain](http://blog.csdn.net/yzsy0818/article/details/7670619#)[copy](http://blog.csdn.net/yzsy0818/article/details/7670619#)
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
- <title>My JSP 'isOnline.jsp' starting page</title>
- 
- <metahttp-equiv="pragma"content="no-cache">
- <metahttp-equiv="cache-control"content="no-cache">
- <metahttp-equiv="expires"content="0">
- <metahttp-equiv="keywords"content="keyword1,keyword2,keyword3">
- <metahttp-equiv="description"content="This is my page">
- <!--
-     <link rel="stylesheet" type="text/css" href="styles.css">
-     -->
- 
- </head>
- 
- <body>
- <%  
- 
- session=request.getSession();  
- 
- session.setAttribute("userName",request.getParameter("uName"));  
- 
- response.sendRedirect("showOnline.jsp");  
- %>
- </body>
- </html>
showOnline.jsp
**[html]**[view
 plain](http://blog.csdn.net/yzsy0818/article/details/7670619#)[copy](http://blog.csdn.net/yzsy0818/article/details/7670619#)
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
- <title>My JSP 'showOnline.jsp' starting page</title>
- 
- <metahttp-equiv="pragma"content="no-cache">
- <metahttp-equiv="cache-control"content="no-cache">
- <metahttp-equiv="expires"content="0">
- <metahttp-equiv="keywords"content="keyword1,keyword2,keyword3">
- <metahttp-equiv="description"content="This is my page">
- <!--
-     <link rel="stylesheet" type="text/css" href="styles.css">
-     -->
- 
- </head>
- 
- <body>
- <%  
- ArrayList showList=(ArrayList)(getServletContext().getAttribute("list"));  
- out.print("在线人数 "+showList.size()+"<br>");  
- for(int i=0;i<showList.size();i++){  
- out.print(showList.get(i)+"在线"+"<br>");  
- }  
- %>
- <br>
- <ahref="index.jsp">退出</a>
- </body>
- </html>
