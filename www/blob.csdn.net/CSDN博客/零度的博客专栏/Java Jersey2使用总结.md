# Java Jersey2使用总结 - 零度的博客专栏 - CSDN博客
2016年05月06日 10:42:18[零度anngle](https://me.csdn.net/zmx729618)阅读数：6296
### 前言
在短信平台一期工作中，为便于移动平台的开发，使用了Java Jersey框架开发RESTFul风格的Web Service接口。在使用的过程中发现了一些问题并积累了一些项目经验，做了一下总结，便于个人成长，同时也希望对有需要的同仁有好的借鉴和帮助。
### 简介
Jersey是JAX-RS（JSR311）开源参考实现用于构建    [RESTful](https://zh.wikipedia.org/wiki/REST)    Web service，它包含三个部分： 
- 
**核心服务器（Core Server）**        ：通过提供JSR 311中标准化的注释和API标准化，可以用直观的方式开发RESTful Web服务。     
- 
**核心客户端(Core Client)**         ：Jersey客户端API能够帮助开发者与RESTful服务轻松通信；     
- 
**集成（Integration)**         ：Jersey还提供可以轻松继承Spring、Guice、Apache Abdera的库。     
在本次开发中使用Jersey2.0，并且仅使用了核心服务器。
### 设置Jersey环境
#### Maven
```
<!--jersey-->
<dependency>
  <groupId>org.glassfish.jersey.containers</groupId>
  <artifactId>jersey-container-servlet-core</artifactId>
  <version>2.0</version>
</dependency>
<!--JAXB API-->
<dependency>
  <groupId>javax.xml.ws</groupId>
  <artifactId>jaxws-api</artifactId>
  <version>2.1</version>
</dependency>
<!-- Json支持 -->
<dependency>
  <groupId>org.codehaus.jackson</groupId>
  <artifactId>jackson-core-asl</artifactId>
  <version>1.9.12</version>
</dependency>
<dependency>
  <groupId>org.codehaus.jackson</groupId>
  <artifactId>jackson-mapper-asl</artifactId>
  <version>1.9.12</version>
</dependency>
<dependency>
  <groupId>org.codehaus.jackson</groupId>
  <artifactId>jackson-jaxrs</artifactId>
  <version>1.9.12</version>
</dependency>
```
#### 引入Jar文件方式
从Jersey开发包中将以下库复制的WEB-INF下的库目录：
- 
服务器：jersey-server.jar 、jersey-container-servlet-core.jar、jersey-container-servlet.jar、javax.ws.rs-api-2.0.jar
- 
客户端：jersey-client.jar
- 
common：jersey-common.jar
- 
        json支持：在Jersey2.0中需要使用        [Jackson1.9](http://wiki.fasterxml.com/JacksonDownload)         才能支持json。     
### Hello World
以下将展示一个Hello World
**第一步：**    编写一个名为HelloResource的资源，它接受Http Get请求并响应“Hello Jersey” 
```java
```
@Path("/hello")
public class HelloResource {
  @GET
  @Produces(MediaType.TEXT_PLAIN)
  public String sayHello() {
    return "Hello Jersey";
  }
}
```
```
**第二步：**    编写JAX-RS application  
```java
```
public class APIApplication extends ResourceConfig {
    public APIApplication() {
  //加载Resource
  register(HelloResource.class);
  //注册数据转换器
  register(JacksonJsonProvider.class);
  // Logging.
  register(LoggingFilter.class);
    }
}
```
```
**第三步：**    在web.xml文件中定义servelt调度程序，目的是将所有REST请求发送到Jersey容器。除了声明Jersey Servlet外，还需定义一个初始化参数，指定JAX-RS application。 
```
<!--用于定义 RESTful Web Service 接口-->
<servlet>
  <servlet-name>JerseyServlet</servlet-name>
  <servlet-class>org.glassfish.jersey.servlet.ServletContainer</servlet-class>
  <init-param>
    <param-name>javax.ws.rs.Application</param-name>
    <param-value>cn.com.mink.resource.APIApplication</param-value>
  </init-param>
  <load-on-startup>1</load-on-startup>
</servlet>
<servlet-mapping>
  <servlet-name>JerseyServlet</servlet-name>
  <url-pattern>/services/*</url-pattern>
</servlet-mapping>
```
**第四步：**    测试程序  
在命令终端中输入以下命令，将会看到“Hello Jersey”。
curl      [http://host:port/services/hello](http://blog.segmentfault.com/lenbo_ma/http://host:port/services/hello)
或者在浏览器中输入以下URL，将会看到“Hello Jersey”
[http://host:port/services/hello](http://blog.segmentfault.com/lenbo_ma/http://host:port/services/hello)
### 使用
#### 资源
Root Resource And Sub-Resource
资源是组成RESTful服务的关键部分，可以使用HTTP方法（如：GET、POST、PUT和DELETE）操作资源。在JAX-RX中，资源通过POJO实现，使用   
`@Path`     注释组成其标识符。资源可以有子资源，父资源是资源集合，子资源是成员资源。 
在以下样例代码中，
Resources是"/services" URI组成是集合资源，UserResource是“/services/user” URI组成的成员资源;
```java
```
@Path("/services")
public class Resources {
  @Path("/user")
  public UserResource getUserResource() {
    ...
  }
  @Path("/book")
  public BookResource getBookResource() {
    ...
  }
}
```
```
UserResource是“/user” URI组成的集合资源，getUser是“/user/{username}” URI组成的资源方法
```java
```
@Path("/user")
public class UserResource {
  @GET
  @Path("{username"})
  @Produces("application/json")
  public User getUser(@PathParam("username") String userName) {
    ...
  }
}
```
```
HTTP Methods
HTTP方法映射到资源的CRUD（创建、读取、更新和删除）操作，基本模式如下：
- HTTP      *GET*       ：读取/列出/检索单个或资源集合。   
- HTTP      *POST*       ：新建资源。    
- HTTP      *PUT*       ：更新现有资源或资源集合。    
- HTTP      *DELETE*       ：删除资源或资源集合。    
@Produces
`@Produces`     注释用来指定将要返回给client端的数据标识类型（MIME）。   
`@Produces`     可以作为class注释，也可以作为方法注释，方法的    
```
@Produces
```
     注释将会覆盖class的注释。  
- 
指定一个MIME类型
@Produces("application/json")
- 
指定多个MIME类型
@Produces({"application/json","application/xml"})
@Consumes
`@Consumes`     与   
`@Produces`     相反，用来指定可以接受client发送过来的MIME类型，同样可以用于class或者method，也可以指定多个MIME类型，一般用于   
`@PUT`     ，    `@POST`     。 
#### 参数（Parameter Annotations）
Parameter Annotations用于获取client发送的数据。本文只介绍常用的注解，更多详见   
[Jersey用户手册](https://jersey.java.net/documentation/latest/jaxrs-resources.html#d0e1424)
@PathParam
使用    `@PathParam`     可以获取URI中指定规则的参数，比如： 
```java
```
@GET
@Path("{username"})
@Produces(MediaType.APPLICATION_JSON)
public User getUser(@PathParam("username") String userName) {
    ...
}
```
```
当浏览器请求    [http://localhost/user/jack](http://localhost/user/jack)     时，userName值为jack。 
@QueryParam
`@QueryParam`     用于获取GET请求中的查询参数，如： 
```java
```
@GET
@Path("/user")
@Produces("text/plain")
public User getUser(@QueryParam("name") String name,
                    @QueryParam("age") int age) {
    ...
}
```
```
当浏览器请求    [http://host:port/user?name=rose&age=25](http://blog.segmentfault.com/lenbo_ma/http://host:port/user?name=rose&age=25)    
 时，name值为rose，age值为25。如果需要为参数设置默认值，可以使用    `@DefaultValue`     ，如： 
```java
```
@GET
@Path("/user")
@Produces("text/plain")
public User getUser(@QueryParam("name") String name,
                    @DefaultValue("26") @QueryParam("age") int age) {
    ...
}
```
```
当浏览器请求    [http://host:port/user?name=rose](http://blog.segmentfault.com/lenbo_ma/http://host:port/user?name=rose)    
 时，name值为rose，age值为26。  
@FormParam
`@FormParam`     ，顾名思义，从POST请求的表单参数中获取数据。如： 
```java
```
@POST
@Consumes("application/x-www-form-urlencoded")
public void post(@FormParam("name") String name) {
    // Store the message
}
```
```
@BeanParam
当请求参数很多时，比如客户端提交一个修改用户的PUT请求，请求中包含很多项用户信息。这时可以用   
`@BeanParam`     。 
```java
```
@POST
@Consumes("application/x-www-form-urlencoded")
public void update(@BeanParam User user) {
    // Store the user data
}
```
```
User Bean定义如下：
```
@XmlRootElement(name = "user")
public class User {
  @PathParam("userName)
  private String userName;
  @FormParam("name")
  private String name;
  @FormParam("telephone")
  private String telephone;
  @FormParam("email")
  private String email;
  public String getUserName() {
    return userName;
  }
  public void setUserName(String userName) {
    this.userName = userName;
  }
  ...
}
```
使用Map
在一个大型的server中，因为参数的多变，参数结构的调整都会因为以上几种方式而遇到问题，这时可以考虑使用   
`@Context`     注释，并获取UriInfo实例，如下： 
```
@GET
public String get(@Context UriInfo ui) {
    MultivaluedMap<String, String> queryParams = ui.getQueryParameters();
    MultivaluedMap<String, String> pathParams = ui.getPathParameters();
}
```
同样还可以通过    `@Context`     注释获取   
`ServletConfig`     、    
```
ServletContext
```
     、    `HttpServletRequest`     、   
`HttpServletResponse`     和    
```
HttpHeaders
```
     等，如下：  
```java
```
@Path("/")
public class Resource {
  @Context
  HttpServletRequest req;
  @Context
  ServletConfig servletConfig;
  @Context
  ServletContext servletContext;
  @GET
  public String get(@Context HttpHeaders hh) {
    MultivaluedMap<String, String> headerParams = hh.getRequestHeaders();
    Map<String, Cookie> pathParams = hh.getCookies();
  }
}
```
```
#### Jersey返回Json和Xml
    JAX-RS支持使用JAXB（Java API for XML Binding）将JavaBean绑定到XML或JSON，反之亦然。JavaBean必须使用   
`@XmlRootElement`     标注，没有   
`@XmlElement`     注释的字段将包含一个名称与之相同的XML元素，如下：  
```java
```
@XmlRootElement
public class OptionResult {
  @XmlElement(name = "code")
  private String result;
  private String errorMsg;
  public String getResult() {
    return result;
  }
  public void setResult(String result) {
    this.result = result;
  }
  public String getErrorMsg() {
    return errorMsg;
  }
  public void setErrorMsg(String errorMsg) {
    this.errorMsg = errorMsg;
  }
}
```
```
然后在REST服务中使用：
```java
```
@Path("/user")
public class UserResource {
  @POST
  @Produces("application/json")
  public OptionResult create(@BeanParam User user) {
    ...
  }
}
```
```
最后，要注册数据转换器，该转换器会自动将JavaBean转换为json数据：
```java
```
public class APIApplication extends ResourceConfig {
    public APIApplication() {
  //加载Model
  register(OptionResult.class);
  //加载与OptionResult同一个packge的Model
  //packages(OptionResult.class.getPackage().getName());
  //加载Resource
  register(UserResource.class);
  //注册数据转换器
  register(JacksonJsonProvider.class);
  // Logging.
  register(LoggingFilter.class);
    }
}
```
```
*说明*    ：返回XML数据的原理相同，仅仅是数据转换器不同，只需要在APIApplication中同时注册XML数据转换器即可，详见   
[Jersey用户手册](https://jersey.java.net/documentation/latest/media.html#xml)
### 问题总结
#### Ajax请求（POST、PUT和DELETE）无法将数据提交到Jersey容器
问题阐述
    在短信平台的开发中，数据的CRUD全部使用Ajax技术完成，因此必须使用POST、PUT和DELETE请求。此三种请求的content-type均为“application/x-www-form-urlencoded”，使用UTF-8编码会变成“application/x-www-form-urlencoded; UTF-8”。在使用Firefox的tamperdata扩展调试程序的过程中发现，当content-type为“application/x-www-form-urlencoded”时，Jersey容器能够通过   
`@FormParam`     注解获取到提交的数据，而content-type为“application/x-www-form-urlencoded; UTF-8”时便获取不到。 
解决方案
最终我使用Java Filter和Jersey RequestFilter解决了问题。首先在Java Filter中使用UTF8将Request中的数据编码，然后在Jersey RequestFilter中将request对象中的content-type修改为“application/x-www-form-urlencoded”。如：
```java
```
public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
  HttpServletRequest req = (HttpServletRequest)request;
  req.setCharacterEncoding("UTF-8");
}
public class RequestFilter implements ContainerRequestFilter {
  @Override
  public void filter(ContainerRequestContext context) throws IOException {
    String headerString = context.getHeaderString("content-type");
    if (headerString != null) {
      //如果content-type以"application/x-www-form-urlencoded"开头，则处理
       if (headerString.startsWith(MediaType.APPLICATION_FORM_URLENCODED))
        context.getHeaders().putSingle("content-type", MediaType.APPLICATION_FORM_URLENCODED);
    }
  }
}
```
```
最后在web.xml中注册Java Filter（要注册在Jersey容器之前），在APIApplication中注册Jersey RequestFilter，如下：
```java
```
public class APIApplication extends ResourceConfig {
    public APIApplication() {
        register(RequestFilter.class);
    }
}
```
```
***说明***    ：在修复此问题后，在Github的Jersey源代码仓库中看到已经有人发现并修复了此问题，在下个Jersey正式版本中应该不会再出现这样的问题，详见   
[此Discussion](https://github.com/jersey/jersey/pull/24)
### 后记
本人在使用Jersey的过程中发现网上有关Jersey的中文资料并不多，因此将本期开发中的使用经验总结于此，便于同样对Jersey感兴趣的同仁参考。如果你也有Jersey的开发经验并且对Jersey拥有浓厚的兴趣，欢迎与我联系并一起探讨技术，愿共同进步！
