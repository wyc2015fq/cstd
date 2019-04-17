# Resteasy配置及其使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月02日 10:03:32[boonya](https://me.csdn.net/boonya)阅读数：20449








      resteasy秉承restful风格，它的简单已配置比使用servlet更加方便。它支持PUT   、 POST 、  GET  等常用HTTP请求方式，并且提供annotation的 Java类可配置功能将Java对象数据JSON转化。

**1、访问方式**

  PUT    POST   GET   （常用HTTP请求方式）
**2、下载资源jar包支持[如图]![](https://img-blog.csdn.net/20131102095920203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)3、web.xml配置**

```
<context-param>
		<param-name>resteasy.resources</param-name>
		<param-value>com.rest.UserService</param-value>
	</context-param>
	
	<context-param>
		<param-name>resteasy.servlet.mapping.prefix</param-name>
		<param-value>/service</param-value>
	</context-param>


	<listener>
		<listener-class>
			org.jboss.resteasy.plugins.server.servlet.ResteasyBootstrap
		</listener-class>
	</listener>


	<servlet>
		<servlet-name>Resteasy</servlet-name>
		<servlet-class>org.jboss.resteasy.plugins.server.servlet.HttpServletDispatcher</servlet-class>
	</servlet>


	<servlet-mapping>
		<servlet-name>Resteasy</servlet-name>
		<url-pattern>/services/*</url-pattern>
	</servlet-mapping>
```
**4、编写控制访问**

```java
package com.rest;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
//import javax.ws.rs.FormParam;//获取表单参数
import javax.ws.rs.Produces;
//import javax.ws.rs.core.Context;//请求上下文HttpServletRequest
//import javax.ws.rs.QueryParam;//用于获取数据库查询参数
@Path("userservice")
public class UserService {
	/**
	 * 初始化用户数据
	 */
	static Map<Integer,Object> map=new HashMap<Integer, Object>();
	
	static{
		User user1=new User("linda","623423","163242@ms.com",23,"female");
		map.put(1, user1);
		User user2=new User("veppa","734423","7d3242@ms.com",21,"male");
		map.put(2, user2);
		User user3=new User("nokka","973423","1fddf2@ms.com",23,"female");
		map.put(3, user3);
	}
	
	/**
	 * 获取指定Id的用户信息
	 * @param id
	 * @return
	 */
	@GET
	@Path("user/{id}")
	@Produces("application/json")
	public User getById(@PathParam("id") Integer id) {
	    return (User) map.get(id);
	}
	
	/**
	 * 获取所有用户列表的JSON数据
	 * @return
	 */
	@GET
	@Path("users")
	@Produces("application/json")
	public List<User> getUsers() {
		List<User> users=new ArrayList<User>();
		for(Integer index: map.keySet()){
			User user=(User) map.get(index);
			users.add(user);
		}
	    return users; 
	}
	
	/**
	 * 获取的用户传入的信息
	 * @param msg
	 * @return
	 */
	@GET
	@Path("user/trans/{msg}")
	public String  getMessage(@PathParam("msg") String msg) {
	    return "[Hello dear! ]"+",MSG:{"+msg+"}";
	}


}
```
**5、测试**

  输入访问:http://localhost:8080/resteasy/services/userservice/users

 打印结果： [{"email":"163242@ms.com","age":23,"gender":"female","name":"linda","password":"623423"},

 {"email":"7d3242@ms.com","age":21,"gender":"male","name":"veppa","password":"734423"},

 {"email":"1fddf2@ms.com","age":23,"gender":"female","name":"nokka","password":"973423"}]
            


