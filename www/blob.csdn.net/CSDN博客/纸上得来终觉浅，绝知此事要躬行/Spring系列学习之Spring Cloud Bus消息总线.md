# Spring系列学习之Spring Cloud Bus消息总线 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 16:02:55[boonya](https://me.csdn.net/boonya)阅读数：25标签：[Spring																[Cloud																[Bus](https://so.csdn.net/so/search/s.do?q=Bus&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-cloud-bus](https://spring.io/projects/spring-cloud-bus)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[入门](#%E5%85%A5%E9%97%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Bus使用轻量级消息代理链接分布式系统的节点。 然后，这可以用于广播状态改变（例如，配置改变）或其他管理指令。 目前唯一的实现是使用AMQP代理作为传输，但是相同的基本功能集（以及一些更多取决于传输）是其他传输的路线图。

## 入门

只要Spring Cloud Bus AMQP和RabbitMQ在类路径上，任何Spring Boot应用程序都会尝试联系localhost:5672上的RabbitMQ服务器（spring.rabbitmq.addresses的默认值）：

```java
@Configuration
@EnableAutoConfiguration
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

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/2.1.0.RC3/)|
|----|----|----|
|2.0.1 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-bus/spring-cloud-bus.html)|[API Doc.](https://cloud.spring.io/spring-cloud-bus/spring-cloud-bus.html)|
|2.0.0 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/2.0.0.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/2.0.0.RELEASE/)|
|1.3.5 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-bus/1.3.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-bus/1.3.x/)|
|1.3.4 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/1.3.4.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/1.3.4.RELEASE/)|
|1.2.1 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/1.2.1.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-bus)|
|1.1.1 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/1.1.1.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-bus)|
|1.0.3 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-bus/1.0.3.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-bus)|

# 示例

尝试一些示例：
- [**Bus Clients**](https://github.com/spring-cloud-samples/customers-stores)](https://so.csdn.net/so/search/s.do?q=Cloud&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




