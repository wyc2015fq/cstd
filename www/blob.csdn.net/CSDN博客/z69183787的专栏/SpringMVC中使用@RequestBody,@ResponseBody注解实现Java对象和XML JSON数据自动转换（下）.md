# SpringMVC中使用@RequestBody,@ResponseBody注解实现Java对象和XML/JSON数据自动转换（下） - z69183787的专栏 - CSDN博客
2016年10月11日 11:46:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2465
上一篇分析了[spring](http://lib.csdn.net/base/javaee)中是如何通过使用@ResponseBody注解，实现自动转换[Java](http://lib.csdn.net/base/javaee)对象为XML,JSON数据到Reponse中。
接下来看看如果request中包含了XML或者JSON数据，反过来，如何来通过@RequestBody注解来自动转换为Java对象。
**配合@RequestBody注解，以及HTTP Request Header中的Content-Type属性，HTTP Request Body中包含的**XML或者JSON数据**可以自动被转换成对应的**Java对象**。**
1）首先在controller中添加handlermapping。
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48312317#)[copy](http://blog.csdn.net/fw0124/article/details/48312317#)
- /**
-  * 根据request header中的Content-Type自动转换XML/JSON->EmployeeX对象
-  * 根据request header中的Accept自动选择返回XML or JSON
-  */
- @RequestMapping(method=RequestMethod.POST)  
- @ResponseBody
- public EmployeeServiceResponse createEmployee(@RequestBody EmployeeX employee) {  
-     logger.debug("creating a employee:[{}]", employee);  
- returnnew EmployeeServiceResponse(0, "");  
- }  
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48312317#)[copy](http://blog.csdn.net/fw0124/article/details/48312317#)
- package learning.webapp.model;  
- 
- import javax.xml.bind.annotation.XmlRootElement;  
- 
- @XmlRootElement
- publicclass EmployeeServiceResponse {  
- private Integer errcode;  
- private String errmsg;  
- 
- public EmployeeServiceResponse() {  
-     }  
- 
- public EmployeeServiceResponse(Integer errcode, String errmsg) {  
- this.errcode = errcode;  
- this.errmsg = errmsg;  
-     }  
- 
- public Integer getErrcode() {  
- return errcode;  
-     }  
- 
- publicvoid setErrcode(Integer errcode) {  
- this.errcode = errcode;  
-     }  
- 
- public String getErrmsg() {  
- return errmsg;  
-     }  
- 
- publicvoid setErrmsg(String errmsg) {  
- this.errmsg = errmsg;  
-     }  
- }  
2）运行以下两个测试函数
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48312317#)[copy](http://blog.csdn.net/fw0124/article/details/48312317#)
-     @Test
- publicvoid testJsonRequestResponse() throws IOException, URISyntaxException {  
-         String url = "http://localhost:8080/employees";  
-         HttpHeaders requestHeaders = new HttpHeaders();  
-         requestHeaders.set("Accept", "application/json");  
-         requestHeaders.set("Content-Type", "application/json");  
- 
-         String jsonStr = "{\"name\":\"Jack\",\"salary\":16000}";  
- 
-         RestTemplate restTemplate = new RestTemplate();  
-         HttpEntity<String> httpEntity = new HttpEntity<String>(jsonStr, requestHeaders);  
-         String jsonData = restTemplate.postForObject(url, httpEntity, String.class);  
- 
-         System.out.println(jsonData);  
-     }  
- 
- @Test
- publicvoid testXmlRequestResponse() throws IOException, URISyntaxException {  
-         String url = "http://localhost:8080/employees";  
-         HttpHeaders requestHeaders = new HttpHeaders();  
-         requestHeaders.set("Accept", "application/xml");  
-         requestHeaders.set("Content-Type", "application/xml");  
- 
-         String xmlStr = "<employeeX><name>Jack</name><salary>16000</salary></employeeX>";  
- 
-         RestTemplate restTemplate = new RestTemplate();  
-         HttpEntity<String> httpEntity = new HttpEntity<String>(xmlStr, requestHeaders);  
-         String xmlData = restTemplate.postForObject(url, httpEntity, String.class);  
- 
-         System.out.println(xmlData);  
-     }  
3）我们甚至可以任意组合请求和应答中的XML/JSON。比如Request=XML，Response=JSON
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48312317#)[copy](http://blog.csdn.net/fw0124/article/details/48312317#)
- @Test
- publicvoid testXmlRequestJsonResponse() throws IOException, URISyntaxException {  
-     String url = "http://localhost:8080/employees";  
-     HttpHeaders requestHeaders = new HttpHeaders();  
-     requestHeaders.set("Accept", "application/json");  
-     requestHeaders.set("Content-Type", "application/xml");  
- 
-     String xmlStr = "<employeeX><name>Jack</name><salary>16000</salary></employeeX>";  
- 
-     RestTemplate restTemplate = new RestTemplate();  
-     HttpEntity<String> httpEntity = new HttpEntity<String>(xmlStr, requestHeaders);  
-     String jsonData = restTemplate.postForObject(url, httpEntity, String.class);  
- 
-     System.out.println(jsonData);  
- }  
类似上一篇中的分析，最关键的处理在AbstractMessageConverterMethodArgumentResolver.readWithMessageConverters(）。它会根据request header中的Content-Type属性来选择合适的message converter.
![](https://img-blog.csdn.net/20150909104429915)
**---------------------**
**以下引用自参考资料1：**
如果不想使用<mvc:annotation-driven/>中默认的RequestMappingHandlerAdapter的话，我们可以在重新定义这个bean，spring会覆盖掉默认的RequestMappingHandlerAdapter。
为何会覆盖，请参考博客：[http://www.cnblogs.com/fangjian0423/p/spring-Ordered-interface.html](http://www.cnblogs.com/fangjian0423/p/spring-Ordered-interface.html)
```xml
<bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">
  <property name="messageConverters">
    <list>
      <bean class="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>
      <bean class="org.springframework.http.converter.StringHttpMessageConverter"/>
      <bean class="org.springframework.http.converter.ResourceHttpMessageConverter"/>
    </list>
  </property>
</bean>
```
或者如果只想换messageConverters的话。
```xml
<mvc:annotation-driven>
  <mvc:message-converters>
    <bean class="org.example.MyHttpMessageConverter"/>
    <bean class="org.example.MyOtherHttpMessageConverter"/>
  </mvc:message-converters>
</mvc:annotation-driven>
```
**[参考资料]**
1）http://www.cnblogs.com/fangjian0423/p/springMVC-xml-json-convert.html
2）http://my.oschina[.NET](http://lib.csdn.net/base/dotnet)/lichhao/blog/172562
