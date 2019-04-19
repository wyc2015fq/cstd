# Hive-An overview - shine302的博客 - CSDN博客
置顶2018年04月13日 22:05:27[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：53标签：[Hive																[入门](https://so.csdn.net/so/search/s.do?q=入门&t=blog)](https://so.csdn.net/so/search/s.do?q=Hive&t=blog)
个人分类：[Hive](https://blog.csdn.net/shine302/article/category/7559969)
1）Hive是什么？
    大数据来临，海量的数据存储在分布式文件系统上。我们想像传统关系型数据库一样使用HDFS以及像使用SQL语句一样使用HDFS。Hive应运而生，Hive是适合于数据仓库应用的程序，进行静态数据分析，不需要给出快速响应结果，数据本身不会频繁变化。
2）Hive和关系型数据差别
不支持记录级别的更新、插入、删除、事务、联机事务处理。
3）Hive与Hadoop MapReduce 之间的关系
首先我们以WordCount为例，回顾一下Hadoop MapReduce 处理数据的流程：
对于文件中的每一行来说，Mapper会将其中的每个单词，输出为一个键值对，键是单词，值为1/总频数。
其次，按照键值对进行排序，所有的键相同的放在一起。
接着，将键相同的数据，交给同一个reduce处理。
最终，reduce求和并输出。
上述过程的底层细节，是由一个job到另一个job的重复性工作。
幸运的是，研究人员引入了Hive来处理这些重复性工作，让用户使用数据时像SQL那样简单。
与hive交互有好几种方式，主要的命令行CLI。
![](https://img-blog.csdn.net/20180413221530568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所有的命令查询都会进入到驱动模块（Driver）,该模块会进行编译、优化、执行。将这些命令封装为XML。传递给Hadoop中的master，master会启用jobtracker与namenode 协调使用DFS。
更直白一点，使用hive，就是代码写的少了，写的简单了。
