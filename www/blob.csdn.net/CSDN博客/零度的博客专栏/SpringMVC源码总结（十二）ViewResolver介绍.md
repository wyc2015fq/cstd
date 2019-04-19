# SpringMVC源码总结（十二）ViewResolver介绍 - 零度的博客专栏 - CSDN博客
2017年02月27日 12:41:01[零度anngle](https://me.csdn.net/zmx729618)阅读数：262标签：[springmvc](https://so.csdn.net/so/search/s.do?q=springmvc&t=blog)
个人分类：[SpringMVC](https://blog.csdn.net/zmx729618/article/category/6248665)
首先我们先看看ModelAndView中重要的View接口。 
View接口：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- String getContentType();  
- 
- /**
-      * Render the view given the specified model.
-      * <p>The first step will be preparing the request: In the JSP case,
-      * this would mean setting model objects as request attributes.
-      * The second step will be the actual rendering of the view,
-      * for example including the JSP via a RequestDispatcher.
-      * @param model Map with name Strings as keys and corresponding model
-      * objects as values (Map can also be {@code null} in case of empty model)
-      * @param request current HTTP request
-      * @param response HTTP response we are building
-      * @throws Exception if rendering failed
-      */
- //上面说的很清楚，对于jsp来说，第一步就是将model作为request的attributes；第二步才开始渲染view
- void render(Map<String, ?> model, HttpServletRequest request, HttpServletResponse response) throws Exception;  
![](http://static.blog.csdn.net/images/save_snippets.png)
再看下ViewResolver接口：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- View resolveViewName(String viewName, Locale locale) throws Exception;  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
它是对给定的viewName找到对应的View对象，然后使用该view对象的render方法将本身的内容写到response中。 
然后就看下，当我们的处理函数返回一个viewName时，SpringMVC是如何渲染的。
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
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
- //这里是我们的关注重点，就是进行视图渲染的过程
-             processDispatchResult(processedRequest, response, mappedHandler, mv, dispatchException);  
-         }  
- catch (Exception ex) {  
-             triggerAfterCompletion(processedRequest, response, mappedHandler, ex);  
-         }  
- catch (Error err) {  
-             triggerAfterCompletionWithError(processedRequest, response, mappedHandler, err);  
-         }  
![](http://static.blog.csdn.net/images/save_snippets.png)
继续看下processDispatchResult是如何来渲染的
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatevoid processDispatchResult(HttpServletRequest request, HttpServletResponse response,  
-             HandlerExecutionChain mappedHandler, ModelAndView mv, Exception exception) throws Exception {  
- 
- boolean errorView = false;  
- 
- if (exception != null) {  
- if (exception instanceof ModelAndViewDefiningException) {  
-                 logger.debug("ModelAndViewDefiningException encountered", exception);  
-                 mv = ((ModelAndViewDefiningException) exception).getModelAndView();  
-             }  
- else {  
-                 Object handler = (mappedHandler != null ? mappedHandler.getHandler() : null);  
-                 mv = processHandlerException(request, response, handler, exception);  
-                 errorView = (mv != null);  
-             }  
-         }  
- 
- // Did the handler return a view to render?
- //这里是我们关注的重点
- if (mv != null && !mv.wasCleared()) {  
-             render(mv, request, response);  
- if (errorView) {  
-                 WebUtils.clearErrorRequestAttributes(request);  
-             }  
-         }  
- else {  
- if (logger.isDebugEnabled()) {  
-                 logger.debug("Null ModelAndView returned to DispatcherServlet with name '" + getServletName() +  
- "': assuming HandlerAdapter completed request handling");  
-             }  
-         }  
- 
- if (WebAsyncUtils.getAsyncManager(request).isConcurrentHandlingStarted()) {  
- // Concurrent handling started during a forward
- return;  
-         }  
- 
- if (mappedHandler != null) {  
-             mappedHandler.triggerAfterCompletion(request, response, null);  
-         }  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedvoid render(ModelAndView mv, HttpServletRequest request, HttpServletResponse response) throws Exception {  
- // Determine locale for request and apply it to the response.
-         Locale locale = this.localeResolver.resolveLocale(request);  
-         response.setLocale(locale);  
- 
-         View view;  
- if (mv.isReference()) {  
- // We need to resolve the view name.
-             view = resolveViewName(mv.getViewName(), mv.getModelInternal(), locale, request);  
- if (view == null) {  
- thrownew ServletException("Could not resolve view with name '" + mv.getViewName() +  
- "' in servlet with name '" + getServletName() + "'");  
-             }  
-         }  
- else {  
- // No need to lookup: the ModelAndView object contains the actual View object.
-             view = mv.getView();  
- if (view == null) {  
- thrownew ServletException("ModelAndView [" + mv + "] neither contains a view name nor a " +  
- "View object in servlet with name '" + getServletName() + "'");  
-             }  
-         }  
- 
- // Delegate to the View object for rendering.
- if (logger.isDebugEnabled()) {  
-             logger.debug("Rendering view [" + view + "] in DispatcherServlet with name '" + getServletName() + "'");  
-         }  
- try {  
-             view.render(mv.getModelInternal(), request, response);  
-         }  
- catch (Exception ex) {  
- if (logger.isDebugEnabled()) {  
-                 logger.debug("Error rendering view [" + view + "] in DispatcherServlet with name '" +  
-                         getServletName() + "'", ex);  
-             }  
- throw ex;  
-         }  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这里可以看到整体的处理流程。首先判断view是不是一个视图的名称，若是需要找到这个视图名称对应的View对象，然后便是调用view对象的render方法，渲染到response中。 
由于我们的处理函数经常仅仅是返回一个view名称，所以我们重点要看看它是如何根据视图名称来找到对应的View对象的，即resolveViewName方法内容。其实上文已经说明了View接口和ViewResolver 接口，ViewResolver 接口就是根据view名称来找到对应的View对象的，所以看下面就会很清晰明白
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected View resolveViewName(String viewName, Map<String, Object> model, Locale locale,  
-             HttpServletRequest request) throws Exception {  
- 
- for (ViewResolver viewResolver : this.viewResolvers) {  
-             View view = viewResolver.resolveViewName(viewName, locale);  
- if (view != null) {  
- return view;  
-             }  
-         }  
- returnnull;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这里就是对DispatcherServlet的private List<ViewResolver> viewResolvers属性进行遍历找到一个能够获取View对象的ViewResolver，并返回这个view对象。 
至此整个流程便走通了，接下来就是要看看有哪些ViewResolver以及它们的注册来源是什么？ 
常用的ViewResolver有：FreeMarkerViewResolver、InternalResourceViewResolver、VelocityViewResolver等。 
接下来就是如何来注册这些ViewResolver：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedvoid initStrategies(ApplicationContext context) {  
-         initMultipartResolver(context);  
-         initLocaleResolver(context);  
-         initThemeResolver(context);  
-         initHandlerMappings(context);  
-         initHandlerAdapters(context);  
-         initHandlerExceptionResolvers(context);  
-         initRequestToViewNameTranslator(context);  
- //我们关注的重点
-         initViewResolvers(context);  
-         initFlashMapManager(context);  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
还是在DispatcherServlet的初始化策略中，调用了initViewResolvers，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatevoid initViewResolvers(ApplicationContext context) {  
- this.viewResolvers = null;  
- 
- if (this.detectAllViewResolvers) {  
- // Find all ViewResolvers in the ApplicationContext, including ancestor contexts.
-             Map<String, ViewResolver> matchingBeans =  
-                     BeanFactoryUtils.beansOfTypeIncludingAncestors(context, ViewResolver.class, true, false);  
- if (!matchingBeans.isEmpty()) {  
- this.viewResolvers = new ArrayList<ViewResolver>(matchingBeans.values());  
- // We keep ViewResolvers in sorted order.
-                 OrderComparator.sort(this.viewResolvers);  
-             }  
-         }  
- else {  
- try {  
-                 ViewResolver vr = context.getBean(VIEW_RESOLVER_BEAN_NAME, ViewResolver.class);  
- this.viewResolvers = Collections.singletonList(vr);  
-             }  
- catch (NoSuchBeanDefinitionException ex) {  
- // Ignore, we'll add a default ViewResolver later.
-             }  
-         }  
- 
- // Ensure we have at least one ViewResolver, by registering
- // a default ViewResolver if no other resolvers are found.
- if (this.viewResolvers == null) {  
- this.viewResolvers = getDefaultStrategies(context, ViewResolver.class);  
- if (logger.isDebugEnabled()) {  
-                 logger.debug("No ViewResolvers found in servlet '" + getServletName() + "': using default");  
-             }  
-         }  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这和HandleMapping和HandlerAdapter的初始化过程基本类似。this.detectAllViewResolvers是DispatcherServlet的一个boolean属性，可以在web.xml文件中修改这个值，默认是true。
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /** Detect all ViewResolvers or just expect "viewResolver" bean? */
- privateboolean detectAllViewResolvers = true;  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
当detectAllViewResolvers为true,意味着就会获取从xml文件中解析出来的ViewResolver。如果为false,则直接去找bean name为"viewResolver"并且是ViewResolver类型的作为DispatcherServlet的ViewResolver。 
当上述两种情况都没有找到，则会启用默认的ViewResolver，在this.viewResolvers = getDefaultStrategies(context, ViewResolver.class)中，这个过程已经多次说过，可以见本系列第一篇HandleMapping的来源。它就是依据DispatcherServlet.properties文件中所配置的ViewResolver，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- org.springframework.web.servlet.ViewResolver=org.springframework.web.servlet.view.InternalResourceViewResolver  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
也就是默认采用的是InternalResourceViewResolver。 
再说说在xml文件中配置ViewResolver的情况，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <bean class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">  
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
![](http://static.blog.csdn.net/images/save_snippets.png)
这里是以FreeMarkerViewResolver为例来说明，它的配置内容还是需要有待继续研究。这里只是粗略的说下它的继承情况。 
FreeMarkerViewResolver继承AbstractTemplateViewResolver继承UrlBasedViewResolver继承AbstractCachingViewResolver。 
首先是抽象类AbstractCachingViewResolver：它加入了缓存功能，它有几个重要的属性。
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /** Default maximum number of entries for the view cache: 1024 */
- publicstaticfinalint DEFAULT_CACHE_LIMIT = 1024;  
- 
- /** The maximum number of entries in the cache */
- privatevolatileint cacheLimit = DEFAULT_CACHE_LIMIT;  
- 
- 
- 
- /** Fast access cache for Views, returning already cached instances without a global lock */
- privatefinal Map<Object, View> viewAccessCache = new ConcurrentHashMap<Object, View>(DEFAULT_CACHE_LIMIT);  
- 
- /** Map from view key to View instance, synchronized for View creation */
- @SuppressWarnings("serial")  
- privatefinal Map<Object, View> viewCreationCache =  
- new LinkedHashMap<Object, View>(DEFAULT_CACHE_LIMIT, 0.75f, true) {  
- @Override
- protectedboolean removeEldestEntry(Map.Entry<Object, View> eldest) {  
- if (size() > getCacheLimit()) {  
-                         viewAccessCache.remove(eldest.getKey());  
- returntrue;  
-                     }  
- else {  
- returnfalse;  
-                     }  
-                 }  
-             };  
![](http://static.blog.csdn.net/images/save_snippets.png)
属性一：cacheLimit 最大的缓存数量，默认为1024。 
属性二：viewAccessCache 是ConcurrentHashMap类型的，适合高并发。 
属性三：viewCreationCache是LinkedHashMap类型的 
我们再来看下，由view名称来解析到view视图对象的具体过程：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- public View resolveViewName(String viewName, Locale locale) throws Exception {  
- //这里进行了是否进行缓存的判断，即cacheLimit是否大于0
- if (!isCache()) {  
- //不进行缓存，始终每次都创建
- return createView(viewName, locale);  
-         }  
- else {  
- //viewAccessCache viewCreationCache两者的key
-             Object cacheKey = getCacheKey(viewName, locale);  
-             View view = this.viewAccessCache.get(cacheKey);  
- if (view == null) {  
- synchronized (this.viewCreationCache) {  
-                     view = this.viewCreationCache.get(cacheKey);  
- if (view == null) {  
- // Ask the subclass to create the View object.
-                         view = createView(viewName, locale);  
- if (view == null && this.cacheUnresolved) {  
-                             view = UNRESOLVED_VIEW;  
-                         }  
- if (view != null) {  
- this.viewAccessCache.put(cacheKey, view);  
- this.viewCreationCache.put(cacheKey, view);  
- if (logger.isTraceEnabled()) {  
-                                 logger.trace("Cached view [" + cacheKey + "]");  
-                             }  
-                         }  
-                     }  
-                 }  
-             }  
- return (view != UNRESOLVED_VIEW ? view : null);  
-         }  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
对于Object cacheKey = getCacheKey(viewName, locale);默认为viewName + "_" + locale; 
但是可以被子类覆盖，子类UrlBasedViewResolver覆盖了它，变成只有viewName。 
先从viewAccessCache中看能否找到已缓存的view视图，若能找到则返回。若未找到则加上同步锁synchronized (this.viewCreationCache)，进入这个方法之后，最关键的是仍需要进行一次判断view = this.viewCreationCache.get(cacheKey)，看看是否已经创建过了，并不是viewAccessCache和viewCreationCache他们所缓存的内容不一样而是如果没有这个判断，则会有多线程问题。 
如线程1和线程2同时要解析相同的view名称，他们都来到同步锁synchronized (this.viewCreationCache)之前，线程2先拿到锁，线程1等待，线程2创建好view视图后，加入viewCreationCache和viewAccessCache，并释放锁。此时线程1获得锁，进入同步锁synchronized (this.viewCreationCache)内部，若不进行判断，则线程1又会去创建一次view视图。所以view = this.viewCreationCache.get(cacheKey)并判断view是否为null这一步骤是十分有用的。 
创建View视图的任务就交给了子类来实现。resolveViewName这个方法基本上就分析完了，应该还会想到，它的那个cacheLimit限制好像还没发挥出作用。 
继续回看
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatefinal Map<Object, View> viewAccessCache = new ConcurrentHashMap<Object, View>(DEFAULT_CACHE_LIMIT);  
- 
- privatefinal Map<Object, View> viewCreationCache =  
- new LinkedHashMap<Object, View>(DEFAULT_CACHE_LIMIT, 0.75f, true) {  
- @Override
- protectedboolean removeEldestEntry(Map.Entry<Object, View> eldest) {  
- if (size() > getCacheLimit()) {  
-                         viewAccessCache.remove(eldest.getKey());  
- returntrue;  
-                     }  
- else {  
- returnfalse;  
-                     }  
-                 }  
-             };  
![](http://static.blog.csdn.net/images/save_snippets.png)
viewCreationCache 的类型是LinkedHashMap，但是它复写了protected boolean removeEldestEntry(Map.Entry<Object, View> eldest)方法，当该方法返回true时，LinkedHashMap则会删除最老的key。在这里我们可以看到，当viewCreationCache 的所存的View数量达到cacheLimit时，就会删除最老的那个key和value，同时也会使viewAccessCache删除这个key和value。 
viewAccessCache主要是用来高并发的访问，viewCreationCache 则是用来统计最老的key。他们所存储的view都是一样的。
