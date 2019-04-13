
# Spark RDD 之间的依赖关系 - Zhang's Wikipedia - CSDN博客


2019年01月30日 18:26:21[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：91


RDD就是一个**不可变**的带分区的记录集合，Spark提供了RDD上的两类操作，转换（transformation）和动作（action）。
转换是用来定义一个新的RDD，包括map, flatMap, filter, union, sample, join, groupByKey, cogroup, ReduceByKey, cros, sortByKey, mapValues等。
动作是返回一个结果，包括collect, reduce, count, save, lookupKey。
## 1. 宽窄依赖
![](https://img-blog.csdnimg.cn/20190131223839946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=,size_16,color_FFFFFF,t_70)[](https://img-blog.csdnimg.cn/20190131223839946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=,size_16,color_FFFFFF,t_70)依赖关系本质上刻画了一种数据流向；
窄依赖：一父对一子，更严谨地说，最多有一个孩子；
宽依赖：一父对多子；也即父节点对应的分区（partition）会划分为多份（需要 shuffle），分别由子节点进行处理；
这时需要进行 shuffle，也即会依据Record的key进行数据重组，这个过程即为Shuffle（洗牌）
[](https://img-blog.csdnimg.cn/20190131223839946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=,size_16,color_FFFFFF,t_70)

