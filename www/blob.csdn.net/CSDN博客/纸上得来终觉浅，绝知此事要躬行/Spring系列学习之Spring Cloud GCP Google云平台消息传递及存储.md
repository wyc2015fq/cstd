# Spring系列学习之Spring Cloud GCP Google云平台消息传递及存储 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 15:16:41[boonya](https://me.csdn.net/boonya)阅读数：208








英文原文：[https://spring.io/projects/spring-cloud-gcp](https://spring.io/projects/spring-cloud-gcp)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[入门](#%E5%85%A5%E9%97%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud GCP项目旨在使Spring Framework成为Google Cloud Platform（GCP）的一流公民。


目前，Spring Cloud GCP可以让您利用Spring框架的强大功能和简单性：
-     发布和订阅Google Cloud Pub / Sub主题
-     使用一些属性配置Spring JDBC以使用Google Cloud SQL
-     从Google Cloud Storage备份的Spring Resources写入和读取
-     使用Google Cloud Pub / Sub在后台与Spring Integration交换消息
-     使用Spring Cloud Sleuth和Google Stackdriver Trace跟踪应用程序的执行情况
-     使用由Google Runtime Configuration API备份的Spring Cloud Config配置您的应用程序
-     通过Spring Integration GCS渠道适配器消费和生成Google云端存储数据

## 特性

Spring Cloud GCP项目提供以下功能：
-     用于依赖版本管理的物料清单（BOM）
-     Spring Cloud GCP Pub / Sub，包括PubSubTemplate
-     Google云端存储的Spring资源抽象
-     适用于Google Cloud Pub / Sub和Google Cloud Storage的Spring集成渠道适配器
-     Pub / Sub的Spring Cloud Stream Binder
-     Spring Data Spanner（BETA）
-     Spring Boot启动器
-     GCP支持
-     Google Cloud Config（BETA）
-     Google Cloud Spanner（BETA）
-     Google Cloud Logging
-     Google Cloud Pub / Sub
-     Google Cloud SQL
-     Google云端存储
-     与Spring云侦探的Stackdriver踪影

## 入门

有许多可用的资源可以帮助您尽快掌握我们的库。


Spring Initializr中有三个Spring Cloud GCP条目：
-     GCP支持
-     GCP消息传递
-     GCP存储

GCP支持条目包含对每个Spring Cloud GCP集成的自动配置支持。大多数自动配置代码仅在其他依赖项添加到类路径时才启用。


GCP消息传递条目添加了GCP支持条目和所有必需的依赖项，以便Google Cloud Pub / Sub集成开箱即用。


GCP存储条目添加了GCP支持条目和所有必需的依赖项，以便Google云端存储集成开箱即用。


在代码质询中，您使用一个集成逐步执行任务。 [Google Developers Codelabs](https://codelabs.developers.google.com/spring/)页面提供了许多挑战。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.1.0 RC2 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gcp/1.1.0.RC2/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gcp/1.1.0.RC2/)|
|----|----|----|
|1.0.1 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-cloud-gcp/docs/1.0.1.BUILD-SNAPSHOT/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-gcp/docs/1.0.1.BUILD-SNAPSHOT/api/)|
|1.0.0 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-cloud-gcp/docs/1.0.0.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-gcp/docs/1.0.0.RELEASE/api/)|

# 示例

尝试一些例子：
- [**Spring Cloud GCP Code Samples**](https://github.com/spring-cloud/spring-cloud-gcp/tree/master/spring-cloud-gcp-samples)





