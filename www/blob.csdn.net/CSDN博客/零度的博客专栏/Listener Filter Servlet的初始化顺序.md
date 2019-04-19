# Listener/Filter/Servlet的初始化顺序 - 零度的博客专栏 - CSDN博客
2016年05月13日 14:39:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：1824
         Listener、Filter、Servlet都有一个初始化的过程，对应的初始化方法分别为：
        contextInitialized(ServletContextEvent arg0)
        init(FilterConfig filterConfig)
        init(ServletConfig config)
        那么，它们的初始化顺序是什么呢？看下面的测试实例：
TestServlet.java
Java代码  ![收藏代码](http://tianhei.iteye.com/images/icon_star.png)
- package com.cos;  
- import java.io.IOException;  
- import javax.servlet.GenericServlet;  
- import javax.servlet.ServletConfig;  
- import javax.servlet.ServletException;  
- import javax.servlet.ServletRequest;  
- import javax.servlet.ServletResponse;  
- 
- publicclass TestServlet extends GenericServlet {  
- @Override
- publicvoid init(ServletConfig config){  
-         System.out.println("Servlet 初始化 。。。");  
-     }  
- @Override
- publicvoid service(ServletRequest arg0, ServletResponse arg1) throws ServletException, IOException {  
-         System.out.println("Servlet service 。。。");  
-     }  
- }  
TestFilter.java
Java代码  ![收藏代码](http://tianhei.iteye.com/images/icon_star.png)
- package com.cos;  
- import java.io.IOException;  
- import javax.servlet.Filter;  
- import javax.servlet.FilterChain;  
- import javax.servlet.FilterConfig;  
- import javax.servlet.ServletException;  
- import javax.servlet.ServletRequest;  
- import javax.servlet.ServletResponse;  
- 
- publicclass TestFilter implements Filter{  
- publicvoid init(FilterConfig filterConfig) throws ServletException {  
-         System.out.println("Filter 初始化。。。");  
-     }  
- publicvoid doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {  
-         System.out.println("doFilter 。。。");  
-         chain.doFilter(request, response);  
-     }  
- publicvoid destroy() {  
-         System.out.println("Filter 销毁。。。");  
-     }  
- }  
TestListener.java
Java代码  ![收藏代码](http://tianhei.iteye.com/images/icon_star.png)
- package com.cos;  
- import javax.servlet.ServletContextEvent;  
- import javax.servlet.ServletContextListener;  
- 
- publicclass TestListener implements ServletContextListener{  
- publicvoid contextInitialized(ServletContextEvent arg0) {  
-         System.out.println("Listener 初始化。。。");  
-     }  
- publicvoid contextDestroyed(ServletContextEvent arg0) {  
-         System.out.println("Listener 销毁。。。");  
-     }  
- }  
web.xml
Xml代码  ![收藏代码](http://tianhei.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.5"xmlns="http://java.sun.com/xml/ns/javaee"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
- <servlet>
- <servlet-name>s</servlet-name>
- <servlet-class>com.cos.TestServlet</servlet-class>
- </servlet>
- <servlet-mapping>
- <servlet-name>s</servlet-name>
- <url-pattern>/login</url-pattern>
- </servlet-mapping>
- <filter>
- <filter-name>f</filter-name>
- <filter-class>com.cos.TestFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>f</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- <listener>
- <listener-class>com.cos.TestListener</listener-class>
- </listener>
- </web-app>
启动tomcat后，在控制台里以此打印出：
Listener 初始化。。。
Filter 初始化。。。
信息: Server startup in 675 ms
       可以看出：Listener > Filter > Servlet，Listener的初始化最早，Filter次之。他俩的初始化都是在容器启动完成之前初始化的。Servlet没有初始化，原因是没有匹配的请求进来。如果想要servlet自动初始化，需要在指定的servlet中配置<load-on-startup>参数，没有此标签，默认启动时servlet不进行初始化。
       初始化的顺序跟Listener、Filter、Servlet在web.xml中的顺序无关，而多个Filter或多个Servlet的时候，谁的mapping在前面，谁先初始化。
       如果web.xml中配置了<context-param>，初始化顺序：context-param > Listener > Filter > Servlet
