# spark submit 运行开发的应用程序 - BlueSky - CSDN博客
2015年11月21日 17:16:06[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：590
## sbt打包
经过一段时间的理解和学习，对sbt有点了了解，也成功安装成功，并打好了自己的程序jar包。
1、工程目录
保证sbt安装成功，可以console下命令：sbt sbt -version
```
sbt sbt-version
[info] Loading global plugins from /home/hadoop/.sbt/0.13/plugins
[info] Set current project to hadoop (in build file:/home/hadoop/)
[info] 0.13.9
```
建立wordcount文件夹
`mkdir wordcount`
wordcount目录下建立文件，如下文件结构（以下在wordcount/）：
```
.
./simple.sbt
./src
./src/main
./src/main/scala
./src/main/scala/SimpleApp.scala
```
其中simple.sbt内容为：
```
name := "Simple Project"
version := "1.0"
scalaVersion := "2.10.4"
libraryDependencies += "org.apache.spark" %% "spark-core" % "1.5.2"
```
编写SimpleApp.scala如下所示
```
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
object SimpleApp {
    def main(args: Array[String]) {
    val logFile = "words/a.txt" //words/a.txt为我的hdfs文件系统的文件hdfs：//Master/words/a.txt
    val conf = new SparkConf().setAppName("Simple Application")
    val sc = new SparkContext(conf)
    val logData = sc.textFile(logFile, 2)
    val numAs = logData.filter(line => line.contains("a")).count()
    val numBs = logData.filter(line => line.contains("b")).count()
    println("Lines with a: %s, Lines with b: %s".format(numAs, numBs))
    
    val word = logData.flatMap(_.split(' ')).map((_,1)).reduceByKey(_+_)
    word.saveAsTextFile("/user/wordresult.txt")
    word.collect().foreach(println)
  } 
}
```
然后回到wordcount/ 目录下，运行命令
```
sbt package
[info] Loading global plugins from /home/hadoop/.sbt/0.13/plugins
[info] Set current project to Simple Project (in build file:/home/hadoop/wordcount/)
[success] Total time: 3 s, completed Nov 21, 2015 12:53:34 AM
```
```bash
这时目录下target/scala-2.10/simple-project_2.10-1.0.jar生成完成
```
## 提交任务到集群
`/usr/local/spark/spark-1.5.1-bin-hadoop2.6/bin/spark-submit --master spark://192.168.203.148:7077 --name "Test" simple-project_2.10-1.0.jar`
Ok了，任务就会提交去执行，
```
15/11/20 23:34:45 INFO spark.SparkContext: Running Spark version 1.5.1
15/11/20 23:34:46 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable
15/11/20 23:34:46 INFO spark.SecurityManager: Changing view acls to: hadoop
15/11/20 23:34:46 INFO spark.SecurityManager: Changing modify acls to: hadoop
15/11/20 23:34:46 INFO spark.SecurityManager: SecurityManager: authentication disabled; ui acls disabled; users with view permissions: Set(hadoop); users with modify permissions: Set(hadoop)
15/11/20 23:34:47 INFO slf4j.Slf4jLogger: Slf4jLogger started
15/11/20 23:34:47 INFO Remoting: Starting remoting
15/11/20 23:34:47 INFO Remoting: Remoting started; listening on addresses :[akka.tcp://sparkDriver@192.168.203.148:52546]
15/11/20 23:34:47 INFO util.Utils: Successfully started service 'sparkDriver' on port 52546.
15/11/20 23:34:47 INFO spark.SparkEnv: Registering MapOutputTracker
15/11/20 23:34:48 INFO spark.SparkEnv: Registering BlockManagerMaster
15/11/20 23:34:48 INFO storage.DiskBlockManager: Created local directory at /tmp/blockmgr-4506a450-379f-4c69-943b-1a27a10b5755
15/11/20 23:34:48 INFO storage.MemoryStore: MemoryStore started with capacity 534.5 MB
15/11/20 23:34:48 INFO spark.HttpFileServer: HTTP File server directory is /tmp/spark-ef706932-7cff-4ccf-9377-911855fe49ea/httpd-3942d94c-a00d-4f60-a574-5f20e0f365e4
15/11/20 23:34:48 INFO spark.HttpServer: Starting HTTP Server
15/11/20 23:34:48 INFO server.Server: jetty-8.y.z-SNAPSHOT
15/11/20 23:34:48 INFO server.AbstractConnector: Started SocketConnector@0.0.0.0:35862
```
......
```
15/11/21 00:06:01 INFO scheduler.TaskSetManager: Finished task 0.0 in stage 5.0 (TID 8) in 216 ms on 192.168.203.149 (1/2)
15/11/21 00:06:01 INFO scheduler.TaskSetManager: Finished task 1.0 in stage 5.0 (TID 9) in 217 ms on 192.168.203.150 (2/2)
15/11/21 00:06:01 INFO scheduler.TaskSchedulerImpl: Removed TaskSet 5.0, whose tasks have all completed, from pool 
15/11/21 00:06:01 INFO scheduler.DAGScheduler: ResultStage 5 (collect at SimpleApp.scala:17) finished in 0.217 s
15/11/21 00:06:01 INFO scheduler.DAGScheduler: Job 3 finished: collect at SimpleApp.scala:17, took 0.295072 s
(!=,1)
(textFile.flatMap(line,1)
(return,1)
(is,1)
(result.saveAsTextFile(args(2)),1)
(<output>"),1)
(org.apache.spark._,1)
(line.split("\\s+")).map(word,1)
(<master>,1)
(,41)
(main(args:Array[String]){,1)
(sc,1)
(new,1)
(<input>,1)
(=,3)
(textFile,1)
(WordCount,1)
(import,2)
((args.length,1)
(if,1)
(},3)
(println("usuage,1)
(3){,1)
(result,1)
(val,3)
(object,1)
(sc.textFile(args(1)),1)
(SparkContext(args(0),"WordCount",System.getenv("SPARK_HOME"),Seq(System.getenv("SPARK_TEST_JAR"))),1)
(=>,2)
({,1)
(def,1)
((word,1)).reduceByKey(_+_),1)
(org.test.WordCount,1)
(SparkContext._,1)
15/11/21 00:06:02 INFO spark.SparkContext: Invoking stop() from shutdown hook
15/11/21 00:06:02 INFO handler.ContextHandler: stopped o.s.j.s.ServletContextHandler{/metrics/json,null}
15/11/21 00:06:02 INFO handler.ContextHandler: stopped o.s.j.s.ServletContextHandler{/stages/stage/kill,null}
15/11/21 00:06:02 INFO handler.ContextHandler: stopped o.s.j.s.ServletContextHandler{/api,null}
```
上面可以看到，程序中统计结果打印出来了，任务完成。
等下再把kafka实例打包，测试下
