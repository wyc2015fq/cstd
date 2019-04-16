# javaweb基础第十六课：HttpServletResponse接口 - PeterBishop - CSDN博客





2019年01月02日 19:13:11[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：29
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来讲HttpServletResponse接口


public interface HttpServletResponse extends ServletResponse
  可以看到HttpServletResponse接口是ServletResponse的子接口

  里面有很多方法，就不细讲了，反正也不常用



  说下HttpServletResponse的作用:
- 可以通过HttpServletResponse对象获取PrintWriter后向页面输出数据
- 重定向



HttpServletResponseTeachServlet.java
package com.servlet;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;

import java.io.PrintWriter;



*/** * HttpServletResponse**的作用: * 1.可以通过HttpServletResponse对象获取PrintWriter后向页面输出数据 * 2.重定向 */*public class HttpServletResponseTeachServlet extends HttpServlet {



    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        String url = req.getRequestURL().toString();

        if (url.endsWith("/printWriter"))

        {

            printWriterTeach(req,resp);

        }

        if (url.endsWith("/redirect"))

        {

            redirectTeach(req,resp);

        }

        if (url.endsWith("/123"))

        {

            System.*out*.println("重定向到请求 /123");

        }

    }



    private void printWriterTeach(HttpServletRequest req, HttpServletResponse resp) {

        PrintWriter writer = null;

        try {

            //这个是设置字符编码

            //不过过滤器应该已经过滤好了

            resp.setCharacterEncoding("UTF-8");

            writer = resp.getWriter();

            writer.write("<a href='http://www.oschina.net'>开源中国</a>");

            //Chrome默认编码好像是gbk，我等会去设置一下

            //360浏览器可以自动检测出UTF-8编码



        } catch (IOException e) {

            e.printStackTrace();

        }finally {

            if (writer != null)

                writer.close();

        }

    }



    private void redirectTeach(HttpServletRequest req, HttpServletResponse resp) {

        try {

            resp.sendRedirect("/123");

            //重定向: 向服务器发起新的请求，且浏览器地址栏变成重定向请求的url

        } catch (IOException e) {

            e.printStackTrace();

        }

    }





}


web.xml
<servlet>

    <servlet-name>ResponseServlet</servlet-name>

    <servlet-class>com.servlet.HttpServletResponseTeachServlet</servlet-class>

</servlet>



<servlet-mapping>

    <servlet-name>ResponseServlet</servlet-name>

    <url-pattern>/printWriter</url-pattern>

</servlet-mapping>



<servlet-mapping>

    <servlet-name>ResponseServlet</servlet-name>

    <url-pattern>/redirect</url-pattern>

</servlet-mapping>



<servlet-mapping>

    <servlet-name>ResponseServlet</servlet-name>

    <url-pattern>/123</url-pattern>

</servlet-mapping>




