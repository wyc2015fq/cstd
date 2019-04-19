# Hive、Hbase、mysql区别 - 数据之美的博客 - CSDN博客
2017年08月06日 13:38:05[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：426
# Hive和HBase的区别
[Hive](http://lib.csdn.net/base/hive)是为了简化编写MapReduce程序而生的，使用MapReduce做过数据分析的人都知道，很多分析程序除业务逻辑不同外，程序流程基本一样。在这种情况下，就需要[hive](http://lib.csdn.net/base/hive)这样的用戶编程接口。Hive本身不存储和计算数据，它完全依赖于HDFS和MapReduce，Hive中的表纯逻辑，就是些表的定义等，也就是表的元数据。使用SQL实现Hive是因为SQL大家都熟悉，转换成本低，类似作用的Pig就不是SQL。
[Hbase](http://lib.csdn.net/base/hbase)为查询而生的，它通过组织起节点內所有机器的內存，提供一個超大的內存Hash表，它需要组织自己的[数据结构](http://lib.csdn.net/base/datastructure)，包括磁盘和內存中的，而Hive是不做这个的，表在[hbase](http://lib.csdn.net/base/hbase)中是物理表，而不是逻辑表，[搜索引擎](http://lib.csdn.net/base/searchengine)使用它來存储索引，以满足查询的实时性需求。
hive类似CloudBase，也是基于[Hadoop](http://lib.csdn.net/base/hadoop)分布式计算平台上的提供data warehouse的sql功能的一套软件。使得存储在[hadoop](http://lib.csdn.net/base/hadoop)里面的海量数据的汇总，即席查询简单化。hive提供了一套QL的查询语言，以sql为基础，使用起来很方便。
HBase是一个分布式的基于列存储的非关系型[数据库](http://lib.csdn.net/base/mysql)。HBase的查询效率很高，主要由于查询和展示结果。 
hive是分布式的关系型数据库。主要用来并行分布式处理大量数据。hive中的所有查询除了"select * from table;"都是需要通过Map\Reduce的方式来执行的。由于要走Map\Reduce，即使一个只有1行1列的表，如果不是通过select * from table;方式来查询的，可能也需要8、9秒。但hive比较擅长处理大量数据。当要处理的数据很多，并且Hadoop集群有足够的规模，这时就能体现出它的优势。
通过hive的存储接口，hive和Hbase可以整合使用。
1、hive是sql语言，通过数据库的方式来操作hdfs文件系统，为了简化编程，底层计算方式为mapreduce。
2、hive是面向行存储的数据库。 
3、Hive本身不存储和计算数据，它完全依赖于HDFS和MapReduce，Hive中的表纯逻辑。 
4、HBase为查询而生的，它通过组织起节点內所有机器的內存，提供一個超大的內存Hash表 。
5、hbase不是关系型数据库，而是一个在hdfs上开发的面向列的分布式数据库，不支持sql。
6、hbase是物理表，不是逻辑表，提供一个超大的内存hash表，搜索引擎通过它来存储索引，方便查询操作。
7、hbase是列存储。
Hive和Hbase有各自不同的特征：hive是高延迟、结构化和面向分析的，hbase是低延迟、非结构化和面向编程的。Hive数据仓库在hadoop上是高延迟的。
其中HBase位于结构化存储层，Hadoop HDFS为HBase提供了高可靠性的底层存储支持，Hadoop MapReduce为HBase提供了高性能的计算能力，Zookeeper为HBase提供了稳定服务和failover机制。
转： 
觉得在问区别之前，我应该显示说相同点，这么一想，又糊涂了，hive和hbase哪里像了，好像哪里都不像，既然哪里都不像，又何来的“区别是什么”这一问题，他俩所有的都算区别。
那么，hive是什么？
白话一点再加不严格一点，hive可以认为是map-reduce的一个包装。hive的意义就是把好写的hive的sql转换为复杂难写的map-reduce程序。 
于是，hbase是什么？
同样白话一点加不严格一点，hbase可以认为是hdfs的一个包装。他的本质是数据存储，是个NoSql数据库；hbase部署于hdfs之上，并且克服了hdfs在随机读写方面的缺点。 
所以要问hive和hbase的区别，就应该问问map-reduce和hdfs之间的区别，问区别，就要先说说他俩哪里像。
# Hive和数据库的比较
由于 Hive 采用了 SQL 的查询语言 HQL，因此很容易将 Hive 理解为数据库。其实从结构上来看，Hive 和数据库除了拥有类似的查询语言，再无类似之处。本文将从多个方面来阐述 Hive 和数据库的差异。数据库可以用在 Online 的应用中，但是Hive 是为数据仓库而设计的，清楚这一点，有助于从应用角度理解 Hive 的特性。
## 查询语言
由于SQL被广泛的应用在数据仓库中，因此，专门针对Hive的特性设计了类SQL的查询语言HQL。熟悉SQL开发的开发者可以很方便的使用Hive进行开发。
## 数据存储位置
Hive 是建立在 Hadoop 之上的，所有 Hive 的数据都是存储在 HDFS 中的。而数据库则可以将数据保存在块设备或者本地文件系统中。
## 数据格式
Hive 中没有定义专门的数据格式，数据格式可以由用户指定，用户定义数据格式需要指定三个属性：列分隔符（通常为空格、”\t”、”\x001″）、行分隔符（”\n”）以及读取文件数据的方法（Hive 中默认有三个文件格式TextFile，SequenceFile以及RCFile）。由于在加载数据的过程中，不需要从用户数据格式到 Hive 定义的数据格式的转换，因此，Hive在加载的过程中不会对数据本身进行任何修改，而只是将数据内容复制或者移动到相应的HDFS目录中。而在数据库中，不同的数据库有不同的存储引擎，定义了自己的数据格式。所有数据都会按照一定的组织存储，因此，数据库加载数据的过程会比较耗时。
## 数据更新
由于Hive是针对数据仓库应用设计的，而数据仓库的内容是读多写少的。因此，Hive中不支持对数据的改写和添加，所有的数据都是在加载的时候中确定好的。而数据库中的数据通常是需要经常进行修改的，因此可以使用 INSERT INTO …  VALUES 添加数据，使用 UPDATE … SET修改数据。
## 索引
之前已经说过，Hive在加载数据的过程中不会对数据进行任何处理，甚至不会对数据进行扫描，因此也没有对数据中的某些Key建立索引。Hive要访问数据中满足条件的特定值时，需要暴力扫描整个数据，因此访问延迟较高。由于 MapReduce 的引入， Hive 可以并行访问数据，因此即使没有索引，对于[大数据](http://lib.csdn.net/base/hadoop)量的访问，Hive 仍然可以体现出优势。数据库中，通常会针对一个或者几个列建立索引，因此对于少量的特定条件的数据的访问，数据库可以有很高的效率，较低的延迟。由于数据的访问延迟较高，决定了 Hive 不适合在线数据查询。
## 执行
Hive中大多数查询的执行是通过 Hadoop 提供的 MapReduce 来实现的（类似 select * from tbl的查询不需要MapReduce）。而数据库通常有自己的执行引擎。
## 执行延迟
之前提到，Hive 在查询数据的时候，由于没有索引，需要扫描整个表，因此延迟较高。另外一个导致 Hive 执行延迟高的因素是 MapReduce框架。由于MapReduce 本身具有较高的延迟，因此在利用MapReduce 执行Hive查询时，也会有较高的延迟。相对的，数据库的执行延迟较低。当然，这个低是有条件的，即数据规模较小，当数据规模大到超过数据库的处理能力的时 候，Hive的并行计算显然能体现出优势。
## 可扩展性
由于Hive是建立在Hadoop之上的，因此Hive的可扩展性是和Hadoop的可扩展性是一致的（世界上最大的Hadoop 集群在 Yahoo!，2009年的规模在4000 台节点左右）。而数据库由于 ACID 语义的严格限制，扩展行非常有限。目前最先进的并行数据库 [Oracle](http://lib.csdn.net/base/oracle)在理论上的扩展能力也只有100台左右。
## 数据规模
由于Hive建立在集群上并可以利用MapReduce进行并行计算，因此可以支持很大规模的数据；对应的，数据库可以支持的数据规模较小。
