# Spring系列学习之Spring Cloud Zookeeper服务发现及分布式配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 18:14:57[boonya](https://me.csdn.net/boonya)阅读数：285








英文原文：[https://spring.io/projects/spring-cloud-zookeeper](https://spring.io/projects/spring-cloud-zookeeper)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Zookeeper通过自动配置和Spring环境以及其他Spring编程模型习惯用法为Spring Boot应用程序提供Apache Zookeeper集成。通过一些简单的注释，您可以快速启用和配置应用程序内的公共模式，并使用Zookeeper构建大型分布式系统。提供的模式包括服务发现和分布式配置。

## 特性
-     服务发现：可以向Zookeeper注册实例，客户端可以使用Spring管理的bean发现实例
-     通过Spring Cloud Netflix支持Ribbon，客户端负载均衡器
-     通过Spring Cloud Netflix支持Zuul，一个动态路由器和过滤器
-     分布式配置：使用Zookeeper作为数据存储

## 快速开始

只要Spring Cloud Zookeeper，Apache Curator和Zookeeper Java Client在类路径上，任何带有@EnableDiscoveryClient的Spring Boot应用程序都会尝试联系localhost：2181上的Zookeeper代理（zookeeper.connectString的默认值）。

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

必须运行本地Zookeeper服务器。 有关如何运行Zookeeper服务器的信息，请参阅[Zookeeper文档](https://zookeeper.apache.org/doc/trunk/zookeeperStarted.html)。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC2 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/2.1.0.RC2/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/2.1.0.RC2/)|
|----|----|----|
|2.0.1 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-zookeeper/spring-cloud-zookeeper.html)|[API Doc.](https://cloud.spring.io/spring-cloud-zookeeper/spring-cloud-zookeeper.html)|
|2.0.0 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/2.0.0.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/2.0.0.RELEASE/)|
|1.2.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-zookeeper/1.2.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-zookeeper/1.2.x/)|
|1.2.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/1.2.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/1.2.2.RELEASE/)|
|1.1.3 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/1.1.3.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/1.1.3.RELEASE/)|
|1.1.2 SNAPSHOT|[Reference Doc.](https://github.com/spring-cloud/spring-cloud-zookeeper/tree/master/docs/src/main/asciidoc)|[API Doc.](https://github.com/spring-cloud/spring-cloud-zookeeper/)|
|1.1.1 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/1.1.1.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-zookeeper)|
|1.0.4 SNAPSHOT|[Reference Doc.](https://github.com/spring-cloud/spring-cloud-zookeeper/tree/1.0.x/docs/src/main/asciidoc)|[API Doc.](https://github.com/spring-cloud/spring-cloud-zookeeper/tree/1.0.x)|
|1.0.3 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-zookeeper/1.0.3.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-zookeeper/tree/1.0.x)|

# 示例

尝试一些例子：
- [**Zookeeper Sample**](https://github.com/spring-cloud/spring-cloud-zookeeper/tree/master/spring-cloud-zookeeper-sample)



