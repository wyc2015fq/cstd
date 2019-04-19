# SpringMVC 4.1使用ResponseBodyAdvice支持jsonp - 零度的博客专栏 - CSDN博客
2017年01月04日 11:57:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：8840
ResponseBodyAdvice是一个接口，接口描述，
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52290032#)[copy](http://blog.csdn.net/z69183787/article/details/52290032#)[print](http://blog.csdn.net/z69183787/article/details/52290032#)[?](http://blog.csdn.net/z69183787/article/details/52290032#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1849373/fork)
- package org.springframework.web.servlet.mvc.method.annotation;  
- 
- /**
-  * Allows customizing the response after the execution of an {@code @ResponseBody}
-  * or an {@code ResponseEntity} controller method but before the body is written
-  * with an {@code HttpMessageConverter}.
-  *
-  * <p>Implementations may be may be registered directly with
-  * {@code RequestMappingHandlerAdapter} and {@code ExceptionHandlerExceptionResolver}
-  * or more likely annotated with {@code @ControllerAdvice} in which case they
-  * will be auto-detected by both.
-  *
-  * @author Rossen Stoyanchev
-  * @since 4.1
-  */
- publicinterface ResponseBodyAdvice<T> {  
- 
- /**
-     * Whether this component supports the given controller method return type
-     * and the selected {@code HttpMessageConverter} type.
-     * @param returnType the return type
-     * @param converterType the selected converter type
-     * @return {@code true} if {@link #beforeBodyWrite} should be invoked, {@code false} otherwise
-     */
- boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType);  
- 
- /**
-     * Invoked after an {@code HttpMessageConverter} is selected and just before
-     * its write method is invoked.
-     * @param body the body to be written
-     * @param returnType the return type of the controller method
-     * @param selectedContentType the content type selected through content negotiation
-     * @param selectedConverterType the converter type selected to write to the response
-     * @param request the current request
-     * @param response the current response
-     * @return the body that was passed in or a modified, possibly new instance
-     */
-    T beforeBodyWrite(T body, MethodParameter returnType, MediaType selectedContentType,  
-          Class<? extends HttpMessageConverter<?>> selectedConverterType,  
-          ServerHttpRequest request, ServerHttpResponse response);  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package org.springframework.web.servlet.mvc.method.annotation;
/**
 * Allows customizing the response after the execution of an {@code @ResponseBody}
 * or an {@code ResponseEntity} controller method but before the body is written
 * with an {@code HttpMessageConverter}.
 *
 * <p>Implementations may be may be registered directly with
 * {@code RequestMappingHandlerAdapter} and {@code ExceptionHandlerExceptionResolver}
 * or more likely annotated with {@code @ControllerAdvice} in which case they
 * will be auto-detected by both.
 *
 * @author Rossen Stoyanchev
 * @since 4.1
 */
public interface ResponseBodyAdvice<T> {
   /**
    * Whether this component supports the given controller method return type
    * and the selected {@code HttpMessageConverter} type.
    * @param returnType the return type
    * @param converterType the selected converter type
    * @return {@code true} if {@link #beforeBodyWrite} should be invoked, {@code false} otherwise
    */
   boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType);
   /**
    * Invoked after an {@code HttpMessageConverter} is selected and just before
    * its write method is invoked.
    * @param body the body to be written
    * @param returnType the return type of the controller method
    * @param selectedContentType the content type selected through content negotiation
    * @param selectedConverterType the converter type selected to write to the response
    * @param request the current request
    * @param response the current response
    * @return the body that was passed in or a modified, possibly new instance
    */
   T beforeBodyWrite(T body, MethodParameter returnType, MediaType selectedContentType,
         Class<? extends HttpMessageConverter<?>> selectedConverterType,
         ServerHttpRequest request, ServerHttpResponse response);
}
```
作用：
> 
Allows customizing the response after the execution of an {@code @ResponseBody} or an {@code ResponseEntity} controller method but before the body is written
with an {@code HttpMessageConverter}.
其中一个方法就是 beforeBodyWrite 在使用相应的HttpMessageConvert 进行write之前会被调用，就是一个切面方法。
和jsonp有关的实现类是AbstractJsonpResponseBodyAdvice，如下是 beforeBodyWrite 方法的实现，
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52290032#)[copy](http://blog.csdn.net/z69183787/article/details/52290032#)[print](http://blog.csdn.net/z69183787/article/details/52290032#)[?](http://blog.csdn.net/z69183787/article/details/52290032#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1849373/fork)
- @Override
- publicfinal Object beforeBodyWrite(Object body, MethodParameter returnType,  
-       MediaType contentType, Class<? extends HttpMessageConverter<?>> converterType,  
-       ServerHttpRequest request, ServerHttpResponse response) {  
- 
-    MappingJacksonValue container = getOrCreateContainer(body);  
-    beforeBodyWriteInternal(container, contentType, returnType, request, response);  
- return container;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Override
public final Object beforeBodyWrite(Object body, MethodParameter returnType,
      MediaType contentType, Class<? extends HttpMessageConverter<?>> converterType,
      ServerHttpRequest request, ServerHttpResponse response) {
   MappingJacksonValue container = getOrCreateContainer(body);
   beforeBodyWriteInternal(container, contentType, returnType, request, response);
   return container;
}
```
位于AbstractJsonpResponseBodyAdvice的父类中，而beforeBodyWriteInternal是在AbstractJsonpResponseBodyAdvice中实现的 ，如下，
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52290032#)[copy](http://blog.csdn.net/z69183787/article/details/52290032#)[print](http://blog.csdn.net/z69183787/article/details/52290032#)[?](http://blog.csdn.net/z69183787/article/details/52290032#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1849373/fork)
- @Override
- protectedvoid beforeBodyWriteInternal(MappingJacksonValue bodyContainer, MediaType contentType,  
-       MethodParameter returnType, ServerHttpRequest request, ServerHttpResponse response) {  
- 
-    HttpServletRequest servletRequest = ((ServletServerHttpRequest) request).getServletRequest();  
- 
- for (String name : this.jsonpQueryParamNames) {  
-       String value = servletRequest.getParameter(name);  
- if (value != null) {  
-          MediaType contentTypeToUse = getContentType(contentType, request, response);  
-          response.getHeaders().setContentType(contentTypeToUse);  
-          bodyContainer.setJsonpFunction(value);  
- return;  
-       }  
-    }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Override
protected void beforeBodyWriteInternal(MappingJacksonValue bodyContainer, MediaType contentType,
      MethodParameter returnType, ServerHttpRequest request, ServerHttpResponse response) {
   HttpServletRequest servletRequest = ((ServletServerHttpRequest) request).getServletRequest();
   for (String name : this.jsonpQueryParamNames) {
      String value = servletRequest.getParameter(name);
      if (value != null) {
         MediaType contentTypeToUse = getContentType(contentType, request, response);
         response.getHeaders().setContentType(contentTypeToUse);
         bodyContainer.setJsonpFunction(value);
         return;
      }
   }
}
```
就是根据callback 请求参数或配置的其他参数来确定返回jsonp协议的数据。
如何实现jsonp？
首先继承AbstractJsonpResponseBodyAdvice ，如下，
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52290032#)[copy](http://blog.csdn.net/z69183787/article/details/52290032#)[print](http://blog.csdn.net/z69183787/article/details/52290032#)[?](http://blog.csdn.net/z69183787/article/details/52290032#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1849373/fork)
- package com.usoft.web.controller.jsonp;  
- 
- import org.springframework.web.bind.annotation.ControllerAdvice;  
- import org.springframework.web.servlet.mvc.method.annotation.AbstractJsonpResponseBodyAdvice;  
- 
- /**
-  * 
-  */
- @ControllerAdvice(basePackages = "com.usoft.web.controller.jsonp")  
- publicclass JsonpAdvice extends AbstractJsonpResponseBodyAdvice {  
- public JsonpAdvice() {  
- super("callback", "jsonp");  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.usoft.web.controller.jsonp;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.servlet.mvc.method.annotation.AbstractJsonpResponseBodyAdvice;
/**
 * 
 */
@ControllerAdvice(basePackages = "com.usoft.web.controller.jsonp")
public class JsonpAdvice extends AbstractJsonpResponseBodyAdvice {
    public JsonpAdvice() {
        super("callback", "jsonp");
    }
}
```
super("callback", "jsonp");的意思就是当请求参数中包含callback 或 jsonp参数时，就会返回jsonp协议的数据。其value就作为回调函数的名称。
这里必须使用@ControllerAdvice注解标注该类，并且配置对哪些Controller起作用。关于注解@ControllerAdvice 的作用这里不做描述。
Controller实现jsonp，
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52290032#)[copy](http://blog.csdn.net/z69183787/article/details/52290032#)[print](http://blog.csdn.net/z69183787/article/details/52290032#)[?](http://blog.csdn.net/z69183787/article/details/52290032#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1849373/fork)
- package com.usoft.web.controller.jsonp;  
- 
- import org.springframework.stereotype.Controller;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import org.springframework.web.bind.annotation.ResponseBody;  
- 
- import com.usoft.web.controller.JsonMapper;  
- import com.usoft.web.controller.Person;  
- 
- /**
-  * jsonp
-  */
- @Controller
- publicclass JsonpController {  
- 
- /**
-      * callback({"id":1,"age":12,"name":"lyx"})
-      * 
-      * @param args
-      */
- publicstaticvoid main(String args[]) {  
-         Person person = new Person(1, "lyx", 12);  
-         System.out.println(JsonMapper.nonNullMapper().toJsonP("callback",  
-             person));  
-     }  
- 
- @RequestMapping("/jsonp1")  
- public Person jsonp1() {  
- returnnew Person(1, "lyx", 12);  
-     }  
- 
- @RequestMapping("/jsonp2")  
- @ResponseBody
- public Person jsonp2() {  
- returnnew Person(1, "lyx", 12);  
-     }  
- 
- @RequestMapping("/jsonp3")  
- @ResponseBody
- public String jsonp3() {  
- return JsonMapper.nonNullMapper().toJsonP("callback",  
- new Person(1, "lyx", 12));  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.usoft.web.controller.jsonp;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import com.usoft.web.controller.JsonMapper;
import com.usoft.web.controller.Person;
/**
 * jsonp
 */
@Controller
public class JsonpController {
    /**
     * callback({"id":1,"age":12,"name":"lyx"})
     * 
     * @param args
     */
    public static void main(String args[]) {
        Person person = new Person(1, "lyx", 12);
        System.out.println(JsonMapper.nonNullMapper().toJsonP("callback",
            person));
    }
    @RequestMapping("/jsonp1")
    public Person jsonp1() {
        return new Person(1, "lyx", 12);
    }
    @RequestMapping("/jsonp2")
    @ResponseBody
    public Person jsonp2() {
        return new Person(1, "lyx", 12);
    }
    @RequestMapping("/jsonp3")
    @ResponseBody
    public String jsonp3() {
        return JsonMapper.nonNullMapper().toJsonP("callback",
            new Person(1, "lyx", 12));
    }
}
```
jsonp2 方法就是 一个jsonp协议的调用。http://localhost:8081/jsonp2?callback=test可以直接调用这个方法，并且返回jsonp协议的数据。
通过debug代码，我们来看一下他是怎么返回jsonp协议的数据的。
正因为我们前面在 该Controller 上配置了 JsonpAdvice 的 ControllerAdvice，在调用 MappingJackson2HttpMessageConverter的write（）方法往回写数据的时候，首先会调用
beforeBodyWrite，具体的代码如下，
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52290032#)[copy](http://blog.csdn.net/z69183787/article/details/52290032#)[print](http://blog.csdn.net/z69183787/article/details/52290032#)[?](http://blog.csdn.net/z69183787/article/details/52290032#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1849373/fork)
- @Override
- protectedvoid beforeBodyWriteInternal(MappingJacksonValue bodyContainer, MediaType contentType,  
-       MethodParameter returnType, ServerHttpRequest request, ServerHttpResponse response) {  
- 
-    HttpServletRequest servletRequest = ((ServletServerHttpRequest) request).getServletRequest();  
- 
- for (String name : this.jsonpQueryParamNames) {  
-       String value = servletRequest.getParameter(name);  
- if (value != null) {  
-          MediaType contentTypeToUse = getContentType(contentType, request, response);  
-          response.getHeaders().setContentType(contentTypeToUse);  
-          bodyContainer.setJsonpFunction(value);  
- return;  
-       }  
-    }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Override
protected void beforeBodyWriteInternal(MappingJacksonValue bodyContainer, MediaType contentType,
      MethodParameter returnType, ServerHttpRequest request, ServerHttpResponse response) {
   HttpServletRequest servletRequest = ((ServletServerHttpRequest) request).getServletRequest();
   for (String name : this.jsonpQueryParamNames) {
      String value = servletRequest.getParameter(name);
      if (value != null) {
         MediaType contentTypeToUse = getContentType(contentType, request, response);
         response.getHeaders().setContentType(contentTypeToUse);
         bodyContainer.setJsonpFunction(value);
         return;
      }
   }
}
```
当请求参数中含有配置的相应的回调参数时，就会bodyContainer.setJsonpFunction(value);这就标志着 返回的数据时jsonp格式的数据。
然后接下来就到了 MappingJackson2HttpMessageConverter 的write（）方法真正写数据的时候了。看他是怎么写数据的，相关的代码如下，
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52290032#)[copy](http://blog.csdn.net/z69183787/article/details/52290032#)[print](http://blog.csdn.net/z69183787/article/details/52290032#)[?](http://blog.csdn.net/z69183787/article/details/52290032#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1849373/fork)
- @Override
- protectedvoid writeInternal(Object object, HttpOutputMessage outputMessage)  
- throws IOException, HttpMessageNotWritableException {  
- 
-    JsonEncoding encoding = getJsonEncoding(outputMessage.getHeaders().getContentType());  
-    JsonGenerator generator = this.objectMapper.getFactory().createGenerator(outputMessage.getBody(), encoding);  
- try {  
-       writePrefix(generator, object);  
-       Class<?> serializationView = null;  
-       Object value = object;  
- if (value instanceof MappingJacksonValue) {  
-          MappingJacksonValue container = (MappingJacksonValue) object;  
-          value = container.getValue();  
-          serializationView = container.getSerializationView();  
-       }  
- if (serializationView != null) {  
- this.objectMapper.writerWithView(serializationView).writeValue(generator, value);  
-       }  
- else {  
- this.objectMapper.writeValue(generator, value);  
-       }  
-       writeSuffix(generator, object);  
-       generator.flush();  
- 
-    }  
- catch (JsonProcessingException ex) {  
- thrownew HttpMessageNotWritableException("Could not write content: " + ex.getMessage(), ex);  
-    }  
- }  
- @Override
- protectedvoid writePrefix(JsonGenerator generator, Object object) throws IOException {  
- if (this.jsonPrefix != null) {  
-       generator.writeRaw(this.jsonPrefix);  
-    }  
-    String jsonpFunction =  
-          (object instanceof MappingJacksonValue ? ((MappingJacksonValue) object).getJsonpFunction() : null);  
- if (jsonpFunction != null) {  
-       generator.writeRaw(jsonpFunction + "(");  
-    }  
- }  
- @Override
- protectedvoid writeSuffix(JsonGenerator generator, Object object) throws IOException {  
-    String jsonpFunction =  
-          (object instanceof MappingJacksonValue ? ((MappingJacksonValue) object).getJsonpFunction() : null);  
- if (jsonpFunction != null) {  
-       generator.writeRaw(");");  
-    }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Override
protected void writeInternal(Object object, HttpOutputMessage outputMessage)
      throws IOException, HttpMessageNotWritableException {
   JsonEncoding encoding = getJsonEncoding(outputMessage.getHeaders().getContentType());
   JsonGenerator generator = this.objectMapper.getFactory().createGenerator(outputMessage.getBody(), encoding);
   try {
      writePrefix(generator, object);
      Class<?> serializationView = null;
      Object value = object;
      if (value instanceof MappingJacksonValue) {
         MappingJacksonValue container = (MappingJacksonValue) object;
         value = container.getValue();
         serializationView = container.getSerializationView();
      }
      if (serializationView != null) {
         this.objectMapper.writerWithView(serializationView).writeValue(generator, value);
      }
      else {
         this.objectMapper.writeValue(generator, value);
      }
      writeSuffix(generator, object);
      generator.flush();
   }
   catch (JsonProcessingException ex) {
      throw new HttpMessageNotWritableException("Could not write content: " + ex.getMessage(), ex);
   }
}
@Override
protected void writePrefix(JsonGenerator generator, Object object) throws IOException {
   if (this.jsonPrefix != null) {
      generator.writeRaw(this.jsonPrefix);
   }
   String jsonpFunction =
         (object instanceof MappingJacksonValue ? ((MappingJacksonValue) object).getJsonpFunction() : null);
   if (jsonpFunction != null) {
      generator.writeRaw(jsonpFunction + "(");
   }
}
@Override
protected void writeSuffix(JsonGenerator generator, Object object) throws IOException {
   String jsonpFunction =
         (object instanceof MappingJacksonValue ? ((MappingJacksonValue) object).getJsonpFunction() : null);
   if (jsonpFunction != null) {
      generator.writeRaw(");");
   }
}
```
代码非常清晰。看我们jsonp调用的结果。
http://localhost:8081/jsonp2?callback=test
响应消息如下，
> 
HTTP/1.1 200 OK
Server: Apache-Coyote/1.1
Content-Type: application/javascript
Transfer-Encoding: chunked
Date: Sun, 19 Jul 2015 13:01:02 GMT
test({"id":1,"age":12,"name":"lyx"});
