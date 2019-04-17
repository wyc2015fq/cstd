# Spring系列学习之Spring Cloud Pipelines微服务管道 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 16:05:37[boonya](https://me.csdn.net/boonya)阅读数：245标签：[Spring																[Cloud																[Pipelines](https://so.csdn.net/so/search/s.do?q=Pipelines&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)





英文原文：[https://spring.io/projects/spring-cloud-pipelines](https://spring.io/projects/spring-cloud-pipelines)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[特性](#%E7%89%B9%E6%80%A7)

[Spring Boot配置](#Spring%20Boot%E9%85%8D%E7%BD%AE)

[运行Cloud Foundry的演示](#%E8%BF%90%E8%A1%8CCloud%20Foundry%E7%9A%84%E6%BC%94%E7%A4%BA)

[Concourse](#Concourse)

[Jenkins](#Jenkins)

[根据您的需求调整管道](#%E6%A0%B9%E6%8D%AE%E6%82%A8%E7%9A%84%E9%9C%80%E6%B1%82%E8%B0%83%E6%95%B4%E7%AE%A1%E9%81%93)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

[示例](#%E7%A4%BA%E4%BE%8B)

# 概述

**重要：**该项目已弃用。它被迁移到：[Cloud Pipelines](https://github.com/CloudPipelines/)。你可以在[这里](https://spring.io/blog/2018/11/13/spring-cloud-pipelines-to-cloud-pipelines-migration)读更多关于它的内容。


Spring，Spring Boot和Spring Cloud是允许开发人员加快创建新业务功能的工具。然而，众所周知，该功能只有在生产中才有价值。这就是为什么公司花费大量时间和资源来构建自己的部署管道。


该项目试图解决以下问题：
-     创建公共部署管道
-     传播良好的测试和部署实践
-     加快将功能部署到生产所需的时间



运行，配置和部署应用程序的常用方法可以降低新开发人员在更改项目时融入的支持成本和时间。


目前，我们支持开箱即用的以下CI / CD系统
-     Concourse
-     Jenkins

## 特性

Spring Cloud Pipelines具有以下功能
-     Concourse的固定微服务管道
-     使用Job DSL插件的Jenkins自以为是的微服务管道
-     Jenkins使用Declarative Pipeline（Jenkinsfile）的固定微服务管道
-     Cloud Foundry PaaS支持
-     Kubernetes PaaS支持



固定的管道看起来像这样：
*Concourse流程*

![](https://img-blog.csdnimg.cn/20181225154015805.png)

*Jenkins流程*

![](https://img-blog.csdnimg.cn/20181225154227849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)



## Spring Boot配置


**重要：**该项目已弃用。它被迁移到：[Cloud Pipelines](https://github.com/CloudPipelines/)。你可以在[这里](https://spring.io/blog/2018/11/13/spring-cloud-pipelines-to-cloud-pipelines-migration)读更多关于它的内容。

### 运行Cloud Foundry的演示

您可以通过运行默认的演示设置轻松启动，使用Docker / PCF Dev或Minikube在本地运行整个系统。


我们正在提供一个将构建和部署的演示管道--Github Webhook应用程序。


总而言之，有以下项目参与整个微服务设置。
- [Github Analytics](https://github.com/spring-cloud-samples/github-analytics) - 具有REST端点并使用消息传递的应用程序。我们的业务应用。
- [Github Webhook](https://github.com/spring-cloud-samples/github-webhook) - 发出Github Analytics使用的消息的项目。我们的业务应用。
- [Eureka](https://github.com/spring-cloud-samples/github-eureka) - 简单的Eureka服务器。这是基础架构应用程序。
- [Github Analytics Stub Runner Boot](https://github.com/spring-cloud-samples/github-analytics-stub-runner-boot) - Stub Runner引导服务器，用于使用Github Analytics进行测试。使用Eureka和Messaging。这是基础架构应用程序。



有关详细信息，请查看文档。

### Concourse
-     分叉[Github Webhook](https://github.com/spring-cloud-samples/github-webhook)和[https://github.com/spring-cloud-samples/github-analytics](https://github.com/spring-cloud-samples/github-analytics) [Github Analytics]存储库
-     启动[PCF Dev](https://pivotal.io/platform/pcf-tutorials/getting-started-with-pivotal-cloud-foundry-dev/introduction)
-     克隆spring-cloud-pipelines项目
-     去concourse文件夹
-     运行./setup-docker-compose.sh为Concourse设置密钥
-     运行./start.sh 192.168.99.100以启动Concourse和Artifactory（例如，对于192.168.99.100是您的docker映像的外部IP。如果您没有Docker Machine，您可以运行./what_my_ip.sh脚本并传递该值这里）
-     运行./deploy_infra.sh将基础设施应用程序（eureka和stub runner）上传到Artifactory
-     运行./login.sh 192.168.99.100登录到您的大厅（其中192.168.99.100是您的Docker Machine ip - 如果您使用的是localhost，则不必提供该值）
-     根据提供的credentials-sample-cf.yml设置您的credentials.yml（指向您的fork [Github Webhook]（[https://github.com/spring-cloud-samples/github-webhook](https://github.com/spring-cloud-samples/github-webhook))））
-     运行./set-pipeline.sh为github-webhook设置管道
-     在Concourse中取消暂停并运行管道！
-     管道通过后，您可以为github-analytics设置新管道



对于Kubernetes一步一步的指南，请查看[文档](https://cloud.spring.io/spring-cloud-pipelines/single/spring-cloud-pipelines.html#jenkins-pipeline-k8s)

### Jenkins
-     分享[Github Webhook](https://github.com/spring-cloud-samples/github-webhook)和[Github Analytics](https://github.com/spring-cloud-samples/github-analytics)存储库
-     启动[PCF Dev](https://pivotal.io/platform/pcf-tutorials/getting-started-with-pivotal-cloud-foundry-dev/introduction)
-     克隆spring-cloud-pipelines项目
-     转到jenkins文件夹
-     运行./start.sh yourGithubUser yourGithubPass yourGithubOrg启动Jenkins和Artifactory（yourGithubUser和yourGithubPass是管道所需的Github凭证，用于标记存放在yourGithubOrg中的分叉仓库;如果您更喜欢使用密钥，也可以传递-key path_to_private_key基于Git存储库的身份验证）
-     运行./deploy_infra.sh将基础设施应用程序（eureka和stub runner）上传到Artifactory
-     转到Jenkins并单击jenkins-pipeline-seed以生成管道作业
-     转到github-webhook管道视图，然后单击播放-play按钮以运行管道



对于Kubernetes一步一步的指南，请查看[文档](https://cloud.spring.io/spring-cloud-pipelines/single/spring-cloud-pipelines.html#jenkins-pipeline-k8s)

### 根据您的需求调整管道

可以将此存储库视为管道的模板。我们提供一些自以为是的实施，您可以根据自己的需要进行调整。使用它来构建生产项目的最佳方法是将Spring Cloud Pipelines存储库下载为ZIP，然后在那里初始化一个Git项目并根据需要进行修改。


使用master分支代码的示例。您可以使用标签，例如1.0.0.M8。然后您的网址将如下所示`[https://github.com/spring-cloud/spring-cloud-pipelines/archive/1.0.0.M8.zip](https://github.com/spring-cloud/spring-cloud-pipelines/archive/1.0.0.M8.zip)` .

```
$ curl -LOk https://github.com/spring-cloud/spring-cloud-pipelines/archive/master.zip
$ unzip master.zip
$ cd spring-cloud-pipelines-master
$ git init
$ ./gradlew customize
$ # modify the pipelines to suit your needs
$ git add .
$ git commit -m "Initial commit"
$ git remote add origin ${YOUR_REPOSITORY_URL}
$ git push origin master
```

您还可以克隆存储库，以防您希望与上游存储库中的更改保持一致。 为了避免产生很多合并冲突，我们鼓励使用自定义文件夹挂钩来覆盖函数。

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|1.0.0 M8 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-pipelines/1.0.0.M8/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-pipelines/1.0.0.M8/)|
|----|----|----|
|1.0.0 SNAPSHOT|[Reference Doc.](https://cloud.spring.io/spring-cloud-pipelines/spring-cloud-pipelines.html)|[API Doc.](https://cloud.spring.io/spring-cloud-pipelines/spring-cloud-pipelines.html)|



# 示例

尝试一些例子：
- [**Github Analytics**](https://github.com/spring-cloud-samples/github-analytics)具有REST端点并使用消息传递的应用程序。 我们的业务应用。
- [**Github Webhook**](https://github.com/spring-cloud-samples/github-webhook)发出Github Analytics使用的消息的项目。 我们的业务应用。
- [**Github Analytics Kubernetes**](https://github.com/spring-cloud-samples/github-analytics-kubernetes)具有REST端点并使用消息传递的应用程序。 我们的业务应用。
- [**Github Webhook Kubernetes**](https://github.com/spring-cloud-samples/github-webhook-kubernetes)发出Github Analytics使用的消息的项目。 我们的业务应用。
- [**Eureka**](https://github.com/spring-cloud-samples/github-eureka)简单的Eureka服务器。 这是基础架构应用程序。
- [**Github Analytics Stub Runner Boot**](https://github.com/spring-cloud-samples/github-analytics-stub-runner-boot)Stub Runner引导服务器，用于模拟Github Analytics依赖项
- [**DDD By Examples - factory**](https://github.com/ddd-by-examples/factory)通过Spring Cloud Pipelines部署到生产环境的单片应用程序](https://so.csdn.net/so/search/s.do?q=Cloud&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)




