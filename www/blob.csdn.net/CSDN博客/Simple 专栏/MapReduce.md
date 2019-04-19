# MapReduce - Simple 专栏 - CSDN博客
2015年04月11日 20:53:06[Simple_Zz](https://me.csdn.net/love284969214)阅读数：272标签：[MapReduce																[hadoop																[大数据](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=MapReduce&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
### MapReduce 定义
- Mapreduce是一个分布式运算程序的编程框架，是用户开发“基于hadoop的数据分析应用”的核心框架；
- Mapreduce核心功能是将用户编写的业务逻辑代码和自带默认组件整合成一个完整的分布式运算程序，并发运行在一个hadoop集群上。
### MapReduce 优点
- MapReduce 易于编程。它简单的实现一些接口，就可以完成一个分布式程序，这个分布式程序可以分布到大量廉价的 PC 机器运行。也就是说你写一个分布式程序，跟写一个简单的串行程序是一模一样的。 就是因为这个特点使得 MapReduce 编程变得非常流行。
- 良好的扩展性。当你的计算资源不能得到满足的时候，你可以通过简单的增加机器来扩展它的计算能力。
- 高容错性。MapReduce 设计的初衷就是使程序能够部署在廉价的 PC 机器上，这就要求它具有很高的容错性。比如其中一台机器挂了，它可以把上面的计算任务转移到另外一个节点上面上运行，不至于这个任务运行失败，而且这个过程不需要人工参与，而完全是由 Hadoop 内部完成的。
- 适合 PB 级以上海量数据的离线处理。这里加红字体离线处理，说明它适合离线处理而不适合在线处理。比如像毫秒级别的返回一个结果，MapReduce 很难做到。
### MapReduce 缺点
- MapReduce不擅长做实时计算、流式计算、DAG（有向图）计算。
- 实时计算。MapReduce 无法像 Mysql 一样，在毫秒或者秒级内返回结果。
- 流式计算。流式计算的输入数据时动态的，而 MapReduce 的输入数据集是静态的，不能动态变化。这是因为 MapReduce 自身的设计特点决定了数据源必须是静态的。
- DAG（有向图）计算。多个应用程序存在依赖关系，后一个应用程序的输入为前一个的输出。在这种情况下，MapReduce 并不是不能做，而是使用后，每个MapReduce 作业的输出结果都会写入到磁盘，会造成大量的磁盘IO，导致性能非常的低下。
### MapReduce 核心思想
- 分布式的运算程序往往需要分成至少2个阶段。
- 第一个阶段的maptask并发实例，完全并行运行，互不相干。
- 第二个阶段的reduce task并发实例互不相干，但是他们的数据依赖于上一个阶段的所有maptask并发实例的输出。
- MapReduce编程模型只能包含一个map阶段和一个reduce阶段，如果用户的业务逻辑非常复杂，那就只能多个mapreduce程序，串行运行。
### MapReduce 进程
- 一个完整的mapreduce程序在分布式运行时有三类实例进程。
- MrAppMaster：负责整个程序的过程调度及状态协调。
- MapTask：负责map阶段的整个数据处理流程。
- ReduceTask：负责reduce阶段的整个数据处理流程。
### MapReduce 编程规范（八股文）
用户编写的程序分成三个部分：Mapper，Reducer，Driver(提交运行mr程序的客户端)
Mapper阶段
- 用户自定义的Mapper要继承自己的父类。
- Mapper的输入数据是KV对的形式（KV的类型可自定义）。
- Mapper中的业务逻辑写在map()方法中。
- Mapper的输出数据是KV对的形式（KV的类型可自定义）。
- map()方法（maptask进程）对每一个<K,V>调用一次。
Reducer阶段
- 用户自定义的Reducer要继承自己的父类。
- Reducer的输入数据类型对应Mapper的输出数据类型，也是KV。
- Reducer的业务逻辑写在reduce()方法中。
- Reducetask进程对每一组相同k的<k,v>组调用一次reduce()方法。
Driver阶段
- 整个程序需要一个Drvier来进行提交，提交的是一个描述了各种必要信息的job对象。
