# Spring系列学习之Spring Cloud OpenFeign声明性HTTP REST客户端 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 20:55:05[boonya](https://me.csdn.net/boonya)阅读数：290标签：[Spring																[Cloud																[OpenFeign](https://so.csdn.net/so/search/s.do?q=OpenFeign&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-cloud-openfeign](https://spring.io/projects/spring-cloud-openfeign)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[入门](#%E5%85%A5%E9%97%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

该项目通过自动配置和Spring环境以及其他Spring编程模型习惯用法为Spring Boot应用程序提供OpenFeign集成。

## 特性
-      声明性REST客户端：Feign创建使用JAX-RS或Spring MVC注释修饰的接口的动态实现

## 入门

```java
@SpringBootApplication
@EnableFeignClients
public class WebApplication {

	public static void main(String[] args) {
		SpringApplication.run(WebApplication.class, args);
	}

	@FeignClient("name")
	static interface NameService {
		@RequestMapping("/")
		public String getName();
	}
}
```

[https://github.com/spring-cloud-samples/feign-eureka/blob/master/client/src/main/java/demo/HelloClientApplication.java](https://github.com/spring-cloud-samples/feign-eureka/blob/master/client/src/main/java/demo/HelloClientApplication.java)

```java
package demo;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.cloud.netflix.feign.EnableFeignClients;
import org.springframework.cloud.netflix.feign.FeignClient;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import static org.springframework.web.bind.annotation.RequestMethod.GET;

/**
 * @author Spencer Gibb
 */
@SpringBootApplication
@EnableDiscoveryClient
@RestController
@EnableFeignClients
public class HelloClientApplication {
	@Autowired
	HelloClient client;

	@RequestMapping("/")
	public String hello() {
		return client.hello();
	}

	public static void main(String[] args) {
		SpringApplication.run(HelloClientApplication.class, args);
	}

	@FeignClient("HelloServer")
	interface HelloClient {
		@RequestMapping(value = "/", method = GET)
		String hello();
	}
}
```



## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-openfeign/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-openfeign/2.1.0.RC3/)|
|----|----|----|
|2.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-openfeign/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-openfeign/2.0.x/)|
|2.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-openfeign/2.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-openfeign/2.0.2.RELEASE/)|

# 示例

尝试一些例子：
- [**Feign Using Eureka**](https://github.com/spring-cloud-samples/feign-eureka)](https://so.csdn.net/so/search/s.do?q=Cloud&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




