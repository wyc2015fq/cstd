# Spring系列学习之Spring Boot - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 00:54:33[boonya](https://me.csdn.net/boonya)阅读数：604标签：[Spring																[Boot](https://so.csdn.net/so/search/s.do?q=Boot&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)








英文原文：[https://spring.io/projects/spring-boot](https://spring.io/projects/spring-boot)

**目录**

[概览](#%E6%A6%82%E8%A7%88)

[特性](#%E7%89%B9%E6%80%A7)

[谈论和录像](#%E8%B0%88%E8%AE%BA%E5%92%8C%E5%BD%95%E5%83%8F)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[指南](#%E6%8C%87%E5%8D%97)

[示例](#%E7%A4%BA%E4%BE%8B)

[学习例子](#%E5%AD%A6%E4%B9%A0%E4%BE%8B%E5%AD%90)

[IDE创建项目](#IDE%E5%88%9B%E5%BB%BA%E9%A1%B9%E7%9B%AE)

# 概览

Spring Boot可以轻松创建独立的，生产级的基于Spring的应用程序，您可以“运行”。


我们对Spring平台和第三方库进行了常用性的组合，因此您可以轻松上手。 大多数Spring Boot应用程序只需要很少的Spring配置。

## 特性
-      创建独立的Spring应用程序
-      直接嵌入Tomcat，Jetty或Undertow（无需部署WAR文件）
-      提供自以为是的“入门”依赖项以简化构建配置
-      尽可能自动配置Spring和第三方库
-      提供生产就绪功能，例如指标，运行状况检查和外部化配置
-      绝对没有代码生成，也不需要XML配置 

你也可以加入[Spring Boot的Gitter社区](https://gitter.im/spring-projects/spring-boot)

## 谈论和录像
- [这是一种魔术：在Spring Boot的封面下](https://content.pivotal.io/springone-platform-2017/its-a-kind-of-magic-under-the-covers-of-spring-boot-brian-clozel-st%C3%A9phane-nicoll)
- [Spring Boot 2.0有什么新功能](https://content.pivotal.io/springone-platform-2017/whats-new-in-spring-boot-2-0-phillip-webb-madhura-bhave)
- [介绍Spring Boot 2.0网络研讨会](https://content.pivotal.io/webinars/mar-13-introducing-spring-boot-2-0-webinar)
- [使用Spring Boot测试驱动开发](https://content.pivotal.io/springone-platform-2017/test-driven-development-with-spring-boot-sannidhi-jalukar-madhura-bhave)
- [使用Spring Boot 2.0从Zero到Hero](https://content.pivotal.io/springone-platform-2017/from-zero-to-hero-with-spring-boot-brian-clozel)

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的文档， 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.1 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-boot/docs/2.1.1.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-boot/docs/2.1.1.RELEASE/api/)|
|----|----|----|
|2.2.0 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-boot/docs/2.2.0.BUILD-SNAPSHOT/reference/html/)|[API Doc.](https://docs.spring.io/spring-boot/docs/2.2.0.BUILD-SNAPSHOT/api/)|
|2.1.2 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-boot/docs/2.1.2.BUILD-SNAPSHOT/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-boot/docs/2.1.2.BUILD-SNAPSHOT/api/)|
|2.0.8 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-boot/docs/2.0.8.BUILD-SNAPSHOT/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-boot/docs/2.0.8.BUILD-SNAPSHOT/api/)|
|2.0.7 GA|[Reference Doc.](https://docs.spring.io/spring-boot/docs/2.0.7.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-boot/docs/2.0.7.RELEASE/api/)|
|1.5.19 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-boot/docs/1.5.19.BUILD-SNAPSHOT/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-boot/docs/1.5.19.BUILD-SNAPSHOT/api/)|
|1.5.18 GA|[Reference Doc.](https://docs.spring.io/spring-boot/docs/1.5.18.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-boot/docs/1.5.18.RELEASE/api/)|

## 指南



该指南旨在在15-30分钟内完成，提供快速，实用的说明，用于为Spring的任何开发任务构建入门应用程序。
- [**Building a RESTful Web Service**](https://spring.io/guides/gs/rest-service)了解如何使用Spring创建RESTful Web服务
- [**Spring Boot Docker**](https://spring.io/guides/gs/spring-boot-docker)使用Docker的主题指南以及如何为Spring Boot应用程序创建容器映像[](https://spring.io/guides/gs/spring-boot-docker)

# 示例

## 学习例子

尝试一些例子：
- [**Sagan**](https://github.com/spring-io/sagan)spring.io网站背后的Spring Boot应用程序
- [**Initializr**](https://github.com/spring-io/initializr)start.spring.io，由Spring Boot提供支持

## IDE创建项目

注：IntelliJ IDEA 可以通过Spring Initializr快速创建一个Spring Boot程序。

![](https://img-blog.csdnimg.cn/20181221005239365.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)




