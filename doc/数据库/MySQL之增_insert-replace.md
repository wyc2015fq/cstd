# MySQL之增_insert-replace



MySQL增删改查之增insert、replace

一、INSERT语句

　　带有values子句的insert语句，用于数据的增加

语法：

```
INSERT   [INTO]  tbl_name[(col_name,...)]
　　{VALUES | VALUE} (expr ,...),(...),...
```

　　①用来把一个新行插入到表中

　　②为和其它数据库保持一致，不要省略INTO关键字以及使用VALUES而不是value关键字

　　③插入一行时，要求必须对该行所有的列赋值。但是赋值方式可以是显式赋值（直接给出值）和隐式赋值（由MySQL自动赋值）

 

基本格式：

　　insert into <表名> (列名1,列名2,列名3,......)  values (值1,值2,值3,......);

解析：

　　①(列名1,列名2,列名3,......)称为 列清单

　　②(值1,值2,值3,......)称为 值清单

　　③列清单和值清单中的列和值必须相互对应(数据类型)

 

1、在表名后面列出所有的列名

示例：插入一个新的球队到teams表中

mysql> INSERT INTO teams(teamno,playerno,division) VALUES(3,6,'third');

 

2、在表名后面省略所有的列名

要求：VALUES子句中的值必须按照列在表结构中的顺序来一一赋值

mysql> INSERT INTO teams VALUES(4,104,'third');

 

3、在表名后面只列出部分的列名

注意：所有没有明确赋值的列，将通过隐式赋值自动得到null值

mysql> insert into <表名> (列名1,列名3......)  values (值1,值3,......);

添加一个新行数据，没有指定赋值的列，结果集改行数据的列名2处隐式赋null值

 

当然，可使用字面量NULL给列赋空值

mysql> insert into <表名> (列名1,列名2,列名3)  values (值1,null,值3);

 

4、VALUES子句中除了字面量，还可以使用函数、计算、标量子查询等

INSERT INTO <表名1>(列1,列2)

　　VALUES((SELECT count(*) FROM 表名2),

　　　　(SELECT sum(列名) FROM 表名3));

注意：子查询必须放在单独的小括号中

 

一条INSERT语句可以插入多个行：

示例：添加4个新的球队

INSERT INTO teams(teamno, captainno,division)

  VALUES (6,7,'third'),

​            (7,27,'fourth'),

​            (8,39,'fourth'),

​            (9,112,'sixth');  

注意：这种语法只要有1行出错，则插入全部取消 

INSERT语句中可以使用IGNORE选项来当INSERT语句出错时，不显示错误消息：但是INSERT语句不会执行

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170426183020334-864829403.png)

 

5、从其他表中复制数据：带子查询的INSERT语句，实现从其他表中(可按条件)复制数据进入目标表

语法：

```
INSERT   [INTO]  tbl_name[(col_name,...)]
　　SELECT ...
```

注意：

　　①如果在表名后面列出了列名，那么列的数量和数据类型必须和子查询的select列表相匹配

　　②如果在表名后面没有列出了列名，默认就是直接复制

insert....  select.... 语句常在需要进行数据表的备份时使用

 

无条件复制：将一个表中的原始数据记录，插入到另一个表中

按条件copy：将一个表查询出来的进行聚合操作之后的记录，插入到目标表中

 ![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170426183508990-1828404930.png)

当然，可以把本表中的行再次添加到本表中，但是必须注意主键值不要重复。

 

 

二、REPLACE语句：替代已有的行

　　INSERT语句的一个变种；

当添加新行时：

　　①如果主键值重复，那么就覆盖表中已有的行

　　②如果没有主键值重复，则插入该行

语法：

```
REPLACE [INTO] tbl_name [(col_name,...)]
　　VALUES (expr,...),(...),...

或者

REPLACE [INTO] tbl_name [(col_name,...)]
　　SELECT ...
```

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170426183921131-572026148.png)

@author：http://www.cnblogs.com/geaozhang/