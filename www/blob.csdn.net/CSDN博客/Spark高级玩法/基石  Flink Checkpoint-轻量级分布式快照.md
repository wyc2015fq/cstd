# 基石 | Flink Checkpoint-轻量级分布式快照 - Spark高级玩法 - CSDN博客
2018年08月09日 00:01:22[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：788
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
本文是Checkpoint系列非源码最后一篇文章。
[必会:关于SparkStreaming checkpoint那些事儿](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485157&idx=1&sn=6714f0579d230000336fac78fe9a730a&chksm=9f38e5cda84f6cdb2e924bcae2127f025cdef9757a611407eb2e028927d54a8ab02b752e5963&scene=21#wechat_redirect)
[flink超越Spark的Checkpoint机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485181&idx=1&sn=05623930a6bcb09596d1a252d440fd4e&chksm=9f38e5d5a84f6cc3c56ae158a005dbe9122f16a6d27782a682bf1c9c5f1ac9f022d75c89a95b&scene=21#wechat_redirect)
前面两篇，一篇是spark的driver的Checkpoint细节及使用的时候注意事项。一篇是flink的Checkpoint的一些上层解释。本文主要是将flink的Checkpoint的基石--轻量级分布式快照。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)1. 简介
分布式数据流处理是数据密集型计算的新兴范例，它允许对大量数据进行连续计算，以低端到端延迟为目标，同时保证高吞吐量。 一些要求实时应用程序可以从Apache Flink 和Naiad 等数据流处理系统中受益，特别是在实时分析领域（例如预测分析和复杂事件处理）。 容错在这样的系统中是至关重要的，因为在大多数现实世界的用例中都不能避免发生故障。当前已知的方法，保证有状态处理系统上的恰一次语义，是执行状态的全局一致快照。 但是，存在两个主要缺点，使得它们的应用对于实时流处理而言效率低下:
- 
同步快照技术会暂时停止分布式计算的整体运行，以便获得整体状态的一致视图。
- 
此外，据我们所知，分布式快照的所有现有算法都将通道中传输的记录或在整个执行图中未处理的消息作为快照状态的一部分。大多数情况下，这些内容要大于要求的状态。
在这项工作中，我们专注于提供轻量级快照，专门针对分布式有状态数据流系统，对性能影响较小。 我们的解决方案提供具有低空间成本的异步状态快照，其仅包含非循环执行拓扑中的运算符状态。 此外，针对循环执行图的情况，我们通过在拓扑的选定部分上应用下游备份，将快照状态保持为最小。 我们的技术不会停止流操作，它只会引入很小的运行时开销。 本文的贡献可归纳如下：
- 
我们提出并实现了一种异步快照算法，该算法可以实现在非循环执行图上的最小快照。
- 
我们描述并实现了我们的算法的泛化，该算法适用于循环执行图。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)2. 背景：Apache Flink
我们当前的工作以Apache Flink Streaming（一种分布式流分析系统，Apache Flink Stack的一部分）对故障容错的需求为指导。 Apache Flink架构设计目标是统一批处理和流式处理。 Flink中的分析作业被编译为任务的有向图。 数据元素从外部源获取，并以pipeline方式通过任务图。 任务根据收到的数据不断操纵其内部状态，并产生新的输出。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)2.1 流式编程模型
Apache Flink 流API主要是处理无界流数据。 可以从外部源（例如消息队列，套接字流，自定义生成器）或通过对其他DataStream进行操作来创建DataStream。 DataStreams支持多种运算符，例如map，filter和reduce，这些是以高阶函数的形式支持的，并且是以每个记录为单位逐步调用并生成新的DataStream。 通过将并行实例放置在相应流的不同分区上运行，可以并行化每个运算符，从而实现流转换的分布式执行。
结合下面的小代码片段讲解：
val env : StreamExecutionEnvironment = ...
env.setParallelism(2)
val wordStream = env.readTextFile(path)
val countStream = wordStream.groupBy(_).count
countStream.print
上面代码示例显示了如何在Apache Flink中实现简单的单词统计。 在该程序中，从文本文件中读取字，并将每个字的当前计数打印到标准输出。 这是一个有状态的流程序，因为数据源需要知道它们当前的文件偏移量，并且计数器需要将每个字的当前计数保持为其内部状态。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)2.2 分布式数据流执行
当用户执行应用程序时，所有DataStream操作符都编译成执行图，该执行图原则上是有向图G =（T，E），其中顶点T表示任务，边E表示任务之间的数据通道。 对于单词统计的示例，下图描绘了执行图。如图所示，操作符的每个实例都封装在相应的任务上。任务可以进一步分类为source和sink。 此外，M表示在并行执行期间由任务传输的所有记录的集合。每个任务 t ∈ T 包含了运算符实例的独立执行，由以下内容组成：
- 
一组输入输出通道：It，Ot⊆E;
- 
操作符状态st
- 
用户定义函数（UDF）ft。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWyjBXBmUhO4m3bqsAUyiaowyBbo9QNNWibRjKubBf0OeefyBJ2T8wGlKG2Aiaum8ic6bU7pD8rOV4H2g/640?wx_fmt=png)
**数据摄取是基于拉的：**
在执行期间，每个任务都消费输入记录，更新其操作符状态并根据其用户定义的函数生成新记录。 更具体地说，对于由任务 t ∈ T 接收的每个记录 r ∈ M，根据其UDF ft：st，r            
