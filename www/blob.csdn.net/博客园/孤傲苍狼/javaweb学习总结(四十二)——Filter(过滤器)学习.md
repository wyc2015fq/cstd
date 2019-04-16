# javaweb学习总结(四十二)——Filter(过滤器)学习 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [javaweb学习总结(四十二)——Filter(过滤器)学习](https://www.cnblogs.com/xdp-gacl/p/3948353.html)



## 一、Filter简介

　　Filter也称之为过滤器，它是Servlet技术中最激动人心的技术，WEB开发人员通过Filter技术，对web服务器管理的所有web资源：例如Jsp, Servlet, 静态图片文件或静态 html 文件等进行拦截，从而实现一些特殊的功能。例如实现URL级别的权限访问控制、过滤敏感词汇、压缩响应信息等一些高级功能。
　　Servlet API中提供了一个Filter接口，开发web应用时，如果编写的Java类实现了这个接口，则把这个java类称之为过滤器Filter。通过Filter技术，开发人员可以实现用户在访问某个目标资源之前，对访问的请求和响应进行拦截，如下所示：

![](data:image/png;base64,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JKR6jRpDoqJOnDhhR3cTgGVyFFgmM1B18sAyHZLi4uILFy6MjY52dXf3q1ev57BhcZtTN16+ZN3ypF6/Jt+5o/+4cQ2Cg6vXqBExeHBWVtazZ8+of3vg4BQWFp48eXL0mDHuHh5efn5dBr86Yc3q1Sc/tMUrZPLFC9O2bO49amS9hg2r16jRf+DA/fv3P3r0iPo5MBlYJkeBZTIDVScPLNOxKSoqunr16rx581pIpZWrVGnQtGmPqMhRiYlz3tqdfPGCqW1JuXpl/r53YlYs7zNmdJNWrapUrdogKGjK1KkffvjhkydPqH9XUOEoLi5WKBTLly/v2Llz1WrVvGvX7tC3j2x6fGzKpqXZWWm5OaY2PF2Rt/L48ekZ6cPemBMWMahuUNALlSs3b9lSPmvW2bNnCe8PaTmwTI4Cy2QGqk4eWGbF4eHDh6dPn16yZEm/AQMCAgMrVark4e3dSCpt+/LL3SKHDpgwYfC0aZGz5CMWzB+5aGHU7NmD4+MHTpr48vBh7fv0Dm4T4lunTqVKlfz8/V/u1WvWrFnZ2dm//vor9e8EQCnPnz9XKBRpaWmjRo9u0759LWfnKlWr+jdo0Lx9+47h4S8PHzZw0sTB8fFRs2eVNPy1OXNkM6YPmjKl96hRnQf0b9mpU/3Gjas7OVV78cVmLVoMiYxcs2bNhQsX/v77b+rfzDrAMjkKLJMZqDp5YJkVlidPnty8efPYsWM7d+5cvnz5tPj4mPHjh48c+apM9kpExGvDh0ePGxcbG7t48eKtW7ceOXIkLy/v4cOH1D81AEahVCrv379/9erV9957Lz09PTExMTY2Nmb8+GEjRkQMHlzS8LHR0VOmTp03b15KSsqBAwfOnz9/9+5dOzrV0iRgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweGCZHAWWyQxUnTywTAAAcHhgmRwFlskMVJ08sEwAAHB4YJkcBZbJDFSdPLBMAABweMpf5X97/FgCOICwDRUH6iGDUqiLAAAAwIbgVR4AAAAAAFgfWCYAAAAAALA+sEwAAAAAAGB9/h95jxaqbfZR0wAAAABJRU5ErkJggg==)

## 二、Filter是如何实现拦截的？

　　Filter接口中有一个doFilter方法，当我们编写好Filter，并配置对哪个web资源进行拦截后，WEB服务器每次在调用web资源的service方法之前，都会先调用一下filter的doFilter方法，因此，在该方法内编写代码可达到如下目的：
- 调用目标资源之前，让一段代码执行。
- 是否调用目标资源（即是否让用户访问web资源）。
- 调用目标资源之后，让一段代码执行。

　　web服务器在调用doFilter方法时，会传递一个filterChain对象进来，filterChain对象是filter接口中最重要的一个对 象，它也提供了一个doFilter方法，开发人员可以根据需求决定是否调用此方法，调用该方法，则web服务器就会调用web资源的service方 法，即web资源就会被访问，否则web资源不会被访问。

## 三、Filter开发入门

### 3.1、Filter开发步骤

　　Filter开发分为二个步骤：
- 编写java类实现Filter接口，并实现其doFilter方法。
- 在 web.xml 文件中使用<filter>和<filter-mapping>元素对编写的filter类进行注册，并设置它所能拦截的资源。

**过滤器范例：**

```
1 package me.gacl.web.filter;
 2 
 3 import java.io.IOException;
 4 
 5 import javax.servlet.Filter;
 6 import javax.servlet.FilterChain;
 7 import javax.servlet.FilterConfig;
 8 import javax.servlet.ServletException;
 9 import javax.servlet.ServletRequest;
10 import javax.servlet.ServletResponse;
11 
12 /**
13 * @ClassName: FilterDemo01
14 * @Description:filter的三种典型应用：
15 *                     1、可以在filter中根据条件决定是否调用chain.doFilter(request, response)方法，
16 *                        即是否让目标资源执行
17 *                     2、在让目标资源执行之前，可以对request\response作预处理，再让目标资源执行
18 *                     3、在目标资源执行之后，可以捕获目标资源的执行结果，从而实现一些特殊的功能
19 * @author: 孤傲苍狼
20 * @date: 2014-8-31 下午10:09:24
21 */ 
22 public class FilterDemo01 implements Filter {
23 
24     @Override
25     public void init(FilterConfig filterConfig) throws ServletException {
26         System.out.println("----过滤器初始化----");
27     }
28 
29     @Override
30     public void doFilter(ServletRequest request, ServletResponse response,
31             FilterChain chain) throws IOException, ServletException {
32         
33         //对request和response进行一些预处理
34         request.setCharacterEncoding("UTF-8");
35         response.setCharacterEncoding("UTF-8");
36         response.setContentType("text/html;charset=UTF-8");
37         
38         System.out.println("FilterDemo01执行前！！！");
39         chain.doFilter(request, response);  //让目标资源执行，放行
40         System.out.println("FilterDemo01执行后！！！");
41     }
42 
43     @Override
44     public void destroy() {
45         System.out.println("----过滤器销毁----");
46     }
47 }
```

　　在web. xml中配置过滤器：

```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <web-app version="3.0" 
 3     xmlns="http://java.sun.com/xml/ns/javaee" 
 4     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
 5     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
 6     http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
 7   <display-name></display-name>    
 8   <welcome-file-list>
 9     <welcome-file>index.jsp</welcome-file>
10   </welcome-file-list>
11   
12   <!--配置过滤器-->
13   <filter>
14       <filter-name>FilterDemo01</filter-name>
15       <filter-class>me.gacl.web.filter.FilterDemo01</filter-class>
16   </filter>
17   
18   <!--映射过滤器-->
19   <filter-mapping>
20       <filter-name>FilterDemo01</filter-name>
21       <!--“/*”表示拦截所有的请求 -->
22       <url-pattern>/*</url-pattern>
23   </filter-mapping>
24   
25 </web-app>
```

### 3.2、Filter链

　　在一个web应用中，可以开发编写多个Filter，这些Filter组合起来称之为一个Filter链。
　　web服务器根据Filter在web.xml文件中的注册顺序，决定先调用哪个Filter，当第一个Filter的doFilter方法被调用时，web服务器会创建一个代表Filter链的FilterChain对象传递给该方法。在doFilter方法中，开发人员如果调用了FilterChain对象的doFilter方法，则web服务器会检查FilterChain对象中是否还有filter，如果有，则调用第2个filter，如果没有，则调用目标资源。

## 四、Filter的生命周期

### 4.1、Filter的创建

　　Filter的创建和销毁由**WEB服务器负责**。 web 应用程序**启动时**，web 服务器将**创建Filter 的实例对象**，并调用其init方法，完成对象的初始化功能，从而为后续的用户请求作好拦截的准备工作**，****filter对象只会创建一次，init方法也只会执行一次**。通过init方法的参数，可获得代表当前filter配置信息的FilterConfig对象。

### 4.2、Filter的销毁

　　Web容器调用destroy方法销毁Filter。destroy方法在Filter的生命周期中仅执行一次。在destroy方法中，可以释放过滤器使用的资源。

### 4.3、FilterConfig接口

　　用户在配置filter时，可以使用<init-param>为filter配置一些初始化参数，当web容器实例化Filter对象，调用其init方法时，会把封装了filter初始化参数的filterConfig对象传递进来。因此开发人员在编写filter时，通过filterConfig对象的方法，就可获得：
　　String getFilterName()：得到filter的名称。
　　String getInitParameter(String name)： 返回在部署描述中指定名称的初始化参数的值。如果不存在返回null.
　　Enumeration getInitParameterNames()：返回过滤器的所有初始化参数的名字的枚举集合。
　　public ServletContext getServletContext()：返回Servlet上下文对象的引用。
**范例：利用FilterConfig得到filter配置信息**

```
1 package me.gacl.web.filter;
 2 
 3 import java.io.IOException;
 4 import java.util.Enumeration;
 5 import javax.servlet.Filter;
 6 import javax.servlet.FilterChain;
 7 import javax.servlet.FilterConfig;
 8 import javax.servlet.ServletException;
 9 import javax.servlet.ServletRequest;
10 import javax.servlet.ServletResponse;
11 
12 public class FilterDemo02 implements Filter {
13 
14     /* 过滤器初始化
15      * @see javax.servlet.Filter#init(javax.servlet.FilterConfig)
16      */
17     @Override
18     public void init(FilterConfig filterConfig) throws ServletException {
19         System.out.println("----过滤器初始化----");
20         /**
21          *  <filter>
22                   <filter-name>FilterDemo02</filter-name>
23                   <filter-class>me.gacl.web.filter.FilterDemo02</filter-class>
24                   <!--配置FilterDemo02过滤器的初始化参数-->
25                   <init-param>
26                       <description>配置FilterDemo02过滤器的初始化参数</description>
27                       <param-name>name</param-name>
28                       <param-value>gacl</param-value>
29                   </init-param>
30                   <init-param>
31                       <description>配置FilterDemo02过滤器的初始化参数</description>
32                       <param-name>like</param-name>
33                       <param-value>java</param-value>
34                   </init-param>
35             </filter>
36             
37              <filter-mapping>
38                   <filter-name>FilterDemo02</filter-name>
39                   <!--“/*”表示拦截所有的请求 -->
40                   <url-pattern>/*</url-pattern>
41              </filter-mapping>
42          */
43         //得到过滤器的名字
44         String filterName = filterConfig.getFilterName();
45         //得到在web.xml文件中配置的初始化参数
46         String initParam1 = filterConfig.getInitParameter("name");
47         String initParam2 = filterConfig.getInitParameter("like");
48         //返回过滤器的所有初始化参数的名字的枚举集合。
49         Enumeration<String> initParameterNames = filterConfig.getInitParameterNames();
50         
51         System.out.println(filterName);
52         System.out.println(initParam1);
53         System.out.println(initParam2);
54         while (initParameterNames.hasMoreElements()) {
55             String paramName = (String) initParameterNames.nextElement();
56             System.out.println(paramName);
57         }
58     }
59 
60     @Override
61     public void doFilter(ServletRequest request, ServletResponse response,
62             FilterChain chain) throws IOException, ServletException {
63         System.out.println("FilterDemo02执行前！！！");
64         chain.doFilter(request, response);  //让目标资源执行，放行
65         System.out.println("FilterDemo02执行后！！！");
66     }
67 
68     @Override
69     public void destroy() {
70         System.out.println("----过滤器销毁----");
71     }
72 }
```

## 五、Filter的部署

　　Filter的部署分为两个步骤：

　　1、注册Filter

　　2、映射Filter

### 5.1、注册Filter

　　开发好Filter之后，需要在web.xml文件中进行注册，这样才能够被web服务器调用

　　在web.xml文件中注册Filter范例：

```
1 <filter>
 2           <description>FilterDemo02过滤器</description>
 3           <filter-name>FilterDemo02</filter-name>
 4           <filter-class>me.gacl.web.filter.FilterDemo02</filter-class>
 5           <!--配置FilterDemo02过滤器的初始化参数-->
 6           <init-param>
 7               <description>配置FilterDemo02过滤器的初始化参数</description>
 8               <param-name>name</param-name>
 9               <param-value>gacl</param-value>
10           </init-param>
11           <init-param>
12               <description>配置FilterDemo02过滤器的初始化参数</description>
13               <param-name>like</param-name>
14               <param-value>java</param-value>
15           </init-param>
16 </filter>
```

　　<description>用于添加描述信息，该元素的内容可为空，<description>可以不配置。

　　<filter-name>用于为过滤器指定一个名字，该元素的内容不能为空。
　　<filter-class>元素用于指定过滤器的完整的限定类名。
　　<init-param>元素用于为过滤器指定初始化参数，它的子元素<param-name>指定参数的名字，<param-value>指定参数的值。在过滤器中，可以使用FilterConfig接口对象来访问初始化参数。如果过滤器不需要指定初始化参数，那么<init-param>元素可以不配置。

### 5.2、映射Filter

　　在web.xml文件中注册了Filter之后，还要在web.xml文件中映射Filter

```
1  <!--映射过滤器-->
2   <filter-mapping>
3       <filter-name>FilterDemo02</filter-name>
4       <!--“/*”表示拦截所有的请求 -->
5       <url-pattern>/*</url-pattern>
6   </filter-mapping>
```

　　<filter-mapping>元素用于设置一个 Filter 所负责拦截的资源。一个Filter拦截的资源可通过两种方式来指定：Servlet 名称和资源访问的请求路径
　　<filter-name>子元素用于设置filter的注册名称。该值必须是在<filter>元素中声明过的过滤器的名字
　　<url-pattern>设置 filter 所拦截的请求路径(过滤器关联的URL样式)
　　<servlet-name>指定过滤器所拦截的Servlet名称。
　　<dispatcher>指定过滤器所拦截的资源被 Servlet 容器调用的方式，可以是REQUEST,INCLUDE,FORWARD和ERROR之一，默认REQUEST。用户可以设置多个<dispatcher> 子元素用来指定 Filter 对资源的多种调用方式进行拦截。如下：

```
1 <filter-mapping>
2     <filter-name>testFilter</filter-name>
3    <url-pattern>/index.jsp</url-pattern>
4    <dispatcher>REQUEST</dispatcher>
5    <dispatcher>FORWARD</dispatcher>
6 </filter-mapping>
```

　　<dispatcher> 子元素可以设置的值及其意义：
- REQUEST：当用户直接访问页面时，Web容器将会调用过滤器。如果目标资源是通过RequestDispatcher的include()或forward()方法访问时，那么该过滤器就不会被调用。
- INCLUDE：如果目标资源是通过RequestDispatcher的include()方法访问时，那么该过滤器将被调用。除此之外，该过滤器不会被调用。
- FORWARD：如果目标资源是通过RequestDispatcher的forward()方法访问时，那么该过滤器将被调用，除此之外，该过滤器不会被调用。
- ERROR：如果目标资源是通过声明式异常处理机制调用时，那么该过滤器将被调用。除此之外，过滤器不会被调用。









