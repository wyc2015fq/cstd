# Spring系列学习之Spring Cloud Task微服务上下文运行任务 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 17:19:48[boonya](https://me.csdn.net/boonya)阅读数：869








英文原文：[https://spring.io/projects/spring-cloud-task](https://spring.io/projects/spring-cloud-task)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

[@EnableTask注解](#%40EnableTask%E6%B3%A8%E8%A7%A3)

# 概述

Spring Cloud Task允许用户使用Spring Cloud开发和运行短期微服务，并在云和本地运行，甚至在Spring Cloud Data Flow上运行。只需添加@EnableTask并将您的应用程序作为Spring Boot应用程序（单个应用程序上下文）运行。 如果您不熟悉Spring Cloud Task，请查看我们的[入门文档](https://docs.spring.io/spring-cloud-task/docs/2.0.0.RELEASE/reference/htmlsingle/#getting-started)。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.0.0 CURRENT GA|[Reference Doc.](https://docs.spring.io/spring-cloud-task/docs/2.0.0.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-task/docs/2.0.0.RELEASE/apidocs/)|
|----|----|----|
|2.1.0 M2 PRE|[Reference Doc.](https://docs.spring.io/spring-cloud-task/docs/2.1.0.M2/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-task/docs/2.1.0.M2/apidocs/)|
|1.3.1 GA|[Reference Doc.](https://docs.spring.io/spring-cloud-task/docs/1.3.1.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-task/docs/1.3.1.RELEASE/apidocs/)|
|1.3.0 GA|[Reference Doc.](https://docs.spring.io/spring-cloud-task/docs/1.3.0.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-task/docs/1.3.0.RELEASE/apidocs/)|
|1.2.3 GA|[Reference Doc.](https://docs.spring.io/spring-cloud-task/docs/1.2.3.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-task/docs/1.2.3.RELEASE/apidocs/)|
|1.2.2 GA|[Reference Doc.](https://docs.spring.io/spring-cloud-task/docs/1.2.2.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-task/docs/1.2.2.RELEASE/apidocs/)|
|1.1.2 GA|[Reference Doc.](https://docs.spring.io/spring-cloud-task/docs/1.1.2.RELEASE/reference/htmlsingle/)|[API Doc.](https://docs.spring.io/spring-cloud-task/docs/1.1.2.RELEASE/apidocs/)|

# 示例

要完成我们的应用程序，我们需要创建一个Java文件。 默认情况下，Maven从src / main / java编译源代码，因此您需要创建该文件夹结构。 然后，您需要添加名为src / main / java / com / example / SampleTask.java的文件，如以下示例所示：

```java
package com.example;

import org.springframework.boot.*;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.task.configuration.EnableTask;
import org.springframework.context.annotation.Bean;

@SpringBootApplication
@EnableTask
public class SampleTask {

	@Bean
	public CommandLineRunner commandLineRunner() {
		return new HelloWorldCommandLineRunner();
	}

	public static void main(String[] args) {
		SpringApplication.run(SampleTask.class, args);
	}

	public static class HelloWorldCommandLineRunner implements CommandLineRunner {

		@Override
		public void run(String... strings) throws Exception {
			System.out.println("Hello, World!");
		}
	}
}
```

虽然它可能看起来很小，但仍有相当多的事情发生。 有关Spring Boot细节的更多信息，请参阅[Spring Boot参考文档](https://docs.spring.io/spring-boot/docs/current/reference/html/)。


我们还需要在src / main / resources中创建一个application.properties文件。 我们需要在application.properties中配置两个属性：我们需要设置应用程序名称（转换为任务名称），我们需要将Spring Cloud Task的日志记录设置为DEBUG，以便我们可以看到正在发生的事情。 以下示例显示了如何执行这两个操作：

```
logging.level.org.springframework.cloud.task=DEBUG
spring.application.name=helloWorld
```

## **@EnableTask注解**

我们示例中的第一个非引导注释是@EnableTask注解。这个类级别的注释告诉Spring Cloud Task引导它的功能。默认情况下，它会导入其他配置类（SimpleTaskConfiguration）。此附加配置注册TaskRepository及其使用的基础结构。


开箱即用，TaskRepository使用内存映射来记录任务的结果。对于生产环境而言，Map不是实用的解决方案，因为一旦任务结束，Map就会消失。但是，为了快速入门体验，我们将此作为默认使用，并在日志中回显该存储库中正在更新的内容。在第8章“配置”部分（本文档后面）中，我们将介绍如何自定义Spring Cloud Task提供的部分的配置。


当我们的示例应用程序运行时，Spring Boot启动我们的HelloWorldCommandLineRunner并输出我们的“Hello，World！”消息到标准输出。 TaskLifecyceListener在存储库中记录任务的开始和任务的结束。



