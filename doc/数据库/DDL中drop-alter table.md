# DDL中drop-alter table



一、DROP TABLE语句：用于删除数据表

> 　　DROP TABLE removes one or more tables.
>
> 　　You must have the DROP privilege for each table.
>
> 　　All table data and the table definition are removed, so be careful with this statement!

 基本格式：

　　drop table <table_name>

```
DROP [TEMPORARY] TABLE [IF EXISTS]
    tbl_name [, tbl_name] ...
    [RESTRICT | CASCADE]
```

TEMPORARY：用于删除临时表(推荐针对性使用)，删除临时表不会结束当前的事务

IF EXISTS：用于在表不存在时，阻止错误消息的显示

[RESTRICT | CASCADE]：现在什么也不做 

 

其实删表语法如上，还是挺简单的，但是当一个表被删除时，所有依赖于该表的对象也被删除(例如约束、索引、视图和权限等)

所以，删除表之前(注意)：

　　①确认这个表没有人在使用，确实是一个废除的表

　　②看好确认是这个数据库的表

------

 

顺带提一下，如何重命名表：

```
RENAME  TABLE  tbl_name TO  new_tbl_name   
　　　　　　  [, tbl_name2 TO new_tbl_name2] …
```

当然，可以使用ALTER TABLE语句替换该语句，如下……

 

 

二、ALTER TABLE语句：添加、修改和删除列

基本格式：

　　1.修改表名：alter table 原表名 rename to 新表名;

　　2.新增列：alter table 表名 add [column] 列名 varchar(20) ;

　　3.删除列：alter table 表名 drop [column] 列名;

　　4.修改列名： alter table 表名 change 原列名 新列名 varchar(20) ;

　　5.修改列属性：alter table 表名 modify 列名 varchar(20) ;

```
ALTER [IGNORE]  TABLE  tbl_name 
　　 ADD [COLUMN] col_name  column_definition 
　　　　[FIRST | AFTER col_name ] 
　　|ADD [COLUMN] (col_name  column_definition,...)
　　|MODIFY [COLUMN] col_name column_definition
　　　　[FIRST | AFTER col_name] 
　　|DROP [COLUMN] col_name 
　　|CHANGE [COLUMN] old_col_name  new_col_name column_definition 
　　[FIRST|AFTER col_name]
```

FIRST：表示表头行

AFTER col_name：表示在col_name的后面，即下一行(没有before选项)

 

\##Copy一张TEAMS表用作示例

mysql> create table teams_copy

​    　-> as

​    　-> select * from TEAMS;

------

 1、添加列

　　①新列自动成为表中的最后一个列，除非指定了FIRST或AFTER选项(表中已有的行在新列上自动得到NULL值或默认值)

例1：添加type列给teams_copy表中

```
mysql> alter table teams_copy
    -> add type1 char(1);    #添加的新列归位最后一个列

mysql> alter table teams_copy
    -> add type2 char(1) default'1';    #添加列的同时设置默认值

mysql> alter table teams_copy
    -> add type3 char(1) after PLAYERNO;    #指定新列的位置

mysql> select * from teams_copy;
+--------+----------+-------+----------+-------+-------+
| TEAMNO | PLAYERNO | type3 | DIVISION | type1 | type2 |
+--------+----------+-------+----------+-------+-------+
|      1 |        6 | NULL  | first    | NULL  | 1     |
|      2 |       27 | NULL  | second   | NULL  | 1     |
+--------+----------+-------+----------+-------+-------+
```

　　②如果添加新列时指定了NOT NULL约束

则：

　　字符串列自动得到空字符串；

　　数字列自动得到0；

　　日期类型自动得到0日期；

　　时间类型自动得到00:00:00

```
mysql> alter table teams_copy
    -> add (
    ->   type4 char(1) NOT NULL,
    ->   type5 INTEGER NOT NULL,
    ->   type6 DATE NOT NULL,
    ->   tpye7 time not null );
```

mysql> select * from teams_copy;　　#截的一部分图

![img](https://images2015.cnblogs.com/blog/1113510/201705/1113510-20170501160735570-1478856789.png)

 

2、删除列

　　列值是否有数据都可以删除，依赖于该列的其它数据库对象，如索引、权限也将被删除

```
mysql> alter table teams_copy
    -> drop type6;

mysql> alter table teams_copy drop type2;
mysql> alter table teams_copy drop type4;
mysql> alter table teams_copy drop type5;    #只能一列一列的删

mysql> select * from teams_copy;
+--------+----------+-------+----------+-------+----------+
| TEAMNO | PLAYERNO | type3 | DIVISION | type1 | tpye7    |
+--------+----------+-------+----------+-------+----------+
|      1 |        6 | NULL  | first    | NULL  | 00:00:00 |
|      2 |       27 | NULL  | second   | NULL  | 00:00:00 |
+--------+----------+-------+----------+-------+----------+
```

 

3、修改列---经常用到

基本格式：

　　alter table <表名> modify <列名> 数据类型(……) 选项；

1）修改列的宽度：

　　如果是增加列宽，都无所谓

　　如果是减少列宽，就必须保证原有的值要能放得下，否则出错

2）改变数据类型时，列中的值必须能转变为新的类型

```
mysql> desc student;
+-------+---------------+------+-----+---------+-------+
| Field | Type          | Null | Key | Default | Extra |
+-------+---------------+------+-----+---------+-------+
| id    | int(11)       | NO   | PRI | NULL    |       |
| name  | varchar(20)   | NO   |     | NULL    |       |
| sex   | enum('M','F') | YES  |     | NULL    |       |
+-------+---------------+------+-----+---------+-------+
3 rows in set (0.00 sec)

mysql> select * from student;
+----+--------+------+
| id | name   | sex  |
+----+--------+------+
|  1 | 张三   | M    |
|  2 | 李四   | F    |
|  5 | 王五   | NULL |
+----+--------+------+
3 rows in set (0.00 sec)

mysql> alter table student
    -> modify id smallint not null;
Query OK, 3 rows affected (0.29 sec)
Records: 3  Duplicates: 0  Warnings: 0

mysql> alter table student
    -> modify name int not null;
ERROR 1366 (HY000): Incorrect integer value: '张三' for column 'name' at row 1
```

解析：student表中id列值可转换为新的数据类型，但是name列的值无法转变，所以modify失败。

3）修改列名(不要加引号)

```
mysql> alter table student
    -> change id 学号 int;

mysql> select * from student;
+--------+--------+------+
| 学号   | name    | sex  |
+--------+--------+------+
|      1 | 张三    | M    |
|      2 | 李四    | F    |
|      5 | 王五    | NULL |
+--------+--------+------+
```

 

4、修改约束

　　可以添加或删除约束，但是不能修改一个已有的约束。

```
ALTER [IGNORE] TABLE tbl_name
　　 ADD [CONSTRAINT [symbol]] PRIMARY KEY(col_name,...)
　　|ADD [CONSTRAINT [symbol]] UNIQUE (col_name,...)
　　|ADD [CONSTRAINT [symbol]] FOREIGN KEY (col_name,...)
　　　　References parent_table(col_name)
　　|DROP PRIMARY KEY
　　|DROP FOREIGN KEY fk_symbo
　　|DROP {INDEX|KEY} index_name
```

 

示例：定义两张表t1和t2

```
mysql> create table t1(
    -> 　　a int not null primary key,
    -> 　　b int not null);

mysql> create table t2(
    -> 　　a int not null primary key,
    -> 　　b int not null,
    -> 　　constraint c1 unique(b),
    -> 　　constraint fk1 foreign key(a) references t1(a)
    -> );

1）删除唯一性约束：删除对应的索引(index)即可
mysql> alter table t2
    -> drop index c1;

2）给t1添加外键约束，取名fk2
mysql> alter table t1
    -> add constraint fk2 foreign key(a) references t2(a);

3）删除外键fk2
mysql> alter table t1
    -> drop foreign key fk2;

4）删除t1表的主键
mysql> alter table t1
    -> drop primary key;
```

@author：http://www.cnblogs.com/geaozhang/