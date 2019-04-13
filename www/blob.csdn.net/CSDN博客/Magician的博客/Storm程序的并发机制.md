
# Storm程序的并发机制 - Magician的博客 - CSDN博客


2018年04月06日 22:12:00[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：180所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言：](#前言)
[1、概念](#1概念)
[2、配置并行度](#2配置并行度)
[总结：](#总结)


# 目录
## 前言：
为了在以后的实践中提高Storm程序执行的效率，我们还是有必要了解下对应的Storm程序的并发机制。（哈哈，虽然以博主小菜鸟的水平还没有接触到这种提升程序效率层面的东西（这里只是空谈理论），但是Storm的并行机制还是有必要了解下，毕竟技多不压身嘛！）
## 1、概念
并发度：用户指定的一个任务，可以被多个线程执行，并发度的数量等于线程的数量。一个任务的多个线程，会被运行在多个Worker（JVM）上，有一种类似于平均算法的负载均衡策略。尽可能减少网络IO，和Hadoop中的MapReduce中的本地计算的道理一样。
Workers (JVMs): 在一个物理节点上可以运行一个或多个独立的JVM
进程。一个Topology可以包含一个或多个worker(并行的跑在不同的物理机上), 所以worker process就是执行一个topology的子集, 并且worker只能对应于一个topology。
Executors (threads): 在一个worker JVM进程中运行着多个Java线程。一个executor线程可以执行一个或多个tasks。但一般默认每个executor只执行一个task。一个worker可以包含一个或多个executor,每个component (spout或bolt)至少对应于一个executor, 所以可以说executor执行一个compenent的子集, 同时一个executor只能对应于一个component。
Tasks(bolt/spout instances)：Task就是具体的处理逻辑对象，每一个Spout和Bolt会被当作很多task在整个集群里面执行。每一个task对应到一个线程，而stream grouping则是定义怎么从一堆task发射tuple到另外一堆task。你可以调用TopologyBuilder.setSpout和TopBuilder.setBolt来设置并行度 — 也就是有多少个task。
2、配置并行度
对于并发度的配置, 在storm里面可以在多个地方进行配置, 优先级为：  defaults.yaml < storm.yaml < topology-specific configuration< internal component-specific configuration < external component-specific configuration
worker processes的数目, 可以通过配置文件和代码中配置, worker就是执行进程, 所以考虑并发的效果,数目至少应该大亍machines的数目
executor的数目, component的并发线程数，只能在代码中配置(通过setBolt和setSpout的参数), 例如,setBolt(“green-bolt”, new GreenBolt(), 2)
tasks的数目, 可以不配置, 默认和executor1:1, 也可以通过setNumTasks()配置 。Topology的worker数通过config设置，即执行该topology的worker（java）进程数。它可以通过 storm rebalance 命令任意调整。
动态的改变并行度
Storm支持在不 restart topology 的情况下, 动态的改变(增减) worker processes 的数目和 executors 的数目, 称为rebalancing. 通过Storm web UI，或者通过storm rebalance命令实现：
storm rebalance mytopology -n 5 -e blue-spout=3 -e yellow-bolt=10
并发度描述如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180406215931383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
配置实例
Config conf = newConfig();conf.setNumWorkers(2); //用2个workertopologyBuilder.setSpout("blue-spout", newBlueSpout(),2); //设置2个并发度topologyBuilder.setBolt("green-bolt", newGreenBolt(),2).setNumTasks(4).shuffleGrouping("blue-spout"); //设置2个并发度，4个任务topologyBuilder.setBolt("yellow-bolt", newYellowBolt(),6).shuffleGrouping("green-bolt"); //设置6个并发度StormSubmitter.submitTopology("mytopology", conf, topologyBuilder.createTopology());3个组件的并发度加起来是10，就是说拓扑一共有10个executor，一共有2个worker，每个worker产生10 / 2 = 5条线程。
绿色的bolt配置成2个executor和4个task。为此每个executor为这个bolt运行2个task。
总结：
知道了并发机制后，那么在实际生产中如何指定驱动类中每个组件的并发度数量？如何设置worker的数量？有以下几个参考点：
1，根据上游的数据量来设置Spout的并发度。
2，根据业务复杂度和execute方法执行时间来设置Bolt并发度。
3，根据集群的可用资源来配置，一般情况下70%的资源使用率。
4，Worker的数量理论上根据程序并发度总的Task数量来均分，在实际的业务场景中，需要反复调整。

