# Apache Ranger初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月06日 12:07:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2900
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、Apache ranger简介

Apache ranger是一个Hadoop集群权限框架，提供操作、监控、管理复杂的数据权限，它提供一个集中的管理机制，管理基于yarn的Hadoop生态圈的所有数据权限。

Apache Ranger可以对Hadoop生态的组件如Hive，Hbase进行细粒度的数据访问控制。通过操作Ranger控制台，管理员可以轻松的通过配置策略来控制用户访问HDFS文件夹、HDFS文件、数据库、表、字段权限。这些策略可以为不同的用户和组来设置，同时权限可与hadoop无缝对接。

Apache Ranger 支持以下HDP组件的验证、授权、审计、数据加密、安全管理：

1)Apache Hadoop HDFS

2)Apache Hive

3)Apache HBase

4)Apache Storm

5)Apache Knox

6)Apache Solr

7)Apache Kafka

8)YARN





2、Apache ranger部署

官网：http://ranger.apache.org/ ，参考http://ranger.apache.org/quick_start_guide.html说明。

具体部署待试验，下载的包可以看到支持各类组件：

![](https://img-blog.csdn.net/20170606120054360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


http://shenliang1985.blog.163.com/blog/static/2908380520151126102050593/ 这篇文章说的很多，没细看，后续部署可以参考。





3、Apache ranger原理

Aapche Ranger是以插件的形式集成到HDFS中，由Ranger Admin管理访问策略，Ranger插件定期轮询Admin更新策略到本地，并根据策略信息进行用户访问权限的判定。其中提供管理员管理策略、插件的Ranger web和Ranger Plugin，与Admin之间的通信是基于HTTP的RESTful架构。Ranger集成HDFS的架构图如下：


![](https://img-blog.csdn.net/20170606120507686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








详细参考：http://www.cnblogs.com/qiuyuesu/p/6774520.html




4、Apache ranger集成kerberos认证

参考：http://www.datastart.cn/tech/2016/06/22/ranger-2.html




对于Hadoop集群的安全管理是一个运维的重大课题，这里先对Apache Ranger做一个初步认识，后续需要进一步研究。






