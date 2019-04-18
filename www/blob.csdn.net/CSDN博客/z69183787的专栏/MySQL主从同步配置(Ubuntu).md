# MySQL主从同步配置(Ubuntu) - z69183787的专栏 - CSDN博客
2016年12月27日 18:27:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：315
一、在两台Linux机器上安装MySQL
二、Master主服务器配置(192.168.1.3)
1.编辑my.cnf（命令查找文件位置：find / -name my.cnf）
```
vi /etc/mysql/my.cnf
```
在[mysqld]中添加：
server-id = 1
log_bin = master-bin
log_bin_index = master-bin.index
binlog_do_db = my_data
binlog_ignore_db = mysql
备注：server-id 服务器唯一标识，log_bin 启动MySQL二进制日志，binlog_do_db 指定记录二进制日志的数据库，binlog_ignore_db 指定不记录二进制日志的数据库。
![](http://images2015.cnblogs.com/blog/481063/201609/481063-20160925013511371-1044669421.png)
2.登录主服务器MySQL创建从服务器用到的账户和权限：
```
grant replication slave on *.* to 'masterbackup' @'192.168.1.%' identified by 'masterbackup';
```
（192.168.1.%通配符，表示0-255的IP都可访问主服务器，正式环境请配置指定从服务器IP）
![](http://images2015.cnblogs.com/blog/481063/201609/481063-20160925015023856-1695070551.png)
3.重启MySQL，命令：
```
/etc/init.d/mysql restart
```
4.登录MySQL状态下，查看主服务器状态：
```
show master status;
```
![](http://images2015.cnblogs.com/blog/481063/201609/481063-20160925015859637-403814983.png)
三、Slave从服务器配置（192.168.1.5）
1.编辑my.cnf（命令查找文件位置：find / -name my.cnf）
```
vi /etc/mysql/my.cnf
```
在[mysqld]中添加：
server-id = 2
relay-log = slave-relay-bin
relay-log-index = slave-relay-bin.index
![](http://images2015.cnblogs.com/blog/481063/201609/481063-20160925020451590-964645567.png)
2.重启Slave从服务器MySQL，命令：
```
/etc/init.d/mysql restart
```
3.登录Slave从服务器，连接Master主服务器：
```
change master to master_host='192.168.1.103',master_port=**3306**,master_user='masterbackup',master_password='masterbackup',master_log_file='master-bin.000001',master_log_pos=**2403**;
```
备注：master_host对应主服务器的IP地址，master_port对应主服务器的端口，master_log_file对应show master status显示的File列：master-bin.000001，master_log_pos对应Position列：2403，否则有可能出现同步失败。
![](http://images2015.cnblogs.com/blog/481063/201609/481063-20160925022151402-420940975.png)
4.登录MySQL状态下，启动Slave数据同步。
```
start slave;
```
5.登录MySQL状态下，查看Slave信息：
```
show slave status\G;
```
![](http://images2015.cnblogs.com/blog/481063/201609/481063-20160925022415762-489182734.png)
备注：Slave_IO_Running和Slave_SQL_Running都为yes才表示同步成功。
6.登录MySQL状态下，停止同步命令：
```
stop slave;
```
