# MySQL临时表创建及旧表建新表



1、创建临时表

　　临时表是一张表，用来临时保存一些数据

特点：

　　只对创建该临时表的用户可见；

　　当会话结束时，MySQL自动删除临时表。

临时表的核心：建表和删表消耗资源极其少

------

 创建临时表的基本格式：

　　CREATE TEMPORARY TABLE  tbl_name(……); 

①创建的临时表在当前会话，正常使用

②断开连接，再重新连接后执行查询，抛出异常：

　　错误代码： 1146

　　Table ‘db_name.temtbl_name’ doesn‘t exist。//该临时表在会话结束的时候被系统删除。

------

 注意：用户可以创建一个和已有的普通表名字相同的临时表。

在这种情况下，

　　该用户只能看到临时表而看不见同名的普通表；

　　当临时表被删除后，才可以看到普通表。

示例：使用相同的名字创建一个普通表和临时表

```
mysql> create table test_table(num int);
Query OK, 0 rows affected (0.12 sec)

mysql> insert into test_table values(1);
Query OK, 1 row affected (0.04 sec)

mysql> create temporary table test_table(num int,name varchar(20));
Query OK, 0 rows affected (0.04 sec)

mysql> insert into test_table values(2,'临时表测试');
Query OK, 1 row affected (0.00 sec)

mysql> select * from test_table;
+------+-----------------+
| num  | name            |
+------+-----------------+
|    2 | 临时表测试       |
+------+-----------------+
1 row in set (0.00 sec)

mysql> drop table test_table;
Query OK, 0 rows affected (0.00 sec)

mysql> select * from test_table;
+------+
| num  |
+------+
|    1 |
+------+
1 row in set (0.00 sec)
```

 

Q：当创建表时，如果表已经存在了，则MySQL会返回出错消息，我们不希望看到报错的信息，该如何处理？

A：添加IF NOT EXISTS选项，则强制不显示这个出错消息；但是，语句执行失败---建表失败。

```
mysql> create table if not exists PLAYERS(id int(5),name varchar(20));
Query OK, 0 rows affected, 1 warning (0.00 sec)

mysql> show warnings;
+-------+------+--------------------------------+
| Level | Code | Message                        |
+-------+------+--------------------------------+
| Note  | 1050 | Table 'PLAYERS' already exists |
+-------+------+--------------------------------+
1 row in set (0.00 sec)

mysql> show tables;
+-------------------+
| Tables_in_TENNIS  |
+-------------------+
| COMMITTEE_MEMBERS |
| MATCHES           |
| PENALTIES         |
| PLAYERS           |
| TEAMS             |
| test_table        |
+-------------------+
6 rows in set (0.00 sec)
```

 

 

2、根据已有的表来创建新表

语法1：只想拷贝表结构

　　CREATE TABLE new_tbl LIKE orig_tbl;

将从源表复制列名、数据类型、大小、非空约束以及索引；而表的内容以及其它约束不会复制，新表是一张空表。

```
mysql> desc TEAMS;
+----------+---------+------+-----+---------+-------+
| Field    | Type    | Null | Key | Default | Extra |
+----------+---------+------+-----+---------+-------+
| TEAMNO   | int(11) | NO   | PRI | NULL    |       |
| PLAYERNO | int(11) | NO   |     | NULL    |       |
| DIVISION | char(6) | NO   |     | NULL    |       |
+----------+---------+------+-----+---------+-------+
3 rows in set (0.00 sec)

mysql> create table copy_TEAMS like TEAMS;
Query OK, 0 rows affected (0.11 sec)

mysql> select * from copy_TEAMS;
Empty set (0.00 sec)

mysql> desc copy_TEAMS;
+----------+---------+------+-----+---------+-------+
| Field    | Type    | Null | Key | Default | Extra |
+----------+---------+------+-----+---------+-------+
| TEAMNO   | int(11) | NO   | PRI | NULL    |       |
| PLAYERNO | int(11) | NO   |     | NULL    |       |
| DIVISION | char(6) | NO   |     | NULL    |       |
+----------+---------+------+-----+---------+-------+
3 rows in set (0.00 sec)
```

 

语法2：根据SELECT子查询的拷贝

　　CREATE TABLE new_tbl [AS] SELECT {*|column,...} FROM orig_tbl;

新表的结构由select列表决定；同时把查询返回的结果集中的行插入到目标表中；只能把非空约束带入到新表中(在有的时候就会显得很鸡肋了)，也不会复制索引

```
mysql> create table p_m
    -> as
    -> select a.NAME,a.SEX,b.MATCHNO,b.WON,b.LOST
    -> from PLAYERS a,MATCHES b
    -> where a.PLAYERNO=b.PLAYERNO;
Query OK, 13 rows affected (0.15 sec)
Records: 13  Duplicates: 0  Warnings: 0

mysql> select * from p_m;
+-----------+-----+---------+-----+------+
| NAME      | SEX | MATCHNO | WON | LOST |
+-----------+-----+---------+-----+------+
| Parmenter | M   |       1 |   3 |    1 |
| Parmenter | M   |       2 |   2 |    3 |
| Parmenter | M   |       3 |   3 |    0 |
| Baker     | M   |       4 |   3 |    2 |
| Hope      | M   |       5 |   0 |    3 |
| Everett   | M   |       6 |   1 |    3 |
| Brown     | M   |       7 |   3 |    0 |
| Newcastle | F   |       8 |   0 |    3 |
| Collins   | F   |       9 |   3 |    2 |
| Moorman   | F   |      10 |   3 |    2 |
| Bailey    | F   |      11 |   2 |    3 |
| Bailey    | F   |      12 |   1 |    3 |
| Newcastle | F   |      13 |   0 |    3 |
+-----------+-----+---------+-----+------+
13 rows in set (0.00 sec)
```

 

通过根据已有表来创建新表，添加选项temporary，创建临时副本用来做练习最合适：表内容可一样，会话结束，临时表自动删除，原始表中的内容不受任何影响。

注意：

　　如果在表名后面指定的列名和原始表中的列名相同，则可以改变列的大小和非空约束；

　　如果在表名后面指定的列名和原始表中的列名不同，则它作为一个新的列。

```
mysql> select * from TEAMS;
+--------+----------+----------+
| TEAMNO | PLAYERNO | DIVISION |
+--------+----------+----------+
|      1 |        6 | first    |
|      2 |       27 | second   |
+--------+----------+----------+

mysql> CREATE TABLE teams_copy
    -> (
    ->    teamno INTEGER NOT NULL PRIMARY KEY,
    ->    playerno INTEGER NULL,
    ->    division char(10) NOT NULL,
    ->    coach varchar(20)
    -> )
    -> as
    -> select * from TEAMS;
Query OK, 2 rows affected (0.11 sec)
Records: 2  Duplicates: 0  Warnings: 0

mysql> select * from teams_copy;
+-------+--------+----------+----------+
| coach | TEAMNO | PLAYERNO | DIVISION |
+-------+--------+----------+----------+
| NULL  |      1 |        6 | first    |
| NULL  |      2 |       27 | second   |
+-------+--------+----------+----------+
```

@author：http://www.cnblogs.com/geaozhang/