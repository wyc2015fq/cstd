# SpringMVC之HandlerMethodReturnValueHandler - 零度的博客专栏 - CSDN博客
2016年11月04日 13:58:40[零度anngle](https://me.csdn.net/zmx729618)阅读数：8529
                
HandlerMethodReturnValueHandler是RequestMappingHandlerAdapter用来处理当含有@RequestMapping的方法调度完成后，后面要进行的事情。 
首先是HandlerMethodReturnValueHandler的自定义注册： 
mvc:annotation-driven配置如下： 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:annotation-driven>  
-         <mvc:return-value-handlers>  
-             <bean class="org.springframework.web.servlet.mvc.method.annotation.ModelAndViewMethodReturnValueHandler"></bean>  
-         </mvc:return-value-handlers>  
-     </mvc:annotation-driven>  
![](http://static.blog.csdn.net/images/save_snippets.png)
在启动AnnotationDrivenBeanDefinitionParser来解析mvc:annotation-driven标签的过程中(见本系列第三篇博客)，会注册我们所配置的HandlerMethodReturnValueHandler，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- ManagedList<?> returnValueHandlers = getReturnValueHandlers(element, parserContext);  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private ManagedList<?> getReturnValueHandlers(Element element, ParserContext parserContext) {  
-         Element handlersElement = DomUtils.getChildElementByTagName(element, "return-value-handlers");  
- if (handlersElement != null) {  
- return extractBeanSubElements(handlersElement, parserContext);  
-         }  
- returnnull;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
然后将会这些自定义的HandlerMethodReturnValueHandler设置到RequestMappingHandlerAdapter的customReturnValueHandlers属性中， 
RequestMappingHandlerAdapter的两个重要属性： 
customReturnValueHandlers：存放我们自定义的HandlerMethodReturnValueHandler； 
returnValueHandlers：存放最终所有的HandlerMethodReturnValueHandler； 
如下所示：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass RequestMappingHandlerAdapter extends AbstractHandlerMethodAdapter  
- implements BeanFactoryAware, InitializingBean {  
- 
- private List<HandlerMethodArgumentResolver> customArgumentResolvers;  
- 
- private HandlerMethodArgumentResolverComposite argumentResolvers;  
- 
- private HandlerMethodArgumentResolverComposite initBinderArgumentResolvers;  
- 
- //这里这里这里这里这里这里这里这里
- private List<HandlerMethodReturnValueHandler> customReturnValueHandlers;  
- //这里这里这里这里这里这里这里这里
- private HandlerMethodReturnValueHandlerComposite returnValueHandlers;  
![](http://static.blog.csdn.net/images/save_snippets.png)
returnValueHandlers的属性类型为HandlerMethodReturnValueHandlerComposite，里面也有一个list集合，来存放所有的HandlerMethodReturnValueHandler。 
HandlerMethodReturnValueHandlerComposite结构如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass HandlerMethodReturnValueHandlerComposite implements HandlerMethodReturnValueHandler {  
- 
- protectedfinal Log logger = LogFactory.getLog(getClass());  
- 
- privatefinal List<HandlerMethodReturnValueHandler> returnValueHandlers =  
- new ArrayList<HandlerMethodReturnValueHandler>();  
- 
-     /**  
![](http://static.blog.csdn.net/images/save_snippets.png)
在RequestMappingHandlerAdapter创建出来后，会执行afterPropertiesSet()方法，在该方法中会设置所有的HandlerMethodReturnValueHandler到RequestMappingHandlerAdapter的returnValueHandlers属性中如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicvoid afterPropertiesSet() {  
- if (this.argumentResolvers == null) {  
-             List<HandlerMethodArgumentResolver> resolvers = getDefaultArgumentResolvers();  
- this.argumentResolvers = new HandlerMethodArgumentResolverComposite().addResolvers(resolvers);  
-         }  
- if (this.initBinderArgumentResolvers == null) {  
-             List<HandlerMethodArgumentResolver> resolvers = getDefaultInitBinderArgumentResolvers();  
- this.initBinderArgumentResolvers = new HandlerMethodArgumentResolverComposite().addResolvers(resolvers);  
-         }  
- if (this.returnValueHandlers == null) {  
- //获取所有的HandlerMethodReturnValueHandler
-             List<HandlerMethodReturnValueHandler> handlers = getDefaultReturnValueHandlers();  
- this.returnValueHandlers = new HandlerMethodReturnValueHandlerComposite().addHandlers(handlers);  
-         }  
-         initControllerAdviceCache();  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
getDefaultReturnValueHandlers()方法会获取默认要注册的和我们自定义的HandlerMethodReturnValueHandler，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private List<HandlerMethodReturnValueHandler> getDefaultReturnValueHandlers() {  
-         List<HandlerMethodReturnValueHandler> handlers = new ArrayList<HandlerMethodReturnValueHandler>();  
- 
- // Single-purpose return value types
-         handlers.add(new ModelAndViewMethodReturnValueHandler());  
-         handlers.add(new ModelMethodProcessor());  
-         handlers.add(new ViewMethodReturnValueHandler());  
-         handlers.add(new HttpEntityMethodProcessor(getMessageConverters(), this.contentNegotiationManager));  
-         handlers.add(new HttpHeadersReturnValueHandler());  
-         handlers.add(new CallableMethodReturnValueHandler());  
-         handlers.add(new DeferredResultMethodReturnValueHandler());  
-         handlers.add(new AsyncTaskMethodReturnValueHandler(this.beanFactory));  
- 
- // Annotation-based return value types
-         handlers.add(new ModelAttributeMethodProcessor(false));  
-         handlers.add(new RequestResponseBodyMethodProcessor(getMessageConverters(), this.contentNegotiationManager));  
- 
- // Multi-purpose return value types
-         handlers.add(new ViewNameMethodReturnValueHandler());  
-         handlers.add(new MapMethodProcessor());  
- 
- // Custom return value types
- //这里这里会从customReturnValueHandlers属性中获取我们自定的HandlerMethodReturnValueHandler
- if (getCustomReturnValueHandlers() != null) {  
-             handlers.addAll(getCustomReturnValueHandlers());  
-         }  
- 
- // Catch-all
- if (!CollectionUtils.isEmpty(getModelAndViewResolvers())) {  
-             handlers.add(new ModelAndViewResolverMethodReturnValueHandler(getModelAndViewResolvers()));  
-         }  
- else {  
-             handlers.add(new ModelAttributeMethodProcessor(true));  
-         }  
- 
- return handlers;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
至此，所有的HandlerMethodReturnValueHandler的注册已经完成。我们可以再回顾下，在该系列的第三篇博客中介绍HandlerMethodReturnValueHandler的使用。 
第一步：获取合适的HandlerAdapter，当方法含有@RequestMaiing注释的时候，便选择RequestMappingHandlerAdapter来进行方法的调度处理 
第二步：方法的调度处理过程为：首先执行方法体，然后根据返回值来选择一个合适的HandlerMethodReturnValueHandler，如下代码：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicfinalvoid invokeAndHandle(ServletWebRequest webRequest,  
-             ModelAndViewContainer mavContainer, Object... providedArgs) throws Exception {  
- 
-         Object returnValue = invokeForRequest(webRequest, mavContainer, providedArgs);  
- 
-         setResponseStatus(webRequest);  
- 
- if (returnValue == null) {  
- if (isRequestNotModified(webRequest) || hasResponseStatus() || mavContainer.isRequestHandled()) {  
-                 mavContainer.setRequestHandled(true);  
- return;  
-             }  
-         }  
- elseif (StringUtils.hasText(this.responseReason)) {  
-             mavContainer.setRequestHandled(true);  
- return;  
-         }  
- 
-         mavContainer.setRequestHandled(false);  
- //重点重点重点重点重点重点重点重点重点重点
- try {  
- this.returnValueHandlers.handleReturnValue(returnValue, getReturnValueType(returnValue), mavContainer, webRequest);  
-         }  
- catch (Exception ex) {  
- if (logger.isTraceEnabled()) {  
-                 logger.trace(getReturnValueHandlingErrorMessage("Error handling return value", returnValue), ex);  
-             }  
- throw ex;  
-         }  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
this.returnValueHandlers.handleReturnValue(returnValue, getReturnValueType(returnValue), mavContainer, webRequest)会遍历所有的已注册的HandlerMethodReturnValueHandler判断他们支不支持returnValue的返回类型。如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicvoid handleReturnValue(  
-             Object returnValue, MethodParameter returnType,  
-             ModelAndViewContainer mavContainer, NativeWebRequest webRequest)  
- throws Exception {  
- 
-         HandlerMethodReturnValueHandler handler = getReturnValueHandler(returnType);  
-         Assert.notNull(handler, "Unknown return value type [" + returnType.getParameterType().getName() + "]");  
-         handler.handleReturnValue(returnValue, returnType, mavContainer, webRequest);  
-     }  
- 
- /**
-      * Find a registered {@link HandlerMethodReturnValueHandler} that supports the given return type.
-      */
- private HandlerMethodReturnValueHandler getReturnValueHandler(MethodParameter returnType) {  
- for (HandlerMethodReturnValueHandler returnValueHandler : returnValueHandlers) {  
- if (logger.isTraceEnabled()) {  
-                 logger.trace("Testing if return value handler [" + returnValueHandler + "] supports [" +  
-                         returnType.getGenericParameterType() + "]");  
-             }  
- if (returnValueHandler.supportsReturnType(returnType)) {  
- return returnValueHandler;  
-             }  
-         }  
- returnnull;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
找到支持的HandlerMethodReturnValueHandler后，就要执行它的handleReturnValue方法。 
下面就具体介绍下下常用的这几个HandlerMethodReturnValueHandler； 
HttpEntityMethodProcessor：用来处理返回值类型是HttpEntity的方法，简单用法如下
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/httpEntity",method=RequestMethod.GET)  
- public HttpEntity<String> testHttpEntity() throws UnsupportedEncodingException{  
-         String body="中国";  
-         HttpHeaders headers=new HttpHeaders();  
-         headers.add("Content-type","text/html;charset=GBK");  
-         HttpEntity<String> ret=new HttpEntity<String>(body,headers);  
- return ret;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
就是在构建http协议的返回体和返回头。 
使用案例如，文件下载。 
经常有人直接用HttpServletRequest和HttpServletResponse来做文件下载，这种方式便与web容器产生的对象耦合在一起，不推荐使用，而是直接使用spring为我们提供的HttpEntityMethodProcessor这一返回值处理器，虽然springmvc最终还是用HttpServletResponse来实现，但是这种方式便断开我们直接与web容器之间的耦合。 
这一过程分析： 
当这个方法执行完成之后，会调用HttpEntityMethodProcessor的handleReturnValue方法， 
该方法内容就是为response设置响应头，然后将响应体的内容写入response的body中，此时又会涉及到HttpMessageConverter，当HttpEntity中的body类型为String,又会让StringHttpMessageConverter来进行转换。这和@ResponseBody的处理过程是一样的。 
ViewNameMethodReturnValueHandler：主要用来处理返回值是String类型（前提不含@ResponseBody标签），它会将返回的字符串作为view视图的名字，如下所示。 
![](http://dl2.iteye.com/upload/attachment/0100/5530/bb975463-d2d7-3da4-8be2-6f1651830328.png)
另一种用法，当返回的字符串以redirect:开始，不再作为view视图名而是作为重定向的地址，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/string",method=RequestMethod.GET)  
- public String testString(){  
- return"redirect:/string";  
-     }  
- 
![](http://static.blog.csdn.net/images/save_snippets.png)
有了重定向，也有转发。以forward：开头便是转发。
如下： 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/string",method=RequestMethod.GET)  
- public String testString(){  
- return"forward:/string";  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
ModelMethodProcessor:用来处理返回类型为Model的，它默认采用请求路径作为视图名称，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/model",method=RequestMethod.GET)  
- public Model handleModel(String name) throws Exception {  
-         Model model=new ExtendedModelMap();  
-         model.addAttribute("name",name);  
- return model;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
![](http://dl2.iteye.com/upload/attachment/0100/5532/dab8e003-4fae-3c4b-8191-fe3a6204aa31.png)
ModelAndViewMethodReturnValueHandler：用来处理返回值类型为ModelAndView，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/modelandview",method=RequestMethod.GET)  
- public ModelAndView testModelAndView() throws Exception {  
- returnnew ModelAndView("hello");  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
RequestResponseBodyMethodProcessor:则是用于处理方法中含有@ResponseBody注解，或类上含有@ResponseBody注解。这一处理过程在本系列的第三篇博客中有介绍，这里不再叙述。 
还有其他的HandlerMethodReturnValueHandler，这里仅仅是作为引路，对HandlerMethodReturnValueHandler有个整体的认识，具体的内容，需要读者去具体研究。
