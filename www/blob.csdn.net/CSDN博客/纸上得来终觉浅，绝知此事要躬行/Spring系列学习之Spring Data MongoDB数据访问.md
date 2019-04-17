# Spring系列学习之Spring Data MongoDB数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 20:47:57[boonya](https://me.csdn.net/boonya)阅读数：50








英文原文：[https://spring.io/projects/spring-data-mongodb](https://spring.io/projects/spring-data-mongodb)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[介绍](#%E4%BB%8B%E7%BB%8D)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

[MongoDB](https://www.mongodb.org/)的Spring Data是Spring Data项目的一部分，旨在为新数据存储提供熟悉且一致的基于Spring的编程模型，同时保留特定于商店的特性和功能。

## 介绍

Spring Data MongoDB项目提供与MongoDB文档数据库的集成。 Spring Data MongoDB的关键功能区域是一个POJO中心模型，用于与MongoDB DBCollection交互并轻松编写Repository样式数据访问层。

## 特性
-     Spring配置支持使用基于Java的@Configuration类或Mongo驱动程序实例和副本集的XML命名空间。
-     MongoTemplate助手类，可提高执行常见Mongo操作的效率。包括文档和POJO之间的集成对象映射。
-     异常转换为Spring的可移植数据访问异常层次结构
-     功能丰富的对象映射与Spring的转换服务集成
-     基于注释的映射元数据，但可扩展以支持其他元数据格式
-     持久性和映射生命周期事件
-     使用MongoReader/MongoWriter抽象的低级映射
-     基于Java的查询，标准和更新DSL
-     自动实现Repository接口，包括支持自定义finder方法。
-     QueryDSL集成以支持类型安全查询。
-     跨存储持久性 - 使用MongoDB透明地持久保存/检索具有字段的JPA实体的支持
-     Log4j日志appender
-     地理空间整合
-     Map-Reduce集成
-     JMX管理和监控
-     CDI对存储库的支持
-     GridFS支持

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/mongodb/docs/2.1.3.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/mongodb/docs/2.1.3.RELEASE/api)|
|----|----|----|
|2.2.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/mongodb/docs/2.2.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/mongodb/docs/2.2.0.M1/api)|
|2.2.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-mongodb#learn)|[API Doc.](https://spring.io/projects/spring-data-mongodb#learn)|
|2.1.4 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-mongodb#learn)|[API Doc.](https://spring.io/projects/spring-data-mongodb#learn)|
|2.0.13 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-mongodb#learn)|[API Doc.](https://spring.io/projects/spring-data-mongodb#learn)|
|2.0.12 GA|[Reference Doc.](https://docs.spring.io/spring-data/mongodb/docs/2.0.12.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/mongodb/docs/2.0.12.RELEASE/api)|
|1.10.18 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-mongodb#learn)|[API Doc.](https://spring.io/projects/spring-data-mongodb#learn)|
|1.10.17 GA|[Reference Doc.](https://docs.spring.io/spring-data/mongodb/docs/1.10.17.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/mongodb/docs/1.10.17.RELEASE/api)|

# 示例

尝试一些例子：
- [**Spring Data Book**](https://github.com/spring-projects/spring-data-book)





