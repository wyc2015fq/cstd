# javaweb基础第十课：jsp的9大隐式对象 - PeterBishop - CSDN博客





2018年12月25日 15:23:52[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：22








  这节课我们讲jsp的九大隐式对象





![](https://img-blog.csdnimg.cn/20181225152308988.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

重点是前五个，response对象用的都少，这节课主讲out和request



jsp_teach02.jsp

```
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>

<head>

    <title>Title</title>

</head>

<body>

<%--<%%>里也不可以写jsp的注释，总之就是不可以出现 '%>' --%>

<%-- out.print()方法等价于 <%= %>--%>

    <%

        out.print("123");

        //out对象可以输出html

        out.print("<a href='http://www.baidu.com'>百度一下，你就知道</a>");





    %>

    <%--注意，<%=%> 这个不能写在<%%>里 --%>

    <%="123" %>

<br>

<%="<a href='http://www.baidu.com'>百度一下，你就知道</a>"%>



<%--下面说request对象，用的最多的就是setAttribute和getAttribute--%>

<a href="/testRequest">测试request</a>

<%=request.getAttribute("msg")%>

<%--

    这里的request取的就是TestRequestServlet的service方法里req设置的attribute

    request.setAttribute()和request.getAttribute()方法常用来设置各种提示信息或者用来存储页面展示需要的数据

--%>

</body>

</html>
```



TestRequestServlet.java:

```
package com.servlet;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;



public class TestRequestServlet extends HttpServlet {



    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        req.setAttribute("msg","测试request对象");

        req.getRequestDispatcher("/WEB-INF/jsp_teach02.jsp").forward(req,resp);

    }

    //写完别忘了配web.xml

}
```





