# 不停止 MySQL 服务增加从库的两种方式 - 文章 - 伯乐在线
原文出处： [李振良](http://lizhenliang.blog.51cto.com/7876557/1669829)
现在生产环境MySQL数据库是一主一从，由于业务量访问不断增大，故再增加一台从库。前提是不能影响线上业务使用，也就是说不能重启MySQL服务，为了避免出现其他情况，选择在网站访问量低峰期时间段操作。
一般在线增加从库有两种方式，一种是通过mysqldump备份主库，恢复到从库，mysqldump是逻辑备份，数据量大时，备份速度会很慢，锁表的时间也会很长。另一种是通过xtrabackup工具备份主库，恢复到从库，xtrabackup是物理备份，备份速度快，不锁表。为什么不锁表？因为自身会监控主库日志，如果有更新的数据，就会先写到一个文件中，然后再回归到备份文件中，从而保持数据一致性。
服务器信息：
- 主库：192.168.18.212（原有）
- 从库1：192.168.18.213（原有）
- 从库2：192.168.18.214（新增）
- 数据库版本：MySQL5.5
- 存储引擎：Innodb
- 测试库名：weibo
# 一、mysqldump方式
MySQL主从是基于binlog日志，所以在安装好数据库后就要开启binlog。这样好处是，一方面可以用binlog恢复数据库，另一方面可以为主从做准备。
原有主库配置参数如下：


```
# vi my.cnf
server-id = 1             #id要唯一
log-bin = mysql-bin         #开启binlog日志
auto-increment-increment = 1   #在Ubuntu系统中MySQL5.5以后已经默认是1
auto-increment-offset = 1 
slave-skip-errors = all      #跳过主从复制出现的错误
```
**1. 主库创建同步账号**


```
mysql> grant all on *.* to 'sync'@'192.168.18.%' identified by 'sync';
```
**2. 从库配置MySQL**


```
# vi my.cnf
server-id = 3             #这个设置3
log-bin = mysql-bin         #开启binlog日志
auto-increment-increment = 1   #这两个参数在Ubuntu系统中MySQL5.5以后都已经默认是1
auto-increment-offset = 1 
slave-skip-errors = all      #跳过主从复制出现的错误
```
**3. 备份主库**


```
# mysqldump -uroot -p123 --routines --single_transaction --master-data=2 --databases weibo > weibo.sql
```
参数说明：
- –routines：导出存储过程和函数
- –single_transaction：导出开始时设置事务隔离状态，并使用一致性快照开始事务，然后unlock tables;而lock-tables是锁住一张表不能写操作，直到dump完毕。
- –master-data：默认等于1，将dump起始（change master to）binlog点和pos值写到结果中，等于2是将change master to写到结果中并注释。
**4. 把备份库拷贝到从库**


```
# scp weibo.sql root@192.168.18.214:/home/root
```
**5. 在主库创建test_tb表，模拟数据库新增数据，weibo.sql是没有的**


```
mysql> create table test_tb(id int,name varchar(30));
```
**6. 从库导入备份库**


```
# mysql -uroot -p123 -e 'create database weibo;'
# mysql -uroot -p123 weibo < weibo.sql
```
**7. 在备份文件weibo.sql查看binlog和pos值**


```
# head -25 weibo.sql
-- CHANGE MASTER TO MASTER_LOG_FILE='mysql-bin.000001', MASTER_LOG_POS=107;   #大概22行
```
**8. 从库设置从这个日志点同步，并启动**


```
mysql> change master to master_host='192.168.18.212',
    -> master_user='sync',
    -> master_password='sync',
    -> master_log_file='mysql-bin.000001',
    -> master_log_pos=107;
mysql> start slave;
```


```
mysql> show slave status\G;
ERROR 2006 (HY000): MySQL server has gone away
No connection. Trying to reconnect...
Connection id:    90
Current database: *** NONE ***
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 192.168.18.212
                  Master_User: sync
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin.000001
          Read_Master_Log_Pos: 358
               Relay_Log_File: mysqld-relay-bin.000003
                Relay_Log_Pos: 504
        Relay_Master_Log_File: mysql-bin.000001
             Slave_IO_Running: Yes
            Slave_SQL_Running: Yes
......
```
可以看到IO和SQL线程均为YES，说明主从配置成功。
**9. 从库查看weibo库里面的表**


```
mysql> show tables;
+---------------------------+
| Tables_in_weibo           |
+---------------------------+
| test_tb                   |
```
发现刚才模拟创建的test_tb表已经同步过来！
# 二、xtrabackup方式（推荐）
在上面配置基础上做实验，先删除掉从库配置：


```
mysql> stop slave;         #停止同步
mysql> reset slave;        #清除从连接信息
mysql> show slave status\G;   #再查看从状态，可以看到IO和SQL线程都为NO
mysql> drop database weibo;   #删除weibo库
```
此时，从库现在和新装的一样，继续前进！
**1. 主库使用xtrabackup备份**


```
# innobackupex --user=root --password=123 ./
```
生成一个以时间为命名的备份目录：2015-07-01_16-49-43


```
# ll 2015-07-01_16-49-43/
total 18480
drwxr-xr-x 5 root root     4096 Jul  1 16:49 ./
drwx------ 4 root root     4096 Jul  1 16:49 ../
-rw-r--r-- 1 root root      188 Jul  1 16:49 backup-my.cnf
-rw-r----- 1 root root 18874368 Jul  1 16:49 ibdata1
drwxr-xr-x 2 root root     4096 Jul  1 16:49 mysql/
drwxr-xr-x 2 root root     4096 Jul  1 16:49 performance_schema/
drwxr-xr-x 2 root root    12288 Jul  1 16:49 weibo/
-rw-r--r-- 1 root root       21 Jul  1 16:49 xtrabackup_binlog_info
-rw-r----- 1 root root       89 Jul  1 16:49 xtrabackup_checkpoints
-rw-r--r-- 1 root root      563 Jul  1 16:49 xtrabackup_info
-rw-r----- 1 root root     2560 Jul  1 16:49 xtrabackup_logfile
```
**2. 把备份目录拷贝到从库上**


```
# scp -r 2015-07-01_16-49-43 root@192.168.18.214:/home/root
```
**3. 从库上把MySQL服务停掉，删除datadir目录，将备份目录重命名为datadir目录**


```
# sudo rm -rf /var/lib/mysql/
# sudo mv 2015-07-01_16-49-43/ /var/lib/mysql
# sudo chown mysql.mysql -R /var/lib/mysql
# sudo /etc/init.d/mysql start
# ps -ef |grep mysql    #查看已经正常启动
mysql     8832     1  0 16:55 ?        00:00:00 /usr/sbin/mysqld
```
**4. **在主库创建test_tb2表，模拟数据库新增数据****


```
mysql> create table test_tb2(id int,name varchar(30));
```
**5. 从备份目录中xtrabackup_info文件获取到binlog和pos位置**


```
# cat /var/lib/mysql/xtrabackup_info 
uuid = 201af9db-1fce-11e5-96b0-525400e4239d
name = 
tool_name = innobackupex
tool_command = --user=root --password=... ./
tool_version = 1.5.1-xtrabackup
ibbackup_version = xtrabackup version 2.2.11 based on MySQL server 5.6.24 Linux (x86_64) (revision id: )
server_version = 5.5.43-0ubuntu0.12.04.1-log
start_time = 2015-07-01 16:49:43
end_time = 2015-07-01 16:49:46
lock_time = 1
binlog_pos = filename 'mysql-bin.000001', position 429    #这个位置
innodb_from_lsn = 0
innodb_to_lsn = 1598188
partial = N
incremental = N
format = file
compact = N
compressed = N
```
**6. 从库设置从这个日志点同步，并启动**


```
mysql> change master to master_host='192.168.18.212',
    -> master_user='sync',
    -> master_password='sync',
    -> master_log_file='mysql-bin.000001',
    -> master_log_pos=429;
mysql> start slave;
```


```
mysql> show slave status\G;
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 192.168.18.212
                  Master_User: sync
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin.000001
          Read_Master_Log_Pos: 539
               Relay_Log_File: mysqld-relay-bin.000002
                Relay_Log_Pos: 363
        Relay_Master_Log_File: mysql-bin.000001
             Slave_IO_Running: Yes
            Slave_SQL_Running: Yes
......
```
可以看到IO和SQL线程均为YES，说明主从配置成功。
**9. 从库查看weibo库里面的表**


```
mysql> show tables;
+---------------------------+
| Tables_in_weibo           |
+---------------------------+
| test_tb                   |
```
发现刚才模拟创建的test_tb2表已经同步过来。
更多Xtrabackup使用查看博文：[http://blog.jobbole.com/94629/](http://blog.jobbole.com/94629/)
