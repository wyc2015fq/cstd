# 如何在MySQL数据库添加和修改字段以及操作数据库 - xqhrs232的专栏 - CSDN博客
2018年11月28日 17:20:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：70
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/qq_37138818/article/details/82145358](https://blog.csdn.net/qq_37138818/article/details/82145358)
相关文章
1、MySQL数据表增加字段----[https://blog.csdn.net/liuboxx1/article/details/80649643](https://blog.csdn.net/liuboxx1/article/details/80649643)
2、[MySQL中的表中增加删除字段](https://www.cnblogs.com/softidea/p/6135097.html)----[https://www.cnblogs.com/softidea/p/6135097.html](https://www.cnblogs.com/softidea/p/6135097.html)
一》 数据的操作
1.登录数据库
>mysql -u root -p 数据库名称
2---创建数据库
命令：：create database 数据库名 charset utf8;
3--打开数据库
命令：：use 数据库名；
4--显示创建数据库的语句
命令：：show create database 数据库名；
5--删除数据库
命令：：drop database 数据库名；
二》 数据表的操作
1--核实当前数据库
命令：：select database(); 或者status;
2--创建数据表
命令：：create table 数据表名 （字段定义）charset utf8；
3--显示创建数据表的语句
命令：：show create table 数据表名
4--显示数据表的结构
命令：：desc 数据表名；
5--更改表名
命令：：rename table 旧的数据表名 to 新的数据表名；
6--显示数据库数据表的情况
命令：：show tables;
7--删除数据表
命令：：drop table 数据表名；
三》》数据表的---字段操作
1 --增：向数据表添加字段
命令：：alter table 数据表名 add 字段名 数据类型【约束】 after 字段名-是放在谁的后面；
2 ---改：分为两种情况：
一是只修改数据表类型或约束
命令：：alter table 数据表名 modify 字段名 新的数据类型【新的约束】；
二是修改整个字段
命令：：alter table 数据表名 change 旧的字段名 新的字段名 新的数据类型【约束】；
3 ---查；
命令：desc 数据表名；
4--删：把字段从数据表中移除
命令 alter table drop 字段名；
四》》数据表的--数据的操作
1--增 ：把数据存入数据表中；
命令：：insert into 数据表名（字段名1，字段名2）values（‘值1’，‘值2‘）；
2--改：修改字段的值
命令：：update 数据表名 set 字段名1=’新值1‘，字段名2=’新值2‘ where 字句；
3-查：把数据显示出来
命令：：select *|字段列表 from 数据表 ； 是显示所有的数据
命令：：select *|字段列表 from 数据表 where 字句 ； 是筛选查询的记录
命令：：select *|字段列表 from 数据表 where 字句 order by 字段名；
是按照字段名排序 asc 升序 desc 降序
命令：：select *|字段列表 from 数据表 where 字句 order by 字段名 limit start,num;
是将查询的记录再次限制显示的条数，start开始位置 num 条数
4--统计数据表的个数’
命令：：select count（*） from 数据表名；
5--删：将数据从数据表移除；
命令：：delete from 数据表名 ； 是删除数据表的所有数据；
命令：：delete from 数据表名 where 字段； 是按照要求删除相应的数据；
如整合期间有遇到什么问题 可以加群 858507220 一起讨论哦。
--------------------- 
作者：qq_37138818 
来源：CSDN 
原文：https://blog.csdn.net/qq_37138818/article/details/82145358 
版权声明：本文为博主原创文章，转载请附上博文链接！
