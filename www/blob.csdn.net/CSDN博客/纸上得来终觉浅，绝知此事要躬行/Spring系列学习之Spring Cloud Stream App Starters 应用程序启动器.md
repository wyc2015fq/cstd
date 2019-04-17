# Spring系列学习之Spring Cloud Stream  App Starters 应用程序启动器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 17:10:35[boonya](https://me.csdn.net/boonya)阅读数：211








英文原文：[https://cloud.spring.io/spring-cloud-stream-app-starters/](https://cloud.spring.io/spring-cloud-stream-app-starters/)

**目录**

[Spring Cloud Stream App Starters](#Spring%20Cloud%20Stream%20App%20Starters)

[特性](#%E7%89%B9%E6%80%A7)

[可用应用](#%E5%8F%AF%E7%94%A8%E5%BA%94%E7%94%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[使用Spring Boot 1.5.x + Spring Cloud Stream 1.3.x：](#%E4%BD%BF%E7%94%A8Spring%20Boot%201.5.x%20%2B%20Spring%20Cloud%20Stream%201.3.x%EF%BC%9A)

[使用Spring Boot 2.0.x + Spring Cloud Stream 2.0.x:](#%E4%BD%BF%E7%94%A8Spring%20Boot%202.0.x%20%2B%20Spring%20Cloud%20Stream%202.0.x%3A)

[使用Spring Boot 2.1.x + Spring Cloud Stream 2.1.x:](#%E4%BD%BF%E7%94%A8Spring%20Boot%202.1.x%20%2B%20Spring%20Cloud%20Stream%202.1.x%3A)

[HTTP Repository Location for Apps](#http-repository-location-for-apps)

[RabbitMQ Apps](#rabbitmq-apps)

[Apache Kafka Apps](#apache-kafka-apps)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[相关项目](#%E7%9B%B8%E5%85%B3%E9%A1%B9%E7%9B%AE)

## Spring Cloud Stream App Starters

Spring Cloud Stream Application Starters是基于[Spring Boot](https://projects.spring.io/spring-boot/)的[Spring Integration](https://projects.spring.io/spring-integration/)应用程序，可提供与外部系统的集成。 Spring Cloud Stream应用程序可与[Spring Cloud Data Flow](https://cloud.spring.io/spring-cloud-dataflow/)一起使用，以创建，部署和编排消息驱动的微服务应用程序。

Spring Cloud Stream Application Starters是独立的可执行应用程序，可通过Apache Kafka和RabbitMQ等消息传递中间件进行通信。 这些应用程序可以在各种运行时平台上独立运行，包括：Cloud Foundry，Apache Yarn，Apache Mesos，Kubernetes，Docker，甚至可以在您的笔记本电脑上运行。

## 特性
-      独立运行作为Spring Boot应用程序
-      将微服务组合为Spring Cloud Data Flow中的流管道
-      将微服务应用程序用作maven或docker工件
-      通过命令行，环境变量或YAML文件覆盖配置参数
-      提供基础架构以单独测试应用程序
-      从此版本的[Spring Initializr](https://start-scs.cfapps.io/)下载为初学者

## 可用应用
|**Source**|**Processor**|**Sink**|
|----|----|----|
|file|aggregator|aggregate-counter|
|ftp|bridge|cassandra|
|gemfire|filter|counter|
|gemfire-cq|groovy-filter|field-value-counter|
|http|groovy-transform|file|
|jdbc|header-enricher|ftp|
|jms|httpclient|gemfire|
|load-generator|pmml|gpfdist|
|loggregator|python-http|hdfs|
|mail|python-jython|hdfs-dataset|
|mongodb|scriptable-transform|jdbc|
|mqtt|splitter|log|
|rabbit|tasklaunchrequest-transform|mongodb|
|s3|tcp-client|mqtt|
|sftp|tensorflow|pgcopy|
|syslog|transform|rabbit|
|tcp|twitter-sentiment|redis-pubsub|
|tcp-client|grpc|router|
|time||s3|
|trigger||sftp|
|triggertask||task-launcher-cloudfoundry|
|twitterstream||task-launcher-local|
|||task-launcher-dataflow|
|||task-launcher-yarn|
|||tcp|
|||throughput|
|||websocket|



## 快速开始



第1步 - 从这里下载最新的基于Kafka 10的时间源应用程序[例如：/1.3.0.RELEASE/time-source-kafka-10-1.3.0.RELEASE.jar]


第2步 - 从这里下载最新的基于Kafka 10的日志接收器应用程序[例如：/1.3.0.RELEASE/log-sink-kafka-10-1.3.0.RELEASE.jar]


第3步 - 启动Kafka 0.10.1.1


第4步 - 运行时间源并绑定到ticktock主题

```
java -jar time-source-kafka-***.jar --spring.cloud.stream.bindings.output.destination=ticktock
```

第5步 - 运行Log Sink并绑定到ticktock主题

```
java -jar log-sink-kafka-***.jar --spring.cloud.stream.bindings.input.destination=ticktock
```

第6步- 在控制台中验证ticktock日志

流应用程序启动器和Spring云数据流（**）

## 使用Spring Boot 1.5.x + Spring Cloud Stream 1.3.x：
|**Artifact Type**|**Stable Release**|**Snapshot/Milestone Release**|
|----|----|----|
|RabbitMQ + Maven|http://bit.ly/Celsius-SR3-stream-applications-rabbit-maven|http://bit.ly/Celsius-BUILD-SNAPSHOT-stream-applications-rabbit-maven|
|RabbitMQ + Docker|http://bit.ly/Celsius-SR3-stream-applications-rabbit-docker|http://bit.ly/Celsius-BUILD-SNAPSHOT-stream-applications-rabbit-docker|
|Apache Kafka 0.10 + Maven|http://bit.ly/Celsius-SR3-stream-applications-kafka-10-maven|http://bit.ly/Celsius-BUILD-SNAPSHOT-stream-applications-kafka-10-maven|
|Apache Kafka 0.10 + Docker|http://bit.ly/Celsius-SR3-stream-applications-kafka-10-docker|http://bit.ly/Celsius-BUILD-SNAPSHOT-stream-applications-kafka-10-docker|

## **使用Spring Boot 2.0.x + Spring Cloud Stream 2.0.x:**
|**Artifact Type**|**Stable Release**|**Snapshot/Milestone Release**|
|----|----|----|
|RabbitMQ + Maven|http://bit.ly/Darwin-SR3-stream-applications-rabbit-maven|http://bit.ly/Darwin-BUILD-SNAPSHOT-stream-applications-rabbit-maven|
|RabbitMQ + Docker|http://bit.ly/Darwin-SR3-stream-applications-rabbit-docker|http://bit.ly/Darwin-BUILD-SNAPSHOT-stream-applications-rabbit-docker|
|Apache Kafka + Maven|http://bit.ly/Darwin-SR3-stream-applications-kafka-maven|http://bit.ly/Darwin-BUILD-SNAPSHOT-stream-applications-kafka-maven|
|Apache Kafka + Docker|http://bit.ly/Darwin-SR3-stream-applications-kafka-docker|http://bit.ly/Darwin-BUILD-SNAPSHOT-stream-applications-kafka-docker|

## **使用Spring Boot 2.1.x + Spring Cloud Stream 2.1.x:**
|**Artifact Type**|**Milestone Release**|**Snapshot Release**|
|----|----|----|
|RabbitMQ + Maven|http://bit.ly/Einstein-M1-stream-applications-rabbit-maven|http://bit.ly/Einstein-BUILD-SNAPSHOT-stream-applications-rabbit-maven|
|RabbitMQ + Docker|http://bit.ly/Einstein-M1-stream-applications-rabbit-docker|http://bit.ly/Einstein-BUILD-SNAPSHOT-stream-applications-rabbit-docker|
|Apache Kafka + Maven|http://bit.ly/Einstein-M1-stream-applications-kafka-maven|http://bit.ly/Einstein-BUILD-SNAPSHOT-stream-applications-kafka-maven|
|Apache Kafka + Docker|http://bit.ly/Einstein-M1-stream-applications-kafka-docker|http://bit.ly/Einstein-BUILD-SNAPSHOT-stream-applications-kafka-docker|

 （**）通过查看参考指南中的[应用程序注册](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#supported-apps-and-tasks)部分，了解如何在Spring Cloud Data Flow中使用app-starters

## HTTP Repository Location for Apps

### RabbitMQ Apps

With Spring Boot 1.5.x:
- [Stable Release](https://repo.spring.io/libs-release/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Celsius.SR3/spring-cloud-stream-app-descriptor-Celsius.SR3.rabbit-apps-maven-repo-url.properties)
- [Latest Snapshot](https://repo.spring.io/libs-snapshot/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Celsius.BUILD-SNAPSHOT/spring-cloud-stream-app-descriptor-Celsius.BUILD-SNAPSHOT.rabbit-apps-maven-repo-url.properties)

With Spring Boot 2.0.x:
- [Stable Release](https://repo.spring.io/libs-release/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Darwin.SR3/spring-cloud-stream-app-descriptor-Darwin.SR3.rabbit-apps-maven-repo-url.properties)
- [Latest Snapshot](https://repo.spring.io/libs-snapshot/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Darwin.BUILD-SNAPSHOT/spring-cloud-stream-app-descriptor-Darwin.BUILD-SNAPSHOT.rabbit-apps-maven-repo-url.properties)

### Apache Kafka Apps

With Spring Boot 1.5.x:
- [Stable Release](https://repo.spring.io/libs-release/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Celsius.SR3/spring-cloud-stream-app-descriptor-Celsius.SR3.kafka-10-apps-maven-repo-url.properties)
- [Latest Snapshot](https://repo.spring.io/libs-snapshot/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Celsius.BUILD-SNAPSHOT/spring-cloud-stream-app-descriptor-Celsius.BUILD-SNAPSHOT.kafka-10-apps-maven-repo-url.properties)

With Spring Boot 2.0.x:
- [Stable Release](https://repo.spring.io/libs-release/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Darwin.SR3/spring-cloud-stream-app-descriptor-Darwin.SR3.kafka-apps-maven-repo-url.properties)
- [Latest Snapshot](https://repo.spring.io/libs-snapshot/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Darwin.BUILD-SNAPSHOT/spring-cloud-stream-app-descriptor-Darwin.BUILD-SNAPSHOT.kafka-10-apps-maven-repo-url.properties)

With Spring Boot 2.1.x:
- [Milestone Release](https://repo.spring.io/libs-milestone/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Einstein.M1/spring-cloud-stream-app-descriptor-Einstein.M1.kafka-apps-maven-repo-url.properties)
- [Latest Snapshot](https://repo.spring.io/libs-snapshot/org/springframework/cloud/stream/app/spring-cloud-stream-app-descriptor/Einstein.BUILD-SNAPSHOT/spring-cloud-stream-app-descriptor-Einstein.BUILD-SNAPSHOT.kafka-apps-maven-repo-url.properties)



## 发布版本

Spring Cloud Stream App Starters

Release

Documentation

Einstein M1

[Reference](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Einstein.M1/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Einstein.M1/api/)

Einstein

[Reference](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Einstein.BUILD-SNAPSHOT/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Einstein.BUILD-SNAPSHOT/api/)

Darwin SR3

[Reference](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Darwin.SR3/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Darwin.SR3/api/)

Darwin

[Reference](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Darwin.BUILD-SNAPSHOT/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Darwin.BUILD-SNAPSHOT/api/)

Celsius SR3

[Reference](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Celsius.SR3/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-stream-app-starters/docs/Celsius.SR3/api/)

## 相关项目
- [Spring Cloud Stream](https://cloud.spring.io/spring-cloud-stream/)
- [Spring Cloud Data Flow](https://cloud.spring.io/spring-cloud-dataflow/)
- [Spring Cloud Task](https://github.com/spring-cloud/spring-cloud-task/)



