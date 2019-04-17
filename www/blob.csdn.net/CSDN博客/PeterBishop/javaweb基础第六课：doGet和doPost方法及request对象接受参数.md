# javaweb基础第六课：doGet和doPost方法及request对象接受参数 - PeterBishop - CSDN博客





2018年12月21日 18:21:48[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：67








  这节课我们讲讲doGet和doPost方法以及request对象接收参数



RequestServlet.java:


package com.teach;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;



*/** * **自定义一个servlet */*public class RequestServlet extends HttpServlet {



    */**     * **当浏览器发送/get路径的请求是会被分发到这个servlet     * 如果请求方式是get，则会调用doGet方法     * 如果请求方式是post，则或调用doPost方法     * 如果没有重写doGet方法和doPost方法，则会默认调用service方法     * **@param ****req     *** **@param ****resp     *** **@throws **ServletException     * **@throws **IOException     */    *@Override

    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        /*

            HttpServletRequest 类型的对象我们称为request对象(请求对象)

            request对象有个getParameter(String name)方法可以获取请求传来的参数,返回值是String

            如果request要获取的参数的名字请求里没有就会返回null

         */

        String username = req.getParameter("username");

        String password = req.getParameter("password");

        String code = req.getParameter("code");

        System.*out*.println("username: " + username);

        System.*out*.println("password: " + password);

        System.*out*.println("code: " + code);

        //可以看到我们完美的获取到了username和password参数



    }



    @Override

    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        String id = req.getParameter("id");

        String name = req.getParameter("name");

        System.*out*.println("id: " + id);

        System.*out*.println("name: " + name);

        //可以看到post请求会执行这个方法，且参数成功获取

    }

}


index.jsp:
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

  <head>

    <title>welcome</title>

  </head>

  <body>

   欢迎页面

   <a href="/test" onclick="javascript:**void**(0)">测试一下</a>



   <!--

        写个表单，发送一个get请求，url为/get，参数为username和password

        然后里面写个提交按钮

        表单里发请求好像不能发get请求还带参数。。。，那用a标签

   -->

   <a href="/get?username=1234&password=5678">get</a>



   <!--

        来试试post请求



        post请求使用form表单发送

        form表单里的input标签的value值会作为参数值，name属性会作为参数名

        使用submit按钮提交请求

    -->

   <form action="/post" method="post">

       id: <input type="text" name="id" > <br> <br>

       name: <input type="text" name="name" > <br> <br>

       <button type="submit">post</button>

   </form>

  </body>

</html>


web.xml:
<?xml version="1.0" encoding="UTF-8"?>

<!--web-app 是整个web.xml的根标签，所有的配置都必须写在web-app里-->

<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"

         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"

         version="4.0">

    <!--先来说下怎么分配浏览器发来的请求-->



    <!--

        什么是servlet？

          servlet就是一个用于处理浏览器发来的请求的对象

    -->



    <!--配置servlet-->

    <servlet>

        <!--servlet-name 设置配置的servlet的名称-->

        <servlet-name>myServlet</servlet-name>

        <!--servlet-class 设置该servlet对应的类-->

        <servlet-class>com.teach.MyServlet</servlet-class>

    </servlet>



    <!--分配请求由 servlet-mapping 标签完成-->

    <servlet-mapping>

        <!--servlet-name里写要分配给的Servlet-->

        <servlet-name>myServlet</servlet-name>

        <!--url-pattern里写请求路径-->

        <url-pattern>/test</url-pattern>

    </servlet-mapping>

    

    <!--配置我们刚刚写好的RequestServlet-->

    <servlet>

        <servlet-name>requestServlet</servlet-name>

        <servlet-class>com.teach.RequestServlet</servlet-class>

    </servlet>

    

    <servlet-mapping>

        <servlet-name>requestServlet</servlet-name>

        <url-pattern>/get</url-pattern>

    </servlet-mapping>



    <!--同一个servlet可以配置多个请求路径-->

    <servlet-mapping>

        <servlet-name>requestServlet</servlet-name>

        <url-pattern>/post</url-pattern>

    </servlet-mapping>

</web-app>




