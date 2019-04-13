
# HDFS的简介及基本操作（常用的命令参数介绍） - Magician的博客 - CSDN博客


2017年10月30日 11:29:27[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：307标签：[hdfs																](https://so.csdn.net/so/search/s.do?q=hdfs&t=blog)[简介																](https://so.csdn.net/so/search/s.do?q=简介&t=blog)[原理																](https://so.csdn.net/so/search/s.do?q=原理&t=blog)[命令行																](https://so.csdn.net/so/search/s.do?q=命令行&t=blog)[基本操作																](https://so.csdn.net/so/search/s.do?q=基本操作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=命令行&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=原理&t=blog)个人分类：[大数据																](https://blog.csdn.net/qq_16633405/article/category/6905043)[HDFS																](https://blog.csdn.net/qq_16633405/article/category/7258212)[
							](https://blog.csdn.net/qq_16633405/article/category/6905043)
所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)
[
																	](https://so.csdn.net/so/search/s.do?q=原理&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=简介&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=简介&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=hdfs&t=blog)

[目录](#目录)[前言：](#前言)
[1、HDFS基本概念](#1hdfs基本概念)
[2、HDFS基本操作](#2hdfs基本操作)
[总结：](#总结)


# 目录
## 前言：
总算有空来接着写大数据的学习笔记了，今天就把之前学过的HDFS的基础知识详细的介绍一下，如有哪点写的不足希望大家多多指教。
## 1、HDFS基本概念
1.1、前言：
**设计思想**
分而治之：将大文件、大批量文件，分布式存放在大量服务器上，以便于采取分而治之的方式对海量数据进行运算分析。
**在大数据系统中作用：**
为各类分布式运算框架（如：mapreduce，spark，tez，……）提供数据存储服务。
**重点概念：**文件切块，副本存放，元数据。
1.2、 HDFS的概念和特性：
首先，它是一个文件系统，用于存储文件，通过统一的命名空间——目录树来定位文件；
其次，它是分布式的，由很多服务器联合起来实现其功能，集群中的服务器有各自的角色；
**重要特性如下：**
（1）HDFS中的文件在物理上是**分块存储（block）**，块的大小可以通过配置参数( dfs.blocksize)来规定，默认大小在hadoop2.x版本中是**128M**，老版本中是64M
（2）HDFS文件系统会给客户端提供一个统一的抽象目录树，客户端通过路径来访问文件，形如：hdfs://namenode:port/dir-a/dir-b/dir-c/file.data
（3）目录结构及文件分块信息(**元数据**)的管理由namenode节点承担
——namenode是HDFS集群主节点，负责维护整个hdfs文件系统的目录树，以及每一个路径（文件）所对应的block块信息（block的id，及所在的datanode服务器）
（4）文件的各个block的存储管理由datanode节点承担
—- datanode是HDFS集群从节点，每一个block都可以在多个datanode上存储多个副本（副本数量也可以通过参数设置dfs.replication）
（5）HDFS是设计成适应一次写入，多次读出的场景，且不支持文件的修改
(注：适合用来做数据分析，并不适合用来做网盘应用，因为，不便修改，延迟大，网络开销大，成本太高)
## 2、HDFS基本操作
**2.1、HDFS的shell(命令行客户端)操作**
HDFS提供shell命令行客户端，使用方法如下：
![这里写图片描述](https://img-blog.csdn.net/20171030114432317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171030114432317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2.2 常用命令参数介绍**
[
](https://img-blog.csdn.net/20171030114432317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)-help  功能：输出这个命令参数手册
-ls  功能：显示目录信息 示例： hadoop fs -ls hdfs://hadoop-server01:9000/ 备注：这些参数中，所有的hdfs路径都可以简写 –>hadoop fs -ls / 等同于上一条命令的效果
-mkdir  功能：在hdfs上创建目录 示例：hadoop fs -mkdir -p /aaa/bbb/cc/dd
-moveFromLocal  功能：从本地剪切粘贴到hdfs 示例：hadoop fs - moveFromLocal /home/hadoop/a.txt /aaa/bbb/cc/dd
-moveToLocal  功能：从hdfs剪切粘贴到本地 示例：hadoop fs - moveToLocal /aaa/bbb/cc/dd /home/hadoop/a.txt
–appendToFile 功能：追加一个文件到已经存在的文件末尾示例：hadoop fs -appendToFile ./hello.txt hdfs://hadoop-server01:9000/hello.txt可以简写为：Hadoop fs -appendToFile ./hello.txt /hello.txt
-cat  功能：显示文件内容  示例：hadoop fs -cat /hello.txt
-text  功能：以字符形式打印一个文件的内容 。示例：hadoop fs -text /weblog/access_log.1
-chgrp  -chmod -chown 功能：linux文件系统中的用法一样，对文件所属权限 示例： hadoop fs -chmod 666 /hello.txt hadoop fs -chown someuser:somegrp /hello.txt
-copyFromLocal  功能：从本地文件系统中拷贝文件到hdfs路径去 示例：hadoop fs -copyFromLocal ./jdk.tar.gz /aaa/ -copyToLocal  功能：从hdfs拷贝到本地 示例：hadoop fs
-copyToLocal /aaa/jdk.tar.gz
-cp  功能：从hdfs的一个路径拷贝hdfs的另一个路径 示例： hadoop  fs  -cp /aaa/jdk.tar.gz /bbb/jdk.tar.gz.2
-mv  功能：在hdfs目录中移动文件 示例： hadoop  fs  -mv
/aaa/jdk.tar.gz /
-get  功能：等同于copyToLocal，就是从hdfs下载文件到本地 示例：hadoop fs -get /aaa/jdk.tar.gz -getmerge  功能：合并下载多个文件 示例：比如hdfs的目录
/aaa/下有多个文件:log.1, log.2,log.3,… hadoop fs -getmerge /aaa/log.*
./log.sum
-put  功能：等同于copyFromLocal 示例：hadoop  fs  -put /aaa/jdk.tar.gz /bbb/jdk.tar.gz.2
-rm  功能：删除文件或文件夹 示例：hadoop fs -rm -r /aaa/bbb/
-rmdir  功能：删除空目录 示例：hadoop fs -rmdir /aaa/bbb/ccc
-df  功能：统计文件系统的可用空间信息 示例：hadoop  fs  -df -h  /   -
-du  功能：统计文件夹的大小信息 示例： hadoop fs -du -s -h /aaa/*
-count  功能：统计一个指定目录下的文件节点数量 示例：hadoop fs -count /aaa/
-setrep  功能：设置hdfs中文件的副本数量 示例：hadoop fs -setrep 3 /aaa/jdk.tar.gz <这里设置的副本数只是记录在namenode的元数据中，是否真的会有这么多副本，还得看datanode的数量>
总结：
对于HDFS来讲，如果想达到博主一样的菜鸟级别的水平，必须得知道以下的几个知识点：
首先知道HDFS是什么；其次知道三个主要概念即文件切块、副本存放、元数据；然后要知道HDFS中对应的角色即NameNode、Datanode的工作机制和职责以及HDFS读写数据的原理；最后要掌握常用的HDFS-shell命令行。个人感觉如果知道以上的一些知识点，HDFS应该算是入门了吧。
接下来将介绍下HDFS的客户端应用开发等。（
详见：[https://blog.csdn.net/qq_16633405/article/details/78403316](https://blog.csdn.net/qq_16633405/article/details/78403316)）

