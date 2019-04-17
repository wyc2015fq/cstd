# Spring系列学习之Spring Cloud Skipper发现应用程序并管理其生命周期 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 16:35:15[boonya](https://me.csdn.net/boonya)阅读数：280








英文原文：[https://cloud.spring.io/spring-cloud-skipper/](https://cloud.spring.io/spring-cloud-skipper/)

**目录**

[Spring Cloud Skipper](#Spring%20Cloud%20Skipper)

[概览](#%E6%A6%82%E8%A7%88)

[特性](#%E7%89%B9%E6%80%A7)

[入门](#%E5%85%A5%E9%97%A8)

[历史](#%E5%8E%86%E5%8F%B2)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[相关项目](#%E7%9B%B8%E5%85%B3%E9%A1%B9%E7%9B%AE)

## Spring Cloud Skipper

Skipper是一种工具，允许您在多个云平台上发现应用程序并管理其生命周期。

## 概览

Skipper是一个工具，允许您发现Spring Boot应用程序并管理其在多个云平台上的生命周期。您可以单独使用Skipper或将其与Continuous Integration管道集成，以帮助实现应用程序的持续部署。


系统设计受到Kubernetes生态系统中执行资源模板和/或编排的大量项目的影响，因此受到航海启发的项目名称为Skipper。


Skipper中的应用程序捆绑为包，其中包含模板化配置文件和用于填充模板的默认值集。您可以在安装或升级软件包时覆盖这些默认值。 Skipper提供了一种方法来协调不同版本之间的应用程序的升级/回滚过程，采取最小的操作集使系统进入所需状态。

## 特性

Skipper的主要特点是：
-     定义可以部署Spring Boot应用程序的多个平台帐户。支持的平台包括Local，Cloud Foundry和Kubernetes。
-     Mustache模板化文件中的替换变量，描述如何将应用程序部署到平台。
-     搜索现有应用程序的包存储库。
-     基于简单的工作流程安装/升级/回滚软件包。
-     存储已解析模板文件（也称为“应用程序清单”）的历史记录，这些文件代表已部署到特定版本的平台的最终描述。
-     通过独立的交互式shell或通过Web API使用来补充CI工作流。


[Spring Cloud Deployer实现](https://github.com/spring-cloud/spring-cloud-deployer#implementations)提供了将Spring Boot应用程序部署到多个平台的方法。支持的模板文件格式基于YAML，并描述创建Spring Cloud Deployer [AppDeploymentRequest](https://github.com/spring-cloud/spring-cloud-deployer/blob/master/spring-cloud-deployer-spi/src/main/java/org/springframework/cloud/deployer/spi/core/AppDeploymentRequest.java)所需的变量集

## 入门


[参考指南](https://docs.spring.io/spring-cloud-skipper/docs/current/reference/htmlsingle/#getting-started-installing-skipper)是入门的最佳位置。

## 历史



Skipper的演变是出于在Spring Cloud Data Flow中执行“Stream Edits”的需要。后来人们认识到，为了提供该功能，应该创建一个更通用的项目Skipper，以便它也可以成为Spring Cloud Data Flow环境之外的有用工具包。

## 发布版本

Spring Cloud Skipper

Release

Documentation

2.0.0 M1

[Reference](http://docs.spring.io/spring-cloud-skipper/docs/2.0.0.M1/reference/htmlsingle/#getting-started)[API](http://docs.spring.io/spring-cloud-skipper/docs/2.0.0.M1/api/)

2.0.0

[Reference](http://docs.spring.io/spring-cloud-skipper/docs/2.0.0.BUILD-SNAPSHOT/reference/htmlsingle/#getting-started)[API](http://docs.spring.io/spring-cloud-skipper/docs/2.0.0.BUILD-SNAPSHOT/api/)

1.1.2

[Reference](http://docs.spring.io/spring-cloud-skipper/docs/1.1.2.RELEASE/reference/htmlsingle/#getting-started)[API](http://docs.spring.io/spring-cloud-skipper/docs/1.1.2.RELEASE/api/)

1.1.1

[Reference](http://docs.spring.io/spring-cloud-skipper/docs/1.1.1.BUILD-SNAPSHOT/reference/htmlsingle/#getting-started)[API](http://docs.spring.io/spring-cloud-skipper/docs/1.1.1.BUILD-SNAPSHOT/api/)

1.0.10

[Reference](http://docs.spring.io/spring-cloud-skipper/docs/1.0.10.BUILD-SNAPSHOT/reference/htmlsingle/#getting-started)[API](http://docs.spring.io/spring-cloud-skipper/docs/1.0.10.BUILD-SNAPSHOT/api/)

## 相关项目
- [Spring Cloud Data Flow](https://cloud.spring.io/spring-cloud-dataflow/)
- [Spring Statemachine](https://cloud.spring.io/spring-statemachine/)





