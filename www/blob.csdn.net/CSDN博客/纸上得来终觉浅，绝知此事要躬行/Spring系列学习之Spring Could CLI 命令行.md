# Spring系列学习之Spring Could CLI 命令行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 16:15:21[boonya](https://me.csdn.net/boonya)阅读数：48








英文原文：[https://cloud.spring.io/spring-cloud-cli/](https://cloud.spring.io/spring-cloud-cli/)

**目录**

[Spring Cloud CLI](#Spring%20Cloud%20CLI)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[启动服务器](#%E5%90%AF%E5%8A%A8%E6%9C%8D%E5%8A%A1%E5%99%A8)

[编写一个Groovy脚本并运行它](#%E7%BC%96%E5%86%99%E4%B8%80%E4%B8%AAGroovy%E8%84%9A%E6%9C%AC%E5%B9%B6%E8%BF%90%E8%A1%8C%E5%AE%83)

[加密和解密](#%E5%8A%A0%E5%AF%86%E5%92%8C%E8%A7%A3%E5%AF%86)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[示例项目](#%E7%A4%BA%E4%BE%8B%E9%A1%B9%E7%9B%AE)

## Spring Cloud CLI

Spring Cloud CLI是Spring Boot CLI的一组扩展，可以运行Spring Cloud服务器应用程序并在Groovy脚本中编写Spring Cloud应用程序并从shell运行它们。 为所有Spring Cloud项目提供自动导入和依赖关系管理。

## 快速开始

要安装CLI，请确保您具有Spring Boot CLI（1.4.4或更高版本）：

```
$ spring version
Spring CLI v1.5.3.RELEASE
```

例如, 对于SDKMan用户

```
$ sdk install springboot 1.5.3.RELEASE
$ sdk use springboot 1.5.3.RELEASE
```

并安装Spring Cloud插件：

```
$ spring install org.springframework.cloud:spring-cloud-cli:1.3.1.RELEASE
```



### 启动服务器

要运行配置服务器：

```
$ spring cloud configserver
```

您还可以运行eureka，zipkin，hystrixdashboard，h2，kafka或dataflow（如果在命令行上用空格分隔，则可以运行多个应用程序）。

### 编写一个Groovy脚本并运行它



安装了Cloud CLI扩展的任何Spring Boot CLI应用程序都将是Spring Cloud Config Client，因此请将spring.cloud.config.uri设置为指向Config Server并且您处于业务状态（默认为“http：// localhost：8888"）。


这是一个完整的Eureka服务器：

```java
@EnableEurekaServer
@RestController
class Eureka {}
```

### 加密和解密

示例（对称密钥）：

```
$ spring encrypt --key=secret foo
```

**重要信息：先决条件：**要使用加密和解密功能，您需要在JVM中安装全功能JCE（默认情况下不存在）。 您可以从Oracle下载“Java Cryptography Extension（JCE）Unlimited Strength Jurisdiction Policy Files”，并按照安装说明进行操作（实际上将JRE lib / security目录中的2个策略文件替换为您下载的那些）。

## 发布版本

Spring Cloud CLI

Release

Documentation

2.0.1

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-cli/2.0.0.RELEASE/)[API](http://github.com/spring-cloud/spring-cloud-cli)

2.0.0

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-cli/2.0.0.RELEASE/)[API](http://github.com/spring-cloud/spring-cloud-cli)

1.3.2

[Reference](http://cloud.spring.io/spring-cloud-static/spring-cloud-cli/1.3.2.RELEASE/)[API](http://github.com/spring-cloud/spring-cloud-cli)

## 示例项目

[Minimal (Groovy) Client](https://github.com/spring-cloud-samples/scripts/blob/master/demo/client.groovy)



