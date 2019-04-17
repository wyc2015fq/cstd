# Elasticsearch搜索服务器了解和安装 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月07日 17:11:10[boonya](https://me.csdn.net/boonya)阅读数：218








**目录**

[官方下载地址](#%E5%AE%98%E6%96%B9%E4%B8%8B%E8%BD%BD%E5%9C%B0%E5%9D%80)

[安装步骤说明](#%E5%AE%89%E8%A3%85%E6%AD%A5%E9%AA%A4%E8%AF%B4%E6%98%8E)

[Windows安装](#Windows%E5%AE%89%E8%A3%85)

[Linux安装](#Linux%E5%AE%89%E8%A3%85)

[学习进阶](#%E5%AD%A6%E4%B9%A0%E8%BF%9B%E9%98%B6)

[应用场景](#%E5%BA%94%E7%94%A8%E5%9C%BA%E6%99%AF)

ElasticSearch是一个基于Lucene的搜索服务器。它提供了一个分布式多用户能力的全文搜索引擎，基于RESTful web接口。Elasticsearch是用Java开发的，并作为Apache许可条款下的开放源码发布，是当前流行的企业级搜索引擎。设计用于[云计算](https://baike.baidu.com/item/%E4%BA%91%E8%AE%A1%E7%AE%97/9969353)中，能够达到实时搜索，稳定，可靠，快速，安装使用方便。

我们建立一个网站或应用程序，并要添加搜索功能，但是想要完成搜索工作的创建是非常困难的。我们希望搜索解决方案要运行速度快，我们希望能有一个零配置和一个完全免费的搜索模式，我们希望能够简单地使用JSON通过HTTP来索引数据，我们希望我们的搜索服务器始终可用，我们希望能够从一台开始并扩展到数百台，我们要实时搜索，我们要简单的多租户，我们希望建立一个云的解决方案。因此我们利用Elasticsearch来解决所有这些问题及可能出现的更多其它问题。

![](https://img-blog.csdn.net/20180907164349931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 官方下载地址

[https://www.elastic.co/downloads/elasticsearch](https://www.elastic.co/downloads/elasticsearch)

下载最新版本。（本文示例版本为6.4.0）

### 安装步骤说明

安装前提：服务器必须安装了JDK。

### ![](https://img-blog.csdn.net/20180907164637210?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### Windows安装

1、解压elasticsearch-6.4.0.zip

2、进入解压目录下的\elasticsearch-6.4.0\bin目录，双击elasticsearch.bat启动。

![](https://img-blog.csdn.net/20180907165231339?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3、验证是否正常启动，访问[http://localhost:9200/](http://localhost:9200/)

![](https://img-blog.csdn.net/20180907165332884?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

当然可以直接在控制台命令访问：

![](https://img-blog.csdn.net/2018090717281874?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4、默认情况下，Elastic 只允许本机访问，如果需要远程访问，需修改解压路径下\elasticsearch-6.4.0\config\elasticsearch.yml
`network.host: 0.0.0.0`
### Linux安装

 1、解压elasticsearch-6.4.0.tar.gz
`$ tar zxvf elasticsearch-6.4.0.tar.gz`
2、进入elasticsearch-6.4.0目录并启动

```
$ cd elasticsearch-6.4.0
$ ./bin/elasticsearch
```

 3、如果一切正常，Elastic 就会在默认的9200端口运行。这时，打开另一个命令行窗口，请求该端口，会得到说明信息。

```
$ curl http://localhost:9200 {
  "name" : "NHfd20Y",
  "cluster_name" : "elasticsearch",
  "cluster_uuid" : "bwdA5W3fQy63U_ywWbV3GQ",
  "version" : {
    "number" : "6.4.0",
    "build_flavor" : "default",
    "build_type" : "zip",
    "build_hash" : "595516e",
    "build_date" : "2018-08-17T23:18:47.308994Z",
    "build_snapshot" : false,
    "lucene_version" : "7.4.0",
    "minimum_wire_compatibility_version" : "5.6.0",
    "minimum_index_compatibility_version" : "5.0.0"
  },
  "tagline" : "You Know, for Search"
}
```

4、默认情况下，Elastic 只允许本机访问，如果需要远程访问，需修改$ elasticsearch-6.4.0\config\elasticsearch.yml，如下：
`network.host: 0.0.0.0`
### 学习进阶

[https://www.elastic.co/guide/en/elasticsearch/reference/current/getting-started.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/getting-started.html)

Elasticsearch是一个高度可扩展的开源全文搜索和分析引擎。它允许您快速，近实时地存储，搜索和分析大量数据。它通常用作底层引擎/技术，为具有复杂搜索功能和要求的应用程序提供支持。

教程的其余部分中，将引导您完成启动和运行Elasticsearch，查看其中的内容以及执行索引，搜索和修改数据等基本操作的过程。在本教程结束时，您应该很好地了解Elasticsearch是什么，它是如何工作的，并希望能够启发它，看看如何使用它来构建复杂的搜索应用程序或从数据中挖掘智能。

### 应用场景

以下是Elasticsearch可用于的一些示例用例：
- 您运行在线网上商店，允许您的客户搜索您销售的产品。在这种情况下，您可以使用Elasticsearch存储整个产品目录和库存，并为它们提供搜索和自动填充建议。
- 您希望收集日志或交易数据，并且希望分析和挖掘此数据以查找趋势，统计信息，摘要或异常。在这种情况下，您可以使用Logstash（Elasticsearch / Logstash / Kibana堆栈的一部分）来收集，聚合和解析数据，然后让Logstash将此数据提供给Elasticsearch。一旦数据在Elasticsearch中，您就可以运行搜索和聚合来挖掘您感兴趣的任何信息。
-  您运行价格警报平台，允许精通价格的客户指定一条规则，例如“我有兴趣购买特定的电子产品，如果小工具的价格在下个月内从任何供应商降至X美元以下，我希望收到通知” 。在这种情况下，您可以刮取供应商价格，将其推入Elasticsearch并使用其反向搜索（Percolator）功能来匹配价格变动与客户查询，并最终在发现匹配后将警报推送给客户。
- 您有分析/业务智能需求，并希望快速调查，分析，可视化并询问有关大量数据的特定问题（想想数百万或数十亿条记录）。在这种情况下，您可以使用Elasticsearch存储数据，然后使用Kibana（Elasticsearch / Logstash / Kibana堆栈的一部分）构建自定义仪表板，以便可视化对您来说重要的数据方面。此外，您可以使用Elasticsearch聚合功能针对您的数据执行复杂的商业智能查询。



