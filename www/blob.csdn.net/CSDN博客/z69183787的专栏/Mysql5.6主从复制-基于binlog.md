# Mysql5.6主从复制-基于binlog - z69183787的专栏 - CSDN博客
2017年04月15日 11:23:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：537

其余问题参考：
http://m.blog.csdn.net/article/details?id=17263509
http://m.blog.csdn.net/article/details?id=17264633
MySQL5.6开始主从复制有两种方式：基于日志（binlog）；基于GTID（全局事务标示符）。
此文章是基于日志方式的配置步骤
环境：
master数据库IP：192.168.247.128
slave数据库IP：192.168.247.130
mysql版本：5.6.14
1.修改master配置文件并重启服务：
[mysqld]
server_id=1
binlog-ignore-db=test#不记录binlog
replicate-ignore-db=test#不复制test库的binlog
log-bin=mysql-bin
binlog_cache_size = 1M
binlog_format=mixed
expire_logs_days=3
2.修改slave配置文件并重启服务：
[mysqld]
server_id=2
binlog-do-db = mydb
binlog-ignore-db=test#不记录binlog
replicate-ignore-db=test#不复制test库的binlog
log-bin=mysql-bin
binlog_cache_size = 1M
binlog_format=mixed
expire_logs_days=3
3.在master上建立用于复制的用户
mysql>grant replication slave, replication client on *.* to 'repl'@'192.168.247.130' identified by 'pwd';
4.备份master的数据
方法1：数据前先锁表，保证数据一致性
mysql> FLUSH TABLES WITH READ LOCK;
mysql> SHOW MASTER STATUS;
+—————–+————+—————-+——————–+
|File             | Position   |  Binlog_Do_DB  |  Binlog_Ignore_DB  |  
+—————–+————+—————-+——————–+
|mysql-bin.000015 |       1273  |                |                    |
+—————–+————+—————-+——————–+
记录文件名和pos号
开始备份数据库
# mysqldump -uroot -p mydb > /tmp/mydb.sql
备份完毕，现在可以解锁数据库表
mysql> UNLOCK TABLES;
方法2：使用--lock-all-tables和--master-data参数结合，导出数据
# mysqldump -uroot -p --hex-blob --lock-all-tables -R --triggers --databases mydb --master-data=2 --default-character-set='utf8' --quick> /tmp/mydb.sql
[有关--master-data参数说明](http://blog.csdn.net/seteor/article/details/17263509)
5.拷贝备份文件到slave，并导入
#scp /tmp/mydb.sql
#mysql -uroot -p -B mydb </tmp/mydb.sql
6.在slave上同步binlog
mysql>CHANGE MASTER TO MASTER_HOST='192.168.247.128',MASTER_USER='repl',MASTER_PASSWORD='pwd',MASTER_LOG_FILE='mysql-bin.000015',MASTER_LOG_POS=1273;
如果是方法2导出的数据，则通过以下语句查询binlog文件名和pos位置：
# grep -i "CHANGE MASTER TO" /tmp/mydb.sql
--CHANGE MASTER TO MASTER_LOG_FILE='mysql-bin.000015', MASTER_LOG_POS=1273;
7.开启复制
mysql> START slave;
Query OK, 0 rows affected, 1 warning (0.00 sec)
8.查看slave状态
mysql> show slave status\G
*************************** 1. row ***************************
               Slave_IO_State: 
                  Master_Host: 192.168.247.128
                  Master_User: repl
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin.000015
          Read_Master_Log_Pos: 1273
               Relay_Log_File: DBtest1-relay-bin.000001
                Relay_Log_Pos: 4
        Relay_Master_Log_File: mysql-bin.000015
             Slave_IO_Running: No
            Slave_SQL_Running: Yes
              Replicate_Do_DB: 
          Replicate_Ignore_DB: 
           Replicate_Do_Table: 
       Replicate_Ignore_Table: 
      Replicate_Wild_Do_Table: 
  Replicate_Wild_Ignore_Table: 
                   Last_Errno: 0
                   Last_Error: 
                 Skip_Counter: 0
          Exec_Master_Log_Pos: 1273
              Relay_Log_Space: 120
              Until_Condition: None
               Until_Log_File: 
                Until_Log_Pos: 0
           Master_SSL_Allowed: No
           Master_SSL_CA_File: 
           Master_SSL_CA_Path: 
              Master_SSL_Cert: 
            Master_SSL_Cipher: 
               Master_SSL_Key: 
        Seconds_Behind_Master: NULL
Master_SSL_Verify_Server_Cert: No
                Last_IO_Errno: 1593
                Last_IO_Error: Fatal error: The slave I/O thread stops because master and slave have equal MySQL server UUIDs; these UUIDs must be different for replication to work.
               Last_SQL_Errno: 0
               Last_SQL_Error: 
  Replicate_Ignore_Server_Ids: 
             Master_Server_Id: 1
                  Master_UUID: 
             Master_Info_File: /var/lib/mysql/master.info
                    SQL_Delay: 0
          SQL_Remaining_Delay: NULL
      Slave_SQL_Running_State: Slave has read all relay log; waiting for the slave I/O thread to update it
           Master_Retry_Count: 86400
                  Master_Bind: 
      Last_IO_Error_Timestamp: 131210 19:04:04
     Last_SQL_Error_Timestamp: 
               Master_SSL_Crl: 
           Master_SSL_Crlpath: 
           Retrieved_Gtid_Set: 
            Executed_Gtid_Set: 
                Auto_Position: 0
1 row in set (0.00 sec)
可以看到io进程报错: master and slave have equal MySQL server UUIDs
因为我的虚拟机是在mysql安装好以后克隆的，所以在mysql的数据目录下的auto.cnf文件中的uuid一样，所以导致错误
解决方法：删除slave上的auto.cnf，重启mysql服务会自动生成新的auto.cnf,uuid也会变化。
重启后再次查看正常，插入数据正常。
