# Jersey实现Restful webservice及Ajax访问 - 零度的博客专栏 - CSDN博客
2016年05月06日 15:12:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：3494
jsp
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<html>
  <head>
   <script src="js/jquery/jquery.js"></script>
   <script>
   		function rest1(){
   			$.ajax({
			type: 'POST',			
			url: 'http://localhost:8080/RestDemo/webapi/users',
			dataType: "json",
            contentType: "application/json",
            data: "{\"userId\":\"004\", \"userName\":\"Susan\", \"age\":\"33\"}",
			cache : false,
			error:function(){alert('系统连接失败，请稍后再试！')},
			success: function(obj){	
				alert(obj.userName);
			}	  
		});
   		}
   </script>
   
    <script>
   		function rest2(){
   			$.ajax({
			type: 'POST',
			url: 'http://localhost:8080/RestDemo/webapi/users/user',
			dataType: "json",
            contentType: "application/x-www-form-urlencoded",             
            // data: "{\"userId\":\"004\", \"userName\":\"Susan\", \"age\":\"33\"}",
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
   </script>
   
    <script>
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
    for more information on Jersey!
    
      <input type="button" onclick="rest1();"  value="sss">
      <input type="button" onclick="rest2();" value="aaa">
      <input type="button" onclick="rest3();" value="ddd">
  </body>
</html>
```
java
```
package org.nercita.zmx;
import javax.xml.bind.annotation.XmlRootElement;
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
   public User addStudent(@FormParam("j") String j)
   {
		System.out.println(j);
		User user  = new User();    
	    user.setAge("27");    
	    user.setUserId("005");    
	    user.setUserName("Fmand");    
	    return user;
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
   	return u;
   }
  
   
    
 
   
}
```
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
web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!-- This web.xml file is not required when using Servlet 3.0 container,
     see implementation details http://jersey.java.net/nonav/documentation/latest/jax-rs.html -->
<web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
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
pom.xml
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>org.nercita.ltxx</groupId>
    <artifactId>RestDemo</artifactId>
    <packaging>war</packaging>
    <version>0.0.1-SNAPSHOT</version>
    <name>RestDemo</name>
    <build>
        <finalName>RestDemo</finalName>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>2.5.1</version>
                <inherited>true</inherited>
                <configuration>
                    <source>1.7</source>
                    <target>1.7</target>
                </configuration>
            </plugin>
        </plugins>
    </build>
    <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>org.glassfish.jersey</groupId>
                <artifactId>jersey-bom</artifactId>
                <version>${jersey.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>
    <dependencies>
        
        <dependency>
            <groupId>org.glassfish.jersey.containers</groupId>
            <artifactId>jersey-container-servlet-core</artifactId>
            <!-- use the following artifactId if you don't need servlet 2.x compatibility -->
            <!-- artifactId>jersey-container-servlet</artifactId -->
        </dependency>
        
        <!-- uncomment this to get JSON support
        <dependency>
            <groupId>org.glassfish.jersey.media</groupId>
            <artifactId>jersey-media-moxy</artifactId>
        </dependency>
        -->
	    
		<!--Jackson  Json支持 -->
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
	    
    </dependencies>
    <properties>
        <jersey.version>2.22.2</jersey.version>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>
</project>
```
