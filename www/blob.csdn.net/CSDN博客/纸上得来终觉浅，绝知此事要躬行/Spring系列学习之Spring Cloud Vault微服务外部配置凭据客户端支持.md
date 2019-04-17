# Spring系列学习之Spring Cloud Vault微服务外部配置凭据客户端支持 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 18:07:49[boonya](https://me.csdn.net/boonya)阅读数：256








英文原文：[https://cloud.spring.io/spring-cloud-vault/](https://cloud.spring.io/spring-cloud-vault/)

**目录**

[Spring Cloud Vault](#Spring%20Cloud%20Vault)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[添加Spring Cloud Vault配置模块](#%E6%B7%BB%E5%8A%A0Spring%20Cloud%20Vault%E9%85%8D%E7%BD%AE%E6%A8%A1%E5%9D%97)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[相关项目](#%E7%9B%B8%E5%85%B3%E9%A1%B9%E7%9B%AE)

## Spring Cloud Vault

Spring Cloud Vault Config为分布式系统中的外部化配置提供客户端支持。 使用HashiCorp的Vault，您可以在所有环境中为应用程序管理外部机密属性。 Vault可以管理静态和动态机密，例如远程应用程序/资源的用户名/密码，并为外部服务提供凭据，如MySQL，PostgreSQL，Apache Cassandra，MongoDB，Consul，AWS等。

## 特性
-      从Vault检索机密并使用远程属性源初始化Spring Environment。
-      获取使用SSL保护的机密。
-      为MySQL，PostgreSQL，Apache Cassandra，MongoDB，Consul，AWS和RabbitMQ生成凭据。
-      令牌，AppId，AppRole，客户端证书，Cubbyhole，以及AWS EC2和IAM，Kubernetes身份验证。
-      Bootstrap应用程序上下文：主应用程序的父上下文，可以训练为执行任何操作。
-      通过Spring Cloud Vault Connector使用HashiCorp的Vault服务代理进行Cloud Foundry集成。

## 快速开始

在项目中开始使用spring-cloud-vault的推荐方法是使用依赖关系管理系统 - 下面的代码段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的Maven和Gradle构建入门指南。

*Maven：*

```
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-vault-dependencies</artifactId>
            <version>2.1.0.RC1</version>
            <scope>import</scope>
            <type>pom</type>
        </dependency>
    </dependencies>
</dependencyManagement>

<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-vault-config</artifactId>
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

*Gradle：*

```
buildscript {
	dependencies {
		classpath "io.spring.gradle:dependency-management-plugin:0.5.2.RELEASE"
	}
}

apply plugin: "io.spring.dependency-management"

dependencyManagement {
	imports {
		mavenBom 'org.springframework.cloud:spring-cloud-vault-dependencies:2.1.0.RC1'
	}
}

dependencies {
	compile 'org.springframework.cloud:spring-cloud-starter-vault-config'
}repositories {
    maven {
        url 'https://repo.spring.io/libs-milestone'
    }
}
```

要开始使用Spring Cloud Vault，只需包含构建依赖项。


例如，典型的POM依赖关系是：

```
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-vault-config</artifactId>
</dependency>
```

然后配置Vault端点和身份验证

bootstrap.yml用于基于令牌的身份验证

```
spring.application.name: my-application
spring.cloud.vault:
    host: localhost
    port: 8200
    scheme: https
    authentication: TOKEN
    token: …
```

用于AWS-EC2身份验证的bootstrap.yml

```
spring.application.name: my-application
spring.cloud.vault:
    host: localhost
    port: 8200
    scheme: https
    authentication: AWS_EC2
```

最后，使用应用程序中存储在Vault中的属性

```java
@Configuration
@RestController
public class Application {

  @Value("${config.name}")
  String name = "World";

  @RequestMapping("/")
  public String home() {
    return "Hello " + name;
  }

  public static void main(String[] args) {
    SpringApplication.run(Application.class, args);
  }
}
```

Spring Cloud Vault Config使用应用程序名称和活动配置文件从Vaults读取配置属性：

```
/secret/{application}/{profile}
/secret/{application}
/secret/{default-context}/{profile}
/secret/{default-context}
```



## 添加Spring Cloud Vault配置模块



Spring Cloud Vault Config附带了多个支持不同集成的模块。 要在适当的版本中获取依赖关系，您可以在依赖关系管理中包含BOM（物料清单）。 例如，典型的POM依赖关系管理将是：

```
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-vault-dependencies</artifactId>
            <version>x.y.z</version>
            <scope>import</scope>
            <type>pom</type>
        </dependency>
    </dependencies>
</dependencyManagement>

<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-vault-config</artifactId>
    </dependency>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-vault-config-consul</artifactId>
    </dependency>
</dependencies>
```

## 发布版本

Spring Cloud Vault

Release

Documentation

2.1.0 RC1

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-vault/2.1.0.RC1/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-vault/2.1.0.RC1/)

2.0.3

[Reference](http://cloud.spring.io/spring-cloud-vault/2.0.x/)[API](http://cloud.spring.io/spring-cloud-vault/2.0.x/)

2.0.2

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-vault/2.0.2.RELEASE/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-vault/2.0.2.RELEASE/)

1.1.3

[Reference](http://cloud.spring.io/spring-cloud-vault/1.1.x/)[API](http://cloud.spring.io/spring-cloud-vault/1.1.x/)

1.1.2

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-vault/1.1.2.RELEASE/)[API](http://cloud.spring.io/spring-cloud-static/spring-cloud-vault/1.1.2.RELEASE/)

1.1.2

[Reference](http://cloud.spring.io/spring-cloud-vault/1.1.x/)[API](http://cloud.spring.io/spring-cloud-vault/1.1.x/)

1.0.2

[Reference](http://cloud.spring.io/spring-cloud-vault/1.0.2.RELEASE/)[API](http://github.com/spring-cloud/spring-cloud-vault)

## 相关项目
- [Spring Vault repository](https://github.com/spring-projects/spring-vault)
- [Spring Cloud Vault Connector](https://github.com/pivotal-cf/spring-cloud-vault-connector)
- [Examples repository](https://github.com/mp911de/spring-cloud-vault-config-samples)



