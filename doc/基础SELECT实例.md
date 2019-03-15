# 基础SELECT实例



SELECT查询语句

　　---进行单条记录、多条记录、单表、多表、子查询……

```
SELECT
    [ALL | DISTINCT | DISTINCTROW ]
      [HIGH_PRIORITY]
      [MAX_STATEMENT_TIME = N]
      [STRAIGHT_JOIN]
      [SQL_SMALL_RESULT] [SQL_BIG_RESULT] [SQL_BUFFER_RESULT]
      [SQL_CACHE | SQL_NO_CACHE] [SQL_CALC_FOUND_ROWS]
    select_expr [, select_expr ...]
    [FROM table_references
      [PARTITION partition_list]
    [WHERE where_condition]
    [GROUP BY {col_name | expr | position}
      [ASC | DESC], ... [WITH ROLLUP]]
    [HAVING where_condition]
    [ORDER BY {col_name | expr | position}
      [ASC | DESC], ...]
    [LIMIT {[offset,] row_count | row_count OFFSET offset}]
    [PROCEDURE procedure_name(argument_list)]
    [INTO OUTFILE 'file_name'
        [CHARACTER SET charset_name]
        export_options
      | INTO DUMPFILE 'file_name'
      | INTO var_name [, var_name]]
    [FOR UPDATE | LOCK IN SHARE MODE]]
```

select书写使用技巧：

　　①确认需要访问数据来自哪几张表

　　　　from来自某张表或者某几张表

　　　　join添加某张表

　　　　on表连接条件

　　记住一点：每关联一个表就需要加上对应的on条件（on条件就是主外键条件）

　　②通过where条件过滤数据

　　③确认需求里面是否有分组聚合的含义

　　　　分组：group by

　　　　聚合：聚合函数

　　　　聚合条件过滤：having

　　④是否需要排序

　　　　order by

 

1、查询某张表所有数据

```
mysql> select * from temp;

解析：*代表所有列，temp代表表名，不带条件就查询所有数据
```

 

2、查询指定列和条件的数据

```
mysql> select name,age from temp where age = 22;

解析：查询name和age这两列，age 等于22的数据。
```

 

3、对查询的数据进行运算操作

```
mysql> select age+2,age/2,age-2,age*2 from temp where age-2 > 22;

mysql> select PLAYRNO,AMOUNT,AMOUNT*6.5 “Ren Min Bi” from PENALTIES;

解析：查询AMOUNT列数据乘6.5并改列名为Ren Min Bi

使用小括号可以改变运算的优先级
```

 

4、concat函数，字符串连接

```
mysql> select NAME,concat(TOWN,STREET,HOUSENO) “player Home Address” from PLAYERS;

解析：利用concat函数将TOWN,STREET,HOUSENO三列的字符串连接起来，别名列为player Home Address(可以在concat里加' '进行分隔)

注意：concat和null进行连接，会导致连接后的数据成为null

mysql> select * from t1;
+------+--------+--------+
| id   | First  | Last   |
+------+--------+--------+
|    1 | zhang  | jiacai |
|    2 | linghu | NULL   |
+------+--------+--------+

mysql> select id,concat(First,' ',Last) from t1;
+------+------------------------+
| id   | concat(First,' ',Last) |
+------+------------------------+
|    1 | zhang jiacai           |
|    2 | NULL                   |
+------+------------------------+
```

concat_ws函数，指定分隔符的字符串连接

```
mysql> select id,concat_ws(':',First,Last) "Full Name" from t1;
+------+--------------+
| id   | Full Name    |
+------+--------------+
|    1 | zhang:jiacai |
|    2 | linghu       |
+------+--------------+

圆括号里的第一个位置用来指定字符串连接的分隔符
```

 

5、as 列别名(或省略，留空格)

```
mysql> select id as 'num' from t1;

mysql> select id 'num',First from t1;
```

 

6、distinct关键字去掉重复数据

```
mysql> select * from t2;
+------+
| num  |
+------+
|    1 |
|    1 |
|    1 |
|    2 |
+------+

mysql> select distinct num from t2;
+------+
| num  |
+------+
|    1 |
|    2 |
+------+

distinct 多列：去重的是同行多列组合的重复数据

mysql> select distinct id, age from temp;
```

 

7、where条件查询

```
where语句中的条件比较：大于>、大于等于>=、小于<、小于等于<=、等于=、不等于<>

mysql> select * from tableName where a>2 or a>=3 or a<5 or a<=6 or a=7 or a<>0;
```

 

8、and 并且、or 或者、not非

```
mysql> select * from temp where age>20 and name=‘jack’;

解析：查询name等于jack并且年龄大于20的

mysql> select * from tmep where name=‘jack’ or name=‘jackson’;

解析：查询name是jack或是jackson的

mysql> select * from temp where not (age > 20);

解析：取小于等于20的数据

mysql> select * from temp where id not in(1, 2);

解析：查询id数不是1，也不是2的
```

 

9、between v1 and v2：v1和v2之间

```
mysql> select * form temp where age between 20 and 25;

解析：查询age在20和25之间的
```

 

10、in 查询：多个条件 类似于or

```
mysql> select * from temp where id in (1, 2, 3);

解析：查询id在括号中出现的数据
```

 

11、like 模糊查询

```
%:替代0个或多个字符

_：替代一个字符

mysql> select * from temp where name like ‘j%’;

解析：查询name以j开头的(%通配所有)

mysql> select * from temp where name like ‘%k%’;

解析：查询name包含k的

escape转义

mysql> select * from temp where name like ‘\_%’ escape ‘\’;

解析：指定\为转义字符，上面的就可以查询name中以“_”开头的数据
```

 

12、is null、is not null

```
mysql> select * from temp where name is null;

解析：查询为null的数据

mysql> select * from temp where name is not null;

解析：查询不为null的数据
```

 

13、order by排序：desc降序、asc升序

```
mysql> select * from temp order by id;  (默认asc升排序)

mysql> select * from temp order by id desc;  (指定降序排)

多列组合

mysql> select * from temp order by id, age;
```

 

14、limit子句：从结果集中选取最前面或最后面的几行

　　通常和order by连用，放其后面

limit  <获取的行数> [OFFSET <跳过的行数>]

limit [<跳过的行数>,] <获取的行数> 

```
mysql> select playerno,name from players order by playerno asc limit 3,5; 
mysql> select playerno,name from players order by playerno asc limit 5 offset 3;
解析：跳过前面的3行，从第4行开始取，取5行
```

注意：MySQL5.7 doesn't yet support 'LIMIT & IN/ALL/ANY/SOME subquery'

 

15、group by、having 分组聚合

select [聚合函数] 字段名 from 表名

　　[where 查询条件]

　　[group by 字段名]

　　[having 过滤条件]

```
mysql> select salary,count(*) from salary_tab
    -> where salary>=2000
    -> group by salary
    -> having count(*)>=0;
+---------+----------+
| salary  | count(*) |
+---------+----------+
| 2000.00 |        1 |
| 3000.00 |        1 |
+---------+----------+
```

 

\##########欲知详细解析，请听下回分解##########

@author：http://www.cnblogs.com/geaozhang/