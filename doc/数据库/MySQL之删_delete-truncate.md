# MySQL之删_delete-truncate



MySQL增删改查之删_delete-truncate

一、DELETE语句

　　删除数据记录

1、在单表中删除行

语法：

```
DELETE  [IGNORE] FROM  tbl_name
   [WHERE where_condition]
   [ORDER BY ...]
   [LIMIT row_count]
```

　　①从表中删除满足WHERE条件的所有行；

　　②没有WHERE条件，则删除表中的所有行

基本格式：

　　delete from <表名> where <条件表达式>;

　　删除符合指定条件表达式的行记录

注意：

　　不能在delete和from中间指定列名，因为记录要删，总是按照行来删除的，不存在删除一行记录的某几列的值的情况。

 

例1：删除44号球员的罚款

DELETE FROM penalties

　　WHERE playerno=44;

例2：带子查询

DELETE FROM <表1>

　　WHERE joined >

　　(SELECT avg(joined) FROM <表2> WHERE town = 'Stratford');

注意：在WHERE子句的子查询中，不允许访问要删除行的表

DELETE FROM <表1>

　　WHERE joined >

　　(SELECT avg(joined) FROM <表1> WHERE town = 'Stratford');

错误代码：1093

You can't specify target table '<表1>' for update in FROM clause

 

用在DELETE语句中的ORDER BY子句和LIMIT子句的含义和用在UPDATE语句中是类似的

例3：删除4个最高的罚款

DELETE FROM penalties

　　ORDER BY amount DESC,playerno ASC

　　LIMIT 4;

 

2、从多个表中删除行

语法：

```
DELETE [IGNORE] tbl_name[.*] [, tbl_name[.*]] ...
  FROM table_references
  [WHERE where_condition]
```

注意：如果FROM中的表有别名，在DELETE子句中只能使用表别名

示例：从teams和matches表中删除所有3号球队的行

DELETE  teams, matches

　　FROM  teams, matches

　　WHERE  teams.teamno = matches.teamno

　　AND  teams.teamno=3;

解析：两个表中满足连接条件teams.teamno = matches.teamno和过滤条件teams.teamno=3的所有行被删除

 

 

二、TRUNCATE语句

　　清空一张（大）表更有效的方法是使用TRUNCATE语句，比DELETE快得多。

语法：

　　TRUNCATE [TABLE] tbl_name

与delete的区别：

相同点：

　　truncate和 delete只删除表中数据不删除表的结构

不同点：

　　用truncate，保留表结构(定义)，但删除表中所有记录;

　　用delete，只是删除部分记录(always with a WHERE clause)

@author：http://www.cnblogs.com/geaozhang/