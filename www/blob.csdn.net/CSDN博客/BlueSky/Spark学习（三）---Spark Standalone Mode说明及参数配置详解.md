# Spark学习（三）---Spark Standalone Mode说明及参数配置详解 - BlueSky - CSDN博客
2015年11月20日 15:12:36[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：3616
## 启动集群
–启动standalone模式下master server（启动完毕后可以通过[http://master:8080](http://master:8080)来看你对应的spark-url，其中master对应你机器的hostname） 
./sbin/start-master.sh 
–启动one or more works并且将他们连接到master 
./sbin/start-slaves.sh   
–全部启动 
./sbin/start-all.sh
以上对应的关闭为: ./sbin/stop-all.sh 
启动时候对于内存、内核使用的配置可以在conf/spark-env.sh来设置，参数可以参考官方文档：[http://spark.apache.org/docs/latest/spark-standalone.html](http://spark.apache.org/docs/latest/spark-standalone.html)
## 将应用发布到集群
在提交应用到spark集群上，仅仅是传递master的 spark：//IP:port URL 作为sparkContext constructor，运行一个交互的spark shell可以通过以下命令： 
./bin/spark-shell –master spark://IP:PORT 
也可以传递参数–total-executor-cores 来控制集群上spark-shell使用的内核数。
## 启动spark程序
可以通过spark-submit脚本来进行启动一个spark应用程序到集群。 
具体参数可以参照官方配置文件说明： 
[http://spark.apache.org/docs/latest/configuration.html](http://spark.apache.org/docs/latest/configuration.html)
如果你提交过的应用程序一直不能执行成功，那么你就可以kill掉，相应的指令： 
./bin/spark-class org.apache.spark.deploy.Client kill   
其中driver ID可以通过Master web UI在http://:8080 来进行查看。 
你可以在SparkConf中设置相关参数如：
```
val conf = new SparkConf()
             .setMaster(...)
             .setAppName(...)
             .set("spark.cores.max", "10")
val sc = new SparkContext(conf)
```
同样对于上面的spark.core.max设置也可以在cong/spark-env.sh中设置：
```bash
export SPARK_MASTER_OPTS="-Dspark.deploy.defaultCores=<value>"
```
## 监控和日志记录
Spark standalone模式提供一个web用户接口来进行管理集群，master和worker都分别有着自己的web UI来展示集群和作业的详细信息。默认master的web UI端口是8080，同样你也可以通过命令的方式来改变。 
此外，详细的日志对于每个作业的详细的日志输出被写入到每个slave节点的work目录下(默认的路径为SPARK_HOME/work)，你会发现对于每个作业会有两个输出，stdout和stderr（所有的屏幕的输出都被写入到其中）。
## spark配置
1.spark属性配置 
例如：
```
val conf = new SparkConf()
             .setMaster("local[2]")
             .setAppName("CountingSheep")
val sc = new SparkContext(conf)
```
**2、动态载入spark 属性：**
**spark shell和spark-submit提供两种方式来进行动态的载入spark属性，第一种如下A中所示，利用命令的形式；（spark-shell应该是从conf/spark-env.sh中加载）/bin/spark-submit也会从conf/spark-defaults.conf中读取相应的参数配置，其中每一行包含一个key-value对，以空格分开，如B中所示。**
你可以仅仅创建一个空的conf如：
`val sc = new SparkContext(new SparkConf())`
A、然后在runtime的时候提交配置，如：
`./bin/spark-submit --name "My app" --master local[4] --conf spark.shuffle.spill=false --conf "spark.executor.extraJavaOptions=-XX:+PrintGCDetails -XX:+PrintGCTimeStamps" myApp.jar`
B、
```
spark.master            spark://192.168.203.148:7077
spark.executor.memory   4g
spark.eventLog.enabled  true
spark.serializer        org.apache.spark.serializer.KryoSerializer
```
同样你可以通过http://:4040来查看相应的spark属性与环境。
```
Master URL                            Meaning
local         Run Spark locally with one worker thread (i.e. no parallelism at all).
local[K]      Run Spark locally with K worker threads (ideally, set this to the number of cores on your machine).
local[*]     Run Spark locally with as many worker threads as logical cores on your machine.
spark://HOST:PORT    Connect to the given Spark standalone cluster master. The port must be whichever one your master is configured to use, which is 7077 by default.
mesos://HOST:PORT    Connect to the given Mesos cluster. The port must be whichever one your is configured to use, which is 5050 by default. Or, for a Mesos cluster using ZooKeeper, use mesos://zk://....
yarn-client  Connect to a YARN cluster in client mode. The cluster location will be found based on the HADOOP_CONF_DIR or YARN_CONF_DIR variable.
yarn-cluster     Connect to a YARN cluster in cluster mode. The cluster location will be found based on the HADOOP_CONF_DIR or YARN_CONF_DIR variable.
```
