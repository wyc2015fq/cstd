# javaweb基础第七课：登录功能的实现 - PeterBishop - CSDN博客





2018年12月21日 18:27:28[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：42
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来做个登录功能



login.jsp:
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>登录</title>

</head>

<body style="text-align: center">

    <form style="margin-top: 100px;border: 1px solid black;text-align: center" action="/login" method="post">

        <input type="text" name="username" placeholder="用户名"><br><br>

        <input type="password" name="password" placeholder="密码"><br><br>

        <button type="submit">登录</button>

    </form>

</body>

</html>


login_success.jsp:
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>登录</title>

</head>

<body style="text-align: center">

    <form style="margin-top: 100px;border: 1px solid black;text-align: center" action="/login" method="post">

        <input type="text" name="username" placeholder="用户名"><br><br>

        <input type="password" name="password" placeholder="密码"><br><br>

        <button type="submit">登录</button>

    </form>

</body>

</html>


web.xml
<?xml version="1.0" encoding="UTF-8"?>

<!--web-app 是整个web.xml的根标签，所有的配置都必须写在web-app里-->

<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"

         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"

         version="4.0">

    <!--

        welcome-file 设置欢迎页

        可以看到欢迎页面设置成功

    -->

    <welcome-file-list>

        <welcome-file>/WEB-INF/login.jsp</welcome-file>

    </welcome-file-list>



       <!--配置LoginServlet与/login请求映射-->

    <servlet>

        <servlet-name>LoginServlet</servlet-name>

        <servlet-class>com.login.LoginServlet</servlet-class>

    </servlet>

    

    <servlet-mapping>

        <servlet-name>LoginServlet</servlet-name>

        <url-pattern>/login</url-pattern>

    </servlet-mapping>

</web-app>


LoginServlet.java:
package com.login;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;



//这个servlet用来处理登录请求

public class LoginServlet extends HttpServlet {



    String username = "xiaoye";

    String password = "123456";



    @Override

    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        String username = req.getParameter("username");

        String password = req.getParameter("password");



        if (this.username.equals(username) && this.password.equals(password))

        {

            //跳转到登录成功的页面

            */**             * **跳转页面使用request.getRequestDispatcher(要跳转的页面的路径).forward(request,response);             */                          *req.getRequestDispatcher("/WEB-INF/login_success.jsp").forward(req,resp);

            //OK，成功了

        }

    }

}




