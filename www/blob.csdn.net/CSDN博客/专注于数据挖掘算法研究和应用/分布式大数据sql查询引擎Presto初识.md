# 分布式大数据sql查询引擎Presto初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月05日 16:33:12[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2952
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









## 1、背景：

Hive使用MapReduce作为底层计算框架，是专为批处理设计的。但随着数据越来越多，使用Hive进行一个简单的数据查询可能要花费几分到几小时，显然不能满足交互式查询的需求。Presto是一个分布式SQL查询引擎，它被设计为用来专门进行高速、实时的数据分析。它支持标准的ANSI SQL，包括复杂查询、聚合（aggregation）、连接（join）和窗口函数（window functions)。

这其中有两点就值得探究，首先是架构，其次自然是怎么做到低延迟来支持及时交互。

## 2、Presto架构



![](https://img-blog.csdn.net/20170705171005955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


Presto查询引擎是一个Master-Slave的架构，由一个Coordinator节点，一个Discovery Server节点，多个Worker节点组成，Discovery Server通常内嵌于Coordinator节点中。Coordinator负责解析SQL语句，生成执行计划，分发执行任务给Worker节点执行。Worker节点负责实际执行查询任务。Worker节点启动后向Discovery Server服务注册，Coordinator从Discovery Server获得可以正常工作的Worker节点。如果配置了Hive
 Connector，需要配置一个Hive MetaStore服务为Presto提供Hive元信息，Worker节点与HDFS交互读取数据。



![](https://img-blog.csdn.net/20170705171033917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 3、Presto即时查询原理

Presto的运行模型和Hive或MapReduce有着本质的区别。Hive将查询翻译成多阶段的MapReduce任务，一个接着一个地运行；每一个任务从磁盘上读取输入数据并且将中间结果输出到磁盘上。

Presto引擎没有使用MapReduce，而是使用一个定制的查询执行引擎来响应支持SQL语法。除了改进的调度算法之外，所有的数据处理都是在内存中进行的；不同的处理端通过网络组成处理的流水线；避免不必要的磁盘读写和额外的延迟。这种流水线式的执行模型会在同一时间运行多个数据处理段，一旦数据可用的时候就会将数据从一个处理段传入到下一个处理段，大大的减少各种查询的端到端响应时间。特点如下：

1）完全基于内存的并行计算

2）流水线

3）本地化计算

4）动态编译执行计划

5）小心使用内存和数据结构

6）类BlinkDB的近似查询

7）GC控制

## 4、Presto执行查询过程

参考：http://tech.meituan.com/presto.html



## 5、Presto安装部署

参考：http://prestodb-china.com/docs/current/installation.html





