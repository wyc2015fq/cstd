# 一文精通kafka 消费者的三种语义 - Spark高级玩法 - CSDN博客
2018年11月01日 00:22:02[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：563
本文主要是以kafka 09的client为例子，详解kafka client的使用，包括kafka消费者的三种消费语义at-most-once, at-least-once, 和 exactly-once message ，生产者的使用等。
**(一) 创建topic**
bin/kafka-topics --zookeeper localhost:2181 --create --topic normal-topic --partitions 2 --replication-factor 1
**(二) 生产者**
publicclass ProducerExample {
publicstaticvoid main(String[] str) throws InterruptedException, IOException {
            System.out.println("Starting ProducerExample ...");
            sendMessages();
    }
privatestaticvoid sendMessages() throws InterruptedException, IOException {
            Producer<String, String> producer = createProducer();
            sendMessages(producer);
// Allow the producer to complete sending of the messages before program exit.
            Thread.sleep(20);
    }
privatestatic Producer<String, String> createProducer() {
        Properties props = new Properties();
        props.put("bootstrap.servers", "localhost:9092");
        props.put("acks", "all");
        props.put("retries", 0);
// Controls how much bytes sender would wait to batch up before publishing to Kafka.
        props.put("batch.size", 10);
        props.put("linger.ms", 1);
        props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");
returnnew KafkaProducer(props);
    }
privatestaticvoid sendMessages(Producer<String, String> producer) {
String topic = "normal-topic";
        int partition = 0;
        long record = 1;
for (int i = 1; i <= 10; i++) {
            producer.send(
new ProducerRecord<String, String>(topic, partition,                                 Long.toString(record),Long.toString(record++)));
        }
    }
}
**（三）消费者**
消费者注册到kafka有多种方式：
**subscribe:**这种方式在新增topic或者partition或者消费者增加或者消费者减少的时候，会进行消费者组内消费者的再平衡。
**assign:**这种方式注册的消费者不会进行rebalance。
上面两种方式都是可以实现，三种消费语义的。具体API的使用请看下文。
**1. At-most-once Kafka Consumer**
做多一次消费语义是kafka消费者的默认实现。配置这种消费者最简单的方式是
1). enable.auto.commit设置为true。
2). auto.commit.interval.ms设置为一个较低的时间范围。
3). consumer.commitSync()不要调用该方法。
由于上面的配置，就可以使得kafka有线程负责按照指定间隔提交offset。
但是这种方式会使得kafka消费者有两种消费语义：
**a.最多一次语义->at-most-once**
消费者的offset已经提交，但是消息还在处理，这个时候挂了，再重启的时候会从上次提交的offset处消费，导致上次在处理的消息部分丢失。
**b. 最少一次消费语义->at-least-once**
消费者已经处理完了，但是offset还没提交，那么这个时候消费者挂了，就会导致消费者重复消费消息处理。但是由于auto.commit.interval.ms设置为一个较低的时间范围，会降低这种情况出现的概率。
代码如下：
publicclass AtMostOnceConsumer {
publicstaticvoid main(String[] str) throws InterruptedException {
            System.out.println("Starting  AtMostOnceConsumer ...");
            execute();
        }
privatestaticvoid execute() throws InterruptedException {
                KafkaConsumer<String, String> consumer = createConsumer();
// Subscribe to all partition in that topic. 'assign' could be used here
// instead of 'subscribe' to subscribe to specific partition.
                consumer.subscribe(Arrays.asList("normal-topic"));
                processRecords(consumer);
        }
privatestatic KafkaConsumer<String, String> createConsumer() {
                Properties props = new Properties();
                props.put("bootstrap.servers", "localhost:9092");
String consumeGroup = "cg1";
                props.put("group.id", consumeGroup);
// Set this property, if auto commit should happen.
                props.put("enable.auto.commit", "true");
// Auto commit interval, kafka would commit offset at this interval.
                props.put("auto.commit.interval.ms", "101");
// This is how to control number of records being read in each poll
                props.put("max.partition.fetch.bytes", "135");
// Set this if you want to always read from beginning.
// props.put("auto.offset.reset", "earliest");
                props.put("heartbeat.interval.ms", "3000");
                props.put("session.timeout.ms", "6001");
                props.put("key.deserializer",
"org.apache.kafka.common.serialization.StringDeserializer");
                props.put("value.deserializer",
"org.apache.kafka.common.serialization.StringDeserializer");
returnnew KafkaConsumer<String, String>(props);
        }
privatestaticvoid processRecords(KafkaConsumer<String, String> consumer)  {
while (true) {
                        ConsumerRecords<String, String> records = consumer.poll(100);
                        long lastOffset = 0;
for (ConsumerRecord<String, String> record : records) {
                                System.out.printf("\n\roffset = %d, key = %s, value = %s", record.offset(),                                             record.key(), record.value());
                                lastOffset = record.offset();
                         }
                System.out.println("lastOffset read: " + lastOffset);
                process();
                }
        }
privatestaticvoid process() throws InterruptedException {
// create some delay to simulate processing of the message.
                Thread.sleep(20);
        }
}
**2. At-least-once kafka consumer**
实现最少一次消费语义的消费者也很简单。
1). 设置enable.auto.commit为false
2). 消息处理完之后手动调用consumer.commitSync()
这种方式就是要手动在处理完该次poll得到消息之后，调用offset异步提交函数consumer.commitSync()。建议是消费者内部实现密等，来避免消费者重复处理消息进而得到重复结果。最多一次发生的场景是消费者的消息处理完并输出到结果库（也可能是部分处理完），但是offset还没提交，这个时候消费者挂掉了，再重启的时候会重新消费并处理消息。
代码如下：
publicclass AtLeastOnceConsumer {
publicstaticvoid main(String[] str) throws InterruptedException {
            System.out.println("Starting AutoOffsetGuranteedAtLeastOnceConsumer ...");
            execute();
     }
privatestaticvoid execute() throws InterruptedException {
            KafkaConsumer<String, String> consumer = createConsumer();
// Subscribe to all partition in that topic. 'assign' could be used here
// instead of 'subscribe' to subscribe to specific partition.
            consumer.subscribe(Arrays.asList("normal-topic"));
            processRecords(consumer);
     }
privatestatic KafkaConsumer<String, String> createConsumer() {
            Properties props = new Properties();
            props.put("bootstrap.servers", "localhost:9092");
String consumeGroup = "cg1";
            props.put("group.id", consumeGroup);
// Set this property, if auto commit should happen.
            props.put("enable.auto.commit", "true");
// Make Auto commit interval to a big number so that auto commit does not happen,
// we are going to control the offset commit via consumer.commitSync(); after processing             // message.
            props.put("auto.commit.interval.ms", "999999999999");
// This is how to control number of messages being read in each poll
            props.put("max.partition.fetch.bytes", "135");
            props.put("heartbeat.interval.ms", "3000");
            props.put("session.timeout.ms", "6001");
            props.put("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
            props.put("value.deserializer","org.apache.kafka.common.serialization.StringDeserializer");
returnnew KafkaConsumer<String, String>(props);
    }
privatestaticvoid processRecords(KafkaConsumer<String, String> consumer) throws {
while (true) {
                    ConsumerRecords<String, String> records = consumer.poll(100);
                    long lastOffset = 0;
for (ConsumerRecord<String, String> record : records) {
                        System.out.printf("\n\roffset = %d, key = %s, value = %s", record.offset(),                                         record.key(), record.value());
                        lastOffset = record.offset();
                    }
                    System.out.println("lastOffset read: " + lastOffset);
                    process();
// Below call is important to control the offset commit. Do this call after you
// finish processing the business process.
                    consumer.commitSync();
            }
    }
privatestaticvoid process() throws InterruptedException {
// create some delay to simulate processing of the record.
        Thread.sleep(20);
    }
}
**3. 使用subscribe实现Exactly-once **
使用subscribe实现Exactly-once 很简单，具体思路如下：
1). 将enable.auto.commit设置为false。
2). 不调用consumer.commitSync()。
3). 使用subcribe定于topic。
4). 实现一个ConsumerRebalanceListener，在该listener内部执行
consumer.seek(topicPartition,offset)，从指定的topic/partition的offset处启动。
5). 在处理消息的时候，要同时控制保存住每个消息的offset。以原子事务的方式保存offset和处理的消息结果。传统数据库实现原子事务比较简单。但对于非传统数据库，比如hdfs或者nosql，为了实现这个目标，只能将offset与消息保存在同一行。
6). 实现密等，作为保护层。
代码如下：
publicclass ExactlyOnceDynamicConsumer {
privatestatic OffsetManager offsetManager = new OffsetManager("storage2");
publicstaticvoid main(String[] str) throws InterruptedException {
                System.out.println("Starting ExactlyOnceDynamicConsumer ...");
                readMessages();
        }
privatestaticvoid readMessages() throws InterruptedException {
                KafkaConsumer<String, String> consumer = createConsumer();
// Manually controlling offset but register consumer to topics to get dynamically
//  assigned partitions. Inside MyConsumerRebalancerListener use
// consumer.seek(topicPartition,offset) to control offset which messages to be read.
                consumer.subscribe(Arrays.asList("normal-topic"),
new MyConsumerRebalancerListener(consumer));
                processRecords(consumer);
        }
privatestatic KafkaConsumer<String, String> createConsumer() {
                Properties props = new Properties();
                props.put("bootstrap.servers", "localhost:9092");
String consumeGroup = "cg3";
                props.put("group.id", consumeGroup);
// Below is a key setting to turn off the auto commit.
                props.put("enable.auto.commit", "false");
                props.put("heartbeat.interval.ms", "2000");
                props.put("session.timeout.ms", "6001");
// Control maximum data on each poll, make sure this value is bigger than the maximum                   // single message size
                props.put("max.partition.fetch.bytes", "140");
                props.put("key.deserializer",                                 "org.apache.kafka.common.serialization.StringDeserializer");
                props.put("value.deserializer",                         "org.apache.kafka.common.serialization.StringDeserializer");
returnnew KafkaConsumer<String, String>(props);
        }
privatestaticvoid processRecords(KafkaConsumer<String, String> consumer) {
while (true) {
                    ConsumerRecords<String, String> records = consumer.poll(100);
for (ConsumerRecord<String, String> record : records) {
                            System.out.printf("offset = %d, key = %s, value = %s\n", record.offset(),                                     record.key(), record.value());
// Save processed offset in external storage.
                            offsetManager.saveOffsetInExternalStore(record.topic(), record.partition(),                                             record.offset());
                    }
               }
        }
}
publicclass MyConsumerRebalancerListener implements                                 org.apache.kafka.clients.consumer.ConsumerRebalanceListener {
private OffsetManager offsetManager = new OffsetManager("storage2");
private Consumer<String, String> consumer;
public MyConsumerRebalancerListener(Consumer<String, String> consumer) {
this.consumer = consumer;
        }
publicvoid onPartitionsRevoked(Collection<TopicPartition> partitions) {
for (TopicPartition partition : partitions) {
                    offsetManager.saveOffsetInExternalStore(partition.topic(), partition.partition(),                     consumer.position(partition));
                }
        }
publicvoid onPartitionsAssigned(Collection<TopicPartition> partitions) {
for (TopicPartition partition : partitions) {
                        consumer.seek(partition,                             offsetManager.readOffsetFromExternalStore(partition.topic(),                             partition.partition()));
                }
        }
}
/**
* The partition offset are stored in an external storage. In this case in a local file system where 
* program runs.
*/
publicclass OffsetManager {
privateString storagePrefix;
public OffsetManager(String storagePrefix) {
this.storagePrefix = storagePrefix;
        }
/**
        * Overwrite the offset for the topic in an external storage.
        *
        * @param topic - Topic name.
        * @param partition - Partition of the topic.
        * @param offset - offset to be stored.
        */
void saveOffsetInExternalStore(String topic, int partition, long offset) {
try {
                FileWriter writer = new FileWriter(storageName(topic, partition), false);
                BufferedWriter bufferedWriter = new BufferedWriter(writer);
                bufferedWriter.write(offset + "");
                bufferedWriter.flush();
                bufferedWriter.close();
            } catch (Exception e) {
                    e.printStackTrace();
thrownew RuntimeException(e);
            }
        }
/**
            * @return he last offset + 1 for the provided topic and partition.
        */
        long readOffsetFromExternalStore(String topic, int partition) {
try {
                        Stream<String> stream = Files.lines(Paths.get(storageName(topic, partition)));
return Long.parseLong(stream.collect(Collectors.toList()).get(0)) + 1;
                } catch (Exception e) {
                    e.printStackTrace();
                }
return0;
        }
privateString storageName(String topic, int partition) {
return storagePrefix + "-" + topic + "-" + partition;
        }
}
**4. 使用assign实现Exactly-once**
使用assign实现Exactly-once 也很简单，具体思路如下：
1). 将enable.auto.commit设置为false。
2). 不调用consumer.commitSync()。
3). 调用assign注册kafka消费者到kafka
4). 初次启动的时候，调用consumer.seek(topicPartition，offset)来指定offset。
5). 在处理消息的时候，要同时控制保存住每个消息的offset。以原子事务的方式保存offset和处理的消息结果。传统数据库实现原子事务比较简单。但对于非传统数据库，比如hdfs或者nosql，为了实现这个目标，只能将offset与消息保存在同一行。
6). 实现密等，作为保护层。
代码如下：
publicclass ExactlyOnceStaticConsumer {
privatestatic OffsetManager offsetManager = new OffsetManager("storage1");
publicstaticvoid main(String[] str) throws InterruptedException, IOException {
                System.out.println("Starting ExactlyOnceStaticConsumer ...");
                readMessages();
        }
privatestaticvoid readMessages() throws InterruptedException, IOException {
                KafkaConsumer<String, String> consumer = createConsumer();
String topic = "normal-topic";
                int partition = 1;
                TopicPartition topicPartition =
                                registerConsumerToSpecificPartition(consumer, topic, partition);
// Read the offset for the topic and partition from external storage.
                long offset = offsetManager.readOffsetFromExternalStore(topic, partition);
// Use seek and go to exact offset for that topic and partition.
                consumer.seek(topicPartition, offset);
                processRecords(consumer);
        }
privatestatic KafkaConsumer<String, String> createConsumer() {
                Properties props = new Properties();
                props.put("bootstrap.servers", "localhost:9092");
String consumeGroup = "cg2";
                props.put("group.id", consumeGroup);
// Below is a key setting to turn off the auto commit.
                props.put("enable.auto.commit", "false");
                props.put("heartbeat.interval.ms", "2000");
                props.put("session.timeout.ms", "6001");
// control maximum data on each poll, make sure this value is bigger than the maximum                 // single message size
                props.put("max.partition.fetch.bytes", "140");
                props.put("key.deserializer",                                     "org.apache.kafka.common.serialization.StringDeserializer");
                props.put("value.deserializer",                                     "org.apache.kafka.common.serialization.StringDeserializer");
returnnew KafkaConsumer<String, String>(props);
        }
/**
            * Manually listens for specific topic partition. But, if you are looking for example of how to                * dynamically listens to partition and want to manually control offset then see
            * ExactlyOnceDynamicConsumer.java
            */
privatestatic TopicPartition registerConsumerToSpecificPartition(
                    KafkaConsumer<String, String> consumer, String topic, int partition) {
                    TopicPartition topicPartition = new TopicPartition(topic, partition);
                    List<TopicPartition> partitions = Arrays.asList(topicPartition);
                    consumer.assign(partitions);
return topicPartition;
          }
/**
                * Process data and store offset in external store. Best practice is to do these operations
                * atomically. 
                */
privatestaticvoid processRecords(KafkaConsumer<String, String> consumer) throws {
while (true) {
                           ConsumerRecords<String, String> records = consumer.poll(100);
for (ConsumerRecord<String, String> record : records) {
                                    System.out.printf("offset = %d, key = %s, value = %s\n", record.offset(),                                                 record.key(), record.value());
                                    offsetManager.saveOffsetInExternalStore(record.topic(), record.partition(),                                                 record.offset());
                            }
                    }
            }
}
**[完]**
欢迎点赞转发。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
