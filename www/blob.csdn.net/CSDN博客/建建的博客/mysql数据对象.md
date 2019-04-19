# mysql数据对象 - 建建的博客 - CSDN博客
2017年03月15日 11:05:31[纪建](https://me.csdn.net/u013898698)阅读数：777
学习目标:
- 了解掌握常见的几种数据库对象
- 学会如何创建具体的数据对象
mysql 常见的数据对象有哪些：
- DataBase/Schema
- Table
- Index
- View/Trigger/Function/Procedure
多Database用途：
- 业务的隔离
- 资源的隔离
表上的常用数据对象：
- 索引
- 约束
- 视图，触发器，函数，存储过程
索引
什么是数据库索引：
           索引就是数据库中数据的目录：
           索引和数据时两个对象
           索引主要是用来提高数据库的查询效率
           数据库中数据变更同样需要同步索引数据的变更，
                因为索引是按照B+TREE,排好位置的，一旦数据变化，则这个数据的相应位置也要变化，
                这样之后再查找，才能快速索引到，而变化位置就是索引的维护；
查看某个数据库命令：
```
mysql> help create index
Name: 'CREATE INDEX'
Description:
Syntax:
CREATE [ONLINE|OFFLINE] [UNIQUE|FULLTEXT|SPATIAL] INDEX index_name
    [index_type]
    ON tbl_name (index_col_name,...)
    [index_option] ..
```
UNIQUE:唯一索引（用户表的ID，手机号等，唯一信息）
SPATIAL:地理位置索引（搜索周边有哪些人）
两种创建索引及查看表中索引的方法
```
mysql> select * from vc;
+------+------+
| v    | c    |
+------+------+
| AB   | AB   |
+------+------+
**1** row in set (**0.00** sec)
 
mysql> create index idx_v on vc(v);
Query OK, **0** rows affected (**0.02** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
 
mysql> alter table vc add KEY idx_c (c);
Query OK, **0** rows affected (**0.02** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
 
mysql> show index from vc;
+-------+------------+----------+--------------+-------------+-----------+-------------+----------+--------+------+------------+---------+---------------+
| Table | Non_unique | Key_name | Seq_in_index | Column_name | Collation | Cardinality | Sub_part | Packed | Null | Index_type | Comment | Index_comment |
+-------+------------+----------+--------------+-------------+-----------+-------------+----------+--------+------+------------+---------+---------------+
| vc    |          **1** | idx_v    |            **1** | v           | A         |           **1** |     NULL | NULL   | YES  | BTREE      |         |               |
| vc    |          **1** | idx_c    |            **1** | c           | A         |           **1** |     NULL | NULL   | YES  | BTREE      |         |               |
+-------+------------+----------+--------------+-------------+-----------+-------------+----------+--------+------+------------+---------+---------------+
**2** rows in set (**0.00** sec)
 
mysql> show create table vc;
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Table | Create Table                                                                                                                                                   |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------+
| vc    | CREATE TABLE `vc` (
  `v` varchar(**5**) DEFAULT NULL,
  `c` char(**5**) DEFAULT NULL,
  KEY `idx_v` (`v`),
  KEY `idx_c` (`c`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------+
**1** row in set (**0.00** sec)
```
在mysql中 INDEX 和KEY 都代表了索引；
约束
约束：
唯一约束： unique
外键约束： CONSTRAINT
创建唯一约束（唯一索引可以促成唯一约束）：
- 唯一约束是一种特殊的索引
- 唯一约束可以是一个或者多个字段
- 位于约束可以在建表的时候建好，也可以后面补上
- 主键也是一种唯一约束
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719203333669-127687763.png)
索引有哪些：
- 主键索引  ID
- 单键索引  orderid
- 单键索引  bookid
- 组合索引 （userid + orderid）
唯一约束有哪些：
- 主键约束           ID
- 单键唯一索引     orderid
- 组合唯一索引     userid+orderid
添加唯一约束
添加主键索引： 
```
mysql> alter table order add primary key （id）
#实际上是给主键id增加了一个索引，而这个索引又是唯一的所以就这个索引就变成了唯一约束
```
添加唯一索引：
```
mysql>alter table order add unique key idx_uk_orderid(id)
```
外键约束
将两张表的数据通过某种条件关联起来
例子：买东西，订单表，用户信息表；
如果没有外键约束：可能存在不存在用户也能下单成功；
而如果使用外键约束，则在数据库层面是的不存在的用户不能下单成功。
创建外键约束：
将用户表和订单表通过外键关联起来：
```
mysql > alter table order add CONSTRAINT constarint_uid FOREIGN KEY (userid) REFERENCES user(userid)
```
创建外键约束的时候，可以指定在删除，更新父表（被参照的表）时，对子表（做外键关联的表）
进行的相应操作，包括可：restrict（限制），cascade（串联），set null 和 no action。
其中restrict和 no action 相同，是指限制在子表有关联记录的情况下，父表不能更新；
cascade表示父表在更新或删除时，更新或者删除子表对应的记录；
set null 表示父表在更新或者删除时子表的对应字段被set null。
选择后两种方式的时候要谨慎，可能会因为错误的操作导致数据的丢失。
在执行LOAD DATA和 ALTER TABLE 操作的时候，为了加快速度，可以暂时关闭外键约束： set foreign key_checks =0;等完成之后再开启：set foreign key_checks =1
```
mysql > alter table order add CONSTRAINT constarint_uid FOREIGN KEY (userid) REFERENCES user(userid) on delete restrict on update cascade；
```
on update cascade例子： 
```
mysql> select * from c_A;
+----+------+
| id | age  |
+----+------+
|  **1** |   **22** |
|  **2** |    **3** |
|  **3** |    **4** |
+----+------+
**3** rows in set (**0.00** sec)
 
mysql> select * from c_B;
+----+------+
| id | age  |
+----+------+
|  **1** |    **2** |
|  **2** |   **33** |
|  **3** |    **4** |
+----+------+
**3** rows in set (**0.00** sec)
 
mysql> update c_B set id=**11** where age = **2**;
Query OK, **1** row affected (**0.00** sec)
Rows matched: **1**  Changed: **1**  Warnings: **0**
 
mysql> select * from c_B;
+----+------+
| id | age  |
+----+------+
|  **2** |   **33** |
|  **3** |    **4** |
| **11** |    **2** |
+----+------+
**3** rows in set (**0.00** sec)
 
mysql> select * from c_A;
+----+------+
| id | age  |
+----+------+
|  **2** |    **3** |
|  **3** |    **4** |
| **11** |   **22** |
+----+------+
**3** rows in set (**0.00** sec)
```
注意：外键约束更改的只是约束的那个字段；
on delete cascade
|on delete cascade|insert|update|delete|
|----|----|----|----|
|parent|yes|只能更改子表中约束字段没有的值|yes|
|child|只能插入父表中约束字段有的值；|只能更改父表中约束字段没有的值|yes|
on update cascade
|on update cascade|insert|update|delete|
|----|----|----|----|
|parent|yes|yes|只能删子表中约束字段没有的值；|
|child|只能插入父表中约束字段有的值；|只能更新父表中约束字段没有的值|yes|
可以见child只能删除，其余都受限余父表的约束字段；
而parent都能插入，但其中一条受限于子表约束字段；
给order表增加了一个约束,（CONSTRAINT,约束的关键词）约束名 constarint_uid;
他是一个外键约束且order表中userid是外键，把order表中的userid 关联 到  user表的userid；
使用外键的注意事项：
- 必须是innodb表，其他引擎不支持外键
- 相互约束的字段类型必须要一样
- 主表的约束字段要求有索引（上面的例子中，user表就是主表，所以在user表中要userid要求加上索引 ）
- 约束名称必须要唯一，即使不在一张表上（constarint_uid 在整个库中是唯一的）
删除一个约束：
```
mysql> alter table order drop FOREIGN KEY constarint_uid;
```
视图view
view作用：
- 视图将一组查询语句构成的结果集，是一种虚拟结构，并不是实际数据
- 视图能简化数据库的访问，能够将多个查询语句结构化为一个虚拟结构
- 视图可以隐藏数据库后端表结构，提高数据库的安全性
- 视图也是一种权限管理，只对用户提供部分数据
创建一个已完成订单的视图：
```
mysql > create view order_view as select * from order where status = **1**
```
视图例子：
```
mysql> CREATE TABLE t (qty INT, price INT);
mysql> INSERT INTO t VALUES(**3**, **50**);
mysql> CREATE VIEW v AS SELECT qty, price, qty*price AS value FROM t;
mysql> SELECT * FROM v;
+------+-------+-------+
| qty  | price | value |
+------+-------+-------+
|    **3** |    **50** |   **150** |
+------+-------+-------+
```
触发器
触发器
是什么：
触发器是加在表上的特殊程序，当表上出现特定的事件
（insert/update/delete/alter table）时触发该程序执行。
对一个表的一个事件mysql中只能定义一个触发器
做什么：
- 数据订正；
- 迁移表；
- 实现特定的业务逻辑；
触发器有两个用户：
- 执行者
- 调用者  
触发器--基本语法
```
CREATE
[DEFINER = {user | CURRENT_USER}]     -- 定义执行着的权限
TRIGGER trigger_name trigger_time
trigger_event ON tbl_name
FOR EACH ROW                                    --涉及的每一行都会执行trigger_body
trigger_body  t
 
trigger_time:{BEFORE | AFTER}
trigger_event:{INSERT | UPDATE | DELETE}
```
delimiter
结束符的设定，默认是分号。但是在触发器应为要执行一组SQL，会出现分号所以讲结束符 设定一下；
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719203411372-299546139.png)
用 NEW,OLD来引用触发器中发生变化的记录内容
触发触发器的顺序：
before 触发器，行操作，after 触发器；其中任何一个步骤操作发生错误都不会继续执行剩下的操作。
如果是对事务表进行的操作，那么会整个作为一个事务被回滚（rollback）,但是如果是对非事务表进行的操作
，那么已经更新的记录将无法回滚，这也是设计触发器的时候需要注意得问题。
触发器的特点：
- 触发器对性能有损耗，应慎用
- 同一类事件在一个表中只能创建一次
- 对于事务表，触发器执行失败则整个语句回滚
- row格式主从复制，触发器不会在从属库上执行
- 使用触发器时应该防止递归执行
可以协助应用在数据库端确保数据的完整性。
存储过程
定义：存储过程是存储在数据库端的一组SQL语句集，用户可以通过存储过程名和传参多次调用的程序模块。
特点：
- 使用灵活，可以使用流控制语句，自定义变量等完成复杂的业务逻辑
- 提高数据安全性，屏蔽应用程序直接对表的操作，易于进行审计
- 减少网络传输
- 提高代码维护的复杂度，实际使用中要评估场景是否适合
存储过程--基本语法
```
CREATE 
     [ DEFINER = { user | CURRENT_USER } ]                 --定义执行着的权限
     PROCEDURE  sp_name ( [ proc_parameter[ ,... ] ] )
     [ characteristic .. ]  routine_body
 
proc_parameter:
     [ IN | OUT | INOUT]  param_name  type
                                   type:
                                            Any valid MySQL data type
```
IN:输入参数，表示该参数值在调用存储过程中已经指定好了，在调用存储过程中修改该参数不会返回
OUT：输出参数，可以在存储过程内部被改变，可以返回
INOUT:输入输出参数；
characteristic:
     COMMENT 'string'                    --注释
     | [NOT] DETERMINISTIC          --是否会返回确定值
routine_body:
     Valid SQL routine statement    --与trigger类似
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719203546013-2026339282.png)
重点：
- 中间变量定义：DECLARE
- 流控制语句
- 参数传入
查询：数据库中有哪些存储过程
```
mysql> show PROCEDURE STATUS ;
```
查看：某个触发器的详情
```
mysql> SHOW TRIGGER STATUS ;
```
存储过程的使用：
1.设置参数值：
```
mysql> set @total = **5**;
mysql> set @res = **0**;
```
2.调用存储过程：
```
mysql> call proc_test1(@total,@res);
```
3.查看返回值：
```
mysql> select @res
```
删除存储过程或者函数：
```
DROP {PROCEDURE | FUNCTION}  [IF  EXISTS]  sp_name
```
查看存储过程或者函数
SHOW {procedure | function}  status like ‘file_in_stock’
查看存储过程或者函数的定义
show  create {procedure |  function}  sp_name
存储过程-流控制语言
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719203600451-1680784622.png)
自定义函数
- 自定义函数与存储过程类似，但是必须带有返回值
- 自定义函数与sum(),max(),等mysql原生函数使用方法类似： select func(val); select * from tbl where col = func(val)
- 由于自定义函数可能在遍历数据中使用，要注意性能损耗
自定义函数-基本语法
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719203613779-1387410676.png)
最终通过RETURN；
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719203631529-13286883.png)
调用
```
mysql> select func_test1(**4**);
```
确认权限：
创建存储过程或者函数需要：create  routine
修改或者删除存储过程需要：alter  routine
执行存储过程 需要 ：           execute 
可以嵌套使用；
小结
- 触发器和存储过程不利于水平扩展，多用于统计和运维操作中；
- 还有代码管理维护成本比较高；
- 但是：
- 简化应用开发，减少数据传输，提高处理效率；
总结
-  索引的创建于查看
-  约束：
- 　　唯一约束，
- 外键约束：
- alter table order add CONSTRAINT constarint_uid FOREIGN KEY (userid) REFERENCES user(userid)；
- 外键约束四种模式：restrict，no action,set null,cascade;
-  VIEW
-  TIGGER：
- 一个表的一个事件只能定义一个触发器
- delimiter //
- NEW.age  OLD.age
-  PROCEDURE:
- DECLARE,
- 参数传入返回
- 流控制语言
- 使用三步骤
- 初始化参数
- 调用存储过程
- 查看放回值
-  function：
- 必须带有返回值
- 使用select func_test1(14)；
-  SUBSTRING( goods_name,1,5 )：从位置1截取goods_name5个字符
-  A rigth join B on..:B显示B中null字段；
