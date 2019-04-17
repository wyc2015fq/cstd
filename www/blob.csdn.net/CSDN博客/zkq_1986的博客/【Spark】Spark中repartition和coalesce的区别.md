# 【Spark】Spark中repartition和coalesce的区别 - zkq_1986的博客 - CSDN博客





2017年12月07日 16:35:44[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：322








repartition(numPartitions:Int):RDD[T]和coalesce(numPartitions:Int，shuffle:Boolean=false):RDD[T]

他们两个都是RDD的分区进行重新划分，repartition只是coalesce接口中shuffle为true的简易实现，（假设RDD有N个分区，需要重新划分成M个分区）

1）、N<M，分区变多。一般情况下N个分区有数据分布不均匀的状况，利用HashPartitioner函数将数据重新分区为M个，这时需要将shuffle设置为true。

2）如果N>M，分区变少，并且N和M相差不多，(假如N是1000，M是100)那么就可以将N个分区中的若干个分区合并成一个新的分区，最终合并为M个分区，这时可以将shuff设置为false，在shuffl为false的情况下，如果M>N时，coalesce为无效的，不进行shuffle过程，父RDD和子RDD之间是窄依赖关系。

3）如果N>M，分区变少，并且两者相差悬殊，这时如果将shuffle设置为false，父子ＲＤＤ是窄依赖关系，他们同处在一个Stage中，就可能造成spark程序的并行度不够，从而影响性能，如果在M为1的时候，为了使coalesce之前的操作有更好的并行度，可以讲shuffle设置为true。

总之：如果shuff为false时，如果传入的参数大于现有的分区数目，RDD的分区数不变，也就是说不经过shuffle，是无法将RDDde分区数变多的。



