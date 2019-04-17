# Spring系列学习之Spring Apache Kafka - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 21:52:11[boonya](https://me.csdn.net/boonya)阅读数：48








英文原文：[https://spring.io/projects/spring-kafka](https://spring.io/projects/spring-kafka)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Kafka客户端兼容性](#Kafka%E5%AE%A2%E6%88%B7%E7%AB%AF%E5%85%BC%E5%AE%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring for Apache Kafka（spring-kafka）项目将核心Spring概念应用于基于Kafka的消息传递解决方案的开发。 它提供了一个“模板”作为发送消息的高级抽象。 它还通过@KafkaListener注释和“侦听器容器”为消息驱动的POJO提供支持。 这些库促进了依赖注入和声明的使用。 在所有这些情况下，您将看到Spring Framework中的JMS支持和Spring AMQP中的RabbitMQ支持的相似之处。

## 特性
-     KafkaTemplate
-     KafkaMessageListenerContainer
-     @KafkaListener
-     KafkaTransactionManager
-      带嵌入式kafka服务器的spring-kafka-test jar

## Kafka客户端兼容性

Apache Kafka的Spring基于纯java kafka-clients jar。 以下是兼容性矩阵：
|Spring for Apache Kafka Version|Spring Integration for Apache Kafka Version|`kafka-clients`|
|----|----|----|
|2.2.x|3.1.x|2.0.0, 2.1.0|
|2.1.x|3.0.x|1.0.x, 1.1.x, 2.0.0|
|2.0.x|3.0.x|0.11.0.x, 1.0.x|
|1.3.x|2.3.x|0.11.0.x, 1.0.x|
|1.2.x|2.2.x|0.10.2.x|
|1.1.x|2.1.x|0.10.0.x, 0.10.1.x|
|1.0.x|2.0.x|0.9.x.x|
|N/A*|1.3.x|0.8.2.2|

**重要提示：**此矩阵是客户端兼容性;在大多数情况下（自0.10.2.0起），较新的客户可以与较旧的经纪人沟通。所有使用经纪人> = 0.10.x.x（**以及所有spring boot 1.5.x用户**）的用户都建议使用spring-kafka 1.3.x或更高版本，因为[KIP-62](https://cwiki.apache.org/confluence/display/KAFKA/KIP-62%3A+Allow+consumer+to+send+heartbeats+from+a+background+thread)的线程模型更简单。有关客户端/代理兼容性的完整讨论，请参阅Kafka[兼容性矩阵](https://cwiki.apache.org/confluence/display/KAFKA/Compatibility+Matrix)
-     Spring 2.0之前的Spring集成Kafka版本早于Spring for Apache Kafka项目，因此不是基于它。



使用maven或gradle进行版本管理时，将以传递方式引用这些版本。对于1.1.x版本，0.10.1.x是默认版本。


2.1.x默认使用1.1.x kafka-clients。覆盖2.1.x的kafka-clients时，请参阅[文档附录](https://docs.spring.io/spring-kafka/docs/2.1.x/reference/html/deps-for-11x.html)。


2.2.x默认使用2.0.x kafka-clients。覆盖2.2.x的kafka-clients时，请参阅[文档附录](https://docs.spring.io/spring-kafka/docs/2.2.1.BUILD-SNAPSHOT/reference/html/deps-for-21x.html)。
-     Spring Boot 1.5用户应该使用1.3.x（默认情况下，引导依赖关系管理将使用1.1.x，因此应该覆盖它）。
-     Spring Boot 2.0用户应该使用2.0.x（引导依赖管理将使用正确的版本）。
-     Spring Boot 2.1用户应该使用2.2.x（引导依赖管理将使用正确的版本）。

## Spring Boot配置

[Spring for Apache Kafka的Spring Boot自动配置](https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-messaging.html#boot-features-kafka)

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.2.2 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-kafka/docs/2.2.2.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-kafka/docs/2.2.2.RELEASE/api/)|
|----|----|----|
|2.2.3 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-kafka/docs/2.2.3.BUILD-SNAPSHOT/reference/html/)|[API Doc.](https://docs.spring.io/spring-kafka/docs/2.2.3.BUILD-SNAPSHOT/api/)|
|2.1.12 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-kafka/docs/2.1.12.BUILD-SNAPSHOT/reference/html/)|[API Doc.](https://docs.spring.io/spring-kafka/docs/2.1.12.BUILD-SNAPSHOT/api/)|
|2.1.11 GA|[Reference Doc.](https://docs.spring.io/spring-kafka/docs/2.1.11.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-kafka/docs/2.1.11.RELEASE/api/)|
|1.3.9 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-kafka/docs/1.3.9.BUILD-SNAPSHOT/reference/html/)|[API Doc.](https://docs.spring.io/spring-kafka/docs/1.3.9.BUILD-SNAPSHOT/api/)|
|1.3.8 GA|[Reference Doc.](https://docs.spring.io/spring-kafka/docs/1.3.8.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-kafka/docs/1.3.8.RELEASE/api/)|

# 示例

尝试一些示例：
- [**Spring for Apache Kafka Samples**](https://github.com/spring-projects/spring-kafka/tree/master/samples)





