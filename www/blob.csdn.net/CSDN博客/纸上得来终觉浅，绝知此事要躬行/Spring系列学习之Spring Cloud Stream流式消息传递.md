# Spring系列学习之Spring Cloud Stream流式消息传递 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 15:37:59[boonya](https://me.csdn.net/boonya)阅读数：474








英文原文：[https://cloud.spring.io/spring-cloud-stream/](https://cloud.spring.io/spring-cloud-stream/)

**目录**

[Spring Cloud Stream](#Spring%20Cloud%20Stream)

[绑定器实现](#%E7%BB%91%E5%AE%9A%E5%99%A8%E5%AE%9E%E7%8E%B0)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[Apache Kafka客户端兼容性](#Apache%20Kafka%E5%AE%A2%E6%88%B7%E7%AB%AF%E5%85%BC%E5%AE%B9%E6%80%A7)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[示例项目](#sample-projects)

[相关项目](#related-projects)

## Spring Cloud Stream


Spring Cloud Stream是一个用于构建与共享消息传递系统相连的高度可扩展的事件驱动微服务的框架。


该框架提供了基于已经建立和熟悉的Spring习语和最佳实践的灵活编程模型，包括对持久性Pub/sub（发布/订阅），使用者组和有状态分区的支持。

**Spring Cloud Stream的核心构建块是：**
-      目标绑定器：负责提供与外部消息传递系统集成的组件。
-      目标绑定：外部消息传递系统和应用程序之间的桥接消息的生产者和消费者（由目标绑定器创建）。
-      消息：生产者和使用者使用的规范数据结构与目标绑定器（以及通过外部消息传递系统的其他应用程序）进行通信。

![](https://img-blog.csdnimg.cn/20181224152401181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)



## 绑定器实现

Spring Cloud Stream支持各种binder实现，下表包含GitHub项目的链接。


|[RabbitMQ](https://github.com/spring-cloud/spring-cloud-stream-binder-rabbit)|
|----|
|[Apache Kafka](https://github.com/spring-cloud/spring-cloud-stream-binder-kafka)|
|[Amazon Kinesis](https://github.com/spring-cloud/spring-cloud-stream-binder-aws-kinesis)|
|[Google PubSub *(partner maintained)*](https://github.com/spring-cloud/spring-cloud-gcp/tree/master/spring-cloud-gcp-pubsub-stream-binder)|
|[Solace PubSub+ *(partner maintained)*](https://github.com/SolaceProducts/spring-cloud-stream-binder-solace)|
|[Azure Event Hubs *(partner maintained)*](https://github.com/Microsoft/spring-cloud-azure/tree/master/spring-cloud-azure-eventhub-stream-binder)|



## 快速开始

推荐使用spring-cloud-stream的方法是使用[Spring Initializr](https://start.spring.io/)。 只需按照这些说明操作 您还可以使用依赖关系管理系统在项目中开始使用它 - 下面的代码段可以复制并粘贴到您的[构建](https://docs.spring.io/spring-cloud-stream/docs/Elmhurst.RELEASE/reference/htmlsingle/#_quick_start)中。 需要帮忙？ 请参阅我们的[Maven](https://spring.io/guides/gs/maven/)和[Gradle](https://spring.io/guides/gs/gradle/)构建入门指南。（导航到英文原文可以选择版本和依赖方式）

*Maven *

```
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-stream-dependencies</artifactId>
            <version>Fishtown.RC4</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-stream</artifactId>
    </dependency>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-stream-rabbit</artifactId> <!-- or '*-stream-kafka' -->
    </dependency>
</dependencies><repositories>
    <repository>
        <id>spring-milestones</id>
        <name>Spring Milestones</name>
        <url>https://repo.spring.io/libs-milestone</url>
        <snapshots>
            <enabled>false</enabled>
        </snapshots>
    </repository>
</repositories>
```

*Gradle*

```
buildscript {
    dependencies {
        classpath "io.spring.gradle:dependency-management-plugin:0.5.2.RELEASE"
    }
}

apply plugin: "io.spring.dependency-management"

dependencyManagement {
     imports {
          mavenBom 'org.springframework.cloud:spring-cloud-stream-dependencies:Fishtown.RC4'
     }
}

dependencies {
    compile 'org.springframework.cloud:spring-cloud-stream'
    compile 'org.springframework.cloud:spring-cloud-starter-stream-kafka'
}repositories {
    maven {
        url 'https://repo.spring.io/libs-milestone'
    }
}
```

只要Spring Cloud Stream和Spring Cloud Stream Binder依赖项在类路径上，任何带有@EnableBinding的Spring Boot应用程序都会绑定到外部消息传递系统（例如[Rabbit MQ](https://github.com/spring-cloud/spring-cloud-stream-binder-rabbit)或[Apache Kafka](https://github.com/spring-cloud/spring-cloud-stream-binder-kafka)，具体取决于选择的binder实现）。


有关更全面的逐步入门体验，请参阅用户指南的“快速入门”部分。


以下是功能齐全的Spring Cloud Stream应用程序，它接收数据并将数据记录到控制台。

```java
@SpringBootApplication
@EnableBinding(Sink.class)
public class LoggingConsumerApplication {

	public static void main(String[] args) {
		SpringApplication.run(LoggingConsumerApplication.class, args);
	}

	@StreamListener(Sink.INPUT)
	public void handle(Person person) {
		System.out.println("Received: " + person);
	}

	public static class Person {
		private String name;
		public String getName() {
			return name;
		}
		public void setName(String name) {
			this.name = name;
		}
		public String toString() {
			return this.name;
		}
	}
}
```

从前面的例子可以看出：
-      我们使用@EnableBinding（Sink.class）启用了Sink绑定（input-no-output）。 这样做是向框架发出信号以启动与消息传递系统的绑定，它自动创建绑定到Sink.INPUT通道的目标（即队列，主题和其他）。
-      我们添加了一个处理程序方法来接收Person类型的传入消息。 这样做可以让您看到框架的核心功能之一：它尝试自动将传入的消息有效负载转换为Person类型。



您只需运行main（..）方法即可启动应用程序。

## Apache Kafka客户端兼容性

Spring Cloud Stream支持各种Apache Kafka Client版本。 要了解有关它的更多信息，请参阅Wiki页面中的[Apache Kafka客户端兼容性](https://github.com/spring-cloud/spring-cloud-stream/wiki/Kafka-Client-Compatibility)部分。

## 发布版本

Spring Cloud Stream

Release

Documentation

Fishtown RC4

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-stream/2.1.0.RC4/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-stream/2.1.0.RC4)

Fishtown

[Reference](http://docs.spring.io/spring-cloud-stream/docs/Fishtown.BUILD-SNAPSHOT/reference/htmlsingle/)[API](http://docs.spring.io/spring-cloud-stream/docs/Fishtown.BUILD-SNAPSHOT/api/)

Elmhurst SR2

[Reference](http://docs.spring.io/spring-cloud-stream/docs/Elmhurst.SR2/reference/htmlsingle/)[API](http://docs.spring.io/spring-cloud-stream/docs/Elmhurst.SR2/api/)

Ditmars SR4

[Reference](http://docs.spring.io/spring-cloud-stream/docs/Ditmars.SR4/reference/htmlsingle/)[API](http://docs.spring.io/spring-cloud-stream/docs/Ditmars.SR4/api/)

Ditmars

[Reference](http://docs.spring.io/spring-cloud-stream/docs/Ditmars.BUILD-SNAPSHOT/reference/htmlsingle/)[API](http://docs.spring.io/spring-cloud-stream/docs/Ditmars.BUILD-SNAPSHOT/api/)

Chelsea SR2

[Reference](http://docs.spring.io/spring-cloud-stream/docs/Chelsea.SR2/reference/htmlsingle/index.html)[API](http://docs.spring.io/spring-cloud-stream/docs/Chelsea.SR2/api/)

Chelsea

[Reference](http://docs.spring.io/spring-cloud-stream/docs/Chelsea.BUILD-SNAPSHOT/reference/htmlsingle/index.html)[API](http://docs.spring.io/spring-cloud-stream/docs/Chelsea.BUILD-SNAPSHOT/api/)

Brooklyn SR3

[Reference](http://docs.spring.io/spring-cloud-stream/docs/Brooklyn.SR3/reference/htmlsingle/)[API](http://docs.spring.io/spring-cloud-stream/docs/Brooklyn.SR3/api/)

## 示例项目
- [Spring Cloud Stream Samples](https://github.com/spring-cloud/spring-cloud-stream-samples)

## 相关项目
- [Spring Cloud Stream Applications](https://cloud.spring.io/spring-cloud-stream-app-starters/)
- [Spring Cloud Data Flow](https://cloud.spring.io/spring-cloud-dataflow/)



