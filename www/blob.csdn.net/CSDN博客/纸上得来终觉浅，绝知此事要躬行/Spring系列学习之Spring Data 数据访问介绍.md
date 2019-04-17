# Spring系列学习之Spring Data 数据访问介绍 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 17:07:39[boonya](https://me.csdn.net/boonya)阅读数：137








英文原文：[https://spring.io/projects/spring-data](https://spring.io/projects/spring-data)

# 概述

Spring Data的使命是为数据访问提供熟悉且一致的基于Spring的编程模型，同时仍保留底层数据存储的特殊特性。

它使得使用数据访问技术，关系数据库和非关系数据库，map-reduce框架和基于云的数据服务变得容易。 这是一个伞形项目，其中包含许多特定于给定数据库的子项目。 这些项目是通过与这些令人兴奋的技术背后的许多公司和开发人员合作开发的。

## 特性
-     强大的存储库和自定义对象映射抽象
-      从存储库方法名称派生动态查询
-      实现域基类提供基本属性
-      支持透明审核（创建，最后更改）
-      可以集成自定义存储库代码
-      通过JavaConfig和自定义XML命名空间轻松实现Spring集成
-      与Spring MVC控制器的高级集成
-      跨存储持久性的实验支持

## 主要模块
- 
[Spring Data Commons](https://github.com/spring-projects/spring-data-commons) -Core Spring概念支撑每个Spring Data模块。

- 
[Spring Data JDBC](https://spring.io/projects/spring-data-jdbc) - Spring数据存储库支持JDBC。

- 
[Spring Data JDBC Ext](https://spring.io/projects/spring-data-jdbc-ext) - 支持对标准JDBC的数据库特定扩展，包括对Oracle RAC快速连接故障转移的支持，AQ JMS支持以及对使用高级数据类型的支持。

- 
[Spring Data JPA](https://spring.io/projects/spring-data-jpa) - Spring Data 对JPA的支持。

- 
[Spring Data KeyValue](https://github.com/spring-projects/spring-data-keyvalue) -基于映射的存储库和SPI可轻松构建用于键值存储的Spring Data模块。

- 
[Spring Data LDAP](https://spring.io/projects/spring-data-ldap) - Spring Data存储库支持[Spring LDAP](https://github.com/spring-projects/spring-ldap).

- 
[Spring Data MongoDB](https://spring.io/projects/spring-data-mongodb) - MongoDB的基于Spring的对象文档支持和存储库。

- 
[Spring Data Redis](https://spring.io/projects/spring-data-redis) -从Spring应用程序轻松配置和访问Redis。

- 
[Spring Data REST](https://spring.io/projects/spring-data-rest) - 将Spring Data存储库导出为超媒体驱动的RESTful资源。

- 
[Spring Data for Apache Cassandra](https://spring.io/projects/spring-data-cassandra) -轻松配置和访问Apache Cassandra或大规模，高可用性，面向数据的Spring应用程序。

- 
[Spring Data for Apace Geode](https://spring.io/projects/spring-data-geode) -轻松配置和访问Apache Geode，实现高度一致，低延迟，面向数据的Spring应用程序。

- 
[Spring Data for Apache Solr](https://spring.io/projects/spring-data-solr) - 为您的面向搜索的Spring应用程序轻松配置和访问Apache Solr。

- 
[Spring Data for Pivotal GemFire](https://spring.io/projects/spring-data-gemfire) -轻松配置和访问Pivotal GemFire，实现高度一致，低延迟/高吞吐量，面向数据的Spring应用程序。


## 社区模块
- 
[Spring Data Aerospike](https://github.com/aerospike/spring-data-aerospike) - 用于Aerospike的Spring Data模块。

- 
[Spring Data ArangoDB](https://github.com/arangodb/spring-data) - ArangoDB的Spring Data模块。

- 
[Spring Data Couchbase](https://spring.io/projects/spring-data-couchbase) - Couchbase的Spring Data模块。

- 
[Spring Data Azure Cosmos DB](https://github.com/Microsoft/spring-data-cosmosdb) -用于Microsoft Azure Cosmos DB的Spring Data模块。

- 
[Spring Data DynamoDB](https://github.com/derjust/spring-data-dynamodb) - DynamoDB的Spring数据模块。

- 
[Spring Data Elasticsearch](https://spring.io/projects/spring-data-elasticsearch) - Elasticsearch的Spring Data模块。

- 
[Spring Data Hazelcast](https://github.com/hazelcast/spring-data-hazelcast) - 为Hazelcast提供Spring Data存储库支持。

- 
[Spring Data Jest](https://github.com/VanRoy/spring-data-jest) - 基于Jest REST客户端的Elasticsearch的Spring Data模块。

- 
[Spring Data Neo4j](https://spring.io/projects/spring-data-neo4j) - Neo4j的基于Spring的对象图支持和存储库。

- 
[Spring Data Vault](https://spring.io/projects/spring-vault/) -基于[Spring Data KeyValue](https://github.com/spring-projects/spring-data-keyvalue)构建的Vault存储库。




## 相关模块
- Spring Data JDBC Extensions - 提供对Spring Framework中提供的JDBC支持的扩展。
- Spring for Apache Hadoop - 通过提供统一的配置模型和易于使用的API来简化Apache Hadoop，以便使用HDFS，MapReduce，Pig和Hive。
- Spring Content - 将内容与Spring Data Entities关联，并将其存储在许多不同的商店中，包括文件系统，S3，数据库或Mongo的GridFS。



## 孵化中的模块
- [Spring Data R2DBC](https://spring.io/projects/spring-data-r2dbc) - R2DBC的Spring Data支持。



## 发布培训




Spring Data是一个由独立项目组成的总体项目，原则上具有不同的发布节奏。 为了管理投资组合，发布了BOM（物料清单 - 参见此示例），其中包含一组针对单个项目的依赖关系。 发布列车有名称而不是版本，以避免与子项目混淆。


名称是一个字母序列（因此您可以按时间顺序排序）与着名计算机科学家和软件开发人员的名字。 当各个项目的点数累积到临界质量时，或者其中一个项目中存在一个需要每个人都可用的关键错误时，发布序列将推出名称以“-SRX”结尾的“服务版本”， 其中“X”是一个数字。


目前，版本系列包含以下模块：
- 
Spring Data Commons

- 
Spring Data JPA

- 
Spring Data KeyValue

- 
Spring Data LDAP

- 
Spring Data MongoDB

- 
Spring Data Redis

- 
Spring Data REST

- 
Spring Data for Apache Cassandra

- 
Spring Data for Apache Geode

- 
Spring Data for Apache Solr

- 
Spring Data for Pivotal GemFire

- 
Spring Data Couchbase (community module)

- 
Spring Data Elasticsearch (community module)

- 
Spring Data Neo4j (community module)




## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|Lovelace-SR3 CURRENT GA|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|
|----|----|----|
|Moore-M1 PRE|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|
|Moore-BUILD-SNAPSHOT SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|
|Lovelace-BUILD-SNAPSHOT SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|
|Kay-SR12 GA|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|
|Kay-BUILD-SNAPSHOT SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|
|Ingalls-SR17 GA|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|
|Ingalls-BUILD-SNAPSHOT SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data#learn)|[API Doc.](https://spring.io/projects/spring-data#learn)|





