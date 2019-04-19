# Jetty实战(1)之嵌入式运行Jetty - 零度的博客专栏 - CSDN博客
2016年11月16日 10:10:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：931
                
Jetty最常用的一种用法是把Jetty嵌入到自己的Java应用程序中，此时Jetty作为一个后台的Servlet容器运行，接受用户的http请求，下面是一个最简单的嵌入Jetty的用法。
1. 首先使用Maven创建一个java工程
[python][view
 plain](http://blog.csdn.net/kongxx/article/details/7218776#)[copy](http://blog.csdn.net/kongxx/article/details/7218776#)
- mvn archetype:generate -DgroupId=com.google.code.garbagecan.jettystudy -DartifactId=jettystudy -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
2. 修改pom文件，添加或修改编译和依赖部分
[html][view
 plain](http://blog.csdn.net/kongxx/article/details/7218776#)[copy](http://blog.csdn.net/kongxx/article/details/7218776#)
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
- <modelVersion>4.0.0</modelVersion>
- <groupId>com.google.code.garbagecan.jettystudy</groupId>
- <artifactId>jettystudy</artifactId>
- <packaging>jar</packaging>
- <version>1.0-SNAPSHOT</version>
- <name>jettystudy</name>
- <url>http://maven.apache.org</url>
- <build>
- <plugins>
- <plugin>
- <artifactId>maven-compiler-plugin</artifactId>
- <inherited>true</inherited>
- <configuration>
- <source>1.6</source>
- <target>1.6</target>
- <debug>true</debug>
- </configuration>
- </plugin>
- </plugins>
- </build>
- <dependencies>
- <dependency>
- <groupId>org.eclipse.jetty.aggregate</groupId>
- <artifactId>jetty-all</artifactId>
- <version>8.0.4.v20111024</version>
- <type>jar</type>
- <scope>provided</scope>
- </dependency>
- <dependency>
- <groupId>junit</groupId>
- <artifactId>junit</artifactId>
- <version>3.8.1</version>
- <scope>test</scope>
- </dependency>
- </dependencies>
- </project>
![](http://static.blog.csdn.net/images/save_snippets.png)
3. 创建一个Server类，用了启动Jetty server，并且通过一个HelloHandler来处理浏览器发送过来的请求；
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7218776#)[copy](http://blog.csdn.net/kongxx/article/details/7218776#)
- package com.google.code.garbagecan.jettystudy.sample1;  
- 
- import org.eclipse.jetty.server.Server;  
- 
- publicclass MyServer {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server(8080);  
-         server.setHandler(new HelloHandler());  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
4. 创建一个Handler类，用了处理所有客户端的请求
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7218776#)[copy](http://blog.csdn.net/kongxx/article/details/7218776#)
- package com.google.code.garbagecan.jettystudy.sample1;  
- 
- import java.io.IOException;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.eclipse.jetty.server.Request;  
- import org.eclipse.jetty.server.handler.AbstractHandler;  
- 
- publicclass HelloHandler extends AbstractHandler {  
- publicvoid handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)  
- throws IOException, ServletException {  
-         response.setContentType("text/html;charset=utf-8");  
-         response.setStatus(HttpServletResponse.SC_OK);  
-         baseRequest.setHandled(true);  
-         response.getWriter().println("<h1>Hello World</h1>");  
-         response.getWriter().println("Request url: " + target);  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
5. 运行MyServer类，然后通过浏览器访问http://localhost:8080/，可以看到“Hello World！”和请求的url。
