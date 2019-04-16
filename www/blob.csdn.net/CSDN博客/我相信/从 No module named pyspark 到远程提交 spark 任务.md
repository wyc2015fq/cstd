# 从 "No module named pyspark" 到远程提交 spark 任务 - 我相信...... - CSDN博客





2016年04月16日 21:50:28[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：12405








能在本地Mac环境用python提交spark 任务会方便很多，但是在安装了 spark-1.6-bin-without-hadoop  (spark.apache.org/download) 之后，在python 中  “import pyspark” 会报“no module named pyspark” 错误。 没错，这种错误都是 路径问题。




为了本地使用spark，需要在~/.bash_profile 中增加两个环境变量：SPARK_HOME 以及必知的PYTHONPATH

`export SPARK_HOME=/Users/abc/Documents/`spark-1.6.0-bin-without－hadoop ＃这是spark 的安装路径



`export PYTHONPATH=$SPARK_HOME/python:$SPARK_HOME/python/lib/py4j-0.8.2.1-src.zip:$PYTHONPATH`




*注：Py4J 有点像 Python 版的 JNI，通过它， Python 程序可以利用 Python 解释器直接调用Java虚拟机中的 Java 对象，也可以让 Java 调用 Python 对象。*




然后，别忘了，source ~/.bash_profile 让它生效。 运行 python shell，




from pyspark import SparkContext 




都可以了么， 但是 当你单独执行pyspark 或者 在python 中初始化SparkConf 等其它类的时候，又报错了




"Exception in thread "main" java.lang.NoClassDefFoundError: org/apache/hadoop/fs/FSDataInputStream"




spark 访问FS 时库文件丢失，看来spark 和hadoop 的结合还需要指明更多的纽带，简单的换一下 spark distribution吧。将 spark-1.6.0-bin-without-hadoop 换成 spark-1.6.0-bin-hadoop2.6,然后更新 .bash_profile 中SPARK_HOME 的路径。




直接运行pyspark：





$ pyspark

Python 2.7.11 (default, Mar  1 2016, 18:40:10) 

[GCC 4.2.1 Compatible Apple LLVM 7.0.2 (clang-700.1.81)] on darwin

Type "help", "copyright", "credits" or "license" for more information.

16/04/16 21:41:02 INFO spark.SparkContext: Running Spark version 1.6.0

16/04/16 21:41:05 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable

16/04/16 21:41:05 INFO spark.SecurityManager: Changing view acls to: abel,hdfs

16/04/16 21:41:05 INFO spark.SecurityManager: Changing modify acls to: abel,hdfs

16/04/16 21:41:05 INFO spark.SecurityManager: SecurityManager: authentication disabled; ui acls disabled; users with view permissions: Set(abel, hdfs); users with modify permissions: Set(abel, hdfs)

16/04/16 21:41:06 INFO util.Utils: Successfully started service 'sparkDriver' on port 55162.

16/04/16 21:41:06 INFO slf4j.Slf4jLogger: Slf4jLogger started

16/04/16 21:41:06 INFO Remoting: Starting remoting

16/04/16 21:41:07 INFO Remoting: Remoting started; listening on addresses :[akka.tcp://sparkDriverActorSystem@192.168.1.106:55165]

16/04/16 21:41:07 INFO util.Utils: Successfully started service 'sparkDriverActorSystem' on port 55165.

16/04/16 21:41:07 INFO spark.SparkEnv: Registering MapOutputTracker

16/04/16 21:41:07 INFO spark.SparkEnv: Registering BlockManagerMaster

16/04/16 21:41:07 INFO storage.DiskBlockManager: Created local directory at /private/var/folders/wk/fxn2zdyd7rz8rm66rst4h15w0000gn/T/blockmgr-6de54d08-31c9-430e-ac3c-9f3e0635e486

16/04/16 21:41:07 INFO storage.MemoryStore: MemoryStore started with capacity 511.5 MB

16/04/16 21:41:07 INFO spark.SparkEnv: Registering OutputCommitCoordinator

16/04/16 21:41:07 INFO server.Server: jetty-8.y.z-SNAPSHOT

16/04/16 21:41:07 INFO server.AbstractConnector: Started SelectChannelConnector@0.0.0.0:4040

16/04/16 21:41:07 INFO util.Utils: Successfully started service 'SparkUI' on port 4040.

16/04/16 21:41:07 INFO ui.SparkUI: Started SparkUI at http://192.168.1.106:4040

16/04/16 21:41:07 INFO executor.Executor: Starting executor ID driver on host localhost

16/04/16 21:41:07 INFO util.Utils: Successfully started service 'org.apache.spark.network.netty.NettyBlockTransferService' on port 55167.

16/04/16 21:41:07 INFO netty.NettyBlockTransferService: Server created on 55167

16/04/16 21:41:07 INFO storage.BlockManagerMaster: Trying to register BlockManager

16/04/16 21:41:07 INFO storage.BlockManagerMasterEndpoint: Registering block manager localhost:55167 with 511.5 MB RAM, BlockManagerId(driver, localhost, 55167)

16/04/16 21:41:07 INFO storage.BlockManagerMaster: Registered BlockManager

Welcome to

      ____              __

     / __/__  ___ _____/ /__

    _\ \/ _ \/ _ `/ __/  '_/

   /__ / .__/\_,_/_/ /_/\_\   version 1.6.0

      /_/




Using Python version 2.7.11 (default, Mar  1 2016 18:40:10)

SparkContext available as sc, HiveContext available as sqlContext.
>>>




OK， 至此，pyspark 算是在本机的MAC 环境中可以基本上正常工作了。




