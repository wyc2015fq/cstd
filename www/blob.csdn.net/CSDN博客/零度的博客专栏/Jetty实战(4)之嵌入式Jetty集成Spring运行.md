# Jetty实战(4)之嵌入式Jetty集成Spring运行 - 零度的博客专栏 - CSDN博客
2016年11月16日 13:36:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：1556
                
1. 首先修改pom.xml文件，添加spring的依赖项
[html][view
 plain](http://blog.csdn.net/kongxx/article/details/7227107#)[copy](http://blog.csdn.net/kongxx/article/details/7227107#)
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
- 
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring</artifactId>
- <version>2.5.6</version>
- <type>jar</type>
- <scope>provided</scope>
- </dependency>
- 
- <dependency>
- <groupId>junit</groupId>
- <artifactId>junit</artifactId>
- <version>3.8.1</version>
- <scope>test</scope>
- </dependency>
- </dependencies>
- </project>
![](http://static.blog.csdn.net/images/save_snippets.png)
2. 创建一个Server类，用来通过spring来启动Jetty server
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7227107#)[copy](http://blog.csdn.net/kongxx/article/details/7227107#)
- package com.google.code.garbagecan.jettystudy.sample4;  
- 
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- 
- publicclass MyServer {  
- publicstaticvoid main(String[] args) throws Exception {  
- new ClassPathXmlApplicationContext("/com/google/code/garbagecan/jettystudy/sample4/spring.xml");  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
3. 创建一个Handler类，用了处理http请求
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7227107#)[copy](http://blog.csdn.net/kongxx/article/details/7227107#)
- package com.google.code.garbagecan.jettystudy.sample4;  
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
- publicclass MyHandler extends AbstractHandler {  
- publicvoid handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)  
- throws IOException, ServletException {  
-         response.setContentType("text/html;charset=utf-8");  
-         response.setStatus(HttpServletResponse.SC_OK);  
-         baseRequest.setHandled(true);  
-         response.getWriter().println("<h1>Hello World</h1>");  
-         response.getWriter().println("<li>Request url: " + target + "</li>");  
-         response.getWriter().println("<li>Server port: " + request.getServerPort() + "</li>");  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
4. 创建一个spring配置文件，并放在com/google/code/garbagecan/jettystudy/sample4/spring.xml位置，内容如下.
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7227107#)[copy](http://blog.csdn.net/kongxx/article/details/7227107#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans xmlns="http://www.springframework.org/schema/beans"
-     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-     xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">  
- 
-     <bean id="Server"class="org.eclipse.jetty.server.Server" init-method="start" destroy-method="stop">  
-         <property name="connectors">  
-             <list>  
-                 <bean id="Connector"class="org.eclipse.jetty.server.nio.SelectChannelConnector">  
-                     <property name="port" value="8080" />  
-                 </bean>  
-             </list>  
-         </property>  
- 
-         <property name="handler">  
-             <bean id="handlers"class="org.eclipse.jetty.server.handler.HandlerList">  
-                 <property name="handlers">  
-                     <list>  
-                         <bean class="com.google.code.garbagecan.jettystudy.sample4.MyHandler" />  
-                         <bean class="org.eclipse.jetty.server.handler.DefaultHandler" />  
-                     </list>  
-                 </property>  
-             </bean>  
-         </property>  
-     </bean>  
- </beans>  
![](http://static.blog.csdn.net/images/save_snippets.png)
其中定义了Jetty Server的配置，包括Connector和Handler等等。
5. 运行MyServer类，然后通过http://localhost:8080/来访问。
