# Hive表路径修改和分区表数据加载命令 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月22日 14:57:45[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6391
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、对于同一张表，如果不是分区，那要变更数据来源，就只有改变其hdfs存储路径：

修改tmp_fjs_ftp表的location，两种方式：

方式一：通过修改表DDL

alter table tmp_fjs_ftp set location 'hdfs://nameservice-ha/tmp/ast/ast ';

方式二：直接修改hive的meta info:

update ` tmp_fjs_ftp ` set `DB_LOCATION_URI` = replace(DB_LOCATION_URI,"oldpath","newpath");


方式二提示语法错误，待研究；




2、如果是带分区的同一张表，要加载数据，命令如下：

1）分区表：


CREATE TABLE test(


    ……


)


PARTITIONED BY ( country STRING ); #分区键和字段不能重复



2）加载数据:



LOAD DATA LOCAL INPATH '/path/to/local/files' OVERWRITE  INTO TABLE test PARTITION (country='CHINA')



有LOCAL表示从本地文件系统加载（文件会被拷贝到HDFS中）



无LOCAL表示从HDFS中加载数据（注意：文件直接被移动！！！而不是拷贝！！！ 并且。。文件名都不带改的。。）



OVERWRITE  表示是否覆盖表中数据（或指定分区的数据）（没有OVERWRITE  会直接APPEND，而不会滤重!）




