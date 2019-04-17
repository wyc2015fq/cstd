# Apache Kafka教程A系列：消费者群体示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月03日 17:01:26[boonya](https://me.csdn.net/boonya)阅读数：132标签：[Apache																[Kafka](https://so.csdn.net/so/search/s.do?q=Kafka&t=blog)](https://so.csdn.net/so/search/s.do?q=Apache&t=blog)
个人分类：[ApacheKafka](https://blog.csdn.net/boonya/article/category/6722001)








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_consumer_group_example.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_consumer_group_example.htm)

消费者群体（group）是来自Kafka主题的多线程或多机器消费。



#### 消费群体
- 消费者可以通过使用samegroup.id加入一个组。
- 一个组的最大并行度是该组中的消费者的数量←分区的数量。
- Kafka将一个主题的分区分配给组中的使用者，以便每个分区仅由组中的一位消费者使用。
- 卡夫卡保证只有群组中的单个消费者阅读消息。
- 消费者可以按照存储在日志中的顺序查看消息。


##### 重新平衡消费者

添加更多流程/线程将导致Kafka重新平衡。 如果任何消费者或经纪商未能向ZooKeeper发送心跳，则可以通过Kafka集群重新配置它。 在这种重新平衡期间，Kafka会将可用分区分配给可用线程，可能会将分区移至另一个进程。


import java.util.Properties;import java.util.Arrays;import org.apache.kafka.clients.consumer.KafkaConsumer;import org.apache.kafka.clients.consumer.ConsumerRecords;import org.apache.kafka.clients.consumer.ConsumerRecord;publicclassConsumerGroup{publicstaticvoid main(String[] args)throwsException{if(args.length <2){System.out.println("Usage: consumer <topic> <groupname>");return;}String topic = args[0].toString();Stringgroup= args[1].toString();Properties props =newProperties();
      props.put("bootstrap.servers","localhost:9092");
      props.put("group.id",group);
      props.put("enable.auto.commit","true");
      props.put("auto.commit.interval.ms","1000");
      props.put("session.timeout.ms","30000");
      props.put("key.deserializer","org.apache.kafka.common.serializa-tion.StringDeserializer");
      props.put("value.deserializer","org.apache.kafka.common.serializa-tion.StringDeserializer");KafkaConsumer<String,String> consumer =newKafkaConsumer<String,String>(props);
      
      consumer.subscribe(Arrays.asList(topic));System.out.println("Subscribed to topic "+ topic);int i =0;while(true){ConsumerRecords<String,String> records = con-sumer.poll(100);for(ConsumerRecord<String,String> record : records)System.out.printf("offset = %d, key = %s, value = %s\n", 
               record.offset(), record.key(), record.value());}}}
**编译**


javac -cp “/path/to/kafka/kafka_2.11-0.9.0.0/libs/*" ConsumerGroup.java
**执行**


>>java -cp “/path/to/kafka/kafka_2.11-0.9.0.0/libs/*":. 
ConsumerGroup <topic-name> my-group
>>java -cp "/home/bala/Workspace/kafka/kafka_2.11-0.9.0.0/libs/*":. 
ConsumerGroup <topic-name> my-group
在这里，我们创建了一个样本组名称作为my-group与两个消费者。 同样，您可以在组中创建您的组和消费者数量。

**输入**

打开生产者CLI并发送一些消息，如 -


Test consumer group 01
Test consumer group 02
**第一个过程的输出**


Subscribed to topic Hello-kafka
offset =3, key =null, value =Test consumer group01
**第二个过程的输出**



Subscribed to topic Hello-kafka
offset = 3, key = null, value = Test consumer group 02现在希望你能通过使用Java客户端演示了解SimpleConsumer和ConsumeGroup。 现在您已经了解了如何使用Java客户端发送和接收消息。 让我们在下一章继续卡夫卡与大数据技术的整合。




