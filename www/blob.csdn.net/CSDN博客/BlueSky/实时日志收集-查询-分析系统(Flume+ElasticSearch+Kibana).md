# 实时日志收集-查询-分析系统(Flume+ElasticSearch+Kibana) - BlueSky - CSDN博客
2016年03月29日 09:44:25[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1525
# 实时日志收集，查询，分析系统
设计方案：Flume（日志收集） +　ElasticSearch（日志查询）+ Kibana（日志分析与展示）
实验使用场景：通过ambari部署集群后，可以添加自己的日志系统，记录每个组件的产生的日志，实时的查询分析
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E4%B8%80%E3%80%81Flume%E6%A6%82%E8%BF%B0%EF%BC%9A)一、Flume概述：
Apache Flume is a distributed, reliable, and available system for efficiently collecting, aggregating and moving large amounts of log data from many different sources to a centralized data store.
The use of Apache Flume is not only restricted to log data aggregation. Since data sources are customizable, Flume can be used to transport massive quantities of event data including but not limited to network traffic data, social-media-generated
 data, email messages and pretty much any data source possible.
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E4%BA%8C%E3%80%81Flume%E6%9E%B6%E6%9E%84)二、Flume架构
每一个Flume agent包含三种类型的组件：source（从数据源获取生成event data），channel（接收source给put来的event data），sink（从channel取走event data）
注意上面写的是一个flume agent包含三种而不是三个
![flume-arch](http://7xlhna.com1.z0.glb.clouddn.com/flume_arch.png)flume-arch
解释下什么是event data？
官方解释：A Flume event is defined as a unit of data flow having a byte payload and an optional set of string attributes.
简单理解：flume event data = headers + body，其中body的类型是byte[]，headers的类型是Map，event代表着一个数据流的最小完整单元，如果是source是从文本文件中读数据，那event的body通常就是每行的内容，headers可以自行添加。
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E4%B8%89%E3%80%81Flume%E9%9C%80%E8%A6%81%E7%90%86%E8%A7%A3%E7%9A%84%E5%86%85%E5%AE%B9)三、Flume需要理解的内容
- 
如何配好一个最简单的flume.conf，使得flume agent正常工作；
- 
Flume的flow的种类和适用场景；
- 
Flume的官方提供的sources，channels，sinks，如提供的不满足需求，可自定义适用于自己场景的source、channel和sink；
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E5%9B%9B%E3%80%81ElasticSearch%E6%A6%82%E8%BF%B0)四、ElasticSearch概述
Elasticsearch是一个基于Apache Lucene(TM)的开源的、实时的、分布式的、全文存储、搜索、分析引擎。
Lucene使用起来非常复杂，ES（ElasticSearch）可以看成对其进行了封装，提供了丰富的REST API，上手非常容易。
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E4%BA%94%E3%80%81ElasticSearch%E7%9A%84%E6%95%B0%E6%8D%AE%E6%A8%A1%E5%9E%8B%E7%9A%84%E7%AE%80%E5%8D%95%E7%90%86%E8%A7%A3)五、ElasticSearch的数据模型的简单理解
在Elasticsearch中，有几个概念（关键词），有别于我们使用的关系型数据库中的概念，注意类比：
```
Relational DB	-> Databases 		-> Tables -> Rows 		-> Columns
Elasticsearch	-> Indices(Index) 	-> Types  -> Documents 	-> Fields
```
Elasticsearch集群可以包含多个索引(indices)（数据库），每一个索引可以包含多个类型(types)（表），每一个类型包含多个文档(documents)（行），然后每个文档包含多个字段(Fields)（列）。
如何定位es中的一个文档（Document）？
通过Index（索引: 文档存储的地方） +　Type（类型:文档代表的对象的类）+ Document_ID（唯一标识：文档的唯一标识），在ES内部的元数据表示为：_index + _type + _id。
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E5%85%AD%E3%80%81Kibana%E6%A6%82%E8%BF%B0)六、Kibana概述
可以看成是ES的一个插件，提供的功能：
- 
Flexible analytics and visualization platform
- 
Real-time summary and charting of streaming data
- 
Intuitive interface for a variety of users
- 
Instant sharing and embedding of dashboards
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E4%B8%83%E3%80%81%E7%B3%BB%E7%BB%9F%E5%AE%9E%E7%8E%B0%EF%BC%9A)七、系统实现：
环境：
```
（1）JDK版本：java -version
java version "1.7.0_75"
OpenJDK Runtime Environment (rhel-2.5.4.0.el6_6-x86_64 u75-b13)
OpenJDK 64-Bit Server VM (build 24.75-b04, mixed mode)
（2）Flume1.6.0
（3）ElasticSearch1.7.5
```
注意：我这里实验显示Flume1.6.0不能导数据到ES2.2.0
![log-arch](http://7xlhna.com1.z0.glb.clouddn.com/log_arch.png)log-arch
Flume使用的conf，可以简单的设置如下：
```
agent.sources = yarnSrc hbaseSrc
agent.channels = memoryChannel
agent.sinks = elasticsearch
# source1:hdfsSrc
agent.sources.hdfsSrc.type = exec
agent.sources.hdfsSrc.command = tail -F /var/log/tbds/hdfs/hdfs/hadoop-hdfs-datanode-10.151.139.111.log
agent.sources.hdfsSrc.channels = memoryChannel
# source2:yarnSrc
agent.sources.yarnSrc.type = exec
agent.sources.yarnSrc.command = tail -F /var/log/tbds/yarn/yarn/yarn-yarn-nodemanager-10.151.139.111.log
agent.sources.yarnSrc.channels = memoryChannel
# source3:hbaseSrc
agent.sources.hbaseSrc.type = exec
agent.sources.hbaseSrc.command = tail -F /var/log/tbds/hbase/hbase-hbase-regionserver-10.151.139.111.log
agent.sources.hbaseSrc.channels = memoryChannel
# sink1:localSink
agent.sinks.localSink.type = file_roll
agent.sinks.localSink.sink.directory = /var/log/flume
agent.sinks.localSink.sink.rollInterval = 0
agent.sinks.localSink.channel = memoryChannel
# sink2:esSink
agent.sinks.elasticsearch.channel = memoryChannel
agent.sinks.elasticsearch.type = org.apache.flume.sink.elasticsearch.ElasticSearchSink
agent.sinks.elasticsearch.hostNames = 10.151.139.111:9300
agent.sinks.elasticsearch.indexName = basis_log_info
agent.sinks.elasticsearch.batchSize = 100
agent.sinks.elasticsearch.indexType = logs
agent.sinks.elasticsearch.clusterName = my-test-es-cluster
agent.sinks.elasticsearch.serializer = org.apache.flume.sink.elasticsearch.ElasticSearchLogStashEventSerializer
# channel1
agent.channels.memoryChannel.type = memory
agent.channels.memoryChannel.capacity = 100
```
注意要在flume/lib下加入两个包：
lucene-core-4.10.4.jar
elasticsearch-1.7.5.jar
```
The elasticsearch and lucene-core jars required for your environment must be placed in the lib directory of the Apache Flume installation.
```
之后分别运行elasticsearch和flume即可。
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E5%85%AB%E3%80%81%E7%B3%BB%E7%BB%9F%E6%94%B9%E8%BF%9B%EF%BC%9A)八、系统改进：
- 
配置flume interceptor加入各类headers，重写ElasticSearchLogStashEventSerializer使得event的header部分可以作为es的文档的field
- 
memory channel与file channel的结合，参见美团日志系统的改进
- 
日志若是错误信息，并不是每一行都是作为es的一条文档，而是若干行的内容才是es的一个文档的message
#### [](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/#%E4%B9%9D%E3%80%81%E7%B3%BB%E7%BB%9F%E5%AE%9E%E7%8E%B0%E6%95%88%E6%9E%9C%EF%BC%9A)九、系统实现效果：
导入es的文档数据结构：
![es-data-structure](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/)es-data-structure
Kibana展示：
![kibana-result](http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/)kibana-result
References：
- 
Apache Flume：[https://flume.apache.org/](https://flume.apache.org/)
- 
elastic.co：[https://www.elastic.co/](https://www.elastic.co/)
- 
Elasticsearch权威指南（中文版）：[https://www.gitbook.com/book/looly/elasticsearch-the-definitive-guide-cn/details](https://www.gitbook.com/book/looly/elasticsearch-the-definitive-guide-cn/details)
- 
Kibana ：[https://www.elastic.co/products/kibana](https://www.elastic.co/products/kibana)
转自：http://qiaowei.xyz/2016/02/27/2016-02-27-shi-shi-ri-zhi-shou-ji-cha-xun-fen-xi-xi-tong-flume-plus-elasticsearch-plus-kibana/
