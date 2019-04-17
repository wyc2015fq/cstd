# Spring系列学习之Spring Data R2DBC数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 21:01:50[boonya](https://me.csdn.net/boonya)阅读数：445标签：[Spring																[Data																[R2DBC](https://so.csdn.net/so/search/s.do?q=R2DBC&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-data-r2dbc](https://spring.io/projects/spring-data-r2dbc)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Data R2DBC是更大的Spring Data系列的一部分，可以轻松实现基于R2DBC的存储库。 R2DBC代表Reactive Relational Database Connectivity，它是一个使用反应式驱动程序集成关系数据库的孵化器。 Spring Data R2DBC为R2DBC应用熟悉的Spring抽象和存储库支持。 它可以更轻松地构建在响应式应用程序堆栈中使用关系数据访问技术的Spring驱动的应用程序。


Spring Data R2DBC旨在简化概念。 为了实现这一点，它不提供缓存，延迟加载，后写或ORM框架的许多其他功能。 这使得Spring Data R2DBC成为一个简单，有限，自以为是的对象映射器。


Spring Data R2DBC允许功能方法与您的数据库交互，提供DatabaseClient作为应用程序的入口点。


通过选择数据库驱动程序并创建DatabaseClient实例开始：
- 
[Postgres](https://github.com/r2dbc/r2dbc-postgresql) (`io.r2dbc:r2dbc-postgresql`)

- 
[H2](https://github.com/r2dbc/r2dbc-h2) (`io.r2dbc:r2dbc-h2`)

- 
[Microsoft SQL Server](https://github.com/r2dbc/r2dbc-mssql) (`io.r2dbc:r2dbc-mssql`)


PostgreSQL示例：

```java
PostgresqlConnectionFactory connectionFactory = new PostgresqlConnectionFactory(PostgresqlConnectionConfiguration.builder()
		.host(…)
		.database(…)
		.username(…)
		.password(…).build());

DatabaseClient client = DatabaseClient.create(connectionFactory);

Mono<Integer> affectedRows = client.execute()
        .sql("UPDATE person SET name = 'Joe'")
        .fetch().rowsUpdated();

Flux<Person> all = client.execute()
        .sql("SELECT id, name FROM person")
        .as(Person.class)
        .fetch().all();
```

客户端API提供以下功能：
-      执行通用SQL并消耗更新计数/行结果。
-      具有分页和排序的通用SELECT。
- SELECT具有分页和排序的映射对象。
-      带参数绑定的通用INSERT。
- INSERT映射对象。
-      使用本机语法进行参数绑定。
-      结果消耗：更新计数，未映射（Map <String，Object>），映射到实体，提取功能。
-      使用@Query注释方法的反应式存储库。
-      事务管理。

## 快速开始



使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。 

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.0.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/r2dbc/docs/1.0.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/r2dbc/docs/1.0.0.M1/api)|
|----|----|----|
|1.0.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-r2dbc#learn)|[API Doc.](https://spring.io/projects/spring-data-r2dbc#learn)|

# 示例

尝试一些例子：
- [**Spring Data R2DBC Examples**](https://github.com/spring-projects/spring-data-examples/tree/master/r2dbc/example)](https://so.csdn.net/so/search/s.do?q=Data&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




