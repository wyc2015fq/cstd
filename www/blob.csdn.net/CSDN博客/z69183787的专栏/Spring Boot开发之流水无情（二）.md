# Spring Boot开发之流水无情（二） - z69183787的专栏 - CSDN博客
2015年06月16日 17:06:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8490
[上篇](http://qindongliang.iteye.com/blog/2205633)散仙写了一个很简单的入门级的Spring
 Boot的例子，没啥技术含量，不过，其实学任何东西只要找到第一个突破口，接下来的事情就好办了，人最怕什么？ 我想莫过于干一件事情，没有下手的地方了，而当你一旦找到了这感觉，就可以很喜悦的顺藤摸瓜般的探索你强烈想探索求知的某种事物了，这种冥冥之中玄而又玄的感觉是什么？回想一下： 
（1） 当你把第一个某种编程语言的Hello World的例子，成功的运行在一个IDE中 
（2） 当你第一次从老家出发到达了某个你从未涉足过的地方 
（3） 当你成功的完成了第一次网购 
一切的不相关的事物之间的，其实都有类似的学习历程，正是所谓的触类旁通，万法归宗！ 
关于Spring Boot这块，其实里面涉及的内容非常多也非常灵活，散仙的项目中用到的模块也只是冰山一角，不可能完全覆盖所有有关的模块，完全是为了项目而用技术，不是为了技术而诞生了这个项目，所以这次记录的心得，完全是项目有关的，而不是Spring Boot一个学习教程，这一点，希望各位看官明白，如果想专攻学习Spring Boot，推荐点击官网更详细的[教程](http://docs.spring.io/spring-boot/docs/current/reference/htmlsingle/#boot-documentation)
一个项目是由多种不同层面的技术组成的，所以散仙不会只写有关Spring Boot方面的，虽然Spring Boot是主线。大多数情况下，都会集成多种不同的技术，只有各自发挥自己擅长的优势，才能组合出最优的架构。 
好了，多说了点废话，下面看下本篇记录Spring Boot的几个知识点： 
（一）一个Maven+Spring Boot项目基本的包结构形式 
（二）一个简单的在Spring Boot项目集成安全控制 
（二）如何在Spring Boot中记录log日志 
（四）Spring Boot中几个常用注解介绍 
ok下面开始正题： 
（一）先看下，官网给出的一个简单的包结构组成： 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- com  
-  +- example  
-      +- myproject  
-          +- Application.java  
-          |  
-          +- domain  
-          |   +- Customer.java  
-          |   +- CustomerRepository.java  
-          |  
-          +- service  
-          |   +- CustomerService.java  
-          |  
-          +- web  
-              +- CustomerController.java  
然后，看下散仙实际工作中的项目结构： 
![](http://dl2.iteye.com/upload/attachment/0108/0167/baf7488e-a8d1-3760-b7c0-dd0ccb4669fb.png)
需要注意的地方如下： 
1，在src/main/java包下的第一层结构中，是必须放一个含有main方法的主启动的类，而且只能有一个main方法，如果再出现其他的main方法，在使用maven编译打包时，会报编译错误，当然在src/test/java包下，可以出现多个，但建议最好使用Junit进行单元测试. 
这个main方法启动，就会启动内嵌的tomcat或jetty容器，然后加载所有需要加载的或扫描的类或资源文件。上篇博客中，散仙为了测试方便，是直接在当前的Conroller中，启动了一个测试服务，这样做适合单独调试，如果是生产环境下的启动方法，则是散仙前面所讲的，在java包的根目录下建立一个main方法类，负责启动所有的资源。 
在本例中，散仙的main代码如下： 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- package com.dhgate.rate;  
- 
- import org.springframework.boot.SpringApplication;  
- import org.springframework.boot.autoconfigure.EnableAutoConfiguration;  
- import org.springframework.context.annotation.ComponentScan;  
- import org.springframework.context.annotation.Configuration;  
- 
- @Configuration//配置控制
- @EnableAutoConfiguration//启用自动配置
- @ComponentScan//组件扫描
- publicclass ApplicationMain {  
- publicstaticvoid main(String[] args) throws Exception {  
- //启动Spring Boot项目的唯一入口
-         SpringApplication.run(ApplicationMain.class, args);  
- 
-     }  
- 
- }  
2，在src/main/resource目录下面，是放置一些配置文件，或模板支持的文件，如JSP，Velocity,Freemaker等，这里面比较常用或重要的一个文件，就是Spring Boot的集中式配置文件application.properties这个文件了，这里面给其他大部分的组件，提供了一个可集中管理和配置的中心，包括安全控制，redis，solr，mangodb的连接信息，以及数据库连接池，容器端口号，jmx，java mail，动态模板等。此外这个目录下默认是可以访问静态资源的，比如我们的css，js，或者第三方的一些引用文件等。 
（二）关于在Spring Boot的配置简单的安全访问控制，这一点非常容易，我们只需要做如下二步，即可。 
1，在pom文件中，引入spring-boot-starter-security的maven依赖。 
Xml代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- <dependency>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-security</artifactId>
- </dependency>
2，在application.properties中，配置访问的用户名和密码 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- #用户名 密码配置  
- security.user.name=admin  
- security.user.password=test  
再次，访问链接时，会出现以下，拦截页面，就代表配置成功： 
![](http://dl2.iteye.com/upload/attachment/0108/0427/d81686a4-f24e-3a1d-81f9-95821cd5f592.png)
当然这只是，一个初级的配置，更复杂的配置，可以分不用角色，在控制范围上，能够拦截到方法级别的权限控制。 
（三）日志的重要性，不言而喻，Spring Boot支持大部分的log配置，其中包括： 
（1）java util logging 
（2）log4j 
（3）log4j2 
（4）logbak 
默认的情况下spring boot会选择logback作为日志记录的载体，当然要想它正常的工作，需要依赖 Commons Logging， Java Util Logging， Log4J 或 SLF4J，相信大部分同学都是选择使用log4j.properties作为我们的日志配置和管理，但是散仙在Spring Boot中一直没有测试集成成功，所以就放弃使用log4j.properties作为日志载体，而是选择了Spring Boot推荐的logbak作为日志的配置文件，用过之后感觉也不错。 
使用步骤： 
1，将logbak.xml拷贝至resource目录下的根目录，然后在logbak.xml中，配置相关的log生成规则，log级别，以及日志路径，log的字符编码集，这个非常重要，因为刚开始用这个log记录程序运行的信息时，发现它不支持中文log，后来经查名，需要配置相关的log编码才可以正确记录对应的信息。一个通用的配置如下： 
Xml代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- <!-- Logback configuration. See http://logback.qos.ch/manual/index.html -->
- <configurationscan="true"scanPeriod="10 seconds">
- 
- <!-- Simple file output -->
- <appendername="FILE"class="ch.qos.logback.core.rolling.RollingFileAppender">
- <!-- encoder defaults to ch.qos.logback.classic.encoder.PatternLayoutEncoder -->
- <encoder>
- <pattern>
-             [ %-5level] [%date{yyyy-MM-dd HH:mm:ss}] %logger{96} [%line] - %msg%n  
- </pattern>
- <charset>UTF-8</charset><!-- 此处设置字符集 -->
- </encoder>
- 
- <rollingPolicyclass="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
- <!-- rollover daily 配置日志所生成的目录以及生成文件名的规则 -->
- <fileNamePattern>logs/mylog-%d{yyyy-MM-dd}.%i.log</fileNamePattern>
- <timeBasedFileNamingAndTriggeringPolicy
- class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
- <!-- or whenever the file size reaches 64 MB -->
- <maxFileSize>64 MB</maxFileSize>
- </timeBasedFileNamingAndTriggeringPolicy>
- </rollingPolicy>
- 
- 
- <filterclass="ch.qos.logback.classic.filter.ThresholdFilter">
- <level>DEBUG</level>
- </filter>
- <!-- Safely log to the same file from multiple JVMs. Degrades performance! -->
- <prudent>true</prudent>
- </appender>
- 
- 
- <!-- Console output -->
- <appendername="STDOUT"class="ch.qos.logback.core.ConsoleAppender">
- <!-- encoder defaults to ch.qos.logback.classic.encoder.PatternLayoutEncoder -->
- <encoder>
- <pattern>
-               [ %-5level] [%date{yyyy-MM-dd HH:mm:ss}] %logger{96} [%line] - %msg%n  
- </pattern>
- <charset>GBK</charset><!-- 此处设置字符集 -->
- </encoder>
- <!-- Only log level WARN and above -->
- <filterclass="ch.qos.logback.classic.filter.ThresholdFilter">
- <level>WARN</level>
- </filter>
- </appender>
- 
- 
-   <!-- Enable FILE and STDOUT appenders for all log messages.  
-        By default, only log at level INFO and above. -->
- <rootlevel="INFO">
- <appender-refref="FILE"/>
- <appender-refref="STDOUT"/>
- </root>
- 
- <!-- For loggers in the these namespaces, log at all levels. -->
- <loggername="pedestal"level="ALL"/>
- <loggername="hammock-cafe"level="ALL"/>
- <loggername="user"level="ALL"/>
- </configuration>
2，在application.properties中，指定log文件的加载路径，已经配置通用的log日志级别： 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- #指定log的配置文件，以及记录Spring Boot的log级别  
- logging.config=logback.xml  
- logging.level.org.springframework.web: INFO  
（四）介绍下Spring Boot中几个常用的注解，其中大部分都是来自Spring MVC的注解,这些注解使用的频率一般都在90%之上。 
（1）@RestController和@Controller指定一个类，作为控制器的注解 
（2）@RequestMapping方法级别的映射注解，这一个用过Spring MVC的小伙伴相信都很熟悉 
（3）@EnableAutoConfiguration和@SpringBootApplication是类级别的注解，根据maven依赖的jar来自动猜测完成正确的spring的对应配置，只要引入了spring-boot-starter-web的依赖，默认会自动配置Spring MVC和tomcat容器 
（4）@Configuration类级别的注解，一般这个注解，我们用来标识main方法所在的类,完成元数据bean的初始化。 
（5）@ComponentScan类级别的注解，自动扫描加载所有的Spring组件包括Bean注入，一般用在main方法所在的类上 
（6）@ImportResource类级别注解，当我们必须使用一个xml的配置时，使用@ImportResource和@Configuration来标识这个文件资源的类。 
（7）@Autowired注解，一般结合@ComponentScan注解，来自动注入一个Service或Dao级别的Bean 
（8）@Component类级别注解，用来标识一个组件，比如我自定了一个filter，则需要此注解标识之后，Spring Boot才会正确识别。 
ok，本篇的讲解，就到此结束，这些都是项目工程的基础知识，了解这些，有助于我们快速的上手一个Spring Boot应用。 
下篇博客的内容会写： 
（1）如何在Spring Boot项目中集成Spring Loaded框架，来完成模块热加载和代码动态编译，有了这个东西，我们开发的效率会更加高效，大部分情况下我们改了一个类或方法的代码之后，都不需要重启应用，因为Spring Loaded会定时重新编译并加载代码。 
（2）如何在Spring Boot项目中集成Velocity,Spring Boot支持多种模板引擎，包括 
1，FreeMarker 
2，Groovy 
3，Thymeleaf 
4，Velocity 
5，JSP （Spring Boot官方不推荐） 
我们可以根据熟悉程度来选择自己喜欢的引擎。 
