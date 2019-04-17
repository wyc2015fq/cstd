# Spring系列学习之Spring Cloud Sleuth分布式跟踪解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 16:49:35[boonya](https://me.csdn.net/boonya)阅读数：168








英文原文：[https://spring.io/projects/spring-cloud-sleuth](https://spring.io/projects/spring-cloud-sleuth)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Sleuth为Spring Cloud实施分布式跟踪解决方案，大量借用[Dapper](http://research.google.com/pubs/pub36356.html)，[Zipkin](https://github.com/openzipkin/zipkin)和HTrace。对于大多数用户来说，侦探应该是隐形的，并且所有与外部系统的交互都应该自动进行检测。您可以简单地在日志中捕获数据，也可以将数据发送到远程收集器服务。

## 特性

Span是基本工作单元。例如，发送RPC是一个新的跨度，就像向RPC发送响应一样。跨度由跨度的唯一64位ID和跨度为其一部分的跟踪的另一个64位ID标识。 Spans还有其他数据，例如描述，键值注释，导致它们的跨度的ID以及进程ID（通常是IP地址）。跨度启动和停止，他们跟踪他们的时间信息。创建跨度后，必须在将来的某个时刻停止它。一组跨度形成一个称为Trace的树状结构。例如，如果您正在运行分布式大数据存储，则可能会由put请求形成跟踪。


Spring Cloud Sleuth功能：
-     将跟踪和跨度ID添加到Slf4J MDC，因此您可以从日志聚合器中的给定跟踪或跨度中提取所有日志。
-     提供对常见分布式跟踪数据模型的抽象：跟踪，跨距（形成DAG），注释，键值注释。松散地基于HTrace，但兼容Zipkin（Dapper）。
-     乐器常见的入口和出口点来自Spring应用程序（servlet过滤器，休息模板，预定动作，消息通道，zuul过滤器，假装客户端）。
-     如果spring-cloud-sleuth-zipkin可用，则该应用程序将通过HTTP生成并收集与Zipkin兼容的跟踪。默认情况下，它将它们发送到localhost（端口9411）上的Zipkin收集器服务。使用spring.zipkin.baseUrl配置服务的位置。

## Spring Boot配置

将侦听（Sleuth）添加到类路径：

*Maven：*

```
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-sleuth</artifactId>
            <version>${spring-cloud-sleuth.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-sleuth</artifactId>
    </dependency>
</dependencies>
```

*Gradle：*

```
buildscript {
    dependencies {
        classpath "io.spring.gradle:dependency-management-plugin:0.5.2.RELEASE"
    }
}

apply plugin: "io.spring.dependency-management"

dependencyManagement {
     imports {
          mavenBom "org.springframework.cloud:spring-cloud-sleuth:${springCloudSleuthVersion}"
     }
}
dependencies {
    compile 'org.springframework.cloud:spring-cloud-starter-sleuth'
}
```

只要Spring Cloud Sleuth位于类路径上，任何Spring Boot应用程序都将生成跟踪数据：

```java
@SpringBootApplication
@RestController
public class Application {

  private static Logger log = LoggerFactory.getLogger(DemoController.class);

  @RequestMapping("/")
  public String home() {
    log.info("Handling home");
    return "Hello World";
  }

  public static void main(String[] args) {
    SpringApplication.run(Application.class, args);
  }

}
```

运行此应用程序，然后点击主页。 您将在日志中看到填充了traceId和spanId。 如果这个应用程序调用另一个应用程序（例如使用RestTemplate），它将在标头中发送跟踪数据，如果接收器是另一个Sleuth应用程序，您将看到跟踪继续存在。
**重要**

而不是显式地在处理程序中记录请求，您可以设置logging.level.org.springframework.web.servlet.DispatcherServlet = DEBUG
**重要**

如果使用Zipkin，请通过设置（对于2.0.x）spring.sleuth.sampler.probability或（直到2.0.x）spring.sleuth.sampler.percentage（默认值：0.1，即10％）来配置导出跨度的概率）。 否则，您可能会认为侦探无法正常工作，因为它省略了一些跨度。
**重要**

设置spring.application.name = bar（例如）以查看服务名称以及跟踪和跨度ID。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/2.1.0.RC3/)|
|----|----|----|
|2.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-sleuth/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-sleuth/2.0.x/)|
|2.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/2.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/2.0.2.RELEASE/)|
|1.3.5 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-sleuth/1.3.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-sleuth/1.3.x/)|
|1.3.4 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/1.3.4.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/1.3.4.RELEASE/)|
|1.2.7 SNAPSHOT|[Reference Doc.](https://cloud.spring.io/spring-cloud-sleuth/1.2.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-sleuth/1.2.x/)|
|1.2.6 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/1.2.6.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/1.2.6.RELEASE/)|
|1.2.5 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/1.2.5.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-sleuth)|
|1.1.4 SNAPSHOT|[Reference Doc.](https://cloud.spring.io/spring-cloud-sleuth/1.1.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-sleuth/1.1.x/)|
|1.1.3 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/1.1.3.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-sleuth)|
|1.0.12 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-sleuth/1.0.12.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-sleuth)|

# 示例

尝试一些例子：
- [**Sleuth Web Mvc**](https://github.com/openzipkin/sleuth-webmvc-example)Simple Sleuth Web MVC示例



