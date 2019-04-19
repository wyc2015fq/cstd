# MySQL主从复制（Master-Slave）与读写分离（MySQL-Proxy）实践 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月18日 16:36:30[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：365
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
**主服务器上（注：应该是允许从机访问）**
 GRANT REPLICATION SLAVE ON *.* to ‘rep1’@’192.168.10.131’ identified by ‘password’;
**从服务器**
将 server-id = 1修改为 server-id = 10，并确保这个ID没有被别的MySQL服务所使用。
执行同步SQL语句
mysql> change master to
master_host=’192.168.10.130’,
master_user=’rep1’,
master_password=’password’,
master_log_file=’mysql-bin.000005’,
master_log_pos=261;
正确执行后启动Slave同步进程
mysql> start slave;
**如果主服务器已经存在应用数据，则在进行主从复制时，需要做以下处理：**
(1)主数据库进行锁表操作，不让数据再进行写入动作
mysql> FLUSH TABLES WITH READ LOCK;
(2)查看主数据库状态
mysql> show master status;
(3)记录下 FILE 及 Position 的值。
将主服务器的数据文件（整个/opt/mysql/data目录）复制到从服务器，建议通过tar归档压缩后再传到从服务器解压。
编者注：或者也可以导出导入吧
(4)取消主数据库锁定
mysql> UNLOCK TABLES;

接下来，我们进行MySQL读写分离的安装与配置。
略
MySQL-Proxy实际上非常不稳定，在高并发或有错误连接的情况下，进程很容易自动关闭，因此打开--keepalive参数让进程自动恢复是个比较好的办法，但还是不能从根本上解决问题，因此通常最稳妥的做法是在每个从服务器上安装一个MySQL-Proxy供自身使用，虽然比较低效但却能保证稳定性；
Amoeba for MySQL 是一款优秀的中间件软件，同样可以实现读写分离，负载均衡等功能，并且稳定性要大大超过MySQL-Proxy，建议大家用来替代MySQL-Proxy，甚至MySQL-Cluster。

[http://heylinux.com/archives/1004.html](http://heylinux.com/archives/1004.html)
