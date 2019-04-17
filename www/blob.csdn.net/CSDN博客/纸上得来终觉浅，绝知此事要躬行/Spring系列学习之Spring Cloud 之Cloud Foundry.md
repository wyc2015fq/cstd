# Spring系列学习之Spring Cloud 之Cloud Foundry - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 16:26:52[boonya](https://me.csdn.net/boonya)阅读数：510








英文原文：[https://cloud.spring.io/spring-cloud-cloudfoundry/](https://cloud.spring.io/spring-cloud-cloudfoundry/)

**目录**

[Spring Cloud Cloud Foundry](#Spring%20Cloud%20Cloud%20Foundry)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[示例项目](#%E7%A4%BA%E4%BE%8B%E9%A1%B9%E7%9B%AE)

## Spring Cloud Cloud Foundry


Spring Cloud for Cloudfoundry可以轻松地在Cloud Foundry（平台即服务）中运行Spring Cloud应用程序。 Cloud Foundry具有“服务”的概念，即“绑定”到应用程序的中间件，实质上为其提供包含凭据的环境变量（例如，用于服务的位置和用户名）。



### 特性

spring-cloud-cloudfoundry-web项目为Cloud Foundry中的一些增强的Web应用功能提供基本支持：自动绑定到单点登录服务，并可选择为发现启用粘性路由。

spring-cloud-cloudfoundry-discovery项目提供Spring Cloud Commons DiscoveryClient的实现，因此您可以@EnableDiscoveryClient并提供您的凭据spring.cloud.cloudfoundry.discovery。[email，password]然后您可以直接使用DiscoveryClient或通过 一个LoadBalancerClient（如果你没有连接到[Pivotal Web Services](https://run.pivotal.io/)，也是* .url）。

*     注意：如果您正在寻找绑定到服务的方法，那么这是错误的库。 请查看[Spring Cloud Connectors](https://github.com/spring-cloud/spring-cloud-connectors)。*



## 快速开始

在项目中使用spring-cloud-cloudfoundry的推荐方法是使用依赖关系管理系统 - 下面的代码段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的[Maven](https://spring.io/guides/gs/maven/)和[Gradle](https://spring.io/guides/gs/gradle/)构建入门指南。

*Maven*

```
<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-cloudfoundry-web</artifactId>
        <version>2.1.0.RC2</version>
    </dependency>
</dependencies><repositories>
    <repository>
        <id>spring-milestones</id>
        <name>Spring Milestones</name>
        <url>https://repo.spring.io/libs-milestone</url>
        <snapshots>
            <enabled>false</enabled>
        </snapshots>
    </repository>
</repositories>
```

*Gradle*

```
dependencies {
    compile 'org.springframework.cloud:spring-cloud-cloudfoundry-web:2.1.0.RC2'
}repositories {
    maven {
        url 'https://repo.spring.io/libs-milestone'
    }
}
```



### 发布版本

Spring Cloud for Cloud Foundry

Release

Documentation

2.1.0 RC2

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-cloudfoundry/2.1.0.RC2/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-cloudfoundry/2.1.0.RC2/)

2.0.2

[Reference](http://cloud.spring.io/spring-cloud-cloudfoundry/2.0.x/)[API](http://cloud.spring.io/spring-cloud-cloudfoundry/2.0.x/)

2.0.1

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-cloudfoundry/2.0.1.RELEASE/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-cloudfoundry/2.0.1.RELEASE/)

1.1.3

[Reference](http://cloud.spring.io/spring-cloud-cloudfoundry/1.1.x/)[API](http://cloud.spring.io/spring-cloud-cloudfoundry/1.1.x/)

1.1.2

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-cloudfoundry/1.1.2.RELEASE/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-cloudfoundry/1.1.2.RELEASE/)

1.0.2

[Reference](http://cloud.spring.io/spring-cloud-cloudfoundry/spring-cloud-cloudfoundry.html)[API](http://github.com/spring-cloud/spring-cloud-cloudfoundry)

1.0.1

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-cloudfoundry/1.0.1.RELEASE/)[API](http://github.com/spring-cloud/spring-cloud-cloudfoundry)

### 示例项目

[Cloud Foundry Sample](https://github.com/spring-cloud/spring-cloud-cloudfoundry/tree/master/spring-cloud-cloudfoundry-sample)





