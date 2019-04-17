# Spring系列学习之Spring Cloud微服务相关项目总览 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 15:13:02[boonya](https://me.csdn.net/boonya)阅读数：199








英文原文：[https://spring.io/projects/spring-cloud](https://spring.io/projects/spring-cloud)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[主要项目](#%E4%B8%BB%E8%A6%81%E9%A1%B9%E7%9B%AE)

[Spring Cloud Config](#a-href-http-cloud-spring-io-spring-cloud-config-spring-cloud-config-a)

[Spring Cloud Netflix](#a-href-http-cloud-spring-io-spring-cloud-netflix-spring-cloud-netflix-a)

[Spring Cloud Bus](#a-href-http-cloud-spring-io-spring-cloud-bus-spring-cloud-bus-a)

[Spring Cloud Cloudfoundry](#a-href-http-cloud-spring-io-spring-cloud-cloudfoundry-spring-cloud-cloudfoundry-a)

[Spring Cloud Open Service Broker](#a-href-http-cloud-spring-io-spring-cloud-open-service-broker-spring-cloud-open-service-broker-a)

[Spring Cloud Cluster](#a-href-http-github-com-spring-cloud-spring-cloud-cluster-spring-cloud-cluster-a)

[Spring Cloud Consul](#a-href-http-cloud-spring-io-spring-cloud-consul-spring-cloud-consul-a)

[Spring Cloud Security](#a-href-http-cloud-spring-io-spring-cloud-security-spring-cloud-security-a)

[Spring Cloud Sleuth](#a-href-http-cloud-spring-io-spring-cloud-sleuth-spring-cloud-sleuth-a)

[Spring Cloud Data Flow](#a-href-http-cloud-spring-io-spring-cloud-dataflow-spring-cloud-data-flow-a)

[Spring Cloud Stream](#a-href-http-cloud-spring-io-spring-cloud-stream-spring-cloud-stream-a)

[Spring Cloud Stream App Starters](#a-href-http-cloud-spring-io-spring-cloud-stream-app-starters-spring-cloud-stream-app-starters-a)

[Spring Cloud Task](#a-href-http-cloud-spring-io-spring-cloud-task-spring-cloud-task-a)

[Spring Cloud Task App Starters](#a-href-http-cloud-spring-io-spring-cloud-task-app-starters-spring-cloud-task-app-starters-a)

[Spring Cloud Zookeeper](#a-href-http-cloud-spring-io-spring-cloud-zookeeper-spring-cloud-zookeeper-a)

[Spring Cloud AWS](#a-href-http-github-com-spring-cloud-spring-cloud-aws-spring-cloud-aws-a)

[Spring Cloud Connectors](#a-href-http-github-com-spring-cloud-spring-cloud-connectors-spring-cloud-connectors-a)

[Spring Cloud Starters](#a-href-http-github-com-spring-cloud-spring-cloud-starters-spring-cloud-starters-a)

[Spring Cloud CLI](#a-href-http-github-com-spring-cloud-spring-cloud-cli-spring-cloud-cli-a)

[Spring Cloud Contract](#a-href-http-cloud-spring-io-spring-cloud-contract-spring-cloud-contract-a)

[Spring Cloud Gateway](#a-href-http-cloud-spring-io-spring-cloud-gateway-spring-cloud-gateway-a)

[Spring Cloud OpenFeign](#a-href-http-cloud-spring-io-spring-cloud-openfeign-spring-cloud-openfeign-a)

[Spring Cloud Pipelines](#a-href-http-cloud-spring-io-spring-cloud-pipelines-spring-cloud-pipelines-a)

[Spring Cloud Function](#a-href-http-github-com-spring-cloud-spring-cloud-function-spring-cloud-function-a)

[发布练习](#%E5%8F%91%E5%B8%83%E7%BB%83%E4%B9%A0)

[交流视频](#%E4%BA%A4%E6%B5%81%E8%A7%86%E9%A2%91)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud为开发人员提供了快速构建分布式系统中一些常见模式的工具（例如配置管理，服务发现，断路器，智能路由，微代理，控制总线，一次性令牌，全局锁定，领导选举，分布式会话，集群状态）。分布式系统的协调导致锅炉板模式，使用Spring Cloud开发人员可以快速站起来实现这些模式的服务和应用程序。它们适用于任何分布式环境，包括开发人员自己的笔记本电脑，裸机数据中心和Cloud Foundry等托管平台。

## 特性



Spring Cloud专注于为典型用例提供良好的开箱即用体验，并为其他用户提供可扩展性机制。
-     分布式/版本化配置
-     服务注册和发现
-     路由
-     服务到服务调用
-     负载均衡
-     断路器
-     全局锁
-     领导选举和集群状态
-     分布式消息

Spring Cloud采用了一种非常具有声明性的方法，通常只需更改类路径和/或注释即可获得大量的内容。作为发现客户端的示例应用程序：

```java
@SpringBootApplication
@EnableDiscoveryClient
public class Application {
	public static void main(String[] args) {
		SpringApplication.run(Application.class, args);
	}
}
```

## 主要项目

### [Spring Cloud Config](https://cloud.spring.io/spring-cloud-config)

由git存储库支持的集中式外部配置管理。 配置资源直接映射到Spring Environment，但如果需要，可以由非Spring应用程序使用。

### [Spring Cloud Netflix](https://cloud.spring.io/spring-cloud-netflix)

与各种Netflix OSS组件集成（Eureka，Hystrix，Zuul，Archaius等）。

### [Spring Cloud Bus](https://cloud.spring.io/spring-cloud-bus)

用于将服务和服务实例与分布式消息传递链接在一起的事件总线 用于跨群集传播状态更改（例如，配置更改事件）。

### [Spring Cloud Cloudfoundry](https://cloud.spring.io/spring-cloud-cloudfoundry)

将您的应用程序与Pivotal Cloud Foundry集成。 提供服务发现实现，还可以轻松实现受SSO和OAuth2保护的资源。

### [Spring Cloud Open Service Broker](https://cloud.spring.io/spring-cloud-open-service-broker)

提供构建实现Open Service Broker API的服务代理的起点。

### [Spring Cloud Cluster](https://github.com/spring-cloud/spring-cloud-cluster)

领导者选举和共同的有状态模式与Zookeeper，Redis，Hazelcast，Consul的抽象和实现。

### [Spring Cloud Consul](https://cloud.spring.io/spring-cloud-consul)

Hashicorp Consul的服务发现和配置管理。

### [Spring Cloud Security](https://cloud.spring.io/spring-cloud-security)

为Zuul代理中的负载平衡OAuth2 rest客户端和身份验证头中继提供支持。

### [Spring Cloud Sleuth](https://cloud.spring.io/spring-cloud-sleuth)

Spring Cloud应用程序的分布式跟踪，与Zipkin，HTrace和基于日志（例如ELK）的跟踪兼容。

### [Spring Cloud Data Flow](https://cloud.spring.io/spring-cloud-dataflow)

适用于现代运行时的可组合微服务应用程序的云本机编排服务。 易于使用的DSL，拖放式GUI和REST-API共同简化了基于微服务的数据管道的整体编排。

### [Spring Cloud Stream](https://cloud.spring.io/spring-cloud-stream)

轻量级事件驱动的微服务框架，可快速构建可连接到外部系统的应用程序。 在Spring Boot应用程序之间使用Apache Kafka或RabbitMQ发送和接收消息的简单声明模型。

### [Spring Cloud Stream App Starters](https://cloud.spring.io/spring-cloud-stream-app-starters)

Spring Cloud Stream App Starters是基于Spring Boot的Spring Integration应用程序，可提供与外部系统的集成。

### [Spring Cloud Task](https://cloud.spring.io/spring-cloud-task)

一种短命的微服务框架，用于快速构建执行有限数据处理的应用程序。 用于向Spring Boot应用程序添加功能和非功能功能的简单声明。

### [Spring Cloud Task App Starters](https://cloud.spring.io/spring-cloud-task-app-starters)

Spring Cloud Task App Starters是Spring Boot应用程序，可能是任何进程，包括不能永久运行的Spring Batch作业，它们在有限的数据处理期后结束/停止。

### [Spring Cloud Zookeeper](https://cloud.spring.io/spring-cloud-zookeeper)

使用Apache Zookeeper进行服务发现和配置管理。

### [Spring Cloud AWS](https://github.com/spring-cloud/spring-cloud-aws)

与托管的Amazon Web Services轻松集成。 它提供了一种使用众所周知的Spring习语和API（如消息传递或缓存API）与AWS提供的服务进行交互的便捷方式。 开发人员可以围绕托管服务构建应用程序，而无需关心基础结构或维护。

### [Spring Cloud Connectors](https://github.com/spring-cloud/spring-cloud-connectors)

使各种平台中的PaaS应用程序可以轻松连接到数据库和消息代理（该项目以前称为“Spring Cloud”）等后端服务。

### [Spring Cloud Starters](https://github.com/spring-cloud/spring-cloud-starters)

Spring Boot风格的入门项目，可以简化Spring Cloud用户的依赖管理。 （作为项目停产并在Angel.SR2之后与其他项目合并。）

### [Spring Cloud CLI](https://github.com/spring-cloud/spring-cloud-cli)

Spring Boot CLI插件，用于在Groovy中快速创建Spring Cloud组件应用程序

### [Spring Cloud Contract](https://cloud.spring.io/spring-cloud-contract)

Spring Cloud Contract是一个总体项目，其中包含帮助用户成功实施消费者驱动合同方法的解决方案。

### [Spring Cloud Gateway](https://cloud.spring.io/spring-cloud-gateway)

Spring Cloud Gateway是一款基于Project Reactor的智能可编程路由器。

### [Spring Cloud OpenFeign](https://cloud.spring.io/spring-cloud-openfeign)

Spring Cloud OpenFeign通过自动配置和Spring环境以及其他Spring编程模型习惯用法提供Spring Boot应用程序的集成。

### [Spring Cloud Pipelines](https://cloud.spring.io/spring-cloud-pipelines)

Spring Cloud Pipelines提供了一个固定意见的部署管道，其中包含确保您的应用程序可以零停机方式部署并轻松回滚出错的步骤。

### [Spring Cloud Function](https://github.com/spring-cloud/spring-cloud-function)

Spring Cloud Function通过函数促进业务逻辑的实现。 它支持无服务器提供商之间的统一编程模型，以及独立运行（本地或PaaS）的能力。

## 发布练习



Spring Cloud是一个由独立项目组成的总体项目，原则上具有不同的发布节奏。 为了管理投资组合，发布了BOM（物料清单），其中包含一组针对单个项目的依赖关系（见下文）。 发布列车有名称而不是版本，以避免与子项目混淆。 这些名称是一个字母序列（所以你可以按时间顺序排序）伦敦地铁站的名称（“天使”是第一个版本，“布里克斯顿”是第二个版本）。 当各个项目的点数累积到临界质量时，或者其中一个项目中存在一个需要每个人都可用的关键错误时，发布列车将推出名称结尾为“.SRX”的“服务发布”， 其中“X”是一个数字。

*表1.发布列Spring Boot兼容性*
|Release Train|Boot Version|
|----|----|
|Greenwich|2.1.x|
|Finchley|2.0.x|
|Edgware|1.5.x|
|Dalston|1.5.x|

* 表2.发布练习内容*
|Component|Edgware.SR5|Finchley.SR2|Finchley.BUILD-SNAPSHOT|
|----|----|----|----|
|spring-cloud-aws|1.2.3.RELEASE|2.0.1.RELEASE|2.0.1.BUILD-SNAPSHOT|
|spring-cloud-bus|1.3.3.RELEASE|2.0.0.RELEASE|2.0.1.BUILD-SNAPSHOT|
|spring-cloud-cli|1.4.1.RELEASE|2.0.0.RELEASE|2.0.1.BUILD-SNAPSHOT|
|spring-cloud-commons|1.3.5.RELEASE|2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-contract|1.2.6.RELEASE|2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-config|1.4.5.RELEASE|2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-netflix|1.4.6.RELEASE|2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-security|1.2.3.RELEASE|2.0.1.RELEASE|2.0.1.BUILD-SNAPSHOT|
|spring-cloud-cloudfoundry|1.1.2.RELEASE|2.0.1.RELEASE|2.0.1.BUILD-SNAPSHOT|
|spring-cloud-consul|1.3.5.RELEASE|2.0.1.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-sleuth|1.3.5.RELEASE|2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-stream|Ditmars.SR4|Elmhurst.SR1|Elmhurst.BUILD-SNAPSHOT|
|spring-cloud-zookeeper|1.2.2.RELEASE|2.0.0.RELEASE|2.0.1.BUILD-SNAPSHOT|
|spring-boot|1.5.16.RELEASE|2.0.6.RELEASE|2.0.7.BUILD-SNAPSHOT|
|spring-cloud-task|1.2.3.RELEASE|2.0.0.RELEASE|2.0.1.BUILD-SNAPSHOT|
|spring-cloud-vault|1.1.2.RELEASE|2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-gateway|1.0.2.RELEASE|2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-openfeign||2.0.2.RELEASE|2.0.2.BUILD-SNAPSHOT|
|spring-cloud-function|1.0.1.RELEASE|1.0.0.RELEASE|1.0.1.BUILD-SNAPSHOT|

Finchley构建并使用Spring Boot 2.0.x，预计不会与Spring Boot 1.5.x一起使用。

*注意：Dalston发布列车将于2018年12月[达到使用寿命](https://spring.io/blog/2018/06/19/spring-cloud-finchley-release-is-available).Eginware将遵循Spring Boot 1.5.x的生命周期结束周期。*


Dalston和Edgware发布版基于Spring Boot 1.5.x构建，预计不会与Spring Boot 2.0.x一起使用。
注意

卡姆登发布练习标志着[寿命终结](https://spring.io/blog/2018/06/19/spring-cloud-finchley-release-is-available)。


Camden发布系列基于Spring Boot 1.4.x构建，但也经过1.5.x测试。
注意

Brixton和Angel发布列车于2017年7月标志着[寿命终止](https://spring.io/blog/2017/07/21/spring-cloud-dalston-sr2-is-available-now#end-of-life-for-angel-and-brixton-release-trains)（EOL）。


Brixton版本系列基于Spring Boot 1.3.x构建，但也经过了1.4.x的测试。


Angel版本系列基于Spring Boot 1.2.x构建，在某些方面与Spring Boot 1.3.x不兼容。 Brixton构建于Spring Boot 1.3.x之上，与1.2.x同样不兼容。一些基于Angel构建的库和大多数应用程序都可以在Brixton上正常运行，但是在使用spring-cloud-security 1.0.x的OAuth2功能的任何地方都需要进行更改（它们大部分都转移到1.3.0中的Spring Boot）。


使用依赖关系管理工具来控制版本。如果您正在使用Maven，请记住声明的第一个版本获胜，因此按顺序声明BOM，第一个通常是最新版本（例如，如果您想使用带有Brixton.RELEASE的Spring Boot 1.3.6，请放置引导BOM第一）。如果您使用Spring依赖关系管理插件，则同样的规则适用于Gradle。
注意

版本系列包含spring-cloud-dependencies以及spring-cloud-starter-parent。您可以像使用spring-boot-starter-parent一样使用父级（如果您使用的是Maven）。如果您只需要依赖关系管理，那么“依赖关系”版本是同一事物的仅限BOM的版本（它只包含依赖关系管理，没有插件声明或直接引用Spring或Spring Boot）。如果您使用的是Spring Boot父POM，则可以使用Spring Cloud中的BOM。相反的情况并非如此：使用Cloud父级使得使用Boot BOM更改Spring Boot及其依赖项的版本变得不可能或至少不可靠。


## 交流视频
- [Beginner’s Guide To Spring Cloud](https://www.youtube.com/watch?v=aO3W-lYnw-o)

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|Finchley SR2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/Finchley.SR2/)|[API Doc.](https://spring.io/projects/spring-cloud#learn)|
|----|----|----|
|Greenwich RC2 PRE|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/Greenwich.RC2/)|[API Doc.](https://spring.io/projects/spring-cloud#learn)|
|Greenwich SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-cloud#learn)|[API Doc.](https://spring.io/projects/spring-cloud#learn)|
|Finchley SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-cloud#learn)|[API Doc.](https://spring.io/projects/spring-cloud#learn)|
|Edgware SR5 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/Edgware.SR5/index.html)|[API Doc.](https://spring.io/projects/spring-cloud#learn)|
|Edgware SNAPSHOT|[Reference Doc.](https://spring.io/projects/spring-cloud#learn)|[API Doc.](https://spring.io/projects/spring-cloud#learn)|
|Dalston SR5 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/Dalston.SR5/)|[API Doc.](https://spring.io/projects/spring-cloud#learn)|

# 示例

尝试一些例子：
- [**Config Server**](https://github.com/spring-cloud-samples/configserver)
- [**Service Registry**](https://github.com/spring-cloud-samples/eureka)
- [**Circuit Breaker Dashboard**](https://github.com/spring-cloud-samples/hystrix-dashboard)
- [**Business Application** (Customers and Stores)](https://github.com/spring-cloud-samples/customers-stores)
- [**OAuth2 Authorization Server**](https://github.com/spring-cloud-samples/authserver)
- [**OAuth2 SSO Client**](https://github.com/spring-cloud-samples/sso)
- [**Integration Test Samples**](https://github.com/spring-cloud-samples/tests)
- [**Spring Cloud Contract Samples**](https://github.com/spring-cloud-samples/spring-cloud-contract-samples)



