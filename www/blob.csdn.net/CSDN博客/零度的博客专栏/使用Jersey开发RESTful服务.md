# 使用Jersey开发RESTful服务 - 零度的博客专栏 - CSDN博客
2016年05月06日 09:43:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：6848
**REST**
2000 年由 Roy Fielding 在博士论文中提出，他是 HTTP 规范 1.0 和 1.1 版的首席作者之一。
REST 中最重要的概念是*资源（resources）* ，使用全球 ID（通常使用 URI）标识。客户端应用程序使用 HTTP 方法（`GET/ POST/ PUT/ DELETE` ）操作资源或资源集。RESTful Web 服务是使用 HTTP 和 REST 原理实现的 Web 服务。通常，RESTful            Web 服务应该定义以下方面：
- Web 服务的基/根 URI，比如 http://host/<appcontext>/resources。
- 支持 MIME 类型的响应数据，包括 JSON/XML/ATOM 等等。
- 服务支持的操作集合（例如 `POST、GET、PUT`  或               `DELETE)`
如下表所示：
|**方法/资源 **|资源集合，                  URI 如：http://host/<appctx>/resources|成员资源，URI                  如：http://host/<appctx>/resources/1234|
|----|----|----|
|**GET**|列出资源集合的所有成员|检索标识为 1234 的资源的表示形式。|
|PUT|使用一个集合更新（替换）另一个集合。|更新标记为 1234 的数字资源。|
|POST|在集合中创建数字资源|在下面创建一个子资源。|
|DELETE|删除整个资源集合。|删除标记为 1234 的数字资源。|
**JSR－311  Java API for RESTful Web Services (JAX-RS) 1.0 and 1.1**
      JAX-RS是将在JavaEE 6引起的一种新技术。 JAX-RS即Java API for RESTful Web Services，是一个Java  编程语言的[应用程序接口](http://baike.baidu.com/view/592964.htm) ，支持按照表述性状态转移（REST）架构风格创建Web服务。JAX-RS使用了Java  SE5引入的Java标注来简化Web服务的客户端和[服务端](http://baike.baidu.com/view/1087294.htm)
 的开发和部署。包括：
- @Path，标注资源类或者方法的相对路径  　　
- @GET，@PUT，@POST，@DELETE，标注方法是HTTP请求的类型。  　　
- @Produces，标注返回的MIME媒体类型  　　
- @Consumes，标注可接受请求的MIME媒体类型
- @PathParam，@QueryParam，@HeaderParam，@CookieParam，@MatrixParam，@FormParam,分别标注方法的参数来自于HTTP请求的不同位置，例如@PathParam来自于URL的路径，@QueryParam来自于URL的查询参数，@HeaderParam来自于HTTP请求的头信息，@CookieParam来自于HTTP请求的Cookie。
**Jersey 实现**
Jersey 是 JAX-RS 的参考实现，它包含三个主要部分。
-                核心服务器（Core Server）：通过提供 JSR 311 中标准化的注释和 API 标准化，您可以用直观的方式开发 RESTful Web 服务。
-                核心客户端（Core Client）：Jersey 客户端 API 帮助您与 REST 服务轻松通信。
-                集成（Integration）：Jersey 还提供可以轻松集成 Spring、Guice、Apache Abdera 的库
实例应用：
一、环境
1、Eclipse Juno R2
2. Tomcat 7
3. Jersey 2.7  下载地址（ https://jersey.java.net/download.html）
二、流程
1.Eclipse 中创建一个 Dynamic Web Project ,本例为“RestDemo”
2.按个各人习惯建好包，本例为“com.waylau.rest.resources”
![](http://b.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=7807f0deb119ebc4c478769cb21dbec1/0b55b319ebc4b745429f78adcdfc1e178a821505.jpg?referer=0b4df88379f40ad14cf3f2d3531c&x=.jpg)
3.解压jaxrs-ri-2.7，
将api、ext、lib文件夹下的jar包都放到项目的lib下；
![](http://d.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=41a40894bc096b6385195e553c08f679/f31fbe096b63f6242082f6b88544ebf81b4ca3aa.jpg?referer=dc3f5bcfaf3459829c9dd1a20d68&x=.jpg)
项目引入jar包
![](http://d.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=5011985bf503918fd3d13dcf610657aa/63d0f703918fa0ec3bccabb6249759ee3d6ddb06.jpg?referer=1b02ce10013b5bb5e7c015ce251c&x=.jpg)
也可以使用maven直接使用jersey的骨架进行：首先jersey提供的骨架加入到eclipse中
Chances are you are using Apache Maven as a build & dependency management tool for your project. If you do, there is a very easy and convenient way to start playing with Jersey 2.22.2 by generating the skeleton application from one of the Jersey 2 maven
 archetypes that we provide. For instance, to create a Jersey 2.22.2 application using the Grizzly 2 HTTP server container, use
`﻿﻿﻿﻿mvn archetype:generate -DarchetypeGroupId=org.glassfish.jersey.archetypes -DarchetypeArtifactId=jersey-quickstart-grizzly2 -DarchetypeVersion=2.22.2`
If you want to create a Servlet container deployable Jersey 2.22.2 web application instead, use
`mvn archetype:generate -DarchetypeGroupId=org.glassfish.jersey.archetypes -DarchetypeArtifactId=jersey-quickstart-webapp -DarchetypeVersion=2.22.2`
4.在resources包下建一个class“HelloResource”
**[java]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21391033#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21391033#)[print](http://blog.csdn.net/kkkloveyou/article/details/21391033#)[?](http://blog.csdn.net/kkkloveyou/article/details/21391033#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/240400/fork)
- package com.waylau.rest.resources;  
- 
- 
- import javax.ws.rs.GET;  
- import javax.ws.rs.Path;  
- import javax.ws.rs.Produces;  
- import javax.ws.rs.PathParam;  
- import javax.ws.rs.core.MediaType;  
- 
- 
- @Path("/hello")  
- publicclass HelloResource {  
- @GET
- @Produces(MediaType.TEXT_PLAIN)  
- public String sayHello() {  
- return"Hello World!" ;  
-     }  
- 
- 
- @GET
- @Path("/{param}")    
- @Produces("text/plain;charset=UTF-8")  
- public String sayHelloToUTF8(@PathParam("param") String username) {  
- return"Hello " + username;  
-     }  
- 
- }  
```java
package com.waylau.rest.resources;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.PathParam;
import javax.ws.rs.core.MediaType;
@Path("/hello")
public class HelloResource {
	@GET
	@Produces(MediaType.TEXT_PLAIN)
	public String sayHello() {
		return "Hello World!" ;
	}
 
	
	@GET
	@Path("/{param}")  
	@Produces("text/plain;charset=UTF-8")
	public String sayHelloToUTF8(@PathParam("param") String username) {
		return "Hello " + username;
	}
	
}
```
5.修改web.xml,添加基于Servlet-的部署
**[html]**[view plain](http://blog.csdn.net/kkkloveyou/article/details/21391033#)[copy](http://blog.csdn.net/kkkloveyou/article/details/21391033#)[print](http://blog.csdn.net/kkkloveyou/article/details/21391033#)[?](http://blog.csdn.net/kkkloveyou/article/details/21391033#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/240400/fork)
- <servlet>
- <servlet-name>Way REST Service</servlet-name>
- <servlet-class>org.glassfish.jersey.servlet.ServletContainer</servlet-class>
- <init-param>
- <param-name>jersey.config.server.provider.packages</param-name>
- <param-value>com.waylau.rest.resources</param-value>
- </init-param>
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
		<param-name>jersey.config.server.provider.packages</param-name>
      	<param-value>com.waylau.rest.resources</param-value>
   	   </init-param>
    <load-on-startup>1</load-on-startup>
  </servlet>
  
  <servlet-mapping>
    <servlet-name>Way REST Service</servlet-name>
    <url-pattern>/rest/*</url-pattern>
  </servlet-mapping>
```
6.项目部署到tomcat,运行
7.浏览器输入要访问的uri地址
[http://localhost:8089/RestDemo/rest/hello](http://localhost:8089/RestDemo/rest/hello)
输出Hello World!
![](http://c.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=32792b8bb8a1cd1101b672258929b9c1/d000baa1cd11728b8db54399cafcc3cec2fd2cf5.jpg?referer=02302bc8af4bd1135dda83028cab&x=.jpg)
[http://localhost:8089/RestDemo/rest/hello/Way你好吗](http://localhost:8089/RestDemo/rest/hello/Way%E4%BD%A0%E5%A5%BD%E5%90%97)
输出Hello Way你好吗
![](http://h.hiphotos.bdimg.com/album/s%3D550%3Bq%3D90%3Bc%3Dxiangce%2C100%2C100/sign=9608ef4534d3d539c53d0fc60abc986d/14ce36d3d539b6006511c00ceb50352ac75cb7aa.jpg?referer=240eae6c34d12f2e97129a50f168&x=.jpg)

