# SpringMVC中使用@RequestBody,@ResponseBody注解实现Java对象和XML/JSON数据自动转换（上） - z69183787的专栏 - CSDN博客
2016年10月11日 11:45:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1330
Spring3.1开始使用新的HandlerMapping 和 HandlerAdapter 来支持@Contoller 和@RequestMapping注解处理:处理器映射RequestMappingHandlerMapping和处理器适配器RequestMappingHandlerAdapter组合来代替Spring2.5 开始的处理器映射DefaultAnnotationHandlerMapping和处理器适配器AnnotationMethodHandlerAdapter。
HandlerMapping：请求到处理器的映射，如果映射成功返回一个HandlerExecutionChain 对象（包含一个Handler处理器（页面控制器）对象、多个HandlerInterceptor 拦截器）对象；
HandlerAdapter：HandlerAdapter 将会把处理器包装为适配器，从而支持多种类型的处理器，即适配器设计模式的应用，从而很容易支持很多类型的处理器。
**配合@ResponseBody注解，以及HTTP Request Header中的Accept属性，Controller返回的[Java](http://lib.csdn.net/base/javaee)对象可以自动被转换成对应的XML或者JSON数据。**
先看一个例子，只需要简单的几步，就可以返回XML数据。（本文使用[spring](http://lib.csdn.net/base/javaee)版本 4.1.6，并使用maven做项目构建）
1）在配置文件中添加
**[html]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- <!-- 自动扫描的包名 -->
- <context:component-scanbase-package="learning.webapp.controller"/>
- <!-- 默认的注解映射的支持 -->
- <mvc:annotation-driven/>
2）添加以下几个java类
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- package learning.webapp.model;  
- 
- publicclass Employee {  
- private String name;  
- privateint salary;  
- 
- public Employee() {  
-     }  
- 
- public Employee(String name, int salary) {  
- this.name = name;  
- this.salary = salary;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicint getSalary() {  
- return salary;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- publicvoid setSalary(int salary) {  
- this.salary = salary;  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- package learning.webapp.model;  
- 
- import javax.xml.bind.annotation.XmlRootElement;  
- 
- @XmlRootElement
- publicclass EmployeeX extends Employee {  
- public EmployeeX() {  
- super();  
-     }  
- 
- public EmployeeX(String name, int salary) {  
- super(name, salary);  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- package learning.webapp.controller;  
- 
- import learning.webapp.model.Employee;  
- import learning.webapp.model.EmployeeX;  
- 
- import org.springframework.stereotype.Controller;  
- import org.springframework.web.bind.annotation.PathVariable;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import org.springframework.web.bind.annotation.RequestMethod;  
- import org.springframework.web.bind.annotation.ResponseBody;  
- 
- @Controller
- @RequestMapping("/employees")  
- publicclass XmlOrJsonController {  
- @RequestMapping(value="/xml/{name}", method=RequestMethod.GET)  
- @ResponseBody
- public Employee getEmployeeXml(@PathVariable String name) {  
- returnnew EmployeeX(name, 16000);  
-     }  
- }  
3) 在Eclipse中使用Jetty插件启动Web Server，然后在浏览器中访问：
![](https://img-blog.csdn.net/20150908011511053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
非常简单！Spring是怎么实现这个转换的呢？我们先了解下Spring的消息转换机制。
在SpringMVC中，可以使用@RequestBody和@ResponseBody两个注解，分别完成请求报文到对象和对象到响应报文的转换，底层这种灵活的消息转换机制，就是Spring3.x中新引入的HttpMessageConverter即消息转换器机制。
我们可以用下面的图，简单描述一下这个过程。
![](https://img-blog.csdn.net/20150908012959249)
这里最关键的就是<mvc:annotation-driven/>，加了这句配置，Spring会调用org.springframework.web.servlet.config.AnnotationDrivenBeanDefinitionParser来解析。
在这个类的parse(Element, ParserContext)方法中，分别实例化了RequestMappingHandlerMapping，RequestMappingHandlerAdapter等诸多类。
RequestMappingHandlerAdapter是请求处理的适配器，我们重点关注它的messageConverters属性。
1）RequestMappingHandlerAdapter在调用handle()的时候，会委托给ServletInvocableHandlerMethod的invokeAndHandle()方法进行处理，这个方法又调用HandlerMethodReturnValueHandlerComposite类进行处理。
HandlerMethodReturnValueHandlerComposite维护了一个HandlerMethodReturnValueHandler列表。
由于我们使用了@ResponseBody注解，getReturnValueHandler就会返回RequestResponseBodyMethodProcessor的实例。
![](https://img-blog.csdn.net/20150908013507741)
![](https://img-blog.csdn.net/20150908013759315)
2）之后RequestResponseBodyMethodProcessor.handleReturnValue(）方法会被调用。此方法会调用AbstractMessageConverterMethodProcessor.writeWithMessageConverters(）。它会根据request header中的Accept属性来选择合适的message converter.
![](https://img-blog.csdn.net/20150908014127095)
![](https://img-blog.csdn.net/20150908014707042)
3) messageConverters中有如下的6个converter. 它们是从哪里来的呢？前面提到，**AnnotationDrivenBeanDefinitionParser.parse(Element, ParserContext)**方法中，分别实例化了RequestMappingHandlerMapping，RequestMappingHandlerAdapter以及**messageConverters**属性。
需要关注org.springframework.http.converter.xml.Jaxb2RootElementHttpMessageConverter这个类，就是它实现了返回对象到XML的转换。
![](https://img-blog.csdn.net/20150908014450748)
![](https://img-blog.csdn.net/20150908014915650)
4）看一下getMessageConverters（）中的处理。有5个message converter是一定会加进来的。
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- if (convertersElement == null || Boolean.valueOf(convertersElement.getAttribute("register-defaults"))) {  
-     messageConverters.setSource(source);  
-     messageConverters.add(createConverterDefinition(ByteArrayHttpMessageConverter.class, source));  
- 
-     RootBeanDefinition stringConverterDef = createConverterDefinition(StringHttpMessageConverter.class, source);  
-     stringConverterDef.getPropertyValues().add("writeAcceptCharset", false);  
-     messageConverters.add(stringConverterDef);  
- 
-     messageConverters.add(createConverterDefinition(ResourceHttpMessageConverter.class, source));  
-     messageConverters.add(createConverterDefinition(SourceHttpMessageConverter.class, source));  
-     messageConverters.add(createConverterDefinition(AllEncompassingFormHttpMessageConverter.class, source));  
然后再看，这里jaxb2Present为true, 因此Jaxb2RootElementHttpMessageConverter被添加到messageConverters中。
![](https://img-blog.csdn.net/20150908015300087)
5）看一下jaxb2Present的定义，原来javax.xml.bind.Binder这个类是JDK中包含的类，所以jaxb2Present=true。
![](https://img-blog.csdn.net/20150908015701269)
6）我们看一下Jaxb2RootElementHttpMessageConverter的canWrite（）方法。返回true的条件有两个
a) 返回对象的类具有XmlRootElement注解；
b) 请求头中的Accept属性包含application/xml。
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- @Override
- publicboolean canWrite(Class<?> clazz, MediaType mediaType) {  
- return (AnnotationUtils.findAnnotation(clazz, XmlRootElement.class) != null && canWrite(mediaType));  
- }  
7) 在chrome中打开开发者工具，可以看到请求头中确实包含了Accept=application/xml
![](https://img-blog.csdn.net/20150908020725695)
接下来我们看看如果想要返回JSON数据，应该怎么做？
根据上面的分析，首先我们需要添加一个支持JSON的message converter. 前面分析getMessageConverters（）代码的时候，看到
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- if (jackson2Present) {  
-     RootBeanDefinition jacksonConverterDef = createConverterDefinition(MappingJackson2HttpMessageConverter.class, source);  
-     GenericBeanDefinition jacksonFactoryDef = createObjectMapperFactoryDefinition(source);  
-     jacksonConverterDef.getConstructorArgumentValues().addIndexedArgumentValue(0, jacksonFactoryDef);  
-     messageConverters.add(jacksonConverterDef);  
- }  
- elseif (gsonPresent) {  
-     messageConverters.add(createConverterDefinition(GsonHttpMessageConverter.class, source));  
- }  
然后再来看看jackson2Present和gsonPresent的定义。
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- privatestaticfinalboolean jackson2Present =  
-             ClassUtils.isPresent("com.fasterxml.jackson.databind.ObjectMapper", AnnotationDrivenBeanDefinitionParser.class.getClassLoader()) &&  
-                     ClassUtils.isPresent("com.fasterxml.jackson.core.JsonGenerator", AnnotationDrivenBeanDefinitionParser.class.getClassLoader());  
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- privatestaticfinalboolean gsonPresent =  
-         ClassUtils.isPresent("com.google.gson.Gson", AnnotationDrivenBeanDefinitionParser.class.getClassLoader());  
所以我们只要把Jackson2或者GSON加入工程的class path，Spring就会自动把GsonHttpMessageConverter加进来。
1）我们在POM中添加以下依赖
**[html]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- <dependency>
- <groupId>com.fasterxml.jackson.core</groupId>
- <artifactId>jackson-databind</artifactId>
- <version>2.6.1</version>
- </dependency>
或者
**[html]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- <dependency>
- <groupId>com.google.code.gson</groupId>
- <artifactId>gson</artifactId>
- <version>2.3.1</version>
- </dependency>
2）在XmlOrJsonController.java中添加getEmployeeJson（）方法
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- @RequestMapping(value="/json/{name}", method=RequestMethod.GET)  
- @ResponseBody
- public Employee getEmployeeJson(@PathVariable String name) {  
- returnnew Employee(name, 16000);  
- }  
和getEmployeeXml（）相比，这里唯一的不同是返回对象变成了Employee，因为Employee类上没有@XmlRootElement注解，所以Spring不会选择Jaxb2RootElementHttpMessageConverter。又因为Accept属性中包含了*/*，表示接受任意格式返回数据，所以GsonHttpMessageConverter的canWrite()方法返回true.这样Spring就会选择MappingJackson2HttpMessageConverter或者GsonHttpMessageConverter来进行数据转换。
![](https://img-blog.csdn.net/20150908132730932)
至此，我们知道请求头中的Accept属性是一个很关键的东西，我们可以根据这个在Controller中写一个方法，根据Accept的值自动返回XML或者JSON数据。
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- /**
-  * 根据request header中的Accept自动选择返回XML or JSON 
-  */
- @RequestMapping(value="/{name}", method=RequestMethod.GET)  
- @ResponseBody
- public Employee getEmployee(@PathVariable String name) {  
- returnnew EmployeeX(name, 16000);  
- }  
因为浏览器的Accept值不方便修改，我们自己写客户端来调用。
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48280083#)[copy](http://blog.csdn.net/fw0124/article/details/48280083#)
- package learning.webapp;  
- 
- import java.io.IOException;  
- import java.io.InputStream;  
- import java.net.URI;  
- import java.net.URISyntaxException;  
- 
- import org.junit.Test;  
- import org.springframework.http.HttpEntity;  
- import org.springframework.http.HttpHeaders;  
- import org.springframework.http.HttpMethod;  
- import org.springframework.http.client.ClientHttpRequest;  
- import org.springframework.http.client.ClientHttpResponse;  
- import org.springframework.http.client.SimpleClientHttpRequestFactory;  
- import org.springframework.web.client.RestTemplate;  
- 
- publicclass XmlOrJasonControllerTest {  
- 
- @Test
- publicvoid testJsonResponse() throws IOException, URISyntaxException {  
-         String url = "http://localhost:8080/employees/Jack";  
- 
-         ClientHttpRequest request = new SimpleClientHttpRequestFactory().createRequest(new URI(url), HttpMethod.GET);  
-         request.getHeaders().set("Accept", "application/json");  
-         ClientHttpResponse response = request.execute();  
-         InputStream is = response.getBody();  
- byte bytes[] = newbyte[(int) response.getHeaders().getContentLength()];  
-         is.read(bytes);  
- 
-         String jsonData = new String(bytes);  
-         System.out.println(jsonData);  
-     }  
- 
- @Test
- publicvoid testXmlResponse() throws IOException, URISyntaxException {  
-         String url = "http://localhost:8080/employees/Jack";  
- // response headers 中包含Transfer-Encoding:chunked，没有content length,
-         HttpHeaders requestHeaders = new HttpHeaders();  
-         requestHeaders.set("Accept", "application/xml");  
- 
-         RestTemplate restTemplate = new RestTemplate();  
-         HttpEntity<Object> httpEntity = new HttpEntity<Object>(requestHeaders);  
-         String xmlData = restTemplate.exchange(url, HttpMethod.GET, httpEntity, String.class).getBody();  
- 
-         System.out.println(xmlData);  
-     }  
- }  
**[参考资料]**
1）http://www.cnblogs.com/fangjian0423/p/springMVC-xml-json-convert.html
2）http://my.oschina[.NET](http://lib.csdn.net/base/dotnet)/lichhao/blog/172562
