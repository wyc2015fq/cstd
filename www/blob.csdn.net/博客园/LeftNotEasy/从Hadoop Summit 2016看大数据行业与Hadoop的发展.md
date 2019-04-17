# 从Hadoop Summit 2016看大数据行业与Hadoop的发展 - LeftNotEasy - 博客园







# [从Hadoop Summit 2016看大数据行业与Hadoop的发展](https://www.cnblogs.com/LeftNotEasy/p/my-thoughts-from-hadoop-summit-2016.html)





# 前言：

好吧我承认已经有四年多没有更新博客了。。。。

在这四年中发生了很多事情，换了工作，换了工作的方向。在工作的第一年的时候接触机器学习，从那之后的一年非常狂热的学习机器学习的相关技术，也写了一些自己的理解和感悟。今天大概看了一下这个博客的总体阅读人数已经有70多万了，印象中之前还只有十多二十万。很高兴这些文章能够帮助你更好的理解一些机器学习相关的基础知识，非常感谢各位读者和爬虫机器人(:-p)的支持！

后来个人选择将工作的方向从机器学习换到了Hadoop相关领域，中间有很多感悟我想之后再单独写一写。好吧不废话了正文开始:

*（版权声明：本文由leftnoteasy发布于 [http://leftnoteasy.cnblogs.com](http://leftnoteasy.cnblogs.com), 本文可以部分或者全部的被引用，但请注明出处，可以联系wheeleast (at) gmail.com, 也可以加我微博: [http://weibo.com/leftnoteasy](http://weibo.com/leftnoteasy))*

*注：*
- *这篇正文是我对知乎问题（[https://www.zhihu.com/question/48135832](https://www.zhihu.com/question/48135832), 怎么看待Hadoop Summit 2016 和 Spark summit 2016?) 的回答，发布在此博客的时候稍有修改。*

# 正文:

![](https://images2015.cnblogs.com/blog/89123/201607/89123-20160705074806936-1054204355.jpg)

上周参加了在硅谷圣何塞举行的的Hadoop Summit 2016 (Hadoop 峰会），说说自己的感受（跟我的雇主无关，仅代表个人观点）。

链接：今年Hadoop Summit的日程表：[http://hadoopsummit.org/san-jose/agenda/，目前官方还没有把PPT和录像更新出来](http://hadoopsummit.org/san-jose/agenda/，目前官方还没有把PPT和录像更新出来)。

另外今年的Summit庆祝了Hadoop项目成立十周年。从参会人数来说，今年达到了创纪录的5000人，对比去年的4000人增长了25%，这对于一个已经十年的项目来说非常不容易。

# 看到的一些行业趋势：

## 1）Hadoop及其生态圈（包括Spark等等）在各行各业落地并且得到广泛的应用。

目前在美国，无论行业是IT，金融相关（包括银行保险)，电信，制造业，还是餐饮，百货零售都已经广泛的用上了Hadoop。看了一些有意思的演讲，比如说
- Progressive（美国最大的车保公司之一）通过实时采集用户驾驶的数据（比如说加、减速行为；车辆经过的路线等等）来决定是否需要改变用户的车保价格。可以参考一篇老一点的文章：[http://www.zdnet.com/article/how-auto-insurer-progressive-collected-10-billion-miles-of-driving-data-from-its-customers/](http://www.zdnet.com/article/how-auto-insurer-progressive-collected-10-billion-miles-of-driving-data-from-its-customers/)
- 福特公司也有一个类似的演讲，通过采集汽车里面的设置的传感器，实时反馈给服务端来优化驾驶的体验。

对于这些非IT企业，利用Hadoop生态圈里面的套件能够实现超大的数据处理规模（比如说福特汽车总共产生的数据可以达到一天TB级别），实时性（信用卡反欺诈需要在秒以内返回结果），丰富的分析手段（SQL、机器学习等）。这些新的数据分析的手段能够实实在在的产生商业价值，比如说只要保险公司能够降低1%的风险，产生的利润就非常可观了。

## 2）物联网 (IOT) + 实时（Realtime） + 机器学习是今年最火的话题

相对于去年来说，这三个话题的曝光率大大提高。今年总共有160多场演讲，其中物联网就有近20场演讲，机器学习有10多场演讲，实时有近10场演讲。

这三个话题其实互相关联，比如说物联网通过传感器采集了N多的数据（飞机引擎能够每小时产生35TB的数据，还有比如上面提到的福特汽车），这些数据需要立刻决定保留或者丢弃，对于保留的那部分的数据也要能够很快的做出决定。实时起到了很大的作用。在这次的某一个演讲中（忘了是哪家公司了），需要采集飞机上的雷达数据来预测天气是否危险，如果不能做到实时那几乎就是草菅人命了。

对于这种大规模的数据只用传统的SQL进行数据处理是远远不够的，特别是一些非结构化的数据（比如说雷达云图）。那么很多的机器学习的方法就能够排上用场了。这三个方向一定会在未来更火的。

# 关于Hadoop(YARN/HDFS)项目的趋势

对于Hadoop项目本身是个人工作也是我关注得最多的方向，这里也总结一下。

## 继续往易用方向发展

关于易用主要是两个方面，a. 方便安装部署, b. 方便运维。对于安装部署来说这几年的一些工具已经可以把问题解决得很好了，比如说Apache Ambari。对于运维来说则在今年涌现了很多的新的亮点，比如说来自Hortonworks的Service Asembly，Service Asembly也就是服务的组合，举个例子来说一个数据服务需要安装ZooKeeper、Kafka、HBase、Spark，并且需要让他们工作在一起。传统的方式是分别部署这些项目并且手动的把他们互相配置起来。现在可以通过Docker container加上预先写好的配置文件模板让这些服务一次性的在YARN上面启动和关闭。

## YARN更好的支持长时间服务(Long running service, LRS)

相对于普通的mapreduce、spark程序，长时间服务需要跑几天甚至几个月，YARN对于LRS的支持在近一年内有很大的进展。一些新完成、正在开发中的功能有类似于
- DNS（每个container有自己的地址, YARN-4757)
- Container自动重启(YARN-4725)
- Container重复利用（allocation reuse, YARN-4726)

还有我做的/正在做的几个功能:
- 动态改变运行中Container的资源（Resizing running container, YARN-1197）
- 方便的让程序在每个节点上跑一份（Affinity/Anti-affinity, YARN-1042)
- 以及资源抢占的一些改进 (YARN-4108/YARN-4390)

这些功能会大大的帮助在YARN上面跑LRS。

## 更大更快更强

Hadoop从来没有停止过性能上的优化，今年一些相关的改进:
- YARN RM Federation: 支持超大的YARN集群，据称微软已经通过这个支持了5万节点的YARN集群
- 下一代的YARN Timeline server (YARN-2928), 这个可以很好的把YARN集群里面的各种信息以及应用程序的信息存储、关联起来。
- HDFS Tiered Storage: 更好的在HDFS中支持管理不同的文件系统（比如说内存、SSD、本地磁盘、云存储）
- YARN resource overcommmitment (YARN-1011), 这个可以根据资源的实际使用情况来觉得是否可以多分配一些container来得到更好的资源利用率。

# （杂）总结和建议

一些个人的建议希望对你有帮助：
- 如果你的公司的足够多的数据，看看能不能用Hadoop生态圈的项目（不管是Hive还是Spark）进行优化，传统行业会一个一个被大数据公司占据掉，比如说阿里占据了零售业、金融业，赶不上这趟车就晚了!
- 如果你是初学者并且希望学大数据相关的技术，流行的开源项目是很好的选择。如果你想学Hadoop的话记得要学新一点的版本比如说2.7.x，1.x已经淘汰了。
- 如果你对贡献开源项目有兴趣，可以考虑考虑YARN，里面能做的东西还很多，并且社区很友好。












