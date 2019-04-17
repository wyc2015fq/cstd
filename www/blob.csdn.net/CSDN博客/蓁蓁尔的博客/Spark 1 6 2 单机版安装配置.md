# Spark 1.6.2 单机版安装配置 - 蓁蓁尔的博客 - CSDN博客





2016年10月19日 21:24:00[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：1998








本文将介绍Apache Spark 1.6.2在单机的部署，与在集群中部署的步骤基本一致，只是少了一些master和slave文件的配置。直接安装scala与Spark就可以在单机使用，但如果用到hdfs系统的话hadoop和jdk也要配置，建议全部安装配置好。转载请注明博客原文地址：[http://blog.tomgou.xyz/spark-160-dan-ji-an-zhuang-pei-zhi.html](http://blog.tomgou.xyz/spark-160-dan-ji-an-zhuang-pei-zhi.html)

**1. Spark的安装准备**

Spark官网的文档 [http://spark.apache.org/docs/latest/](http://spark.apache.org/docs/latest/) 里是这样说的： 

Spark runs on Java 7+, Python 2.6+ and R 3.1+. For the Scala API, Spark 1.6.2 uses Scala 2.10. You will need to use a compatible Scala version (2.10.x). 

我的电脑环境是Ubuntu 14.04.4 LTS，还需要安装：
> 
[jdk-8u73-linux-x64.tar.gz](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
[hadoop-2.6.0.tar.gz](http://apache.claz.org/hadoop/common/hadoop-2.6.0/hadoop-2.6.0.tar.gz)
[scala-2.10.4.tgz](http://www.scala-lang.org/download/2.10.4.html)
[spark-1.6.2-bin-hadoop2.6.tgz](http://spark.apache.org/downloads.html)
**补充：**

（1）.最好新建一个目录，将上述文件都放在放在同一个文件夹中。 

（2）.因为之后要安装最新版本的IntelliJ IDEA，IDEA 2016只支持jdk1.8或更高版本，为防止之后麻烦，此处最好不要用jdk1.7及以下。 
**2. 安装jdk**

解压jdk安装包到任意目录：
```
cd /home/tom
$ tar -xzvf jdk-8u73-linux-x64.tar.gz
$ sudo vim /etc/profile
```

编辑/etc/profile文件，在最后加上java环境变量：

```bash
export JAVA_HOME=/home/tom/jdk1.8.0_73/
export JRE_HOME=/home/tom/jdk1.8.0_73/jre
export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH
export CLASSPATH=.:$JAVA_HOME/lib:$JRE_HOME/lib:$CLASSPATH
```

保存并更新/etc/profile：

```bash
$ source /etc/profile
```

查看是否成功：
`$ java -version`
**3. 配置ssh localhost**

确保安装好ssh：

```bash
$ sudo apt-get update
$ sudo apt-get install openssh-server
$ sudo /etc/init.d/ssh start
```

生成并添加密钥：

```
$ ssh-keygen -t rsa
$ cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
$ chmod 0600 ~/.ssh/authorized_keys
```

如果已经生成过密钥，只需执行后两行命令。 测试ssh localhost

```
$ ssh localhost
$ exit
```

**补充：**

此处若出现下类错误：

> 
下列软件包有未满足的依赖关系： openssh-server : 依赖: openssh-client (= 1:5.1p1-6ubuntu2)


**解决：**因为openssh-server是依赖于openssh-clien的,那ubuntu不是自带了openssh-client吗?原由是自带的openssh-clien与所要安装的openssh-server所依赖的版本不同,这里所依赖的版本是1:6.6p1-2ubuntu1所以要安装对应版本的openssh-clien,来覆盖掉ubuntu自带的：
`$ sudo apt-get install openssh-client=1:6.6p1-2ubuntu1`
然后再继续上述步骤，安装openssh-server。 
**4. 安装hadoop2.6.0**

解压hadoop2.6.0到任意目录：
```
$ cd /home/tom
$ wget http://apache.claz.org/hadoop/common/hadoop-2.6.0/hadoop-2.6.0.tar.gz
$ tar -xzvf hadoop-2.6.0.tar.gz
```

编辑/etc/profile文件，在最后加上java环境变量：

```bash
export HADOOP_HOME=/home/tom/hadoop-2.6.0
export HADOOP_INSTALL=$HADOOP_HOME
export HADOOP_MAPRED_HOME=$HADOOP_HOME
export HADOOP_COMMON_HOME=$HADOOP_HOME
export HADOOP_HDFS_HOME=$HADOOP_HOME
export YARN_HOME=$HADOOP_HOME
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
export PATH=$PATH:$HADOOP_HOME/sbin:$HADOOP_HOME/bin
```

编辑$HADOOP_HOME/etc/hadoop/hadoop-env.sh文件
`$ vim $HADOOP_HOME/etc/hadoop/hadoop-env.sh`
在最后加上：
`export JAVA_HOME=/home/tom/jdk1.8.0_73/`
修改Configuration文件：

```bash
$ cd $HADOOP_HOME/etc/hadoop
```

修改core-site.xml：

```xml
<configuration>
<property>
  <name>fs.default.name</name>
    <value>hdfs://localhost:9000</value>
</property>
</configuration>
```

修改hdfs-site.xml：

```xml
<configuration>
<property>
 <name>dfs.replication</name>
 <value>1</value>
</property>
<property>
  <name>dfs.name.dir</name>    <value>file:///home/tom/hadoopdata/hdfs/namenode</value>
</property>
<property>
  <name>dfs.data.dir</name>
    <value>file:///home/tom/hadoopdata/hdfs/datanode</value>
</property>
</configuration>
```

第一个是dfs的备份数目，单机用1份就行，后面两个是namenode和datanode的目录。 

修改mapred-site.xml：

```xml
<configuration>
 <property>
  <name>mapreduce.framework.name</name>
   <value>yarn</value>
 </property>
</configuration>
```

修改yarn-site.xml：

```xml
<configuration>
 <property>
  <name>yarn.nodemanager.aux-services</name>
    <value>mapreduce_shuffle</value>
 </property>
</configuration>
```

初始化hadoop：
`$ hdfs namenode -format`
启动
`$ $HADOOP_HOME/sbin/start-all.sh`
停止
`$ $HADOOP_HOME/sbin/stop-all.sh`
检查WebUI，浏览器打开端口：[http://localhost:8088](http://localhost:8088)

> 
port 8088: cluster and all applications 

  port 50070: Hadoop NameNode 

    port 50090: Secondary NameNode 

  port 50075: DataNode
hadoop运行后可使用jps命令查看,若得到一下结果则安装没问题：

```
10057 Jps
9611 ResourceManager
9451 SecondaryNameNode
9260 DataNode
9102 NameNode
9743 NodeManager
```

**5. 安装scala**

解压scala安装包到任意目录：

```
$ cd /home/tom
 $ tar -xzvf scala-2.10.4.tgz
 $ sudo vim /etc/profile
```

在/etc/profile文件的末尾添加环境变量：

```bash
export SCALA_HOME=/home/tom/scala-2.10.4
export PATH=$SCALA_HOME/bin:$PATH
```

保存并更新/etc/profile：

```bash
$ source /etc/profile
```

查看是否成功：
`$ scala -version`
**6. 安装Spark**

解压spark安装包到任意目录：

```
$ cd /home/tom
$ tar -xzvf spark-1.6.2-bin-hadoop2.6.tgz
$ mv spark-1.6.2-bin-hadoop2.6 spark-1.6.2
$ sudo vim /etc/profile
```

在/etc/profile文件的末尾添加环境变量：

```bash
export SPARK_HOME=/home/tom/spark-1.6.2
export PATH=$SPARK_HOME/bin:$PATH
```

保存并更新/etc/profile：

```bash
$ source /etc/profile
```

在conf目录下复制并重命名spark-env.sh.template为spark-env.sh：

```
$ cp spark-env.sh.template spark-env.sh
$ vim spark-env.sh
```

在spark-env.sh中添加：

```
export JAVA_HOME=/home/tom/jdk1.8.0_73/
export SCALA_HOME=/home/tom//scala-2.10.4
export SPARK_MASTER_IP=localhost
export SPARK_WORKER_MEMORY=4G
```

启动
`$ $SPARK_HOME/sbin/start-all.sh`
停止
`$ $SPARK_HOME/sbin/stop-all.sh`
测试Spark是否安装
`$ $SPARK_HOME/bin/run-example SparkPi`
结果：
`Pi is roughly 3.14716`
检查WebUI，浏览器打开端口：[http://localhost:8080](http://localhost:8080)










