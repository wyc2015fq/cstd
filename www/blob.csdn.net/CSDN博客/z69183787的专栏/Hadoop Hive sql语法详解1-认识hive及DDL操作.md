# Hadoop Hive sql语法详解1-认识hive及DDL操作 - z69183787的专栏 - CSDN博客
2016年10月26日 10:25:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：955
hive或许我们有一个整体的认识，可以转换为mapreduce，那么具体是如何做的那？
1.编写的mapreduce能否成为hive插件那？
2.hive如何创建？
3.hive是如何查询数据的？
1.认识hive：
Hive 是基于Hadoop 构建的一套数据仓库分析系统，它提供了丰富的SQL查询方式来分析存储在Hadoop 分布式文件系统中的数据，可以将结构
化的数据文件映射为一张数据库表，并提供完整的SQL查询功能，可以将SQL语句转换为MapReduce任务进行运行，通过自己的SQL
 去查询分析需
要的内容，这套SQL 简称Hive SQL，使不熟悉mapreduce
 的用户很方便的利用SQL 语言查询，汇总，分析数据。而mapreduce开发人员可以把
己写的mapper 和reducer 作为插件来支持Hive 做更复杂的数据分析。
     它与关系型数据库的SQL 略有不同，但支持了绝大多数的语句如DDL、DML 以及常见的聚合函数、连接查询、条件查询。HIVE不适合用于联机
online)事务处理，也不提供实时查询功能。它最适合应用在基于大量不可变数据的批处理作业。
HIVE的特点：可伸缩（在Hadoop的集群上动态的添加设备），可扩展，容错，输入格式的松散耦合。
2.  DDL 操作
创建简单表
hive> CREATE TABLE pokes (foo INT, bar STRING); 
复杂一下如下：
创建外部表：
CREATE EXTERNAL TABLE page_view(viewTime INT, userid BIGINT,
     page_url STRING, referrer_url STRING,
     ip STRING COMMENT 'IP Address of the User',
     country STRING COMMENT 'country of origination')
COMMENT 'This is the staging page view table'
ROW FORMAT DELIMITED FIELDS TERMINATED BY '\054'
STORED AS TEXTFILE
LOCATION '<hdfs_location>';
建分区表:
CREATE TABLE par_table(viewTime INT, userid BIGINT,
     page_url STRING, referrer_url STRING,
     ip STRING COMMENT 'IP Address of the User')
COMMENT 'This is the page view table'
PARTITIONED BY(date STRING, pos STRING)
ROW FORMAT DELIMITED ‘\t’
   FIELDS TERMINATED BY '\n'
STORED AS SEQUENCEFILE;
建Bucket表
CREATE TABLE par_table(viewTime INT, userid BIGINT,
     page_url STRING, referrer_url STRING,
     ip STRING COMMENT 'IP Address of the User')
COMMENT 'This is the page view table'
PARTITIONED BY(date STRING, pos STRING)
CLUSTERED BY(userid) SORTED BY(viewTime) INTO 32 BUCKETS
ROW FORMAT DELIMITED ‘\t’
   FIELDS TERMINATED BY '\n'
STORED AS SEQUENCEFILE;
创建表并创建索引字段ds
hive> CREATE TABLE invites (foo INT, bar STRING) PARTITIONED BY (ds STRING); 
复制一个空表
CREATE TABLE empty_key_value_store
LIKE key_value_store;
例子
create table  user_info (user_id int, cid string, ckid string, username string) 
row format delimited 
fields terminated by '\t'
lines terminated by '\n';
导入数据表的数据格式是：字段之间是tab键分割，行之间是断行。
及要我们的文件内容格式：
100636  100890  c5c86f4cddc15eb7        yyyvybtvt
100612  100865  97cc70d411c18b6f        gyvcycy
100078  100087  ecd6026a15ffddf5        qa000100
显示所有表
hive> SHOW TABLES;
按正条件（正则表达式）显示表，
hive> SHOW TABLES '.*s';
