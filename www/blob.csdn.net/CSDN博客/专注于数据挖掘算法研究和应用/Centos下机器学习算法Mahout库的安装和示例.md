# Centos下机器学习算法Mahout库的安装和示例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月27日 09:29:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2151








                
1、Mahout简介

Apache Mahout是Apache Software Foundation(ASF)旗下的一个开源项目，提供了一些经典的机器学习算法，旨在帮助开发人员更加方便快捷地创建智能应用程序。

Mahout的主要目标是建立针对大规模数据集可伸缩的机器学习算法，主要包括以下五个部分：

1）频繁模式挖掘：挖掘数据中频繁出现的项集；

2）聚类：将诸如文本、文档之类的数据分成局部相关的组；

3）分类：利用已经存在的分类文档训练分类器，对未分类的文档进行分类；

4）推荐引擎（协同过滤）：获取用户的行为并从中发现用户可能喜欢的事务；

5）频繁子项挖掘：利用一个项集（查询记录或购物记录）去识别经常一起出现的项目。



1、安装

Mahout可下载源码进行编译安装，这里直接下载已编译好的二进制文件安装。

Mahout可在本地模式下运行，也可利用hadoop的MapReduce作业运行。

1）下载：http://mirror.bjtu.edu.cn/apache/mahout

    最新版本：apache-mahout-distribution-0.12.2.tar.gz

2）解压文件到指定目录：

   tar -zxvf apache-mahout-distribution-0.12.2.tar.gz -C /home/mahout

   参数-C指定解压的文件夹，可指定到$HADOOP_HOME/下。

   这里采用本地模式运行，不配置Mahout所需的Hadoop环境变量，当前JDK是要提前安装好。

   实际应用中，整合在Hadoop上应用最佳。

3）验证，进入/home/mahout/目录下执行：

   bin/mahout -help 可以列出Mahout的所有命令。



3、示例

示例使用Mahout中的FPGrowth算法实现频繁模式挖掘。

1）数据获取：http://fimi.ua.ac.be/data/retail.dat

   一组零售销售记录数据，用个空格划分。

2）执行算法：sequential模式执行，如果是hadoop则是mapreduce

   bin/mahout fpg -i /tmp/retail.dat -o /tmp/ -k 50 -method sequential -regex '[\ ]' -s 2



这里只是简单引入Mahout及本地模式命令运行相关算法，有兴趣的可以深入掌握这个机器学习库，只要提供数据即可。

Apache Mahout还提供了一个协同过滤算法的实现库Taste，用Java实现，方便用户定义和实现自己的推荐算法，有兴趣也可以部署来用用。

个人觉得机器学习算法还是要自己一步步实现比较有感觉，而且可以交汇验证优化，Spark平台的MLIB库还是比较喜欢。





