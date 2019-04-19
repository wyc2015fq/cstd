# Spark学习（二）---kafka+SparkStreaming的搭建与连接 - BlueSky - CSDN博客
2015年11月19日 21:18:31[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：10933
环境说明：
三台机器（Centos 6.5）：
Master 192.168.203.148
Slave1 192.168.203.149
Slave2 192.168.203.150
## **第一步：环境**
**spark环境配置：**
**spark安装很简单，可以参考网上教程，说下spark的配置：**
**主要是${SPARK_HOME}/conf/slaves中配置如下：**
```
Master
Slave1
Slave2
```
**${SPARK_HOME}/conf/spark-env.sh配置如下：**
```
export JAVA_HOME=/usr/local/java/jdk1.8.0_65
export SCALA_HOME=/usr/local/scala/scala-2.10.5
export SPARK_MASTER_IP=192.168.203.148
export SPARK_WORKER_MEMORY=1g
export HADOOP_CONF_DIR=/usr/local/hadoop/etc/hadoop
```
**zookeeper搭建**
**kafka搭建：**
参照_zhenwei博客：
http://blog.csdn.net/wang_zhenwei/article/details/48346327
http://blog.csdn.net/wang_zhenwei/article/details/48357131
安装完成，发送消息为：
![](https://img-blog.csdn.net/20151119210945387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151119211004478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## **第二步：jar包下载**
由于SparkStreaming读取kafka是依赖于其他jar包所以还要下载jar包对应分别为：
**spark-streaming-kafka_2.11-0.8.2.2.jar**
**下载地址：http://mvnrepository.com/artifact/org.apache.kafka/kafka_2.11/0.8.2.2**
kafka_2.10-0.8.1.jar,
**metrics-core-2.2.0.jar,**
**zkclient-0.4.jar（下面的这三个在上面网址中搜索可以找到）**
## **第三步：创建scala程序：**
创建myKafkaStreaming.scala:
```
import org.apache.spark.streaming.kafka._
import org.apache.spark.streaming.StreamingContext._
import org.apache.spark.streaming.kafka.KafkaUtils
val ssc = new StreamingContext(sc,Seconds(2))
val zkQuorum = "Slave3:2181,Slave1:2181,Master:2181"
val group = "test-consumer-group"
val topics = "kafkatopic"
val numThreads = 1
val topicMap = topics.split(",").map((_,numThreads.toInt)).toMap
val lineMap = KafkaUtils.createStream(ssc,zkQuorum,group,topicMap)
val lines = lineMap.map(_._2)
val words = lines.flatMap(_.split(" "))
val pair = words.map(x => (x,1))
val wordCounts = pair.reduceByKeyAndWindow(_ + _,_ - _,Minutes(2),Seconds(2),2)
wordCounts.print
ssc.checkpoint("/words/checkpoint")
ssc.start
ssc.awaitTermination
```
## **第四步：运行程序**
**两种方法：**
**第一种将上面的程序通过spark-shell形式提交运行；**
**${SPARK_HOME}/bin/spark-shell --jars kafka_2.10-0.8.2.1.jar,metrics-core-2.2.0.jar,spark-streaming-kafka_2.10-1.5.0.jar,zkclient-0.4.jar --master  spark://192.168.203.148:7077**
说明：我将对应的jar包拷到了${SPARK_HOME}中，--master这需要注意下 后面跟的ip是需要通过WebUi中查看到的：如左上角：spark://192.168.203.148:7077，如果写的不对可能会提示如下错误cluster.ClusterScheduler:
 Initial job has not accepted any resources; check your cluster UI to ensure that workers are registered and have sufficient memory
![](https://img-blog.csdn.net/20151119211201581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图中可以看到我已经提交的app任务，但是slave1节点掉了，可以看到任务运行ok了。
第二种方法：把上面的文件打成jar包通过spark-submit来进行提交，由于一直尝试用sbt但一直下载不成功所以暂时还没试，明天试试，或者换用maven来用。
## **四、新技能**
如果想在spark-shell交互环境中一下子输入很多条语句在一起执行而不是一条一条输入。
```
scala> ：paste
//put your code
//Then if you finish,ctrl+d组合键进行执行上面输入的所有代码
```
## 五、遇到的其他错误：
spark在提交任务时，出现如下错误：
# [spark WARNTaskSchedulerImpl:Initial job has not accepted any resources; check your cluster UI to](http://www.07net01.com/2015/03/803804.html)
15/03/26 22:29:36 WARN TaskSchedulerImpl: Initial job has not accepted any resources; check your cluster UI to ensure that workers are registered and have sufficient memory 15/03/26 22:29:51 WARN TaskSchedulerImpl: Initial job has not accepted any resources;
 check your cluster UI to ensure that workers are registered and have sufficient memory 15/03/26 22:30:06 WARN TaskSchedulerImpl: Initial job has not accepted any resources; check your cluster UI to ensure that workers are registered and have sufficient memory
 15/03/26 22:30:21 WARN TaskSchedulerImpl: Initial job has not accepted any resources; check your cluster UI to ensure that workers are registered and have sufficient memory
从警告信息上看，初始化job时没有获取到任何资源；提示检查集群，确保workers可以被注册并有足够的内存资源。
如上问题产生的原因是多方面的，可能原因如下：
1.因为提交任务的节点不能和spark工作节点交互，因为提交完任务后提交任务节点上会起一个进程，展示任务进度，大多端口为4044，工作节点需要反馈进度给该该端口，所以如果主机名或者IP在hosts中配置不正确。所以检查下主机名和ip是否配置正确
2.有可能是内存不足
  检查内存
conf.set("spark.executor.memory", "3000m")
Make sure to set `SPARK_LOCAL_IP` and`SPARK_MASTER_IP`.
  查看8080端口，确保一些workers保持Alive状态，确保 some cores 是可利用的。
最后自己通过WebUi来查看，竟然是所有节点都掉了，为什么会这样？待研究，于是重启了下集群Make it！
## 六、Spark配置说明
参照：http://blog.csdn.net/chenxingzhen001/article/details/11835399
一、环境变量spark-env.sh配置项
SCALA_HOME              #指向你的scala安装路径
MESOS_NATIVE_LIBRARY   #如果你要在Mesos上运行集群的话
SPARK_WORKER_MEMORY  #作业可使用的内存容量，默认格式1000M或者 2G (默认:  所有RAM去掉给操作系统用的1 GB);每个作业独立的内存空间由SPARK_MEM决定。
SPARK_JAVA_OPTS   #添加JVM选项。你可以通过-D来获取任何系统属性 eg: SPARK_JAVA_OPTS+="-Dspark.kryoserializer.buffer.mb=1024"
SPARK_MEM     #设置每个节点所能使用的内存总量。他们应该和JVM‘s -Xmx选项的格式保持一致（e.g.300m或1g）。注意：这个选项将很快被弃用支持系统属性spark.executor.memory，所以我们推荐将它使用在新代码中。
SPARK_DAEMON_MEMORY   #分配给Spark master和worker守护进程的内存空间(默认512M)
SPARK_DAEMON_JAVA_OPTS  #Spark master和worker守护进程的JVM选项（默认：none)
二、System Properties
Property NameDefaultMeaning
spark.executor.memory512mAmount of memory to use per executor process, in the same format as JVM memory strings (e.g. `512m`, `2g`).
spark.akka.frameSize10mMaximum message size to allow in "control plane" communication (for serialized tasks and task results), in MB. Increase this if your tasks need to send back large results to the driver (e.g. using collect() on a large dataset).
spark.default.parallelism8Default number of tasks to use for distributed shuffle operations (groupByKey, reduceByKey, etc) when not set by user.
spark.akka.frameSize: 控制Spark中通信消息的最大容量 （如 task 的输出结果），默认为10M。当处理大数据时，task 的输出可能会大于这个值，需要根据实际数据设置一个更高的值。如果是这个值不够大而产生的错误，可以从 worker的日志 中进行排查。通常 worker 上的任务失败后，master 的运行日志上出现”Lost TID: “的提示，可通过查看失败的 worker 的日志文件($SPARK_HOME/worker/下面的log文件) 中记录的任务的 Serialized
 size of result 是否超过10M来确定。
spark.default.parallelism: 控制Spark中的分布式shuffle过程默认使用的task数量，默认为8个。如果不做调整，数据量大时，就容易运行时间很长，甚至是出Exception，因为8个task无法handle那么多的数据。 注意这个值也不是说设置得越大越好。
spark.local.dir：Spark 运行时的临时目录，例如 map 的输出文件，保存在磁盘的 RDD 等都保存在这里。默认是 /tmp 这个目录，而一开始我们搭建的小集群上 /tmp 这个目录的空间只有2G，大数据量跑起来就出 Exception （”No space left on device”）了。

