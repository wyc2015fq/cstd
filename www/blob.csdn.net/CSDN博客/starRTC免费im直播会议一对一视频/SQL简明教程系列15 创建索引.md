# SQL简明教程系列15 创建索引 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月23日 14:27:36[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：80
CREATE INDEX用于在表中创建索引。
索引使数据库应用程序可以更快地查找数据。
注：更新一个包含索引的表比更新一个没有索引的表更多的时间，这是由于索引本身也需要更新。因此，理想的做法是仅仅在常常被搜索的列（以及表）上面创建索引。
在表上创建一个简单的索引。允许使用重复的值：
CREATE INDEX index_name
ON table_name **(**column_name**)**
注："column_name" 规定需要索引的列。
在表上创建一个唯一的索引。唯一的索引意味着两个行不能拥有相同的索引值。
CREATE UNIQUE INDEX index_name
ON table_name **(**column_name**)**
如果希望以降序索引某个列中的值，您可以在列名之后添加保留字DESC：
CREATE INDEX PersonIndex
ON Person **(**LastName DESC**)**
假如您希望索引不止一个列，您可以在括号中列出这些列的名称，用逗号隔开：
CREATE INDEX PersonIndex
ON Person **(**LastName, FirstName**)**
可以使用 DROP INDEX命令删除表中的索引。
ALTER TABLE table_name DROP INDEX index_name
删除表：
DROP TABLE 表名
删除数据库：
DROP DATABASE  数据库名称
删除所有数据:
delete * from 表
