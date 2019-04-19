# Mysql最基本、最常用命令集锦 - 三少GG - CSDN博客
2013年07月28日 13:35:19[三少GG](https://me.csdn.net/scut1135)阅读数：1165

--------------------------------------------------------------------------------------------------------
mysql默认安装之后root是没有密码的 会提示你设置root密码                             |
试试：
**mysql -uroot -p                                                                                                  |**
**会提示输入密码，必备！**否则创建新数据库会遇到权限问题等。
创建用户：
**grant all on ***.* **to NewUserName@HostName identified by “password” ;**
grant命令：
grant 权限1,权限2,…权限n on 数据库名称.表名称 to 用户名@用户地址 identified by '连接口令';
例子：
mysql>grant select,insert,update,delete,create,drop on mydb.mytable to lzpddd@192.168.1.88 identified by 'mypassword';
给来自192.168.1.88的用户lzpddd分配可对数据库 mydb的mytable表进行select,insert,update,delete,create,drop等操作的权限，并设定口令为'mypassword';
mysql>grant all privileges on *.* to lzpddd@192.168.1.88 identified by 'mypassword';
给来自192.168.1.88的用户lzpddd分配可对所有数据库的所有表进行所有操作的权限限，并设定口令为'mypassword';
**mysql>grant all privileges on *.* to [lzpddd@'%'](mailto:lzpddd@' rel=) identified by 'mypassword';**
给来自任何IP地址的用户lzpddd分配可对所有数据库的所有表进行所有操作的权限限，并设定口令为'mypassword';
--------------------------------------------------------------------------------------------------------
1:使用SHOW语句找出在服务器上当前存在什么数据库：
mysql> SHOW DATABASES;
2:2、创建一个数据库MYSQLDATA
**mysql> CREATE DATABASE MYSQLDATA;**
3:选择你所创建的数据库
mysql> USE MYSQLDATA; (按回车键出现Database changed 时说明操作成功！)
4:查看现在的数据库中存在什么表
mysql> SHOW TABLES;
5:创建一个数据库表
mysql> CREATE TABLE MYTABLE (name VARCHAR(20), sex CHAR(1));
6:显示表的结构：
mysql> DESCRIBE MYTABLE;
7:往表中加入记录
mysql> insert into MYTABLE values (”hyq”,”M”);
8:用文本方式将数据装入数据库表中（例如D:/mysql.txt）
mysql> **LOAD DATA LOCAL INFILE “D:/mysql.txt” INTO TABLE MYTABLE;9:导入.sql文件命令（例如D:/mysql.sql）mysql>use database;mysql>source d:/mysql.sql;**
10:删除表
mysql>drop TABLE MYTABLE;
11:清空表
mysql>delete from MYTABLE;
12:更新表中数据
mysql>update MYTABLE set sex=”f” where name=’hyq’;
以下是无意中在网络看到的使用MySql的管理心得,
在windows中MySql以服务形式存在，在使用前应确保此服务已经启动，未启动可用net start mysql命令启动。而Linux中启动时可用“/etc/rc.d/init.d/mysqld start”命令，注意启动者应具有管理员权限。
刚安装好的MySql包含一个含空密码的root帐户和一个匿名帐户，这是很大的安全隐患，对于一些重要的应用我们应将安全性尽可能提高，在这里应把匿名帐户删除、 root帐户设置密码，可用如下命令进行：
use mysql;
delete from User where User=”";
update User set Password=PASSWORD(’newpassword’) where User=’root’;
如果要对用户所用的登录终端进行限制，可以更新User表中相应用户的Host字段，在进行了以上更改后应重新启动数据库服务，此时登录时可用如下类似命令：
mysql -uroot -p;
mysql -uroot -pnewpassword;
mysql mydb -uroot -p;
mysql mydb -uroot -pnewpassword;
上面命令参数是常用参数的一部分，详细情况可参考文档。此处的mydb是要登录的数据库的名称。
在 进行开发和实际应用中，用户不应该只用root用户进行连接数据库，虽然使用root用户进行测试时很方便，但会给系统带来重大安全隐患，也不利于管理技 术的提高。我们给一个应用中使用的用户赋予最恰当的数据库权限。如一个只进行数据插入的用户不应赋予其删除数据的权限。MySql的用户管理是通过 User表来实现的，添加新用户常用的方法有两个，一是在User表插入相应的数据行，同时设置相应的权限；二是通过GRANT命令创建具有某种权限的用 户。其中GRANT的常用用法如下：
**grant all on mydb.* to NewUserName@HostName identified by “password” ;**
grant usage on** *.***to NewUserName@HostName identified by “password”;
grant select,insert,update on mydb.* to NewUserName@HostName identified by “password”;
grant update,delete on mydb.TestTable to NewUserName@HostName identified by “password”;
若 要给此用户赋予他在相应对象上的权限的管理能力，可在GRANT后面添加WITH GRANT OPTION选项。而对于用插入User表添加的用户，Password字段应用PASSWORD 函数进行更新加密，以防不轨之人窃看密码。对于那些已经不用的用户应给予清除，权限过界的用户应及时回收权限，回收权限可以通过更新User表相应字段， 也可以使用REVOKE操作。
一、SQL速成
结构查询语言(SQL)是用于查询关系数据库的标准语言，它包括若干关键字和一致的语法，便于数据库元件(如表、索引、字段等)的建立和操纵。
以下是一些重要的SQL快速参考，有关SQL的语法和在标准SQL上增加的特性，请查询MySQL手册。
  1．创建表
表是数据库的最基本元素之一，表与表之间可以相互独立，也可以相互关联。创建表的基本语法如下：
  create table table_name  
  (column_name data无效 {identity |null|not null}，
  …)  
其中参数table_name和column_name必须满足用户数据库中的识别器(identifier)的要求，参数data无效是一个标准的SQL类型或由用户数据库提供的类型。用户要使用non-null从句为各字段输入数据。
  create table还有一些其他选项，如创建临时表和使用select子句从其他的表中读取某些字段组成新表等。还有，在创建表是可用PRIMARY KEY、KEY、INDEX等标识符设定某些字段为主键或索引等。
书写上要注意：
在一对圆括号里的列出完整的字段清单。
字段名间用逗号隔开。
字段名间的逗号后要加一个空格。
最后一个字段名后不用逗号。
所有的SQL陈述都以分号";"结束。
例：
  mysql> CREATE TABLE test (blob_col BLOB， index(blob_col(10))); 
2．创建索引
索引用于对数据库的查询。一般数据库建有多种索引方案，每种方案都精于某一特定的查询类。索引可以加速对数据库的查询过程。创建索引的基本语法如下：
  create index index_name  
  on table_name (col_name[(length)]，... ) 
例：
  mysql> CREATE INDEX part_of_name ON customer (name(10));  
3．改变表结构
在数据库的使用过程中，有时需要改变它的表结构，包括改变字段名，甚至改变不同数据库字段间的关系。可以实现上述改变的命令是alter，其基本语法如下：
  alter table table_name alter_spec [， alter_spec ...] 
例：
  mysql> ALTER TABLE t1 CHANGE a b INTEGER;  
4．删除数据对象
很多数据库是动态使用的，有时可能需要删除某个表或索引。大多数数据库对象可以下面的命令删除：
  drop object_name  
  mysql> DROP TABLE tb1;  
5．执行查询
查询是使用最多的SQL命令。查询数据库需要凭借结构、索引和字段类型等因素。大多数数据库含有一个优化器(optimizer)，把用户的查询语句转换成可选的形式，以提高查询效率。
值得注意的是MySQL不支持SQL92标准的嵌套的where子句，即它只支持一个where子句。其基本语法如下：
  SELECT [STRAIGHT_JOIN] [SQL_SMALL_RESULT] [SQL_BIG_RESULT]  [HIGH_PRIORITY]  
  [DISTINCT | DISTINCTROW | ALL]  
  select_expression，... 
  [INTO {OUTFILE | DUMPFILE} ’file_name’ export_options]  
  [FROM table_references  
  ][WHERE where_definition]  
  [GROUP BY col_name，...] 
  [HAVING where_definition]  
  [ORDER BY {unsigned_integer | col_name | formula} ][ASC | DESC] ，...]  
  [LIMIT ][offset，] rows] 
  [PROCEDURE procedure_name] ]  
其中where从句是定义选择标准的地方，where_definition可以有不同的格式，但都遵循下面的形式：
字段名操作表达式
字段名操作字段名
在第一种形式下，标准把字段的值与表达式进行比较；在第二种形式下，把两个字段的值进行比较。根据所比较的数据类型，search_condition中的操作可能选以下几种：
  = 检查是否相等
！=  检查是否不等
> (或>=)检查左边值是否大于(或大于等于)右边值
  < (或<=) 检查左边值是否小于(或小于等于)右边值
  [not] between 检查左边值是否在某个范围内
  [not] in 检查左边是否某个特定集的成员
  [not] like 检查左边是否为右边的子串
  is [not] null 检查左边是否为空值
在这里，可以用通配符_代表任何一个字符，％代表任何字符串。使用关键字<AND>、<OR>和<NOT>可以生成复杂的词，它们运行检查时使用布尔表达式的多重标准集。
例：
  mysql> select t1.name， t2.salary from employee AS t1，  info AS t2 where t1.name
 = t2.name;  
  mysql> select college， region， seed from tournament 
  ORDER BY region， seed; 
  mysql> select col_name from tbl_name WHERE col_name > 0;  
6．修改表中数据
在使用数据库过程中，往往要修改其表中的数据，比如往表中添加新数据，删除表中原有数据，或对表中原有数据进行更改。它们的基本语法如下：
数据添加：
  insert [into] table_name [(column(s))]  
  values (expression(s))  
例：
  mysql> INSERT INTO tbl_name (col1，col2) VALUES(15，col1*2);  
数据删除：
删除  from table_name where search_condition 
数据更改：
更新  table_name 
  set column1=expression1，
  column2=expression2，… 
  where search_condition  
7．数据库切换
当存在多个数据库时，可以用下面的命令定义用户想使用的数据库：
  use database_name  
8．统计函数
  SQL有一些统计函数，它们对于生成数据表格很有帮助。下面介绍几个常用的统计函数：
  sum (exepression) 计算表达式的和
  avg (exepression) 计算表达式的平均值
  count (exepression) 对表达式进行简单的计数
  count (*) 统计记录数
  max (exepression) 求最大值
  min (exepression) 求最小值
其中exepression为任何有效的SQL表达式，它可以是一个或多个记录，也可以是别的SQL函数的组合。
二、MySQL使用导引
  1．运用MySQL建立新数据库
在shell下运行：
＄>mysqladmin  create database01 
  Database "database01" created.  
2．启动MySQL 
在shell下运行：
＄>mysql  
  Welcome to the MySQL monitor. Commands end with ; or g.  
  Your MySQL connection id is 22 to server version: 3.21.   29a-gamma-debug   
无效  ’help’ for help. 
3．更换数据库
  mysql>use database01  
  database changed.  
4．创建表
  mysql>create table table01 (field01 integer， field02 char(10)); 
  Query OK， 0 rows affected (0.00 sec) 
5．列出表清单
  mysql>show tables;  
  Tables in database01  
  Table01  
  table02  
6．列出表中的字段清单
  mysql>show columns from table01;  
  Field 无效 Null Key Default Extra 
  field01 int(11) YES  
  field02 char(10) YES  
7．表的数据填写
插入数据
  mysql>insert into table01 (field01， field02) values (1，  ’first’); 
  Query OK， 1 row affected (0.00 sec) 
8．字段的增加
  ...一次一个字段
  mysql>alter table table01 add column field03 char(20);  
  Query OK， l row affected (0.04 sec) 
  Records: 1 Duplicates: 0 Warnings: 0  
  ...一次多个字段
  mysql>alter table table01 add column field04 date，  add column field05 time; 
  Query OK， l row affected (0.04 sec) 
  Records: 1 Duplicates: 0 Warnings: 0  
注意：每一列都必须以"add  column"重新开始。
它运行了吗？让我们看看。
  mysql>select * from table01;  
  field01 field02 field03 field04 field05  
  1 first NULL NULL NULL  
9．多行命令输入
  MySQL命令行界面允许把陈述作为一行输入，也可以把它展开为多行输入。这两者之间并没有语法上的区别。使用多行输入，你可以将SQL陈述一步步分解，从而使你更容易理解。
在多行方式下，注释器把每一行都添加到前面的行后，直到你用分号";"来结束这个SQL陈述。一旦键入分号并按回车键，这个陈述即被执行。
下面的例子是同一个严格的SQL陈述的两种输入方法：
单行输入
  Mysql>create table table33 (field01 integer， field02 char(30)); 
多行输入
  Mysql>create table table33  
  ->(field01  
  ->integer，
  ->field02  
  ->char(30));  
注意不能将单词断开，如：
正确
  mysql>create table table33  
  ->( field01  
  ->integer，
  ->field02  
  ->char(30));  
错误
  mysql>create table table33  
  ->( field01 inte  
  ->ger，
  ->field02  
  ->char(30));  
当插入或更改数据时，不能将字段的字符串展开到多行里，否则硬回车将被储存到数据中：
标准操作
  mysql>insert into table33 (field02)  
  ->values  
  ->(’who thought of foo?’);  
硬回车储存到数据中
  mysql>insert into table33 (field02)  
  ->values  
  ->(’who thought  
  ->of foo?’);  
结果如下：
  mysql>select * from table33;  
  field01 field02  
  NULL who thought of foo?  
  NULL who thought  
  Of foo?  
10．表的数据嵌入
  mysql>insert into table01 (field01， field02， field03，
 field04， field05) values 
  ->(2， ’second’， ’another’， ’1999-10-23’， 
 ’10:30:00’);  
  Query OK， 1 row affected (0.00 sec) 
标准日期格式是"yyyy-mm-dd"。
标准时间格式是"hh:mm:ss"。
引号内要求所给的是上述的标准日期和时间格式。
日期也可以"yyyymmdd"形式，时间也可以"hhmmss"形式输入，但其值不需要再加引号。
数字值不需要加引号。这种保存与数据类型无关，这些数据类型都有格式化的专栏来包含(例如：文本，日期，时间，整数等)。
  MySQL有一个很有用的命令缓冲区。它保存着你目前已经键入的SQL语句利用它，对于相同的命令，你就不必一遍又一遍地重复输入。下一步我们就来看这样的一个例子。
利用命令缓冲区(及任意的日期和时间格式)增加另一个数据
按两次键盘上的向上箭头键。
回车。
在圆括号内输入新的值，并以分号结尾。
  (3，  ’a third’， ’more’， 19991024，
 103004);  
回车。
新值存在里面了吗？
  mysql>select * from table01;  
  field01 field02 field03 field04 field05  
  1 first NULL NULL NULL  
  2 second another 1999-10-23 10:30:00  
3 a third more  1999-10-24 10:30:04  
11．表的数据更新
一次修改一个字段
再次注意语法。文本需要加引号但数字不要。
  mysql>更新 table01 set field03=’new info’ where field01=1; 
  Query OK， 1 row affected (0.00 sec) 
一次改变多个字段
记住在每一个更新的字段间用逗号隔开。
  mysql>更新 table01 set field04=19991022，  field05=062218 where field01=1; 
  Query OK， 1 row affected (0.00 sec) 
一次更新多个数据
  mysql>更新 table01 set field05=152901 where  field04>19990101; 
  Query OK， 3 rows affected (0.00 sec) 
12．删除数据
  mysql>删除 from table01 where field01=3; 
  Query OK， 1 row affected (0.00 sec) 
13．退出
  mysql>quit  
  Bye  
现在你已经了解了一些运行MySQL中的数据库的根本命令。由于MySQL是通过执行SQL调用来操作的，在你的处理过程中需要一个强有力工具的充足的数组。例如，通过联接相关的字段，你可以同时显示几个表中的数据。同样，SQL允许综合显示、更新或者删除多个符合具体标准的数据。如果你还想精通掌握它，下一步就要学习所有SQL的知识。
**数据类型**其中的长度以字节为单位
|名称|长度|用法|
|----|----|----|
|TINYINT(M)BIT,BOOL,BOOLEAN|1|如果为无符号数，可以存储从0到255的数;否则可以存储从-128到127的数。|
|SMALLINT(M)|2|如果为无符号数，可以存储从0到65535的数;否则可以存储从-32768到32767的数。|
|MEDIUMINT(M)|3|如果为无符号数，可以存储从0到16777215的数;否则可以存储从-8388608到8388607的数|
|INT(M)INTEGER(M)|4|如果为无符号数，可以存储从0到4294967295的数，否则可以存储从-2147483648到2147483647的数。|
|BIGINT(M)|8|如果为无符号数，可以存储从0到18446744073709551615的数，否则可以存储从-9223372036854775808到9223372036854775807的数。|
|FLOAT(precision)|4或8|这里的precision是可以直达53的整数。如果precision<=24则转换为FLOAT，如果precision>24并且precision<=53则转换为DOUBLE。|
|FLOAT(M,D)|4|单精度浮点数。|
|DOUBLE(M,D),DOUBLE PRECISION,REAL|8|双精度浮点。|
|DECIMAL(M,D),DEC,NUMERIC,FIXED|M+1或M+2|未打包的浮点数。|
|DATE|3|以YYYY-MM-DD的格式显示。|
|DATETIMEHH:MM:SS|8|以YYYY-MM-DD的格式显示。|
|TIMESTAMP|4|以YYYY-MM-DD的格式显示。|
|TIME|3|以HH:MM:SS的格式显示。|
|YEAR|1|以YYYY的格式显示。|
|CHAR(M)|M|定长字符串。|
|VARCHAR(M)|最大M|变长字符串。M<=255.|
|TINYBLOB,TINYTEXT|最大255|TINYBLOB为大小写敏感，而TINYTEXT不是大小写敏感的。|
|BLOB,TEXT|最大64K|BLOB为大小敏感的，而TEXT不是大小写敏感的。|
|MEDIUMBLOB,MEDIUMTEXT|最大16M|MEDIUMBLOB为大小写敏感的，而MEDIUMTEXT不是大小敏感的。|
|LONGBLOB,LONGTEXT|最大4G|LONGBLOB为大小敏感的，而LONGTEXT不是大小敏感的。|
|ENUM(VALUE1,….)|1或2|最大可达65535个不同的值。|
|SET(VALUE1,….)|可达8|最大可达64个不同的值。|
[Mysql常用命令行大全](http://www.blogjava.net/supperchen/archive/2012/10/11/389340.html)
**第一招、mysql服务的启动和停止**
net stop mysql
net start mysql
**第二招、登陆mysql**
语法如下： mysql -u用户名 -p用户密码
键入命令mysql -uroot -p， 回车后提示你输入密码，输入12345，然后回车即可进入到mysql中了，mysql的提示符是：
mysql>
注意，如果是连接到另外的机器上，则需要加入一个参数-h机器IP
**第三招、增加新用户**
格式：grant 权限 on 数据库.* to 用户名@登录主机 identified by "密码"
如，增加一个用户user1密码为password1，让其可以在本机上登录， 并对所有数据库有查询、插入、修改、删除的权限。首先用以root用户连入mysql，然后键入以下命令：
grant select,insert,update,delete on *.* to user1@localhost Identified by "password1";
如果希望该用户能够在任何机器上登陆mysql，则将localhost改为"%"。
如果你不想user1有密码，可以再打一个命令将密码去掉。
grant select,insert,update,delete on mydb.* to user1@localhost identified by "";
**第四招： 操作数据库**
登录到mysql中，然后在mysql的提示符下运行下列命令，每个命令以分号结束。
1、 显示数据库列表。
show databases;
缺省有两个数据库：mysql和test。 mysql库存放着mysql的系统和用户权限信息，我们改密码和新增用户，实际上就是对这个库进行操作。
2、 显示库中的数据表：
use mysql;
show tables;
3、 显示数据表的结构：
describe 表名;
4、 建库与删库：
create database 库名;
drop database 库名;
5、 建表：
use 库名;
create table 表名(字段列表);
drop table 表名;
6、 清空表中记录：
delete from 表名;
7、 显示表中的记录：
select * from 表名;
**第五招、导出和导入数据**
1. 导出数据：
mysqldump --opt test > mysql.test
即将数据库test数据库导出到mysql.test文件，后者是一个文本文件
如：mysqldump -u root -p123456 --databases dbname > mysql.dbname
就是把数据库dbname导出到文件mysql.dbname中。
2. 导入数据:
mysqlimport -u root -p123456 < mysql.dbname。
不用解释了吧。
3. 将文本数据导入数据库:
文本数据的字段数据之间用tab键隔开。
use test;
load data local infile "文件名" into table 表名;
1:使用SHOW语句找出在服务器上当前存在什么数据库：
mysql> SHOW DATABASES;
2:2、创建一个数据库MYSQLDATA
mysql> CREATE DATABASE MYSQLDATA;
3:选择你所创建的数据库
mysql> USE MYSQLDATA; (按回车键出现Database changed 时说明操作成功!)
4:查看现在的数据库中存在什么表
mysql> SHOW TABLES;
5:创建一个数据库表
mysql> CREATE TABLE MYTABLE (name VARCHAR(20), sex CHAR(1));
6:显示表的结构：
mysql> DESCRIBE MYTABLE;
7:往表中加入记录
mysql> insert into MYTABLE values ("hyq","M");
8:用文本方式将数据装入数据库表中(例如D:/mysql.txt)
mysql> LOAD DATA LOCAL INFILE "D:/mysql.txt" INTO TABLE MYTABLE;
9:导入.sql文件命令(例如D:/mysql.sql)
mysql>use database;
mysql>source d:/mysql.sql;
10:删除表
mysql>drop TABLE MYTABLE;
11:清空表
mysql>delete from MYTABLE;
12:更新表中数据
mysql>update MYTABLE set sex="f" where name='hyq';
posted on 2006-01-10 16:21 happytian 阅读(6) 评论(0) 编辑 收藏 收藏至365Key
13：备份数据库
mysqldump -u root 库名>xxx.data
14：例2：连接到远程主机上的MYSQL
假设远程主机的IP为：110.110.110.110，用户名为root,密码为abcd123。则键入以下命令：
mysql -h110.110.110.110 -uroot -pabcd123
(注:u与root可以不用加空格，其它也一样)
3、退出MYSQL命令： exit (回车)
=====================================================================
1:使用SHOW语句找出在服务器上当前存在什么数据库：
mysql> SHOW DATABASES;
2:2、创建一个数据库MYSQLDATA
mysql> CREATE DATABASE MYSQLDATA;
3:选择你所创建的数据库
mysql> USE MYSQLDATA; (按回车键出现Database changed 时说明操作成功！)
4:查看现在的数据库中存在什么表
mysql> SHOW TABLES;
5:创建一个数据库表
mysql> CREATE TABLE MYTABLE (name VARCHAR(20), sex CHAR(1));
6:显示表的结构：
mysql> DESCRIBE MYTABLE;
7:往表中加入记录
mysql> insert into MYTABLE values ("hyq","M");
8:用文本方式将数据装入数据库表中（例如D:/mysql.txt）
mysql> LOAD DATA LOCAL INFILE "D:/mysql.txt" INTO TABLE MYTABLE;
9:导入.sql文件命令（例如D:/mysql.sql）
mysql>use database;
mysql>source d:/mysql.sql;
10:删除表
mysql>drop TABLE MYTABLE;
11:清空表
mysql>delete from MYTABLE;
12:更新表中数据
mysql>update MYTABLE set sex="f" where name=hyq;
以下是无意中在网络看到的使用MySql的管理心得,
摘自:http://www1.xjtusky.com/article/htmldata/2004_12/3/57/article_1060_1.html
在windows中MySql以服务形式存在，在使用前应确保此服务已经启动，未启动可用net start mysql命令启动。而Linux中启动时可用“/etc/rc.d/init.d/mysqld start"命令，注意启动者应具有管理员权限。
刚安装好的MySql包含一个含空密码的root帐户和一个匿名帐户，这是很大的安全隐患，对于一些重要的应用我们应将安全性尽可能提高，在这里应把匿名帐户删除、 root帐户设置密码，可用如下命令进行：
use mysql;
delete from User where User="";
update User set Password=PASSWORD(newpassword) where User=root;
如果要对用户所用的登录终端进行限制，可以更新User表中相应用户的Host字段，在进行了以上更改后应重新启动数据库服务，此时登录时可用如下类似命令：
mysql -uroot -p;
mysql -uroot -pnewpassword;
mysql mydb -uroot -p;
mysql mydb -uroot -pnewpassword;
上面命令参数是常用参数的一部分，详细情况可参考文档。此处的mydb是要登录的数据库的名称。
在进行开发和实际应用中，用户不应该只用root用户进行连接数据库，虽然使用root用户进行测试时很方便，但会给系统带来重大安全隐患，也不利于管理技术的提高。我们给一个应用中使用的用户赋予最恰当的数据库权限。如一个只进行数据插入的用户不应赋予其删除数据的权限。MySql的用户管理是通过User表来实现的，添加新用户常用的方法有两个，一是在User表插入相应的数据行，同时设置相应的权限；二是通过GRANT命令创建具有某种权限的用户。其中GRANT的常用用法如下：
grant all on mydb.* to NewUserName@HostName identified by "password" ;
grant usage on *.* to NewUserName@HostName identified by "password";
grant select,insert,update on mydb.* to NewUserName@HostName identified by "password";
grant update,delete on mydb.TestTable to NewUserName@HostName identified by "password";
若要给此用户赋予他在相应对象上的权限的管理能力，可在GRANT后面添加WITH GRANT OPTION选项。而对于用插入User表添加的用户，Password字段应用PASSWORD 函数进行更新加密，以防不轨之人窃看密码。对于那些已经不用的用户应给予清除，权限过界的用户应及时回收权限，回收权限可以通过更新User表相应字段，也可以使用REVOKE操作。
下面给出本人从其它资料(www.cn-java.com)获得的对常用权限的解释：
全局管理权限：
FILE: 在MySQL服务器上读写文件。
PROCESS: 显示或杀死属于其它用户的服务线程。
RELOAD: 重载访问控制表，刷新日志等。
SHUTDOWN: 关闭MySQL服务。
数据库/数据表/数据列权限：
ALTER: 修改已存在的数据表(例如增加/删除列)和索引。
CREATE: 建立新的数据库或数据表。
DELETE: 删除表的记录。
DROP: 删除数据表或数据库。
INDEX: 建立或删除索引。
INSERT: 增加表的记录。
SELECT: 显示/搜索表的记录。
UPDATE: 修改表中已存在的记录。
特别的权限：
ALL: 允许做任何事(和root一样)。
USAGE: 只允许登录--其它什么也不允许做。
常用MYSQL命令
启动：net start mySql;
　　进入：mysql -u root -p/mysql -h localhost -u root -p databaseName;
　　列出数据库：show databases;
　　选择数据库：use databaseName;
　　列出表格：show tables；
　　创建数据表：mysql> CREATE TABLE mytable (name VARCHAR(20), sex CHAR(1),
-> birth DATE, birthaddr VARCHAR(20));
    显示表格列的属性：show columns from tableName；
    修改表的结构：DESCRIBE mytable;
　　建立数据库：source fileName.txt;
　　匹配字符：可以用通配符_代表任何一个字符，％代表任何字符串;
　　增加一个字段：alter table tabelName add column fieldName dateType;
　　增加多个字段：alter table tabelName add column fieldName1 dateType,add columns fieldName2 dateType;
　　多行命令输入:注意不能将单词断开;当插入或更改数据时，不能将字段的字符串展开到多行里，否则硬回车将被储存到数据中;
　　增加一个管理员帐户：grant all on *.* to user@localhost identified by "password";
　　每条语句输入完毕后要在末尾填加分号';'，或者填加'\g'也可以；
　　查询时间：select now();
　　查询当前用户：select user();
　　查询数据库版本：select version();
　　查询当前使用的数据库：select database();
    用文本方式将数据装入一个数据库表
　　如果一条一条地输入，很麻烦。我们可以用文本文件的方式将所有记录加入你的数据库表中。创建一个文本文件“mysql.txt”，每行包含一个记录，用定位符(tab)把值分开，并且以在CREATE TABLE语句中列出的列次序给出，例如：
abccs f 1977-07-07 china 　 mary f 1978-12-12 usa tom m 1970-09-02 usa
　　使用下面命令将文本文件“mytable.txt”装载到mytable表中:mysql> LOAD DATA LOCAL INFILE "mytable.txt" INTO TABLE pet;
　　再使用如下命令看看是否已将数据输入到数据库表中：mysql> select * from mytable;
(e129) 　　
　　1、删除student_course数据库中的students数据表：
　　rm -f student_course/students.*
　　2、备份数据库：(将数据库test备份)
　　mysqldump -u root -p test>c:\test.txt
　　备份表格：(备份test数据库下的mytable表格)
　　mysqldump -u root -p test mytable>c:\test.txt
　　将备份数据导入到数据库：(导回test数据库)
　　mysql -u root -p test<c:\test.txt
　　3、创建临时表：(建立临时表zengchao)
　　create temporary table zengchao(name varchar(10));
　　4、创建表是先判断表是否存在
　　create table if not exists students(……);
　　5、从已经有的表中复制表的结构
　　create table table2 select * from table1 where 1<>1;
　　6、复制表
　　create table table2 select * from table1;
　　7、对表重新命名
　　alter table table1 rename as table2;
　　8、修改列的类型
　　alter table table1 modify id int unsigned;//修改列id的类型为int unsigned
　　alter table table1 change id sid int unsigned;//修改列id的名字为sid，而且把属性修改为int unsigned
　　9、创建索引
　　alter table table1 add index ind_id (id);
　　create index ind_id on table1 (id);
　　create unique index ind_id on table1 (id);//建立唯一性索引
　　10、删除索引
　　drop index idx_id on table1;
　　alter table table1 drop index ind_id;
　　11、联合字符或者多个列(将列id与":"和列name和"="连接)
　　select concat(id,':',name,'=') from students;
　　12、limit(选出10到20条)<第一个记录集的编号是0>
　　select * from students order by id limit 9,10;
　　13、MySQL不支持的功能
　　事务，视图，外键和引用完整性，存储过程和触发器
　　14、MySQL会使用索引的操作符号
　　<,<=,>=,>,=,between,in,不带%或者_开头的like
　　15、使用索引的缺点
　　1)减慢增删改数据的速度；
　　2）占用磁盘空间；
　　3）增加查询优化器的负担；
　　当查询优化器生成执行计划时，会考虑索引，太多的索引会给查询优化器增加工作量，导致无法选择最优的查询方案；
　　16、分析索引效率
　　方法：在一般的SQL语句前加上explain；
　　分析结果的含义：
　　1）table：表名；
　　2）type：连接的类型，(ALL/Range/Ref)。其中ref是最理想的；
　　3）possible_keys：查询可以利用的索引名；
　　4）key：实际使用的索引；
　　5）key_len：索引中被使用部分的长度（字节）；
　　6）ref：显示列名字或者"const"（不明白什么意思）；
　　7）rows：显示MySQL认为在找到正确结果之前必须扫描的行数；
　　8）extra：MySQL的建议；
　　17、使用较短的定长列
　　1）尽可能使用较短的数据类型；
　　2）尽可能使用定长数据类型；
　　a）用char代替varchar，固定长度的数据处理比变长的快些；
　　b）对于频繁修改的表，磁盘容易形成碎片，从而影响数据库的整体性能；
　　c）万一出现数据表崩溃，使用固定长度数据行的表更容易重新构造。使用固定长度的数据行，每个记录的开始位置都是固定记录长度的倍数，可以很容易被检测到，但是使用可变长度的数据行就不一定了；
　　d）对于MyISAM类型的数据表，虽然转换成固定长度的数据列可以提高性能，但是占据的空间也大；
　　18、使用not null和enum
　　尽量将列定义为not null，这样可使数据的出来更快，所需的空间更少，而且在查询时，MySQL不需要检查是否存在特例，即null值，从而优化查询；
　　如果一列只含有有限数目的特定值，如性别，是否有效或者入学年份等，在这种情况下应该考虑将其转换为enum列的值，MySQL处理的更快，因为所有的enum值在系统内都是以标识数值来表示的；
　　19、使用optimize table
　　对于经常修改的表，容易产生碎片，使在查询数据库时必须读取更多的磁盘块，降低查询性能。具有可变长的表都存在磁盘碎片问题，这个问题对blob数据类型更为突出，因为其尺寸变化非常大。可以通过使用optimize table来整理碎片，保证数据库性能不下降，优化那些受碎片影响的数据表。 optimize table可以用于MyISAM和BDB类型的数据表。实际上任何碎片整理方法都是用mysqldump来转存数据表，然后使用转存后的文件并重新建数据表；
　　20、使用procedure analyse()
　　可以使用procedure analyse()显示最佳类型的建议，使用很简单，在select语句后面加上procedure analyse()就可以了；例如：
　　select * from students procedure analyse();
　　select * from students procedure analyse(16,256);
　　第二条语句要求procedure analyse()不要建议含有多于16个值，或者含有多于256字节的enum类型，如果没有限制，输出可能会很长；
　　21、使用查询缓存
　　1）查询缓存的工作方式：
　　第一次执行某条select语句时，服务器记住该查询的文本内容和查询结果，存储在缓存中，下次碰到这个语句时，直接从缓存中返回结果；当更新数据表后，该数据表的任何缓存查询都变成无效的，并且会被丢弃。
　　2）配置缓存参数：
　　变量：query_cache _type，查询缓存的操作模式。有3中模式，0：不缓存；1：缓存查询，除非与select sql_no_cache开头；2：根据需要只缓存那些以select sql_cache开头的查询；query_cache_size：设置查询缓存的最大结果集的大小，比这个值大的不会被缓存。
　　22、调整硬件
　　1）在机器上装更多的内存；
　　2）增加更快的硬盘以减少I/O等待时间；
　　寻道时间是决定性能的主要因素，逐字地移动磁头是最慢的，一旦磁头定位，从磁道读则很快；
　　3）在不同的物理硬盘设备上重新分配磁盘活动；
　　如果可能，应将最繁忙的数据库存放在不同的物理设备上，这跟使用同一物理设备的不同分区是不同的，因为它们将争用相同的物理资源（磁头）。
1、启动MySQL服务器
　两种方法: 一是用winmysqladmin，如果机器启动时已自动运行，则可直接进入下一步操作。 二是在DOS方式下运行 d:\mysql\bin\mysqld
　(假设mysql 安装在d:\mysql);当然也可以 net start mysql命令启动。而Linux中启动时可用“/etc/rc.d/init.d/mysqld start”命令，注 意启动者应具有管理员权限。
2、进入mysql交互操作界面
　　在DOS方式下，运行: d:\mysql\bin\mysql
　　出现: mysql 的提示符” mysql> “，此时已进入mysql的交互操作方式。
　　如果出现 “ERROR 2003: Can′t connect to MySQL server on ′localhost′ (10061)“，说明你的MySQL还没有启动。
3、退出MySQL操作界面
　　在mysql>提示符下输入quit可以随时退出交互操作界面:　　
　　mysql> quit
　　Bye
4、第一条命令(多条命令用 “,” 分隔 ,一条命令可以分成多行输入，直到出现分号”;”为止
　　mysql> select version(),current_date();
　　mysql>Select (20+5)*4;
　　mysql>Select (20+5)*4,sin(pi()/3);
　　mysql>Select (20+5)*4 AS Result,sin(pi()/3); (AS: 指定假名为Result)
5、常用sql 语句
1:使用SHOW语句找出在服务器上当前存在什么数据库：
mysql> SHOW DATABASES;
2:2、创建一个数据库MYSQLDATA
mysql> Create DATABASE MYSQLDATA;
3:选择你所创建的数据库
mysql> USE MYSQLDATA; (按回车键出现Database changed 时说明操作成功！)
4:查看现在的数据库中存在什么表
mysql> SHOW TABLES;
5:创建一个数据库表
mysql> Create TABLE MYTABLE (name VARCHAR(20), sex CHAR(1));
6:显示表的结构：
mysql> DESCRIBE MYTABLE;
7:往表中加入记录
mysql> insert into MYTABLE values (”hyq”,”M”);
8:用文本方式将数据装入数据库表中（例如D:/mysql.txt）
mysql> LOAD DATA LOCAL INFILE “D:/mysql.txt” INTO TABLE MYTABLE;
9:导入.sql文件命令（例如D:/mysql.sql）
mysql>use database;
mysql>source d:/mysql.sql;
10:删除表
mysql>drop TABLE MYTABLE;
11:清空表
mysql>delete from MYTABLE;
12:更新表中数据
mysql>update MYTABLE set sex=”f” where name=’hyq’;
13:插入数据
mysq>linsert into MYTABLE values (1, ‘第二章’, 0×2134545);
14:登录后使用数据库 mysql
mysql -u root -p mysql
mysql -u root -p -h 11.11.11.11 database
15:mysql jdbc连接url 使用中文
jdbc:mysql://localhost/test?useUnicode=true&characterEncoding=gb2312
[separator]
其它：
show processlist;列出每一笔联机的信息。
show variables;列出mysql的系统设定。
show tables from db_name;列出db_name中所有数据表;
show [full] columns from table_name;列出table_name中完整信息，如栏名、类型,包括字符集编码。
show index from table_name; 列出table_name中所有的索引。
show table status;；列出当前数据库中数据表的信息。
show table status from db_name;；列出当前db_name中数据表的信息。
alter table table_name engine innodb|myisam|memory ;更改表类型
explain table_name / describe table_name ; 列出table_name完整信息，如栏名、类型。
show create table table_name 显示当前表的建表语句
alter table table_name add primary key (picid) ; 向表中增加一个主键
alter table table_name add column userid int after picid 修改表结构增加一个新的字段
alter table table_name character set gb2312 改变表的编码
select user(); 显示当前用户。
select password(’root’); 显示当前用户密码
select now(); 显示当前日期
flush privileges 在不重启的情况下刷新用户权限
mysqld –default-character-set=gb2312 ;设置默认字符集为gb2312
6、安全
刚安装好的MySql包含一个含空密码的root帐户和一个匿名帐户，这是很大的安全隐患，对于一些重要的应用我们应将安全性尽可能提高，在这里应把匿名帐户删除、 root帐户设置密码，可用如下命令进行：
use mysql;
delete from User where User=”";
update User set Password=PASSWORD(’newpassword’) where User=’root’;
改完重启 mysql 才能生效.
上面命令参数是常用参数的一部分，详细情况可参考文档。此处的mydb是要登录的数据库的名称。
在进行开发和实际应用中，用户不应该只用root用户进行连接数据库，虽然使用root用户进行测试时很方便，但会给系统带来重大安全隐患，也不利于管理技术的提高。我们给一个应用中使用的用户赋予最恰当的数据库权限。如一个只进行数据插入的用户不应赋予其删除数据的权限。MySql的用户管理是通过 User表来实现的，添加新用户常用的方法有两个，一是在User表插入相应的数据行，同时设置相应的权限；二是通过GRANT命令创建具有某种权限的用户。其中GRANT的常用用法如下：
grant all on mydb.* to NewUserName@HostName identified by “password” ;
grant usage on *.* to NewUserName@HostName identified by “password”;
grant select,insert,update on mydb.* to NewUserName@HostName identified by “password”;
grant update,delete on mydb.TestTable to NewUserName@HostName identified by “password”;
grant all privileges on *.* to root@localhost
grant select,insert,delete,update,alter,create,drop on lybbs.* to NewUserName@”%” identified by “lybbs”;
若要给此用户赋予他在相应对象上的权限的管理能力，可在GRANT后面添加WITH GRANT OPTION选项。而对于用插入User表添加的用户，Password字段应用PASSWORD 函数进行更新加密，以防不轨之人窃看密码。对于那些已经不用的用户应给予清除，权限过界的用户应及时回收权限，回收权限可以通过更新User表相应字段，也可以使用REVOKE操作。
全局管理权限：
FILE: 在MySQL服务器上读写文件。
PROCESS: 显示或杀死属于其它用户的服务线程。
RELOAD: 重载访问控制表，刷新日志等。
SHUTDOWN: 关闭MySQL服务。
数据库/数据表/数据列权限：
Alter: 修改已存在的数据表(例如增加/删除列)和索引。
Create: 建立新的数据库或数据表。
Delete: 删除表的记录。
Drop: 删除数据表或数据库。
INDEX: 建立或删除索引。
Insert: 增加表的记录。
Select: 显示/搜索表的记录。
Update: 修改表中已存在的记录。
特别的权限：
ALL: 允许做任何事(和root一样)。
USAGE: 只允许登录–其它什么也不允许做。
修改默认密码：
cd d:\mysql\bin
mysqladmin -u root -p password 123456
回车出现
Enter password: ( 注:这是叫你输入原密码. 刚安装时密码为空,所以直接回车即可)
此时mysql 中账号 root 的密码 被改为 123456 安装完毕
一、引言
　　 想使用Linux已经很长时间了，由于没有硬性任务一直也没有系统学习，近日由于工作需要必须使用Linux下的MySQL。本以为有Windows下使用SQL Server的经验，觉得在Linux下安装MySql应该是易如反掌的事，可在真正安装和使用MySQL时走了很多弯路，遇见很多问题，毕竟Linux和Windows本身就有很大区别。为了让和我一样的初学者在学习的过程中少走弯路，尽快入门，写了此文，希望对您有所帮助。本文的Linux环境是Red Hat 9.0，MySQL是4.0.16。
　　 二、安装Mysql
　　 1、下载MySQL的安装文件
　　 安装MySQL需要下面两个文件：
　　 MySQL-server-4.0.16-0.i386.rpm　　　
　　 MySQL-client-4.0.16-0.i386.rpm
　　 下载地址为：http://www.mysql.com/downloads/mysql-4.0.html， 打开此网页，下拉网页找到“Linux x86 RPM downloads”项，找到“Server”和“Client programs”项，下载需要的上述两个rpm文件。
　　 2、安装MySQL
　　 rpm文件是Red Hat公司开发的软件安装包，rpm可让Linux在安装软件包时免除许多复杂的手续。该命令在安装时常用的参数是 –ivh ,其中i表示将安装指定的rmp软件包，V表示安装时的详细信息，h表示在安装期间出现“#”符号来显示目前的安装过程。这个符号将持续到安装完成后才停止。
　　 1）安装服务器端
　　 在有两个rmp文件的目录下运行如下命令：
　　 [root@test1 local]# rpm -ivh MySQL-server-4.0.16-0.i386.rpm
　　 显示如下信息。
　　　 warning: MySQL-server-4.0.16-0.i386.rpm: V3 DSA signature: NOKEY, key ID 5072e1f5
　　 Preparing...　　　　　　　########################################### [100%]
　　 1:MySQL-server　　　　　########################################### [100%]
　　　 。。。。。。（省略显示）
　　 /usr/bin/mysqladmin -u root password 'new-password'
　　 /usr/bin/mysqladmin -u root -h test1 password 'new-password'
　　　 。。。。。。（省略显示）
　　 Starting mysqld daemon with databases from /var/lib/mysql
　　 如出现如上信息，服务端安装完毕。测试是否成功可运行netstat看Mysql端口是否打开，如打开表示服务已经启动，安装成功。Mysql默认的端口是3306。
　　 [root@test1 local]# netstat -nat
　　 Active Internet connections (servers and established)
　　 Proto Recv-Q Send-Q Local Address　　　　　 Foreign Address　　　　 State　　　
　　 tcp　　0　　0 0.0.0.0:3306　　　　 0.0.0.0:*　　　　　 LISTEN　　　
　　 上面显示可以看出MySQL服务已经启动。
　　 2）安装客户端
　　 运行如下命令：
　　 [root@test1 local]# rpm -ivh MySQL-client-4.0.16-0.i386.rpm
　　 warning: MySQL-client-4.0.16-0.i386.rpm: V3 DSA signature: NOKEY, key ID 5072e1f5
　　 Preparing...　　　　########################################### [100%]
　　 1:MySQL-client　 ########################################### [100%]
　　 显示安装完毕。
　　 用下面的命令连接mysql,测试是否成功。
　 三、登录MySQL
　　 登录MySQL的命令是mysql， mysql 的使用语法如下：
　　 mysql [-u username] [-h host] [-p[password]] [dbname]
　　 username 与 password 分别是 MySQL 的用户名与密码，mysql的初始管理帐号是root，没有密码，注意：这个root用户不是Linux的系统用户。MySQL默认用户是root，由于初始没有密码，第一次进时只需键入mysql即可。
　　 [root@test1 local]# mysql
　　 Welcome to the MySQL monitor.　Commands end with ; or \g.
　　 Your MySQL connection id is 1 to server version: 4.0.16-standard
　　 Type 'help;' or '\h' for help. Type '\c' to clear the buffer.
　　 mysql>
　　 出现了“mysql>”提示符，恭喜你，安装成功！
　　 增加了密码后的登录格式如下：
　　 mysql -u root -p
　　 Enter password: (输入密码)
　　 其中-u后跟的是用户名，-p要求输入密码，回车后在输入密码处输入密码。
　　 注意：这个mysql文件在/usr/bin目录下，与后面讲的启动文件/etc/init.d/mysql不是一个文件。
　　 四、MySQL的几个重要目录
　　 MySQL安装完成后不象SQL Server默认安装在一个目录，它的数据库文件、配置文件和命令文件分别在不同的目录，了解这些目录非常重要，尤其对于Linux的初学者，因为Linux本身的目录结构就比较复杂，如果搞不清楚MySQL的安装目录那就无从谈起深入学习。
　　 下面就介绍一下这几个目录。
　　 1、数据库目录
　　 /var/lib/mysql/
　　 2、配置文件
　　 /usr/share/mysql（mysql.server命令及配置文件）
　　 3、相关命令
　　 /usr/bin(mysqladmin mysqldump等命令)
　　 4、启动脚本
　　 /etc/rc.d/init.d/（启动脚本文件mysql的目录）
　 五、修改登录密码
　　 MySQL默认没有密码，安装完毕增加密码的重要性是不言而喻的。
　　 1、命令
　　 usr/bin/mysqladmin -u root password 'new-password'
　　 格式：mysqladmin -u用户名 -p旧密码 password 新密码
　　 2、例子
　　 例1：给root加个密码123456。
　　 键入以下命令 ：
　　 [root@test1 local]# /usr/bin/mysqladmin -u root password 123456
　　 注：因为开始时root没有密码，所以-p旧密码一项就可以省略了。
　　 3、测试是否修改成功
　　 1）不用密码登录
　　 [root@test1 local]# mysql
　　 ERROR 1045: Access denied for user: 'root@localhost' (Using password: NO)
　　 显示错误，说明密码已经修改。
　　 2）用修改后的密码登录
　　 [root@test1 local]# mysql -u root -p
　　 Enter password: (输入修改后的密码123456)
　　 Welcome to the MySQL monitor.　Commands end with ; or \g.
　　 Your MySQL connection id is 4 to server version: 4.0.16-standard
　　 Type 'help;' or '\h' for help. Type '\c' to clear the buffer.
　　 mysql>
　　 成功！
　　 这是通过mysqladmin命令修改口令，也可通过修改库来更改口令。
　　 六、启动与停止
　　 1、启动
　　 MySQL安装完成后启动文件mysql在/etc/init.d目录下，在需要启动时运行下面命令即可。
　　 [root@test1 init.d]# /etc/init.d/mysql start
　　 2、停止
　　 /usr/bin/mysqladmin -u root -p shutdown
　　 3、自动启动
　　 1）察看mysql是否在自动启动列表中
　　 [root@test1 local]#　/sbin/chkconfig –list
　　 2）把MySQL添加到你系统的启动服务组里面去
　　 [root@test1 local]#　/sbin/chkconfig　– add　mysql
　　 3）把MySQL从启动服务组里面删除。
　　 [root@test1 local]#　/sbin/chkconfig　– del　mysql
七、更改MySQL目录
　　 MySQL默认的数据文件存储目录为/var/lib/mysql。假如要把目录移到/home/data下需要进行下面几步：
　　 1、home目录下建立data目录
　　 cd /home
　　 mkdir data
　　 2、把MySQL服务进程停掉：
　　 mysqladmin -u root -p shutdown
　　 3、把/var/lib/mysql整个目录移到/home/data
　　 mv /var/lib/mysql　/home/data/
　　 这样就把MySQL的数据文件移动到了/home/data/mysql下
　　 4、找到my.cnf配置文件
　　 如果/etc/目录下没有my.cnf配置文件，请到/usr/share/mysql/下找到*.cnf文件，拷贝其中一个到/etc/并改名为my.cnf)中。命令如下：
　　 [root@test1 mysql]# cp /usr/share/mysql/my-medium.cnf　/etc/my.cnf
　　 5、编辑MySQL的配置文件/etc/my.cnf
　　 为保证MySQL能够正常工作，需要指明mysql.sock文件的产生位置。 修改socket=/var/lib/mysql/mysql.sock一行中等号右边的值为：/home/mysql/mysql.sock 。操作如下：
　　 vi　 my.cnf　　　 (用vi工具编辑my.cnf文件，找到下列数据修改之)
　　 # The MySQL server
　　　 [mysqld]
　　　 port　　　= 3306
　　　 #socket　 = /var/lib/mysql/mysql.sock（原内容，为了更稳妥用“#”注释此行）
　　　 socket　 = /home/data/mysql/mysql.sock　　　（加上此行）
　　 6、修改MySQL启动脚本/etc/rc.d/init.d/mysql
　　 最后，需要修改MySQL启动脚本/etc/rc.d/init.d/mysql，把其中datadir=/var/lib/mysql一行中，等号右边的路径改成你现在的实际存放路径：home/data/mysql。
　　 [root@test1 etc]# vi　/etc/rc.d/init.d/mysql
　　 #datadir=/var/lib/mysql　　　　（注释此行）
　　 datadir=/home/data/mysql　　 （加上此行）
　　 7、重新启动MySQL服务
　　 /etc/rc.d/init.d/mysql　start
　　 或用reboot命令重启Linux
　　 如果工作正常移动就成功了，否则对照前面的7步再检查一下。
　　 八、MySQL的常用操作
　　 注意：MySQL中每个命令后都要以分号；结尾。
　　 1、显示数据库
　　 mysql> show databases;
　　 +----------+
　　 | Database |
　　 +----------+
　　 | mysql　　|
　　 | test　　 |
　　 +----------+
　　 2 rows in set (0.04 sec)
　　 Mysql刚安装完有两个数据库：mysql和test。mysql库非常重要，它里面有MySQL的系统信息，我们改密码和新增用户，实际上就是用这个库中的相关表进行操作。
　　 2、显示数据库中的表
　　 mysql> use mysql; （打开库，对每个库进行操作就要打开此库，类似于foxpro ）
　　 Database changed
　　 mysql> show tables;
　　 +-----------------+
　　 | Tables_in_mysql |
　　 +-----------------+
　　 | columns_priv　　|
　　 | db　　　　　　　|
　　 | func　　　　　　|
　　 | host　　　　　　|
　　 | tables_priv　　 |
　　 | user　　　　　　|
　　 +-----------------+
　　 6 rows in set (0.01 sec)
　　 3、显示数据表的结构：
　　 describe 表名;
　　 4、显示表中的记录：
　　 select * from 表名;
　　 例如：显示mysql库中user表中的纪录。所有能对MySQL用户操作的用户都在此表中。
　　 Select * from user;
　　 5、建库：
　　 create database 库名;
　　 例如：创建一个名字位aaa的库
　　 mysql> create databases aaa;
6、建表：
　　 use 库名；
　　 create table 表名 (字段设定列表)；
　　 例如：在刚创建的aaa库中建立表name,表中有id(序号，自动增长)，xm（姓名）,xb（性别）,csny（出身年月）四个字段
　　 use aaa;
　　 mysql> create table name (id int(3) auto_increment not null primary key, xm char(8),xb char(2),csny date);
　　 可以用describe命令察看刚建立的表结构。
　　 mysql> describe name;
　　 +-------+---------+------+-----+---------+----------------+
　　 | Field | Type　　| Null | Key | Default | Extra　　　　　|
　　 +-------+---------+------+-----+---------+----------------+
　　 | id　　| int(3)　|　　　| PRI | NULL　　| auto_increment |
　　 | xm　　| char(8) | YES　|　　 | NULL　　|　　　　　　　　|
　　 | xb　　| char(2) | YES　|　　 | NULL　　|　　　　　　　　|
　　 | csny　| date　　| YES　|　　 | NULL　　|　　　　　　　　|
　　 +-------+---------+------+-----+---------+----------------+
　　 7、增加记录
　　 例如：增加几条相关纪录。
　　 mysql> insert into name values('','张三','男','1971-10-01');
　　 mysql> insert into name values('','白云','女','1972-05-20');
　　 可用select命令来验证结果。
　　 mysql> select * from name;
　　 +----+------+------+------------+
　　 | id | xm　 | xb　 | csny　　　 |
　　 +----+------+------+------------+
　　 |　1 | 张三 | 男　 | 1971-10-01 |
　　 |　2 | 白云 | 女　 | 1972-05-20 |
　　 +----+------+------+------------+
　　 8、修改纪录
　　 例如：将张三的出生年月改为1971-01-10
　　 mysql> update name set csny='1971-01-10' where xm='张三';
　　 9、删除纪录
　　 例如：删除张三的纪录。
　　 mysql> delete from name where xm='张三';
　　 10、删库和删表
　　 drop database 库名;
　　 drop table 表名；
　　 九、增加MySQL用户
　　 格式：grant select on 数据库.* to 用户名@登录主机 identified by "密码"
例1、增加一个用户user_1密码为123，让他可以在任何主机上登录，并对所有数据库有查询、插入、修改、删除的权限。首先用以root用户连入MySQL，然后键入以下命令：
　　 mysql> grant select,insert,update,delete on *.* to user_1@"%" Identified by "123";
例1增加的用户是十分危险的，如果知道了user_1的密码，那么他就可以在网上的任何一台电脑上登录你的MySQL数据库并对你的数据为所欲为了，解决办法见例2。
　　 例2、增加一个用户user_2密码为123,让此用户只可以在localhost上登录，并可以对数据库aaa进行查询、插入、修改、删除的操作（localhost指本地主机，即MySQL数据库所在的那台主机），这样用户即使用知道user_2的密码，他也无法从网上直接访问数据库，只能通过MYSQL主机来操作aaa库。
　　 mysql>grant select,insert,update,delete on aaa.* to user_2@localhost identified by "123";
　　 用新增的用户如果登录不了MySQL，在登录时用如下命令：
　　 mysql -u user_1 -p　-h 192.168.113.50　（-h后跟的是要登录主机的ip地址）
　　 十、备份与恢复
　　 1、备份
　　 例如：将上例创建的aaa库备份到文件back_aaa中
　　 [root@test1 root]# cd　/home/data/mysql　(进入到库目录，本例库已由val/lib/mysql转到/home/data/mysql，见上述第七部分内容)
　　 [root@test1 mysql]# mysqldump -u root -p --opt aaa > back_aaa
　　 2、恢复
　　 [root@test mysql]# mysql -u root -p ccc < back_aaa
