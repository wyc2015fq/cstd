
# 大数据之Spark集群安装及简单使用 - Magician的博客 - CSDN博客


2018年04月07日 16:30:07[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：185所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)
[1、Spark集群安装](#1spark集群安装)[1.1. 安装](#11-安装)
[2、启动Spark Shell](#2启动spark-shell)[2.1、启动spark shell](#21启动spark-shell)
[2.2、在spark shell中编写WordCount程序](#22在spark-shell中编写wordcount程序)


# 目录
# 1、Spark集群安装
## 1.1. 安装
**1.1.1.    机器部署**
准备两台以上Linux服务器，安装好JDK1.7
**1.1.2.    下载Spark安装包**
![这里写图片描述](https://img-blog.csdn.net/20180407161459302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下载地址：[http://www.apache.org/dyn/closer.lua/spark/spark-1.5.2/spark-1.5.2-bin-hadoop2.6.tgz](http://www.apache.org/dyn/closer.lua/spark/spark-1.5.2/spark-1.5.2-bin-hadoop2.6.tgz)
上传解压安装包
上传spark-1.5.2-bin-hadoop2.6.tgz安装包到Linux上
解压安装包到指定位置
tar -zxvf spark-1.5.2-bin-hadoop2.6.tgz -C /usr/local
**1.1.3.    配置Spark**
进入到Spark安装目录
cd /usr/local/spark-1.5.2-bin-hadoop2.6
进入conf目录并重命名并修改spark-env.sh.template文件
cd conf/
mv spark-env.sh.template spark-env.sh
vi spark-env.sh
**在该配置文件中添加如下配置**
export JAVA_HOME=/usr/java/jdk1.7.0_45
export SPARK_MASTER_IP=node1
export SPARK_MASTER_PORT=7077
保存退出
重命名并修改slaves.template文件
mv slaves.template slaves
vi slaves
**在该文件中添加子节点所在的位置（Worker节点）**
node2
node3
node4
保存退出
**将配置好的Spark拷贝到其他节点上**
scp -r spark-1.5.2-bin-hadoop2.6/ node2:/usr/local/
scp -r spark-1.5.2-bin-hadoop2.6/ node3:/usr/local/
scp -r spark-1.5.2-bin-hadoop2.6/ node4:/usr/local/
**Spark集群配置完毕，目前是1个Master，3个Work，在node1.itcast.cn上启动Spark集群**
/usr/local/spark-1.5.2-bin-hadoop2.6/sbin/start-all.sh
启动后执行jps命令，主节点上有Master进程，其他子节点上有Work进行，登录Spark管理界面查看集群状态（主节点）：[http://node1:8080/](http://node1:8080/)
![这里写图片描述](https://img-blog.csdn.net/20180407162105950?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
到此为止，Spark集群安装完毕，但是有一个很大的问题，那就是Master节点存在单点故障，要解决此问题，就要借助zookeeper，并且启动至少两个Master节点来实现高可靠，配置方式比较简单：
Spark集群规划：node1，node2是Master；node3，node4，node5是Worker
安装配置zk集群，并启动zk集群
停止spark所有服务，修改配置文件spark-env.sh，**在该配置文件中删掉SPARK_MASTER_IP并添加如下配置**
export SPARK_DAEMON_JAVA_OPTS=”-Dspark.deploy.recoveryMode=ZOOKEEPER -Dspark.deploy.zookeeper.url=zk1,zk2,zk3 -Dspark.deploy.zookeeper.dir=/spark”
1.在node1节点上修改slaves配置文件内容指定worker节点
2.在node1上执行sbin/start-all.sh脚本，然后在node2上执行sbin/start-master.sh启动第二个Master
# 2、启动Spark Shell
spark-shell是Spark自带的交互式Shell程序，方便用户进行交互式编程，用户可以在该命令行下用scala编写spark程序。
## 2.1、启动spark shell
这里需要先启动对应的Spark集群
/root/apps/spark/bin/spark-shell  –master spark://shizhan:7077  –executor-memory 2g  –total-executor-cores 2
**参数说明：**
–master spark://shizhan:7077 指定Master的地址
–executor-memory 2g 指定每个worker可用内存为2G
–total-executor-cores 2 指定整个集群使用的cup核数为2个
注意：
如果启动spark shell时没有指定master地址，但是也可以正常启动spark shell和执行spark shell中的程序，其实是启动了spark的local模式，**该模式仅在本机启动一个进程，没有与集群建立联系。**
Spark Shell中已经默认将SparkContext类初始化为对象sc。用户代码如果需要用到，则直接应用sc即可
## 2.2、在spark shell中编写WordCount程序
```python
1.
```
```python
首先启动hdfs
```
```python
2.
```
```python
向hdfs上传一个文件到hdfs://
```
```python
192.168
```
```python
.112
```
```python
.200
```
```python
:
```
```python
9000
```
```python
/words
```
```python
.txt
```
```python
3.
```
```python
在spark shell中用scala语言编写spark程序
sc
```
```python
.textFile
```
```python
(
```
```python
"hdfs://192.168.112.200:9000/wordcount/input/README.txt"
```
```python
)
```
```python
.flatMap
```
```python
(_
```
```python
.split
```
```python
(
```
```python
" "
```
```python
))
```
```python
.map
```
```python
((_,
```
```python
1
```
```python
))
```
```python
.reduceByKey
```
```python
(_+_)
```
```python
.saveAsTextFile
```
```python
(
```
```python
"hdfs://192.168.112.200:9000/out1"
```
```python
)
```
```python
4.
```
```python
使用hdfs命令查看结果
hdfs dfs -ls hdfs://node1
```
```python
.itcast
```
```python
.cn
```
```python
:
```
```python
9000
```
```python
/
```
```python
out
```
```python
/p*
```
**说明：**
sc是SparkContext对象，该对象时提交spark程序的入口
textFile(hdfs://node1.itcast.cn:9000/words.txt)是hdfs中读取数据
flatMap(_.split(” “))先map在压平
map((_,1))将单词和1构成元组
reduceByKey(*+*)按照key进行reduce，并将value累加
saveAsTextFile(“hdfs://node1.itcast.cn:9000/out”)将结果写入到hdfs中
对于SparkSQL及SparkRDD的介绍请参考以下两篇文章：
SparkSQL：[https://blog.csdn.net/qq_16633405/article/details/79844593](https://blog.csdn.net/qq_16633405/article/details/79844593)
SparkRDD：[https://blog.csdn.net/qq_16633405/article/details/79843539](https://blog.csdn.net/qq_16633405/article/details/79843539)

