
# spark 宽窄依赖 - leofionn的博客 - CSDN博客


2018年03月22日 14:34:46[leofionn](https://me.csdn.net/qq_36142114)阅读数：194


# 宽依赖与窄依赖
窄依赖是指父RDD的每个分区只被子RDD的一个分区所使用，子RDD分区通常对应常数个父RDD分区(O(1)，与数据规模无关)
ShuffleDependency和Hadoop MapReduce中Shuffle的数据依赖相同
相应的，宽依赖是指父RDD的每个分区都可能被多个子RDD分区所使用，子RDD分区通常对应所有的父RDD分区(O(n)，与数据规模有关)
NarrowDependency指某个具体的Rdd，其区分partitoin a 最多被子RDD中的一个分区partitoin b 依赖。此种情况只有Map任务，是不需要发生Shuffle过程的，窄依赖由分为1：1,1:n
宽依赖和窄依赖如下图所示：
![宽依赖和窄依赖示例](https://img-blog.csdn.net/20160913233559680)
相比于宽依赖，窄依赖对优化很有利 ，主要基于以下两点：
宽依赖往往对应着shuffle操作，需要在运行过程中将同一个父RDD的分区传入到不同的子RDD分区中，中间可能涉及多个节点之间的数据传输；而窄依赖的每个父RDD的分区只会传入到一个子RDD分区中，通常可以在一个节点内完成转换。
当RDD分区丢失时（某个节点故障），spark会对数据进行重算。
对于窄依赖，由于父RDD的一个分区只对应一个子RDD分区，这样只需要重算和子RDD分区对应的父RDD分区即可，所以这个重算对数据的利用率是100%的；
对于宽依赖，重算的父RDD分区对应多个子RDD分区，这样实际上父RDD 中只有一部分的数据是被用于恢复这个丢失的子RDD分区的，另一部分对应子RDD的其它未丢失分区，这就造成了多余的计算；更一般的，宽依赖中子RDD分区通常来自多个父RDD分区，极端情况下，所有的父RDD分区都要进行重新计算。
如下图所示，b1分区丢失，则需要重新计算a1,a2和a3，这就产生了冗余计算(a1,a2,a3中对应b2的数据)。
![宽依赖](https://img-blog.csdn.net/20160913234059245)

![](//img-blog.csdn.net/20180322142846778?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



【来自@若泽大数据】


