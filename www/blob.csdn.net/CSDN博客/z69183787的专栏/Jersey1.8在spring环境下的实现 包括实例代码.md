# Jersey1.8在spring环境下的实现 包括实例代码 - z69183787的专栏 - CSDN博客
2013年04月27日 16:53:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5164
个人分类：[Restful-Jersey框架																[Restful-Api](https://blog.csdn.net/z69183787/article/category/2175699)](https://blog.csdn.net/z69183787/article/category/2185955)
最近项目里面要使用rest或者webservice来实现一些对外的接口。
 下载地址：[http://download.java.net/maven/2/com/sun/jersey/contribs/jersey-spring/](http://download.java.net/maven/2/com/sun/jersey/contribs/jersey-spring/)
或官网
[https://maven.java.net/content/repositories/releases/com/sun/jersey/contribs/jersey-spring/1.9/](https://maven.java.net/content/repositories/releases/com/sun/jersey/contribs/jersey-spring/1.9/)
所需包：
jersey-client-1.8.jar
jersey-core-1.8.jar
jersey-json-1.8.jar
jersey-server-1.8.jar
jersey-spring-1.8.jar
查了很多资料，最后选择了Restlet、Apache CXF、Jersey。
Restlet是抛弃了servlet api，自身实现了一些api，最关键的是没有提供spring的集成。
CXF很好，很强大，但是觉得它的json不爽，总是要先绕道jaxb。
Jersey是sun推出来的，很轻量、很环保、支持JAX-RS规范，据说要整合到jdk中去的。
最终权衡之下选择了Jersey。
Jersey的官方网站[http://jersey.java.net/](http://jersey.java.net/)，相关文件可以在里面下载的。
**1.资料，在线文档。**
[http://jersey.java.net/nonav/documentation/latest/getting-started.html](http://jersey.java.net/nonav/documentation/latest/getting-started.html)
可以通过maven进行环境的搭建。
我没有使用maven的习惯，最后手工下载的，下载了好久，基本把所有插件都下载到了。
这边附近只能上传10M，上传一张相关的lib截图，需要的另外附上。
**2.与spring集成**
在classpath里里加入jersey-spring.jar，web.xml里面划一个url如/restapi/* 归Jersey的SpringServlet管即可。 然后SpringServlet就会在Spring的Context里查找支持某个具体path的Bean来出来处理请求。
Xml代码  ![收藏代码](http://xosadan.iteye.com/images/icon_star.png)
- <!-- Jersey 配置 -->
- <servlet>
- <servlet-name>JerseyServlet</servlet-name>
- <servlet-class>com.sun.jersey.spi.spring.container.servlet.SpringServlet</servlet-class>
- <init-param>
- <param-name>com.sun.jersey.config.property.packages</param-name>
- <!-- 系统启动时扫描的包的路径-->
- <param-value>com.tianque.locationtalk.api</param-value>
- <param-name>com.sun.jersey.api.json.POJOMappingFeature</param-name>
- <param-value>true</param-value>
- 
- </init-param>
- </servlet>
**3.参数支持和接口**
@PathParam用来表示Restful风格下在URL中的id等标识。
@QueryParameter可以转换任何有以String为参数的构造函数的类，还可以设定defaultValue, 当参数为空时会自动抛异常
@POST+ @Consumes 以POST方式传递XML或JSON编码的Java对象
@Context UriInfo，@Context HttpHeaders, @Context HttpServletRequest 获取灵活的，不固定的参数. 可以从原版HttpServletRequest中获取,也可以用封装好的更方便的UriInfo和HttpHeaders.
**4.使用Jackson处理JSON**
 在classpath里加入jersey-json.jar后，在服务端，web.xml里SpringServlet的定义要加入
```java
<init-param>
			<param-name>com.sun.jersey.api.json.POJOMappingFeature</param-name>
			<param-value>true</param-value>
		</init-param>
```
**5.要生成rest的service类**
pojo对象，必须加入**@XmlRootElement**，用于标识对象转化成xml或者json
Java代码  ![收藏代码](http://xosadan.iteye.com/images/icon_star.png)
- package com.xxx.locationtalk.api;  
- 
- import javax.xml.bind.annotation.XmlRootElement;  
- 
- 
- @XmlRootElement
- publicclass  User {  
- 
- public Long userId;  
- public String userName;  
- public String nickName;  
- 
- public User(){  
- 
-         }  
- public User(Long userId,String userName,String nickName){  
- this.userId =userId;  
- this.userName=userName;  
- this.nickName=nickName;  
-         }  
-     }  
具体映射出去的service类,其中调用了某个spring的service去读取数据库，各个不同的url实现了xml的对象输出，json对象的输出，以及list对象的输出，以及一些赋值操作和json对象赋值操作。
Java代码  ![收藏代码](http://xosadan.iteye.com/images/icon_star.png)
- package com.xxx.locationtalk.api;  
- 
- 
- import java.io.UnsupportedEncodingException;  
- import java.net.URLEncoder;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import javax.ws.rs.GET;  
- import javax.ws.rs.PUT;  
- import javax.ws.rs.Path;  
- import javax.ws.rs.Produces;  
- import javax.ws.rs.QueryParam;  
- import javax.ws.rs.core.MediaType;  
- 
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.context.annotation.Scope;  
- import org.springframework.stereotype.Component;  
- 
- import com.xxx.locationtalk.domain.MobileNumber;  
- import com.xxx.locationtalk.service.MobileNumberService;  
- 
- @Component
- @Scope("request")  
- @Path("/bookapi")  
- publicclass BookService {  
- @Autowired
- private MobileNumberService mobileNumberService;  
- 
- public MobileNumberService getMobileNumberService() {  
- return mobileNumberService;  
-     }  
- 
- publicvoid setMobileNumberService(MobileNumberService mobileNumberService) {  
- this.mobileNumberService = mobileNumberService;  
-     }  
- //外部传过来的参数
- @QueryParam("id") String userId;  
- 
- @GET
- @Produces(MediaType.TEXT_PLAIN)  
- public String helloWorld() {  
-         String ret = "Hello World!";  
- return ret;  
-     }  
- 
- // 子路径
- @Path("getUsers")     
- @GET
- @Produces( {MediaType.APPLICATION_XML })  
- public List<User>  getUsers() {  
-         List<User> users = new ArrayList<User>();  
- for (long i = 1; i < 5; i++) {  
-             User user = new User();  
-             user.userId=(i);  
-             user.userName=("yuhan" + i);  
-             user.nickName=("supben" + i);  
-             users.add(user);  
-         }    
- return users;  
-     }    
- 
- // 子路径
- @Path("getUserById")  
- @GET
- @Produces( {MediaType.APPLICATION_JSON })  
- public User getUsersById(){  
-         User user = new User();  
- //使用spring的类读取数据库信息，随便赋值
-         MobileNumber object=mobileNumberService.getMobileNumberById(Long.valueOf(userId));  
-         user.userId=object.getId();  
-         user.userName=object.getUsersName();  
-         user.nickName=object.getMobileNumber();  
- return user;  
-     }  
- @Path("getUserXmlById")  
- @GET
- @Produces( {MediaType.APPLICATION_XML })  
- public User getUsersByIdForXML(){  
-         User user = new User();  
-         MobileNumber object=mobileNumberService.getMobileNumberById(Long.valueOf(userId));  
-         user.userId=object.getId();  
-         user.userName=object.getUsersName();  
-         user.nickName=object.getMobileNumber();  
- return user;  
-     }  
- @Path("setUser")  
- @PUT
- @Produces( {MediaType.APPLICATION_JSON })  
- public User setUser(User user){  
- if(user==null){  
-             System.out.println("数值为空！！");  
-             user.userId=(13486197987L);  
-             user.userName=("yuhan");  
-             user.nickName=("supben");  
-         }else{  
-             System.out.println("user.userId="+user.userId);  
-             System.out.println("user.userName="+user.userName);  
-             System.out.println("user.nickName="+user.nickName);  
-             user.userId=(user.userId);  
-             user.userName=(user.userName);  
-             user.nickName=(user.nickName);        
-         }  
- 
- return user;  
-     }  
- // 子路径
- @Path("getUserByXML")  
- @GET
- @Produces( {MediaType.APPLICATION_XML })  
- public User getUserByIdWithXML() throws UnsupportedEncodingException{  
-         User user = new User();  
-         user.userId=(10000L);  
-         user.userName=URLEncoder.encode(("哈哈"), "utf-8");  
-         user.nickName=URLEncoder.encode(("老色狼"), "utf-8");  
- return user;  
-     }    
- 
- 
- }  
然后通过浏览器发布出去，在浏览器中即可通过
[http://localhost:8080/api/bookapi/getUserByXML](http://localhost:8080/bookapi/getUserByXML)获得一个xml格式的对象
[http://localhost:8080/api/bookapi/setUser](http://localhost:8080/bookapi/setUser)可以对一个user对象进行赋值
[http://localhost:8080/api/bookapi/getUserXmlById](http://localhost:8080/api/bookapi/getUserXmlById)可以传入一个id参数，获得一个xml格式的对象
**6.client的demo演示**
Java代码  ![收藏代码](http://xosadan.iteye.com/images/icon_star.png)
- package com.xxx.locationtalk.api;  
- 
- import java.io.UnsupportedEncodingException;  
- 
- import javax.ws.rs.core.MediaType;  
- import javax.ws.rs.core.MultivaluedMap;  
- 
- import com.sun.jersey.api.client.Client;  
- import com.sun.jersey.api.client.ClientResponse;  
- import com.sun.jersey.api.client.WebResource;  
- import com.sun.jersey.api.client.config.ClientConfig;  
- import com.sun.jersey.api.client.config.DefaultClientConfig;  
- import com.sun.jersey.api.json.JSONConfiguration;  
- import com.sun.jersey.core.util.MultivaluedMapImpl;  
- import com.sun.jersey.json.impl.provider.entity.JSONRootElementProvider;  
- 
- publicclass ClientDemo{  
- publicstaticvoid main(String[] args) throws UnsupportedEncodingException{  
-         ClientConfig clientConfig = new DefaultClientConfig();  
-         clientConfig.getFeatures().put(JSONConfiguration.FEATURE_POJO_MAPPING, Boolean.TRUE);  
- 
- 
-         Client c = Client.create(clientConfig);  
-         String url = "http://localhost:8080/api/bookapi";  
-         WebResource r = c.resource(url);  
- 
-         User user = new User();  
-         user.userId=1000L;  
-         user.userName="江江123";  
-         user.nickName="老色狼";  
- 
-         System.out.println("状态码="+r.path("setUser").accept(new String[] {MediaType.APPLICATION_JSON}).  
-         entity(user, MediaType.APPLICATION_JSON).put(ClientResponse.class).getStatus());  
- 
- 
-         MultivaluedMap<String, String> param = new MultivaluedMapImpl();    
-         param.add("userId", "10000");    
-         param.add("userName", "脏话");  
-         param.add("nickName", "脏话色狼");  
- 
-         System.out.println(r.queryParams(param)    
-                 .type(MediaType.APPLICATION_JSON).post(String.class));  
- 
- 
-     }  
- }  
** 这个client演示了俩种rest的实现方法**
输入URL Query参数 client.path("/xxx").queryParam("name", name)输入JSON编码的对象 client.path("/xxx").entity(user, MediaType.APPLICATION_JSON)
附件包括
一张相关的jersey用到的所有lib截图
官方下载的最新jersey说明文档
官方下载的jersey1.8.zip，包括核心的lib以及doc
