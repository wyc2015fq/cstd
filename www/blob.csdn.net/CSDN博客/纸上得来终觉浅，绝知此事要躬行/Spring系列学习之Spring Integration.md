# Spring系列学习之Spring Integration - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 14:22:09[boonya](https://me.csdn.net/boonya)阅读数：1173








英文原文：[https://spring.io/projects/spring-integration](https://spring.io/projects/spring-integration)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[介绍](#%E4%BB%8B%E7%BB%8D)

[特性](#%E7%89%B9%E6%80%A7)

[例子](#%E4%BE%8B%E5%AD%90)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[指南](#%E6%8C%87%E5%8D%97)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

扩展Spring编程模型以支持众所周知的[企业集成模式](http://www.eaipatterns.com/)。 Spring Integration在基于Spring的应用程序中实现轻量级消息传递，并支持通过声明适配器与外部系统集成。与Spring对远程处理，消息传递和调度的支持相比，这些适配器提供了更高级别的抽象。 Spring Integration的主要目标是提供一个简单的模型来构建企业集成解决方案，同时保持关注点的分离，这对于生成可维护，可测试的代码至关重要。

## 介绍

使用Spring Framework鼓励开发人员使用接口进行编码，并使用依赖注入（DI）为普通旧Java对象（PO​​JO）提供执行其任务所需的依赖项。 Spring Integration将这一概念更进一步，其中POJO使用消息传递范例连接在一起，并且各个组件可能不了解应用程序中的其他组件。这种应用程序是通过组装细粒度可重用组件来构建的，以形成更高级别的功能。通过精心设计，这些流程可以模块化，并在更高的层次上重复使用。


除了将细粒度组件连接在一起外，Spring Integration还提供多种通道适配器和网关，以便与外部系统进行通信。通道适配器用于单向集成（发送或接收）;网关用于请求/回复方案（入站或出站）。有关适配器和网关的完整列表，请参阅参考文档。


Spring Cloud Stream项目建立在Spring Integration之上，其中Spring Integration用作消息驱动的微服务的引擎。

## 特性
-     实施大多数企业集成模式
-     端点Endpoint
-     频道Channel（点对点和发布/订阅）
-     聚合（Aggregator）
-     过滤（Filter）
-     变压器（Transformer）
-     控制总线（Control Bus）
-     ...
-     与外部系统集成
-     REST / HTTP
-     FTP/ SFTP
-     推特（Twitter）
-     Web服务（SOAP和ReST）
-     TCP/ UDP
-     JMS
-     RabbitMQ
-     电子邮件（Email）
-     ...
-     该框架具有广泛的JMX支持
-     将框架组件公开为MBean
-     适配器从MBean获取属性，调用操作，发送/接收通知
- 

## 例子

在下面的“快速入门”应用程序中，您可以看到使用相同的网关接口来调用两个完全不同的服务实现。 要构建和运行该程序，您需要如上所述的spring-integration-ws和spring-integration-xml模块。

```java
public class Main {

	public static void main(String... args) throws Exception {
		ApplicationContext ctx =
			new ClassPathXmlApplicationContext("context.xml");
		// Simple Service
		TempConverter converter =
			ctx.getBean("simpleGateway", TempConverter.class);
		System.out.println(converter.fahrenheitToCelcius(68.0f));
		// Web Service
		converter  = ctx.getBean("wsGateway", TempConverter.class);
		System.out.println(converter.fahrenheitToCelcius(68.0f));
	}
}
```

```java
public interface TempConverter {

	float fahrenheitToCelcius(float fahren);

}
```

```
<!-- Simple Service -->

<int:gateway id="simpleGateway"
	service-interface="foo.TempConverter"
	default-request-channel="simpleExpression" />

<int:service-activator id="expressionConverter"
	input-channel="simpleExpression"
	expression="(payload - 32) / 9 * 5"/>

<!-- Web Service -->

<int:gateway id="wsGateway" service-interface="foo.TempConverter"
	default-request-channel="viaWebService" />

<int:chain id="wsChain" input-channel="viaWebService">
	<int:transformer
	   expression="'<FahrenheitToCelsius xmlns="https://www.w3schools.com/xml/"><Fahrenheit>XXX</Fahrenheit></FahrenheitToCelsius>'.replace('XXX', payload.toString())" />
	<int-ws:header-enricher>
		<int-ws:soap-action value="https://www.w3schools.com/xml/FahrenheitToCelsius"/>
	</int-ws:header-enricher>
	<int-ws:outbound-gateway
		uri="https://www.w3schools.com/xml/tempconvert.asmx"/>
	<int-xml:xpath-transformer
		xpath-expression="/*[local-name()='FahrenheitToCelsiusResponse']/*[local-name()='FahrenheitToCelsiusResult']"/>
</int:chain>
```

 这是使用Java DSL（和Spring Boot）的相同应用程序（Web服务部分）。 如果不使用Spring Boot，则需要直接使用spring-boot-starter-integration依赖项或spring-integration-java-dsl。 如果您使用Spring Integration从5.0开始，则不需要任何其他依赖项 - Java DSL包含在核心项目中：

```java
@Configuration
@SpringBootApplication
@IntegrationComponentScan
public class Application {

  public static void main(String[] args) {
    ConfigurableApplicationContext ctx = SpringApplication.run(Application.class, args);
    TempConverter converter = ctx.getBean(TempConverter.class);
    System.out.println(converter.fahrenheitToCelcius(68.0f));
    ctx.close();
  }

  @MessagingGateway
  public interface TempConverter {

    @Gateway(requestChannel = "convert.input")
    float fahrenheitToCelcius(float fahren);

  }

  @Bean
  public IntegrationFlow convert() {
      return f -> f
        .transform(payload ->
              "<FahrenheitToCelsius xmlns=\"https://www.w3schools.com/xml/\">"
            +     "<Fahrenheit>" + payload + "</Fahrenheit>"
            + "</FahrenheitToCelsius>")
        .enrichHeaders(h -> h
            .header(WebServiceHeaders.SOAP_ACTION,
                "https://www.w3schools.com/xml/FahrenheitToCelsius"))
        .handle(new SimpleWebServiceOutboundGateway(
            "https://www.w3schools.com/xml/tempconvert.asmx"))
        .transform(Transformers.xpath("/*[local-name()=\"FahrenheitToCelsiusResponse\"]"
            + "/*[local-name()=\"FahrenheitToCelsiusResult\"]"));
  }

}
```

## Spring Boot配置

[Spring集成的Spring Boot自动配置](https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-integration.html)

## 快速开始



使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|5.1.1 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-integration/docs/5.1.1.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-integration/docs/5.1.1.RELEASE/api/)|
|----|----|----|
|5.1.2 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-integration/docs/5.1.2.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-integration/docs/5.1.2.BUILD-SNAPSHOT/api/)|
|5.0.11 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-integration/docs/5.0.11.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-integration/docs/5.0.11.BUILD-SNAPSHOT/api/)|
|5.0.10 GA|[Reference Doc.](https://docs.spring.io/spring-integration/docs/5.0.10.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-integration/docs/5.0.10.RELEASE/api/)|
|4.3.19 SNAPSHOT|[Reference Doc.](https://docs.spring.io/spring-integration/docs/4.3.19.BUILD-SNAPSHOT/reference/html)|[API Doc.](https://docs.spring.io/spring-integration/docs/4.3.19.BUILD-SNAPSHOT/api/)|
|4.3.18 GA|[Reference Doc.](https://docs.spring.io/spring-integration/docs/4.3.18.RELEASE/reference/html)|[API Doc.](https://docs.spring.io/spring-integration/docs/4.3.18.RELEASE/api/)|

## 指南

该指南旨在在15-30分钟内完成，提供快速，实用的说明，用于为Spring的任何开发任务构建入门应用程序。
- [**Integrating Data**](https://spring.io/guides/gs/integration)了解如何构建使用Spring Integration获取数据，处理数据并将其写入文件的应用程序。

# 示例

尝试一些示例：
- [**50+ Spring Integration Samples**](https://github.com/SpringSource/spring-integration-samples)





