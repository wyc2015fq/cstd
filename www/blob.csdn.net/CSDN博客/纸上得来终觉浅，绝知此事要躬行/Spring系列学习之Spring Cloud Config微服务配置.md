# Spring系列学习之Spring Cloud Config微服务配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 17:51:12[boonya](https://me.csdn.net/boonya)阅读数：63标签：[Spring																[Cloud																[Config](https://so.csdn.net/so/search/s.do?q=Config&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-cloud-config](https://spring.io/projects/spring-cloud-config)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[入门](#%E5%85%A5%E9%97%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Config为分布式系统中的外部化配置提供服务器和客户端支持。使用Config Server，您可以在所有环境中管理应用程序的外部属性。客户端和服务器上的概念映射与Spring Environment和PropertySource抽象相同，因此它们非常适合Spring应用程序，但可以与任何语言运行的任何应用程序一起使用。当应用程序通过部署管道从开发到测试并进入生产时，您可以管理这些环境之间的配置，并确保应用程序具有迁移时需要运行的所有内容。服务器存储后端的默认实现使用git，因此它可以轻松支持配置环境的标记版本，以及可用于管理内容的各种工具。添加替代实现并使用Spring配置插入它们很容易。

## 特性

Spring Cloud Config Server功能：
-     用于外部配置的HTTP，基于资源的API（名称 - 值对或等效的YAML内容）
-     加密和解密属性值（对称或非对称）
-     使用@EnableConfigServer可以在Spring Boot应用程序中轻松嵌入



Config Client功能（适用于Spring应用程序）：
-     绑定到Config Server并使用远程属性源初始化Spring Environment
-     加密和解密属性值（对称或非对称）

## 入门

只要Spring Boot Actuator和Spring Config Client在类路径上，任何Spring Boot应用程序都会尝试联系http：// localhost：8888上的配置服务器，默认值为spring.cloud.config.uri。如果您想更改此默认值，可以在bootstrap.yml |properties中设置spring.cloud.config.uri或通过系统属性或环境变量。

```java
@Configuration
@EnableAutoConfiguration
@RestController
public class Application {

  @Value("${config.name}")
  String name = "World";

  @RequestMapping("/")
  public String home() {
    return "Hello " + name;
  }

  public static void main(String[] args) {
    SpringApplication.run(Application.class, args);
  }

}
```

示例中config.name的值（或以正常Spring Boot方式绑定的任何其他值）可以来自本地配置或来自远程Config Server。 默认情况下，Config Server优先。 要查看应用程序中的/ env端点，请查看configServer属性源。


要运行自己的服务器，请使用spring-cloud-config-server依赖项和@EnableConfigServer。 如果设置spring.config.name = configserver，则应用程序将在端口8888上运行，并从示例存储库提供数据。 您需要spring.cloud.config.server.git.uri来根据自己的需要找到配置数据（默认情况下，它是git存储库的位置，可以是本地文件：.. URL）。 

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/2.1.0.RC3/)|
|----|----|----|
|2.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-config/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-config/2.0.x/)|
|2.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/2.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/2.0.2.RELEASE/)|
|1.4.5 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-config/1.4.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-config/1.4.x/)|
|1.4.4 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/1.4.4.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/1.4.4.RELEASE/)|
|1.3.4 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/1.3.4.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-config/1.3.4.RELEASE/)|



# 示例

尝试一些示例：
- [**Config Server**](https://github.com/spring-cloud-samples/configserver)
- [**Config Clients**](https://github.com/spring-cloud-samples/customers-stores)
- [**Minimal (Groovy) Client**](https://github.com/spring-cloud-samples/scripts/blob/master/demo/app.groovy)](https://so.csdn.net/so/search/s.do?q=Cloud&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




