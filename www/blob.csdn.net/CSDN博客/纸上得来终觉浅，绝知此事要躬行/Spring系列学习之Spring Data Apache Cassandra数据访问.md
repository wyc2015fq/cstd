# Spring系列学习之Spring Data Apache Cassandra数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 21:25:54[boonya](https://me.csdn.net/boonya)阅读数：132








英文原文：[https://spring.io/projects/spring-data-cassandra](https://spring.io/projects/spring-data-cassandra)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Apache Cassandra NoSQL数据库为寻求处理高速，大容量和可变数据流的解决方案的团队提供了许多新功能。 这种新的思维方式引入了新的概念和学习曲线，可能会让团队成员和团队经理感到恐惧。 Apache Cassandra的Spring Data为过去使用过其他Spring Data模块的人提供了一个熟悉的界面。


使用Apache Cassandra的Spring Data时，使用Apache Cassandra开发应用程序的学习曲线显着减少。 凭借注释POJO保持高水平的能力，或具有高性能数据提取功能的低级别，Apache Cassandra模板的Spring Data肯定能满足每个应用程序的需求。

## 特性
-      基于常见的Spring Data接口构建存储库
-      支持同步和异步数据操作
-      异步回调支持
-      支持基于XML的Keyspace创建和CQL表创建
-      所有集群和会话功能的JavaConfig和XML支持
-      异常转换为熟悉的Spring DataAccessException层次结构
-      方便的QueryBuilders消除了学习CQL的需要
-      自动实现Repository接口，包括支持自定义查询方法
-      基于最新的DataStax Enterprise CQL Java驱动程序

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/cassandra/docs/2.1.3.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/cassandra/docs/2.1.3.RELEASE/api)|
|----|----|----|
|2.2.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/cassandra/docs/2.2.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/cassandra/docs/2.2.0.M1/api)|
|2.2.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-cassandra#learn)|[API Doc.](https://spring.io/projects/spring-data-cassandra#learn)|
|2.1.4 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-cassandra#learn)|[API Doc.](https://spring.io/projects/spring-data-cassandra#learn)|
|2.0.13 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-cassandra#learn)|[API Doc.](https://spring.io/projects/spring-data-cassandra#learn)|
|2.0.12 GA|[Reference Doc.](https://docs.spring.io/spring-data/cassandra/docs/2.0.12.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/cassandra/docs/2.0.12.RELEASE/api)|
|1.5.18 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-cassandra#learn)|[API Doc.](https://spring.io/projects/spring-data-cassandra#learn)|
|1.5.17 GA|[Reference Doc.](https://docs.spring.io/spring-data/cassandra/docs/1.5.17.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/cassandra/docs/1.5.17.RELEASE/api)|





