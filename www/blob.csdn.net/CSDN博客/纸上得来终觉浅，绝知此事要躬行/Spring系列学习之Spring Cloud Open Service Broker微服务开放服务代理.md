# Spring系列学习之Spring Cloud Open Service Broker微服务开放服务代理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 15:35:44[boonya](https://me.csdn.net/boonya)阅读数：339








英文原文：[https://spring.io/projects/spring-cloud-open-service-broker](https://spring.io/projects/spring-cloud-open-service-broker)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Open Service Broker是一个用于构建实现Open Service Broker API的Spring Boot应用程序的框架。


Open Service Broker API项目允许开发人员为云本地平台（如Cloud Foundry，Kubernetes和OpenShift）中运行的应用程序提供服务。 Spring Cloud Open Service Broker提供了一个基于Spring Boot的框架，使您能够在支持Open Service Broker API的平台上为您自己的托管服务快速创建服务代理。

## Spring Boot配置

在项目中开始使用Spring Cloud Open Service Broker的推荐方法是使用依赖关系管理系统。 下面的一个代码段可以复制并粘贴到您的构建中。

*使用Maven：*

```
<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-open-service-broker-webmvc</artifactId>
        <version>2.1.0.RELEASE</version>
    </dependency>
</dependencies>
```

*使用Gradle：*

```
dependencies {
    compile 'org.springframework.cloud:spring-cloud-starter-open-service-broker-webmvc:2.1.0.RELEASE'
}
```

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|3.0.0 M3 PRE|[Reference Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/3.0.0.M3/reference/html5)|[API Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/3.0.0.M3/apidocs/)|
|----|----|----|
|3.0.0 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/3.0.0.BUILD-SNAPSHOT/reference/html5)|[API Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/3.0.0.BUILD-SNAPSHOT/apidocs/)|
|2.1.1 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/2.1.1.BUILD-SNAPSHOT/reference/html5)|[API Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/2.1.1.BUILD-SNAPSHOT/apidocs/)|
|2.1.0 GA|[Reference Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/2.1.0.RELEASE/reference/html5)|[API Doc.](https://docs.spring.io/spring-cloud-open-service-broker/docs/2.1.0.RELEASE/apidocs/)|



# 示例

尝试一些例子：
- [**Bookstore Service Broker**](https://github.com/spring-cloud-samples/bookstore-service-broker)示例服务代理应用程序



