# Spark—弹性分布式数据集RDD - shine302的博客 - CSDN博客
2018年04月18日 13:57:12[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：146
1）RDD的起源
Hadoop的数据存储模式为：从物理存储上加载数据、操作数据、然后写入物理存储设备。
![](https://img-blog.csdn.net/20180418090247285?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
缺点1：运行迭代式算法很慢、无法进行交互式的数据挖掘
解决方法：数据存储在内存中
缺点2：实现数据集的容错性（数据恢复）代价高（庞大的数据集网络传输）
解决方法：高度受限的共享内存(思想如下)
![](https://img-blog.csdn.net/20180418143844326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）RDD定义
RDD是分布式内存的一个抽象概念，是一种高度受限的共享内存模型，即RDD是只读的记录分区的集合，能够跨越集群的所有节点进行并行计算，是一种基于工作集的应用抽象。
3）RDD是如何存储的？
物理存储：
![](https://img-blog.csdn.net/2018041814475163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
逻辑存储：
        每个RDD 都被分为多个分区，这些分区运行在集群中的不同节点上。
3）RDD的工作流程
     (1) 从外部数据创建出输入RDD。
     (2) 使用诸如filter() 这样的转化操作对RDD 进行转化，以定义新的RDD。
     (3) 告诉Spark 对需要被重用的中间结果RDD 执行persist() 操作。
     (4) 使用行动操作（例如count() 和first() 等）来触发一次并行计算，Spark 会对计算进行 优化后再执行。
4）RDD的创建方式
读取外部数据集，以及在驱动器程序中对一个集合进行并行化（sc.parallelize(["pandas", "i like pandas"])）
5）RDD操作（算子）
RDD操作分为两种：转化操作、行动操作
RDD的转化操作是返回新RDD的操作。转换操作会有谱系（lineage）图记录不同RDD之间的依赖关系。
![](https://img-blog.csdn.net/20180418190837470?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
惰性求值是为了将一些操作合并到一起计算，减少计算数据的步骤。
RDD的行动操作是对数据集进行实际的计算。
6）RDD操作的实现
    基本上需要向Spark传递函数，当你传递的对象是某个对象的成员，或者包含了对某个对象中一个字段的引用时（例
如self.field），Spark 就会把整个对象发到工作节点上，这可能比你想传递的东西大得多。
    替代的方案是，只把你所需要的字段从对象中拿出来放到一个局部变量中，然后传递这个局部变量。
7）常见的转化操作和行动操作
转化操作：
map（x=>x*x）
filter（x=>x!=1）
![](https://img-blog.csdn.net/2018041820153973?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180418202609562?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
标红的开销大：这是因为需要数据混洗
行动操作：
reduce()。它接收一个函数作为参数，这个函数要操作两个RDD 的元素类型的数据并返回一个同样类型的新元素
![](https://img-blog.csdn.net/20180418204521308?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
fold() 和reduce() 类似，接收一个与reduce() 接收的函数签名相同的函数，再加上一个
“初始值”来作为每个分区第一次调用时的结果。你所提供的初始值应当是你提供的操作
的单位元素；也就是说，使用你的函数对这个初始值进行多次计算不会改变结果（例如+
对应的0，* 对应的1，或拼接操作对应的空列表）。
aggregate() 函数则把我们从返回值类型必须与所操作的RDD 类型相同的限制中解放出
来。与fold() 类似，使用aggregate() 时，需要提供我们期待返回的类型的初始值。然后
通过一个函数把RDD 中的元素合并起来放入累加器。考虑到每个节点是在本地进行累加
的，最终，还需要提供第二个函数来将累加器两两合并。
![](https://img-blog.csdn.net/20180418204506938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
take(n) 返回RDD 中的n 个元素，并且尝试只访问尽量少的分区，因此该操作会得到一个
不均衡的集合。需要注意的是，这些操作返回元素的顺序与你预期的可能不一样。
如果为数据定义了顺序，就可以使用top() 从RDD 中获取前几个元素。top() 会使用数据
的默认顺序，但我们也可以提供自己的比较函数，来提取前几个元素。
有时需要在驱动器程序中对我们的数据进行采样。takeSample(withReplacement, num,
seed) 函数可以让我们从数据中获取一个采样，并指定是否替换。
![](https://img-blog.csdn.net/20180418204811801?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
8）不同类型间RDD转换
在Scala 中，将RDD 转为有特定函数的RDD（比如在RDD[Double] 上进行数值操作）是
由隐式转换来自动处理的。2.4.1 节中提到过，我们需要加上import org.apache.spark.
SparkContext._ 来使用这些隐式转换。
9）RDD持久化（缓存）
persist() 会把数据以序列化的形式缓存在JVM 的堆空间中。
如果要缓存的数据太多，内存中放不下，Spark 会自动利用最近最少使用（LRU）的缓存
策略把最老的分区从内存中移除。对于仅把数据存放在内存中的缓存级别，下一次要用到
已经被移除的分区时，这些分区就需要重新计算。但是对于使用内存与磁盘的缓存级别的
分区来说，被移除的分区都会写入磁盘。不论哪一种情况，都不必担心你的作业因为缓存
了太多数据而被打断。不过，缓存不必要的数据会导致有用的数据被移出内存，带来更多
重算的时间开销。
最后，RDD 还有一个方法叫作unpersist()，调用该方法可以手动把持久化的RDD 从缓
存中移除。
![](https://img-blog.csdn.net/20180418205516850?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
10）RDD的依赖关系
窄依赖：
两种：1对1的依赖关系，如filter、map
1对1的范围依赖关系 如 union
![](https://img-blog.csdn.net/20180418210050863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
窄依赖的源码分析：
1对1的依赖关系，子RDD仅仅依赖于父RDD中分区ID相同的分区
1对1的范围依赖关系 如 union
：将父RDD中分区
宽依赖：一种进行RDD操作时，会产生shuffle的操作。
![](https://img-blog.csdn.net/20180418211141581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3）RDD的特性和弹性
      最后讲
