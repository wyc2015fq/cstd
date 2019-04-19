# kafka入门详细讲解-aino007-51CTO博客
Kafka是由Apache软件基金会开发的一个开源流处理平台，由Scala和Java编写。Kafka是一种高吞吐量的分布式发布订阅消息系统，它可以处理消费者规模的网站中的所有动作流数据。 这种动作（网页浏览，搜索和其他用户的行动）是在现代网络上的许多社会功能的一个关键因素。 这些数据通常是由于吞吐量的要求而通过处理日志和日志聚合来解决。 对于像Hadoop的一样的日志数据和离线分析系统，但又要求实时处理的限制，这是一个可行的解决方案。Kafka的目的是通过Hadoop的并行加载机制来统一线上和离线的消息处理，也是为了通过集群来提供实时的消息。
推荐Kafka学习视频：[kafka入门详细讲解](https://pan.baidu.com/s/1ycyHrdW2DHdXvjetcGCLCg)
一、kafka 简介
`   kafka是一种高吞吐量的分布式发布订阅消息系统，它可以处理消费者规模的网站中的所有动作流数据。这种动作（网页浏览，搜索和其他用户的行动）是在现代网络上的许多社会功能的一个关键因素。这些数据通常是由于吞吐量的要求而通过处理日志和日志聚合来解决。`
1.1 kafka名词解释
producer：生产者。
consumer：消费者。
topic: 消息以topic为类别记录,Kafka将消息种子(Feed)分门别类,每一类的消息称之为一个主题(Topic)。
broker：以集群的方式运行,可以由一个或多个服务组成，每个服务叫做一个broker;消费者可以订阅一个或多个主题(topic),并从Broker拉数据,从而消费这些已发布的消息。
每个消息（也叫作record记录,也被称为消息）是由一个key，一个value和时间戳构成。
1.2 kafka有四个核心API介绍
应用程序使用producer API发布消息到1个或多个topic中。
应用程序使用consumer API来订阅一个或多个topic,并处理产生的消息。
应用程序使用streams API充当一个流处理器,从1个或多个topic消费输入流,并产生一个输出流到1个或多个topic,有效地将输入流转换到输出流。
connector API允许构建或运行可重复使用的生产者或消费者,将topic链接到现有的应用程序或数据系统。 
1.3 kafka基基原理
```
通常来讲，消息模型可以分为两种：队列和发布-订阅式。队列的处理方式是一组消费者从服务器读取消息，一条消息只有其中的一个消费者来处理。在发布-订阅模型中，消息被广播给所有的消费者，接收到消息的消费者都可以处理此消息。Kafka为这两种模型提供了单一的消费者抽象模型： 消费者组(consumer group)。消费者用一个消费者组名标记自己。
   一个发布在Topic上消息被分发给此消费者组中的一个消费者。假如所有的消费者都在一个组中，那么这就变成了queue模型。假如所有的消费者都在不同的组中，那么就完全变成了发布-订阅模型。更通用的， 我们可以创建一些消费者组作为逻辑上的订阅者。每个组包含数目不等的消费者，一个组内多个消费者可以用来扩展性能和容错。       
   并且，kafka能够保证生产者发送到一个特定的Topic的分区上，消息将会按照它们发送的顺序依次加入，也就是说，如果一个消息M1和M2使用相同的producer发送，M1先发送，那么M1将比M2的offset低，并且优先的出现在日志中。消费者收到的消息也是此顺序。如果一个Topic配置了复制因子（replication facto）为N,那么可以允许N-1服务器宕机而不丢失任何已经提交（committed）的消息。此特性说明kafka有比传统的消息系统更强的顺序保证。但是，相同的消费者组中不能有比分区更多的消费者，否则多出的消费者一直处于空等待，不会收到消息。
```
1.4 kafka应用场景
构建实时的流数据管道，可靠地获取系统和应用程序之间的数据。
构建实时流的应用程序，对数据流进行转换或反应。
1.5 主题和日志 (Topic和Log)
`  每一个分区(partition)都是一个顺序的、不可变的消息队列,并且可以持续的添加。分区中的消息都被分了一个序列号,称之为偏移量(offset),在每个分区中此偏移量都是唯一的。Kafka集群保持所有的消息,直到它们过期,无论消息是否被消费了。实际上消费者所持有的仅有的元数据就是这个偏移量，也就是消费者在这个log中的位置。 这个偏移量由消费者控制：正常情况当消费者消费消息的时候，偏移量也线性的的增加。但是实际偏移量由消费者控制，消费者可以将偏移量重置为更老的一个偏移量，重新读取消息。 可以看到这种设计对消费者来说操作自如， 一个消费者的操作不会影响其它消费者对此log的处理。 再说说分区。Kafka中采用分区的设计有几个目的。一是可以处理更多的消息，不受单台服务器的限制。Topic拥有多个分区意味着它可以不受限的处理更多的数据。第二，分区可以作为并行处理的单元，稍后会谈到这一点。`
1.6 分布式(Distribution)
`   Log的分区被分布到集群中的多个服务器上。每个服务器处理它分到的分区。根据配置每个分区还可以复制到其它服务器作为备份容错。 每个分区有一个leader，零或多个follower。Leader处理此分区的所有的读写请求，而follower被动的复制数据。如果leader宕机，其它的一个follower会被推举为新的leader。 一台服务器可能同时是一个分区的leader，另一个分区的follower。 这样可以平衡负载，避免所有的请求都只让一台或者某几台服务器处理。`
二、kafka 安装
2.1 jdk安装
#以oracle jdk为例，下载地址http://java.sun.com/javase/downloads/index.jsp
yum -y install jdk-8u141-linux-x64.rpm
2.2 安装zookeeper
wget [http://apache.forsale.plus/zookeeper/zookeeper-3.4.9/zookeeper-3.4.9.tar.gz](http://apache.forsale.plus/zookeeper/zookeeper-3.4.9/zookeeper-3.4.9.tar.gz)
tar zxf zookeeper-3.4.9.tar.gz
mv zookeeper-3.4.9 /data/zk
修改配置文件内容如下所示:
[root@localhost ~]# cat /data/zk/conf/zoo.cfg
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/data/zk/data/zookeeper
dataLogDir=/data/zk/data/logs
clientPort=2181
maxClientCnxns=60
autopurge.snapRetainCount=3
autopurge.purgeInterval=1
server.1=zk01:2888:3888
server.2=zk02:2888:3888
server.3=zk03:2888:3888
参数说明：
server.id=host:port:port:表示了不同的zookeeper服务器的自身标识，作为集群的一部分，每一台服务器应该知道其他服务器的信息。用户可以从“server.id=host:port:port” 中读取到相关信息。在服务器的data(dataDir参数所指定的目录)下创建一个文件名为myid的文件，这个
文件的内容只有一行，指定的是自身的id值。比如，服务器“1”应该在myid文件中写入“1”。这个id必须在集群环境中服务器标识中是唯一的，且大小在1～255之间。这一样配置中，zoo1代表第一台服务器的IP地址。第一个端口号（port）是从follower连接到leader机器的
端口，第二个端口是用来进行leader选举时所用的端口。所以，在集群配置过程中有三个非常重要的端口：clientPort：2181、port:2888、port:3888。
如果想更换日志输出位置，除了在zoo.cfg加入"dataLogDir=/data/zk/data/logs"外，还需要修改zkServer.sh文件，大概修改方式地方在125行左右，内容如下：
125 ZOO_LOG_DIR="$($GREP "^[[:space:]]*dataLogDir" "$ZOOCFG" | sed -e 's/.*=//')"
126 if [ ! -w "$ZOO_LOG_DIR" ] ; then
127 mkdir -p "$ZOO_LOG_DIR"
128 fi
在启动服务之前，还需要分别在zookeeper创建myid，方式如下：
echo 1 >  /data/zk/data/zookeeper/myid
启动服务
/data/zk/bin/zkServer.sh start
验证服务
### 查看相关端口号[root@localhost ~]# ss -lnpt|grep java
LISTEN     0      50          :::34442                   :::*                   users:(("java",pid=2984,fd=18))LISTEN     0      50       ::ffff:192.168.15.133:3888                    :::*                   users:(("java",pid=2984,fd=26))
LISTEN     0      50          :::2181                    :::*                   users:(("java",pid=2984,fd=25))###查看zookeeper服务状态
[root@localhost ~]# /data/zk/bin/zkServer.sh status
`ZooKeeper JMX enabled by default`
Using config: /data/zk/bin/../conf/zoo.cfgMode: follower
2.3 安装kafka
tar zxf kafka_2.11-0.11.0.0.tgz
mv kafka_2.11-0.11.0.0 /data/kafka
修改配置
[root@localhost ~]# grep -Ev "^#|^$" /data/kafka/config/server.properties
broker.id=0
delete.topic.enable=true
listeners=PLAINTEXT://192.168.15.131:9092
num.network.threads=3
num.io.threads=8
socket.send.buffer.bytes=102400
socket.receive.buffer.bytes=102400
socket.request.max.bytes=104857600
log.dirs=/data/kafka/data
num.partitions=1
num.recovery.threads.per.data.dir=1
offsets.topic.replication.factor=1
transaction.state.log.replication.factor=1
transaction.state.log.min.isr=1
log.flush.interval.messages=10000
log.flush.interval.ms=1000
log.retention.hours=168
log.retention.bytes=1073741824
log.segment.bytes=1073741824
log.retention.check.interval.ms=300000
zookeeper.connect=192.168.15.131:2181,192.168.15.132:2181,192.168.15.133:2181
zookeeper.connection.timeout.ms=6000
group.initial.rebalance.delay.ms=0
提示：其他主机将该机器的kafka目录拷贝即可，然后需要修改broker.id、listeners地址。
启动服务
/data/kafka/bin/kafka-server-start.sh /data/kafka/config/server.properties
验证服务
推荐Kafka学习视频：[kafka入门详细讲解](https://pan.baidu.com/s/1ycyHrdW2DHdXvjetcGCLCg)
