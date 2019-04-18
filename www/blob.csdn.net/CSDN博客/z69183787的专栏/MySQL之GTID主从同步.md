# MySQL之GTID主从同步 - z69183787的专栏 - CSDN博客
2017年04月15日 11:42:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：385
## 简介
Simple replication
这是一种最简单的主从同步复制方式，添加一台新的mysql服务作为slave服务，并在此服务上回放master上所有的事务，当slave服务启动后，会去读取master上全部的binary log，这种方式简单快捷，但是需要耗费时间。
## master服务设为只读
如果已经配置了主从同步并且正在运行，先将master所有服务设为只读，然后确保slave上所有服务已经将数据同步更新。
`SET @@global.read_only = ON;`- 1
## 停止master服务
将所有master服务停止。
## 配置GTID
master、slave服务上修改my.ini或者my.cnf，server_id确保不重复，然后启动服务。
```
server_id=1
log_bin=mysql-bin-1
gtid_mode=on #开启GTID模式
binlog_format=mixed #启用混合模式
enforce-gtid-consistency=true #强制GTID一致性
log-slave-updates=true #从库从主库复制数据时可以写入到binlog日志
skip_slave_start=1 
binlog-do-db=db1 #同步数据库
binlog-do-db=db2
expire_logs_days=7
max_binlog_size=20M
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
## 同步账号
master服务上创建同步账号。
```
mysql> grant replication slave on *.* to 'repl'@'%' identified by 'repl';
# 格式：mysql> GRANT REPLICATION SLAVE ON *.* TO '帐号'@'从服务器IP或主机名' IDENTIFIED BY '密码';
```
- 1
- 2
- 3
## 开启slave
```
mysql> CHANGE MASTER TO
     >     MASTER_HOST = '192.168.2.63',
     >     MASTER_PORT = 3306,
     >     MASTER_USER = 'repl',
     >     MASTER_PASSWORD = 'repl',
     >     MASTER_AUTO_POSITION = 1;
mysql> start slave;
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
## master取消只读
### MySQL之GTID主从同步
