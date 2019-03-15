# 入门常用SQL及官方文档的使用



**SQL语句基础理论**

　　SQL是操作和检索关系型数据库的标准语言，标准SQL语句可用于操作关系型数据库。

**5大主要类型：**

　　①DQL（Data Query Language，数据查询语言）语句**，**主要由于select关键字完成，查询语句是SQL语句中最复杂，功能最丰富的语句。

 

　　②DML（Data Munipulation Language，数据操作语言）语句，DML语句修改后数据将保持较好的一致性；操作表的语句，如增插insert、更新update、删除delete等；

 

　　③DDL（Data Definition Language，数据定义语言）语句，操作数据对象的语言，包括创建create、删除drop、修改alter数据库对象。

 

　　④DCL（Data Control Language，数据控制语言）语句，主要有grant、revoke语句。

 

　　⑤事务控制语句**：**主要有commit、rollback和savepoint三个关键字完成

 

**注意：**

　　 SQL 对大小写不敏感：SELECT 与 select 是相同的；

　　MySQL 的SQL语句以分号 (;) 作为结束标识。

 

DBA所有的操作必须来自于官方文档

DBA所有的操作必须来自于官方文档

DBA所有的操作必须来自于官方文档

（重要的事情说三遍嘛！！）

 

**一、官方文档的简单介绍：**

　　1、Tutorial：将MySQL常用的一些操作使用一个场景串联起来

　　　　只是关注里面的灰色部分就可以，按照里面的灰色操作部分顺一遍

　　---详细的跟着官方文档的tutorial模块操作一遍对于初学者还是很有必要的

　　2、server Administrator：MySQL管理需要的一些命令、工具、参数等

　　3、SQL Syntax

　　SQL语法，使用最多，特别是DDL语句一定要使用SQL语法进行参考（增删改查）

　　4、Server Option / Variable Reference：MySQL的参数和状态值，使用较多

　　5、Functions and Operators

　　MySQL常用函数和操作符，使用较多

　　6、Views and Stored Programs

　　视图、存储过程、函数、触发器、event语法参考

　　7、Optimization：优化

　　非常值得细致的看一遍，此篇文档不仅仅用来参考，更多的是用来学习优化知识，算是DBA进阶宝典

　　8、Partitioning

　　如果是要进行表分区，此文档是必须参考的资料，也是唯一参考的资料

　　9、Information Schema、Performance Schema

　　中级DBA常用的两个参考资料

　　10、Spatial Extensions

　　地理位置信息（附近的人、附近的餐馆……）

　　11、Replication

　　MySQL使用复制功能，常用的参考资料

　　12、Semisynchronous Replication

　　半同步复制，个别场合会用到

 

**二、官方文档使用技巧**

　　1、{}大括号括起来并且中间有个|管道符的代表 n 选一，必须输入

　　2、[]中括号里括起来的表示可有可无

　　3、小写字母代表变量，将来可以替换

　　4、大写字母代表关键字，必须输入

　　5、一般有 sepcification 后缀的表示这是一个规范，后边还会有详细的介绍

　　6、 |管道符连接的信息没有用{}大括号括起来代表可以有其中一个也可以同时都有

 

三、MySQL数据库常用命令：help contents帮助

1、显示所有数据库

　　show databases：列出MySQL数据库管理系统的数据库列表

2、创建数据库

　　create database [if not exists] dbName：创建名字为dbName指定的数据库

3、删除数据库

　　drop database dbName：删除dbName指定的数据库

4、统计数据库信息 

　　show table status [{from|in} db_name] [like 'pattern'|where expr] \G：将输出Mysql数据库管理系统的性能及统计信息

示例：

　　show table status from RUNOOB;    #显示数据库 RUNOOB 中所有表的信息

　　show table status from RUNOOB like 'runoob%';    #表名以runoob开头的表的信息

　　show table status from RUNOOB like 'runoob%'\G;    #加上\G，查询结果按列打印 

5、使用数据库

　　use dbName：切换、使用指定数据库 

6、显示表

　　show tables：显示指定数据库的所有表

7、显示表结构

　　desc tableName：显示表结构describe(desc)

　　show columns from tableName：显示数据表的属性，属性类型，主键信息 ，是否为 NULL，默认值等其他信息---类似于desc tableName

8、显示表索引

　　show index from 数据表：显示数据表的详细索引信息，包括PRIMARY KEY（主键）

9、创建表

　　create table tableName(列声明) ：创建一张表

示例：创建 students 表，存放 学号(id)、姓名(name)、性别(sex)、年龄(age)、联系电话(tel) 

　　create table students （ id int unsigned not null auto_increment primary key, name char(8) not null, sex char(4) not null, age tinyint unsigned not null, tel char(13) null default "-" );

10、显示表内容(数据)

　　select * from tableName：显示数据表的具体内容

　　select 列名称 from tableName [where 条件]：按条件查询表中的数据

11、修改表中内容(数据)---增、改、删

增：insert

　　insert [into] tableName [(列名1, 列名2, 列名3, ...)] values (值1, 值2, 值3, ...) ：向表中插入数据

示例：

　　insert into classmate values("xiaoming","male",18,"china","tennis");

改：update 

　　update tableName set 列名称=新值 where 更新条件：更新表中的数据

示例：

　　update classmate set address="chongyi" where id=3

 删：delete

　　delete from tableName where 删除条件：删除表中的数据

批量删：drop、truncate

　　drop table tableName：删除表（包括表的结构和全部数据）

　　truncate tableName：删除表（删除表里的全部数据，保留表结构）

12、创建后表的修改alter table

添加列

　　alter table 表名 add 列名 列数据类型 [after 插入位置];

示例:

　　alter table students add address char(60)：在表的最后追加列 address

　　alter table students add birthday date after age：在age 的列后插入列birthday

修改列

　　alter table 表名 change 列名称 列新名称 新数据类型;

示例:

　　alter table students change tel telphone char(13) default "-"：将表中的tel列改名为telphone

　　alter table students change name name char(16) not null：将 name 列的数据类型改为 char(16)

删除列

　　alter table 表名 drop 列名称;

示例:

　　alter table students drop birthday：删除 birthday 列 

重命名表

　　alter table 表名 rename 新表名;

示例:

　　alter table students rename workmates：重命名 students 表为 workmates

13、关于事务操作

　　1. BEGIN 或 START TRANSACTION：显式地开启一个事务；

　　2. COMMIT 或 COMMIT WORK：COMMIT会提交事务，并使已对数据库进行的所有修改成为永久性的；

　　3. ROLLBACK 或 ROLLBACK WORK：回滚会结束用户的事务，并撤销正在进行的所有未提交的修改；

　　4. SAVEPOINT identifier：SAVEPOINT允许在事务中创建一个保存点，一个事务中可以有多个SAVEPOINT；

　　5. RELEASE SAVEPOINT identifier：release(释放)，删除一个事务的保存点，当没有指定的保存点时，执行该语句会抛出一个异常；

　　6. ROLLBACK TO identifier：把事务回滚到标记点；

　　7. SET TRANSACTION；用来设置事务的隔离级别。

@author：http://www.cnblogs.com/geaozhang/