# MySQL常见建表选项及约束



> 阅读目录---MySQL常见的建表选项及约束：
>
> 1、[create table选项](https://www.cnblogs.com/geaozhang/p/6786105.html#xuanxiang)
>
> 　　1、[指定列选项](https://www.cnblogs.com/geaozhang/p/6786105.html#liexuanxiang)：[default](https://www.cnblogs.com/geaozhang/p/6786105.html#default)、[comment](https://www.cnblogs.com/geaozhang/p/6786105.html#liecomment)
>
> 　　2、指定表选项：engine、auto_increment、comment
>
> 2、[create table约束](https://www.cnblogs.com/geaozhang/p/6786105.html#yueshu)　
>
> 　　1、[not null](https://www.cnblogs.com/geaozhang/p/6786105.html#not_null)：非空约束
>
> 　　2、[unique](https://www.cnblogs.com/geaozhang/p/6786105.html#unique)：唯一约束
>
> 　　3、[primary key](https://www.cnblogs.com/geaozhang/p/6786105.html#primary)：主键约束
>
> 　　4、[foreign key](https://www.cnblogs.com/geaozhang/p/6786105.html#foreign)：外键
>
> 　　5、[check](https://www.cnblogs.com/geaozhang/p/6786105.html#check)：检查---enum、set

　　 

一、CREATE TABLE 选项

1、在定义列的时候，指定列选项

1）DEFAULT <literal>：定义列的默认值

　　当插入一个新行到表中并且没有给该列明确赋值时，如果定义了列的默认值，将自动得到默认值 ；如果没有，则为null。

```
mysql> create table people                                                                 
    -> (
    -> 　　id int not null primary key,
    -> 　　name varchar(20) not null,
    -> 　　sex char(1) default 'm'
    -> );

mysql> insert into people(id,name) values(1,'张三');
mysql> insert into people values(2,'李四','f');

mysql> select * from people;
+----+--------+------+
| id | name   | sex  |
+----+--------+------+
|  1 | 张三    | m    |
|  2 | 李四    | f    |
+----+--------+------+
```

当然，也可以在INSERT和UPDATE语句中使用DEFAULT关键字显式地给列赋默认值：

```
mysql> insert into people values(3,'王五',default);

mysql> update people set sex=default where id=2;

mysql> select * from people;
+----+--------+------+
| id | name   | sex  |
+----+--------+------+
|  1 | 张三    | m    |
|  2 | 李四    | m    |
|  3 | 王五    | m    |
+----+--------+------+
```

 函数default(column)可以得到一个列的默认值：

```
mysql> select default(sex) from people;
+--------------+
| default(sex) |
+--------------+
| m            |
| m            |
| m            |
+--------------+
```

 

2）comment：用来给列添加注释，最多255个字符，注释会保存到数据字典中。

　　创建带有列注释的表stu_comment

```
mysql> create table stu_comment
    -> (
    -> id int not null primary key
    ->   comment '学号',
    -> name varchar(20) not null
    ->   comment '姓名'
    -> );
```

 　从数据字典查询注释信息

```
mysql> select column_name,column_comment
    -> from information_schema.columns
    -> where table_name='stu_comment';
+-------------+----------------+
| column_name | column_comment |
+-------------+----------------+
| id          | 学号            |
| name        | 姓名            |
+-------------+----------------+
```

 

2、在CREATE TABLE语句中的表选项

1）engine：指定表使用的存储引擎

存储引擎：决定了数据如何存储以及如何访问，还有事务如何处理

MySQL允许对每个表使用不同的存储引擎，如果在create table语句中没有指定存储引擎，则使用默认的存储引擎。

　　mysql> show engines;    #查询所有支持的存储引擎

　　mysql> CREATE TABLE sexes(sex char(1) NOT NULL) ENGINE = INNODB; 

注意：存储引擎是个重点，后面我们详细讲解。

2）auto_increment：决定当向表中插入第一行时，自增列得到的第一个值是多少

3）comment：给表添加注释

```
mysql> create table mycomm(num int) comment '测试表';

mysql> select table_name,table_comment
    -> from information_schema.tables
    -> where table_name='mycomm';
+------------+---------------+
| table_name | table_comment |
+------------+---------------+
| mycomm     | 测试表         |
+------------+---------------+
```

 

 

二、CREATE TABLE 约束

作用：可以为列定义约束（constraint）

　　约束主要是防止非法数据进入到表中，确保数据的正确性和一致性（统称数据完整性）；

　　约束也可以防止一个表被删除 。

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170429185750990-1377135432.png)

注意：

　　1）MySQL中约束保存在information_schema.table_constraints中，可以通过该表查询约束信息；

　　2）进行约束定义的时间：使用create table语句、使用alter table语句。

常用的约束的类型：5种

　　①not null：非空约束，指定某列不为空

　　②unique：唯一约束，指定某列和几列组合的数据不能重复

　　③primary key：主键约束，指定某列的数据不能重复、唯一

　　④foreign key：外键，指定该列记录属于主表中的一条记录，参照另一条数据

　　⑤check：检查，指定一个表达式，用于检验指定数据

 

约束定义的语法：

```
列级别：
CREATE TABLE table_name(column_name data_type
　　[ [NOT NULL] | [UNIQUE [KEY] | PRIMARY KEY]
　　|CHECK(expr)],…)

表级别：
CREATE TABLE table_name(
　　column_name  data_type [NOT NULL],
   　　column_name data_type [not null],…,
　　[CONSTRAINT constraint_name] PRIMARY KEY (col_name,...)
　　|[CONSTRAINT constraint_name] unique (col_name,...)
　　|[CONSTRAINT constraint_name] foreign KEY (col_name) REFERENCES tbl_name (index_col_name)
　　|check(expr)  
```

注意：

　　1）NOT NULL约束只能在列级别定义，作用在多个列上的约束只能定义在表级别，例如复合主键约束；

　　2）列级别上不能定义外键约束，并且不能给约束起名字，由MySQL自动命名（NOT NULL除外）；

　　3）表级别上定义的约束可以给约束起名字（CHECK约束除外）

 

1、、not null非空约束

作用：用于确保当前列的值不为空。

```
mysql> create table temp_nn(id int not null);

约束直接对DML操作带来影响
mysql> insert into temp_nn values(1);
Query OK, 1 row affected (0.00 sec)

mysql> insert into temp_nn values(null);
ERROR 1048 (23000): Column 'id' cannot be null

具有非空约束的列不允许有null值
```

注意：非空约束只能出现在表对象的列上。

 

2、unique唯一约束

　　1.唯一约束是指定table的列或列组合不能重复，保证数据的唯一性，约束的列不允许有重复值；

　　2.唯一约束不允许出现重复的值，但是可以为多个null；

　　3.同一个表可以有多个唯一约束，多个列组合的约束

```
mysql> create table temp_uk(
    ->   id int not null unique,
    ->   name varchar(20) unique);

mysql> insert into temp_uk values(1,'a');
mysql> insert into temp_uk values(2,'a');
ERROR 1062 (23000): Duplicate entry 'a' for key 'name'
mysql> insert into temp_uk values(2,null);
mysql> insert into temp_uk values(3,null);

mysql> select * from temp_uk;
+----+------+
| id | name |
+----+------+
|  2 | NULL |
|  3 | NULL |
|  1 | a    |
+----+------+
```

可见，唯一性约束的列可以有多个null值，因为null <> null

　　4.在创建唯一约束时，如果不给唯一约束名称，就默认和列名相同；

　　5.唯一约束不仅可以在一个表内创建，而且可以同时多表创建组合唯一约束。

```
mysql> create table test(
    ->   id int not null,
    ->   name varchar(20),
    ->   password varchar(16),
---使用表级约束语法   
    ->   constraint uk_name_pwd unique(name,password)
    -> );    #表示用户名和密码组合不能重复
Query OK, 0 rows affected (0.08 sec)

查询数据字典，查看唯一键约束的信息
mysql> select * from information_schema.table_constraints
    -> where table_name='test';
```

 

3、primary key主键约束

　　primary key = not null + unique

主键：用来唯一的标示表中的每一行(类型一般为整型或者字符串)

　　具有主键约束的列不允许有null值，并且不允许有重复值;

　　每个表最多只允许一个主键(可定义联合主键)，主键名总是PRIMARY。

```
mysql> create table temp_pk(
    -> id int primary key);

mysql> insert into temp_pk values(1),(2);

mysql> insert into temp_pk values(1);
ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY'

mysql> update temp_pk set id=1 where id=2;
ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY'

mysql> insert into temp_pk values(null);
ERROR 1048 (23000): Column 'id' cannot be null
```

!!给主键一个新的名字，但在数据字典中，主键名还是显示primary

 

联合主键（用几个列进行唯一标识一行）

```
mysql> create table temp_pk(
    -> id int,
    -> name varchar(20),
    -> constraint pk_id_name primary key(id,name)
    -> );
Query OK, 0 rows affected (0.06 sec)

mysql> desc temp_pk;
+-------+-------------+------+-----+---------+-------+
| Field | Type        | Null | Key | Default | Extra |
+-------+-------------+------+-----+---------+-------+
| id    | int(11)     | NO   | PRI | NULL    |       |
| name  | varchar(20) | NO   | PRI | NULL    |       |
+-------+-------------+------+-----+---------+-------+
2 rows in set (0.00 sec)

mysql> insert into temp_pk values(1,'张三');
Query OK, 1 row affected (0.00 sec)
mysql> insert into temp_pk values(2,'李四');
Query OK, 1 row affected (0.01 sec)

mysql> insert into temp_pk values(1,'王五');
Query OK, 1 row affected (0.00 sec)

mysql> insert into temp_pk values(1,'张三');
ERROR 1062 (23000): Duplicate entry '1-张三' for key 'PRIMARY'

mysql> select * from temp_pk;
+----+--------+
| id | name   |
+----+--------+
|  1 | 张三   |
|  1 | 王五   |
|  2 | 李四   |
+----+--------+
3 rows in set (0.00 sec)
```

 

4、foreign key外键约束

外键约束：

　　参照完整性约束，保证一个或两个表之间的参照完整性，外键是构建于一个表的两个字段或是两个表的两个字段之间的参照关系。

注意：

　　1）具有外键约束的列的值不能随便给，必须满足外键所引用的主键的取值；

　　2）一张表中可以定义多个外键；

　　3）外键列默认可以给null值。

按照定义，外键必须引用一个主键或者唯一键，引用的主键一般在另外一张表中，也可以是本表的主键(后者称为“自引用”)。

父子表：

　　外键所在的表叫做子表、从表

　　外键所引用的主键所在的表叫做父表、主表

注意：父子表是相对而言的，表a可以是表b的子表，但同时也可以是表c的父表

示例：创建外键约束

```
//创建父表
mysql> create table dept(
    ->   deptid int,
    ->   dname varchar(20),
    ->   constraint dept_deptid_pk primary key(deptid)
    -> );

mysql> insert into dept(deptid,dname) values(10,'市场部');
mysql> insert into dept(deptid,dname) values(20,'销售部');

//创建子表（表级别创建外键约束）
mysql> create table emp(
    ->   id int,
    ->   name varchar(20),
    ->   deptid int,
    ->   constraint emp_id_pk primary key(id),
    ->   constraint emp_deptid_fk  foreign key(deptid)
    ->     references dept(deptid)
    -> ); 
```

查询数据字典，查看外键约束的信息：

　　上面创建子表的时候给外键约束命名emp_deptid_fk；

　　如果不给外键约束命名，那么默认的名字是表名_ibfk_n, n是整数,从1开始；

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170429192415787-272355118.png)

此时，emp表中deptid列(外键约束)受dept主表限制

```
mysql> insert into emp(id,name,deptid) values(1,'张三',10);
Query OK, 1 row affected (0.00 sec)
mysql> insert into emp(id,name,deptid) values(2,'李四',10);
Query OK, 1 row affected (0.00 sec)
mysql> insert into emp(id,name,deptid) values(3,'王五',50);　　#insert主表deptid列没有的数据
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`test`.`emp`, CONSTRAINT `emp_deptid_fk` FOREIGN KEY (`deptid`) REFERENCES `dept` (`deptid`))

mysql> update emp set deptid=30 where id=1;　　#update主表deptid列没有的数据
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`test`.`emp`, CONSTRAINT `emp_deptid_fk` FOREIGN KEY (`deptid`) REFERENCES `dept` (`deptid`))
外键的update更新操作规则如下删除规则……

mysql> delete from dept where deptid=10;　　#delete父表中的行(子表中有引用的数据行)
ERROR 1451 (23000): Cannot delete or update a parent row: a foreign key constraint fails (`test`.`emp`, CONSTRAINT `emp_deptid_fk` FOREIGN KEY (`deptid`) REFERENCES `dept` (`deptid`))

外键的默认删除规则：
　　当删除父表中的行时，如果子表中有依赖于被删除父行的子行存在，那么就不允许删除，并抛出异常（默认对外键使用on delete restrict 或on delete no action选项）
```

 

外键引用定义： 

```
reference_definition:
    REFERENCES tbl_name (index_col_name,...)
      [MATCH FULL | MATCH PARTIAL | MATCH SIMPLE]
      [ON DELETE reference_option]
      [ON UPDATE reference_option]
reference_option:
    RESTRICT | CASCADE | SET NULL | NO ACTION
```

 在定义外键约束时，通过使用on delete cascade或者on delete set null选项，可以改变外键的默认删除规则：

　　①ON DELETE CASCADE：级联删除。当删除父表中的行时，如果子表中有依赖于被删除父行的子行存在，那么连同子行一起删除（很危险！！！）

　　②ON DELETE SET NULL：当删除父表中的行时，如果子表中有依赖于被删除父行的子行存在，那么不删除，而是将子行的外键列设置为null 

　　……

```
mysql> create table emp(
    ->   id int,
    ->   name varchar(20),
    ->   deptid int,
    ->   constraint emp_id_pk primary key(id),
    ->   constraint emp_deptid_fk  foreign key(deptid)
　　 ->     references dept(deptid)
    ->   on delete cascade
    -> );
……
```

 

5、check约束

　　MySQL可以使用check约束，但check约束对数据验证没有任何作用。

　　Oracle中可以使用check约束，是有相应的作用的。

```
mysql> create table test_ck(
    ->   id int check(id>0)
    -> );

mysql> insert into test_ck values(-100);

mysql> select * from test_ck;
+------+
| id   |
+------+
| -100 |
+------+
```

The CHECK clause is parsed but ignored by all storage engines。

定义数据库列时，可以使用ENUM（enumeration，枚举）和SET（集合）类型：变通的实现CHECK约束

两者的区别是：

　　使用ENUM，只能选一个值；

　　使用SET，可以选多个值；

ENUM和SET中的值都必须是字符串类型。

1、enum枚举类型

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170429192625006-1134809465.png)

注意：

　　在内部存储ENUM值时，MYSQL给ENUM中的每个值一个顺序号码：第一个值的顺序号码是1，第二个值的顺序号码是2，以此类推。当排序或比较ENUM的时候，使用这些顺序号码进行。

```
mysql> select * from student order by sex;
+----+--------+------+
| id | name   | sex  |
+----+--------+------+
|  5 | 王五    | NULL |
|  1 | 张三    | M    |
|  2 | 李四    | F    |
+----+--------+------+
```

2、set类型：因为可以列举多个值，所以在建表列约束中常被使用到。

```
mysql> create table team(
    ->   teamno int not null,
    ->   division set('north','south','east','west')
    -> );

mysql> insert into team values(1,'west');
mysql> insert into team values(2,'west,south');
mysql> insert into team values(4,null);
mysql> insert into team values(3,'east,asia');
ERROR 1265 (01000): Data truncated for column 'division' at row 1

mysql> select * from team;
+--------+------------+
| teamno | division   |
+--------+------------+
|      1 | west       |
|      2 | south,west |
|      4 | NULL       |
+--------+------------+
```

@author：http://www.cnblogs.com/geaozhang/