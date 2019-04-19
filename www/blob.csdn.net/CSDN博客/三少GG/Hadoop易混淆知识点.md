# Hadoop易混淆知识点 - 三少GG - CSDN博客
2013年08月28日 10:42:50[三少GG](https://me.csdn.net/scut1135)阅读数：924
## 1.解析Partition
Mapper最终处理的键值对<key, value>，是需要送到Reducer去合并的，合并的时候，有相同key的键/值对会送到同一个Reducer那。**哪个key到哪个Reducer的分配过程，是由Partitioner规定的**。它只有一个方法，
**[java]**[view
 plain](http://blog.csdn.net/xw13106209/article/details/6912069#)[copy](http://blog.csdn.net/xw13106209/article/details/6912069#)
- getPartition(Text key, Text value, int numPartitions)  
输入是Map的结果对<key, value>和Reducer的数目，输出则是分配的Reducer（整数编号）。**就是指定Mappr输出的键值对到哪一个reducer上去**。系统缺省的Partitioner是HashPartitioner，它以key的Hash值对Reducer的数目取模，得到对应的Reducer。**这样保证如果有相同的key值，肯定被分配到同一个reducre上。如果有N个reducer，编号就为0,1,2,3……(N-1)**。
Reducer是所有用户定制Reducer类的基类，和Mapper类似，它也有setup，reduce，cleanup和run方法，其中setup和cleanup含义和Mapper相同，reduce是真正合并Mapper结果的地方，它的输入是key和这个key对应的所有value的一个**迭代器**，同时还包括Reducer的上下文。系统中定义了两个非常简单的Reducer，IntSumReducer和LongSumReducer，分别用于对整形/长整型的value求和。
2. GroupingComparator
《Pro Hadoop》中解释:
reduce方法每次是读一条记录，读到相应的key，但是处理value集合时，处理完当前记录的values后，还会判断下一条记录是不是和当前的key是不是同一个组，如果是的话，会继续读取这些记录的值，而这个记录也会被认为已经处理了，直到记录不是当前组，这次reduce调用才结束，这样一次reduce调用就会处理掉一个组中的所有记录，而不仅仅是一条了。
这个有什么用呢？如果不用分组，那么同一组的记录就要在多次reduce方法中独立处理，那么有些状态数据就要传递了，就会增加复杂度，在一次调用中处理的话，这些状态只要用方法内的变量就可以的。比如查找最大值，只要读第一个值就可以了。
