# Spring系列学习之Spring Data JPA数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 20:19:59[boonya](https://me.csdn.net/boonya)阅读数：80








英文原文：[https://spring.io/projects/spring-data-jpa](https://spring.io/projects/spring-data-jpa)



# 概述

Spring Data JPA是更大的Spring Data系列的一部分，可以轻松实现基于JPA的存储库。此模块处理对基于JPA的数据访问层的增强支持。它使构建使用数据访问技术的Spring驱动应用程序变得更加容易。


在相当长的一段时间内，实现应用程序的数据访问层一直很麻烦。必须编写太多样板代码来执行简单查询以及执行分页和审计。 Spring Data JPA旨在通过减少实际需要的工作量来显着改善数据访问层的实现。作为开发人员，您编写存储库接口，包括自定义查找器方法，Spring将自动提供实现。

## 特性
-     基于Spring和JPA构建存储库的复杂支持
-     支持[Querydsl](http://www.querydsl.com/)谓词，从而支持类型安全的JPA查询
-     透明审核域类
-     分页支持，动态查询执行，集成自定义数据访问代码的能力
-     在引导时验证@Query带注释的查询
-     支持基于XML的实体映射
-     基于JavaConfig的存储库配置，介绍@EnableJpaRepositories。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-data/jpa/docs/2.1.3.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/jpa/docs/2.1.3.RELEASE/api)|
|----|----|----|
|2.2.0 M1 PRE|[Reference Doc.](https://docs.spring.io/spring-data/jpa/docs/2.2.0.M1/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/jpa/docs/2.2.0.M1/api)|
|2.2.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-jpa#learn)|[API Doc.](https://spring.io/projects/spring-data-jpa#learn)|
|2.1.4 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-jpa#learn)|[API Doc.](https://spring.io/projects/spring-data-jpa#learn)|
|2.0.13 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-jpa#learn)|[API Doc.](https://spring.io/projects/spring-data-jpa#learn)|
|2.0.12 GA|[Reference Doc.](https://docs.spring.io/spring-data/jpa/docs/2.0.12.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/jpa/docs/2.0.12.RELEASE/api)|
|1.11.18 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-data-jpa#learn)|[API Doc.](https://spring.io/projects/spring-data-jpa#learn)|
|1.11.17 GA|[Reference Doc.](https://docs.spring.io/spring-data/jpa/docs/1.11.17.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-data/jpa/docs/1.11.17.RELEASE/api)|

## 指南

该指南旨在在15-30分钟内完成，提供快速，实用的说明，用于为Spring的任何开发任务构建入门应用程序。
- [**Accessing Data with JPA**](https://spring.io/guides/gs/accessing-data-jpa)了解如何使用Spring Data JPA处理JPA数据持久性。

# 示例

尝试一些例子：
- [**Spring Data JPA Examples **](https://github.com/spring-projects/spring-data-examples/tree/master/jpa)
- [**Spring Data Book Examples**](https://github.com/spring-projects/spring-data-book/tree/master/jpa)



