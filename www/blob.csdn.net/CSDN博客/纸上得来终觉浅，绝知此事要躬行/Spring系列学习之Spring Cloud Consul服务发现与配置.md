# Spring系列学习之Spring Cloud Consul服务发现与配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 18:06:58[boonya](https://me.csdn.net/boonya)阅读数：62








英文原文：



# 概述

Spring Cloud Consul通过自动配置和Spring环境以及其他Spring编程模型习惯用法提供Spring Boot应用程序的Consul集成。通过一些简单的注释，您可以快速启用和配置应用程序内的常见模式，并使用Hashicorp的Consul构建大型分布式系统。提供的模式包括服务发现，分布式配置和控制总线。

## 特征

Spring Cloud Consul功能：
-     服务发现：可以向Consul代理注册实例，客户端可以使用Spring管理的bean发现实例
-     通过Spring Cloud Netflix支持Ribbon，客户端负载均衡器
-     通过Spring Cloud Netflix支持Zuul，一个动态路由器和过滤器
-     分布式配置：使用Consul Key / Value存储
-     控制总线：使用Consul事件的分布式控制事件

## 快速开始

只要Spring Cloud Consul和Consul API在类路径上，任何带有@EnableDiscoveryClient的Spring Boot应用程序都会尝试联系localhost：8500上的Consul代理（spring.cloud.consul.host和spring.cloud.consul.port的默认值）：

```java
@Configuration
@EnableAutoConfiguration
@EnableDiscoveryClient
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

必须运行本地Consul代理。 请参阅[Consul代理文档](https://consul.io/docs/agent/basics.html)，了解如何运行代理。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/2.1.0.RC3/)|
|----|----|----|
|2.0.2 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-consul/spring-cloud-consul.html)|[API Doc.](https://cloud.spring.io/spring-cloud-consul/spring-cloud-consul.html)|
|2.0.1 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/2.0.1.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/2.0.1.RELEASE/)|
|1.3.5 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-consul/1.3.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-consul/1.3.x/)|
|1.3.4 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/1.3.4.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/1.3.4.RELEASE/)|
|1.2.3 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/1.2.3.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/1.2.3.RELEASE/)|

# 示例

尝试一些例子：
- [**Consul Sample**](https://github.com/spring-cloud/spring-cloud-consul/tree/master/spring-cloud-consul-sample)



