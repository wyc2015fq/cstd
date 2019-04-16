# javaweb基础第十五课：ServletContext接口 - PeterBishop - CSDN博客





2018年12月31日 15:01:05[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：17
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来讲ServletContext接口。


public interface ServletContext 
  这个接口非常单纯，没有任何父接口



  说下ServletContext到底是个啥。

  ServletContext类型的对象是java web项目的一个全局对象，整个web项目只有一个ServletContext对象。

  ServletComtext对象的生命周期为从服务器启动到服务器停止。

  ServletContext对象对应jsp内置对象的application对象

  ServletContext对象可以保存一些全局信息



ServletComtextTeachServlet.java
package com.servlet;



import javax.servlet.ServletContext;

import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;



*/** * **用于讲解ServletContext接口的Servlet类 */*public class ServletContextTeachServlet extends HttpServlet {



    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {



        */**         * 1. **如何获取ServletContext对象         *   this.getServletContext() 或者 this.getServletConfig().getServletContext()         *   解释： HttpServlet继承了GenericServlet，而GenericServlet实现了ServletConfig接口         *          所以ServletConfig能干的事HttpServlet都能干         */        *ServletContext context = this.getServletContext();



        */**         * **可以看到ServletContext里有很多方法，挑常用的说         */        *//获取web项目的初始化参数，该参数的配置在web.xml的<context-param>标签里

        String initParameter = context.getInitParameter("name");



        //设置application域的参数

        context.setAttribute("age",20);



        //获取application域的参数

        Object age = context.getAttribute("age");

        System.*out*.println(age);



        req.getRequestDispatcher("/WEB-INF/app.jsp").forward(req,resp);

        //别忘了在web.xml里配置下

    }

}


web.xml
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"

         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"

         version="4.0">

    

    

    <context-param>

        <param-name>name</param-name>

        <param-value>JavaWebTeach02Project</param-value>

    </context-param><servlet>

        <servlet-name>ServletContextTeachServlet</servlet-name>

        <servlet-class>com.servlet.ServletContextTeachServlet</servlet-class>

    </servlet>

    

    <servlet-mapping>

        <servlet-name>ServletContextTeachServlet</servlet-name>

        <url-pattern>/app</url-pattern>

    </servlet-mapping>

</web-app>


/WEB-INF/app.jsp
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>Title</title>

</head>

<body>

    年龄: **<%=**application.getAttribute("age")**%>**</body>

</html>




