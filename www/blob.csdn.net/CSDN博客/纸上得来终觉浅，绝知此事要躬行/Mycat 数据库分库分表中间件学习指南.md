# Mycat 数据库分库分表中间件学习指南 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 15:23:50[boonya](https://me.csdn.net/boonya)阅读数：822










对于分库分表的数据库设计很多情况下还是需要的，我们就此作为学习的起点，一起来看下前世来自阿里的Cobar而今的Mycat。



# Mycat是什么


一个彻底开源的，面向企业应用开发的大数据库集群 支持事务、ACID、可以替代MySQL的加强版数据库 一个可以视为MySQL集群的企业级数据库，用来替代昂贵的Oracle集群 一个融合内存缓存技术、NoSQL技术、HDFS大数据的新型SQL Server 结合传统数据库和新型分布式数据仓库的新一代企业级数据库产品 一个新颖的数据库中间件产品



**目标**

低成本的将现有的单机数据库和应用平滑迁移到“云”端，解决数据存储和业务规模迅速增长情况下的数据瓶颈问题。

**长期规划2.0**



完全实现分布式事务，完全的支持分布式。 通过Mycat web（eye）完成可视化配置，及智能监控，自动运维。 通过mysql 本地节点，完整的解决数据扩容难度，实现自动扩容机制，解决扩容难点。 支持基于zookeeper的主从切换及Mycat集群化管理。 通过Mycat Balance 替代第三方的Haproxy，LVS等第三方高可用，完整的兼容Mycat集群节点的动态上下线。 接入Spark等第三方工具，解决数据分析及大数据聚合的业务场景。 通过Mycat智能优化，分析分片热点，提供合理的分片建议，索引建议，及数据切分实时业务建议。

**优势**



基于阿里开源的Cobar产品而研发，Cobar的稳定性、可靠性、优秀的架构和性能以及众多成熟的使用案例使得MYCAT一开始就拥有一个很好的起点，站在巨人的肩膀上，我们能看到更远。业界优秀的开源项目和创新思路被广泛融入到MYCAT的基因中，使得MYCAT在很多方面都领先于目前其他一些同类的开源项目，甚至超越某些商业产品。

MYCAT背后有一支强大的技术团队，其参与者都是5年以上资深软件工程师、架构师、DBA等，优秀的技术团队保证了MYCAT的产品质量。

MYCAT并不依托于任何一个商业公司，因此不像某些开源项目，将一些重要的特性封闭在其商业产品中，使得开源项目成了一个摆设。

**mycat监控**



支持对Mycat、Mysql性能监控 支持对Mycat的JVM内存提供监控服务 支持对线程的监控 支持对操作系统的CPU、内存、磁盘、网络的监控 ![](http://mycat.org.cn/img/mycateye1.png)









# 基础学习资源




官方站点：[http://mycat.org.cn/](http://mycat.org.cn/)


Github开源站点：[https://github.com/MyCATApache/Mycat-Server](https://github.com/MyCATApache/Mycat-Server)


Mycat权威指南：[PDF](http://mycat.org.cn/document/Mycat_V1.6.0.pdf)


# Mycat前世今生

2013年阿里的Cobar在社区使用过程中发现存在一些比较严重的问题，及其使用限制，经过Mycat发起人第一次改良，第一代改良版——Mycat诞生。 Mycat开源以后，一些Cobar的用户参与了Mycat的开发，最终Mycat发展成为一个由众多软件公司的实力派架构师和资深开发人员维护的社区型开源软件。

2014年Mycat首次在上海的《中华架构师》大会上对外宣讲，更多的人参与进来，随后越来越多的项目采用了Mycat。

2015年5月，由核心参与者们一起编写的第一本官方权威指南《Mycat权威指南》电子版发布，累计超过500本，成为开源项目中的首创。

2015年10月为止，Mycat项目总共有16个Committer。

截至2015年11月，超过300个项目采用Mycat，涵盖银行、电信、电子商务、物流、移动应用、O2O的众多领域和公司。

截至2015年12月，超过4000名用户加群或研究讨论或测试或使用Mycat。

Mycat是基于开源cobar演变而来，我们对cobar的代码进行了彻底的重构，使用NIO重构了网络模块，并且优化了Buffer内核，增强了聚合，Join等基本特性，同时兼容绝大多数数据库成为通用的数据库中间件。1.4 版本以后 完全的脱离基本cobar内核，结合Mycat集群管理、自动扩容、智能优化，成为高性能的中间件。我们致力于开发高性能数据库中间而努力。永不收费，永不闭源，持续推动开源社区的发展。

Mycat吸引和聚集了一大批业内大数据和云计算方面的资深工程师，Mycat的发展壮大基于开源社区志愿者的持续努力，感谢社区志愿者的努力让Mycat更加强大，同时我们也欢迎社区更多的志愿者，特别是公司能够参与进来，参与Mycat的开发，一起推动社区的发展，为社区提供更好的开源中间件。

Mycat还不够强大，Mycat还有很多不足，欢迎社区志愿者的持续优化改进。

# 关键特性





支持SQL92标准 支持MySQL、Oracle、DB2、SQL Server、PostgreSQL等DB的常见SQL语法 遵守Mysql原生协议，跨语言，跨平台，跨数据库的通用中间件代理。 基于心跳的自动故障切换，支持读写分离，支持MySQL主从，以及galera cluster集群。 支持Galera for MySQL集群，Percona Cluster或者MariaDB cluster 基于Nio实现，有效管理线程，解决高并发问题。 支持数据的多片自动路由与聚合，支持sum,count,max等常用的聚合函数,支持跨库分页。 支持单库内部任意join，支持跨库2表join，甚至基于caltlet的多表join。 支持通过全局表，ER关系的分片策略，实现了高效的多表join查询。 支持多租户方案。 支持分布式事务（弱xa）。 支持XA分布式事务（1.6.5）。 支持全局序列号，解决分布式下的主键生成问题。 分片规则丰富，插件化开发，易于扩展。 强大的web，命令行监控。 支持前端作为MySQL通用代理，后端JDBC方式支持Oracle、DB2、SQL Server 、 mongodb 、巨杉。 支持密码加密 支持服务降级 支持IP白名单 支持SQL黑名单、sql注入攻击拦截 支持prepare预编译指令（1.6） 支持非堆内存(Direct Memory)聚合计算（1.6） 支持PostgreSQL的native协议（1.6） 支持mysql和oracle存储过程，out参数、多结果集返回（1.6） 支持zookeeper协调主从切换、zk序列、配置zk化（1.6） 支持库内分表（1.6） 集群基于ZooKeeper管理，在线升级，扩容，智能优化，大数据处理（2.0开发版）。







## 下载：



[https://github.com/MyCATApache/Mycat-download](https://github.com/MyCATApache/Mycat-download)具体下载哪个版本以发布为准，推荐1.4,1.5.

## 安装：

下载的文件直接解压即可。

## 运行：

### linux：

./mycat start 启动

./mycat stop 停止

./mycat console 前台运行

./mycat install 添加到系统自动启动（暂未实现）

./mycat remove 取消随系统自动启动（暂未实现）

./mycat restart 重启服务

./mycat pause 暂停

./mycat status 查看启动状态

### win：

直接运行startup_nowrap.bat，如果出现闪退，在cmd 命令行运行，查看出错原因。 

## 内存配置：

启动前，一般需要修改JVM配置参数，打开conf/wrapper.conf文件，如下行的内容为2G和2048，可根据本机配置情况修改为512M或其它值。以下配置跟jvm参数完全一致，可以根据自己的jvm参数调整。

Java Additional Parameters

wrapper.java.additional.1=

wrapper.java.additional.1=-DMYCAT_HOME=.

wrapper.java.additional.2=-server

wrapper.java.additional.3=-XX:MaxPermSize=64M

wrapper.java.additional.4=-XX:+AggressiveOpts

wrapper.java.additional.5=-XX:MaxDirectMemorySize=100m

wrapper.java.additional.6=-Dcom.sun.management.jmxremote

wrapper.java.additional.7=-Dcom.sun.management.jmxremote.port=1984

wrapper.java.additional.8=-Dcom.sun.management.jmxremote.authenticate=false

wrapper.java.additional.9=-Dcom.sun.management.jmxremote.ssl=false

wrapper.java.additional.10=-Xmx100m

wrapper.java.additional.11=-Xms100m

wrapper.java.additional.12=-XX:+UseParNewGC

wrapper.java.additional.13=-XX:+UseConcMarkSweepGC

wrapper.java.additional.14=-XX:+UseCMSCompactAtFullCollection

wrapper.java.additional.15=-XX:CMSFullGCsBeforeCompaction=0

wrapper.java.additional.16=-XX:CMSInitiatingOccupancyFraction=70

以下配置作废：

wrapper.java.initmemory=3

wrapper.java.maxmemory=64

### Mycat连接测试：

测试mycat与测试mysql完全一致，mysql怎么连接，mycat就怎么连接。

推荐先采用命令行测试：

mysql -uroot -proot -P8066 -h127.0.0.1

如果采用工具连接，1.4,1.3目前部分工具无法连接，会提示database not selected，建议采用高版本，navicat测试。1.5已经修复了部分工具连接。




