# 用Jersey构建RESTful服务-JAVA对象转成XML/Json输出 - 零度的博客专栏 - CSDN博客
2016年05月06日 13:42:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：4530
1、XML数据类型输出：
一、 总体说明
XML和JSON 是最为常用的数据交换格式。本例子演示如何将java对象，转成XML输出。
二、流程
1.在上文的例子中，创建一个包“com.waylau.rest.bean”
2.在该包下创建一个JAVA类"User"
**[java]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21415735#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21415735#)[print](http://blog.csdn.net/kkkloveyou/article/details/21415735#)[?](http://blog.csdn.net/kkkloveyou/article/details/21415735#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/241378/fork)
- package com.waylau.rest.bean;  
- import javax.xml.bind.annotation.XmlRootElement;  
- /*
-  * 用户 bean
-  * @author waylau.com
-  * 2014-3-17
-  */
- @XmlRootElement
- publicclass User {  
- 
- private String userId;  
- private String userName;  
- private String age;  
- 
- public User() {};  
- 
- public User(String userId, String userName, String age) {  
- this.userId = userId;  
- this.userName = userName;  
- this.age = age;  
-     }  
- public String getUserId() {  
- return userId;  
-     }  
- publicvoid setUserId(String userId) {  
- this.userId = userId;  
-     }  
- public String getUserName() {  
- return userName;  
-     }  
- publicvoid setUserName(String userName) {  
- this.userName = userName;  
-     }  
- public String getAge() {  
- return age;  
-     }  
- publicvoid setAge(String age) {  
- this.age = age;  
-     }  
- }  
```java
package com.waylau.rest.bean;
import javax.xml.bind.annotation.XmlRootElement;
/*
 * 用户 bean
 * @author waylau.com
 * 2014-3-17
 */
@XmlRootElement
public class User {
	
	private String userId;
	private String userName;
	private String age;
	
	public User() {};
	
	public User(String userId, String userName, String age) {
		this.userId = userId;
		this.userName = userName;
		this.age = age;
	}
	public String getUserId() {
		return userId;
	}
	public void setUserId(String userId) {
		this.userId = userId;
	}
	public String getUserName() {
		return userName;
	}
	public void setUserName(String userName) {
		this.userName = userName;
	}
	public String getAge() {
		return age;
	}
	public void setAge(String age) {
		this.age = age;
	}
}
```
注意：该类上面增加了一个注解“@XmlRootElement”，在将该类转化成XML时，说明这个是XML的根节点。

3.在“com.waylau.rest.resources”中，增加资源“UserResource“，代码如下：
**[java]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21415735#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21415735#)[print](http://blog.csdn.net/kkkloveyou/article/details/21415735#)[?](http://blog.csdn.net/kkkloveyou/article/details/21415735#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/241378/fork)
- package com.waylau.rest.resources;  
- 
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.List;  
- import java.util.Map;  
- 
- import javax.ws.rs.Path;  
- import javax.ws.rs.Produces;  
- import javax.ws.rs.PathParam;  
- import javax.ws.rs.core.MediaType;  
- import javax.ws.rs.DELETE;  
- import javax.ws.rs.GET;  
- import javax.ws.rs.POST;  
- import javax.ws.rs.PUT;  
- 
- import com.waylau.rest.bean.User;  
- 
- @Path("/users")  
- publicclass UserResource {  
- privatestatic Map<String,User> userMap = new HashMap<String,User>();//存储用户
- /**
-      * 查询所有
-      * @return
-      */
- @GET
- @Produces(MediaType.APPLICATION_XML)  
- public List<User> getAllUsers(){       
-         List<User> users = new ArrayList<User>();  
-         User u1 = new User("001","WayLau","26");  
-         User u2 = new User("002","King","23");  
-         User u3 = new User("003","Susan","21");  
- 
-         userMap.put(u1.getUserId(), u1);  
-         userMap.put(u2.getUserId(), u2);  
-         userMap.put(u3.getUserId(), u3);  
- 
-         users.addAll( userMap.values() );  
- return users;  
-     }  
- 
- @GET
- @Path("/getUserXml")  
- @Produces(MediaType.APPLICATION_XML)  
- public User getUserXml() {  
-      User user  = new User();  
-      user.setAge("21");  
-      user.setUserId("004");  
-      user.setUserName("Amand");  
- return user;  
-     }  
- 
- 
- 
- }  
其中MediaType.APPLICATION_XML说明了是以XML形式输出
在浏览器输入http://localhost:8089/RestDemo/rest/users/getUserXml，输出单个对象
![](http://h.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=30ef47f39c2f07085b052a05d91fc9a4/267f9e2f07082838a80989a8ba99a9014c08f102.jpg?referer=276bf8ae700e0cf3f9e07bcb3f00&x=.jpg)
在浏览器输入 http://localhost:8089/RestDemo/rest/users  输出对象的集合
![](http://e.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=9cd4b18cb8a1cd1101b672258929b9c1/d000baa1cd11728b2318d99ecafcc3cec3fd2c50.jpg?referer=ac9db1cfaf4bd1135dda82028c4e&x=.jpg)
2、数据输出为JSON类型：
一、 总体说明
XML和JSON 是最为常用的数据交换格式。本例子演示如何将java对象，转成JSON输出。
二、流程
1.在上文项目中，
在“com.waylau.rest.resources.UserResource“中增加代码，代码如下：
**[java]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[print](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[?](http://blog.csdn.net/kkkloveyou/article/details/21485243#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243856/fork)
- @GET
- @Path("/getUserJson")    
- @Produces(MediaType.APPLICATION_JSON)    
- public User getUserJson() {    
-  User user  = new User();    
-  user.setAge("27");    
-  user.setUserId("005");    
-  user.setUserName("Fmand");    
- return user;    
- }   
```java
@GET  
    @Path("/getUserJson")  
    @Produces(MediaType.APPLICATION_JSON)  
    public User getUserJson() {  
     User user  = new User();  
     user.setAge("27");  
     user.setUserId("005");  
     user.setUserName("Fmand");  
     return user;  
    }
```
MediaType.APPLICATION_JSON 说明输出的是JSON格式
2,运行项目，浏览器输入[http://localhost:8089/RestDemo/rest/users/getUserJson](http://localhost:8089/RestDemo/rest/users/getUserJson)
期望获取到json的数据，此时，项目报错
**[html]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[print](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[?](http://blog.csdn.net/kkkloveyou/article/details/21485243#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243856/fork)
- org.glassfish.jersey.message.internal.MessageBodyProviderNotFoundException: MessageBodyWriter not found for media type=application/json, type=class com.waylau.rest.bean.User, genericType=class com.waylau.rest.bean.User.  
-     at org.glassfish.jersey.message.internal.WriterInterceptorExecutor$TerminalWriterInterceptor.aroundWriteTo(WriterInterceptorExecutor.java:247)  
-     at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)  
-     at org.glassfish.jersey.filter.LoggingFilter.aroundWriteTo(LoggingFilter.java:293)  
-     at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)  
-     at org.glassfish.jersey.server.internal.JsonWithPaddingInterceptor.aroundWriteTo(JsonWithPaddingInterceptor.java:103)  
-     at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)  
-     at org.glassfish.jersey.server.internal.MappableExceptionWrapperInterceptor.aroundWriteTo(MappableExceptionWrapperInterceptor.java:88)  
-     at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)  
-     at org.glassfish.jersey.message.internal.MessageBodyFactory.writeTo(MessageBodyFactory.java:1154)  
-     at org.glassfish.jersey.server.ServerRuntime$Responder.writeResponse(ServerRuntime.java:571)  
-     at org.glassfish.jersey.server.ServerRuntime$Responder.processResponse(ServerRuntime.java:378)  
-     at org.glassfish.jersey.server.ServerRuntime$Responder.process(ServerRuntime.java:368)  
-     at org.glassfish.jersey.server.ServerRuntime$1.run(ServerRuntime.java:262)  
```
org.glassfish.jersey.message.internal.MessageBodyProviderNotFoundException: MessageBodyWriter not found for media type=application/json, type=class com.waylau.rest.bean.User, genericType=class com.waylau.rest.bean.User.
	at org.glassfish.jersey.message.internal.WriterInterceptorExecutor$TerminalWriterInterceptor.aroundWriteTo(WriterInterceptorExecutor.java:247)
	at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)
	at org.glassfish.jersey.filter.LoggingFilter.aroundWriteTo(LoggingFilter.java:293)
	at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)
	at org.glassfish.jersey.server.internal.JsonWithPaddingInterceptor.aroundWriteTo(JsonWithPaddingInterceptor.java:103)
	at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)
	at org.glassfish.jersey.server.internal.MappableExceptionWrapperInterceptor.aroundWriteTo(MappableExceptionWrapperInterceptor.java:88)
	at org.glassfish.jersey.message.internal.WriterInterceptorExecutor.proceed(WriterInterceptorExecutor.java:162)
	at org.glassfish.jersey.message.internal.MessageBodyFactory.writeTo(MessageBodyFactory.java:1154)
	at org.glassfish.jersey.server.ServerRuntime$Responder.writeResponse(ServerRuntime.java:571)
	at org.glassfish.jersey.server.ServerRuntime$Responder.processResponse(ServerRuntime.java:378)
	at org.glassfish.jersey.server.ServerRuntime$Responder.process(ServerRuntime.java:368)
	at org.glassfish.jersey.server.ServerRuntime$1.run(ServerRuntime.java:262)
```
![](http://g.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=5f3758b36509c93d03f20ef2af0689e1/7e3e6709c93d70cfade7b8b0fadcd100baa12b69.jpg?referer=70ed5c049c510fb3210e42a79527&x=.jpg)
此时，需要获取json转换包的支持 。
可以由多种方式实现：MOXy、JSON-P、Jackson、Jettison等，本例为Jackson。
3. jackson-all-1.9.11.jar 下载地址[http://wiki.fasterxml.com/JacksonDownload](http://wiki.fasterxml.com/JacksonDownload)
4. 项目中引入jackson-all-1.9.11.jar
5.在“com.waylau.rest”目录下创建RestApplication.java
**[java]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[print](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[?](http://blog.csdn.net/kkkloveyou/article/details/21485243#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243856/fork)
- package com.waylau.rest;  
- 
- import org.codehaus.jackson.jaxrs.JacksonJsonProvider;  
- import org.glassfish.jersey.filter.LoggingFilter;  
- import org.glassfish.jersey.server.ResourceConfig;  
- 
- /**
-  * 应用
-  * @author waylau.com
-  * 2014-3-18
-  */
- publicclass RestApplication extends ResourceConfig {  
- public RestApplication() {  
- 
- //服务类所在的包路径
-      packages("com.waylau.rest.resources");  
- //注册JSON转换器
-      register(JacksonJsonProvider.class);  
- 
-     }  
- }  
```java
package com.waylau.rest;
import org.codehaus.jackson.jaxrs.JacksonJsonProvider;
import org.glassfish.jersey.filter.LoggingFilter;
import org.glassfish.jersey.server.ResourceConfig;
 
/**
 * 应用
 * @author waylau.com
 * 2014-3-18
 */
public class RestApplication extends ResourceConfig {
    public RestApplication() {
 
     //服务类所在的包路径
     packages("com.waylau.rest.resources");
     //注册JSON转换器
     register(JacksonJsonProvider.class);
 
    }
}
```
6.修改web.xml，初始化从RestApplicaton进入应用，如下：
**[html]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[print](http://blog.csdn.net/kkkloveyou/article/details/21485243#)[?](http://blog.csdn.net/kkkloveyou/article/details/21485243#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243856/fork)
- <servlet>
- <servlet-name>Way REST Service</servlet-name>
- <servlet-class>org.glassfish.jersey.servlet.ServletContainer</servlet-class>
- <init-param>
- <param-name>javax.ws.rs.Application</param-name>
- <param-value>com.waylau.rest.RestApplication</param-value>
- </init-param>
- 
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>Way REST Service</servlet-name>
- <url-pattern>/rest/*</url-pattern>
- </servlet-mapping>
```
<servlet>  
     <servlet-name>Way REST Service</servlet-name>
	 <servlet-class>org.glassfish.jersey.servlet.ServletContainer</servlet-class>
	  <init-param>
	       <param-name>javax.ws.rs.Application</param-name>
	       <param-value>com.waylau.rest.RestApplication</param-value>
	   </init-param>
	 
    <load-on-startup>1</load-on-startup>
  </servlet>
  
  <servlet-mapping>
    <servlet-name>Way REST Service</servlet-name>
    <url-pattern>/rest/*</url-pattern>
  </servlet-mapping>
```
7.运行项目，再次访问[http://localhost:8089/RestDemo/rest/users/getUserJson](http://localhost:8089/RestDemo/rest/users/getUserJson)
即可输出JSON文本
![](http://a.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=d3225f7df536afc30a0c3f6083229af9/79f0f736afc37931da389494e9c4b74543a9113e.jpg?referer=796c6b856c061d95245103085fd5&x=.jpg)
