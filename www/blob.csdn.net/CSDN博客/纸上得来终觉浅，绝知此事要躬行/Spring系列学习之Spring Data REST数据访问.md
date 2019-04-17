# Spring系列学习之Spring Data REST数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 21:16:17[boonya](https://me.csdn.net/boonya)阅读数：89标签：[Spring																[Data																[REST](https://so.csdn.net/so/search/s.do?q=REST&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-data-rest](https://spring.io/projects/spring-data-rest)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Data REST是Spring Data项目的一部分，可以轻松地在Spring Data存储库之上构建超媒体驱动的REST Web服务。


Spring Data REST构建于Spring Data存储库之上，分析应用程序的域模型，并为模型中包含的聚合公开超媒体驱动的HTTP资源。

## 特性
-     使用HAL作为媒体类型为您的域模型公开可发现的REST API。
-     公开代表您的模型的[集合，项目和关联资源](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#repository-resources)。
-     通过[导航链接](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#paging-and-sorting)支持分页。
-     允许动态过滤收集资源。
-     为存储库中定义的[查询方法公开专用搜索资源](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#repository-resources.query-method-resource)。
-     允许通过处理Spring ApplicationEvents来[挂钩处理REST请求](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#events)。
-     公开有关作为ALPS和JSON Schema发现的模型的[元数据](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#metadata)。
-     允许通过[投影](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#projections-excerpts)定义客户特定的表示。
-     发布[HAL浏览器](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#_the_hal_browser)的自定义变体以利用公开的元数据。
-     目前支持JPA，MongoDB，Neo4j，Solr，Cassandra，Gemfire。
-     允许对公开的默认资源进行[高级自定义](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#customizing-sdr)。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|3.1.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/rest/docs/3.1.3.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/rest/docs/3.1.3.RELEASE/api)|
|----|----|----|
|3.2.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/rest/docs/3.2.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/rest/docs/3.2.0.M1/api)|
|3.2.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-rest#learn)|[API Doc.](https://spring.io/projects/spring-data-rest#learn)|
|3.1.4 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-rest#learn)|[API Doc.](https://spring.io/projects/spring-data-rest#learn)|
|3.0.13 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-rest#learn)|[API Doc.](https://spring.io/projects/spring-data-rest#learn)|
|3.0.12 GA|[Reference Doc.](https://docs.spring.io/spring-data/rest/docs/3.0.12.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/rest/docs/3.0.12.RELEASE/api)|
|2.6.18 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-rest#learn)|[API Doc.](https://spring.io/projects/spring-data-rest#learn)|
|2.6.17 GA|[Reference Doc.](https://docs.spring.io/spring-data/rest/docs/2.6.17.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/rest/docs/2.6.17.RELEASE/api)|

# 示例

尝试一些示例：
- [**Spring Data Examples - REST**](https://github.com/spring-projects/spring-data-examples/tree/master/rest)
- [**Spring-a-Gram**](https://github.com/gregturn/spring-a-gram)
- [**Spring RESTBucks**](https://github.com/olivergierke/spring-restbucks)
- [**Spring Data Book Example**](https://github.com/SpringSource/spring-data-book/tree/master/rest)](https://so.csdn.net/so/search/s.do?q=Data&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




