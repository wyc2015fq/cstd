# Restful webservice 注意事项 - z69183787的专栏 - CSDN博客
2012年10月27日 12:38:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1923
2篇文章，大体概述了 rest 的架构及一般操作
[http://www.vogella.com/articles/REST/article.html](http://www.vogella.com/articles/REST/article.html)
[http://www.ibm.com/developerworks/cn/web/wa-aj-tomcat/](http://www.ibm.com/developerworks/cn/web/wa-aj-tomcat/)
所需jar包：jersey-server jersey-core jersey-api jsr311-api-1.0 asm-3.1若需使用客服端功能，需要 jersey-client。客户端功能如何使用，在上述2个参考资料中有。
在自己使用时碰到了一些问题，比如 请求数据格式为json时，会遇到类型问题。尝试后，发现需要对该json对应的bo对象进行 jaxb 序列化。
jsp
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<html>
  <head>
   <script src="js/jquery-1.7.1.js"></script>
   
   <script>
   		function rest(){
   			$.ajax({
			type: 'POST',
			//url: 'services/helloWorld/doGet2',
			url: 'services/helloWorld/test1019',
			dataType: "json",
            contentType: "application/json",
          //  data: "{\"name\":\"zhangsan\",\"age\":28,\"address\":\"beijing\",\"tel\":\"01082658866\"}",
            data:  "{\"name\":\"zhangsan\"}",
			cache : false,
			error:function(){alert('系统连接失败，请稍后再试！')},
			success: function(obj){	
				alert(obj.name);
			}	  
		});
   		}
   </script>
   
    <script>
   		function rest2(){
   			$.ajax({
			type: 'POST',
			//url: 'services/helloWorld/doGet2',
			url: 'services/helloWorld/test1020',
			dataType: "json",
            contentType: "application/json",
          //  data: "{\"name\":\"zhangsan\",\"age\":28,\"address\":\"beijing\",\"tel\":\"01082658866\"}",
            data:  {
              		"j":"{\"name\":\"zhangsan\"}"
          			}	,
			cache : false,
			error:function(){alert('系统连接失败，请稍后再试！')},
			success: function(obj){	
				alert(obj.name);
			}	  
		});
   		}
   </script>
   
    <script>
   		function rest3(){
   			$.ajax({
			type: 'POST',
			//url: 'services/helloWorld/doGet2',
			url: 'services/helloWorld/delete/10',
			dataType: "json",
            contentType: "application/json",
          //  data: "{\"name\":\"zhangsan\",\"age\":28,\"address\":\"beijing\",\"tel\":\"01082658866\"}",
            data:  {
              		"j":"{\"name\":\"zhangsan\"}"
          			}	,
			cache : false,
			error:function(){alert('系统连接失败，请稍后再试！')},
			success: function(obj){	
				alert(obj.name);
			}	  
		});
   		}
   </script>
  </head>
  
  <body>
    This is my JSP page. <br>
    
    <button onclick="rest();" value="sss">
    <button onclick="rest2();" value="aaa">
      <button onclick="rest3();" value="ddd">
  </body>
</html>
```
java：
```java
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import javax.servlet.http.HttpServletResponse;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.HEAD;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Request;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriInfo;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
/*@Context： 使用该注释注入上下文对象，比如 Request、Response、UriInfo、ServletContext 等。
@Context  
    Request request;  
    @Context  
    UriInfo uriInfo;   内部类
@Path("{contact}")：这是 @Path 注释，与根路径 “/contacts” 结合形成子资源的 URI。
@PathParam("contact")：该注释将参数注入方法参数的路径，在本例中就是联系人 id。
其他可用的注释有 @FormParam、@QueryParam 等。
@Produces：响应支持多个 MIME 类型。在本例和上一个示例中，APPLICATION/XML 将是默认的 MIME 类型。
*/
@Path("helloWorld")
public class helloWorld {
	@Context
	UriInfo uriInfo;
	@Context
	Request request;
	String contact;
	
	public helloWorld(UriInfo uriInfo, Request request, 
			String contact) {
		this.uriInfo = uriInfo;
		this.request = request;
		this.contact = contact;
	}
	
	private String age;
	
	@GET  
    @Path("/query")  
    public Response getUsers(  
        @QueryParam("from") int from,  
        @QueryParam("to") int to,  
        @QueryParam("orderBy") List<String> orderBy) {  
   
        return Response  
           .status(200)  
           .entity("getUsers is called, from : " + from + ", to : " + to  
            + ", orderBy" + orderBy.toString()).build();  
   
    }  
	
	/*URL输入为:users/query?from=100&to=200&orderBy=age&orderBy=name
			此时,输出为:
			getUsers is called, from : 100, to : 200, orderBy[age, name] 
	
	要注意的是,跟@pathparam不同,@queryparam中,指定的是URL中的参数是以键值对的形式出现的,而在程序中
	@QueryParam("from") int from则读出URL中from的值, 而@pathparem中,URL中只出现参数的值,不出现键值对,比如: “/users/2011/06/30” */
	@GET  
	@Path("{year}/{month}/{day}")  
    public Response getUserHistory(  
            @PathParam("year") int year,  
            @PathParam("month") int month,   
            @PathParam("day") int day) {  
   
       String date = year + "/" + month + "/" + day;  
   
       return Response.status(200)  
        .entity("getUserHistory is called, year/month/day : " + date)  
        .build();  
   
    }
	@GET
	@Path("sayHi")
	//@Path("{contact}") 
	//AA  (@PathParam("contact") String contact)
	//访问URL /helloWorld/XXX  contact = XXX
	public String sayHi(@DefaultValue("s123") @QueryParam("name") String yourname) {
		//System.out.println(yourname);
		return yourname;
	}
	
	@GET
	@Path("doGet")
	@Produces(MediaType.APPLICATION_JSON)
	public List<Person> doGet(){
		System.out.println("doGet");
		Person p = new Person("1",2,"3","4");
		Person p1 = new Person("1",2,"3","4");
		List<Person> l = new ArrayList<Person>();
		l.add(p);l.add(p1);
		return l;
	}
	
	@GET
	@Path("doGet2")
	@Produces(MediaType.APPLICATION_JSON)
	public Person doGet2(){
		Person p = new Person("1",2,"3","4");
		return p;
	}
	
	
	@POST
	@Path("doPost")
	@Consumes(MediaType.APPLICATION_JSON)
	@Produces("text/html")
	public String doPost(Person person){
		System.out.println("doPost`1234 "+person.getName()+" "+person.getAge());
		return "doPost111";
	}
	
	@DELETE
	@Path("doDelete")
	public String doDelete(){
		System.out.println("doDelete");
		return "doDelete";
	}
	
	@PUT
	@Path("doPut")
	public String doPut(){
		System.out.println("doPut");
		return "doPut";
	}
	
	@HEAD
	@Path("doHead")
	public String doHead(){
		System.out.println("doHead");
		return "doHead";
	}
	@POST
	@Path("test1019")
	@Produces(MediaType.APPLICATION_JSON)
	@Consumes( {MediaType.APPLICATION_JSON, MediaType.APPLICATION_XML })
	public Test test1019(Test t) {
		//throw new UnsupportedOperationException("Not yet implemented.");
		System.out.println(t.getName());
		t.setName("zs");
		return t;
	}
	@POST
	@Path("test1020")
	public Test test1020(@FormParam("j") String j) {
		//throw new UnsupportedOperationException("Not yet implemented.");
		System.out.println(j);
		Test t = new Test();
		t.setName("zs");
		return t;
	}
	
	@POST
    @Path("delete/{id}")
    public String deleteCategory(@PathParam("id") int id) {
        System.out.println("处理删除类别逻辑，接受的数据为id:"+id);
        return "ok";
    }
	 
	
	
	@POST
	@Path("new")
	@Produces(MediaType.TEXT_HTML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public void newContact(
			@FormParam("id") String id,
			@FormParam("name") String name,
			@Context HttpServletResponse servletResponse
	) throws IOException {
		Contact c = new Contact(id,name,new ArrayList<String>());
			
		URI uri = uriInfo.getAbsolutePathBuilder().path(id).build();
		Response.created(uri).build();
			
		//servletResponse.sendRedirect("../pages/new_contact.html");
	}  
	
	@SuppressWarnings("unused")
	@POST
	@Path("update")
	@Produces(MediaType.TEXT_HTML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	private Response putAndGetResponse(Contact c) {
		Response res;
		res = Response.created(uriInfo.getAbsolutePath()).build();
		return res;
	}   
	
	
	public static void main(String[] args){
		Person p = new Person("1",2,"3","4");
		Person p1 = new Person("1",2,"3","4");
		List<Person> l = new ArrayList<Person>();
		l.add(p);l.add(p1);
		JSONObject json = JSONObject.fromObject(l);
		System.out.println(json);
	}
	
}
```
bo：json格式也需要jaxb的注解 不是很明白。其次 映射时，若属性为public 则需要在 get方法上增加@XMLTransient，表示不序列化此属性；
或将属性设为private，映射加至 get方法处。
```java
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;
@XmlRootElement(name = "test")
public class Test {
	public Test(){
		this.name="ddddd";
	}
	private String name;
	@XmlElement(name = "name")
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	
}
```
```java
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlTransient;
@XmlRootElement(name = "person")
public class Person
        {
        	@XmlElement(name = "name")
            public String name;
 			@XmlElement(name = "age")
            public int age ;
 			@XmlElement(name = "address")
            public String address ;
 			@XmlElement(name = "tel")
            public String tel ;
            public Person(String name,int age,String address,String tel){
            	this.name = name;
            	this.age = age;
            	this.address=address;
            	this.tel=tel;
            }
            public Person(){
            	
            }
            @XmlTransient
			public String getName() {
				return name;
			}
			public void setName(String name) {
				this.name = name;
			}
			@XmlTransient
			public int getAge() {
				return age;
			}
			public void setAge(int age) {
				this.age = age;
			}
			@XmlTransient
			public String getAddress() {
				return address;
			}
			public void setAddress(String address) {
				this.address = address;
			}
			@XmlTransient
			public String getTel() {
				return tel;
			}
			public void setTel(String tel) {
				this.tel = tel;
			}
            
            
        }
```
web.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5" 
	xmlns="http://java.sun.com/xml/ns/javaee" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
	http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
  <servlet>
  	<display-name>JAX-RS REST Servlet</display-name>
  	<servlet-name>JAX-RS REST Servlet</servlet-name>
  	<servlet-class>
  		com.sun.jersey.spi.container.servlet.ServletContainer
  	</servlet-class>
  	<!-- 指示包含资源的 Java 包。
  	<init-param>
	    <param-name>com.sun.jersey.config.property.packages</param-name>
	    <param-value>sample.hello.resources</param-value>
  	</init-param> -->
  	<load-on-startup>1</load-on-startup>
  </servlet>
  <servlet-mapping>
  	<servlet-name>JAX-RS REST Servlet</servlet-name>
  	<url-pattern>/services/*</url-pattern>
  </servlet-mapping>
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
</web-app>
```

