
# Zookeeper的简介及命令行操作 - Magician的博客 - CSDN博客


2018年03月30日 10:53:00[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：76所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)
[前言](#前言)[1、Zookeeper简介](#1zookeeper简介)
[2、Zookeeper结构](#2zookeeper结构)
[3、Zookeeper常用的命令行操作](#3zookeeper常用的命令行操作)
[总结：](#总结)

# 目录
# 前言
作为一款第三方的协调服务框架，ZK被应用在许多地方，如：Hbase中用于存储-ROOT表的位置信息；Storm中用于保存任务分配的信息、心跳信息、元数据信息等。只要是在整个设计中需要一个第三方的组件来满足“元数据的存储、提供监听服务”的需求，ZK都可以满足。接下来就简单介绍下ZK。
## 1、Zookeeper简介
Zookeeper**是一个分布式协调服务**；就是**为用户的分布式应用程序提供协调服务**
A、zookeeper是为别的分布式程序服务的
B、Zookeeper本身就是一个分布式程序（**只要有半数以上节点存活，ZK就能正常服务）**
C、Zookeeper所提供的服务涵盖：主从协调、服务器节点动态上下线、统一配置管理、分布式共享锁、统一名称服务……
D、虽然说可以提供各种服务，**但是zookeeper在底层其实只提供了两个功能：**
管理(存储，读取)用户程序提交的数据（状态数据）；
并为用户程序提供数据节点监听服务；
**Zookeeper常用应用场景：**
1、服务器集群上的采集程序需要实时从相关的服务器拿数据，若服务器挂了，会造成损失。需要一个第三方来记录每台服务器的采集数据的状态信息和监听每台服务器的状态，以及提供选举策略来支撑由那台服务器来接替挂了的服务器。
2、若几台服务器的一些配置文件相同，可以将这些文件放置到第三方上，通过第三方来传输这些文件到相应的服务器上（更新配置文件时也只需更新第三方上的一个配置文件即可）。
**Zookeeper集群的角色：  Leader 和  follower  （Observer）**
**Zookeeper特性：**
1、Zookeeper：一个leader，多个follower组成的集群
2、全局数据一致：每个server保存一份相同的数据副本，client无论连接到哪个server，数据都是一致的
3、分布式读写，更新请求转发，由leader实施
4、更新请求顺序进行，来自同一个client的更新请求按其发送顺序依次执行
5、**数据更新原子性**，一次数据更新要么成功，要么失败
6、实时性，在一定时间范围内，client能读到最新数据
## 2、Zookeeper结构
**2.1、Zookeeper数据结构：**
1、层次化的目录结构，命名符合常规文件系统规范(见下图)
2、每个节点在zookeeper中叫做znode,并且其有一个唯一的路径标识
3、节点Znode可以包含数据和子节点（但是EPHEMERAL类型的节点不能有子节点，下一页详细讲解）
4、客户端应用可以在节点上设置监视器（后续详细讲解）
**2.2、数据结构的图**
![这里写图片描述](https://img-blog.csdn.net/20180330102215462?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2.3、节点类型**
1、Znode有**两种类型**：
短暂（ephemeral）（断开连接自己删除）
持久（persistent）（断开连接不删除）
2、Znode有四种形式的目录节点（默认是persistent ）（带序列和不带序列的）
PERSISTENT
PERSISTENT_SEQUENTIAL（持久序列/test0000000019 ）
EPHEMERAL
EPHEMERAL_SEQUENTIAL
3、创建znode时设置顺序标识，znode名称后会附加一个值，顺序号是一个单调递增的计数器，由父节点维护
4、在分布式系统中，顺序号可以被用于为所有的事件进行全局排序，这样客户端可以通过顺序号推断事件的顺序
## 3、Zookeeper常用的命令行操作
```python
运行 zkCli.sh –server
```
```python
<
```
```python
ip
```
```python
>
```
```python
进入命令行工具
```
![这里写图片描述](https://img-blog.csdn.net/20180330104044116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
1
```
```python
、使用 ls 命令来查看当前 ZooKeeper 中所包含的内容：
[zk:
```
```python
202.115
```
```python
.
```
```python
36.251
```
```python
:
```
```python
2181
```
```python
(CONNECTED)
```
```python
1
```
```python
] ls /
```
```python
2
```
```python
、创建一个新的 znode ，使用 create /zk myData 。这个命令创建了一个新的 znode 节点“ zk ”以及与它关联的字符串：
[zk:
```
```python
202.115
```
```python
.
```
```python
36.251
```
```python
:
```
```python
2181
```
```python
(CONNECTED)
```
```python
2
```
```python
] create /zk
```
```python
"myData“
```
```python
3
```
```python
、我们运行 get 命令来确认 znode 是否包含我们所创建的字符串：
[zk:
```
```python
202.115
```
```python
.
```
```python
36.251
```
```python
:
```
```python
2181
```
```python
(CONNECTED)
```
```python
3
```
```python
] get /zk
```
```python
#监听这个节点的变化,当另外一个客户端改变/zk时（这里只是改变/zk中的数据内容，当文件出现增删时用ls /zk watch）,它会打出下面的
```
```python
#WATCHER::
```
```python
#WatchedEvent state:SyncConnected type:NodeDataChanged path:/zk
```
```python
[zk: localhost:
```
```python
2181
```
```python
(CONNECTED)
```
```python
4
```
```python
] get /zk watch
```
```python
4
```
```python
、下面我们通过 set 命令来对 zk 所关联的字符串进行设置：
[zk:
```
```python
202.115
```
```python
.
```
```python
36.251
```
```python
:
```
```python
2181
```
```python
(CONNECTED)
```
```python
4
```
```python
] set /zk
```
```python
"zsl“
```
```python
5
```
```python
、下面我们将刚才创建的 znode 删除：
[zk:
```
```python
202.115
```
```python
.
```
```python
36.251
```
```python
:
```
```python
2181
```
```python
(CONNECTED)
```
```python
5
```
```python
] delete /zk
```
```python
6
```
```python
、删除节点：rmr
[zk:
```
```python
202.115
```
```python
.
```
```python
36.251
```
```python
:
```
```python
2181
```
```python
(CONNECTED)
```
```python
5
```
```python
] rmr /zk
```
# 总结：
关于ZK你应该知道以下几个点：
1、知道是什么；干什么用的。
2、会用。
3、知道内部的工作原理。
如果你知道以上的知识，那么恭喜你已经达到和博主一样的小菜鸟的级别。接下来会再介绍下ZK内部的选举机制
（详见：[https://blog.csdn.net/qq_16633405/article/details/79754728](https://blog.csdn.net/qq_16633405/article/details/79754728)）。

