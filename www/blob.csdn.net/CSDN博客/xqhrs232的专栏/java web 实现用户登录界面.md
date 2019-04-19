# java web 实现用户登录界面 - xqhrs232的专栏 - CSDN博客
2018年10月18日 16:12:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：244
原文地址::[https://blog.csdn.net/TYPYDEF/article/details/78137337?utm_source=blogxgwz3](https://blog.csdn.net/TYPYDEF/article/details/78137337?utm_source=blogxgwz3)
相关文章
1、JAVAWEB用户登录功能的实现----[https://wenku.baidu.com/view/4482957eff4733687e21af45b307e87101f6f89e.html](https://wenku.baidu.com/view/4482957eff4733687e21af45b307e87101f6f89e.html)
2、Java Web两种登录页面实现方式----[https://wenku.baidu.com/view/9efbe4b19f3143323968011ca300a6c30c22f183.html](https://wenku.baidu.com/view/9efbe4b19f3143323968011ca300a6c30c22f183.html)
前面写的都是C语言的程序，今天开始陆续连载java语言的。
就以java web实现用户登录界面的一个简单web应用程序开头吧。
1.提交页面userLogin.jsp,主要代码如下：
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<form action="userReceive.jsp" method="post">
姓  名：<input type="text" name="RdName"><br>
密  码：<input type="password" name="RdPasswd"><br><br>
<input type="submit" value="确定">
</form>
2.接受信息并验证程userReceive.jsp，关键代码如下：
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
  <body>
    <%String Name=request.getParameter("RdName");
    String Passwd=request.getParameter("RdPasswd");
     if (Name.equals("lichaoyu")&&Passwd.equals("123456")){%>
     <jsp:forward page="loginCorrect.jsp"/>
     <%}else{%>
     <jsp:forward page="loginFalse.jsp"/>
     <%}%>
  </body>
3.成功登入页面loginCorrect.jsp,关键代码如下：
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
  <body>
    <%String Name=request.getParameter("RdName"); %>
    欢迎，<%=Name%>成功登录！
  </body>
4.登录失败页面loginFalse.jsp,关键代码如下：
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
  <body>
  用户名或密码错误，登录失败！<br>
  </body>
--------------------- 
作者：_Black_Sky 
来源：CSDN 
原文：https://blog.csdn.net/TYPYDEF/article/details/78137337 
版权声明：本文为博主原创文章，转载请附上博文链接！
