# 大数据测试学习笔记之基准测试HiBench - zhusongziye的博客 - CSDN博客





2018年03月19日 20:42:14[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1808








### 简介

HiBench是一套基准测试套件，用于帮助我们评估不同的大数据框架性能指标（包括处理速度、吞吐等）的负载指标，可以评估Hadoop、Spark和流式负载等，具体的工作负载有：
- 
Sort

- 
WordCount

- 
TeraSort

- 
Sleep

- 
SQL

- 
PageRank

- 
Nutch indexing

- 
Bayes

- 
Kmeans

- 
NWeight

- 
enhanced DFSIO

- 
等等


同样的它还可以用于评估Spark Stream、Flink、Storm和Gearpump。




### 工作负载

对这些工作负载进行分类记录如下，总体可以分为6大类：micro、ml（machine learning）、sql、graph、websearch和streaming。

Micro基准
- 
Sort
使用RandomTextWriter生成测试数据，Sort工作负载对它的文本输入数据进行排序来进行基准测试

- 
WordCount
使用RandomTextWriter生成测试数据，WordCount工作负载对输入数据中每个单词的出现情况进行统计

- 
TeraSort
是由Jim Gray创建的标准基准。其输入数据由Hadoop TeraGen示例程序生成。

- 
Sleep
使每个任务休眠一定的时间来测试调度框架

- 
enhanced DFSIO (dfsioe)
增强的DFSIO通过生成大量执行写入和读取的任务来测试Hadoop集群的HDFS吞吐量。它测量每个map任务的平均I/O速率、每个map任务的平均吞吐量以及HDFS集群的聚合吞吐量。
注:这个工作负载不支持Spark。


Machine Learning基准
- 
贝叶斯分类 (Bayes)
朴素贝叶斯是一种简单的多类分类算法，具有独立于每一对特征的假设。这个工作负载是在spark.mllib中实现并使用自动生成的文档，这些文档的单词遵循zipfian分布。关键字用于文本生成/usr/share/dict/linux.words.ords也从默认的linux文件。

- 
k-means聚类(Kmeans)
这个工作负载测试是在spark.mllib中实现的K-means(一种著名的知识发现和数据挖掘的聚类算法)。输入数据集是由基于均匀分布和Guassian分布的GenKMeansDataset生成的。

- 
逻辑回归(LR)
逻辑回归(Logistic Regression, LR)是预测分类响应的常用方法。这个工作负载是在spark.mllib中实现， LBFGS优化器和输入数据集是LogisticRegressionDataGenerator基于随机生成决策树的平衡。它包含三种不同类型的数据类型，包括分类数据、连续数据和二进制数据。

- 
交替最小二乘(ALS)
交互最小二乘法(ALS)算法是一种著名的协同过滤算法。这个工作负载是在spark.mllib中实现和输入数据集是由RatingDataGenerator为产品推荐系统生成的。

- 
梯度增加树(GBT)
梯度增强树(GBT)是一种使用决策树组合的流行的回归方法。这个工作负载是在spark.mllib中实现， GradientBoostingTreeDataGenerator生成的输入数据集。

- 
线性回归(线性)
线性回归(线性回归)是一个在spark.mllib中实现的工作负载。mllib SGD优化器。输入数据集是由LinearRegressionDataGenerator生成的。

- 
潜在狄利克雷分配(LDA)
潜在的Dirichlet分配(LDA)是一个主题模型，它从一个文本文档集合中推断主题。这个工作负载是在spark.mllib中实现和输入数据集由LDADataGenerator生成。

- 
主成分分析(PCA)
主成分分析(PCA)是一种寻找旋转的统计方法，使得第一个坐标有最大的方差，而每个后续的坐标都有最大的方差。PCA在降维方面得到了广泛的应用。这个工作负载是在spark.mllib中实现。输入数据集由PCADataGenerator生成。

- 
随机森林(RF)
随机森林(RF)是决策树的集合。随机森林是最成功的分类和回归机器学习模型之一。为了降低过度拟合的风险，他们联合了许多决策树。这个工作负载是在spark.mllib中实现， RandomForestDataGenerator生成的输入数据集。

- 
支持向量机(SVM)
支持向量机(SVM)是大规模分类任务的标准方法。这个工作负载是在spark.mllib中实现和输入数据集由SVMDataGenerator生成。

- 
奇异值分解(SVD)
奇异值分解(SVD)将矩阵分解成三个矩阵。这个工作负载是在spark.mllib中实现及其输入数据集由SVDDataGenerator生成。


SQL基准
- 
扫描(扫scan),连接(join),聚合(aggregation)
这些工作负载是基于SIGMOD 09论文“对大规模数据分析方法的比较”和HIVE-396进行开发的。它包含用于执行本文描述的典型OLAP查询的Hive查询(聚合和连接)。它的输入也会自动生成带有超链接的网络数据。


Websearch基准
- 
PageRank
这个工作负载基准PageRank算法在Spark-MLLib/Hadoop中实现(在pegasus 2.0中包含一个搜索引擎排名基准)。数据源是由Web数据生成的，其超链接遵循Zipfian分布。

- 
Nutch索引(nutchindexing)
大规模搜索索引是MapReduce最重要的用途之一。这个工作负载测试Nutch中的索引子系统，这是一个流行的开源(Apache项目)搜索引擎。工作负载使用自动生成的Web数据，其超链接和单词都遵循Zipfian分布和相应的参数。用来生成网页文本的命令是默认的linux命令文件。


Graph基准
- 
NWeight(NWeight)
NWeight是由Spark GraphX和pregel实现的一种迭代的图形并行算法。该算法计算两个n-hop的顶点之间的关联。


Streaming基准
- 
身份(Identity)
这个工作负载从Kafka读取输入数据，然后立即将结果写入Kafka，不涉及复杂的业务逻辑。

- 
重新分区(Repartition)
这个工作负载从Kafka读取输入数据，并通过创建更多或更少的分区来更改并行度。它测试了流框架中的数据洗牌效率。

- 
有状态Wordcount(Wordcount)
每隔几秒就会收到Kafka的词汇量。这将测试流框架中的有状态操作符性能和检查点/Acker成本。

- 
Fixwindow(Fixwindow)
工作负载执行基于窗口的聚合。它在流框架中测试窗口操作的性能。





### 总结

在查阅HiBench基准测试套件提供资料时，其提供的系列基准测试工具，每个后续都需要花时间深入的去学习和实践，主要需要从以下几个方面进行：
- 
在概念方面，需要进一步理透，以便深入理解

- 
需要去安装配置，实战对比不同环境下基准测试数据

- 
获取基准测试数据后，对数据深入分析形成一套自己的理解




