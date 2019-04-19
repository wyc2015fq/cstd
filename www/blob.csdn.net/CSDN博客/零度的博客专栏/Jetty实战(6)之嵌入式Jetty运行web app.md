# Jetty实战(6)之嵌入式Jetty运行web app - 零度的博客专栏 - CSDN博客
2016年11月16日 13:43:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：452
                
要说嵌入式运行Jetty，最常用的还应该是运行一个标准的war文件或者指定一个webapp目录。
0. 首先需要添加Jetty运行时webapp的依赖包，下面是一个完整的pom.xml文件
[html][view
 plain](http://blog.csdn.net/kongxx/article/details/7237034#)[copy](http://blog.csdn.net/kongxx/article/details/7237034#)
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
- <version>2.3.1</version>
- <configuration>
- <source>1.6</source>
- <target>1.6</target>
- <debug>true</debug>
- </configuration>
- </plugin>
- </plugins>
- </build>
- <dependencies>
- <!-- Spring support -->
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring</artifactId>
- <version>2.5.6</version>
- </dependency>
- 
- <!-- Jetty -->
- <dependency>
- <groupId>org.eclipse.jetty.aggregate</groupId>
- <artifactId>jetty-all</artifactId>
- <version>8.0.4.v20111024</version>
- </dependency>
- 
- <!-- Jetty Webapp -->
- <dependency>
- <groupId>org.eclipse.jetty</groupId>
- <artifactId>jetty-webapp</artifactId>
- <version>8.0.4.v20111024</version>
- </dependency>
- 
- <!-- JSP Support -->
- <dependency>
- <groupId>org.glassfish.web</groupId>
- <artifactId>javax.servlet.jsp</artifactId>
- <version>2.2.3</version>
- </dependency>
- 
- <!-- EL Support -->
- <dependency>
- <groupId>org.glassfish.web</groupId>
- <artifactId>javax.el</artifactId>
- <version>2.2.3</version>
- </dependency>
- 
- <!-- JSTL Support -->
- <dependency>
- <groupId>org.glassfish.web</groupId>
- <artifactId>javax.servlet.jsp.jstl</artifactId>
- <version>1.2.1</version>
- <exclusions>
- <exclusion>
- <artifactId>jstl-api</artifactId>
- <groupId>javax.servlet.jsp.jstl</groupId>
- </exclusion>
- </exclusions>
- </dependency>
- </dependencies>
- </project>
![](http://static.blog.csdn.net/images/save_snippets.png)
1. 运行标准的war文件
1.1 首先找一个完整的war包，这里使用了struts2自带的一个例子应用程序struts2-blank.war；
1.2 创建自己的Jetty Server启动类WebAppContextWithWarServer，其中指定了war文件的路径，并指定context路径为"/myapp"
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7237034#)[copy](http://blog.csdn.net/kongxx/article/details/7237034#)
- package com.google.code.garbagecan.jettystudy.sample6;  
- 
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.webapp.WebAppContext;  
- 
- publicclass WebAppContextWithWarServer {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server(8080);  
- 
-         WebAppContext context = new WebAppContext();  
-         context.setContextPath("/myapp");  
-         context.setWar("E:/share/test/struts2-blank.war");  
-         server.setHandler(context);  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
1.3 运行WebAppContextWithWarServer类，然后访问// http://localhost:8080/myapp/就可以看到struts2的例子界面了。
2. 运行一个webapp目录
2.1 还是用上面的struts2-blank.war，将这个war包解压后放到一个目录下；
2.2 创建自己的Jetty Server启动类WebAppContextWithFolderServer，其中指定了webapp目录，并指定context路径为"/myapp"
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7237034#)[copy](http://blog.csdn.net/kongxx/article/details/7237034#)
- package com.google.code.garbagecan.jettystudy.sample6;  
- 
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.webapp.WebAppContext;  
- 
- publicclass WebAppContextWithFolderServer {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server(8080);  
- 
-         WebAppContext context = new WebAppContext();  
-         context.setContextPath("/myapp");  
-         context.setDescriptor("E:/share/test/struts2-blank/WEB-INF/web.xml");  
-         context.setResourceBase("E:/share/test/struts2-blank");  
-         context.setParentLoaderPriority(true);  
-         server.setHandler(context);  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
2.3 运行WebAppContextWithFolderServer类，然后访问// http://localhost:8080/myapp/就可以看到struts2的例子界面了。
