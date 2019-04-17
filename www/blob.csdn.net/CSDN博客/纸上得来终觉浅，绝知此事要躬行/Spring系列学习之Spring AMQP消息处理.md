# Spring系列学习之Spring AMQP消息处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 16:14:39[boonya](https://me.csdn.net/boonya)阅读数：79








英文原文：[https://spring.io/projects/spring-amqp](https://spring.io/projects/spring-amqp)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring AMQP项目将核心Spring概念应用于基于AMQP的消息传递解决方案的开发。 它提供了一个“模板”作为发送和接收消息的高级抽象。 它还为具有“侦听器容器”的消息驱动的POJO提供支持。 这些库有助于管理AMQP资源，同时促进依赖注入和声明性配置的使用。 在所有这些情况下，您将看到Spring Framework中与JMS支持的相似之处。


该项目由两部分组成; spring-amqp是基础抽象，spring-rabbit是RabbitMQ实现。

## 特性
-      用于异步处理入站消息的侦听器容器
-      RabbitTemplate用于发送和接收消息
-      RabbitAdmin用于自动声明队列，交换和绑定

## Spring Boot配置
- [Spring AMQP的Spring Boot自动配置（RabbitMQ）](https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-messaging.html#boot-features-amqp)

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.3 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-amqp/docs/2.1.3.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-amqp/docs/2.1.3.BUILD-SNAPSHOT/api/)|
|----|----|----|
|2.1.2 GA|[Reference Doc.](https://docs.spring.io/spring-amqp/docs/2.1.2.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-amqp/docs/2.1.2.RELEASE/api/)|
|2.0.11 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-amqp/docs/2.0.11.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-amqp/docs/2.0.11.BUILD-SNAPSHOT/api/)|
|2.0.10 GA|[Reference Doc.](https://docs.spring.io/spring-amqp/docs/2.0.10.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-amqp/docs/2.0.10.RELEASE/api/)|
|1.7.12 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-amqp/docs/1.7.12.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-amqp/docs/1.7.12.BUILD-SNAPSHOT/api/)|
|1.7.11 GA|[Reference Doc.](https://docs.spring.io/spring-amqp/docs/1.7.11.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-amqp/docs/1.7.11.RELEASE/api/)|

# 示例

尝试一些例子：
- [**Spring AMQP samples**](https://github.com/spring-projects/spring-amqp-samples)





