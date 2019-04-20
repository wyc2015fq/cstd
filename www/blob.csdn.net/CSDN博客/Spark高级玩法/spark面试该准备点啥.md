# spark面试该准备点啥 - Spark高级玩法 - CSDN博客
2019年03月06日 22:16:34[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：528

最近很多球友都说在准备面试，不知道准备点啥，尤其是spark，实际上星球里浪尖分享的内容真的都掌握了，应对一般面试绝对没问题，但是遗憾的事情是很多人都是处于不会主动搜集资料，主动梳理知识，主动记忆整理知识，而是伸手要粮的境地。浪尖觉得这个是阻止你成长的罪魁祸手。前天跟朋友聚餐就说道这种情况，不努力，不加班给自己喂粮的，没有足够量和时间积累的人很难在一个领域里有所建树。
主动学习，保持激情，不断提高～
言归正传，大部分面试者被面试的spark问题估计都会集中于spark core，spark streaming和spark sql，至于mllib和graphx这些估计都是了解项，当然也要逐步去学习structured streaming。所以今天浪尖就帮助大家梳理一下spark的主要知识点吧～
手机客户端纯手打字，有错别字请勿怪～
1. spark的runtime
这个主要是standalone模式下的master和worker，executor，driver，再加上yarn模式下的resourcemanager和nodemanager。要了解一个spark应用提交的过程，也即是driver和executor在集群管理器内部启动的过程，然后就是rpc过程，各个角色的作用。
高手的问题就是如何给两者分配合适内存，然后executor执行task倾斜的表现及如何解决。这些都在星球里分享过了。然后如何对executor的存活挂掉新增进行监控告警。executor动态分配表现及带来的问题。
再高级一点就是driver和executor的类加载器及加载类的原理及过程，当然包括rpc，依赖传输，task调度等。
就这吧……
2.spark core
spark core股名思义就是spark的核心内容，主要是rdd的五大特性及操作算子特点介绍。
DAG的生成，划分，task的调度执行。
数据本地性原理及如何利用和会存在哪些隐患。这也在公众号发过文章和视频了。
reducebykey，groupbykey的区别等等类似算子对比，如何高效使用mappartition，然后foreachPartition与foreach之间的区别及底层实现原理，这些星球里➕公众号都发过了。
宽依赖，窄依赖等老生常谈的不多说了...基础都不去了解记忆的人面试不过很正常。
来点猛料，广播变量的原理及演变过程，使用场景，使用广播变量一定划算吗？大变量咋办呢？
累加器的原理及应用场景，累加器使用有陷阱么？
序列化，反序列化，闭包，垃圾回收机制（过期rdd的回收，cache的回收等）。这个星球里打过了～
checkpoint如何在spark core应用呢？何种场景适合？源码系列教程。
并行度相关配置，这个星球里也反复强调了，合理设置可以大幅度提高性能。
害怕了吗，还是就此打住吧，太多了～
在星球里➕公众号都有讲过了。
3.spark streaming
spark streaming核心原理大家都知道是微批处理。
基于receiver和direct api两种模式的原理，最好读懂源码。
主要是跟Kafka 结合的两种模式的区别。
direct这种模式如何实现仅一次处理。checkpoint的使用。
如何进行状态管理，upstatebykey，redis，hbase，alluxio作为状态管理存储设备的时候优缺点，然后就是故障恢复会引起的问题及如何避免等等吧。
合理设置批处理时间，为啥批处理时间不能太大，也不能太小，task倾斜，数据倾斜如何解决。
内存申请，kafka分区设置的依据是啥？
并行度问题，这个也是浪尖反复强调的，彻底理解对spark任务调优帮助很大。
blockrdd和kafkardd的底层区别。
与spark sql和hivecontext结合使用。
广播变量的使用及释放机制等。
动态分区发现和topic发现机制。
executor存活监控，task执行情况监控，未处理队列积累的健康告警（非常重要）等价于对lagsize的监控告警。
小文件问题，星球里文章很详细。根源上避免才是王道。顺便提一句：为啥namenode那么怕小文件呢？
作为7*24小时的应用程序，如何进行监控告警及故障自动恢复～
可怕的内容，多的一笔，拿着手机一个字一个字打，好累。
4.spark sql
在数仓的领域，实时处理都用它，而且structured streaming也逐步依赖于sql引擎了。
常见算子的使用及理解，并行度问题，大小表join，如何广播小表。
join，group by等数据倾斜如何发现及处理方法，这个浪尖还专门录制过视频，星球里球友应该都知道，不知道回去翻看一下。
常见的存储格式，parquet，txt，json，orc对比及对性能的影响。
调优大部分也是针对并行度，文件大小，数据倾斜，task倾斜，内存和cpu合理设置等。
5.structured streaming
这个我也系统整理了案例，分享到了星球里，要是没用过的话，建议用一下。
动态表，增量sql引擎，仅一次处理，维表join等非常好用，watermark，还有就是繁杂的join 机制。
当然限制还是很多的，期待后续版本。
spark streaming在spark 2.4的时候都没更新了，后面就主推sql引擎相关内容了，还是值得期待的。
不过话虽这么说，我觉得flink也相对好用，就是可能bug多些，新版本好点。
spark streaming structured streaming与flink区别，请参考浪尖以前的文章。
今天用手机客户端手打这篇文章，手指尖都算了，而且地铁坐过了一站，😄，有用就帮忙点个赞，转发一下～
星球里近600位好友都在等你，更多优质内容来吧。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUA897iaVrqzvnyqx3CDj4DuS0h8FicicOCSaQXprPicYeqcg9TfAe3KVtWsX2CQ3bseYUDGIHtpHwbog/640?wx_fmt=jpeg)知识星球
