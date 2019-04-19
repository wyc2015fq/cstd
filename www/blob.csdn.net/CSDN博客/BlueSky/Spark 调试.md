# Spark 调试 - BlueSky - CSDN博客
2016年04月19日 09:03:09[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：4068
## 1概述
针对调试，根据不同的调试对象可以分为两类：
1、应用程序的调试。
2、框架源码的调试。
在IDE中调试时采用的方法也对应有两种：
1、本地调试：可以简单理解为调试与被调试对象之间不需要通过通信的方式执行。
2、远程调试。
通常这两种方式都是可用的，只是在某些情况下，当被调试对象部署在远程机器节点中，而我们的调试工具在本地机器时，通常需要选择远程调试的方式。
Java单步调试的话，也可以借助java提供的工具类（$JAVA_HOME/bin/jdb）工具进行，类似于gdb之类的工具，有兴趣可以扩展下。
## 2Spark调试的准备工作
## 2.1     spark部分
下载所需的Spark部署包，以及对应的Spark源码包。
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaj9hQWv4P1ICqF2mbmFvlMytm34YdePygIibgBqB17IfqU4IibEIhZchQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
登录官网http://spark.apache.org/，到下载页面。
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9ia48g5zWAibV5tTnPKdbdnIEsNU7Fbjl8jPB40tfZfIB0Xo9ZkNdia9mlg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
选择1.6.1版本，分别下载部署包和源码包。
1、spark-1.6.1-bin-hadoop2.6.tgz：部署包用于部署集群环境，关于部署部分，请参考相关的文档。
2、spark-1.6.1.tgz：源码包主要用于调试时的源码关联。
选中下载对象后，点击下载，比如源码包的下载如下所示：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaAvBibibFtkxsic6j1v48VvCG26K3wOrDy2zyibUTUJVPKDHialuwEvAvA5A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
## 2.2     IDE部分
在此选择IntelliJ IDEA，进入下载页面：http://www.jetbrains.com/idea/download/
选择对应版本，下载并安装到集群中某个节点上（作为后续调试时的IDE）。
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaxxV9wkOtVMmoBm6wibqhNr7jxo0FjZ9SzdHn9gTKNTcIPtZBx7MAfQQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
## 3Spark应用程序的调试
Spark应用程序的调试有以下一种方式：
1、spark-shell脚本提供的单步调试。
2、IDE中采用local模式的调试。
3、IDE中提交到cluster的调试。
下面分别给出简单的调试步骤。
## 3.1     spark-shell脚本提供的单步调试
spark-shell脚本是Spark框架提供的交互式界面，可以单步调试应用程序，直接在部署节点上进入部署目录，执行Spark的bin/spark-shell脚本即可。
[harli@cluster04 spark]$ ./bin/spark-shell
log4j:WARN No appenders   could be found for logger (org.apache.hadoop.metrics2.lib.MutableMetricsFactory).
log4j:WARN Please   initialize the log4j system properly.
log4j:WARN See   http://logging.apache.org/log4j/1.2/faq.html#noconfig for more info.
Using Spark's repl log4j   profile: org/apache/spark/log4j-defaults-repl.properties
To adjust logging level use   sc.setLogLevel("INFO")
Welcome to
      ____              __
     / __/__    ___ _____/ /__
    _\ \/ _ \/ _ `/ __/  '_/
   /___/ .__/\_,_/_/ /_/\_\   version 1.6.1
      /_/
Using Scala version 2.10.5   (Java HotSpot(TM) 64-Bit Server VM, Java 1.7.0_71)
Type in expressions to have   them evaluated.
Type :help for more   information.
Spark context available as   sc.
16/04/17 09:18:44 WARN   Connection: BoneCP specified but not present in CLASSPATH (or one of   dependencies)
16/04/17 09:18:44 WARN   Connection: BoneCP specified but not present in CLASSPATH (or one of   dependencies)
16/04/17 09:18:50 WARN   ObjectStore: Version information not found in metastore.   hive.metastore.schema.verification is not enabled so recording the schema
   version 1.2.0
16/04/17 09:18:50 WARN   ObjectStore: Failed to get database default, returning NoSuchObjectException
16/04/17 09:18:52 WARN   Connection: BoneCP specified but not present in CLASSPATH (or one of   dependencies)
16/04/17 09:18:52 WARN   Connection: BoneCP specified but not present in CLASSPATH (or one of   dependencies)
16/04/17 09:18:56 WARN   ObjectStore: Version information not found in metastore.   hive.metastore.schema.verification is not enabled so recording the schema
   version 1.2.0
16/04/17 09:18:56 WARN   ObjectStore: Failed to get database default, returning NoSuchObjectException
SQL context available as   sqlContext.
scala>
默认情况下为local模式，也可以指定--master为集群的Master URL。
之后可以在交互式界面以链式方式执行代码，也可以单步执行代码并跟踪代码的输入输出等信息。
交互式模式是调试应用程序常用的最简单的方式。
## 3.2     IDE中采用local模式的调试。
首先启动IDEA，进入安装目录，启动idea.sh脚本，如下所示：
[harli@cluster05 bin]$ pwd
/home/harli/IDEA/idea-IU-141.2735.5/bin
[harli@cluster05 bin]$ ./idea.sh
在此使用WordCount作为案例，在IDEA中构建一个工程，并创建objectWordCount。工程的目录结构如下所示：
通过菜单查看目录：
确认将指定的目录Mark as为正确的Sources或Tests等。
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iauUmxU46EnAt1VaF65FLXrNoPG8NP5PqeW01zbNIHRj5PiczAaicnDfYg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
对应的源码如下所示：
import   org.apache.spark.{SparkContext, SparkConf}
object WordCount {
def main(args:   Array[String]): Unit = {
valconf = new   SparkConf().setAppName("WordCount").setMaster("local")
valsc = new   SparkContext(conf)
valtextFile =   sc.textFile("/home/harli/cluster16/spark/README.md")
valwordCounts =   textFile.flatMap(line =>line.split(" ")).
      map(word => (word,   1)).reduceByKey((a, b) => a + b)
wordCounts.collect().foreach(println)
sc.stop()
  }
}
当以local模式本地调试应用程序时，需要设置Master为local，即代码中的：.setMaster("local")。
然后在main方法中设置断点，并在方法中右键，选择Debug选项，如下所示：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9ia6OS5YzEboibhx7hLrUiafjTqlxRNriaLMjZo9gISokbqHWqnoNK7ibgdQw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
执行后会在断点处停止，并高亮显示当前执行代码，对应的，会出现调试debugger窗口和控制台console窗口，如下所示：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iavwKgHvWpBT8Z9LXCrNT3NGDyeqIpmpWn76icDfeGCSqsKWXoQKOel7w/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
在控制台窗口中可以看到输出信息。
在调试窗口中可以跟踪各个变量或表达式等。
图中方框部分对应调试的方式，可以单步执行，进入方法，强制进入方法，退出方法等等。
左下角的绿色三角按钮是全速运行，直到下一个断点为止。红色方框表示停止。
大部分相关的信息可以在鼠标停在上面的时候获取提示信息。
比如下面是强制跟踪进去后进入的map方法：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9ia5arKSFiaXAJsvUeIE9Dcq9Ywy3V2icjALISDdOicaSf4fUicoYJtcu2Q5g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
## 3.3     IDE中提交到cluster的调试
cluster模式的调试，可以采用本地调试和远程调试两种。远程调试的话可以打开应用程序对应的调试参数，然后在IDE中远程跟踪调试。具体方式和下一章一样，因此本节仅给出IDE的本地调试才cluster模式提交的应用程序。
即，在部署模式为client时，应用程序是提交点执行，因此可以本地跟踪调试，对应的cluster部署时，会由调度机制选择具体节点执行应用程序，因此需要远程调试。
注意：在以cluster模式提交应用程序之前，先启动spark集群（Standalone）。
集群监控界面：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iauDZYYnclcoibMVC6rtvCEhZMFZyxFmyKribQ4u0L24qvxnYS4EP5ibBkQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
红色部分对应提交的master url信息，启动worker或提交应用时需要指定该信息。
以cluster模式提交集群时，为了尽可能模拟spark-submit脚本的提交应用程序方式，因此以脚本对应的SparkSubmit作为java进程入口，此时需要先关联源码，关联的步骤如下。
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaHcDWZKpsqUgJgxbuAaTOEyfhCUfzaX5xQJEUibWz0MdRuSwPhzYl7BQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
打开Project Structure…
选择类库，当前使用的类库为1.6.1版本的lib下全部jar包。如下所示：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaM8icBBORHtCbhbmm6wwF7APL8J1Ml858sQWOSMs9VDcxBia2DU5rmibQg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
选择需要关联源码的jar包，点击左边的+，添加源码关联：
添加前，确保已经将下载下来的源码解压到本地：
[harli@cluster05 github]$   tarzxvf /mnt/hgfs/Cluster\ Tools/spark-1.6.1.tgz -C ./
[harli@cluster05 github]$   ls
build.sbtbak      spark        Test     TestProjectWordCount
MySpark           spark-1.5.1  Test1    TestProject1
SimpleAppProjectspark-1.6.1  Test2    TestProjectIde
其中spark-1.6.1为对应源码。
点击添加后，选择spark-1.6.1源码目录，然后点击OK：
之后会扫描目录，结束后弹出窗口：
直接点击OK，即可将源码关联进来。
点击OK，完成关联。
之后按Ctrl+N键，打开类查找窗口：
查找入口类SparkSubmit，跳到对应的scala类。
此时可以在SparkSubmit中设置断点，也可以直接在应用程序中设置断点。
设置好断点后，编辑调试参数，打开编辑窗口：
在Configuration配置界面上，设置主类为org.apache.spark.deploy.SparkSubmit，设置应用程的参数Program arguments，即应用程序的参数，该参数和提交应用程序的spark-submit脚本的参数一样，当前为：
--master   spark://cluster04:7077 --class WordCount/home/harli/github/Test2/out/artifacts/Test2_jar/Test2.jar
当前执行应用程序的主类为WordCount（如果有package路径，需要同时指定），Test2.jar为该类所在的jar包，即对应主资源（确保已经构建了该jar包——可以参考开发环境搭建等文档）。
注意：此时需要将应用程序中的.setMaster("local")去掉——参考配置属性的优先级。
在SparkSubmit的main方法中设置断点，点击右上角的调试按钮：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaGDTFYPOpwWU4YdRESHxgiamfIylUeWyUNWdMGYegcHIl9A1X0eV7jibg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
运行后会停在SparkSubmit设置的断点上，如下所示：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaOpdUc4H98Hxze2JTDH9IM1BSVibJwicf4Kbp2tK9T2uuHqtCsPMFWUYw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
然后全速运行，直到跟踪到应用程序的断点，如下所示：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaFBZ1BNCQn7nbU7svhtMVJo6Df2kxYZDnjUNibQ2c3Mzia3m1IIOGbopw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
## 4Spark框架源码的调试
下面以调试Master组件为例，调试Spark框架源码。
先停止集群，然后启动Master组件，Worker组件是向Master注册的，可以在Master组件启动完成后再启动Worker组件。
调试之前，需要设置JVM进程的调试参数，在Master 所在节点的conf/spark-env.sh脚本中添加以下配置：
SPARK_MASTER_OPTS="$SPARK_MASTER_OPTS   -Xdebug -server -Xrunjdwp:transport=dt_socket,address=5005,server=y,suspend=y"
当前Master组件所在机器节点为cluster04，调试监听端口设置为5005.
在Master组件对应的Master入口类中设置断点，首先，通过Ctrl+N找到该类，如下所示：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iauuLFOgWdFgFpCWs3MFo2ib2a6fd54icPfc35ib1rqxW7icnz4wCyEMnqew/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
设置断点：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaoWSXFxXP4NmUD7ia2hIwlH735Ac2ZI2CtGDSDZAXF3PWniaJlmkBaMHA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
然后，通过master启动脚本启动Master组件，即Master的jvm进程，如下所示：
[harli@cluster04 spark]$   ./sbin/start-master.sh
starting   org.apache.spark.deploy.master.Master, logging to   /home/harli/cluster16/spark/logs/spark-harli-org.apache.spark.deploy.master.Master-1-cluster04.out
[harli@cluster04 spark]$
查看日志文件：
Spark Command:   /lib/jdk1.7.0_71/bin/java -cp   /home/harli/cluster16/spark/conf/:/home/harli/cluster16/spark/lib/spark-assembly-1.6.1-hadoop2.6.0.jar:/home/harli/cluster16/spark/lib/datanucleus-rdbms-3.2.9.jar:/home/harli/cluster16/spark/lib/datanucleus-core-3.2.10.jar:/home/harli/cluster16/spark/lib/datanucleus-api-jdo-3.2.6.jar
   -Xdebug -server -Xrunjdwp:transport=dt_socket,address=5005,server=y,suspend=y   -Xms1g -Xmx1g -XX:MaxPermSize=256m org.apache.spark.deploy.master.Master --ip   cluster04 --port 7077 --webui-port 8080
========================================
Listening for transport   dt_socket at address: 5005
~                                                                                
~                                                          
前面是执行的java命令，启动会，会提示：Listening for transport dt_socket at address: 5005
，表示当前已经启动，并监听端口5005.
此时，可以在IDE中启动一个远程调试，步骤如下：
1、编辑配置
2、添加一个远程调试remote：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9ianbRcZf83c2JXKBP7oqKgiaSV0IxXdqTJBnJ94vveIUyQkPSoCqv2PJA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
在配置中设置正确的hostname和port，分别对应Master组件启动的机器节点cluster04和之前设置的监听端口5005。
3、启动调试：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iazsvbrkEePXTUgnWFup9sF7fIicEJFniaC7Y74CuUbe5bWQZGfYTXxpxA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
调试时，重要的是知道自己的调试目标，main是入口点，这里还可以设置一些消息处理的断点进行跟踪。
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9ia7ogTqjkskQja6icMH1lCv47VyoOHDJCwzjjPVhMLBKVNh6uYfdNkaibw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
可以看到，已经跟踪进了main入口点。继续全速运行，到消息的处理代码处：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaiaUiaI1wMOVU9BvqGXYaabIXS7Ds4xz9xxr6Oic7IusAf9RAQeR8WmQdg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
这里是选中领导后的处理代码。
此时可以单独启动一个Worker节点，然后跟踪注册消息的处理代码。
到其他节点上，启动worker组件，代码如下所示：
[harli@cluster04 spark]$   ./sbin/start-slave.sh    spark://cluster04:7077
starting   org.apache.spark.deploy.worker.Worker, logging to   /home/harli/cluster16/spark/logs/spark-harli-org.apache.spark.deploy.worker.Worker-1-cluster04.out
其中，spark://cluster04:7077对应为Master的URL信息，在8080端口界面上可以查看。
继续查看IDE中的跟踪代码：
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iavAlN5vzd9VRhApDYay5TqTRbjxiaR9wZvUiaRYoOw4YoiakR6mibMb5ORw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
可以跟踪到Worker组件的注册消息。
## 5补充
注意：调试哪个进程就需要在哪个进程启动的命令中添加调试参数，Spark框架的组件是通过脚本来启动的，因此可以在脚本中设置对应参数，每个组件分别提供了各自的环境变量，用于设置JVM进程启动时的参数。
1、对于Spark 的守护进程的调试参数配置
    （1）可以在SPARK_DAEMON_JAVA_OPTS环境变量中统一添加远程调试参数，此时要确保要调试的守护进程不在同一台机器上。
    （2）可以在具体的守护进程特定的XXX_OPTS环境变量中设置。此时，如果调试的守护进程在同一台机器上，可以通过设置不同的调试端口进行区分。
2、环境变量参考spark-env.sh文件，有以下几种：
    SPARK_MASTER_OPTS：Master组件
    SPARK_WORKER_OPTS：Worker组件
    SPARK_HISTORY_OPTS：历史服务组件
    SPARK_SHUFFLE_OPTS：外部Shuffle服务组件
3、对应Executor的调试
需要知道对应的JVM进程的主类（比如CoarseGrainedExecutorBackend），然后设置断点，另外要找出控制该jvm进程启动参数所对应的配置属性，可以在--conf "spark.executor.extraJavaOptions”中设置调试参数。调试时，需要注意Executor启动的个数，以及最终在哪个节点上启动，同时在IDEA中设置该节点为remote配置中的hostname信息。
4.       应用程序的远程调试
    (1)查看spark-submit脚本，最终使用了spark-class脚本来运行org.apache.spark.deploy.SparkSubmit类；
    (2) "$SPARK_HOME"/bin/spark-class org.apache.spark.deploy.SparkSubmit "$@"
    (3)查看spark-class脚本，最终使用类org.apache.spark.launcher.Main构建提交命令
    (4)查看org.apache.spark.launcher.Main类，找到命令构建处
List<String>cmd = builder.buildCommand(env);
跟进代码
![](http://mmbiz.qpic.cn/mmbiz/FgIoUicU9YiafVLkSZgibodFKUyXkty8Q9iaARCzX106hqN21x2gCxjfTP66GYOQMP4c5JxL46q1mibNx8Aobo2Nr6Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
箭头处为提交命令构建的具体构建类
查看具体的构建命令buildCommand的实现代码：
@Override
  public List<String>buildCommand(Map<String, String>env) throws IOException   {
  if (PYSPARK_SHELL_RESOURCE.equals(appResource) && !printHelp) {
  return buildPySparkShellCommand(env);
  } else if (SPARKR_SHELL_RESOURCE.equals(appResource) && !printHelp) {
  return buildSparkRCommand(env);
  } else {
  return buildSparkSubmitCommand(env);
  }
  }
以buildSparkSubmitCommand为例，查看代码，找到其中代码：
if (isThriftServer(mainClass))   {
  addOptionString(cmd, System.getenv("SPARK_DAEMON_JAVA_OPTS"));
  }
  addOptionString(cmd, System.getenv("SPARK_SUBMIT_OPTS"));
  addOptionString(cmd, System.getenv("SPARK_JAVA_OPTS"));
OK，对于的JVM options包含以上SPARK_DAEMON_JAVA_OPTS 、SPARK_SUBMIT_OPTS 、SPARK_JAVA_OPTS
1、SPARK_DAEMON_JAVA_OPTS：对应所有守护进程——修改后需重启进程才有效
2、SPARK_JAVA_OPTS的修改会影响所有JVM的执行
3、针对远程调试目标为org.apache.spark.deploy.SparkSubmit —— 参见第1点：spark-submit脚本
4、因此只需要修改针对性的环境变量SPARK_SUBMIT_OPTS即可
5、在SPARK_SUBMIT_OPTS中添加远程调试的opts（同上），然后使用spark-submit提交应用，提交后在IDEA等工具中远程调试opts中指定host和port处即可。
注意：由于分布式环境，IO有timeout等现在，因此调试时需注意时间。
