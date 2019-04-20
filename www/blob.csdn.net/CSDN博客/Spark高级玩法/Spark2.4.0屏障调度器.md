# Spark2.4.0屏障调度器 - Spark高级玩法 - CSDN博客
2018年11月14日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：196
前几天，浪尖发了一篇文章，讲了Spark 2.4发布更新情况：
[Spark2.4.0发布了！](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485800&idx=1&sn=792f4e26ca0bef6118d8faa11bf998d4&chksm=9f38ea40a84f63566997bb4879613f2cbc7f75f726cbd7208b8de9f5a83d9cf03bec9cee7031&scene=21#wechat_redirect)
其中，就有一项说到Spark 为了支持深度学习而引入的屏障调度器。本文就详细讲讲。
基于消息传递结构的计算模型和Spark计算模型是有很大区别。在Spark 内部，每个Stage的某一个一个task不会依赖于相同Stage任何其他的task，因此，Spark的task 可以被独立进行调度执行。为了在Spark中嵌入MPI功能，需要引入一个新的调度模型，暂时命名为“屏障调度”（浪尖直译自barrier scheduling），该调度模型会同时启动任务，并为用户提供足够的信息和工具，将分布式DL训练嵌入到Spark Pipeline中。 Spark还为MPI任务引入了一种新的容错机制。当任何MPI任务在中间失败时，Spark将中止所有任务并重新启动该stage。
**1.****要求**
**概述**
- 
每个job中单个barrier stage。
- 
每个job中多个barrier stage。
- 
多job且每个job都带有barrier stage。
- 
Barrier stage 请求的slot比可用的slot多(无动态资源申请)。
- 
Barrier stage请求的slot比可用的slot多(有动态资源申请)。(Spark 2.4就不要想了)
**目标**
- 
支持barrier调度：对于同一个barrierstage同时启动所有task，并且提供给用户足够的信息和工具，以便用户可以嵌入分布式DL训练模型。
- 
正确的处理失败的场景。
- 
Barrier执行模式支持运行与Standalone模式
- 
使用yarn/mesos/k8s的用户可以再有BarrierStage的时候设置MPI。
**安全**
用户使用外部线程启动MPI任务的时候，存在外部进行不被杀死而导致内存泄漏的风险。Barrier tasks会使用远程客户端相互交流，但是不会影响Spark当前的安全模型。
API变化
```
class RDD[T] {
  /** Indicates that Spark must launch the tasks together for the current stage. */
  def barrier(): RDDBarrier[T] = ???
}
/** A [[TaskContext]] with extra info and tooling for a barrier stage. */
class BarrierTaskContext extends TaskContext {
  /** Sets a global barrier and waits until all tasks in this stage hit this
barrier. */
  def barrier(): Unit = ???
  /** Returns the all task infos in this stage. */
  def getTaskInfos(): Array[BarrierTaskInfo]
}
/** Represents an RDD barrier, which forces Spark to launch tasks of this stage
together. */
class RDDBarrier[T] {
  /** Maps partitions together with a provided [[BarrierTaskContext]]. */
  def mapPartitions[S](f: Iterator[T] => Iterator[S]): RDD[S] = ???
  /** TODO extra conf(e.g. timeout) */
}
```
使用案例
```
rdd.barrier().mapPartitions { iter =>
    // Write iter to disk.
    ???
    // Fetch TaskContext
    val context = BarrierTaskContext.get()
    // Wait until all tasks finished writing.
    context.barrier()
    // The 0-th task launches an MPI job.
    if (context.partitionId() == 0) {
      val hosts = context.getTaskInfos().map(_.address)
         // Set up MPI machine file using host infos.
      ???
      // Launch the MPI job by calling mpirun.
??? }
    // Wait until the MPI job finished.
    context.barrier()
    // Collect output and return.
??? }
```
**3.****架构**
**设计提议**
为了使spark支持屏障调度(barrier scheduling)，在Spark内部增加了RDDBarrier和BarrierTaskContext。
**BarrierStage**
如果没有充足的slot资源，barrier stage不会被拉起(也即是空闲的core 必须能够拉起该barrier所有tasks)，这样设计使为了满足一次拉起所有task的目标。
同时当任意的task执行失败的时候，总是重启整个barrier stage。
判断一个stage是否是Barrier stage的一种方式是跟踪Stage所包含的RDD，如果该stage包含RDDBarrier 或者至少一个父RDD是RDDBarrier，那么该stage是一个barrier stage，当然要以shuffleDependency为界限。
**调度Barrier Tasks**
目前，TaskScheduler会尽可能的在可用的slot上调度task，所以通常不会同时启动同一个stage的所有task。因此需要在barrier stage 的task在调度之前加上资源可用性判断。由于任务的局部性问题，仍然可能仅启动整个barrier stage的部分tasks，因此必须在启动任务之前在此检查确认同一个barrier stage的所有task同时被启动。
Barrier tasks预计比常规tasks具有更长的生命周期，因此barriertasks可能会在相对长的时间范围内占用集群资源，后续提交的任务估计会延迟运行或者仅使用更少的slot运行。建议使用Fair调度策略进行调度，而不是默认的FIFO调度策略，并将barrier任务独立运行，这样至少可以保证普通任务可以在配置给定最少的集群资源上运行。
另一个问题是barrier stage可以提交，但是集群当前没有足够的slot来同时启动所有barrier tasks。如果启用了动态资源分配，则在等待一段时间后，可能会或可能不会满足要求（取决于允许的最大节点）。对于Spark 2.4，提出了一个简单的解决方案，它只检查当前运行的slot的总数，如果数量不足以同时启动同一个stage的所有屏障任务，则取消该job。目标是在3.0的时候可以更好地与动态资源分配集成。对于Spark 2.4，在启用动态资源分配时，job会立即失败，或者job无法连续提交，因为它试图提交一个barrier stage，该stage需要比集群中当前可用的slot更多的slot。
**Task Barrier**
Barrier tasks应允许用户在task执行过程中插入同步操作，这可以通过在BarrierTaskContext中引入全局barrier操作来实现，这使得当前任务等待直到同一stage中的所有task都达到此barrier。将为BarrierTaskContext.barrier（）提交单独的设计文档。
关注公众号，**bigdatatip**，回复barrier 即可获得该文档。
**失败容错**
为确保正确性，当任何task失败时，barrier stage始终会重试整个stage。因此，将要求杀死失败stage的所有正在运行的任务，并且还保证每个单个stage最多只能运行一个taskset (没有zombie task)，这是非常简单的。理想情况下，除了在zombie taskset中杀死正在运行的任务需要一段时间，每个单一stage只应运行一个taskset，必须将失败的taskset标记为zombie 并正确处理TaskKilled消息。
**推测任务(Speculativetask)**
在barrier 执行模式中，要求每个barrier task必须仅有一个唯一的task ID，目的是其他的tasks 可以直接使用该ID和它交互。这也就意味着每个task只能尝试启动一次，因此必须禁止推测执行。
此外，3.0的时候可能会将Spark任务推测执行设置为单个stage的配置而不是全局配置。
**SparkContext.runJob()/PartitionPruningRDD**
SparkContext.runJob()执行的时候可以仅是所有分区的子集，其中一个用例是RDD.first()，不会执行所有分区。这种是与barrer执行模式冲突的，可能无法启动某些barrier tasks。在barrier stage检测到这种用法，会由于不支持该操作而抛出异常。
ParititionPruningRDD的情况类似，它只在满足`partitionFilterFunc`的分区上启动任务。我们将在barrierstage 检测PartitionPruningRDD并抛出显式异常。
以上问题都与父RDD与生成的RDD具有不同分区数的问题有关（例如union()/ coalesce()/ first()/ take()/ PartitionPruningRDD），因此可以检测RDD的血统链条，然后在job 提交的时候立即停止。
如果RDD依赖于多个barrier RDD（例如，barrierRdd1.zip（barrierRdd2）），也将立即停止，如果发生这种情况，则无法确保`barrier()`调用的正确行为。
针对Spark 3.0，可以进一步调查上述用例并提出支持它们的方法。
本文牵涉到的英文原文，关注公众号 **bigdatatip**，输入 **barrier** 获取。
推荐阅读：
[大数据啊大数据！](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485810&idx=1&sn=178ce450fef932d27026dc5e2951ac3a&chksm=9f38ea5aa84f634c91b518671e135a4761e9d0a0a0912488944653aac2baf66f83579540078b&scene=21#wechat_redirect)
[Spark SQL的几个里程碑！](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485824&idx=1&sn=c92e846196fc11c01e9edfa8db3e7ff8&chksm=9f38eaa8a84f63bea211fc1d3950954833285c7b77cb9b48cda240676776e79f14788a944d6d&scene=21#wechat_redirect)
[最常见的Kafka面试题及答案](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485515&idx=1&sn=d1a33b86adfef9305fa22d4528f184fe&chksm=9f38eb63a84f6275b476193d011e708f7ec2a976bc1c0180606e0627e580c965d5c42f70fec2&scene=21#wechat_redirect)
[干货|kafka流量监控的原理及实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485616&idx=1&sn=68b76f206dddeca045363413f783a986&chksm=9f38eb98a84f628ea8562c2a57225cedeea22159e96888705751c540ff3762282650ce8ce1e6&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
欢迎点赞，转发！
