# Spring系列学习之Spring Cloud Contract测试消息传递 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 18:38:31[boonya](https://me.csdn.net/boonya)阅读数：222








英文原文：[https://spring.io/projects/spring-cloud-contract](https://spring.io/projects/spring-cloud-contract)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[Server / Producer方面](#Server%20%2F%20Producer%E6%96%B9%E9%9D%A2)

[Client / Consumer方面](#Client%20%2F%20Consumer%E6%96%B9%E9%9D%A2)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

Spring Cloud Contract是一个总体项目，其中包含帮助用户成功实施[消费者驱动合同](http://martinfowler.com/articles/consumerDrivenContracts.html)方法的解决方案。目前，Spring Cloud Contract包含Spring Cloud Contract Verifier项目。


Spring Cloud Contract Verifier是一个支持基于JVM的应用程序的消费者驱动合同（CDC）开发的工具。它附带了用Groovy或YAML编写的合同定义语言（DSL）。合同定义用于生成以下资源：
-     默认情况下，在对客户端代码（客户端测试）进行集成测试时，[WireMock](http://wiremock.org/)（HTTP Server Stub）将使用JSON存根定义。测试代码仍然必须手工编写，测试数据由Spring Cloud Contract Verifier生成。
-     如果你使用消息路由消息路由。我们正在与Spring Integration，Spring Cloud Stream和Apache Camel集成。但是，如果您愿意，可以设置自己的集成。
-     验证测试（默认情况下，在JUnit或Spock中）用于验证API的服务器端实现是否符合合同（服务器测试）。 Spring Cloud Contract Verifier生成完整测试。

Spring Cloud Contract Verifier将TDD提升到软件架构的水平。


要了解Spring Cloud Contract如何支持其他语言，请查看此[博客文章](https://spring.io/blog/2018/02/13/spring-cloud-contract-in-a-polyglot-world)。

## 特性


**在尝试测试与其他服务通信的应用程序时，我们可以执行以下两项操作之一：**
-     部署所有微服务并执行端到端测试
-     模拟单元/集成测试中的其他微服务

两者都有其优点，但也有很多缺点。让我们关注后者。

部署所有微服务并执行端到端测试

**好处：**
-     模拟生产
-     测试服务之间的真实通信


**缺点：**
-     为了测试一个微服务，我们必须部署6个微服务，几个数据库等。
-     将进行测试的环境将被锁定用于单个测试套件（即，在此期间没有其他人能够运行测试）。
-     很长时间跑
-     很晚的反馈
-     非常难以调试


**在单元/集成测试中模拟其他微服务**


好处：
-     非常快速的反馈
-     无基础设施要求



缺点：
-     服务的实现者创建存根，因此他们可能与现实无关
-     你可以通过测试和生产失败去生产



为了解决上述问题，创建了带有Stub Runner的Spring Cloud Contract Verifier。他们的主要想法是给你非常快速的反馈，而不需要建立整个微服务世界。

**Spring Cloud Contract Verifier功能：**
-     确保HTTP / Messaging存根（在开发客户端时使用）正在执行实际的服务器端实现
-     促进验收测试驱动开发方法和微服务架构风格
-     提供一种方法来发布在通信双方立即可见的合同中的更改
-     生成服务器端使用的样板测试代码

## Spring Boot配置


[有关详细的分步指南，请查看文档](https://cloud.spring.io/spring-cloud-contract/spring-cloud-contract.html#_step_by_step_guide_to_cdc)。 您可以在下面找到简化版本。

### **Server / Producer方面**

在服务器（HTTP）/生产者（消息传递）方面添加Spring Cloud Contract Verifier Maven / Gradle插件。 我们假设我们项目的组ID是com.example，工件id是http-server。


在配置中传递的测试的基类可能看起来像这样（我们假设我们正在使用[Rest Assured]（[http://rest-assured.io/](http://rest-assured.io/)）并且我们想要为FraudDetectionController执行CDC 你在写）

```java
package com.example;

// imports

public class MvcTest {

  @Before
  public void setup() {
    RestAssuredMockMvc.standaloneSetup(new FraudDetectionController());
  }

}
```

在src/test/resources/contracts中添加一个Contract定义。 例如名为shouldMarkClientAsFraud.groovy

```java
org.springframework.cloud.contract.spec.Contract.make {
  request {
    method 'PUT'
    url '/fraudcheck'
    body("""
    {
      "clientId":"1234567890",
      "loanAmount":99999
    }
    """)
    headers {
      header('Content-Type', 'application/vnd.fraud.v1+json')
    }
  }
response {
  status 200
  body("""
  {
    "fraudCheckStatus": "FRAUD",
    "rejectionReason": "Amount too high"
  }
  """)
  headers {
    header('Content-Type': 'application/vnd.fraud.v1+json')
  }
 }
}
```

如果你必须添加一个插件，将为你生成测试和存根。

*Maven*

```
<build>
		<plugins>
			<plugin>
				<groupId>org.springframework.cloud</groupId>
				<artifactId>spring-cloud-contract-maven-plugin</artifactId>
				<version>${spring-cloud-contract.version}</version>
				<extensions>true</extensions>
				<configuration>
					<baseClassForTests>com.example.MvcTest</baseClassForTests>
				</configuration>
			</plugin>
		</plugins>
	</build>
```

*Gradle*

```
buildscript {
    repositories {
      mavenCentral()
    }
    dependencies {
      classpath "org.springframework.boot:spring-boot-gradle-plugin:${springBootPluginVersion}"
      classpath "org.springframework.cloud:spring-cloud-contract-gradle-plugin:${springCloudContractVersion}"
    }
}

apply plugin: 'spring-boot'
apply plugin: 'spring-cloud-contract'

dependencyManagement {
    imports {
      mavenBom "org.springframework.cloud:spring-cloud-contract-dependencies:${springCloudDependencies}"
    }
}

dependencies {
    testCompile "org.springframework.cloud:spring-cloud-starter-contract-verifier"
}

contracts {
	baseClassForTests = 'com.example.MvcTest'
}
```

一旦您尝试从合同构建应用程序，将在/generated-test-sources/contracts下的输出文件夹中生成测试。

```java
package org.springframework.cloud.contract.verifier.tests;

// imports

public class ContractVerifierTest extends MvcTest {

  @Test
  public void validate_shouldMarkClientAsFraud() throws Exception {
    // given:
      MockMvcRequestSpecification request = given()
          .header("Content-Type", "application/vnd.fraud.v1+json")
          .body("{\"clientId\":\"1234567890\",\"loanAmount\":99999}");

    // when:
      ResponseOptions response = given().spec(request)
          .put("/fraudcheck");

    // then:
      assertThat(response.statusCode()).isEqualTo(200);
      assertThat(response.header("Content-Type")).isEqualTo("application/vnd.fraud.v1+json");
    // and:
      DocumentContext parsedJson = JsonPath.parse(response.getBody().asString());
      assertThatJson(parsedJson).field("fraudCheckStatus").isEqualTo("FRAUD");
      assertThatJson(parsedJson).field("rejectionReason").isEqualTo("Amount too high");
  }
}
```

一旦您通过并重新运行工件的构建和安装，Spring Cloud Contract Verifier就会将合同转换为HTTP服务器存根定义。 目前我们正在支持[WireMock](http://wiremock.org/)。 存根将出现在stubs/mappings/下的输出文件夹中，如下所示：

```
{
  "uuid" : "6c509a40-18f3-498c-a19c-c9f8b56957de",
  "request" : {
    "url" : "/fraudcheck",
    "method" : "PUT",
    "headers" : {
      "Content-Type" : {
        "equalTo" : "application/vnd.fraud.v1+json"
      }
    },
    "bodyPatterns" : [ {
      "matchesJsonPath" : "$[?(@.loanAmount == 99999)]"
    }, {
      "matchesJsonPath" : "$[?(@.clientId == '1234567890')]"
    } ]
  },
  "response" : {
    "status" : 200,
    "body" : "{\"fraudCheckStatus\":\"FRAUD\",\"rejectionReason\":\"Amount too high\"}",
    "headers" : {
      "Content-Type" : "application/vnd.fraud.v1+json"
    }
  }
}
```

CDC（消费者驱动合同）背后的想法是分享通信双方之间的合同。 Gradle和Maven插件通过使用存根分类器生成带有存根和合约定义的jar来帮助您实现这一点。 只需将其上传到某个中央存储库，其他人可以将其重新用于集成测试。

### Client / Consumer方面

在客户端（HTTP）/使用者（消息传递）方面，它足以为正确的Spring Cloud Contract Stub Runner实现提供依赖性。 在这种情况下，因为我们的示例包含与WireMock的HTTP通信作为HTTP Server Stub。 我们将选择以下依赖项：

*Maven*

```
<dependencyManagement>
  <dependencies>
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-dependencies</artifactId>
      <version>${spring-cloud-dependencies.version}</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
  </dependencies>
</dependencyManagement>

<dependencies>
  <dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-contract-stub-runner</artifactId>
    <scope>test</scope>
  </dependency>
</dependencies>
```

*Gradle*

```
buildscript {
    repositories {
      mavenCentral()
    }
    dependencies {
      classpath "org.springframework.boot:spring-boot-gradle-plugin:${springBootPluginVersion}"
    }
}

apply plugin: 'spring-boot'

dependencyManagement {
    imports {
      mavenBom "org.springframework.cloud:spring-cloud-contract-dependencies:${springCloudDependencies}"
    }
}

dependencies {
    testCompile "org.springframework.cloud:spring-cloud-starter-contract-stub-runner"
}
```

最后一步是在测试中设置Stub Runner以自动下载所需的存根。 要实现这一点，您必须传递@AutoConfigureStubRunner注释。 该注释具有一组您可以设置的属性。 如果您不喜欢这种方法，您也可以在测试属性中设置这些值。


对于Spring Cloud Contract 1.2.x

```java
@RunWith(SpringRunner.class)
@SpringBootTest
@AutoConfigureStubRunner(ids = {"com.example:http-server:+:stubs:8080"}, workOffline = true)
public class LoanApplicationServiceTests {
```

对于从2.0.x开始的Spring Cloud Contract

```java
@RunWith(SpringRunner.class)
@SpringBootTest
@AutoConfigureStubRunner(ids = {"com.example:http-server:+:stubs:8080"}, stubsMode = StubRunnerProperties.StubsMode.LOCAL)
public class LoanApplicationServiceTests {
```

这样，具有组ID com.example的工件，工件id http-server，在最新版本中，具有存根分类器将在端口8080处注册。由于传递了workOffline标志，因此将不从远程存储库下载存根 - 它 将在当地的Maven回购中搜索。 一旦您的测试上下文启动，执行以下代码将不会导致404，因为Spring Cloud Contract Stub Runner将自动启动测试中的WireMock服务器并使用从服务器端生成的存根来提供它。

```java
HttpHeaders httpHeaders = new HttpHeaders();
httpHeaders.add("Content-Type", "application/vnd.fraud.v1+json");
String response = restTemplate.exchange("http://localhost:8080/fraudcheck", HttpMethod.PUT,
            new HttpEntity<>("{\"clientId\":\"1234567890\",\"loanAmount\":99999}", httpHeaders),
            String.class);
assertThat(response).isEqualTo("{\"fraudCheckStatus\":\"FRAUD\",\"rejectionReason\":\"Amount too high\"}");
```

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC3 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/2.1.0.RC3/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/2.1.0.RC3/)|
|----|----|----|
|2.0.3 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-contract/2.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-contract/2.0.x/)|
|2.0.2 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/2.0.2.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/2.0.2.RELEASE/)|
|1.2.6 SNAPSHOT CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-contract/1.2.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-contract/1.2.x/)|
|1.2.5 CURRENT GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/1.2.5.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/1.2.5.RELEASE/)|
|1.1.6 SNAPSHOT|[Reference Doc.](https://cloud.spring.io/spring-cloud-contract/1.1.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-contract/1.1.x/)|
|1.1.5 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/1.1.5.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/1.1.5.RELEASE/)|
|1.0.5 SNAPSHOT|[Reference Doc.](https://cloud.spring.io/spring-cloud-contract/1.0.x/)|[API Doc.](https://cloud.spring.io/spring-cloud-contract/1.0.x/)|
|1.0.4 GA|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/1.0.4.RELEASE/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-contract/1.0.4.RELEASE/)|

# 示例

尝试一些例子：
- [**Spring Cloud Contract Samples (Edgware)**](https://github.com/spring-cloud-samples/spring-cloud-contract-samples)使用Boot 1.5和Edgware版本列的Spring Cloud Contract样本
- [**Spring Cloud Contract Samples (Finchley)**](https://github.com/spring-cloud-samples/spring-cloud-contract-samples/tree/2.0.x)使用Boot 2.x和Finchley发布列表进行Spring Cloud Contract的示例
- [**Spring Cloud Contract workshops**](http://cloud-samples.spring.io/spring-cloud-contract-samples/workshops.html)与Spring Cloud合同的研讨会
- [**Spring Cloud Contract with NodeJS**](https://github.com/spring-cloud-samples/spring-cloud-contract-nodejs)您可以在此处阅读有关Polyglot方法的更多信息https://spring.io/blog/2018/02/13/spring-cloud-contract-in-a-polyglot-world



