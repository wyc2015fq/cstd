# Mysql主从复制、多主多从配置 - 零度的博客专栏 - CSDN博客
2017年05月26日 10:22:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：1146
目录:
1. [MySQL](http://lib.csdn.net/base/14)复制原理
2. mysql一主多从
3. mysql 双主
4. mysql 读写分离
5. mysql + keepalived高可用
6. mysql 监控
7. mysql 参数详解
8. mysql 监控
9. mysql replication 常见错误及处理方法
实验环境说明 ：
10.0.0.25 nameserver.baby.local监控主机
10.0.0.101nd1.baby.localmysql master
10.0.0.102nd2.baby.localmysql master
10.0.0.103nd3.baby.localmysql slave
10.0.0.104nd4.baby.localmysql slave
# mysql复制原理
### 一. 同步复制原理
mysql的复制(replication)是异步复制,即从一个mysql实列或端口(称之为Master)复制到另一个mysql实列的或端口(称之为slave);复制操作由3个进程完成,其中2个(SQL进程和I/O进程)在Slave上,另一个在Master(binlog dump)上;
要实现复制,必须打开Master端的二进制日志(log-bin),log-bin记录着整个数据对的操作信息,所有slave从master端获取该更新的日志,将其传送到本地并写到本地文件中,然后在读取本地文件内容执行日志中记录的更新操作;
slave上已经完整拷贝master数据后,就可以连接到master上然后等待处理更新了.如果master当机或者slave连接断开,slave会定期尝试连接到master上直到能重连并且等待更新.重试的时间间隔由--master-connect-retry选项来控制,它的默认值是60秒.
每个slave都记录了它关闭时的日志位置.msater是不知道有多少个slave连接上来或者哪个slave从什么时候开始更新.
注：启用同步后,所有要同步的更新操作都必须在master上执行.否则,必须注意不要造成用户在master上的更新和在slave上的更新引起冲突.
### 二. 同步实现步骤
①　MySQL同步功能由3个线程(master上1个,slave上2个)来实现.执行START SLAVE语句后,SLAVE就创建一个I/O线程.
②　I/O线程连接到master上,并请求master发送二进制日志中的语句.
③　master创建一个线程来把日志的内容发送到slave上.这个线程在master上执行SHOW PROCESSLIST 语句后的结果中的 Binlog Dump 线程便是.
④　slave上的I/O线程读取master的 Binlog Dump 线程发送的语句,并且把它们拷贝到其数据目录下的中继日志(relay logs)中.
⑤　第三个是SQL线程,salve用它来读取中继日志,然后执行它们来更新数据.
每个master有多个线程,他为每个slave连接都创建一个线程,可以用 > show processlist 查看; 而每个slave只有I/O和SQL线程.
slave有2个线程的好处：把读日志和执行分开成2个独立的任务.执行任务如果慢的话,读日志任务不会跟着慢下来.
如：slave停止了一段时间,那么I/O线程可以在slave启动后很快地从master上读取全部日志,尽管SQL线程可能落后I/O线程好几的小时.如果slave在SQL线程没全部执行完就停止了,但I/O线程却已经把所有的更新日志都读取并且保存在本地的中继日志中了,因此在slave再次启动后就会继续执行它们了.这就允许在master上清除二进制日志,因为slave已经无需去master读取更新日志了.
# mysql一主多从
一. 实验环境说明
10.0.0.101nd1.baby.localmysql master
10.0.0.102nd2.baby.localmysql slave
10.0.0.103nd3.baby.localmysql slave
1. 建立主从复制的帐号,为什么不把账户的地址写成”%”,这样是为了安全着想,那个账户只能在指定的从服务器上使用;
mysql> GRANT REPLICATION SLAVE ON *.* TO 'repl103'@'10.0.0.103' IDENTIFIED BY 'yangcan';    
mysql> GRANT REPLICATION SLAVE ON *.* TO 'repl102'@'10.0.0.102' IDENTIFIED BY 'yangcan';  
mysql> FLUSH PRIVILEGES;
2. 备份已有数据,并把数据复制到各从节点上;如果是空的数据,就跳过此步;
mysql> FLUSH TABLES WITH READ LOCK;
mysql> SHOW MASTER STATUS;
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000001 |      106 | test         |                  |
+------------------+----------+--------------+------------------+
[root@nd1 ~]# tar zcvf /tmp/mysql_fullbak.tar.gz /var/lib/mysql/
[root@nd1 ~]# rsync -zrvz --delete /tmp/mysql_fullbak.tar.gz nd2.baby.local:/tmp/
[root@nd1 ~]# rsync -zrvz --delete /tmp/mysql_fullbak.tar.gz nd3.baby.local:/tmp/
3. 解锁读
mysql> UNLOCK TABLES;
Query OK, 0 rows affected (0.00 sec)
4. 配置主服务器:修改mysql master 10.0.0.101 的主配文件;
[root@nd1 ~]# vi /etc/my.cnf 
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
#### Master ####
server-id                = 1
log-bin                  = mysql-bin
log-bin-index           = mysql-bin.index
relay-log               = mysql-relay
relay-log-index        = mysql-relay.index
expire-logs-days        = 10
max-binlog-size         = 100M
log-slave-updates       = 1
binlog-do-db            = test
replicate-do-db         = test
binlog-ignore-db        = mysql
replicate-ignore-db     = mysql
# 参数解释
server-id=1表示是本机的序号为1,一般来讲就是master的意思
log-bin表示打开binlog,打开该选项才可以通过I/O写到Slave的relay-log,也是可以进行replication的前提
binlog-do-db=test表示需要备份的数据库是test这个数据库
log-slave-updates= 1从前一台机器上同步过来的数据才能同步到下一台机器 
replicate-do-db=test 表示同步test数据库
如果需要备份多个数据库,那么应该写多行
binlog-do-db=test1
binlog-do-db=test2
replicate-do-db=test1
replicate-do-db=test2
# 重启服务
[root@nd1 ~]# /etc/init.d/mysqld restart
5. 修改从服务器: 10.0.0.102 和 10.0.0.103 的配置文件,添加server-id = 2 或者 3;并分别重启服务;
10.0.0.102:
[root[@nd](http://my.oschina.net/dangwho)3 ~]# vi /etc/my.cnf 
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
server-id               = 2
10.0.0.103:
[root[@nd](http://my.oschina.net/dangwho)3 ~]# vi /etc/my.cnf 
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
server-id               = 3
6. 在102 和 103上配置同步sql语句;这些语句也可以写在配置文件中,方面维护;
mysql> CHANGE MASTER TO
    ->   MASTER_HOST='10.0.0.101',
    ->   MASTER_USER='repl102',# 在103 上面改成repl103
    ->   MASTER_PASSWORD='yangcan',
    ->   MASTER_PORT=3306,
    ->   MASTER_LOG_FILE='mysql-bin.000001',
    ->   MASTER_LOG_POS=106,
    ->   MASTER_CONNECT_RETRY=10;
Query OK, 0 rows affected (0.02 sec)
mysql> START SLAVE;
Query OK, 0 rows affected (0.00 sec)
mysql> SHOW SLAVE STATUS\G;
.......
             Slave_IO_Running: Yes
            Slave_SQL_Running: Yes
7. 查看主的状态
mysql> show master status;
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000001 |      106 | test         |                  |
+------------------+----------+--------------+------------------+
1 row in set (0.00 sec)
# 主服务器上开启了2个Binlog Dump进程,
mysql> SHOW PROCESSLIST;
+----+---------+----------------------+-------+-------------+------+----------------------------------------------------------------+------------------+
| Id | User    | Host                 | db    | Command     | Time | State                                                          | Info             |
+----+---------+----------------------+-------+-------------+------+----------------------------------------------------------------+------------------+
|  3 | root    | localhost            | mysql | Query       |    0 | NULL                                                           | SHOW PROCESSLIST |
| 34 | repl102 | nd2:57981            | NULL  | Binlog Dump | 1428 | Has sent all binlog to slave; waiting for binlog to be updated | NULL             |
| 71 | repl103 | nd3.baby.local:37984 | NULL  | Binlog Dump | 1043 | Has sent all binlog to slave; waiting for binlog to be updated | NULL             |
+----+---------+----------------------+-------+-------------+------+----------------------------------------------------------------+------------------+
3 rows in set (0.00 sec)
8. 测试环节,在主10.0.0.101 上test数据库上新建,删除数据,然后在从上同步查看,是否和主同步;
a). 在主上操作:
mysql> create table t1(id int);
mysql> show tables;
+----------------+
| Tables_in_test |
+----------------+
| t1             |
+----------------+
mysql> insert into t1 values(1);
b). 在从上查看:
mysql> select * from test.t1;
+------+
| id   |
+------+
|    1 |
+------+
c). 当在主上新建其他数据库时,在从上并不能查看得到,因为更新只设置了test数据库,
mysql双主
### 一.实验环境说明
10.0.0.101nd1.baby.localmasterA
10.0.0.102nd2.baby.localmasterB
10.0.0.103nd3.baby.lcoalslave
1. 和一主多从的步骤是一样的,只是同样的步骤执行2次而已;
2. mysql服务必须监听0.0.0.0 
### 二. 双主的配置
1. 在masterA上新建一个账户,用户masterB同步数据
masterA:
mysql> GRANT REPLICATION SLAVE ON *.* TO 'repl102'@'10.0.0.%' IDENTIFIED BY 'yangcan';   
mysql> FLUSH PRIVILEGES;
mysql> FLUSH TABLES WITH READ LOCK;
[root@nd1 ~]# mysqldump -uroot -pyangcan --databases test >/tmp/testA.sql
[root@nd1 ~]# scp /tmp/testA.sql 10.0.0.104:/tmp
mysql> UNLOCK TABLES;
Query OK, 0 rows affected (0.00 sec)
masterB:
上同样的建立复制到账户,并导入数据;
mysql> GRANT REPLICATION SLAVE ON *.* TO 'repl101'@'10.0.0.%' IDENTIFIED BY 'yangcan';                  
mysql> FLUSH TABLES WITH READ LOCK;
[root@varnish ~]# mysqldump -uroot -pyangcan --databases test >/tmp/testB.sql
[root@varnish ~]# scp /tmp/testB.sql 10.0.0.101:/tmp/
# 分别在masterA 和 masterB上分别导入对方的数据.但是有一个问题在这里提出: 因为存在主键冲突的情况,导出数据的时候,不要把主键给导出来了;如果是一方没有数据,那就直接导入数据就好了,不过也要跳过主键;
[root@nd2 ~]# mysql -uroot -pyangcan </tmp/testA.sql 
[root@nd1 ~]# mysql -uroot -pyangcan </tmp/testB.sql 
2. 修改masterA和masterB 的配置文件 ,只是server-id 不同而已.
masterA:的配置文件
[root@nd1 ~]# vi /etc/my.cnf
[mysqld]
datadir=/var/lib/mysql
#datadir=/mysqldata
socket=/var/lib/mysql/mysql.sock
user=mysql
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
#### Master ####
server-id               = 1
log-bin                 = mysql-bin
log-bin-index           = mysql-bin.index
relay-log               = mysql-relay
relay-log-index         = mysql-relay.index
expire-logs-days        = 10
max-binlog-size         = 100M
log-slave-updates       = 1
binlog-do-db            = test
replicate-do-db         = test
binlog-ignore-db        = mysql
replicate-ignore-db     = mysql
sync-binlog             = 1
auto-increment-increment = 2
auto-increment-offset   = 1
# masterB:的配置文件
[root@nd2 ~]# vi /etc/my.cnf 
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
        ### Master ####
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
symbolic-links=0
server-id               = 2
log-bin                 = mysql-bin
log-bin-index           = mysql-bin.index
relay-log               = mysql-relay
relay-log-index         = mysql-relay.index
expire-logs-days        = 10
max-binlog-size         = 100M
log-slave-updates
skip-slave-start
slave-skip-errors       = all
binlog-do-db            = test
replicate-do-db         = test
binlog-ignore-db        = mysql
replicate-ignore-db     = mysql
sync-binlog            = 1
auto-increment-increment = 2
auto-increment-offset   = 2
3. 分别在masterA 和masterB上获取File 和 Position 位置.
masterA :
mysql> FLUSH TABLES WITH READ LOCK;
mysql> SHOW MASTER STATUS;         
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000009 |      106 | test         | mysql            |
+------------------+----------+--------------+------------------+
mysql> UNLOCK TABLES;
masterB :
mysql> FLUSH TABLES WITH READ LOCK;
mysql> SHOW MASTER STATUS;
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000001 |      106 | test         | mysql            |
+------------------+----------+--------------+------------------+
mysql> UNLOCK TABLES;
4. 配置主从,分别在masterA 和 masterB上配置对方的从;
a). 在masterB 10.0.0.102上配置从;配置之前需保存数据的一致性;
mysql> CHANGE MASTER TO   MASTER_HOST='10.0.0.101',   MASTER_USER='repl102',   MASTER_PASSWORD='yangcan',   MASTER_PORT=3306,   MASTER_LOG_FILE='mysql-bin.000009',   MASTER_LOG_POS=106,   MASTER_CONNECT_RETRY=10;
mysql> START SLAVE;
b). masterA上配置从,主为masterB;
mysql> CHANGE MASTER TO   MASTER_HOST='10.0.0.102',   MASTER_USER='repl101',   MASTER_PASSWORD='yangcan',   MASTER_PORT=3306,   MASTER_LOG_FILE='mysql-bin.000009',   MASTER_LOG_POS=106,   MASTER_CONNECT_RETRY=10;
mysql> START SLAVE;
5. 查看状态, 其中第1 和第2 上是此服务器做为从开始的2个线程;第3的Binlog Dump线程数做为主提供给从的发生binlog的线程;
mysql> SHOW PROCESSLIST\G;
*************************** 1. row ***************************
     Id: 3
   User: system user
   Host: 
     db: NULL
Command: Connect
   Time: 3963
  State: Waiting for master to send event
   Info: NULL
*************************** 2. row ***************************
     Id: 4
   User: system user
   Host: 
     db: NULL
Command: Connect
   Time: 2642
  State: Has read all relay log; waiting for the slave I/O thread to update it
   Info: NULL
*************************** 3. row ***************************
     Id: 5
   User: repl102
   Host: nd2:49309
     db: NULL
Command: Binlog Dump
   Time: 2898
  State: Has sent all binlog to slave; waiting for binlog to be updated
   Info: NULL
*************************** 4. row ***************************
     Id: 6
   User: root
   Host: localhost
     db: NULL
Command: Query
   Time: 0
  State: NULL
   Info: SHOW PROCESSLIST
4 rows in set (0.00 sec)
6. 测试,在其中的一个数据库test数据中建立一个表,插入数据
mysql> SHOW CREATE TABLE t3\G;
*************************** 1. row ***************************
       Table: t3
Create Table: CREATE TABLE `t3` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=latin1
1 row in set (0.00 sec)
mysql> SELECT * FROM t3;
+----+---------------+
| ID | name          |
+----+---------------+
|  1 | this is 101   |
|  3 | this is 101.1 |
|  4 | this is 102   |
|  6 | this is 102.1 |
+----+---------------+
### 错误问题
1. 当在masterA上配置从正确,但在10.0.0.102 masterB上配置从时出现错误,可能是之前这2台机器已经配置过主从的信息,扰乱了实验;配置文件又没有错误,但是提示:
mysql> CHANGE MASTER TO  MASTER_HOST='10.0.0.101',  MASTER_USER='repl',  MASTER_PASSWORD='yangcan',  MASTER_PORT=3306,  MASTER_LOG_FILE=' masterA-bin.000005',  MASTER_LOG_POS=106,  MASTER_CONNECT_RETRY=10;                 
ERROR 1201 (HY000): Could not initialize master info structure; more error messages can be found in the MySQL error log
# 日志的消息,在启动mysqld服务的时候,也提示错误信息!
tail -f /var/log/mysqld
130210 23:34:52 [ERROR] Failed to open the relay log './mysqld-relay-bin.000032' (relay_log_pos 251)
130210 23:34:52 [ERROR] Could not find target log during relay log initialization
130210 23:34:52 [ERROR] Failed to initialize the master info structure
130210 23:34:52 [Note] Event Scheduler: Loaded 0 events
130210 23:34:52 [Note] /usr/libexec/mysqld: ready for connections.
nd2: '5.1.61-log'  socket: '/var/lib/mysql/mysql.sock'  port: 3306  Source distribution
130210 23:35:19 [ERROR] Failed to open the relay log './mysqld-relay-bin.000032' (relay_log_pos 251)
130210 23:35:19 [ERROR] Could not find target log during relay log initialization
解决的步骤:
在masterB上执行 mysql> RESET SLAVE;
或者 
mysql> CHANGE MASTER TO
MASTER_LOG_FILE=’<Relay_Master_Log_File>’,
MASTER_LOG_POS=<Exec_Master_Log_Pos>;
mysql> START SLAVE;
2. 解决了步骤1的错误之后,又提示 “ [ERROR] Error reading master configuration “ 错误,处理的办法也是执行”  RESET SLAVE; ”
130211 18:42:24 mysqld_safe Starting mysqld daemon with databases from /var/lib/mysql
130211 18:42:25  InnoDB: Initializing buffer pool, size = 8.0M
130211 18:42:25  InnoDB: Completed initialization of buffer pool
130211 18:42:25  InnoDB: Started; log sequence number 0 44233
130211 18:42:25 [ERROR] Error reading master configuration
130211 18:42:25 [ERROR] Failed to initialize the master info structure
130211 18:42:25 [Note] Event Scheduler: Loaded 0 events
130211 18:42:25 [Note] /usr/libexec/mysqld: ready for connections.
Version: '5.1.61-log'  socket: '/var/lib/mysql/mysql.sock'  port: 3306  Source distribution
130211 18:43:52 [ERROR] Error reading master configuration
130211 18:44:12 [ERROR] Error reading master configuration
3. 获取帮助:
mysql> HELP MASTER;
   CHANGE MASTER TO
   LOAD DATA FROM MASTER
   LOAD TABLE FROM MASTER
   PURGE BINARY LOGS
   RESET MASTER
   SHOW BINARY LOGS
   SHOW MASTER STATUS
mysql> HELP SLAVE;
   ALTER EVENT
   CREATE EVENT
   RESET SLAVE
   SHOW SLAVE HOSTS
   SHOW SLAVE STATUS
   START SLAVE
   STOP SLAVE
# 可以在开始SLAVE进程的时候,指定master_log_file  和 master_log_pos的位置
mysql> HELP START;
START SLAVE [thread_type [, thread_type] ... ]
START SLAVE [SQL_THREAD] UNTIL
    MASTER_LOG_FILE = 'log_name', MASTER_LOG_POS = log_pos
START SLAVE [SQL_THREAD] UNTIL
    RELAY_LOG_FILE = 'log_name', RELAY_LOG_POS = log_pos
thread_type: IO_THREAD | SQL_THREAD
flush logs# 从当前时间新创建一个binlog日志
show master status#
show slave status#
reset master# 清空所有的bin-log日志
mysqlbinlog --no-defaults /var/lib/mysql/mysql-bin.0000000001
reset slave# 重置slave,删除master.info 和 relay-log.info 以及所有的relay log文件;使用之前,必须先停止从STOP SLAVE;
FLUSH TABLES WITH READ LOCK#
# 在开启binlog日志备份数据的时候,加上-l (读锁)和-F参数,就重新开启一个新的binlog日志文件里,便于以后的数据恢复!!如果同时备份多个数据库带了”--databases 或者 --all-databases”参数,需要配和使用”--lock-all-tables 或者 --all-databases”参数,那样只刷新一个日志了;
  mysqldump -uroot -pyangcan -F  -l --all-databases --master-data >/tmp/test.sql 
 mysql-mmm
写在最前面:
1. 此实验室在mysql双主的基础上做到,因为双主做的时候,复制都是用的单个的账户,没有统一,这里复制全部用’repl’@’10.0.0.%’这账户;这里把masterB同步的账户信息改变,masterA上同样的操作,只是把红色部分字体换过来就好了.
>>在masterA上新建新的同步账户
mysql> GRANT REPLICATION SLAVE ON *.* TO 'repl'@'10.0.0.%' IDENTIFIED BY 'yangcan';
mysql> FLUSH PRIVILEGES;
>>在masterB上更改同步到账户:
mysql> STOP SLAVE; 
mysql> CHANGE MASTER TO   MASTER_HOST='10.0.0.101',   MASTER_USER='repl',   MASTER_PASSWORD='yangcan';
2. 增加2台slave,nd3分别对应nd1, nd4分别对应nd2;因为都是新库,并没有做数据的同步操作,所以数据同步到操作略过,这里就只演示nd3的的同步过程,nd4的同步过程相仿,只是修改下server-id=4即可;
a). 修改配置文件
[root@nd3 ~]# vi /etc/my.cnf 
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
symbolic-links=0
default-character-set=utf8
server-id               = 3
log-bin                 = mysql-bin
log-bin-index           = mysql-bin.index
relay-log               = mysql-relay
relay-log-index         = mysql-relay.index
b). 在masterA上操作:获取主nd1的状态,灰色部分是一个数据备份的过程,如果是新库,就可以跳过,只需获取到主的状态就可以了;
mysql> FLUSH TABLES WITH READ LOCK;
mysql> SHOW MASTER STATUS;
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000001 |     1924 | test         | mysql            |
+------------------+----------+--------------+------------------+
[root@nd1 ~]# mysqldump -uroot -pyangcan --database test >/tmp/testbak.sql
mysql> UNLOCK TABLES;
[root@nd1 ~]# scp /tmp/testbak.sql 10.0.0.103:/tmp/
c). 在从执行sql语句同步.
[root@nd3 ~]# mysql -uroot -pyangcan < /tmp/testbak.sql 
mysql> CHANGE MASTER TO   MASTER_HOST='10.0.0.101',   MASTER_USER='repl',   MASTER_PASSWORD='yangcan',   MASTER_PORT=3306,   MASTER_LOG_FILE='mysql-bin.000001',   MASTER_LOG_POS=1924,   MASTER_CONNECT_RETRY=10;
mysql> START SLAVE;
mysql> SHOW SLAVE STATUS\G;
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 10.0.0.101
                  Master_User: repl
                  Master_Port: 3306
                Connect_Retry: 10
              Master_Log_File: mysql-bin.000001
          Read_Master_Log_Pos: 1924
               Relay_Log_File: mysql-relay.000002
                Relay_Log_Pos: 251
        Relay_Master_Log_File: mysql-bin.000001
             Slave_IO_Running: Yes
            Slave_SQL_Running: Yes
3. 其实在这一步,多主多从已经搭建成功了;
一. 实验环境说明
参考资料:
[http://mysql-mmm.org/mmm2:guide](http://mysql-mmm.org/mmm2:guide)
主机分布:
|function|ip|hostname|server id|
|----|----|----|----|
|monitoring host|10.0.0.25|nameserver|-|
|master A|10.0.0.101|nd1|1|
|master B|10.0.0.102|nd2|2|
|slave 1|10.0.0.103|nd3|3|
|slave 2|10.0.0.104|nd4|4|
IP分布:
|ip|role|description|
|----|----|----|
|10.0.0.50|writer|Your application should connect to this IP for write queries.|
|10.0.0.51|reader|Your application should connect to one of these four IPs for read queries|
|10.0.0.52|reader| |
|10.0.0.53|reader| |
|10.0.0.54|reader| |
创建用户:
|function|description|privileges|accounts status|
|----|----|----|----|
|monitor user|被用作mmm monitor 去检查 MySQL servers的状态|REPLICATION CLIENT|all databases(nd1..nd4)|
|GRANT REPLICATION CLIENT  ON *.* TO 'mmm_monitor'@'10.0.0.%' IDENTIFIED BY 'yangcan';| | | |
|agent user|被用作mmm agent 去改变read-only模式,replication master, etc.|SUPER, REPLICATION CLIENT, PROCESS|all databases(nd1..nd4)|
|GRANT SUPER, REPLICATION CLIENT, PROCESS ON *.* TO 'mmm_agent'@'10.0.0.%'   IDENTIFIED BY 'yangcan';| | | |
|replication user|used for replication|REPLICATION SLAVE|master databases(nd1,nd2)|
|GRANT REPLICATION SLAVE  ON *.* TO 'repl'@'10.0.0.%' IDENTIFIED BY 'yangcan'| | | |
注: replication的密码不能超过32个字符;
一. 安装mmm
采用的是rpm包安装,先配置安装源,把enabled=0 改成 enabled=1;
# rpm -ivh [http://mirrors.ustc.edu.cn/fedora/epel/6/i386/epel-release-6-8.noarch.rpm](http://mirrors.ustc.edu.cn/fedora/epel/6/i386/epel-release-6-8.noarch.rpm)
[root@nameserver ~]# yum list|grep mysql-mmm
mysql-mmm.noarch                        2.2.1-1.el6                 @/mysql-mmm-2.2.1-1.el6-i386.noarch
mysql-mmm-monitor.noarch                2.2.1-1.el6                 @epel       
mysql-mmm-agent.noarch                  2.2.1-1.el6                 epel        
mysql-mmm-tools.noarch                  2.2.1-1.el6                 epel  
# 监控主机(10.0.0.25)上安装mysql-mmm-monitor
[root@nameserver ~]# yum install mysql-mmm-tools  mysql-mmm-monitor -y
# 4台数据库服务器上安装mysql-mmm-agent
[root@nd2 ~]# yum install mysql-mmm-agent mysql-mmm-tools -y
# 二. 配置
1.  4台数据库主机的 mmm_common.conf 下面的配置文件是其中的一台数据库的配置文件,其他数据库的mmm_common.conf 配置文是一样的
[root@nd1 ~]# cat /etc/mysql-mmm/mmm_common.conf 
active_master_role      writer
<host default>
    cluster_interface       eth1
    pid_path                /var/run/mysql-mmm/mmm_agentd.pid
    bin_path                /usr/libexec/mysql-mmm/
    replication_user        repl
    replication_password    yangcan
    agent_user              mmm_agent
    agent_password          yangcan
</host>
<host db1>
    ip      10.0.0.101
    mode    master
    peer    db2
</host>
<host db2>
    ip      10.0.0.102
    mode    master
    peer    db1
</host>
<host db3>
    ip      10.0.0.103
    mode    slave
</host>
<host db4>
    ip      10.0.0.104
    mode    slave
</host>
<role writer>
    hosts   db1, db2
    ips     10.0.0.50
    mode    exclusive
</role>
<role reader>
    hosts   db1, db2, db3, db4
    ips     10.0.0.51, 10.0.0.52, 10.0.0.53, 10.0.0.54
    mode    balanced
</role>
# 把mmm_common.conf配置文件复制到其他数据库主机中
[root@nd1 ~]# rsync -arvz /etc/mysql-mmm/mmm_common.conf 10.0.0.102:/etc/mysql-mmm/
[root@nd1 ~]# rsync -arvz /etc/mysql-mmm/mmm_common.conf 10.0.0.103:/etc/mysql-mmm/
[root@nd1 ~]# rsync -arvz /etc/mysql-mmm/mmm_common.conf 10.0.0.104:/etc/mysql-mmm/
[root@nd1 ~]# rsync -arvz /etc/mysql-mmm/mmm_common.conf 10.0.0.105:/etc/mysql-mmm/
2. mmm_agent.conf 的配置,根据需求,修改this db1这个部分!
[root@nd1 ~]# cat /etc/mysql-mmm/mmm_agent.conf 
include mmm_common.conf
# The 'this' variable refers to this server.  Proper operation requires 
# that 'this' server (db1 by default), as well as all other servers, have the 
# proper IP addresses set in mmm_common.conf.
this db1
3. 监控主机的配置
[root@nameserver ~]# cat /etc/mysql-mmm/mmm_mon.conf 
include mmm_common.conf
<monitor>
    ip                  10.0.0.25
    pid_path            /var/run/mysql-mmm/mmm_mond.pid
    bin_path            /usr/libexec/mysql-mmm
    status_path         /var/lib/mysql-mmm/mmm_mond.status
    ping_ips            10.0.0.101, 10.0.0.102, 10.0.0.103, 10.0.0.104, 10.0.0.25
    auto_set_online     60
    # The kill_host_bin does not exist by default, though the monitor will
    # throw a warning about it missing.  See the section 5.10 "Kill Host 
    # Functionality" in the PDF documentation.
    #
    # kill_host_bin     /usr/libexec/mysql-mmm/monitor/kill_host
    #
</monitor>
<host default>
    monitor_user        mmm_monitor
    monitor_password    yangcan
</host>
debug 0
4. 修改所有mysql的配置文件,在[mysqld] 加入read-only=1选项;
[root@nd1 ~]# cat /etc/my.cnf 
[mysqld]
read_only               = 1
5. 启动mysql-mmm-agent 和 监控主机上面的mysql-mmm-monitor服务进程
[root@nd1 ~]# /etc/init.d/mysql-mmm-agent start
[root@nd1 ~]# chkconfig mysql-mmm-agent on
[root@nameserver mysql-mmm]# /etc/init.d/mysql-mmm-monitor start
[root@nameserver mysql-mmm]# chkconfig mysql-mmm-monitor on
注意事项:
1. 插入数据时,必须先进入这个数据库,不能在数据库外面开始插入
[root@nameserver ~]# mysql -uyangcan -pyangcan -h 10.0.0.50;
mysql> insert into test.tata values(500);# 插入按test.tata这样带数据库路径插入时,数据没有复制过去
mysql> use test;
mysql> insert into tata values(500);# 先进入test数据库,在插入,数据同步成功;
2. 当架构是2主多从的时候,在从上配置master的时候,指定的是master的实际ip,不是mysql-mmm浮动的IP;如果从库的主库down掉之后,mysql-mmm会自动的更新从库到另外一个正常的主库上面更新;二. 常用操作
[root@nameserver ~]# mmm_control show
  db1(10.0.0.101) master/ONLINE. Roles: reader(10.0.0.51)
  db2(10.0.0.102) master/ONLINE. Roles: reader(10.0.0.54), writer(10.0.0.50)
  db3(10.0.0.103) slave/ONLINE. Roles: reader(10.0.0.53)
  db4(10.0.0.104) slave/ONLINE. Roles: reader(10.0.0.52)
[root@nd2 ~]# /etc/init.d/mysqld stop
[root@nameserver ~]# mmm_control show
  db1(10.0.0.101) master/ONLINE. Roles: reader(10.0.0.51), reader(10.0.0.52), writer(10.0.0.50)
  db2(10.0.0.102) master/HARD_OFFLINE. Roles: 
  db3(10.0.0.103) slave/ONLINE. Roles: reader(10.0.0.53), reader(10.0.0.54)
  db4(10.0.0.104) slave/REPLICATION_FAIL. Roles: 
9. 常见错误的处理方法
9.1 主从服务器上常使用的命令
主服务器上的相关命令：
show master status
show slave hosts
show logs
SHOW BINLOG EVENTS [ IN 'logname' ] [ FROM pos ] [ LIMIT [offset,] rows ]
purge logs to ‘log_name’
purge logs before ‘date’
reset master(老版本flush master)#主机端运行,清除所有的日志,这条命令就是原来的FLUSH MASTER
SET SQL_LOG_BIN=0|1 #主机端运行,需要super权限,用来开停日志,随意开停,会造成主机从机数据不一致,造成错误
从服务器上的相关命令:
slave start
slave stop
SLAVE STOP IO_THREAD # 此线程把master段的日志写到本地
SLAVE start IO_THREAD
SLAVE STOP SQL_THREAD   # 此线程把写到本地的日志应用于数据库
SLAVE start SQL_THREAD
reset slave#从机运行,清除日志同步位置标志,并重新生成master.info;虽然重新生成了master.info,但是并不起用,最好,将从机的mysql进程重启一下,
SET GLOBAL SQL_SLAVE_SKIP_COUNTER=n 
# 客户端运行,用来跳过几个事件,只有当同步进程出现错误而停止的时候才可以执行.
LOAD TABLE tblname FROM MASTER 
#从机运行,从主机端重读指定的表的数据,每次只能读取一个,受timeout时间限制,需要调整timeout时间.执行这个命令需要同步账号有 reload和super权限.以及对相应的库有select权限.如果表比较大,要增加net_read_timeout 和 net_write_timeout的值
show slave status(SUPER,REPLICATION CLIENT)
CHANGE MASTER TO MASTER_HOST=, MASTER_PORT=,MASTER_USER=, MASTER_PASSWORD= //动态改变master信息
PURGE MASTER [before 'date'] 删除master端已同步过的日志
## 9.2 同步线程的状态
### 9.2.1 Master 同步线程状态
以下列出了master的 Binlog Dump 线程 State 字段中最常见的几种状态.如果在master上没有 Binlog Dump 线程,那么同步就没有在运行.
也就是说,没有slave连接上来.使用show processlist 查看;
Sending binlog event to slave
事件是由二进制日志构成,一个事件通常由更新语句加上其他信息.线程读取到一个事件并正发送到slave上.
Finished reading one binlog; switching to next binlog
读取完了一个二进制日志,正切换到下一个.
Has sent all binlog to slave; waiting for binlog to be updated
已经读取完全部未完成更新日志,并且全部都发送到slave了.它处于空闲状态,正等待在master上执行新的更新操作以在二进制日志中产生新
的事件,然后读取它们.
Waiting to finalize termination
当前线程停止了,这个时间很短.
### 9.2.2 Slave的I/O线程状态
以下列出了slave的I/O线程 State 字段中最常见的几种状态.从MySQL 4.1.1开始,这个状态在执行 SHOW SLAVE STATUS 语句结果的
Slave_IO_State 字段也会出现.这意味着可以只执行 SHOW SLAVE STATUS 语句就能了解到更多的信息.
Connecting to master
# 该线程证尝试连接到master上.
Checking master version
# 确定连接到master后出现的一个短暂的状态.
Registering slave on master
# 确定连接到master后出现的一个短暂的状态.
Requesting binlog dump
# 确定连接到master后出现的一个短暂的状态.该线程向master发送一个请求,告诉它要请求的二进制文件以及开始位置.
Waiting to reconnect after a failed binlog dump request
# 如果二进制日志转储(binary log dump)请求失败了(由于连接断开),该线程在休眠时进入这个状态,并定期重连.重连的时间间隔由 –
master-connect-retry 选项来指定.
Reconnecting after a failed binlog dump request
# 该线程正尝试重连到master.
Waiting for master to send event
# 已经连接到master,正等待它发送二进制日志.如果master闲置时,这个状态可能会持续较长时间,如果它等待超过 slave_read_timeout 秒
,就会发生超时.这时,它就会考虑断开连接,然后尝试重连.
Queueing master event to the relay log
# 已经读取到一个事件,正把它拷贝到中继日志中以备SQL线程处理.
Waiting to reconnect after a failed master event read
# 读日志时发生错误(由于连接断开).该线程在重连之前休眠 master-connect-retry 秒.
Reconnecting after a failed master event read
# 正尝试重连到master.当连接确定后,状态就变成 Waiting for master to send event.
Waiting for the slave SQL thread to free enough relay log space
# relay_log_space_limit 的值非零,中继日志的大小总和超过这个值了.I/O线程等待SQL线程先处理中继日志然后删除它们以释放足够的空间
.
Waiting for slave mutex on exit
# 当前线程停止了,这个时间很短.
### 9.2.3 Slave的SQL线程状态
以下列出了slave的SQL线程 State 字段中最常见的几种状态：
Reading event from the relay log
# 从中继日志里读到一个事件以备执行.
Has read all relay log; waiting for the slave I/O thread to update it
# 已经处理完中继日志中的全部事件了,正等待I/O线程写入更新的日志.
Waiting for slave mutex on exit
# 当前线程停止了,这个时间很短.
### 9.3 常见故障
1. 在从服务器上使用show slave status\G;Slave_IO_Running,为No,则说明IO_THREAD没有启动,请执行:
start slave io_thread;
Slave_SQL_Running为No.则复制出错,查看Last_error字段排除错误后执行:
start slave sql_thread;
2. 主从复制不止何故停止了,我该怎么办？
复制错误多半是因为日志错误引起的,所以首先要搞清楚是主日志错误还是中继日志错误,从错误信息里一般就能判断,如果不能可以使用类似下面的mysqlbinlog命令：
shell> mysqlbinlog <MASTER_BINLOG_FILE> > /dev/null
shell> mysqlbinlog <SLAVE_BINLOG_FILE> > /dev/null
# 如果没有错误,则不会有任何输出,反之如果有错误,则会显示出来.
如果是主日志错误,则需要在从服务器使用SET GLOBAL sql_slave_skip_counter,如下：
mysql> SET GLOBAL sql_slave_skip_counter = 1;
mysql> START SLAVE;
注：如果有多个错误,可能需要执行多次（提醒：主从服务器数据可能因此不一致）.关于这个的设置,后面有详细的讲解;
如果是中继日志错误,只要在从服务器使用SHOW SLAVE STATUS结果中的日志信息重新CHANGE MASTER TO即可,系统会抛弃当前的中继日志,重新下载：
mysql> CHANGE MASTER TO
MASTER_LOG_FILE=’<Relay_Master_Log_File>’,
MASTER_LOG_POS=<Exec_Master_Log_Pos>;
mysql> START SLAVE;
至于为什么使用的是Relay_Master_Log_File & Exec_Master_Log_Pos,参见概述.
3. 主服务器宕机了,如何把从服务器提升会主服务器？
在一主多从的环境总,需选择数据最新的从服务器做新的主服务器.在 一主（Server1）两从（Server2,、Server3）的环境中,Server1宕机后,等到Server1和Server2把宕机前同步到的 日志都执行完,比较Master_Log_File和Read_Master_Log_Pos就可以判断出谁快谁慢,因为Server2从 Server1同步的数据（1582）比Server3从Server1同步的数据（1493）新,所以应该提升Server2为新的主服务器,那么 Server3在CHANGE MASTER TO到Server2的时候应该使用什么样的参数呢？1582-1493=89,而Server2的最后的二进制日志位置是8167,所以答案是 8167-89=8078.
技巧:
主从服务器中的表可以使用不同的表类型.比如主服务器可以使用InnoDB表类型,提供事务,行锁等高级特性,从服务器可以使用MyISAM表类型,内存消耗少,易备份等优点.
还有一个例子,一台主服务器如果同时带很多个从服务器的话,势必会影响其性能,此时可以拿出一台服务器作为从服务器代理,使用 BLACKHOLE表类型,只记录日志,不写数据,由它带多台从服务器,从而提升性能.
主从服务器中的表可以使用不同的键类型.比如主服务器用InnoDB,键用VARCHAR的话节省空间,从服务器使用MyISAM,键用CHAR提高速度,因为MyISAM有静态表一说.
主从服务器中的表可以使用不同的索引.主服务器主要用来应付写操作,所以除了主键和唯一索引等保证数据关系的索引一般都可以不加,从服务器一般用来应付读操作,所以可以针对查询特征设置索引,再进一步,不同的从服务器可以针对不同的查询设置不同的索引.
###  9.4 show slave status 的参数解释
MySQL同步功能由3个线程(master上1个,slave上2个)来实现.执行 DE>START SLAVEDE> 语句后,slave就创建一个I/O线程.I/O线程连接到master上,并请求master发送二进制日志中的语句.master创建一个线程来把日 志的内容发送到slave上.这个线程在master上执行 DE>SHOW PROCESSLISTDE> 语句后的结果中的 DE>Binlog DumpDE> 线程便是.slave上的I/O线程读取master的 DE>Binlog DumpDE> 线程发送的语句,并且把它们拷贝到其数据目录下的中继日志(relay logs)中.第三个是SQL线程,salve用它来读取中继日志,然后执行它们来更新数据.
如上所述,每个master/slave上都有3个线程.每个master上有多个线程,它为每个slave连接都创建一个线程,每个slave只有I/O和SQL线程.
mysql> show slave status\G;
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 10.0.0.101
                  Master_User: repl
                  Master_Port: 3306
                Connect_Retry: 10
              Master_Log_File: mysql-bin.000007
          Read_Master_Log_Pos: 2269
               Relay_Log_File: mysql-relay.000005
                Relay_Log_Pos: 251
        Relay_Master_Log_File: mysql-bin.000007
             Slave_IO_Running: Yes
            Slave_SQL_Running: Yes
              Replicate_Do_DB: 
          Replicate_Ignore_DB: 
           Replicate_Do_Table: 
       Replicate_Ignore_Table: 
      Replicate_Wild_Do_Table: 
  Replicate_Wild_Ignore_Table: 
                   Last_Errno: 0
                   Last_Error: 
                 Skip_Counter: 0
          Exec_Master_Log_Pos: 2269
              Relay_Log_Space: 2710
              Until_Condition: None
               Until_Log_File: 
                Until_Log_Pos: 0
           Master_SSL_Allowed: No
           Master_SSL_CA_File: 
           Master_SSL_CA_Path: 
              Master_SSL_Cert: 
            Master_SSL_Cipher: 
               Master_SSL_Key: 
        Seconds_Behind_Master: 0
Master_SSL_Verify_Server_Cert: No
                Last_IO_Errno: 0
                Last_IO_Error: 
               Last_SQL_Errno: 0
               Last_SQL_Error: 
1 row in set (0.00 sec)
××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
SHOW SLAVE STATUS会返回以下字段：
Slave_IO_State
# SHOW PROCESSLIST输出的State字段的拷贝.SHOW PROCESSLIST用于从属I/O线程.如果线程正在试图连接到主服务器,正在等待来自主服务器的时间或正在连接到主服务器等,本语句会通知您
Master_User# 被用于连接主服务器的当前用户.
Master_Port# 当前的主服务器接口.
Connect_Retry# –master-connect-retry选项的当前值
Master_Log_File# I/O线程当前正在读取的主服务器二进制日志文件的名称.
Read_Master_Log_Pos# 在当前的主服务器二进制日志中,I/O线程已经读取的位置.
Relay_Log_File# SQL线程当前正在读取和执行的中继日志文件的名称.
Relay_Log_Pos# 在当前的中继日志中,SQL线程已读取和执行的位置.
Relay_Master_Log_File# 由SQL线程执行的包含多数近期事件的主服务器二进制日志文件的名称.
Slave_IO_Running# I/O线程是否被启动并成功地连接到主服务器上.
Slave_SQL_Running# SQL线程是否被启动.
Replicate_Do_DB,Replicate_Ignore_DB# 使用–replicate-do-db和–replicate-ignore-db选项指定的数据库清单.
Replicate_Do_Table
Replicate_Ignore_Table
Replicate_Wild_Do_Table
Replicate_Wild_Ignore_Table# 选项指定的表清单.
Last_Errno
Last_Error
# 被多数最近被执行的查询返回的错误数量和错误消息.错误数量为0并且消息为空字符串意味着“没有错误”.如果Last_Error值不是空值,它也会在从属服务器的错误日志中作为消息显示.
举例说明：
Last_Errno: 1051
Last_Error: Error 'Unknown table 't4'' on query. Default database: 'test'. Query: 'drop table t4'
# 该消息指示,表t4曾经存在于在主服务器中并已被取消了,但是它没有在从属服务器中存在过,因此对于从属服务器,DROP TABLE失败.（举例说明,在设置复制时,如果您忘记了把此表拷贝到从属服务器中,则这有可能发生.）
Skip_Counter# 最近被使用的用于SQL_SLAVE_SKIP_COUNTER的值.
Exec_Master_Log_Pos
# 来自主服务器的二进制日志的由SQL线程执行的上一个时间的位置（Relay_Master_Log_File）.在主服务器的二进制日志中的 (Relay_Master_Log_File, Exec_Master_Log_Pos)对应于在中继日志中的(Relay_Log_File,Relay_Log_Pos).
Relay_Log_Space# 所有原有的中继日志结合起来的总大小.
Until_Condition
Until_Log_File
Until_Log_Pos
# 在START SLAVE语句的UNTIL子句中指定的值.
Until_Condition具有以下值：
o 如果没有指定UNTIL子句,则没有值
o 如果从属服务器正在读取,直到达到主服务器的二进制日志的给定位置为止,则值为Master
o 如果从属服务器正在读取,直到达到其中继日志的给定位置为止,则值为Relay
Until_Log_File和Until_Log_Pos用于指示日志文件名和位置值.日志文件名和位置值定义了SQL线程在哪个点中止执行.
Master_SSL_Allowed,Master_SSL_CA_File,Master_SSL_CA_Path,Master_SSL_Cert,Master_SSL_Cipher,Master_SSL_Key
这些字段显示了被从属服务器使用的参数.这些参数用于连接主服务器.
Master_SSL_Allowed具有以下值：
o 如果允许对主服务器进行SSL连接,则值为Yes
o 如果不允许对主服务器进行SSL连接,则值为No
o 如果允许SSL连接,但是从属服务器没有让SSL支持被启用,则值为Ignored.
与SSL有关的字段的值对应于–master-ca,–master-capath,–master-cert,–master-cipher和–master-key选项
########################################################################
### 9.5 Replication延时的类型
1、固定性的延时
——Slave的数据持续性的落后于Master并且一直无法与Master的数据保持一致.
——Slave的数据经常在白天落后于Master,而在晚上可以赶上并与Master的记录保持一致.
这种类型的延时通常是由于Slave服务器的负载已经到达了上限或在白天访问量大的时候到达上限造成的.
2、非固定性的延时
——Slave的数据只是短暂的落后于Master,可在短时间内恢复,这类型的延时通常与批量任务和报表有关,效率差的查询也会导致这类延时.
### 9.6 Mysql Replication的限制
Mysql的Replication是单线程的,意味着只能有效的使用一个CPU内核和一个磁盘,一条复杂的查询或者事务都导致进程被阻塞,不过现在针对5.1版本的多线程Replication补丁,[http://forge.mysql.com/wiki/ReplicationFeatures/ParallelSlave](http://forge.mysql.com/wiki/ReplicationFeatures/ParallelSlave),还是pre版,有很多限制,感兴趣的可以去看看.
### 9.7 Replication的容量
1、理解什么是Replication的容量
可以将Replication暂停一个小时,重新启动Replication后,观察Slave的数据多久可以与Master一致.从 Replication重新启动到和Master数据一致所花费的时间与Replication暂停的时间的比值就是Replication的容量.
2、建议保持Replication的容量在3倍以上,即延迟一个小时的数据,Slave只需要20分钟就能与Master的数据一致.
### 9.8 Replication的优化
1. 5.0的mysql中避免类似以下的更新语句
INSERT … SELECT
UPDATE …. WHERE
复杂的查询会导致Replication线程阻塞.如果是insert或update与select结合的语句,可以讲select单独执行并保存在临时表中,然后再执行insert或者update.
如果使用的是5.1的mysql,新功能中的行级Replication（RBR）可以解决这个问题.RBR可以将在Master上通过复杂查询后更新的结果直接传给Slave,Slave可以直接将结果更新到数据库中.
2. 避免大的事务
太大的事务会造成Replication长时间阻塞,数据会严重滞后于Master.
Slave服务器的硬件选择
更快的CPU内核,对于单线程的Replication多核CPU是没有任何优势的.更高速的硬盘,包括更高的转速和更好的高速缓存命中率,如果有钱的话上SSD吧.
### 9.9 主从结构的扩展性问题
1. 如何降低写操作的频率
Master的写操作会扩散到所有的Slave上,所以高频率的写操作会降低Slave的读操作效率.
至少保持一台Slave做全库同步,其他的Slave可以只做部分表的同步.当然,这需要web应用程序的配合来分配哪些查询读哪些Slave.将 一些更新操作放到memcached中,例如session和计数器.Slave使用myisam引擎,将一些写入量很大的更新操作直接在slave上执行,而不通过Replication.
2. 如何更有效的利用Slave的硬件资源[ 变形虫]
使用分区
有选择的对表进行同步
在Slave上对数据进行归档.
Session的持久化
为不同的应用服务器分配不同的Slave进行读操作.
或者根据查询类型的不同来分配不同的Slave.
3. 如何使你的程序最大化的利用Slave
将对数据更新不敏感的查询放到Slave上,而需要实时数据的查询则放到Master.通过session的持久化,让做了修改的用户首先看到修改的内容,其他的用户可以等待Slave更新后再查看新内容.
对于某些数据,可以用memcached来存放数据的版本号,读Slave的程序可以先对比Slave的数据和memcached数据的版本,如果 不一致则去读master.用户和博客类的信息可以用这种方法.在查询前可以通过SHOW SLAVE STATUS检测Slave的状态,然后根据返回的结果进行服务器的选择.
### 9.10 减少主从复制的差距
有时候因为主服务器的更新过于频繁,造成了从服务器更新速度较慢,当然问题是多种多样,有可能是网络搭建的结构不好或者硬件的性能较差,从而使得主从服务器之间的差距越来越大,最终对某些应用产生了影响,在这种情况下,我们需要定期进行主从服务器的数据同步,具体步骤如下
在主服务器上
mysql> FLUSH TABLES WITH READ LOCK;
Query OK, 0 rows affected (0.03 sec)
mysql> show master status\G;
*************************** 1. row ***************************
File: mysql-bin.000004
Position: 102
Binlog_Do_DB:
Binlog_Ignore_DB:
1 row in set (0.00 sec)
记录出日志的名字和偏移量,这些是从服务器复制的目的目标;在从服务器上,使用MASTER_POS_WAIT()函数得到复制坐标值
mysql> select master_pos_wait('mysql-bin.000004','102');
+-------------------------------------------+
| master_pos_wait('mysql-bin.000004','102') |
+-------------------------------------------+
|                                      0                         |
+-------------------------------------------+
1 row in set (0.00 sec)
这个select 语句会阻塞直到从服务器达到指定日志文件和偏移量后,返回0,如果是-1,则表示超时退出,查询是0时,表示从服务器与主服务器已经同步
关于mysql复制时,跳过错误的处理方法
在MYSQL进行Replication的时候,有时候会由于主从端的POS点不同,导致复制的SQL进程出现错误,从而导致主从复制失败.比如在主端复制一个ID是100的到从端,而此时由于某种原因从端已经有了ID=100的记录,这时进行INSERT操作的时候,就会主键重复,插入失败.这时候需要跳过这条错误.方法如下
1. 停止SLAVE 服务
mysql> STOP SLAVE;
2. 设置跳过的EVENT个数
mysql> SET GLOBAL SQL_SLAVE_SKIP_COUNTER = n;
3. 启动SLAVE服务
mysql> START SLAVE;
# 关于n的解释
SET GLOBAL SQL_SLAVE_SKIP_COUNTER = n
This statement skips the next N events from the master
n代表跳过N个events,这里最重要的是理解event的含义, 跳过n个事务的概念.在mysql中,对于sql的 binary log 他实际上是由一连串的event组成的一个组,即事务组.
我们在master上可以通过
SHOW BINLOG EVENTS 来查看一个sql里有多少个event.
# 查看具体的binlog事务的出错点.
SHOW BINLOG EVENTS [IN 'log_name'] [FROM pos] [LIMIT [offset,] row_count]
# 具体的演示,状态是Slave_SQL_Running 为 No;出错的语句为 "drop table t4";可以通过2个方法获得'log_name' 和 'pos'
1. show slave status看到 " Slave_SQL_Running: No " 为NO, " Exec_Master_Log_Pos: 524 " 最后执行的位置
mysql> show slave status\G;
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 10.0.0.101
                  Master_User: repl
                  Master_Port: 3306
                Connect_Retry: 10
              Master_Log_File: mysql-bin.000005
          Read_Master_Log_Pos: 1162
               Relay_Log_File: mysql-relay.000002
                Relay_Log_Pos: 439
        Relay_Master_Log_File: mysql-bin.000005
             Slave_IO_Running: Yes
            Slave_SQL_Running: No
.....
                   Last_Errno: 1051
                   Last_Error: Error 'Unknown table 't4'' on query. Default database: 'test'. Query: 'drop table t4'
                 Skip_Counter: 0
          Exec_Master_Log_Pos: 524
              Relay_Log_Space: 3004
              Until_Condition: None
               Until_Log_File: 
                Until_Log_Pos: 0
.....
                Last_IO_Errno: 0
                Last_IO_Error: 
               Last_SQL_Errno: 1051
               Last_SQL_Error: Error 'Unknown table 't4'' on query. Default database: 'test'. Query: 'drop table t4'
1. 也可以通过mysql的错误日志,通过We stopped at log 'mysql-bin.000005' position 524 得知出问题的pos点:524和binlog的日志文件是:mysql-bin.000005
# tail -f /var/log/mysqld.log
130221 17:31:46 [Note] Slave SQL thread initialized, starting replication in log 'mysql-bin.000005' at position 430, relay log './mysql-relay.000002' position: 345
130221 17:31:46 [Note] 'SQL_SLAVE_SKIP_COUNTER=1' executed at relay_log_file='./mysql-relay.000002', relay_log_pos='345', master_log_name='mysql-bin.000005', master_log_pos='430' and new position at relay_log_file='./mysql-relay.000002', relay_log_pos='439', master_log_name='mysql-bin.000005', master_log_pos='524' 
130221 17:31:46 [ERROR] Slave SQL: Error 'Unknown table 't4'' on query. Default database: 'test'. Query: 'drop table t4', Error_code: 1051
130221 17:31:46 [Warning] Slave: Unknown table 't4' Error_code: 1051
130221 17:31:46 [ERROR] Error running query, slave SQL thread aborted. Fix the problem, and restart the slave SQL thread with "SLAVE START". We stopped at log 'mysql-bin.000005' position 524
130221 17:31:46 [Note] Slave I/O thread: connected to master 'repl@10.0.0.101:3306',replication started in log 'mysql-bin.000005' at position 703
3. 获取N值.
通过错误可知,是使用'drop table t4' 这条语句导致错误了;如果想跳出这个错误,那就需要计算跳出多少个事务的N的那个值是多少;
mysql> show binlog events in 'mysql-bin.000005' from 524;
+------------------+------+------------+-----------+-------------+--------------------------------------------+
| Log_name         | Pos  | Event_type | Server_id | End_log_pos | Info                                       |
+------------------+------+------------+-----------+-------------+--------------------------------------------+
| mysql-bin.000005 |  524 | Query      |         1 |         605 | use `test`; drop table t4                  |-->1
| mysql-bin.000005 |  605 | Query      |         1 |         703 | use `test`; insert into tt values("hello") |-->2
| mysql-bin.000005 |  703 | Query      |         1 |         794 | use `test`; create table t4(id int)        |-->3
| mysql-bin.000005 |  794 | Query      |         1 |         886 | use `test`; insert into t4 values(1)       |-->4
| mysql-bin.000005 |  886 | Query      |         1 |         978 | use `test`; insert into t4 values(2)       | -->5
| mysql-bin.000005 |  978 | Query      |         1 |        1070 | use `test`; insert into t4 values(3)       | -->6
| mysql-bin.000005 | 1070 | Query      |         1 |        1162 | use `test`; insert into t4 values(4)       | -->7
+------------------+------+------------+-----------+-------------+--------------------------------------------+
出错的语句Pos位置为524,需要跳到Pos 为703的位置,很明显是N为2,那么可以执行
mysql> SET GLOBAL sql_slave_skip_counter =2
处理办法2: 在配置文件中,直接指定跳过的错误
修改mysql配置文件 /etc/my.cnf ;在 [mysqld]下加一行 slave_skip_errors = 1051 ,保存重启mysql,mysql slave可以正常同步了
