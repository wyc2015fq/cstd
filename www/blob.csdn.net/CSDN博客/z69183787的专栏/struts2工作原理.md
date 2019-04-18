# struts2工作原理 - z69183787的专栏 - CSDN博客
2015年05月20日 13:28:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：596
![struts2工作原理 - sam - 姜昌君](http://img2.ph.126.net/0YqdM9S5TxSpWNP5pG72Mg==/2481483394698654472.jpg)
**1. Struts2架构图**
请求首先通过Filter chain，Filter主要包括ActionContextCleanUp，它主要清理当前线程的ActionContext和Dispatcher；FilterDispatcher主要通过AcionMapper来决定需要调用哪个Action。 
ActionMapper取得了ActionMapping后，在Dispatcher的serviceAction方法里创建ActionProxy，ActionProxy创建ActionInvocation，然后ActionInvocation调用Interceptors，执行Action本身，创建Result并返回，当然，如果要在返回之前做些什么，可以实现PreResultListener。
**2. Struts2部分类介绍**
这部分从Struts2参考文档中翻译就可以了。 
ActionMapper 
ActionMapper其实是HttpServletRequest和Action调用请求的一个映射，它屏蔽了Action对于Request等java Servlet类的依赖。Struts2中它的默认实现类是DefaultActionMapper，ActionMapper很大的用处可以根据自己的需要来设计url格式，它自己也有Restful的实现，具体可以参考文档的docs\actionmapper.html。 
ActionProxy&ActionInvocation 
Action的一个代理，由ActionProxyFactory创建，它本身不包括Action实例，默认实现DefaultActionProxy是由ActionInvocation持有Action实例。ActionProxy作用是如何取得Action，无论是本地还是远程。而ActionInvocation的作用是如何执行Action，拦截器的功能就是在ActionInvocation中实现的。 
ConfigurationProvider&Configuration 
ConfigurationProvider就是Struts2中配置文件的解析器，Struts2中的配置文件主要是尤其实现类XmlConfigurationProvider及其子类StrutsXmlConfigurationProvider来解析。
**3. Struts2请求流程**
1、客户端发送请求 
2、请求先通过ActionContextCleanUp-->FilterDispatcher 
3、FilterDispatcher通过ActionMapper来决定这个Request需要调用哪个Action 
4、如果ActionMapper决定调用某个Action，FilterDispatcher把请求的处理交给ActionProxy，这儿已经转到它的Delegate--Dispatcher来执行 
5、ActionProxy根据ActionMapping和ConfigurationManager找到需要调用的Action类 
6、ActionProxy创建一个ActionInvocation的实例 
7、ActionInvocation调用真正的Action，当然这涉及到相关拦截器的调用 
8、Action执行完毕，ActionInvocation创建Result并返回，当然，如果要在返回之前做些什么，可以实现PreResultListener。添加PreResultListener可以在Interceptor中实现。=======================================================================附简洁版处理过程：
Struts 2框架的处理过程：
客户端初始化一个指向Servlet容器（如Tomcat）的请求。
这个请求经过一系列过滤器（如ActionContextCleanUp、SiteMesh等）。
FilterDispatcher被调用，FilterDispatcher询问ActionMapper来决定这个请求是否需要调用某个Action。
如果ActionMapper决定需要调用某个Action，FilterDispatcher把请求交给ActionProxy。
ActionProxy通过Configuration Manager询问Struts配置文件，找到需要调用的Action类。
ActionProxy创建一个ActionInvocation实例。
ActionInvocation实例使用命名的模式来调用，回调Action的execute方法。
一旦Action执行完毕，ActionInvocation负责根据Struts.xml的配置返回结果
ActionContextCleanUp的作用:
延长action中属性的生命周期，包括自定义属性，以便在jsp页面中进行访问，让actionContextcleanup过滤器来清除属性，不让action自己清除。
    为了使用WebWork,我们只需要在web.xml配置FilterDispatcher一个过滤器即可,阅读一下FilterDispatcher的JavaDoc和源码,我们可以看到它调用了:
 finally
 {
            ActionContextCleanUp.cleanUp(req);
 } 
在ActionContextCleanUp中,有这样的代码:
req.setAttribute(CLEANUP_PRESENT, Boolean.TRUE); 
如果FilterDispatcher检测到这个属性,就不会清除ActionContext中的内容了,而由ActionContextCleanUp后续的代码来清除,保证了一系列的Filter访问正确的ActionContext.
文档中提到,如果用到SiteMesh的Filter或者其他类似Filter,那么设置顺序是:
 ActionContextCleanUp filter
 SiteMesh filter
 FilterDispatcher
 所以最后我们的web.xml应该类似这样:
    <filter>
        <filter-name>ActionContextCleanUp</filter-name>
        <filter-class>com.opensymphony.webwork.dispatcher.ActionContextCleanUp</filter-class>
    </filter>
    <filter>
        <filter-name>sitemesh</filter-name>
        <filter-class>com.opensymphony.webwork.sitemesh.FreeMarkerPageFilter</filter-class>
    </filter>
    <filter>
        <filter-name>webwork</filter-name>
        <filter-class>com.opensymphony.webwork.dispatcher.FilterDispatcher</filter-class>
    </filter>
    <filter-mapping>
        <filter-name>ActionContextCleanUp</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
    <filter-mapping>
        <filter-name>sitemesh</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
    <filter-mapping>
        <filter-name>webwork</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
---------------------------------------------------------------------------------------------------------------------
在Struts 2.1.6之前，ActionContextCleanUp的完整路径是com.opensymphony.webwork.dispatcher.ActionContextCleanUp，现在的路径变成了org.apache.struts2.dispatcher.ActionContextCleanup。
     那么这个类究竟有什么用处呢？是不是一定要用呢？
     下面是这个类内部的注释。
     Special filter designed to work with the FilterDispatcher and allow
for easier integration with SiteMesh. Normally, ordering your filters to have
SiteMesh go first, and then FilterDispatcher go second is perfectly fine.
However, sometimes you may wish to access Struts features, including the
value stack, from within your SiteMesh decorators. Because FilterDispatcher
cleans up the ActionContext, your decorator won't have access to the
data you want.
     By adding this filter, the FilterDispatcher will know to not clean up and
instead defer cleanup to this filter. The ordering of the filters should then be:
1.this filter
2.SiteMesh filter
3.FilterDispatcher
      就是说，一般情况下，如果你要用SiteMesh或者其他过滤器，一般是放在FilterDispatcher或者是现在的StrutsPrepareAndExecuteFilter之前。在调用完所有过滤器的doFilter方法后，核心过滤器FilterDispatcher或者StrutsPrepareAndExecuteFilter会清空ActionContext，如果其他过滤器要一直使用value
 stack等struts的特性时，如果不用ActionContextCleanUp的话，便得不到想要的值。
ActionContextCleanUp的作用就是上面用粗体标注出来的那一句。它会在doFilter方法里设置一个计数器counter的初始值为1，有了这个值，后续的核心过滤器就不会清空ActionContext，而是由之前的过滤器也就是ActionContextCleanUp来清空ActionContext。
