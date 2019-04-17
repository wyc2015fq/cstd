# Apache Kafka教程A系列：基本操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月03日 11:07:54[boonya](https://me.csdn.net/boonya)阅读数：170








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_basic_operations.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_basic_operations.htm)

首先，让我们开始实施单节点单代理配置，然后我们将设置迁移到单节点多代理配置。

希望你现在已经在你的机器上安装了Java，ZooKeeper和Kafka。 在转移到Kafka集群设置之前，首先您需要启动ZooKeeper，因为Kafka集群使用ZooKeeper。



#### 启动ZooKeeper

打开一个新终端并输入以下命令 -
`bin/zookeeper-server-start.sh config/zookeeper.properties`
要启动Kafka Broker，请键入以下命令 -
`bin/kafka-server-start.sh config/server.properties`
启动Kafka Broker后，在ZooKeeper终端上输入命令jps，您将看到以下响应 -

```
821 QuorumPeerMain
928 Kafka
931 Jps
```
现在你可以看到在终端上运行的两个守护进程，其中QuorumPeerMain是ZooKeeper守护进程，另一个守护进程是Kafka守护进程。


#### 单节点单代理配置
在这个配置中你有一个ZooKeeper和broker id实例。 以下是配置它的步骤 - 


创建一个Kafka主题 - Kafka提供了一个名为kafka-topics.sh的命令行实用程序来在服务器上创建主题。 打开新的终端并键入下面的示例。

语法

```
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 
--partitions 1 --topic topic-name
```

示例

```
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1   
--partitions 1 --topic Hello-Kafka
```

我们只用一个分区和一个复制因子创建了一个名为Hello-Kafka的主题。 以上创建的输出将与以下输出类似 -



输出 - 创建主题Hello-Kafka


创建主题后，您可以在Kafka代理终端窗口中获取通知，并在config / server.properties文件的“/ tmp / kafka-logs /”中指定所创建主题的日志。



#### 主题列表

要获得Kafka服务器中的主题列表，可以使用以下命令 -

语法
`bin/kafka-topics.sh --list --zookeeper localhost:2181`
输出
`Hello-Kafka`由于我们已经创建了一个主题，因此它只会列出Hello-Kafka。 假设，如果您创建多个主题，您将在输出中获得主题名称。


##### 启动生产者发送消息

语法
`bin/kafka-console-producer.sh --broker-list localhost:9092 --topic topic-name`从上面的语法来看，生产者命令行客户端需要两个主要参数 - 


代理名单 

 我们想要发送消息的代理名单。 在这种情况下，我们只有一个代理 。 Config / server.properties文件包含代理端口ID，因为我们知道我们的代理正在监听端口9092，因此您可以直接指定它。

主题名称 - 以下是主题名称的示例。

示例
`bin/kafka-console-producer.sh --broker-list localhost:9092 --topic Hello-Kafka`
生产者将等待stdin的输入并发布到Kafka集群。 默认情况下，每个新行都作为新消息发布，然后在config / producer.properties文件中指定默认生产者属性。 现在，您可以在终端中键入几行消息，如下所示。

输出

```
$ bin/kafka-console-producer.sh --broker-list localhost:9092 
--topic Hello-Kafka[2016-01-16 13:50:45,931] 
WARN property topic is not valid (kafka.utils.Verifia-bleProperties)
Hello
My first message
```
`My second message`
##### 启动消费者接收消息

与生产者类似，默认的消费者属性在config / consumer.properties文件中指定。 打开一个新终端并输入以下消息消息语法。

语法


```
bin/kafka-console-consumer.sh --zookeeper localhost:2181 —topic topic-name 
--from-beginning
```


示例

```
bin/kafka-console-consumer.sh --zookeeper localhost:2181 —topic Hello-Kafka 
--from-beginning
```

输出

```
Hello
My first message
My second message
```
最后，您可以从生产者的终端输入消息，并看到它们出现在消费者的终端中。 到目前为止，您对使用单个代理的单节点群集有非常好的理解。 现在让我们转到多个代理配置。


#### 单节点多代理配置
在转到多代理群集设置之前，首先启动您的ZooKeeper服务器。


##### 创建多个Kafka代理 

- 我们在con-fig / server.properties中已经有一个Kafka代理实例。 现在我们需要多个代理实例，因此将现有的server.prop- erties文件复制到两个新的配置文件中，并将其重命名为server-one.properties和server-two.properties。 然后编辑两个新文件并分配以下更改 - 

##### config/ server-one.properties

```
# The id of the broker. This must be set to a unique integer for each broker.
broker.id=1
# The port the socket server listens on
port=9093
# A comma seperated list of directories under which to store log files
log.dirs=/tmp/kafka-logs-1
```

##### config/server-two.properties

```
# The id of the broker. This must be set to a unique integer for each broker.
broker.id=2
# The port the socket server listens on
port=9094
# A comma seperated list of directories under which to store log files
log.dirs=/tmp/kafka-logs-2
```

##### 启动多个代理 

- 在三台服务器上完成所有更改后，打开三个新的终端，逐个启动每个代理。

```
Broker1
bin/kafka-server-start.sh config/server.properties
Broker2
bin/kafka-server-start.sh config/server-one.properties
Broker3
bin/kafka-server-start.sh config/server-two.properties
```
现在我们有三台不同的代理在机器上运行。 通过在ZooKeeper终端上输入jps来检查所有的守护进程，然后你会看到响应。


#### 创建一个主题
让我们为此主题指定复制因子值为三，因为我们有三个不同的代理正在运行。 如果您有两个代理商，则分配的副本值将为两个。

语法

```
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 3 
-partitions 1 --topic topic-name
```

示例

```
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 3 
-partitions 1 --topic Multibrokerapplication
```

输出
`created topic “Multibrokerapplication”`
描述命令用于检查哪个代理正在监听当前创建的主题，如下所示 -

```
bin/kafka-topics.sh --describe --zookeeper localhost:2181 
--topic Multibrokerappli-cation
```

输出

```
bin/kafka-topics.sh --describe --zookeeper localhost:2181 
--topic Multibrokerappli-cation

Topic:Multibrokerapplication    PartitionCount:1 
ReplicationFactor:3 Configs:
   
Topic:Multibrokerapplication Partition:0 Leader:0 
Replicas:0,2,1 Isr:0,2,1
```
从上面的输出中，我们可以得出结论，第一行给出了所有分区的摘要，显示了我们已经选择的主题名称，分区计数和复制因子。 在第二行中，每个节点将成为分区随机选择部分的领导者。

在我们的案例中，我们看到我们的第一个代理（与broker.id 0）是领导者。 然后副本：0,2,1意味着所有经纪人最终复制主题Isr是一组同步副本。 那么，这是复制品的一部分，目前活着并被领导者追上。


##### 启动生产者发送消息

此过程与单个代理程序设置中的相同。

示例

```
bin/kafka-console-consumer.sh --zookeeper localhost:2181 
—topic Multibrokerapplica-tion --from-beginning
```

输出

```
bin/kafka-console-consumer.sh --zookeeper localhost:2181 
—topic Multibrokerapplica-tion —from-beginning
This is single node-multi broker demo
This is the second message
```

##### 启动消费者接收消息

此过程与单个代理设置中显示的相同。

语法

```
bin/kafka-console-consumer.sh --zookeeper localhost:2181 
—topic Multibrokerapplica-tion --from-beginning
```

输出

```
bin/kafka-console-consumer.sh --zookeeper localhost:2181 
—topic Multibrokerapplica-tion —from-beginning
This is single node-multi broker demo
This is the second message
```

#### 基本主题操作
在本章中，我们将讨论各种基本的主题操作。


##### 修改主题

正如您已经了解如何在Kafka集群中创建主题。 现在让我们使用以下命令修改创建的主题

语法

```
bin/kafka-topics.sh —zookeeper localhost:2181 --alter --topic topic_name 
--parti-tions count
```

示例

```
We have already created a topic “Hello-Kafka” with single partition count and one replica factor. 
Now using “alter” command we have changed the partition count.
bin/kafka-topics.sh --zookeeper localhost:2181 
--alter --topic Hello-kafka --parti-tions 2
```

输出

```
WARNING: If partitions are increased for a topic that has a key, 
the partition logic or ordering of the messages will be affected
Adding partitions succeeded!
```

##### 删除主题

要删除主题，您可以使用以下语法。


语法
`bin/kafka-topics.sh --zookeeper localhost:2181 --delete --topic topic_name`
示例
`bin/kafka-topics.sh --zookeeper localhost:2181 --delete --topic Hello-kafka`
输出
`> Topic Hello-kafka marked for deletion`注意 - 如果delete.topic.enable未设置为true，则这不会产生影响。



