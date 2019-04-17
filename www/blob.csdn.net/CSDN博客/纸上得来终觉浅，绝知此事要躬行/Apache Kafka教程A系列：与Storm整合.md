# Apache Kafka教程A系列：与Storm整合 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月03日 17:22:14[boonya](https://me.csdn.net/boonya)阅读数：97








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_integration_storm.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_integration_storm.htm)

在本章中，我们将学习如何将Kafka与Apache Storm集成。

#### 关于Storm
Storm最初是由Nathan Marz和BackType团队创建的。 在很短的时间内，Apache Storm成为分布式实时处理系统的标准，允许您处理大量数据。 Storm速度非常快，每个节点每秒处理超过一百万个元组的基准时钟。 Apache Storm持续运行，从配置的源（Spouts）中消耗数据并将数据传递到处理管道（Bolts）。 组合式，喷嘴和螺栓构成一个拓扑。


#### 与Storm整合

Kafka和Storm自然而然地相互补充，他们强大的合作能够实现快速移动大数据的实时流式分析。 Kafka和Storm的整合是为了让开发人员更容易地从Storm拓扑中获取和发布数据流。



##### 概念流程
喷口(spout)是溪流(stream)的来源。例如，喷口可以读取Kafka主题中的元组并将它们作为流发送。螺栓消耗输入流，处理并可能发射新的流。 Bolts可以做任何事情，从运行功能，过滤元组，流聚合，流式连接，与数据库交谈等等。 Storm拓扑中的每个节点并行执行。一个拓扑无限期地运行，直到你终止它。 Storm会自动重新分配任何失败的任务。此外，即使机器停机并且信息丢失，Storm也可以保证不会丢失数据。

让我们详细介绍Kafka-Storm集成API。有三个主要类将Kafka和Storm结合在一起。他们如下 - 


##### BrokerHosts - ZkHosts＆StaticHosts
BrokerHosts是一个接口，ZkHosts和StaticHosts是它的两个主要实现。 ZkHosts用于通过在ZooKeeper中维护详细信息来动态跟踪卡夫卡代理，而StaticHosts用于手动/静态设置卡夫卡代理及其详细信息。 ZkHosts是访问卡夫卡代理的简单而快捷的方式。

ZkHosts的签名如下 -


public ZkHosts(String brokerZkStr, String brokerZkPath)
public ZkHosts(String brokerZkStr)其中brokerZkStr是ZooKeeper主机，brokerZkPath是维护Kafka代理细节的ZooKeeper路径。


##### KafkaConfig API

该API用于定义Kafka集群的配置设置。 Kafka Con-fig的签名定义如下


public KafkaConfig(BrokerHosts hosts, string topic)- hosts - BrokerHosts可以是ZkHosts / StaticHosts。
- topic - 主题名称。



##### SpoutConfig API

Spoutconfig是KafkaConfig的扩展，支持额外的ZooKeeper信息。


public SpoutConfig(BrokerHosts hosts, string topic, string zkRoot, string id)- hosts - BrokerHosts可以是BrokerHosts接口的任何实现
- topic - 主题名称。
- zkRoot - ZooKeeper根路径。
- id - 喷口存储在Zookeeper中消耗的偏移量的状态。 该ID应该唯一标识你的喷口。



##### SchemeAsMultiScheme

SchemeAsMultiScheme是一个接口，它规定了从Kafka消耗的ByteBuffer如何转换为Storm元组。 它来自MultiScheme并接受Scheme类的实现。 Scheme类有很多实现，一个这样的实现是StringScheme，它将字节解析为一个简单的字符串。 它还控制输出字段的命名。 签名定义如下。


public SchemeAsMultiScheme(Scheme scheme)

- Scheme - 从kafka消耗的字节缓冲区。



##### KafkaSpout API
KafkaSpout是我们的喷口实现，它将与Storm整合。 它从kafka主题获取消息并将其作为元组发送到Storm生态系统中。 KafkaSpout从SpoutConfig获取配置细节。


以下是创建一个简单的卡夫卡喷嘴的示例代码。


// ZooKeeper connection stringBrokerHosts hosts =newZkHosts(zkConnString);//Creating **SpoutConfig** ObjectSpoutConfig spoutConfig =newSpoutConfig(hosts, 
   topicName,"/"+ topicName UUID.randomUUID().toString());//convert the ByteBuffer to String.
spoutConfig.scheme =newSchemeAsMultiScheme(newStringScheme());//Assign SpoutConfig to KafkaSpout.KafkaSpout kafkaSpout =newKafkaSpout(spoutConfig);
#### Bolt创建

Bolt是一个将元组作为输入，处理元组并生成新的元组作为输出的组件。 Bolts将实施IRichBolt界面。 在这个程序中，使用两个螺栓类WordSplitter-Bolt和WordCounterBolt来执行操作。

IRichBolt接口有以下方法 - 

- prepare - 为螺栓提供执行的环境。 执行者将运行此方法来初始化喷口。
- execute - 处理输入的单个元组。
- cleanup - 当螺栓即将关闭时调用。
- declareOutputFields - 声明元组的输出模式。


让我们创建SplitBolt.java，它实现了将句子分成单词和CountBolt.java的逻辑，它实现了逻辑来分离唯一的单词并计算它的出现次数。

##### SplitBolt.java


import java.util.Map;import backtype.storm.tuple.Tuple;import backtype.storm.tuple.Fields;import backtype.storm.tuple.Values;import backtype.storm.task.OutputCollector;import backtype.storm.topology.OutputFieldsDeclarer;import backtype.storm.topology.IRichBolt;import backtype.storm.task.TopologyContext;publicclassSplitBoltimplementsIRichBolt{privateOutputCollector collector;@Overridepublicvoid prepare(Map stormConf,TopologyContext context,OutputCollector collector){this.collector = collector;}@Overridepublicvoid execute(Tuple input){String sentence = input.getString(0);String[] words = sentence.split(" ");for(String word: words){
         word = word.trim();if(!word.isEmpty()){
            word = word.toLowerCase();
            collector.emit(newValues(word));}}

      collector.ack(input);}@Overridepublicvoid declareOutputFields(OutputFieldsDeclarer declarer){
      declarer.declare(newFields("word"));}@Overridepublicvoid cleanup(){}@OverridepublicMap<String,Object> getComponentConfiguration(){returnnull;}}
##### CountBolt.java
import java.util.Map;import java.util.HashMap;import backtype.storm.tuple.Tuple;import backtype.storm.task.OutputCollector;import backtype.storm.topology.OutputFieldsDeclarer;import backtype.storm.topology.IRichBolt;import backtype.storm.task.TopologyContext;publicclassCountBoltimplementsIRichBolt{Map<String,Integer> counters;privateOutputCollector collector;@Overridepublicvoid prepare(Map stormConf,TopologyContext context,OutputCollector collector){this.counters =newHashMap<String,Integer>();this.collector = collector;}@Overridepublicvoid execute(Tuple input){String str = input.getString(0);if(!counters.containsKey(str)){
         counters.put(str,1);}else{Integer c = counters.get(str)+1;
         counters.put(str, c);}
   
      collector.ack(input);}@Overridepublicvoid cleanup(){for(Map.Entry<String,Integer> entry:counters.entrySet()){System.out.println(entry.getKey()+" : "+ entry.getValue());}}@Overridepublicvoid declareOutputFields(OutputFieldsDeclarer declarer){}@OverridepublicMap<String,Object> getComponentConfiguration(){returnnull;}}


#### 提交到拓扑
Storm拓扑基本上是一个Thrift结构。 TopologyBuilder类提供了简单而简单的方法来创建复杂的拓扑。 TopologyBuilder类具有设置喷口（setSpout）和设置螺栓（setBolt）的方法。 最后，TopologyBuilder创建了拓扑学来创建拓朴学。 shuffleGrouping和fieldsGrouping方法有助于设置喷嘴和螺栓的流分组。

本地群集 - 为了开发目的，我们可以使用LocalCluster对象创建本地群集，然后使用LocalCluster类的submitTopology方法提交拓扑。


##### KafkaStormSample.java


import backtype.storm.Config;import backtype.storm.LocalCluster;import backtype.storm.topology.TopologyBuilder;import java.util.ArrayList;import java.util.List;import java.util.UUID;import backtype.storm.spout.SchemeAsMultiScheme;import storm.kafka.trident.GlobalPartitionInformation;import storm.kafka.ZkHosts;import storm.kafka.Broker;import storm.kafka.StaticHosts;import storm.kafka.BrokerHosts;import storm.kafka.SpoutConfig;import storm.kafka.KafkaConfig;import storm.kafka.KafkaSpout;import storm.kafka.StringScheme;publicclassKafkaStormSample{publicstaticvoid main(String[] args)throwsException{Config config =newConfig();
      config.setDebug(true);
      config.put(Config.TOPOLOGY_MAX_SPOUT_PENDING,1);String zkConnString ="localhost:2181";String topic ="my-first-topic";BrokerHosts hosts =newZkHosts(zkConnString);SpoutConfig kafkaSpoutConfig =newSpoutConfig(hosts, topic,"/"+ topic,    
         UUID.randomUUID().toString());
      kafkaSpoutConfig.bufferSizeBytes =1024*1024*4;
      kafkaSpoutConfig.fetchSizeBytes =1024*1024*4;
      kafkaSpoutConfig.forceFromStart =true;
      kafkaSpoutConfig.scheme =newSchemeAsMultiScheme(newStringScheme());TopologyBuilder builder =newTopologyBuilder();
      builder.setSpout("kafka-spout",newKafkaSpout(kafkaSpoutCon-fig));
      builder.setBolt("word-spitter",newSplitBolt()).shuffleGroup-ing("kafka-spout");
      builder.setBolt("word-counter",newCountBolt()).shuffleGroup-ing("word-spitter");LocalCluster cluster =newLocalCluster();
      cluster.submitTopology("KafkaStormSample", config, builder.create-Topology());Thread.sleep(10000);
      
      cluster.shutdown();}}
在移动编译之前，Kakfa-Storm集成需要馆长ZooKeeper客户端java库。 策展人版本2.9.1支持Apache Storm 0.9.5版本（我们在本教程中使用）。 下载下面指定的jar文件并将其放在java类路径中。


- curator-client-2.9.1.jar
- curator-framework-2.9.1.jar

在包含依赖文件后，使用以下命令编译程序


javac -cp "/path/to/Kafka/apache-storm-0.9.5/lib/*" *.java
**执行**
启动Kafka Producer CLI（在上一章中解释），创建一个名为my-first-topic的新主题并提供一些示例消息，如下所示 -


hello
kafka
storm
spark
test message
another test message
现在使用以下命令执行应用程序 -


java -cp “/path/to/Kafka/apache-storm-0.9.5/lib/*”:. KafkaStormSample
此应用程序的输出示例如下所示 -


storm : 1
test : 2
spark : 1
another : 1
kafka : 1
hello : 1
message : 2

import backtype.storm.Config;import backtype.storm.LocalCluster;import backtype.storm.topology.TopologyBuilder;import java.util.ArrayList;import java.util.List;import java.util.UUID;import backtype.storm.spout.SchemeAsMultiScheme;import storm.kafka.trident.GlobalPartitionInformation;import storm.kafka.ZkHosts;import storm.kafka.Broker;import storm.kafka.StaticHosts;import storm.kafka.BrokerHosts;import storm.kafka.SpoutConfig;import storm.kafka.KafkaConfig;import storm.kafka.KafkaSpout;import storm.kafka.StringScheme;publicclassKafkaStormSample{publicstaticvoid main(String[] args)throwsException{Config config =newConfig();      config.setDebug(true);      config.put(Config.TOPOLOGY_MAX_SPOUT_PENDING,1);String zkConnString ="localhost:2181";String topic ="my-first-topic";BrokerHosts hosts =newZkHosts(zkConnString);SpoutConfig kafkaSpoutConfig =newSpoutConfig(hosts, topic,"/"+ topic,             UUID.randomUUID().toString());      kafkaSpoutConfig.bufferSizeBytes =1024*1024*4;      kafkaSpoutConfig.fetchSizeBytes =1024*1024*4;      kafkaSpoutConfig.forceFromStart =true;      kafkaSpoutConfig.scheme =newSchemeAsMultiScheme(newStringScheme());TopologyBuilder builder =newTopologyBuilder();      builder.setSpout("kafka-spout",newKafkaSpout(kafkaSpoutCon-fig));      builder.setBolt("word-spitter",newSplitBolt()).shuffleGroup-ing("kafka-spout");      builder.setBolt("word-counter",newCountBolt()).shuffleGroup-ing("word-spitter");LocalCluster cluster =newLocalCluster();      cluster.submitTopology("KafkaStormSample", config, builder.create-Topology());Thread.sleep(10000);            cluster.shutdown();}
}




