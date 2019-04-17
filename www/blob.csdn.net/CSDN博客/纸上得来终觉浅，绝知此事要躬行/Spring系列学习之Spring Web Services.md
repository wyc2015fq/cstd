# Spring系列学习之Spring Web Services - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 23:11:40[boonya](https://me.csdn.net/boonya)阅读数：264标签：[Spring																[Web Services](https://so.csdn.net/so/search/s.do?q=Web Services&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)








英文原文：[https://spring.io/projects/spring-ws](https://spring.io/projects/spring-ws)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Spring Web Services（Spring-WS）是Spring社区的一个产品，专注于创建文档驱动的Web服务。 Spring Web Services旨在促进契约优先的SOAP服务开发，允许使用多种方式之一来操作XML有效负载来创建灵活的Web服务。该产品基于Spring本身，这意味着您可以使用Spring概念（如依赖注入）作为Web服务的组成部分。


人们使用Spring-WS有很多原因，但大多数人在找到替代Web服务最佳实践后缺乏的替代SOAP堆栈时会被它吸引。 Spring-WS使最佳实践变得简单。这包括诸如WS-I基本配置文件，契约优先开发以及合同与实现之间松散耦合等实践。 Spring Web服务的其他主要功能包括：

## 特性
-     使最佳实践成为一种简单的实践：Spring Web Services使执行最佳实践变得更加容易。这包括诸如WS-I基本配置文件，契约优先开发以及合同与实现之间松散耦合等实践。
-     强大的映射：您可以将传入的XML请求分发到任何对象，具体取决于消息有效内容，SOAP Action标头或XPath表达式。
-     XML API支持：传入的XML消息可以在标准的JAXP API（如DOM，SAX和StAX）中处理，也可以在JDOM，dom4j，XOM甚至编组技术中处理。
-     灵活的XML编组：Spring Web Services发行版中的Object / XML Mapping模块支持JAXB 1和2，Castor，XMLBeans，JiBX和XStream。因为它是一个单独的模块，所以您也可以在非Web服务代码中使用它。
-     重用您的Spring专业知识：Spring-WS使用Spring应用程序上下文进行所有配置，这应该有助于Spring开发人员快速，快速地获得最佳速度。此外，Spring-WS的体系结构类似于Spring-MVC的体系结构。
-     支持WS-Security：WS-Security允​​许您签署SOAP消息，加密和解密它们，或对它们进行身份验证。
-     与Acegi Security集成：Spring Web Services的WS-Security实现提供与Spring Security的集成。这意味着您也可以将现有配置用于SOAP服务。
-     由Maven构建：这有助于您在自己的基于Maven的项目中有效地重用Spring Web Services工件。
-     Apache许可证。您可以放心地在项目中使用Spring-WS。

## Spring Boot配置



Spring Boot会自动：
-     在servlet容器中配置MessageDispatcherServlet
-     扫描所有.wsdl和.xsd文档以获取WSDL和模式定义的bean

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|3.0.4 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-ws/docs/3.0.4.RELEASE/reference)|[API Doc.](https://docs.spring.io/spring-ws/docs/3.0.4.RELEASE/api/)|
|----|----|----|
|3.0.5 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-ws/docs/3.0.5.BUILD-SNAPSHOT/reference)|[API Doc.](https://docs.spring.io/spring-ws/docs/3.0.5.BUILD-SNAPSHOT/api/)|
|2.4.4 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-ws/docs/2.4.4.BUILD-SNAPSHOT/reference)|[API Doc.](https://docs.spring.io/spring-ws/docs/2.4.4.BUILD-SNAPSHOT/api/)|
|2.4.3 GA|[Reference Doc.](https://docs.spring.io/spring-ws/docs/2.4.3.RELEASE/reference)|[API Doc.](https://docs.spring.io/spring-ws/docs/2.4.3.RELEASE/api/)|




