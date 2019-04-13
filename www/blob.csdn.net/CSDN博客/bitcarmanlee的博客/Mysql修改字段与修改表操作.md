
# Mysql修改字段与修改表操作 - bitcarmanlee的博客 - CSDN博客


2018年06月05日 15:38:18[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：4217标签：[mysql																](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)[alter																](https://so.csdn.net/so/search/s.do?q=alter&t=blog)[修改字段																](https://so.csdn.net/so/search/s.do?q=修改字段&t=blog)[修改表																](https://so.csdn.net/so/search/s.do?q=修改表&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=修改字段&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=alter&t=blog)个人分类：[db dw																](https://blog.csdn.net/bitcarmanlee/article/category/6152094)
[
																								](https://so.csdn.net/so/search/s.do?q=alter&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)

当我们需要针对mysql的表名或者字段名需要修改的时候，这个时候就需要用到mysql的alter命令。
为了方便起见，首先我们新建一张用户表。
```python
create
```
```python
table
```
```python
user
```
```python
(
```
```python
`id`
```
```python
INT
```
```python
NOT
```
```python
NULL
```
```python
AUTO_INCREMENT,
```
```python
`court_id`
```
```python
INT
```
```python
NOT
```
```python
NULL
```
```python
DEFAULT
```
```python
1
```
```python
,
```
```python
`user_id`
```
```python
INT
```
```python
NOT
```
```python
NULL
```
```python
DEFAULT
```
```python
0
```
```python
,
```
```python
PRIMARY
```
```python
KEY
```
```python
(
```
```python
`id`
```
```python
)
)
  ENGINE = InnoDB
```
```python
DEFAULT
```
```python
CHARSET = utf8;
```
## 1. 添加删除字段
首先如果我们想删除user_id字段：
```python
mysql> alter table user drop user
```
```python
_id;
Query OK, 0 rows affected (0.26 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+---------+------+-----+---------+----------------+
```
```python
| Field    | Type    | Null | Key | Default | Extra          |
+----------+---------+------+-----+---------+----------------+
```
```python
| id       | int(11) | NO   | PRI | NULL    | auto
```
```python
_increment |
| court_
```
```python
id | int(11) | NO   |     | 1       |                |
```
```python
+----------+
```
```python
---------
```
```python
+------+
```
```python
-----
```
```python
+---------+
```
```python
----------------+
2 rows in set (0.01 sec)
```
注意：如果数据表中只剩余一个字段则无法使用DROP来删除字段。
我们发现user_id删除错误，想添加回来：
```python
mysql> alter table user add user
```
```python
_id int;
Query OK, 0 rows affected (0.25 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+---------+------+-----+---------+----------------+
```
```python
| Field    | Type    | Null | Key | Default | Extra          |
+----------+---------+------+-----+---------+----------------+
```
```python
| id       | int(11) | NO   | PRI | NULL    | auto
```
```python
_increment |
| court_
```
```python
id | int(11) | NO   |     | 1       |                |
```
```python
| user_id  | int(11) | YES  |     | NULL    |                |
+----------+---------+------+-----+---------+----------------+
```
```python
3 rows in set (0.00 sec)
```
可以看出来，此时user_id自动添加到了表的最后。
如果我们想指定新增字段的位置，可以使用MySQL提供的关键字 FIRST (设定位第一列)， AFTER 字段名（设定位于某个字段之后）。
```python
mysql> alter table user drop user
```
```python
_id;
Query OK, 0 rows affected (0.30 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> alter table user add user
```
```python
_id int first;
Query OK, 0 rows affected (0.24 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+---------+------+-----+---------+----------------+
```
```python
| Field    | Type    | Null | Key | Default | Extra          |
+----------+---------+------+-----+---------+----------------+
```
```python
| user
```
```python
_id  | int(11) | YES  |     | NULL    |                |
| id       | int(11) | NO   | PRI | NULL    | auto_
```
```python
increment |
```
```python
| court_id | int(11) | NO   |     | 1       |                |
+----------+---------+------+-----+---------+----------------+
```
```python
3 rows in set (0.01 sec)
```
此时first已经指定user_id为第一个字段。
```python
mysql> alter table user drop user
```
```python
_id;
Query OK, 0 rows affected (0.27 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> alter table user add user
```
```python
_id int after id;
Query OK, 0 rows affected (0.36 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+---------+------+-----+---------+----------------+
```
```python
| Field    | Type    | Null | Key | Default | Extra          |
+----------+---------+------+-----+---------+----------------+
```
```python
| id       | int(11) | NO   | PRI | NULL    | auto
```
```python
_increment |
| user_
```
```python
id  | int(11) | YES  |     | NULL    |                |
```
```python
| court_id | int(11) | NO   |     | 1       |                |
+----------+---------+------+-----+---------+----------------+
```
```python
3 rows in set (0.00 sec)
```
此时可以看到，user_id位于id后面了。
## 2.修改字段的名称类型
ALTER命令中使用 MODIFY 或 CHANGE 子句能满足以上需求。
```python
mysql> alter table user modify court
```
```python
_id char(16);
Query OK, 0 rows affected (0.25 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+----------+------+-----+---------+----------------+
```
```python
| Field    | Type     | Null | Key | Default | Extra          |
+----------+----------+------+-----+---------+----------------+
```
```python
| id       | int(11)  | NO   | PRI | NULL    | auto
```
```python
_increment |
| user_
```
```python
id  | int(11)  | YES  |     | NULL    |                |
```
```python
| court_id | char(16) | YES  |     | NULL    |                |
+----------+----------+------+-----+---------+----------------+
```
```python
3 rows in set (0.00 sec)
```
change的语法比较奇葩。请看下面的例子
```python
mysql> alter table user change court
```
```python
_id court_
```
```python
id int;
Query OK, 0 rows affected (0.24 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+---------+------+-----+---------+----------------+
```
```python
| Field    | Type    | Null | Key | Default | Extra          |
+----------+---------+------+-----+---------+----------------+
```
```python
| id       | int(11) | NO   | PRI | NULL    | auto
```
```python
_increment |
| user_
```
```python
id  | int(11) | YES  |     | NULL    |                |
```
```python
| court_id | int(11) | YES  |     | NULL    |                |
+----------+---------+------+-----+---------+----------------+
```
```python
3 rows in set (0.00 sec)
```
在 CHANGE 关键字之后，紧跟着的是你要修改的字段名，必须要指定新字段名及类型，即使字段名不变！
## 3.修改字段的默认值
为字段添加默认值
```python
mysql> alter table user alter court
```
```python
_id set default 1;
Query OK, 0 rows affected (0.04 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+---------+------+-----+---------+----------------+
```
```python
| Field    | Type    | Null | Key | Default | Extra          |
+----------+---------+------+-----+---------+----------------+
```
```python
| id       | int(11) | NO   | PRI | NULL    | auto
```
```python
_increment |
| user_
```
```python
id  | int(11) | YES  |     | NULL    |                |
```
```python
| court_id | int(11) | YES  |     | 1       |                |
+----------+---------+------+-----+---------+----------------+
```
```python
3 rows in set (0.00 sec)
```
删除某个字段的默认值
```python
mysql> alter table user alter court
```
```python
_id drop default;
Query OK, 0 rows affected (0.03 sec)
Records: 0  Duplicates: 0  Warnings: 0
```
```python
mysql> desc user;
+----------+---------+------+-----+---------+----------------+
```
```python
| Field    | Type    | Null | Key | Default | Extra          |
+----------+---------+------+-----+---------+----------------+
```
```python
| id       | int(11) | NO   | PRI | NULL    | auto
```
```python
_increment |
| user_
```
```python
id  | int(11) | YES  |     | NULL    |                |
```
```python
| court_id | int(11) | YES  |     | NULL    |                |
+----------+---------+------+-----+---------+----------------+
```
```python
3 rows in set (0.00 sec)
```
## 4.修改表名
可以在 ALTER TABLE 语句中使用 RENAME 子句来实现修改表名的目的。
```python
mysql
```
```python
>
```
```python
alter table user rename
```
```python
to
```
```python
user_test;
Query OK,
```
```python
0
```
```python
rows
```
```python
affected (
```
```python
0.05
```
```python
sec)
```
## 5.修改表的引擎
可以在ALTER TABLE中设置ENGINE属性，得到改变数据表引擎的目的。
```python
mysql> alter table user_test ENGINE = MYISAM;
Query OK, 0 rows affected (0.18 sec)
Records: 0  Duplicates: 0  Warnings: 0
mysql> show table status like 'user_test'\G
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
** 1. row **
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
Name: user_test
```
```python
Engine: MyISAM
```
```python
Version: 10
```
```python
Row_format: Fixed
```
```python
Rows: 0
```
```python
Avg
```
```python
_row_
```
```python
length: 0
```
```python
Data_length: 0
```
```python
Max
```
```python
_data_
```
```python
length: 3659174697238527
   Index_length: 1024
```
```python
Data_free: 0
```
```python
Auto_increment: 1
```
```python
Create_time: 2018-06-05 15:36:17
```
```python
Update_time: 2018-06-05 15:36:17
```
```python
Check_time: NULL
```
```python
Collation: utf8_general_ci
```
```python
Checksum: NULL
```
```python
Create_options:
```
```python
Comment:
```
```python
1 row in set (0.00 sec)
```

