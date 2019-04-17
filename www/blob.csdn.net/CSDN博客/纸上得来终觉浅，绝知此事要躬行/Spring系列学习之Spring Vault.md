# Spring系列学习之Spring Vault - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 22:50:59[boonya](https://me.csdn.net/boonya)阅读数：421标签：[Spring																[Vault](https://so.csdn.net/so/search/s.do?q=Vault&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)








英文原文：[https://spring.io/projects/spring-vault](https://spring.io/projects/spring-vault)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Vault提供熟悉的Spring抽象和客户端支持，用于访问，存储和撤销机密。 它提供了与Vault交互的低级和高级抽象，使用户免于基础设施问题。


通过[HashiCorp的Vault](https://www.vaultproject.io/)，您可以在所有环境中为应用程序管理外部机密数据。 Vault可以管理静态和动态机密，例如应用程序数据，远程应用程序/资源的用户名/密码，并为外部服务提供凭据，如MySQL，PostgreSQL，Apache Cassandra，Consul，AWS等。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.1 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-vault/docs/2.1.1.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-vault/docs/2.1.1.RELEASE/api/)|
|----|----|----|
|2.2.0 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-vault#learn)|[API Doc.](https://spring.io/projects/spring-vault#learn)|
|2.1.2 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-vault#learn)|[API Doc.](https://spring.io/projects/spring-vault#learn)|
|2.0.3 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-vault#learn)|[API Doc.](https://spring.io/projects/spring-vault#learn)|
|2.0.2 GA|[Reference Doc.](https://docs.spring.io/spring-vault/docs/2.0.2.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-vault/docs/2.0.2.RELEASE/api/)|
|1.1.3 SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-vault#learn)|[API Doc.](https://spring.io/projects/spring-vault#learn)|
|1.1.2 GA|[Reference Doc.](https://docs.spring.io/spring-vault/docs/1.1.2.RELEASE/reference/html/)|[API Doc.](https://docs.spring.io/spring-vault/docs/1.1.2.RELEASE/api/)|

# 示例

尝试一些例子：
- [**Samples**](https://github.com/mp911de/spring-cloud-vault-config-samples)Spring Vault和Spring Cloud Vault示例
- [**Guide: Retrieve sensitive configuration from Vault**](https://spring.io/guides/gs/vault-config/)本指南将指导您完成使用Spring Cloud Vault构建从HashiCorp Vault检索其配置属性的应用程序的过程。
- [**Accessing Vault**](https://spring.io/guides/gs/accessing-vault/)本指南将引导您完成使用Spring Vault构建从HashiCorp Vault（秘密管理工具）加载机密的应用程序的过程。






