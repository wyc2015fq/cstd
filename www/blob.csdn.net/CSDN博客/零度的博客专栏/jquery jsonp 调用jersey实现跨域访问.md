# jquery jsonp 调用jersey实现跨域访问 - 零度的博客专栏 - CSDN博客
2016年06月15日 16:01:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：1592
1、web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!-- This web.xml file is not required when using Servlet 3.0 container,
     see implementation details http://jersey.java.net/nonav/documentation/latest/jax-rs.html -->
<web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
    
    <security-constraint> 
	     <web-resource-collection> 
	         <web-resource-name>My App</web-resource-name> 
	         <url-pattern>/test/*</url-pattern> 
	     </web-resource-collection> 
	     <auth-constraint> 
	         <role-name>tomcat</role-name> 
	     </auth-constraint> 
    </security-constraint> 
    <login-config> 
	     <auth-method>DIGEST</auth-method>  <!-- DIGEST here --> 
	     <realm-name>My Realm</realm-name> 
    </login-config>
    
    
    <welcome-file-list>
         <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>
    
      
    
    <servlet>
        <servlet-name>Jersey Web Application</servlet-name>
        <servlet-class>org.glassfish.jersey.servlet.ServletContainer</servlet-class>
        <!-- <init-param>
            <param-name>jersey.config.server.provider.packages</param-name>
            <param-value>org.nercita.zmx</param-value>
        </init-param> -->
        <init-param>  
            <param-name>javax.ws.rs.Application</param-name>  
            <param-value>org.nercita.zmx.RestApplication</param-value>  
        </init-param>
        
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>Jersey Web Application</servlet-name>
        <url-pattern>/webapi/*</url-pattern>
    </servlet-mapping>
</web-app>
```
2、User.java
```
package org.nercita.zmx;
import javax.xml.bind.annotation.XmlRootElement;
@XmlRootElement
public class User {
	
	private String userId;
	private String userName;
	private String age;
	private String email;
	
	public User() {};
	
	public User(String userId, String userName, String age, String email) {
		this.userId = userId;
		this.userName = userName;
		this.age = age;
		this.email = email;
	}
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
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
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
	@Override
	public String toString() {
		return "{ \"userId\" : \"" + userId + "\", \"userName\" : \"" + userName + "\", \"age\" :\""
				+ age + "\", \"email\" :\"" + email + "\"}";
	}
	
	
	
}
```
3.RestApplication.java
```
package org.nercita.zmx;
import org.codehaus.jackson.jaxrs.JacksonJsonProvider;
import org.glassfish.jersey.server.ResourceConfig;
public class RestApplication extends ResourceConfig {
	
	
     public RestApplication() {
		
          //服务类所在的包路径  
          packages("org.nercita.zmx");  
          //注册JSON转换器  
          register(JacksonJsonProvider.class); 
     
     }
}
```
4.服务端：UserResource.java
```
package org.nercita.zmx;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
@Path("/users")
public class UserResource {
	
	private static Map<String,User> userMap = new HashMap<String,User>();//存储用户
	
	 /**
     * 查询所有
     * @return
     */
    @GET
    @Produces(MediaType.APPLICATION_XML)
    public List<User> getAllUsers(){     
		List<User> users = new ArrayList<User>();
		User u1 = new User("001","WayLau","26");
		User u2 = new User("002","King","23");
		User u3 = new User("003","Susan","21");
		
		userMap.put(u1.getUserId(), u1);
		userMap.put(u2.getUserId(), u2);
		userMap.put(u3.getUserId(), u3);
		
		users.addAll( userMap.values() );
		return users;
    }
    
    @GET
    @Path("/getUserXml")
    @Produces(MediaType.APPLICATION_XML)
    public User getUserXml() {
     User user  = new User();
     user.setAge("21");
     user.setUserId("004");
     user.setUserName("Amand");
     return user;
    }
    
    
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
   
   @GET    
   @Path("/getUserJsonp")    
   @Consumes(MediaType.APPLICATION_FORM_URLENCODED)  
   @Produces(MediaType.TEXT_PLAIN)
   public String getUserJsonp(@QueryParam("data") String data,@QueryParam("callback") String jsonpcallback) {    
	 System.out.println("data="+ data);  
	 System.out.println("jsonpcallback="+ jsonpcallback);	 
	 User user  = new User();    
     user.setAge("27");    
     user.setUserId("005");    
     user.setUserName("Fmand");  
    // return jsonpcallback+"( {\"id\":\"1001\"})";  
     return jsonpcallback+"("+user.toString()+")";
   }
   
   
   
   
   /**
	* 增加
	* @param user
	*/
   @POST
   @Consumes({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
   @Produces(MediaType.APPLICATION_JSON)
   public User createStudent(User user)
   {
		userMap.put(user.getUserId(), user );
		return user;
   }
   
   
   /**
	* 增加
	* @param user
	*/
   @POST
   @Path("/user")
   @Produces(MediaType.APPLICATION_JSON)
   public User addStudent(@FormParam("user")String user)
   {
		System.out.println(user);
		
		User u  = new User();    
	    u.setAge("27");    
	    u.setUserId("005");    
	    u.setUserName("Fmand");
		userMap.put(u.getUserId(), u );    
	    return u;
   }
   
   
   /**
	* 删除
	* @param user
	*/
   @POST
   @Path("/delete/{id}") 
   @Produces(MediaType.APPLICATION_JSON)
   @Consumes({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
   public User delStudent(@PathParam("id")String id)
   {
		System.out.println(id);
		User user  = new User();    
	    user.setAge("27");    
	    user.setUserId("005");    
	    user.setUserName("Fmand");    
	    return user;
   }
   
   
	
	/**
	 * 删除
	 * @param id
	 */
   @DELETE
   @Path("{id}")
   public void deleteStudent(@PathParam("id")String id){
   	userMap.remove(id);
   }
   
   
   
   
   /**
    * 修改
    * @param user
    */
   @PUT
   @Consumes(MediaType.APPLICATION_XML)
   public void updateStudent(User user){
		userMap.put(user.getUserId(), user );
   }
   /**
    * 根据id查询
    * @param id
    * @return
    */
   @GET
   @Path("{id}")
   @Produces({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
   public User getUserById(@PathParam("id") String id){
   	User u = userMap.get(id);
    u= new User();
    u.setAge("27");    
    u.setUserId("005");    
    u.setUserName("Fmand");    
   	System.out.println(u);
   	return u;
   }
  
   
    
 
   
}
```
服务端jsonp包装实现代码：
```
@GET    
   @Path("/getUserJsonp")    
   @Consumes(MediaType.APPLICATION_FORM_URLENCODED)  
   @Produces(MediaType.TEXT_PLAIN)
   public String getUserJsonp(@QueryParam("data") String data,@QueryParam("callback") String jsonpcallback) {    
	 System.out.println("data="+ data);  
	 System.out.println("jsonpcallback="+ jsonpcallback);	 
	 User user  = new User();    
     user.setAge("27");    
     user.setUserId("005");    
     user.setUserName("Fmand");  
    // return jsonpcallback+"( {\"id\":\"1001\"})";  
     return jsonpcallback+"("+user.toString()+")";
   }
```
5、客户端 index.jsp
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<html>
  <head>
  
   <script src="../js/jquery/jquery.js"></script>
   <script>
   		function rest1(){
   			$.ajax({
			type: 'GET',
		//	async:false,  			
			url: 'http://localhost:8080/RestDemo/webapi/users/getUserJsonp?data=data',
			dataType: "jsonp",
		//	contentType: "application/x-www-form-urlencoded",
			jsonp: "callback",//服务端用于接收callback调用的function名的参数  
		//	cache : false,
			error:function(){alert('系统连接失败，请稍后再试！')},
			success: function(obj){	
				alert(obj.userName);
			}	  
		});
   		}
   		function rest2(){
   			$.ajax({
			type: 'POST',
			url: 'http://localhost:8080/RestDemo/webapi/users/user',
			dataType: "json",			                                   
            contentType: "application/x-www-form-urlencoded",
          //data: "{\"userId\":\"004\", \"userName\":\"Susan\", \"age\":\"33\"}",
            data:  {
              		"user": "{\"userId\":\"004\", \"userName\":\"Susan\", \"age\":\"33\"}"
          		   },
			cache : false,
			error:function(){alert('系统连接失败，请稍后再试！')},
			success: function(obj){	
				alert(obj.userName);
			}	  
		});
   		}
   		function rest3(){
   			$.ajax({
			type: 'POST',
			url: 'http://localhost:8080/RestDemo/webapi/users/delete/001',
			dataType: "json",
            contentType: "application/json",
          //  data: "{\"userId\":\"004\", \"userName\":\"Susan\", \"age\":\"33\"}",           
			cache : false,
			error:function(){alert('系统连接失败，请稍后再试！')},
			success: function(obj){	
				alert(obj.userName);
			}	  
		});
   		}
   </script>
  </head>
  
  <body>
    <h2>Jersey RESTful Web Application!</h2>
    <p><a href="webapi/myresource">Jersey resource</a>
    <p>Visit <a href="http://jersey.java.net">Project Jersey website</a>
    for more information on Jersey!<br>
    
      <input type="button" onclick="rest1();"  value="sss">
      <input type="button" onclick="rest2();" value="aaa">
      <input type="button" onclick="rest3();" value="ddd">
  </body>
</html>
```
客户端jquery使用jsonp调用代码：
```
$.ajax({
				type: 'GET',
				async:false,  			
				url: 'http://localhost:8080/RestDemo/webapi/users/getUserJsonp?data=data',
				dataType: "jsonp",
				contentType: "application/x-www-form-urlencoded",
				jsonp: "callback",//服务端用于接收callback调用的function名的参数  
				cache : false,
				error:function(){alert('系统连接失败，请稍后再试！')},
				success: function(obj){	
					alert(obj.userName);
				}	  
		    });
```
          上述只需要注意关键的服务端jsop包装与客户端使用jsonp调用的核心代码（即红颜色的代码部分，已经单独拿出来，方便读者参考），就可以实现跨域的调用。
          最后我们进行测试，测试过程在一台笔记本上进行，为了模拟出跨域的效果，需要做一些配置。具体的做法是，修改系统的hosts文件，在Windows系统上，hosts文件位于%windir%/System32/Drivers/etc/目录下。本次测试中，定义了[www.example1.com](http://www.example1.com)和[www.example2.com](http://www.example2.com)两个域名，配置如下：
```
127.0.0.1 www.example1.com
127.0.0.1 www.example2.com
```
        具体过程就不在赘述，读者可通过修改url中的域名自行测试。注意：由于Jsonp本质上是通过动态<script> tag实现的，因此Jsonp只能支持GET方式的请求。

