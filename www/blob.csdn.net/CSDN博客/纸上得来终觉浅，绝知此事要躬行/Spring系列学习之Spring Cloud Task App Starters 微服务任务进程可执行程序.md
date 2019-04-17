# Spring系列学习之Spring Cloud Task App Starters 微服务任务进程可执行程序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 17:51:10[boonya](https://me.csdn.net/boonya)阅读数：248








英文原文：[http://cloud.spring.io/spring-cloud-task-app-starters/](http://cloud.spring.io/spring-cloud-task-app-starters/)

**目录**

[Spring Cloud Task App Starters](#Spring%20Cloud%20Task%20App%20Starters)

[特性](#%E7%89%B9%E6%80%A7)

[可用应用](#%E5%8F%AF%E7%94%A8%E5%BA%94%E7%94%A8)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[任务应用程序启动器和Spring云数据流（**）](#%E4%BB%BB%E5%8A%A1%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%90%AF%E5%8A%A8%E5%99%A8%E5%92%8CSpring%E4%BA%91%E6%95%B0%E6%8D%AE%E6%B5%81%EF%BC%88**%EF%BC%89)

[HTTP Repository Location for Apps](#http-repository-location-for-apps)

[发布版本](#%E5%8F%91%E5%B8%83%E7%89%88%E6%9C%AC)

[相关项目](#%E7%9B%B8%E5%85%B3%E9%A1%B9%E7%9B%AE)

## Spring Cloud Task App Starters



Spring Cloud Task Application Starters是Spring Boot应用程序，可能是任何进程，包括不能永久运行的Spring Batch作业，并且它们在某些时候结束/停止。 Spring Cloud Task Applications可与Spring Cloud Data Flow一起使用，以创建，部署和编排短期数据微服务。

Spring Cloud Task Application Starters是独立的可执行应用程序，可用于按需用例，例如数据库迁移，机器学习和计划操作。 这些应用程序可以在各种运行时平台上独立运行，包括：Cloud Foundry，Apache Yarn，Apache Mesos，Kubernetes，Docker，甚至可以在您的笔记本电脑上运行。

## 特性
-      独立运行作为Spring Boot应用程序
-      编排为短暂的数据微服务
-      将数据微服务应用程序用作maven或docker工件
-      通过命令行，环境变量或YAML文件覆盖配置参数
-      提供基础架构以单独测试应用程序
-      从此版本的Spring Initializr下载为初学者

## 可用应用
|**Task**|
|----|
|composed-task-runner|
|timestamp|
|timestamp-batch|

## 快速开始



第1步 - 从此处下载最新的时间戳应用程序[例如：/1.3.0.RELEASE/timestamp-task-1.3.0.RELEASE.jar]


第2步 - 运行时间戳应用程序

```
java -jar timestamp-task-***.jar
```

第3步 - 在控制台中验证时间戳日志

第4步 - 验证时间戳应用程序是否已关闭



## 任务应用程序启动器和Spring云数据流（**）

**With Spring Boot 1.5.x + Spring Cloud Task 1.2.x:**
|**Artifact Type**|**Stable Release**|**Milestone/Snapshot Release**|
|----|----|----|
|Maven|http://bit.ly/Clark-SR1-task-applications-maven|http://bit.ly/Clark-BUILD-SNAPSHOT-task-applications-maven|
|Docker|http://bit.ly/Clark-SR1-task-applications-docker|http://bit.ly/Clark-BUILD-SNAPSHOT-task-applications-docker|

**With Spring Boot 2.0.x + Spring Cloud Task 2.0.x:**
|**Artifact Type**|**Stable Release**|**Milestone/Snapshot Release**|
|----|----|----|
|Maven|http://bit.ly/Dearborn-SR1-task-applications-maven|http://bit.ly/Dearborn-BUILD-SNAPSHOT-task-applications-maven|
|Docker|http://bit.ly/Dearborn-SR1-task-applications-docker|http://bit.ly/Dearborn-BUILD-SNAPSHOT-task-applications-docker|

**With Spring Boot 2.1.x + Spring Cloud Task 2.1.x:**
|**Artifact Type**|**Milestone Release**|**Snapshot Release**|
|----|----|----|
|Maven|http://bit.ly/Elston-M1-task-applications-maven|http://bit.ly/Elston-BUILD-SNAPSHOT-task-applications-maven|
|Docker|http://bit.ly/Elston-M1-task-applications-docker|http://bit.ly/Elston-BUILD-SNAPSHOT-task-applications-docker|

（**）通过查看参考指南中的[app-registration](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#_registering_a_task_application)部分，了解如何在Spring Cloud Data Flow中使用app-starters 

## HTTP Repository Location for Apps

在这里，您可以找到任务应用程序的uber jar的直接链接。

With Spring Boot 1.5.x + Spring Cloud Task 1.2.x:
- [Stable Release](https://repo.spring.io/libs-release/org/springframework/cloud/task/app/spring-cloud-task-app-descriptor/Clark.RELEASE/spring-cloud-task-app-descriptor-Clark.RELEASE.task-apps-maven-repo-url.properties)
- [Latest Snapshot/Milestone](https://repo.spring.io/libs-snapshot/org/springframework/cloud/task/app/spring-cloud-task-app-descriptor/Clark.BUILD-SNAPSHOT/spring-cloud-task-app-descriptor-Clark.BUILD-SNAPSHOT.task-apps-maven-repo-url.properties)

With Spring Boot 2.0.x + Spring Cloud Task 2.0.x:
- [Stable Release](https://repo.spring.io/libs-release-local/org/springframework/cloud/task/app/spring-cloud-task-app-descriptor/Dearborn.SR1/spring-cloud-task-app-descriptor-Dearborn.SR1.task-apps-maven-repo-url.properties)
- [Latest Snapshot/Milestone](https://repo.spring.io/libs-snapshot-local/org/springframework/cloud/task/app/spring-cloud-task-app-descriptor/Dearborn.BUILD-SNAPSHOT/spring-cloud-task-app-descriptor-Dearborn.BUILD-SNAPSHOT.task-apps-maven-repo-url.properties)

With Spring Boot 2.1.x + Spring Cloud Task 2.1.x:
- [Milestone Release](https://repo.spring.io/libs-snapshot/org/springframework/cloud/task/app/spring-cloud-task-app-descriptor/Elston.M1/spring-cloud-task-app-descriptor-Elston.M1.task-apps-maven-repo-url.properties)
- [Latest Snapshot](https://repo.spring.io/libs-snapshot/org/springframework/cloud/task/app/spring-cloud-task-app-descriptor/Elston.BUILD-SNAPSHOT/spring-cloud-task-app-descriptor-Elston.BUILD-SNAPSHOT.task-apps-maven-repo-url.properties)



## 发布版本

Spring Cloud Task App Starters

Release

Documentation

Elston M1

[Reference](http://docs.spring.io/spring-cloud-task-app-starters/docs/Elston.M1/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-task-app-starters/docs/Elston.M1/api/)

Elston

[Reference](http://docs.spring.io/spring-cloud-task-app-starters/docs/Elston.BUILD-SNAPSHOT/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-task-app-starters/docs/Elston.BUILD-SNAPSHOT/api/)

Dearborn SR1

[Reference](http://docs.spring.io/spring-cloud-task-app-starters/docs/Dearborn.SR1/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-task-app-starters/docs/Dearborn.SR1/api/)

Clark

[Reference](http://docs.spring.io/spring-cloud-task-app-starters/docs/Clark.RELEASE/reference/htmlsingle)[API](http://docs.spring.io/spring-cloud-task-app-starters/docs/Clark.RELEASE/api/)

## 相关项目
- [Spring Cloud Task](https://github.com/spring-cloud/spring-cloud-task/)
- [Spring Cloud Data Flow](https://cloud.spring.io/spring-cloud-dataflow/)
- [Spring Cloud Stream](https://cloud.spring.io/spring-cloud-stream/)



