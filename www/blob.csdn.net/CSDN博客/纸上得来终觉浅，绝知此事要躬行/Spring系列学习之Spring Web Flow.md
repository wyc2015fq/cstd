# Spring系列学习之Spring Web Flow - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 22:59:37[boonya](https://me.csdn.net/boonya)阅读数：293








英文原文：[https://projects.spring.io/spring-webflow/](https://projects.spring.io/spring-webflow/)

**目录**

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[版本](#%C2%A0%E7%89%88%E6%9C%AC)

[示例](#%E7%A4%BA%E4%BE%8B)

Spring Web Flow构建于Spring MVC之上，允许实现Web应用程序的“流程”。 流程封装了一系列步骤，指导用户执行某些业务任务。 它跨越多个HTTP请求，具有状态，处理事务数据，可重用，并且可能是动态的，并且本质上是长期运行的。

Spring Web Flow的最佳位置是具有受控导航功能的有状态Web应用程序，例如办理登机手续，申请贷款，购物车结帐，甚至向表单添加确认步骤。 这些场景的共同点是以下一个或多个特征：
-      有一个明确的开始和结束点。
-      用户必须按特定顺序浏览一组屏幕。
-      直到最后一步，更改才会完成。
-      一旦完成，就不可能意外地重复交易

## 快速开始

在项目中使用spring-webflow的推荐方法是使用依赖关系管理系统 - 下面的代码段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的Maven和Gradle构建入门指南。（导航到英文原文可选择版本和依赖方式）

Maven

```
<dependencies>
    <dependency>
        <groupId>org.springframework.webflow</groupId>
        <artifactId>spring-webflow</artifactId>
        <version>2.5.0.RELEASE</version>
    </dependency>
</dependencies>
```

Gradle

```
dependencies {
    compile 'org.springframework.webflow:spring-webflow:2.5.0.RELEASE'
}
```

Spring Web Flow提供了一种声明性流定义语言，用于在更高级别的抽象上创作流。 它允许将它集成到各种应用程序中，而无需对流程编程模型进行任何更改（包括Spring MVC，JSF，甚至Portlet Web应用程序）。 以下是在具有导航要求的有状态Web应用程序中观察到的常见问题：
-      可视化流程非常困难。
-      应用程序有很多代码访问HTTP会话。
-      实施受控导航很重要，但不可能。
-      正确的浏览器后退按钮支持似乎无法实现。
-      浏览器和服务器与“后退”按钮使用不同步。
-      多个浏览器选项卡会导致HTTP会话数据出现并发问题。



Spring Web Flow为上述问题提供了解决方案。



##  版本

Release

Documentation

2.5.0

[Reference](http://docs.spring.io/spring-webflow/docs/2.5.0.RELEASE/reference/html)[API](http://docs.spring.io/spring-webflow/docs/2.5.0.RELEASE/api/)

2.4.5

[Reference](http://docs.spring.io/spring-webflow/docs/2.4.5.RELEASE/reference/html)[API](http://docs.spring.io/spring-webflow/docs/2.4.5.RELEASE/api/)

## 示例

[spring-webflow-samples repository](https://github.com/SpringSource/spring-webflow-samples)





