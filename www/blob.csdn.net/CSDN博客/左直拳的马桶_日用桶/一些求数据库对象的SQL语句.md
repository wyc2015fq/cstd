# 一些求数据库对象的SQL语句 - 左直拳的马桶_日用桶 - CSDN博客
2014年04月30日 22:08:25[左直拳](https://me.csdn.net/leftfist)阅读数：1707
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
```
use [mydb]
go
--存储过程
SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE routine_type='PROCEDURE'
AND SPECIFIC_NAME LIKE '...%'
ORDER BY CREATED;
--自定义函数
SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE routine_type='FUNCTION'
AND SPECIFIC_NAME LIKE '...%'
ORDER BY CREATED;
--视图
select name
  from sysobjects tab 
  where xtype = 'V'
  and name like '...%'
--表
  select name
  from sysobjects tab 
  where xtype = 'U'
  and name like '...%' or name like '...%'
```
