# Web容器中DefaultServlet详解 - z69183787的专栏 - CSDN博客
2019年01月22日 10:13:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：57
个人分类：[Servlet](https://blog.csdn.net/z69183787/article/category/2175855)
[https://blog.csdn.net/qq_30920821/article/details/78328608](https://blog.csdn.net/qq_30920821/article/details/78328608)
Web容器中DefaultServlet详解
一、什么是defaultServlet
  我们以最熟悉的Tomcat服务器为例。我们都知道Jsp和servlet都需要web容器才能运行。但是实际上呢我们的web应用中可以没有任何servlet或者jsp（至少表面上是这样的）只需要一个web.xml，设置在servlet 3.0中，这个也可以省略。但是我们同样可以通过链接来访问应用内的资源文件。例如.jpg,.html,.js这类的静态文件。这就是DefaultServlet的作用了。它在tomat的安装目录下的conf/web.xml中有定义。该web.xml对于所有tomcat加载的的web application都会应用，会和application本身指定的web.xml进行合并。 
<!--在conf/web.xml中的DefaultServlet的定义-->
<servlet>
        <servlet-name>default</servlet-name>
        <servlet-class>org.apache.catalina.servlets.DefaultServlet</servlet-class>
        <init-param>
            <param-name>debug</param-name>
            <param-value>0</param-value>
        </init-param>
        <init-param>
            <param-name>listings</param-name>
            <param-value>false</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
<!--defaultServletMapping-->
 <!-- The mapping for the default servlet -->
    <servlet-mapping>
        <servlet-name>default</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>
配置分析: 
1. 非常熟悉的配置对不对，这就是一个非常常见的servlet配置。该servlet的name为default,其对应的servlet-class类为org.apache.catalina.servlets.DefaultServlet。 
2. 对应的init-param标签中两个初始化参数，一个是debug,这个参数设置了debug的级别。一个是listings参数，该参数的作用是当对应URL并没有对应一个资源时，是否要显示资源目录，该参数配置为了false，也就是不显示。 
3. load-on-startup为1是说明当应用启动时就在加载该servlet，默认的情况下是用户第一次访问该servlet方法时才会实例化并加载servlet。 
4. DefaultServlet的servlet-mapping配置的为/,很多帖子或者书籍上都说/的URL映射符会映射除了*.xxx类带后缀的地址URL，事实上/映射可以处理所有的请求，一般只有defaultServlet会配置为/,如果自定义的Servlet也配置为/,那么将会覆盖defaultservlet的配置。
二、JspServlet的作用
  同理，在Tomcat Server中运行的web application，对于类似于.jsp的java动态服务页同样不需要我们自己处理。我们可以直接通过一个URL例如/index.jsp来访问到该页面，这是因为在conf/web.xml中有一个JspServlet来处理这类请求。
 <!--在conf/web.xml中有定义-->
 <servlet>
    <servlet-name>jsp</servlet-name>
    <servlet-class>org.apache.jasper.servlet.JspServlet</servlet-class>
    <init-param>
        <param-name>fork</param-name>
        <param-value>false</param-value>
    </init-param>
    <init-param>
        <param-name>xpoweredBy</param-name>
        <param-value>false</param-value>
    </init-param>
    <load-on-startup>3</load-on-startup>
</servlet>
...............
<!-- The mappings for the JSP servlet -->
<servlet-mapping>
    <servlet-name>jsp</servlet-name>
    <url-pattern>*.jsp</url-pattern>
    <url-pattern>*.jspx</url-pattern>
</servlet-mapping>
配置分析 
1. 文档解释： The JSP page compiler and execution servlet, which is the mechanism ，used by Tomcat to support JSP pages. Traditionally, this servlet ，is mapped to the URL pattern “*.jsp”. This servlet supports the，following initialization parameters (default values are in square brackets）。 
2. 直接翻译：JSP页的编译和执行Servlet, 是Tomcat支持JSP页面的机制。另外，这个servlet被映射的URL模式为”*.jsp”。 同时也支持一些初始化参数。它们都具有一些默认值。具体解释可以看conf/web.xml中的注释。
三、DefaultServlet在实际开发中的应用。
JSP/Servlet架构的web项目
   这类架构的项目无需显示配置DefaultSerlvet，一般来说，jsp后缀请求会交给JspServlet,静态资源请求图片等会交给DefaultServlet处理。
SSM中DispatcherServlet的URL配置 
”*.do,*.action等配置”，当servlet-mapping配置为*.do这类带指定后缀的时候，静态资源文件依旧可以交给DefaultServlet处理。(*.do的DispatcherServlet不会拦截类似*.jsp,*.jpg的请求)
<!-- Spring MVC -->
    <servlet>
        <servlet-name>SpringMVC</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <description>SpringMVC</description>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:spring/spring-mvc.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>SpringMVC</servlet-name>
        <url-pattern>*.do</url-pattern>
    </servlet-mapping>
        <servlet-mapping>
        <servlet-name>SpringMVC</servlet-name>
        <url-pattern>*.action</url-pattern>
    </servlet-mapping>
mapping为“/”的配置,当Dispatcher Servlet的mapping配置为/的时候，这时候会覆盖DefaultServlet的配置(DispathcerServlet会更先匹配到)，那么对于.jpg,.png类似这种静态资源就得不到处理（之前是defaultServlet）会处理，所以这类资源就会得到404错误。所以我们需要有额外的配置来解决这个问题。
继续通过DefaultServlet来处理。我们在应用的web.xml中将常用的静态资源URL映射到defaultServlet上。
 <!--通过defaultServlet来处理jpg-->
    <servlet-mapping>
        <servlet-name>default</servlet-name>
        <url-pattern>*.jpg</url-pattern>
    </servlet-mapping>
    <!--通过DefaultServlet来处理png-->
    <servlet-mapping>
        <servlet-name>default</servlet-name>
        <url-pattern>*.png</url-pattern>
    </servlet-mapping>
    <!--通过defaultServlet来处理.js文件-->
    <servlet-mapping>
        <servlet-name>default</servlet-name>
        <url-pattern>*.js</url-pattern>
    </servlet-mapping>
    <!--通过defaultServlet来处理.css文件-->
    <servlet-mapping>
        <servlet-name>default</servlet-name>
        <url-pattern>*.css</url-pattern>
    </servlet-mapping>
通过Spring的<mvc:resource>来配置资源映射，将静态资源的映射进行放行。
<!-- 放行静态资源 -->
<!--放行js文件-->
<mvc:resources location="/js/" mapping="/js/**"/> <!--放行css文件-->
<mvc:resources location="/css/" mapping="/css/**"/>
<!--放行img类的资源文件-->
 <mvc:resources location="/images/" mapping="/images/**"/>
通过Spring MVC的<mvc:default-handler/>：通过这个标签可以启动Spring 
MVC定义的一个类似于Tomcat的DefaultServlet的DefaultServletHttpRequestHandler类。
<mvc:default-servlet-handler />
