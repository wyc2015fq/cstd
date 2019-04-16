# Hadoop与Spark以及那些坑 - lyx的专栏 - CSDN博客





2016年12月21日 20:29:00[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2563








这两天在搭建Hadoop与Spark的平台，要求是能够运行Spark，并且用python编程。笔者也不打算写一个很详细的细节教程，简单做一个笔记blog。

## 1.选择

        笔者一开始是在虚拟机上搭建的，创建了三个ubuntu虚拟机，然后开始布置分布式系统，但是，后来发现，资源完全不够用。笔者台式机16G内存，2T硬盘，i7第四代处理器，然而，还是被hadoop拖死。

        所以，笔者建议，用物理机器，而且只用一台。作为学习，我们用伪分布式完全就可以了。

## 2.安装Hadoop

    这个部分其实不是那么难，但是前提是你得有linux基础。基本就是那么几个步骤：

1 )查看自己的ip，指令ifconfig

2)修改/etc/hostname中设备的名字，比如master什么的

3)为以后的ssh做准备，最好也改一下/etc/hosts中的名称

4)把hadoop的文件包解压，推荐在Home下

5)安装JDK，其实就是解压。在/etc/profile中加入JDK的环境变量：

export JAVA_HOME=/opt/jdk1.6.0_45

export PATH=$PATH:$JAVA_HOME/bin


6)配置Hadoop，其实就是修改配置文件，配置文件在/etc下的hadoop里面。



```
1、hadoop-env.sh
export JAVA_HOME=/opt/modules/jdk1.6.0_45
2、yarn-env.sh
export JAVA_HOME=/opt/modules/jdk1.6.0_45
3、mapred-env.sh
export JAVA_HOME=/opt/modules/jdk1.6.0_45
4、core-site.xml
<property>		 
	<name>fs.default.name</name>
	<value>hdfs://hadoop-yarn.dragon.org:8020</value>
</property>

<property>
	<name>hadoop.tmp.dir</name>
	<value>/opt/modules/hadoop-2.2.0/data/tmp</value>
</property>
5、hdfs-site.xml
	<property>		
	<name>dfs.replication</name>
	<value>1</value>
	</property>
6、yarn-site.xml
	<property>
		<name>yarn.nodemanager.aux-services</name>
		<value>mapreduce_shuffle</value>
	</property>
			
7、mapred-site.xml
	<property>	 	        		
		<name>mapreduce.framework.name</name>
		<value>yarn</value>
	</property>
```
7)启动HDFS
NameNode、DataNode、SecondaryNameNode

* NameNode 格式化

bin/hdfs namenode -format

* 启动NameNode

sbin/hadoop-daemon.sh start namenode

* 启动DataNode

sbin/hadoop-daemon.sh start datanode

* 启动SecondaryNameNode

sbin/hadoop-daemon.sh start secondarynamenode



8)启动YARN

ResourceManager、NodeManager

* 启动ResourceManger

sbin/yarn-daemon.sh start resourcemanager

* 启动NodeManager

sbin/yarn-daemon.sh start nodemanager


    到这里其实我们已经配好了hadoop的结构了。做两遍之后，发现其实也是挺简单的。

## 3.运行hadoop及一些细节

       我们上面运行了HDFS和YARN之后，先来说明一下HDFS，YARN之间的关系。

       Hadoop体系下，其实有三大部分：分布式文件系统，YARN资源调度云操作系统，MapReduce计算框架。

       打开HDFS和YARN之后其实我们就可以开始进行MapReduce运算了。计算先放一下，hdfs和yarn运行起来了总得让我看见吧。

      hdfs分为三个部分，namenode、secondarynamenode、datanode。看名字也比较好理解了，分别是master节点，二级master节点和从机节点。

       为什么master的节点叫做namenode呢？因为master本质上就是记录下面每一个从机的资源和名字，然后根据这个来调度。说白了，Master就是一个花名册，当然就是namenode了。secondarynamenode是什么呢？花名册读取迅速，往往存在内存里面，那么掉电了怎么办？所以时不时的我们要备份一下，这个过程是secondarynamenode来完成的。datanode就不说了，数据节点嘛。

       那么YARN呢？yarn分为resourcemanager与nodemanager，HDFS是数据流，YARN下面则是资源流。资源管理者当然就是master了，节点管理那就是从机呗。

        我们怎么看这些东西有没有跑起来呢？jps这个查看守护进程的东西就可以看到。

有没有别的办法呢？有，通过web ui来看，直观很多。localhost：50070就是默认的HDFS的监控web页面，50090是secondarynamenode的；8088与8042则分别是resourcemansger与nodemanager的页面地址。

        既然我们有了HDFS的存储和YARN的操作系统，那么我们就可以跑起来了。

        在hadoop文件的bin下面，‘bin/hadoop jar share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar pi 2 10 ’ 这条语句，你就可以用蒙特卡洛方法来计算pi的值。如果最后算出来了， 那么就说明，整个体系初步通过了。

## 4.Spark安装与配置

        其实安装Spark也并不复杂。首先得安装Scala。我们去官网下载Scala的压缩包，scala-2.11.7.tgz这样的。

解压到一个文件夹，推荐在home下面建立一个tools文件夹，然后解压在里面。解压之后，配置~/.bash_profile环境变量。

export SCALA_HOME=/home/tools/scala

export PATH=$SCALA_HOME/bin:$PATH


        使用source ~/.bash_profile生效。


        然后在终端输入 scala -version，查看是否配置完成，然后输入scala即可以像python一样进行交互式编程了。

        同样的，我们去Spark官网上下载安装包，比如spark-1.4.0-bin-hadoop2.6.tgz，版本根据自己的hadoop版本选择，推荐使用2.6.0的hadoop，笔者试过，2.2.0的用2.3.0匹配的spark也可以。

同样的，下载之后解压到我们的tools里面，然后配置~/.bash_profile环境变量。




export SPARK_HOME=/home/tools/spark

export PATH=$SPARK_HOME/bin:$PATH


        接下来我们就要像配置hadoop一样配置Spark，还是那句话，其实就是改几个文件。

/conf/spark-env.sh中加入



export SPARK_HOME=/home/vm/tools/spark

export SCALA_HOME=/home/vm/tools/scala

export JAVA_HOME=/home/vm/tools/jdk

export SPARK_MASTER_IP=192.168.62.129

export SPARK_WORKER_MEMORY=512m



/conf/spark-defaults.conf中加入

spark.master spark://master的ip或者hostname

spark.serializer org.apache.spark.serializer.KryoSerializer


然后，cd /home/tools/spark/sbin start-all.sh就可以测试是否成功配置了。

## 5.python环境

        首先你要下载pycharm，下载社区版本的吧，专业版的要收钱，然后pip install一个py4j

        这里，我们要注意一下你这个py4j是安装在哪里的。笔者pip之后，被下载在了spark 下的lib里面，需要自己去解压出来放到上面一级目录才可以。

         装好pycharm后在安装目录的bin下打开pycharm.sh，然后开始一个新的项目，接下来在Run里面修改配置（Edit Configurations），我们加入spark的目录和spark下python两个目录就可以了，环境变量的名称分别是SPARK_NAME与PYTHONPATH，笔者怀疑不用这个也是可以的。

在新建的项目里新建一个python文件


```python
from pyspark import SparkContext, SparkConf
appName ="XXX" #你的应用程序名称
master= "local" 
conf = SparkConf().setAppName(appName).setMaster(master)
sc = SparkContext(conf=conf)
data = [1, 2, 3, 4, 5]
distData = sc.parallelize(data)
res = distData.reduce(lambda a, b: a + b)
print (res)
```





一个很简单的例子，能够计算data这个list中的数据和。




参考文章：

http://www.cnblogs.com/simplestupid/p/4679016.html


http://www.cnblogs.com/simplestupid/archive/2015/07/29/4684791.html


http://www.aboutyun.com/thread-6843-1-1.html

http://www.tuicool.com/articles/iAbInuj
















