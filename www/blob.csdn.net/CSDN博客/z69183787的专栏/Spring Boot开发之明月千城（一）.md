# Spring Boot开发之明月千城（一） - z69183787的专栏 - CSDN博客
2015年06月16日 17:06:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10193
最近数据分析的项目也即将告一段落了，中间也积累了很多知识，特此记录一下。其中用的最爽的Web组合开发就是Intellij IDEA + Maven + Spring Boot + Velocity
 + Boostrap + jQuery了，关于后端的数据分析和处理的Hadoop模块，会放在Hadoop分类的博客文章中记录。 
![](http://dl2.iteye.com/upload/attachment/0107/8583/71228faa-8d8c-30e7-a939-711d03565e88.png)
Spring Boot提供了一个强大的一键式Spring的集成开发环境，能够单独进行一个Spring应用的开发，其中： 
（1）集中式配置（application.properties）+注解，大大简化了开发流程 
（2）内嵌的Tomcat和Jetty容器，可直接打成jar包启动，无需提供Java war包以及繁琐的Web配置 
（3）提供了Spring各个插件的基于Maven的pom模板配置，开箱即用，便利无比。
（4）可以在任何你想自动化配置的地方，实现可能 
（5）提供更多的企业级开发特性，如何系统监控，健康诊断，权限控制 
（6） 无冗余代码生成和XML强制配置 
（7）提供支持强大的Restfult风格的编码，非常简洁 
当然Spring Boot提供的功能，远远比上面的强大，散仙会在后续文章中，逐渐以实际工作中的项目为背景，穿插记录使用Spring Boot的心得体会。 
下面看一个入门级的例子： 
pom依赖： 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- <?xml version="1.0" encoding="UTF-8"?>  
- <project xmlns="http://maven.apache.org/POM/4.0.0"
-          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-          xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
-     <modelVersion>4.0.0</modelVersion>  
- 
-     <groupId>com.spring.boot</groupId>  
-     <artifactId>springboot</artifactId>  
-     <version>1.0-SNAPSHOT</version>  
- 
-     <parent>  
-         <groupId>org.springframework.boot</groupId>  
-         <artifactId>spring-boot-starter-parent</artifactId>  
-         <version>1.2.3.RELEASE</version>  
-     </parent>  
-     <dependencies>  
-         <dependency>  
-             <groupId>org.springframework.boot</groupId>  
-             <artifactId>spring-boot-starter-web</artifactId>  
-         </dependency>  
-     </dependencies>  
- </project>  
核心代码： 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- package controller;  
- 
- import org.springframework.boot.SpringApplication;  
- import org.springframework.boot.autoconfigure.EnableAutoConfiguration;  
- import org.springframework.stereotype.Controller;  
- import org.springframework.web.bind.annotation.RequestBody;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import org.springframework.web.bind.annotation.ResponseBody;  
- 
- /**
-  * Created by 三劫散仙 on 2015/4/24.
-  */
- @Controller
- @EnableAutoConfiguration
- publicclass HellowController {  
- 
- 
- 
- @RequestMapping("/hellow")  
- @ResponseBody
- public String hellow(){  
- 
- return"哈喽，Spring Boot ！";  
-     }  
- 
- 
- publicstaticvoid main(String[] args) {  
- //第一个简单的应用，
-         SpringApplication.run(HellowController.class,args);  
- 
-     }  
- 
- }  
控制台输出： 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- D:\soft\Java\jdk1.7.0_04\bin\java -Didea.launcher.port=7532"-Didea.launcher.bin.path=D:\idea\ideainstall\IntelliJ IDEA 14.0.2\bin" -Dfile.encoding=UTF-8 -classpath "D:\soft\Java\jdk1.7.0_04\jre\lib\charsets.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\deploy.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\javaws.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\jce.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\jfr.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\jsse.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\management-agent.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\plugin.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\resources.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\rt.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\ext\dnsns.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\ext\localedata.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\ext\sunec.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\ext\sunjce_provider.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\ext\sunmscapi.jar;D:\soft\Java\jdk1.7.0_04\jre\lib\ext\zipfs.jar;D:\idea\ideaworkspace\Springboot\target\classes;C:\Users\qin\.m2\repository\org\springframework\boot\spring-boot-starter-web\1.2.3.RELEASE\spring-boot-starter-web-1.2.3.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\boot\spring-boot-starter\1.2.3.RELEASE\spring-boot-starter-1.2.3.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\boot\spring-boot\1.2.3.RELEASE\spring-boot-1.2.3.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\boot\spring-boot-autoconfigure\1.2.3.RELEASE\spring-boot-autoconfigure-1.2.3.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\boot\spring-boot-starter-logging\1.2.3.RELEASE\spring-boot-starter-logging-1.2.3.RELEASE.jar;C:\Users\qin\.m2\repository\org\slf4j\jcl-over-slf4j\1.7.11\jcl-over-slf4j-1.7.11.jar;C:\Users\qin\.m2\repository\org\slf4j\slf4j-api\1.7.11\slf4j-api-1.7.11.jar;C:\Users\qin\.m2\repository\org\slf4j\jul-to-slf4j\1.7.11\jul-to-slf4j-1.7.11.jar;C:\Users\qin\.m2\repository\org\slf4j\log4j-over-slf4j\1.7.11\log4j-over-slf4j-1.7.11.jar;C:\Users\qin\.m2\repository\ch\qos\logback\logback-classic\1.1.3\logback-classic-1.1.3.jar;C:\Users\qin\.m2\repository\ch\qos\logback\logback-core\1.1.3\logback-core-1.1.3.jar;C:\Users\qin\.m2\repository\org\yaml\snakeyaml\1.14\snakeyaml-1.14.jar;C:\Users\qin\.m2\repository\org\springframework\boot\spring-boot-starter-tomcat\1.2.3.RELEASE\spring-boot-starter-tomcat-1.2.3.RELEASE.jar;C:\Users\qin\.m2\repository\org\apache\tomcat\embed\tomcat-embed-core\8.0.20\tomcat-embed-core-8.0.20.jar;C:\Users\qin\.m2\repository\org\apache\tomcat\embed\tomcat-embed-el\8.0.20\tomcat-embed-el-8.0.20.jar;C:\Users\qin\.m2\repository\org\apache\tomcat\embed\tomcat-embed-logging-juli\8.0.20\tomcat-embed-logging-juli-8.0.20.jar;C:\Users\qin\.m2\repository\org\apache\tomcat\embed\tomcat-embed-websocket\8.0.20\tomcat-embed-websocket-8.0.20.jar;C:\Users\qin\.m2\repository\com\fasterxml\jackson\core\jackson-databind\2.4.5\jackson-databind-2.4.5.jar;C:\Users\qin\.m2\repository\com\fasterxml\jackson\core\jackson-annotations\2.4.5\jackson-annotations-2.4.5.jar;C:\Users\qin\.m2\repository\com\fasterxml\jackson\core\jackson-core\2.4.5\jackson-core-2.4.5.jar;C:\Users\qin\.m2\repository\org\hibernate\hibernate-validator\5.1.3.Final\hibernate-validator-5.1.3.Final.jar;C:\Users\qin\.m2\repository\javax\validation\validation-api\1.1.0.Final\validation-api-1.1.0.Final.jar;C:\Users\qin\.m2\repository\org\jboss\logging\jboss-logging\3.1.3.GA\jboss-logging-3.1.3.GA.jar;C:\Users\qin\.m2\repository\com\fasterxml\classmate\1.0.0\classmate-1.0.0.jar;C:\Users\qin\.m2\repository\org\springframework\spring-core\4.1.6.RELEASE\spring-core-4.1.6.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\spring-web\4.1.6.RELEASE\spring-web-4.1.6.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\spring-aop\4.1.6.RELEASE\spring-aop-4.1.6.RELEASE.jar;C:\Users\qin\.m2\repository\aopalliance\aopalliance\1.0\aopalliance-1.0.jar;C:\Users\qin\.m2\repository\org\springframework\spring-beans\4.1.6.RELEASE\spring-beans-4.1.6.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\spring-context\4.1.6.RELEASE\spring-context-4.1.6.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\spring-webmvc\4.1.6.RELEASE\spring-webmvc-4.1.6.RELEASE.jar;C:\Users\qin\.m2\repository\org\springframework\spring-expression\4.1.6.RELEASE\spring-expression-4.1.6.RELEASE.jar;D:\idea\ideainstall\IntelliJ IDEA 14.0.2\lib\idea_rt.jar" com.intellij.rt.execution.application.AppMain controller.HellowController  
- 
-   .   ____          _            __ _ _  
-  /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \  
- ( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \  
-  \\/  ___)| |_)| | | | | || (_| |  ) ) ) )  
-   '  |____| .__|_| |_|_| |_\__, | / / / /  
-  =========|_|==============|___/=/_/_/_/  
-  :: Spring Boot ::        (v1.2.3.RELEASE)  
- 
- 2015-04-2401:12:41.399  INFO 4428 --- [           main] controller.HellowController              : Starting HellowController on qin-PC with PID 4428 (D:\idea\ideaworkspace\Springboot\target\classes started by qin in D:\idea\ideaworkspace\Springboot)  
- 2015-04-2401:12:41.458  INFO 4428 --- [           main] ationConfigEmbeddedWebApplicationContext : Refreshing org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@2923a47c: startup date [Fri Apr 2401:12:41 CST 2015]; root of context hierarchy  
- 2015-04-2401:12:42.393  INFO 4428 --- [           main] o.s.b.f.s.DefaultListableBeanFactory     : Overriding bean definition for bean 'beanNameViewResolver': replacing [Root bean: class [null]; scope=; abstract=false; lazyInit=false; autowireMode=3; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=org.springframework.boot.autoconfigure.web.ErrorMvcAutoConfiguration$WhitelabelErrorViewConfiguration; factoryMethodName=beanNameViewResolver; initMethodName=null; destroyMethodName=(inferred); defined in class path resource [org/springframework/boot/autoconfigure/web/ErrorMvcAutoConfiguration$WhitelabelErrorViewConfiguration.class]] with [Root bean: class [null]; scope=; abstract=false; lazyInit=false; autowireMode=3; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=org.springframework.boot.autoconfigure.web.WebMvcAutoConfiguration$WebMvcAutoConfigurationAdapter; factoryMethodName=beanNameViewResolver; initMethodName=null; destroyMethodName=(inferred); defined in class path resource [org/springframework/boot/autoconfigure/web/WebMvcAutoConfiguration$WebMvcAutoConfigurationAdapter.class]]  
- 2015-04-2401:12:44.399  INFO 4428 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat initialized with port(s): 8080 (http)  
- 2015-04-2401:12:44.959  INFO 4428 --- [           main] o.apache.catalina.core.StandardService   : Starting service Tomcat  
- 2015-04-2401:12:44.973  INFO 4428 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/8.0.20
- 2015-04-2401:12:45.332  INFO 4428 --- [ost-startStop-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext  
- 2015-04-2401:12:45.332  INFO 4428 --- [ost-startStop-1] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 3880 ms  
- 2015-04-2401:12:46.274  INFO 4428 --- [ost-startStop-1] o.s.b.c.e.ServletRegistrationBean        : Mapping servlet: 'dispatcherServlet' to [/]  
- 2015-04-2401:12:46.295  INFO 4428 --- [ost-startStop-1] o.s.b.c.embedded.FilterRegistrationBean  : Mapping filter: 'characterEncodingFilter' to: [/*]
- 2015-04-24 01:12:46.296  INFO 4428 --- [ost-startStop-1] o.s.b.c.embedded.FilterRegistrationBean  : Mapping filter: 'hiddenHttpMethodFilter' to: [/*]
- 2015-04-24 01:12:46.481  INFO 4428 --- [           main] s.w.s.m.m.a.RequestMappingHandlerAdapter : Looking for @ControllerAdvice: org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@2923a47c: startup date [Fri Apr 24 01:12:41 CST 2015]; root of context hierarchy
- 2015-04-24 01:12:46.542  INFO 4428 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/hellow],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto public java.lang.String controller.HellowController.hellow()
- 2015-04-24 01:12:46.544  INFO 4428 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto public org.springframework.http.ResponseEntity<java.util.Map<java.lang.String, java.lang.Object>> org.springframework.boot.autoconfigure.web.BasicErrorController.error(javax.servlet.http.HttpServletRequest)
- 2015-04-24 01:12:46.545  INFO 4428 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],methods=[],params=[],headers=[],consumes=[],produces=[text/html],custom=[]}" onto public org.springframework.web.servlet.ModelAndView org.springframework.boot.autoconfigure.web.BasicErrorController.errorHtml(javax.servlet.http.HttpServletRequest)
- 2015-04-24 01:12:46.595  INFO 4428 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
- 2015-04-24 01:12:46.596  INFO 4428 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/webjars/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
- 2015-04-24 01:12:46.639  INFO 4428 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**/favicon.ico] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]  
- 2015-04-2401:12:46.708  INFO 4428 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Registering beans for JMX exposure on startup  
- 2015-04-2401:12:46.829  INFO 4428 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8080 (http)  
- 2015-04-2401:12:46.830  INFO 4428 --- [           main] controller.HellowController              : Started HellowController in 5.978 seconds (JVM running for6.739)  
- 2015-04-2401:13:29.470  INFO 4428 --- [nio-8080-exec-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring FrameworkServlet 'dispatcherServlet'
- 2015-04-2401:13:29.470  INFO 4428 --- [nio-8080-exec-1] o.s.web.servlet.DispatcherServlet        : FrameworkServlet 'dispatcherServlet': initialization started  
- 2015-04-2401:13:29.486  INFO 4428 --- [nio-8080-exec-1] o.s.web.servlet.DispatcherServlet        : FrameworkServlet 'dispatcherServlet': initialization completed in 16 ms  
默认的内嵌容器，为tomcat，当然这个我们可以随便指定，包括端口号，http访问： 
![](http://dl2.iteye.com/upload/attachment/0107/8550/1f2788b6-1e94-3388-8136-d21be1e19260.png)
ok，至此，我们一个简单的http应用就开发完毕了，给人的感觉就是简直比用Python的Django和PHP还轻，而且还是我们十分熟悉的JAVA开发，所以与很多其他的JAVA开源项目相结合非常容易，比如Apache Lucene，Solr，Hadoop，Spark，ElasticSearch等，支持很多好处，不言而喻！ 
