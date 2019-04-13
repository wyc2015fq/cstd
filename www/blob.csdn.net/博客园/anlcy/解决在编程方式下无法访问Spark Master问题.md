
# 解决在编程方式下无法访问Spark Master问题 - anlcy - 博客园






# [解决在编程方式下无法访问Spark Master问题](https://www.cnblogs.com/camilla/p/8296314.html)
我们可以选择使用spark-shell，spark-submit或者编写代码的方式运行Spark。在产品环境下，利用spark-submit将jar提交到spark，是较为常见的做法。但是在开发期间，每次都需要编译jar去做提交是一件麻烦事儿。尤其是在IDE例如IntelliJ Idea下，更直接的方式还是在main()方法中直接通过SparkContext运行。例如：
object DataFrameApp {
def main(args: Array[String]): Unit = {
val sparkConf = new SparkConf().setAppName("DataFrame")
.setMaster("local[*]")
val sc = new SparkContext(sparkConf)
}
}
在我们的产品中，更需要采用编程方式去运行Spark数据分析。因为我们希望将数据分析的逻辑封装（暴露）为REST服务。我们选择了Spary作为REST框架。在这种方式下，应该由客户端的请求触发任务的执行。为了性能考虑，我们会在启动spary-can时实例化SparkContext，然后将其传递给真正执行任务的Actor。当然，如何将Spark Context的创建与spray-can容器的启动结合起来，则是另外一个坑，但好歹这个坑已被填平，我会在下一篇博客中介绍。
上述代码在IntelliJ中运行没有任何问题。这里给出build.sbt中设置的依赖：
scalaVersion := "2.11.6"
libraryDependencies ++= {
val sparkVersion = "1.3.1"
val hadoopVersion = "2.6.0"
Seq(
"org.apache.spark"    %% "spark-core"             % sparkVersion,
"org.apache.spark"    %% "spark-sql"               % sparkVersion,
"org.apache.spark"    %% "spark-catalyst"        % sparkVersion,
"org.apache.hadoop" %  "hadoop-client"          % hadoopVersion,
"org.postgresql"       %  "postgresql"               % "9.4-1201-jdbc41"
)
}
在产品环境下，我们不可能将master设置为local模式。目前，我们并没有将spark部署到yarn或者mesos下，而是选择了最简单的standalone方式。方法就是运行SPARK_HOME/sbin目录下的脚本start-master.sh或者start-all.sh。而在客户端，需要将SparkConf的master设置为部署的spark url。如何获知这个url呢？当Spark启动成功后，假设机器的IP为192.168.1.4，则可以通过访问192.168.1.4:8080访问Spark Web UI：
![spark ui](http://7u2mak.com1.z0.glb.clouddn.com/spark_ui.png)
这个页面显示了url地址：spark://192.168.1.4:7077。根据[Spark官方文档对部署模式的说明](http://spark.apache.org/docs/latest/spark-standalone.html)，我们可以将该URL设置到SparkConf下，例如：
trait SparkContextSupport {
val sparkConf = new SparkConf().setAppName("Spark-Spike")
.setMaster("spark://192.168.1.4:7077")
val sc = new SparkContext(sparkConf)
}
object PostgreSqlFetcherApp extends SparkContextSupport {
def main(args: Array[String]): Unit = {
val sqlContext = new SQLContext(sc)
val url = "jdbc:postgresql://localhost:5432/demo?user=zhangyi"
val dataFrame = sqlContext.load("jdbc", Map(
"url" -> url,
"driver" -> "org.postgresql.Driver",
"dbtable" -> "tab_datasets"
))
dataFrame.registerTempTable("Employees")
val emps = sqlContext.sql("select name from Employees")
emps.take(100).map(row => row.getString(0)).foreach(println)
sc.stop()
}
}
当然，我们也可以在/etc/hosts下为该ip地址设置hostname，从而通过hostname来访问。
运行这段程序会发生什么呢？很不幸，它在创建SparkContext的过程中抛出了如下错误：
/05/18 09:41:12 INFO AppClient$ClientActor: Connecting to master akka.tcp://sparkMaster@192.168.1.4:7077/user/Master...
/05/18 09:41:12 WARN ReliableDeliverySupervisor: Association with remote system [akka.tcp://sparkMaster@192.168.1.4:7077] has failed, address is now gated for [5000] ms. Reason is: [Disassociated].
/05/18 09:41:32 INFO AppClient$ClientActor: Connecting to master akka.tcp://sparkMaster@192.168.1.4:7077/user/Master...
/05/18 09:41:32 WARN ReliableDeliverySupervisor: Association with remote system [akka.tcp://sparkMaster@192.168.1.4:7077] has failed, address is now gated for [5000] ms. Reason is: [Disassociated].
/05/18 09:41:52 ERROR SparkDeploySchedulerBackend: Application has been killed. Reason: All masters are unresponsive! Giving up.
/05/18 09:41:52 WARN SparkDeploySchedulerBackend: Application ID is not initialized yet.
/05/18 09:41:52 ERROR TaskSchedulerImpl:Exiting due to error from cluster scheduler: All masters are unresponsive! Giving up.
Spark客户端与standalone方式部署的spark master是通过AKKA的remote actor来通信的。根据这段错误信息，我直觉认为是获取path为akka.tcp://sparkMaster@192.168.1.4:7077/user/Master的RemoteActor出现了问题。通过单步调试结合阅读源代码，我看到在standalone模式下，创建SparkContext时，会创建对应的TaskSchedulerImpl与SparkDeploySchedulerBackend对象。之后，它会执行SparkDeploySchedulerBackend的start()方法，进而跟踪到ClientActor的创建。ClientActor是一个AKKA actor，它会在启动前（actor被创建后会自动地异步方式启动）执行钩子方法preStart()。如下为Spark的源代码：
class ClientActor extends Actor with ActorLogReceive with Logging {
override def preStart() {
context.system.eventStream.subscribe(self, classOf[RemotingLifecycleEvent])
try {
registerWithMaster()
} catch {
case e: Exception =>
logWarning("Failed to connect to master", e)
markDisconnected()
context.stop(self)
}
}
def registerWithMaster() {
tryRegisterAllMasters()
import context.dispatcher
var retries = 0
registrationRetryTimer = Some {
context.system.scheduler.schedule(REGISTRATION_TIMEOUT, REGISTRATION_TIMEOUT) {
Utils.tryOrExit {
retries += 1
if (registered) {
registrationRetryTimer.foreach(_.cancel())
} else if (retries >= REGISTRATION_RETRIES) {
markDead("All masters are unresponsive! Giving up.")
} else {
tryRegisterAllMasters()
}
}
}
}
}
def tryRegisterAllMasters() {
for (masterAkkaUrl <- masterAkkaUrls) {
logInfo("Connecting to master " + masterAkkaUrl + "...")
val actor = context.actorSelection(masterAkkaUrl)
actor ! RegisterApplication(appDescription)
}
}
}
注意tryRegisterAllMasters()方法的实现以及调用。启动ClientActor时，会根据设置的重试次数，不停地去尝试注册所有的Master，实现即为调用ActorContext的actorSelection()方法，根据传入的masterAkkaUrl获得remote actor。根据actor的path，以及发送的RegisterApplicaition消息，可以了解到这个remote actor就是定义在org.apache.spark.deploy.master包中Master。
根据前面看到的错误信息，我想当然地认为是通信问题导致无法获得remote actor。然后通过单步调试，结果颠覆了我的猜测，执行到如下步骤是可以获得actor对象的：
val actor=context.actorSelection(masterAkkaUrl)
在spark master业已启动的前提下，我编写了如下程序验证了remote actor是可以正常获得：
object RemoteActorApp extends App {
val system = ActorSystem("spike-spark-issue")
val actor = system.actorSelection("akka.tcp://sparkMaster@192.168.1.4:7077/user/Master")
if (actor == null) println("null actor") else println("correct")
}
之后就是冗长而耗时的解决问题时间。无论是通过google查找解决方案，还是通过spark user list去咨询问题，又或者阅读spark源代码，种种方式不一而足，弄得我精力憔悴，费时费力，最后也没有找到解决方案。唯一找到一个相对靠谱的是Mithra在[StackOverFlow上的自问自答](http://stackoverflow.com/questions/27415501/why-is-it-so-hard-to-run-simple-spark-streaming-spark-1-1-1-maven-dependency)，同时他也将这个解决方案放到了spark user list上。他的主要总结为：
1.Make sure your spark version and version in your pom is same；
2.Hadoop version of the spark is the version with which spark is build or use spark hadoop prebuild version；
3.Update your spark-env.sh with required details:
export JAVA_HOME=/User/java/
export SPARK_MASTER_IP=xyz
export SPARK_WORKER_CORES=2
export SPARK_WORKER_INSTANCES=1
export SPARK_MASTER_PORT=7077
export SPARK_WORKER_MEMORY=4g
export MASTER=spark://${SPARK_MASTER_IP}:${SPARK_MASTER_PORT}
export SPARK_LOCAL_IP=xyz
4..Make sure you clean compile package your jar file every time before you code submit your spark application。
当然，这个帖子要解决的问题是spark-submit而非我这里说的编程方式。相同的设置下，我运行spark-submit并没有出现前面的问题。
不过他山之玉，可以攻石，不妨借鉴这里的建议。我也确认了spark的版本，sbt中依赖的spark版本为1.3.1，我运行的spark master也是同样的版本。我最初也怀疑是hadoop的问题。我在部署spark时，并没有安装hadoop。为了解决此问题，我专门安装了2.6版本的hadoop，然后执行如下命令重新编译了spark 1.3.1，从而保证hadoop版本与spark是兼容的：build/mvn-Phadoop-2.6-Dhadoop.version=2.6.0-DskipTestscleanpackage
我甚至在spark-env.sh中配置了与hadoop有关的目录配置，当然也包括前面建议中提到的相关配置：
export HADOOP_CONF_DIR=/Users/zhangyi/lib/hadoop-2.6.0/etc/hadoop
export SPARK_MASTER_IP=192.168.1.4
export SPARK_MASTER_PORT=7077
export SPARK_WORKER_CORES=1
export SPARK_WORKER_MEMORY=2G
export SPARK_WORKER_INSTANCES=1
export SPARK_LOCAL_IP=192.168.1.4
不幸的是，问题依然存在！
痛定思痛，冷静下来，我在反思自己，觉得自己似乎走进了一个误区。因为有spark的源代码，有google和spark user list，我想当然地希望通过看到的错误信息去网上寻找相似的问题，从而获得解决方案。当查找没有结果时，我又过度地相信自己能够通过源代码发现一些端倪。我甚至考虑通过attach process的方式尝试着为remote actor设置断点，从而进行问题跟踪。然而，我却忘了要解决问题，首先要分析问题出现的根由，并由此进行下一步分析与判断。要做到这一点，最有效的手段其实是通过日志。
默认情况下，下载并编译后的spark并没有开启日志记录功能。spark使用了log4j记录日志，在conf目录下提供了log4j.properties.template文件。复制该文件，并命名为log4j.properties，利用默认的日志配置即可。重新运行start-all.sh脚本，启动spark master，然后再回到intellij下运行main函数。结果，让我惊奇地发现日志文件（日志文件出现在logs目录下）中出现了如下错误信息：
java.io.InvalidClassException: org.apache.spark.deploy.Command; local
class incompatible: stream classdesc serialVersionUID 8789839749593513237, local class serialVersionUID = -4145741279224749316
发生序列化问题的Command类其实是一个普通的样例类：
import scala.collection.Map
private[spark] case class Command(
mainClass: String,
arguments: Seq[String],
environment: Map[String, String],
classPathEntries: Seq[String],
libraryPathEntries: Seq[String],
javaOpts: Seq[String]) {
}
Scala的样例类（case class）自身支持了对象的序列化。为何会发生序列化不兼容的情况呢？由于两边的spark版本是完全一致的，这让我想起是否因为scala版本不一致。
阅读[Spark官方文档-Building Spark](http://spark.apache.org/docs/latest/building-spark.html)发现，Spark通过maven进行build时，默认scala版本为2.10。若要为Scala 2.11进行编译，需要运行如下命令：
dev/change-version-to-2.11.sh
mvn -Pyarn -Phadoop-2.4 -Dscala-2.11 -DskipTests clean package
编译后的spark包放在SPARK_HOME/assembly/target/scala-2.11。此时，如果运行sbin下的start-all.sh脚本，会提示找不到assembly/target/scala-2.10下的包，这是因为脚本仍然以2.10版本去启动spark。所以还需要在spark-env.sh中配置Scala版本：exportSPARK_SCALA_VERSION="2.11"
待这一切配置妥当，并针对正确版本进行编译后，通过start-all.sh启动spark，然后回到IntelliJ下运行前面的一段代码，从控制台中能够看到如下信息：
/05/19 21:07:20 INFO AppClient$ClientActor: Connecting to master akka.tcp://sparkMaster@192.168.1.4:7077/user/Master...
/05/19 21:07:20 INFO SparkDeploySchedulerBackend: Connected to Spark cluster with app ID app-20150519210720-0000
如同玩游戏升级打怪，眼看这一道关卡算是通过了，还未来得及喘一口气，前方路途又出状况了。控制台不讨好地又打印出如下错误信息：
Exception in thread "main" java.lang.ClassNotFoundException: org.postgresql.Driver at java.net.URLClassLoader.findClass(URLClassLoader.java:381) at java.lang.ClassLoader.loadClass(ClassLoader.java:424) at sun.misc.Launcher$AppClassLoader.loadClass(Launcher.java:331) at java.lang.ClassLoader.loadClass(ClassLoader.java:357) at java.lang.Class.forName0(Native Method) at java.lang.Class.forName(Class.java:264)
由于前面的代码利用Spark提供的JDBC API访问了PostgreSQL。我虽然在build.sbt中添加了对PostgreSQL驱动的依赖，且在local模式下运行正常；但在启动spark master时，并没有在classpath下添加驱动的jar包，导致访问数据库时，无法找到数据库驱动，从而抛出此异常。解决方案是在SPARK_HOME/bin/compute-classpath.sh中将数据库驱动追加到classpath下。我们可以考虑在SPARK_HOME下创建一个libs目录，专门用于存放程序需要的外部依赖jar包。现在，把PostgreSQL的驱动程序jar包拷贝到该目录下，然后在compute-classpath.sh脚本中增加如下配置：
appendToClasspath"$FWDIR/libs/postgresql-9.4-1201-jdbc41.jar"
重新启动spark，然后运行程序，就获得了在我看来相当美妙的结果。
本文出自：http://ju.outofmemory.cn/entry/162428





