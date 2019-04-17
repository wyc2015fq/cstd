# Spring系列学习之Spring Batch - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 14:54:33[boonya](https://me.csdn.net/boonya)阅读数：24标签：[Spring																[Batch](https://so.csdn.net/so/search/s.do?q=Batch&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)








英文原文：[https://spring.io/projects/spring-batch](https://spring.io/projects/spring-batch)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[指南](#%E6%8C%87%E5%8D%97)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

轻量级，全面的批处理框架，旨在开发对企业系统日常运营至关重要的强大批处理应用程序。

Spring Batch提供了可重复使用的功能，这些功能对于处理大量记录至关重要，包括记录/跟踪，事务管理，作业处理统计，作业重启，跳过和资源管理。它还提供更高级的技术服务和功能，通过优化和分区技术实现极高容量和高性能的批处理作业。简单和复杂的大批量批处理作业可以高度可扩展的方式利用框架来处理大量信息。

## 特性
-     事务管理
-     基于块的处理
-     声明性I/O.
-     启动/停止/重新启动
-     重试/跳过
-     基于Web的管理界面（[Spring Cloud Data Flow](https://cloud.spring.io/spring-cloud-dataflow)）

## Spring Boot配置

Spring Boot提供了spring-boot-starter-batch依赖项。查看[批处理应用程序的专用参考文档](https://docs.spring.io/spring-boot/docs/current/reference/html/howto-batch-applications.html)部分。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|4.1.0 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-batch/4.1.x/reference/html/index.html)|[API Doc.](https://docs.spring.io/spring-batch/4.1.x/api/index.html)|
|----|----|----|
|4.0.1 GA|[Reference Doc.](https://docs.spring.io/spring-batch/4.0.x/reference/html/index.html)|[API Doc.](https://docs.spring.io/spring-batch/4.0.x/api/index.html)|
|3.0.9 GA|[Reference Doc.](https://docs.spring.io/spring-batch/3.0.x/reference/html/index.html)|[API Doc.](https://docs.spring.io/spring-batch/3.0.x/apidocs/index.html)|

## 指南

该指南旨在在15-30分钟内完成，提供快速，实用的说明，用于为Spring的任何开发任务构建入门应用程序。
- [**Creating a Batch Service**](https://spring.io/guides/gs/batch-processing)了解如何创建基本的批处理驱动解决方案。

# 示例

尝试一些示例：
- [**20+ Spring Batch samples**](https://github.com/spring-projects/spring-batch/tree/master/spring-batch-samples)






