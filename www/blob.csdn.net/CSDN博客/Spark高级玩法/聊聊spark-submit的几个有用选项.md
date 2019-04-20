# 聊聊spark-submit的几个有用选项 - Spark高级玩法 - CSDN博客
2018年06月19日 19:21:30[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：746

我们使用spark-submit时，必然要处理我们自己的配置文件、普通文件、jar包，今天我们不讲他们是怎么走的，我们讲讲他们都去了哪里，这样我们才能更好的定位问题。
我们在使用spark-submit把我们自己的代码提交到yarn集群运行时，spark会在yarn集群上生成两个进程角色，一个是driver，一个是executor，当这两个角色进程需要我们传递一些资源和信息时，我们往往会使用spark-submit的选项来进行传递。那么这些资源和信息，在使用spark-submit指定了之后，都去了哪里呢，为什么远在机房的driver和executor能正确的读到这些东东呢？为什么我明明按照spark-submit的帮助信息指定了这些东西，但是driver或者executor还是报错呢？本篇文章提供一个方法帮大家进行相关问题的定位。
Yarn配置
其实spark的driver和executor都是需要把这些资源拉取到其本地才能正常使用的，yarn为driver和executor都提供了container这样的资源容器来启动这些进程，但是container也是要和服务器绑定的，那么也就是说虽然driver和executor申请到一定的cpu和内存之后就能启动，但是他们也会涉及到和持久化存储打交道，那么我们就需要配置这样的本地磁盘目录，通知yarn中启动的container，如果涉及到文件，可以把这些文件暂存到哪里。这个配置信息在hadoop的core-site.xml中,就是hadoop.tmp.dir:
   <property>
      <name>hadoop.tmp.dir</name>
      <value>/Users/liyong/software/hadoop/hadoop-2.7.6/tmp</value>
      <description>A base for other temporary directories.</description>
    </property>
我们配置了这个目录之后，那么在远程服务器上，启动了container之后，这个目录就回作为container绑定的进程的工作目录了。
验证一下
为了让大家能立刻验证，我们不自己写代码，这样就不需要搭建环境啦打包啦这些乱七八糟的事情了，我们把spark的编译包下载下来就可以了，而且建议大家先在单机进行验证，这样就不用登录到集群其他节点了。首先来看最简单的例子：
    ./bin/spark-submit \
    --class org.apache.spark.examples.SparkPi \
    --master yarn \
    --num-executors 2 \
    examples/target/original-spark-examples_2.11-2.3.0.jar  100000
当我们把这个任务提交到yarn之后，我们去来观察一下刚才配置的yarn临时目录，这时会生成一个和本次提交的job相关的子目录：
 ./nm-local-dir/usercache/liyong/appcache/application_1529231285216_0012
其中最后一个application_1529231285216_0012正是我们本次job的applicationId, 进入这个目录之后，我们可以看到很多子目录，其中以container开头的，正是我们提交的job的driver和executor的工作目录，我们随便找一个来看看，其中又有很多子目录和子文件：
- 
**original-spark-examples_2.11-2.3.0.jar**
这个正是我们这次提交的job的驱动代码打包后的jar文件，已经通过网络发送了过来，供executor的jvm去进行类加载。
- 
**__spark_libs__ **
这个子目录，存储了spark这个计算框架本身依赖的一系列jar包，我们可以看到一共有240个jar包，那么我们回到spark工程根目录，看看assembly/target/scala-2.11/jars/这个目录，刚好240个jar包，说明spark在提交任务时，把自身依赖的jar包发送到了各个container的本地目录下，spark计算框架需要的文件系统、配置、网络、集群等相关的功能，都需要这些jar包的支持
- 
__spark_conf__ 
这个子目录，存储的正是我们指定的相关配置文件，包括两个： 
其中的__hadoop_conf__存储的正是我们通过HADOOP_CONF_DIR环境变量指定的hadoop相关配置文件，我们来框几个大家熟悉的： 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVBl4YkmGy8BwicC7FMsg581agQxTPFQDuOq36agLKhyHrFBfB57QbjTaV6pQT5nzzMsz9OS5omKNQ/640?wx_fmt=png)
大家可以把这些文件一一打开，和我们的hadoop客户端的配置文件进行对比一下。还有一个是__spark_conf__.properties文件正是我们的conf/spark-defaults.conf文件，不信你打开对比一下，其实也就是换了个马甲，大家一定要认识轮家啊。
--jars选项
**英文说明：**
    Comma-separated list of jars to include on the driver and executor classpaths.
**中文解释：**
    需要driver和executor能在其classpath下找到的jar包列表，也就是说，通过这个选项在spark客户端指定的jar包，会被发送到driver和executor所在节点的classpaths下。我们在进行spark应用开发时，时常会需要还是用到一些spark计算框架本身没有的依赖jar包，那么我们可以在使用maven或者IDE进行打包时，把需要的依赖都打包到一起，但这并非一种好的方式，因为这样的话应用包和依赖包耦合性太强，而且依赖比较多的话，我们的打包过程也会很慢，手动把这个包上传到服务器也会很慢，这就拖慢了我们的整个测试和验证流程，所以我们可以使用--jars这个选项，来让spark计算框架帮我们把需要的依赖进行分发。我们来验证一把：
    ./bin/spark-submit \
    --class org.apache.spark.examples.SparkPi \
    --master yarn \
    --num-executors 2 \
    --jars /Users/liyong/.m2/repository/org/eclipse/jetty/jetty-plus/9.3.20.    v20170531/jetty-plus-9.3.20.v20170531.jar \
    examples/target/original-spark-examples_2.11-2.3.0.jar  100000
在上边这个命令中，我们通过--jars指定了一个在driver端和executor端需要使用的jar包：jetty-plus-9.3.20.v20170531.jar，我们执行一下，然后去应用所在的目录查看一下，这时我们发现在每个container的本地目录下，jetty-plus-9.3.20.v20170531.jar这个包已经安全的躺在那里了，所以下次我们再遇到类找不到的问题，我们就可以去这个目录下看看，jvm类加载时需要的jar是否在这个目录下，如果不在，那就肯定会报类找不到的异常了，如果在，那么我们可以使用jar或者unzip命令解压开这个jar包看看到底有没有需要的class文件。妈妈再也不用担心我跑spark时找不到类啦！
--files选项
**英文说明：**
    Comma-separated list of files to be placed in the working directory of each executor. File paths of these files in executors can be accessed via SparkFiles.get(fileName).
**中文解释：**
这个选项指定的文件会被放置到executor的工作目录，这样的话executor就可以通过SparkFiles.get(fileName)这个方法返回这个文件在本地的绝对路径名，后边就可以通过各种方式来访问这个文件了。
    我们在编写spark应用时，除了需要给spark提供类加载使用的jar包依赖，有时也需要使用一些普通的文件资源，比如我们要做地理位置相关的开发，就需要使用IP地址包这样的文件；或者我们会使用hive的一些小表(一般是小的维度表)文件，在spark中和这些表的文件进行关联查询，那么spark就提供了--files这样的选项，来帮助我们完成这样的工作。注意，这里特别说明了,文件会被暂存在executor的工作目录下，并没有说被存储在driver的工作目录下，但是经过测试发现，driver和executor的工作目录下都能知道这个文件。我们来验证一下：
./bin/spark-submit \
--class org.apache.spark.examples.SparkPi \
--master yarn \
--num-executors 2 \
--jars /Users/liyong/.m2/repository/org/eclipse/jetty/jetty-plus/9.3.20.v20170531/jetty-plus-9.3.20.v20170531.jar \
 --files README.md \
examples/target/original-spark-examples_2.11-2.3.0.jar  100000
我们这个提交命令中，通过--files选型指定了一个客户端目录下的README.md文件，好了我们来执行一下:
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVBl4YkmGy8BwicC7FMsg581Jekv5rJZtxda8wZ6tYiaMVicaibwtJFKBzw1g8t2c2mRDV01fCLMP6ENA/640?wx_fmt=png)
我们可以看到，本地的三个container(包括driver所在的container)的工作目录下，都可以找到这个README.md文件了。
--properties-file选项
**英文说明：**
Path to a file from which to load extra properties. If not specified, this will look for conf/spark-defaults.conf.
** 中文解释**：
通过这个文件指定配置信息，如果没有指定，spark会使用conf/spark-defaults.conf这个文件作为默认的配置文件。好了，这个说明很明确了，我们只需要来验证一下即可：
 ./bin/spark-submit \
--class org.apache.spark.examples.SparkPi \
--master yarn \
--num-executors 2 \
--jars /Users/liyong/.m2/repository/org/eclipse/jetty/jetty-plus/9.3.20.v20170531/jetty-plus-9.3.20.v20170531.jar \
 --properties-file conf/myown-spark.conf \
 --files README.md \
examples/target/original-spark-examples_2.11-2.3.0.jar  100000
我们在spark客户端的conf目录下，把spark-default.conf文件拷贝一份，命名为myown-spark.conf，为了和spark-default.conf进行区分，我们在这个我们自己的配置文件中配置3个spark-default.conf里没有使用的配置项：
     spark.serializer  org.apache.spark.serializer.KryoSerializer
     spark.driver.memory              1g
     spark.executor.extraJavaOptions  -XX:+PrintGCDetails
提交一下,我们可以在临时目录的container的__spark_conf__/目录下找到__spark_conf__.properties这个文件，文件名还是和之前的一样，那么我们打开看看，我们自己配置的几个配置项是否在里边：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVBl4YkmGy8BwicC7FMsg581bQUibldM1dwicEVTPerhs6vv4SAGe1rias8GVWn9iaho4zWbo74sH9V1gQ/640?wx_fmt=png)
看到了吧，标红色被就是我们自己配置文件中的三个配置项。同时这里大家要注意，要使用spark的配置框架，所有的配置项都需要使用spark作为前缀才行，如果我们不想使用这样方式，那就需要配合--files选项，把我们自己的配置文件作为普通的资源文件防止到container的工作目录下，然后使用java或者scala的配置文件sdk去加载了。
好了，今天先介绍几个比较常用的选项，其他的选项大家也可以通过文章里的方式来验证一下。我们现在知道了spark在进行job提交时，我们配置的资源和依赖都去了哪里，那么下来就是要大家去自己探索一下这些资源和依赖都是怎么被发送到各个节点上去的了。
**更多spark学习和实际项目经验，请关注spark技术学院，BAT一线工程师为你答****疑解惑：**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFU7Oic5ibSSV5sJGB2RgzSeqQpuBicxOPgCBglwQPt9rnatXN0VblKhw9WicqSGsuTU9o0Sj12698QpuQ/640?wx_fmt=jpeg)
