# spark job运行参数优化 - zhanlijun - 博客园
# [spark job运行参数优化](https://www.cnblogs.com/LBSer/p/4129481.html)
#### [http://www.cnblogs.com/LBSer/p/4129481.html](http://www.cnblogs.com/LBSer/p/4129481.html)
## 一、问题
      使用spark join两张表（5000w*500w）总是出错，报的异常显示是在shuffle阶段。
```java;gutter
14/11/27 12:05:49 ERROR storage.DiskBlockObjectWriter: Uncaught exception while reverting partial writes to file /hadoop/application_1415632483774_448143/spark-local-20141127115224-9ca8/04/shuffle_1_1562_27
java.io.FileNotFoundException: /hadoop/application_1415632483774_448143/spark-local-20141127115224-9ca8/04/shuffle_1_1562_27 (No such file or directory)
        at java.io.FileOutputStream.open(Native Method)
        at java.io.FileOutputStream.<init>(FileOutputStream.java:212)
        at org.apache.spark.storage.DiskBlockObjectWriter.revertPartialWritesAndClose(BlockObjectWriter.scala:178)
        at org.apache.spark.shuffle.hash.HashShuffleWriter$$anonfun$revertWrites$1.apply(HashShuffleWriter.scala:118)
        at org.apache.spark.shuffle.hash.HashShuffleWriter$$anonfun$revertWrites$1.apply(HashShuffleWriter.scala:117)
        at scala.collection.IndexedSeqOptimized$class.foreach(IndexedSeqOptimized.scala:33)
        at scala.collection.mutable.ArrayOps$ofRef.foreach(ArrayOps.scala:108)
        at org.apache.spark.shuffle.hash.HashShuffleWriter.revertWrites(HashShuffleWriter.scala:117)
        at org.apache.spark.shuffle.hash.HashShuffleWriter.stop(HashShuffleWriter.scala:89)
        at org.apache.spark.scheduler.ShuffleMapTask.runTask(ShuffleMapTask.scala:73)
        at org.apache.spark.scheduler.ShuffleMapTask.runTask(ShuffleMapTask.scala:41)
        at org.apache.spark.scheduler.Task.run(Task.scala:54)
        at org.apache.spark.executor.Executor$TaskRunner.run(Executor.scala:177)
        at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1145)
        at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:615)
        at java.lang.Thread.run(Thread.java:724)
```
    出问题的代码块（scala）
```
1 val cRdd = iRdd.leftOuterJoin(label).map {
2      case (id, (iMap, Some(set))) => (id, (iMap, set))
3      case (id, (iMap, None)) => (id, (iMap, new HashSet[Int]()))
4    }.persist(StorageLevel.MEMORY_AND_DISK)
```
## 二、问题分析与解决
     一般spark job很多问题都是来源于系统资源不够用，通过监控日志等判断是内存资源占用过高导致的问题，因此尝试通过配置参数的方法来解决。
### 1）--conf spark.akka.frameSize=100
     此参数控制Spark中通信消息的最大容量 （如task的输出结果），默认为10M。当处理大数据时，task的输出可能会大于这个值，需要根据实际数据设置一个更高的值。尝试将此参数设置成100M后，问题未能解决。
### 2）--conf spark.shuffle.manager=SORT
     Spark默认的shuffle采用Hash模式，在HASH模式下，每一次shuffle会生成M*R的数量的文件（M指的是Map的数目，R指的是Reduce的数目），而当Map和Reduce的数目开得较大时，会产生相当规模的文件，与此同时带来了大量的内存开销。
     为了降低系统资源，可以采用Sort模式，Sort模式只产生M数量的文件。具体可以参考：[Sort-based Shuffle之初体验](http://www.cnblogs.com/hseagle/p/3979744.html)
     在我们的应用场景下，采用Sort模式后，shuffle时间比之前增大了1/3，但是问题依旧未解决。
### 3）--conf spark.yarn.executor.memoryOverhead=4096
     executor堆外内存设置。起初是1024M，未能跑过，后改为4096M，Job就能跑通，原因是程序使用了大量的堆外内存。

