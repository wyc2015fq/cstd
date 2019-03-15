# SELECT中常用的子查询操作



MySQL中的子查询

　　是在MySQL中经常使用到的一个操作，不仅仅是用在DQL语句中，在DDL语句、DML语句中也都会常用到子查询。

子查询的定义：

　　子查询是将一个查询语句嵌套在另一个查询语句中；

　　在特定情况下，一个查询语句的条件需要另一个查询语句来获取，内层查询（inner query）语句的查询结果，可以为外层查询（outer query）语句提供查询条件。

 

特点（规范）：

　　①子查询必须放在小括号中

　　②子查询一般放在比较操作符的右边，以增强代码可读性

　　③子查询(小括号里的内容)可出现在几乎所有的SELECT子句中(如：SELECT子句、FROM子句、WHERE子句、ORDER BY子句、HAVING子句……)

 

(相关、不相关)子查询分类：

　　①标量子查询(scalar subquery)：返回1行1列一个值

　　②行子查询(row subquery)：返回的结果集是 1 行 N 列

　　③列子查询(column subquery)：返回的结果集是 N 行 1列

　　④表子查询(table subquery)：返回的结果集是 N 行 N 列

可以使用的操作符：= > < >= <= <> ANY IN SOME ALL EXISTS

注意：一个子查询会返回一个标量（就一个值）、一个行、一个列或一个表，这些子查询称之为标量、行、列和表子查询

　　1、如果子查询返回一个标量值（就一个值），那么外部查询就可以使用：=、>、<、>=、<=和<>符号进行比较判断；

　　2、如果子查询返回的不是一个标量值，而外部查询使用了比较符和子查询的结果进行了比较，那么就会抛出异常。

 

环境搭建：employee雇员表、department部门表、TENNIS数据库

```
mysql> select * from department;
+------+-----------+--------------+---------------------+
| d_id | d_name    | function     | address             |
+------+-----------+--------------+---------------------+
| 1001 | 科技部     | 研发产品       | 3号楼5层            |
| 1002 | 生产部     | 生产产品       | 5号楼1层            |
| 1003 | 销售部     | 策划销售       | 1号楼销售大厅        |
+------+-----------+--------------+---------------------+
3 rows in set (0.02 sec)

mysql> select * from employee;
+------+------+--------+------+------+--------------------+
| num  | d_id | name   | age  | sex  | homeaddr           |
+------+------+--------+------+------+--------------------+
|    1 | 1001 | 张三    |   26 | 男   | 北京市海淀区         |
|    2 | 1001 | 李四    |   24 | 女   | 上海市黄浦区         |
|    3 | 1002 | 王五    |   25 | 男   | 江西省赣州市         |
|    4 | 1004 | Aric   |   15 | 男   | England            |
+------+------+--------+------+------+--------------------+
4 rows in set (0.00 sec)
mysql> use TENNIS
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> show tables;
+-------------------+
| Tables_in_TENNIS  |
+-------------------+
| COMMITTEE_MEMBERS |
| MATCHES           |
| PENALTIES         |
| PLAYERS           |
| TEAMS             |
+-------------------+
5 rows in set (0.00 sec)
```

 

 

**一、不相关子查询**

　　不相关，主查询和子查询是不相关的关系。也就是意味着在子查询中没有使用到外部查询的表中的任何列。

　　先执行子查询，然后执行外部查询

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170424140641506-263017814.png)

1、标量子查询(scalar subquery)：返回1行1列一个值

　　因为是标量子查询，结果是一个值，所以可用来进行算数运算。

　　可以使用 = > < >= <= <> 操作符对子查询的结果进行比较：

```
mysql> select num,name
    -> from employee
    -> where d_id=(
    -> 　　select d_id
    -> 　　from department
    -> 　　where d_name='科技部');
+------+--------+
| num  | name   |
+------+--------+
|    1 | 张三    |
|    2 | 李四    |
+------+--------+
mysql> select num,name
    -> from employee
    -> where d_id=(
    -> 　　select d_id
    -> 　　from department
    -> 　　where d_name='财务部');
Empty set (0.00 sec)
注意：如果子查询返回空值，可能导致外部查询的where条件也为空，从而外部查询的结果集为空。
```

 

```
mysql> SELECT  playerno,town,sex
    -> FROM  PLAYERS
    -> WHERE (town,sex) = ((SELECT town FROM PLAYERS WHERE playerno=7),
    ->                     (SELECT sex FROM PLAYERS WHERE playerno=44));
+----------+-----------+-----+
| playerno | town      | sex |
+----------+-----------+-----+
|        2 | Stratford | M   |
|        6 | Stratford | M   |
|        7 | Stratford | M   |
|       39 | Stratford | M   |
|       57 | Stratford | M   |
|       83 | Stratford | M   |
|      100 | Stratford | M   |
+----------+-----------+-----+
7 rows in set (0.01 sec)
注意： (列,列,…)叫做行表达式，比较时是比较列的组合。
```

 

2、行子查询(row subquery)：返回的结果集是 1 行 N 列

　　使用行表达式进行比较，可以使用 = > < >= <= <> in操作符

```
mysql> select d_id from department;
+------+
| d_id |
+------+
| 1001 |
| 1002 |
| 1003 |
+------+

mysql> select * from employee
    -> where d_id in
    -> 　　(select d_id from department);
+------+------+--------+------+------+--------------------+
| num  | d_id | name   | age  | sex  | homeaddr           |
+------+------+--------+------+------+--------------------+
|    1 | 1001 | 张三    |   26 | 男   | 北京市海淀区         |
|    2 | 1001 | 李四    |   24 | 女   | 上海市黄浦区         |
|    3 | 1002 | 王五    |   25 | 男   | 江西省赣州市         |
+------+------+--------+------+------+--------------------+
```

解析：此处首先查询出department表中所有d_id字段的信息，并将结果作为条件，接着查询employee表中以d_id为条件的所有字段信息；NOT IN的效果与上面刚好相反。

 

3、列子查询(column subquery)：返回的结果集是 N 行 1列

　　必须使用 IN、ANY 和 ALL 操作符对子查询返回的结果进行比较

注意：ANY 和 ALL 操作符不能单独使用，其前面必须加上单行比较操作符= > < >= <= <>

1）带ANY关键字的子查询：ANY关键字表示满足其中任一条件

```
mysql> select * from employee
    -> where d_id !=any
    -> 　　(select d_id from department);
+------+------+--------+------+------+--------------------+
| num  | d_id | name   | age  | sex  | homeaddr           |
+------+------+--------+------+------+--------------------+
|    1 | 1001 | 张三    |   26 | 男   | 北京市海淀区         |
|    2 | 1001 | 李四    |   24 | 女   | 上海市黄浦区         |
|    3 | 1002 | 王五    |   25 | 男   | 江西省赣州市         |
|    4 | 1004 | Aric   |   15 | 男   | England            |
+------+------+--------+------+------+--------------------+
```

2）带ALL关键字的子查询：ALL关键字表示满足其中所有条件

```
mysql> select * from employee
    -> where d_id >=all
    -> 　　(select d_id from department);
+------+------+------+------+------+----------+
| num  | d_id | name | age  | sex  | homeaddr |
+------+------+------+------+------+----------+
|    4 | 1004 | Aric |   15 | 男    | England |
+------+------+------+------+------+----------+
```

 

注意：如果子查询的结果集中有null值，使用>ALL 和not in操作符时，必须去掉子查询结果集中的null值，否则查询结果错误

```
mysql> select * from department
    -> where d_id >all    #>all背后执行and操作
    -> 　　(select d_id from employee);
Empty set (0.01 sec)
结果为空：子查询的结果集中包含null值(子查询结果集中没有主查询里的1004行，则为空)。
```

 

4、表子查询(table subquery)：返回的结果集是 N 行 N 列

　　必须使用 IN、ANY 和 ALL 操作符对子查询返回的结果进行比较

示例：在committee_members表中，得到任职日期和卸任日期与具有Secretary职位的一行相同的所有行

```
mysql> select *
    -> from COMMITTEE_MEMBERS
    -> where (begin_date,end_date) in
    -> (
    ->   select begin_date,end_date
    ->   from COMMITTEE_MEMBERS
    ->   where position='Secretary'
    -> );
+----------+------------+------------+-----------+
| PLAYERNO | BEGIN_DATE | END_DATE   | POSITION  |
+----------+------------+------------+-----------+
|        6 | 1990-01-01 | 1990-12-31 | Secretary |
|        8 | 1990-01-01 | 1990-12-31 | Treasurer |
|        8 | 1991-01-01 | 1991-12-31 | Secretary |
|       27 | 1990-01-01 | 1990-12-31 | Member    |
|       27 | 1991-01-01 | 1991-12-31 | Treasurer |
|       57 | 1992-01-01 | 1992-12-31 | Secretary |
|      112 | 1992-01-01 | 1992-12-31 | Member    |
+----------+------------+------------+-----------+
7 rows in set (0.05 sec)
```

 

 

**二、相关子查询(correlated subquery)**

　　在子查询中使用到了外部查询的表中的任何列。

　　先执行外部查询，然后执行子查询

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170424143218428-110458898.png)

相关子查询的执行步骤：

　　①先执行外部查询，得到的行叫做候选行

　　②使用某个候选行来执行子查询

　　③使用子查询的返回值来决定该候选行是出现在最终的结果集中还是被丢弃

　　④重复以上步骤2和3，将所有的候选行处理完毕，得到最终的结果

 ![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170424143518569-1408502458.png)

示例：得到项目是‘研发产品’的雇员的编号

```
mysql> select num
    -> from employee
    -> where '研发产品'=(
    -> 　　select function
    -> 　　from department
    -> 　　where d_id=employee.d_id);
+------+
| num  |
+------+
|    1 |
|    2 |
+------+
```

解析：

　　1）主查询得到候选行，一行一行的拿去执行子查询；

　　2）主查询表employee的候选行的d_id和子查询的d_id匹配，返回值进行where过滤；

　　3）符合，加入最终结果集；

　　4）不符合，将候选行丢弃，接着进行处理下一个候选行。

 

带EXISTS关键字的相关子查询(EXISTS存在)

专门判断子查询的结果集是否不为空：

　　非空空返回true

　　空返回false

当返回的值为true时，外层查询语句将进行查询，否则不进行查询

```
mysql> select * from employee
    -> where exists
    -> (select d_name from department where d_id=1004);
Empty set (0.00 sec)
```

此处内层循环并没有查询到满足条件的结果，因此返回false，外层查询不执行

 

EXISTS关键字可以与其他的查询条件一起使用，条件表达式与EXISTS关键字之间用AND或者OR来连接

```
mysql> select * from employee
    -> where age>24 and exists
    -> (select d_name from department where d_id=1003);
+------+------+--------+------+------+--------------------+
| num  | d_id | name   | age  | sex  | homeaddr           |
+------+------+--------+------+------+--------------------+
|    1 | 1001 | 张三    |   26 | 男   | 北京市海淀区         |
|    3 | 1002 | 王五    |   25 | 男   | 江西省赣州市         |
+------+------+--------+------+------+--------------------+
```

@author：http://www.cnblogs.com/geaozhang/