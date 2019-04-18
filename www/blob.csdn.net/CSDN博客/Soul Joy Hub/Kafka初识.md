# Kafka初识 - Soul Joy Hub - CSDN博客

2016年07月24日 18:10:47[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：502


# 问题一

> 
写出增加Kafka的Partition命令

`bin/kafka-add-partitions.sh --topic test --partition 2  --zookeeper  192.168.197.170:2181,192.168.197.171:2181`
# 问题二

> 
列出配置Kafka删除日志的配置参数

|参数|说明(解释)|
|----|----|
|log.roll.hours =24*7|这个参数会在日志segment没有达到log.segment.bytes设置的大小，也会强制新建一个segment会被 topic创建时的指定参数覆盖|
|log.cleanup.policy = delete|日志清理策略选择有：delete和compact主要针对过期数据的处理，或是日志文件达到限制的额度，会被 topic创建时的指定参数覆盖|
|log.cleaner.io.buffer.size=512*1024|日志清理时候用到的IO块大小一般不需要修改|
|log.cleaner.io.buffer.load.factor =0.9|日志清理中hash表的扩大因子一般不需要修改|
|log.cleaner.backoff.ms=15000|检查是否处罚日志清理的间隔|
|log.cleaner.min.cleanable.ratio=0.5|日志清理的频率控制，越大意味着更高效的清理，同时会存在一些空间上的浪费，会被topic创建时的指定参数覆盖|

# 问题三

> 
Kafka中有一个topic： tfc_pass，实时接收过车记录，字段为：号牌（hphm）、过车时间（gcsj）、速度（sd）；inceptor中有一张黑名单的表，包括字段号牌号码(hphm);请编写一个StreamJob，实时将属于黑名单表的过车记录写到结果表result中。

## Producer

```java
public classClusterProducer extends Thread {
  private static final Log log =LogFactory.getLog(ClusterProducer.class);

  public void sendData() {
    Random rnd = new Random();
    Properties props =PropertiesParser.getProperties(PropertiesSettings.PRODUCER_FILE_NAME);
    if (props == null) {
      log.error("can't loadspecified file " + PropertiesSettings.PRODUCER_FILE_NAME);
       return;
    }
    //set the producer configurationproperties
    ProducerConfig config = newProducerConfig(props);
    Producer<String, String> producer= new Producer<String, String>(config);

    //Send the data
    KeyedMessage<String, String> data;
    int count = 0;
    String url = "jdbc:inceptor://127.0.0.1:1433;DatabaseName=tmp";  
    String username = "sa";  
    String password = "";  

    Connection conn = DriverManager.getConnection(url, username, password);  
    Statement stmt = conn.createStatement();  
    ResultSet rs = stmt.executeQuery("select hphm from tfc_pass");  
    while (rs.next()) {  
    data = new KeyedMessage<String,String>("tfc_pass", count+'', rs.getString("hphm") );
    producer.send(data);
    count++;
    } 

    stmt.close();  
    conn.close();  
    producer.close();
  }

  public void run() {
    sendData();
  }

  public static void main(String[] args) {
    new ClusterProducer().sendData();
  }
}
```

## Consumer

```java
public class Consumerextends Thread {
  private static final Log log =LogFactory.getLog(Consumer.class);
  private final ConsumerConnector consumer;
  private final String topic;

  public Consumer(String topic) {
    consumer =kafka.consumer.Consumer.createJavaConsumerConnector(
        createConsumerConfig());
    this.topic = topic;
  }

  private static ConsumerConfigcreateConsumerConfig() {
    Properties props = new Properties();
     props.put("zookeeper.connect", KafkaProperties.zkConnect);
    props.put("group.id",KafkaProperties.groupId);
     props.put("zookeeper.session.timeout.ms", "400");
     props.put("zookeeper.sync.time.ms", "200");
     props.put("auto.commit.interval.ms", "1000");

    return new ConsumerConfig(props);

  }

  public void run() {
    String url = "jdbc:inceptor://127.0.0.1:1433;DatabaseName=tmp";  
    String username = "sa";  
    String password = ""; 
    Connection conn = DriverManager.getConnection(url, username, password);  
    Statement stmt = conn.createStatement();  

    Map<String, Integer>topicCountMap = new HashMap<String, Integer>();
    topicCountMap.put(topic, newInteger(1));
    Map<String,List<KafkaStream<byte[], byte[]>>> consumerMap =consumer.createMessageStreams(topicCountMap);
    KafkaStream<byte[], byte[]>stream = consumerMap.get(topic).get(0);
    ConsumerIterator<byte[], byte[]>it = stream.iterator();
    while (it.hasNext()) {
      String val = new String(it.next().message());
      stmt.executeUpdate("insert into blacklist values('"+val+"')") ;
    }

    stmt.close();  
    conn.close();  
  }

  public static void main(String[] args) {
    Consumer client = new Consumer("cluster_statistics_topic");
    client.run();
  }
}
```

