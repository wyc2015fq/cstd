# Spring系列学习之Spring Cloud Security微服务用户数据安全 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 16:25:36[boonya](https://me.csdn.net/boonya)阅读数：163








英文原文：[https://spring.io/projects/spring-cloud-security](https://spring.io/projects/spring-cloud-security)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[入门](#%E5%85%A5%E9%97%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Security提供了一组原语，用于构建安全的应用程序和服务，而且操作简便。可以在外部（或集中）进行大量配置的声明性模型有助于实现大型协作的远程组件系统，通常具有中央身份管理服务。它也非常易于在Cloud Foundry等服务平台中使用。在Spring Boot和Spring Security OAuth2的基础上，我们可以快速创建实现常见模式的系统，如单点登录，令牌中继和令牌交换。

## 特性

Spring Cloud Security功能：
-     从Zuul代理中的前端到后端服务中继SSO令牌
-     资源服务器之间的中继令牌
-     使Feign客户端的拦截器行为类似于OAuth2RestTemplate（获取令牌等）
-     在Zuul代理中配置下游身份验证

## 入门

如果您的应用程序还具有Spring Cloud Zuul嵌入式反向代理（使用@EnableZuulProxy），那么您可以要求它将OAuth2访问令牌下游转发到它所代理的服务。因此，上面的SSO应用程序可以简单地增强，如下所示：

```java
@SpringBootApplication
@EnableOAuth2Sso
@EnableZuulProxy
class Application {

}
```

它将（除了记录用户并获取令牌之外）将身份验证令牌下游传递给/proxy/* services。 如果使用@EnableResourceServer实现这些服务，那么它们将在正确的标头中获得有效的标记。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-security/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-security/2.1.0.RC3/)|
|----|----|----|
|2.0.2 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-security/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-security/2.0.x/)|
|2.0.1 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-security/2.0.1.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-security/2.0.1.RELEASE/)|
|1.2.4 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-security/1.2.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-security/1.2.x/)|
|1.2.3 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-security/1.2.3.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-security/1.2.3.RELEASE/)|

# 示例

尝试一些例子：
- [**SSO**](https://github.com/spring-cloud-samples/sso)
- [**Auth Server**](https://github.com/spring-cloud-samples/authserver)
- [**SSO Groovy**](https://github.com/spring-cloud-samples/scripts/blob/master/demo/sso.groovy)
- [**Resource server**](https://github.com/spring-cloud-samples/scripts/blob/master/demo/resource.groovy)



