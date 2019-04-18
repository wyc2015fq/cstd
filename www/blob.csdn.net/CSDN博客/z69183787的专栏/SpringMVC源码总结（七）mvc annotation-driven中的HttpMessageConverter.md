# SpringMVC源码总结（七）mvc:annotation-driven中的HttpMessageConverter - z69183787的专栏 - CSDN博客
2016年10月14日 16:36:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：811
这一篇文章主要介绍下HttpMessageConverter整个注册过程包含自定义的HttpMessageConverter，然后对一些HttpMessageConverter进行具体介绍。 
HttpMessageConverter接口介绍：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicinterface HttpMessageConverter<T> {  
- 
- /**
-      * Indicates whether the given class can be read by this converter.
-      * @param clazz the class to test for readability
-      * @param mediaType the media type to read, can be {@code null} if not specified.
-      * Typically the value of a {@code Content-Type} header.
-      * @return {@code true} if readable; {@code false} otherwise
-      */
- boolean canRead(Class<?> clazz, MediaType mediaType);  
- 
- /**
-      * Indicates whether the given class can be written by this converter.
-      * @param clazz the class to test for writability
-      * @param mediaType the media type to write, can be {@code null} if not specified.
-      * Typically the value of an {@code Accept} header.
-      * @return {@code true} if writable; {@code false} otherwise
-      */
- boolean canWrite(Class<?> clazz, MediaType mediaType);  
- 
- /**
-      * Return the list of {@link MediaType} objects supported by this converter.
-      * @return the list of supported media types
-      */
-     List<MediaType> getSupportedMediaTypes();  
- 
- /**
-      * Read an object of the given type form the given input message, and returns it.
-      * @param clazz the type of object to return. This type must have previously been passed to the
-      * {@link #canRead canRead} method of this interface, which must have returned {@code true}.
-      * @param inputMessage the HTTP input message to read from
-      * @return the converted object
-      * @throws IOException in case of I/O errors
-      * @throws HttpMessageNotReadableException in case of conversion errors
-      */
-     T read(Class<? extends T> clazz, HttpInputMessage inputMessage)  
- throws IOException, HttpMessageNotReadableException;  
- 
- /**
-      * Write an given object to the given output message.
-      * @param t the object to write to the output message. The type of this object must have previously been
-      * passed to the {@link #canWrite canWrite} method of this interface, which must have returned {@code true}.
-      * @param contentType the content type to use when writing. May be {@code null} to indicate that the
-      * default content type of the converter must be used. If not {@code null}, this media type must have
-      * previously been passed to the {@link #canWrite canWrite} method of this interface, which must have
-      * returned {@code true}.
-      * @param outputMessage the message to write to
-      * @throws IOException in case of I/O errors
-      * @throws HttpMessageNotWritableException in case of conversion errors
-      */
- void write(T t, MediaType contentType, HttpOutputMessage outputMessage)  
- throws IOException, HttpMessageNotWritableException;  
- 
- }  
从HttpInputMessage中读取数据： T read(Class<? extends T> clazz, HttpInputMessage inputMessage)，前提clazz能够通过canRead(clazz，mediaType)测试。 
向HttpOutputMessage中写入数据：void write(T t, MediaType contentType, HttpOutputMessage outputMessage)，前提能够通过canWrite方法。 
简单举例： 
如StringHttpMessageConverter，read方法就是根据编码类型将HttpInputMessage中的数据变为字符串。write方法就是根据编码类型将字符串数据写入HttpOutputMessage中。 
HttpMessageConverter的使用场景： 
它主要是用来转换request的内容到一定的格式，转换输出的内容的到response。 
看下自定义的使用方式：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:annotation-driven>  
-         <mvc:message-converters register-defaults="true">  
-             <bean class="org.springframework.http.converter.StringHttpMessageConverter">  
-                 <constructor-arg value="UTF-8"/>  
-             </bean>  
-         </mvc:message-converters>  
-     </mvc:annotation-driven>  
首先还是在对mvc:annotation-driven解析的AnnotationDrivenBeanDefinitionParser中，有这么一个方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- ManagedList<?> messageConverters = getMessageConverters(element, source, parserContext);  
获取所有的HttpMessageConverter，最终设置到RequestMappingHandlerAdapter的private List<HttpMessageConverter<?>> messageConverters属性上。看下具体的获取过程：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- private ManagedList<?> getMessageConverters(Element element, Object source, ParserContext parserContext) {  
-         Element convertersElement = DomUtils.getChildElementByTagName(element, "message-converters");  
-         ManagedList<? super Object> messageConverters = new ManagedList<Object>();  
- if (convertersElement != null) {  
-             messageConverters.setSource(source);  
- for (Element beanElement : DomUtils.getChildElementsByTagName(convertersElement, "bean", "ref")) {  
-                 Object object = parserContext.getDelegate().parsePropertySubElement(beanElement, null);  
-                 messageConverters.add(object);  
-             }  
-         }  
- 
- if (convertersElement == null || Boolean.valueOf(convertersElement.getAttribute("register-defaults"))) {  
-             messageConverters.setSource(source);  
-             messageConverters.add(createConverterDefinition(ByteArrayHttpMessageConverter.class, source));  
- 
-             RootBeanDefinition stringConverterDef = createConverterDefinition(StringHttpMessageConverter.class, source);  
-             stringConverterDef.getPropertyValues().add("writeAcceptCharset", false);  
-             messageConverters.add(stringConverterDef);  
- 
-             messageConverters.add(createConverterDefinition(ResourceHttpMessageConverter.class, source));  
-             messageConverters.add(createConverterDefinition(SourceHttpMessageConverter.class, source));  
-             messageConverters.add(createConverterDefinition(AllEncompassingFormHttpMessageConverter.class, source));  
- 
- if (romePresent) {  
-                 messageConverters.add(createConverterDefinition(AtomFeedHttpMessageConverter.class, source));  
-                 messageConverters.add(createConverterDefinition(RssChannelHttpMessageConverter.class, source));  
-             }  
- if (jaxb2Present) {  
-                 messageConverters.add(createConverterDefinition(Jaxb2RootElementHttpMessageConverter.class, source));  
-             }  
- if (jackson2Present) {  
-                 messageConverters.add(createConverterDefinition(MappingJackson2HttpMessageConverter.class, source));  
-             }  
- elseif (jacksonPresent) {  
-                 messageConverters.add(createConverterDefinition(  
-                         org.springframework.http.converter.json.MappingJacksonHttpMessageConverter.class, source));  
-             }  
-         }  
- return messageConverters;  
-     }  
该过程第一步： 
解析并获取我们自定义的HttpMessageConverter， 
该过程第二步： 
<mvc:message-converters register-defaults="true">有一个register-defaults属性，当为true时，仍然注册默认的HttpMessageConverter，当为false则不注册，仅仅使用用户自定义的HttpMessageConverter。 
获取完毕，便会将这些HttpMessageConverter设置进RequestMappingHandlerAdapter的messageConverters属性中。 
然后就是它的使用过程，HttpMessageConverter主要针对那些不会返回view视图的response： 
即含有方法含有@ResponseBody或者返回值为HttpEntity等类型的，它们都会用到HttpMessageConverter。以@ResponseBody举例： 
在Spring中要用注解就必须要在xml中配置两个bean，分别是RequestMappingHandlerAdapter和RequestMappingHandlerMapping两个类，前者是注册注解处理器的后者是进行注解映射的，里面都封装了Spring默认的一些注解处理器和返回值处理器
```
private List<HandlerMethodArgumentResolver> getDefaultArgumentResolvers() {
        List<HandlerMethodArgumentResolver> resolvers = new ArrayList<HandlerMethodArgumentResolver>();
        // Annotation-based argument resolution
        resolvers.add(new RequestParamMethodArgumentResolver(getBeanFactory(), false));
        resolvers.add(new RequestParamMapMethodArgumentResolver());
        resolvers.add(new PathVariableMethodArgumentResolver());
        resolvers.add(new PathVariableMapMethodArgumentResolver());
        resolvers.add(new MatrixVariableMethodArgumentResolver());
        resolvers.add(new MatrixVariableMapMethodArgumentResolver());
        resolvers.add(new ServletModelAttributeMethodProcessor(false));
        **resolvers.add(new RequestResponseBodyMethodProcessor(getMessageConverters()));**
```
```
private List<HandlerMethodReturnValueHandler> getDefaultReturnValueHandlers() {
        List<HandlerMethodReturnValueHandler> handlers = new ArrayList<HandlerMethodReturnValueHandler>();
        // Single-purpose return value types
        handlers.add(new ModelAndViewMethodReturnValueHandler());
        handlers.add(new ModelMethodProcessor());
        handlers.add(new ViewMethodReturnValueHandler());
        handlers.add(new HttpEntityMethodProcessor(getMessageConverters(), this.contentNegotiationManager));
        handlers.add(new HttpHeadersReturnValueHandler());
        handlers.add(new CallableMethodReturnValueHandler());
        handlers.add(new DeferredResultMethodReturnValueHandler());
        handlers.add(new AsyncTaskMethodReturnValueHandler(this.beanFactory));
        // Annotation-based return value types
        handlers.add(new ModelAttributeMethodProcessor(false));
        handlers.add(new RequestResponseBodyMethodProcessor(getMessageConverters(), this.contentNegotiationManager));
```
上面的代码是RequestMappingHandlerAdapter中Spring默认的注解处理器（resolvers）和返回值处理器（handlers）集合，RequestMappingHandlerAdapter和RequestMappingHandlerMapping都在org.springframework.web.servlet.mvc.method.annotation包下面， RequestMappingHandlerAdapter不仅仅是定义注解处理器和返回值处理器，还可以定义消息转换器（messageconverters）和视图模型转换器（modelAndViewResolvers）等等，大家可以自己去看一下代码， 
首先先决定由哪个HandlerMethodReturnValueHandler来处理返回值，由于是@ResponseBody所以将会由RequestResponseBodyMethodProcessor来处理（**上述代码中，红色行为该处理器注入了所有messageConverters**），然后就是如下的写入：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protected <T> void writeWithMessageConverters(T returnValue, MethodParameter returnType,  
-             ServletServerHttpRequest inputMessage, ServletServerHttpResponse outputMessage)  
- throws IOException, HttpMediaTypeNotAcceptableException {  
- 
-         Class<?> returnValueClass = returnValue.getClass();  
-         HttpServletRequest servletRequest = inputMessage.getServletRequest();  
-         List<MediaType> requestedMediaTypes = getAcceptableMediaTypes(servletRequest);  
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
- for (HttpMessageConverter<?> messageConverter : this.messageConverters) {  
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
选取一个合适的content-type，再由这个content-type和返回类型来选取合适的HttpMessageConverter，找到合适的HttpMessageConverter后，便调用它的write方法。 
接下来就说一说一些具体的HttpMessageConverter。 
AbstractHttpMessageConverter：提供了进一步的抽象，将是否支持相应的MediaType这一共有的功能实现，它的子类只需关心是否支持返回类型。 
AbstractHttpMessageConverter子类-StringHttpMessageConverter：如用于处理字符串到response中，这就要涉及编码问题，这一过程在本系列的第四篇文章中做过详细说明，这里跳过。 
AbstractHttpMessageConverter子类-ByteArrayHttpMessageConverter：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass ByteArrayHttpMessageConverter extends AbstractHttpMessageConverter<byte[]> {  
- 
- /** Creates a new instance of the {@code ByteArrayHttpMessageConverter}. */
- public ByteArrayHttpMessageConverter() {  
- super(new MediaType("application", "octet-stream"), MediaType.ALL);  
-     }  
- 
- @Override
- publicboolean supports(Class<?> clazz) {  
- returnbyte[].class.equals(clazz);  
-     }  
- 
- @Override
- publicbyte[] readInternal(Class<? extendsbyte[]> clazz, HttpInputMessage inputMessage) throws IOException {  
- long contentLength = inputMessage.getHeaders().getContentLength();  
-         ByteArrayOutputStream bos =  
- new ByteArrayOutputStream(contentLength >= 0 ? (int) contentLength : StreamUtils.BUFFER_SIZE);  
-         StreamUtils.copy(inputMessage.getBody(), bos);  
- return bos.toByteArray();  
-     }  
- 
- @Override
- protected Long getContentLength(byte[] bytes, MediaType contentType) {  
- return (long) bytes.length;  
-     }  
- 
- @Override
- protectedvoid writeInternal(byte[] bytes, HttpOutputMessage outputMessage) throws IOException {  
-         StreamUtils.copy(bytes, outputMessage.getBody());  
-     }  
- 
- }  
源码就很清晰明了。它专门负责byte[]类型的转换。 
AbstractHttpMessageConverter子类-MappingJacksonHttpMessageConverter：用于转换Object到json字符串类型。已过时,使用的是http://jackson.codehaus.org中Jackson 1.x的ObjectMapper，取代者为MappingJackson2HttpMessageConverter。依赖为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <dependency>   
-         <groupId>org.codehaus.jackson</groupId>   
-         <artifactId>jackson-core-asl</artifactId>   
-         <version>1.9.11</version>   
-     </dependency>   
- 
-     <dependency>   
-         <groupId>org.codehaus.jackson</groupId>   
-         <artifactId>jackson-mapper-asl</artifactId>   
-         <version>1.9.11</version>   
-     </dependency>   
- 
AbstractHttpMessageConverter子类-MappingJackson2HttpMessageConverter： 
它所使用的json转换器是http://jackson.codehaus.org中Jackson 2.x的ObjectMapper。 
依赖的jar包为有3个，jackson-databind和它的两个依赖jackson-annotations、jackson-core，但是有了jackson-databind的pom文件会去自动下载它的依赖，所以只需增添jackson-databind的pom即可获取上述3个jar包：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <dependency>  
-     <dependency>  
-         <groupId>com.fasterxml.jackson.core</groupId>  
-         <artifactId>jackson-databind</artifactId>  
-         <version>2.4.2</version>   
-     </dependency>  
接下来便说道：在注册HttpMessageConverter过程中的一些问题：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- if (convertersElement == null || Boolean.valueOf(convertersElement.getAttribute("register-defaults"))) {  
-             messageConverters.setSource(source);  
-             messageConverters.add(createConverterDefinition(ByteArrayHttpMessageConverter.class, source));  
- 
-             RootBeanDefinition stringConverterDef = createConverterDefinition(StringHttpMessageConverter.class, source);  
-             stringConverterDef.getPropertyValues().add("writeAcceptCharset", false);  
-             messageConverters.add(stringConverterDef);  
- 
-             messageConverters.add(createConverterDefinition(ResourceHttpMessageConverter.class, source));  
-             messageConverters.add(createConverterDefinition(SourceHttpMessageConverter.class, source));  
-             messageConverters.add(createConverterDefinition(AllEncompassingFormHttpMessageConverter.class, source));  
- 
- if (romePresent) {  
-                 messageConverters.add(createConverterDefinition(AtomFeedHttpMessageConverter.class, source));  
-                 messageConverters.add(createConverterDefinition(RssChannelHttpMessageConverter.class, source));  
-             }  
- if (jaxb2Present) {  
-                 messageConverters.add(createConverterDefinition(Jaxb2RootElementHttpMessageConverter.class, source));  
-             }  
- if (jackson2Present) {  
-                 messageConverters.add(createConverterDefinition(MappingJackson2HttpMessageConverter.class, source));  
-             }  
- elseif (jacksonPresent) {  
-                 messageConverters.add(createConverterDefinition(  
-                         org.springframework.http.converter.json.MappingJacksonHttpMessageConverter.class, source));  
-             }  
-         }  
这段代码是在注册默认的HttpMessageConverter，但是个别HttpMessageConverter也是有条件的。即相应的jar包存在，才会去注册它。如MappingJackson2HttpMessageConverter，if (jackson2Present) { 
messageConverters.add(createConverterDefinition(MappingJackson2HttpMessageConverter.class, source));当jackson2Present为true时才会注册。而jackson2Present的值如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatestaticfinalboolean jackson2Present =  
-             ClassUtils.isPresent("com.fasterxml.jackson.databind.ObjectMapper", AnnotationDrivenBeanDefinitionParser.class.getClassLoader()) &&  
-                     ClassUtils.isPresent("com.fasterxml.jackson.core.JsonGenerator", AnnotationDrivenBeanDefinitionParser.class.getClassLoader());  
也就是当com.fasterxml.jackson.databind.ObjectMapper和com.fasterxml.jackson.core.JsonGenerator存在在classpath中才会去加载MappingJackson2HttpMessageConverter。 
同理，MappingJacksonHttpMessageConverter的判断如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- privatestaticfinalboolean jacksonPresent =  
-             ClassUtils.isPresent("org.codehaus.jackson.map.ObjectMapper", AnnotationDrivenBeanDefinitionParser.class.getClassLoader()) &&  
-                     ClassUtils.isPresent("org.codehaus.jackson.JsonGenerator", AnnotationDrivenBeanDefinitionParser.class.getClassLoader());  
所以当我们程序没法转换json时，你就需要考虑是否已经把MappingJacksonHttpMessageConverter或者MappingJackson2HttpMessageConverter的依赖加进来了，官方推荐使用MappingJackson2HttpMessageConverter。
