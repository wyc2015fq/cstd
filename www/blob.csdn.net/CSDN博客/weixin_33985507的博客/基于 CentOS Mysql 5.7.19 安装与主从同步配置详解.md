# 基于 CentOS Mysql 5.7.19 安装与主从同步配置详解 - weixin_33985507的博客 - CSDN博客
2017年08月02日 11:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：23
## 1、为什么要使用主从同步
1.如果主服务器出现问题，可以快速切换到从服务器提供的服务
2.可以在从服务器上执行查询操作，降低主服务器的访问压力
3.可以在从服务器上执行备份，以避免备份期间影响主服务器的服务
> 
注意：一般只有更新不频繁的数据或者对实时性要求不高的数据可以通过从服务器查询，实时性要求高的数据仍然需要从主服务器获得。
[Window 数据库主从（Master/Slave）同步安装与配置详解](https://www.jianshu.com/p/17e1ccc7d537)
## 2、 CentOS安装Mysql 5.7.19
[Centos7.3 安装Mysql并修改初始密码](https://www.jianshu.com/p/4a41a6df19a6)
[Window 安装Mysql并修改初始密码](https://www.jianshu.com/p/9df5a7ed53b5)
## 3、测试环境
我这里使用**parallels desktop** 虚拟机安装的Centos操作系统
数据库版本： 5.7.19
主机A：10.211.55.3（Master）
主机B：10.211.55.4（Slave）
```
service mysqld stop #停止数据库
service mysqld start #启动数据库 
service mysqld restart #重启数据库
```
![1342351-3580c14d49ee55a9.png](https://upload-images.jianshu.io/upload_images/1342351-3580c14d49ee55a9.png)
系统信息
## 4、配置主服务器Master
### 4.1 给从服务器设置授权用户(创建复制帐号)
建立一个帐户javen，并且只能允许从 10.211.55.4 这个地址上来登陆，密码是123456。
```
mysql> grant replication slave on *.* to 'javen'@'10.211.55.4' identified by '123456';
mysql> flush privileges;
```
### 4.2 主服务器Master配置
在 `etc/my.cnf`的`[mysqld]`下添加如下内容
```
port=3306
 #[必须]启用二进制日志
binlog-ignore-db=mysql 
#[必须]服务器唯一ID，默认是1
server-id= 1
#只保留7天的二进制日志，以防磁盘被日志占满
expire-logs-days  = 7 
#不备份的数据库
binlog-ignore-db=information_schema
binlog-ignore-db=performation_schema
binlog-ignore-db=sys
binlog-ignore-db=gogs
```
### 4.3 重启MySQL服务并设置读取锁定
```
service mysqld restart
```
在主服务器上设置读取锁定有效，确保没有数据库操作，以便获得一个一致性的快照
```
mysql -u root -proot -P3306
mysql> flush tables with read lock;
```
### 4.4 查看主服务器上当前的二进制日志名和偏移量值
```
mysql> show master status;
+------------------+----------+--------------+-------------------------------------------------------+-------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB                                      | Executed_Gtid_Set |
+------------------+----------+--------------+-------------------------------------------------------+-------------------+
| mysql-bin.000001 |      154 |              | mysql,information_schema,performation_schema,sys,gogs |                   |
+------------------+----------+--------------+-------------------------------------------------------+-------------------+
1 row in set (0.00 sec)
```
![1342351-31b4963a4f64c120.png](https://upload-images.jianshu.io/upload_images/1342351-31b4963a4f64c120.png)
这里的 File 、Position 是在配置Salve的时候要使用到的，Binlog_Do_DB表示要同步的数据库，Binlog_Ignore_DB 表示Ignore的数据库，这些都是在配置的时候进行指定的。
另外：如果执行这个步骤始终为Empty set(0.00 sec)，那说明前面的my.cnf 没配置对。
## 5、配置从服务器Slave
### 5.1 修改从数据库的配置
修改之后完整的配置如下：
```
# For advice on how to change settings please see
# http://dev.mysql.com/doc/refman/5.7/en/server-configuration-defaults.html
[mysqld]
#
# Remove leading # and set to the amount of RAM for the most important data
# cache in MySQL. Start at 70% of total RAM for dedicated server, else 10%.
# innodb_buffer_pool_size = 128M
#
# Remove leading # to turn on a very important data integrity option: logging
# changes to the binary log between backups.
# log_bin
#
# Remove leading # to set options mainly useful for reporting servers.
# The server defaults are faster for transactions and fast SELECTs.
# Adjust sizes as needed, experiment to find the optimal values.
# join_buffer_size = 128M
# sort_buffer_size = 2M
# read_rnd_buffer_size = 2M
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
##以下是添加的内容
log-bin=mysql-bin
server-id=3
binlog-ignore-db  = mysql
binlog-ignore-db  = information_schema
binlog-ignore-db  = performation_schema
binlog-ignore-db  = sys
log-slave-updates
slave-skip-errors=all
slave-net-timeout=60
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
log-error=/var/log/mysqld.log
pid-file=/var/run/mysqld/mysqld.pid
```
### 5.2 重启从数据库并设置Slave数据库
```
service mysqld restart
```
登录从数据库并做如下设置
```
mysql> stop slave;  #关闭Slave
mysql> change master to master_host='10.211.55.3',master_user='javen',master_password='123456',master_log_file='mysql-bin.000001', master_log_pos= 154;
mysql> start slave;  #开启Slave
```
**注意**：在这里指定Master的信息，master_log_file是在配置Master的时候的File选项， master_log_pos是在配置Master的Position 选项，这里要进行对应。
### 5.3 查看Slave配置的信息
```
mysql> show slave status \G;
*************************** 1. row ***************************
               Slave_IO_State:
                  Master_Host: 10.211.55.3
                  Master_User: javen
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin.000001
          Read_Master_Log_Pos: 154
               Relay_Log_File: centos-linux-2-relay-bin.000001
                Relay_Log_Pos: 4
        Relay_Master_Log_File: mysql-bin.000001
             Slave_IO_Running: No
            Slave_SQL_Running: No
              Replicate_Do_DB:
          Replicate_Ignore_DB:
           Replicate_Do_Table:
       Replicate_Ignore_Table:
      Replicate_Wild_Do_Table:
  Replicate_Wild_Ignore_Table:
                   Last_Errno: 0
                   Last_Error:
                 Skip_Counter: 0
          Exec_Master_Log_Pos: 154
              Relay_Log_Space: 154
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
                Last_IO_Errno: 0
                Last_IO_Error:
               Last_SQL_Errno: 0
               Last_SQL_Error:
  Replicate_Ignore_Server_Ids:
             Master_Server_Id: 0
                  Master_UUID:
             Master_Info_File: /var/lib/mysql/master.info
                    SQL_Delay: 0
          SQL_Remaining_Delay: NULL
      Slave_SQL_Running_State:
           Master_Retry_Count: 86400
                  Master_Bind:
      Last_IO_Error_Timestamp:
     Last_SQL_Error_Timestamp:
               Master_SSL_Crl:
           Master_SSL_Crlpath:
           Retrieved_Gtid_Set:
            Executed_Gtid_Set:
                Auto_Position: 0
         Replicate_Rewrite_DB:
                 Channel_Name:
           Master_TLS_Version:
1 row in set (0.00 sec)
ERROR:
No query specified
mysql> unlock tables;
Query OK, 0 rows affected (0.00 sec)
mysql>  start slave;
Query OK, 0 rows affected (0.00 sec)
mysql>
```
## 6、关闭掉主数据库的读取锁定
```
mysql> unlock tables;
```
## 7、在主数据库中创建一个表以及添加数据测试
![1342351-3b6b6afabeb74fd6.png](https://upload-images.jianshu.io/upload_images/1342351-3b6b6afabeb74fd6.png)
主库
![1342351-9886d50aa72e8464.png](https://upload-images.jianshu.io/upload_images/1342351-9886d50aa72e8464.png)
从库
