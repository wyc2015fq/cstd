# 在SpringMVC Controller中注入Request成员域 - z69183787的专栏 - CSDN博客
2017年12月20日 11:40:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：665
[https://www.cnblogs.com/abcwt112/p/7777258.html](https://www.cnblogs.com/abcwt112/p/7777258.html)
# 主题
　　在工作中遇到1个问题....我们定义了一个Controller基类,所有Springmvc自定义的controller都继承它....在它内部定义一个@Autowired HttpServletRequest request;可不可以? 能不能从这个对象里取requestParamters和attributes?
 多线程之间会不会影响?
# 思考
初次思考,我想这应该是不行的.为什么呢?
注入bean是在spring容器启动的时候...request的实现类是在tomcat里(我使用的servlet容器是tomcat)....我又没在spring的容器里配置这个bean.注入应该是失败的...
退一步说,就算是成功了....那注入的也就是1个对象而已.每次servlet接受到请求都会重新生成1个request...这明显和之前启动的那个对象不同吧....怎么想都不可能成功...
如果确实是这样的....那就没有这篇文章了....后来实践了一下..发现这个注入是可以的.使用起来取数据也没任何问题....
其实我那个时候debug看了一下,基本就知道为什么可以取到数据了..但是我并不知道原理和Spring(Springmvc)的处理流程...所以现在研究了一下并记录下来...
# 原理
首先给大家看一下在方法中注入request作为参数和在成员域中注入request的 注入的request对象之间的区别....
![](http://images2017.cnblogs.com/blog/721497/201711/721497-20171103100952545-1892831485.png)
![](http://images2017.cnblogs.com/blog/721497/201711/721497-20171103101007576-1932354152.png)
成员域注入的时候注入的是1个代理对象.是 AutowireUtils.ObjectFactoryDelegatingInvocationHandler的实例.
方法注入的就是一般tomcat原生的requestFacade对象.
所以这是不同的...
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     /**
 2      * Reflective InvocationHandler for lazy access to the current target object.
 3      */
 4     @SuppressWarnings("serial")
 5     private static class ObjectFactoryDelegatingInvocationHandler implements InvocationHandler, Serializable {
 6 
 7         private final ObjectFactory<?> objectFactory;
 8 
 9         public ObjectFactoryDelegatingInvocationHandler(ObjectFactory<?> objectFactory) {
10             this.objectFactory = objectFactory;
11         }
12 
13         @Override
14         public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
15             String methodName = method.getName();
16             if (methodName.equals("equals")) {
17                 // Only consider equal when proxies are identical.
18                 return (proxy == args[0]);
19             }
20             else if (methodName.equals("hashCode")) {
21                 // Use hashCode of proxy.
22                 return System.identityHashCode(proxy);
23             }
24             else if (methodName.equals("toString")) {
25                 return this.objectFactory.toString();
26             }
27             try {
28                 return method.invoke(this.objectFactory.getObject(), args);
29             }
30             catch (InvocationTargetException ex) {
31                 throw ex.getTargetException();
32             }
33         }
34     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
当代理对象(就是成员域request)的大部分方法被调用的时候,ObjectFactoryDelegatingInvocationHandler会使用objectFactory获取对象(原生request),再调用对象上的方法.
然后我们来看下XmlWebApplicationContext初始化到请求到进入controller里几个对注入request成员域有影响的步骤.
## refresh方法和postProcessBeanFactory方法
ApplicationContext的抽象实现类AbstractApplicationContext(基本是所有ac的父类)里定义了ac的refresh方法(包含了使用BeanFactory注入bean)的流程..
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 @Override
 2     public void refresh() throws BeansException, IllegalStateException {
 3         synchronized (this.startupShutdownMonitor) {
 4             // Prepare this context for refreshing.
 5             // 记录开始wac开始初始化的时间,设置激活标记,servlet的相关param设置到env(之前做过1次),校验env中必须的props
 6             prepareRefresh();
 7 
 8             // Tell the subclass to refresh the internal bean factory.
 9             // 将旧的BF里的bean删掉,新建1个BF,设置部分属性,加载XML配置文件
10             ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();
11 
12             // Prepare the bean factory for use in this context.
13             // 1.设置BF解析bean配置需要用到的一些对象比如env. 2.注册一些BeanPostProcessor比如ApplicationContextAwareProcessor去设置Aware需要的对象
14             // 3.忽略一些特定class注入的对象,设置一些特定class注入的对象为指定值
15             // 4.将一些env中的properties map当做bean注册到BF中
16             prepareBeanFactory(beanFactory);
17 
18             try {
19                 // Allows post-processing of the bean factory in context subclasses.
20                 // 1.设置一个BeanPostProcess为ServletContextAware的实现类注入servlet相关对象
21                 // 2.在BF中增加requetsScope等Scope
22                 // 3.把servletContext,Config,ServletInitParams,ServletAttribute当做Bean注册到BF中
23                 postProcessBeanFactory(beanFactory);
24 
25                 // Invoke factory processors registered as beans in the context.
26                 // 初始化并调用BeanFactoryPostProcessor
27                 invokeBeanFactoryPostProcessors(beanFactory);
28 
29                 // Register bean processors that intercept bean creation.
30                 // 注册BeanPostProcessors并注册到BF中去
31                 registerBeanPostProcessors(beanFactory);
32 
33                 // Initialize message source for this context.
34                 initMessageSource();
35 
36                 // Initialize event multicaster for this context.
37                 initApplicationEventMulticaster();
38 
39                 // Initialize other special beans in specific context subclasses.
40                 onRefresh();
41 
42                 // Check for listener beans and register them.
43                 registerListeners();
44 
45                 // Instantiate all remaining (non-lazy-init) singletons.
46                 finishBeanFactoryInitialization(beanFactory);
47 
48                 // Last step: publish corresponding event.
49                 finishRefresh();
50             } catch (BeansException ex) {
51                 logger.warn("Exception encountered during context initialization - cancelling refresh attempt", ex);
52 
53                 // Destroy already created singletons to avoid dangling resources.
54                 destroyBeans();
55 
56                 // Reset 'active' flag.
57                 cancelRefresh(ex);
58 
59                 // Propagate exception to caller.
60                 throw ex;
61             }
62         }
63     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
其中有1个模板方法
postProcessBeanFactory(beanFactory);
这个方法允许AbstractApplicationContext的子类覆盖它并实现对BF的定制(这个时候bean的defination路径已经指定了,但是bean还没加载).
AbstractRefreshableWebApplicationContext覆盖了这个方法
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     /**
 2      * Register request/session scopes, a {@link ServletContextAwareProcessor}, etc.
 3      * 1.设置一个BeanPostProcess为ServletContextAware的实现类注入servlet相关对象
 4      * 2.在BF中增加requetsScope等Scope
 5      * 3.把servletContext,Config,ServletInitParams,ServletAttribute当做Bean注册到BF中
 6      *
 7      */
 8     @Override
 9     protected void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) {
10         // 设置一个BeanPostProcess为ServletContextAware的实现类注入servlet相关对象
11         beanFactory.addBeanPostProcessor(new ServletContextAwareProcessor(this.servletContext, this.servletConfig));
12         beanFactory.ignoreDependencyInterface(ServletContextAware.class);
13         beanFactory.ignoreDependencyInterface(ServletConfigAware.class);
14 
15         // 在BF中增加requetsScope等Scope
16         WebApplicationContextUtils.registerWebApplicationScopes(beanFactory, this.servletContext);
17         // 把servletContext,Config,ServletInitParams,ServletAttribute当做Bean注册到BF中
18         WebApplicationContextUtils.registerEnvironmentBeans(beanFactory, this.servletContext, this.servletConfig);
19     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
其中有一步
WebApplicationContextUtils.registerWebApplicationScopes(beanFactory, this.servletContext);
这里设置了一些特殊的bean的scope,比如request,session,globalSession,application.(当然这个不是我这篇文章的主题.)
同时设置了一些特殊的autowired bean
beanFactory.registerResolvableDependency(ServletRequest.class, new RequestObjectFactory());
beanFactory.registerResolvableDependency(ServletResponse.class, new ResponseObjectFactory());
beanFactory.registerResolvableDependency(HttpSession.class, new SessionObjectFactory());
beanFactory.registerResolvableDependency(WebRequest.class, new WebRequestObjectFactory());
ServletRequest的实现类(比如HttpServletRequest)被指定使用RequestObjectFactory注入.
## RequestObjectFactory
RequestObjectFactory就是1个ObjectFactory就是前面ObjectFactoryDelegatingInvocationHandler里的ObjectFactory.所以在成员域request对象上调用方法其实就是通过RequestObjectFactory获取对象再调用方法.
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     /**
 2      * Factory that exposes the current request object on demand.
 3      */
 4     @SuppressWarnings("serial")
 5     private static class RequestObjectFactory implements ObjectFactory<ServletRequest>, Serializable {
 6 
 7         @Override
 8         public ServletRequest getObject() {
 9             return currentRequestAttributes().getRequest();
10         }
11 
12         @Override
13         public String toString() {
14             return "Current HttpServletRequest";
15         }
16     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     /**
 2      * Return the current RequestAttributes instance as ServletRequestAttributes.
 3      *
 4      * @see RequestContextHolder#currentRequestAttributes()
 5      */
 6     private static ServletRequestAttributes currentRequestAttributes() {
 7         RequestAttributes requestAttr = RequestContextHolder.currentRequestAttributes();
 8         if (!(requestAttr instanceof ServletRequestAttributes)) {
 9             throw new IllegalStateException("Current request is not a servlet request");
10         }
11         return (ServletRequestAttributes) requestAttr;
12     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
RequestObjectFactory的getObject方法很简单,就是调用静态方法
RequestContextHolder.currentRequestAttributes().getRequest()
## RequestContextHolder
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     public static RequestAttributes currentRequestAttributes() throws IllegalStateException {
 2         RequestAttributes attributes = getRequestAttributes();
 3         if (attributes == null) {
 4             if (jsfPresent) {
 5                 attributes = FacesRequestAttributesFactory.getFacesRequestAttributes();
 6             }
 7             if (attributes == null) {
 8                 throw new IllegalStateException("No thread-bound request found: " +
 9                         "Are you referring to request attributes outside of an actual web request, " +
10                         "or processing a request outside of the originally receiving thread? " +
11                         "If you are actually operating within a web request and still receive this message, " +
12                         "your code is probably running outside of DispatcherServlet/DispatcherPortlet: " +
13                         "In this case, use RequestContextListener or RequestContextFilter to expose the current request.");
14             }
15         }
16         return attributes;
17     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     /**
 2      * Return the RequestAttributes currently bound to the thread.
 3      * @return the RequestAttributes currently bound to the thread,
 4      * or {@code null} if none bound
 5      */
 6     public static RequestAttributes getRequestAttributes() {
 7         RequestAttributes attributes = requestAttributesHolder.get();
 8         if (attributes == null) {
 9             attributes = inheritableRequestAttributesHolder.get();
10         }
11         return attributes;
12     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
1     private static final ThreadLocal<RequestAttributes> requestAttributesHolder =
2             new NamedThreadLocal<RequestAttributes>("Request attributes");
3 
4     private static final ThreadLocal<RequestAttributes> inheritableRequestAttributesHolder =
5             new NamedInheritableThreadLocal<RequestAttributes>("Request context");
```
上面是一些关键方法
所以最终其实request是从threadlocal中取...
## FrameworkServlet
 那么request是什么时候设置到threadlocal中去的呢?
是在Springmvc的dispatcherServlet的父类FrameworkServlet里操作的.
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     /**
 2      * Delegate GET requests to processRequest/doService.
 3      * <p>Will also be invoked by HttpServlet's default implementation of {@code doHead},
 4      * with a {@code NoBodyResponse} that just captures the content length.
 5      * @see #doService
 6      * @see #doHead
 7      */
 8     @Override
 9     protected final void doGet(HttpServletRequest request, HttpServletResponse response)
10             throws ServletException, IOException {
11 
12         processRequest(request, response);
13     }
14 
15     /**
16      * Delegate POST requests to {@link #processRequest}.
17      * @see #doService
18      */
19     @Override
20     protected final void doPost(HttpServletRequest request, HttpServletResponse response)
21             throws ServletException, IOException {
22 
23         processRequest(request, response);
24     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
不管你是doGet还是doPost还是doXXX方法都是委托processRequest方法去做的.
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 /**
 2      * Process this request, publishing an event regardless of the outcome.
 3      * <p>The actual event handling is performed by the abstract
 4      * {@link #doService} template method.
 5      */
 6     protected final void processRequest(HttpServletRequest request, HttpServletResponse response)
 7             throws ServletException, IOException {
 8 
 9         long startTime = System.currentTimeMillis();
10         Throwable failureCause = null;
11 
12         LocaleContext previousLocaleContext = LocaleContextHolder.getLocaleContext();
13         LocaleContext localeContext = buildLocaleContext(request);
14 
15         RequestAttributes previousAttributes = RequestContextHolder.getRequestAttributes();
16         ServletRequestAttributes requestAttributes = buildRequestAttributes(request, response, previousAttributes);
17 
18         WebAsyncManager asyncManager = WebAsyncUtils.getAsyncManager(request);
19         asyncManager.registerCallableInterceptor(FrameworkServlet.class.getName(), new RequestBindingInterceptor());
20 
21         initContextHolders(request, localeContext, requestAttributes);
22 
23         try {
24             doService(request, response);
25         }
26         catch (ServletException ex) {
27             failureCause = ex;
28             throw ex;
29         }
30         catch (IOException ex) {
31             failureCause = ex;
32             throw ex;
33         }
34         catch (Throwable ex) {
35             failureCause = ex;
36             throw new NestedServletException("Request processing failed", ex);
37         }
38 
39         finally {
40             resetContextHolders(request, previousLocaleContext, previousAttributes);
41             if (requestAttributes != null) {
42                 requestAttributes.requestCompleted();
43             }
44 
45             if (logger.isDebugEnabled()) {
46                 if (failureCause != null) {
47                     this.logger.debug("Could not complete request", failureCause);
48                 }
49                 else {
50                     if (asyncManager.isConcurrentHandlingStarted()) {
51                         logger.debug("Leaving response open for concurrent processing");
52                     }
53                     else {
54                         this.logger.debug("Successfully completed request");
55                     }
56                 }
57             }
58 
59             publishRequestHandledEvent(request, response, startTime, failureCause);
60         }
61     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
其中调用了
initContextHolders(request, localeContext, requestAttributes);
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     private void initContextHolders(
 2             HttpServletRequest request, LocaleContext localeContext, RequestAttributes requestAttributes) {
 3 
 4         if (localeContext != null) {
 5             LocaleContextHolder.setLocaleContext(localeContext, this.threadContextInheritable);
 6         }
 7         if (requestAttributes != null) {
 8             RequestContextHolder.setRequestAttributes(requestAttributes, this.threadContextInheritable);
 9         }
10         if (logger.isTraceEnabled()) {
11             logger.trace("Bound request context to thread: " + request);
12         }
13     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
就是在这里设置到RequestContextHolder的threadlocal中去的...
# 小结
1.在controller中注入的request是jdk动态代理对象,ObjectFactoryDelegatingInvocationHandler的实例.当我们调用成员域request的方法的时候其实是调用了objectFactory的getObject()对象的相关方法.这里的objectFactory是RequestObjectFactory.
2.RequestObjectFactory的getObject其实是从RequestContextHolder的threadlocal中去取值的.
3.请求刚进入springmvc的dispatcherServlet的时候会把request相关对象设置到RequestContextHolder的threadlocal中去.
