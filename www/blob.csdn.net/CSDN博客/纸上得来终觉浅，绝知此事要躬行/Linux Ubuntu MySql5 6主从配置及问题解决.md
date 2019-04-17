# Linux Ubuntu MySql5.6主从配置及问题解决 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月02日 11:11:07[boonya](https://me.csdn.net/boonya)阅读数：998








虚拟机装的是MySql5.6，复制实例后出现  Fatal error: The slave I/O thread stops because master and slave have equal MySQL server UUIDs; these UUIDs must be different for replication to work错误 ；要解决此问题网上很多说的是修改/data/atuo.cnf文件重置server-uuid，不过笔者找了半天都没找到此文件（find -name  auto.cnf或者 whereis auto.cnf），Mysql官方文档讲述的是此文件不可修改也不可手动创建，所以使用mysql5.6做主从备份的朋友需要注意了，这个版本的复制或克隆会有问题，必须卸载干净才能重装，否则问题会重现浪费许多时间。

本人使用的VMware workstation10（Oracle VirtualBox也可）镜像是Ubuntu-server-15.04，本文所讲问题是Mysql版本UUID的标识导致的。

Mysql主从配置请参考：[https://dev.mysql.com/doc/refman/5.5/en/replication-howto.html](https://dev.mysql.com/doc/refman/5.5/en/replication-howto.html)


Mysql的安装请参考：[http://blog.csdn.net/boonya/article/details/53908033](http://blog.csdn.net/boonya/article/details/53908033)


## 卸载Mysql

执行以下命令卸载Mysql：



`sudo apt-get autoremove mysql-server mysql-client`
卸载不能完全需要删除残余：



`sudo find /etc -name "*mysql*" |xargs  rm -rf `
如果不能删除，查询那些路径：




`sudo find /etc -name "*mysql*" |xargs `使用chmod获取路径权限：



`chmod 777 /etc`
如果还是不行强制删除：





``

```
sudo rm /var/lib/mysql/ -R

sudo rm /etc/mysql/ -R

sudo apt-get autoremove mysql* --purge
sudo apt-get remove apparmor
```

注：有三个文件删不掉不影响，执行以下命令进验证

`dpkg -l | grep mysql # 若没有返回，说明已完成卸载.`

## Master主配置

IP:192.168.178.128

修改mysql配置：sudo vi /etc/mysql/mysql.conf.d/mysqld.cnf



```
server-id=1
log-bin=mysql-bin
```

修改后重启mysql：



`sodu /etc/init.d/mysql restart`




此时Slave无法访问Master需要在Master（mysql）窗口为Slave赋予权限：

```
grant all privileges on *.* to 'root'@'192.168.178.129' identified by '123456';
 
flush privileges;
```

查看Master状态：



```
mysql> show master status;
+------------------+----------+--------------+------------------+-------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB | Executed_Gtid_Set |
+------------------+----------+--------------+------------------+-------------------+
| mysql-bin.000001 |      620 |              |                  |                   |
+------------------+----------+--------------+------------------+-------------------+
1 row in set (0.00 sec)

mysql>
```





## Slave从配置

IP:192.168.178.129



修改mysql配置：sudo vi /etc/mysql/mysql.conf.d/mysqld.cnf

```
server-id=2
```
修改后重启mysql：`sodu /etc/init.d/mysql restart`进入mysql命令行，mysql -u root -p 输入密码进入，并执行以下 命令：
```
stop slave;

change master to master_host='192.168.178.128', master_user='root', master_password='123456', master_log_file='mysql-bin.000001', master_log_pos=620;

start slave;
```

查看Slave状态：


`show slave status;`格式化显示Slave状态：



`show slave status\G;`
如果看到如下输出标识主从配置完成：





```
mysql> show slave status\G;
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 192.168.178.128
                  Master_User: root
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin.000001
          Read_Master_Log_Pos: 620
               Relay_Log_File: mysqld-relay-bin.000002
                Relay_Log_Pos: 283
        Relay_Master_Log_File: mysql-bin.000001
             Slave_IO_Running: Yes
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
          Exec_Master_Log_Pos: 620
              Relay_Log_Space: 457
              Until_Condition: None
               Until_Log_File:
                Until_Log_Pos: 0
           Master_SSL_Allowed: No
           Master_SSL_CA_File:
           Master_SSL_CA_Path:
              Master_SSL_Cert:
            Master_SSL_Cipher:
               Master_SSL_Key:
        Seconds_Behind_Master: 0
Master_SSL_Verify_Server_Cert: No
                Last_IO_Errno: 0
                Last_IO_Error:
               Last_SQL_Errno: 0
               Last_SQL_Error:
  Replicate_Ignore_Server_Ids:
             Master_Server_Id: 1
                  Master_UUID: 544196d6-ccc3-11e6-8a15-000c29fc7ee8
             Master_Info_File: /var/lib/mysql/master.info
                    SQL_Delay: 0
          SQL_Remaining_Delay: NULL
      Slave_SQL_Running_State: Slave has read all relay log; waiting for the slave I/O thread to update it
           Master_Retry_Count: 86400
                  Master_Bind:
      Last_IO_Error_Timestamp:
     Last_SQL_Error_Timestamp:
               Master_SSL_Crl:
           Master_SSL_Crlpath:
           Retrieved_Gtid_Set:
            Executed_Gtid_Set:
                Auto_Position: 0
1 row in set (0.00 sec)

ERROR:
No query specified

mysql>
```

主要是 Slave_IO_Running: Yes
            Slave_SQL_Running: Yes  标识成功！！！！



否则看到：



```
mysql> show slave status\G;
*************************** 1. row ***************************
               Slave_IO_State:
                  Master_Host: 192.168.178.128
                  Master_User: root
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin.000001
          Read_Master_Log_Pos: 620
               Relay_Log_File: mysqld-relay-bin.000001
                Relay_Log_Pos: 4
        Relay_Master_Log_File: mysql-bin.000001
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
          Exec_Master_Log_Pos: 620
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
      Last_IO_Error_Timestamp: 170601 18:58:02
     Last_SQL_Error_Timestamp:
               Master_SSL_Crl:
           Master_SSL_Crlpath:
           Retrieved_Gtid_Set:
            Executed_Gtid_Set:
                Auto_Position: 0
1 row in set (0.00 sec)

ERROR:
No query specified

mysql>
```

## 注意事项

1、操作Master已经存在而果Slave不存在数据库时，需要先停掉Slave同步。

2、如果客户端无法连接数据库请执行如下脚本：


`GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '123456' WITH GRANT OPTION;`

收官之作：

![](https://img-blog.csdn.net/20170602120054991)










