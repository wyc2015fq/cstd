# mysql删除重复记录，保存Id最小的一条 - z69183787的专栏 - CSDN博客
2016年12月02日 18:59:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：915
方法1：
1、创建一个临时表，选取需要的数据。
2、清空原表。
3、临时表数据导入到原表。
4、删除临时表。
mysql> select * from student;
+----+------+
| ID | NAME |
+----+------+
| 11 | aa |
| 12 | aa |
| 13 | bb |
| 14 | bb |
| 15 | bb |
| 16 | cc |
+----+------+
6 rows in set
mysql> create temporary table temp as select min(id),name from student group by name;
Query OK, 3 rows affected
Records: 3 Duplicates: 0 Warnings: 0
mysql> truncate table student;
Query OK, 0 rows affected
mysql> insert into student select * from temp;
Query OK, 3 rows affected
Records: 3 Duplicates: 0 Warnings: 0
mysql> select * from student;
+----+------+
| ID | NAME |
+----+------+
| 11 | aa |
| 13 | bb |
| 16 | cc |
+----+------+
3 rows in set
mysql> drop temporary table temp;
Query OK, 0 rows affected
这个方法，显然存在效率问题。
方法2：按name分组，把最小的id保存到临时表，删除id不在最小id集合的记录，如下：
mysql> create temporary table temp as select min(id) as MINID from student group by name;
Query OK, 3 rows affected
Records: 3 Duplicates: 0 Warnings: 0
mysql> delete from student where id not in (select minid from temp);
Query OK, 3 rows affected
mysql> select * from student;
+----+------+
| ID | NAME |
+----+------+
| 11 | aa |
| 13 | bb |
| 16 | cc |
+----+------+
3 rows in set 
方法3：直接在原表上操作，容易想到的sql语句如下：
mysql> delete from student where id not in (select min(id) from student group by name);
执行报错：1093 - You can't specify target table 'student' for update in FROM clause
原因是：更新数据时使用了查询，而查询的数据又做了更新的条件，mysql不支持这种方式。
怎么规避这个问题？
再加一层封装，如下：
mysql> delete from student where id not in (select minid from (select min(id) as minid from student group by name) b);
Query OK, 3 rows affected
mysql> select * from student;
+----+------+
| ID | NAME |
+----+------+
| 11 | aa |
| 13 | bb |
| 16 | cc |
+----+------+
3 rows in set
-------------------------------------------------------------------------------------------------
首先一个, 没看出来你为什么需要第一个in 子查询;
第二个, "You can't specify target table 'x' for update in FROM clause"
-- 不能 更新表的 同时, 在子查询里 select同样的表.
[](http://dev.mysql.com/doc/refman/5.1/en/subquery-errors.html)[http://dev.mysql.com/doc/refman/5.1/en/subquery-errors.html](http://dev.mysql.com/doc/refman/5.1/en/subquery-errors.html)
给出sql, 注意去重的话, 我们只需group by再取最小id即可, 不需要 having count(*)>1, 切切:
```java
delete B 
from bt_ask_to_cate_backup B,
   (SELECT min(id) id
     FROM bt_ask_to_cate_backup 
     group by ask_id,cate 
   ) T
where B.id <> T.id;
```
不对以上sql造成后果负责! 注意备份!
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
表结构如下：
mysql> desc test1;
+--------------+------------------+------+-----+---------+----------------+
| Field        | Type             | Null | Key | Default | Extra          |
+--------------+------------------+------+-----+---------+----------------+
| ID           | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
| SENDERNAME   | varchar(32)      | YES  |     | NULL    |                |
| RECEIVERNAME | varchar(64)      | YES  |     | NULL    |                |
| SENDER       | varchar(64)      | NO   |     | NULL    |                |
| RECEIVER     | varchar(64)      | NO   |     | NULL    |                |
| SUBJECT      | varchar(512)     | NO   |     | NULL    |                |
| CONTENT      | text             | NO   |     | NULL    |                |
| PRIORITY     | int(11)          | NO   | MUL | NULL    |                |
| STATUS       | int(11)          | NO   | MUL | NULL    |                |
| CREATETIME   | datetime         | NO   |     | NULL    |                |
| SENDTIME     | datetime         | YES  |     | NULL    |                |
+--------------+------------------+------+-----+---------+----------------+
subject和RECEIVER   需要做uniq key,但设计时未做，后面的数据就有很多重复的记录。
1. 查询需要删除的记录，会保留一条记录。
select a.id,a.subject,a.RECEIVER from test1 a left join (select c.subject,c.RECEIVER ,max(c.id) as  bid from test1 c where status=0 GROUP BY RECEIVER,SUBJECT having count(1) >1) b on a.id< b.bid where  a.subject=b.subject and a.RECEIVER = b.RECEIVER and a.id
 < b.bid
2. 删除重复记录，只保留一条记录。注意，subject,RECEIVER 要索引，否则会很慢的。
delete a from test1 a, (select c.subject,c.RECEIVER ,max(c.id) as  bid from test1 c where status=0 GROUP BY RECEIVER,SUBJECT having count(1) >1) b where a.subject=b.subject and a.RECEIVER = b.RECEIVER and a.id < b.bid;
一个比较巧的方法 from [http://www.php18.com/archives/Mysql_3012.html](http://www.php18.com/archives/Mysql_3012.html)
Mysql删除重复的记录（只保留一条）
原理很简单：
把不重复的记录复制到一个新table,  之后删除旧table, 再把新table改回旧table的名不就行了？
create table new_table_name  select *, count(distinct title_name) from old_table_name  group by title_name
new_table_name就是新表名
old_table_name 为原表名
title_name 就是不想重复的字段
注意：这个方法创建的新表后面会多一个字段，删除即可
