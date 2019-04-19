# MYSQL学习笔记 - 2019跟着小虎玩着去软考 - CSDN博客
2009年12月29日 18:13:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1538标签：[mysql																[数据库																[character																[table																[database																[variables](https://so.csdn.net/so/search/s.do?q=variables&t=blog)](https://so.csdn.net/so/search/s.do?q=database&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Database](https://blog.csdn.net/littletigerat/article/category/613908)
** MYSQL学习笔记**
# 1.查看有哪些数据库,显示数据库列表
show databases;
# 2.显示表的结构
## 方法一：
use student;
describe student;
## 方法二：
des student.student;
# 方法三：
show columns from student;
# 3.显示MYSQL的版本
select version();
# 4.显示库中的数据表
usemysql;
show tables;
# 5.建库
create database 库名;
# 6.删除库
drop database 库名;
# 7.建数据表
use 库名
create table 表名(字段列表);
# 8.清空表中记录
delete from 表名
# 9.删除数据表
drop table 表名;
# 10.如何登陆到mysql
运行cmd
进入C:/Program Files/MySQL/MySQL Server 5.1/bin
执行mysql -u root –p
或者mysql -uroot –pliuchang
或者mysql -uroot –p
或者mysql –h localhost –uroot -p
输入密码即可
# 11.退出mysql
Quit或者exit
# 12.如何修改用户的密码
在DOS下进入如下目录
C:/Program Files/MySQL/MySQL Server 5.1/bin
运行如下命令：
mysqladmin -uroot -pliuchang password lc
1、给root加个密码lc。首先在DOS下进入mysql的bin目录下，然后键入以下命令
mysqladmin -uroot -password ab12 
注：因为开始时root没有密码，所以-p旧密码一项就可以省略了。
# 13.将文本数据导入到数据库中
**load data local infile ‘****文件名’into table 表名;**
# 14.数据的导出
select * from 数据库.数据表名 into outfile‘文件名’;
# 15.数据库的备份
## 1.导出整个数据库
## 2.导出一个表
用法：mysqldump -u 用户名 -p 数据库名 表名> 导出的文件名
mysqldump -uroot -p student student>d:/learn/db_student.student.backup
## 3.导出一个数据结构
用法：mysqldump -uroot -p -d --add-drop-table 数据库名>导出文件名
mysqldump -uroot -p -d --add-drop-table test>d:/learn/db_test.structure.backup
**注意：
-d表示没有数据
--add-drop-table表示在每一个create语句之前添加一个drop table语句
打开c:/learn/db_test.structure.backup文件，和先前的两个文件比较，你会发现该文件里没有我们插入的数据.
# 16.数据库的导入
# 17.重命名数据表
mysql > alter table 老表名称 rename 新表名称;
# 18.删除数据库前有提示
mysqladmin drop database name 删除数据库前有提示
# 19.刷新数据库
# 20.删除数据库表中的记录
delete from 数据表名 where 删除的条件
# 21.往数据表中插入数据
insert into t_student values(1,'zhangsan','male','1986-11-24');
# 22.中文显示问题
latin1
MySQL中默认字符集的设置有四级:服务器级，数据库级，表级。最终是字段级的字符集设置。注意前三种均为默认设置，并不代码你的字段最终会使用这个字符集设置。所以我们建议要用show create table table; 或show full fields from tableName; 来检查当前表中字段的字符集设置。
MySQL中关于连接环境的字符集设置有Client端，connection, results 通过这些参数，MySQL就知道你的客户端工具用的是什么字符集，结果集应该是什么字符集。这样MySQL就会做必要的翻译，一旦这些参数有误，自然会导致字符串在转输过程中的转换错误。基本上99%的乱码由些造成
用SHOW CHARACTER SET命令可显示服务器支持的字符集列表
show full columns from t1;
show create table t1;
**当前联接系统参数****show variables like 'char%'**
1. 中文，请确保表中该字段的字符集为中文兼容：
big5| Big5 Traditional Chinese
gb2312| GB2312 Simplified Chinese
gbk| GBK Simplified Chinese
utf8| UTF-8 Unicode
2. 确保，联接参数与这个字段字符集一致,你可以用 set name 'charsetname';
比如， set name 'gbk';
这条命令会同时修改 character_set_client,character_set_connection,character_set_results
(如果你的这架MySQL中都为中文，则你可以在my.ini或my.cnf中加上或修改这个参数, 参数文件修改后需重启MySQL服务)
[mysql]
default-character-set=gbk
# 23.显示字符集
# 24.显示表格
show tables from student;
# 25.连接MYSQL
格式： mysql -h主机地址 -u用户名 -p用户密码
连接远程机器：
E:/mysql>mysql -h10.4.3.188 -uptsdb -p
等价写法
E:/mysql>mysql --host=10.4.3.188 --user=ptsdb --password
连接本地机器：
E:/mysql>mysql -uroot -p 
等价写法
E:/mysql>mysql --user=root -password
（注:u与root可以不用加空格，其它也一样）
# 26.显示创建的表
show create table student;
# 27.查看索引信息
show index from student;
# 28.解释mysql如何使用索引来处理select语句及连接表的
explain select * from student;
# 29.索引的分类
索引可以分为四种类型：主键、唯一索引、全文索引、普通索引
主键：唯一且没有null值。
唯一索引：除了不能有重复的记录外，其它和普通索引一样。
普通索引：允许重复的值出现。
全文索引：用来对大表的文本域(char，varchar，text)进行索引。
# 30.如何重置自动增加计数器的值：
create table tablename(fieldname int auto_increment,%5
