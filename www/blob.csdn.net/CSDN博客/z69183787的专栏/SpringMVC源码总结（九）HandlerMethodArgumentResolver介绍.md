# SpringMVC源码总结（九）HandlerMethodArgumentResolver介绍 - z69183787的专栏 - CSDN博客
2016年10月14日 16:38:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：664
本文章主要介绍HandlerMethodArgumentResolver在SpringMVC中的使用，介绍几个HandlerMethodArgumentResolver具体的使用情况，然后说明HandlerMethodArgumentResolver的注册来源以及如何自定义注册。 
首先具体看下请求映射到的handler的对应的映射函数的参数形式有哪些： 
HandlerMethodArgumentResolver接口只有两个方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- //判断是否支持要转换的参数类型
- boolean supportsParameter(MethodParameter parameter);  
- //当支持后进行相应的转换
- Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception;  
HandlerMethodArgumentResolver接口的抽象类：AbstractMessageConverterMethodArgumentResolver仅仅引入了HttpMessageConverter，即转换的工作有这些HttpMessageConverter来完成具体的转换和判断由子类来实现。 
如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicabstractclass AbstractMessageConverterMethodArgumentResolver implements HandlerMethodArgumentResolver {  
- 
- protectedfinal Log logger = LogFactory.getLog(getClass());  
- 
- protectedfinal List<HttpMessageConverter<?>> messageConverters;  
- 
- protectedfinal List<MediaType> allSupportedMediaTypes;  
- //略
- }  
AbstractMessageConverterMethodArgumentResolver 的抽象子类AbstractMessageConverterMethodProcessor仅仅是加入了对响应数据进行转换的支持。 
也就是AbstractMessageConverterMethodProcessor的子类不仅可以用来转换请求数据，也可以用来转换响应数据。 
AbstractMessageConverterMethodProcessor的子类HttpEntityMethodProcessor，支持请求和响应的转换，代码如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- return HttpEntity.class.equals(parameter.getParameterType());  
-     }  
- 
- @Override
- publicboolean supportsReturnType(MethodParameter returnType) {  
- return HttpEntity.class.isAssignableFrom(returnType.getParameterType());  
-     }  
使用场景如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/http",method=RequestMethod.POST)  
- @ResponseBody
- public Map<String,Object> testHttp(HttpEntity<String> httpEntity)   
- //略 
-         }  
- 
- 
- @RequestMapping(value="/test/httpEntity",method=RequestMethod.GET)  
- public HttpEntity<String> testHttpEntity(){  
- //略
-     }  
AbstractMessageConverterMethodProcessor的子类RequestResponseBodyMethodProcessor：支持@RequestBody和@ResponseBody,代码如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- //查找参数中是否含有@RequestBody注解
- return parameter.hasParameterAnnotation(RequestBody.class);  
-     }  
- 
- @Override
- publicboolean supportsReturnType(MethodParameter returnType) {  
- 
- //查找参数中是否含有@RequestBody注解或者controller类上是否含有@RequestBody
- return ((AnnotationUtils.findAnnotation(returnType.getContainingClass(), ResponseBody.class) != null) ||  
-                 (returnType.getMethodAnnotation(ResponseBody.class) != null));  
-     }  
使用场景如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/requestBody",method=RequestMethod.POST)  
- @ResponseBody
- public Map<String,Object> testrequestBody(@RequestBody Map<String,Object> map1){  
-         Map<String,Object> map=new HashMap<String,Object>();  
-         map.put("name","lg");  
-         map.put("age",23);  
-         map.put("date",new Date());  
- return map;  
-     }  
HttpEntityMethodProcessor具体的解析参数的过程：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest webRequest, WebDataBinderFactory binderFactory)  
- throws IOException, HttpMediaTypeNotSupportedException {  
- 
-         HttpInputMessage inputMessage = createInputMessage(webRequest);  
-         Type paramType = getHttpEntityType(parameter);  
- 
-         Object body = readWithMessageConverters(webRequest, parameter, paramType);  
- returnnew HttpEntity<Object>(body, inputMessage.getHeaders());  
-     }  
就是通过HttpMessageConverter来进一步的判断是否支持HttpEntity<T>中我们想要的T类型以及是否支持相应的content-type，如public Map<String,Object> testHttp(HttpEntity<String> httpEntity) ，则会选择StringHttpMessageConverter来进行转换。具体的选择过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected <T> Object readWithMessageConverters(HttpInputMessage inputMessage,  
-             MethodParameter methodParam, Type targetType) throws IOException, HttpMediaTypeNotSupportedException {  
- 
-         MediaType contentType;  
- try {  
-             contentType = inputMessage.getHeaders().getContentType();  
-         }  
- catch (InvalidMediaTypeException ex) {  
- thrownew HttpMediaTypeNotSupportedException(ex.getMessage());  
-         }  
- if (contentType == null) {  
-             contentType = MediaType.APPLICATION_OCTET_STREAM;  
-         }  
- 
-         Class<?> contextClass = methodParam.getContainingClass();  
- 
- for (HttpMessageConverter<?> converter : this.messageConverters) {  
- if (converter instanceof GenericHttpMessageConverter) {  
-                 GenericHttpMessageConverter<?> genericConverter = (GenericHttpMessageConverter<?>) converter;  
- if (genericConverter.canRead(targetType, contextClass, contentType)) {  
- if (logger.isDebugEnabled()) {  
-                         logger.debug("Reading [" + targetType + "] as \"" +  
-                                 contentType + "\" using [" + converter + "]");  
-                     }  
- return genericConverter.read(targetType, contextClass, inputMessage);  
-                 }  
-             }  
-             Class<T> targetClass = (Class<T>)  
-                     ResolvableType.forMethodParameter(methodParam, targetType).resolve(Object.class);  
- if (converter.canRead(targetClass, contentType)) {  
- if (logger.isDebugEnabled()) {  
-                     logger.debug("Reading [" + targetClass.getName() + "] as \"" +  
-                             contentType + "\" using [" + converter + "]");  
-                 }  
- return ((HttpMessageConverter<T>) converter).read(targetClass, inputMessage);  
-             }  
-         }  
- 
- thrownew HttpMediaTypeNotSupportedException(contentType, this.allSupportedMediaTypes);  
-     }  
同理RequestResponseBodyMethodProcessor也会使用相应的HttpMessageConverter来进行转换。如public Map<String,Object> testrequestBody(@RequestBody Map<String,Object> map1)则会选择MappingJackson2HttpMessageConverter或者MappingJacksonHttpMessageConverter来完成转换。
再看看另一类的HandlerMethodArgumentResolver： 
RequestParamMethodArgumentResolver支持的类型有，一种是含@RequestParam注解的参数，另一种就是简单类型，如Integer、String、Date、URI, URL,Locale等： 
源代码如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicboolean supportsParameter(MethodParameter parameter) {  
-         Class<?> paramType = parameter.getParameterType();  
- if (parameter.hasParameterAnnotation(RequestParam.class)) {  
- if (Map.class.isAssignableFrom(paramType)) {  
-                 String paramName = parameter.getParameterAnnotation(RequestParam.class).value();  
- return StringUtils.hasText(paramName);  
-             }  
- else {  
- returntrue;  
-             }  
-         }  
- else {  
- if (parameter.hasParameterAnnotation(RequestPart.class)) {  
- returnfalse;  
-             }  
- elseif (MultipartFile.class.equals(paramType) || "javax.servlet.http.Part".equals(paramType.getName())) {  
- returntrue;  
-             }  
- elseif (this.useDefaultResolution) {  
- return BeanUtils.isSimpleProperty(paramType);  
-             }  
- else {  
- returnfalse;  
-             }  
-         }  
-     }  
BeanUtils.isSimpleProperty(paramType)判断是否是简单类型的具体内容如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * Check if the given type represents a "simple" property:
-      * a primitive, a String or other CharSequence, a Number, a Date,
-      * a URI, a URL, a Locale, a Class, or a corresponding array.
-      * <p>Used to determine properties to check for a "simple" dependency-check.
-      * @param clazz the type to check
-      * @return whether the given type represents a "simple" property
-      * @see org.springframework.beans.factory.support.RootBeanDefinition#DEPENDENCY_CHECK_SIMPLE
-      * @see org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory#checkDependencies
-      */
- publicstaticboolean isSimpleProperty(Class<?> clazz) {  
-         Assert.notNull(clazz, "Class must not be null");  
- return isSimpleValueType(clazz) || (clazz.isArray() && isSimpleValueType(clazz.getComponentType()));  
-     }  
- 
- 
- publicstaticboolean isSimpleValueType(Class<?> clazz) {  
- return ClassUtils.isPrimitiveOrWrapper(clazz) || clazz.isEnum() ||  
-                 CharSequence.class.isAssignableFrom(clazz) ||  
-                 Number.class.isAssignableFrom(clazz) ||  
-                 Date.class.isAssignableFrom(clazz) ||  
-                 clazz.equals(URI.class) || clazz.equals(URL.class) ||  
-                 clazz.equals(Locale.class) || clazz.equals(Class.class);  
-     }  
即当请求为 http://localhost:8080/test?name=abc时，处理函数若为test(String name)，则对name的解析就是采用RequestParamMethodArgumentResolver来解析的。 
RequestHeaderMethodArgumentResolver:主要用来处理含有@RequestHeader注解的参数，但同时该参数又不是Map类型。如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- return parameter.hasParameterAnnotation(RequestHeader.class)  
-                 && !Map.class.isAssignableFrom(parameter.getParameterType());  
-     }  
- 
- 
- @Override
- protected Object resolveName(String name, MethodParameter parameter, NativeWebRequest request) throws Exception {  
-         String[] headerValues = request.getHeaderValues(name);  
- if (headerValues != null) {  
- return (headerValues.length == 1 ? headerValues[0] : headerValues);  
-         }  
- else {  
- returnnull;  
-         }  
-     }  
源代码已经说明的很明白了。 
使用场景：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/requestHeader",method=RequestMethod.GET)  
- @ResponseBody
- public Map<String,Object> testrequestHeader(@RequestHeader String  Accept){  
若想获取所有的header信息：则使用另一个RequestHeaderMapMethodArgumentResolver，它则用来获取所有的header信息：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass RequestHeaderMapMethodArgumentResolver implements HandlerMethodArgumentResolver {  
- 
- //这里已经写明白了，要求参数必须含有@RequestHeader注解，并且是Map类型
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- return parameter.hasParameterAnnotation(RequestHeader.class)  
-                 && Map.class.isAssignableFrom(parameter.getParameterType());  
-     }  
- 
- @Override
- public Object resolveArgument(  
-             MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest webRequest, WebDataBinderFactory binderFactory)  
- throws Exception {  
- 
-         Class<?> paramType = parameter.getParameterType();  
- 
- if (MultiValueMap.class.isAssignableFrom(paramType)) {  
-             MultiValueMap<String, String> result;  
- if (HttpHeaders.class.isAssignableFrom(paramType)) {  
-                 result = new HttpHeaders();  
-             }  
- else {  
-                 result = new LinkedMultiValueMap<String, String>();  
-             }  
- for (Iterator<String> iterator = webRequest.getHeaderNames(); iterator.hasNext();) {  
-                 String headerName = iterator.next();  
- for (String headerValue : webRequest.getHeaderValues(headerName)) {  
-                     result.add(headerName, headerValue);  
-                 }  
-             }  
- return result;  
-         }  
- else {  
-             Map<String, String> result = new LinkedHashMap<String, String>();  
- for (Iterator<String> iterator = webRequest.getHeaderNames(); iterator.hasNext();) {  
-                 String headerName = iterator.next();  
-                 String headerValue = webRequest.getHeader(headerName);  
-                 result.put(headerName, headerValue);  
-             }  
- return result;  
-         }  
-     }  
- }  
从上面的解析过程可以看出，参数类型可以是普通的Map类型，也可以是MultiValueMap或者进一步的HttpHeaders，他们与普通Map类型的区别是他们对value值后者们是以List形式存放，前者是以String形式存放。 
使用场景：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/requestHeader",method=RequestMethod.GET)  
- @ResponseBody
- public Map<String,Object> testrequestHeader(@RequestHeader Map<String,Object> map1){  
- 
- 
- public Map<String,Object> testrequestHeader(@RequestHeader MultiValueMap<String,Object> map1){  
PathVariableMethodArgumentResolver:主要针对含有@PathVariable的参数，代码如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- if (!parameter.hasParameterAnnotation(PathVariable.class)) {  
- returnfalse;  
-         }  
- if (Map.class.isAssignableFrom(parameter.getParameterType())) {  
-             String paramName = parameter.getParameterAnnotation(PathVariable.class).value();  
- return StringUtils.hasText(paramName);  
-         }  
- returntrue;  
-     }  
- 
- 
- 
- @Override
- @SuppressWarnings("unchecked")  
- protected Object resolveName(String name, MethodParameter parameter, NativeWebRequest request) throws Exception {  
-         Map<String, String> uriTemplateVars =  
-             (Map<String, String>) request.getAttribute(  
-                     HandlerMapping.URI_TEMPLATE_VARIABLES_ATTRIBUTE, RequestAttributes.SCOPE_REQUEST);  
- return (uriTemplateVars != null) ? uriTemplateVars.get(name) : null;  
-     }  
对于支持的类型也说明的很详细。首先必须含有@PathVariable注解，其次如果是Map类型，必须要指定@PathVariable的值，即这个 
ArgumentResolver只能获取一个uri变量。要想获取多个则要使用PathVariableMapMethodArgumentResolver：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
-         PathVariable annot = parameter.getParameterAnnotation(PathVariable.class);  
- return ((annot != null) && (Map.class.isAssignableFrom(parameter.getParameterType()))  
-                 && (!StringUtils.hasText(annot.value())));  
-     }  
- 
- public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception {  
- 
- @SuppressWarnings("unchecked")  
-         Map<String, String> uriTemplateVars =  
-                 (Map<String, String>) webRequest.getAttribute(  
-                         HandlerMapping.URI_TEMPLATE_VARIABLES_ATTRIBUTE, RequestAttributes.SCOPE_REQUEST);  
- 
- if (!CollectionUtils.isEmpty(uriTemplateVars)) {  
- returnnew LinkedHashMap<String, String>(uriTemplateVars);  
-         }  
- else {  
- return Collections.emptyMap();  
-         }  
-     }  
它要求必须含有@PathVariable注解，并且必须是Map类型，并且@PathVariable注解的value没有值。同时我们可以从PathVariableMapMethodArgumentResolver和PathVariableMethodArgumentResolver上面看出，他们的取值都是从request的属性上进行获取的webRequest.getAttribute( 
HandlerMapping.URI_TEMPLATE_VARIABLES_ATTRIBUTE, RequestAttributes.SCOPE_REQUEST);也就是说，在解析完@RequestMapping匹配工作后，便将这些参数设置进request的属性上，属性名为HandlerMapping.URI_TEMPLATE_VARIABLES_ATTRIBUTE。其他的HandlerMethodArgumentResolver可以自行研究，这里不再说明。 
至此，我们就要说明下HandlerMethodArgumentResolver的注册来源： 
它的来源分为两部分，一部分spring默认的HandlerMethodArgumentResolver，另一部分就是我们自定义的HandlerMethodArgumentResolver。 
还是先看mvc:annotation-driven中配置自定义的HandlerMethodArgumentResolver：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:annotation-driven >  
-         <mvc:argument-resolvers>  
-             <bean class="xxx"></bean>  
-         </mvc:argument-resolvers>  
-     </mvc:annotation-driven>    
在mvc:argument-resolvers标签下配置相应的自定义的HandlerMethodArgumentResolver。 
然后在mvc:annotation-driven的注解驱动类AnnotationDrivenBeanDefinitionParser中会有这样的代码：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- ManagedList<?> argumentResolvers = getArgumentResolvers(element, parserContext);  
- 
- //略
- if (argumentResolvers != null) {  
-             handlerAdapterDef.getPropertyValues().add("customArgumentResolvers", argumentResolvers);  
-         }  
其中getArgumentResolvers就是获取我们自定义的HandlerMethodArgumentResolver
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private ManagedList<?> getArgumentResolvers(Element element, ParserContext parserContext) {  
-         Element resolversElement = DomUtils.getChildElementByTagName(element, "argument-resolvers");  
- if (resolversElement != null) {  
-             ManagedList<BeanDefinitionHolder> argumentResolvers = extractBeanSubElements(resolversElement, parserContext);  
- return wrapWebArgumentResolverBeanDefs(argumentResolvers, parserContext);  
-         }  
- returnnull;  
-     }  
从上面的代码可以看出，获取我们自定义的HandlerMethodArgumentResolver然后把它设置进RequestMappingHandlerAdapter的customArgumentResolvers参数中，RequestMappingHandlerAdapter有两个与HandlerMethodArgumentResolver有关的参数：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private List<HandlerMethodArgumentResolver> customArgumentResolvers;  
- private HandlerMethodArgumentResolverComposite argumentResolvers;  
HandlerMethodArgumentResolverComposite 也仅仅是内部存放一个List<HandlerMethodArgumentResolver>集合，同时本身又继承HandlerMethodArgumentResolver，所以它的实现都是靠内部的List<HandlerMethodArgumentResolver>集合来实现的。
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatefinal List<HandlerMethodArgumentResolver> argumentResolvers =  
- new LinkedList<HandlerMethodArgumentResolver>();  
- 
- //使用了适合高并发的ConcurrentHashMap来进行缓存
- privatefinal Map<MethodParameter, HandlerMethodArgumentResolver> argumentResolverCache =  
- new ConcurrentHashMap<MethodParameter, HandlerMethodArgumentResolver>(256);  
- 
- 
- /**
-      * Return a read-only list with the contained resolvers, or an empty list.
-      */
- public List<HandlerMethodArgumentResolver> getResolvers() {  
- return Collections.unmodifiableList(this.argumentResolvers);  
-     }  
- 
- /**
-      * Whether the given {@linkplain MethodParameter method parameter} is supported by any registered
-      * {@link HandlerMethodArgumentResolver}.
-      */
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- return getArgumentResolver(parameter) != null;  
-     }  
- 
- /**
-      * Iterate over registered {@link HandlerMethodArgumentResolver}s and invoke the one that supports it.
-      * @exception IllegalStateException if no suitable {@link HandlerMethodArgumentResolver} is found.
-      */
- @Override
- public Object resolveArgument(  
-             MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest webRequest, WebDataBinderFactory binderFactory)  
- throws Exception {  
- 
-         HandlerMethodArgumentResolver resolver = getArgumentResolver(parameter);  
-         Assert.notNull(resolver, "Unknown parameter type [" + parameter.getParameterType().getName() + "]");  
- return resolver.resolveArgument(parameter, mavContainer, webRequest, binderFactory);  
-     }  
- 
- /**
-      * Find a registered {@link HandlerMethodArgumentResolver} that supports the given method parameter.
-      */
- private HandlerMethodArgumentResolver getArgumentResolver(MethodParameter parameter) {  
-         HandlerMethodArgumentResolver result = this.argumentResolverCache.get(parameter);  
- if (result == null) {  
- for (HandlerMethodArgumentResolver methodArgumentResolver : this.argumentResolvers) {  
- if (logger.isTraceEnabled()) {  
-                     logger.trace("Testing if argument resolver [" + methodArgumentResolver + "] supports [" +  
-                             parameter.getGenericParameterType() + "]");  
-                 }  
- if (methodArgumentResolver.supportsParameter(parameter)) {  
-                     result = methodArgumentResolver;  
- this.argumentResolverCache.put(parameter, result);  
- break;  
-                 }  
-             }  
-         }  
- return result;  
-     }  
在RequestMappingHandlerAdapter完成参数设置后，会调用afterPropertiesSet方法
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
-             List<HandlerMethodReturnValueHandler> handlers = getDefaultReturnValueHandlers();  
- this.returnValueHandlers = new HandlerMethodReturnValueHandlerComposite().addHandlers(handlers);  
-         }  
-         initControllerAdviceCache();  
-     }  
getDefaultArgumentResolvers方法完成了所有的HandlerMethodArgumentResolver的汇总，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private List<HandlerMethodArgumentResolver> getDefaultArgumentResolvers() {  
-         List<HandlerMethodArgumentResolver> resolvers = new ArrayList<HandlerMethodArgumentResolver>();  
- 
- // Annotation-based argument resolution
-         resolvers.add(new RequestParamMethodArgumentResolver(getBeanFactory(), false));  
-         resolvers.add(new RequestParamMapMethodArgumentResolver());  
-         resolvers.add(new PathVariableMethodArgumentResolver());  
-         resolvers.add(new PathVariableMapMethodArgumentResolver());  
-         resolvers.add(new MatrixVariableMethodArgumentResolver());  
-         resolvers.add(new MatrixVariableMapMethodArgumentResolver());  
-         resolvers.add(new ServletModelAttributeMethodProcessor(false));  
-         resolvers.add(new RequestResponseBodyMethodProcessor(getMessageConverters()));  
-         resolvers.add(new RequestPartMethodArgumentResolver(getMessageConverters()));  
-         resolvers.add(new RequestHeaderMethodArgumentResolver(getBeanFactory()));  
-         resolvers.add(new RequestHeaderMapMethodArgumentResolver());  
-         resolvers.add(new ServletCookieValueMethodArgumentResolver(getBeanFactory()));  
-         resolvers.add(new ExpressionValueMethodArgumentResolver(getBeanFactory()));  
- 
- // Type-based argument resolution
-         resolvers.add(new ServletRequestMethodArgumentResolver());  
-         resolvers.add(new ServletResponseMethodArgumentResolver());  
-         resolvers.add(new HttpEntityMethodProcessor(getMessageConverters()));  
-         resolvers.add(new RedirectAttributesMethodArgumentResolver());  
-         resolvers.add(new ModelMethodProcessor());  
-         resolvers.add(new MapMethodProcessor());  
-         resolvers.add(new ErrorsMethodArgumentResolver());  
-         resolvers.add(new SessionStatusMethodArgumentResolver());  
-         resolvers.add(new UriComponentsBuilderMethodArgumentResolver());  
- 
- // Custom arguments
- //获取我们自定义的HandlerMethodArgumentResolver
- if (getCustomArgumentResolvers() != null) {  
-             resolvers.addAll(getCustomArgumentResolvers());  
-         }  
- 
- // Catch-all
-         resolvers.add(new RequestParamMethodArgumentResolver(getBeanFactory(), true));  
-         resolvers.add(new ServletModelAttributeMethodProcessor(true));  
- 
- return resolvers;  
-     }  
不仅汇总了spring默认的，同时加进来我们自定义的HandlerMethodArgumentResolver。至此，HandlerMethodArgumentResolver的来龙去脉都说清楚了。然后就是我们自定义HandlerMethodArgumentResolver,下一篇文章再说。 

