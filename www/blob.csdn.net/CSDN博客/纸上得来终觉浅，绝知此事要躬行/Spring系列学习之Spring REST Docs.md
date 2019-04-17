# Spring系列学习之Spring REST Docs - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 14:41:07[boonya](https://me.csdn.net/boonya)阅读数：72








英文原文：[https://spring.io/projects/spring-restdocs](https://spring.io/projects/spring-restdocs)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring REST Docs可帮助您记录RESTful服务。


它结合了使用[Asciidoctor](http://asciidoctor.org/)编写的手写文档和使用[Spring MVC Test](https://docs.spring.io/spring/docs/current/spring-framework-reference/htmlsingle#spring-mvc-test-framework)生成的自动生成的片段。 这种方法使您免受[Swagger](http://swagger.io/)等工具生成的文档的限制。


它可以帮助您生成准确，简洁和结构良好的文档。 然后，该文档允许您的用户轻松获取所需信息。

## Spring Boot配置



Spring Boot提供@AutoConfigureRestDocs注释，以便在测试中利用[Spring REST Docs](https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-testing.html#boot-features-testing-spring-boot-applications-testing-autoconfigured-rest-docs)。

## 快速开始



使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.0.3 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-restdocs/docs/2.0.3.RELEASE/reference/html5/)|[API Doc.](https://docs.spring.io/spring-restdocs/docs/2.0.3.RELEASE/api/)|
|----|----|----|
|2.0.4 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-restdocs/docs/2.0.4.BUILD-SNAPSHOT/reference/html5/)|[API Doc.](https://docs.spring.io/spring-restdocs/docs/2.0.4.BUILD-SNAPSHOT/api/)|
|1.2.7 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-restdocs/docs/1.2.7.BUILD-SNAPSHOT/reference/html5/)|[API Doc.](https://docs.spring.io/spring-restdocs/docs/1.2.7.BUILD-SNAPSHOT/api/)|
|1.2.6 GA|[Reference Doc.](https://docs.spring.io/spring-restdocs/docs/1.2.6.RELEASE/reference/html5/)|[API Doc.](https://docs.spring.io/spring-restdocs/docs/1.2.6.RELEASE/api/)|



# 示例

尝试一些示例：
- [**Spring HATEOAS**](https://github.com/spring-projects/spring-restdocs/tree/master/samples/rest-notes-spring-hateoas)演示为使用Spring HATEOAS实现的服务创建入门指南和API指南
- [**Spring Data REST**](https://github.com/spring-projects/spring-restdocs/tree/master/samples/rest-notes-spring-data-rest)演示为使用Spring Data REST实现的服务创建入门指南和API指南
- [**Web Test Client**](https://github.com/spring-projects/spring-restdocs/tree/master/samples/web-test-client)演示Spring REST文档与Spring WebFlux的WebTestClient的使用
- [**Grails**](https://github.com/spring-projects/spring-restdocs/tree/master/samples/rest-notes-grails)演示如何使用Grails和Spock的Spring REST文档。
- [**Slate**](https://github.com/spring-projects/spring-restdocs/tree/master/samples/rest-notes-slate)演示使用Markdown和Slate的Spring REST Docs
- [**TestNG**](https://github.com/spring-projects/spring-restdocs/tree/master/samples/testng)演示Spring REST Docs与TestNG的使用
- [**JUnit 5**](https://github.com/spring-projects/spring-restdocs/tree/master/samples/junit5)演示使用JUnit 5的Spring REST Docs







