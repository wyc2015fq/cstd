# Spring系列学习之Spring HATEOAS - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 14:30:26[boonya](https://me.csdn.net/boonya)阅读数：77








英文原文：[https://spring.io/projects/spring-hateoas](https://spring.io/projects/spring-hateoas)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring HATEOAS提供了一些API，以便在使用Spring，特别是Spring MVC时，轻松创建遵循HATEOAS原则的REST表示。 它试图解决的核心问题是链接创建和表示组装。

## 特性
-      链接，资源表示模型的模型类
-      链接构建器API以创建指向Spring MVC控制器方法的链接
-      支持HAL等超媒体格式

## Spring Boot配置

Spring Boot将执行以下操作：


-      配置HAL支持
-      注册对[实体链接](https://docs.spring.io/spring-hateoas/docs/current/reference/html/#fundamentals.obtaining-links.entity-links)的支持
-      连接消息转换器支持。

## 快速开始



使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|0.25.0 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-hateoas/docs/0.25.0.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-hateoas/docs/0.25.0.RELEASE/api/)|
|----|----|----|
|1.0.0 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-hateoas/docs/1.0.0.BUILD-SNAPSHOT/reference/html/)|[API Doc.](https://docs.spring.io/spring-hateoas/docs/1.0.0.BUILD-SNAPSHOT/apidocs/)|
|0.26.0 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-hateoas/docs/0.26.0.BUILD-SNAPSHOT/reference/html/)|[API Doc.](https://docs.spring.io/spring-hateoas/docs/0.26.0.BUILD-SNAPSHOT/apidocs/)|
|0.25.1 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-hateoas/docs/0.25.1.BUILD-SNAPSHOT/reference/html/)|[API Doc.](https://docs.spring.io/spring-hateoas/docs/0.25.1.BUILD-SNAPSHOT/apidocs/)|

# 示例

尝试一些例子：
- [**Building a Hypermedia-Driven RESTful Web Service**](https://spring.io/guides/gs/rest-hateoas)





