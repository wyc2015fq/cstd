# SpringMVC源码总结（三）mvc:annotation-driven和mvc:message-converters简单介绍 - z69183787的专栏 - CSDN博客
2016年10月14日 16:32:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11656
上一篇文章讲述了最简单的mvc:annotation-driven，这次就要说说@ResponseBody注解，很明显这个注解就是将方法的返回值作为reponse的body部分。我们进一步分析下这个过程涉及到的内容，首先就是方法返回的类型，可以是字节数组、字符串、对象引用等，将这些返回类型以什么样的内容格式（即response的content-type类型，同时还要考虑到客户端是否接受这个类型）存进response的body中返回给客户端是一个问题，对于这个过程的处理都是靠许许多多的HttpMessageConverter转换器来完成的，这便是本章要讲的内容。 
常用的content-type类型有：text/html、text/plain、text/xml、application/json、application/x-www-form-urlencoded、image/png等，不同的类型，对body中的数据的解析也是不一样的。 
我们的@ResponseBody可以指定content-type，打开ResponseBody注释，我们可以看到这两个属性consumes和produces，它们就是用来指定request的content-type和response的content-type的。都可以接收一个或者多个，用法注释中已经给出了说明。
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * The consumable media types of the mapped request, narrowing the primary mapping.
-      * <p>The format is a single media type or a sequence of media types,
-      * with a request only mapped if the {@code Content-Type} matches one of these media types.
-      * Examples:
-      * <pre class="code">
-      * consumes = "text/plain"
-      * consumes = {"text/plain", "application/*"}
-      * </pre>
-      * Expressions can be negated by using the "!" operator, as in "!text/plain", which matches
-      * all requests with a {@code Content-Type} other than "text/plain".
-      * <p><b>Supported at the type level as well as at the method level!</b>
-      * When used at the type level, all method-level mappings override
-      * this consumes restriction.
-      * @see org.springframework.http.MediaType
-      * @see javax.servlet.http.HttpServletRequest#getContentType()
-      */
-     String[] consumes() default {};  
- 
- /**
-      * The producible media types of the mapped request, narrowing the primary mapping.
-      * <p>The format is a single media type or a sequence of media types,
-      * with a request only mapped if the {@code Accept} matches one of these media types.
-      * Examples:
-      * <pre class="code">
-      * produces = "text/plain"
-      * produces = {"text/plain", "application/*"}
-      * </pre>
-      * Expressions can be negated by using the "!" operator, as in "!text/plain", which matches
-      * all requests with a {@code Accept} other than "text/plain".
-      * <p><b>Supported at the type level as well as at the method level!</b>
-      * When used at the type level, all method-level mappings override
-      * this consumes restriction.
-      * @see org.springframework.http.MediaType
-      */
-     String[] produces() default {};  
当request的content-type不在consumes指定的范围内，则这个request就不会匹配到这个方法。produces 同时指定了方法的返回值将以什么样的content-type写入response的body中。如果这个属性进行了配置下文在获取服务器端指定的content-type就是所配置的值，否则则会获取默认的所有content-type 
当我们对@ResponseBody什么都没有配置时，SpringMVC便启用默认的策略帮我们自动寻找一种最佳的方式将方法的返回值写入response的body中。 
接下来，我们就需要探究SpringMVC是如何处理这一过程的。先说说我的方式，就是调试，当方法执行完返回后，看DispatcherServlet的doDispatch方法的代码：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
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
- 
- if (!mappedHandler.applyPreHandle(processedRequest, response)) {  
- return;  
-                 }  
- 
- try {  
- //这里就是执行我们的业务逻辑，并且对返回结果进行处理的地方
- // Actually invoke the handler.
-                     mv = ha.handle(processedRequest, response, mappedHandler.getHandler());  
-                 }  
mv = ha.handle(processedRequest, response, mappedHandler.getHandler());这里是适配器进行调度我们的handler地方，由于我们使用的是注解，所以对应的适配器是RequestMappingHandlerAdapter，通过一步步的函数调用，最终找到我们的关注重点到RequestMappingHandlerAdapter的方法invokeHandleMethod，具体实现逻辑：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private ModelAndView invokeHandleMethod(HttpServletRequest request,  
-             HttpServletResponse response, HandlerMethod handlerMethod) throws Exception {  
- 
-         ServletWebRequest webRequest = new ServletWebRequest(request, response);  
- 
-         WebDataBinderFactory binderFactory = getDataBinderFactory(handlerMethod);  
-         ModelFactory modelFactory = getModelFactory(handlerMethod, binderFactory);  
-         ServletInvocableHandlerMethod requestMappingMethod = createRequestMappingMethod(handlerMethod, binderFactory);  
- 
-         ModelAndViewContainer mavContainer = new ModelAndViewContainer();  
-         mavContainer.addAllAttributes(RequestContextUtils.getInputFlashMap(request));  
-         modelFactory.initModel(webRequest, mavContainer, requestMappingMethod);  
-         mavContainer.setIgnoreDefaultModelOnRedirect(this.ignoreDefaultModelOnRedirect);  
- 
-         AsyncWebRequest asyncWebRequest = WebAsyncUtils.createAsyncWebRequest(request, response);  
-         asyncWebRequest.setTimeout(this.asyncRequestTimeout);  
- 
- final WebAsyncManager asyncManager = WebAsyncUtils.getAsyncManager(request);  
-         asyncManager.setTaskExecutor(this.taskExecutor);  
-         asyncManager.setAsyncWebRequest(asyncWebRequest);  
-         asyncManager.registerCallableInterceptors(this.callableInterceptors);  
-         asyncManager.registerDeferredResultInterceptors(this.deferredResultInterceptors);  
- 
- if (asyncManager.hasConcurrentResult()) {  
-             Object result = asyncManager.getConcurrentResult();  
-             mavContainer = (ModelAndViewContainer) asyncManager.getConcurrentResultContext()[0];  
-             asyncManager.clearConcurrentResult();  
- 
- if (logger.isDebugEnabled()) {  
-                 logger.debug("Found concurrent result value [" + result + "]");  
-             }  
-             requestMappingMethod = requestMappingMethod.wrapConcurrentResult(result);  
-         }  
- 
- //这里是重点，执行handler的业务逻辑，对于@ResponseBody分支的处理在这里
-         requestMappingMethod.invokeAndHandle(webRequest, mavContainer);  
- 
- if (asyncManager.isConcurrentHandlingStarted()) {  
- returnnull;  
-         }  
- //这里便是分水岭，要么返回一个ModelAndView，对于@ResponseBody的返回内容已写进response的body中，在这里要返回null。
- return getModelAndView(mavContainer, modelFactory, webRequest);  
-     }  
继续深入看下这个requestMappingMethod.invokeAndHandle方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * Invokes the method and handles the return value through a registered
-      * {@link HandlerMethodReturnValueHandler}.
-      *
-      * @param webRequest the current request
-      * @param mavContainer the ModelAndViewContainer for this request
-      * @param providedArgs "given" arguments matched by type, not resolved
-      */
- publicfinalvoid invokeAndHandle(ServletWebRequest webRequest,  
-             ModelAndViewContainer mavContainer, Object... providedArgs) throws Exception {  
- //这里执行完方法体，并返回结果内容
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
- 
- try {  
- //重点在这里
- this.returnValueHandlers.handleReturnValue(returnValue, getReturnValueType(returnValue), mavContainer, webRequest);  
-         }  
- catch (Exception ex) {  
- if (logger.isTraceEnabled()) {  
-                 logger.trace(getReturnValueHandlingErrorMessage("Error handling return value", returnValue), ex);  
-             }  
- throw ex;  
-         }  
-     }  
mavContainer是ModelAndViewContainer类型，主要存储着model信息和view信息，它的一个属性requestHandled为true表示response直接处理不需要view的解决方案（即是需要返回一个视图的）。这里的mavContainer.setRequestHandled(false)只是初始时默认采用view的解决方案。 
继续看this.returnValueHandlers.handleReturnValue具体内容：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**这里已经写了，要遍历已经注册的HandlerMethodReturnValueHandler，然后执行那个支持returnValue的那一个HandlerMethodReturnValueHandler
-      * Iterate over registered {@link HandlerMethodReturnValueHandler}s and invoke the one that supports it.
-      * @exception IllegalStateException if no suitable {@link HandlerMethodReturnValueHandler} is found.
-      */
- @Override
- publicvoid handleReturnValue(  
-             Object returnValue, MethodParameter returnType,  
-             ModelAndViewContainer mavContainer, NativeWebRequest webRequest)  
- throws Exception {  
- 
-         HandlerMethodReturnValueHandler handler = getReturnValueHandler(returnType);  
-         Assert.notNull(handler, "Unknown return value type [" + returnType.getParameterType().getName() + "]");  
-         handler.handleReturnValue(returnValue, returnType, mavContainer, webRequest);  
-     }  
继续看下它是如何找到合适的HandlerMethodReturnValueHandler的
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
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
遍历所有的已注册的HandlerMethodReturnValueHandler，然后调用他们的supportsReturnType方法来判断他们各自是否支持这个返回值类型，通过调试发现会有13个HandlerMethodReturnValueHandler，之后再说这些数据是在什么时候哪个地方注册的。列举下常用的： 
- 
ModelAndViewMethodReturnValueHandler：支持返回值是ModelAndView类型的 
ModelMethodProcessor：支持返回值是Model的 
ViewMethodReturnValueHandler：支持返回值是View 
HttpEntityMethodProcessor：支持返回值是HttpEntity 
RequestResponseBodyMethodProcess：支持类上或者方法上含有@ResponseBody注解的 
ViewNameMethodReturnValueHandler：支持返回类型是void或者String 
所以我们想扩展的话，就可以自定实现一个HandlerMethodReturnValueHandler，然后在初始化时注册进去（这个过程后面再说）。言归正转，对于本工程RequestResponseBodyMethodProcess是支持的，所以它将被作为HandlerMethodReturnValueHandler返回，继续执行上面的handler.handleReturnValue(returnValue, returnType, mavContainer, webRequest)方法，我们来看下RequestResponseBodyMethodProcess具体的处理过程： 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicvoid handleReturnValue(Object returnValue, MethodParameter returnType,  
-             ModelAndViewContainer mavContainer, NativeWebRequest webRequest)  
- throws IOException, HttpMediaTypeNotAcceptableException {  
- //走到这一步，说明该方法不需要view的方案，所以要将requestHandled标示置为true,供其他注释使用判断当前方法的返回值处理策略
-         mavContainer.setRequestHandled(true);  
- if (returnValue != null) {  
-             writeWithMessageConverters(returnValue, returnType, webRequest);  
-         }  
-     }  
方法writeWithMessageConverters的具体内容为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected <T> void writeWithMessageConverters(T returnValue, MethodParameter returnType,  
-             ServletServerHttpRequest inputMessage, ServletServerHttpResponse outputMessage)  
- throws IOException, HttpMediaTypeNotAcceptableException {  
- 
-         Class<?> returnValueClass = returnValue.getClass();  
-         HttpServletRequest servletRequest = inputMessage.getServletRequest();  
- //获取客户端Accept字段接收的content-type
-         List<MediaType> requestedMediaTypes = getAcceptableMediaTypes(servletRequest);  
- //获取服务器端指定的content-type，如果@RequestMapping中的produces配置了content-type，则返回此content-type，若果没有，则获取所有HttpMessageConverter所支持的content-type，然后通过requestedMediaTypes和producibleMediaTypes 对比，选定一个最合适的content-type作为
- //selectedMediaType
-         List<MediaType> producibleMediaTypes = getProducibleMediaTypes(servletRequest, returnValueClass);  
- 
-         Set<MediaType> compatibleMediaTypes = new LinkedHashSet<MediaType>();  
- for (MediaType requestedType : requestedMediaTypes) {  
- for (MediaType producibleType : producibleMediaTypes) {  
- if (requestedType.isCompatibleWith(producibleType)) {  
-                     compatibleMediaTypes.add(getMostSpecificMediaType(requestedType, producibleType));  
-                 }  
-             }  
-         }  
- if (compatibleMediaTypes.isEmpty()) {  
- thrownew HttpMediaTypeNotAcceptableException(producibleMediaTypes);  
-         }  
- 
-         List<MediaType> mediaTypes = new ArrayList<MediaType>(compatibleMediaTypes);  
-         MediaType.sortBySpecificityAndQuality(mediaTypes);  
- 
-         MediaType selectedMediaType = null;  
- for (MediaType mediaType : mediaTypes) {  
- if (mediaType.isConcrete()) {  
-                 selectedMediaType = mediaType;  
- break;  
-             }  
- elseif (mediaType.equals(MediaType.ALL) || mediaType.equals(MEDIA_TYPE_APPLICATION)) {  
-                 selectedMediaType = MediaType.APPLICATION_OCTET_STREAM;  
- break;  
-             }  
-         }  
- 
- if (selectedMediaType != null) {  
-             selectedMediaType = selectedMediaType.removeQualityValue();  
- for (HttpMessageConverter<?> messageConverter :   
- //遍历所有已注册的HttpMessageConverter，选出一个支持返回值类型returnValueClass和
- //selectedMediaType的HttpMessageConverter来进行写入数据到response的body中。
- this.messageConverters) {  
- if (messageConverter.canWrite(returnValueClass, selectedMediaType)) {  
-                     ((HttpMessageConverter<T>) messageConverter).write(returnValue, selectedMediaType, outputMessage);  
- if (logger.isDebugEnabled()) {  
-                         logger.debug("Written [" + returnValue + "] as \"" + selectedMediaType + "\" using [" +  
-                                 messageConverter + "]");  
-                     }  
- return;  
-                 }  
-             }  
-         }  
- thrownew HttpMediaTypeNotAcceptableException(this.allSupportedMediaTypes);  
-     }  
获取客户端的content-type，只需解析Accept头字段即可，获取服务器端指定的content-type则分两种情况，第一种情况为：你在@RequestMapping中指定了produces的content-type类型（会将这一信息存进request的属性中，属性名为HandlerMapping接口名+'.producibleMediaTypes'）如果没指定，则第二种情况：获取所有的已注册的messageConverter，获取它们所有的支持的content-type类型，并且过滤掉那些不支持returnValueClass的类型。然后在这两组List<MediaType>
 requestedMediaTypes和producibleMediaTypes中进行比较匹配（这里的比较规则也挺多的，涉及到q值，有兴趣你们可以总结下），选出一个最合适的content-type，至此有了返回值类型returnValueClass和要写进reponseBody的content-type类型，然后就是要找到一个支持这两者的HttpMessageConverter，已注册的HttpMessageConverter如下： 
- 
ByteArrayHttpMessageConverter：支持返回值类型为byte[]，content-type为application/octet-stream,*/* 
StringHttpMessageConverter：支持的返回值类型为String,content-type为 text/plain;charset=ISO-8859-1,*/* 
ResourceHttpMessageConverter：支持的返回值类型为Resource,content-type为 */* 
SourceHttpMessageConverter：支持的返回值类型为DomSource,SAXSource,Source,StreamSource,content-type为application/xml,text/xml,application/*+xml 
MappingJacksonHttpMessageConverter：判断返回值能否被格式化成json,content-type为 application/json,application/*+json 
AllEncompassingFormHttpMessageConverter：支持的返回值类型为MultiValueMap,content-type为application/x-www-form-urlencoded,multipart/form-data 
对于我们的工程来说，返回类型为String，选出来的最合适的content-type是text/html,并且StringHttpMessageConverter的*/*是兼容任意类型的，所以StringHttpMessageConverter会被选中,然后将返回值以text/html形式写进response的body中。 
顺便说下对于content-length是这样获取的： 
首先从指定的content-type（本工程即text/html）中获取字符集，若能获取到则使用该字符集，若获取不到则使用默认的字符集，对于本工程来说，text/html不像application/json;charset=utf-8那样含有字符集，所以将会使用StringHttpMessageConverter默认的字符集 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicstaticfinal Charset DEFAULT_CHARSET = Charset.forName("ISO-8859-1");  
- 
- privatefinal Charset defaultCharset;  
- 
- privatefinal List<Charset> availableCharsets;  
- 
- privateboolean writeAcceptCharset = true;  
- 
- 
- /**
-      * A default constructor that uses {@code "ISO-8859-1"} as the default charset.
-      * @see #StringHttpMessageConverter(Charset)
-      */
- public StringHttpMessageConverter() {  
- this(DEFAULT_CHARSET);  
-     }  
- 
- /**
-      * A constructor accepting a default charset to use if the requested content
-      * type does not specify one.
-      */
- public StringHttpMessageConverter(Charset defaultCharset) {  
- super(new MediaType("text", "plain", defaultCharset), MediaType.ALL);  
- this.defaultCharset = defaultCharset;  
- this.availableCharsets = new ArrayList<Charset>(Charset.availableCharsets().values());  
-     }  
StringHttpMessageConverter有两个构造函数。当你没有给它指定字符集时，使用默认的ISO-8859-1，这便是造成乱码的一个原因，由于我们经常使用utf-8，所以可以在构造它时指定一下字符集。继续content-length的计算，有了字符集就好办了，计算方法为str.getBytes(字符集).length便是content-length的值,代码如下。
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- if (headers.getContentLength() == -1) {  
-             Long contentLength = getContentLength(t, headers.getContentType());  
- if (contentLength != null) {  
-                 headers.setContentLength(contentLength);  
-             }  
-         }  
和
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- protected Long getContentLength(String s, MediaType contentType) {  
-         Charset charset = getContentTypeCharset(contentType);  
- try {  
- return (long) s.getBytes(charset.name()).length;  
-         }  
- catch (UnsupportedEncodingException ex) {  
- // should not occur
- thrownew IllegalStateException(ex);  
-         }  
-     }  
继续说StringHttpMessageConverter的写入过程，
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- protectedvoid writeInternal(String s, HttpOutputMessage outputMessage) throws IOException {  
- if (this.writeAcceptCharset) {  
-             outputMessage.getHeaders().setAcceptCharset(getAcceptedCharsets());  
-         }  
-         Charset charset = getContentTypeCharset(outputMessage.getHeaders().getContentType());  
- //重点
-         StreamUtils.copy(s, charset, outputMessage.getBody());  
-     }  
上面的charset就是ISO-8859-1，也就是将返回的字符串以ISO-8859-1编码写进response的body中。至此就完成了，然后就出现了ä¸­å½这种乱码。 
下一篇文章将会详细说说乱码，再下一篇文章还要继续本节遗留的很多问题，第一个HandlerMethodReturnValueHandler的来源及使用以及我们来自定义一个HandlerMethodReturnValueHandler，第二个问题：其他HttpMessageConverter的使用以及自定义HttpMessageConverter。
- [message-converters介绍.zip](http://dl.iteye.com/topics/download/4feefb65-5684-334b-afaf-d561ba5b8eaf) (6.8 KB)
- 下载次数: 14
