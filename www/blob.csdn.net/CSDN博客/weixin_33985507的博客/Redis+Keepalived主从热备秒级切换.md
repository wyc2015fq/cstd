# Redis+Keepalived主从热备秒级切换 - weixin_33985507的博客 - CSDN博客
2016年05月01日 21:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
当 Master 与 Slave 均运作正常时, Master负责服务，Slave负责Standby； 
当 Master 挂掉，Slave 正常时, Slave接管服务，同时关闭主从复制功能； 
当 Master 恢复正常，则从Slave同步数据，同步数据之后关闭主从复制功能，恢复Master身份，于此同时Slave等待Master同步数据完成之后，恢复Slave身份。 
然后依次循环……
需要注意的是，这样做需要在Master与Slave上都开启本地化策略，否则在互相自动切换的过程中，未开启本地化的一方会将另一方的数据清空，造成数据完全丢失。
**Master 10.10.51.241**
**Slave   10.10.51.242**
**vip       10.10.51.240**
**master****配置：**
```
mkdir –p /etc/keepalived
```
```
vi /etc/keepalived/keepalived.conf
#加入如下内容脚本
vrrp_script chk_redis { 
        script "/etc/keepalived/scripts/redis_check.sh"   #监控脚本 
        interval 2                                        #监控时间 
} 
vrrp_instance VI_1 { 
        state MASTER                            #设置为MASTER
        interface eth0                          #监控网卡    
        virtual_router_id 51
        priority 101                            #权重值
        authentication { 
             auth_type PASS                     #加密 
             auth_pass redis                    #密码 
        } 
        track_script { 
             chk_redis                          #执行上面定义的chk_redis
        } 
        virtual_ipaddress { 
             10.10.51.240                       #VIP 
        }
        notify_master /etc/keepalived/scripts/redis_master.sh
        notify_backup /etc/keepalived/scripts/redis_backup.sh
        notify_fault  /etc/keepalived/scripts/redis_fault.sh
        notify_stop   /etc/keepalived/scripts/redis_stop.sh 
}
```
```
vi /etc/keepalived/scripts/redis_master.sh
#!/bin/bash
REDISCLI="/opt/redis/bin/redis-cli"
LOGFILE="/var/log/keepalived-redis-state.log"
echo "[master]" >> $LOGFILE
date >> $LOGFILE
echo "Being master...." >> $LOGFILE 2>&1
echo "Run SLAVEOF cmd ..." >> $LOGFILE
$REDISCLI SLAVEOF 10.10.51.242(slave) 6379 >> $LOGFILE  2>&1
sleep 10  #延迟10秒以后待数据同步完成后再取消同步状态
echo "Run SLAVEOF NO ONE cmd ..." >> $LOGFILE
$REDISCLI SLAVEOF NO ONE >> $LOGFILE 2>&1
```
```
vi /etc/keepalived/scripts/redis_backup.sh
#!/bin/bash
REDISCLI="/opt/redis/bin/redis-cli"
LOGFILE="/var/log/keepalived-redis-state.log"
echo "[backup]" >> $LOGFILE
date >> $LOGFILE
echo "Being slave...." >> $LOGFILE 2>&1
sleep 15 #延迟15秒待数据被对方同步完成之后再切换主从角色
echo "Run SLAVEOF cmd ..." >> $LOGFILE
$REDISCLI SLAVEOF 10.10.51.242(slave) 6379 >> $LOGFILE  2>&1
```
**slave配置****：**
```
vi /etc/keepalived/keepalived.conf
vrrp_script chk_redis {
        script "/etc/keepalived/scripts/redis_check.sh"   #监控脚本 
        interval 2                                        #监控时间 
} 
vrrp_instance VI_1 { 
        state BACKUP                                #设置为BACKUP 
        interface eth0                              #监控网卡
        virtual_router_id 51 
        priority 100                                #比MASTRE权重值低 
        authentication { 
             auth_type PASS 
             auth_pass redis                        #密码与MASTRE相同
        } 
        track_script { 
             chk_redis                              #执行上面定义的chk_redis
        } 
        virtual_ipaddress { 
             10.10.51.240                           #VIP 
        } 
        notify_master /etc/keepalived/scripts/redis_master.sh
        notify_backup /etc/keepalived/scripts/redis_backup.sh
        notify_fault  /etc/keepalived/scripts/redis_fault.sh
        notify_stop   /etc/keepalived/scripts/redis_stop.sh 
}
```
```
vi /etc/keepalived/scripts/redis_master.sh
#!/bin/bash
REDISCLI="/opt/redis/bin/redis-cli"
LOGFILE="/var/log/keepalived-redis-state.log"
echo "[master]" >> $LOGFILE
date >> $LOGFILE
echo "Being master...." >> $LOGFILE 2>&1
echo "Run SLAVEOF cmd ..." >> $LOGFILE
$REDISCLI SLAVEOF 10.10.51.241(master) 6379 >> $LOGFILE  2>&1
sleep 10 #延迟10秒以后待数据同步完成后再取消同步状态
echo "Run SLAVEOF NO ONE cmd ..." >> $LOGFILE
$REDISCLI SLAVEOF NO ONE >> $LOGFILE 2>&1
```
```
vi /etc/keepalived/scripts/redis_backup.sh
#!/bin/bash
REDISCLI="/opt/redis/bin/redis-cli"
LOGFILE="/var/log/keepalived-redis-state.log"
echo "[backup]" >> $LOGFILE
date >> $LOGFILE
echo "Being slave...." >> $LOGFILE 2>&1
sleep 15 #延迟15秒待数据被对方同步完成之后再切换主从角色
echo "Run SLAVEOF cmd ..." >> $LOGFILE
$REDISCLI SLAVEOF 10.10.51.241(master) 6379 >> $LOGFILE  2>&1
```
**以下脚本分别在master和slave上创建：**
```
mkdir /etc/keepalived/scripts
```
```
vi /etc/keepalived/scripts/redis_check.sh
#!/bin/bash
ALIVE=`/opt/redis/bin/redis-cli PING`
if [ "$ALIVE" == "PONG" ]; then
echo $ALIVE
     exit 0
else
     echo $ALIVE
     exit 1
fi
```
```
vi /etc/keepalived/scripts/redis_fault.sh
#!/bin/bash
LOGFILE=/var/log/keepalived-redis-state.log
echo "[fault]" >> $LOGFILE
date >> $LOGFILE
```
```
vi /etc/keepalived/scripts/redis_stop.sh
#!/bin/bash
LOGFILE=/var/log/keepalived-redis-state.log
echo "[stop]" >> $LOGFILE
date >> $LOGFILE
```
```
chmod +x /etc/keepalived/scripts/*.sh    #授权
```
