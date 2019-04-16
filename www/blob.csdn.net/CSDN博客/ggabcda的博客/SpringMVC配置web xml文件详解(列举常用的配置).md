# SpringMVC配置web.xml文件详解(列举常用的配置) - ggabcda的博客 - CSDN博客





2017年09月05日 21:13:48[ggabcda](https://me.csdn.net/ggabcda)阅读数：2131








**常用的web.xml的配置**

1、Spring 框架解决字符串编码问题：过滤器 **CharacterEncodingFilter**（filter-name） 
2、在web.xml配置监听器**ContextLoaderListener**（listener-class） 
ContextLoaderListener的作用就是启动Web容器时，自动装配ApplicationContext的配置信息。因为它实现了ServletContextListener这个接口，在web.xml配置这个监听器，启动容器时，就会默认执行它实现的方法。 
3、部署applicationContext的xml文件：**contextConfigLocation**（context-param下的param-name） 
4、DispatcherServlet是前置控制器，配置在web.xml文件中。拦截匹配的请求,Servlet拦截匹配规则要自已定义,把拦截下来的请求,依据某某规则分发到目标Controller来处理。 
**DispatcherServlet**（servlet-name、servlet-class、init-param、param-name(contextConfigLocation)、param-value） 
在DispatcherServlet的初始化过程中，框架会在web应用的 WEB-INF文件夹下寻找名为[servlet-name]-servlet.xml 的配置文件，生成文件中定义的bean

**(1) 配置filter过滤器**

```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <web-app version="2.5" 
 3     xmlns="http://java.sun.com/xml/ns/javaee" 
 4     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
 5     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
 6     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
 7 
 8 <!-- 解决Spring框架中从页面传来的字符串的中文乱码问题
 9    Spring框架给我们提供过滤器**CharacterEncodingFilter  **10    这个过滤器就是针对于每次浏览器请求进行过滤的，然后再其之上添加了父类没有的功能即处理字符编码。  
11    其中**encoding**用来设置编码格式，**forceEncoding**用来设置是否理会 request.getCharacterEncoding()方法，设置为**true**则强制覆盖之前的编码格式,根据需要设置。-->  
12     <filter>  
13         <filter-name>**characterEncodingFilter**</filter-name>
14         <filter-class>**org.springframework.web.filter.CharacterEncodingFilter**</filter-class>  
15         <init-param>  
16             <param-name>**encoding**</param-name>  **// 用来指定一个具体的字符集 **
17             <param-value>**UTF-8**</param-value>  
18         </init-param>  
19         <init-param>  
20             <param-name>**forceEncoding**</param-name>  **//true：无论request是否指定了字符集，都使用encoding；false：如果request已指定一个字符集，则不使用encoding **
21             <param-value>**true**</param-value>  
22         </init-param>  
23     </filter>  
24     <filter-mapping>  
25         <filter-name>**characterEncodingFilter**</filter-name>  
26         <url-pattern>**/***</url-pattern>  
27     </filter-mapping>
```

配置节点详解:

**<filter>** : 指定一个过滤器

**<filter-name>** : 为过滤器指定一个名字,该元素不能为空

**<filter-class>** : 指定过滤器的完整的限定类名

**<init-param>** : 为过滤器指定初始化参数

**<param-name>** : 指定参数的名字

**<param-value>** : 指定参数的值

**<filter-mapping>** : 用于设置一个Filter所负责拦截的资源

**<filter-name>** : 用于设置filter的注册名称,该值必须是在<filter>元素中声明过的过滤器名称

**<url-pattern>** : 设置filter所拦截的请求路径(过滤器关联的url样式)

**(2) Servlet配置**

```
1      <servlet> 
 2         <servlet-name>**DispatcherServlet**</servlet-name>  // 指定一个servlet名称
 3         <servlet-class>**org.springframework.web.servlet.DispatcherServlet**</servlet-class>  //指定servlet的类全路径
 4         <init-param>  
 5             <param-name>**contextConfigLocation**</param-name>  //初始化参数名
 6             <param-value>**classpath:spring/dispatcher-servlet.xml**</param-value>  //初始化参数值
 7         </init-param>  
 8         <load-on-startup>**1**</load-on-startup>  //指定当web容器启动时,加载servlet的顺序
 9     </servlet>  
10     <servlet-mapping>  
11         <servlet-name>**DispatcherServlet**</servlet-name>   //servlet名称       
12         <url-pattern>**/**</url-pattern>   //映射路径
13     </servlet-mapping>
```

配置节点详解:

1)使用SpringMVC配置DispatcherServlet是第一步,DispatcherServlet是一个Servlet,,所以可以配置多个DispatcherServlet

2)DispatcherServlet是前置控制器，配置在web.xml文件中。拦截匹配的请求，Servlet拦截匹配规则要自已定义，把拦截下来的请求，依据某某规则分发到目标Controller(我们写的Action)来处理。

3) **<servlet> : **在DispatcherServlet的初始化过程中，框架会在web应用的 WEB-INF文件夹下寻找名为[servlet-name]-servlet.xml 的配置文件，生成文件中定义的bean。

4)**<servlet-name>**:  servlet名称

5)****<servlet-class>****:  servlet类全路径

6)********<param-name>******** : 初始化参数名

7)********<param-value>********: 初始化参数值

8)********<load-on-startup>********: 指定当Web应用启动时，加载Servlet的顺序

9)********<url-pattern>********: 映射路径

**(3) 指定欢迎页面配置**



```
1 <welcome-file-list>
2     <welcome-file>**hello.jsp**</welcome-file>  //指定欢迎页面
3 </welcome-file-list>
```



**(4) 监听器配置**



```
1 <listener>
2       <listerner-class>**org.springframework.web.context.ContextLoaderListener**</listener-class>
3 </listener>
```



**(5) 会话超时配置**



```
<session-config>
      <session-timeout>**100**</session-timeout>
</session-config>
```



**(6) 配置错误页面**

1) 通过错误码来配置error-page



```
1 <!--配置了当系统发生404错误时，跳转到错误处理页面NotFound.jsp-->
2 <error-page>
3        <error-code>**404**</error-code>
4        <location>**/NotFound.jsp**</location>
5 </error-page>
```



2) 通过异常的类型来配置error-page

```
<!--配置了当系统发生java.lang.NullException（即空指针异常）时，跳转到错误处理页面error.jsp-->
<error-page>
       <exception-type>**java.lang.NullException**</exception-type>
       <location>**/error.jsp**</location>
</error-page>
```

最后配置一个简单的web.xml即可实现SpeingMVC框架

```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <web-app version="2.5" 
 3     xmlns="http://java.sun.com/xml/ns/javaee" 
 4     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
 5     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
 6     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
 7   <welcome-file-list>
 8     <welcome-file>**hello.jsp**</welcome-file>
 9   </welcome-file-list>
10   <servlet>
11       <servlet-name>**SpringMVC**</servlet-name>
12       <servlet-class>**org.springframework.web.servlet.DispatcherServlet**</servlet-class>
13       <load-on-startup>**1**</load-on-startup>
14   </servlet>
15   <servlet-mapping>
16       <servlet-name>**SpringMVC**</servlet-name>
17       <url-pattern>**/**</url-pattern>
18   </servlet-mapping>
19 </web-app>
```

有需要补充的还需各位阅读的大牛们给点建议!

如果您觉得阅读本文对您有帮助，请点一下“**推荐**”按钮，您的“**推荐**”将是我最大的写作动力！另外您也可以选择**【关注我】**，可以很方便找到我！

本文版权归作者和博客园共有，来源网址：https://www.cnblogs.com/欢迎各位转载，但是未经作者本人同意，转载文章之后必须在文章页面明显位置给出作者和原文连接，否则保留追究法律责任的权利！





