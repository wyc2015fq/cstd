# Spring系列学习之Spring Roo - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 22:22:39[boonya](https://me.csdn.net/boonya)阅读数：287








英文原文：[https://projects.spring.io/spring-roo/](https://projects.spring.io/spring-roo/)

**目录**

[令人兴奋的功能 - Spring Boot，Spring Data，......](#%E4%BB%A4%E4%BA%BA%E5%85%B4%E5%A5%8B%E7%9A%84%E5%8A%9F%E8%83%BD%20-%20Spring%20Boot%EF%BC%8CSpring%20Data%EF%BC%8C......)

[零风险 - 没有妥协的生产力](#%E9%9B%B6%E9%A3%8E%E9%99%A9%20-%20%E6%B2%A1%E6%9C%89%E5%A6%A5%E5%8D%8F%E7%9A%84%E7%94%9F%E4%BA%A7%E5%8A%9B)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[先决条件](#%E5%85%88%E5%86%B3%E6%9D%A1%E4%BB%B6)

[使用Spring Tool Suite（STS）](#%E4%BD%BF%E7%94%A8Spring%20Tool%20Suite%EF%BC%88STS%EF%BC%89)

[运行Roo](#%E8%BF%90%E8%A1%8CRoo)

[从shell运行](#%E4%BB%8Eshell%E8%BF%90%E8%A1%8C)

[Roadmap](#roadmap)

[Sample Projects](#sample-projects)

[External Links](#external-links)

[Books Spring Roo 1.x.](#books-spring-roo-1x)

[Project Lead](#project-lead)

面向Java开发人员的下一代快速应用程序开发工具。 使用Roo，您可以在几分钟内轻松构建完整的Java应用程序。

Spring Roo是一款轻量级的开发工具，可以快速，轻松地提供即时结果。最重要的是，您使用Java编写100％的代码并重用所有现有的Java知识，技能和经验。


Spring Roo与其他生产力工具的不同之处在于：
-     结果快速
-     100％用Java开发
-     使用方便
-     快速，简单的删除
-     完全妥协免费
-     积极，有益的社区
-     综合技术集成
-     通过Roo附加组件扩展点
-     Spring认证的架构

## 令人兴奋的功能 - Spring Boot，Spring Data，......



使用Roo创建Spring Web应用程序比以往更容易，但它也可以简化您的下一代技术之旅。


Spring Roo生成的应用程序专注于更新的Spring技术，如Spring IO平台，Spring Data等。确实，Spring Roo创建了Spring Boot应用程序。


Roo已经支持很酷的功能，如REST API脚手架和SOAP客户端以及SOAP服务生成，以便更轻松地与外部系统集成。此外，我们还有增量数据库内省，Thymeleaf视图，Spring Web Flow，Spring Security以及更积极的开发。

## 零风险 - 没有妥协的生产力

使用Roo，您仍然使用Java编写所有内容。其创新方法完全在编译时运行，并与您的IDE完全兼容。此外，由于没有运行时部分，采用也简化了，风险也很低。从第一天起，所有Java开发人员都会感到舒适并且工作效率很高。 Roo真正代表了生产力而不妥协。


参考指南包括所有功能的详细说明，以及主要用例的详尽用户指南。

## 快速开始

### 先决条件

要开始使用，请确保您具有[所需的系统依赖性](https://docs.spring.io/spring-roo/docs/2.0.x/reference/html/#getting-started-requirements)。

### 使用Spring Tool Suite（STS）

要在STS 3.8.2+上安装Spring Roo 2.0，请阅读参考文档[安装](https://docs.spring.io/spring-roo/docs/2.0.x/reference/html/#getting-started-install-roo)指南。

### 运行Roo

最后，打开STS IDE并[创建Spring Roo项目](https://docs.spring.io/spring-roo/docs/2.0.x/reference/html/#using-spring-roo-create-boot-app)。

### 从shell运行

开始使用spring-roo的推荐方法是在系统上下载和解压缩zip。

Version

Download

2.0.0

[Download](http://spring-roo-repository.springsource.org.s3.amazonaws.com/release/ROO/spring-roo-2.0.0.RELEASE.zip)

大多数时候你会在STS IDE中使用Roo，但有时它会错过从终端运行Roo的可能性。


Roo是一个标准的Java应用程序，完全独立于Roo发行版ZIP中。 因此，如果您愿意，可以从其中一个下载站点下载Roo，并从终端运行它。


下载，解压缩并安装Roo后，请尝试以下步骤来创建应用程序：

```
mkdir hello
cd hello
roo.sh
roo> project setup --topLevelPackage com.foo
roo> jpa setup --provider HIBERNATE --database HYPERSONIC_IN_MEMORY
roo> entity jpa --class ~.domain.Timer
roo> field string --fieldName message --notNull
roo> repository jpa --all
roo> service --all
roo> web mvc setup
roo> web mvc view setup --type THYMELEAF
roo> web mvc controller --all --responseType THYMELEAF
roo> web mvc controller --all --pathPrefix /api
roo> quit
mvn spring-boot:run
```

### Roadmap
- [Looking ahead to Spring Roo 2.0](https://spring.io/blog/2015/03/30/looking-ahead-to-spring-roo-2-0)

### Sample Projects
- [The Northwind application](https://github.com/spring-projects/spring-roo/blob/master/runtime/classpath/src/main/resources/northwind-multimodule.roo)
- [Shop application with REST services](https://github.com/spring-projects/spring-roo/blob/master/runtime/classpath/src/main/resources/restfulshop.roo)
- [The classic Pet Clinic application](https://github.com/spring-projects/spring-roo/blob/master/runtime/classpath/src/main/resources/clinic.roo)

### External Links
- DZone news on [Spring Roo from STS](http://java.dzone.com/articles/working-spring-roo-spring-tool)
- DZone news on [Customizing Spring Roo tagx](https://dzone.com/articles/customizing-spring-roo-tagx)
- DZone news on [Spring Boot Apps Using Spring Roo](https://dzone.com/articles/how-to-generate-spring-boot-applications-using-spr)
- DZone news on ["Spring Roo 2.0.0.M3"](https://dzone.com/articles/spring-roo-200m3-released)

### Books Spring Roo 1.x.
- [Spring Roo in Action](https://www.manning.com/rimple/)
- [Getting Started with Roo](http://shop.oreilly.com/product/0636920020981.do)
- [Spring Roo 1.1 Cookbook](https://oreilly.com/catalog/0636920020981)

### Project Lead
- [DISID Corporation](http://www.disid.com/)



