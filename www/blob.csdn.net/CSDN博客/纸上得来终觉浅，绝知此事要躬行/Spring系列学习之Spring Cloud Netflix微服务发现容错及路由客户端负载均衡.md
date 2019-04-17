# Spring系列学习之Spring Cloud Netflix微服务发现容错及路由客户端负载均衡 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 15:27:38[boonya](https://me.csdn.net/boonya)阅读数：95标签：[Spring																[Cloud																[Netflix](https://so.csdn.net/so/search/s.do?q=Netflix&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-cloud-netflix](https://spring.io/projects/spring-cloud-netflix)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[入门](#%E5%85%A5%E9%97%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Netflix通过自动配置和Spring环境以及其他Spring编程模型习惯用法为Spring Boot应用程序提供Netflix OSS集成。通过一些简单的注释，您可以快速启用和配置应用程序内的常见模式，并使用经过实战考验的Netflix组件构建大型分布式系统。提供的模式包括服务发现（Eureka），断路器（Hystrix），智能路由（Zuul）和客户端负载均衡（Ribbon）。

## 特性

Spring Cloud Netflix功能：
-     服务发现：可以注册Eureka实例，客户端可以使用Spring管理的bean发现实例
-     服务发现：可以使用声明性Java配置创建嵌入式Eureka服务器
-     断路器：Hystrix客户端可以使用简单的注释驱动方法装饰器构建
-     断路器：带有声明性Java配置的嵌入式Hystrix仪表板
-     声明性REST客户端：Feign创建使用JAX-RS或Spring MVC注释修饰的接口的动态实现
-     客户端负载均衡器：功能区
-     外部配置：从Spring Environment到Archaius的桥接（使用Spring Boot约定启用Netflix组件的本机配置）
-     路由器和过滤器：Zuul过滤器的自动注册，以及反向代理创建的配置方法的简单约定

## 入门

只要Spring Cloud Netflix和Eureka Core在类路径上，任何带有@EnableEurekaClient的Spring Boot应用程序都会尝试联系[http:// localhost:8761](http://localhost:8761/)上的Eureka服务器（默认值为eureka.client.serviceUrl.defaultZone）：

```java
@SpringBootApplication
@EnableEurekaClient
@RestController
public class Application {

  @RequestMapping("/")
  public String home() {
    return "Hello World";
  }

  public static void main(String[] args) {
    SpringApplication.run(Application.class, args);
  }

}
```

要运行您自己的服务器，请使用spring-cloud-starter-netflix-eureka-server依赖项和@EnableEurekaServer。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/2.1.0.RC3/)|
|----|----|----|
|2.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-netflix/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-netflix/2.0.x/)|
|2.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/2.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/2.0.2.RELEASE/)|
|1.4.7 SNAPSHOT|[Reference Doc.](https://cloud.spring.io/spring-cloud-netflix/1.4.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-netflix/1.4.x/)|
|1.4.6 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/1.4.6.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/1.4.6.RELEASE/)|
|1.3.6 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/1.3.6.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/1.3.6.RELEASE/)|
|1.3.5 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/1.3.5.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-netflix)|
|1.2.7 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-netflix/1.2.7.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-netflix/tree/1.2.x)|

# 示例

尝试一些示例：
- [**Service Discovery**](https://github.com/spring-cloud-samples/eureka)
- [**Circuit Breaker Dashboard**](https://github.com/spring-cloud-samples/hystrix-dashboard)](https://so.csdn.net/so/search/s.do?q=Cloud&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




