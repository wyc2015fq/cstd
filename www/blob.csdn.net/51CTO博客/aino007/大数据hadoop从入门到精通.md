# 大数据hadoop从入门到精通-aino007-51CTO博客
前言：
•   一、背景介绍
•   二、大数据介绍
正文：
•   一、大数据相关的工作介绍
•   二、大数据工程师的技能要求
•   三、大数据学习规划
•   四、持续学习资源推荐（书籍，博客，网站）
•   五、项目案例分析（批处理+实时处理）
前言
一、背景介绍
本人目前是一名大数据工程师，项目数据50T，日均数据增长20G左右，个人是从Java后端开发，经过3个月的业余自学成功转型大数据工程师。
附上本人参考学习视频：
参考学习视频：[https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q](https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q)
二、大数据介绍
大数据本质也是数据，但是又有了新的特征，包括数据来源广、数据格式多样化（结构化数据、非结构化数据、Excel文件、文本文件等）、数据量大（最少也是TB级别的、甚至可能是PB级别）、数据增长速度快等。
针对以上主要的4个特征我们需要考虑以下问题：
- 数据来源广，该如何采集汇总？，对应出现了Sqoop，Cammel，Datax等工具。
- 数据采集之后，该如何存储？，对应出现了GFS，HDFS，TFS等分布式文件存储系统。
- 由于数据增长速度快，数据存储就必须可以水平扩展。
- 数据存储之后，该如何通过运算快速转化成一致的格式，该如何快速运算出自己想要的结果？
对应的MapReduce这样的分布式运算框架解决了这个问题；但是写MapReduce需要Java代码量很大，所以出现了Hive，Pig等将SQL转化成MapReduce的解析引擎；
普通的MapReduce处理数据只能一批一批地处理，时间延迟太长，为了实现每输入一条数据就能得到结果，于是出现了Storm/JStorm这样的低时延的流式计算框架；
但是如果同时需要批处理和流处理，按照如上就得搭两个集群，Hadoop集群（包括HDFS+MapReduce+Yarn）和Storm集群，不易于管理，所以出现了Spark这样的一站式的计算框架，既可以进行批处理，又可以进行流处理（实质上是微批处理）。
- 而后Lambda架构，Kappa架构的出现，又提供了一种业务处理的通用架构。
- 为了提高工作效率，加快运速度，出现了一些辅助工具：
- Ozzie，azkaban：定时任务调度的工具。
- Hue，Zepplin：图形化任务执行管理，结果查看工具。
- Scala语言：编写Spark程序的最佳语言，当然也可以选择用Python。
- Python语言：编写一些脚本时会用到。
- Allluxio，Kylin等：通过对存储的数据进行预处理，加快运算速度的工具。
以上大致就把整个大数据生态里面用到的工具所解决的问题列举了一遍，知道了他们为什么而出现或者说出现是为了解决什么问题，进行学习的时候就有的放矢了。
正文
一、大数据相关工作介绍
大数据方向的工作目前主要分为三个主要方向:
- 大数据工程师
- 数据分析师
- 大数据科学家
- 其他（数据挖掘等）
二、大数据工程师的技能要求
附上大数据工程师技能图：
必须掌握的技能11条
- Java高级(虚拟机、并发)
- Linux 基本操作
- Hadoop（HDFS+MapReduce+Yarn ）
- HBase（JavaAPI操作+Phoenix ）
- Hive(Hql基本操作和原理理解）
- Kafka
- Storm/JStorm
- Scala
- Python
- Spark (Core+sparksql+Spark streaming ）
- 辅助小工具(Sqoop/Flume/Oozie/Hue等)
高阶技能6条
- 机器学习算法以及mahout库加MLlib
- R语言
- Lambda 架构
- Kappa架构
- Kylin
- Alluxio
三、学习路径
假设每天可以抽出3个小时的有效学习时间，加上周末每天保证10个小时的有效学习时间；
3个月会有（21*3+4*2*10）*3=423小时的学习时间。
第一阶段（基础阶段）
1）Java 高级学习（《深入理解Java虚拟机》、《Java高并发实战》）—30小时
- 掌握多线程。
- 掌握并发包下的队列。
- 了解JMS。
- 掌握JVM技术。
- 掌握反射和动态代理。
2）Linux学习（跟鸟哥学就ok了）—–20小时
- Linux操作系统介绍与安装。
- Linux常用命令。
- Linux常用软件安装。
- Linux网络。
- 防火墙。
- Shell编程等。
参考学习视频：[https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q](https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q)
第二阶段（攻坚阶段）
3）Hadoop （《Hadoop 权威指南》）—80小时
- HDFS
- HDFS的概念和特性。
- HDFS的shell操作。
- HDFS的工作机制。
- HDFS的Java应用开发。
- MapReduce
- 运行WordCount示例程序。
- 
了解MapReduce内部的运行机制。 
- MapReduce程序运行流程解析。
- MapTask并发数的决定机制。
- MapReduce中的combiner组件应用。
- MapReduce中的序列化框架及应用。
- MapReduce中的排序。
- MapReduce中的自定义分区实现。
- MapReduce的shuffle机制。
- MapReduce利用数据压缩进行优化。
- MapReduce程序与YARN之间的关系。
- MapReduce参数优化。
- MapReduce的Java应用开发
4）Hive（《Hive开发指南》）–20小时
- Hive 基本概念
- Hive 应用场景。
- Hive 与hadoop的关系。
- Hive 与传统数据库对比。
- Hive 的数据存储机制。
- Hive 基本操作
- Hive 中的DDL操作。
- 在Hive 中如何实现高效的JOIN查询。
- Hive 的内置函数应用。
- Hive shell的高级使用方式。
- Hive 常用参数配置。
- Hive 自定义函数和Transform的使用技巧。
- Hive UDF/UDAF开发实例。
- Hive 执行过程分析及优化策略
5）HBase（《HBase权威指南》）—20小时
- hbase简介。
- habse安装。
- hbase数据模型。
- hbase命令。
- hbase开发。
- hbase原理。
参考学习视频：[https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q](https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q)
6）Scala（《快学Scala》）–20小时
- Scala概述。
- Scala编译器安装。
- Scala基础。
- 数组、映射、元组、集合。
- 类、对象、继承、特质。
- 模式匹配和样例类。
- 了解Scala Actor并发编程。
- 理解Akka。
- 理解Scala高阶函数。
- 理解Scala隐式转换。
参考学习视频：[https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q](https://pan.baidu.com/s/1SnaWp_ls0V3Z_CgmhGkT7Q)
