# Spring系列学习之Spring Data Elasticsearch数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 22:40:23[boonya](https://me.csdn.net/boonya)阅读数：761








英文原文：[https://spring.io/projects/spring-data-elasticsearch](https://spring.io/projects/spring-data-elasticsearch)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Elasticsearch的Spring Data是Spring Data项目的一部分，该项目旨在为新数据存储提供熟悉且一致的基于Spring的编程模型，同时保留特定于商店的特性和功能。

Spring Data Elasticsearch项目提供与Elasticsearch搜索引擎的集成。 Spring Data Elasticsearch的关键功能区域是一个POJO中心模型，用于与Elastichsearch文档交互并轻松编写存储库样式数据访问层。

## 特性
-     使用基于Java的@Configuration类的Spring配置支持或ES客户端实例的XML命名空间。
- ElasticsearchTemplate助手类，可提高执行常见ES操作的效率。包括文档和POJO之间的集成对象映射。
-     功能丰富的对象映射与Spring的转换服务集成
-     基于注释的映射元数据，但可扩展以支持其他元数据格式
-     自动实现Repository接口，包括支持自定义finder方法。
-     CDI对存储库的支持

## 快速开始



使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|3.1.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/3.1.3.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/3.1.3.RELEASE/api)|
|----|----|----|
|3.2.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/3.2.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/3.2.0.M1/api)|
|3.2.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|[API Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|
|3.1.4 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|[API Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|
|3.0.13 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|[API Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|
|3.0.12 GA|[Reference Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/3.0.12.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/3.0.12.RELEASE/api)|
|2.1.18 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|[API Doc.](https://spring.io/projects/spring-data-elasticsearch#learn)|
|2.1.17 GA|[Reference Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/2.1.17.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/elasticsearch/docs/2.1.17.RELEASE/api)|



