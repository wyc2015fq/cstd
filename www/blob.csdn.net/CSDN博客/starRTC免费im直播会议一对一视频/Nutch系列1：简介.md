# Nutch系列1：简介 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月19日 13:10:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：120
由Java实现的，开放源代码(open-source)的web搜索引擎。
Nutch 致力于让每个人都能很容易, 同时花费很少就可以配置世界一流的Web搜索引擎. 为了完成这一宏伟的目标, Nutch必须能够做到:
- 每个月取几十亿网页
- 为这些网页维护一个索引
- 对索引文件进行每秒上千次的搜索
- 提供高质量的搜索结果
- 以最小的成本运作
Nutch主要分为两个部分:爬虫crawler和查询searcher。
Crawler主要用于从网络上抓取网页并为这些网页建立索引。
Searcher主要利用这些索引检索用户的查找关键词来产生查找结果。两者之间的接口是索引，所以除去索引部分，两者之间的耦合度很低。
Crawler和Searcher两部分尽量分开的目的主要是为了使两部分可以分布式配置在硬件平台上，例如将Crawler和Searcher分别放在两个主机上，这样可以提升性能。
Nutch是基于Lucene的。
那么我们应该使用Lucene还是Nutch呢？
最简单的回答是：如果你不需要抓取数据的话，应该使用Lucene。
比如你有数据源，需要为这些数据提供一个搜索页面。在这种情况下，最好的方式是直接从数据库中取出数据并用Lucene API 建立索引。
而当你没有本地数据源，或者数据源非常分散的情况下，应该使用Nutch。
艺搜参考
[http://nutch.apache.org/](http://nutch.apache.org/)
[http://baike.so.com/doc/5108365.html](http://baike.so.com/doc/5108365.html)
