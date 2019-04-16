# javaweb基础第十四课：ServletConfig接口 - PeterBishop - CSDN博客





2018年12月31日 14:58:17[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来讲ServletConfig接口



  在讲ServletConfig之前我们先来说下Servlet，来看看HttpServlet类
public abstract class HttpServlet extends GenericServlet
  可以看到，HttpServlet类是继承自GenericServlet类



  再来看看GenericServlet类
public abstract class GenericServlet implements Servlet, ServletConfig, Serializable
  我们发现了一个很神奇的事情，那就是GenericServlet实现了ServletConfig接口，也就是说GenericServlet对象本身就是一个ServletConfig对象，然后，HttpServlet又继承了GenericServlet，我们自定义的Servlet又继承了HttpServlet，所以，我们自定义的Servlet其实也是一个ServletConfig



  那么，ServletConfig到底是什么呢？
public interface ServletConfig {

    

    //获取servlet名称，就是在web.xml里配置的servlet-name

    String getServletName();



    //获取ServletContext对象

    ServletContext getServletContext();



    //获取初始化参数，就是在web.xml的servlet标签里的init-param子标签的内容

    String getInitParameter(String name);



    //获取初始化参数的参数名的集合

    Enumeration<String> getInitParameterNames();

}


如何获取Servlet对应的ServletConfig呢？

先来看看Servlet接口
public interface Servlet {

    void init(ServletConfig var1) throws ServletException;



//获取该servlet对应的ServletConfig对象ServletConfig getServletConfig();



    void service(ServletRequest var1, ServletResponse var2) throws ServletException, IOException;



    String getServletInfo();



    void destroy();

}
所以，servlet只需要调用自身的getServletConfig()方法就可以获取到ServletConfig对象


package com.servlet;



import javax.servlet.ServletConfig;

import javax.servlet.ServletContext;

import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;

import java.util.Enumeration;



*/** * **用于讲解ServletConfif接口的Servlet类 */*public class ServletConfigTeachServlet extends HttpServlet {



    /*

        web.xml中的配置

    <servlet>

        <servlet-name>ServletConfigTeachServlet</servlet-name>

        <servlet-class>com.servlet.ServletConfigTeachServlet</servlet-class>

        <init-param>

            <param-name>name</param-name>

            <param-value>xiaoye</param-value>

        </init-param>

        <init-param>

            <param-name>age</param-name>

            <param-value>20</param-value>

        </init-param>

    </servlet>



    <servlet-mapping>

        <servlet-name>ServletConfigTeachServlet</servlet-name>

        <url-pattern>/servletConfig</url-pattern>

    </servlet-mapping>

     */

    

    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        System.*out*.println("ServletConfigTeachServlet起作用啦");

        ServletConfig servletConfig = this.getServletConfig();



        String servletName = servletConfig.getServletName();

        System.*out*.println("servlet-name: " + servletName);

        // servlet-name: ServletConfigTeachServlet



        Enumeration<String> initParameterNames = servletConfig.getInitParameterNames();

        while (initParameterNames.hasMoreElements())

        {

            String paramName = initParameterNames.nextElement();

            String paramValue = servletConfig.getInitParameter(paramName);

            System.*out*.println("init-param: " + paramName + " -> " + paramValue);

        }

        /*

           init-param: name -> xiaoye

           init-param: age -> 20

         */



        //这个就演示下，下节课讲ServletContext对象

        ServletContext servletContext = servletConfig.getServletContext();



    }

}



<!--

    这里有个问题，ServletConfigTeachServlet和LifeCycleTeachServlet都可以匹配到/servletConfig请求

    那么，请问当/servletConfig请求到来时，谁起作用?

    A. 只有ServletConfigTeachServlet起作用

    B. 只有LifeCycleTeachServlet起作用

    C. LifeCycleTeachServlet先起作用，ServletConfigTeachServlet后起作用

    D. ServletConfigTeachServlet先起作用，LifeCycleTeachServlet后起作用



    大家猜一下

    OK，验证下

    答案是A

    大家可能会奇怪，为什么答案是A啊，“LifeCycleTeachServlet起作用啦”也输出了啊

    因为，请求路径是: "http://localhost:8080/favicon.ico"，并不是/servletConfig请求



    结论:

        一个请求只可能被分发给一个Servlet，匹配规则: 具体优先于模糊，/servletConfig肯定比/具体吧



    来试下其他请求,比如/123

    LifeCycleTeachServlet起作用啦

    请求过来啦

    请求路径是: "http://localhost:8080/123"

    LifeCycleTeachServlet起作用啦

    请求过来啦

    请求路径是: "http://localhost:8080/favicon.ico"



-->




