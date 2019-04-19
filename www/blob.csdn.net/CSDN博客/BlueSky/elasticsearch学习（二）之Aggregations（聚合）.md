# elasticsearch学习（二）之Aggregations（聚合） - BlueSky - CSDN博客
2015年12月04日 20:13:37[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：4436
## 基本概念
 Elasticsearch有一个功能叫做聚合(aggregations)，它允许你在数据上生成复杂的分析统计。它很像SQL中的
```
GROUP
 BY
```
但是功能更强大。
### 官方java-api原文 
地址：https://www.elastic.co/guide/en/elasticsearch/client/java-api/1.7/java-aggs.html
There are many different types of aggregations, each with its own purpose and output. To better understand these types, it is often easier to break them into two main families:
- Bucketing
A family of aggregations that build buckets, where each bucket is associated with a key and
 a document criterion. When the aggregation is executed, all the buckets criteria are evaluated on every document in the context and when a criterion matches, the document is considered to "fall in" the relevant bucket. By the end of the aggregation process,
 we’ll end up with a list of buckets - each one with a set of documents that "belong" to it.- Metric
Aggregations that keep track and compute metrics over a set of documents.
The interesting part comes next. Since each bucket effectively defines a document set (all documents belonging to the bucket), one can potentially associate aggregations on the bucket level, and those will execute within the context of that bucket. This is
 where the real power of aggregations kicks in:
### 对应译文
有许多不同类型的聚合的，每个都有其自己的目的和输出。为了更好地理解这些类型的，它通常更容易将它们分解为两个主要的家族：
大雨（大量桶）--（这个真不知道如何译好）
一个家庭聚集的建立桶，每个桶都与一个键和一个文档标准相关联的。当聚合被执行时，在每一个文档的上下文中所有的桶的标准都被评估，如果标准匹配那么就认为将这个文档扔进这个相关的同种进行处理。等到聚合进程全部结束，我们将会获得一的桶的清单--其中每一个桶包含着一系列的属于这个桶的文档。
度量
聚集跟踪和度量一系列的文档。有趣的部分随之而来。由于每一个桶被有效的定义为一个文档集（所有的文档属于这个桶），我们就可以潜在地在桶的级别上联系聚合，同时这些将会被执行在桶的环境之下。这就是聚合的真正力量所在：聚合可以被嵌套!
## Metric Aggregation（聚合操作）
Es的java api支持很多种的聚合操作，也就是大部分的搜索相关的操作都可以通过aggregation的组合来进行操作，相当于普通关系型数据库的group by的操作。
其中Metric Aggregation包括很多的聚合类型，可以参照官方的api，地址：https://www.elastic.co/guide/en/elasticsearch/client/java-api/1.7/_metrics_aggregations.html#_metrics_aggregations
主要说一下其中一个不太好理解的一个：
### [Percentile Aggregation](http://www.elastic.co/guide/en/elasticsearch/reference/1.7/search-aggregations-metrics-percentile-aggregation.html)
Here is an example on how to create the aggregation request:
```java
MetricsAggregationBuilder aggregation =
        AggregationBuilders
                .percentiles("agg")
                .field("height");
```
You can provide your own percentiles instead of using defaults:
```java
MetricsAggregationBuilder aggregation =
        AggregationBuilders
                .percentiles("agg")
                .field("height")
                .percentiles(1.0, 5.0, 10.0, 20.0, 30.0, 75.0, 95.0, 99.0);
```
#### [](https://www.elastic.co/guide/en/elasticsearch/client/java-api/1.7/_metrics_aggregations.html#_use_aggregation_response_8)Use aggregation response[edit](https://github.com/elastic/elasticsearch/edit/1.7/docs/java-api/aggregations/metrics/percentile-aggregation.asciidoc)
Import Aggregation definition classes:
```java
import org.elasticsearch.search.aggregations.metrics.percentiles.Percentile;
import org.elasticsearch.search.aggregations.metrics.percentiles.Percentiles;
```
```java
// sr is here your SearchResponse object
Percentiles agg = sr.getAggregations().get("agg");
// For each entry
for (Percentile entry : agg) {
    double percent = entry.getPercent();    // Percent
    double value = entry.getValue();        // Value
    logger.info("percent [{}], value [{}]", percent, value);
}
```
This will basically produce for the first example:
percent [1.0], value [0.814338896154595]
percent [5.0], value [0.8761912455821302]
percent [25.0], value [1.173346540141847]
percent [50.0], value [1.5432023318692198]
percent [75.0], value [1.923915462033674]
percent [95.0], value [2.2273644908535335]
percent [99.0], value [2.284989339108279]
以上截取的官方文档，主要是说明下其返回结果的意义：
如上所示：percent代表的百分比，本以为这是个很简单的东东，但是才发现这个percentiles实现的背后是用的一个比较复杂的算法--
# [digest](https://github.com/tdunning/t-digest)具体可参考文章：https://github.com/tdunning/t-digest/blob/master/docs/t-digest-paper/histo.pdf
经过自己的研究和测试，目前发现这个的作用就是对观测field的一种统计，比如可以取10%对应的具体的值，也可以参照其分布了解数据的大致分布，比如下面的所示，可以看到大部分数据在15-29之间，但也会偶然陡峭到60-150
整体来说，没有弄太明白它的percentiles的作用和具体意义，但是据说作用很大，以后用到再好好研究啦
```
"values" : {
            "1.0": 15,
            "5.0": 20,
            "25.0": 23,
            "50.0": 25,
            "75.0": 29,
            "95.0": 60,
            "99.0": 150
         }
```
