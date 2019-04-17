# javaweb基础四课：web.xml解读与Servlet - PeterBishop - CSDN博客





2018年12月21日 18:11:36[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：22
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来讲web.xml和Servlet



  web.xml是整个web项目的核心配置文件，几乎所有的配置都需要写在web.xml里。



  web.xml
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

</web-app>




