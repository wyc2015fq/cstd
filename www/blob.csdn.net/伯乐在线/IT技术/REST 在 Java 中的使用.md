# REST 在 Java 中的使用 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [求知者](http://www.jobbole.com/members/zhaoypmit) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
REST是一种混合的架构风格，它的由来以及它的架构元素在笔者的前两篇文《[REST架构风格的由来](http://blog.jobbole.com/108595/)》和《[REST架构风格的架构元素](http://blog.jobbole.com/108692/)》中已经描述了。本篇主要描述一下J2EE对REST的支持。
Java是在J2EE6中引入了对REST的支持，即JSR-311(JAX-RS 1.1: The JavaTM API for RESTful Web Services)，现在JAX-RS到2.0版本了，对应的是JSR-339，JSR是Java技术规范提案，由JCP组织进行管理。该规范使得使用Java进行开发的人员使用一套固定的接口来开发 REST 应用，避免了依赖于第三方框架。JAX-RS是一套接口，具体实现由第三方提供，例如 Sun 的参考实现 Jersey、Apache 的CXF 以及 JBoss 的 RESTEasy，下文会有对它们如何发布rest服务的简单介绍。
# 1.常用API介绍
## 1.1 javax.ws.rs包(用于创建RESTful服务资源的高级接口和注解)
### 1.1.1 统一接口
JAX-RS使用@javax.ws.rs.POST 、@javax.ws.rs.GET 、@javax.ws.rs.PUT、@javax.ws.rs.DELETE四个注解代表了对资源的CRUD(Create, Retrieve, Update, Delete)操作。
### 1.1.2 定位资源
注:下文中url中的root代表你的web应用的名称加上你在web.xml中servlet-mapping元素设置的路径。
@javax.ws.rs.ApplicationPath 标识应用路径，用于由@Path提供的所有资源uri的基本uri。当发布在一个servlet容器中，它的值可以使用web.xml中的servlet-mapping进行重写。
@javax.ws.rs.Path 标识要请求的资源类或类方法的uri路径。
@javax.ws.rs.PathParam 将uri中指定的路径参数绑定到资源方法参数，资源类的字段，或资源类的bean属性。
比如：


```
@Path("/user")
public class UserResource {
 @GET
 @Path("{userName}")
 @Produces(MediaType.APPLICATION_JSON)
 public User getUser(@PathParam("userName") String userName) {
 ...
 }
}
```
当浏览器请求http://localhost:8080/root/user/boglond时，方法中userName值为boglond。
@javax.ws.rs.QueryParam 将http请求的Query参数绑定到资源方法参数，资源类的字段，或资源类的bean属性。
eg:


```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getUser")
 @Produces(MediaType.APPLICATION_JSON)
 public User getUser(@QueryParam("userName") String userName,@QueryParam("age") int age) {
 ...
 }
}
```
当浏览器请求http://localhost:8080/root/user/getUser?userName=boglond&age=26时，方法中userName值为boglond，age值为26。
@javax.ws.rs.FormParam 将http请求的Form表单中的参数绑定到资源方法参数。
eg:


```
@Path("/user")
public class UserRecource {
 @javax.ws.rs.POST
 @Path("/insert")
 @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
 public String insertUser(@FormParam("userName") String userName,@FormParam("age") int age){
 ...
 }
}
```
页面中的form表单如下


```
<form action="/root/user/insert" method="post">
 姓名:<input name="userName" id= "userName" type = "text" value="boglond"/></br>
 年龄:<input name="age" id= "age" type = "text" value="26"/>
 <button type = "submit">提交</button>
</form>
```
提交表单后方法中userName值为boglond，age值为26。
@javax.ws.rs.CookieParam 将http cookie的值绑定到资源方法参数，资源类的字段，或资源类的bean属性。
eg:

Java
```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getCookieParam")
 @Produces(MediaType.APPLICATION_JSON)
 public String getCookieParam(@CookieParam("JSESSIONID") String jsessionId) {
 ...
 }
}
```
当浏览器请求http://localhost:8080/root/user/getCookieParam时，方法中jsessionId值为”FFF7BDDC46579DBDDDEB3E94776A2623″,此值不是固定的,不同的浏览器有不同的值。
@javax.ws.rs.HeaderParam 将http header的值绑定到资源方法参数，资源类的字段，或资源类的bean属性。
eg:

Java
```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getHeaderParam")
 @Produces(MediaType.APPLICATION_JSON)
 public String getHeaderParam(@HeaderParam("Accept") String accept,@HeaderParam("Accept-Language") 
  String acceptLang) {
 ...
 }
}
```
当浏览器请求http://localhost:8080/root/user/getHeaderParam时，方法中accept值为”text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8″，acceptLang值为”zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3″，不同的浏览器，值有所不同。
@javax.ws.rs.MatrixParam 将uri矩阵参数的值绑定到资源方法参数，资源类的字段，或资源类的bean属性。
eg:

Java
```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getUser")
 @Produces(MediaType.APPLICATION_JSON)
 public User getUser(@MatrixParam("userName") String userName,@MatrixParam("age") int age) {
 ...
 }
}
```
当浏览器请求http://localhost:8080/root/user/getUser;userName=boglond;age=26时，方法中userName值为boglond，age值为26。
@javax.ws.rs.DefaultValue 设置 @PathParam, @QueryParam, @MatrixParam, @CookieParam, @FormParam, or @HeaderParam参数的默认值。如果它们没有接收到值，就使用默认值。
@javax.ws.rs.BeanParam 将请求中的bean绑定到资源方法参数，资源类的字段，或资源类的bean属性。
eg:
bean的配置:

Java
```
@javax.xml.bind.annotation.XmlRootElement
public class UserBean{
 @FormParam("userName")
 private String userName;
 @FormParam("age")
 private int age;
 ...
}
```
资源类方法的配置:

Java
```
@Path("/user")
public class UserRecource {
 @javax.ws.rs.POST
 @Path("/insertUserBean")
 @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
 public String insertUserBean(@BeanParam UserBean userBean){
 ...
 }
}
```
页面中的form表单如下:


```
<form action="/root/user/insertUserBean" method="post">
 姓名:<input name="userName" id= "userName" type = "text" value="boglond"/></br>
 年龄:<input name="age" id= "age" type = "text" value="26"/>
 <button type = "submit">提交</button>
</form>
```
当提交表单后insertUserBean方法中的userBean对象的属性值会映射为表单中对应的值。
### 1.1.3 表述资源的数据格式(通过媒体类型表述)
@javax.ws.rs.Consumes 定义一个资源类的方法或MessageBodyReader能够接受的媒体类型。方法级别的@Consumes会覆盖类级别的@Consumes。(指定将要返回给client端的数据MIME类型)
@javax.ws.rs.Produces 定义一个资源类的方法或MessageBodyWriter能够产生的媒体类型。方法级别的@Produces会覆盖类级别的@Produces。(指定可以接受client发送过来的MIME类型)
eg:@Produces(“application/json”);指定多个MIME类型 @Produces({“application/json”,”application/xml”})
常见的媒体类型如


```
text/html ： HTML格式
text/plain ：纯文本格式
text/xml ：  XML格式(它会忽略xml头所指定编码格式而默认采用us-ascii编码)
image/gif ：gif图片格式
image/jpeg ：jpg图片格式
image/png：png图片格式
application/xhtml+xml ：XHTML格式
application/xml     ： XML格式(它会根据xml头指定的编码格式来编码)
application/atom+xml  ：Atom XML聚合格式
application/json    ： JSON数据格式
application/pdf       ：pdf格式
application/msword  ： Word文档格式
application/octet-stream ： 二进制流数据（如常见的文件下载）
application/x-www-form-urlencoded ： <form encType=””>中默认的encType，form表单数据被编码为key/value格式发送到服务器（表单默认的提交数据的格式）
multipart/form-data ： 需要在表单中进行文件上传时，就需要使用该格式。
```
媒体类型的常量值在javax.ws.rs.core.MediaType中。
### 1.1.4 编解码
@javax.ws.rs.Encoded 禁用由 @QueryParam, @PathParam, @FormParam or @MatrixParam绑定的参数值的自动解码
eg:

Java
```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getUser")
 @Produces(MediaType.APPLICATION_JSON)
 public User getUser(@MatrixParam("userName") String userName,
 @MatrixParam("age") int age) {
 ...
 }
}
```
此时没有使用@Encoded
当浏览器请求http://localhost:8080/root/user/getUser;userName=boglond%5CD;age=26时,方法中userName值为boglond\D，age值为26。
当给方法参数加上@Encoded时,
浏览器请求http://localhost:8080/root/user/getUser;userName=boglond%5CD;age=26,方法中userName值为boglond%5CD，age值为26。
@Encoded可加在方法的上面禁用此方法所有参数的自动解码。
## 1.2 javax.ws.rs.client包(JAX-RS客户端API)
javax.ws.rs.client.WebTarget 由资源URI标识的资源目标。
javax.ws.rs.client.ClientBuilder 用于加载客户端实例的主入口点。
eg:使用 request() 函数来初始化一个请求并用后续的 post 或者get等方法来指定请求的类型。

Java
```
Client client = ClientBuilder.newClient();
client.target(..).request().get();
```
通过 path() 和 resolveTemplate() 方法来处理动态的 URL 路径参数。


```
client.target(..).path(..).resolveTemplate(..);
javax.ws.rs.client.InvocationCallback<RESPONSE> 可以实现从调用处理接收异步处理事件。
javax.ws.rs.client.ClientRequestFilter 客户端请求过滤器。
javax.ws.rs.client.ClientResponseFilter 客户端响应过滤器。
```
## 1.3 javax.ws.rs.container包(特定容器JAX-RS API )
@javax.ws.rs.container.ResourceContext 提供对资源类实例的访问,使用@javax.ws.rs.core.Context 注入。
eg:

Java
```
public String getResourceContext(@javax.ws.rs.core.Context ResourceContext resourceContext) {
...
}
```
@javax.ws.rs.container.ContainerRequestContext 容器请求过滤器上下文。为过滤器提供特定于请求的信息的可变类，例如请求URI，消息头，消息实体或请求范围属性。暴露的setter方法允许修改暴露的请求特定信息。
@javax.ws.rs.container.ContainerResponseContext 容器响应过滤器上下文。为过滤器提供特定于响应的信息的可变类，例如消息头，消息实体或请求范围属性。暴露的setter方法允许修改暴露的响应特定信息。
## 1.4 javax.ws.rs.core包(用于创建RESTful服务资源的低级接口和注解)
@javax.ws.rs.core.MediaType 媒体类型常量类。
j
avax.ws.rs.core.Cookie 在请求中转移的HTTP Cookie的值。
eg:


```
Cookie c = new Cookie(name, value);
javax.ws.rs.core.Form 表示使用"application / x-www-form-urlencoded"媒体类型编码的HTML表单数据请求实体。
```
eg:


```
Form f = new Form();
f.param(name, value)
```
javax.ws.rs.core.Response.Status  http所定义的响应状态码。
javax.ws.rs.core.HttpHeaders 一个可注入的接口，提供对HTTP头信息的访问。
eg:


```
@GET
@Path("/getHttpHeaders")
@Produces(MediaType.APPLICATION_JSON)
public String getHttpHeaders(@javax.ws.rs.core.Context HttpHeaders httpHeaders) {
...
}
```
@javax.ws.rs.core.UriInfo 一个可注入的接口，提供对应用程序和请求URI信息的访问。
eg:


```
@GET
@Path("/getUriInfo")
@Produces(MediaType.APPLICATION_JSON)
public String getUriInfo(@javax.ws.rs.core.Context UriInfo uriInfo) {
...
}
```
@javax.ws.rs.core.Context 将信息注入类字段，bean属性或方法参数。如
## 1.5 javax.ws.rs.ext包(为JAX-RS API支持的类型提供扩展)
javax.ws.rs.ext.MessageBodyReader<T> 反序列化
javax.ws.rs.ext.MessageBodyWriter<T> 序列化
# 2.Jersey发布rest服务简介
Jersey项目使用apache Maven构建，它的相关模块都可从Maven中央仓库获取。本项目是基于JAX-RS 2.0的。
## 2.1 创建一个maven工程
如图
![jersey-demo](http://jbcdn2.b0.upaiyun.com/2016/12/309a4967f332fc083d00774b08da4ca3.png)
## 2.2 在pom中添加依赖


```
<dependency>
     <groupId>javax.ws.rs</groupId>
     <artifactId>javax.ws.rs-api</artifactId>
     <version>2.0.1</version>
</dependency>
<dependency>
     <groupId>org.glassfish.jersey.containers</groupId>
     <!-- if your container implements Servlet API older than 3.0, use "jersey-container-servlet-core"  -->
     <artifactId>jersey-container-servlet</artifactId> 
     <version>2.25</version>
</dependency>
```
## 2.3 配置web.xml
在web.xml中添加


```
<servlet>
      <servlet-name>jersey-demo</servlet-name>
      <servlet-class>org.glassfish.jersey.servlet.ServletContainer</servlet-class>
      <init-param>
          <param-name>jersey.config.server.provider.packages</param-name>
          <!-- 这个包路径换成你的包路径,多个包路径可以使用逗号分隔-->
          <param-value>com.boglond.jersey</param-value>
      </init-param>
      <load-on-startup>1</load-on-startup>
</servlet>
<servlet-mapping>
      <servlet-name>jersey-demo</servlet-name>
      <!-- 这个设置为你想要的路径-->
      <url-pattern>/resources/*</url-pattern>
</servlet-mapping>
```
## 2.4 创建资源类

Java
```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getUser")
 @Produces(MediaType.APPLICATION_JSON)
 public User getUser(@QueryParam("userName") String userName,
 @QueryParam("age") int age) {
 ...
 }
}
```
访问:在浏览器里输入http://localhost:8080/jersey-demo/resources/user/getUser?userName=boglond&age=30即可访问。
# 3.CXF发布rest服务简介
## 3.1 创建一个maven工程
如图
![cxf-demo](http://jbcdn2.b0.upaiyun.com/2016/12/665dd86ed307596822f87b50f5184e17.png)
## 3.2 在pom中添加依赖
CXF的依赖


```
<dependency>
    <groupId>org.apache.cxf</groupId>
    <artifactId>cxf-rt-frontend-jaxrs</artifactId>
    <version>3.0.0</version>
</dependency>      
<dependency>
    <groupId>org.apache.cxf</groupId>
    <artifactId>cxf-rt-transports-http-jetty</artifactId>
    <version>3.0.0</version>
</dependency>
```
日志的依赖


```
<dependency>
    <groupId>log4j</groupId>
    <artifactId>log4j</artifactId>
    <version>1.2.17</version>
</dependency>
<dependency>
	<groupId>org.slf4j</groupId>
	<artifactId>slf4j-log4j12</artifactId>
	<version>1.7.6</version>
</dependency>
```
## 3.3 创建资源类

Java
```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getUser")
 @Produces(MediaType.APPLICATION_JSON)
 public User getUser(@QueryParam("userName") String userName,
 @QueryParam("age") int age) {
 ...
 }
}
```
## 3.4 发布REST服务

Java
```
package com.boglond.cxf;
import java.util.ArrayList;
import java.util.List;
import org.apache.cxf.jaxrs.JAXRSServerFactoryBean;
import org.apache.cxf.jaxrs.lifecycle.ResourceProvider;
import org.apache.cxf.jaxrs.lifecycle.SingletonResourceProvider;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
public class CxfServer {
  private final static Logger logger = LoggerFactory.getLogger(CxfServer.class);
  public static void main(String[] args) {  
    List<Class<?>> resourceClassList = new ArrayList<Class<?>>();
    resourceClassList.add(UserRecource.class);    
    List<ResourceProvider> resourceProviderList = new ArrayList<ResourceProvider>();
    resourceProviderList.add(new SingletonResourceProvider(new UserRecource()));
    // 发布 REST 服务
    JAXRSServerFactoryBean factory = new JAXRSServerFactoryBean();
    factory.setAddress("http://localhost:11002/cxf-demo/recources");
    factory.setResourceClasses(resourceClassList);
    factory.setResourceProviders(resourceProviderList);
    factory.create();
    logger.info("使用CXF发布REST服务成功");
  }
}
```
访问:在浏览器里输入http://localhost:8080/cxf-demo/recources/user/getUser?userName=boglond&age=26即可访问。
# 4.RESTEasy发布rest服务简介
## 4.1 创建一个maven工程
如图
![resteasy-demo](http://jbcdn2.b0.upaiyun.com/2016/12/d4288b0fb262fa5fb6d8f6bc34aa3fed.png)
## 4.2 在pom中添加依赖


```
<dependency>
      <groupId>org.jboss.resteasy</groupId>
      <artifactId>resteasy-jaxrs</artifactId>
      <version>3.1.0.Final</version>
</dependency>
<dependency>
      <groupId>org.jboss.resteasy</groupId>
      <artifactId>resteasy-jaxb-provider</artifactId>
      <version>3.1.0.Final</version>
</dependency>
如果servlet容器使用的是servlet3.0 需要加入如下依赖
<dependency>
      <groupId>org.jboss.resteasy</groupId>
      <artifactId>resteasy-servlet-initializer</artifactId>
      <version>3.1.0.Final</version>
</dependency>
```
注:如果你配置的maven仓库地址下载不下来resteasy依赖的jar,就将你的maven conf文件夹下的setting.xml 中的仓库url替换为下面的url。<url>http://repository.jboss.org/nexus/content/groups/public/</url>
## 4.3 配置web.xml
在web.xml中添加


```
<context-param>
	  <param-name>resteasy.resources</param-name>
	  <!-- 这是你的资源类 -->
	  <param-value>com.boglond.resteasy.UserRecource</param-value>
</context-param>
<listener>
	<listener-class>
	      org.jboss.resteasy.plugins.server.servlet.ResteasyBootstrap
	</listener-class>
</listener>
<servlet>
	<servlet-name>Resteasy</servlet-name>
	<servlet-class>
	      org.jboss.resteasy.plugins.server.servlet.HttpServletDispatcher
	</servlet-class>
</servlet>
<servlet-mapping>
	<servlet-name>Resteasy</servlet-name>
	<url-pattern>/*</url-pattern>
</servlet-mapping>
```
## 4.4 创建资源类

Java
```
@Path("/user")
public class UserRecource {
 @GET
 @Path("/getUser")
 @Produces(MediaType.APPLICATION_JSON)
 public User getUser(@QueryParam("userName") String userName,
 @QueryParam("age") int age) {
 ...
 }
}
```
访问:在浏览器里输入http://localhost:8080/resteasy-demo/user/getUser?userName=boglond&age=30即可访问。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/12/bb02779858039f72f3a7d77f15abe136.jpg)![](http://jbcdn2.b0.upaiyun.com/2016/12/47ff1a8dbb792119813a92a5cff1bba3.jpg)
