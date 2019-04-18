# SpringMVC源码总结（八）类型转换PropertyEditor的背后 - z69183787的专栏 - CSDN博客
2016年10月14日 16:37:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：591
PropertyEditor是Spring最初采用的转换策略。将会转移到Converter上。本文章主要对@InitBinder注解背后代码层面的运行过程做介绍。所以最好先熟悉它的用法然后来看通代码流程。 
先看实例，controller代码如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Controller
- publicclass FormAction{  
- 
- // 这样的方法里，一般是用来注册一些PropertyEditor
- @InitBinder
- publicvoid initBinder(WebDataBinder binder) throws Exception {    
-         DateFormat df = new SimpleDateFormat("yyyy---MM---dd HH:mm:ss");    
-         CustomDateEditor dateEditor = new CustomDateEditor(df, true);    
-         binder.registerCustomEditor(Date.class, dateEditor);        
-     }     
- 
- 
- @RequestMapping(value="/test/json",method=RequestMethod.GET)  
- @ResponseBody
- public Map<String,Object> getFormData(Date date){  
-         Map<String,Object> map=new HashMap<String,Object>();  
-         map.put("name","lg");  
-         map.put("age",23);  
-         map.put("date",new Date());  
- return map;  
-     }  
- }  
xml文件仅仅开启mvc:ananotation-driven:
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:annotation-driven />    
然后访问  http://localhost:8080/test/json?date=2014---08---3 03:34:23，便看到成功的获取到了数据。接下来源代码代码分析这一过程： 
由于使用了@RequestMapping所以会选择RequestMappingHandlerAdapter来调度执行相应的方法，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * Invoke the {@link RequestMapping} handler method preparing a {@link ModelAndView}
-      * if view resolution is required.
-      */
- private ModelAndView invokeHandleMethod(HttpServletRequest request,  
-             HttpServletResponse response, HandlerMethod handlerMethod) throws Exception {  
- 
-         ServletWebRequest webRequest = new ServletWebRequest(request, response);  
- //我们关注的重点重点重点重点重点重点重点重点
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
-         requestMappingMethod.invokeAndHandle(webRequest, mavContainer);  
- 
- if (asyncManager.isConcurrentHandlingStarted()) {  
- returnnull;  
-         }  
- 
- return getModelAndView(mavContainer, modelFactory, webRequest);  
-     }  
这里面就是整个执行过程。首先绑定请求参数到方法的参数上，然后执行方法，接下来根据方法返回的类型来选择合适的HandlerMethodReturnValueHandler来进行处理，最后要么走view路线，要么直接写入response的body中返回。 
我们此时关注的重点是：如何绑定请求参数到方法的参数上的呢？ 
WebDataBinderFactory binderFactory = getDataBinderFactory(handlerMethod); 
针对每次对该handlerMethod请求产生一个绑定工厂，由这个工厂来完成数据的绑定。 
这里的handlerMethod包含了 controller对象FormAction和、test/json映射到的方法即getFormData。 
然后详细看下getDataBinderFactory的实现：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private WebDataBinderFactory getDataBinderFactory(HandlerMethod handlerMethod) throws Exception {  
- //这里的handlerType便是controller的类型FormAction
-         Class<?> handlerType = handlerMethod.getBeanType();  
-         Set<Method> methods = this.initBinderCache.get(handlerType);  
- if (methods == null) {  
- //关注点1：找出FormAction类的所有的含有@InitBinder的方法（方法的返回类型必须为void），找到后同时缓存起来
-             methods = HandlerMethodSelector.selectMethods(handlerType, INIT_BINDER_METHODS);  
- this.initBinderCache.put(handlerType, methods);  
-         }  
-         List<InvocableHandlerMethod> initBinderMethods = new ArrayList<InvocableHandlerMethod>();  
- // Global methods first
- //关注点2：再寻找出全局的初始化Binder的方法
- for (Entry<ControllerAdviceBean, Set<Method>> entry : this.initBinderAdviceCache .entrySet()) {  
- if (entry.getKey().isApplicableToBeanType(handlerType)) {  
-                 Object bean = entry.getKey().resolveBean();  
- for (Method method : entry.getValue()) {  
-                     initBinderMethods.add(createInitBinderMethod(bean, method));  
-                 }  
-             }  
-         }  
- for (Method method : methods) {  
-             Object bean = handlerMethod.getBean();  
-             initBinderMethods.add(createInitBinderMethod(bean, method));  
-         }  
- //关注点3：找到了所有的与该handlerMethod有关的初始化binder的方法，保存起来
- return createDataBinderFactory(initBinderMethods);  
-     }  
上面稍微做了些注释，然后看下详细的内容： 
关注点1：就是使用过滤，过滤类为：INIT_BINDER_METHODS，如下
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * MethodFilter that matches {@link InitBinder @InitBinder} methods.
-      */
- publicstaticfinal MethodFilter INIT_BINDER_METHODS = new MethodFilter() {  
- 
- @Override
- publicboolean matches(Method method) {  
- return AnnotationUtils.findAnnotation(method, InitBinder.class) != null;  
-         }  
-     };  
这个过滤类就是在handlerType即FormAction中过滤那些含有@InitBinder注解的方法。找到了之后就缓存起来，供下次使用。key为：handlerType，value为找到的方法。存至initBinderCache中。 
关注点2：从initBinderAdviceCache中获取所有支持这个handlerType的method。这一块有待继续研究，这个initBinderAdviceCache是如何初始化来的等等。针对目前的工程来说，initBinderAdviceCache是为空的。 
关注点3：遍历所有找到的和handlerType有关的method,然后封装成InvocableHandlerMethod，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- for (Method method : methods) {  
-             Object bean = handlerMethod.getBean();  
-             initBinderMethods.add(createInitBinderMethod(bean, method));  
-         }  
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private InvocableHandlerMethod createInitBinderMethod(Object bean, Method method) {  
-         InvocableHandlerMethod binderMethod = new InvocableHandlerMethod(bean, method);  
-         binderMethod.setHandlerMethodArgumentResolvers(this.initBinderArgumentResolvers);  
-         binderMethod.setDataBinderFactory(new DefaultDataBinderFactory(this.webBindingInitializer));  
-         binderMethod.setParameterNameDiscoverer(this.parameterNameDiscoverer);  
- return binderMethod;  
-     }  
在封装的过程中，同时设置一些RequestMappingHandlerAdapter的一些参数进去initBinderArgumentResolvers、webBindingInitializer、parameterNameDiscoverer。 
封装完所有的方法后，创建出最终的WebDataBinderFactory。如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected InitBinderDataBinderFactory createDataBinderFactory(List<InvocableHandlerMethod> binderMethods)  
- throws Exception {  
- 
- returnnew ServletRequestDataBinderFactory(binderMethods, getWebBindingInitializer());  
-     }  
getWebBindingInitializer()也是RequestMappingHandlerAdapter的webBindingInitializer参数。 
至此绑定数据的工厂完成了，包含了这个handlerType的所有的PropertyEditor。这是准备工作，然后就是等待执行这个我们自己的方法getFormData执行时来完成参数的绑定过程。 
绑定参数过程即getFormData的执行过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- ServletInvocableHandlerMethod requestMappingMethod = createRequestMappingMethod(handlerMethod, binderFactory);  
- 略  
- requestMappingMethod.invokeAndHandle(webRequest, mavContainer);  
其中的requestMappingMethod经过了进一步的包装，已经包含刚才已经创建的绑定工厂。 
执行过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicfinal Object invokeForRequest(NativeWebRequest request, ModelAndViewContainer mavContainer,  
-             Object... providedArgs) throws Exception {  
- 
-         Object[] args = getMethodArgumentValues(request, mavContainer, providedArgs);  
- if (logger.isTraceEnabled()) {  
-             StringBuilder sb = new StringBuilder("Invoking [");  
-             sb.append(getBeanType().getSimpleName()).append(".");  
-             sb.append(getMethod().getName()).append("] method with arguments ");  
-             sb.append(Arrays.asList(args));  
-             logger.trace(sb.toString());  
-         }  
-         Object returnValue = invoke(args);  
- if (logger.isTraceEnabled()) {  
-             logger.trace("Method [" + getMethod().getName() + "] returned [" + returnValue + "]");  
-         }  
- return returnValue;  
-     }  
分两大步，绑定参数和执行方法体。最重要的就是如何来绑定参数呢？
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private Object[] getMethodArgumentValues(NativeWebRequest request, ModelAndViewContainer mavContainer,  
-             Object... providedArgs) throws Exception {  
- 
-         MethodParameter[] parameters = getMethodParameters();  
-         Object[] args = new Object[parameters.length];  
- for (int i = 0; i < parameters.length; i++) {  
-             MethodParameter parameter = parameters[i];  
-             parameter.initParameterNameDiscovery(this.parameterNameDiscoverer);  
-             GenericTypeResolver.resolveParameterType(parameter, getBean().getClass());  
-             args[i] = resolveProvidedArgument(parameter, providedArgs);  
- if (args[i] != null) {  
- continue;  
-             }  
- if (this.argumentResolvers.supportsParameter(parameter)) {  
- try {  
-                     args[i] = this.argumentResolvers.resolveArgument(  
-                             parameter, mavContainer, request, this.dataBinderFactory);  
- continue;  
-                 }  
- catch (Exception ex) {  
- if (logger.isTraceEnabled()) {  
-                         logger.trace(getArgumentResolutionErrorMessage("Error resolving argument", i), ex);  
-                     }  
- throw ex;  
-                 }  
-             }  
- if (args[i] == null) {  
-                 String msg = getArgumentResolutionErrorMessage("No suitable resolver for argument", i);  
- thrownew IllegalStateException(msg);  
-             }  
-         }  
- return args;  
-     }  
绑定参数又引出来另一个重要名词：HandlerMethodArgumentResolver。args[i] = this.argumentResolvers.resolveArgument( 
parameter, mavContainer, request, this.dataBinderFactory);的具体内容如下： 
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
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
遍历所有已注册的HandlerMethodArgumentResolver，然后找出一个适合的来进行参数绑定，对于本工程来说，getFormData(Date date)的参数date默认是request params级别的，所以使用RequestParamMethodArgumentResolver来处理这一过程。处理过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicfinal Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception {  
- 
-         Class<?> paramType = parameter.getParameterType();  
-         NamedValueInfo namedValueInfo = getNamedValueInfo(parameter);  
- 
-         Object arg = resolveName(namedValueInfo.name, parameter, webRequest);  
- if (arg == null) {  
- if (namedValueInfo.defaultValue != null) {  
-                 arg = resolveDefaultValue(namedValueInfo.defaultValue);  
-             }  
- elseif (namedValueInfo.required) {  
-                 handleMissingValue(namedValueInfo.name, parameter);  
-             }  
-             arg = handleNullValue(namedValueInfo.name, arg, paramType);  
-         }  
- elseif ("".equals(arg) && (namedValueInfo.defaultValue != null)) {  
-             arg = resolveDefaultValue(namedValueInfo.defaultValue);  
-         }  
- 
- if (binderFactory != null) {  
-             WebDataBinder binder = binderFactory.createBinder(webRequest, null, namedValueInfo.name);  
-             arg = binder.convertIfNecessary(arg, paramType, parameter);  
-         }  
- 
-         handleResolvedValue(arg, namedValueInfo.name, parameter, mavContainer, webRequest);  
- 
- return arg;  
-     }  
NamedValueInfo namedValueInfo = getNamedValueInfo(parameter);获取参数信息，就是按照@RequestParam的3个属性来收集的，即defaultValue=null、required=false、name=date, 
Object arg = resolveName(namedValueInfo.name, parameter, webRequest);然后就是获取原始数据,获取过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- protected Object resolveName(String name, MethodParameter parameter, NativeWebRequest webRequest) throws Exception {  
-         Object arg;  
- 
-         HttpServletRequest servletRequest = webRequest.getNativeRequest(HttpServletRequest.class);  
-         MultipartHttpServletRequest multipartRequest =  
-             WebUtils.getNativeRequest(servletRequest, MultipartHttpServletRequest.class);  
- 
- if (MultipartFile.class.equals(parameter.getParameterType())) {  
-             assertIsMultipartRequest(servletRequest);  
-             Assert.notNull(multipartRequest, "Expected MultipartHttpServletRequest: is a MultipartResolver configured?");  
-             arg = multipartRequest.getFile(name);  
-         }  
- elseif (isMultipartFileCollection(parameter)) {  
-             assertIsMultipartRequest(servletRequest);  
-             Assert.notNull(multipartRequest, "Expected MultipartHttpServletRequest: is a MultipartResolver configured?");  
-             arg = multipartRequest.getFiles(name);  
-         }  
- elseif(isMultipartFileArray(parameter)) {  
-             assertIsMultipartRequest(servletRequest);  
-             Assert.notNull(multipartRequest, "Expected MultipartHttpServletRequest: is a MultipartResolver configured?");  
-             arg = multipartRequest.getFiles(name).toArray(new MultipartFile[0]);  
-         }  
- elseif ("javax.servlet.http.Part".equals(parameter.getParameterType().getName())) {  
-             assertIsMultipartRequest(servletRequest);  
-             arg = servletRequest.getPart(name);  
-         }  
- elseif (isPartCollection(parameter)) {  
-             assertIsMultipartRequest(servletRequest);  
-             arg = new ArrayList<Object>(servletRequest.getParts());  
-         }  
- elseif (isPartArray(parameter)) {  
-             assertIsMultipartRequest(servletRequest);  
-             arg = RequestPartResolver.resolvePart(servletRequest);  
-         }  
- else {  
-             arg = null;  
- if (multipartRequest != null) {  
-                 List<MultipartFile> files = multipartRequest.getFiles(name);  
- if (!files.isEmpty()) {  
-                     arg = (files.size() == 1 ? files.get(0) : files);  
-                 }  
-             }  
- if (arg == null) {  
- //对于本工程，我们的重点在这里这里这里这里这里这里
-                 String[] paramValues = webRequest.getParameterValues(name);  
- if (paramValues != null) {  
-                     arg = paramValues.length == 1 ? paramValues[0] : paramValues;  
-                 }  
-             }  
-         }  
- 
- return arg;  
-     }  
通过webRequest.getParameterValues(name)来获取原始的字符串。这里便有涉及到了容器如tomcat的处理过程，这一获取参数的过程在本系列的第五篇文章tomcat的获取参数中进行了详细的源码介绍，那一篇主要是介绍乱码的。本文章不再介绍，接着说，这样就可以获取到我们请求的原始字符串"2014---08---3 03:34:23"，接下来便是执行转换绑定的过程：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- if (binderFactory != null) {  
-             WebDataBinder binder = binderFactory.createBinder(webRequest, null, namedValueInfo.name);  
-             arg = binder.convertIfNecessary(arg, paramType, parameter);  
-         }  
这一过程就是要寻找我们已经注册的所有的PropertyEditor来进行转换，如果还没有找到，则使用另一套转换流程，使用conversionService来进行转换。我们慢慢来看这一过程，有了binderFactory便可以创建出WebDataBinder，具体的创建过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicfinal WebDataBinder createBinder(NativeWebRequest webRequest, Object target, String objectName)  
- throws Exception {  
-         WebDataBinder dataBinder = createBinderInstance(target, objectName, webRequest);  
- if (this.initializer != null) {  
- this.initializer.initBinder(dataBinder, webRequest);  
-         }  
-         initBinder(dataBinder, webRequest);  
- return dataBinder;  
-     }  
先创建出WebDataBinder，然后使用initializer的initBinder方法来初始化一些PropertyEditor，initializer的类型为我们常见的ConfigurableWebBindingInitializer即在mvc:annotation-driven时默认注册的最终设置为RequestMappingHandlerAdapter的webBindingInitializer属性值。this.initializer.initBinder(dataBinder,
 webRequest);过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Override
- publicvoid initBinder(WebDataBinder binder, WebRequest request) {  
-         binder.setAutoGrowNestedPaths(this.autoGrowNestedPaths);  
- if (this.directFieldAccess) {  
-             binder.initDirectFieldAccess();  
-         }  
- if (this.messageCodesResolver != null) {  
-             binder.setMessageCodesResolver(this.messageCodesResolver);  
-         }  
- if (this.bindingErrorProcessor != null) {  
-             binder.setBindingErrorProcessor(this.bindingErrorProcessor);  
-         }  
- if (this.validator != null && binder.getTarget() != null &&  
- this.validator.supports(binder.getTarget().getClass())) {  
-             binder.setValidator(this.validator);  
-         }  
- if (this.conversionService != null) {  
-             binder.setConversionService(this.conversionService);  
-         }  
- if (this.propertyEditorRegistrars != null) {  
- for (PropertyEditorRegistrar propertyEditorRegistrar : this.propertyEditorRegistrars) {  
-                 propertyEditorRegistrar.registerCustomEditors(binder);  
-             }  
-         }  
-     }  
即设置一些我们conversionService、messageCodesResolver、validator 等，这些参数即我们在mvc:annotation中进行设置的，若无设置，采用默认的。 
继续执行initBinder(dataBinder, webRequest);
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicvoid initBinder(WebDataBinder binder, NativeWebRequest request) throws Exception {  
- for (InvocableHandlerMethod binderMethod : this.binderMethods) {  
- if (isBinderMethodApplicable(binderMethod, binder)) {  
-                 Object returnValue = binderMethod.invokeForRequest(request, null, binder);  
- if (returnValue != null) {  
- thrownew IllegalStateException("@InitBinder methods should return void: " + binderMethod);  
-                 }  
-             }  
-         }  
-     }  
执行那些适合我们已经创建的WebDataBinder，怎样才叫适合的呢？看isBinderMethodApplicable(binderMethod, binder)方法
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedboolean isBinderMethodApplicable(HandlerMethod initBinderMethod, WebDataBinder binder) {  
-         InitBinder annot = initBinderMethod.getMethodAnnotation(InitBinder.class);  
-         Collection<String> names = Arrays.asList(annot.value());  
- return (names.size() == 0 || names.contains(binder.getObjectName()));  
-     }  
当initBinderMethod上的@InitBinder注解指定了value，该value可以是多个，当它包含了我们的方法的参数date,则这个initBinderMethod就会被执行。当@InitBinder注解没有指定value，则也会被执行。所以为了不用执行一些不必要的initBinderMethod，我们最好为这些initBinderMethod上的@InitBinder加上value限定。对于我们写的initBinder便因此开始执行了。 
由binderFactory创建出来的WebDataBinder就此完成，然后才是详细的转换过程：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- public <T> T convertIfNecessary(String propertyName, Object oldValue, Object newValue,  
-             Class<T> requiredType, TypeDescriptor typeDescriptor) throws IllegalArgumentException {  
- 
-         Object convertedValue = newValue;  
- 
- // Custom editor for this type?
-         PropertyEditor editor = this.propertyEditorRegistry.findCustomEditor(requiredType, propertyName);  
- 
-         ConversionFailedException firstAttemptEx = null;  
- 
- // No custom editor but custom ConversionService specified?
-         ConversionService conversionService = this.propertyEditorRegistry.getConversionService();  
- 
- //略
- }  
这里首先使用已注册的PropertyEditor，当仍然没有找到时才使用ConversionService。对于本工程来说，由于已经手动注册了对于Date的转换的PropertyEditor即CustomDateEditor，然后便会执行CustomDateEditor的具体的转换过程。至此，大体过程就算是完了。

