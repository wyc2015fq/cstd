# JavaWeb —— HttpServlet 类 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:22:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：23








# 【概述】

虽然 GenericServlet 类实现了 javax.servlet.Servlet 接口与 javax.servlet.ServletConfig 接口，简化了代码，但实际开发中，大多需要使用 Servlet 处理 HTTP 协议的请求并对请求做出响应，所以通过继承 GenericServlet 类不是很灵活，因为经常需要将 ServletRequest/ServletResponse 对象强转为 HttpServletRequest/HttpServletResponse 对象。

javax.servlet.http.HttpServlet 类对 GenericServlet 类进行了扩展，为 HTTP 请求的处理提供了灵活的方法。

HttpServlet 类是一个抽象类，实现了 service() 方法，针对 HTTP 定义的 7 种请求类型提供了相应的方法：doGet()、doPost()、doPut()、doDelete()、doHead()、doTrace()、doOptions() ，在这 7 种方法中，除了 doTrace()、doOptions() 方法进行了简单实现外，并没有对其他方法进行实现，需要在使用过程中根据实际需要进行重写。

# 【HttpServlet 对象】

在 Java 中，通常说的 Servlet 是指 HttpServlet 对象，在声明一个对象为 Servlet 时，需要继承 HttpServlet 类，继承该类后，可以重写 HttpServlet 类中的方法对 HTTP 请求进行处理。

一个 Servlet 对象结构如下，通过继承 HttpServlet 类被声明为一个 Servlet 对象，其中包含 6 个方法，其中 init()、destroy() 方法为 Servlet 初始化与生命周期结束所调用的方法，其余 4 个方法为 Servlet 针对处理不同的 HTTP 请求类型所提供的方法，例如：<form> 表单对象所声明的 method 属性为 post，提交到 Servlet 对象处理时，Servlet 将调用 doPost() 方法进行处理。

```java
public class Test extends HttpServlet{
    private static final long serialVersionUID = 1L;
    //构造器
    public Test() {
        super();
    }
    //初始化方法
    public void init() throws ServletException{
    }
    //处理HTTP的GET请求
    public void doGet(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{	
    }
    //处理HTTP的POST请求
    public void doPost(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{
    }
    //处理HTTP的PUT请求
    public void doPut(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{
    }
    //处理HTTP的DELETE请求
    public void doDelete(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{
    }
    //销毁方法
    public void destroy() {
        super.destroy();
    }
}
```

# 【实例】

1.创建名为 Test 的Servlet 对象，在该类中重写 doGet() 方法，用于处理 HTTP 的 GET 请求，通过 PrintWriter 对象进行简单输出。

```java
package servlet;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class Test extends HttpServlet {
    private static final long serialVersionUID = 1L;
       
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //设置请求以及响应的内容类型以及编码方式
        response.setContentType("text/html; charset=UTF-8");
        response.setCharacterEncoding("UTF-8");
		
        PrintWriter out=response.getWriter();
        out.println("hello,world");
    }
}
```

2.在 web.xml 文件对 Test 进行配置

```
<servlet>
  <servlet-name>out</servlet-name>
  <servlet-class>servlet.Test</servlet-class>
</servlet>
<servlet-mapping>
  <servlet-name>out</servlet-name>
  <url-pattern>/test</url-pattern>
</servlet-mapping>
```





