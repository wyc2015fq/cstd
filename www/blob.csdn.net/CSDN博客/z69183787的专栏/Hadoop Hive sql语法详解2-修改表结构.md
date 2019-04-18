# Hadoop Hive sql语法详解2-修改表结构 - z69183787的专栏 - CSDN博客
2016年10月26日 11:04:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：522
hive同样也面对传统数据库的一些操作，那么hive
1.如何增加分区、删除分区？
2.如何重命名表?
3.如何修改列的名字、类型、位置、注释?
4.如何增加/更新列?
5.如何增加表的元数据信息?
表添加一列 ：
- 
hive> ALTER TABLE pokes ADD COLUMNS (new_col INT);
复制代码
添加一列并增加列字段注释
- 
hive> ALTER TABLE invites ADD COLUMNS (new_col2 INT COMMENT 'a comment');
复制代码
更改表名：
- 
hive> ALTER TABLE events RENAME TO 3koobecaf;
复制代码
删除列：hive>
 DROP TABLE pokes;
增加、删除分区
•增加
- 
ALTER TABLE table_name ADD [IF NOT EXISTS] partition_spec [ LOCATION 'location1' ] partition_spec [ LOCATION 'location2' ] ...
- 
      partition_spec:
- 
  : PARTITION (partition_col = partition_col_value, partition_col = partiton_col_value, ...)
复制代码
•删除
- 
ALTER TABLE table_name DROP partition_spec, partition_spec,...
复制代码
重命名表•
- 
ALTER TABLE table_name RENAME TO new_table_name
复制代码
修改列的名字、类型、位置、注释：
- 
ALTER TABLE table_name CHANGE [COLUMN] col_old_name col_new_name column_type [COMMENT col_comment] [FIRST|AFTER column_name]
复制代码
•这个命令可以允许改变列名、数据类型、注释、列位置或者它们的任意组合
表添加一列 ：
- 
hive> ALTER TABLE pokes ADD COLUMNS (new_col INT);
复制代码
添加一列并增加列字段注释
- 
hive> ALTER TABLE invites ADD COLUMNS (new_col2 INT COMMENT 'a comment');
复制代码
增加/更新列
- 
ALTER TABLE table_name ADD|REPLACE COLUMNS (col_name data_type [COMMENT col_comment], ...)  
复制代码
• ADD是代表新增一字段，字段位置在所有列后面(partition列前)
     REPLACE则是表示替换表中所有字段。
增加表的元数据信息
- 
ALTER TABLE table_name SET TBLPROPERTIES table_properties table_properties:
- 
         :[property_name = property_value…..]
复制代码
•用户可以用这个命令向表中增加metadata
改变表文件格式与组织•
- 
ALTER TABLE table_name SET FILEFORMAT file_format
- 
ALTER TABLE table_name CLUSTERED BY(userid) SORTED BY(viewTime) INTO num_buckets BUCKETS
复制代码
这个命令修改了表的物理存储属性
创建／删除视图
- 
CREATE VIEW [IF NOT EXISTS] view_name [ (column_name [COMMENT column_comment], ...) ][COMMENT view_comment][TBLPROPERTIES (property_name = property_value, ...)] AS SELECT
复制代码
•增加视图
•如果没有提供表名，视图列的名字将由定义的SELECT表达式自动生成
•如果修改基本表的属性，视图中不会体现，无效查询将会失败
•视图是只读的，不能用LOAD/INSERT/ALTER
•DROP VIEW view_name
•删除视图
创建数据库
- 
CREATE DATABASE name
复制代码
显示命令
- 
show tables;
- 
show databases;
- 
show partitions ;
- 
show functions
- 
describe extended table_name dot col_name
复制代码
