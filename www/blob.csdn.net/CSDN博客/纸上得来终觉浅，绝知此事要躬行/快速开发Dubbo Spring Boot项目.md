# 快速开发Dubbo Spring Boot项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年02月19日 17:03:06[boonya](https://me.csdn.net/boonya)阅读数：821








Project Address:[https://gitee.com/mirrors/Dubbo-Spring-Boot-Project](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project)


# Dubbo Spring Boot Project

[Dubbo](https://github.com/alibaba/dubbo) Spring Boot Project makes it easy to create [Spring Boot](https://github.com/spring-projects/spring-boot/) application using Dubbo as RPC Framework. What's more , it aslo provides
- [auto-configure features](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-autoconfigure) (e.g.annotation-driven, auto configuration, externalized configuration).
- [production-ready features](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-actuator) (e.g.security, health checks, externalized configuration).

> 
Dubbo *|ˈdʌbəʊ|* is a high-performance, java based [RPC](https://en.wikipedia.org/wiki/Remote_procedure_call) framework open-sourced by Alibaba. As in many RPC systems, dubbo is based around the idea of defining a service, specifying the methods that can be called remotely with their parameters and return types. On the server side, the server implements this interface and runs a dubbo server to handle client calls. On the client side, the client has a stub that provides the same methods as the server.


## Integrate with Maven

You can introduce the latest `dubbo-spring-boot-starter` to your project by adding the following dependency to your pom.xml


<dependency><groupId>com.alibaba.boot</groupId><artifactId>dubbo-spring-boot-starter</artifactId><version>1.0.0-SNAPSHOT</version></dependency>


If your project failed to resolve the dependency, try to add the following repository:


<repositories><repository><id>sonatype-nexus-snapshots</id><url>https://oss.sonatype.org/content/repositories/snapshots</url><releases><enabled>false</enabled></releases><snapshots><enabled>true</enabled></snapshots></repository></repositories>


## Getting Started

If you don't know about Dubbo , please take a few minutes to learn [http://dubbo.io/](http://dubbo.io/) . After that  you could dive deep into dubbo [user guide](http://dubbo.io/books/dubbo-user-book-en/).

Usually , There are two usage scenarios for Dubbo applications , one is Dubbo service(s) provider , another is Dubbo service(s) consumer, thus let's get a quick start on them.

First of all , we suppose an interface as Dubbo RPC API that  a service provider exports and a service client consumes :


publicinterfaceDemoService{StringsayHello(Stringname);}


### Dubbo service(s) provider

Service Provider implements `DemoService` :


@Service(version="1.0.0",application="${dubbo.application.id}",protocol="${dubbo.protocol.id}",registry="${dubbo.registry.id}")publicclassDefaultDemoServiceimplementsDemoService{publicStringsayHello(Stringname){return"Hello, "+name+" (from Spring Boot)";}}


then , provides a bootstrap class :


@SpringBootApplicationpublicclassDubboProviderDemo{publicstaticvoidmain(String[]args){SpringApplication.run(DubboProviderDemo.class,args);}}


last , configures `application.properties` :


# Spring boot application
spring.application.name=dubbo-provider-demoserver.port=9090management.port=9091# Base packages to scan Dubbo Components (e.g @Service , @Reference)
dubbo.scan.basePackages=com.alibaba.boot.dubbo.demo.provider.service# Dubbo Config properties
## ApplicationConfig Bean
dubbo.application.id=dubbo-provider-demodubbo.application.name=dubbo-provider-demo## ProtocolConfig Bean
dubbo.protocol.id=dubbodubbo.protocol.name=dubbodubbo.protocol.port=12345## RegistryConfig Bean
dubbo.registry.id=my-registrydubbo.registry.address=N/A


`DefaultDemoService`'s placeholders( `${dubbo.application.id}`, `${dubbo.protocol.id}`, `${dubbo.registry.id}` ) sources from `application.properties`.

More details , please refer to [Dubbo Provider Sample](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-samples/dubbo-spring-boot-sample-provider).

### Dubbo service(s) consumer

Service consumer requires Spring Beans to reference `DemoService` :


@RestControllerpublicclassDemoConsumerController{@Reference(version="1.0.0",application="${dubbo.application.id}",url="dubbo://localhost:12345")privateDemoServicedemoService;@RequestMapping("/sayHello")publicStringsayHello(@RequestParamStringname){returndemoService.sayHello(name);}}


then , also provide a bootstrap class :


@SpringBootApplication(scanBasePackages="com.alibaba.boot.dubbo.demo.consumer.controller")publicclassDubboConsumerDemo{publicstaticvoidmain(String[]args){SpringApplication.run(DubboConsumerDemo.class,args);}}


last , configures `application.properties` :


# Spring boot application
spring.application.name=dubbo-consumer-demoserver.port=8080management.port=8081# Dubbo Config properties
## ApplicationConfig Bean
dubbo.application.id=dubbo-consumer-demodubbo.application.name=dubbo-consumer-demo## ProtocolConfig Bean
dubbo.protocol.id=dubbodubbo.protocol.name=dubbodubbo.protocol.port=12345


If `DubboProviderDemo` works well , please mark sure Dubbo service(s) is active.

More details , please refer to [Dubbo Consumer Sample](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-samples/dubbo-spring-boot-sample-consumer)

## Getting help

Having trouble with Dubbo Spring Boot? We’d like to help!
- If you are upgrading, read the [release notes](https://github.com/dubbo/dubbo-spring-boot-project/releases) for upgrade instructions and "new and noteworthy" features.
- Ask a question - You can join [ours google groups](https://groups.google.com/group/dubbo) , or subscribe [Dubbo User Mailling List](mailto:dubbo+subscribe@googlegroups.com).
- Report bugs at [github.com/dubbo/dubbo-spring-boot-project/issues](https://github.com/dubbo/dubbo-spring-boot-project/issues).

## Building from Source

If you want to try out thr latest features of Dubbo Spring Boot , it can be easily built with the [maven wrapper](https://github.com/takari/maven-wrapper). Your JDK is 1.7 or above.


$ ./mvnw clean install



## Modules

There are some modules in Dubbo Spring Boot Project  , let's take a look at below overview :

### [dubbo-spring-boot-parent](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-parent)

The main usage of `dubbo-spring-boot-parent` is providing dependencies management for other modules.

### [dubbo-spring-boot-autoconfigure](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-autoconfigure)

`dubbo-spring-boot-autoconfigure` uses Spring Boot's `@EnableAutoConfiguration` which helps core Dubbo's components to be auto-configured by `DubboAutoConfiguration`. It reduces code, eliminates XML configuration.

### [dubbo-spring-boot-actuator](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-actuator)

`dubbo-spring-boot-actuator` provides production-ready features (e.g. [health checks](https://github.com/dubbo/dubbo-spring-boot-project/tree/master/dubbo-spring-boot-actuator#health-checks), [endpoints](https://github.com/dubbo/dubbo-spring-boot-project/tree/master/dubbo-spring-boot-actuator#endpoints), and [externalized configuration](https://github.com/dubbo/dubbo-spring-boot-project/tree/master/dubbo-spring-boot-actuator#externalized-configuration)).

### [dubbo-spring-boot-starter](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-starter)

`dubbo-spring-boot-starter` is a standard Spring Boot Starter, which contains [dubbo-spring-boot-autoconfigure](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-autoconfigure) and [dubbo-spring-boot-actuator](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-actuator). It will be imported into your application directly.

### [dubbo-spring-boot-samples](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-samples)

The samples project of Dubbo Spring Boot that includes two parts:

#### [Dubbo Provider Sample](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-samples/dubbo-spring-boot-sample-provider)

Dubbo Service will be exported on localhost with port `12345`.
- [Health Checks](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-actuator#health-checks) : [http://localhost:9091/health](http://localhost:9091/health)
- [Dubbo Endpoint](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-actuator#endpoints) : [http://localhost:9091/dubbo](http://localhost:9091/dubbo)

#### [Dubbo Consumer Sample](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-samples/dubbo-spring-boot-sample-consumer)

Dubbo Service will be consumed at Spring WebMVC `Controller` .
- Demo `Controller` : [http://localhost:8080/sayHello?name=HelloWorld](http://localhost:8080/sayHello?name=HelloWorld)
- [Health Checks](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-actuator#health-checks) : [http://localhost:8081/health](http://localhost:8081/health)
- [Dubbo Endpoint](https://gitee.com/mirrors/Dubbo-Spring-Boot-Project/blob/master/dubbo-spring-boot-actuator#endpoints) : [http://localhost:8081/dubbo](http://localhost:8081/dubbo)



