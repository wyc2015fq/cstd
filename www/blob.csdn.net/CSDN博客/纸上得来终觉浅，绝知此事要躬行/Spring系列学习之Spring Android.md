# Spring系列学习之Spring Android - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 17:50:19[boonya](https://me.csdn.net/boonya)阅读数：97








英文原文：[https://projects.spring.io/spring-android/](https://projects.spring.io/spring-android/)

**目录**

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[下载](#%E4%B8%8B%E8%BD%BD)

[用法](#%E7%94%A8%E6%B3%95)

[示例](#%E7%A4%BA%E4%BE%8B)

Spring for Android是Spring Framework的扩展，旨在简化原生Android应用程序的开发。

Spring for Android是一个框架，旨在提供用于Android应用程序的Spring系列项目的组件。 像所有Spring项目一样，Spring for Android的真正强大之处在于它可以轻松扩展。

## 特性
-      适用于Android的Rest客户端
-      Auth支持访问安全API

## 快速开始

### 下载

在项目中使用spring-android的推荐方法是使用依赖管理系统 - 下面的代码片段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的Maven和Gradle构建入门指南。

## 用法

以下示例说明了如何搜索Google：

```java
// The connection URL 
String url = "https://ajax.googleapis.com/ajax/" + 
    "services/search/web?v=1.0&q={query}";

// Create a new RestTemplate instance
RestTemplate restTemplate = new RestTemplate();

// Add the String message converter
restTemplate.getMessageConverters().add(new StringHttpMessageConverter());

// Make the HTTP GET request, marshaling the response to a String
String result = restTemplate.getForObject(url, String.class, "Android");
```

## 示例

[Spring for Android Samples](https://github.com/SpringSource/spring-android-samples)





