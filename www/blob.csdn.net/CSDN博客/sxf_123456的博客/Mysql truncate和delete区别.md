# Mysql truncate和delete区别 - sxf_123456的博客 - CSDN博客
2018年09月02日 21:12:56[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：255
1、truncate不用写日志，delete要写日志，前者的删除效率要高于后者,前者是整体删除后者是逐句删除
2、删除语句比较
清空表 truncate table table_name;
清空表 delete * from table_name;
3、truncate 是自增列的值会从1开始，而delete是从删除那条记录的ID+1开始
4、truncate是删除所有数据，而delete可以利用where条件语句删除部分数据
