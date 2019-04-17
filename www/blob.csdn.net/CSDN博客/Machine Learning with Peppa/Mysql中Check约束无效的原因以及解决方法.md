# Mysql中Check约束无效的原因以及解决方法 - Machine Learning with Peppa - CSDN博客





2017年11月30日 22:13:36[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：3294
所属专栏：[深入浅出数据库与Mysql](https://blog.csdn.net/column/details/18665.html)









首先看下面这段代码：

```
mysql> create table ss
    -> (
    ->  no int check(no <= 4 and no >=
    -> );
Query OK, 0 rows affected (0.11 sec)

mysql> insert into ss values(1);
Query OK, 1 row affected (0.01 sec)

mysql> select * from ss;
+------+
| no   |
+------+
|    1 |
+------+
1 row in set (0.00 sec)

mysql> insert into ss values(5);
Query OK, 1 row affected (0.03 sec)

mysql> select * from ss;
+------+
| no   |
+------+
|    1 |
|    5 |
+------+
2 rows in set (0.00 sec)
```

**很明显，CHECK语句在声明中并未起到作用**，为什么呢？

原因是因为，不同于SQL，在MYSQL中，CHECK只是一段可调用但无意义的子句。MySQL会直接忽略。

CHECK子句会被分析，但是会**被忽略。**

请参见“CREATE TABLE语法”：接受这些子句但又忽略子句的原因是为了提高兼容性，以便更容易地从其它SQL服务器中导入代码，并运行应用程序，创建带参考数据的表。

解决这个问题有两种办法： 

1.  

如果需要设置CHECK约束的字段范围小，并且比较容易列举全部的值，就可以考虑将该字段的类型设置为枚举类型 enum()或集合类型set()。比如性别字段可以这样设置，插入枚举值以外值的操作将不被允许。
```
1 mysql> create table checkDemoTable(a enum('男','女'),b int,id int,primary key(id));
 2 Query OK, 0 rows affected
 3 
 4 mysql> insert into checkDemoTable values('男',1,1);
 5 Query OK, 1 row affected
 6 
 7 mysql> select * from checkDemoTable;
 8 +----+---+----+
 9 | a  | b | id |
10 +----+---+----+
11 | 男 | 1 |  1 |
12 +----+---+----+
13 1 row in set
```

2. 

 如果需要设置CHECK约束的字段范围大，且列举全部值比较困难，比如：>0的值，那就只能使用触发器来代替约束实现数据的有效性了。如下代码，可以保证a>0。

```
CREATE TRIGGER TestField1_BeforeInsert BEFORE INSERT ON checkDemoTable
FOR EACH ROW
BEGIN
IF NEW.a < 0 THEN
SET NEW.a = 0;
END IF;
```

此外，在MYSQL中，域(Domain)与断言(Assertion)也是无法使用的，原因类似于CHECK，后续将会在其他文章中提到。更多相关代码可以访问 
[我的GITHUB](https://github.com/hatwanghacker/DataBase-Mysql)




