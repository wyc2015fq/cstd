# Spring系列学习之Spring Session MongoDB会话管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月26日 00:06:18[boonya](https://me.csdn.net/boonya)阅读数：1538








英文原文：[https://spring.io/projects/spring-session-data-mongodb](https://spring.io/projects/spring-session-data-mongodb)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Spring会话MongoDB提供了一个API和实现，用于通过利用Spring Data MongoDB来管理存储在MongoDB中的用户会话信息。

## 特性

Spring Session MongoDB提供以下功能：
-      用于管理用户会话的API和实现
-      HttpSession - 允许在应用程序容器（即Tomcat）中立方式中替换HttpSession
-      集群会话 - Spring会话使得支持集群会话变得微不足道，而不依赖于特定于应用程序容器的解决方案。
-      多个浏览器会话 - Spring会话支持在单个浏览器实例中管理多个用户的会话（即，与Google类似的多个经过身份验证的帐户）。
-      RESTful API - Spring Session允许在头文件中提供会话ID以使用RESTful API WebSocket - 提供在接收WebSocket消息时保持HttpSession活动的能力

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.1 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.1.1.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.1.1.RELEASE/api/)|
|----|----|----|
|2.1.2 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.1.2.BUILD-SNAPSHOT/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.1.2.BUILD-SNAPSHOT/api/)|
|2.0.5 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.0.5.BUILD-SNAPSHOT/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.0.5.BUILD-SNAPSHOT/api/)|
|2.0.4 GA|[Reference Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.0.4.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-session-data-mongodb/docs/2.0.4.RELEASE/api/)|



