
# Flume介绍 - leofionn的博客 - CSDN博客


2018年04月09日 22:17:04[leofionn](https://me.csdn.net/qq_36142114)阅读数：39


一、Flume是一种分布式，可靠且可用的服务，用于高效地收集，汇总和移动大量日志数据。它具有基于流式数据流的简单而灵活的架构。它具有可靠的可靠性机制和许多故障转移和恢复机制，具有强大的容错性和容错能力。它使用一个简单的可扩展数据模型，允许在线分析应用程序。
![](https://img-blog.csdn.net/20180409220645353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
二、flume中event的相关概念：flume的核心是把数据从数据源(source)收集过来，在将收集到的数据送到指定的目的地(sink)。为了保证输送的过程一定成功，在送到目的地(sink)之前，会先缓存数据(channel),待数据真正到达目的地(sink)后，flume在删除自己缓存的数据。
在整个数据的传输的过程中，流动的是event，即事务保证是在event级别进行的。那么什么是event呢？—–event将传输的数据进行封装，是flume传输数据的基本单位，如果是文本文件，通常是一行记录，event也是事务的基本单位。event从source，流向channel，再到sink，本身为一个字节数组，并可携带headers(头信息)信息。event代表着一个数据的最小完整单元，从外部数据源来，向外部的目的地去。一个完整的event包括：event headers、event body、event信息(即文本文件中的单行记录)。
三、Flume三大核心组件
Source： exec/Spooling Directory/Taildir Source/NetCat
采集数据，将数据从产生的源头采集过来，输出到channel中
Channel:  Memory/File
连接source和sink，类似于一个队列或者缓冲区
Sink: HDFS/Logger/Avro/Kafka
从Channel中获取数据，将数据写到目标目的地
把数据写入到下一个Agent的Source中去
agent里面包含3个核心的组件：source—->channel—–>sink,类似生产者、仓库、消费者的架构。agent本身是一个java进程，运行在日志收集节点—所谓日志收集节点就是服务器节点。

【来自@若泽大数据】


