# 深入解析MySQL视图VIEW



>  阅读目录---深入解析MySQL视图
>
> 1、创建视图
>
> 2、[查看视图](https://www.cnblogs.com/geaozhang/p/6792369.html#chakanshitu)
>
> 3、[视图的更改](https://www.cnblogs.com/geaozhang/p/6792369.html#shitudegenggai)
>
> 　　1、create or replace view
>
> 　　2、alter
>
> 　　3、DML
>
> 　　4、drop
>
> 4、[使用with check option约束](https://www.cnblogs.com/geaozhang/p/6792369.html#yueshu)
>
> 　　[嵌套视图](https://www.cnblogs.com/geaozhang/p/6792369.html#qiantao)
>
> 5、[定义视图时的其他选项](https://www.cnblogs.com/geaozhang/p/6792369.html#qitaxuanxiang)：algorithm、definer、[SQL security视图权限](https://www.cnblogs.com/geaozhang/p/6792369.html#shituquanxian)
>
> 6、[视图查询语句的处理](https://www.cnblogs.com/geaozhang/p/6792369.html#chuli)
>
> 　　1、替代方法
>
> 　　2、具体化方法

Q：什么是视图？视图是干什么用的？

A：

　　视图（view）是一种虚拟存在的表，是一个逻辑表，本身并不包含数据。作为一个select语句保存在数据字典中的。

　　通过视图，可以展现基表的部分数据；视图数据来自定义视图的查询中使用的表，使用视图动态生成。

基表：用来创建视图的表叫做基表base table

Q：为什么要使用视图？

A：因为视图的诸多优点，如下

　　1）简单：使用视图的用户完全不需要关心后面对应的表的结构、关联条件和筛选条件，对用户来说已经是过滤好的复合条件的结果集。

　　2）安全：使用视图的用户只能访问他们被允许查询的结果集，对表的权限管理并不能限制到某个行某个列，但是通过视图就可以简单的实现。

　　3）数据独立：一旦视图的结构确定了，可以屏蔽表结构变化对用户的影响，源表增加列对视图没有影响；源表修改列名，则可以通过修改视图来解决，不会造成对访问者的影响。

总而言之，使用视图的大部分情况是为了保障数据安全性，提高查询效率。

 

一、创建视图

```
CREATE [OR REPLACE] [ALGORITHM = {UNDEFINED | MERGE | TEMPTABLE}]
    VIEW view_name [(column_list)]
    AS select_statement
   [WITH [CASCADED | LOCAL] CHECK OPTION]
```

1）OR REPLACE：表示替换已有视图

2）ALGORITHM：表示视图选择算法，默认算法是UNDEFINED(未定义的)：MySQL自动选择要使用的算法 ；merge合并；temptable临时表

3）select_statement：表示select语句

4）[WITH [CASCADED | LOCAL] CHECK OPTION]：表示视图在更新时保证在视图的权限范围之内

　　cascade是默认值，表示更新视图的时候，要满足视图和表的相关条件

　　local表示更新视图的时候，要满足该视图定义的一个条件即可

TIPS：推荐使用WHIT [CASCADED|LOCAL] CHECK OPTION选项，可以保证数据的安全性 

基本格式：

　　create view <视图名称>[(column_list)]

​       as select语句

​       with check option;

1、在单表上创建视图

```
mysql> create view v_F_players(编号,名字,性别,电话)
    -> as
    -> select PLAYERNO,NAME,SEX,PHONENO from PLAYERS
    -> where SEX='F'
    -> with check option;
Query OK, 0 rows affected (0.00 sec)

mysql> desc v_F_players;
+--------+----------+------+-----+---------+-------+
| Field  | Type     | Null | Key | Default | Extra |
+--------+----------+------+-----+---------+-------+
| 编号    | int(11)  | NO   |     | NULL    |       |
| 名字    | char(15) | NO   |     | NULL    |       |
| 性别    | char(1)  | NO   |     | NULL    |       |
| 电话    | char(13) | YES  |     | NULL    |       |
+--------+----------+------+-----+---------+-------+
4 rows in set (0.00 sec)

mysql> select * from  v_F_players;
+--------+-----------+--------+------------+
| 编号    | 名字      | 性别    | 电话        |
+--------+-----------+--------+------------+
|      8 | Newcastle | F      | 070-458458 |
|     27 | Collins   | F      | 079-234857 |
|     28 | Collins   | F      | 010-659599 |
|    104 | Moorman   | F      | 079-987571 |
|    112 | Bailey    | F      | 010-548745 |
+--------+-----------+--------+------------+
5 rows in set (0.02 sec)
```

2、在多表上创建视图

```
mysql> create view v_match
    -> as 
    -> select a.PLAYERNO,a.NAME,MATCHNO,WON,LOST,c.TEAMNO,c.DIVISION
    -> from 
    -> PLAYERS a,MATCHES b,TEAMS c
    -> where a.PLAYERNO=b.PLAYERNO and b.TEAMNO=c.TEAMNO;
Query OK, 0 rows affected (0.03 sec)

mysql> select * from v_match;
+----------+-----------+---------+-----+------+--------+----------+
| PLAYERNO | NAME      | MATCHNO | WON | LOST | TEAMNO | DIVISION |
+----------+-----------+---------+-----+------+--------+----------+
|        6 | Parmenter |       1 |   3 |    1 |      1 | first    |
|       44 | Baker     |       4 |   3 |    2 |      1 | first    |
|       83 | Hope      |       5 |   0 |    3 |      1 | first    |
|      112 | Bailey    |      12 |   1 |    3 |      2 | second   |
|        8 | Newcastle |      13 |   0 |    3 |      2 | second   |
+----------+-----------+---------+-----+------+--------+----------+
5 rows in set (0.04 sec)
```

视图将我们不需要的数据过滤掉，将相关的列名用我们自定义的列名替换。视图作为一个访问接口，不管基表的表结构和表名有多复杂。

 　　如果创建视图时不明确指定视图的列名，那么列名就和定义视图的select子句中的列名完全相同；

　　如果显式的指定视图的列名就按照指定的列名。

注意：显示指定视图列名，要求视图名后面的列的数量必须匹配select子句中的列的数量。

[【回到顶部】](https://www.cnblogs.com/geaozhang/p/6792369.html#mulu)

二、查看视图

1、使用show create view语句查看视图信息

```
mysql> show create view v_F_players\G;
*************************** 1. row ***************************
                View: v_F_players
         Create View: CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `v_F_players` AS select `PLAYERS`.`PLAYERNO` AS `编号`,`PLAYERS`.`NAME` AS `名字`,`PLAYERS`.`SEX` AS `性别`,`PLAYERS`.`PHONENO` AS `电话` from `PLAYERS` where (`PLAYERS`.`SEX` = 'F') WITH CASCADED CHECK OPTION
character_set_client: utf8
collation_connection: utf8_general_ci
1 row in set (0.00 sec)
```

2、视图一旦创建完毕，就可以像一个普通表那样使用，视图主要用来查询

mysql> select * from view_name;

3、有关视图的信息记录在information_schema数据库中的views表中

```
mysql> select * from information_schema.views 
    -> where TABLE_NAME='v_F_players'\G;
*************************** 1. row ***************************
       TABLE_CATALOG: def
        TABLE_SCHEMA: TENNIS
          TABLE_NAME: v_F_players
     VIEW_DEFINITION: select `TENNIS`.`PLAYERS`.`PLAYERNO` AS `编号`,`TENNIS`.`PLAYERS`.`NAME` AS `名字`,`TENNIS`.`PLAYERS`.`SEX` AS `性别`,`TENNIS`.`PLAYERS`.`PHONENO` AS `电话` from `TENNIS`.`PLAYERS` where (`TENNIS`.`PLAYERS`.`SEX` = 'F')
        CHECK_OPTION: CASCADED
        IS_UPDATABLE: YES
             DEFINER: root@localhost
       SECURITY_TYPE: DEFINER
CHARACTER_SET_CLIENT: utf8
COLLATION_CONNECTION: utf8_general_ci
1 row in set (0.00 sec)        
```

[【回到顶部】](https://www.cnblogs.com/geaozhang/p/6792369.html#mulu)

三、视图的更改

1、CREATE OR REPLACE VIEW语句修改视图

基本格式：

　　create or replace view view_name as select语句;

在视图存在的情况下可对视图进行修改，视图不在的情况下可创建视图

2、ALTER语句修改视图

```
ALTER
    [ALGORITHM = {UNDEFINED | MERGE | TEMPTABLE}]
    [DEFINER = { user | CURRENT_USER }]
    [SQL SECURITY { DEFINER | INVOKER }]
VIEW view_name [(column_list)]
AS select_statement
    [WITH [CASCADED | LOCAL] CHECK OPTION]
```

注意：修改视图是指修改数据库中已存在的表的定义，当基表的某些字段发生改变时，可以通过修改视图来保持视图和基本表之间一致

3、DML操作更新视图

　　因为视图本身没有数据，因此对视图进行的dml操作最终都体现在基表中

```
mysql> create view v_student as select * from student;

mysql> select * from v_student;
+--------+--------+------+
| 学号    | name   | sex  |
+--------+--------+------+
|      1 | 张三    | M    |
|      2 | 李四    | F    |
|      5 | 王五    | NULL |
+--------+--------+------+

mysql> update v_student set name='钱六' where 学号='1';

mysql> select * from student;
+--------+--------+------+
| 学号    | name   | sex  |
+--------+--------+------+
|      1 | 钱六    | M    |
|      2 | 李四    | F    |
|      5 | 王五    | NULL |
+--------+--------+------+
```

当然，视图的DML操作，不是所有的视图都可以做DML操作。

有下列内容之一，视图不能做DML操作：

　　①select子句中包含distinct

　　②select子句中包含组函数

　　③select语句中包含group by子句

　　④select语句中包含order by子句

　　⑤select语句中包含union 、union all等集合运算符

　　⑥where子句中包含相关子查询

　　⑦from子句中包含多个表

　　⑧如果视图中有计算列，则不能更新

　　⑨如果基表中有某个具有非空约束的列未出现在视图定义中，则不能做insert操作

4、drop删除视图

　　删除视图是指删除数据库中已存在的视图，删除视图时，只能删除视图的定义，不会删除数据，也就是说不动基表：

```
DROP VIEW [IF EXISTS]   
view_name [, view_name] ...
```

mysql> drop view v_student;

如果视图不存在，则抛出异常；使用IF EXISTS选项使得删除不存在的视图时不抛出异常。

[【回到顶部】](https://www.cnblogs.com/geaozhang/p/6792369.html#mulu)

四、使用WITH CHECK OPTION约束 

对于可以执行DML操作的视图，定义时可以带上WITH CHECK OPTION约束

作用：

　　对视图所做的DML操作的结果，不能违反视图的WHERE条件的限制。

示例：创建视图，包含1960年之前出生的所有球员（老兵）

```
mysql> create view v_veterans
    -> as
    -> select * from PLAYERS
    -> where birth_date < '1960-01-01'
    -> with check option;
Query OK, 0 rows affected (0.01 sec)

mysql> select * from v_veterans;
+----------+---------+----------+------------+-----+--------+----------------+---------+----------+-----------+------------+----------+
| PLAYERNO | NAME    | INITIALS | BIRTH_DATE | SEX | JOINED | STREET         | HOUSENO | POSTCODE | TOWN      | PHONENO    | LEAGUENO |
+----------+---------+----------+------------+-----+--------+----------------+---------+----------+-----------+------------+----------+
|        2 | Everett | R        | 1948-09-01 | M   |   1975 | Stoney Road    | 43      | 3575NH   | Stratford | 070-237893 | 2411     |
|       39 | Bishop  | D        | 1956-10-29 | M   |   1980 | Eaton Square   | 78      | 9629CD   | Stratford | 070-393435 | NULL     |
|       83 | Hope    | PK       | 1956-11-11 | M   |   1982 | Magdalene Road | 16A     | 1812UP   | Stratford | 070-353548 | 1608     |
+----------+---------+----------+------------+-----+--------+----------------+---------+----------+-----------+------------+----------+
3 rows in set (0.02 sec)
```

此时，使用update对视图进行修改：

```
mysql> update v_veterans
    -> set BIRTH_DATE='1970-09-01'
    -> where PLAYERNO=39;
ERROR 1369 (HY000): CHECK OPTION failed 'TENNIS.v_veterans'
```

因为违反了视图中的WHERE birth_date < '1960-01-01'子句，所以抛出异常；

利用with check option约束限制，保证更新视图是在该视图的权限范围之内。

 

嵌套视图：定义在另一个视图的上面的视图

```
mysql> create view v_ear_veterans
    -> as
    -> select * from v_veterans
　　 -> where JOINED < 1980;
```

使用WITH CHECK OPTION约束时，(不指定选项则默认是CASCADED)

可以使用CASCADED或者 LOCAL选项指定检查的程度：

　　①WITH CASCADED CHECK OPTION：检查所有的视图

　　　　例如：嵌套视图及其底层的视图

　　②WITH LOCAL CHECK OPTION：只检查将要更新的视图本身

　　　　对嵌套视图不检查其底层的视图　

[【回到顶部】](https://www.cnblogs.com/geaozhang/p/6792369.html#mulu)

五、定义视图时的其他选项

```
CREATE [OR REPLACE]   
　　[ALGORITHM = {UNDEFINED | MERGE | TEMPTABLE}]  
　　[DEFINER = { user | CURRENT_USER }]  
　　[SQL SECURITY { DEFINER | INVOKER }]
VIEW view_name [(column_list)]  
AS select_statement  
　　[WITH [CASCADED | LOCAL] CHECK OPTION]
```

1、ALGORITHM选项：选择在处理定义视图的select语句中使用的方法

　　①UNDEFINED：MySQL将自动选择所要使用的算法

　　②MERGE：将视图的语句与视图定义合并起来，使得视图定义的某一部分取代语句的对应部分

　　③TEMPTABLE：将视图的结果存入临时表，然后使用临时表执行语句

缺省ALGORITHM选项等同于ALGORITHM = UNDEFINED

 

2、DEFINER选项：指出谁是视图的创建者或定义者

　　①definer= '用户名'@'登录主机'

　　②如果不指定该选项，则创建视图的用户就是定义者，指定关键字CURRENT_USER(当前用户)和不指定该选项效果相同

 

3、SQL SECURITY选项：要查询一个视图，首先必须要具有对视图的select权限。

　　但是，如果同一个用户对于视图所访问的表没有select权限，那会怎么样？

SQL SECURITY选项决定执行的结果：

　　①SQL SECURITY DEFINER：定义(创建)视图的用户必须对视图所访问的表具有select权限，也就是说将来其他用户访问表的时候以定义者的身份，此时其他用户并没有访问权限。

　　②SQL SECURITY INVOKER：访问视图的用户必须对视图所访问的表具有select权限。

缺省SQL SECURITY选项等同于SQL SECURITY DEFINER　

视图权限总结：

　　使用root用户定义一个视图(推荐使用第一种)：u1、u2

　　　　1）u1作为定义者定义一个视图，u1对基表有select权限，u2对视图有访问权限：u2是以定义者的身份访问可以查询到基表的内容；

　　　　2）u1作为定义者定义一个视图，u1对基表没有select权限，u2对视图有访问权限，u2对基表有select权限：u2访问视图的时候是以调用者的身份，此时调用者是u2，可以查询到基表的内容。

[【回到顶部】](https://www.cnblogs.com/geaozhang/p/6792369.html#mulu)

六、视图查询语句的处理

示例：所有有罚款的球员的信息

创建视图：

```
mysql> create view cost_raisers
    -> as
    -> select * from PLAYERS
    -> where playerno in (select playerno from PENALTIES);
```

查询视图：

```
mysql> select playerno from cost_raisers
    -> where town='Stratford';
+----------+
| PLAYERNO |
+----------+
|        6 |
+----------+ 
```

1、替代方法：

　　先把select语句中的视图名使用定义视图的select语句来替代；

　　再处理所得到的select语句。

```
mysql> select playerno from
　　 -> (
    ->   select * from PLAYERS
    ->   where playerno in->   　　(select playerno from PENALTIES)
    -> )as viewformula
    -> where town='Stratford';
+----------+
| PLAYERNO |
+----------+
|        6 |
+----------+
```

2、具体化方法：

　　先处理定义视图的select语句，这会生成一个中间的结果集；

　　然后，再在中间结果上执行select查询。

mysql> select <列名> from <中间结果>;　　　　　　

　　[【回到顶部】](https://www.cnblogs.com/geaozhang/p/6792369.html#mulu)

@author：http://www.cnblogs.com/geaozhang/