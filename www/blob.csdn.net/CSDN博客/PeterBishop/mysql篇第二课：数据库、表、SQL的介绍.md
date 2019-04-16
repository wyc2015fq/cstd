# mysql篇第二课：数据库、表、SQL的介绍 - PeterBishop - CSDN博客





2018年11月23日 13:14:13[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来学习下数据库、表和SQL



## **数据库存储数据的特点**
- 将数据放到表中，表再放到库中
- 一个数据库中可以有多个表，每个表都有一个的名字，用来标识自己。表名具有唯一性。
- 表具有一些特性，这些特性定义了数据在表中如何存储，类似java中 “类”的设计。
- 表由列组成，我们也称为字段。所有表都是由一个或多个列组成的，每一列类似java 中的”属性”
- 表中的数据是按行存储的，每一行类似于java中的“对象”。表中的每一行被称为一条记录



Mysql常见命令:

1.查看当前所有的数据库(分号不要掉)

show databases;

2.打开指定的库

use 库名

3.查看当前库的所有表

show tables;

4.查看其它库的所有表

show tables from 库名;

5.创建表

create table 表名(



列名列类型,

列名列类型，

         ...

);

6.查看表结构

desc 表名;



SQL(Structure Query Language)

结构化查询语言，用于和DBMS通信

说白了就是可以通过sql操作数据库

上面的创建表就是用SQL创建表



### **SQL的语言分类**

​DQL（Data Query Language）：数据查询语言 select

DML(Data Manipulate Language):数据操作语言 insert 、update、delete 

DDL（Data Define Languge）：数据定义语言 create、drop、alter 

TCL（Transaction Control Language）：事务控制语言 commit、rollback



以上不用记，反正也没人问这些具体概念，我们只是按这个分类学而已



