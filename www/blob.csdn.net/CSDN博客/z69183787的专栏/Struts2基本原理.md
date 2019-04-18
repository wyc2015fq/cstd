# Struts2基本原理 - z69183787的专栏 - CSDN博客
2014年08月21日 15:06:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：710
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
## Struts2请求响应流程：
在struts2的应用中，从用户请求到服务器返回相应响应给用户端的过程中，包含了许多组件如：Controller、ActionProxy、ActionMapping、Configuration Manager、ActionInvocation、Inerceptor、Action、Result等。下面我们来具体看看这些组件有什么联系，它们之间是怎样在一起工作的。
![](https://img-blog.csdn.net/20130904161742156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V3ZW54aWFuZzkxMzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（1）  客户端（Client）向Action发用一个请求（Request）
（2）  Container通过web.xml映射请求，并获得控制器（Controller）的名字
（3）  容器（Container）调用控制器（StrutsPrepareAndExecuteFilter或FilterDispatcher）。在Struts2.1以前调用FilterDispatcher，Struts2.1以后调用StrutsPrepareAndExecuteFilter
（4）  控制器（Controller）通过ActionMapper获得Action的信息
（5）  控制器（Controller）调用ActionProxy
（6）  ActionProxy读取struts.xml文件获取action和interceptor stack的信息。
（7）  ActionProxy把request请求传递给ActionInvocation
（8）  ActionInvocation依次调用action和interceptor
（9）  根据action的配置信息，产生result
（10） Result信息返回给ActionInvocation
（11） 产生一个HttpServletResponse响应
（12） 产生的响应行为发送给客服端。
阐述struts2的执行流程。 
  Struts 2框架本身大致可以分为3个部分：核心控制器FilterDispatcher、业务控制器Action和用户实现的企业业务逻辑组件。
  核心控制器FilterDispatcher是Struts 2框架的基础，包含了框架内部的控制流程和处理机制。业务控制器Action和业务逻辑组件是需要用户来自己实现的。用户在开发Action和业务逻辑组件的同时，还需要编写相关的配置文件，供核心控制器FilterDispatcher来使用。 Struts 2的工作流程相对于Struts 1要简单，与WebWork框架基本相同，所以说Struts 2是WebWork的升级版本。
  基本简要流程如下：
      1、客户端浏览器发出HTTP请求。
      2、根据web.xml配置，该请求被FilterDispatcher接收。
      3、根据struts.xml配置，找到需要调用的Action类和方法， 并通过IoC方式，将值注入给Aciton。
      4、Action调用业务逻辑组件处理业务逻辑，这一步包含表单验证。
      5、Action执行完毕，根据struts.xml中的配置找到对应的返回结果result，并跳转到相应页面。
      6、返回HTTP响应到客户端浏览器。
工作原理
  在Struts2框架中的处理大概分为以下几个步骤 
  1 客户端初始化一个指向Servlet容器（例如Tomcat）的请求 
  2 这个请求经过一系列的过滤器（Filter）（这些过滤器中有一个叫做ActionContextCleanUp的可选过滤器，这个过滤器对于Struts2和其他框架的集成很有帮助，例如：SiteMesh Plugin） 
  3 接着FilterDispatcher被调用，FilterDispatcher询问ActionMapper来决定这个请是否需要调用某个Action 
  4 如果ActionMapper决定需要调用某个Action，FilterDispatcher把请求的处理交给ActionProxy 
  5 ActionProxy通过Configuration Manager询问框架的配置文件，找到需要调用的Action类 
  6 ActionProxy创建一个ActionInvocation的实例。 
  7 ActionInvocation实例使用命名模式来调用，在调用Action的过程前后，涉及到相关拦截器（Intercepter）的调用。 
  8 一旦Action执行完毕，ActionInvocation负责根据struts.xml中的配置找到对应的返回结果。返回结果通常是（但不总是，也可 能是另外的一个Action链）一个需要被表示的JSP或者FreeMarker的模版。在表示的过程中可以使用Struts2 框架中继承的标签。在这个过程中需要涉及到ActionMapper 
工作流程
  1、客户端浏览器发出HTTP请求.
  2、根据web.xml配置，该请求被FilterDispatcher接收
  3、根据struts.xml配置，找到需要调用的Action类和方法， 并通过IoC方式，将值注入给Aciton
  4、Action调用业务逻辑组件处理业务逻辑，这一步包含表单验证。
  5、Action执行完毕，根据struts.xml中的配置找到对应的返回结果result，并跳转到相应页面
  6、返回HTTP响应到客户端浏览器
看到网友的对Struts2的原理总结，我自己也总结以便后续的面试，以下是我的疑问
  1、客服端发起一个请求，通过HTTP协议指向Tomcat容器，tomcat拿到请求她干了什么？
  2、我们web.xml配置
  <filter>
        <filter-name>struts2</filter-name>
        <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
 </filter>
 <filter-mapping>
        <filter-name>struts2</filter-name>
        <url-pattern>/*</url-pattern>
</filter-mapping>
我们从web配置文件中可以看到org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter，这个Filter这个类有什么用？
这个 /* 是拦截所有的请求，他拦截了请求做了什么处理？ <url-pattern>/*</url-pattern>
 3.我们struts.xml配置
<struts>
 <package name="default" namespace="/" extends="struts-default">
        <action name="hello">
            <result>
                /Hello.jsp
            </result>
        </action>
    </package>
</struts>
谁负责根据struts.xml配置，找到需要调用的Action类和方法呢？
 4。最后根据struts.xml中的配置找到对应的返回结果result，在返回HTTP响应到客户端浏览器
其实用最简单的七个步骤：
  1：客户端在浏览器中输入一个url地址。
  2：这个url请求通过http协议发送给tomcat。
  3：tomcat根据url找到对应项目里面的web.xml文件。
  4：在web.xml里面会发现有struts2的配置。
  5：然后会找到struts2对应的struts.xml配置文件。
  6：根据url解析struts.xml配置文件就会找到对应的class。
  7：调用完class返回一个字String，根据struts.xml返回到对应的jsp。
struts2标签分类：
  1.表单UI标签
  2.非表单UI标签
  3.控制标签
  4.数据标签
为什么使用struts2？
  1.开源
  2.mvc框架
  3.纯pojo的Action
  4.更好的标签特性
  5.易测试
  6.易扩展
