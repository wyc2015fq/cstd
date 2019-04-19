# Hive 基本概念 - Simple 专栏 - CSDN博客
2018年10月09日 20:16:13[Simple_Zz](https://me.csdn.net/love284969214)阅读数：70
Hive 是基于Hadoop的一个数据仓库工具，可以将结构化的数据文件映射为一张数据库表，并提供简单的sql查询功能，可以将sql语句转换为MapReduce任务进行运行。 其优点是学习成本低，可以通过类SQL语句快速实现简单的MapReduce统计，不必开发专门的MapReduce应用，十分适合数据仓库的统计分析。
Hive是建立在 Hadoop 上的数据仓库基础构架。它提供了一系列的工具，可以用来进行数据提取转化加载（ETL），这是一种可以存储、查询和分析存储在 Hadoop 中的大规模数据的机制。Hive 定义了简单的类 SQL 查询语言，称为 HQL，它允许熟悉 SQL 的用户查询数据。同时，这个语言也允许熟悉 MapReduce 开发者的开发自定义的 mapper 和 reducer 来处理内建的 mapper 和 reducer 无法完成的复杂的分析工作。
Hive的本质是：将 HQL 转化成 MapReduce 程序。
- Hive处理的数据存储在HDFS
- Hive分析数据底层的实现是MapReduce
- 执行程序运行在Yarn上
![](https://img-blog.csdn.net/20181009201229812?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 优点
- 操作接口采用类SQL语法，提供快速开发的能力（简单、容易上手）。
- 避免了去写MapReduce，减少开发人员的学习成本。
- Hive的执行延迟比较高，因此Hive常用于数据分析，对实时性要求不高的场合。
- Hive优势在于处理大数据，对于处理小数据没有优势，因为Hive的执行延迟比较高。
- Hive支持用户自定义函数，用户可以根据自己的需求来实现自己的函数。
### 缺点
1.Hive的HQL表达能力有限
- 迭代式算法无法表达。
- 数据挖掘方面不擅长，由于MapReduce数据处理流程的限制，效率更高的算法却无法实现。
2.Hive的效率比较低
- Hive自动生成的MapReduce作业，通常情况下不够智能化。
- Hive调优比较困难，粒度较粗。
