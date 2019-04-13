
# 2012 年NOSQL学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客

2012年08月27日 14:52:44[littletigerat](https://me.csdn.net/littletigerat)阅读数：2232


**2012****年****NOSQL****学习笔记之一**
# 一、NOSQL是什么的缩写
NoSQL是Not Only SQL的缩写，而不是Not
 SQL。
# 二、NOSQL是什么？
首先NOSQL是一个数据库管理系统，但是他不同于传统的关系型数据库如Oracle、MySQL、DB等。
# 三、NOSQL与传统的关系型数据库有什么不同
1.NOSQL不使用SQL作为查询语言，也就是不需要Select语句查询；
2.NOSQL数据存储可以不需要固定的表格模式，也就是不需要先Create创建表结构，然后把数据insert表里面；
3.NOSQL会经常避免使用SQL的JOIN操作；
4.不遵循ACID的属性要求；
5.一般具有水平可扩展性的特征；
Relational  DataBase  Management关系型数据库，简称RDBM。
Relational  DataBase  Management  System关系型数据库管理系统，简称RDBMS。
# 四、
# 水平扩展性是什么意思？
水平扩展性(horizontal scalability)指能够连接多个软硬件的特性,这样可以将多个服务器从逻辑上看成一个实体。
# 五、NOSQL的前世今生
NoSQL一词最早出现于1998年，是Carlo
 Strozzi开发的一个轻量、开源、不提供SQL功能的关系数据库。
2009年，Last.fm的Johan
 Oskarsson发起了一次关于分布式开源数据库的讨论，来自Rackspace的Eric Evans再次提出了NoSQL的概念，这时的NoSQL主要指非关系型、分布式、不提供ACID的数据库设计模式。
2009年在亚特兰大举行的"no:sql(east)"讨论会是一个里程碑，其口号是"select
 fun, profit from real_world where relational=false;"。因此，对NoSQL最普遍的解释是“非关系型的”，强调Key-Value
 Stores和文档数据库的优点，而不是单纯的反对RDBMS。

