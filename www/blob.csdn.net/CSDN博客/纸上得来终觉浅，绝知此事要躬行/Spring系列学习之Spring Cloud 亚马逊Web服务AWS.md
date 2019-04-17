# Spring系列学习之Spring Cloud 亚马逊Web服务AWS - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 15:52:21[boonya](https://me.csdn.net/boonya)阅读数：324标签：[Spring																[Cloud																[AWS](https://so.csdn.net/so/search/s.do?q=AWS&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-cloud-aws](https://spring.io/projects/spring-cloud-aws)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[基于注解的SQS队列侦听器](#%E5%9F%BA%E4%BA%8E%E6%B3%A8%E8%A7%A3%E7%9A%84SQS%E9%98%9F%E5%88%97%E4%BE%A6%E5%90%AC%E5%99%A8)

[基于注解的SNS监听器](#%E5%9F%BA%E4%BA%8E%E6%B3%A8%E8%A7%A3%E7%9A%84SNS%E7%9B%91%E5%90%AC%E5%99%A8)

[消息模板](#%C2%A0%E6%B6%88%E6%81%AF%E6%A8%A1%E6%9D%BF)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud for Amazon Web Services是Spring Cloud项目的一部分，可以简化与托管Amazon Web Services的集成。它提供了一种使用众所周知的Spring习语和API（如消息传递或缓存API）与AWS提供的服务进行交互的便捷方式。开发人员可以围绕托管服务构建应用程序，而无需关心基础结构或维护。

## 特性
-     [SQS]的Spring Messaging API实现（[http://aws.amazon.com/sqs/](http://aws.amazon.com/sqs/)）。
-     [ElastiCache]的Spring Cache API实现（[http://aws.amazon.com/elasticache/](http://aws.amazon.com/elasticache/)）。
-     基于注释的[SNS]（[http://aws.amazon.com/sns/](http://aws.amazon.com/sns/)）端点（HTTP）映射。
-     按照[CloudFormation]（[http://aws.amazon.com/cloudformation/](http://aws.amazon.com/cloudformation/)）堆栈中定义的逻辑名访问资源。
-     基于[RDS]（[http://aws.amazon.com/rds/](http://aws.amazon.com/rds/)）实例的逻辑名称创建自动JDBC DataSource。
-     Ant风格路径匹配[S3]（[http://aws.amazon.com/s3/](http://aws.amazon.com/s3/)）存储桶的ResourceLoader。

### 基于注解的SQS队列侦听器

```java
@MessageMapping("logicalQueueName")
private void receiveMessage(Person person, @Header("SenderId") String senderId) {
    // ...
}
```

### 基于注解的SNS监听器

```java
@Controller
@RequestMapping("/sns/receive")
public class SnsEndpointController {

@NotificationMessageMapping
public void receiveNotification(@NotificationMessage String message, @NotificationSubject String subject) {
    // ...
}

@NotificationSubscriptionMapping
public void confirmSubscription(NotificationStatus notificationStatus) {
    notificationStatus.confirmSubscription();
}
```

###  消息模板

```java
snsTemplate.sendNotification("SnsTopic", "message", "subject");
sqsTemplate.convertAndSend("Queue", new Person("John", "Doe"));
```

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC1 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/2.1.0.RC1/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/2.1.0.RC1/)|
|----|----|----|
|2.0.2 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-aws/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-aws/2.0.x/)|
|2.0.1 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/2.0.1.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/2.0.1.RELEASE/)|
|1.2.4 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-aws/1.2.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-aws/1.2.x/)|
|1.2.3 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/1.2.3.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/1.2.3.RELEASE/)|
|1.1.4 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/1.1.4.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-aws)|
|1.0.4 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-aws/1.0.4.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-aws)|

# 示例

尝试一些例子：
- [**Reference Application**](https://github.com/spring-cloud-samples/aws-refapp)](https://so.csdn.net/so/search/s.do?q=Cloud&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




