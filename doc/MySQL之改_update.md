# MySQL之改_update



MySQL增删改查之改_update

UPDATE语句　　

　　进行数据记录的更新。

1、更新单个表中的值

语法：

```
UPDATE   [IGNORE]  table_reference
　　SET col_name1=expr1 [, col_name2=expr2,],...
　　[WHERE where_condition]
　　[ORDER BY ...]
　　[LIMIT row_count]
```

①有where子句的，将满足WHERE条件的所有行的一个或多个列值改为新的值；

②没有WHERE子句，则会对该表中所有的记录的指定的列的所有行，进行修改更新操作。

 

基本格式：

　　update <表名> set <列名>=<值或值的表达式>  where <条件表达式>；

例1：把95号球员的联盟会员号码改为2000（值）

UPDATE  players

　　SET  leagueno = 2000

　　WHERE playerno = 95;

例2： 把所有的罚款增加5%（值的表达式）

UPDATE penalties

　　SET amount = amount*1.05; 

例3： 把住在Stratford的球员的获胜局数设为0（指定列所有行）

UPDATE matches

　　SET won = 0

　　WHERE playerno IN

　　(SELECT playerno FROM players WHERE town='Stratford');

注意：在SET子句的子查询中，不允许访问要更新的表。

UPDATE <表1>

　　SET amount = amount-(SELECT avg(amount) FROM 表1);

！！上述写法不允许

解决(分两步)：

mysql> SET @avg_amount = (SELECT avg(amount) FROM <表1>);

mysql> UPDATE <表1>

​        SET amount = amount - @avg_amount;

------

UPDATE语句中的ORDER BY子句

　　UPDATE语句中可以使用ORDER BY子句，要求以排序的顺序来依次更新行。

在某些场景可能有用，例如：想要把所有罚款的罚款编号都加1

　　①如果从罚款编号为1的行开始更新，要么就会发生主键值重复异常；

　　②如果从罚款编号最大的行开始更新，就没有问题。

示例：把所有罚款的编号增加1

UPDATE  penalties

　　SET  paymentno = paymentno + 1

　　ORDER BY  paymentno DESC;

------

 UPDATE语句中的LIMIT子句

　　UPDATE语句中可以使用LIMIT子句，指定一次更新的行数

示例：把4个最高的罚款额增加5%(罚款额相同则更新编号小的球员)

UPDATE  penalties

　　SET  amount= amount *1.05

　　ORDER BY  amount DESC, playerno ASC

　　LIMIT 4;

 

 

2、更新多个表中的值

优点：要么两个表都更新，要么两个表都不更新

语法：

```
UPDATE   [IGNORE]  table_references
     SET col_name1=expr1 [, col_name2=expr2,],...
     [WHERE where_condition]
```

基本格式：

　　update <表名> set <列名1>=<值或值的表达式>, <列名2>=<值或值的表达式> ... where <条件表达式> ；

注意：

　　①table_references可以使用任何合法的连接语法

　　②不能使用ORDER BY和LIMIT子句

示例：把一个first分级球队的所有比赛的获胜局数设为0，并把first分级球队的队长编号改为112

UPDATE matches m,teams t

　　SET m.won = 0,*t.playerno = 112*

WHERE t.teamno = m.teamno

AND t.division = 'first';

解析：首先执行一个二表连接查询，从两个表中找到满足连接条件 t.teamno = m.teamno 的所有行，然后对这些行分别进行更新。

@author：http://www.cnblogs.com/geaozhang/