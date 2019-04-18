# 搭建一个RESTFUL风格的Web Service (Maven版本) - z69183787的专栏 - CSDN博客
2015年06月10日 22:57:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3015
[该教程翻译自Spring官方，并进行适当删减。]
**你将搭建的**
你将搭建一个可以接受Http Get 请求的web service， 
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- http://localhost:8080/greeting  
并将以JSON字符串的形式返回问候，
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- {"id":1,"content":"Hello, World!"}  
工具
一个文本编辑器，JDK1.6及以上，Maven 3.0+或者Gradle 1.11+。（本文将使用Maven)
下面是pom.xml文件的清单：
**[html]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- 
- <groupId>org.springframework</groupId>
- <artifactId>gs-rest-service</artifactId>
- <version>0.1.0</version>
- 
- <parent>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-parent</artifactId>
- <version>1.1.5.RELEASE</version>
- </parent>
- 
- <dependencies>
- <dependency>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-web</artifactId>
- </dependency>
- </dependencies>
- 
- <properties>
- <start-class>hello.Application</start-class>
- </properties>
- 
- <build>
- <plugins>
- <plugin>
- <artifactId>maven-compiler-plugin</artifactId>
- <version>2.3.2</version>
- </plugin>
- <plugin>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-maven-plugin</artifactId>
- </plugin>
- </plugins>
- </build>
- 
- <repositories>
- <repository>
- <id>spring-releases</id>
- <url>http://repo.spring.io/libs-release</url>
- </repository>
- </repositories>
- <pluginRepositories>
- <pluginRepository>
- <id>spring-releases</id>
- <url>http://repo.spring.io/libs-release</url>
- </pluginRepository>
- </pluginRepositories>
- </project>
**新建项目**
首先你新建一个符合Maven规范的目录结构， src/main/java/hello
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- └── src  
-     └── main  
-         └── java  
-             └── hello  
在hello目录下，新建一个Greeting类作为“问候”的javabean。代码清单如下：
**[java]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- package hello;  
- 
- publicclass Greeting {  
- 
- privatefinallong id;  
- privatefinal String content;  
- 
- public Greeting(long id, String content) {  
- this.id = id;  
- this.content = content;  
-     }  
- 
- publiclong getId() {  
- return id;  
-     }  
- 
- public String getContent() {  
- return content;  
-     }  
- }  
在下面的步骤你将会看到，spring将使用Jackson JSON自动将Greeting的对象转成JSON字符串。
接下来是新建一个类做控制器。
在Spring构建一个Restful风格的web service，需要一个处理请求的控制器。
同样在hello目录，代码清单如下：
**[java]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- package hello;  
- 
- import java.util.concurrent.atomic.AtomicLong;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import org.springframework.web.bind.annotation.RequestParam;  
- import org.springframework.web.bind.annotation.RestController;  
- 
- @RestController
- publicclass GreetingController {  
- 
- privatestaticfinal String template = "Hello, %s!";  
- privatefinal AtomicLong counter = new AtomicLong();  
- 
- @RequestMapping("/greeting")  
- public Greeting greeting(@RequestParam(value="name", required=false, defaultValue="World") String name) {  
- returnnew Greeting(counter.incrementAndGet(),  
-                             String.format(template, name));  
-     }  
- }  
写过Spring MVC的对@RequestMapping和@RequestParam等一定不陌生。
传统的MVC控制器和Restful 风格Web Service控制器的主要区别是，产生HTTP 响应的方式不同。不同于依赖视图技术将数据解析成HTML，这个控制器填充并返回一个对象。
这段代码使用Spring4.o新的注解：RestController,表明该类的每个方法返回对象而不是视图。它实际就是@Controller和@ResponseBody混合使用的简写方法。
Greeting对象会被转换成JSON字符串，这得益于Spring 的HTTP消息转换支持，你不必人工处理。由于Jackson2在classpath里，Spring的MappingJackson2HttpMessageConverter会自动完成这一工作。
尽管你可以将这个服务打包成传统的WAR文件部署到应用服务器，但下面将会创建一个独立的应用，使用main方法可以将所有东西打包到一个可执行的jar文件。并且，你将使用Sping对内嵌Tomcat servlet容器的支持，作为HTPP 运行时环境，没必要部署成一个tomcat外部实例。
**[java]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- package hello;  
- 
- import org.springframework.boot.autoconfigure.EnableAutoConfiguration;  
- import org.springframework.boot.SpringApplication;  
- import org.springframework.context.annotation.ComponentScan;  
- 
- @ComponentScan
- @EnableAutoConfiguration
- publicclass Application {  
- 
- publicstaticvoid main(String[] args) {  
-         SpringApplication.run(Application.class, args);  
-     }  
- }  
main方法使用了SpringApplication工具类。这将告诉Spring去读取Application的元信息，并在Spring的应用上下文作为一个组件被管理。
@ComponentScan注解告诉Spring在hello包下遍历带有@Component注解的类。这将保证Spring能找到并注册GreetingController，因为它被@RestController标记，这也是@Component的一种。
@EnableAutoConfiguration注解会基于你的类加载路径的内容切换合理的默认行为。比如，因为应用要依赖内嵌版本的tomcat（tomcat-embed-core.jar)，所有一个tomcat服务器会被启动并代替你进行合理的配置。再比如，因为应用要依赖Spring 的 MVC框架(spring-webmvc.jar),一个Spring MVC的DispatcherServlet将被配置并注册，不再需要web.xml文件。自动配置是很强大灵活的机制。
使用maven可以这样执行，
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- mvn clean package  
然后，
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)[copy](http://blog.csdn.net/chenloveit/article/details/38779149?utm_source=tuicool#)
- java -jar target/gs-rest-service-0.1.0.jar  
运行后无意外会出现Spring Boot结果:
![](https://img-blog.csdn.net/20140823190301893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxvdmVpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在浏览器测试：
![](https://img-blog.csdn.net/20140823190230906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxvdmVpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
带参数的测试：
![](https://img-blog.csdn.net/20140823190506771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxvdmVpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
