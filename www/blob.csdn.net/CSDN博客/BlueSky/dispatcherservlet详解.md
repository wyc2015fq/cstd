# dispatcherservlet详解 - BlueSky - CSDN博客
2015年12月21日 15:36:47[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：295
个人分类：[Java Spring](https://blog.csdn.net/ASIA_kobe/article/category/6039238)
## 转自：http://www.iteye.com/topic/1125421
## 3.1、DispatcherServlet作用
DispatcherServlet是前端控制器设计模式的实现，提供Spring Web MVC的集中访问点，而且负责职责的分派，而且与Spring IoC容器无缝集成，从而可以获得Spring的所有好处。 具体请参考第二章的图2-1。
DispatcherServlet主要用作职责调度工作，本身主要用于控制流程，主要职责如下：
1、文件上传解析，如果请求类型是multipart将通过MultipartResolver进行文件上传解析；
2、通过HandlerMapping，将请求映射到处理器（返回一个HandlerExecutionChain，它包括一个处理器、多个HandlerInterceptor拦截器）；
3、  通过HandlerAdapter支持多种类型的处理器(HandlerExecutionChain中的处理器)；
4、通过ViewResolver解析逻辑视图名到具体视图实现；
5、本地化解析；
6、渲染具体的视图等；
7、如果执行过程中遇到异常将交给HandlerExceptionResolver来解析。
从以上我们可以看出DispatcherServlet主要负责流程的控制（而且在流程中的每个关键点都是很容易扩展的）。
## 3.2、DispatcherServlet在web.xml中的配置
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <servlet>  
-     <servlet-name>chapter2</servlet-name>  
-     <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
-     <load-on-startup>1</load-on-startup>  
- </servlet>  
- <servlet-mapping>  
-     <servlet-name>chapter2</servlet-name>  
-     <url-pattern>/</url-pattern>  
- </servlet-mapping>  
**load-on-startup：**表示启动容器时初始化该Servlet；
**url-pattern：**表示哪些请求交给Spring Web MVC处理， “/” 是用来定义默认servlet映射的。也可以如“*.html”表示拦截所有以html为扩展名的请求。
该DispatcherServlet默认使用WebApplicationContext作为上下文，Spring默认配置文件为“/WEB-INF/[servlet名字]-servlet.xml”。
DispatcherServlet也可以配置自己的初始化参数，覆盖默认配置：
摘自Spring Reference
|**参数**|**描述**|
|----|----|
|contextClass|实现WebApplicationContext接口的类，当前的servlet用它来创建上下文。如果这个参数没有指定， 默认使用XmlWebApplicationContext。|
|contextConfigLocation|传给上下文实例（由contextClass指定）的字符串，用来指定上下文的位置。这个字符串可以被分成多个字符串（使用逗号作为分隔符） 来支持多个上下文（在多上下文的情况下，如果同一个bean被定义两次，后面一个优先）。|
|namespace|WebApplicationContext命名空间。默认值是[server-name]-servlet。|
因此我们可以通过添加初始化参数
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <servlet>  
-     <servlet-name>chapter2</servlet-name>  
-     <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
-     <load-on-startup>1</load-on-startup>  
-     <init-param>  
-         <param-name>contextConfigLocation</param-name>  
-         <param-value>classpath:spring-servlet-config.xml</param-value>  
-     </init-param>  
- </servlet>  
如果使用如上配置，Spring Web MVC框架将加载“classpath:spring-servlet-config.xml”来进行初始化上下文而不是“/WEB-INF/[servlet名字]-servlet.xml”。
## 3.3、上下文关系
集成Web环境的通用配置：
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <context-param>  
-       <param-name>contextConfigLocation</param-name>  
-       <param-value>  
-           classpath:spring-common-config.xml,  
-           classpath:spring-budget-config.xml  
-       </param-value>  
- </context-param>  
- <listener>  <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>  
- </listener>  
如上配置是Spring集成Web环境的通用配置；一般用于加载除Web层的Bean（如DAO、Service等），以便于与其他任何Web框架集成。
contextConfigLocation：表示用于加载Bean的配置文件；
contextClass：表示用于加载Bean的ApplicationContext实现类，默认WebApplicationContext。
创建完毕后会将该上下文放在ServletContext：
servletContext.setAttribute(
WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE,
this.context);
**ContextLoaderListener初始化的上下文和DispatcherServlet初始化的上下文关系，如图3-1**
![](http://dl.iteye.com/upload/attachment/0071/1167/2522b621-90f4-3558-9677-10af15124ade.jpg)
图3-1
从图中可以看出：
ContextLoaderListener初始化的上下文加载的Bean是对于整个应用程序共享的，不管是使用什么表现层技术，一般如DAO层、Service层Bean；
DispatcherServlet初始化的上下文加载的Bean是只对Spring Web MVC有效的Bean，如Controller、HandlerMapping、HandlerAdapter等等，该初始化上下文应该只加载Web相关组件。
## 3.4、DispatcherServlet初始化顺序
**继承体系结构如下所示：**
![](http://dl.iteye.com/upload/attachment/0071/1169/13d77614-4268-306d-8b14-ebc0f24bfac1.jpg)
1、**HttpServletBean继承HttpServlet，**因此在Web容器启动时将调用它的init方法，该初始化方法的主要作用
：：：将Servlet初始化参数（init-param）设置到该组件上（如contextAttribute、contextClass、namespace、contextConfigLocation），通过BeanWrapper简化设值过程，方便后续使用；
：：：提供给子类初始化扩展点，initServletBean()，该方法由FrameworkServlet覆盖。
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- public abstract class HttpServletBean extends HttpServlet implements EnvironmentAware{  
- @Override  
-     public final void init() throws ServletException {  
-        //省略部分代码  
-        //1、如下代码的作用是将Servlet初始化参数设置到该组件上  
- //如contextAttribute、contextClass、namespace、contextConfigLocation；  
-        try {  
-            PropertyValues pvs = new ServletConfigPropertyValues(getServletConfig(), this.requiredProperties);  
-            BeanWrapper bw = PropertyAccessorFactory.forBeanPropertyAccess(this);  
-            ResourceLoader resourceLoader = new ServletContextResourceLoader(getServletContext());  
-            bw.registerCustomEditor(Resource.class, new ResourceEditor(resourceLoader, this.environment));  
-            initBeanWrapper(bw);  
-            bw.setPropertyValues(pvs, true);  
-        }  
-        catch (BeansException ex) {  
-            //…………省略其他代码  
-        }  
-        //2、提供给子类初始化的扩展点，该方法由FrameworkServlet覆盖  
-        initServletBean();  
-        if (logger.isDebugEnabled()) {  
-            logger.debug("Servlet '" + getServletName() + "' configured successfully");  
-        }  
-     }  
-     //…………省略其他代码  
- }  
**2、FrameworkServlet继承HttpServletBean，**通过initServletBean()进行Web上下文初始化，该方法主要覆盖一下两件事情：
    初始化web上下文；
    提供给子类初始化扩展点；
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- public abstract class FrameworkServlet extends HttpServletBean {  
- @Override  
-     protected final void initServletBean() throws ServletException {  
-         //省略部分代码  
-        try {  
-              //1、初始化Web上下文  
-            this.webApplicationContext = initWebApplicationContext();  
-              //2、提供给子类初始化的扩展点  
-            initFrameworkServlet();  
-        }  
-         //省略部分代码  
-     }  
- }  
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- protected WebApplicationContext initWebApplicationContext() {  
-         //ROOT上下文（ContextLoaderListener加载的）  
-        WebApplicationContext rootContext =  
-               WebApplicationContextUtils.getWebApplicationContext(getServletContext());  
-        WebApplicationContext wac = null;  
-        if (this.webApplicationContext != null) {  
-            // 1、在创建该Servlet注入的上下文  
-            wac = this.webApplicationContext;  
-            if (wac instanceof ConfigurableWebApplicationContext) {  
-               ConfigurableWebApplicationContext cwac = (ConfigurableWebApplicationContext) wac;  
-               if (!cwac.isActive()) {  
-                   if (cwac.getParent() == null) {  
-                       cwac.setParent(rootContext);  
-                   }  
-                   configureAndRefreshWebApplicationContext(cwac);  
-               }  
-            }  
-        }  
-        if (wac == null) {  
-              //2、查找已经绑定的上下文  
-            wac = findWebApplicationContext();  
-        }  
-        if (wac == null) {  
-             //3、如果没有找到相应的上下文，并指定父亲为ContextLoaderListener  
-            wac = createWebApplicationContext(rootContext);  
-        }  
-        if (!this.refreshEventReceived) {  
-              //4、刷新上下文（执行一些初始化）  
-            onRefresh(wac);  
-        }  
-        if (this.publishContext) {  
-            // Publish the context as a servlet context attribute.  
-            String attrName = getServletContextAttributeName();  
-            getServletContext().setAttribute(attrName, wac);  
-            //省略部分代码  
-        }  
-        return wac;  
-     }  
从initWebApplicationContext（）方法可以看出，基本上如果ContextLoaderListener加载了上下文将作为根上下文（DispatcherServlet的父容器）。
最后调用了onRefresh()方法执行容器的一些初始化，这个方法由子类实现，来进行扩展。
**3、DispatcherServlet继承FrameworkServlet**，并实现了onRefresh()方法提供一些前端控制器相关的配置：
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- public class DispatcherServlet extends FrameworkServlet {  
-      //实现子类的onRefresh()方法，该方法委托为initStrategies()方法。  
-     @Override  
-     protected void onRefresh(ApplicationContext context) {  
-        initStrategies(context);  
-     }  
- 
-     //初始化默认的Spring Web MVC框架使用的策略（如HandlerMapping）  
-     protected void initStrategies(ApplicationContext context) {  
-        initMultipartResolver(context);  
-        initLocaleResolver(context);  
-        initThemeResolver(context);  
-        initHandlerMappings(context);  
-        initHandlerAdapters(context);  
-        initHandlerExceptionResolvers(context);  
-        initRequestToViewNameTranslator(context);  
-        initViewResolvers(context);  
-        initFlashMapManager(context);  
-     }  
- }  
从如上代码可以看出，DispatcherServlet启动时会进行我们需要的Web层Bean的配置，如HandlerMapping、HandlerAdapter等，而且如果我们没有配置，还会给我们提供默认的配置。
从如上代码我们可以看出，整个DispatcherServlet初始化的过程和做了些什么事情，具体主要做了如下两件事情：
1、初始化Spring Web MVC使用的Web上下文，并且可能指定父容器为（ContextLoaderListener加载了根上下文）；
2、初始化DispatcherServlet使用的策略，如HandlerMapping、HandlerAdapter等。
