
# Storm集群的安装及简单使用 - Magician的博客 - CSDN博客


2018年04月06日 18:23:57[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：112所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言：](#前言)
[1、集群部署的基本流程](#1集群部署的基本流程)
[2、Storm常用操作命令（了解）](#2storm常用操作命令了解)
[3、Storm集群的进程及日志熟悉（知道对应的日志的位置出了问题知道去哪找原因）](#3storm集群的进程及日志熟悉知道对应的日志的位置出了问题知道去哪找原因)
[4、Storm源码目录分析（重要）](#4storm源码目录分析重要)
[总结：](#总结)


# 目录
## 前言：
这里先简单说一下Storm的安装的主要步骤，还会介绍下Storm的常用的操作指令以及各个进程对应的日志信息，之后会简单介绍下Storm的源码目录。
## 1、集群部署的基本流程
**集群部署的流程：下载安装包、解压安装包、修改配置文件、分发安装包、启动集群**（其他集群部署通用的流程）
下载和解压安装包在这里就省略，直接进入修改配置文件
**1.3、修改配置文件**
vi /export/servers/storm/conf/storm.yaml
输入以下内容：
```python
#指定storm使用的zk集群
```
```python
storm.zookeeper.servers:
     -
```
```python
"shizhan"
```
```python
-
```
```python
"mini2"
```
```python
-
```
```python
"mini3"
```
```python
#指定storm本地状态保存地址
```
```python
storm.local.dir:
```
```python
"/root/apps/storm/workdir"
```
```python
#指定storm集群中的nimbus节点所在的服务器
```
```python
nimbus.host:
```
```python
"shizhan"
```
```python
#指定nimbus启动JVM最大可用内存大小
```
```python
nimbus.childopts:
```
```python
"-Xmx1024m"
```
```python
#指定supervisor启动JVM最大可用内存大小
```
```python
supervisor.childopts:
```
```python
"-Xmx1024m"
```
```python
#指定supervisor节点上，每个worker启动JVM最大可用内存大小
```
```python
worker.childopts:
```
```python
"-Xmx768m"
```
```python
#指定ui启动JVM最大可用内存大小，ui服务一般与nimbus同在一个节点上。
```
```python
ui.childopts:
```
```python
"-Xmx768m"
```
```python
#指定supervisor节点上，启动worker时对应的端口号，每个端口对应槽，每个槽位对应一个worker
```
```python
supervisor.slots.ports:
    -
```
```python
6700
```
```python
-
```
```python
6701
```
```python
-
```
```python
6702
```
```python
-
```
```python
6703
```
**1.4、分发安装包**
scp -r /export/servers/apache-storm-0.9.5 storm02:/export/servers
**1.5、启动集群(后台启动)**
**在nimbus.host所属的机器上启动 nimbus服务**
cd /export/servers/storm/bin/
nohup ./storm nimbus &
**在nimbus.host所属的机器上启动ui服务**
cd /export/servers/storm/bin/
nohup ./storm ui &
**在其它个点击上启动supervisor服务**
cd /export/servers/storm/bin/
nohup ./storm supervisor &
**1.6、查看集群**
访问nimbus.host:/8080，即可看到storm的ui界面。
![这里写图片描述](https://img-blog.csdn.net/20180406181455970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2、Storm常用操作命令（了解）
有许多简单且有用的命令可以用来管理拓扑，它们可以提交、杀死、禁用、再平衡拓扑。
提交任务命令格式：storm jar 【jar路径】 【拓扑包名.拓扑类名】 【拓扑名称】
bin/storm jar examples/storm-starter/storm-starter-topologies-0.9.6.jar storm.starter.WordCountTopology wordcount
杀死任务命令格式：storm kill 【拓扑名称】 -w 10（执行kill命令时可以通过-w [等待秒数]指定拓扑停用以后的等待时间）
storm kill topology-name -w 10
停用任务命令格式：storm deactivte  【拓扑名称】
storm deactivte topology-name
我们能够挂起或停用运行中的拓扑。当停用拓扑时，所有已分发的元组都会得到处理，但是spouts的nextTuple方法不会被调用。销毁一个拓扑，可以使用kill命令。它会以一种安全的方式销毁一个拓扑，首先停用拓扑，在等待拓扑消息的时间段内允许拓扑完成当前的数据流。
启用任务命令格式：storm activate【拓扑名称】
storm activate topology-name
重新部署任务命令格式：storm rebalance  【拓扑名称】
storm rebalance topology-name
再平衡使你重分配集群任务。这是个很强大的命令。比如，你向一个运行中的集群增加了节点。再平衡命令将会停用拓扑，然后在相应超时时间之后重分配工人，并重启拓扑。
3、Storm集群的进程及日志熟悉（知道对应的日志的位置出了问题知道去哪找原因）
3.1、部署成功之后，启动storm集群。
依次启动集群的各种角色
3.2、查看nimbus的日志信息
在nimbus的服务器上
cd /export/servers/storm/logs
tail -100f /export/servers/storm/logs/nimbus.log
3.3、查看ui运行日志信息
在ui的服务器上，一般和nimbus一个服务器
cd /export/servers/storm/logs
tail -100f /export/servers/storm/logs/ui.log
3.4、查看supervisor运行日志信息
在supervisor服务上
cd /export/servers/storm/logs
tail -100f /export/servers/storm/logs/supervisor.log
3.5、查看supervisor上worker运行日志信息
在supervisor服务上
cd /export/servers/storm/logs
tail -100f /export/servers/storm/logs/worker-6702.log
4、Storm源码目录分析（重要）
![这里写图片描述](https://img-blog.csdn.net/20180406181940568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180406181940568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
扩展包中的三个项目，使storm能与hbase、hdfs、kafka交互
![这里写图片描述](https://img-blog.csdn.net/20180406181947727?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
总结：
以上的一些知识只是作为一个了解，目的为了让你在以后的工作或学习中用到相关的技能时，能够直接上手。

[
](https://img-blog.csdn.net/20180406181940568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
