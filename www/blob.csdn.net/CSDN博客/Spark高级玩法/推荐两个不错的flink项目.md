# 推荐两个不错的flink项目 - Spark高级玩法 - CSDN博客
2018年10月12日 00:01:50[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：8150
题外话，感觉文章可以就点击一些广告呗，谢谢。
最近flink真是风生水起，但是浪院长看来这不过是阿里错过了创造spark影响力之后，想要在flink领域创建绝对的影响力。但是，不可否认flink在实时领域确实目前来看独树一帜，当然也有它不适合的地方，比如今天要推荐的第一个基于flink开发的项目，流表和维表的join，还有很多地方还是用spark streaming更合适，但是整体的流处理而言flink确实很优秀，虽然目前测出了一些bug，后面会发文说明一下flink开发时候常见的坑和已有的自身bug。接下来转入正题。
**flinkStreamSQL**
熟悉flink的应该都了解，flink支持流表之间的join，但到1.6为止都不支持流表和维表的join。浪尖最近，也在开发流平台，需要到flink流表和维表的join。那么针对这个大家第一印象，可以写个算子去实现，比如map等。但是浪尖这里开发的流平台不是说自己写api，而是用户通过sql去实现创建source，sink，udf，sql等，这个时候要进行维表join，大家可能是想到了udf。是的对于只有一个维表的情况下使用udf比较方便，但是多个维表，相对就麻烦很多了。
而基于flink开发的flinkStreamSQL主要是实现了flink 流表和维表的join，其主要功能如下：
- 
自定义create table 语法（包括源表,输出表,维表）
- 
自定义create function 语法
- 
实现了流与维表的join
浪尖花了个把小时看了一下源码，源码思路很清晰，主要是两个步骤：
- 
用flink api实现维表的功能: 要实现维表功能就要用到 flink Aysnc I/O 这个功能，是由阿里巴巴贡献给apache flink的。关于异步IO的介绍，可以参考：https://yq.aliyun.com/articles/457385
- 
解析流与维表join的sql语法转化成底层的flinkAPI 
源码下载地址：
https://github.com/DTStack/flinkStreamSQL
为了方便大家阅读，这里浪尖也把维表转化的过程主要函数贴出来吧：
主函数
Main#main
SQL解析
SqlTree sqlTree = SqlParser.parseSql(sql)
拆读
SqlParser#parseSql
TableInfoParserFactory#parseWithTableType
注册表
registerTable
存在维表的话，维表转换与逻辑sql执行
SideSqlExec#exec
也即是
sideSqlExec.exec(result.getExecSql(), sideTableMap, tableEnv, registerTableCache);
不存在维表的话
tableEnv.sqlUpdate(result.getExecSql());
**FlinkX**
FlinkX主要是用来做数据同步的，实现了多种异构数据源之间高效的数据迁移。
不同的数据源头被抽象成不同的Reader插件，不同的数据目标被抽象成不同的Writer插件。理论上，FlinkX框架可以支持任意数据源类型的数据同步工作。作为一套生态系统，每接入一套新数据源该新加入的数据源即可实现和现有的数据源互通。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWdpJicYoykV7hkAUH1icQxJZQ2y4M7LTZZ1leASibLGI4ahojCwUwl7eiab1UsBBllrgBc3Q801RNQVg/640)
在底层实现上，FlinkX依赖Flink，数据同步任务会被翻译成StreamGraph在Flink上执行，工作原理如下图：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWdpJicYoykV7hkAUH1icQxJZx5AuRqLoMm2E9FlurYAbRcIqYrmWFXTAh0dg4k1tQooibFnichbgapdQ/640)
使用安装部署，可以参考：
https://github.com/DTStack/flinkx
推荐阅读：
[重要 : 优化flink的四种方式](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485344&idx=1&sn=cde8915c6983dcfe541043cd6db8dbe6&chksm=9f38e488a84f6d9eb7b961a2c4fa05f2bd85fa82df81e4d8e56d0c22d86ff413a0baaacea01e&scene=21#wechat_redirect)
[flink超越Spark的Checkpoint机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485181&idx=1&sn=05623930a6bcb09596d1a252d440fd4e&chksm=9f38e5d5a84f6cc3c56ae158a005dbe9122f16a6d27782a682bf1c9c5f1ac9f022d75c89a95b&scene=21#wechat_redirect)
[干货:Flink+Kafka 0.11端到端精确一次处理语义实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485150&idx=1&sn=ba0570a9f631501a189d8184807d518b&chksm=9f38e5f6a84f6ce0dd805eae87d6c62e2c96670f14c2bac1607961614ba49f310a4630e31a3b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
