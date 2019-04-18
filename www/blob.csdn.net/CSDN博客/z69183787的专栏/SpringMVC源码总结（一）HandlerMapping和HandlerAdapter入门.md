# SpringMVC源码总结（一）HandlerMapping和HandlerAdapter入门 - z69183787的专栏 - CSDN博客
2016年10月14日 16:24:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1315
刚接触SpringMVC,对它的xml文件配置一直比较模模糊糊，最近花了一点时间稍微看了下源代码，再加上调试，开始逐渐理解它，网上的类似的内容有很多，写本文主要是自己加深一下理解。本文适合用过SpringMVC的开发者，言归正传，首先搭建一个最简单的工程体验一下。 
该工程是基于maven的，pom配置不再说明，所使用的spring版本4.0.5。 
首先是web.xml文件配置，最简单的配置 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <!DOCTYPE web-app PUBLIC  
- "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
- "http://java.sun.com/dtd/web-app_2_3.dtd" >  
- 
- <web-app>  
-   <display-name>Archetype Created Web Application</display-name>  
-   <servlet>  
-         <servlet-name>mvc</servlet-name>  
-         <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
-         <load-on-startup>1</load-on-startup>  
-     </servlet>  
- 
-     <servlet-mapping>  
-         <servlet-name>mvc</servlet-name>  
-         <url-pattern>/*</url-pattern>  
-     </servlet-mapping>  
- </web-app>  
然后是mvc-servlet.xml文件的配置，上面配置DispatcherServlet会默认加载[servlet-name]-servlet.xml文件。对于我的配置，会去加载mvc-servlet.xml文件。 
mvc-servlet.xml文件的内容：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <?xml version="1.0" encoding="UTF-8" ?>  
- <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:util="http://www.springframework.org/schema/util" xmlns:context="http://www.springframework.org/schema/context"
-     xsi:schemaLocation="http://www.springframework.org/schema/beans
-     http://www.springframework.org/schema/beans/spring-beans-3.1.xsd
-     http://www.springframework.org/schema/mvc
-     http://www.springframework.org/schema/mvc/spring-mvc-3.1.xsd
-     http://www.springframework.org/schema/util
-     http://www.springframework.org/schema/util/spring-util-2.0.xsd
-     http://www.springframework.org/schema/context 
-     http://www.springframework.org/schema/context/spring-context-3.2.xsd">
- 
-     <bean name="/index"class="com.lg.mvc.HomeAction"></bean>  
-     <bean class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">  
-         <property name="templateLoaderPath" value="/WEB-INF/views" />  
-         <property name="defaultEncoding" value="utf-8" />  
-         <property name="freemarkerSettings">  
-             <props>  
-                 <prop key="locale">zh_CN</prop>  
-             </props>  
-         </property>  
-     </bean>  
-     <bean class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">  
-         <property name="suffix" value=".html" />  
-         <property name="contentType" value="text/html;charset=utf-8" />  
-         <property name="requestContextAttribute" value="request" />  
-         <property name="exposeRequestAttributes" value="true" />  
-         <property name="exposeSessionAttributes" value="true" />  
-     </bean>  
- </beans>  
在该配置中定义了一个HomeAction的Bean。内容为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- package com.lg.mvc;  
- 
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.springframework.web.servlet.ModelAndView;  
- import org.springframework.web.servlet.mvc.Controller;  
- 
- publicclass HomeAction implements Controller{  
- 
- @Override
- public ModelAndView handleRequest(HttpServletRequest request,  
-             HttpServletResponse response) throws Exception {  
- returnnew ModelAndView("hello");  
-     }  
- }  
这是最原始的mvc做法，要继承Controller接口，先从原始的说起，最后再过渡到@Controller和@RequestMapping注解式的配置。它在mvc-serlet.xml文件中的配置有一个关键的属性name="/index"。 
WEB-INF/view目录下有一个简单的hello.html，内容为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <html>  
-     <head>  
- 
-     </head>  
-     <body>  
-         hello lg !  
-     </body>  
- </html>  
至此该工程就写完了，部署到tomcat中，项目路径为/,运行一下。 
访问 http://localhost:8080/index 
![](http://dl2.iteye.com/upload/attachment/0100/2677/c502670f-46e0-338e-b2d7-d5a12b9a487a.png)
至此整个工程就算搭建成功了。 
下面就要说说原理了。 
用过python Django框架的都知道Django对于访问方式的配置就是，一个url路径和一个函数配对，你访问这个url，就会直接调用这个函数，简单明了。对于java的面向对象来说，就要分两步走。第一步首先要找到是哪个对象，即handler，本工程的handler则是HomeAction对象。第二步要找到访问的函数，即HomeAction的handleRequest方法。所以就出现了两个源码接口 HandlerMapping和HandlerAdapter，前者负责第一步，后者负责第二步。借用网上的SpringMVC架构图。 
![](http://dl2.iteye.com/upload/attachment/0100/2683/a14e5d39-5fcd-35a2-9187-de846501b56d.png)
HandlerMapping接口的实现（只举了我认识的几个） ： 
BeanNameUrlHandlerMapping ：通过对比url和bean的name找到对应的对象 
SimpleUrlHandlerMapping ：也是直接配置url和对应bean,比BeanNameUrlHandlerMapping功能更多 
DefaultAnnotationHandlerMapping : 主要是针对注解配置@RequestMapping的，已过时 
RequestMappingHandlerMapping ：取代了上面一个 
HandlerAdapter 接口实现： 
HttpRequestHandlerAdapter ： 要求handler实现HttpRequestHandler接口，该接口的方法为                                                             void handleRequest(HttpServletRequest request, HttpServletResponse response)也就是  handler必须有一个handleRequest方法 
SimpleControllerHandlerAdapter：要求handler实现Controller接口，该接口的方法为ModelAndView handleRequest(HttpServletRequest request, HttpServletResponse response)，也就是本工程采用的 
AnnotationMethodHandlerAdapter ：和上面的DefaultAnnotationHandlerMapping配对使用的，也已过时 
RequestMappingHandlerAdapter : 和上面的RequestMappingHandlerMapping配对使用，针对@RequestMapping 
先简单的说下这个工程的流程，访问http://localhost:8080/index首先由DispatcherServlet进行转发，通过BeanNameUrlHandlerMapping(含有 /index->HomeAction的配置)，找到了HomeAction,然后再拿HomeAction和每个adapter进行适配，由于HomeAction实现了Controller接口，所以最终会有SimpleControllerHandlerAdapter来完成对HomeAction的handleRequest方法的调度。然后就顺利的执行了我们想要的方法，后面的内容不在本节中说明。 
了解了大概流程，然后就需要看源代码了。 
首先就是SpringMVC的入口类，DispatcherServlet，它实现了Servlet接口，不再详细说DispatcherServlet的细节，不然又是一大堆的内容。每次请求都会调用它的doService->doDispatch，我们关注的重点就在doDispatch方法中。 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedvoid doDispatch(HttpServletRequest request, HttpServletResponse response) throws Exception {  
-         HttpServletRequest processedRequest = request;  
-         HandlerExecutionChain mappedHandler = null;  
- boolean multipartRequestParsed = false;  
- 
-         WebAsyncManager asyncManager = WebAsyncUtils.getAsyncManager(request);  
- 
- try {  
-             ModelAndView mv = null;  
-             Exception dispatchException = null;  
- 
- try {  
-                 processedRequest = checkMultipart(request);  
-                 multipartRequestParsed = (processedRequest != request);  
- //这个是重点，第一步由HandlerMapping找到对应的handler
- // Determine handler for the current request.
-                 mappedHandler = getHandler(processedRequest);  
- if (mappedHandler == null || mappedHandler.getHandler() == null) {  
-                     noHandlerFound(processedRequest, response);  
- return;  
-                 }  
- 
- // Determine handler adapter for the current request.
- //这是第二步，找到合适的HandlerAdapter，然后由它来调度执行handler的方法
-                 HandlerAdapter ha = getHandlerAdapter(mappedHandler.getHandler());  
- 
- // Process last-modified header, if supported by the handler.
-                 String method = request.getMethod();  
- boolean isGet = "GET".equals(method);  
- if (isGet || "HEAD".equals(method)) {  
- long lastModified = ha.getLastModified(request, mappedHandler.getHandler());  
- if (logger.isDebugEnabled()) {  
-                         logger.debug("Last-Modified value for [" + getRequestUri(request) + "] is: " + lastModified);  
-                     }  
- if (new ServletWebRequest(request, response).checkNotModified(lastModified) && isGet) {  
- return;  
-                     }  
-                 }  
- 
- if (!mappedHandler.applyPreHandle(processedRequest, response)) {  
- return;  
-                 }  
- 
- try {  
- // Actually invoke the handler.
-                     mv = ha.handle(processedRequest, response, mappedHandler.getHandler());  
-                 }  
- finally {  
- if (asyncManager.isConcurrentHandlingStarted()) {  
- return;  
-                     }  
-                 }  
- 
-                 applyDefaultViewName(request, mv);  
-                 mappedHandler.applyPostHandle(processedRequest, response, mv);  
-             }  
- catch (Exception ex) {  
-                 dispatchException = ex;  
-             }  
-             processDispatchResult(processedRequest, response, mappedHandler, mv, dispatchException);  
-         }  
- catch (Exception ex) {  
-             triggerAfterCompletion(processedRequest, response, mappedHandler, ex);  
-         }  
- catch (Error err) {  
-             triggerAfterCompletionWithError(processedRequest, response, mappedHandler, err);  
-         }  
- finally {  
- if (asyncManager.isConcurrentHandlingStarted()) {  
- // Instead of postHandle and afterCompletion
-                 mappedHandler.applyAfterConcurrentHandlingStarted(processedRequest, response);  
- return;  
-             }  
- // Clean up any resources used by a multipart request.
- if (multipartRequestParsed) {  
-                 cleanupMultipart(processedRequest);  
-             }  
-         }  
-     }  
第一步详细查看：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected HandlerExecutionChain getHandler(HttpServletRequest request) throws Exception {  
- for (HandlerMapping hm : this.handlerMappings) {  
- if (logger.isTraceEnabled()) {  
-                 logger.trace(  
- "Testing handler map [" + hm + "] in DispatcherServlet with name '" + getServletName() + "'");  
-             }  
-             HandlerExecutionChain handler = hm.getHandler(request);  
- if (handler != null) {  
- return handler;  
-             }  
-         }  
- returnnull;  
-     }  
可以看到就是通过遍历所有已注册的HandlerMapping来找到对应的handler,然后构建出一个HandlerExecutionChain，它包含了handler和HandlerMapping本身的一些拦截器，如下
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass HandlerExecutionChain {  
- 
- privatefinal Object handler;  
- 
- private HandlerInterceptor[] interceptors;  
- 
- private List<HandlerInterceptor> interceptorList;  
- 
- //其他代码省略
- }  
其中HandlerMapping的getHandler实现：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicfinal HandlerExecutionChain getHandler(HttpServletRequest request) throws Exception {  
-         Object handler = getHandlerInternal(request);  
- if (handler == null) {  
-             handler = getDefaultHandler();  
-         }  
- if (handler == null) {  
- returnnull;  
-         }  
- // Bean name or resolved handler?
- if (handler instanceof String) {  
-             String handlerName = (String) handler;  
-             handler = getApplicationContext().getBean(handlerName);  
-         }  
- return getHandlerExecutionChain(handler, request);  
-     }  
这里的getHandlerInternal(request)是个抽象方法，由具体的HandlerMapping来实现，获取到的handler如果为空,则获取默认配置的handler，如果handler为String类型，则表示这个则会去Spring容器里面去找这样名字的bean。 
再看下BeanNameUrlHandlerMapping的getHandlerInternal（request）的具体实现（通过一系列的接口设计，之后再好好看看这个设计，到BeanNameUrlHandlerMapping这只用实现该方法中的一部分），如下
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass BeanNameUrlHandlerMapping extends AbstractDetectingUrlHandlerMapping {  
- 
- /**
-      * Checks name and aliases of the given bean for URLs, starting with "/".
-      */
- @Override
- protected String[] determineUrlsForHandler(String beanName) {  
-         List<String> urls = new ArrayList<String>();  
- if (beanName.startsWith("/")) {  
-             urls.add(beanName);  
-         }  
-         String[] aliases = getApplicationContext().getAliases(beanName);  
- for (String alias : aliases) {  
- if (alias.startsWith("/")) {  
-                 urls.add(alias);  
-             }  
-         }  
- return StringUtils.toStringArray(urls);  
-     }  
- 
- }  
这里面注释说，bean的name必须以/开头，它才处理，将信息存储在Map<String, Object> handlerMap中，对于本工程来说就是{'/index':HomeAction对象}。 
至此这里完成了第一步，下面开始第二步，即方法HandlerAdapter ha = getHandlerAdapter(mappedHandler.getHandler());的具体实现：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected HandlerAdapter getHandlerAdapter(Object handler) throws ServletException {  
- for (HandlerAdapter ha : this.handlerAdapters) {  
- if (logger.isTraceEnabled()) {  
-                 logger.trace("Testing handler adapter [" + ha + "]");  
-             }  
- if (ha.supports(handler)) {  
- return ha;  
-             }  
-         }  
- thrownew ServletException("No adapter for handler [" + handler +  
- "]: The DispatcherServlet configuration needs to include a HandlerAdapter that supports this handler");  
-     }  
遍历所有的HandlerAdapter,判断他们是否支持这个handler。 
我们来看下HttpRequestHandlerAdapter的supports(handler)方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass HttpRequestHandlerAdapter implements HandlerAdapter {  
- 
- @Override
- publicboolean supports(Object handler) {  
- //就是判断handler是否实现了HttpRequestHandler接口
- return (handler instanceof HttpRequestHandler);  
-     }  
- 
- @Override
- public ModelAndView handle(HttpServletRequest request, HttpServletResponse response, Object handler)  
- throws Exception {  
- //若handler实现了HttpRequestHandler接口，则调用该接口的方法，执行我们在该方法中写的业务逻辑
-         ((HttpRequestHandler) handler).handleRequest(request, response);  
- returnnull;  
-     }  
- 
- @Override
- publiclong getLastModified(HttpServletRequest request, Object handler) {  
- if (handler instanceof LastModified) {  
- return ((LastModified) handler).getLastModified(request);  
-         }  
- return -1L;  
-     }  
- 
- }  
同理SimpleControllerHandlerAdapter也是这样类似的逻辑
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass SimpleControllerHandlerAdapter implements HandlerAdapter {  
- 
- @Override
- publicboolean supports(Object handler) {  
- return (handler instanceof Controller);  
-     }  
- 
- @Override
- public ModelAndView handle(HttpServletRequest request, HttpServletResponse response, Object handler)  
- throws Exception {  
- 
- return ((Controller) handler).handleRequest(request, response);  
-     }  
- 
- @Override
- publiclong getLastModified(HttpServletRequest request, Object handler) {  
- if (handler instanceof LastModified) {  
- return ((LastModified) handler).getLastModified(request);  
-         }  
- return -1L;  
-     }  
- 
- }  
剩余两个AnnotationMethodHandlerAdapter和RequestMappingHandlerAdapter就比较复杂，我也没看。 
按照本工程的配置，则SimpleControllerHandlerAdapter是支持HomeAction的，然后就会执行SimpleControllerHandlerAdapter的handle(processedRequest, response, mappedHandler.getHandler())方法。本质上就会调用HomeAction实现Controller接口的方法。至此就分析完了。 
了解过程了之后，然后就是最重要的也是经常配置出问题的地方。DispatcherServlet的handlerMappings和handlerAdapters的来源问题。 
DispatcherServlet初始化的时候，会调用一个方法如下： 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedvoid initStrategies(ApplicationContext context) {  
-         initMultipartResolver(context);  
-         initLocaleResolver(context);  
-         initThemeResolver(context);  
- //初始化一些HandlerMapping
-         initHandlerMappings(context);  
- //初始化一些HandlerAdapter
-         initHandlerAdapters(context);  
-         initHandlerExceptionResolvers(context);  
-         initRequestToViewNameTranslator(context);  
-         initViewResolvers(context);  
-         initFlashMapManager(context);  
-     }  
这里可以看到，它会初始化一些HandlerMapping和HandlerAdapter，这两个方法非常重要，理解了这两个方法你就会知道，配置不对问题出在哪里，下面具体看下这两个方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatevoid initHandlerMappings(ApplicationContext context) {  
- this.handlerMappings = null;  
- 
- if (this.detectAllHandlerMappings) {  
- // Find all HandlerMappings in the ApplicationContext, including ancestor contexts.
-             Map<String, HandlerMapping> matchingBeans =  
-                     BeanFactoryUtils.beansOfTypeIncludingAncestors(context, HandlerMapping.class, true, false);  
- if (!matchingBeans.isEmpty()) {  
- this.handlerMappings = new ArrayList<HandlerMapping>(matchingBeans.values());  
- // We keep HandlerMappings in sorted order.
-                 OrderComparator.sort(this.handlerMappings);  
-             }  
-         }  
- else {  
- try {  
-                 HandlerMapping hm = context.getBean(HANDLER_MAPPING_BEAN_NAME, HandlerMapping.class);  
- this.handlerMappings = Collections.singletonList(hm);  
-             }  
- catch (NoSuchBeanDefinitionException ex) {  
- // Ignore, we'll add a default HandlerMapping later.
-             }  
-         }  
- 
- // Ensure we have at least one HandlerMapping, by registering
- // a default HandlerMapping if no other mappings are found.
- if (this.handlerMappings == null) {  
- this.handlerMappings = getDefaultStrategies(context, HandlerMapping.class);  
- if (logger.isDebugEnabled()) {  
-                 logger.debug("No HandlerMappings found in servlet '" + getServletName() + "': using default");  
-             }  
-         }  
-     }  
detectAllHandlerMappings是DispatcherServlet的一个属性，你是可以在web.xml中配置的，默认是true，如果为true,则会去从本工程mvc-servlet.xml文件中去探测所有实现了HandlerMapping的bean，如果有，则加入DispatcherServlet的handlerMappings中。如果detectAllHandlerMappings为false,则直接去容器中找id="handlerMapping"且实现了HandlerMapping的bean.如果以上都没找到，则会去加载默认的HandlerMapping。
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /** Detect all HandlerMappings or just expect "handlerMapping" bean? */
- privateboolean detectAllHandlerMappings = true;  
本工程由于没有配置HandlerMapping,所以它会去加载默认的，下面看看默认的配置是什么
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected <T> List<T> getDefaultStrategies(ApplicationContext context, Class<T> strategyInterface) {  
-         String key = strategyInterface.getName();  
- //defaultStrategies存储了默认的配置
-         String value = defaultStrategies.getProperty(key);  
- if (value != null) {  
-             String[] classNames = StringUtils.commaDelimitedListToStringArray(value);  
-             List<T> strategies = new ArrayList<T>(classNames.length);  
- for (String className : classNames) {  
- try {  
-                     Class<?> clazz = ClassUtils.forName(className, DispatcherServlet.class.getClassLoader());  
-                     Object strategy = createDefaultStrategy(context, clazz);  
-                     strategies.add((T) strategy);  
-                 }  
- catch (ClassNotFoundException ex) {  
- thrownew BeanInitializationException(  
- "Could not find DispatcherServlet's default strategy class [" + className +  
- "] for interface [" + key + "]", ex);  
-                 }  
- catch (LinkageError err) {  
- thrownew BeanInitializationException(  
- "Error loading DispatcherServlet's default strategy class [" + className +  
- "] for interface [" + key + "]: problem with class file or dependent class", err);  
-                 }  
-             }  
- return strategies;  
-         }  
- else {  
- returnnew LinkedList<T>();  
-         }  
-     }  
继续看看defaultStrategies是如何初始化的：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatestaticfinal Properties defaultStrategies;  
- 
- static {  
- // Load default strategy implementations from properties file.
- // This is currently strictly internal and not meant to be customized
- // by application developers.
- try {  
- //这里的DEFAULT_STRATEGIES_PATH就是DispatcherServlet.properties
-             ClassPathResource resource = new ClassPathResource(DEFAULT_STRATEGIES_PATH, DispatcherServlet.class);  
-             defaultStrategies = PropertiesLoaderUtils.loadProperties(resource);  
-         }  
- catch (IOException ex) {  
- thrownew IllegalStateException("Could not load 'DispatcherServlet.properties': " + ex.getMessage());  
-         }  
-     }  
这里使用静态代码块来加载配置文件DispatcherServlet.properties，它所在位置就是和DispatcherServlet同一目录下面的，如下图所示： 
![](http://dl2.iteye.com/upload/attachment/0100/2735/61234404-0fb0-370b-a908-d62728f53929.png)
该默认的配置文件的内容如下： 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- # Default implementation classes for DispatcherServlet's strategy interfaces.  
- # Used as fallback when no matching beans are found in the DispatcherServlet context.  
- # Not meant to be customized by application developers.  
- 
- org.springframework.web.servlet.LocaleResolver=org.springframework.web.servlet.i18n.AcceptHeaderLocaleResolver  
- 
- org.springframework.web.servlet.ThemeResolver=org.springframework.web.servlet.theme.FixedThemeResolver  
- 
- #这里就是默认的HandlerMapping的配置  
- org.springframework.web.servlet.HandlerMapping=org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping,\  
-     org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping  
- #这里就是默认的HandlerAdapter的配置  
- org.springframework.web.servlet.HandlerAdapter=org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter,\  
-     org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter,\  
-     org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter  
- 
- org.springframework.web.servlet.HandlerExceptionResolver=org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerExceptionResolver,\  
-     org.springframework.web.servlet.mvc.annotation.ResponseStatusExceptionResolver,\  
-     org.springframework.web.servlet.mvc.support.DefaultHandlerExceptionResolver  
- 
- org.springframework.web.servlet.RequestToViewNameTranslator=org.springframework.web.servlet.view.DefaultRequestToViewNameTranslator  
- 
- org.springframework.web.servlet.ViewResolver=org.springframework.web.servlet.view.InternalResourceViewResolver  
- 
- org.springframework.web.servlet.FlashMapManager=org.springframework.web.servlet.support.SessionFlashMapManager  
也就是说，当你什么都没有配置时，默认会加载以上的配置。正是由于有了上述默认配置的BeanNameUrlHandlerMapping（它要求name必须是以/开头的），它才会存储我们在mvc-servlet.xml中配置的<bean name="/index" class="com.lg.mvc.HomeAction"></bean>,同样正是由于有了SimpleControllerHandlerAdapter（由于handler实现了Controller接口，所以它的support方法支持我们的handler），才会调度执行HomeAction的handleRequest方法。
- [最简单的SpringMVC搭建.zip](http://dl.iteye.com/topics/download/d0ce4e36-9220-36d8-900b-359e5bd847a9) (6.2 KB)
- 下载次数: 23
- 

