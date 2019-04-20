# 初识PB级数据分析利器Prestodb - Spark高级玩法 - CSDN博客
2017年12月26日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2489
- 
初始PB级数据分析利器Prestodb
- 
什么是prestodb
- 
prestodb整体架构
- 
物理执行计划
# 什么是prestodb
prestodb,是facebook开源的一款sql on hadoop系统，是facebook的工程师对hive的查询速度忍无可忍后，下决心开发的一款高性能查询引擎，基于java8编写，其基于page的pipeline技术，使其具有高效的交互式查询性能，并可以高效的控制GC；而其和底层数据源解耦的特性，使其能够对接各类数据源，并具有跨源查询的特性。目前在国内，有京东、美团、同城以及滴滴等公司对prestodb有深度使用，在国外，除facebook外，还有uber等公司对prestodb有深度使用，而teradata公司则在维护独立的分支，并将其作为自己的主打的即系查询产品的后台。本文介绍prestodb，先主要介绍presto的架构和查询原理，presto的搭建比较简单，大家可以参考官网的文章进行操作。
# prestodb整体架构
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIr3LtczZLR3x173of9pG1PlCCOha7ibmLHhQkWzA17wQJ8jN2lMiaNdqQ/?wx_fmt=png)
如上图所示，prestodb主要由一个coordinator和多个worker组成，coordinaor节点负责和client对接，接收client发送过来的各类请求(DDL和DML)。coordinator在接收到client的请求后，就开始进行请求的处理，最后把查理结果返回给client。coordinator在进行请求处理时，对各类sql语句进行词法解析、语法分析、语义分析、优化、生成执行计划最后在调度模块进行任务的分发，把子任务分发到各个worker节点。worker节点是实际的执行节点，会执行包括聚合、排序、join以及去重等操作。整体执行流程如下图所示：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIOoYibvTXgaoc1vPHeiaTxYYhjvIIt7aicX6z9opU9ibBGDlRIA5709u1aA/?wx_fmt=png)
其中大多数流程会在后边会进行详细的介绍。
这篇文章的主要目的是入门和普及分布式sql的执行原理，看过一些其他的相关文章，都是从上到下的介绍，个人感觉这样不利于入门，很多人看到执行计划那里就“知难而退”了。所以我在介绍presto时，准备采用从下到上的方式来介绍。
分布式sql说白了也是sql，既然是sql，典型的几个查询语句就是groupby、orderby、join等。本文以groupby为例来进行介绍，orderby和join等的执行流程也会在后续文章中进行介绍。
# 物理执行计划
物理执行计划是最接近我们理解的一个步骤了，所以这里先来看看presot中得物理执行计划。假设我们有一张订单表，这个订单表的数据分布在两个节点上，node1上的数据分片是：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIfoCreYyVpc9bu1EiaxNnwKIXiarZ1libS9Co4xJOTI5U6Ip4LWFFcAVyQ/?wx_fmt=png)
node2上得数据分片是：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJI0ibkg453mwkhZxymn5K8KuhTVakzvZTAZ7s2eOrbeC2JEM0yeACFfiaw/?wx_fmt=png)
假设我们有这样一个分组聚合查询查询：
- 
`SELECT `
- 
`    sum(totalprice),orderpriority`
- 
`FROM `
- 
`    orders`
- 
`where `
- 
`    custkey<100`
- 
`GROUP BY`
- 
`    orderpriority`
这个查询中有过滤(custkey<100)，有分组(group by orderpriority)，有聚合(sum)，是一个非常典型的数据库查询语句。我们先不看presto怎么实现这样的查询，我们可以先自己想想如果我们自己去完成这样的操作我们要怎么做？首先，我们肯定会先从数据源读取数据，但是读到数据并非符合我们的需要，这时我们就需要对数据按照一定的条件进行过滤，过滤完之后的数据就是我们感兴趣的数据；那么过滤完之后留下的数据我们是先聚合还是先分组呢？肯定是先分组了，如果没有分组，聚合也就无从谈起了，因为聚合就是针对一个组内的数据进行的聚合，如果是不同组就没有聚合的必要了。分组的方法很多，可以使用hash的方式完成分组，也可以使用sort的方式完成分组，或者更高级的，如果有倒排索引，组已经就是分好的了，省了很多事情。presto采用hash的方式完成分组。等分组完成后，就可以在一个组内进行聚合操作了，比如进行计数统计、求和或者求平均等。
好了，那么我们看看presot的执行流程(物理执行计划)是否符合我们的预期：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIOib3FvDzX1KIFf7RyHeBknJO2Mv2YDej02vP5ibqupRV7kAdziavyl60g/?wx_fmt=png)
上图所示有三个大的方框，每个方框表示一个presto执行过程中的一个stage（类似spark中的stage，以shuffle操作作为stage的边界），先看标号为3的方框，在这个方框内有三个operator，最下边的是ScanFilterAndProjectOperotor，从名字就可以看出，这个物理操作符的主要任务是扫描、过滤和投影，扫描和过滤后的数据如下：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIFI5aJotQFRHnnSc6iboKryxBhF4oRQ9kHyEYYyIPrVicolgsB5jgA7Yw/?wx_fmt=png)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIiaiahQSkvh6nguicCq0p4135hh1RkC1RaxgYWs5fI7s7RCFiahmbV2FvmA/?wx_fmt=png)
可以看出，custkey大于100的行已经被过滤掉了。这里的投影(project)的意思是把上层operator需要的字段值筛选出来，以上图的sql为例，要帅选的字段包括orderpriority和totalprice，因为这两个字段才是后续的分组和聚合需要的字段，而字段custkey只是起了一个过滤数据的作用，所以在project是不会被向上层传递。所以上层operator接收到的数据如下：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJI11Up9gibKljcqPHyIdfy0cKrcvug4zACObnGp94a4QWiaibQJubO7rv5A/?wx_fmt=png)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJId6maMpAr3hLsE22gY9pnXhAtsicaoPOA6eV8kPRUeoeP9jBPylwGM3Q/?wx_fmt=png)
等数据被发送到上一级的operator后，就要开始进行分组了，这里用到的操作符是HashAggregationOperator，从名字可以看出，这里使用了hash的方式对数据进行了分组，即把具有相同hash值的数据作为一个组，同时在分组完成后进行了聚合操作(Aggregation)，注意这里的HashAggregationOperator后跟了一个(partial)关键字，表示这一步的分组和聚合只是完成一个局部的分组聚合，这是因为数据在分布式环境下，一个group的数据可能是存在多个partition下，甚至是跨不同物理机的，所以需要首先完成局部聚合，降低后续要进行shuffle的数据量。这就类似spark中的reduceByKey算子，可以对数据进行map side combine，从而减少shuffle时数据的传输量，而如果使用groupByKey，没有进行map side combine，就会产生大量的数据需要通过网络传输。经过这个局部聚合后的数据如下图所示：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIicRtekXEY15LgAVrb4NpfEbeAV1hDRpOhhce8VZpj9RzhmfMfE3I3Ug/?wx_fmt=png)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIvdMDL25y0W4qYoJp5Pqia009kELErmwjDhNTbhw1tibuWbSibAoOMUgvg/?wx_fmt=png)
其中在node1，有三个分组，分别是high,mid,low，在node2右两个分组，分别是high和low。
在往上看，操作符是PartitionedOutputOperator，可以看出这是一个分区操作，主要是把上一步聚合分组和聚合的结果根据分组的key(即组名，例如high,low等)进行分区，写到不同的分区文件中(类似spark的shuffle操作中shuffle write)。至此，方框3(stage 3)中的操作完成。
接下来到了stage2 ，在这个stage中，首先要进行的上一个stage最后阶段输出数据的拉取(类似spark中得shuffle read)，在presto中对应的操作符是ExchangeOperator，拉取了数据之后，展现给上一层操作符的数据如下：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU3e5vZS0JibjsyMWyibHywJIIdic3YKe9cgC9QIWgggbdkNGSTKymjtW0LT5zBqnjF8ObZVQSXAD6cw/?wx_fmt=png)
可以看出，不同节点相同分组key的数据已经被聚合分组在一起了，这时只需要再进行一次聚合操作即可最终的分组聚合操作。在进行分组聚合查询时，经常会使用到hiving这样的操作，而再上一层的FilterAndProject操作就是进行分组聚合结果的过滤和投影，最后把这样的处理结果交给最后一个stage，即stage1，进行结果的输出。
至此，一个典型的group by查询的分布式物理执行计划就按照一定的逻辑执行完成了。从以上这个物理执行流程，我们可以看出presto在进行分布式数据处理时和sparksql有诸多类似的地方，其实不止是和sparksql类似，所有的分布式sql都是遵循这样那个的基本原理。
后续文章的计划如下：
- 
presto中的逻辑执行计划和调度
- 
presto中的词法分析、语法分析以及语义分析
- 
presto如何通过spi对接不同的数据源
- 
presto对接hive
- 
presto对接kafka
- 
presto对接RDBMS
- 
presto对接nosql
**推荐阅读：**
1，[HBase在滴滴出行的应用场景和最佳实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484343&idx=1&sn=cd0c5bd1fbd8d55dd9779d336880bb3d&chksm=9f38e09fa84f6989105d027e60beb463332313724813ea114ec3f4e29707c8da75fc09434269&scene=21#wechat_redirect)
2，[SparkMLLib中基于DataFrame的TF-IDF](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484310&idx=1&sn=acf0a573d24c112a4a20370f5430848f&chksm=9f38e0bea84f69a8d50db328b373fdd8a867e2caee3262236c1cf57188bf87cefdfaa456607a&scene=21#wechat_redirect)
3，[实战phoenix](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484253&idx=1&sn=2d1a9100dae4abae0efc2a4fea6fded5&chksm=9f38e075a84f69639b03aaa5491c9d9016a3e9057a37f3bf3dc892159fcde39dbb361350d7ac&scene=21#wechat_redirect)
4，[大数据查询——HBase读写设计与实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484239&idx=1&sn=93c55d3a91f6ff9646389755844cabe7&chksm=9f38e067a84f697182d978ba8c3d89a4b3b7349beccbf070a9dc958b5738a83e6cd9bf24b49d&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)

**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

