# Spring系列学习之Spring Session 用户会话管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月26日 00:01:48[boonya](https://me.csdn.net/boonya)阅读数：1408








英文原文：[https://spring.io/projects/spring-session](https://spring.io/projects/spring-session)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[模块](#%E6%A8%A1%E5%9D%97)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%C2%A0%E7%A4%BA%E4%BE%8B)

# 概述

Spring Session提供了用于管理用户会话信息的API和实现。

## 特性

Spring Session使得支持集群会话变得微不足道，而不依赖于特定于应用程序容器的解决方案。它还提供透明集成：
- HttpSession - 允许以应用程序容器（即Tomcat）中立的方式替换HttpSession，支持在头文件中提供会话ID以使用RESTful API
- WebSocket - 提供在接收WebSocket消息时保持HttpSession活动的能力
- WebSession - 允许以应用程序容器中立方式替换Spring WebFlux的WebSession

## 模块

Spring Session包含以下模块：
-     Spring Session Core - 提供核心Spring Session功能和API
-     Spring会话数据Redis - 提供由Redis支持的SessionRepository和ReactiveSessionRepository实现以及配置支持
-     Spring Session JDBC - 提供由关系数据库和配置支持支持的SessionRepository实现
-     Spring Session Hazelcast - 提供由Hazelcast和配置支持支持的SessionRepository实现

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.3 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-session/docs/2.1.3.BUILD-SNAPSHOT/reference/html5/)|[API Doc.](https://docs.spring.io/spring-session/docs/2.1.3.BUILD-SNAPSHOT/api/)|
|----|----|----|
|2.1.2 GA|[Reference Doc.](https://docs.spring.io/spring-session/docs/2.1.2.RELEASE/reference/html5/)|[API Doc.](https://docs.spring.io/spring-session/docs/2.1.2.RELEASE/api/)|
|2.0.9 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-session/docs/2.0.9.BUILD-SNAPSHOT/reference/html5/)|[API Doc.](https://docs.spring.io/spring-session/docs/2.0.9.BUILD-SNAPSHOT/api/)|
|2.0.8 GA|[Reference Doc.](https://docs.spring.io/spring-session/docs/2.0.8.RELEASE/reference/html5/)|[API Doc.](https://docs.spring.io/spring-session/docs/2.0.8.RELEASE/api/)|
|1.3.5 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-session/docs/1.3.5.BUILD-SNAPSHOT/reference/html5/)|[API Doc.](https://docs.spring.io/spring-session/docs/1.3.5.BUILD-SNAPSHOT/api/)|
|1.3.4 GA|[Reference Doc.](https://docs.spring.io/spring-session/docs/1.3.4.RELEASE/reference/html5/)|[API Doc.](https://docs.spring.io/spring-session/docs/1.3.4.RELEASE/api/)|

#  示例

尝试一些例子：
- [**HttpSession & Redis**](https://docs.spring.io/spring-session/docs/current/reference/html5/guides/java-redis.html)
- [**Spring Boot Integration**](https://docs.spring.io/spring-session/docs/current/reference/html5/guides/boot-redis.html)
- [**Spring Security Integration**](https://docs.spring.io/spring-session/docs/current/reference/html5/guides/java-security.html)
- [**RESTful APIs**](https://docs.spring.io/spring-session/docs/current/reference/html5/guides/java-rest.html)
- [**WebSocket Integration**](https://docs.spring.io/spring-session/docs/current/reference/html5/guides/boot-websocket.html)



