
# Spark Streaming通过JDBC操作数据库 - anlcy - 博客园






# [Spark Streaming通过JDBC操作数据库](https://www.cnblogs.com/camilla/p/8350988.html)
本文记录了学习使用Spark Streaming通过JDBC操作数据库的过程，源数据从Kafka中读取。
Kafka从0.10版本提供了一种新的消费者API，和0.8不同，因此Spark Streaming也提供了两种API与之对应，其中spark-streaming-kafka-0-8支持Kafka 0.8.2.1以后的Broker；spark-streaming-kafka-0-10支持0.10.0以上Broker，处于实验阶段。两者的对比如下表所示。
|spark-streaming-kafka-0-8 | spark-streaming-kafka-0-10
---------------  |------------------------  |--------------------------
Broker Version    | 0.8.2.1 or higher        | 0.10.0 or higher
Api Stability     | Stable                   | Experimental
Language Support  | Scala, Java, Python      | Scala, Java
Receiver DStream  | Yes                      | No
Direct DStream    | Yes                      | Yes
SSL / TLS Support | No                       | Yes
Offset Commit Api | No                       | Yes
Dynamic Topic Subscription | No              | Yes
Spark Streaming集成Kafka的说明可以参考如下资料：
> spark-streaming-kafka-0-10
> [http://spark.apache.org/docs/latest/streaming-kafka-0-10-integration.html](https://link.jianshu.com/?t=http://spark.apache.org/docs/latest/streaming-kafka-0-10-integration.html)

> spark-streaming-kafka-0-8
> [http://spark.apache.org/docs/latest/streaming-kafka-0-8-integration.html](https://link.jianshu.com/?t=http://spark.apache.org/docs/latest/streaming-kafka-0-8-integration.html)

## 1.2 运行组件
本文所写的例子基于JDK1.8和Scala 2.11，运行依赖组件的情况如下表所示。
|组件|部署方式|IP地址|操作系统|
|---|---|---|---|
|spark-2.0.1|伪分布式|192.168.1.91|CentOS 7.1|
|Kafka-0.10.0.1|伪分布式|192.168.1.90|CentOS 7.1|
|postgresql-9.4.5|单机|192.168.1.213|CentOS 7.1|
数据库中创建了一张名为kafka_message的表，共有三个字段，都是varchar类型。
`CREATE TABLE kafka_message (
    timeseq varchar(16),
    thread varchar(32),
    message varchar(255)
);`
# 2. 代码
## 2.1 pom.xml
依赖的lib如下。
`<dependencies>
    <dependency>
        <groupId>org.apache.spark</groupId>
        <artifactId>spark-streaming_2.11</artifactId>
        <version>2.0.1</version>
    </dependency>
    <dependency>
        <groupId>org.apache.spark</groupId>
        <artifactId>spark-streaming-kafka-0-10_2.11</artifactId>
        <version>2.0.1</version>
    </dependency>
    <dependency>
        <groupId>com.jolbox</groupId>
        <artifactId>bonecp</artifactId>
        <version>0.8.0.RELEASE</version>
    </dependency>
    <dependency>
        <groupId>postgresql</groupId>
        <artifactId>postgresql</artifactId>
        <version>9.1-901-1.jdbc4</version>
    </dependency>
</dependencies>`
## 2.2 数据库连接池
`import java.sql.Connection
import com.jolbox.bonecp.{BoneCP, BoneCPConfig}
import org.slf4j.LoggerFactory
 /**
  *  数据库连接池，使用了BoneCP
  */
object ConnectionPool {
  val logger = LoggerFactory.getLogger(this.getClass)
  //连接池配置
  private val connectionPool: Option[BoneCP] = {
    try{
      Class.forName("org.postgresql.Driver")
      val config = new BoneCPConfig()
      config.setJdbcUrl("jdbc:postgresql://192.168.1.213/yourdb")
      config.setUsername("postgres")
      config.setPassword("******")
      config.setLazyInit(true)
      config.setMinConnectionsPerPartition(3)
      config.setMaxConnectionsPerPartition(5)
      config.setPartitionCount(5)
      config.setCloseConnectionWatch(true)
      config.setLogStatementsEnabled(false)
      Some(new BoneCP(config))
    }catch {
      case exception: Exception =>
        logger.warn("Create Connection Error: \n" + exception.printStackTrace())
        None
    }
  }
  // 获取数据库连接
  def getConnection: Option[Connection] = {
    connectionPool match {
      case Some(pool) => Some(pool.getConnection)
      case None => None
    }
  }
  // 释放数据库连接
  def closeConnection(connection:Connection): Unit = {
    if(!connection.isClosed) {
      connection.close()
    }
  }`
## 2.3 Kafka -> Spark-Streaming -> JDBC
Spark Streaming从Kafka中读取数据，并把数据写入数据库。SPark Streaming编程的基本顺序是：
创建Spark Streaming上下文
从数据源接口创建DStream
在DStream上做转换（Transformations）
指定计算结果存储的位置
启动计算
代码如下，详见注释。
`import java.sql.Connection
import org.apache.kafka.common.serialization.StringDeserializer
import org.apache.spark.SparkConf
import org.apache.spark.streaming.kafka010.ConsumerStrategies._
import org.apache.spark.streaming.kafka010.KafkaUtils
import org.apache.spark.streaming.kafka010.LocationStrategies._
import org.apache.spark.streaming.{Seconds, StreamingContext}
import org.slf4j.LoggerFactory
/**
  * 从Kafka中读取数据，并把数据写入数据库。
  */
object KafkaToDB {
  val logger = LoggerFactory.getLogger(this.getClass)
  def main(args: Array[String]): Unit = {
    // 参数校验
    if (args.length < 2) {
      System.err.println(
        s"""
           |Usage: KafkaToDB <brokers> <topics>
           |  <brokers> broker列表，至少1个，以英文逗号分割
           |  <topics> topic列表，至少一个，以英文逗号分割
           |""".stripMargin)
      System.exit(1)
    }
    // 处理参数
    val Array(brokers, topics) = args
    val topicSet: Set[String] = topics.split(",").toSet 
    val kafkaParams: Map[String, Object] = Map[String, Object](
      "bootstrap.servers" -> brokers,
      "key.deserializer" -> classOf[StringDeserializer],
      "value.deserializer" -> classOf[StringDeserializer],
      "group.id" -> "example",
      "auto.offset.reset" -> "latest",
      "enable.auto.commit" -> (false: java.lang.Boolean)
    )
    // 1.创建上下文，以每1秒间隔的数据作为一个批次
    val sparkConf = new SparkConf().setAppName("KafkaToDB")
    val streamingContext = new StreamingContext(sparkConf, Seconds(1))
    // 2.创建输入流，获取数据。流操作基于DStream，InputDStream继承于DStream
    val stream = KafkaUtils.createDirectStream[String, String](
      streamingContext,
      PreferConsistent,
      Subscribe[String, String](topicSet, kafkaParams)
    )
    // 3. DStream上的转换操作
    // 取消息中的value数据，以英文逗号分割，并转成Tuple3
    val values = stream.map(_.value.split(","))
      .filter(x => x.length == 3)
      .map(x => new Tuple3[String, String, String](x(0), x(1), x(2)))
    // 输入前10条到控制台，方便调试
    values.print()
    // 4.同foreachRDD保存到数据库
    val sql = "insert into kafka_message(timeseq, thread, message) values (?,?,?)"
    values.foreachRDD(rdd => {
      val count = rdd.count()
      println("-----------------count:" + count)
      if (count > 0) {
        rdd.foreachPartition(partitionOfRecords => {
          val conn = ConnectionPool.getConnection.orNull
          if (conn != null) {
            partitionOfRecords.foreach(data => insert(conn, sql, data))
            ConnectionPool.closeConnection(conn)
          }
        })
      }
    })
    // 5. 启动计算
    streamingContext.start() 
    streamingContext.awaitTermination() // 等待中断结束计算    
  }

  /**
    * 保存数据到数据库
    *
    * @param conn 数据库连接
    * @param sql  prepared statement sql
    * @param data 要保存的数据，Tuple3结构
    */
  def insert(conn: Connection, sql: String, data: (String, String, String)): Unit = {
    try {
      val ps = conn.prepareStatement(sql)
      ps.setString(1, data._1)
      ps.setString(2, data._2)
      ps.setString(3, data._3)
      ps.executeUpdate()
      ps.close()
    } catch {
      case e: Exception =>
        logger.error("Error in execution of insert. " + e.getMessage)
    }
  }
}`
# 3. 任务运行
## 3.1 数据库驱动配置
由于本次程序运行环境是spark standalone 的伪分布式，指定SPARK_CLASSPATH时，相当于同时指定了driver和executor的classpath。
编辑spark-env.sh。
`vi $SPARK_HOME/conf/spark-env.sh`输入以下内容，注意把postgresql驱动包放在对应的位置。
`export SPARK_CLASSPATH=$SPARK_CLASSPATH:/opt/jobs/postgresql-9.1-901-1.jdbc4.jar`实际上，Spark已经不推荐使用SPARK_CLASSPATH参数了，启动的时候，我们会发现如下的日志，提示我们用`spark.executor.extraClassPath`和`spark.driver.extraClassPath`来代替。如果是spark local模式，只需指定`spark.driver.extraClassPath`即可。
日志如下。
`16/10/21 15:15:33 WARN SparkConf: 
SPARK_CLASSPATH was detected (set to ':/opt/jobs/postgresql-9.1-901-1.jdbc4.jar').
This is deprecated in Spark 1.0+.
Please instead use:
 - ./spark-submit with --driver-class-path to augment the driver classpath
 - spark.executor.extraClassPath to augment the executor classpath
        
16/10/21 15:15:33 WARN SparkConf: Setting 'spark.executor.extraClassPath' to ':/opt/jobs/
postgresql-9.1-901-1.jdbc4.jar' as a work-around.
16/10/21 15:15:33 WARN SparkConf: Setting 'spark.driver.extraClassPath' to ':/opt/jobs/
postgresql-9.1-901-1.jdbc4.jar' as a work-around.`
## 3.2 启动任务
`$SPARK_HOME/bin/spark-submit \ 
--master spark://192.168.1.91:7077
--class com.xxx.streaming.KafkaToDB \
 spark-streaming-demo.jar 192.168.1.90:9092 my-topic`在192.168.1.90上，通过kafka命令发送消息，命令如下：
`bin/kafka-console-producer.sh --broker-list 192.168.1.90:9092 --topic mytopic`消息如下：
`1475589918658    thread-15    msg-0
1475589920177    thread-15    msg-1`之后，我们可以在控制台看到消息被输出，也可以在数据库中查询到这些数据。
# 4.Spark程序找不到JDBC驱动的问题
控制台曾经报出找不到JDBC驱动的异常，日志如下。
`java.sql.SQLException: No suitable driver found for jdbc:……`该问题可以用前文所述的数据库驱动配置办法解决。在这里推荐一篇博客，对这个问题给出的很好的解释。
[http://www.codexiu.cn/spark/blog/12672/](https://link.jianshu.com/?t=http://www.codexiu.cn/spark/blog/12672/)
博客来源：https://www.jianshu.com/p/a73c0c95d2fe





