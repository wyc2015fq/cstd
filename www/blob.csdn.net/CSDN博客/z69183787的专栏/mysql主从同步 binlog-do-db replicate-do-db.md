# mysql主从同步 binlog-do-db replicate-do-db - z69183787的专栏 - CSDN博客
2017年04月15日 11:15:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13088

在主服务器上为从服务器建立一个用户：
grant replication slave on *.* to '用户名'@'主机' identified by '密码';
如果使用的是MySQL 4.0.2之前的版本，则用file权限来代替replication slave
编辑主服务器的配置文件：/etc/my.cnf
[bi](http://www.itpub.net/tree/index_339/)nlog_do_db是指定binlog日志记录那些库的二进制日志。replicate_do_db则在slave库中指定同步那些库的binlog日志。
在主从互备环境中，有没有必要每个服务器都同时配置binlog_do_db和replicate_do_db?理由是什么？
binlog-do-db：指定mysql的binlog日志记录哪个db
Replicate_Do_DB：参数是在slave上配置，指定slave要复制哪个库
在master上设置binlog_do_弊端：
1、过滤操作带来的负载都在master上
2、无法做基于时间点的复制（利用binlog）。
server-id=1
log-bin
[binlog-do-db](http://dev.mysql.com/doc/refman/5.0/en/replication-options-binary-log.html#option_mysqld_binlog-do-db)=需要复制的数据库名，如果复制多个数据库，重复设置这个选项即可
[binlog-ignore-db](http://dev.mysql.com/doc/refman/5.0/en/replication-options-binary-log.html#option_mysqld_binlog-ignore-db)=不需要复制的数据库苦命，如果复制多个数据库，重复设置这个选项即可
注意：如果你想做一个复杂点的结构：比如说，A->B->C，其中B是A的从服务器，同时B又是C的主服务器，那么B服务器除了需要打开log-bin之外，还需要打开[log-slave-updates](http://dev.mysql.com/doc/refman/5.0/en/replication-options-slave.html#option_mysqld_log-slave-updates)选项，你可以再B上使用“show
 variables like 'log%';”来确认是否已经生效。
编辑从服务器的配置文件：/etc/my.cnf
server-id=2
master-host=主机
master-user=用户名
master-password=密码
master-port=端口
[replicate-do-db](http://dev.mysql.com/doc/refman/5.0/en/replication-options-slave.html#option_mysqld_replicate-do-db)=需要复制的数据库名，如果复制多个数据库，重复设置这个选项即可
[replicate-ignore-db](http://dev.mysql.com/doc/refman/5.0/en/replication-options-slave.html#option_mysqld_replicate-ignore-db)=需要复制的数据库名，如果复制多个数据库，重复设置这个选项即可
配置主从服务器的my.cnf时，留心各自的server-id一定要彼此独立，不能重复，否则，会出现如下错误：
Slave: received end packet FROM server, apparent master shutdown
另一个需要注意的是最好在从服务器的my.cnf里设置[read_only](http://dev.mysql.com/doc/refman/5.0/en/server-system-variables.html#sysvar_read_only)选项，防止发生意外（连接用户不能有SUPER权限，否则无效）。
记得先手动同步一下主从服务器，数据量小的话可以用mysqldump，它有一个[master-data](http://dev.mysql.com/doc/refman/5.0/en/mysqldump.html#option_mysqldump_master-data)参数很有用，通过使用此参数，导出的SQL文件里会自动包含CHANGE MASTER TO
 MASTER_LOG_FILE='...', MASTER_LOG_POS=...;，这样创建从服务器就更方便了。
如果数据量大的话不太适合使用mysqldump（慢），如果是myisam表的话，加上--lock-all-tables参数，如果是innodb表的话，加上--single-transaction参数。
而应该采用拷贝文件的方式，请按如下操作步骤：
先在主服务器上锁定所有的表，以免在复制过程中数据发生变化：
mysql> flush tables with read lock;
然后在主服务器上查询当前二进制文件的文件名及偏移位置：
mysql > show master status;
然后停止主服务器上的MySQL服务：
shell> mysqladmin -u root shutdown
注意：如果仅是MyISAM的话，可以不停止MySQL服务，但要在复制数据文件的过程中保持只读锁，如果是InnoDB的话，必须停止MySQL服务。
再拷贝数据文件：
shell> tar -cvf /tmp/mysql-snapshot.tar .
拷贝完别忘了启动主服务上的MySQL服务了。
然后把数据文件应用到从服务器上，再次启动slave的时候使用，记得启动时加上skip-slave-start选项，使之不会立刻去连接master，再在从服务器上设置相关的二进制日志信息：
mysql> [CHANGE MASTER TO](http://dev.mysql.com/doc/refman/5.0/en/change-master-to.html)
->     MASTER_HOST='master_host_name',
->     MASTER_USER='replication_user_name',
->     MASTER_PASSWORD='replication_password',
->     MASTER_LOG_FILE='recorded_log_file_name',
->     MASTER_LOG_POS=recorded_log_position;
启动从服务器上的复制线程：
mysql> start slave;
验证主从设置是否已经成功，可以输入如下命令：
mysql> show slave status\G
会得到类似下面的列表：
Slave_IO_Running: Yes
Slave_SQL_Running: Yes
如果这两个选项不全是Yes，那就说明你前面某个步骤配置错了。
应该保证从服务器上任何数据的修改都是通过从主服务器上复制操作获取的，换句话说，从服务器应该是只读的，如果不能保证这一点，则可能造成主从数据不一致。可以在从服务器的my.cnf里加入read-only参数来实现这一点，唯一需要注意的一点事read-only仅对没有super权限的用户有效。所以最好核对一下连接从服务器的用户，确保其没有super权限。
从理想角度看，主从数据库应该无故障的运转下去，可以有时候还是会出现一些莫名其妙的问题，比如说即便从未在从服务器上手动更新过数据，但还是可能遇到“Error: 1062 Duplicate entry”错误，具体原因不详，可能是MySQL本身的问题。遇到这类问题的时候，从服务器会停止复制操作，我们只能手动解决问题，具体的操作步骤如下：
mysql> set global sql_slave_skip_counter = 1;
mysql> start slave;
同样的操作可能需要进行多次，也可以设置自动处理此类操作，在从服务器的my.cnf里设置：
slave-skip-errors=1062
最后再唠叨一下日志的问题：时间长了，数据库服务器上的二进制文件会越来越多，清理是必要的，你可以设置自动清理，相关参数是[expire_logs_days](http://dev.mysql.com/doc/refman/5.0/en/server-system-variables.html#sysvar_expire_logs_days)，也可以使用手动删除的方式，但这里说的手动不是指rm，而是指[PURGE
 BINARY LOGS](http://dev.mysql.com/doc/refman/5.0/en/purge-binary-logs.html)，删除任何日志前，最好在所有的从服务器上通过show slave status命令确认一下相关日志是否已经无用。
更详细的介绍参考官方文档：[How to Set Up Replication](http://dev.mysql.com/doc/refman/5.0/en/replication-howto.html)，不喜欢英文的话可以看老叶同志的[中文翻译](http://imysql.cn/2006_05_17_mysql_replication_1)。
补充：[ERROR] Error in Log_event::read_log_event(): 'Event too big'
在使用主从复制的时候，出现的问题多半是和日志（主服务器的二进制日志，从服务器的延迟日志）相关的。比如说加入你遇到了上面的错误，你可以根据错误日志的信息在主从数据库服务器上分别执行：
mysqlbinlog 日志文件 > /dev/null
查看错误，如果没有错误，则不会有任何输出，反之会输出错误信息，如果确定了错误是出现在主服务器二进制日志上，可以跳过适当的位置，再在从服务器上重新设定LOG_POS，如果确定了错误是出现在从服务器延迟日志上，则可以删除从服务器的延迟日志（使用CHANGE TO MASTER的时候，除非设定了延迟日志信息，否则会自动删除延迟日志），并在从服务器上重新设定LOG_POS。期间也可以考虑手动执行不能自动执行的SQL日志。
补充：配置的时候如果版本允许最好打开[sync_binlog](http://dev.mysql.com/doc/refman/5.0/en/replication-options-binary-log.html#sysvar_sync_binlog)选项。
补充：有时候，从服务器延迟日志可能已经损坏，这时需要执行CHANGE MASTER TO设置新的日志文件信息，但是在从服务器上SHOW SLAVE STATUS会显示很多日志信息，他们的含义有所不同：
Master_Log_File:Read_Master_Log_Pos 是IO相关的日志信息
Relay_Master_Log_File:Exec_Master_Log_Pos 是SQL相关的日志信息
从服务器需要设置的是SQL相关的日志信息：
slave stop;
change master to master_log_file=’(binlog name in relay_master_log_file)’, master_log_pos=(exec_master_log_pos number);
slave start;
1) When you are using the master as a consistent snapshot, use SHOW MASTER STATUS to determine the position.
2) When you are using a slave as a consistent snapshot, use SHOW SLAVE STATUS and Exec_Master_Log_Pos.
[参考链接](http://www.mysqlperformanceblog.com/2008/07/07/how-show-slave-status-relates-to-change-master-to/)
补充：缺省情况下，从服务器会以主机名命名延迟日志，所以一旦你修改了从服务器的主机名就会造成问题，新版MySQL会提示你这个情况：
[Warning] Neither --relay-log nor --relay-log-index were used;
so replication may break when this MySQL server acts as a slave and has his hostname changed!!
Please use '--relay-log=name-relay-bin' to avoid this problem.
原文地址：[http://zhumeng8337797.blog.163.com/blog/static/100768914201183013058142/](http://zhumeng8337797.blog.163.com/blog/static/100768914201183013058142/)
