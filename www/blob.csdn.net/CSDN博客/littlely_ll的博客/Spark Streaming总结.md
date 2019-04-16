# Spark Streaming总结 - littlely_ll的博客 - CSDN博客





2017年09月24日 16:37:44[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：207标签：[spark																[Streaming](https://so.csdn.net/so/search/s.do?q=Streaming&t=blog)](https://so.csdn.net/so/search/s.do?q=spark&t=blog)
个人分类：[pyspark](https://blog.csdn.net/littlely_ll/article/category/6967450)








## 初始化StreamingContext

创建SparkContext后要做的事情： 

    1. 通过创建DStreams定义输出源 

    2. 通过应用transform算子定义流计算，输出操作到Dstream 

    3. 用streamingContext.start()开始接受数据并进行处理 

    4. 用streamingContext.awaitTermination()等待处理过程结束 

    5. 可以使用streamingContext.stop()手动停止处理过程
注意事项： 

    • 一旦context开始，就不能设置或添加新的流计算 

    • 一旦context被终止，就不能在重启 

    • 只有一个StreamingContext能同时在JVM中活动 

    • 停止StreamingContext同时也会停止SparkContext,要想只停止StreamingContext设置stop()选项stopSparkContext为false 

    • SparkContext能重新使用创建多个StreamingContext，只要在下一个StreamingContext被创建之前，先前的StreamingContext被终止并且SparkContext没有被终止
### Input DStreams and Receivers

Spark Streaming 提供了两种内置的streaming sources:

```
• Basic sources: 在StreamingContext API中直接可用，例如文件系统和socket连接
• Advanced sources: 像Kafka, Flume, etc.
```

注意事项：

```
▪ 当本地运行spark streaming程序时， 不要用“local”或“local[1]”作为master URL，这两个都意味着只有一个线程用于运行任务。如果你基于一个receiver所使用input Dstream的话，那么单个线程会用于运行receiver，而不会有线程去处理接受的数据。因此在本地运行时，用“local[n]”作为master URL。
▪ 在集群上运行时，分配给spark streaming应用的内核数量要大于receiver的数量，否则系统将会只接收数据，而不能处理它。
```

### Transformations on DStreams
|Transformation|Meaning|
|----|----|
|map(func)|Return a new DStream by passing each element of the source DStream through a function func.|
|flatMap(func)|Similar to map, but each input item can be mapped to 0 or more output items.|
|filter(func)|Return a new DStream by selecting only the records of the source DStream on which func returns true.|
|repartition(numPartitions)|Changes the level of parallelism in this DStream by creating more or fewer partitions.|
|union(otherStream)|Return a new DStream that contains the union of the elements in the source DStream and otherDStream.|
|count()|Return a new DStream of single-element RDDs by counting the number of elements in each RDD of the source DStream.|
|reduce(func)|Return a new DStream of single-element RDDs by aggregating the elements in each RDD of the source DStream using a function func (which takes two arguments and returns one). The function should be associative and commutative so that it can be computed in parallel.|
|countByValue()|When called on a DStream of elements of type K, return a new DStream of (K, Long) pairs where the value of each key is its frequency in each RDD of the source DStream.|
|reduceByKey(func, [numTasks])|When called on a DStream of (K, V) pairs, return a new DStream of (K, V) pairs where the values for each key are aggregated using the given reduce function. Note: By default, this uses Spark’s default number of parallel tasks (2 for local mode, and in cluster mode the number is determined by the config property spark.default.parallelism) to do the grouping. You can pass an optional numTasks argument to set a different number of tasks.|
|join(otherStream, [numTasks])|When called on two DStreams of (K, V) and (K, W) pairs, return a new DStream of (K, (V, W)) pairs with all pairs of elements for each key.|
|cogroup(otherStream, [numTasks])|When called on a DStream of (K, V) and (K, W) pairs, return a new DStream of (K, Seq[V], Seq[W]) tuples.|
|transform(func)|Return a new DStream by applying a RDD-to-RDD function to every RDD of the source DStream. This can be used to do arbitrary RDD operations on the DStream.|
|updateStateByKey(func)|Return a new “state” DStream where the state for each key is updated by applying the given function on the previous state of the key and the new values for the key. This can be used to maintain arbitrary state data for each key.|

#### UpdateStateByKey 算子

当要用新信息连续更新状态时，updateStateByKey算子允许你保持任意状态，需要做两步：

```
• 定义状态: 状态可以是任意数据类型
• 定义状态更新函数：用先前的状态和后来输入流的新值，怎样设置更新状态函数
```

例：定义一个状态更新函数

```python
def updateFunction(newValues, runningCount):
    if runningCount is None:
        runningCount = 0
    return sum(newValues, runningCount)
```

应用于包含词Dstream（如pairs Dstream包含(word, 1)）
`runningCounts = pairs.updateStateByKey(updateFunction)`
更新函数被调用于每一个单词。 
**应用updateStateByKey要求配置checkpoint路径**

#### Transform算子

例：

```
spamInfoRDD = sc.pickleFile(...)  # RDD containing spam information
## join data stream with spam information to do data cleaning
cleanedDStream = wordCounts.transform(lambda rdd: rdd.join(spamInfoRDD).filter(...))
```

Window算子 
![这里写图片描述](https://img-blog.csdn.net/20170924162615543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从上图知，算子应用到数据的3个时间段，滑块对应2个时间段 

Window算子需要设置两个参数： 

    • Window length: window的持续时间(图中为3) 

    • Sliding interval：window算子的应用区间(图中为2)
例： 

比如要产生一个每10秒生成前30秒的汇总数据，则可以使用reduceByKeyAndWindow

```
Reduce last 30 seconds of data, every 10 seconds
windowedWordCounts = pairs.reduceByKeyAndWindow(lambda x, y: x + y, lambda x, y: x - y, 30, 10)
```

Pairs是前30秒Dstream(word, 1)由reduceByKey生成的。

### 常用window算子：
|ransformation|Meaning|
|----|----|
|window(windowLength, slideInterval)|Return a new DStream which is computed based on windowed batches of the source DStream.|
|countByWindow(windowLength, slideInterval)|Return a sliding window count of elements in the stream.|
|reduceByWindow(func, windowLength, slideInterval)|Return a new single-element stream, created by aggregating elements in the stream over a sliding interval using func. The function should be associative and commutative so that it can be computed correctly in parallel.|
|reduceByKeyAndWindow(func, windowLength, slideInterval, [numTasks])|When called on a DStream of (K, V) pairs, returns a new DStream of (K, V) pairs where the values for each key are aggregated using the given reduce function func over batches in a sliding window. Note: By default, this uses Spark’s default number of parallel tasks (2 for local mode, and in cluster mode the number is determined by the config property spark.default.parallelism) to do the grouping. You can pass an optional numTasks argument to set a different number of tasks.|
|reduceByKeyAndWindow(func, invFunc, windowLength, slideInterval, [numTasks])|A more efficient version of the above reduceByKeyAndWindow() where the reduce value of each window is calculated incrementally using the reduce values of the previous window. This is done by reducing the new data that enters the sliding window, and “inverse reducing” the old data that leaves the window. An example would be that of “adding” and “subtracting” counts of keys as the window slides. However, it is applicable only to “invertible reduce functions”, that is, those reduce functions which have a corresponding “inverse reduce” function (taken as parameter invFunc). Like in reduceByKeyAndWindow, the number of reduce tasks is configurable through an optional argument. Note that checkpointing must be enabled for using this operation.|
|transform|Meaning|
|----|----|
|countByValueAndWindow(windowLength,slideInterval,[numTasks])|When called on a DStream of (K, V) pairs, returns a new DStream of (K, Long) pairs where the value of each key is its frequency within a sliding window. Like in reduceByKeyAndWindow, the number of reduce tasks is configurable through an optional argument.|

## Checkpointing

Spark streaming必须对运行失败有弹性，checkpoint检查容错性

两种类型数据需要checkpoint： 

    • Metadata checkpointing：保存所定义的流式计算信息，用来回复恢复从运行的节点上失败的信息，Metadata包括： 

        ○ Configuration: 用来创建streaming application的 

        ○ Dstream operations: 定义streaming application的Dstream operation的集合 

        ○ Incomplete batches: 正在排队还没有完成的工作 

    • Data checkpointing：把产生的RDDs保存到可信的容器
总的来说，metadata checkpointing对于从驱动失败中恢复很重要，而数据和RDD checkpointing对于使用stateful transformations的基本函数也很重要。

## 什么时候加强checkpointing

```
• 应用状态转换算子(stateful transformations)：例如使用updateStateByKey或reduceByKeyAndWindow
• 从驱动器运行失败的程序中恢复
```









