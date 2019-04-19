# Spring MVC入门（二） - 左直拳的马桶_日用桶 - CSDN博客
2017年09月14日 11:27:14[左直拳](https://me.csdn.net/leftfist)阅读数：311
在我昨天那个Spring MVC例子里，运行以后，页面呈现一个登录界面。 
![这里写图片描述](https://img-blog.csdn.net/20170914112416679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个登录界面，内容位于一个文件名叫“\WebContent\page\home.jsp”的视图里。
此刻，浏览器访问的地址是：`http://localhost:8080/SpringMVC/`，其实用这个地址`http://localhost:8080/SpringMVC/index.html` 访问也是一样的。
那么，系统是如何处理，在我们请求”/”或者”/index.html”的时候，最终将home.jsp的内容返回的呢？
首先，在web.xml里，指明了默认首页
```
<welcome-file-list>
      <welcome-file>index.html</welcome-file>
      <welcome-file>index.htm</welcome-file>
    </welcome-file-list>
```
然后，在控制器HomeController.java里有这么一个action，指明了请求index的，就用这个`public ModelAndView index()` 来处理。
```java
@RequestMapping("index")
    public ModelAndView index(){
        //创建模型跟视图，用于渲染页面。并且指定要返回的页面为home页面
        ModelAndView mav = new ModelAndView("home");
        return mav;
    }
```
这个action位于HomeController里面，系统是咋知道要到HomeController里找这个action呢？难道HomeController是系统默认的入口？不是的，主要是因为在web.xml里配置了一个Servlet：
```xml
<!-- 配置Spring MVC DispatcherServlet -->  
    <servlet>  
        <servlet-name>MVC</servlet-name>  
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
        <!-- 初始化参数 -->  
        <init-param>  
            <!-- 加载SpringMVC的xml到 spring的上下文容器中 --> 
            <!-- 这个"contextConfigLocation"名字似乎不能修改的，不奇怪，参数名 --> 
            <param-name>contextConfigLocation</param-name>  
            <param-value>  
                /WEB-INF/classes/mvc*.*  
            </param-value>  
        </init-param>  
        <load-on-startup>1</load-on-startup>  
    </servlet>
```
按照规则`/WEB-INF/classes/mvc*.*`，有mvc-context.xml与之对应，里面内容有：
```xml
<!-- SpringMVC配置 -->
    <!-- 通过component-scan 让Spring扫描org.swinglife.controller下的所有的类，让Spring的代码注解生效 -->
    <context:component-scan base-package="org.swinglife.controller"></context:component-scan>
    <!-- 配置SpringMVC的视图渲染器， 让其前缀为:/ 后缀为.jsp  将视图渲染到/page/<method返回值>.jsp中 -->
    <beans:bean
        class="org.springframework.web.servlet.view.InternalResourceViewResolver"
        p:prefix="/page/" p:suffix=".jsp">
        </beans:bean>
```
在示例里面，我们的控制器都在包“org.swinglife.controller”里，可见系统在初始化之处，就会扫描这个包里所有的代码，就知道映射”index”应该对应哪个action了。
假如我将”index”的映射改到其他action，那就会用其他action来处理：
```java
@Controller
public class OtherController {
    /***
     * 其他的跳转方法，通过String的返回值来跳转页面
     * @return
     */
    @RequestMapping("index")
    public String other(){
        return "other";
    }
}
```
![这里写图片描述](https://img-blog.csdn.net/20170914112404414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果映射重复，比如2个action都指向“index”，那就会报错。那如果有同名的映射怎么处理，我估计是路径会不一样。
附完整的web .xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://java.sun.com/xml/ns/javaee" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd" id="WebApp_ID" version="3.0">
    <display-name>SpringMVC</display-name>
<!--     监听spring上下文容器   -->
    <listener>  
        <listener-class>  
            org.springframework.web.context.ContextLoaderListener  
        </listener-class>  
    </listener>  
<!--     加载spring的xml配置文件到 spring的上下文容器中   -->
    <context-param>  
        <param-name>contextConfigLocation</param-name>  
        <param-value>classpath:root-context.xml</param-value>  
    </context-param>  
    <!-- 配置Spring MVC DispatcherServlet -->  
    <servlet>  
        <servlet-name>MVC</servlet-name>  
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
        <!-- 初始化参数 -->  
        <init-param>  
            <!-- 加载SpringMVC的xml到 spring的上下文容器中 -->  
            <param-name>contextConfigLocation</param-name>
            <param-value>
                /WEB-INF/classes/mvc*.*  
            </param-value>  
        </init-param>  
        <load-on-startup>1</load-on-startup>  
    </servlet>  
    <!-- 配置DispatcherServlet所需要拦截的 url -->  
    <servlet-mapping>  
        <servlet-name>MVC</servlet-name>  
        <url-pattern>*.html</url-pattern>  
        <url-pattern>*.htm</url-pattern>          
    </servlet-mapping>
    <welcome-file-list>
      <welcome-file>index.html</welcome-file>
      <welcome-file>index.htm</welcome-file>
    </welcome-file-list>
</web-app>
```
　　监听器Listener就是在application,session,request三个对象创建、销毁或者往其中添加修改删除属性时自动执行代码的功能组件。
　　Listener是Servlet的监听器，可以监听客户端的请求，服务端的操作等。 
参考文章： 
[Listener 监听器](http://www.cnblogs.com/hellojava/archive/2012/12/26/2833840.html)
