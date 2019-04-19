# 命令行导入sql - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2013年06月07日 11:12:25[baofenny](https://me.csdn.net/jiaminbao)阅读数：1630
我在这里总结一下在Windows下关于数据库导入、导出文件，希望各位不吝赐教！
一.MYSQL的命令行模式的设置：
桌面->我的电脑->属性->环境变量->新建->
PATH=“；path\mysql\bin;”其中path为MYSQL的安装路径。
二.简单的介绍一下命令行进入MYSQL的方法：
1.C:\>mysql -h hostname -u username -p 
按ENTER键，等待然后输入密码。这里hostname为服务器的名称，如localhost，username为MYSQL的用户名，如root。
进入命令行后可以直接操作MYSQL了。
2.简单介绍一下MYSQL命令：
   mysql->CREATE DATABASE dbname;//创建数据库
   mysql->CREATE TABLE tablename;//创建表
   mysql->SHOW DATABASES;//显示数据库信息，有那些可用的数据库。
  mysql->USE dbname;//选择数据库
   mysql->SHOW TABLES;//显示表信息，有那些可用的表
   mysql->DESCRIBE tablename;//显示创建的表的信息
三.从数据库导出数据库文件：
1.将数据库mydb导出到e:\mysql\mydb.sql文件中：
打开开始->运行->输入cmd    进入命令行模式
c:\>mysqldump -h localhost -u root -p mydb >e:\mysql\mydb.sql
然后输入密码，等待一会导出就成功了，可以到目标文件中检查是否成功。
2.将数据库mydb中的mytable导出到e:\mysql\mytable.sql文件中：
c:\>mysqldump -h localhost -u root -p mydb mytable>e:\mysql\mytable.sql 
3.将数据库mydb的结构导出到e:\mysql\mydb_stru.sql文件中：
c:\>mysqldump -h localhost -u root -p mydb --add-drop-table >e:\mysql\mydb_stru.sql
四.从外部文件导入数据到数据库中：
从e:\mysql\mydb2.sql中将文件中的SQL语句导入数据库中：
1.从命令行进入mysql，然后用命令CREATE DATABASE mydb2;创建数据库mydb2。
2.退出mysql 可以输入命令exit；或者quit；
3.在CMD中输入下列命令：
c:\>mysql -h localhost -u root -p mydb2 < e:\mysql\mydb2.sql
然后输入密码，就OK了。
五.下面谈一下关于导入文件大小限制问题的解决：
默认情况下：mysql 对导入文件大小有限制的，最大为2M，所以当文件很大时候，直接无法导入，下面就这个问题的解决列举如下：
1.在php.ini中修改相关参数：
影响mysql导入文件大小的参数有三个：
    memory_limit=128M,upload_max_filesize=2M,post_max_size=8M
修改upload_max_filesize=200 M  这里修改满足你需要的大小，
可以同时修改其他两项memory_limit=250M  post_max_size=200M
这样就可以导入200M以下的.sql文件了。
