# Spring系列学习之Spring Data Couchbase数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 22:28:23[boonya](https://me.csdn.net/boonya)阅读数：640








英文原文：[https://spring.io/projects/spring-data-couchbase](https://spring.io/projects/spring-data-couchbase)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Spring Data for Couchbase是Spring Data项目的一部分，旨在为新数据存储提供熟悉且一致的基于Spring的编程模型，同时保留特定于商店的特性和功能。

Spring Data Couchbase项目提供与Couchbase Server数据库的集成。 Spring Data Couchbase的主要功能领域是以POJO为中心的模型，用于与Couchbase Buckets交互并轻松编写Repository样式数据访问层。

## 特性
-     Spring配置支持使用基于Java的@Configuration类或Couchbase驱动程序的XML命名空间（Java SDK版本2.x）。 
-  CouchbaseTemplate助手类可提高执行常见Couchbase操作的效率。包括文档和POJO之间的集成对象映射。
-     异常转换为Spring的可移植数据访问异常层次结构。
-     功能丰富的对象映射与Spring的转换服务集成。
-     基于注释的映射元数据，但可扩展以支持其他元数据格式。
-     自动实现Repository接口，包括支持自定义查找器方法（由Couchbase的查询语言N1QL支持）和PagingAndSortingRepository。
-     对于Couchbase服务器版本<4.0，存储库仍可由Couchbase Views支持。
-     支持地理空间和多维查询（由Couchbase Spatial Views支持）
-     JMX管理和监控
-     可以作为@Cacheable支持的后端，缓存高性能访问所需的任何对象（请参阅Couchbase的github中的兄弟Spring Cache项目，[couchbaselabs/couchbase-spring-cache](https://github.com/couchbaselabs/couchbase-spring-cache)）。

## 快速开始



使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|3.1.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/couchbase/docs/3.1.3.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/couchbase/docs/3.1.3.RELEASE/api)|
|----|----|----|
|3.2.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/couchbase/docs/3.2.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/couchbase/docs/3.2.0.M1/api)|
|3.2.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-couchbase#learn)|[API Doc.](https://spring.io/projects/spring-data-couchbase#learn)|
|3.1.4 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-couchbase#learn)|[API Doc.](https://spring.io/projects/spring-data-couchbase#learn)|
|3.0.13 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-couchbase#learn)|[API Doc.](https://spring.io/projects/spring-data-couchbase#learn)|
|3.0.12 GA|[Reference Doc.](https://docs.spring.io/spring-data/couchbase/docs/3.0.12.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/couchbase/docs/3.0.12.RELEASE/api)|
|2.2.18 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-couchbase#learn)|[API Doc.](https://spring.io/projects/spring-data-couchbase#learn)|
|2.2.17 GA|[Reference Doc.](https://docs.spring.io/spring-data/couchbase/docs/2.2.17.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/couchbase/docs/2.2.17.RELEASE/api)|



