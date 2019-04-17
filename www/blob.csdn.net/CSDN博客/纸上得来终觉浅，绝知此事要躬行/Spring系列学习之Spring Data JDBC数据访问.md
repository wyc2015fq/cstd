# Spring系列学习之Spring Data JDBC数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 17:16:21[boonya](https://me.csdn.net/boonya)阅读数：210








英文原文：[https://spring.io/projects/spring-data-jdbc](https://spring.io/projects/spring-data-jdbc)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[Aggregate Root聚合根](#Aggregate%20Root%E8%81%9A%E5%90%88%E6%A0%B9)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Spring Data JDBC是更大的Spring Data系列的一部分，可以轻松实现基于JDBC的存储库。该模块处理对基于JDBC的数据访问层的增强支持。它使构建使用数据访问技术的Spring驱动程序更容易。


Spring Data JDBC旨在简化概念。为了实现这一点，它不提供缓存，延迟加载，后写或JPA的许多其他功能。这使得Spring Data JDBC成为一种简单，有限，固定的ORM。

## Aggregate Root聚合根

Spring Data存储库的灵感来自于Eric Evans的Domain Driven Design一书中描述的存储库。这样做的一个结果是每个聚合根应该有一个存储库。 Aggregate Root是同一本书中的另一个概念，描述了一个实体，它控制其他实体的生命周期，这些实体一起是聚合。 Aggregate是模型的一个子集，它在对Aggregate Root的方法调用之间保持一致。


Spring Data JDBC尽力鼓励您根据这些想法对域进行建模。

## 特性
-     具有可自定义NamingStrategy的简单聚合的CRUD操作。
-     支持@Query注释。
-     支持MyBatis查询。
-     活动。
-     基于JavaConfig的存储库配置，介绍@EnableJdbcRepositories。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.0.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/jdbc/docs/1.0.3.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/jdbc/docs/1.0.3.RELEASE/api)|
|----|----|----|
|1.1.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/jdbc/docs/1.1.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/jdbc/docs/1.1.0.M1/api)|
|1.1.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-jdbc#learn)|[API Doc.](https://spring.io/projects/spring-data-jdbc#learn)|
|1.0.4 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-jdbc#learn)|[API Doc.](https://spring.io/projects/spring-data-jdbc#learn)|





