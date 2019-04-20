# spark源码单步跟踪阅读-从毛片说起 - Spark高级玩法 - CSDN博客
2018年05月24日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：937
想当年读大学时，那时毛片还叫毛片，现在有文明的叫法了，叫小电影或者爱情动作片。那时宿舍有位大神，喜欢看各种毛片，当我们纠结于毛片上的马赛克时，大神大手一挥说道：这算啥，阅尽天下毛片，心中自然无码！突然想到我们在学习spark时，也可以有这种精神，当我们能读懂spark源码时，spark的技术世界也就真正为我们敞开了大门。台湾C++大师侯捷说过：源码面前，了无秘密！那我们就从如何单步调试spark源码开始讲起吧。
首先开发工具推荐大家选择IntelliJ，Intellij在和scala语言的结合上，比eclipse要好出太多了，其高效的文件索引机制，也可以让我们非常快速的定位源码。不要太担心快捷键的问题，IntelliJ为了拉eclipse的用户过来，可以将快捷键映射为eclipse常用快捷键，学习成本低了很多。
1.本地调试- 
app代码
package com.tencent.cubeli.sparksql
import org.apache.spark.{SparkConf, SparkContext}
import org.apache.spark.sql.SparkSession
object DataFrameCreate {
  def main(args: Array[String]): Unit = {
    val conf = new SparkConf().setMaster("local").setAppName("df create")
    val sc = new SparkContext(conf)
    val spark = SparkSession.builder().appName("Spark SQL basic example").config("spark.some.config.option", "some-value").getOrCreate()
    import spark.implicits._
    val df = spark.read.json("file:///Users/waixingren/bigdata-java/spark/sparkproj/data/nation.json")
    df.filter($"nationkey"<9).
     groupBy("regionkey").
     count.
      show()
   }
}
- 
在app中设置断点，如下图所示：
- 
调试，点下图的虫子标志
在提交了运行之后，程序会在断点处停止，等待单步调试，如下图所示：
点击step in，我们就可以进入到这个filter方法内部进行源码的跟踪了。
2.远程调试
当我们的代码提交到yarn上之后，就是一个分布式运行的模式了，这时就没法通过IDE去启动本地的单步调试，这时就需要使用远程调试方式。这种方式eclipse也有，并不是ideallij独有的，而是java语言为我们提供的，IDE工具只不过利用了这个特性而已。远程调试步骤如下：
- 
源码下载
我们后期在知识星球上陆陆续续的源码讲解，都是基于Spark 2.3.0这个版本，下载地址：
http://spark.apache.org/downloads.html
在下拉框选择2.3.0和Source Code
- 
源码编译
源码编译的主要目的是为了方便我们后期对spark的二次开发，如果没有二次开发的需求，知识想单步调试spark源码，那么这一步也是可以省略的。
对源码包进行解压，进入源码包根目录，编译命令：
- 
把这个编译 后的工程导入到intellij，步骤如下
一路点击next，可以根据实际情况进行修改，不修改也可以，最后点完成，执行spark源码导入，idealilij会进行该工程的依赖解析，等解析完成后就可以在package试图看到该工程的包试图，说明导入成功。
（导入后intellij要进行工程的依赖解析，但是因为我们之前进行过编译，需要的依赖包已经从远程maven仓库下载到了本地，所以这里的依赖接下没有下载过程，只是一个简单的的本地解析）
- 
配置远程调试
点击Edit Configurations...
点击“+”，选择remote
配置jvm远程调试选项，主要是host和port需要修改成spark进程所在的host和jmx端口号，如下图，可以修改“Name：”，“Host”，“Port”，这里的host一定要是spark运行的主机，port需要是远程没有被使用的端口，并且要记住此端口，待会要用。
配置完之后保存。
在spark-defaults.conf文件中添加以下配置：
spark.driver.extraJavaOptions  -Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=5005
spark.executor.extraJavaOptions  -Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=5005
关于这个配置的两点说明：
1) 其中的spark.driver.extraJavaOptions是用来调试driver的，spark.executor.extraJavaOptions是用来调试executor的，如果driver和executor不在同一个节点，那么端口可以相同，如果在同一个节点，那么端口不能一样。
2) 如果suspend=y，说明spark进程在启动时会挂起来，等待ideallij远程连接成功之后在继续启动；如果是n，那么不用等待ideallij远程连接，就启动，但是会一直监听这个端口，等待ideallij远程连接。
- 
远程连接并调试
     等spark进程挂起或者启动成功之后（根据suspend是y或者n来决定），在ideallij中设置断点，并启动刚才配置的远程连接，就可以连接到spark的jvm进程上，并进行远程的单步调试了 。
**总结：spark的学习一定要涉及源码的阅读，光学会使用api是写不好spark程序，要无法进行spark性能调优的。当你的断点停止在spark的api方法时，step in了，就有一个全新的技术世界为了敞开大门；step over了，你就一直是个码农。更多spark经验分享，请加入spark技术学院，bat大牛为你指点迷津！**
