# Spring系列学习之Spring Cloud Commons基础支持库 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 17:37:09[boonya](https://me.csdn.net/boonya)阅读数：93








英文原文：[https://spring.io/projects/spring-cloud-commons](https://spring.io/projects/spring-cloud-commons)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Cloud Context功能](#Spring%20Cloud%20Context%E5%8A%9F%E8%83%BD%EF%BC%9A)

[Spring Cloud Commons功能](#Spring%20Cloud%20Commons%E5%8A%9F%E8%83%BD%EF%BC%9A)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Spring Cloud Commons提供两个库的功能：Spring Cloud Context和Spring Cloud Commons。 Spring Cloud Context为Spring Cloud应用程序的ApplicationContext（引导上下文，加密，刷新范围和环境端点）提供实用程序和特殊服务。 Spring Cloud Commons是一组用于不同Spring Cloud实现的抽象和公共类（例如，Spring Cloud Netflix与Spring Cloud Consul）。

## 特性

### Spring Cloud Context功能
-      引导上下文
- TextEncryptor bean
-      刷新范围
-      用于操纵环境（Environment）的Spring Boot Actuator端点

### Spring Cloud Commons功能
-      DiscoveryClient接口
-      ServiceRegistry接口
-      RestTemplate用于使用DiscoveryClient解析主机名的工具

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC2 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/2.1.0.RC2/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/2.1.0.RC2/)|
|----|----|----|
|2.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-commons/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-commons/2.0.x/)|
|2.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/2.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/2.0.2.RELEASE/)|
|1.3.6 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-commons/1.3.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-commons/1.3.x/)|
|1.3.5 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/1.3.5.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/1.3.5.RELEASE/)|
|1.2.5 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/1.2.5.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/1.2.5.RELEASE/)|
|1.1.8 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/1.1.8.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-commons)|
|1.0.4 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-commons/1.0.4.RELEASE/)|[API Doc.](https://github.com/spring-cloud/spring-cloud-commons)|





