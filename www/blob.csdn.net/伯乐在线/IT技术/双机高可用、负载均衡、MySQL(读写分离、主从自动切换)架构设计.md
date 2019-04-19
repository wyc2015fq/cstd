# 双机高可用、负载均衡、MySQL(读写分离、主从自动切换)架构设计 - 文章 - 伯乐在线
原文出处： [朱茂海](https://www.centos.bz/2014/12/ha-load-balance-mysql-master-slave-architecture/)
### 架构简介
前几天网友来信说帮忙实现这样一个架构：只有两台机器，需要实现其中一台死机之后另一台能接管这台机器的服务，并且在两台机器正常服务时，两台机器都能用上。于是设计了如下的架构。
![](http://ww4.sinaimg.cn/large/7cc829d3gw1f6kdwezz1dj20gi0ka76i.jpg)
此架构主要是由keepalived实现双机高可用，维护了一个外网VIP,一个内网VIP。正常情况时，外网VIP和内网VIP都绑定在server1服务器，web请求发送到server1的nginx，nginx对于静态资源请求就直接在本机检索并返回，对于php的动态请求，则负载均衡到server1和server2。对于SQL请求，会将此类请求发送到Atlas MySQL中间件，Atlas接收到请求之后，把涉及写操作的请求发送到内网VIP，读请求操作发送到mysql从，这样就实现了读写分离。
当主服务器server1宕机时，keepalived检测到后，立即把外网VIP和内网VIP绑定到server2，并把server2的mysql切换成主库。此时由于外网VIP已经转移到了server2，web请求将发送给server2的nginx。nginx检测到server1宕机，不再把请求转发到server1的php-fpm。之后的sql请求照常发送给本地的atlas，atlas把写操作发送给内网VIP，读操作发送给mysql从，由于内网VIP已经绑定到server2了，server2的mysql同时接受写操作和读操作。
当主服务器server1恢复后，server1的mysql自动设置为从，与server2的mysql主同步。keepalived不抢占server2的VIP，继续正常服务。
## 架构要求
要实现此架构，需要三个条件：
- 1、服务器可以设置内网IP，并且设置的内网IP互通;
- 2、服务器可以随意绑定IDC分配给我们使用的外网IP，即外网IP没有绑定MAC地址;
- 3、MySQL服务器支持GTID，即MySQL-5.6.5以上版本。
## 环境说明
**server1**
- eth0: 10.96.153.110(对外IP)
- eth1: 192.168.1.100(对内IP)
**server2**
- eth0: 10.96.153.114(对外IP)
- eth1: 192.168.1.101(对内IP)
系统都是CentOS-6。
**对外VIP:** 10.96.153.239
**对内VIP: ** 192.168.1.150
## hosts设置
/etc/hosts:
192.168.1.100 server1
192.168.1.101 server2
## Nginx PHP MySQL Memcached安装
这几个软件的安装推荐使用[EZHTTP](https://www.lxconfig.com/thread-69-1-1.html)来完成。
## 解决session共享问题
php默认的session存储是在/tmp目录下，现在我们是用两台服务器作php请求的负载，这样会造成session分布在两台服务器的/tmp目录下，导致依赖于session的功能不正常。我们可以使用memcached来解决此问题。
上一步我们已经安装好了memcached，现在只需要配置php.ini来使用memcached，配置如下，打开php.ini配置文件，修改为如下两行的值：

```
session.save_handler = memcache
session.save_path = "tcp://192.168.1.100:11211,tcp://192.168.1.101:11211"
```
之后重启php-fpm生效。
## Nginx配置
- Server1配置


```
http {
[...]
    upstream php-server {
           server 192.168.1.101:9000;
           server 127.0.0.1:9000;
           keepalive 100;
    }
[...]
 server {
    [...]
        location ~ \.php$ {
                        fastcgi_pass   php-server;
                        fastcgi_index  index.php;
                        fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
                        include        fastcgi_params;
        }
    [...]
 }
[...]
}
```
### Server2配置

```
http {
[...]
    upstream php-server {
           server 192.168.1.100:9000;
           server 127.0.0.1:9000;
           keepalive 100;
    }
[...]
 server {
    [...]
        location ~ \.php$ {
                        fastcgi_pass   php-server;
                        fastcgi_index  index.php;
                        fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
                        include        fastcgi_params;
        }
    [...]
 }
[...]
}
```
这两个配置主要的作用是设置php请求的负载均衡。
## MySQL配置
### mysql util安装
我们需要安装mysql util里的主从配置工具来实现主从切换。

```
cd /tmp
wget http://dev.mysql.com/get/Downloads/MySQLGUITools/mysql-utilities-1.5.3.tar.gz
tar xzf mysql-utilities-1.5.3.tar.gz
cd mysql-utilities-1.5.3
python setup.py build
python setup.py install
```
### mysql my.cnf配置
**server1:**


```
[mysql]
[...]
protocol=tcp
[...]
[...]
[mysqld]
[...]
# BINARY LOGGING #
log-bin = /usr/local/mysql/data/mysql-bin
expire-logs-days = 14
binlog-format= row
log-slave-updates=true
gtid-mode=on
enforce-gtid-consistency =true
master-info-repository=TABLE
relay-log-info-repository=TABLE
server-id=1
report-host=server1
report-port=3306
[...]
```
**server2:**

```
[mysql]
[...]
protocol=tcp
[...]
[mysqld]
[...]
# BINARY LOGGING #
log-bin = /usr/local/mysql/data/mysql-bin
expire-logs-days = 14
binlog-format= row
log-slave-updates=true
gtid-mode=on
enforce-gtid-consistency =true
master-info-repository=TABLE
relay-log-info-repository=TABLE
server-id=2
report-host=server2
report-port=3306
[...]
```
这两个配置主要是设置了binlog和启用gtid-mode，并且需要设置不同的server-id和report-host。
### 开放root帐号远程权限
我们需要在两台mysql服务器设置root帐号远程访问权限。

```
mysql> grant all on *.* to 'root'@'192.168.1.%' identified by 'Xp29at5F37' with grant option;
mysql> grant all on *.* to 'root'@'server1' identified by 'Xp29at5F37' with grant option;
mysql> grant all on *.* to 'root'@'server2' identified by 'Xp29at5F37' with grant option;
mysql> flush privileges;
```
### 设置mysql主从
在任意一台执行如下命令：


```
mysqlreplicate --master=root:Xp29at5F37@server1:3306 --slave=root:Xp29at5F37@server2:3306 --rpl-user=rpl:o67DhtaW
```
*# master on server1: … connected.# slave on server2: … connected.# Checking for binary logging on master…# Setting up replication…# …done.*
### 显示主从关系

```
mysqlrplshow --master=root:Xp29at5F37@server1 --discover-slaves-login=root:Xp29at5F37
```
*# master on server1: … connected.# Finding slaves for master: server1:3306*
# Replication Topology Graph
server1:3306 (MASTER)
|
+— server2:3306 – (SLAVE)
### 检查主从状态

```
mysqlrplcheck --master=root:Xp29at5F37@server1 --slave=root:Xp29at5F37@server2
```
*# master on server1: … connected.# slave on server2: … connected.Test Description Status—————————————————————————Checking for binary logging on master [pass]Are there binlog exceptions? [pass]Replication user exists? [pass]Checking server_id values [pass]Checking server_uuid values [pass]Is slave connected to master? [pass]Check master information file [pass]Checking InnoDB compatibility [pass]Checking storage engines compatibility [pass]Checking lower_case_table_names settings [pass]Checking slave delay (seconds behind master) [pass]# …done.*
## Keepalived配置
### keepalived安装(两台都装)
- yum -y install keepalived
- chkconfig keepalived on
### keepalived配置(server1)
- vi /etc/keepalived/keepalived.conf


```
vrrp_sync_group VG_1 {
group {
inside_network
outside_network
}
}
 
vrrp_instance inside_network {
state BACKUP
interface eth1
virtual_router_id 51
priority 101
advert_int 1
authentication {
auth_type PASS
auth_pass 3489
}
virtual_ipaddress {
192.168.1.150/24
}
nopreempt
notify /data/sh/mysqlfailover-server1.sh
}
 
vrrp_instance outside_network {
state BACKUP
interface eth0
virtual_router_id 50
priority 101
advert_int 1
authentication {
auth_type PASS
auth_pass 3489
}
virtual_ipaddress {
10.96.153.239/24
}
nopreempt
}
```
### keepalived配置(server2)

```
rrp_sync_group VG_1 {
group {
inside_network
outside_network
}
}
 
vrrp_instance inside_network {
state BACKUP
interface eth1
virtual_router_id 51
priority 100
advert_int 1
authentication {
auth_type PASS
auth_pass 3489
}
virtual_ipaddress {
192.168.1.150
}
notify /data/sh/mysqlfailover-server2.sh
}
 
vrrp_instance outside_network {
state BACKUP
interface eth0
virtual_router_id 50
priority 100
advert_int 1
authentication {
auth_type PASS
auth_pass 3489
}
virtual_ipaddress {
10.96.153.239/24
}
}
```
此keepalived配置需要注意的是：
- 1、两台server的state都设置为backup，server1增加nopreempt配置，并且server1 priority比server2高，这样用来实现当server1从宕机恢复时，不抢占VIP;
- 2、server1设置notify /data/sh/mysqlfailover-server1.sh,server2设置notify /data/sh/mysqlfailover-server2.sh,作用是自动切换主从
/data/sh/mysqlfailover-server1.sh脚本内容：

```
#!/bin/bash
 
sleep 10
state=$3
result=`mysql -h127.0.0.1 -P3306 -uroot -pXp29at5F37 -e 'show slave status;'`
[[ "$result" == "" ]] && mysqlState="master" || mysqlState="slave"
 
if [[ "$state" == "MASTER" ]];then
  if [[ "$mysqlState" == "slave" ]];then
    mysqlrpladmin --slave=root:Xp29at5F37@server1:3306 failover
  fi
 
elif [[ "$state" == "BACKUP" ]];then
  if [[ "$mysqlState" == "master" ]];then
    mysqlreplicate --master=root:Xp29at5F37@server2:3306 --slave=root:Xp29at5F37@server1:3306 --rpl-user=rpl:o67DhtaW
  fi
fi
 
sed -i 's/proxy-read-only-backend-addresses.*/proxy-read-only-backend-addresses = 192.168.1.150:3306/' /usr/local/mysql-proxy/conf/my.cnf
mysql -h127.0.0.1 -P2345 -uuser -ppwd -e "REMOVE BACKEND 2;"
```
/data/sh/mysqlfailover-server2.sh脚本内容：

```
#!/bin/bash
 
sleep 10
state=$3
result=`mysql -h127.0.0.1 -P3306 -uroot -pXp29at5F37 -e 'show slave status;'`
[[ "$result" == "" ]] && mysqlState="master" || mysqlState="slave"
 
if [[ "$state" == "MASTER" ]];then
  if [[ "$mysqlState" == "slave" ]];then
    mysqlrpladmin --slave=root:Xp29at5F37@server2:3306 failover
  fi
 
elif [[ "$state" == "BACKUP" ]];then
  if [[ "$mysqlState" == "master" ]];then
    mysqlreplicate --master=root:Xp29at5F37@server1:3306 --slave=root:Xp29at5F37@server2:3306 --rpl-user=rpl:o67DhtaW
  fi
fi
 
sed -i 's/proxy-read-only-backend-addresses.*/proxy-read-only-backend-addresses = 192.168.1.150:3306/' /usr/local/mysql-proxy/conf/my.cnf
mysql -h127.0.0.1 -P2345 -uuser -ppwd -e "REMOVE BACKEND 2;"
```
## Atlas设置
### atlas安装
到这里下载最新版本，https://github.com/Qihoo360/Atlas/releases

```
cd /tmp
wget https://github.com/Qihoo360/Atlas/releases/download/2.2.1/Atlas-2.2.1.el6.x86_64.rpm
rpm -i Atlas-2.2.1.el6.x86_64.rpm
```
### atlas配置

```
cd /usr/local/mysql-proxy/conf
cp test.cnf my.cnf
vi my.cnf
```
调整如下参数，

```
proxy-backend-addresses = 192.168.1.150:3306
proxy-read-only-backend-addresses = 192.168.1.101:3306
pwds = root:qtyU1btXOo074Itvx0UR9Q==
event-threads = 8
```
注意：
proxy-backend-addresse设置为内网VIP
proxy-read-only-backend-addresses设置为server2的IP
root:qtyU1btXOo074Itvx0UR9Q==设置数据库的用户和密码，密码是通过/usr/local/mysql-proxy/bin/encrypt Xp29at5F37生成。
更详细参数解释请查看，[Atlas配置详解](https://github.com/Qihoo360/Atlas/wiki/Atlas%E9%83%A8%E5%88%86%E9%85%8D%E7%BD%AE%E5%8F%82%E6%95%B0%E5%8F%8A%E5%8E%9F%E7%90%86%E8%AF%A6%E8%A7%A3)。
### 启动atlas
- /usr/local/mysql-proxy/bin/mysql-proxy –defaults-file=/usr/local/mysql-proxy/conf/my.cnf
之后程序里配置mysql就配置127.0.0.1:1234就好。
### 部署atlas自动维护脚本
在两台机器都部署此脚本，并添加定时任务（如每2分钟运行一次）我们把脚本放在/data/sh/auto_maintain_atlas.sh,脚本内容为：

```
#!/bin/bash
 
count=`mysql -N -h127.0.0.1 -P2345 -uuser -ppwd -e "select * from backends;" | wc -l`
 
if [[ "$count" == "1" ]];then
  result=`mysql -hserver1 -P3306 -uroot -pXp29at5F37 -e 'show slave status\G'`
  if echo "$result" | grep Slave_IO_State;then
    slaveIP=192.168.1.100
  else
    result=`mysql -hserver2 -P3306 -uroot -pXp29at5F37 -e 'show slave status\G'`
    slaveIP=192.168.1.101
  fi
 
        slaveIORunning=`echo "$result" | awk -F':' '/Slave_IO_Running:/{print $2}'`
        slaveSQLRunning=`echo "$result" | awk -F':' '/Slave_SQL_Running:/{print $2}'`
        SlaveSQLRunning_State=`echo "$result" | awk -F':' '/Slave_SQL_Running_State:/{print $2}'`
    
  if [[ "$slaveIORunning" =~ "Yes" && "$slaveSQLRunning" =~ "Yes" && "$SlaveSQLRunning_State" =~ "Slave has read all relay log" ]];then
    mysql -h127.0.0.1 -P2345 -uuser -ppwd -e "add slave ${slaveIP}:3306;"
  fi
fi
```
为什么需要这个脚本呢？假设目前mysql主服务器在s1,s1宕机后，s2接管VIP，接着删除atlas中设置的slave backend，其mysql提升为主。过一段时间后，s1从宕机中恢复，这时候s1的mysql自动切换为从，接着删除atlas中设置的slave backend，开始连接s2的mysql主同步数据。到这个时候我们发现，已经不存在读写分离了，所有的sql都发送给了s2的mysql。auto_maintain_atlas.sh脚本就派上用场了，此脚本会定时的检查主从是否已经同步完成，如果完成就自动增加slave backend，这样读写分离又恢复了，完全不需要人工干预。
## server1主宕机测试
### 测试keepalived是否工作正常
我们来模拟server1宕机。
在server1上执行shutdown关机命令。
此时我们登录server2，执行ip addr命令，输出如下：
*1: lo: mtu 16436 qdisc noqueue state UNKNOWNlink/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00inet 127.0.0.1/8 scope host loinet6 ::1/128 scope hostvalid_lft forever preferred_lft forever2: eth0: mtu 1500 qdisc pfifo_fast state UP qlen 1000link/ether 00:0c:29:81:9d:42 brd ff:ff:ff:ff:ff:ffinet 10.96.153.114/24 brd 10.96.153.255 scope global eth0inet 10.96.153.239/24 scope global secondary eth0inet6 fe80::20c:29ff:fe81:9d42/64 scope linkvalid_lft forever preferred_lft forever3: eth1: mtu 1500 qdisc pfifo_fast state UP qlen 1000link/ether 00:0c:29:81:9d:4c brd ff:ff:ff:ff:ff:ffinet 192.168.1.101/24 brd 192.168.1.255 scope global eth1inet 192.168.1.150/32 scope global eth1inet6 fe80::20c:29ff:fe81:9d4c/64 scope linkvalid_lft forever preferred_lft forever*
我们看到对外VIP 10.96.153.239和对内IP 192.168.1.150已经转移到server2了，证明keepalived运行正常。
### 测试是否自动切换了主从
登录server2的mysql服务器，执行show slave status;命令，如下：
*mysql> show slave statusGEmpty set (0.00 sec)*
我们发现从状态已经为空，证明已经切换为主了。
### 测试server1是否抢占VIP
为什么要测试这个呢？如果server1恢复之后抢占了VIP，而我们的Atlas里后端设置的是VIP，这样server1启动之后，sql的写操作就会向server1的mysql发送，而server1的mysql数据是旧于server2的，所以这样会造成数据不一致，这个是非常重要的测试。
我们先来启动server1，之后执行ip addr，输出如下：
*1: lo: mtu 16436 qdisc noqueue state UNKNOWNlink/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00inet 127.0.0.1/8 scope host loinet6 ::1/128 scope hostvalid_lft forever preferred_lft forever2: eth0: mtu 1500 qdisc pfifo_fast state UP qlen 1000link/ether 00:0c:29:f1:4f:4e brd ff:ff:ff:ff:ff:ffinet 10.96.153.110/24 brd 10.96.153.255 scope global eth0inet6 fe80::20c:29ff:fef1:4f4e/64 scope linkvalid_lft forever preferred_lft forever3: eth1: mtu 1500 qdisc pfifo_fast state UP qlen 1000link/ether 00:0c:29:f1:4f:58 brd ff:ff:ff:ff:ff:ffinet 192.168.1.100/24 brd 192.168.1.255 scope global eth1inet6 fe80::20c:29ff:fef1:4f58/64 scope linkvalid_lft forever preferred_lft forever*
我们看到，server1并没有抢占VIP，测试正常。不过另人郁闷的是，在虚拟机的环境并没有测试成功，不知道为什么。
### 测试server2的atlas是否已经删除slave backend
我们测试这个是为了保证atlas已经没有slave backend，也就是没有从库的设置了，否则当server1恢复时，有可能会把读请求发送给server1的mysql，造成读取了旧数据的问题。
*[root@server1 ~]# mysql -h127.0.0.1 -P2345 -uuser -ppwdmysql> select * from backends;+————-+——————–+——-+——+| backend_ndx | address | state | type |+————-+——————–+——-+——+| 1 | 192.168.1.150:3306 | up | rw |+————-+——————–+——-+——+1 rows in set (0.00 sec)*如果看到只有一个后端，证明运作正常。
### 测试server1 mysql是否设置为从
serve1恢复后，登录server1的mysql服务器，执行show slave status;命令，如下：
*mysql> show slave statusG*************************** 1. row ***************************Slave_IO_State: Opening tablesMaster_Host: server1Master_User: rplMaster_Port: 3306Connect_Retry: 60Master_Log_File: mysql-bin.000015Read_Master_Log_Pos: 48405991Relay_Log_File: mysql-relay-bin.000002Relay_Log_Pos: 361Relay_Master_Log_File: mysql-bin.000015Slave_IO_Running: YesSlave_SQL_Running: yes*
### 测试是否自动恢复读写分离
server1恢复后一段时间，我们可以看是读写分离是否已经恢复。
*[root@server1 ~]# mysql -h127.0.0.1 -P2345 -uuser -ppwdWarning: Using a password on the command line interface can be insecure.Welcome to the MySQL monitor. Commands end with ; or g.Your MySQL connection id is 1Server version: 5.0.99-agent-admin*
Copyright (c) 2000, 2014, Oracle and/or its affiliates. All rights reserved.
Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.
Type ‘help;’ or ‘h’ for help. Type ‘c’ to clear the current input statement.
mysql> select * from backends;
+————-+——————–+——-+——+
| backend_ndx | address | state | type |
+————-+——————–+——-+——+
| 1 | 192.168.1.150:3306 | up | rw |
| 2 | 192.168.1.100:3306 | up | ro |
+————-+——————–+——-+——+
2 rows in set (0.00 sec)
我们看到server1已经被添加为slave backend了。这表示已经成功恢复读写分离。
