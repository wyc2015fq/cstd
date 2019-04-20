# 结合Spark讲一下Flink的runtime - Spark高级玩法 - CSDN博客
2018年11月20日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：178
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vnn1sgeHceL4lqDdY3Gx0EFmFsQ4VRKVMDVJWeJXoptdmVaEMBLPrrw/640)
Flink运行时主要角色有两个：JobManager和TaskManager，无论是standalone集群，on yarn都是要启动这两个角色。有点类似于MRv1的架构了，JobManager主要是负责接受客户端的job，调度job，协调checkpoint等。TaskManager执行具体的Task。TaskManager为了对资源进行隔离和增加允许的task数，引入了slot的概念，这个slot对资源的隔离仅仅是对内存进行隔离，策略是均分，比如taskmanager的管理内存是3GB，假如有三个slot，那么每个slot就仅仅有1GB内存可用。
根据经验，taskslot数最佳默认值就是CPU核心数。使用超线程，每个task slot需要2个或更多硬件线程上下文。
Client这个角色主要是为job提交做些准备工作，比如构建jobgraph提交到jobmanager，提交完了可以立即退出，当然也可以用client来监控进度。
Jobmanager和TaskManager之间通信类似于Spark 的早期版本，采用的是actor系统。
根据以上描述，绘制出运行架构图就是下图：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vibzzWz9OXibWHOqht3MaHmagf5BiayMZkfg1EsibJBf8Wjp8k2crXZjf4A/640)
**Task****到底是什么玩意？**
讲到这可以先回顾一下Spark了，主要三个概念：
**1. Shuffle**
Spark 任务job中shuffle个数决定着stage个数。
**2. 分区**
Spark 算子中RDD的分区数决定者stage任务的并行度。
**3. 分区传递**
复杂的入union，join等暂不提。简单的调用链如下：
`rdd.map-->filter-->reducebykey-->map。`
例子中假设rdd有6个分区，map到fliter的分区数传递是不变，filter到redcuebykey分区就变了，reducebykey的分区有个默认计算公式，星球里讲过了，假设我们在使用reducebykey的时候传入了一个分区数12。
分区数，map是6，filter也是6，reducebykey后面的map就是12。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vCzOZg8lNH80G3pQ034aCd4cs83ricaJMoSLibzGvMRLnsdDt9PiaIv4xQ/640)
override defgetPartitions: Array[Partition] =firstParent[T].partitions
map这类转换完全继承了父RDD的分区器和分区数，默认无法人为设置并行度，只有在shuffle的时候，我们才可以传入并行度。
上述讲解主要是想带着大家搞明白，以下几个概念：
- 
Flink的并行度由什么决定的？
- 
Flink的task是什么？
**1.****Flink****的并行度由什么决定的？**
这个很简单，Flink每个算子都可以设置并行度，然后就是也可以设置全局并行度。
Api的设置
.map(new RollingAdditionMapper()).setParallelism(10)
全局配置在flink-conf.yaml文件中，parallelism.default，默认是1：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vh4hx60NYqFGaUOUUt9Ku2O4sVdf3gJNO2JDpbcuZABNdWu32XWuTYQ/640)
**2.****Flink****的task****是什么？**
按理说应该是每个算子的一个并行度实例就是一个subtask-在这里为了区分暂时叫做substask。那么，带来很多问题，由于flink的taskmanager运行task的时候是每个task采用一个单独的线程，这就会带来很多线程切换开销，进而影响吞吐量。
为了减轻这种情况，flink进行了优化，也即对subtask进行链式操作，链式操作结束之后得到的task，再作为一个调度执行单元，放到一个线程里执行。
如下图的，source/map 两个算子进行了链式；keyby/window/apply有进行了链式，sink单独的一个。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vRBjOroKXCdrBUU8gmv6mIWe1xRyhupwBuia0N7iaC9Evf4Y6Y4Nwk3VA/640)**注释**：图中假设是source/map的并行度都是2，keyby/window/apply的并行度也都是2，sink的是1，总共task有五个，最终需要五个线程。
按照到这一步的理解，画的执行图应该是这样的：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vlrF6oqlTt98u72bdric13CribgGqLKficueLIEaZJ1ZDPCtspY1IHwKkw/640)
有些朋友该说了，据我观察实际上并不是这样的呀。。。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vaoSVboXQicSEJITxpEyZSkmE4jTrClia6zFOyzS6droClQUicCrmOH9eQ/640)这个是实际上是flink又一次优化。
默认情况下，flink允许如果任务是不同的task的时候，允许任务共享slot，当然，前提是必须在同一个job内部。
结果就是，每个slot可以执行job的一整个pipeline，如上图。这样做的好处主要有以下几点：
1.**Flink 集群所需的taskslots数与job中最高的并行度一致。**也就是说我们不需要再去计算一个程序总共会起多少个task了。
2.**更容易获得更充分的资源利用**。如果没有slot共享，那么非密集型操作source/flatmap就会占用同密集型操作 keyAggregation/sink 一样多的资源。如果有slot共享，将基线的2个并行度增加到6个，能充分利用slot资源，同时保证每个TaskManager能平均分配到重的subtasks，比如keyby/window/apply操作就会均分到申请的所有slot里，这样slot的负载就均衡了。
**链式的原则，也即是什么情况下才会对task进行链式操作呢？简单梗概一下：**
- 
上下游的并行度一致
- 
下游节点的入度为1 （也就是说下游节点没有来自其他节点的输入）
- 
上下游节点都在同一个 slot group 中（下面会解释 slot group）
- 
下游节点的 chain 策略为 ALWAYS（可以与上下游链接，map、flatmap、filter等默认是ALWAYS）
- 
上游节点的 chain 策略为 ALWAYS 或 HEAD（只能与下游链接，不能与上游链接，Source默认是HEAD）
- 
两个节点间数据分区方式是 forward（参考理解数据流的分区）
- 
用户没有禁用 chain
推荐阅读：
[Flink异步IO第一讲](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485776&idx=1&sn=bb78ed1f45c7509688526cbd29686f4f&chksm=9f38ea78a84f636e468765b2f870e5f78afe5063f6a94b96cca540c9116fc39d7e4a123b6b6e&scene=21#wechat_redirect)
[Spark2.4.0屏障调度器](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485831&idx=1&sn=fdd4cff8a138cb812f8387a0cef0c39a&chksm=9f38eaafa84f63b961643c9483f0eac1345fd6a887cdd6ad46256bc7e6287c6065a8c7eb3376&scene=21#wechat_redirect)
[推荐两个不错的flink项目](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485652&idx=1&sn=170ece64334979fc5ea5fc5628a33759&chksm=9f38ebfca84f62ea0c1aaaeb8410ec15aec31b1122558662175aa70a2d953f9a3f0e6e904c8f&scene=21#wechat_redirect)
[kafka连接器两种部署模式详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484229&idx=1&sn=d5e87da8a05c40b617f343044ba6d808&chksm=9f38e06da84f697b5181c1aedbeb9205e3af0a81dde6d19487cbde803e01d86fb9367b3a3b3b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
欢迎点赞，转发，给自己小伙伴们学习的机会。
