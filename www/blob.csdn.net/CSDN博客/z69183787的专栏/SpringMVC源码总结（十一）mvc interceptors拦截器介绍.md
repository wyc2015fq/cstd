# SpringMVC源码总结（十一）mvc:interceptors拦截器介绍 - z69183787的专栏 - CSDN博客
2016年10月14日 17:29:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2802
个人分类：[SpringMvc-原理分析](https://blog.csdn.net/z69183787/article/category/6456963)
本文章针对mvc:interceptors标签进行介绍，它的注册过程以及在访问时的拦截过程。 
首先说下接口HandlerInterceptor，它有如下三个方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)  
- throws Exception;  
- 
- void postHandle(  
-             HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView)  
- throws Exception;  
- 
- void afterCompletion(  
-             HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex)  
- throws Exception;  
正常情况下，对于preHandle就是在在处理函数之前先执行，然后再执行处理函数，接着执行postHandle，最后再执行afterCompletion。afterCompletion无论是否出错是肯定要执行的，而postHandle则不是，不一定会执行。之后看源代码就知道他们的执行情况。 
AsyncHandlerInterceptor接口则增添了afterConcurrentHandlingStarted方法，对于此还未研究，先不讨论。 
HandlerInterceptorAdapter则默认实现了上述的接口，所以当我们仅仅要实现某个方法时，只需继承HandlerInterceptorAdapter，然后覆盖相应的方法。 
然后我们就写一个类继承HandlerInterceptorAdapter来进行实验：LoginInterceptor如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicboolean preHandle(HttpServletRequest request,  
-             HttpServletResponse response, Object handler) throws Exception {  
-         System.out.println("preHandle");  
- returnsuper.preHandle(request, response, handler);  
-     }  
- 
- @Override
- publicvoid postHandle(HttpServletRequest request,  
-             HttpServletResponse response, Object handler,  
-             ModelAndView modelAndView) throws Exception {  
-         System.out.println("postHandle");  
- super.postHandle(request, response, handler, modelAndView);  
-     }  
- 
- @Override
- publicvoid afterCompletion(HttpServletRequest request,  
-             HttpServletResponse response, Object handler, Exception ex)  
- throws Exception {  
-         System.out.println("afterCompletion");  
- super.afterCompletion(request, response, handler, ex);  
-     }  
- 
- @Override
- publicvoid afterConcurrentHandlingStarted(HttpServletRequest request,  
-             HttpServletResponse response, Object handler) throws Exception {  
-         System.out.println("afterConcurrentHandlingStarted");  
- super.afterConcurrentHandlingStarted(request, response, handler);  
-     }  
没有做具体的内容，仅仅是打印出一些信息，方便查看执行顺序。 
该接口的基本内容说完了，然后就看下它的配置说明：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:interceptors path-matcher="xxx">  
-         <mvc:interceptor>  
-             <mvc:mapping path="xxx"/>  
-             <mvc:exclude-mapping path="xxxx"/>  
-             <bean class="xxxx"></bean>  
-         </mvc:interceptor>  
-         <bean class="com.lg.mvc.interceptor.LoginInterceptor" />  
-     </mvc:interceptors>  
其实在mvc:interceptors标签中，有两种类型的配置，一种直接配置一个bean（bean和ref归为一类）,另一种还要配置上拦截的路径和排除的路径。直接配置的bean那就代表对所有的请求进行拦截，而对于mvc:interceptor则代表有着更精细的控制。 
而mvc:interceptors的属性path-matcher则表示配置一个自定义的PathMatcher，它主要用来处理路径的匹配规则，默认采用的PathMatcher为AntPathMatcher，具有ant风格的路径规则，如?表示任何单字符，*表示0个或多个字符，**表示0个或多个目录。 
对于本工程来说具体的配置如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:interceptors>  
-         <bean class="com.lg.mvc.interceptor.LoginInterceptor" />  
-     </mvc:interceptors>  
然后就进行源代码分析： 
如何来处理xml文件中所配置的这些HandlerInterceptor的呢？ 
对于mvc:interceptors的解析同样需要我们去看BeanDefinitionParser的实现类，最终会找到InterceptorsBeanDefinitionParser：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- public BeanDefinition parse(Element element, ParserContext parserContext) {  
-         CompositeComponentDefinition compDefinition = new CompositeComponentDefinition(element.getTagName(), parserContext.extractSource(element));  
-         parserContext.pushContainingComponent(compDefinition);  
- //判断是否自定义了PathMatcher
-         RuntimeBeanReference pathMatcherRef = null;  
- if (element.hasAttribute("path-matcher")) {  
-             pathMatcherRef = new RuntimeBeanReference(element.getAttribute("path-matcher"));  
-         }  
- //获取所有的interceptor，在这里我们可以看到所有的interceptor最终都会构建成一个
- //MappedInterceptor
-         List<Element> interceptors = DomUtils.getChildElementsByTagName(element, "bean", "ref", "interceptor");  
- for (Element interceptor : interceptors) {  
-             RootBeanDefinition mappedInterceptorDef = new RootBeanDefinition(MappedInterceptor.class);  
-             mappedInterceptorDef.setSource(parserContext.extractSource(interceptor));  
-             mappedInterceptorDef.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
- 
-             ManagedList<String> includePatterns = null;  
-             ManagedList<String> excludePatterns = null;  
-             Object interceptorBean;  
- if ("interceptor".equals(interceptor.getLocalName())) {  
-                 includePatterns = getIncludePatterns(interceptor, "mapping");  
-                 excludePatterns = getIncludePatterns(interceptor, "exclude-mapping");  
-                 Element beanElem = DomUtils.getChildElementsByTagName(interceptor, "bean", "ref").get(0);  
-                 interceptorBean = parserContext.getDelegate().parsePropertySubElement(beanElem, null);  
-             }  
- else {  
-                 interceptorBean = parserContext.getDelegate().parsePropertySubElement(interceptor, null);  
-             }  
-             mappedInterceptorDef.getConstructorArgumentValues().addIndexedArgumentValue(0, includePatterns);  
-             mappedInterceptorDef.getConstructorArgumentValues().addIndexedArgumentValue(1, excludePatterns);  
-             mappedInterceptorDef.getConstructorArgumentValues().addIndexedArgumentValue(2, interceptorBean);  
- 
- if (pathMatcherRef != null) {  
-                 mappedInterceptorDef.getPropertyValues().add("pathMatcher", pathMatcherRef);  
-             }  
- 
-             String beanName = parserContext.getReaderContext().registerWithGeneratedName(mappedInterceptorDef);  
-             parserContext.registerComponent(new BeanComponentDefinition(mappedInterceptorDef, beanName));  
-         }  
- 
-         parserContext.popAndRegisterContainingComponent();  
- returnnull;  
-     }  
这里就引出来MappedInterceptor的结构类型：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatefinal String[] includePatterns;  
- 
- privatefinal String[] excludePatterns;  
- 
- privatefinal HandlerInterceptor interceptor;  
- 
- private PathMatcher pathMatcher;  
到这里就很明白了，虽然在mvc:interceptors标签中，配置interceptor形式不一样，但是最终都将以MappedInterceptor形式存储，同时来看下MappedInterceptor的match的方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicboolean matches(String lookupPath, PathMatcher pathMatcher) {  
-         PathMatcher pathMatcherToUse = (this.pathMatcher != null) ? this.pathMatcher : pathMatcher;  
- if (this.excludePatterns != null) {  
- for (String pattern : this.excludePatterns) {  
- if (pathMatcherToUse.match(pattern, lookupPath)) {  
- returnfalse;  
-                 }  
-             }  
-         }  
- if (this.includePatterns == null) {  
- returntrue;  
-         }  
- else {  
- for (String pattern : this.includePatterns) {  
- if (pathMatcherToUse.match(pattern, lookupPath)) {  
- returntrue;  
-                 }  
-             }  
- returnfalse;  
-         }  
-     }  
这里便是PathMatcher对于excludePatterns、includePatterns 的使用规则，同时表明本身的PathMatcher若为空，则使用外部传来的PathMatcher。 
至此解析mvc:interceptors标签的过程就完成了。它们最终会注册到ApplicationContext的上下文环境中，等待被使用。 
谁会是他们的使用者呢？我们慢慢来看： 
对于每一个请求，HandlerMapping都会找到对应的handler，并最终封装成一个HandlerExecutionChain，这个HandlerExecutionChain包含有handler和它对应的interceptors，HandlerExecutionChain如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatefinal Object handler;  
- 
- private HandlerInterceptor[] interceptors;  
- 
- private List<HandlerInterceptor> interceptorList;  
- 
- privateint interceptorIndex = -1;  
既然是由HandlerMapping来产生的HandlerExecutionChain，则它需要为每一个它所管辖的handler来装配HandlerInterceptor。所以HandlerMapping必然是mvc:interceptors标签内容的使用者。 
使用者：AbstractHandlerMapping，它的属性有：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private PathMatcher pathMatcher = new AntPathMatcher();  
- 
- privatefinal List<Object> interceptors = new ArrayList<Object>();  
- 
- privatefinal List<HandlerInterceptor> adaptedInterceptors = new ArrayList<HandlerInterceptor>();  
- 
- privatefinal List<MappedInterceptor> mappedInterceptors = new ArrayList<MappedInterceptor>();  
这里便可以看到，它所使用的默认的PathMatcher为AntPathMatcher。接下来我们看下AbstractHandlerMapping的初始化方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedvoid initApplicationContext() throws BeansException {  
-         extendInterceptors(this.interceptors);  
-         detectMappedInterceptors(this.mappedInterceptors);  
-         initInterceptors();  
-     }  
detectMappedInterceptors探测ApplicationContext中已经解析过的MappedInterceptor，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedvoid detectMappedInterceptors(List<MappedInterceptor> mappedInterceptors) {  
-         mappedInterceptors.addAll(  
-                 BeanFactoryUtils.beansOfTypeIncludingAncestors(  
-                         getApplicationContext(), MappedInterceptor.class, true, false).values());  
-     }  
全部存放到AbstractHandlerMapping的mappedInterceptors属性上。 
然后我们继续看看在请求到来时的具体拦截过程：
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
- 
- // Determine handler for the current request.
-                 mappedHandler = getHandler(processedRequest);  
- //略
- }  
对于每个请求先找到对应的HandlerMapping，然后由这个handlerMapping来找到对应请求的handler，然后由handlerMapping自身的interceptor和这个handler来构建一个HandlerExecutionChain。代码如下：
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
这里便是找到一个合适的HandlerMapping，继续看下hm.getHandler(request)这个方法。
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
这里便是找到对应请求的handler。getHandlerExecutionChain(handler, request)这里便是构建HandlerExecutionChain的地方：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected HandlerExecutionChain getHandlerExecutionChain(Object handler, HttpServletRequest request) {  
-         HandlerExecutionChain chain = (handler instanceof HandlerExecutionChain ?  
-                 (HandlerExecutionChain) handler : new HandlerExecutionChain(handler));  
-         chain.addInterceptors(getAdaptedInterceptors());  
- 
-         String lookupPath = this.urlPathHelper.getLookupPathForRequest(request);  
- for (MappedInterceptor mappedInterceptor : this.mappedInterceptors) {  
- if (mappedInterceptor.matches(lookupPath, this.pathMatcher)) {  
-                 chain.addInterceptor(mappedInterceptor.getInterceptor());  
-             }  
-         }  
- 
- return chain;  
-     }  
对于我们关注的重点为它会遍历AbstarctHandlerMapping的mappedInterceptors属性，然后使用默认的pathMatcher，即AntPathMatcher来判断当前的请求是否符合拦截条件，若符合则将mappedInterceptor放进HandlerExecutionChain 中。 
至此一个HandlerExecutionChain便构建好了，包含一个handler和这个handler对应的interceptor。然后看下interceptor的执行过程：
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
- 
- // Determine handler for the current request.
-                 mappedHandler = getHandler(processedRequest);  
- if (mappedHandler == null || mappedHandler.getHandler() == null) {  
-                     noHandlerFound(processedRequest, response);  
- return;  
-                 }  
- 
- // Determine handler adapter for the current request.
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
- //重点1 这里执行interceptor的preHandle方法
- if (!mappedHandler.applyPreHandle(processedRequest, response)) {  
- return;  
-                 }  
- //这里执行处理函数
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
- //重点2：这里执行interceptor的postHandle方法
-                 mappedHandler.applyPostHandle(processedRequest, response, mv);  
-             }  
- catch (Exception ex) {  
-                 dispatchException = ex;  
-             }  
- //重点3：这里执行interceptor的afterCompletion方法
-             processDispatchResult(processedRequest, response, mappedHandler, mv, dispatchException);  
-         }  
- //重点4：当出现异常时，仍然执行afterCompletion方法
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
先看重点1：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- boolean applyPreHandle(HttpServletRequest request, HttpServletResponse response) throws Exception {  
- if (getInterceptors() != null) {  
- for (int i = 0; i < getInterceptors().length; i++) {  
-                 HandlerInterceptor interceptor = getInterceptors()[i];  
- if (!interceptor.preHandle(request, response, this.handler)) {  
-                     triggerAfterCompletion(request, response, null);  
- returnfalse;  
-                 }  
- this.interceptorIndex = i;  
-             }  
-         }  
- returntrue;  
-     }  
执行preHandle方法，一旦有一个preHandle返回false,则触发triggerAfterCompletion：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- void triggerAfterCompletion(HttpServletRequest request, HttpServletResponse response, Exception ex)  
- throws Exception {  
- 
- if (getInterceptors() == null) {  
- return;  
-         }  
- for (int i = this.interceptorIndex; i >= 0; i--) {  
-             HandlerInterceptor interceptor = getInterceptors()[i];  
- try {  
-                 interceptor.afterCompletion(request, response, this.handler, ex);  
-             }  
- catch (Throwable ex2) {  
-                 logger.error("HandlerInterceptor.afterCompletion threw exception", ex2);  
-             }  
-         }  
-     }  
看下这里的for循环的条件，从interceptorIndex开始到0，逆序执行interceptor.afterCompletion。 
重点2 postHandle：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- void applyPostHandle(HttpServletRequest request, HttpServletResponse response, ModelAndView mv) throws Exception {  
- if (getInterceptors() == null) {  
- return;  
-         }  
- for (int i = getInterceptors().length - 1; i >= 0; i--) {  
-             HandlerInterceptor interceptor = getInterceptors()[i];  
-             interceptor.postHandle(request, response, this.handler, mv);  
-         }  
-     }  
这个没有什么特殊，preHandle只有参数HttpServletRequest和HttpServletResponse，而postHandle则加入了返回结果ModelAndView，我们可以对ModelAndView进行进一步的修改，此时的view（若有）还没有经过渲染。 
重点3 ：
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
这里可以看到，如果有view，则渲染完成之后，才会执行triggerAfterCompletion，同时不再拥有对ModelAndView的处理（已经完成了渲染）。所以我们就可以看到当有view时，afterCompletion和postHandle的明显区别。 
重点4：当执行过程发生异常时，也会执行interceptor的afterCompletion方法。 
这里要做下说明，对于preHandler方法是获取不到处理函数的参数值的，如果想对处理函数的参数值进行拦截处理，则要使用Spring AOP。 

