# Spring系列学习之Spring Cloud Function微服务功能目标实现 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 18:49:35[boonya](https://me.csdn.net/boonya)阅读数：267








英文原文：[https://cloud.spring.io/spring-cloud-function/](https://cloud.spring.io/spring-cloud-function/)

**目录**

[Spring Cloud Function](#Spring%20Cloud%20Function)

[特性](#%E7%89%B9%E6%80%A7)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[示例项目](#%E7%A4%BA%E4%BE%8B%E9%A1%B9%E7%9B%AE)

## Spring Cloud Function

Spring Cloud Function是一个具有以下高级目标的项目：
-      通过功能促进业务逻辑的实现。
-      将业务逻辑的开发生命周期与任何特定的运行时目标分离，以便相同的代码可以作为Web端点，流处理器或任务运行。
-      支持无服务器提供商之间的统一编程模型，以及独立运行（本地或PaaS）的能力。
-      在无服务器提供商上启用Spring Boot功能（自动配置，依赖注入，指标）。



它抽象出所有传输细节和基础架构，允许开发人员保留所有熟悉的工具和流程，并专注于业务逻辑。

## 特性

Spring Cloud功能特性：
-      函数-Function，消费者-Consumer和供应商-Supplier类型的@Beans的包装器，使用RabbitMQ，Kafka等将它们作为HTTP端点和/或消息流监听器/发布者公开给外部世界。
-      将作为Java函数体的字符串编译为字节码，然后将它们转换为可以如上所述进行包装的@Beans。
-      使用隔离的类加载器部署包含此类应用程序上下文的JAR文件，以便您可以将它们打包在一个JVM中。
- [AWS Lambda](https://github.com/spring-cloud/spring-cloud-function/tree/master/spring-cloud-function-adapters/spring-cloud-function-adapter-aws)，[Microsoft Azure](https://github.com/spring-cloud/spring-cloud-function/tree/master/spring-cloud-function-adapters/spring-cloud-function-adapter-azure)，[Apache OpenWhisk](https://github.com/spring-cloud/spring-cloud-function/tree/master/spring-cloud-function-adapters/spring-cloud-function-adapter-openwhisk)以及可能的其他“无服务器”服务提供商的适配器。

这是一个完整的，可执行的，可测试的Spring Boot应用程序（实现简单的字符串操作）：

```java
@SpringBootApplication
public class Application {

  @Bean
  public Function<Flux<String>, Flux<String>> uppercase() {
    return flux -> flux.map(value -> value.toUpperCase());
  }

  public static void main(String[] args) {
    SpringApplication.run(Application.class, args);
  }
}
```

## 发布版本

Spring Cloud Function

Release

Documentation

2.0.0 RC3

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-function/2.0.0.RC3/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-function/2.0.0.RC3/)

1.0.1

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-function/1.0.0.RELEASE/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-function/1.0.0.RELEASE/)

## 示例项目
- [Vanilla](https://github.com/spring-cloud/spring-cloud-function/blob/master/spring-cloud-function-samples/function-sample)
- [Plain Old Function](https://github.com/spring-cloud/spring-cloud-function/blob/master/spring-cloud-function-samples/function-sample-pof)
- [AWS Lambda](https://github.com/spring-cloud/spring-cloud-function/tree/master/spring-cloud-function-samples/function-sample-aws)
- [Microsoft Azure](https://github.com/spring-cloud/spring-cloud-function/tree/master/spring-cloud-function-samples/function-sample-azure)
- [Openwhisk](https://github.com/spring-cloud/spring-cloud-function/tree/master/spring-cloud-function-adapters/spring-cloud-function-adapter-openwhisk)





