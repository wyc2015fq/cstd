# Mysql数据库的使用方法 - zhusongziye的博客 - CSDN博客





2018年05月08日 22:48:20[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：614








现在看软件测试的招聘要求，基本都是要求会数据库这一块的，薪水越高所具备的技能也要越多，而数据库知识则是一项必备技能了，所以今天小编就好好的普及下数据库知识方面的基本使用方法！

一、数据库的基本概念
1、 常用的两种引擎：

 (1) InnoDB引擎：
        1）支持ACID，简单地说就是支持事务完整性、一致性；
        2）支持行锁，以及类似ORACLE的一致性读，多用户并发；
        3）独有的聚集索引主键设计方式，可大幅提升并发读写性能；
        4）支持外键；
        5）支持崩溃数据自修复；
       6） InnoDB设计目标是处理大容量数据库系统，它的CPU利用率是其它基于磁盘的关系数据库引擎所不能比的。
       7）它是一个可靠地事务处理引擎，不支持全文本搜索
 (2) MyISAM引擎：
        1）不支持每次查询具有原子性
        2）只支持表所
        3）强调的是性能，其执行速度比InnoDB类型更快，但是不提供事务支持
        4）如果执行大量的SELECT（删除任务），MyISAM是更好的选择

2、数据库的介绍：
        1）数据库-database：保存有组织的数据的容器（通常是一个文件或一组文件）
        2）表-table：某种特定类型数据的结构化清单
        3）模式-schema：关于数据库和表的布局及特性的信息

        4）列-column：表中的一个字段，所有表都是由一个或多个列组成的
        5）行-row：表中的一个记录

        6）数据类型-datatype：所容许的数据的类型，每个表列都有相应的数据类型，它限制（或容许）该列中存储的数据



二、Mysql命令行（如下图）

![](https://img-blog.csdn.net/20180426220807358?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



1、连接mysql——格式： mysql -h主机地址 -u用户名 －p用户密码
       1）连接到本地Mysql，首先打开DOS窗口（快捷键window田字键+R一起调出命令指示框再输入cmd），然后进入目录mysql\bin，再键入命令mysql -u root -p，回车后会提示你输入密码， MYSQL的提示符是： mysql> (默认root用户没有密码)
       2）连接到远程主机上的Mysql：假设远程主机的IP为：218.105.110.116，用户名为root,密码为abcd123456，则键入以下命令：mysql -h218.105.110.116 -u root -p abcd123456；（注：u与root之间可以不用加空格，其它也一样）



2、启动mysql服务：net start mysql

3、停止mysql服务：net stop mysql


4、查询端口是否冲突：netstat –na | findstr 8080 查看被监听的端口 , findstr用于查找后面的端口是否存在。




5、退出mysql：quit或者exit退出


6、修改密码：格式：mysqladmin -u用户名 -p旧密码 password 新密码；例如给root修改密码： mysqladmin -u root -p ab12 password djg345 

三、显示命令

1、显示当前数据库服务器中的数据库列表：mysql> SHOW DATABASES；

2、显示某个数据库中的数据表：mysql> USE 库名；//使用某个库； mysql> SHOW TABLES；//列出库中所有的表


3、显示数据表的结构：mysql> DESCRIBE 表名；

4、建立数据库：mysql> CREATE DATABASE 库名；

5、建立数据表：mysql> USE 库名。mysql> CREATE TABLE 表名 (字段名 VARCHAR(20), 字段名 CHAR(1)；
6、删除数据库：mysql> DROP DATABASE 库名；
7、删除数据表：mysql> DROP TABLE 表名；
8、将表中记录清空：mysql> DELETE FROM 表名；

9、显示表中的记录：mysql> SELECT  *  FROM 表名；

10、往表中插入记录：mysql> INSERT INTO 表名 VALUES (”hyq”,”M”)；
11、更新表中数据：mysql-> UPDATE 表名 SET 字段名1=’a',字段名2=’b’ WHERE 字段名3=’c'；
12、用文本方式将数据装入数据表中：mysql> LOAD DATA LOCAL INFILE “D:/mysql.txt” INTO TABLE 表名；
13、导入.sql文件命令： mysql> USE 数据库名； mysql> SOURCE d:/mysql.sql；
14、命令行修改root密码：mysql>UPDATEmysql.userSETpassword=PASSWORD(’新密码’) WHERE User=’root’；

mysql> FLUSH PRIVILEGES；

15、显示use的数据库名：mysql> SELECT DATABASE()；
16、显示当前的user：mysql> SELECT USER()；



