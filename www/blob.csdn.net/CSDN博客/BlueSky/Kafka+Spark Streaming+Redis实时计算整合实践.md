# Kafka+Spark Streaming+Redis实时计算整合实践 - BlueSky - CSDN博客
2015年11月12日 17:15:44[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1592
							转自：http://shiyanjun.cn/archives/1097.html
# [简单之美](http://shiyanjun.cn/)
## 简单之美，难得简单，享受简单的唯美。
# Main menu
[Skip to content](#content)
- [首页](http://shiyanjun.cn)
- [编程语言](http://shiyanjun.cn/archives/category/programming)
- [算法](http://shiyanjun.cn/archives/category/algorithm)
- [开源技术](http://shiyanjun.cn/archives/category/opensource)
- [数据库](http://shiyanjun.cn/archives/category/database)
- [服务器](http://shiyanjun.cn/archives/category/server)
- [数据挖掘](http://shiyanjun.cn/archives/category/datamining)
- [关于](http://shiyanjun.cn/about)
搜索：
# Kafka+Spark Streaming+Redis实时计算整合实践
[2015-04-28 21:11:35](http://shiyanjun.cn/archives/1097.html)[Yanjun](http://shiyanjun.cn/archives/author/yanjun)
基于Spark通用计算平台，可以很好地扩展各种计算类型的应用，尤其是Spark提供了内建的计算库支持，像Spark Streaming、Spark SQL、MLlib、GraphX，这些内建库都提供了高级抽象，可以用非常简洁的代码实现复杂的计算逻辑、这也得益于Scala编程语言的简洁性。这里，我们基于1.3.0版本的Spark搭建了计算平台，实现基于Spark Streaming的实时计算。
我们的应用场景是分析用户使用手机App的行为，描述如下所示：
- 手机客户端会收集用户的行为事件（我们以点击事件为例），将数据发送到数据服务器，我们假设这里直接进入到Kafka消息队列
- 后端的实时服务会从Kafka消费数据，将数据读出来并进行实时分析，这里选择Spark Streaming，因为Spark Streaming提供了与Kafka整合的内置支持
- 经过Spark Streaming实时计算程序分析，将结果写入Redis，可以实时获取用户的行为数据，并可以导出进行离线综合统计分析
**Spark Streaming介绍**
Spark Streaming提供了一个叫做DStream（Discretized Stream）的高级抽象，DStream表示一个持续不断输入的数据流，可以基于Kafka、TCP Socket、Flume等输入数据流创建。在内部，一个DStream实际上是由一个RDD序列组成的。Sparking Streaming是基于Spark平台的，也就继承了Spark平台的各种特性，如容错（Fault-tolerant）、可扩展（Scalable）、高吞吐（High-throughput）等。
在Spark Streaming中，每个DStream包含了一个时间间隔之内的数据项的集合，我们可以理解为指定时间间隔之内的一个batch，每一个batch就构成一个RDD数据集，所以DStream就是一个个batch的有序序列，时间是连续的，按照时间间隔将数据流分割成一个个离散的RDD数据集，如图所示（来自官网）：
![streaming-dstream](http://shiyanjun.cn/wp-content/uploads/2015/04/streaming-dstream.png)
我们都知道，Spark支持两种类型操作：Transformations和Actions。Transformation从一个已知的RDD数据集经过转换得到一个新的RDD数据集，这些Transformation操作包括map、filter、flatMap、union、join等，而且Transformation具有lazy的特性，调用这些操作并没有立刻执行对已知RDD数据集的计算操作，而是在调用了另一类型的Action操作才会真正地执行。Action执行，会真正地对RDD数据集进行操作，返回一个计算结果给Driver程序，或者没有返回结果，如将计算结果数据进行持久化，Action操作包括reduceByKey、count、foreach、collect等。关于Transformations和Actions更详细内容，可以查看官网文档。
同样、Spark Streaming提供了类似Spark的两种操作类型，分别为Transformations和Output操作，它们的操作对象是DStream，作用也和Spark类似：Transformation从一个已知的DStream经过转换得到一个新的DStream，而且Spark Streaming还额外增加了一类针对Window的操作，当然它也是Transformation，但是可以更灵活地控制DStream的大小（时间间隔大小、数据元素个数），例如window(windowLength, slideInterval)、countByWindow(windowLength, slideInterval)、reduceByWindow(func, windowLength, slideInterval)等。Spark Streaming的Output操作允许我们将DStream数据输出到一个外部的存储系统，如数据库或文件系统等，执行Output操作类似执行Spark的Action操作，使得该操作之前lazy的Transformation操作序列真正地执行。
**Kafka+Spark Streaming+Redis编程实践**
下面，我们根据上面提到的应用场景，来编程实现这个实时计算应用。首先，写了一个Kafka Producer模拟程序，用来模拟向Kafka实时写入用户行为的事件数据，数据是JSON格式，示例如下：
```
{"uid":"068b746ed4620d25e26055a9f804385f","event_time":"1430204612405","os_type":"Android","click_count":6}
```
一个事件包含4个字段：
- uid：用户编号
- event_time：事件发生时间戳
- os_type：手机App操作系统类型
- click_count：点击次数
下面是我们实现的代码，如下所示：
``` scala; title
package org.shirdrn.spark.streaming.utils
import java.util.Properties
import scala.util.Properties
import org.codehaus.jettison.json.JSONObject
import kafka.javaapi.producer.Producer
import kafka.producer.KeyedMessage
import kafka.producer.KeyedMessage
import kafka.producer.ProducerConfig
import scala.util.Random
object KafkaEventProducer {
  private val users = Array(
      "4A4D769EB9679C054DE81B973ED5D768", "8dfeb5aaafc027d89349ac9a20b3930f",
      "011BBF43B89BFBF266C865DF0397AA71", "f2a8474bf7bd94f0aabbd4cdd2c06dcf",
      "068b746ed4620d25e26055a9f804385f", "97edfc08311c70143401745a03a50706",
      "d7f141563005d1b5d0d3dd30138f3f62", "c8ee90aade1671a21336c721512b817a",
      "6b67c8c700427dee7552f81f3228c927", "a95f22eabc4fd4b580c011a3161a9d9d")
  private val random = new Random()
  private var pointer = -1
  def getUserID() : String = {
       pointer = pointer + 1
    if(pointer >= users.length) {
      pointer = 0
      users(pointer)
    } else {
      users(pointer)
    }
  }
  def click() : Double = {
    random.nextInt(10)
  }
  // bin/kafka-topics.sh --zookeeper zk1:2181,zk2:2181,zk3:2181/kafka --create --topic user_events --replication-factor 2 --partitions 2
  // bin/kafka-topics.sh --zookeeper zk1:2181,zk2:2181,zk3:2181/kafka --list
  // bin/kafka-topics.sh --zookeeper zk1:2181,zk2:2181,zk3:2181/kafka --describe user_events
  // bin/kafka-console-consumer.sh --zookeeper zk1:2181,zk2:2181,zk3:22181/kafka --topic test_json_basis_event --from-beginning
  def main(args: Array[String]): Unit = {
    val topic = "user_events"
    val brokers = "10.10.4.126:9092,10.10.4.127:9092"
    val props = new Properties()
    props.put("metadata.broker.list", brokers)
    props.put("serializer.class", "kafka.serializer.StringEncoder")
    val kafkaConfig = new ProducerConfig(props)
    val producer = new Producer[String, String](kafkaConfig)
    while(true) {
      // prepare event data
      val event = new JSONObject()
      event
        .put("uid", getUserID)
        .put("event_time", System.currentTimeMillis.toString)
        .put("os_type", "Android")
        .put("click_count", click)
      // produce event message
      producer.send(new KeyedMessage[String, String](topic, event.toString))
      println("Message sent: " + event)
      Thread.sleep(200)
    }
  }  
}
```
通过控制上面程序最后一行的时间间隔来控制模拟写入速度。下面我们来讨论实现实时统计每个用户的点击次数，它是按照用户分组进行累加次数，逻辑比较简单，关键是在实现过程中要注意一些问题，如对象序列化等。先看实现代码，稍后我们再详细讨论，代码实现如下所示：
``` scala; title
object UserClickCountAnalytics {
  def main(args: Array[String]): Unit = {
    var masterUrl = "local[1]"
    if (args.length > 0) {
      masterUrl = args(0)
    }
    // Create a StreamingContext with the given master URL
    val conf = new SparkConf().setMaster(masterUrl).setAppName("UserClickCountStat")
    val ssc = new StreamingContext(conf, Seconds(5))
    // Kafka configurations
    val topics = Set("user_events")
    val brokers = "10.10.4.126:9092,10.10.4.127:9092"
    val kafkaParams = Map[String, String](
      "metadata.broker.list" -> brokers, "serializer.class" -> "kafka.serializer.StringEncoder")
    val dbIndex = 1
    val clickHashKey = "app::users::click"
    // Create a direct stream
    val kafkaStream = KafkaUtils.createDirectStream[String, String, StringDecoder, StringDecoder](ssc, kafkaParams, topics)
    val events = kafkaStream.flatMap(line => {
      val data = JSONObject.fromObject(line._2)
      Some(data)
    })
    // Compute user click times
    val userClicks = events.map(x => (x.getString("uid"), x.getInt("click_count"))).reduceByKey(_ + _)
    userClicks.foreachRDD(rdd => {
      rdd.foreachPartition(partitionOfRecords => {
        partitionOfRecords.foreach(pair => {
          val uid = pair._1
          val clickCount = pair._2
          val jedis = RedisClient.pool.getResource
          jedis.select(dbIndex)
          jedis.hincrBy(clickHashKey, uid, clickCount)
          RedisClient.pool.returnResource(jedis)
        })
      })
    })
    ssc.start()
    ssc.awaitTermination()
  }
}
```
上面代码使用了Jedis客户端来操作Redis，将分组计数结果数据累加写入Redis存储，如果其他系统需要实时获取该数据，直接从Redis实时读取即可。RedisClient实现代码如下所示：
``` scala; title
object RedisClient extends Serializable {
  val redisHost = "10.10.4.130"
  val redisPort = 6379
  val redisTimeout = 30000
  lazy val pool = new JedisPool(new GenericObjectPoolConfig(), redisHost, redisPort, redisTimeout)
  lazy val hook = new Thread {
    override def run = {
      println("Execute hook thread: " + this)
      pool.destroy()
    }
  }
  sys.addShutdownHook(hook.run)
}
```
上面代码我们分别在local[K]和Spark Standalone集群模式下运行通过。
如果我们是在开发环境进行调试的时候，也就是使用local[K]部署模式，在本地启动K个Worker线程来计算，这K个Worker在同一个JVM实例里，上面的代码默认情况是，如果没有传参数则是local[K]模式，所以如果使用这种方式在创建Redis连接池或连接的时候，可能非常容易调试通过，但是在使用Spark Standalone、YARN Client（YARN Cluster）或Mesos集群部署模式的时候，就会报错，主要是由于在处理Redis连接池或连接的时候出错了。我们可以看一下Spark架构，如图所示（来自官网）：
![spark-cluster-overview](http://shiyanjun.cn/wp-content/uploads/2015/04/spark-cluster-overview.png)
无论是在本地模式、Standalone模式，还是在Mesos或YARN模式下，整个Spark集群的结构都可以用上图抽象表示，只是各个组件的运行环境不同，导致组件可能是分布式的，或本地的，或单个JVM实例的。如在本地模式，则上图表现为在同一节点上的单个进程之内的多个组件；而在YARN Client模式下，Driver程序是在YARN集群之外的一个节点上提交Spark Application，其他的组件都运行在YARN集群管理的节点上。
在Spark集群环境部署Application后，在进行计算的时候会将作用于RDD数据集上的函数（Functions）发送到集群中Worker上的Executor上（在Spark Streaming中是作用于DStream的操作），那么这些函数操作所作用的对象（Elements）必须是可序列化的，通过Scala也可以使用lazy引用来解决，否则这些对象（Elements）在跨节点序列化传输后，无法正确地执行反序列化重构成实际可用的对象。上面代码我们使用lazy引用（Lazy Reference）来实现的，代码如下所示：
``` scala; title
// lazy pool reference
lazy val pool = new JedisPool(new GenericObjectPoolConfig(), redisHost, redisPort, redisTimeout)
...
partitionOfRecords.foreach(pair => {
  val uid = pair._1
  val clickCount = pair._2
  val jedis = RedisClient.pool.getResource
  jedis.select(dbIndex)
  jedis.hincrBy(clickHashKey, uid, clickCount)
  RedisClient.pool.returnResource(jedis)
})
```
另一种方式，我们将代码修改为，把对Redis连接的管理放在操作DStream的Output操作范围之内，因为我们知道它是在特定的Executor中进行初始化的，使用一个单例的对象来管理，如下所示：
``` scala; title
package org.shirdrn.spark.streaming
import org.apache.commons.pool2.impl.GenericObjectPoolConfig
import org.apache.spark.SparkConf
import org.apache.spark.streaming.Seconds
import org.apache.spark.streaming.StreamingContext
import org.apache.spark.streaming.dstream.DStream.toPairDStreamFunctions
import org.apache.spark.streaming.kafka.KafkaUtils
import kafka.serializer.StringDecoder
import net.sf.json.JSONObject
import redis.clients.jedis.JedisPool
object UserClickCountAnalytics {
  def main(args: Array[String]): Unit = {
    var masterUrl = "local[1]"
    if (args.length > 0) {
      masterUrl = args(0)
    }
    // Create a StreamingContext with the given master URL
    val conf = new SparkConf().setMaster(masterUrl).setAppName("UserClickCountStat")
    val ssc = new StreamingContext(conf, Seconds(5))
    // Kafka configurations
    val topics = Set("user_events")
    val brokers = "10.10.4.126:9092,10.10.4.127:9092"
    val kafkaParams = Map[String, String](
      "metadata.broker.list" -> brokers, "serializer.class" -> "kafka.serializer.StringEncoder")
    val dbIndex = 1
    val clickHashKey = "app::users::click"
    // Create a direct stream
    val kafkaStream = KafkaUtils.createDirectStream[String, String, StringDecoder, StringDecoder](ssc, kafkaParams, topics)
    val events = kafkaStream.flatMap(line => {
      val data = JSONObject.fromObject(line._2)
      Some(data)
    })
    // Compute user click times
    val userClicks = events.map(x => (x.getString("uid"), x.getInt("click_count"))).reduceByKey(_ + _)
    userClicks.foreachRDD(rdd => {
      rdd.foreachPartition(partitionOfRecords => {
        partitionOfRecords.foreach(pair => {
          /**
           * Internal Redis client for managing Redis connection {@link Jedis} based on {@link RedisPool}
           */
          object InternalRedisClient extends Serializable {
            @transient private var pool: JedisPool = null
            def makePool(redisHost: String, redisPort: Int, redisTimeout: Int,
                maxTotal: Int, maxIdle: Int, minIdle: Int): Unit = {
              makePool(redisHost, redisPort, redisTimeout, maxTotal, maxIdle, minIdle, true, false, 10000)   
            }
            def makePool(redisHost: String, redisPort: Int, redisTimeout: Int,
                maxTotal: Int, maxIdle: Int, minIdle: Int, testOnBorrow: Boolean,
                testOnReturn: Boolean, maxWaitMillis: Long): Unit = {
              if(pool == null) {
                   val poolConfig = new GenericObjectPoolConfig()
                   poolConfig.setMaxTotal(maxTotal)
                   poolConfig.setMaxIdle(maxIdle)
                   poolConfig.setMinIdle(minIdle)
                   poolConfig.setTestOnBorrow(testOnBorrow)
                   poolConfig.setTestOnReturn(testOnReturn)
                   poolConfig.setMaxWaitMillis(maxWaitMillis)
                   pool = new JedisPool(poolConfig, redisHost, redisPort, redisTimeout)
                   val hook = new Thread{
                        override def run = pool.destroy()
                   }
                   sys.addShutdownHook(hook.run)
              }
            }
            def getPool: JedisPool = {
              assert(pool != null)
              pool
            }
          }
          // Redis configurations
          val maxTotal = 10
          val maxIdle = 10
          val minIdle = 1
          val redisHost = "10.10.4.130"
          val redisPort = 6379
          val redisTimeout = 30000
          val dbIndex = 1
          InternalRedisClient.makePool(redisHost, redisPort, redisTimeout, maxTotal, maxIdle, minIdle)
          val uid = pair._1
          val clickCount = pair._2
          val jedis =InternalRedisClient.getPool.getResource
          jedis.select(dbIndex)
          jedis.hincrBy(clickHashKey, uid, clickCount)
          InternalRedisClient.getPool.returnResource(jedis)
        })
      })
    })
    ssc.start()
    ssc.awaitTermination()
  }
}
```
上面代码实现，得益于Scala语言的特性，可以在代码中任何位置进行class或object的定义，我们将用来管理Redis连接的代码放在了特定操作的内部，就避免了瞬态（Transient）对象跨节点序列化的问题。这样做还要求我们能够了解Spark内部是如何操作RDD数据集的，更多可以参考RDD或Spark相关文档。
在集群上，以Standalone模式运行，执行如下命令：
``` bash; title
cd /usr/local/spark
./bin/spark-submit --class org.shirdrn.spark.streaming.UserClickCountAnalytics --master spark://hadoop1:7077 --executor-memory 1G --total-executor-cores 2 ~/spark-0.0.SNAPSHOT.jar spark://hadoop1:7077
```
可以查看集群中各个Worker节点执行计算任务的状态，也可以非常方便地通过Web页面查看。
下面，看一下我们存储到Redis中的计算结果，如下所示：
```
127.0.0.1:6379[1]> HGETALL app::users::click
1) "4A4D769EB9679C054DE81B973ED5D768"
2) "7037"
3) "8dfeb5aaafc027d89349ac9a20b3930f"
4) "6992"
5) "011BBF43B89BFBF266C865DF0397AA71"
6) "7021"
7) "97edfc08311c70143401745a03a50706"
8) "6874"
9) "d7f141563005d1b5d0d3dd30138f3f62"
10) "7057"
11) "a95f22eabc4fd4b580c011a3161a9d9d"
12) "7092"
13) "6b67c8c700427dee7552f81f3228c927"
14) "7266"
15) "f2a8474bf7bd94f0aabbd4cdd2c06dcf"
16) "7188"
17) "c8ee90aade1671a21336c721512b817a"
18) "6950"
19) "068b746ed4620d25e26055a9f804385f"
```
有关更多关于Spark Streaming的详细内容，可以参考官方文档。
**附录**
这里，附上前面开发的应用所对应的依赖，以及打包Spark Streaming应用程序的Maven配置，以供参考。如果使用maven-shade-plugin插件，配置有问题的话，打包后在Spark集群上提交Application时候可能会报错Invalid signature file digest for Manifest main attributes。参考的Maven配置，如下所示：
``` xml; title
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
     xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
     <modelVersion>4.0.0</modelVersion>
     <groupId>org.shirdrn.spark</groupId>
     <artifactId>spark</artifactId>
     <version>0.0.1-SNAPSHOT</version>
     <dependencies>
          <dependency>
               <groupId>org.apache.spark</groupId>
               <artifactId>spark-core_2.10</artifactId>
               <version>1.3.0</version>
          </dependency>
          <dependency>
               <groupId>org.apache.spark</groupId>
               <artifactId>spark-streaming_2.10</artifactId>
               <version>1.3.0</version>
          </dependency>
          <dependency>
               <groupId>net.sf.json-lib</groupId>
               <artifactId>json-lib</artifactId>
               <version>2.3</version>
          </dependency>
          <dependency>
               <groupId>org.apache.spark</groupId>
               <artifactId>spark-streaming-kafka_2.10</artifactId>
               <version>1.3.0</version>
          </dependency>
          <dependency>
               <groupId>redis.clients</groupId>
               <artifactId>jedis</artifactId>
               <version>2.5.2</version>
          </dependency>
          <dependency>
               <groupId>org.apache.commons</groupId>
               <artifactId>commons-pool2</artifactId>
               <version>2.2</version>
          </dependency>
     </dependencies>
     <build>
          <sourceDirectory>${basedir}/src/main/scala</sourceDirectory>
          <testSourceDirectory>${basedir}/src/test/scala</testSourceDirectory>
          <resources>
               <resource>
                    <directory>${basedir}/src/main/resources</directory>
               </resource>
          </resources>
          <testResources>
               <testResource>
                    <directory>${basedir}/src/test/resources</directory>
               </testResource>
          </testResources>
          <plugins>
               <plugin>
                    <artifactId>maven-compiler-plugin</artifactId>
                    <version>3.1</version>
                    <configuration>
                         <source>1.6</source>
                         <target>1.6</target>
                    </configuration>
               </plugin>
               <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-shade-plugin</artifactId>
                    <version>2.2</version>
                    <configuration>
                         <createDependencyReducedPom>true</createDependencyReducedPom>
                    </configuration>
                    <executions>
                         <execution>
                              <phase>package</phase>
                              <goals>
                                   <goal>shade</goal>
                              </goals>
                              <configuration>
                                   <artifactSet>
                                        <includes>
                                             <include>*:*</include>
                                        </includes>
                                   </artifactSet>
                                   <filters>
                                        <filter>
                                             <artifact>*:*</artifact>
                                             <excludes>
                                                  <exclude>META-INF/*.SF</exclude>
                                                  <exclude>META-INF/*.DSA</exclude>
                                                  <exclude>META-INF/*.RSA</exclude>
                                             </excludes>
                                        </filter>
                                   </filters>
                                   <transformers>
                                        <transformer
                                             implementation="org.apache.maven.plugins.shade.resource.ServicesResourceTransformer" />
                                        <transformer
                                             implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">
                                             <resource>reference.conf</resource>
                                        </transformer>
                                        <transformer
                                             implementation="org.apache.maven.plugins.shade.resource.DontIncludeResourceTransformer">
                                             <resource>log4j.properties</resource>
                                        </transformer>
                                   </transformers>
                              </configuration>
                         </execution>
                    </executions>
               </plugin>
          </plugins>
     </build>
</project>
```
**参考链接**
- [http://spark.apache.org/docs/1.3.0/index.html](http://spark.apache.org/docs/1.3.0/index.html)
- [http://spark.apache.org/docs/1.3.0/cluster-overview.html](http://spark.apache.org/docs/1.3.0/cluster-overview.html)
- [http://spark.apache.org/docs/1.3.0/job-scheduling.html](http://spark.apache.org/docs/1.3.0/job-scheduling.html)
- [http://spark.apache.org/docs/1.3.0/streaming-programming-guide.html](http://spark.apache.org/docs/1.3.0/streaming-programming-guide.html)
- [http://stackoverflow.com/questions/28006517/redis-on-sparktask-not-serializable](http://stackoverflow.com/questions/28006517/redis-on-sparktask-not-serializable)
### 相关文章
- [Spark-1.3.1与Hive整合实现查询分析](http://shiyanjun.cn/archives/1113.html)
- [Shark-0.9.0安装配置运行实践](http://shiyanjun.cn/archives/772.html)
- [RDD：基于内存的集群计算容错抽象](http://shiyanjun.cn/archives/744.html)
- [使用Java编写并运行Spark应用程序](http://shiyanjun.cn/archives/742.html)
- [CentOS 6.4下安装配置Spark-0.9集群](http://shiyanjun.cn/archives/696.html)
# Post navigation
[← 基于Dubbo框架构建分布式服务](http://shiyanjun.cn/archives/1075.html)
[Spark-1.3.1与Hive整合实现查询分析 →](http://shiyanjun.cn/archives/1113.html)
##             评论(14)： “Kafka+Spark Streaming+Redis实时计算整合实践”      
- 
![](http://shiyanjun.cn/wp-content/uploads/2014/11/User.png)Moriarty
[2015-06-10 15:10:08](http://shiyanjun.cn/archives/1097.html#comment-54895)
例子很详细，非常感谢。尤其是带延迟执行的RedisClient的例子对我很有帮助。
另外，按官网的说法，rdd本身就是延迟执行的。所以，在yarn模式下，InternalRedisClient实例的创建会下推到executor上（我简单验证过）。也就是说InternalRedisClient实例并没有经过跨节点的序列化和反序列化，InternalRedisClient类不需要实现serializable接口。
换句话说，作者的方法应该是避免了InternalRedisClient的跨节点序列化，而不是实现了InternalRedisClient跨节点序列化。
如有不对，还请不吝赐教。☺
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=54895#respond' onclick='return addComment.moveForm("comment-54895", "54895", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
<ul class="children">
<li class="comment odd alt depth-2" id="li-comment-54896">
    <article id="comment-54896" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">Moriarty</cite>                </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-54896"><time pubdate datetime="2015-06-10T15:15:18+00:00">
                2015-06-10 15:15:18                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>另外，我实现的时候把从pool获取jedis的代码放到了partition层。<br />
```
虽然，这和放在最里面的foreach里的效率相差不会太大。但是总感觉每处理一条记录就从pool获取一次连接再归还有点不舒服。O(∩_∩)O哈哈~
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=54896#respond' onclick='return addComment.moveForm("comment-54896", "54896", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
</li><!-- #comment-## -->
<li class="comment byuser comment-author-yanjun bypostauthor even depth-2" id="li-comment-54936">
    <article id="comment-54936" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/03/alien_gray.png" width="40" height="40" alt="Yanjun" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo" />                   <cite class="fn"><a href='http://www.shiyanjun.cn' rel='external nofollow' class='url'>Yanjun</a></cite>                </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-54936"><time pubdate datetime="2015-06-15T14:20:46+00:00">
                2015-06-15 14:20:46                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>“rdd本身就是延迟执行的”，说的是延迟执行Transformation操作，而不是说对象的实例化都是延迟的，如果Driver能够实例化，那就是对象了，可能就存在跨节点序列化。</p>
```
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=54936#respond' onclick='return addComment.moveForm("comment-54936", "54936", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
<ul class="children">
<li class="comment odd alt depth-3" id="li-comment-54976">
    <article id="comment-54976" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">NextMark</cite>                </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-54976"><time pubdate datetime="2015-06-16T20:06:39+00:00">
                2015-06-16 20:06:39                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>我这里为什么一直会报 WARN utils.VerifiableProperties: Property metadata.broker.list is not valid 这样的问题？再往后就是这了<br />
```
scheduler.ReceiverTracker: Deregistered receiver for stream 0: Error starting receiver 0 – java.lang.NoSuchMethodException: scala.runtime.Nothing$.(kafka.utils.VerifiableProperties)       at java.lang.Class.getConstructor0(Class.java:2849)       at java.lang.Class.getConstructor(Class.java:1718)       at org.apache.spark.streaming.kafka.KafkaReceiver.onStart(KafkaInputDStream.scala:103)       at org.apache.spark.streaming.receiver.ReceiverSupervisor.startReceiver(ReceiverSupervisor.scala:121)       at org.apache.spark.streaming.receiver.ReceiverSupervisor.start(ReceiverSupervisor.scala:106)       at org.apache.spark.streaming.scheduler.ReceiverTracker$ReceiverLauncher
$anonfun$8.apply(ReceiverTracker.scala:277)       at org.apache.spark.streaming.scheduler.ReceiverTracker$ReceiverLauncher$anonfun$8.apply(ReceiverTracker.scala:269)       at org.apache.spark.SparkContext$$anonfun$runJob$4.apply(SparkContext.scala:1319)<br />	at org.apache.spark.SparkContext$$anonfun$runJob$4.apply(SparkContext.scala:1319)       at org.apache.spark.scheduler.ResultTask.runTask(ResultTask.scala:61)       at org.apache.spark.scheduler.Task.run(Task.scala:56)       at org.apache.spark.executor.Executor$TaskRunner.run(Executor.scala:196)
    at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1145)
    at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:615)
    at java.lang.Thread.run(Thread.java:745)
请指教
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=54976#respond' onclick='return addComment.moveForm("comment-54976", "54976", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
<ul class="children">
<li class="comment byuser comment-author-yanjun bypostauthor even depth-4" id="li-comment-54981">
    <article id="comment-54981" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/03/alien_gray.png" width="40" height="40" alt="Yanjun" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo" />                   <cite class="fn"><a href='http://www.shiyanjun.cn' rel='external nofollow' class='url'>Yanjun</a></cite>                </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-54981"><time pubdate datetime="2015-06-17T10:38:35+00:00">
                2015-06-17 10:38:35                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>可能是版本不兼容吧：<br />
```
java.lang.NoSuchMethodException: scala.runtime.Nothing$.(kafka.utils.VerifiableProperties)
我这里用的Kafka是kafka_2.9.2-0.8.1.1，估计你的版本要高吧
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=54981#respond' onclick='return addComment.moveForm("comment-54981", "54981", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
</li><!-- #comment-## -->
```
- 
![](http://shiyanjun.cn/wp-content/uploads/2014/11/User.png)马跃
```
<div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56250"><time pubdate datetime="2015-10-15T10:35:50+00:00">
                2015-10-15 10:35:50                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>您好，请教个问题，我也正在学习这个例子，发现解析json的时候，用到<br />
```
26 val data = JSONObject.fromObject(line._2) 这个语句
但是发现net.sf.json-lib 这个包下 没有相关的伴生对象，请问您是怎么调用的？
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56250#respond' onclick='return addComment.moveForm("comment-56250", "56250", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
</li><!-- #comment-## -->
<li class="comment even thread-even depth-1" id="li-comment-56404">
    <article id="comment-56404" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">guo</cite>             </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56404"><time pubdate datetime="2015-10-28T10:05:37+00:00">
                2015-10-28 10:05:37                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>想请教一下，为什么我的sparkStreaming在接收kafka消息后，不能及时输出，时间片设置成 2秒，至少5个时间片（也就是10几秒）以后才输出，延迟非常厉害，望不啬赐教，程序非常简单。如下：<br />
```
object KafkaStreaming{
    def main(args: Array[String]): Unit = {
        var conf = new SparkConf
        conf.setSparkHome(“/usr/local/spark-1.3.0″)
        conf.setMaster(“spark://hadoop3:7077″)
        conf.setAppName(“KafkaSpark”)
        conf.set(“SPARK_EXECUTOR_MEMORY”, “2g”)   
        var sc = new SparkContext(conf);
        var ssc = new StreamingContext(sc,Seconds(args(0).toInt))
        val kafkaParams = Map(“metadata.broker.list”->”hadoop2:9092,hadoop3:9092,hadoop4:9092″)
        val topics = Set(“topic1″)
        var kafkaStream = KafkaUtils.createDirectStream[String, String, StringDecoder,StringDecoder](ssc, kafkaParams, topics)
        var flatMapStream = kafkaStream.flatMap(_._2)
        flatMapStream.print()
        //flatMapStream.persist()
        ssc.start()
        ssc.awaitTermination()
    }
}
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56404#respond' onclick='return addComment.moveForm("comment-56404", "56404", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
<ul class="children">
<li class="comment byuser comment-author-yanjun bypostauthor odd alt depth-2" id="li-comment-56406">
    <article id="comment-56406" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/03/alien_gray.png" width="40" height="40" alt="Yanjun" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo" />                   <cite class="fn"><a href='http://www.shiyanjun.cn' rel='external nofollow' class='url'>Yanjun</a></cite>                </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56406"><time pubdate datetime="2015-10-28T10:29:44+00:00">
                2015-10-28 10:29:44                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>不知道你2秒处理的数据量有多大？<br />
```
确保2秒之内能够处理那些数据，并且延迟很小，还有可能和你的网络、集群性能有关。
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56406#respond' onclick='return addComment.moveForm("comment-56406", "56406", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
<ul class="children">
<li class="comment even depth-3" id="li-comment-56408">
    <article id="comment-56408" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">guo</cite>             </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56408"><time pubdate datetime="2015-10-28T11:05:12+00:00">
                2015-10-28 11:05:12                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>2秒就处理我自己手动打的一条消息而已，才几B。网络和集群性能应该可以排除，因为我试过用spark streaming接收从socket发送的数据是没有问题的，只是在接收 kafka消息的时候，才出现延迟这样的问题，我代码写得有问题吗？</p>
```
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56408#respond' onclick='return addComment.moveForm("comment-56408", "56408", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
</li><!-- #comment-## -->
<li class="comment odd alt depth-3" id="li-comment-56409">
    <article id="comment-56409" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">guo</cite>             </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56409"><time pubdate datetime="2015-10-28T13:20:14+00:00">
                2015-10-28 13:20:14                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>经过我测试之后，发现是因为使用了：<br />
```
KafkaUtils.createDirectStream()这个API导致，
我换成：
KafkaUtils.createStream()
就不存在这个问题了，你知道是为什么吗？
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56409#respond' onclick='return addComment.moveForm("comment-56409", "56409", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
<ul class="children">
<li class="comment byuser comment-author-yanjun bypostauthor even depth-4" id="li-comment-56410">
    <article id="comment-56410" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/03/alien_gray.png" width="40" height="40" alt="Yanjun" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo" />                   <cite class="fn"><a href='http://www.shiyanjun.cn' rel='external nofollow' class='url'>Yanjun</a></cite>                </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56410"><time pubdate datetime="2015-10-28T17:31:34+00:00">
                2015-10-28 17:31:34                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>它们内部实现机制不一样，我怀疑使用createDirectStream内部会首先缓存数据，通过判断：当数据够一个batch，或者等到设定的时间，都会将数据作为一个batch去处理。你可以看看源码实现。</p>
```
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56410#respond' onclick='return addComment.moveForm("comment-56410", "56410", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
<ul class="children">
<li class="comment odd alt depth-5" id="li-comment-56419">
    <article id="comment-56419" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">guo</cite>             </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56419"><time pubdate datetime="2015-10-30T13:33:53+00:00">
                2015-10-30 13:33:53                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>还想请教您一个问题，困扰了我好几天，希望能得到你的回复：<br />
```
上面有一句代码：
var flatMapStream = kafkaStream.flatMap(_._2)
我现在改成：
var i = 0
var flatMapStream = kafkaStream.flatMap(line => {
        i += 1                   //i自增
        println(line)            //问题1：这句代码一直都没有打印
        var x = line._2 + i.toString()       //将原有的字符串加上 i 后再进行split
        x.split(” “)            //问题2：结果是i 只自增了一次 ，也就是说，i一直都是1,i+=1只执行了一次，按理说，实时执行，应该会一直自增才对呀
   }
)
上面有两个问题，对应的代码应该都是实时执行的，为什么只执行了一次？
但是，x.split(” “)这句代码却是实时执行的，既然在同一个匿名函数中，应该都会实时执行才对呀？
还望告知原理，spark streaming在我代码提交之后，是如何实时执行的，为什么我自己添加的代码不会实时执行，但是它正常的RDD transfer过程却又能实时执行。。。。
困扰了好久，希望能得到你的解答。谢谢
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56419#respond' onclick='return addComment.moveForm("comment-56419", "56419", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
</li><!-- #comment-## -->
<li class="comment even depth-5" id="li-comment-56420">
    <article id="comment-56420" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">guo</cite>             </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56420"><time pubdate datetime="2015-10-30T13:41:25+00:00">
                2015-10-30 13:41:25                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>如果说我的代码放在flatMap中不能实时执行，那么为什么你的往redis写数据却可以实时执行呢？（你的代码我有点看不懂，所以没有去深究）</p>
```
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56420#respond' onclick='return addComment.moveForm("comment-56420", "56420", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
</li><!-- #comment-## -->
<li class="comment odd alt depth-5" id="li-comment-56422">
    <article id="comment-56422" class="comment">
        <footer class="clearfix comment-head">
            <div class="comment-author vcard">
                <img src="http://shiyanjun.cn/wp-content/uploads/2014/11/User.png" width="40" height="40" alt="" class="avatar avatar-40 wp-user-avatar wp-user-avatar-40 alignnone photo avatar-default" />                    <cite class="fn">guo</cite>             </div><!-- .comment-author .vcard -->
            <div class="comment-meta commentmetadata">
                <a href="http://shiyanjun.cn/archives/1097.html#comment-56422"><time pubdate datetime="2015-10-31T11:32:20+00:00">
                2015-10-31 11:32:20                 </time></a>
                                </div><!-- .comment-meta .commentmetadata -->
        </footer>
        <div class="comment-content"><p>已经明白了，谢谢啦</p>
```
```
<div class="reply">
            <a class='comment-reply-link' href='/archives/1097.html?replytocom=56422#respond' onclick='return addComment.moveForm("comment-56422", "56422", "respond", "1097")'>回复</a>          </div><!-- .reply -->
    </article><!-- #comment-## -->
</li><!-- #comment-## -->
```
```
<div id="respond" class="comment-respond">
            <h3 id="reply-title" class="comment-reply-title">发表评论 <small><a rel="nofollow" id="cancel-comment-reply-link" href="/archives/1097.html#respond" style="display:none;">取消回复</a></small></h3>
                                <form action="http://shiyanjun.cn/wp-comments-post.php" method="post" id="commentform" class="comment-form">
                                                                        <p class="comment-notes">电子邮件地址不会被公开。 必填项已用<span class="required">*</span>标注</p>                            <p class="comment-form-author"><label for="author">姓名 <span class="required">*</span></label> <input id="author" name="author" type="text" value="" size="30" aria-required='true' /></p>
```
电子邮件 *
站点
```
<p class="comment-form-comment"><label for="comment">评论</label> <textarea id="comment" name="comment" cols="45" rows="8" aria-required="true"></textarea></p>                       <p class="form-allowed-tags">您可以使用这些<abbr title="HyperText Markup Language">HTML</abbr>标签和属性： <code><a href="" title=""> <abbr title=""> <acronym title=""> <b> <blockquote cite=""> <cite> <code> <del datetime=""> <em> <i> <q cite=""> <strike> <strong> </code></p>                     <p class="form-submit">
                        <input name="submit" type="submit" id="submit" value="发表评论" />
                        <input type='hidden' name='comment_post_ID' value='1097' id='comment_post_ID' />
```

```
</div> <!-- end #main -->
            <div id="sidebar" class="widget-area col300" role="complementary">
                <div class="widget-wrap"><aside id="recent-posts-2" class="widget widget_recent_entries">       <h2 class="widget-title">最新文章</h2>      <ul>
                <li>
            <a href="http://shiyanjun.cn/archives/1228.html">MapReduce V1：JobTracker端Job/Task数据结构</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1210.html">MapReduce V1：Job提交流程之JobTracker端分析</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1200.html">MapReduce V1：Job提交流程之JobClient端分析</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1186.html">Akka Cluster原理与应用</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1178.html">Akka入门编程实践</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1168.html">Akka框架基本要点介绍</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1154.html">Apache Pig简介与实践</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1119.html">Hadoop YARN架构设计要点</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1113.html">Spark-1.3.1与Hive整合实现查询分析</a>
                    </li>
                <li>
            <a href="http://shiyanjun.cn/archives/1097.html">Kafka+Spark Streaming+Redis实时计算整合实践</a>
                    </li>
            </ul>
    </aside></div><div class="widget-wrap"><aside id="categories-2" class="widget widget_categories"><h2 class="widget-title">文章分类</h2>     <ul>
<li class="cat-item cat-item-8"><a href="http://shiyanjun.cn/archives/category/opensource" title="查看开源技术下的所有文章">开源技术</a> (72)
```
- [Akka](http://shiyanjun.cn/archives/category/opensource/akka) (3)
- [Crunch](http://shiyanjun.cn/archives/category/opensource/crunch) (1)
- [Dubbo](http://shiyanjun.cn/archives/category/opensource/dubbo) (4)
- [Flume](http://shiyanjun.cn/archives/category/opensource/flume) (1)
- [Hadoop](http://shiyanjun.cn/archives/category/opensource/hadoop) (19)
- [Hive](http://shiyanjun.cn/archives/category/opensource/hive) (2)
- [Hue](http://shiyanjun.cn/archives/category/opensource/hue) (1)
- [Impala](http://shiyanjun.cn/archives/category/opensource/impala) (2)
- [Maven](http://shiyanjun.cn/archives/category/opensource/maven) (1)
- [Mina](http://shiyanjun.cn/archives/category/opensource/mina) (6)
- [Mybatis](http://shiyanjun.cn/archives/category/opensource/mybatis) (1)
- [Oozie](http://shiyanjun.cn/archives/category/opensource/oozie) (4)
- [Pig](http://shiyanjun.cn/archives/category/opensource/pig) (1)
- [Solr](http://shiyanjun.cn/archives/category/opensource/solr) (5)
- [Spark](http://shiyanjun.cn/archives/category/opensource/spark) (6)
- [Sqoop](http://shiyanjun.cn/archives/category/opensource/sqoop) (3)
- [Storm](http://shiyanjun.cn/archives/category/opensource/storm) (4)
- [Thrift](http://shiyanjun.cn/archives/category/opensource/thrift) (2)
- [Web Services](http://shiyanjun.cn/archives/category/opensource/webservices) (1)
- [ZooKeeper](http://shiyanjun.cn/archives/category/opensource/zookeeper) (3)
- [数据库](http://shiyanjun.cn/archives/category/database) (5)
- [NoSQL数据库](http://shiyanjun.cn/archives/category/database/nosql) (4)
    - [HBase](http://shiyanjun.cn/archives/category/database/nosql/hbase) (3)
- [Redis](http://shiyanjun.cn/archives/category/database/nosql/redis) (1)
- [关系数据库](http://shiyanjun.cn/archives/category/database/rdb) (1)
    - [MySQL](http://shiyanjun.cn/archives/category/database/rdb/mysql) (1)
- [数据挖掘](http://shiyanjun.cn/archives/category/datamining) (5) 
- [服务器](http://shiyanjun.cn/archives/category/server) (4)
- [Ganglia](http://shiyanjun.cn/archives/category/server/ganglia) (1)
- [HAProxy](http://shiyanjun.cn/archives/category/server/haproxy) (1)
- [Memcached](http://shiyanjun.cn/archives/category/server/memcached) (1)
- [Nginx](http://shiyanjun.cn/archives/category/server/nginx) (1)
- [算法](http://shiyanjun.cn/archives/category/algorithm) (8) 
- [编程语言](http://shiyanjun.cn/archives/category/programming) (4)
- [Java](http://shiyanjun.cn/archives/category/programming/java) (2)
- [Node.js](http://shiyanjun.cn/archives/category/programming/nodejs) (1)
- [PHP](http://shiyanjun.cn/archives/category/programming/php) (1)
## 文章存档
- [http://shiyanjun.cn/archives/date/2015/10](http://shiyanjun.cn/archives/date/2015/10)‘>2015年十月 (2)
- [http://shiyanjun.cn/archives/date/2015/09](http://shiyanjun.cn/archives/date/2015/09)‘>2015年九月 (1)
- [http://shiyanjun.cn/archives/date/2015/08](http://shiyanjun.cn/archives/date/2015/08)‘>2015年八月 (4)
- [http://shiyanjun.cn/archives/date/2015/06](http://shiyanjun.cn/archives/date/2015/06)‘>2015年六月 (1)
- [http://shiyanjun.cn/archives/date/2015/05](http://shiyanjun.cn/archives/date/2015/05)‘>2015年五月 (1)
- [http://shiyanjun.cn/archives/date/2015/04](http://shiyanjun.cn/archives/date/2015/04)‘>2015年四月 (1)
- [http://shiyanjun.cn/archives/date/2015/03](http://shiyanjun.cn/archives/date/2015/03)‘>2015年三月 (2)
- [http://shiyanjun.cn/archives/date/2015/01](http://shiyanjun.cn/archives/date/2015/01)‘>2015年一月 (1)
- [http://shiyanjun.cn/archives/date/2014/12](http://shiyanjun.cn/archives/date/2014/12)‘>2014年十二月 (2)
- [http://shiyanjun.cn/archives/date/2014/11](http://shiyanjun.cn/archives/date/2014/11)‘>2014年十一月 (1)
- [http://shiyanjun.cn/archives/date/2014/10](http://shiyanjun.cn/archives/date/2014/10)‘>2014年十月 (3)
- [http://shiyanjun.cn/archives/date/2014/09](http://shiyanjun.cn/archives/date/2014/09)‘>2014年九月 (1)
- [http://shiyanjun.cn/archives/date/2014/08](http://shiyanjun.cn/archives/date/2014/08)‘>2014年八月 (1)
- [http://shiyanjun.cn/archives/date/2014/07](http://shiyanjun.cn/archives/date/2014/07)‘>2014年七月 (2)
- [http://shiyanjun.cn/archives/date/2014/06](http://shiyanjun.cn/archives/date/2014/06)‘>2014年六月 (2)
- [http://shiyanjun.cn/archives/date/2014/05](http://shiyanjun.cn/archives/date/2014/05)‘>2014年五月 (2)
- [http://shiyanjun.cn/archives/date/2014/04](http://shiyanjun.cn/archives/date/2014/04)‘>2014年四月 (1)
- [http://shiyanjun.cn/archives/date/2014/03](http://shiyanjun.cn/archives/date/2014/03)‘>2014年三月 (14)
- [http://shiyanjun.cn/archives/date/2014/02](http://shiyanjun.cn/archives/date/2014/02)‘>2014年二月 (5)
- [http://shiyanjun.cn/archives/date/2013/12](http://shiyanjun.cn/archives/date/2013/12)‘>2013年十二月 (3)
- [http://shiyanjun.cn/archives/date/2013/11](http://shiyanjun.cn/archives/date/2013/11)‘>2013年十一月 (5)
- [http://shiyanjun.cn/archives/date/2013/10](http://shiyanjun.cn/archives/date/2013/10)‘>2013年十月 (6)
- [http://shiyanjun.cn/archives/date/2013/09](http://shiyanjun.cn/archives/date/2013/09)‘>2013年九月 (13)
- [http://shiyanjun.cn/archives/date/2013/08](http://shiyanjun.cn/archives/date/2013/08)‘>2013年八月 (21)
## 全站标签
[http://shiyanjun.cn/archives/tag/solrcloud](http://shiyanjun.cn/archives/tag/solrcloud)’ class=’tag-link-27’ title=’1’ style=’font-size: 12pt; color: #00a650’>SolrCloud 
[http://shiyanjun.cn/archives/tag/hadoop2](http://shiyanjun.cn/archives/tag/hadoop2)’ class=’tag-link-25’ title=’3’ style=’font-size: 16.9090909091pt; color: #bc8cbf’>Hadoop2 
[http://shiyanjun.cn/archives/tag/nginx](http://shiyanjun.cn/archives/tag/nginx)’ class=’tag-link-17’ title=’1’ style=’font-size: 12pt; color: #00a650’>Nginx 
[http://shiyanjun.cn/archives/tag/mysql](http://shiyanjun.cn/archives/tag/mysql)’ class=’tag-link-49’ title=’1’ style=’font-size: 12pt; color: #00a650’>MySQL 
[http://shiyanjun.cn/archives/tag/mapreduce](http://shiyanjun.cn/archives/tag/mapreduce)’ class=’tag-link-34’ title=’9’ style=’font-size: 23.4545454545pt; color: #8fc63d’>MapReduce 
[http://shiyanjun.cn/archives/tag/thrift](http://shiyanjun.cn/archives/tag/thrift)’ class=’tag-link-22’ title=’4’ style=’font-size: 18.5454545455pt; color: #0f0’>Thrift 
[http://shiyanjun.cn/archives/tag/hwi](http://shiyanjun.cn/archives/tag/hwi)’ class=’tag-link-77’ title=’1’ style=’font-size: 12pt; color: #00a650’>HWI 
[http://shiyanjun.cn/archives/tag/hdfs](http://shiyanjun.cn/archives/tag/hdfs)’ class=’tag-link-72’ title=’5’ style=’font-size: 19.8545454545pt; color: #2f3192’>HDFS 
[http://shiyanjun.cn/archives/tag/hadoop-1-2-1](http://shiyanjun.cn/archives/tag/hadoop-1-2-1)’ class=’tag-link-80’ title=’7’ style=’font-size: 21.8181818182pt; color: #f0f’>Hadoop-1.2.1 
[http://shiyanjun.cn/archives/tag/ictclas](http://shiyanjun.cn/archives/tag/ictclas)’ class=’tag-link-63’ title=’1’ style=’font-size: 12pt; color: #00a650’>ICTCLAS 
[http://shiyanjun.cn/archives/tag/k-means](http://shiyanjun.cn/archives/tag/k-means)’ class=’tag-link-64’ title=’1’ style=’font-size: 12pt; color: #00a650’>K-means 
[http://shiyanjun.cn/archives/tag/node-js](http://shiyanjun.cn/archives/tag/node-js)’ class=’tag-link-75’ title=’1’ style=’font-size: 12pt; color: #00a650’>Node.js 
[http://shiyanjun.cn/archives/tag/jax-ws](http://shiyanjun.cn/archives/tag/jax-ws)’ class=’tag-link-39’ title=’1’ style=’font-size: 12pt; color: #00a650’>JAX-WS 
[http://shiyanjun.cn/archives/tag/mybatis](http://shiyanjun.cn/archives/tag/mybatis)’ class=’tag-link-73’ title=’1’ style=’font-size: 12pt; color: #00a650’>Mybatis 
[http://shiyanjun.cn/archives/tag/storm](http://shiyanjun.cn/archives/tag/storm)’ class=’tag-link-35’ title=’4’ style=’font-size: 18.5454545455pt; color: #0f0’>Storm 
[http://shiyanjun.cn/archives/tag/spring](http://shiyanjun.cn/archives/tag/spring)’ class=’tag-link-46’ title=’1’ style=’font-size: 12pt; color: #00a650’>Spring 
[http://shiyanjun.cn/archives/tag/svm](http://shiyanjun.cn/archives/tag/svm)’ class=’tag-link-33’ title=’1’ style=’font-size: 12pt; color: #00a650’>SVM 
[http://shiyanjun.cn/archives/tag/hbase](http://shiyanjun.cn/archives/tag/hbase)’ class=’tag-link-10’ title=’4’ style=’font-size: 18.5454545455pt; color: #0f0’>HBase 
[http://shiyanjun.cn/archives/tag/hessian](http://shiyanjun.cn/archives/tag/hessian)’ class=’tag-link-41’ title=’1’ style=’font-size: 12pt; color: #00a650’>Hessian 
[http://shiyanjun.cn/archives/tag/solr](http://shiyanjun.cn/archives/tag/solr)’ class=’tag-link-11’ title=’5’ style=’font-size: 19.8545454545pt; color: #2f3192’>Solr 
[http://shiyanjun.cn/archives/tag/zookeeper](http://shiyanjun.cn/archives/tag/zookeeper)’ class=’tag-link-23’ title=’4’ style=’font-size: 18.5454545455pt; color: #0f0’>ZooKeeper 
[http://shiyanjun.cn/archives/tag/%e5%86%b3%e7%ad%96%e6%a0%91](http://shiyanjun.cn/archives/tag/%e5%86%b3%e7%ad%96%e6%a0%91)’ class=’tag-link-58’ title=’2’ style=’font-size: 14.9454545455pt; color: #f16522’>决策树 
[http://shiyanjun.cn/archives/tag/hadoop](http://shiyanjun.cn/archives/tag/hadoop)’ class=’tag-link-9’ title=’24’ style=’font-size: 30pt; color: #ee1d24’>Hadoop 
[http://shiyanjun.cn/archives/tag/tomcat](http://shiyanjun.cn/archives/tag/tomcat)’ class=’tag-link-28’ title=’1’ style=’font-size: 12pt; color: #00a650’>Tomcat 
[http://shiyanjun.cn/archives/tag/mina-2](http://shiyanjun.cn/archives/tag/mina-2)’ class=’tag-link-31’ title=’6’ style=’font-size: 21pt; color: #f0f’>Mina 
[http://shiyanjun.cn/archives/tag/jvm](http://shiyanjun.cn/archives/tag/jvm)’ class=’tag-link-42’ title=’1’ style=’font-size: 12pt; color: #00a650’>JVM 
[http://shiyanjun.cn/archives/tag/ganglia](http://shiyanjun.cn/archives/tag/ganglia)’ class=’tag-link-78’ title=’1’ style=’font-size: 12pt; color: #00a650’>Ganglia 
[http://shiyanjun.cn/archives/tag/spark](http://shiyanjun.cn/archives/tag/spark)’ class=’tag-link-68’ title=’6’ style=’font-size: 21pt; color: #f0f’>Spark 
[http://shiyanjun.cn/archives/tag/flume](http://shiyanjun.cn/archives/tag/flume)’ class=’tag-link-79’ title=’1’ style=’font-size: 12pt; color: #00a650’>Flume 
[http://shiyanjun.cn/archives/tag/web-services](http://shiyanjun.cn/archives/tag/web-services)’ class=’tag-link-40’ title=’1’ style=’font-size: 12pt; color: #00a650’>Web Services 
[http://shiyanjun.cn/archives/tag/id3](http://shiyanjun.cn/archives/tag/id3)’ class=’tag-link-57’ title=’1’ style=’font-size: 12pt; color: #00a650’>ID3 
[http://shiyanjun.cn/archives/tag/pig](http://shiyanjun.cn/archives/tag/pig)’ class=’tag-link-85’ title=’1’ style=’font-size: 12pt; color: #00a650’>Pig 
[http://shiyanjun.cn/archives/tag/sql](http://shiyanjun.cn/archives/tag/sql)’ class=’tag-link-24’ title=’1’ style=’font-size: 12pt; color: #00a650’>SQL 
[http://shiyanjun.cn/archives/tag/redis](http://shiyanjun.cn/archives/tag/redis)’ class=’tag-link-21’ title=’1’ style=’font-size: 12pt; color: #00a650’>Redis 
[http://shiyanjun.cn/archives/tag/php](http://shiyanjun.cn/archives/tag/php)’ class=’tag-link-5’ title=’1’ style=’font-size: 12pt; color: #00a650’>PHP 
[http://shiyanjun.cn/archives/tag/dubbo](http://shiyanjun.cn/archives/tag/dubbo)’ class=’tag-link-36’ title=’4’ style=’font-size: 18.5454545455pt; color: #0f0’>Dubbo 
[http://shiyanjun.cn/archives/tag/memcached](http://shiyanjun.cn/archives/tag/memcached)’ class=’tag-link-76’ title=’1’ style=’font-size: 12pt; color: #00a650’>Memcached 
[http://shiyanjun.cn/archives/tag/libsvm](http://shiyanjun.cn/archives/tag/libsvm)’ class=’tag-link-32’ title=’2’ style=’font-size: 14.9454545455pt; color: #f16522’>libsvm 
[http://shiyanjun.cn/archives/tag/yarn](http://shiyanjun.cn/archives/tag/yarn)’ class=’tag-link-65’ title=’2’ style=’font-size: 14.9454545455pt; color: #f16522’>YARN 
[http://shiyanjun.cn/archives/tag/haproxy](http://shiyanjun.cn/archives/tag/haproxy)’ class=’tag-link-37’ title=’1’ style=’font-size: 12pt; color: #00a650’>HAProxy 
[http://shiyanjun.cn/archives/tag/jregex](http://shiyanjun.cn/archives/tag/jregex)’ class=’tag-link-26’ title=’1’ style=’font-size: 12pt; color: #00a650’>JRegex 
[http://shiyanjun.cn/archives/tag/akka](http://shiyanjun.cn/archives/tag/akka)’ class=’tag-link-86’ title=’3’ style=’font-size: 16.9090909091pt; color: #bc8cbf’>Akka 
[http://shiyanjun.cn/archives/tag/maven](http://shiyanjun.cn/archives/tag/maven)’ class=’tag-link-29’ title=’1’ style=’font-size: 12pt; color: #00a650’>Maven 
[http://shiyanjun.cn/archives/tag/jstorm](http://shiyanjun.cn/archives/tag/jstorm)’ class=’tag-link-83’ title=’1’ style=’font-size: 12pt; color: #00a650’>JStorm 
[http://shiyanjun.cn/archives/tag/java](http://shiyanjun.cn/archives/tag/java)’ class=’tag-link-3’ title=’2’ style=’font-size: 14.9454545455pt; color: #f16522’>Java 
[http://shiyanjun.cn/archives/tag/sqoop](http://shiyanjun.cn/archives/tag/sqoop)’ class=’tag-link-67’ title=’3’ style=’font-size: 16.9090909091pt; color: #bc8cbf’>Sqoop 
[http://shiyanjun.cn/archives/tag/hue](http://shiyanjun.cn/archives/tag/hue)’ class=’tag-link-82’ title=’1’ style=’font-size: 12pt; color: #00a650’>Hue 
[http://shiyanjun.cn/archives/tag/oozie](http://shiyanjun.cn/archives/tag/oozie)’ class=’tag-link-15’ title=’4’ style=’font-size: 18.5454545455pt; color: #0f0’>Oozie 
[http://shiyanjun.cn/archives/tag/%e6%8e%92%e5%ba%8f](http://shiyanjun.cn/archives/tag/%e6%8e%92%e5%ba%8f)’ class=’tag-link-71’ title=’8’ style=’font-size: 22.6363636364pt; color: #16bcb4’>排序 
[http://shiyanjun.cn/archives/tag/crunch](http://shiyanjun.cn/archives/tag/crunch)’ class=’tag-link-84’ title=’1’ style=’font-size: 12pt; color: #00a650’>Crunch 
[http://shiyanjun.cn/archives/tag/hive](http://shiyanjun.cn/archives/tag/hive)’ class=’tag-link-62’ title=’3’ style=’font-size: 16.9090909091pt; color: #bc8cbf’>Hive 
[http://shiyanjun.cn/archives/tag/kafka](http://shiyanjun.cn/archives/tag/kafka)’ class=’tag-link-81’ title=’1’ style=’font-size: 12pt; color: #00a650’>Kafka 
[http://shiyanjun.cn/archives/tag/c4-5](http://shiyanjun.cn/archives/tag/c4-5)’ class=’tag-link-60’ title=’1’ style=’font-size: 12pt; color: #00a650’>C4.5 
[http://shiyanjun.cn/archives/tag/impala](http://shiyanjun.cn/archives/tag/impala)’ class=’tag-link-61’ title=’2’ style=’font-size: 14.9454545455pt; color: #f16522’>Impala
## 最新评论
- [utophia](http://shiyanjun.cn/archives/548.html#comment-56457): C:\Users\cccc\Desktop\svm-test >svm-train.exe -c 128 -g 0.5 train-scale.txt model.model Wrong input…
- [alice](http://shiyanjun.cn/archives/644.html#comment-56455): 已解决，包冲突。删除/usr/lib/oozie/libex ec/下面的文件： （1）jasper*.jar (2)jsp-api*.jar (3)servlet-api*.jar
- [alice](http://shiyanjun.cn/archives/644.html#comment-56452): 我也遇到了这么个问题。你解决了么？
- [博主帮个忙](http://shiyanjun.cn/archives/548.html#comment-56450): 2015-11-11 14:06:17.231 [main] INFO processor.config.Configuration – Load properties file:…
- [NeverMore](http://shiyanjun.cn/archives/548.html#comment-56449): 2015-11-11 00:23:40.436 [main] INFO processor.config.Configuration – Load properties file:…
- [lixiangguo](http://shiyanjun.cn/archives/548.html#comment-56448): processor.dataset.chi.term.vec tor.file=C:\Users\Shirdrn\D esktop\vector\terms.txt…
- [shenclus](http://shiyanjun.cn/archives/1228.html#comment-56447): 看着挺牛逼虽然看不懂
- [lixiangguo](http://shiyanjun.cn/archives/548.html#comment-56446): 请问语料库的样本和测试集放到什么目录，没有在代码中找到读取样 本文件目录的语句
- [rp](http://shiyanjun.cn/archives/744.html#comment-56443): 老实说，没有一部分批处理基础根本看不懂。当然文章还是翻译的不 错的，就是整个感觉spark像hadoop，学习理解成本有点 高，这一点就不如storm，当然，整合的模块什么的storm 也确实比不了。
- [小炮兵](http://shiyanjun.cn/archives/349.html#comment-56439): 我想问一下，既然能够调用远程接口，那么需要把provider 打成jar包导过来，才能使用啊！！ 那有什么意义？
## 站点管理
- [登录](http://shiyanjun.cn/wp-login.php)
- [文章RSS](http://shiyanjun.cn/feed)
- [评论RSS](http://shiyanjun.cn/comments/feed)
- [WordPress.org](https://cn.wordpress.org/)
![UPYUN](http://upfiles.b0.upaiyun.com/logo/180x90.png)
```
</div> <!-- end #content -->
<footer id="colophon" role="contentinfo">
    <div id="site-generator">
        COPYRIGHT © 2013-2018 <a href="http://www.shiyanjun.cn/" rel="designer"><strong>简单之美</strong></a>                   </div>
</footer><!-- #colophon -->
```
   #wp-auto-top{position:fixed;top:85%;right:50%;display:block;margin-right:-49%; z-index:9999;}
   #wp-auto-top-top,#wp-auto-top-comment,#wp-auto-top-bottom{background:url(http://shiyanjun.cn/wp-content/plugins/wp-auto-top/img/29.png) no-repeat;position:relative;cursor:pointer;height:25px;width:29px;margin:10px 0 0;}
   #wp-auto-top-comment{background-position:left -30px;height:32px;}#wp-auto-top-bottom{background-position:left -68px;}
   #wp-auto-top-comment:hover{background-position:right -30px;}
   #wp-auto-top-top:hover{background-position:right 0;}
   #wp-auto-top-bottom:hover{background-position:right -68px ;}
    (function(){
        var corecss = document.createElement('link');
        var themecss = document.createElement('link');
        var corecssurl = "http://shiyanjun.cn/wp-content/plugins/syntaxhighlighter/syntaxhighlighter2/styles/shCore.css?ver=2.1.364";
        if ( corecss.setAttribute ) {
                corecss.setAttribute( "rel", "stylesheet" );
                corecss.setAttribute( "type", "text/css" );
                corecss.setAttribute( "href", corecssurl );
        } else {
                corecss.rel = "stylesheet";
                corecss.href = corecssurl;
        }
        document.getElementsByTagName("head")[0].insertBefore( corecss, document.getElementById("syntaxhighlighteranchor") );
        var themecssurl = "http://shiyanjun.cn/wp-content/plugins/syntaxhighlighter/syntaxhighlighter2/styles/shThemeDefault.css?ver=2.1.364";
        if ( themecss.setAttribute ) {
                themecss.setAttribute( "rel", "stylesheet" );
                themecss.setAttribute( "type", "text/css" );
                themecss.setAttribute( "href", themecssurl );
        } else {
                themecss.rel = "stylesheet";
                themecss.href = themecssurl;
        }
        //document.getElementById("syntaxhighlighteranchor").appendChild(themecss);
        document.getElementsByTagName("head")[0].insertBefore( themecss, document.getElementById("syntaxhighlighteranchor") );
    })();
    SyntaxHighlighter.config.clipboardSwf = 'http://shiyanjun.cn/wp-content/plugins/syntaxhighlighter/syntaxhighlighter2/scripts/clipboard.swf';
    SyntaxHighlighter.config.strings.expandSource = 'show source';
    SyntaxHighlighter.config.strings.viewSource = '查看源代码';
    SyntaxHighlighter.config.strings.copyToClipboard = '复制到剪贴板';
    SyntaxHighlighter.config.strings.copyToClipboardConfirmation = '代码现在在你的剪贴板';
    SyntaxHighlighter.config.strings.print = '打印';
    SyntaxHighlighter.config.strings.help = '帮助';
    SyntaxHighlighter.config.strings.alert = 'SyntaxHighlighter\n\n';
    SyntaxHighlighter.config.strings.noBrush = '无法找到Brush：';
    SyntaxHighlighter.config.strings.brushNotHtmlScript = 'Brush不能设置 html-script选项';
    SyntaxHighlighter.defaults['pad-line-numbers'] = 1;
    SyntaxHighlighter.all();

