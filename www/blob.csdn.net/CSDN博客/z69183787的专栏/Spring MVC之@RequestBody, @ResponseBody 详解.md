# Spring MVC之@RequestBody, @ResponseBody 详解 - z69183787的专栏 - CSDN博客
2014年06月27日 14:51:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1841
## 引言：
接上一篇文章讲述处理@RequestMapping的方法参数绑定之后，详细介绍下@RequestBody、@ResponseBody的具体用法和使用时机；
## 简介：
### @RequestBody
作用： 
      i) 该注解用于读取Request请求的body部分数据，使用系统默认配置的HttpMessageConverter进行解析，然后把相应的数据绑定到要返回的对象上；
      ii) 再把HttpMessageConverter返回的对象数据绑定到 controller中方法的参数上。
使用时机：
A) GET、POST方式提时， 根据request header Content-Type的值来判断:
-     application/x-www-form-urlencoded， 可选（即非必须，因为这种情况的数据@RequestParam, @ModelAttribute也可以处理，当然@RequestBody也能处理）；
-     multipart/form-data, 不能处理（即使用@RequestBody不能处理这种格式的数据）；
-     其他格式， 必须（其他格式包括application/json, application/xml等。这些格式的数据，必须使用@RequestBody来处理）；
B) PUT方式提交时， 根据request header Content-Type的值来判断:
-     application/x-www-form-urlencoded， 必须；
-     multipart/form-data, 不能处理；
-     其他格式， 必须；
说明：request的body部分的数据编码格式由header部分的Content-Type指定；
### @ResponseBody
作用： 
      该注解用于将Controller的方法返回的对象，通过适当的HttpMessageConverter转换为指定格式后，写入到Response对象的body数据区。
使用时机：
      返回的数据不是html标签的页面，而是其他某种格式的数据时（如json、xml等）使用；
## HttpMessageConverter
**[java]**[view
 plain](http://blog.csdn.net/walkerjong/article/details/7520896#)[copy](http://blog.csdn.net/walkerjong/article/details/7520896#)
- <span style="font-family:Microsoft YaHei;">/**
-  * Strategy interface that specifies a converter that can convert from and to HTTP requests and responses.
-  *
-  * @author Arjen Poutsma
-  * @author Juergen Hoeller
-  * @since 3.0
-  */
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
- </span>  
该接口定义了四个方法，分别是读取数据时的 canRead(), read() 和 写入数据时的canWrite(), write()方法。
在使用 <mvc:annotation-driven />标签配置时，默认配置了
```
RequestMappingHandlerAdapter（注意是`RequestMappingHandlerAdapter`不是AnnotationMethodHandlerAdapter,详情查看Spring 3.1 document “16.14 Configuring
 Spring MVC”章节），并为他配置了一下默认的HttpMessageConverter：
```
``
**[java]**[view
 plain](http://blog.csdn.net/walkerjong/article/details/7520896#)[copy](http://blog.csdn.net/walkerjong/article/details/7520896#)
- ByteArrayHttpMessageConverter converts byte arrays.  
- 
- StringHttpMessageConverter converts strings.  
- 
- ResourceHttpMessageConverter converts to/from org.springframework.core.io.Resource for all media types.  
- 
- SourceHttpMessageConverter converts to/from a javax.xml.transform.Source.  
- 
- FormHttpMessageConverter converts form data to/from a MultiValueMap<String, String>.  
- 
- Jaxb2RootElementHttpMessageConverter converts Java objects to/from XML — added if JAXB2 is present on the classpath.  
- 
- MappingJacksonHttpMessageConverter converts to/from JSON — added if Jackson is present on the classpath.  
- 
- AtomFeedHttpMessageConverter converts Atom feeds — added if Rome is present on the classpath.  
- 
- RssChannelHttpMessageConverter converts RSS feeds — added if Rome is present on the classpath.  
`ByteArrayHttpMessageConverter: 负责读取二进制格式的数据和写出二进制格式的数据；`
`StringHttpMessageConverter：   负责读取字符串格式的数据和写出二进制格式的数据；`
ResourceHttpMessageConverter：负责读取资源文件和写出资源文件数据； 
``
FormHttpMessageConverter：       负责读取form提交的数据（能读取的数据格式为 application/x-www-form-urlencoded，不能读取multipart/form-data格式数据）；负责写入application/x-www-from-urlencoded和multipart/form-data格式的数据；
MappingJacksonHttpMessageConverter:  负责读取和写入json格式的数据；
SouceHttpMessageConverter：                   负责读取和写入 xml 中javax.xml.transform.Source定义的数据；
Jaxb2RootElementHttpMessageConverter:  负责读取和写入xml 标签格式的数据；
AtomFeedHttpMessageConverter:              负责读取和写入Atom格式的数据；
RssChannelHttpMessageConverter:           负责读取和写入RSS格式的数据；
`当使用@RequestBody和@ResponseBody注解时，`RequestMappingHandlerAdapter`就使用它们来进行读取或者写入相应格式的数据。`
```
```
## HttpMessageConverter匹配过程：
@RequestBody注解时： 根据Request对象header部分的Content-Type类型，逐一匹配合适的HttpMessageConverter来读取数据；
spring 3.1源代码如下：
**[java]**[view
 plain](http://blog.csdn.net/walkerjong/article/details/7520896#)[copy](http://blog.csdn.net/walkerjong/article/details/7520896#)
- <span style="font-family:Microsoft YaHei;">private Object readWithMessageConverters(MethodParameter methodParam, HttpInputMessage inputMessage, Class paramType)  
- throws Exception {  
- 
-         MediaType contentType = inputMessage.getHeaders().getContentType();  
- if (contentType == null) {  
-             StringBuilder builder = new StringBuilder(ClassUtils.getShortName(methodParam.getParameterType()));  
-             String paramName = methodParam.getParameterName();  
- if (paramName != null) {  
-                 builder.append(' ');  
-                 builder.append(paramName);  
-             }  
- thrownew HttpMediaTypeNotSupportedException(  
- "Cannot extract parameter (" + builder.toString() + "): no Content-Type found");  
-         }  
- 
-         List<MediaType> allSupportedMediaTypes = new ArrayList<MediaType>();  
- if (this.messageConverters != null) {  
- for (HttpMessageConverter<?> messageConverter : this.messageConverters) {  
-                 allSupportedMediaTypes.addAll(messageConverter.getSupportedMediaTypes());  
- if (messageConverter.canRead(paramType, contentType)) {  
- if (logger.isDebugEnabled()) {  
-                         logger.debug("Reading [" + paramType.getName() + "] as \"" + contentType  
-                                 +"\" using [" + messageConverter + "]");  
-                     }  
- return messageConverter.read(paramType, inputMessage);  
-                 }  
-             }  
-         }  
- thrownew HttpMediaTypeNotSupportedException(contentType, allSupportedMediaTypes);  
-     }</span>  
@ResponseBody注解时： 根据Request对象header部分的Accept属性（逗号分隔），逐一按accept中的类型，去遍历找到能处理的HttpMessageConverter；
源代码如下：
**[java]**[view
 plain](http://blog.csdn.net/walkerjong/article/details/7520896#)[copy](http://blog.csdn.net/walkerjong/article/details/7520896#)
- <span style="font-family:Microsoft YaHei;">privatevoid writeWithMessageConverters(Object returnValue,  
-                 HttpInputMessage inputMessage, HttpOutputMessage outputMessage)  
- throws IOException, HttpMediaTypeNotAcceptableException {  
-             List<MediaType> acceptedMediaTypes = inputMessage.getHeaders().getAccept();  
- if (acceptedMediaTypes.isEmpty()) {  
-                 acceptedMediaTypes = Collections.singletonList(MediaType.ALL);  
-             }  
-             MediaType.sortByQualityValue(acceptedMediaTypes);  
-             Class<?> returnValueType = returnValue.getClass();  
-             List<MediaType> allSupportedMediaTypes = new ArrayList<MediaType>();  
- if (getMessageConverters() != null) {  
- for (MediaType acceptedMediaType : acceptedMediaTypes) {  
- for (HttpMessageConverter messageConverter : getMessageConverters()) {  
- if (messageConverter.canWrite(returnValueType, acceptedMediaType)) {  
-                             messageConverter.write(returnValue, acceptedMediaType, outputMessage);  
- if (logger.isDebugEnabled()) {  
-                                 MediaType contentType = outputMessage.getHeaders().getContentType();  
- if (contentType == null) {  
-                                     contentType = acceptedMediaType;  
-                                 }  
-                                 logger.debug("Written [" + returnValue + "] as \"" + contentType +  
- "\" using [" + messageConverter + "]");  
-                             }  
- this.responseArgumentUsed = true;  
- return;  
-                         }  
-                     }  
-                 }  
- for (HttpMessageConverter messageConverter : messageConverters) {  
-                     allSupportedMediaTypes.addAll(messageConverter.getSupportedMediaTypes());  
-                 }  
-             }  
- thrownew HttpMediaTypeNotAcceptableException(allSupportedMediaTypes);  
-         }</span>  
## 补充：
MappingJacksonHttpMessageConverter 调用了 objectMapper.writeValue(OutputStream stream, Object)方法，使用@ResponseBody注解返回的对象就传入Object参数内。若返回的对象为已经格式化好的json串时，不使用@RequestBody注解，而应该这样处理：1、response.setContentType("application/json; charset=UTF-8");2、response.getWriter().print(jsonStr);直接输出到body区，然后的视图为void。
