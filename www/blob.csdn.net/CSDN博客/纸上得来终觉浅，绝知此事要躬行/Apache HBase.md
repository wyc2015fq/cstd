# Apache HBase - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 17:57:13[boonya](https://me.csdn.net/boonya)阅读数：388








原文地址：[http://hbase.apache.org/](http://hbase.apache.org/)


## 欢迎使用Apache HBase™

[Apache](http://www.apache.org/)HBase™是[Hadoop](http://hadoop.apache.org/)数据库，一个分布式，可扩展，大数据存储。


当您需要随机，实时的读/写访问您的大数据时，使用Apache HBase™。这个项目的目标是托管非常大的表 - 数十亿行X百万列 - 在商品硬件群集的顶部。 Apache HBase是一个开源的，分布式的，版本化的，非关系型数据库，按照Google的[Bigtable：Chang等人的结构化数据的分布式存储系统建模](http://research.google.com/archive/bigtable.html)。正如Bigtable利用由Google文件系统提供的分布式数据存储一样，Apache
 HBase在Hadoop和HDFS之上提供类似于Bigtable的功能。

## 下载

点击[这里](http://www.apache.org/dyn/closer.cgi/hbase/)下载Apache HBase™。



我们建议下载当前的稳定版本。


(1.2.x系列是当前的稳定版本行，它取代了1.1.x，1.0.x，0.98.x和0.94.x（1.1.x，1.0.x，0.98.x和0.94.x行仍然看到对于那些不容易更新的人来说，每月一次的错误修复版本）。 注意，0.96是2014年9月1日的EOL。


对于较旧的版本，请检查apache归档 。)



## 特征
-     线性和模块化可扩展性。
-     严格一致的读写。
-     表的自动和可配置分片
-     RegionServer之间的自动故障转移支持。
-     方便的基类，用于将Hadoop MapReduce作业与Apache HBase表一起使用。
-     易于使用的Java API客户端访问。
-     块缓存和布隆过滤器，用于实时查询。
-     通过服务器端筛选查询谓词下推
-     Thrift网关和支持XML，Protobuf和二进制数据编码选项的REST-ful Web服务
-     基于可扩展jruby（JIRB）的shell
-     支持通过Hadoop指标子系统将指标导出到文件或Ganglia;或通过JMX


## 更多信息


请参阅[架构概述](http://hbase.apache.org/book.html#arch.overview)，[Apache HBase参考指南常见问题](http://hbase.apache.org/book.html#faq)和其他文档链接。

**出口控制**


    HBase分发包括加密软件。请参阅[此处](http://hbase.apache.org/export_control.html)的导出控制通知
**行为守则**


    我们期望参与讨论HBase项目邮件列表，Slack和IRC通道以及JIRA问题的参与者遵守Apache Software Foundation的行为准则。更多信息可以在这里找到。


## 新闻


12月8日，2016在旧金山举办的Splice会议


9月26日，2016 HBaseConEast2016在谷歌在切尔西，纽约


五月24th，2016 HBaseCon2016在村，969市场，旧金山


6月25日，2015年HBase夏季聚会2015年在东京


2015年5月7日HBaseCon2015在旧金山


2015年2月17日HBase在圣何塞的Strata + Hadoop World上聚会


2015年1月15日HBase meetup @ AppDynamics在旧金山


2014年11月20日HBase meetup @ WANdisco在圣拉蒙


2014年10月27日Apple在Cupertino举行的HBase Meetup


2014年10月15日HBase Meetup @ Google在纽约市Strata / HW之前的夜晚


2014年9月25日在Palo Alto的HBase Meetup @ Continuuity


2014年8月28日HBase Meetup @ Sift Science在旧金山


2014年7月17日HBase Meetup @ HP在Sunnyvale


2014年6月5日HBase BOF在Hadoop峰会，圣荷西会议中心


2014年5月5日HBaseCon2014在希尔顿旧金山联合广场


三月12th，2014 HBase Meetup @ Ancestry.com在旧金山
            


