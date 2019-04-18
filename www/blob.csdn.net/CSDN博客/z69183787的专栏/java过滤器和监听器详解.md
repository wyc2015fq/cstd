# java过滤器和监听器详解 - z69183787的专栏 - CSDN博客
2014年03月24日 07:11:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1297
** 过滤器**
**1、Filter工作原理（执行流程）**
当客户端发出Web资源的请求时，Web服务器根据应用程序配置文件设置的过滤规则进行检查，若客户请求满足过滤规则，则对客户请求／响应进行拦截，对请求头和请求数据进行检查或改动，并依次通过过滤器链，最后把请求／响应交给请求的Web资源处理。请求信息在过滤器链中可以被修改，也可以根据条件让请求不发往资源处理器，并直接向客户机发回一个响应。当资源处理器完成了对资源的处理后，响应信息将逐级逆向返回。同样在这个过程中，用户可以修改响应信息，从而完成一定的任务。
上面说了，当一个请求符合某个过滤器的过滤条件时该请求就会交给这个过滤器去处理。那么当两个过滤器同时过滤一个请求时谁先谁后呢？这就涉及到了过滤链FilterChain。
所有的奥秘都在Filter的FilterChain中。服务器会按照web.xml中过滤器定义的先后循序组装成一条链，然后一次执行其中的doFilter()方法。执行的顺序就如下图所示，执行第一个过滤器的chain.doFilter()之前的代码，第二个过滤器的chain.doFilter()之前的代码，请求的资源，第二个过滤器的chain.doFilter()之后的代码，第一个过滤器的chain.doFilter()之后的代码，最后返回响应。
![](https://img-my.csdn.net/uploads/201204/03/1333424123_7382.jpg)
这里还有一点想补充：大家有没有想过，上面说的“执行请求的资源”究竟是怎么执行的？对于“执行第一个过滤器的chain.doFilter()之前的代码，第二个过滤器的chain.doFilter()之前的代码”这些我可以理解，无非就是按顺序执行一句句的代码，但对于这个“执行请求的资源”我刚开始却是怎么也想不明白。其实是这样的：
通常我们所访问的资源是一个servlet或jsp页面，而jsp其实是一个被封装了的servlet，于是我们就可以统一地认为我们每次访问的都是一个Servlet,而每当我们访问一个servlet时，web容器都会调用该Servlet的service方法去处理请求。而在service方法又会根据请求方式的不同（Get/Post）去调用相应的doGet()或doPost()方法，实际处理请求的就是这个doGet或doPost方法。写过servlet的朋友都应该知道，我们在doGet（或doPost）方法中是通过response.getWriter（）得到客户端的输出流对象，然后用此对象对客户进行响应。
到这里我们就应该理解了过滤器的执行流程了：执行第一个过滤器的chain.doFilter()之前的代码——>第二个过滤器的chain.doFilter()之前的代码——>……——>第n个过滤器的chain.doFilter()之前的代码——>所请求servlet的service()方法中的代码——>所请求servlet的doGet()或doPost()方法中的代码——>第n个过滤器的chain.doFilter()之后的代码——>……——>第二个过滤器的chain.doFilter()之后的代码——>第一个过滤器的chain.doFilter()之后的代码。
**过滤器生命周期的四个阶段：**
1、实例化：Web容器在部署Web应用程序时对所有过滤器进行实例化。Web容器回调它的无参构造方法。2、初始化：实例化完成之后，马上进行初始化工作。Web容器回调init()方法。
3、过滤：请求路径匹配过滤器的URL映射时。Web容器回调doFilter()方法——主要的工作方法。
4、销毁： Web容器在卸载Web应用程序前，Web容器回调destroy()方法。
**Servlet过滤器开发步骤：**
1、创建实现javax.servlet.Filter接口的类。
2、过滤器的xml配置。
**Servlet过滤器API**
 Servlet过滤器API包含了3个接口，它们都在javax.servlet包中，分别是Filter接口、FilterChain接口和FilterConfig接口。
public Interface Filter
所有的过滤器都必须实现Filter接口。该接口定义了init,doFilter0，destory()三个方法：
  (1) public void init (FilterConfig filterConfig) 
当开始使用servlet过滤器服务时，Web容器调用此方法一次，为服务准备过滤器；然后在需要使用过滤器的时候调用doFilter()，传送给此方法的FilterConfig对象，包含servlet过滤器的初始化参数。
  (2)public void doFilter(ServletRequest request，ServletResponse response，FilterChain chain)    
每个过滤器都接受当前的请求和响应，且FilterChain过滤器链中的过滤器（应该都是符合条件的）都会被执行。doFilter方 法中，过滤器可以对请求和响应做它想做的一切，通过调用他们的方法收集数据，或者给对象添加新的行为。过滤器通过传送至 此方法的FilterChain参数，调用chain．doFilterO将控制权传送给下一个过滤器。当这个调用返回后，过滤器可以在它的 Filter方法的最后对响应做些其他的工作。如果过滤器想要终止请求的处理或得到对响应的完全控制，则可以不调用下一个过滤 器，而将其重定向至其它一些页面。当链中的最后一个过滤器调用chain．doFilterO方法时，将运行最初请求的Servlet。
 (3)public void destroy()
一旦doFilterO方法里的所有线程退出或已超时，容器调用
此方法。服务器调用destoryO以指出过滤器已结束服务，用于释
放过滤器占用的资源。
public interface FilterChain
public void doFilter(ServletRequest request，ServletResponse response)
此方法是由Servlet容器提供给开发者的，用于对资源请求过滤链的依次调用，通过FilterChain调用过滤链中的下一个过滤器，如果是最后一个过滤器，则下一个就调用目标资源。
public interface FilterConfig
 FilterConfig接口检索过滤器名、初始化参数以及活动的Servlet上下文。该接口提供了以下4个方法：
     (1)public java．1ang．String getFilterName0
返回web．xml部署文件中定义的该过滤器的名称。
     (2)public ServletContext getServletContextO
返回调用者所处的servlet上下文。
     (3)public java.1ang.String getlnitParameter(java.1ang.String name)
返回过滤器初始化参数值的字符串形式，当参数不存在时，返回nul1．name是初始化参数名。
     (4)public java.util.Enumeration getlnitParameterNames()
以Enumeration形式返回过滤器所有初始化参数值，如果没有初始化参数，返回为空。
**三、应用实例**
从上面分析可知，实现Servlet过滤器，需要两步：第一步开发过滤器，设计—个实现Fiker接口的类；第二步通过web.xml配置过滤器，实现过滤器和Servlet、JSP页面之间的映射。以下设计一个简单的IP地址过滤器，根据用户的IP地址进行对网站的访问控制。
(1)过滤器的设计ipfilter.java
**[java]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7423215#)[copy](http://blog.csdn.net/csh624366188/article/details/7423215#)[print](http://blog.csdn.net/csh624366188/article/details/7423215#)[?](http://blog.csdn.net/csh624366188/article/details/7423215#)
- package ipf；  
- imp0rt java．io．IOException；  
- imp0rt javax．servlet．*；  
- publicclass ipfilter implements Filter／／实现Filter接口  
- {protected FilterConfig config；  
- protected String rejectedlP；  
- publicvoid init(FilterConfig filterConfig)throws
- ServletException  
- 
- {this．config=filterConfig；／／从Web务器获取过滤器配置对象  
- rejectedlP=config．getlnitParameter( RejectedlP”)：  
- //从配置中取得过滤lP
- }  
- publicvoid doFilter(ServletRequest request，  
- ServletResponse response．FilterChain chain)throws
- IOException，ServletException  
- {RequestDispatcher dispatcher=request.getRequestDispatcher("");  
- String remotelP=request．getRemoteAddrO；//获取客户请求lP
- int i=remotelP．1astlndexOf("．")；  
- int r=rejectedlP．1astlndexOf(”．”)：  
- String relPscope=rejectedlP．substring(0，r);／／过滤lP段  
- if(relPscope．equals(remotelP．substring(O．i)))  
- {      dispatcher．forward(request，response)；//重定向到rejectedError．jsp页面
-         retum；／／阻塞，直接返Web回客户端  
- }  
- else{chain．doFilter(request，response)；//调用过滤链上的下一个过滤器
- }  
- }  
- publicvoid destroy()  
 //过滤器功能完成后，由Web服务器调用执行，回收过滤器资源
注意：chain．doFilterO语句以前的代码用于对客户请求的处理；以后的代码用于对响应进行处理。
(2)配置过滤器
在应用程序Web—INF目录下的web．xml描述符文件中添加以下代码：
**[html]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7423215#)[copy](http://blog.csdn.net/csh624366188/article/details/7423215#)[print](http://blog.csdn.net/csh624366188/article/details/7423215#)[?](http://blog.csdn.net/csh624366188/article/details/7423215#)
- <filter>
- <filter-name>ipfIter<／filter-name>／／过滤器名称  
- <filter-class>ipf．ipfiIter<／filter-class>／／实现过滤器的类  
- <init—param>
- <param—name>RejectedlP<／param-name>／／过滤器初始化参数名RejectedlP  
- <param-value>192.168.12.*／param-value>
- <／init—pamm>
- <／filter>
- <filter-mapping>／／过滤器映射(规律规则)  
- <filter-name>ipfiIter<／filter-name>
- <url—pattem>/*<／ud-pattem>
- ／／映射到Web应用根目录下的所有JSP文件  
- <／filter-mapping>
通过以上设计与配置，就禁止了IP地址处在192.168.12网段的用户对网站的访问。

**监听器**
一、监听器概述
监听你的web应用，监听许多信息的初始化，销毁，增加，修改，删除值等
Servlet监听器用于监听一些重要事件的发生，监听器对象可以在事情发生前、发生后可以做一些必要的处理。
  1.Listener是Servlet的监听器 
  2.可以监听客户端的请求、服务端的操作等。
  3.通过监听器，可以自动激发一些操作，如监听在线用户数量,当增加一个HttpSession时，给在线人数加1。
  4.编写监听器需要实现相应的接口
  5.编写完成后在web.xml文件中配置一下,就可以起作用了
  6.可以在不修改现有系统基础上,增加web应用程序生命周期事件的跟踪
servlet 规范中为每种事件监听器都定义了相应的接口，在编写事件监听器程序时只需实现这些接口就可以了。一些Servlet事件监听器需要在web应用程序的部署 文件描述符文件（web.xml）中进行注册(注册之后才能发布)，一个web.xml可以注册多个servlet事件监听器。web服务器按照它们在web.xml中注册顺序来加载和注册这些servlet事件监听器。servlet事件监听器的注册和调用过程都是由web容器自动完成的，当发生被监听对象被创建，修改，销毁等事件时，web容器将调用与之相关的servlet事件监听器对象的相应方法(所监听到的对象如果在创建、修改、销毁事件触发的时候就会调用这些监听器这就相当于面向事件编程的概念)，用户在这些方法中编写的事件处理代码(相当于JS中的事件响应)即被执行。由于在一个web应用程序中只会为每个事件监听器类创建一个实例对象，有可能出现多个线程同时调用一个事件监听对象的情况，所以要注意多线程安全问题。
二、监听器类型
按监听的对象划分：servlet2.4规范定义的事件有三种：
1.用于监听应用程序环境对象（ServletContext）的事件监听器
2.用于监听用户会话对象（HttpSession）的事件监听器
3.用于监听请求消息对象（ServletRequest）的事件监听器
按监听的事件类项划分
1.用于监听域对象自身的创建和销毁的事件监听器
2.用于监听域对象中的属性的增加和删除的事件监听器
3.用于监听绑定到HttpSession域中的某个对象的状态的事件监听器
在一个web应用程序的整个运行周期内，web容器会创建和销毁三个重要的对象，ServletContext，HttpSession,ServletRequest。
PS：其中Context 为JSP页面包装页面的上下文.由容器创建和初始化,管理对属于JSP中特殊可见部分中已命名对象的访问. 该接口用来定义了一个Servlet的环境对象。也可认为这是多个客户端共享的信息，它与session的区别在于应用范围的不同，session只对应于一个用户。 
servlet2.4中定义了三个接口：
ServletContextListener,HttpSessionListener,ServletRequestListener。分别实现对应的接口就可以实现对应的监听处理
在ServletContextListener接口中定义了两个事件处理方法，分别是
contextInitialized（）和contextDestroyed（）
public void contextInitialized(ServletcontextEvent sce)
这个方法接受一个ServletContextEvent类型参数，在contextInitialized可以通过这个参数获得当前被创建的ServletContext对象。
public void contextDestroyed(ServletContextEvent sce)
2.在HttpSessionListneter接口中共定义了两个事件处理方法，分别是sessionCreated（）和sessionDestroyed（）
public void sessionCreated(HttpSessionEvent se)
这个方法接受一个(HttpSessionEvent 类型参数，在sessionCreated可以通过这个参数获得当前被创建的HttpSession对象。
public void sessionDestroyed(HttpSessionEvent se)
 3.在ServletRequestListener接口中定义了两个事件处理方法，分别是requestInitialized（）和requestDestroyed（）
public void requestInitialized(ServletRequestEvent sre)
这个方法接受一个(ServletRequestEvent 类型参数，在requestInitialized可以通过这个参数获得当前被创建的ServletRequest对象。
public void requestDestroyed(ServletRequestEvent sre)
可 以看出三个监听器接口中定义的方法非常相似，执行原理与应用方式也相似，在web应用程序中可以注册一个或者多个实现了某一接口的事件监听器，web容器 在创建或销毁某一对象（如ServletContext，HttpSession）时就会产生相应的事件对象
（如ServletcontextEvent ，或者HttpSessionEvent），接着依次调用每个事件监听器中的相应处理方法，并将产生的事件对象传递给这些方法。
三、分类及介绍
1． ServletContextListener：用于监听WEB 应用启动和销毁的事件，监听器类需要实现javax.servlet.ServletContextListener 接口。
2． ServletContextAttributeListener：用于监听WEB应用属性改变的事件，包括：增加属性、删除属性、修改属性，监听器类需要实现javax.servlet.ServletContextAttributeListener接口。
3． HttpSessionListener：用于监听Session对象的创建和销毁，监听器类需要实现javax.servlet.http.HttpSessionListener接口或者javax.servlet.http.HttpSessionActivationListener接口，或者两个都实现。
4． HttpSessionActivationListener：用于监听Session对象的钝化/活化事件，监听器类需要实现javax.servlet.http.HttpSessionListener接口或者javax.servlet.http.HttpSessionActivationListener接口，或者两个都实现。
5． HttpSessionAttributeListener：用于监听Session对象属性的改变事件，监听器类需要实现javax.servlet.http.HttpSessionAttributeListener接口。
四、部署
监听器的部署在web.xml文件中配置，在配置文件中，它的位置应该在过滤器的后面Servle的前面
五、示例
**第一步：编写监听器类**
**[html]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7423215#)[copy](http://blog.csdn.net/csh624366188/article/details/7423215#)[print](http://blog.csdn.net/csh624366188/article/details/7423215#)[?](http://blog.csdn.net/csh624366188/article/details/7423215#)
- package cn.listen;  
- 
- import javax.servlet.ServletContextEvent;  
- 
- import javax.servlet.ServletContextListener;  
- 
- 
- public class MyListener implements ServletContextListener {  
- 
-     public void contextDestroyed(ServletContextEvent sce) {  
- 
-            System.out.println("die");  
- 
-     }  
- 
-     public void contextInitialized(ServletContextEvent sce) {  
- 
-            System.out.println("init");   
- 
-     }  
- 
- }   
第二步：布置安装
<listener> 
  <listener-class>cn.listen.MyListener</listener-class>
</listener> 
运行服务器会出现
[20:42:38.406] {main} WebApp[http://default] active
 init
[20:42:38.437] {main} WebApp[http://default/MyProj] active 
监听到了应用启动。
