# Hadoop系列三：Hadoop之Pig篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月11日 09:50:08[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2512
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Pig安装和运行

Pig包括两部分：

—用于描述数据流的语言，称为Pig Latin。

—用于执行PigLatin程序的执行环境，当前有两个环境：单JVM中的本地执行环境和Hadoop集群上的分布式执行环境。

 Pig内部，每个操作或变换是对输入进行数据处理，然后产生输出结果，这些变换操作被转换成一系列MapReduce作业，Pig让程序员不需要知道这些转换具体是如何进行的，这样工程师可以将精力集中在数据上，而非执行的细节上。

1）下载解压

单节点Ubuntu，主机名hp，已部署Hadoop单节点伪分布。

官网[http://pig.apache.org/](http://pig.apache.org/)下载[pig-0.15.0.tar.gz](http://mirrors.hust.edu.cn/apache/pig/pig-0.15.0/pig-0.15.0.tar.gz)解压/home/hp下。

2）环境配置

$sudo gedit/etc/profile

添加如下：

#Hadoop

exportHADOOP_HOME=/home/hp/hadoop

export PIG_HOME=/home/hp/pig

export PIG_CLASSPATH=${HADOOP_HOME}/conf   --mapreduce模式才需要配置

exportPATH=$PATH:${HADOOP_HOME}/bin:${PIG_HOME}/bin

3）本地模式：

Grunt是Pig的外壳程序（shell）。本地模式下，Pig运行在单个JVM中，访问本地文件系统，该模式用于测试或处理小规模数据集。

$pig –Xlocal

4）Mapreduce模式：

在MapReduce模式下，Pig将查询翻译为MapReduce作业，然后在Hadoop集群上执行。

$pig 进入gruntshell 

grunt>help查看命令帮助

grunt>quit退出

grunt>shjps 执行操作系统命令jps

5）测试

运行示例：把Ubuntu下的/etc/passwd文件的第一列提取出来输出，用MapReduce模式跑。

Ø  启动Hadoop集群，$sbin/start-dfs.sh和$sbin/start-yarn.sh，使用jps查看进程。

Ø  首先把/etc/passwd文件put到hadoop的hdfs的input下：

$hadoopfs -put /etc/passwd /input

查看是否已上传到input目录下$hadoop fs –ls /input

Ø  进入Pigshell，运行命令，以':'分隔提取A，然后把A的第一列放入B，dump打出B：

$pig

grunt>A = load '/input/passwd' using PigStorage(':'); //注意等号两边要有空格，注意passwd路径

grunt>B = foreach A generate $0 as id;        

grunt>dump B;

       总结：主要在和hadoop结合配置上，grunt命令需要深入学习。Pig并不适合所有的数据处理任务，和MapReduce一样，它是为数据批处理而设计的，如果想执行的查询只涉及一个大型数据集的一小部分数据，Pig的实现不会很好，因为它要扫描整个数据集或其中很大一部分。

执行Pig程序的方法：

l   脚本：Pig可以运行包含Pig命令的脚本文件，例如，pig script.pig，对于很短的脚本可以通过使用-e选项直接在命令行中输入脚本字符串。

l   Grunt：Pigshell，就是上文的运行模式

l  嵌入式方法：也可以在Java中运行Pig程序，和使用JDBC运行SQL程序类似。Pig其实是对Java的Mapreduce的封装，进一步的抽象，运行的也是java程序，并在此基础上提供其他特性。
            


