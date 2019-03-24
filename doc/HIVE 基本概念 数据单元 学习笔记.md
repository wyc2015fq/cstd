# HIVE 基本概念 数据单元 学习笔记

2018年12月11日 21:14:00

小鹅鹅

阅读数：61

更多

 								所属专栏： 																[Spark / Hadoop生态系列](https://blog.csdn.net/column/details/31206.html) 																 							

 									

​                   					 							 					                   					 					版权声明：本文为博主原创文章，转载请附http://blog.csdn.net/asd136912。					https://blog.csdn.net/asd136912/article/details/84961955				

### 综述

Hive是一个数据仓库基础工具，由Java编写，在Hadoop中用来处理结构化数据，可以把SQL查询转换为一系列在Hadoop集群上运行的作业。

Hive设计的初衷是：对于大量的数据，使得数据汇总，查询和分析更加简单。它提供了SQL，允许用户更加简单地进行查询，汇总和数据分析。同时，Hive的SQL给予了用户多种方式来集成自己的功能，然后做定制化的查询，例如用户自定义函数（User  Defined Functions，UDFs).

Hive**不是为在线事务处理**而设计。它最适合用于传统的数据仓库任务。

![img](https://upload-images.jianshu.io/upload_images/2021461-0684fb36ab813c4f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 数据单元

根据颗粒度的顺序，Hive数据被组织成：

- 数据库：命名空间功能，为了避免表，视图，分区，列等等的命名冲突。数据库也可以用于加强用户或用户组的安全。在hive 0.6.0之后的版本支持，hive提供了`create database dbname`、`use dbname`以及`drop database dbname`这样的语句。

- **表**：相同数据库的同类数据单元。Hive的表在逻辑上由存储的数据和描述表[中数据](https://www.baidu.com/s?wd=%E4%B8%AD%E6%95%B0%E6%8D%AE&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)形式的相关元数据组成。表存储的数据存放在分布式文件系统里，例如HDFS / S3，元数据存储在关系数据库里，当我们创建一张hive的表，还没有为表加载数据的时候，该表在分布式文件系统，例如hdfs上就是一个文件夹（文件目录）

  Hive里的表有两种类型:

  - 一种叫**托管表**，这种表的数据文件存储在hive的数据仓库里

    ```
      	Create table tuoguan_tbl (flied string);
      	Load data local inpath ‘home/hadoop/test.txt’ into table tuoguan_tbl;
    12
    ```

  - 一种叫**外部表**，这种表的数据文件可以存放在hive数据仓库外部的分布式文件系统上，也可以放到hive数据仓库里（注意：hive的数据仓库也就是hdfs上的一个目录，这个目录是hive数据文件存储的默认路径，它可以在hive的配置文件里进行配置，最终也会存放到元数据库里）

    ```
      Create external table external_tbl (flied string)
          Location  ‘/home/hadoop/external_table’;
      Load data local inpath ‘home/hadoop/test.txt’ into table external_tbl;
    123
    ```

    创建外部表时候table之前要加关键字`external`，同时还要用`location`命令指定文件存储的路径，如果不使用`locaction`数据文件也会放置到hive的数据仓库里。

    这两种表在使用的区别主`drop`命令上，`drop`是hive删除表的命令，托管表执行`drop`命令的时候，会删除元数据和存储的数据，而外部表执行`drop`命令时候只删除元数据库里的数据，而不会删除存储的数据。

    另外表的`load`命令，hive加载数据时候不会对元数据进行任何检查，只是简单的移动文件的位置，如果源文件格式不正确，也只有在做查询操作时候才能发现，那个时候错误格式的字段会以NULL来显示。

- **分区**：每个表可以有一个或多个用于决定数据如何存储的分区键。分区（除存储单元之外）也允许用户有效地识别满足指定条件的行。hive里分区的概念是根据“分区列”的值对表的数据进行粗略划分的机制，在hive存储上就体现在表的主目录（hive的表实际显示就是一个文件夹）下的一个子目录，这个文件夹的名字就是我们定义的分区列的名字，没有实际操作经验的人可能会认为分区列是表的某个字段，其实不是这样，分区列不是表里的某个字段，而是独立的列，我们根据这个列存储表的里的数据文件。使用分区是为了**加快数据分区的查询速度**而设计的，我们在查询某个具体分区列里的数据时候没必要进行全表扫描。

  创建分区：

  ```
    Create table logs(ts bigint,line string)
    Partitioned by (dt string,country string);
  12
  ```

  加载数据：

  ```
    Local data local inpath ‘/home/hadoop/par/file01.txt’ into table logs partition (dt=’2012-06-02’,country=’cn’);
  1
  ```

  在hive数据仓库里实际存储的路径如下所示：

  ```
    /user/hive/warehouse/logs/dt=2013-06-02/country=cn/file1.txt
    /user/hive/warehouse/logs/dt=2013-06-02/country=cn/file2.txt
    /user/hive/warehouse/logs/dt=2013-06-02/country=us/file3.txt
    /user/hive/warehouse/logs/dt=2013-06-02/country=us/file4.txt
  1234
  ```

  我们看到在表logs的目录下有了两层子目录dt=2013-06-02和country=cn

  查询操作：

  ```
    Select ts,dt,line  from logs where country=’cn’,
  1
  ```

- **桶**（Buckets or  Clusters）：每个分区的数据，基于表的一些列的哈希函数值，又被分割成桶。上面的table和partition都是目录级别的拆分数据，bucket则是对数据源数据文件本身来拆分数据。使用桶的表会将源数据文件按一定规律拆分成多个文件，要使用bucket。

  物理上，每个桶就是表（或分区）目录里的一个文件，桶文件是按指定字段值进行hash，然后除以桶的个数例如上面例子2，最后去结果余数，因为整数的hash值就是整数本身

  把表组织成桶有两个理由：1. 获取更高的查询处理效率(桶为表加上了额外的结构，具体是连接两个在相同列上划分了桶的表，可以用map端连接高效地实现) 2. 使取样更高效

  打开hive对桶的控制：

  ```
    set hive.enforce.bucketing = true
  1
  ```

  查看sampling数据：

  ```
    hive> select * from student tablesample(bucket 1 out of 2 on id);                                                                               
    Total MapReduce jobs = 1
    Launching Job 1 out of 1
    .......
    OK
    4       18      mac     20120802
    2       21      ljz     20120802
    6       23      symbian 20120802
    Time taken: 20.608 seconds
  123456789
  ```

  tablesample是抽样语句，语法：TABLESAMPLE(BUCKET x OUT OF y)

### Ref

1. Hadoop权威指南
2. <http://www.cnblogs.com/sharpxiajun/archive/2013/06/03/3114560.html>