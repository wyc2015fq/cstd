# Spring系列学习之Spring Data JDBC数据访问拓展 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 20:10:33[boonya](https://me.csdn.net/boonya)阅读数：82








英文原文：[https://spring.io/projects/spring-data-jdbc-ext](https://spring.io/projects/spring-data-jdbc-ext)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Core](#Core)

[Oracle](#Oracle)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

**注意：该项目不再积极开发，将于今年晚些时候束之高阁。**

# 概述

Spring Data JDBC Extensions是Spring Data项目的一部分。 Spring Framework中的JDBC支持非常广泛，涵盖了最常用的功能。 此扩展项目为使用高级Oracle数据库功能提供了额外的支持。


大部分支持由从SpringSource项目“Advanced Pack for Oracle Database”移植的代码组成，该代码可供支持订阅客户使用。 我们现在正在向所有Spring用户提供此代码，并且将在Spring Data JDBC Extensions项目中进行任何新的开发。

## 特性

### Core
-     QueryDSL支持（从1.2.0版开始不推荐使用） - 支持将QueryDSL SQL模块与QueryDslJdbcTemplate一起使用，该QueryDslJdbcTemplate结合了QueryDSL和熟悉的JdbcTemplate的功能。这为查询，插入，更新和删除操作提供了一个类型安全的API，并结合了Spring的JdbcTemplate支持的便利性，例如： RowMapper，资源管理，声明式事务管理等

### Oracle
-     RAC“快速连接故障转移” - RAC“快速连接故障转移”提供了在数据库节点发生故障时使Spring应用程序透明地进行故障转移的功能。
-     Streams AQ（高级队列） - AQ支持提供了使用单个本地事务管理器进行数据库和消息访问的选项，而无需采用昂贵的分布式两阶段提交事务管理。
-     XML类型 - 有关如何将Oracle JDBC扩展用于其本机XML类型的自定义类，示例和文档。
-     高级数据类型 - 有关如何将高级数据类型（如STRUCT和ARRAY）用于Oracle JDBC扩展的自定义类，示例和文档。
-     自定义数据源连接准备程序 - 此功能提供了一个API，用于使用Oracle特定会话设置等自定义连接环境。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.2.1 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/jdbc/old-docs/1.2.1.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-data/jdbc/old-docs/1.2.1.RELEASE/api/)|
|----|----|----|
|2.0.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/jdbc/old-docs/2.0.0.M1/reference/html)|[API Doc.](https://docs.spring.io/spring-data/jdbc/old-docs/2.0.0.M1/api/)|
|2.0.0 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-data/jdbc/old-docs/2.0.0.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-data/jdbc/old-docs/2.0.0.BUILD-SNAPSHOT/api/)|



# 示例

尝试一些例子：

[**Spring Data book examples**](https://github.com/SpringSource/spring-data-book)







