
# 分布式消息队列Kafka集群安装 - 无界 - CSDN博客

置顶2014年02月17日 09:26:54[21aspnet](https://me.csdn.net/21aspnet)阅读数：23410


[kafka](http://incubator.apache.org/kafka/)是LinkedIn开发并开源的一个分布式MQ系统，现在是Apache的一个孵化项目。在它的主页描述kafka为一个高吞吐量的分布式（能将消息分散到不同的节点上）MQ。在这片[博文](http://blog.linkedin.com/2011/01/11/open-source-linkedin-kafka/)中，作者简单提到了开发kafka而不选择已有MQ系统的原因。两个原因：性能和扩展性。Kafka仅仅由7000行Scala编写，据了解，**Kafka每秒可以生产约25万消息（50 MB），每秒处理55万消息（110 MB）。**

Kafka版本：0.8.0
约定：安装3台虚拟机
官网：[http://kafka.apache.org/](http://kafka.apache.org/)
官方文档：[http://kafka.apache.org/documentation.html\#quickstart](http://kafka.apache.org/documentation.html#quickstart)

**下载解压**
\#wget http://mirrors.hust.edu.cn/apache/kafka/0.8.0/kafka-0.8.0-src.tgz
\#tar xzf kafka-0.8.0-src.tgz
\#cd kafka-0.8.0-src
Kafka是用Scala写的，SBT是Simple Build Tool的简称，如果读者使用过Maven，那么可以简单将SBT看做是Scala世界的Maven，虽然二者各有优劣，但完成的工作基本是类似的。
\#\# Building it \#\#
\#./sbt update
\#./sbt package
\#./sbt assembly-package-dependency
以上每一步完成就会提醒[Success]
例如：[success] Total time: 21 s, completed 2014-2-11 10:29:55
![](https://img-blog.csdn.net/20140211103712781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

集群环境需要修改配置文件
\#vim config/server.properties
brokerid：这个每个server（broker）必须唯一，写数字
hostname：这个也是唯一的，写服务器IP即可

```python
############################# Server Basics #############################
# The id of the broker. This must be set to a unique integer for each broker.
broker.id=3
############################# Socket Server Settings #############################
# The port the socket server listens on
port=9092
# Hostname the broker will bind to and advertise to producers and consumers.
# If not set, the server will bind to all interfaces and advertise the value returned from
# from java.net.InetAddress.getCanonicalHostName().
#host.name=localhost
host.name=192.168.2.111
```
还有就是zookeeper.connect也要配置

```python
zookeeper.connect=192.168.19.218:2181,192.168.19.217:2181,192.168.19.214:2181
```
关于zookeeper的安装可以参考此文：[ZooKeeper集群环境安装与配置](http://blog.csdn.net/unix21/article/details/18990123)
其他默认配置即可。

**启动Kafka服务**
\#/usr/kafka-0.8.0-src/bin/kafka-server-start.sh /usr/kafka-0.8.0-src/config/server.properties
![](https://img-blog.csdn.net/20140211123300078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**创建Topic**
\#/usr/kafka-0.8.0-src/bin/kafka-create-topic.sh --zookeeper localhost:2181 --partition 1 --topic test
**查看Topic**
\#/usr/kafka-0.8.0-src/bin/kafka-list-topic.sh --zookeeper localhost:2181
输出：
topic: test     partition: 0    leader: 1       replicas: 1     isr: 1
![](https://img-blog.csdn.net/20140211123546265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
说明：
partiton： partion id，由于此处只有一个partition，因此partition id 为0
leader：当前负责读写的lead broker id
relicas：当前partition的所有replication broker  list
isr：relicas的子集，只包含出于活动状态的broker

**producer发送消息**
\#/usr/kafka-0.8.0-src/bin/kafka-console-producer.sh --broker-list localhost:9092 --topic test
This is a message
This is another message
**consumer接收消息**
\#/usr/kafka-0.8.0-src/bin/kafka-console-consumer.sh --zookeeper localhost:2181 --topic test --from-beginning

注意，如果上述命令不能发送接收消息说明没有配置host，可以直接用ip
**producer发送消息**
\#/usr/kafka-0.8.0-src/bin/kafka-console-producer.sh --broker-list 192.168.19.218:9092 --topic test
![](https://img-blog.csdn.net/20140211142709515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**consumer接收消息**
\#/usr/kafka-0.8.0-src/bin/kafka-console-consumer.sh --zookeeper 192.168.19.218:2181 --topic test --from-beginning
![](https://img-blog.csdn.net/20140211142721796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果要最新的数据，可以不带--from-beginning参数即可。
\#/usr/kafka-0.8.0-src/bin/kafka-console-consumer.sh --zookeeper 192.168.19.218:2181 --topic test

在kafka的核心思路中，不需要在内存里缓存数据，因为操作系统的文件缓存已经足够完善和强大，只要不做随机写，顺序读写的性能是非常高效的。kafka的数据只会顺序append，数据的删除策略是累积到一定程度或者超过一定时间再删除。Kafka另一个独特的地方是将消费者信息保存在客户端而不是MQ服务器，这样服务器就不用记录消息的投递过程，每个客户端都自己知道自己下一次应该从什么地方什么位置读取消息，消息的投递过程也是采用客户端主动pull的模型，这样大大减轻了服务器的负担。Kafka还强调减少数据的序列化和拷贝开销，它会将一些消息组织成Message Set做批量存储和发送，并且客户端在pull数据的时候，尽量以zero-copy的方式传输，利用sendfile(对应java里的FileChannel.transferTo/transferFrom)这样的高级IO函数来减少拷贝开销。可见，kafka是一个精心设计，特定于某些应用的MQ系统，这种偏向特定领域的MQ系统我估计会越来越多，垂直化的产品策略值的考虑。
只要磁盘没有限制并且不出现损失，kafka可以存储相当长时间的消息（一周）。

原文出自：本人另一个博客[http://blog.csdn.net/unix21/](http://blog.csdn.net/unix21/)
﻿﻿
﻿﻿

