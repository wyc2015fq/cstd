# Spring系列学习之Spring CredHub - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 18:34:34[boonya](https://me.csdn.net/boonya)阅读数：241








英文原文：[https://spring.io/projects/spring-credhub](https://spring.io/projects/spring-credhub)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[注入并使用CredHubTemplate](#%E6%B3%A8%E5%85%A5%E5%B9%B6%E4%BD%BF%E7%94%A8CredHubTemplate)

[认证Authentication](#%E8%AE%A4%E8%AF%81Authentication)

[相互TLS](#%E7%9B%B8%E4%BA%92TLS)

[OAuth2](#OAuth2)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

Spring CredHub提供客户端支持，用于存储，检索和删除在Cloud Foundry平台中运行的CredHub服务器的凭据。

CredHub提供了一个安全存储，生成，检索和删除各种类型凭据的API。 Spring CredHub为CredHub API提供了一个Java绑定，可以很容易地将Spring应用程序与CredHub集成。

## 注入并使用CredHubTemplate

CredHubTemplate用于与CredHub交互，通常通过其CredHubOperations接口使用。


以下是在CredHub中设置新凭据的示例：

```java
public class MyApp {
  @Autowired
  CredHubOperations credHubOperations;

  public void writeAndDeleteCredential() {
    PasswordCredentialRequest request =
      PasswordCredentialRequest.builder()
        .overwrite(true)
        .name(new SimpleCredentialName("spring-credhub", "demo"))
        .value(new PasswordCredential("secret"))
        .build();

    CredentialDetails<PasswordCredential> storedCredential =
        credHubOperations.write(request);

    CredentialDetails<PasswordCredential> retrievedCredential =
        credHubOperations.getById(storedCredential.getId());

    credHubOperations.deleteByName(storedCredential.getName());
  }
}
```

以下是在CredHub中生成新凭据的示例：

```java
public class MyApp {
  @Autowired
  CredHubOperations credHubOperations;

  public void generateCredential() {
    PasswordParametersRequest request =
      PasswordParametersRequest.builder()
        .overwrite(true)
        .name(new SimpleCredentialName("spring-credhub", "demo"))
        .parameters(PasswordParameters.builder()
            .length(20)
            .excludeLower(false)
            .excludeUpper(false)
            .excludeNumber(false)
            .includeSpecial(true)
            .build())
        .build();

    CredentialDetails<PasswordCredential> credential =
        credHubOperations.generate(request);
  }
}
```



## 认证Authentication




CredHub支持两种身份验证方法：相互TLS和OAuth2。

### 相互TLS

Mutual TLS是未提供其他身份验证配置时使用的默认身份验证方案。


Cloud Foundry上的Mutual TLS支持需要Java Buildpack的Container Security Provider功能。 使用Spring BuildHub的应用程序应使用Java Buildpack 3.17或更高版本或4.1或更高版本部署到Cloud Foundry。

### OAuth2

可以使用Spring CredHub（例如，在application.yml中）在Spring Boot应用程序中提供以下配置，以对CredHub服务器启用OAuth2身份验证。

```
spring:
  credhub:
    oauth2:
      client-id: [OAuth2 client ID]
      client-secret: [OAuth2 client secret]
      access-token-uri: [OAuth2 token server endpoint]
```

## Spring Boot配置

在项目中使用Spring CredHub的推荐方法是使用依赖关系管理系统。 下面的一个代码段可以复制并粘贴到您的构建中。


使用Maven：

```
<dependencies>
    <dependency>
        <groupId>org.springframework.credhub</groupId>
        <artifactId>spring-credhub-starter</artifactId>
        <version>1.0.1.RELEASE</version>
    </dependency>
</dependencies>
```

使用Gradle:

```
dependencies {
  compile('org.springframework.credhub:spring-credhub-starter:1.0.1.RELEASE')
}
```

如果您为Spring Boot应用程序提供spring.credhub.url属性，Spring CredHub将自动配置CredHubTemplate。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.0.1 CURRENT GA|[Reference Doc.](https://spring.io/projects/spring-credhub#learn)|[API Doc.](https://docs.spring.io/spring-credhub/docs/1.0.1.RELEASE/api/)|
|----|----|----|
|2.0.0 RC1 PRE|[Reference Doc.](https://docs.spring.io/spring-credhub/docs/2.0.0.RC1/reference/html5)|[API Doc.](https://docs.spring.io/spring-credhub/docs/2.0.0.RC1/api/)|
|2.0.0 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-credhub/docs/2.0.0.BUILD-SNAPSHOT/reference/html5)|[API Doc.](https://docs.spring.io/spring-credhub/docs/2.0.0.BUILD-SNAPSHOT/api/)|



