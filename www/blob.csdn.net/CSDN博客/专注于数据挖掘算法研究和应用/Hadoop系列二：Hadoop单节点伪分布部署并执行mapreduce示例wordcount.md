# Hadoop系列二：Hadoop单节点伪分布部署并执行mapreduce示例wordcount - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月10日 17:06:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2499
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









HDFS在集群上实现分布式文件系统，MapReduce在集群上实现了分布式计算和任务处理。HDFS在MapReduce任务处理过程中提供了文件操作和存储等支持，MapReduce在HDFS的基础上实现了任务的分发、跟踪、执行等工作，并收集结果，二者相互作用，完成了Hadoop分布式集群的主要任务。

1）部署单节点伪分布

单节点Ubuntu，主机名hp

第一步：先安装JDK和SSH，并配置SSH无密码登录。

JDK解压安装在/home/hp下。

安装SSH：$sudo apt-get install ssh 

安装rsync：$sudo apt-get install rsync

生成密钥：$ssh-keygen–t rsa –f /home/hp/.ssh/id_rsa 

授权访问：$cat/home/hp/.ssh/id_rsa.pub>>/home/hp/.ssh/authorized_keys

测试：sshlocalhost

第二步：Hadoop解压在/home/hp并配置core-site.xml和hdfs-site.xml

创建如下文件夹

$mkdir tmp

$mkdir hdfs

$mkdir hdfs/data

$mkdir hdfs/name

设置$geditetc/hadoop/core-site.xml

fs.defaultFS值为hdfs://hp:9000

<property>

       <name>fs.defaultFS</name>

       <value>hdfs://hp:9000</value>

</property>

<property>

       <name>hadoop.tmp.dir</name>

       <value>file:/home/hp/hadoop/tmp</value>

   </property>

设置$geditetc/hadoop/hdfs-site.xml

dfs.replication值为1

<property>

       <name>dfs.namenode.name.dir</name>

       <value>/home/hp/hadoop/dfs/name</value>

</property>

<property>

       <name>dfs.datanode.data.dir</name>

       <value>/home/hp/hadoop/dfs/data</value>

   </property>

<property>

       <name>dfs.replication</name>

       <value>1</value>

</property>

<property>

       <name>dfs.namenode.secondary.http-address</name>

       <value>hp:9001</value>

</property>

设置hosts：$sudo gedit /etc/hosts

注释所有行， 追加：

127.0.0.1 hp

127.0.0.1localhost

设置etc/hadoop/hadoop-env.sh的Java路径

$bin/hdfsnamenode –format 

启动集群$sbin/start-dfs.sh

[http://127.0.0.1:50070](http://127.0.0.1:50070/) 打开页面验证单节点伪分布已成功。

第三步：如要运行在yarn上，则配置mapred-site.xml和yarn-site.xml

$geditetc/hadoop/mapred-site.xml

mapreduce.framework.name值为yarn

<property>

      <name>mapreduce.framework.name</name>

       <value>yarn</value>

   </property>

$geditetc/hadoop/yarn-site.xml

yarn.nodemanager.aux-services值为mapreduce_shuffle

<property>

      <name>yarn.nodemanager.aux-services</name>

       <value>mapreduce_shuffle</value>

   </property>

设置etc/hadoop/yarn-env.sh的Java路径

启动yarn$sbin/start-yarn.sh

[http://127.0.0.1:8088](http://127.0.0.1:8088/) 验证启动成功。

2）运行wordcount作业-使用yarn

第一步：在HDFS中创建input文件目录：$bin/hadoop fs –mkdir /input 

第二步：上传文件到hdfs的input目录下

$bin/hadoopfs –put LICENSE.txt /input

本文这里用hadoop安装目录下的LICENSE.txt文件作为wordcount的输入。

$bin/hadoopfs -ls /input

查看文件是否正确传入到/input目录下

$bin/hadoopfs -cat /input/LICENSE.txt查看文件内容

第三步：执行

$bin/hadoop jar

/home/hp/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.7.1.jarwordcount /input /output

第四步：查看结果

$bin/hadoopfs -ls /output

查看输出结果的目录

$bin/hadoopfs -cat /output/part-r-00000

3）运行wordcount作业-不使用yarn

如果不使用yarn,进行mapreduce.job.tracker配置用MRv1来执行job，不需要启动nodemanager。

设置$geditetc/hadoop/mapred-site.xml

<property>

       <name>mapreduce.job.tracker</name>

       <value>hdfs://hp:8001</value>

       <final>true</final>

<property>

启动sbin/start-dfs.sh和sbin/start-yarn.sh后执行

$bin/hadoop jar

/home/hp/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.7.1.jarwordcount /input /output2

查看结果

$bin/hadoopfs -ls /output2

查看输出结果的目录

$bin/hadoopfs -cat /output2/part-r-00000

关闭sbin/stop-dfs.sh和sbin/stop-yarn.sh




总结：过程中，面对不断出现的错误需要查阅资料并调整配置，慢慢理解到整体框架，但深入研究还有待时日。



