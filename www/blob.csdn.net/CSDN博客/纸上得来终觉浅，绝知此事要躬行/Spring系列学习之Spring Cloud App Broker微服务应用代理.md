# Spring系列学习之Spring Cloud App Broker微服务应用代理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 20:36:44[boonya](https://me.csdn.net/boonya)阅读数：306








英文原文：[https://spring.io/projects/spring-cloud-app-broker](https://spring.io/projects/spring-cloud-app-broker)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Spring Cloud App Broker是一个用于构建Spring Boot应用程序的框架，该应用程序实现[Open Service Broker API](https://www.openservicebrokerapi.org/)以将应用程序部署到平台。


Open Service Broker API项目允许开发人员为云本地平台（如Cloud Foundry，Kubernetes和OpenShift）中运行的应用程序提供服务。 Spring Cloud App Broker提供了一个基于Spring Boot的框架，使您能够快速创建服务代理，在配置托管服务时将应用程序和服务部署到平台。


目前支持Cloud Foundry作为服务实例支持应用程序和服务的目标平台。

## Spring Boot配置




在项目中开始使用Spring Cloud App Broker的推荐方法是使用依赖关系管理系统。 下面的一个代码段可以复制并粘贴到您的构建中。


使用Maven：

```
<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-app-broker-cloudfoundry</artifactId>
        <version>1.0.0.BUILD-SNAPSHOT</version>
    </dependency>
</dependencies>
```

使用Gradle：

```
dependencies {
    compile 'org.springframework.cloud:spring-cloud-starter-app-broker-cloudfoundry:1.0.0.BUILD-SNAPSHOT'
}
```

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.0.0 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-cloud-app-broker/docs/1.0.0.BUILD-SNAPSHOT/reference/html5)|[API Doc.](https://docs.spring.io/spring-cloud-app-broker/docs/1.0.0.BUILD-SNAPSHOT/api/)|
|----|----|----|



