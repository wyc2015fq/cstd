# Flume NG：Flume 发展史上的第一次革命 - Spark高级玩法 - CSDN博客
2018年10月05日 00:04:04[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：43
**背景**
Cloudera 开发的分布式日志收集系统 Flume，是 hadoop 周边组件之一。其可以实时的将分布在不同节点、机器上的日志收集到 hdfs中。Flume 初始的发行版本目前被统称为 Flume OG（original generation），属于 cloudera。但随着 FLume功能的扩展，Flume OG 代码工程臃肿、核心组件设计不合理、核心配置不标准等缺点暴露出来，尤其是在 Flume OG 的最后一个发行版本0.94.0 中，日志传输不稳定的现象尤为严重，这点可以在 BigInsights 产品文档的 troubleshooting板块发现。为了解决这些问题，2011 年 10 月 22 号，cloudera 完成了 Flume-728，对 Flume进行了里程碑式的改动：重构核心组件、核心配置以及代码架构，重构后的版本统称为 Flume NG（next generation）；改动的另一原因是将 Flume 纳入 apache 旗下，cloudera Flume 改名为 Apache Flume。
下面将从核心组件变化、角色变化、用户配置变化以及实战等方面阐述 Flume NG 相对于 Flume OG 所发生的革命性变化。
## **核心组件变化**
图 1 和图 3 是两个版本的架构图。
FLUM OG 的特点是：
- 
FLUM OG 有三种角色的节点，如图1：代理节点（agent）、收集节点（collector）、主节点（master）。                 
- 
agent 从各个数据源收集日志数据，将收集到的数据集中到 collector，然后由收集节点汇总存入hdfs。master 负责管理 agent，collector 的活动。       
- 
agent、collector 都称为 node，node 的角色根据配置的不同分为 logical node（逻辑节点）、physical node（物理节点）。对 logical nodes 和 physical nodes的区分、配置、使用一直以来都是使用者最头疼的地方。     
- 
agent、collector 由 source、sink 组成，代表在当前节点数据是从 source 传送到sink。如图 2。
                
##### 图 1. FLUM OG 架构图
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGXyl7OSqGloQlHOpUKQNrJzzlNyicYvWWqgM27b8C1n3dyCY61U40wQg/640)
图 2. OG 节点组成图
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGNDYhrYyLuliax6cNytqmnG5f5dh0SV4SwJS1Ss1OMm95icBUhr3xNHeQ/640)
对应于 OG 的特点，FLUM NG 的特点是：
- 
NG 只有一种角色的节点：代理节点（agent）。
- 
没有 collector、master 节点。这是核心组件最核心的变化。  
- 
去除了 physical nodes、logical nodes 的概念和相关内容。                 
- 
agent 节点的组成也发生了变化。如图 4，NG agent 由 source、sink、channel 组成。                 
##### 图 3. FLUM NG 架构图
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGlHRA88pmK6PWMBfXDV5kVgW6vwEJJ4j5ibJZjEAwVicBcQPhga7zrgxA/640)
图 4. NG 节点组成图
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGBozUvPzjz7vpD1DDfZa1V2ThfPnt3NoMbbGXkequh09VGe5KEibmL1A/640)
从整体上讲，NG 在核心组件上进行了大规模的调整，核心组件的数目由 7 删减到 4。由于 Flume 的使用涉及到众多因素，如avro、thrift、hdfs、jdbc、zookeeper 等，而这些组件和 Flume 的整合都需要关联到所有组件。所以核心组件的改革对整个Flume 的使用影响深远：
- 
大大降低了对用户的要求，如核心组件的变化使得 Flume 的稳定使用不再依赖 zookeeper，用户无需去搭建zookeeper 集群；另外用户也不再纠结于 OG 中的模糊概念（尤其是 physical nodes、logical  nodes，agent、collector）。       
- 
有利于 Flume 和其他技术、hadoop 周边组件的整合，比如在 NG 版本中，Flume 轻松实现了和 jdbc、hbase 的集成。                 
- 
将 OG 版本中复杂、大规模、不稳定的标签移除，Flume实现了向灵活、轻便的转变，而且在功能上更加强大、可扩展性更高，这一点主要表现在用户使用 Flume搭建日志收集集群的过程中，后面的章节会详细介绍。
## **删减节点角色，脱离 zookeeper**
## Zookeeper 是针对大型分布式系统的可靠协调系统，适用于有多类角色集群管理。比如在 hbase 中，对 HMaster、HRegionServer 的管理。
## 在 OG 版本中，Flume 的使用稳定性依赖 zookeeper。它需要 zookeeper对其多类节点（agent、collector、master）的工作进行管理，尤其是在集群中配置多个 master 的情况下。当然，OG也可以用内存的方式管理各类节点的配置信息，但是需要用户能够忍受在机器出现故障时配置信息出现丢失。所以说 OG 的稳定行使用是依赖 zookeeper的。
## 而在 NG 版本中，节点角色的数量由 3 缩减到 1，不存在多类角色的问题，所以就不再需要 zookeeper 对各类节点协调的作用了，由此脱离了对                zookeeper 的依赖。由于 OG 的稳定使用对 zookeeper 的依赖表现在整个配置和使用过程中，这就要求用户掌握对 zookeeper 集群的搭建及其使用（尤其是要熟悉 zookeeper 数据文件夹 data，Flume 节点配置信息存放在此文件中）；掌握 Flume 中和zookeeper 相关的配置。对初次接触 Flume 的用户来讲，这是非常痛苦的事。
## **用户配置变化**
## 从用户角度来讲，配置过程无疑是整个集群搭建的核心步骤。Flume 的配置分为两个部分：安装和数据传输配置。
### **安装**
### OG 在安装时：
- 
 在 flume-env.sh 中设置$JAVA_HOME。
                
- 
需要配置文件 flume-conf.xml。其中最主要的、必须的配置与 master 有关。集群中的每个 Flume都需要配置 master 相关属性（如                flume.master.servers、flume.master.store、flume.master.serverid）。
                
- 
如果想稳定使用 Flume 集群，还需要安装 zookeeper 集群，这需要用户对 zookeeper 有较深入的了解。
                
- 
安装 zookeeper 之后，需要配置 flume-conf.xml 中的相关属性，如        flume.master.zk.use.external、flume.master.zk.servers。
                
- 
在使用 OG 版本传输数据之前，需要启动 master、agent。
                
NG 在安装时，只需要在 flume-env.sh 中设置$JAVA_HOME。
### **数据传输配置**
OG 版本的配置途径有两个：
- 
shell 命令：需要用户掌握 Flume shell 命令；
                
- 
master console 页面：这是 OG 用户最常用的配置方式；弊端在于，除非用户熟悉复杂的各类source，sink 配置函数以及格式（source：大约 25 个，sink：大约 46），否则在复杂的集群环境下，用户每次只能配置一个节点（指定 source、sink）来保证配置的准确性；
                
NG 的配置只需要一个配置文件，这个配置文件中存放 source、sink、channel 的配置。如图 5 中是配置文件 example.conf
                里面的内容，其中 agent_foo 是 agent 名字。然后在启动 agent 时，使用一下命令指定这个配置文件：
$ bin/flume-ng agent
--conf-file example.conf \
--name agent_foo \
-Dflume.root.logger=INFO,console
图 5. example.conf
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGnozwccfLLibVlXTTlSg7EXy1ET2ibne7dhPLQrOdNR9vAm4wm2qMhicCg/640)
**实战 Flume NG**
Flume 最常用的使用场景是，从节点收集日志数据，并以一定的格式存放到分布式文件系统 hdfs（hadoop 文件系统）中。下面介绍如何使用Flume NG 从一个节点收集实时日志，并存放到 hdfs 中。
场景说明：
- 
场景中有两台主机 host1、host2。
                
- 
数据源是 host2上的系统日志文件“/var/log/secure”（登录到系统存取资料的记录，本机的测试系统有多人使用，所以记录在不断的生成）。数据目的地是hadoop 文件系统 hdfs。
                
- 
在 host1、host2 上搭建 hadoop 集群。其中 host1 为namenode、jobtracker，host2 为 datanode、tasktracker。
                
### 使用 ng 搭建日志传输场景：flume+hadoop
场景搭建步骤：
- 
下载 flume-ng 安装包，并解压到 host2。
                    
Flume 发布了两类包：source 和 bin。Source 包用于开发工作，bin 包用于安装 Flume搭建日志收集场景。本次实验用的是 apache-flume-1.2.0-bin.tar.gz
- 
生成配置文件 example.conf。内容如图 6。整个配置分为四部分。表 1 是配置说明。
                
- 
进入 bin 目录，使用一下命令启动 Flume，开始日志收集，控制台输出如图 7，传输到 hdfs 的文件如图8。
                
./flume-ng agent \
--conf-file ../example.conf \
--name agent_ff \
-Dflume.root.logger=INFO,cnsole
##### 图 6. example.conf
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGJF9DVGf8yScWQhezdChLR1usZ2J7OTnWyiaXsAf8QsPhyKVCkm5zLHg/640)
表 1. flume-conf.xml
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX2T647np0gzZsATA7XicicsgzcuwvHOyp8EWg2JE6aRA5jSuvnwiatOVwMGWkOeTXOyU36kic4xWC8sA/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX2T647np0gzZsATA7Xicicsg8Kt1EhxZQFpAqfzk4gQggoibib2y0xia7V3j0ibkjDvzUW3FhptkL9jFLQ/640)
##### 图 7. NG console
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGfXNz5icST4dRUsLyJV2zGmMGyAzNicVLNWy08RG8Fy7c2OPTLNfe89Dw/640)
图 8. hdfs
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGz73uO3gyRbwJic6icsTaXtoaAVGXyPAgJQDfwBD3y5rDIO1E3es8YBoQ/640)
**Flume OG vs Flume NG：用户体验对比**
整体上看，NG 的用户体验要比 OG 好得多，这一点从用户文档上就可见一斑。OG 版本的使用文档有 90 多页，而 OG 只用 20多页的内容就完成了新版Flume 的使用说明。对应于上一节“实战 Flume NG”，我们使用和 NG 中同样的场景说明，下面介绍如何使用 OG搭建同样的日志收集场景。用户可以从场景的搭建上明显的看出差别：NG 的整个过程只涉及到 hadoop、agent，而 OG 则需要涉及到hadoop、zookeeper、master、agent、flume-conf.xml。
### 使用 og 搭建日志传输场景：flume+zookeeper+hadoop
场景搭建步骤：
- 
下载 zookeeper 安装包，并在 host2 上安装 zookeeper-3.4.3。请参考：zookeeperStarted。
                    
                
- 
下载 flume-0.94.0（OG），并解压在 host2 上。
                
- 
配置文件 conf/flume-conf.xml，必须配置的信息如表 2。
                
- 
进入 bin 目录，使用一下命令启动 flume master、agent。
master: ./flume-daemon.sh startmaster
agent: ./flume node -n agent-ff
- 
进入 master 页面：http://host2:35871/flumemaster.jsp。配置  source、sink。如图 9。“Submit Query”后 Flume 便开始收集数据。  
- 
agent-ff 控制台输出如图 10。
                
##### 表 2. flume-conf.xml
##### 图 9. OG 配置
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGkUwgQqGvN3wxAW0fnUAOquLMCS166XCFwYYT3CUhicpsdkaBibJZoRmQ/640)
图 10. OG console
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGN9ESibu1icP5ckzm9ESR2YT6yVIQUYKdZ08TCbGibThJ0z0BEfCiaCrBVg/640)
**结束语**
本文通过对比，从核心组件和用户的角度，阐述了 Flume NG 给 Flume 带来的第一次革命。从核心组件来讲，NG 简化核心组件，移除了 OG版本代码工程臃肿、核心组件设计不合理、核心配置不标准等缺点；另外 NG 脱离了 Flume 稳定性zookeeper 的依赖。从用户角度来讲，NG 版本对用户要求大大降低：安装过程除了 java 无需配置复杂的 Flume 相关属性，也无需搭建 zookeeper集群，安装过程几乎零工作量；数据流的配置过程更加简单、合理，只需要实现 source、sink、channel 的简单配置即可。
原文地址：https://www.ibm.com/developerworks/cn/data/library/bd-1404flumerevolution/index.html
[Spark SQL从入门到精通](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485504&idx=1&sn=e19460f43aa2fe388140a2241e9ea219&chksm=9f38eb68a84f627ee0b6dac115f0684dddba573be04ae7e4e54d17c084cf1946282f81b1fd43&scene=21#wechat_redirect)
[重磅：Flume1-7结合kafka讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484218&idx=1&sn=fe25af996f713b12359bd6e18cef5f31&chksm=9f38e012a84f69044ff9285b9f70e5e8aa6d6458faa306ac59645e307a8791b7acac77d156dc&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
