# mySql学习笔记：比sql server书写要简单 - 左直拳的马桶_日用桶 - CSDN博客
2014年07月09日 22:19:57[左直拳](https://me.csdn.net/leftfist)阅读数：1882
个人分类：[mySql](https://blog.csdn.net/leftfist/article/category/2557425)
在学mySql。总的感觉，mySql与Sql Server差不多，语法都很象，但mySql也许是吸取了SQL SERVER的一些经验，SQL语句书写起来更加简单。
比如说，设置主键、索引，SQL SERVER里的语句我是记不住的，但mysql却很简便：
```
CREATE table t(
id int not null
,name varchar(50) null
,primary key(id)
,index ix_name(name));
```
又如：
```
use test;
drop table if exists t3;
CREATE TABLE t3 (
  id INT NOT NULL,
  NAME CHAR(30) NOT NULL,
  age INT NOT NULL,
  info VARCHAR (255),
  INDEX MultiIdx(id, NAME, age)
);
INSERT INTO t3(id ,NAME,age,info) VALUES(1,'小明',12,'nihao'),(2,'小芳',16,'nihao');
explain select * from t3 where name='小芳';
```
如果存在某表，先删再建之；
显示执行情况，看看复合索引是否被用，太简便了。
mysql 的查询还支持正则表达式
另外，mysql的表支持两种类型：innoDB 和 myISAM。后者不支持事务，但速度更快，很适合用在只读。
