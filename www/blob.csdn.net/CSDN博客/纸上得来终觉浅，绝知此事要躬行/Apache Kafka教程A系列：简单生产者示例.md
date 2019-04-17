# Apache Kafka教程A系列：简单生产者示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月03日 16:43:37[boonya](https://me.csdn.net/boonya)阅读数：127








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_simple_producer_example.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_simple_producer_example.htm)

让我们创建一个使用Java客户端发布和使用消息的应用程序。 卡夫卡生产者客户端由以下API组成。



#### KafkaProducer API

让我们了解本节中最重要的一组Kafka生产者API。 KafkaProducer API的核心部分是KafkaProducer类。 KafkaProducer类提供了一个选项，用于将Kafka代理的构造函数与以下方法连接起来。


- KafkaProducer类提供send方法来异步发送消息到主题。 send()的签名如下：


producer.send(new ProducerRecord<byte[],byte[]>(topic, 
partition, key1, value1) , callback);- ProducerRecord - 生产者管理等待发送的记录缓冲区。
- Callback回调 - 服务器确认记录时执行的用户提供的回调函数（null表示无回叫）。
- KafkaProducer类提供了一种flush方法来确保所有先前发送的消息已经实际完成。 冲洗方法的语法如下 -


public void flush()

- KafkaProducer类提供了partitionFor方法，该方法有助于获取给定主题的分区元数据。 这可以用于自定义分区。 这种方法的签名如下 -


public Map metrics()
它返回生产者维护的内部度量图。


- public void close（） - KafkaProducer类提供close方法块，直到完成所有先前发送的请求。



##### 生产者API
Producer API的核心部分是Producer类。 Producer类提供了一个选项，通过以下方法在其构造函数中连接Kafka代理。

##### 生产者类

生产者类提供send方法，使用以下签名将消息发送到单个或多个主题。


publicvoid send(KeyedMessaget<k,v> message)- sends the data to a single topic,par-titioned by key using either sync or async producer.publicvoid send(List<KeyedMessage<k,v>>messages)- sends data to multiple topics.Properties prop =newProperties();
prop.put(producer.type,”async”)ProducerConfig config =newProducerConfig(prop);有两种类型的生产者 - **同步**和**异步**。

同样的API配置也适用于Sync生产者。 它们之间的区别是同步生产者直接发送消息，但在后台发送消息。 当您想要更高的吞吐量时，首选异步生产者。 在0.8之前的版本中，异步生产者没有回调send（）来注册错误处理程序。 这仅在当前版本0.9中可用。


**public void close（）**

生产者类提供关闭方法来关闭所有卡夫卡代理的生产者池连接。



##### 配置设置

生产者API的主要配置设置在下表中列出以便更好地理解 -


|序号|配置设置和描述|
|----|----|
|1|client.id确定生产者申请。|
|2|producer.type无论是同步还是异步。|
|3|acksacs config控制生产者请求下的标准被视为完成。|
|4|retries如果生产者请求失败，然后自动重试具有特定的值。|
|5|bootstrap.servers引导代理名单。|
|6|linger.ms如果你想减少请求的数量，你可以将linger.ms设置为大于某个值的值。|
|7|key.serializer序列化器接口的键。|
|8|value.serializer序列化器接口的值。|
|9|batch.size缓冲大小。|
|10|buffer.memory控制生产者可用于缓冲的总内存量。|
ProducerRecord API
ProducerRecord是发送给Kafka cluster.ProducerRecord类的键/值对，用于使用以下签名创建包含分区，键和值对的记录。


public ProducerRecord (string topic, int partition, k key, v value)- topic - 将追加记录的用户定义的主题名称。
- partition - 分区计数
- key - 将包含在记录中的密钥。
- value - 记录内容


public ProducerRecord (string topic, k key, v value)ProducerRecord类构造函数用于创建一个记录，其中包含键值对并且没有分区。
- topic - 创建一个分配记录的主题。
- key- 记录的钥匙。
- value - 记录内容。


public ProducerRecord (string topic, v value)ProducerRecord类创建没有分区和键的记录。
- topic - 创建主题。
- value - 记录内容。

下表中列出了ProducerRecord类的方法 -


|序号|类方法和描述|
|----|----|
|1|public string topic()主题将附加到记录。|
|2|public K key()将包含在记录中的key。 如果没有这样的key，null将在这里重新转向。|
|3|public V value()记录内容。|
|4|partition()记录的分区计数。|
SimpleProducer应用程序
在创建应用程序之前，首先启动ZooKeeper和Kafka代理，然后使用create topic命令在Kafka代理中创建自己的主题。 之后，创建一个名为Sim-pleProducer.java的Java类并输入以下代码。



//import util.properties packagesimport java.util.Properties;//import simple producer packagesimport org.apache.kafka.clients.producer.Producer;//import KafkaProducer packagesimport org.apache.kafka.clients.producer.KafkaProducer;//import ProducerRecord packagesimport org.apache.kafka.clients.producer.ProducerRecord;//Create java class named “SimpleProducer”publicclassSimpleProducer{publicstaticvoid main(String[] args)throwsException{// Check arguments length valueif(args.length ==0){System.out.println("Enter topic name”);
         return;
      }
      
      //Assign topicName to string variable
      String topicName = args[0].toString();
      
      // create instance for properties to access producer configs   
      Properties props = new Properties();
      
      //Assign localhost id
      props.put("bootstrap.servers", “localhost:9092");//Set acknowledgements for producer requests.      
      props.put("acks",“all");
      
      //If the request fails, the producer can automatically retry,
      props.put("retries", 0);
      
      //Specify buffer size in config
      props.put("batch.size", 16384);
      
      //Reduce the no of requests less than 0   
      props.put("linger.ms", 1);
      
      //The buffer.memory controls the total amount of memory available to the producer for buffering.   
      props.put("buffer.memory", 33554432);
      
      props.put("key.serializer", 
         "org.apache.kafka.common.serializa-tion.StringSerializer");
         
      props.put("value.serializer", 
         "org.apache.kafka.common.serializa-tion.StringSerializer");
      
      Producer<String, String> producer = new KafkaProducer
         <String, String>(props);
            
      for(int i = 0; i < 10; i++)
         producer.send(new ProducerRecord<String, String>(topicName, 
            Integer.toString(i), Integer.toString(i)));
               System.out.println(“Message sent successfully”);
               producer.close();
   }
}
编译 - 可以使用以下命令编译应用程序。


javac -cp “/path/to/kafka/kafka_2.11-0.9.0.0/lib/*” *.java
执行 - 可以使用以下命令执行应用程序。



java -cp “/path/to/kafka/kafka_2.11-0.9.0.0/lib/*”:. SimpleProducer <topic-name>
输出


Message sent successfully
To check the above output open new terminal and type Consumer CLI command to receive messages.
>> bin/kafka-console-consumer.sh --zookeeper localhost:2181 —topic <topic-name> —from-beginning
1
2
3
4
5
6
7
8
9
10


#### 简单消费者示例

截至目前，我们已经创建了一个生产者向Kafka集群发送消息。 现在让我们创建一个使用者来使用来自Kafka集群的消息。 KafkaConsumer API用于消费来自Kafka集群的消息。 下面定义了KafkaConsumer类的构造函数。



public KafkaConsumer(java.util.Map<java.lang.String,java.lang.Object> configs)

**configs **- 返回消费者配置的Map。

KafkaConsumer类具有下表中列出的以下重要方法。



|序号|方法和描述|
|----|----|
|1|public java.util.Set<TopicPar-tition> assignment()获取消费者当前分配的一组分区。|
|2|public string subscription()订阅给定的主题列表以获取动态签署的分区。|
|3|public void sub-scribe(java.util.List<java.lang.String> topics, ConsumerRe-balanceListener listener)订阅给定的主题列表以获取动态签署的分区。|
|4|public void unsubscribe()取消订阅给定分区列表中的主题。|
|5|public void sub-scribe(java.util.List<java.lang.String> topics)订阅给定的主题列表以获取动态签署的分区。 如果给定的主题列表为空，则将其视为与unsubscribe（）相同。|
|6|public void sub-scribe(java.util.regex.Pattern pattern, ConsumerRebalanceLis-tener listener)参数模式以正则表达式的格式引用订阅模式，而侦听器参数从订阅模式中获取通知。|
|7|public void as-sign(java.util.List<TopicParti-tion> partitions)手动将分区列表分配给客户。|
|8|poll()获取使用其中一个订阅/分配API指定的主题或分区的数据。 如果在轮询数据之前没有订阅主题，这将返回错误。|
|9|public void commitSync()提交最后一次poll（）返回的主题和分区的所有sub-scribed列表的偏移量。 同样的操作应用于commitAsyn（）。|
|10|public void seek(TopicPartition partition, long offset)获取消费者将在下一个poll（）方法中使用的当前偏移值。|
|11|public void resume()恢复暂停的分区。|
|12|public void wakeup()唤醒消费者。|

##### ConsumerRecord API

ConsumerRecord API用于接收来自Kafka集群的记录。 该API由主题名称，分区编号以及指向Kafka分区中记录的偏移量组成。 ConsumerRecord类用于创建具有特定主题名称，分区计数和<key，value>对的消费者记录。 它有以下签名。


public ConsumerRecord(string topic,int partition, long offset,K key, V value)- topic - 从Kafka集群收到的消费者记录的主题名称。
- partition - 主题的分区。
- key - 记录的关键字，如果不存在关键字null将被返回。
- value - 记录内容。



##### ConsumerRecords API

ConsumerRecords API充当ConsumerRecord的容器。 此API用于保留特定主题的每个分区的ConsumerRecord列表。 它的构造函数定义如下。


public ConsumerRecords(java.util.Map<TopicPartition,java.util.List
<Consumer-Record>K,V>>> records)- TopicPartition - 返回特定主题的分区映射。
- records - ConsumerRecord的返回列表。

ConsumerRecords类定义了以下方法。



|序号|方法和描述|
|----|----|
|1|public int count()所有主题的记录数。|
|2|public Set partitions()该记录集中包含数据的分区集（如果没有数据返回，则该集为空）。|
|3|public Iterator iterator()迭代器使您能够遍历集合，获取或重新移动元素。|
|4|public List records()获取给定分区的记录列表。|
配置设置
以下列出了Consumer客户端API主配置设置的配置设置 -


|序号|设置和描述|
|----|----|
|1|bootstrap.servers引导代理名单。|
|2|group.id将个人消费者分配给组。|
|3|enable.auto.commit如果值为true，则为偏移启用自动提交，否则不提交。|
|4|auto.commit.interval.ms返回更新的消耗偏移量被写入ZooKeeper的频率。|
|5|session.timeout.ms指示在放弃并继续使用消息之前，Kafka将等待多少毫秒以等待ZooKeeper响应请求（读取或写入）。|
SimpleConsumer应用程序
生产者申请步骤在这里保持不变。 首先，启动您的ZooKeeper和Kafka代理。 然后使用名为SimpleCon-sumer.java的Java类创建一个SimpleConsumer应用程序并键入以下代码。


import java.util.Properties;import java.util.Arrays;import org.apache.kafka.clients.consumer.KafkaConsumer;import org.apache.kafka.clients.consumer.ConsumerRecords;import org.apache.kafka.clients.consumer.ConsumerRecord;publicclassSimpleConsumer{publicstaticvoid main(String[] args)throwsException{if(args.length ==0){System.out.println("Enter topic name");return;}//Kafka consumer configuration settingsString topicName = args[0].toString();Properties props =newProperties();
      
      props.put("bootstrap.servers","localhost:9092");
      props.put("group.id","test");
      props.put("enable.auto.commit","true");
      props.put("auto.commit.interval.ms","1000");
      props.put("session.timeout.ms","30000");
      props.put("key.deserializer","org.apache.kafka.common.serializa-tion.StringDeserializer");
      props.put("value.deserializer","org.apache.kafka.common.serializa-tion.StringDeserializer");KafkaConsumer<String,String> consumer =newKafkaConsumer<String,String>(props);//Kafka Consumer subscribes list of topics here.
      consumer.subscribe(Arrays.asList(topicName))//print the topic nameSystem.out.println("Subscribed to topic "+ topicName);int i =0;while(true){ConsumerRecords<String,String> records = con-sumer.poll(100);for(ConsumerRecord<String,String> record : records)// print the offset,key and value for the consumer records.System.out.printf("offset = %d, key = %s, value = %s\n", 
            record.offset(), record.key(), record.value());}}}
编译- 可以使用以下命令编译应用程序。


javac -cp “/path/to/kafka/kafka_2.11-0.9.0.0/lib/*” *.java
运行- 应用程序可以使用以下命令执行


java -cp “/path/to/kafka/kafka_2.11-0.9.0.0/lib/*”:. SimpleConsumer <topic-name>
输入 - 打开生产者CLI并发送一些消息给主题。 您可以将smple输入作为'Hello Consumer'。

输出 - 以下将是输出。

Subscribed to topic Hello-Kafka
offset = 3, key = null, value = Hello Consumer



