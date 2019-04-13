
# ubuntu16.04LTS下Spark完全分布式搭建 - lpty的博客 - CSDN博客

2017年01月24日 16:34:03[lpty](https://me.csdn.net/sinat_33741547)阅读数：3662


本文章spark是在[Hadoop2.7完全分布式环境](http://blog.csdn.net/sinat_33741547/article/details/54427113)下搭建的，相关配置如下：
1、ubuntu16.04LTS
2、jdk1.8._111
3、Hadoop2.7.3
一 工具准备
1.spark-2.1.0-bin-hadoop2.7.tgz([下载地址](http://spark.apache.org/downloads.html))
2.scala-2.12.1.tgz([下载地址](http://www.scala-lang.org/download/2.12.1.html))
二 环境搭建
1.scala安装
(1)解压scala-2.12.1.tgz到/usr/local/scala目录下
(2)增加环境变量

```python
root@master:~# gedit .bashrc
```
添加以下内容：

```python
export SCALA_HOME=/usr/local/scala/scala-2.12.1
export PATH=$PATH:$SCALA_HOME/bin
```
(3)刷新环境变量

```python
root@master:~# source .bashrc
```
2.spark安装
(1)解压spark-2.1.0-bin-hadoop2.7.tgz到/usr/local/spark目录下
(2)增加环境变量

```python
root@master:~# gedit .bashrc
```
添加以下内容：

```python
export SPARK_HOME=/usr/local/spark/spark-2.1.0-bin-hadoop2.7
export PATH=$PATH:$SPARK_HOME/bin
```
(3)刷新环境变量

```python
root@master:~# source .bashrc
```
(4)修改spark配置文件
1)$SPARK_HOME/conf/spark-env.sh

```python
cp spark-env.sh.template spark-env.sh
添加以下内容:
export SCALA_HOME=/usr/local/scala/scala-2.12.1
export JAVA_HOME=/usr/local/java/jdk1.8.0_111
export SPARK_MASTER_IP=master
export SPARK_WORKER_MEMORY=512m
export HADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop
```
2)$SPARK_HOME/conf/slaves

```python
cp slaves.template slaves
添加以下内容:
master
slave1
```
3.以上配置均在master节点进行，接下来将配置好的文件复制到slave节点，确保路径等均一致，不清楚的可以先看配置hadoop的文章
三 集群启动
1.启动spark集群

```python
cd $SPARK_HOME
sbin/start-all.sh
```
2.查看

```python
jps
```
3.结果
master节点:

```python
8608 Worker
8488 Master
8670 Jps
```
slave1节点：

```python
6737 Worker
6774 Jps
```
4.关闭集群

```python
sbin/stop-all.sh
```
到这里spark的配置就完成了，有兴趣可以查看我的另一篇博客，基于hdfs和spark的wordcount实例。

