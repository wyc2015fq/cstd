# SELECT中的多表连接



MySQL多表连接查询

连接(join)：将一张表中的行按照某个条件（连接条件）和另一张表中的行连接起来形成一个新行的过程。

　　根据连接查询返回的结果，分3类：

　　　　内连接（inner join）

　　　　外连接（outer join）

　　　　交叉连接（cross join）

　　根据连接条件所使用的操作符，分2类：

　　　　相等连接（使用等号操作符）

　　　　不等连接（不使用等号操作符） 

标准的连接语法：

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170423164228132-1341413247.png)

注意：

　　在连接查询中，一个列可能出现在多张表中，为了避免引起歧义，通常在列名前面加上表名或表别名作为前缀(例：s.sid、x.sid)---使用表别名作为前缀，可以使得SQL代码较短，使用的内存更少(例：stu s,xuanke as x)。

搭建环境：模拟选课

```
mysql> select * from stu;
+------+--------+---------+
| sid  | sname  | sphonum |
+------+--------+---------+
|    1 | 张三   |     110 |
|    2 | 李四   |     120 |
|    3 | 王五   |     130 |
+------+--------+---------+
3 rows in set (0.00 sec)

mysql> select * from tea;
+------+-----------+---------+
| tid  | tname     | tphonum |
+------+-----------+---------+
| 1113 | 相老师    |    1111 |
| 1114 | 冯老师    |    1112 |
+------+-----------+---------+
2 rows in set (0.00 sec)

mysql> select * from course;
+------+--------+
| cid  | cname  |
+------+--------+
|    1 | linux  |
|    2 | mysql  |
|    3 | hadoop |
+------+--------+
3 rows in set (0.00 sec)

mysql> select * from xuanke;
+------+------+------+--------+
| sid  | tid  | cid  | xuefen |
+------+------+------+--------+
|    1 | 1113 |    2 |      2 |
|    1 | 1114 |    1 |      4 |
|    1 | 1113 |    3 |      6 |
|    2 | 1113 |    2 |      2 |
|    2 | 1114 |    1 |      2 |
|    2 | 1113 |    3 |      2 |
+------+------+------+--------+
6 rows in set (0.00 sec)
```

 

1、内连接inner join

　　只返回两张表中所有满足连接条件的行，即使用比较运算符根据每个表中共有的列的值匹配两个表中的行。(inner关键字是可省略的)

①传统的连接写法：

　　在FROM子句中列出所有要连接的表的名字（进行表别名），以逗号分隔；

　　连接条件写在WHERE子句中；

注意：一旦给表定义了别名，那么原始的表名就不能在出现在该语句的其它子句中

```
mysql> select s.sname,c.cname,t.tname,x.xuefen
    -> from stu s,tea t,course c,xuanke x
    -> where s.sid=x.sid and t.tid=x.tid and c.cid=x.cid;
+--------+--------+-----------+--------+
| sname  | cname  | tname     | xuefen |
+--------+--------+-----------+--------+
| 张三    | linux  | 冯老师     |      4 |
| 李四    | linux  | 冯老师     |      2 |
| 张三    | mysql  | 相老师     |      2 |
| 李四    | mysql  | 相老师     |      2 |
| 张三    | hadoop | 相老师     |      6 |
| 李四    | hadoop | 相老师     |      2 |
+--------+--------+-----------+--------+
6 rows in set (0.08 sec)
```

 ②使用on子句(常用)：笔者比较喜欢的方法，因为觉得结构清晰明了。

```
mysql> select s.sname,t.tname,c.cname,x.xuefen
    -> from stu s
    -> join xuanke x
    -> 　　on s.sid=x.sid
    -> join tea t
    -> 　　on x.tid=t.tid
    -> join course c
    -> 　　on c.cid=x.cid;

结果如上……
```

表之间的关系以JOIN指定，ON的条件与WHERE条件相同。

 ③使用using子句

```
mysql> select s.sname,t.tname,c.cname,x.xuefen
    -> from stu s
    -> join xuanke x
    -> 　　using(sid)
    -> join tea t
    -> 　　using(tid)
    -> join course c
　　 -> 　　using(cid);

结果如上……
```

表之间的关系以join指定，using(连接列)进行连接匹配，类似于on。（相对用的会比较少） 

 

2、外连接outer join

　　使用外连接不但返回符合连接和查询条件的数据行，还返回不符合条件的一些行。

在MySQL数据库中外连接分两类(不支持全外连接)：

　　左外连接、右外连接。(outer关键字可省略)。

共同点：都返回符合连接条件和查询条件（即：内连接）的数据行

不同点：

　　①左外连接还返回左表中不符合连接条件，但符合查询条件的数据行。(所谓左表，就是写在left join关键字左边的表)

　　②右外连接还返回右表中不符合连接条件，但符合查询条件的数据行。(所谓右表，就是写在right join关键字右边的表)

```
mysql> select s.sname,x.xuefen
    -> from stu s
    -> left join xuanke x
    -> on s.sid=x.sid;
+--------+--------+
| sname  | xuefen |
+--------+--------+
| 张三   |      2  |
| 张三   |      4  |
| 张三   |      6  |
| 李四   |      2  |
| 李四   |      2  |
| 李四   |      2  |
| 王五   |   NULL  |
+--------+--------+
7 rows in set (0.00 sec)
```

解析：stu表是左表，xuanke表是右表：left join是左连接，stu表中”王五”没有选课，在xueke表中没有数据行，不符合连接条件，返回符合查询条件的数据行，所以xuefen为null。

```
mysql> select s.sname,x.xuefen
    -> from xuanke x
    -> right join stu s
    -> on x.sid=s.sid;

结果如上(用的是右连接的方式)
```

 

------

给连接查询附加条件：

　　1、写在WHERE子句中

　　2、使用AND和连接条件写在一起

！！！但是：

　　对于内连接，两种写法结果相同；

　　对于外连接，两种写法结果不同。

```
mysql> select s.sname,x.xuefen
    -> from stu s
    -> left join xuanke x
    -> on x.sid=s.sid
    -> 　　where sname='张三';
+--------+--------+
| sname  | xuefen |
+--------+--------+
| 张三    |      2 |
| 张三    |      4 |
| 张三    |      6 |
+--------+--------+
3 rows in set (0.01 sec)

mysql> select s.sname,x.xuefen                                                                 
    -> from (select * from stu where sname='张三') s
    -> left join xuanke x
    -> on x.sid=s.sid;
+--------+--------+
| sname  | xuefen |
+--------+--------+
| 张三    |      2 |
| 张三    |      4 |
| 张三    |      6 |
+--------+--------+
3 rows in set (0.00 sec)
```

①先连接后过滤

　　select ……from ……

　　left join ……

　　on 连接条件

　　　　where 过滤条件;

②先过滤后连接

　　select ……from (select ……from ……where 过滤条件)

　　left join ……

　　on 连接条件;

------

 

3、交叉连接—笛卡尔积

　　因为没有连接条件，所进行的表与表间的所有行的连接。

特点：

　　①连接查询没有写任何连接条件

　　②结果集中的总行数就是两张表中总行数的乘积(笛卡尔积)

注意：在实际中，应该要避免产生笛卡尔积的连接，特别是对于大表

```
mysql> select * from stu,tea,course,xuanke;
　　……
　　……
108 rows in set (0.00 sec)
```

若是想专门产生笛卡尔积，可以使用交叉连接

```
mysql> select *
    -> from stu
    -> crosss join tea;
+------+--------+---------+------+-----------+---------+
| sid  | sname  | sphonum | tid  | tname     | tphonum |
+------+--------+---------+------+-----------+---------+
|    1 | 张三    |     110 | 1113 | 相老师     |    1111 |
|    1 | 张三    |     110 | 1114 | 冯老师     |    1112 |
|    2 | 李四    |     120 | 1113 | 相老师     |    1111 |
|    2 | 李四    |     120 | 1114 | 冯老师     |    1112 |
|    3 | 王五    |     130 | 1113 | 相老师     |    1111 |
|    3 | 王五    |     130 | 1114 | 冯老师     |    1112 |
+------+--------+---------+------+-----------+---------+
6 rows in set (0.00 sec)
```

@author：http://www.cnblogs.com/geaozhang/



分类: [MySQL](https://www.cnblogs.com/geaozhang/category/1326927.html)