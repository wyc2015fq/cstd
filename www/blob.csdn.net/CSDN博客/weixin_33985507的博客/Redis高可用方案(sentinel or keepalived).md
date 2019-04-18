# Redis高可用方案(sentinel or keepalived) - weixin_33985507的博客 - CSDN博客
2018年01月02日 18:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
#### 关键词：redis 、 sentinel(哨兵模式)、keepalived
#### 一、Redis单实例
当系统中只运行一台Redis实例时，一旦该redis挂了，必然会导致系统不可用
#### 二、Redis主从同步(备份)
当1台Redis故障时，另外一台可以继续提供服务，一般配置为1主1从或者1主多从。
但是Redis配置主从同步，当redis master节点挂掉之后，slave节点只能提供只读服务，无法提供写服务，所以还需要想办法实现当主redis挂了之后，让从redis升级为主redis。
这里提到一个概念，自动故障转移，redis sentinel带有这个功能，当一个主redis不能提供服务时，redis sentinel可以将一个从redis升级为主redis，并对其他从redis进行配置，让它们使用新的主redis进行复制备份。
#### 三、Redis sentinel(哨兵模式方案)
1、系统环境：
redis-master ： 192.168.83.139
redis-slave ： 192.168.83.140
2、安装redis
> 
cd /opt/apps_install/
wget http://download.redis.io/releases/redis-4.0.1.tar.gz
tar zxvf redis-4.0.1.tar.gz
cd redis-4.0.1
make
ln -s /opt/apps_install/redis-4.0.1 /opt/apps/redis
安装成功则会在当前目录生成src目录，常用的redis命令如redis-server、redis-cli等即在src目录下
3、配置redis主从
① 主redis 192.168.83.139
redis.conf
> 
port 9000           #修改端口是安全的第一步
daemonize  yes
bind  0.0.0.0
pidfile "/opt/apps_install/redis-4.0.1/run/redis_9000.pid"
logfile "/opt/apps_install/redis-4.0.1/logs/redis_9000.log"
② 从redis 192.168.83.140
> 
redis.conf
port 9000 #修改端口是安全的第一步
daemonize  yes
bind  0.0.0.0
pidfile "/opt/apps_install/redis-4.0.1/run/redis_9000.pid"
logfile "/opt/apps_install/redis-4.0.1/logs/redis_9000.log"
slaveof 192.168.83.139 9000 # 确定主从关系
启动redis：
> 
/opt/apps/redis/src/redis-server /opt/apps/redis/redis.conf
查看主redis状态：
> 
/opt/apps/redis/src/redis-cli -p 9000 info replication
![7309052-7db469961f83a88d.png](https://upload-images.jianshu.io/upload_images/7309052-7db469961f83a88d.png)
查看从redis状态：
> 
/opt/apps/redis/src/redis-cli -p 9000 info replication
![7309052-159294c1fc24916f.png](https://upload-images.jianshu.io/upload_images/7309052-159294c1fc24916f.png)
4、搭建redis-sentinel(哨兵模式)
redis-sentinel程序上面已经安装过了，这里只需要修改配置文件就可以了
主从的sentinel.conf配置文件一致，不过当sentinel运行起来之后，sentinel配置文件会动态变更，这里我们最好提前备份一下初始配置文件，养成良好习惯~
> 
cp sentinel.conf sentinel.conf.default
sentinel.conf
> 
daemonize yes
port  9001
logfile  /opt/apps/redis/logs/sentinel.log
pidfile  /opt/apps/redis/run/sentinel.pid
sentinel monitor passport-master 192.168.83.139 9000 2
sentinel down-after-milliseconds passport-master 5000  
sentinel failover-timeout  passport-master 15000
sentinel parallel-syncs passport-master 1
protected-mode no # 很关键，不然sentinel之间是不会自动切换主从的
启动sentinel：
> 
/opt/apps/redis/src/redis-sentinel /opt/apps/redis/sentinel.conf
查看sentinel状态
> 
/opt/apps/redis/src/redis-cli -p 9001 info sentinel
> 
# Sentinel
sentinel_masters:1
sentinel_tilt:0
sentinel_running_scripts:0
sentinel_scripts_queue_length:0
sentinel_simulate_failure_flags:0
master0:name=passport-master,status=ok,address=192.168.83.139:9000,slaves=1,sentinels=2
5、测试
把主redis停掉 192.168.83.139
> 
/opt/apps/redis/src/redis-cli -p 9000 shutdown
查看sentinel日志
> 
9942:X 02 Jan 17:26:17.677 # +sdown master passport-master 192.168.83.139 9000
9942:X 02 Jan 17:26:17.762 # +new-epoch 1
9942:X 02 Jan 17:26:17.763 # +vote-for-leader 167e4e08f36d130b5306a81cf2ebb0159e26e325 1
9942:X 02 Jan 17:26:18.579 # +config-update-from sentinel 167e4e08f36d130b5306a81cf2ebb0159e26e325 192.168.83.140 9001 @ passport-master 192.168.83.139 9000
9942:X 02 Jan 17:26:18.579 # +switch-master passport-master 192.168.83.139 9000 192.168.83.140 9000
9942:X 02 Jan 17:26:18.579 * +slave slave 192.168.83.139:9000 192.168.83.139 9000 @ passport-master 192.168.83.140 9000
9942:X 02 Jan 17:26:23.597 # +sdown slave 192.168.83.139:9000 192.168.83.139 9000 @ passport-master 192.168.83.140 9000
再查看此时sentinel状态
> 
/opt/apps/redis/src/redis-cli -p 9001 info sentinel
> 
# Sentinel
sentinel_masters:1
sentinel_tilt:0
sentinel_running_scripts:0
sentinel_scripts_queue_length:0
sentinel_simulate_failure_flags:0
master0:name=passport-master,status=ok,address=192.168.83.140:9000,slaves=1,sentinels=2
发现此时从redis 192.168.83.140已经提升为主redis，至此redis的sentinel(哨兵模式)已经搭建完成，实现了redis故障的自动转移。
#### 但是
使用redis sentinel模式，在redis故障转移时，主redis的IP地址是要变化的，大家也可以预见，在实际生产环境中，客户端程序如果动态的感知当前的主redis的ip和端口呢？redis-sentinel提供了查询接口
> 
[@redis-master /opt/apps/redis]# src/redis-cli -p 9001
127.0.0.1:9001> sentinel get-master-addr-by-name passport-master
1) "192.168.83.140"
2) "9000"
客户端每次连接redis前，先向sentinel发送请求，获得主redis的ip和port，然后用返回的ip和port连接redis。
这种方法的缺点是显而易见的，每次操作redis至少需要发送两次连接请求，第一次请求sentinel，第二次请求redis。
个人认为更好的办法是使用VIP，VIP方案是，redis系统对外始终是同一ip地址，当redis进行故障转移时，需要做的是将VIP从之前的redis服务器漂移到现在新的主redis服务器上。好了至此，我们的redis高可用的一个新方案出来了，那就是redis+keepalived。
#### 四、redis+keepalived方案
redis-master ： 192.168.83.139
redis-slave ： 192.168.83.140
VIP ： 192.168.83.150
1、安装keepalived
> 
yum -y install keepalived ipvsadm
2、Master-Keepalived配置
> 
global_defs {
   router_id passport_lua_redis
}
vrrp_script chk_redis
{ 
    script "/opt/scripts/keepalived/redis_check.sh"
    interval 2
    timeout  2
    fall     3
} 
vrrp_instance passport_redis {
    state MASTER
    interface eth0
    virtual_router_id 143
    priority 100
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass passportluanginx
    }
    virtual_ipaddress {
        192.168.83.150
    }
    track_script {
	        chk_redis
    }
    notify_master /opt/scripts/keepalived/redis_master.sh 
    notify_backup /opt/scripts/keepalived/redis_backup.sh 
    notify_fault  /opt/scripts/keepalived/redis_fault.sh 
    notify_stop   /opt/scripts/keepalived/redsi_stop.sh 
}
2、Slave-Keepalived配置
> 
global_defs {
   router_id passport_lua_redis
}
vrrp_script chk_redis { 
    script "/opt/scripts/keepalived/redis_check.sh"
    interval 2
    timeout  2
    fall     3
} 
vrrp_instance passport_redis {
    state BACKUP
    interface eth0
    virtual_router_id 143
    priority 90
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass passportluanginx
    }
    virtual_ipaddress {
        192.168.83.150
    }
    track_script {
	        chk_redis
    }
    notify_master /opt/scripts/keepalived/redis_master.sh 
    notify_backup /opt/scripts/keepalived/redis_backup.sh 
    notify_fault  /opt/scripts/keepalived/redis_fault.sh 
    notify_stop   /opt/scripts/keepalived/redsi_stop.sh 
}
服务检查脚本及notify脚本：
redis_check.sh
> 
#!/bin/bash
ALIVE=`/opt/apps/redis/src/redis-cli -p 9000 PING` 
LOGFILE=/opt/apps/redis/logs/keepalived-redis-check.log
echo "[CHECK]" >> $LOGFILE
date >> $LOGFILE
if [ "$ALIVE" == "PONG" ] ;then 
	echo "Success: redis-cli -p 9000 PING $ALIVE" >> $LOGFILE 2>&1
	exit 0
else 
	echo "Failed:  redis-cli -p 9000 PING $ALIVE " >> $LOGFILE 2>&1
        exit 1 
fi 
主redis的redis_master.sh(从redis则把ip地址改为主redis的ip即可) ：
> 
#!/bin/bash
REDISCLI="/opt/apps/redis/src/redis-cli" 
LOGFILE="/opt/apps/redis/logs/keepalived-redis-state.log" 
echo "[master]" >> $LOGFILE 
date >> $LOGFILE 
echo "Being master...." >> $LOGFILE 2>&1 
echo "Run SLAVEOF cmd ..." >> $LOGFILE 
$REDISCLI -p 9000 SLAVEOF 192.168.83.140 9000 >> $LOGFILE  2>&1 
sleep 10 #延迟10秒以后待数据同步完成后再取消同步状态 
echo "Run SLAVEOF NO ONE cmd ..." >> $LOGFILE 
$REDISCLI -p 9000 SLAVEOF NO ONE >> $LOGFILE 2>&1 
主redis的redis_backup.sh(从redis则把ip地址改为主redis的ip即可)：
> 
#!/bin/bash
REDISCLI="/opt/apps/redis/src/redis-cli" 
LOGFILE="/opt/apps/redis/logs/keepalived-redis-state.log" 
echo "[backup]" >> $LOGFILE 
date >> $LOGFILE 
echo "Being slave...." >> $LOGFILE 2>&1 
sleep 15 #延迟15秒待数据被对方同步完成之后再切换主从角色 
echo "Run SLAVEOF cmd ..." >> $LOGFILE 
$REDISCLI -p 9000 SLAVEOF 192.168.83.140 9000 >> $LOGFILE  2>&1 
redis_fault.sh 
> 
# !/bin/bash
LOGFILE=/opt/apps/redis/logs/keepalived-redis-state.log 
echo "[fault]" >> $LOGFILE 
date >> $LOGFILE 
redis_stop.sh
> 
# !/bin/bash
LOGFILE=/opt/apps/redis/logs/keepalived-redis-state.log 
echo "[stop]" >> $LOGFILE 
date >> $LOGFILE 
启动keepalived服务
/etc/rc.d/init.d/keepalived start
测试：
1、主从复制测试
> 
# redis-cli set a hello  //Master 写入数据
# redis-cli get a        //Backup 读取数据 
"hello" 
2、VIP写入测试
> 
[@redis-slave /opt/apps/redis]# src/redis-cli -h 192.168.83.150 -p 9000 set b test
OK
[@redis-slave /opt/apps/redis]# src/redis-cli -h 192.168.83.150 -p 9000 get b
"test"
3、关闭主redis，观察vip是否漂移，数据是否正常
/opt/apps/redis/src/redis-cli -p 9000 shutdown
观察到vip从192.168.83.139漂移至192.168.83.140，并且查看到从redis角色已经转换为主redis
现在给redis写入数据，测试看Master恢复服务后能否正常恢复数据
> 
[@redis-slave /opt/apps/redis]# src/redis-cli -h 192.168.83.150 -p 9000 set c nihao
OK
[@redis-slave /opt/apps/redis]# src/redis-cli -h 192.168.83.150 -p 9000 get c
"nihao"
恢复Master上的Redis，查看ip a
> 
1: lo:mtu 65536 qdisc noqueue state UNKNOWN
 link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00 
 inet 127.0.0.1/8 scope host lo 
 inet6 ::1/128 scope host 
 valid_lft forever preferred_lft forever
2: eth0: mtu 1500 qdisc pfifo_fast state UP qlen 1000
    link/ether 00:0c:29:33:0c:b7 brd ff:ff:ff:ff:ff:ff
    inet 192.168.83.139/24 brd 192.168.83.255 scope global eth0
    inet 192.168.83.150/32 scope global eth0
    inet6 fe80::20c:29ff:fe33:cb7/64 scope link
       valid_lft forever preferred_lft forever
在主redis上查看数据，数据恢复，VIP切换回Master，Redis高可用环境搭建成功
> 
[@redis-master /opt/apps/redis]# src/redis-cli -p 9000 get c
"nihao"
至此，已成功搭建高可用及负载均衡的Redis环境。
更新一版，由于redis的安全问题，在上生产环境时，最好是有密码认证，故这里我们补充一下redis主备模式的密码认证方式
1、redis.conf
在master和slave中都分别添加以下配置：
requirepass 你的密码 # redis密码验证
masterauth 你的密码 # master设置密码后，slave需要同步验证，因为我们是主备切换模式，故主备都配置
2、redis_master.sh redis_backup.sh redis_check.sh
修改脚本相应的redis-cli连接方式，加入"-a 你的密码"参数即可
