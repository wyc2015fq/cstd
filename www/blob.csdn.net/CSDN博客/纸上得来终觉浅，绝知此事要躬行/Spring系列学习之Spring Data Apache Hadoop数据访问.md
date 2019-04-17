# Spring系列学习之Spring Data Apache Hadoop数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 00:12:25[boonya](https://me.csdn.net/boonya)阅读数：858








英文原文：[https://spring.io/projects/spring-hadoop](https://spring.io/projects/spring-hadoop)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[介绍](#%E4%BB%8B%E7%BB%8D)

[特性](#%E7%89%B9%E6%80%A7)

[版本和分发支持](#%E7%89%88%E6%9C%AC%E5%92%8C%E5%88%86%E5%8F%91%E6%94%AF%E6%8C%81)

[Spring Boot 配置](#Spring%20Boot%20%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

**注意：Spring for Apache Hadoop项目将于2019年4月5日达到生命周期终止状态。我们将根据需要发布偶尔的2.5.x维护版本，直到那时为止，然后将项目移至阁楼。 当前的Apache Hadoop 2.5.0版本是使用Apache Hadoop 2.7.3版本构建的，应该与最流行的Hadoop发行版的最新版本兼容。**

## 介绍

Spring for Apache Hadoop通过提供统一的配置模型和易于使用的API来简化Apache Hadoop的开发，以便使用HDFS，MapReduce，Pig和Hive。它还提供与其他Spring生态系统项目（如Spring Integration和Spring Batch）的集成，使您能够为大数据摄取/导出和Hadoop工作流程编排开发解决方案。


查看O'Reilly Media Spring Data：Modern Java Access for Enterprise Java中的书，其中包含有关使用Spring for Apache Hadoop的几个章节。该书的示例代码也可以在GitHub项目spring-data-book中找到。

## 特性
-     支持创建使用依赖注入配置的Hadoop应用程序，并作为标准Java应用程序运行，而不是使用Hadoop命令行实用程序。
-     与Spring Boot集成，可以简单地创建连接到HDFS以读取和写入数据的Spring应用程序。
-     创建和配置使用Java MapReduce，Streaming，Hive，Pig或HBase的应用程序
-     Spring Batch的扩展，支持为任何类型的Hadoop作业或HDFS操作创建基于Hadoop的工作流。
-     使用任何基于JVM的脚本语言编写HDFS操作脚本。
-     轻松创建基于Spring Boot的自定义应用程序，可以部署在YARN上执行。
-     HBase的DAO支持（模板和回调）。
-     支持Hadoop安全性。

## 版本和分发支持

Spring for Apache Hadoop支持许多Apache版本以及Pivotal，Hortonworks和Cloudera的商业发行版。


受支持的发行版因发行版本而异，有关详细信息，请参阅[Wiki页面](https://github.com/spring-projects/spring-hadoop/wiki#supported-distributions)。


另外，请参阅Mavi构建详细信息的[Wiki页面](https://github.com/spring-projects/spring-hadoop/wiki#supported-distributions)。


可以在构建页面上看到大多数受支持版本的持续集成构建。

## Spring Boot 配置

```
<dependencies>
    <dependency>
        <groupId>org.springframework.data</groupId>
        <artifactId>spring-data-hadoop</artifactId>
        <version>2.5.0.RELEASE</version>
    </dependency>
</dependencies>
```

## 快速开始



使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.5.0 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-hadoop/docs/2.5.0.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-hadoop/docs/2.5.0.RELEASE/api/)|
|----|----|----|
|2.5.1 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-hadoop/docs/2.5.1.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-hadoop/docs/2.5.1.BUILD-SNAPSHOT/api/)|

# 示例

尝试一些示例：
- [**Spring for Hadoop Samples**](https://github.com/spring-projects/spring-hadoop-samples)



