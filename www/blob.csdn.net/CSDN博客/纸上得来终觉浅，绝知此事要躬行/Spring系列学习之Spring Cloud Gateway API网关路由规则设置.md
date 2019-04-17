# Spring系列学习之Spring Cloud Gateway API网关路由规则设置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 14:22:26[boonya](https://me.csdn.net/boonya)阅读数：194








英文原文：[https://spring.io/projects/spring-cloud-gateway](https://spring.io/projects/spring-cloud-gateway)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[入门](#%E5%85%A5%E9%97%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[指南](#%E6%8C%87%E5%8D%97)

[示例](#%C2%A0%E7%A4%BA%E4%BE%8B)

# 概述



该项目提供了一个用于在Spring MVC之上构建API网关的库。 Spring Cloud Gateway旨在提供一种简单而有效的方式来路由到API，并为他们提供横切关注点，例如：安全性，监控/指标和弹性。

## 特性

Spring Cloud Gateway功能：
-      基于Spring Framework 5，Project Reactor和Spring Boot 2.0构建
-      能够匹配任何请求属性上的路由。
-      谓词和过滤器特定于路线。
-      Hystrix断路器集成。
-      Spring Cloud DiscoveryClient集成
-      易于编写谓词和过滤器
-      请求率限制
-      路径重写

## 入门

```java
@SpringBootApplication
public class DemogatewayApplication {
	@Bean
	public RouteLocator customRouteLocator(RouteLocatorBuilder builder) {
		return builder.routes()
			.route("path_route", r -> r.path("/get")
				.uri("http://httpbin.org"))
			.route("host_route", r -> r.host("*.myhost.org")
				.uri("http://httpbin.org"))
			.route("rewrite_route", r -> r.host("*.rewrite.org")
				.filters(f -> f.rewritePath("/foo/(?<segment>.*)", "/${segment}"))
				.uri("http://httpbin.org"))
			.route("hystrix_route", r -> r.host("*.hystrix.org")
				.filters(f -> f.hystrix(c -> c.setName("slowcmd")))
				.uri("http://httpbin.org"))
			.route("hystrix_fallback_route", r -> r.host("*.hystrixfallback.org")
				.filters(f -> f.hystrix(c -> c.setName("slowcmd").setFallbackUri("forward:/hystrixfallback")))
				.uri("http://httpbin.org"))
			.route("limit_route", r -> r
				.host("*.limited.org").and().path("/anything/**")
				.filters(f -> f.requestRateLimiter(c -> c.setRateLimiter(redisRateLimiter())))
				.uri("http://httpbin.org"))
			.build();
	}
}
```

要运行您自己的网关，请使用spring-cloud-starter-gateway依赖项。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gateway/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gateway/2.1.0.RC3/)|
|----|----|----|
|2.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-gateway/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-gateway/2.0.x/)|
|2.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gateway/2.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gateway/2.0.2.RELEASE/)|
|1.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-gateway/1.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-gateway/1.0.x/)|
|1.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gateway/1.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-gateway/1.0.2.RELEASE/)|

## 指南

该指南旨在在15-30分钟内完成，提供快速，实用的说明，用于为Spring的任何开发任务构建入门应用程序。
- [**Building a Gateway**](https://spring.io/guides/gs/gateway)了解如何配置网关

#  示例

尝试一些例子：
- [**Spring Cloud Gateway Sample**](https://github.com/spring-cloud/spring-cloud-gateway/tree/master/spring-cloud-gateway-sample)样本包含在项目中并在每次CI运行期间进行测试。
- [**Spring Cloud Samples Gateway**](https://github.com/spring-cloud-samples/spring-cloud-gateway-sample)外部样本显示更复杂的过滤器和谓词用法。





