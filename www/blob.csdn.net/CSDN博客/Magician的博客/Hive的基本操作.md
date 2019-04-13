
# Hive的基本操作 - Magician的博客 - CSDN博客


2018年03月29日 14:46:47[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：96标签：[Hive																](https://so.csdn.net/so/search/s.do?q=Hive&t=blog)[Hql																](https://so.csdn.net/so/search/s.do?q=Hql&t=blog)[基本操作																](https://so.csdn.net/so/search/s.do?q=基本操作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Hql&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Hive&t=blog)个人分类：[Hive																](https://blog.csdn.net/qq_16633405/article/category/7234365)[大数据																](https://blog.csdn.net/qq_16633405/article/category/6905043)[
							](https://blog.csdn.net/qq_16633405/article/category/7234365)
所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)
[
																	](https://so.csdn.net/so/search/s.do?q=Hive&t=blog)


[目录](#目录)
[前言：](#前言)
[1、Hive基本操作](#1hive基本操作)[1.1、DDL操作](#11ddl操作)
[1.2、DML操作](#12dml操作)
[1.3、Hive Join](#13hive-join)
[总结:](#总结)

# 目录
# 前言：
对于Hive来说最重要的一点就是能够用Hql来进行数据分析。而Hql来处理数据比MapReduce方便很多（原理是一样的，Hql底层转化为MapReduce来处理数据）。而随着技术的发展，编写MapReduce程序来处理数据也许会被慢慢淘汰，而Hql仍会保留下来（原因：1、类SQL语法，学习成本比较低。2、大数据时代对数据开发的需求，需要有这样的数据仓库来支撑。）
# 1、Hive基本操作
## 1.1、DDL操作
**1.1.1 、创建表**
建表语法
CREATE [EXTERNAL] TABLE [IF NOT EXISTS] table_name
[(col_name data_type [COMMENT col_comment], …)]
[COMMENT table_comment]
[PARTITIONED BY (col_name data_type [COMMENT col_comment], …)]
[CLUSTERED BY (col_name, col_name, …)
[SORTED BY (col_name [ASC|DESC], …)] INTO num_buckets BUCKETS]
[ROW FORMAT row_format]
[STORED AS file_format]
[LOCATION hdfs_path]
说明：
1、 CREATE TABLE 创建一个指定名字的表。如果相同名字的表已经存在，则抛出异常；用户可以用 IF NOT EXISTS 选项来忽略这个异常。
2、 EXTERNAL关键字可以让用户创建一个外部表，在建表的同时指定一个指向实际数据的路径（LOCATION），**Hive 创建内部表时，会将数据移动到数据仓库指向的路径；若创建外部表，仅记录数据所在的路径，不对数据的位置做任何改变。在删除表的时候，内部表的元数据和数据会被一起删除，而外部表只删除元数据，不删除数据。**
3、 LIKE 允许用户复制现有的表结构，但是不复制数据。
4、 ROW FORMAT DELIMITED [FIELDS TERMINATED BY char] [COLLECTION ITEMS TERMINATED BY char]
[MAP KEYS TERMINATED BY char] [LINES TERMINATED BY char]
| SERDE serde_name [WITH SERDEPROPERTIES (property_name=property_value, property_name=property_value, …)]
用户在建表的时候可以自定义 SerDe 或者使用自带的 SerDe。如果没有指定 ROW FORMAT 或者 ROW FORMAT DELIMITED，将会使用自带的 SerDe。在建表的时候，用户还需要为表指定列，用户在指定表的列的同时也会指定自定义的 SerDe，Hive通过 SerDe 确定表的具体的列的数据。
5、 STORED AS
SEQUENCEFILE|TEXTFILE|RCFILE
如果文件数据是纯文本，可以使用 STORED AS TEXTFILE。如果数据需要压缩，使用 STORED AS SEQUENCEFILE。
6、CLUSTERED BY
对于每一个表（table）或者分区， Hive可以进一步组织成桶，也就是说桶是更为细粒度的数据范围划分。Hive也是 针对某一列进行桶的组织。Hive采用对列值哈希，然后除以桶的个数求余的方式决定该条记录存放在哪个桶当中。
**把表（或者分区）组织成桶（Bucket）有两个理由：**
**（1）获得更高的查询处理效率。**桶为表加上了额外的结构，Hive 在处理有些查询时能利用这个结构。具体而言，连接两个在（包含连接列的）相同列上划分了桶的表，可以使用 Map 端连接 （Map-side join）高效的实现。比如JOIN操作。对于JOIN操作两个表有一个相同的列，如果对这两个表都进行了桶操作。那么将保存相同列值的桶进行JOIN操作就可以，可以大大较少JOIN的数据量。
**（2）使取样（sampling）更高效。**在处理大规模数据集时，在开发和修改查询的阶段，如果能在数据集的一小部分数据上试运行查询，会带来很多方便。
**具体实例**
1、  创建内部表mytable。
![这里写图片描述](https://img-blog.csdn.net/20180329140839336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180329140839336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、  创建外部表pageview。
![这里写图片描述](https://img-blog.csdn.net/20180329140925477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180329140925477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3、  创建分区表invites。
![这里写图片描述](https://img-blog.csdn.net/20180329141015476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180329141015476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4、  创建带桶的表student。
![这里写图片描述](https://img-blog.csdn.net/20180329141026352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1.1.2、修改表**
**增加/删除分区**
语法结构
ALTER TABLE table_name**ADD**[IF NOT EXISTS] partition_spec [ LOCATION ‘location1’ ] partition_spec [ LOCATION ‘location2’ ] …
具体实例
alter table student_p add partition(part=’a’) partition(part=’b’);
![这里写图片描述](https://img-blog.csdn.net/20180329141345868?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**重命名表**
ALTER TABLE table_name**RENAME TO**new_table_name
具体实例：
![这里写图片描述](https://img-blog.csdn.net/20180329141548444?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**增加/更新列**
语法结构
ALTER TABLE table_name ADD|REPLACE COLUMNS (col_name data_type [COMMENT col_comment], …)
注：ADD是代表新增一字段，字段位置在所有列后面(partition列前)，REPLACE则是表示替换表中所有字段。
![这里写图片描述](https://img-blog.csdn.net/20180329141836349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1.1.3、显示命令**
show tables
show databases
show partitions
show functions
desc t_name;
desc extended t_name;
desc formatted table_name;
## 1.2、DML操作
**1.2.1、Load**
语法结构
LOAD DATA [LOCAL] INPATH ‘filepath’ [OVERWRITE] INTO
TABLE tablename [PARTITION (partcol1=val1, partcol2=val2 …)]
**说明：**
1、  Load 操作只是单纯的复制/移动操作，将数据文件移动到 Hive 表对应的位置。
2、  filepath：
相对路径，例如：project/data1
绝对路径，例如：/user/hive/project/data1
包含模式的完整 URI，列如：
hdfs://namenode:9000/user/hive/project/data1
3、  LOCAL关键字
如果指定了 LOCAL， load 命令会去查找本地文件系统中的 filepath。
如果没有指定 LOCAL 关键字，则根据inpath中的uri[M1] 查找文件
4、  OVERWRITE 关键字
如果使用了 OVERWRITE 关键字，则目标表（或者分区）中的内容会被删除，然后再将 filepath 指向的文件/目录中的内容添加到表/分区中。
如果目标表（分区）已经有一个文件，并且文件名和 filepath 中的文件名冲突，那么现有的文件会被新文件所替代。
**具体实例**
1、加载绝对路径数据
![这里写图片描述](https://img-blog.csdn.net/20180329142230718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1.2.2、Insert
将查询结果插入Hive表
语法结构
INSERT OVERWRITE TABLE tablename1 [PARTITION (partcol1=val1, partcol2=val2 …)] select_statement1 FROM from_statement
具体实例
1、基本模式插入。
![这里写图片描述](https://img-blog.csdn.net/20180329142512702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
导出表数据
语法结构
INSERT OVERWRITE [LOCAL] DIRECTORY directory1 SELECT … FROM …
具体实例
1、导出文件到本地。
![这里写图片描述](https://img-blog.csdn.net/20180329142806323?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180329142806323?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、导出数据到HDFS。
![这里写图片描述](https://img-blog.csdn.net/20180329142813155?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1.2.3 SELECT**
基本的Select操作
语法结构
SELECT [ALL | DISTINCT] select_expr, select_expr, …
FROM table_reference
[WHERE where_condition]
[GROUP BY col_list [HAVING condition]]
[CLUSTER BY col_list
| [DISTRIBUTE BY col_list] [SORT BY| ORDER BY col_list]
]
[LIMIT number]
**注：**
1、order by 会对输入做全局排序，因此只有一个reducer，会导致当输入规模较大时，需要较长的计算时间。
2、sort by不是全局排序，其在数据进入reducer前完成排序。因此，如果用sort by进行排序，并且设置mapred.reduce.tasks>1，则sort by只保证每个reducer的输出有序，不保证全局有序。
3、distribute by根据distribute by指定的内容将数据分到同一个reducer。
4、Cluster by 除了具有Distribute by的功能外，还会对该字段进行排序。因此，常常认为cluster by = distribute by + sort by
分桶的作用：最大的作用是提高join的效率。
具体实例
1、查询学生信息按年龄，降序排序。
![这里写图片描述](https://img-blog.csdn.net/20180329143025606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 1.3、Hive Join
语法结构
join_table:
table_reference JOIN table_factor [join_condition]
| table_reference {LEFT|RIGHT|FULL} [OUTER] JOIN table_reference join_condition
| table_reference LEFT SEMI JOIN table_reference join_condition
，因为非等值连接非常难转化到 map/reduce 任务。
另外，Hive 支持多于 2 个表的连接。
写 join 查询时，需要注意几个关键点：
1、只支持等值join。
例如：
SELECT a.* FROM a JOIN b ON (a.id = b.id)
SELECT a.* FROM a JOIN b
ON (a.id = b.id AND a.department = b.department)
是正确的，然而:
SELECT a.* FROM a JOIN b ON (a.id>b.id)
是错误的。
2、可以 join 多于 2 个表。
例如
SELECT a.val, b.val, c.val FROM a JOIN b
ON (a.key = b.key1) JOIN c ON (c.key = b.key2)
如果join中多个表的 join key 是同一个，则 join 会被转化为单个 map/reduce 任务，例如：
SELECT a.val, b.val, c.val FROM a JOIN b
ON (a.key = b.key1) JOIN c
ON (c.key = b.key1)
被转化为单个 map/reduce 任务，因为 join 中只使用了 b.key1 作为 join key。
SELECT a.val, b.val, c.val FROM a JOIN b ON (a.key = b.key1)
JOIN c ON (c.key = b.key2)
而这一 join 被转化为 2 个 map/reduce 任务。因为 b.key1 用于第一次 join 条件，而 b.key2 用于第二次 join。
具体实例
准备数据
表a
1,a
2,b
3,c
4,d
7,y
8,u
表b
2,bb
3,cc
7,yy
9,pp
实验：
**inner join**
select * from a inner join b on a.id=b.id;
+——-+———+——-+———+–+
| a.id  | a.name  | b.id  | b.name  |
+——-+———+——-+———+–+
| 2     | b       | 2     | bb      |
| 3     | c       | 3     | cc      |
| 7     | y       | 7     | yy      |
+——-+———+——-+———+–+
**outer join**除了符合条件的数据外，还显示主表存在，附表为NULL的数据。
select * from a  outer join b on a.id=b.id;
+——-+———+——-+———+–+
| a.id  | a.name  | b.id  | b.name  |
+——-+———+——-+———+–+
| 1     | a       | NULL  | NULL    |
| 2     | b       | 2     | bb      |
| 3     | c       | 3     | cc      |
| 4     | d       | NULL  | NULL    |
| 7     | y       | 7     | yy      |
| 8     | u       | NULL  | NULL    |
**left join**（Left、right指的是以那边的数据来作为基本数据来添加另一张表的数据）
select * from a left join b on a.id=b.id;
+——-+———+——-+———+–+
| a.id  | a.name  | b.id  | b.name  |
+——-+———+——-+———+–+
| 1     | a       | NULL  | NULL    |
| 2     | b       | 2     | bb      |
| 3     | c       | 3     | cc      |
| 4     | d       | NULL  | NULL    |
| 7     | y       | 7     | yy      |
| 8     | u       | NULL  | NULL    |
+——-+———+——-+———+–+
**left semi join**
把a和b都有的属性罗列出来
select * from a**left semi join**b on a.id = b.id;
+——-+———+–+
| a.id  | a.name  |
+——-+———+–+
| 2     | b       |
| 3     | c       |
| 7     | y       |
+——-+———+–+
以上是Hive的一些基本操作，除此之外还有以下一些操作需要掌握：
**保存select查询结果的几种方式：**
1、将查询结果保存到一张新的hive表中
create table t_tmp
as
select * from t_p;
2、将查询结果保存到一张已经存在的hive表中
insert into  table t_tmp
select * from t_p;
3、将查询结果保存到指定的文件目录（可以是本地，也可以是hdfs）
insert overwrite local directory ‘/home/hadoop/test’
select * from t_p;
insert overwrite directory ‘/aaa/test’
select * from t_p;
**\#\#设置变量,设置分桶为true, 设置reduce数量是分桶的数量个数**
set hive.enforce.bucketing = true;
set mapreduce.job.reduces=4;
# 总结:
如果你掌握了以上一些常用的基本操作指令并能根据需求写一些常用的分析语句，那么恭喜你已经和博主一样进化为“小菜鸟”。接下来如果想把Hive玩熟，你除了要知道Hive中常用的内置函数外，还要能够根据需求写一些UDF，最后你还需要理解每条Hql执行后要转化为什么样的MapReduce程序，以此来提高你查询的效率。以上是关于Hive基本操作的总结，目前就想到了这些，后续会接着补充，欢迎各位网友指点。

