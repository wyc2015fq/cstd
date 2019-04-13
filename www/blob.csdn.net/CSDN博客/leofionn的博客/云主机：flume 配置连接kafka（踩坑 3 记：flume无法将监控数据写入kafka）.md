
# 云主机：flume 配置连接kafka（踩坑 3 记：flume无法将监控数据写入kafka） - leofionn的博客 - CSDN博客


2018年05月04日 21:19:02[leofionn](https://me.csdn.net/qq_36142114)阅读数：134


1.Jar放进lib目录
[root@yws76 lib]\# ll
total 51064
-rw-r--r-- 1 root root   23819 Jan 12 11:23 AdvancedExecSource.jar
2.配置文件：
[root@yws76 apache-flume-1.7.0-bin]\# cat conf/exec_memory_kafka.properties
\# Name the components on this agent
a1.sources = r1
a1.sinks = k1
a1.channels = c1
\# Describe/configure the custom exec source
a1.sources.r1.type = com.onlinelog.analysis.ExecSource_JSON
a1.sources.r1.command = tail -F /usr/local/software/flume/data/test.log
a1.sources.r1.hostname = hadoop001
a1.sources.r1.servicename = namenode
\# Describe the sink
a1.sinks.k1.type = org.apache.flume.sink.kafka.KafkaSink
a1.sinks.k1.kafka.topic = onlinelogs
a1.sinks.k1.kafka.bootstrap.servers =内网IP:9092
（这里有坑，如果不配置内网，flume无法将监控数据写入kafka）
a1.sinks.k1.kafka.flumeBatchSize = 6000
a1.sinks.k1.kafka.producer.acks = 1
a1.sinks.k1.kafka.producer.linger.ms = 1
a1.sinks.ki.kafka.producer.compression.type = snappy
\# Use a channel which buffers events in memory
a1.channels.c1.type = memory
a1.channels.c1.keep-alive = 90
a1.channels.c1.capacity = 2000000
a1.channels.c1.transactionCapacity = 6000
（若报错内存溢出等异常时，则修改jvm内存大小，参考这篇https://blog.csdn.net/ghost06211/article/details/52881627）
参考官网：内存块：
The events are stored in an in-memory queue with configurable max size. It’s ideal for flows that need higher throughput and are prepared to lose the staged data in the event of a agent failures. Required properties are in**bold**.
|Property Name|Default|Description|
|---|---|---|
|type|–|The component type name, needs to be memory|
|capacity|100|The maximum number of events stored in the channel|
|transactionCapacity|100|The maximum number of events the channel will take from a source or give to a sink per transaction|
|keep-alive|3|Timeout in seconds for adding or removing an event|
|byteCapacityBufferPercentage|20|Defines the percent of buffer between byteCapacity and the estimated total size of all events in the channel, to account for data in headers. See below.|
|byteCapacity|see description|Maximum total|bytes|of memory allowed as a sum of all events in this channel. The implementation only counts the Event body, which is the reason for providing the byteCapacityBufferPercentage configuration parameter as well. Defaults to a computed value equal to 80% of the maximum memory available to the JVM (i.e. 80% of the -Xmx value passed on the command line). Note that if you have multiple memory channels on a single JVM, and they happen to hold the same physical events (i.e. if you are using a replicating channel selector from a single source) then those event sizes may be double-counted for channel byteCapacity purposes. Setting this value to 0 will cause this value to fall back to a hard internal limit of about 200 GB.|
Example for agent named a1:
```python
a1.channels = c1
a1.channels.c1.type = memory
a1.channels.c1.capacity = 10000
a1.channels.c1.transactionCapacity = 10000
a1.channels.c1.byteCapacityBufferPercentage = 20
a1.channels.c1.byteCapacity = 800000
```
）

\# Bind the source and sink to the channel
a1.sources.r1.channels = c1
a1.sinks.k1.channel = c1

3.启动：
nohup  flume-ng agent -c /opt/software/loganalysis/apache-flume-1.7.0-bin/conf -f /opt/software/loganalysis/apache-flume-1.7.0-bin/conf/exec_memory_kafka.properties -n a1 -Dflume.root.logger=INFO,console &

4.查看
tail -F nohup.out
【来自@若泽大数据】

