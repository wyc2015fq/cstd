# Spring系列学习之Spring Cloud 之Cloud Foundry服务代理(Service Broker) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 16:59:23[boonya](https://me.csdn.net/boonya)阅读数：213








英文原文：[https://spring.io/projects/spring-cloud-cloudfoundry-service-broker](https://spring.io/projects/spring-cloud-cloudfoundry-service-broker)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

该项目提供了一个框架，用于构建Spring Boot项目以快速实现Cloud Foundry的服务代理。

**注意**正在开发此项目的新版本，名称为[Spring Cloud Open Service Broker](https://spring.io/projects/spring-cloud-open-service-broker)。

## Spring Boot配置



在项目中开始使用Spring Cloud Open Service Broker的推荐方法是使用依赖关系管理系统。 下面的一个代码段可以复制并粘贴到您的构建中。


使用Maven：

```
<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-cloudfoundry-service-broker</artifactId>
        <version>${springCloudServiceBrokerVersion}</version>
    </dependency>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-cloudfoundry-service-broker</artifactId>
        <version>${springCloudServiceBrokerVersion}</version>
        <classifier>tests</classifier>
        <scope>test</scope>
    </dependency>
</dependencies>
```

使用Gradle：

```
dependencies {
    compile("org.springframework.cloud:spring-cloud-cloudfoundry-service-broker:1.0.2.RELEASE")
    testCompile(group: "org.springframework.cloud", name: "spring-cloud-cloudfoundry-service-broker", version: "1.0.2.RELEASE", classifier: "tests")
}
```

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.0.3 CURRENT GA|[Reference Doc.](https://github.com/spring-cloud/spring-cloud-cloudfoundry-service-broker/tree/1.0.x)|[API Doc.](https://docs.spring.io/spring-cloud-cloudfoundry-service-broker/docs/1.0.3.RELEASE/apidocs/)|
|----|----|----|
|1.0.4 SNAPSHOT|[Reference Doc.](https://github.com/spring-cloud/spring-cloud-cloudfoundry-service-broker/tree/1.0.x)|[API Doc.](https://docs.spring.io/spring-cloud-cloudfoundry-service-broker/docs/1.0.4.BUILD-SNAPSHOT/apidocs/)|



