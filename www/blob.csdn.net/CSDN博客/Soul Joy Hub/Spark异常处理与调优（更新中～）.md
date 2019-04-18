# Spark异常处理与调优（更新中～） - Soul Joy Hub - CSDN博客

2016年07月04日 20:54:22[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：9947


# 资源调优

[http://blog.csdn.net/u011239443/article/details/52127689](http://blog.csdn.net/u011239443/article/details/52127689)

# 内存

Memory Tuning，Java对象会占用原始数据2~5倍甚至更多的空间。最好的检测对象内存消耗的办法就是创建RDD，然后放到cache里面去，然后在UI上面看storage的变化；当然也可以使用SizeEstimator来估算。使用-XX:+UseCompressedOops选项可以压缩指针（8字节变成4字节）。在调用collect等等API的时候也要小心——大块数据往内存拷贝的时候心里要清楚。内存要留一些给操作系统，比如20%，这里面也包括了OS的buffer cache，如果预留得太少了，会见到这样的错误：
` Required executor memory (235520+23552 MB) is above the max threshold (241664 MB) of this cluster! Please increase the value of ‘yarn.scheduler.maximum-allocation-mb’.`
或者干脆就没有这样的错误，但是依然有因为内存不足导致的问题，有的会有警告，比如这个：
`“ 16/01/13 23:54:48 WARN scheduler.TaskSchedulerImpl: Initial job has not accepted any resources; check your cluster UI to ensure that workers are registered and have sufficient memory`
有的时候连这样的日志都见不到，而是见到一些不清楚原因的executor丢失信息：
`“ Exception in thread “main” org.apache.spark.SparkException: Job aborted due to stage failure: Task 12 in stage 17.0 failed 4 times, most recent failure: Lost task 12.3 in stage 17.0 (TID 1257, ip-10-184-192-56.ec2.internal): ExecutorLostFailure (executor 79 lost)`
Reduce Task的内存使用。在某些情况下reduce task特别消耗内存，比如当shuffle出现的时候，比如sortByKey、groupByKey、reduceByKey和join等，要在内存里面建立一个巨大的hash table。其中一个解决办法是增大level of parallelism，这样每个task的输入规模就相应减小。另外，注意shuffle的内存上限设置，有时候有足够的内存，但是shuffle内存不够的话，性能也是上不去的。我们在有大量数据join等操作的时候，shuffle的内存上限经常配置到executor的50%。

注意原始input的大小，有很多操作始终都是需要某类全集数据在内存里面完成的，那么并非拼命增加parallelism和partition的值就可以把内存占用减得非常小的。我们遇到过某些性能低下甚至OOM的问题，是改变这两个参数所难以缓解的。但是可以通过增加每台机器的内存，或者增加机器的数量都可以直接或间接增加内存总量来解决。

在选择EC2机器类型的时候，要明确瓶颈（可以借由测试来明确），比如我们遇到的情况就是使用r3.8 xlarge和c3.8 xlarge选择的问题，运算能力相当，前者比后者贵50%，但是内存是后者的5倍。

另外，有一些RDD的API，比如cache，persist，都会把数据强制放到内存里面，如果并不明确这样做带来的好处，就不要用它们。

# Executor&Task Lost

## Error Log
- executor lost

```
WARN TaskSetManager: Lost task 1.0 in stage 0.0 (TID 1, aa.local):
    ExecutorLostFailure (executor lost)
```
- task lost

```
WARN TaskSetManager: Lost task 69.2 in stage 7.0 (TID 1145, 192.168.47.217):
    java.io.IOException: Connection from /192.168.47.217:55483 closed
```
- 各种timeout

```
java.util.concurrent.TimeoutException: Futures timed out after [120 second]

    ERROR TransportChannelHandler: Connection to /192.168.47.212:35409 
    has been quiet for 120000 ms while there are outstanding requests.
    Assuming connection is dead; please adjust spark.network.
    timeout if this is wrong
```

> 
由网络或者gc引起,worker或executor没有接收到executor或task的心跳反馈。 

  提高 spark.network.timeout 的值，根据情况改成300(5min)或更高。 

  默认为 120(120s),配置所有网络传输的延时，如果没有主动设置以下参数，默认覆盖其属性  

       spark.core.connection.ack.wait.timeout 

      spark.akka.timeout 

      spark.storage.blockManagerSlaveTimeoutMs 

      spark.shuffle.io.connectionTimeout 

      spark.rpc.askTimeout or spark.rpc.lookupTimeout

# Spark 2.0.0 SPARK-SQL returns NPE Error

## Error Log

```
com.esotericsoftware.kryo.KryoException: java.lang.NullPointerException
Serialization trace:
underlying (org.apache.spark.util.BoundedPriorityQueue)
    at com.esotericsoftware.kryo.serializers.ObjectField.read(ObjectField.java:144)
    at com.esotericsoftware.kryo.serializers.FieldSerializer.read(FieldSerializer.java:551)
    at com.esotericsoftware.kryo.Kryo.readClassAndObject(Kryo.java:793)
    at com.twitter.chill.SomeSerializer.read(SomeSerializer.scala:25)
    at com.twitter.chill.SomeSerializer.read(SomeSerializer.scala:19)
    at com.esotericsoftware.kryo.Kryo.readClassAndObject(Kryo.java:793)
    at org.apache.spark.serializer.KryoSerializerInstance.deserialize(KryoSerializer.scala:312)
    at org.apache.spark.scheduler.DirectTaskResult.value(TaskResult.scala:87)
    at org.apache.spark.scheduler.TaskResultGetter$$anon$2$$anonfun$run$1.apply$mcV$sp(TaskResultGetter.scala:66)
    at org.apache.spark.scheduler.TaskResultGetter$$anon$2$$anonfun$run$1.apply(TaskResultGetter.scala:57)
    at org.apache.spark.scheduler.TaskResultGetter$$anon$2$$anonfun$run$1.apply(TaskResultGetter.scala:57)
    at org.apache.spark.util.Utils$.logUncaughtExceptions(Utils.scala:1793)
    at org.apache.spark.scheduler.TaskResultGetter$$anon$2.run(TaskResultGetter.scala:56)
    at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1142)
    at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:617)
    at java.lang.Thread.run(Thread.java:745)
Caused by: java.lang.NullPointerException
    at org.apache.spark.sql.catalyst.expressions.codegen.LazilyGeneratedOrdering.compare(GenerateOrdering.scala:157)
    at org.apache.spark.sql.catalyst.expressions.codegen.LazilyGeneratedOrdering.compare(GenerateOrdering.scala:148)
    at scala.math.Ordering$$anon$4.compare(Ordering.scala:111)
    at java.util.PriorityQueue.siftUpUsingComparator(PriorityQueue.java:669)
    at java.util.PriorityQueue.siftUp(PriorityQueue.java:645)
    at java.util.PriorityQueue.offer(PriorityQueue.java:344)
    at java.util.PriorityQueue.add(PriorityQueue.java:321)
    at com.twitter.chill.java.PriorityQueueSerializer.read(PriorityQueueSerializer.java:78)
    at com.twitter.chill.java.PriorityQueueSerializer.read(PriorityQueueSerializer.java:31)
    at com.esotericsoftware.kryo.Kryo.readObject(Kryo.java:711)
    at com.esotericsoftware.kryo.serializers.ObjectField.read(ObjectField.java:125)
    ... 15 more
16/05/24 09:42:53 ERROR SparkSQLDriver: Failed in [     select 
             dt.d_year 
            ,item.i_brand_id brand_id 
            ,item.i_brand brand
            ,sum(ss_ext_sales_price) sum_agg
     from  date_dim dt 
          ,store_sales
          ,item
     where dt.d_date_sk = store_sales.ss_sold_date_sk
       and store_sales.ss_item_sk = item.i_item_sk
       and item.i_manufact_id = 436
       and dt.d_moy=12
     group by dt.d_year
          ,item.i_brand
          ,item.i_brand_id
     order by dt.d_year
             ,sum_agg desc
             ,brand_id
     limit 100]
```

莫名其妙的出现空指针异常～后来在网上发现其他人的类似情况：

> 
When kryo serialization is used, the query fails when ORDER BY and LIMIT is combined. After removing either ORDER BY or LIMIT clause, the query also runs.

查了一下，发现是spark 2.0.0对kryo序列化的依赖有bug，到SPARK_HOME/conf/spark-defaults.conf

默认为 ：
`# spark.serializer                 org.apache.spark.serializer.KryoSerializer`
改成：
`spark.serializer org.apache.spark.serializer.JavaSerializer`
![](https://img-blog.csdn.net/20170625152617464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

