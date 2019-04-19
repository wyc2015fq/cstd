# MySQL主主复制3 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月06日 17:25:19[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：244
一、创建并授权用户
在每一台（主）服务器上创建一个用户，并为之授权，使它们可以互相访问彼此的数据库
在Server-1上：
创建一个充许Server-2来访问的用户server2,密码为：server2
mysql> GRANT REPLICATION SLAVE ON *.*
> TO ‘server2’@’192.168.0.2’ IDENTIFIED BY ‘server2’;
在Server-2上：
创建一个充许Server-1来访问的用户server1,密码为：server1
mysql> GRANT REPLICATION SLAVE ON *.*
> TO ‘server1’@’192.168.0.1’ IDENTIFIED BY ‘server1’;
二、修改MySQL主配置文件
在MySQL的主配置文件中修改/添加如下内容：
Server-1上：
[mysqld]
server-id = 10
log-bin = mysql-bin
replicate-do-db = mydb
auto-increment-increment = 2
auto-increment-offset = 1
# service mysqld restart
Server-2上：
[mysqld]
server-id = 20
log-bin = mysql-bin
replicate-do-db = mydb
auto-increment-increment = 2
auto-increment-offset = 2
# service mysqld restart
注：二者只有server-id不同和 auto-increment- offset不同
auto-increment-offset是用来设定数据库中自动增长的起点的，因为这两台服务器都设定了同一个自动增长值2，所以它们的起点必须得不同，这样才能避免两台服务器数据同步时出现主键冲突
replicate-do-db 指定同步的数据库，我们只在两台服务器间同步mydb数据库
另：auto-increment-increment的值应设为整个结构中服务器的总数，本案例用到两台服务器，所以值设为2
三、复制其中一台服务器的数据库到另外一台服务器
这一步中谁做为源数据不重要，重要的是在正式的复制（Replication)开始之前两台服务都能准确的反应彼此的数据。
我们可以先从源数据库中导出数据到备份文件，这里我们使用mysqldump命令
以Server-1上数据库为源数据库
备份数据前先锁表，保证数据一致性
mysql> FLUSH TABLES WITH READ LOCK;
> SHOW MASTER STATUS;
+—————–+————+—————-+——————–+
|File | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+—————–+————+—————-+——————–+
|mysql-bin.000006 | 213 | | |
+—————–+————+—————-+——————–+
该结果显示，源服务器现在处于6号二进制文件的213位置
开始备份数据库
# mysqldump –user=root -p mydb > /tmp/mydb.sql
备份完毕，现在可以解锁数据库表
mysql> UNLOCK TABLES;
Query OK, 0 rows affected (0.00 sec)
四、将备份数据导入Server-2
先在Server-2上创建一个与mydb同名的空数据库
# mysql
> CREATE DATABASE mydb;
>\q
# scp 192.168.0.1:/tmp/mydb.sql ./
# mysql -uroot -p mydb < /tmp/mydb.sql
此时，我们需要注意当前服务器的二进制日志的位置
# mysql
> LOCK TALBES WITH READ LOCK;
> SHOW MASTER STATUS;
+—————–+————+—————-+——————–+
|File | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+—————–+————+—————-+——————–+
|mysql-bin.000001 | 106 | | |
+—————–+————+—————-+——————–+
1 row in set (0.00 sec)
> UNLOCK TABLES;
五、互相通告二进制日志位置
在Server-1上：
# mysql
> CHANGE MASTER TO
> MASTER_HOST=’192.168.0.2′,
> MASTER_USER=’server1′, //???请读者判断下这个地方是不是写错了？？
> MASTER_PASSWORD=’server1′,
> MASTER_LOG_FILE=’mysql-bin.000001′,
> MASTER_LOG_POS=106;
在Server-2上：
# mysql
> CHANGE MASTER TO
> MASTER_HOST=’192.168.0.1′,
> MASTER_USER=’server2′,
> MASTER_PASSWORD=’server2′,
> MASTER_LOG_FILE=’mysql-bin.000006′,
> MASTER_LOG_POS=213;
六、启动复制（Replication）功能
在两台主机上分别执行
# mysql
> START SLAVE;
配置到此完成！
[http://www.178-go.com/archives/mysql-master-master.html](http://www.178-go.com/archives/mysql-master-master.html)
