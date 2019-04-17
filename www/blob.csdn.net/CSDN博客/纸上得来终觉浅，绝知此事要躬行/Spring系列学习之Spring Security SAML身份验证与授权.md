# Spring系列学习之Spring Security SAML身份验证与授权 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 23:55:42[boonya](https://me.csdn.net/boonya)阅读数：1837








英文原文：[http://projects.spring.io/spring-security-saml/](http://projects.spring.io/spring-security-saml/)

**目录**

[Spring Security SAML](#Spring%20Security%20SAML)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[版本](#%E7%89%88%E6%9C%AC)

[资源](#%E8%B5%84%E6%BA%90)

## Spring Security SAML

Spring Security Extension允许在单个应用程序中无缝组合SAML 2.0和其他身份验证和联合机制。

Spring Security SAML Extension允许在单个应用程序中无缝组合SAML 2.0以及身份验证和联合机制。 在身份提供商模式下支持SAML 2.0的所有产品（例如ADFS，Okta，Shibboleth，OpenAM，Efecte EIM或Ping Federate）都可用于连接扩展。

## 特性
-      支持多个SAML 2.0配置文件（Web单点登录，Web单点登录持有者，单点注销，增强型客户端/代理等）
-      IDP和SP初始化单点登录
-      身份提供商发现配置文件和IDP选择
-      元数据互操作性和PKIX信任管理
-      自动服务提供商元数据生成
-      从文件，URL，文件支持的URL加载元数据
-      使用许多身份提供程序处理和自动重新加载元数据
-      处理SAML属性
-      易于集成的示例应用程序
-      流行国内流离失所者快速入门指南
-      多得多…

## 快速开始

在项目中使用spring-security-saml的推荐方法是使用依赖关系管理系统 - 下面的代码片段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的Maven和Gradle构建入门指南。

*Maven：*

```
<dependencies>
    <dependency>
        <groupId>org.springframework.security.extensions</groupId>
        <artifactId>spring-security-saml2-core</artifactId>
        <version>1.0.3.RELEASE</version>
    </dependency>
</dependencies>
```

*Gradle：*

```
dependencies {
    compile 'org.springframework.security.extensions:spring-security-saml2-core:1.0.3.RELEASE'
}
```

## 版本

Spring Security SAML

Release

Documentation

1.0.4

[Reference](http://docs.spring.io/spring-security-saml/docs/1.0.4.BUILD-SNAPSHOT/reference/html/)[API](http://docs.spring.io/spring-security-saml/docs/1.0.4.BUILD-SNAPSHOT/api/)

1.0.3

[Reference](http://docs.spring.io/spring-security-saml/docs/1.0.x/reference/html/)[API](http://docs.spring.io/spring-security-saml/docs/1.0.x/api/)

## 资源
- [Quick Start](https://docs.spring.io/spring-security-saml/docs/1.0.0.RELEASE/reference/html/chapter-quick-start.html)
- [Online demo](https://saml-federation.appspot.com)
- [Spring Security](https://projects.spring.io/spring-security)



