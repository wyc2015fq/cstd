# 用Jersey开发RESTful服务 - 零度的博客专栏 - CSDN博客
2017年03月01日 11:46:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：463
                
REST基础概念：
- 在REST中的一切都被认为是一种资源。
- 每个资源由URI标识。
- 使用统一的接口。处理资源使用POST，GET，PUT，DELETE操作类似创建，读取，更新和删除（CRUD）操作。
- 无状态。每个请求是一个独立的请求。从客户端到服务器的每个请求都必须包含所有必要的信息，以便于理解。
- 通信都是通过展现。例如XML，JSON
RESTful Web服务由于其简单替代了基于SOAP的Web服务，并大型服务提供商所接受。这篇文章使用Jersey框架延伸JAX-RS API将展示如何创建一个REST风格的Web服务和客户端。
在Eclipse中，创建一个新的动态Web项目名为"RESTfulWS"：
![](http://simage.jdon.com/soa/jersey0.png)
下载Jersey zip bundle 这里 [here](https://jersey.java.net/download.html). 需要包：
- asm-3.1.jar
- jersey-client-1.17.1.jar
- jersey-core-1.17.1.jar
- jersey-server-1.17.1.jar
- jersey-servlet-1.17.1.jar
- jsr311-api-1.1.1.jar
![jersey](http://simage.jdon.com/soa/jersey.png)
加入项目：
![](http://simage.jdon.com/soa/jersey2.png)
创建Web服务类：
```
package com.eviac.blog.restws;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
/**
   * 
   *  @author pavithra
   * 
   */
// @Path here defines class level path.  Identifies the URI path that 
                    // a resource class will serve requests  for.
                    @Path("UserInfoService")
                    public class UserInfo {
 //  @GET here defines, this method will method will process HTTP GET
   //  requests.
   @GET
   //  @Path here defines method level path. Identifies the URI path that a
   //  resource class method will serve requests for.
   @Path("/name/{i}")
   //  @Produces here defines the media type(s) that the methods
   //  of a resource class can produce.
   @Produces(MediaType.TEXT_XML)
   //  @PathParam injects the value of URI parameter that defined in @Path
   //  expression, into the method.
   public String  userName(@PathParam("i") String i) {
   String name = i;
     return "<User>" + "<Name>" + name +  "</Name>" + "</User>";
   }
 @GET 
   @Path("/age/{j}") 
   @Produces(MediaType.TEXT_XML)
   public String  userAge(@PathParam("j") int j) {
  int  age = j;
     return "<User>" + "<Age>" + age +  "</Age>" + "</User>";
   }
}
```
配置web.xml
```
<?xml version="1.0"  encoding="UTF-8"?>  
  <web-app  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  xmlns="http://java.sun.com/xml/ns/javaee"  xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"  xsi:schemaLocation="http://java.sun.com/xml/ns/javaee  http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"  id="WebApp_ID" version="2.5">  
     <display-name>RESTfulWS</display-name>  
     <servlet>  
       <servlet-name>Jersey REST Service</servlet-name>  
       <servlet-class>com.sun.jersey.spi.container.servlet.ServletContainer</servlet-class>  
       <init-param>  
         <param-name>com.sun.jersey.config.property.packages</param-name>  
         <param-value>com.eviac.blog.restws</param-value>  
       </init-param>  
       <load-on-startup>1</load-on-startup>  
     </servlet>  
     <servlet-mapping>  
       <servlet-name>Jersey REST Service</servlet-name>  
       <url-pattern>/rest/*</url-pattern>  
     </servlet-mapping>  
  </web-app>
```
点按这个项目 run as ->run on server.
浏览器浏览：http://localhost:8080/RESTfulWS/rest/UserInfoService/name/Pavithra
URL说明：
![](http://simage.jdon.com/soa/resthow.png)
创建一个调用客户端：
```
package com.eviac.blog.restclient;
import javax.ws.rs.core.MediaType;
import com.sun.jersey.api.client.Client;
import  com.sun.jersey.api.client.ClientResponse;
import  com.sun.jersey.api.client.WebResource;
import  com.sun.jersey.api.client.config.ClientConfig;
import  com.sun.jersey.api.client.config.DefaultClientConfig;
/**
   * 
   *  @author pavithra
   * 
   */
public class UserInfoClient {
 public static final String BASE_URI =  "http://localhost:8080/RESTfulWS";
   public static final String PATH_NAME =  "/UserInfoService/name/";
   public static final String PATH_AGE =  "/UserInfoService/age/";
 public static void main(String[] args) {
   String name = "Pavithra";
    int  age = 25;
   ClientConfig config = new DefaultClientConfig();
     Client client = Client.create(config);
     WebResource resource = client.resource(BASE_URI);
   WebResource nameResource =  resource.path("rest").path(PATH_NAME + name);
     System.out.println("Client Response \n"
      +  getClientResponse(nameResource));
     System.out.println("Response \n" + getResponse(nameResource) +  "\n\n");
   WebResource ageResource = resource.path("rest").path(PATH_AGE  + age);
     System.out.println("Client Response \n"
      +  getClientResponse(ageResource));
     System.out.println("Response \n" + getResponse(ageResource));
   }
 /**
    *  Returns client response.
    *  e.g : 
    *  GET http://localhost:8080/RESTfulWS/rest/UserInfoService/name/Pavithra 
    *  returned a response status of 200 OK
    *
    *  @param service
    *  @return
    */
   private static String  getClientResponse(WebResource resource) {
     return resource.accept(MediaType.TEXT_XML).get(ClientResponse.class)
       .toString();
   }
 /**
    *  Returns the response as XML
    *  e.g : <User><Name>Pavithra</Name></User> 
    * 
    *  @param service
    *  @return
    */
   private static String getResponse(WebResource  resource) {
     return resource.accept(MediaType.TEXT_XML).get(String.class);
   }
}
```
运行这个客户端代码，得到结果：
Client Response 
                  GET http://localhost:8080/RESTfulWS/rest/UserInfoService/name/Pavithra returned a response status of 200 OK
                  Response 
                <User><Name>Pavithra</Name></User>
Client Response 
                  GET http://localhost:8080/RESTfulWS/rest/UserInfoService/age/25 returned a response status of 200 OK
                  Response 
                  <User><Age>25</Age></User>
