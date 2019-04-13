
# Spark Streaming如何使用checkpoint容错 - leofionn的博客 - CSDN博客


2018年05月14日 17:02:26[leofionn](https://me.csdn.net/qq_36142114)阅读数：35个人分类：[spark																](https://blog.csdn.net/qq_36142114/article/category/7640197)[群问题																](https://blog.csdn.net/qq_36142114/article/category/7681708)[
							](https://blog.csdn.net/qq_36142114/article/category/7640197)


在互联网场景下，经常会有各种实时的数据处理，这种处理方式也就是流式计算，延迟通常也在毫秒级或者秒级，比较有代表性的几个开源框架，分别是Storm，Spark Streaming和Filnk。
曾经在一个项目里面用过阿里改造后的JStrom，整体感受就是编程略复杂，在不使用Trident Api的时候是不能保证准确一次的数据处理的，但是能保证不丢数据，但是不保证数据重复，我们在使用期间也出现过几次问题，bolt或者worker重启时候会导致大量数据重复计算，这个问没法解决，如果想解决就得使用Trident来保证，使用比较繁琐。
最近在做一个实时流计算的项目，采用的是Spark Steaming，主要是对接Spark方便，当然后续有机会也会尝试非常具有潜力的Filnk，大致流程，就是消费kafka的数据，然后中间做业务上的一些计算，中间需要读取redis，计算的结果会落地在Hbase中，Spark2.x的Streaming能保证准确一次的数据处理，通过spark本身维护kafka的偏移量，但是也需要启用checkpoint来支持，因为你没法预料到可能出现的故障，比如断电，系统故障，或者JVM崩溃等等。
鉴于上面的种种可能，Spark Streaming需要通过checkpoint来容错，以便于在任务失败的时候可以从checkpoint里面恢复。
在Spark Streaming里面有两种类型的数据需要做checkpoint：
A :元数据信息checkpoint 主要是驱动程序的恢复
（1）配置 构建streaming应用程序的配置
（2）Dstream操作 streaming程序中的一系列Dstream操作
（3）没有完成的批处理 在运行队列中的批处理但是没有完成
B：消费数据的checkpoint
保存生成的RDD到一个可靠的存储系统中，常用的HDFS，通常有状态的数据横跨多个batch流的时候，需要做checkpoint

总结下：
元数据的checkpoint是用来恢复当驱动程序失败的场景下
而数据本身或者RDD的checkpoint通常是用来容错有状态的数据处理失败的场景

大多数场景下没有状态的数据或者不重要的数据是不需要激活checkpoint的，当然这会面临丢失少数数据的风险（一些已经消费了，但是没有处理的数据）

如何在代码里面激活checkpoint？
Java代码![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
// 通过函数来创建或者从已有的checkpoint里面构建StreamingContext
def functionToCreateContext(): StreamingContext = {
val ssc =newStreamingContext(...)// new context
val rdds = ssc.socketTextStream(...)// create DStreams
...
ssc.checkpoint("/spark/kmd/checkpoint")// 设置在HDFS上的checkpoint目录
//设置通过间隔时间，定时持久checkpoint到hdfs上
rdds.checkpoint(Seconds(batchDuration*5))
rdds.foreachRDD(rdd=>{
//可以针对rdd每次调用checkpoint
//注意上面设置了，定时持久checkpoint下面这个地方可以不用写
rdd.checkpoint()
}
)
//返回ssc
ssc
}
def main(args:Array){
// 创建context
val context = StreamingContext.getOrCreate(checkpointDirectory, functionToCreateContext _)
// 启动流计算
context.start()
context.awaitTermination()
}

启动项目之后，我们能在HDFS上看到对应目录下面的checkpoint内容
![](http://note.youdao.com/yws/public/resource/425670d240ae5bb553c52bc7566071c2/B159E8729B7A4945B26AB6A3EFA66297)

这里有有两个坑：

（1）处理的逻辑必须写在functionToCreateContext函数中，你要是直接写在main方法中，在首次启动后，kill关闭，再启动就会报错
关闭命令
Java代码![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
yarn application -kill application_1482996264071_34284

再次启动后报错信息
Java代码![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
has not been initialized when recovery from checkpoint

解决方案：将逻辑写在函数中，不要写main方法中，
（2）首次编写Spark Streaming程序中，因为处理逻辑没放在函数中，全部放在main函数中，虽然能正常运行，也能记录checkpoint数据，但是再次启动先报（1）的错误，然后你解决了，打包编译重新上传服务器运行，会发现依旧报错，这次的错误和（1）不一样：
Java代码![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
xxxx classs ClassNotFoundException

但令你疑惑的是明明打的jar包中包含了，这个类，上一次还能正常运行这次为啥就不能了，问题就出在checkpoint上，因为checkpoint的元数据会记录jar的序列化的二进制文件，因为你改动过代码，然后重新编译，新的序列化jar文件，在checkpoint的记录中并不存在，所以就导致了上述错误，如何解决：
也非常简单，删除checkpoint开头的的文件即可，不影响数据本身的checkpoint
Java代码![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
hadoop fs -rm /spark/kmd/check_point/checkpoint*

然后再次启动，发现一切ok，能从checkpoint恢复数据，然后kill掉又一次启动
就能正常工作了。

最后注意的是，虽然数据可靠性得到保障了，但是要谨慎的设置刷新间隔，这可能会影响吞吐量，因为每隔固定时间都要向HDFS上写入checkpoint数据，spark streaming官方推荐checkpoint定时持久的刷新间隔一般为批处理间隔的5到10倍是比较好的一个方式。

