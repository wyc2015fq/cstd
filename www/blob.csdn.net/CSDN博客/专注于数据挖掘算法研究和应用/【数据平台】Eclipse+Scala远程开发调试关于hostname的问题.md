# 【数据平台】Eclipse+Scala远程开发调试关于hostname的问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月13日 09:43:05[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：288








1、代码：



```
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext

object wc {
  def main(args : Array[String]){
    System.setProperty("hadoop.home.dir", "D:\\dev\\hadoop-2.7.4");
    val conf = new SparkConf()//创建SparkConf对象
    conf.setAppName("WordCount").setMaster("spark://master:7077")//设置应用程序的名称，在程序运行的监控界面可以看到名称
    //conf.setAppName("WordCount").setMaster("local")//运行模式
    //创建SparkContext对象，通过传入SparkConf实例来定制Spark运行的具体参数和配置信息
    val sc = new SparkContext(conf)
    /*根据具体的数据来源（HDFS、HBase、Local FS、DB、S3等）通过SparkContext来创建RDD
       * RDD的创建基本有三种方式：根据外部的数据来源（例如HDFS）、根据Scala集合、由其它的RDD操作
       * 数据会被RDD划分成为一系列的Partitions，分配到每个Partition的数据属于一个Task的处理范畴
       */
    //读取本地文件并设置为一个Partion
    val lines = sc.textFile("hdfs://master:9000/tmp/README.md",1)//hdfs路径
    //对每一行的字符串进行单词拆分并把所有行的拆分结果通过flat合并成为一个大的单词集合
    val words = lines.flatMap{line => line.split(" ")}
    val pairs = words.map{word => (word,1)}
    //对相同的Key，进行Value的累计（包括Local和Reducer级别同时Reduce）
    val wordCounts = pairs.reduceByKey(_+_)
    //在命令行中打印该结果
    wordCounts.foreach(wordNumberPair => println(wordNumberPair._1 + " : " +wordNumberPair._2))
    sc.stop()//记得关闭创建的SparkContext对象
    
  }
}
```





执行报错：



```
16/01/07 13:50:16 INFO AppClient$ClientEndpoint: Connecting to master spark://ip:7077...  
16/01/07 13:50:36 ERROR SparkUncaughtExceptionHandler: Uncaught exception in thread Thread[appclient-registration-retry-thread,5,main]  
java.util.concurrent.RejectedExecutionException: Task java.util.concurrent.FutureTask@116465dc rejected from java.util.concurrent.ThreadPoolExecutor@565b576f[Running, pool size = 1, active threads = 1, queued tasks = 0, completed tasks = 0]  
    at java.util.concurrent.ThreadPoolExecutor$AbortPolicy.rejectedExecution(ThreadPoolExecutor.java:2048)
```


2、分析：



查看7077端口发现是监听本机地址，不过先从hostname着手。

本机开发是win7+eclipse环境，所以在C:\Windows\System32\drivers\etc\HOSTS中，增加一行IP Master对应的关系。

再执行，连接可以进入，不过一直出不来结果，WARN提示：


`WARN TaskSchedulerImpl: Initial job has not accepted any resources; check your cluster UI to ensure that workers are registered and have sufficient resources`

3、针对该错误，分析：



    执行到：
`wordCounts.foreach(wordNumberPair => println(wordNumberPair._1 + " : " +wordNumberPair._2))`出现上述错误。查核Spark UI，看开发本机的UI



![](https://img-blog.csdn.net/20170913110401351?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


看执行日志也正常，最后看master上的spark-env.sh配置，SPARK_WORKER_MEMORY=8192m，是否是这个导致这个资源不足的错误提示？

同样的代码提交到集群内任何一台机子都可以正确执行，spark-submit --class wc /home/hadoop/wc.jar

那说明不是这个资源问题，问题还在远程开发调试上。




4、进一步集群分析：

  http://master:8080/ 看到：

![](https://img-blog.csdn.net/20170913141817483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





 任务在集群上提交了各节点执行。

发现提交的用户是本机的用户名administrator，而集群内是hadoop，那是否关系到用户名呢？

找了下sparkconf配置好像没有可以设置用户名，那就只有在windows创建一个hadoop用户然后用来开发提交。






