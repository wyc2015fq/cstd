# Elasticsearch初识 - Soul Joy Hub - CSDN博客

2016年08月07日 21:37:19[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：495标签：[elasticsearch																[服务器																[集群																[管理																[计算机](https://so.csdn.net/so/search/s.do?q=计算机&t=blog)](https://so.csdn.net/so/search/s.do?q=管理&t=blog)](https://so.csdn.net/so/search/s.do?q=集群&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=elasticsearch&t=blog)
个人分类：[大数据平台](https://blog.csdn.net/u011239443/article/category/6296513)


# 问题一

> 
描述Elasticsearch中讲到的以下基本概念，并说明它们之间的区别和联系： 

  Cluster, Node, Index, Type, Document, Shards, Segments.

- 
Cluster

集群，是一组相互独立的、通过高速网络互联的计算机（Node），它们构成了一个组，并以单一系统的模式加以管理。一个客户与集群相互作用时，集群像是一个独立的服务器。集群配置是用于提高可用性和可缩放性。

- 
Node

节点，集群中相互独立的、通过高速网络互联的计算机

- 
Index

索引，使用索引可快速访问文件中的特定信息。

- 
Type

查询类型有：
- 查询匹配
- Dfs, 查询匹配
- 计数
- 扫描

- 
Document

文档  

- 
Shards

一个Node中可以有多个Shard，一个Shard有一个Lucene index，对应一个SE单机。

- 
Segments

一个Shard中的多个分片。

# 问题二

> 
描述Elasticsearch中数据插入（Indexing）过程和查询过程（Search）。

## Indexing
- 一个index请求，是在一个index中创造和改变documents
- index操作会转发给该shard的“primary”。一个shard确保有一个“primary”，并且有0到多个replicas。
- primary会作为该shard的index操作的协调者，来发送index操作给相关的replica，直到返回的确认数量符合，才指示操作成功。

## Search
- 当你查询一个或者多个Elasticsearch index时，查询在所有的shards被执行，依次在所有的segments，然后被合并。
- 事实上，查询同个shard的两个Elasticsearch indexes，和查询有两个shards的一个Elasticsearch index十分相似。两者都是使得两个底层的Lucene indexes被查询。

# 问题三

> 
描述什么是Elasticsearch Mapping以及它的作用。

- 一个schema是一个或者多个fields的描述，它说明了document类型和如何去处理一个document的不同fields。
- Elasticsearch中的schema是一个mapping，它描述了JSON documents中的fields，它们的数据类型以及它们一个如何indexed底层的Lucene indexes。因此，在Elasticsearch terms中，我们通常称该schema为一个“mapping”
- Mapping是一种明确一个document包含多少fields、如何存储、如何indexed的处理。


