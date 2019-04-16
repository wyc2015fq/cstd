# javaweb基础第二十七课：文件下载 - PeterBishop - CSDN博客





2019年01月19日 18:03:03[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：21








FileDownloadServlet.java:
package com.servlet;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;



//文件下载Servlet

public class FileDownloadServlet extends HttpServlet {



    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {



        //如果想要实现下载的功能，下面两句必须加

        resp.setContentType("application/x-msdownload");

        resp.setHeader("Content-Disposition","attachment;filename=abc.txt");

        //filename后的abc.txt可以换成自己需要的文件名，这个文件名是下载时的文件名，不是原文件名



        req.getRequestDispatcher("/upload/1.txt").forward(req,resp);

        //request.getRequestDispatcher()的参数就是原文件的相对路径



    }

}


web.xml
<servlet>

    <servlet-name>FileDownloadServlet</servlet-name>

    <servlet-class>com.servlet.FileDownloadServlet</servlet-class>

</servlet>

<servlet-mapping>

    <servlet-name>FileDownloadServlet</servlet-name>

    <url-pattern>/download</url-pattern>

</servlet-mapping>


file_download.jsp
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>Title</title>

</head>

<body>

    abc.txt&nbsp;<a href="/download">下载文件</a>

</body>

</html>




