# Apache Kafka教程A系列：实时应用程序(Twitter) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月09日 16:31:11[boonya](https://me.csdn.net/boonya)阅读数：60标签：[apache																[kafka																[realtime																[twitter](https://so.csdn.net/so/search/s.do?q=twitter&t=blog)
个人分类：[ApacheKafka](https://blog.csdn.net/boonya/article/category/6722001)





原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_real_time_application.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_real_time_application.htm)

让我们分析一个实时应用程序以获取最新的Twitter供稿及其标签。早些时候，我们看到Storm和Spark与Kafka的整合。在这两种情况下，我们创建了一个Kafka Producer（使用cli）向Kafka生态系统发送消息。然后，storm和spark集成通过使用卡夫卡消费者读取消息，并将其分别注入storm和spark生态系统。所以，我们实际上需要创建一个卡夫卡生产者——

- 使用“Twitter Streaming API”阅读Twitter Feeds
- 处理feeds
- 提取HashTags
- 把它发送给卡夫卡。
一旦Kafka接收到HashTags，Storm / Spark集成就会收到信息并将其发送到Storm / Spark生态系统。


#### Twitter流媒体API
“Twitter流媒体API”可以用任何编程语言访问。 “twitter4j”是一个开放源代码的非官方Java库，它提供了一个基于Java的模块来轻松访问“Twitter Streaming API”。 “twitter4j”提供了一个基于侦听器的框架来访问推文。要访问“Twitter Streaming API”，我们需要登录Twitter开发者帐户，并且应该获得以下OAuth身份验证详细信息。
- Customerkey
- CustomerSecret
- AccessToken
- AccessTookenSecret
一旦开发人员帐户被创建，下载“twitter4j”jar文件并将其放置在java类路径中。

下面列出了完整的Twitter Kafka生产者编码（KafkaTwitterProducer.java） -


import java.util.Arrays;import java.util.Properties;import java.util.concurrent.LinkedBlockingQueue;import twitter4j.*;import twitter4j.conf.*;import org.apache.kafka.clients.producer.Producer;import org.apache.kafka.clients.producer.KafkaProducer;import org.apache.kafka.clients.producer.ProducerRecord;publicclassKafkaTwitterProducer{publicstaticvoid main(String[] args)throwsException{LinkedBlockingQueue<Status> queue =newLinkedBlockingQueue<Sta-tus>(1000);if(args.length <5){System.out.println("Usage: KafkaTwitterProducer <twitter-consumer-key>
            <twitter-consumer-secret> <twitter-access-token>
            <twitter-access-token-secret>
            <topic-name> <twitter-search-keywords>");return;}String consumerKey = args[0].toString();String consumerSecret = args[1].toString();String accessToken = args[2].toString();String accessTokenSecret = args[3].toString();String topicName = args[4].toString();String[] arguments = args.clone();String[] keyWords =Arrays.copyOfRange(arguments,5, arguments.length);ConfigurationBuilder cb =newConfigurationBuilder();
      cb.setDebugEnabled(true).setOAuthConsumerKey(consumerKey).setOAuthConsumerSecret(consumerSecret).setOAuthAccessToken(accessToken).setOAuthAccessTokenSecret(accessTokenSecret);TwitterStream twitterStream =newTwitterStreamFactory(cb.build()).get-Instance();StatusListener listener =newStatusListener(){@Overridepublicvoid onStatus(Status status){      
            queue.offer(status);// System.out.println("@" + status.getUser().getScreenName() +" - "+ status.getText());// System.out.println("@" + status.getUser().getScreen-Name());/*for(URLEntity urle : status.getURLEntities()) {
               System.out.println(urle.getDisplayURL());
            }*//*for(HashtagEntity hashtage : status.getHashtagEntities()) {
               System.out.println(hashtage.getText());
            }*/}@Overridepublicvoid onDeletionNotice(StatusDeletionNotice statusDeletion-Notice){// System.out.println("Got a status deletion notice id:" + statusDeletionNotice.getStatusId());}@Overridepublicvoid onTrackLimitationNotice(int numberOfLimitedStatuses){// System.out.println("Got track limitation notice:" + 
               num-berOfLimitedStatuses);}@Overridepublicvoid onScrubGeo(long userId,long upToStatusId){// System.out.println("Got scrub_geo event userId:" + userId + "upToStatusId:"+ upToStatusId);}@Overridepublicvoid onStallWarning(StallWarning warning){// System.out.println("Got stall warning:" + warning);}@Overridepublicvoid onException(Exception ex){
            ex.printStackTrace();}};
      twitterStream.addListener(listener);FilterQuery query =newFilterQuery().track(keyWords);
      twitterStream.filter(query);Thread.sleep(5000);//Add Kafka producer config settingsProperties props =newProperties();
      props.put("bootstrap.servers","localhost:9092");
      props.put("acks","all");
      props.put("retries",0);
      props.put("batch.size",16384);
      props.put("linger.ms",1);
      props.put("buffer.memory",33554432);
      
      props.put("key.serializer","org.apache.kafka.common.serializa-tion.StringSerializer");
      props.put("value.serializer","org.apache.kafka.common.serializa-tion.StringSerializer");Producer<String,String> producer =newKafkaProducer<String,String>(props);int i =0;int j =0;while(i <10){Status ret = queue.poll();if(ret ==null){Thread.sleep(100);
            i++;}else{for(HashtagEntity hashtage : ret.getHashtagEntities()){System.out.println("Hashtag: "+ hashtage.getText());
               producer.send(newProducerRecord<String,String>(
                  top-icName,Integer.toString(j++), hashtage.getText()));}}}
      producer.close();Thread.sleep(5000);
      twitterStream.shutdown();}}
##### 编译

使用以下命令编译应用程序 -



javac -cp “/path/to/kafka/libs/*”:”/path/to/twitter4j/lib/*”:. KafkaTwitterProducer.java
##### 运行

打开两个控制台。 在一个控制台中运行上面编译的应用程序，如下所示。



java -cp “/path/to/kafka/libs/*”:”/path/to/twitter4j/lib/*”:
. KafkaTwitterProducer <twitter-consumer-key>
<twitter-consumer-secret>
<twitter-access-token>
<twitter-ac-cess-token-secret>
my-first-topic food在另一个窗口中运行上一章中介绍的任何一个Spark / Storm应用程序。 要注意的要点是在两种情况下使用的主题应该是相同的。 在这里，我们用““my-first-topic”作为主题名称。


##### 输出
这个应用程序的输出将取决于关键字和twitter的当前提要。 样本输出在下面指定（storm集成）。


. . .
food : 1
foodie : 2
burger : 1
. . .](https://so.csdn.net/so/search/s.do?q=realtime&t=blog)](https://so.csdn.net/so/search/s.do?q=kafka&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)




