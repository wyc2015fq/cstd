# 分布式发布订阅消息系统Kafka单实例测试 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月07日 11:59:23[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2230








                
1、Kafka简介

Kafka是一种高吞吐量的分布式发布订阅消息系统，它可以处理消费者规模的网站中的所有动作流数据。

kafka对消息保存时根据Topic进行归类，发送消息者成为Producer,消息接受者成为Consumer,此外kafka集群有多个kafka实例组成，每个实例(server)成为broker。

无论是kafka集群，还是producer和consumer都依赖于zookeeper来保证系统可用性集群保存一些meta信息。

![](https://img-blog.csdn.net/20170307115914028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2、Kafka安装和启动



1)下载并解压kafka_2.10-0.10.1.1.tgz

  #tar -xzvf kafka_2.10-0.10.1.1.tgz



2)下载并解压zookeeper-3.4.6.tar.gz

  #tar -xzvf zookeeper-3.4.6.tar.gz



3)启动单实例Zookkeeper服务

  #cd zookeeper-3.4.6

  conf文件夹中新建名zoo.cfg文件，可复制zoo_sample.cfg文件进行相应修改。

  启动服务：

  #bin/zkServer.sh &


4)启动Kafka服务

   #cd kafka_2.10

   配置config/server.properties相应信息，如zookeeper.connect；

   启动服务：

   #bin/kafka-server-start.sh config/server.properties &



3、测试：

   1)创建topic

   #bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic test0307

   通过list命令查看创建的topic:

   #bin/kafka-topics.sh --list --zookeeper localhost:2181

   2)生产消息

    Kafka 使用一个简单的命令行producer，从文件中或者从标准输入中读取消息并发送到服务端。

    默认的每条命令将发送一条消息。运行producer并在控制台中输一些消息，这些消息将被发送到服务端：

    #bin/kafka-console-producer.sh --broker-list localhost:9092 --topic test0307

     I am testing kafka for inputing message into HDFS.

     不发送就ctrl+c退出。

   3)消费消息

     Kafka命令行consumer可以读取消息并输出到标准输出：

     #bin/kafka-console-consumer.sh --zookeeper localhost:2181 --topic test0307 --from-beginning



4、后续待研究： 

1）这是单实例测试，可以在一台机子上配置多个broker，也可以建立kafka集群。

2）kafka的producer可以来自flume的sink，其consumer可以输出到hdfs中。

参考：http://www.cnblogs.com/cssdongl/p/6077311.html
            


