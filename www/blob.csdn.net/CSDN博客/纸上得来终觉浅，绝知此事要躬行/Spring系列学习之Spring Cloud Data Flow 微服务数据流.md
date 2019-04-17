# Spring系列学习之Spring Cloud Data Flow 微服务数据流 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 21:12:30[boonya](https://me.csdn.net/boonya)阅读数：2562








英文原文：[https://cloud.spring.io/spring-cloud-dataflow/](https://cloud.spring.io/spring-cloud-dataflow/)

**目录**

[Spring Cloud数据流](#Spring%20Cloud%E6%95%B0%E6%8D%AE%E6%B5%81)

[概览](#%E6%A6%82%E8%A7%88)

[社区实现](#%E7%A4%BE%E5%8C%BA%E5%AE%9E%E7%8E%B0)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[构建Spring Spring数据流](#%C2%A0%E6%9E%84%E5%BB%BASpring%20Spring%E6%95%B0%E6%8D%AE%E6%B5%81)

[Sample Projects](#sample-projects)

[Related Projects](#related-projects)

[Customer Case Studies](#customer-case-studies)

[Videos](#videos)

## Spring Cloud数据流

Spring Cloud Data Flow是用于构建数据集成和实时数据处理管道的工具包。


管道由Spring Boot应用程序组成，使用Spring Cloud Stream或Spring Cloud Task微服务框架构建。 这使得Spring Cloud Data Flow适用于各种数据处理用例，从导入/导出到事件流和预测分析。

## 概览

Spring Cloud Data Flow服务器使用[Spring Cloud Deployer](https://github.com/spring-cloud/spring-cloud-deployer/)将数据管道部署到现代运行时，例如Cloud Foundry和Kubernetes。


针对各种数据集成和处理场景的一系列预构建[流](https://cloud.spring.io/spring-cloud-stream-app-starters/)和[任务/批处理](https://cloud.spring.io/spring-cloud-task-app-starters/)启动器应用程序有助于学习和实验。


可以使用熟悉的Spring Boot样式编程模型构建针对不同中间件或数据服务的[自定义](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#custom-applications)流和任务应用程序。


简单的[流管道DSL](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#_stream_dsl)可以轻松指定要部署的应用程序以及如何连接输出和输入。在v1.2中添加了一个新的[组合任务DSL](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#_composed_task_dsl)。

[仪表板](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#dashboard-introduction)提供了一个图形编辑器，用于以交互方式构建新管道，以及可部署应用程序和使用度量标准运行应用程序的视图。


Spring Can Data Flow服务器公开了一个[REST API](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#api-guide-overview)，用于组合和部署数据管道。单独的[shell](https://docs.spring.io/spring-cloud-dataflow/docs/current-SNAPSHOT/reference/htmlsingle/#getting-started-deploying-spring-cloud-dataflow)使您可以从命令行轻松使用API​​。

平台实施


开始使用Spring Cloud Data Flow的一种简单方法是遵循下表中特定于平台的实现链接。每个实现都独立发布，具有独立的发布节奏。强烈建议您查看特定于平台的参考文档，以了解有关功能的更多信息。
|**Server Type**|**Stable Release**|**Milestone/Snapshot Release**|
|----|----|----|
|Local Server|[1.7.3.RELEASE](https://repo.spring.io/release/org/springframework/cloud/spring-cloud-dataflow-server-local/1.7.3.RELEASE/spring-cloud-dataflow-server-local-1.7.3.RELEASE.jar)[[docs](https://docs.spring.io/spring-cloud-dataflow/docs/1.7.3.RELEASE/reference/htmlsingle/#getting-started)]|2.0.0.M1[[docs](https://docs.spring.io/spring-cloud-dataflow/docs/1.7.4.BUILD-SNAPSHOT/reference/htmlsingle/#getting-started)]|
|[Cloud Foundry Server](https://cloud.spring.io/spring-cloud-dataflow-server-cloudfoundry/)|[1.7.3.RELEASE](https://repo.spring.io/release/org/springframework/cloud/spring-cloud-dataflow-server-cloudfoundry/1.7.3.RELEASE/spring-cloud-dataflow-server-cloudfoundry-1.7.3.RELEASE.jar)[[docs](https://docs.spring.io/spring-cloud-dataflow-server-cloudfoundry/docs/1.7.3.RELEASE/reference/htmlsingle/#getting-started)]|2.0.0.M1[[docs](https://docs.spring.io/spring-cloud-dataflow/docs/2.0.0.BUILD-SNAPSHOT/reference/htmlsingle/#getting-started-cloudfoundry)]|
|[Kubernetes Server](https://cloud.spring.io/spring-cloud-dataflow-server-kubernetes/)|[1.7.2.RELEASE](https://repo.spring.io/libs-release/org/springframework/cloud/spring-cloud-dataflow-server-kubernetes/1.7.2.RELEASE/spring-cloud-dataflow-server-kubernetes-1.7.2.RELEASE.jar)[[docs](https://docs.spring.io/spring-cloud-dataflow-server-kubernetes/docs/1.7.2.RELEASE/reference/htmlsingle/#kubernetes-getting-started)]|2.0.0.M1[[docs](https://docs.spring.io/spring-cloud-dataflow/docs/2.0.0.BUILD-SNAPSHOT/reference/htmlsingle/#getting-started-kubernetes)]|



## 社区实现
- [Spring Cloud Data Flow for HashiCorp Nomad](https://github.com/donovanmuller/spring-cloud-dataflow-server-nomad)
- [Spring Cloud Data Flow for Red Hat OpenShift](https://github.com/donovanmuller/spring-cloud-dataflow-server-openshift)
- [Spring Cloud Data Flow for Apache Mesos](https://github.com/trustedchoice/spring-cloud-dataflow-server-mesos)



## 快速开始



第1步 - 有两种[入门](https://docs.spring.io/spring-cloud-dataflow/docs/current/reference/htmlsingle/#getting-started)方式。 最快的是下载Spring Cloud Data Flow Local-Server的Docker Compose工件。 （Mac用户可以使用'curl -O'代替'wget'） 

```
wget https://raw.githubusercontent.com/spring-cloud/spring-cloud-dataflow/v1.7.3.RELEASE/spring-cloud-dataflow-server-local/docker-compose.yml
```

第2步 - 从您下载docker-compose.yml的目录中，启动SCDF系统。

```
DATAFLOW_VERSION=1.7.3.RELEASE docker-compose up
```

第3步 - 在[http:// localhost:9393/dashboard](/dashboard)处打开仪表板。


第4步 - 使用“Streams”选项卡下的“Create Stream（s）”来定义和部署流time| log使用“ticktock”这个名称登录。



![Create TickTock Stream](https://img-blog.csdnimg.cn/20181225211231232)

![Deploy TickTock Stream](https://img-blog.csdnimg.cn/20181225211231348)

一旦部署了“ticktock”流，您将在“运行时”选项卡下看到两个流应用程序（ticktock.log和ticktock.time）。 单击“ticktock.log”应用程序的i图标以复制流式日志的路径。

![Deploy TickTock Stream](https://img-blog.csdnimg.cn/20181225211231365)

第5步 - 要验证已部署的流和结果，请从仪表板中复制“stdout”文本框中的路径。 从另一个终端控制台类型： 

```
docker exec -it dataflow-server tail -f <COPIED-STDOUT-PATH>
```

##  构建Spring Spring数据流

Spring Cloud Data Flow基于多个项目构建，生态系统的顶级构建块列在以下可视化表示中。 每个项目代表一个核心功能，它们是孤立地发展的，具有单独的发布节奏 - 按照链接查找有关每个项目的更多详细信息。


|[Spring Cloud Data Flow Local Server](https://cloud.spring.io/spring-cloud-dataflow/)|[Spring Cloud Data Flow Cloud Foundry Server](https://cloud.spring.io/spring-cloud-dataflow-server-cloudfoundry/)|[Spring Cloud Data Flow Kubernetes Server](https://cloud.spring.io/spring-cloud-dataflow-server-kubernetes/)|[Spring Cloud Data Flow Apache Yarn Server](https://cloud.spring.io/spring-cloud-dataflow-server-yarn/)|[Spring Cloud Data Flow Apache Mesos Server](https://cloud.spring.io/spring-cloud-dataflow-server-mesos/)|
|----|----|----|----|----|
|[REST-APIs / Shell / DSL](https://github.com/spring-cloud/spring-cloud-dataflow/)|[Dashboard](https://github.com/spring-cloud/spring-cloud-dataflow-ui/)|[Spring Flo](https://github.com/spring-projects/spring-flo/)|[Spring Cloud Data Flow Metrics Collector](https://github.com/spring-cloud/spring-cloud-dataflow-metrics-collector)|
|----|----|----|----|
|[Spring Cloud Data Flow - Core](https://github.com/spring-cloud/spring-cloud-dataflow/)| | | |

↓     *Uses*     ↓
|[Spring Cloud Deployer - Service Provider Interface (SPI)](https://github.com/spring-cloud/spring-cloud-deployer/)|
|----|

↑     *Implements*     ↑
|[Spring Cloud Deployer Local](https://github.com/spring-cloud/spring-cloud-deployer-local/)|[Spring Cloud Deployer Cloud Foundry](https://github.com/spring-cloud/spring-cloud-deployer-cloudfoundry/)|[Spring Cloud Deployer Kubernetes](https://github.com/spring-cloud/spring-cloud-deployer-kubernetes/)|[Spring Cloud Deployer Yarn](https://github.com/spring-cloud/spring-cloud-deployer-yarn/)|[Spring Cloud Deployer Mesos](https://github.com/spring-cloud/spring-cloud-deployer-mesos/)|
|----|----|----|----|----|

↓     *Deploys*     ↓
|[Spring Cloud Stream App Starters](https://cloud.spring.io/spring-cloud-stream-app-starters/)|[Spring Cloud Task App Starters](https://cloud.spring.io/spring-cloud-task-app-starters/)|
|----|----|
|[Spring Cloud Stream](https://cloud.spring.io/spring-cloud-stream/)|[Spring Cloud Task](https://cloud.spring.io/spring-cloud-task/)|
|----|----|

↓     *Uses*     ↓
|[Spring Integration](https://projects.spring.io/spring-integration/)|[Spring Boot](https://projects.spring.io/spring-boot/)|[Spring Batch](https://projects.spring.io/spring-batch/)|
|----|----|----|

## Sample Projects
- [Twitter Analytics](https://docs.spring.io/spring-cloud-dataflow-samples/docs/current/reference/htmlsingle/#_analytics)
- [Predictive Analytics](https://docs.spring.io/spring-cloud-dataflow-samples/docs/current/reference/htmlsingle/#_data_science)
- [HTTP -> Cassandra](https://docs.spring.io/spring-cloud-dataflow-samples/docs/current/reference/htmlsingle/#spring-cloud-data-flow-samples-http-cassandra-overview)
- [HTTP -> MySQL](https://docs.spring.io/spring-cloud-dataflow-samples/docs/current/reference/htmlsingle/#_http_to_mysql_demo)
- [HTTP -> Gemfire](https://docs.spring.io/spring-cloud-dataflow-samples/docs/current/reference/htmlsingle/#_http_to_gemfire_demo)
- [Functions in SCDF](https://docs.spring.io/spring-cloud-dataflow-samples/docs/current/reference/htmlsingle/#_functions)

## Related Projects
- [Spring Cloud Stream](https://cloud.spring.io/spring-cloud-stream/)
- [Spring Cloud Stream Applications](https://cloud.spring.io/spring-cloud-stream-app-starters/)
- [Spring Cloud Task](https://cloud.spring.io/spring-cloud-task/)
- [Spring Cloud Task Applications](https://cloud.spring.io/spring-cloud-task-app-starters/)
- [Spring Cloud Skipper](https://cloud.spring.io/spring-cloud-skipper/)

## Customer Case Studies
- [Latency Tracing with SCDF](https://content.pivotal.io/springone-platform-2017/latency-and-event-tracing-with-spring-cloud-data-flow-henri-van-den-bulk-craig-duero-tom-martin)
- [Batch Processing with SCDF](https://content.pivotal.io/springone-platform-2017/case-study-of-batch-processing-with-spring-cloud-data-flow-server-in-cloud-foundry-bruce-thelen)
- [Low Latency Event-Driven ETL](https://springoneplatform.io/sessions/rdbms-and-apache-geode-data-movement-low-latency-etl-pipeline-by-using-cloud-native-event-driven-microservices)
- [Domain Driven Design with Spring Cloud Stream](https://content.pivotal.io/springone-platform-2017/deconstructing-monoliths-with-domain-driven-design-rohit-kelapure-david-turanski-rohit-sood)

## Videos
- [CI/CD + SCDF](https://content.pivotal.io/springone-platform-2017/orchestrating-data-microservices-with-spring-cloud-data-flow-mark-pollack)
- [Twitter Sentiment Analytics with TensorFlow](https://www.youtube.com/watch?v=QzF2Iii4s5c)
- [Data Microservices with Spring Cloud Data Flow](https://www.youtube.com/watch?v=Z6_ttC33N0U)
- [Data Microservices in the Cloud](https://www.youtube.com/watch?v=H-T-0WdZ3Sk&index=174&list=PLAdzTan_eSPQ1fuLSBhyB4eEZF7JQM0Mx)
- [Cloud Native Streaming and Event-Driven Microservices](https://www.youtube.com/watch?v=s6mP4lpkxvg&app=desktop)
- [Data Microservices with Spring Cloud Stream, Task, and Spring Cloud Data Flow](https://vimeo.com/172808512)





