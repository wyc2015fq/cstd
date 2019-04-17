# LVS+keepalived+nginx+tomcat部署实现 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月17日 15:42:19[boonya](https://me.csdn.net/boonya)阅读数：1426








From：[http://lovelace.blog.51cto.com/1028430/1550188](http://lovelace.blog.51cto.com/1028430/1550188)


# 拓扑结构图


![wKioL1QOmLrDoQ2uAAHpCQhZ_Bk403.jpg](http://s3.51cto.com/wyfs02/M00/49/16/wKioL1QOmLrDoQ2uAAHpCQhZ_Bk403.jpg)

# 节点分布情况

LVS-dr-master

    eth0: 192.168.146.141





LVS-dr-slave

    eth0: 192.168.146.142








nginx1:

    eth0: 192.168.146.139


nginx2:

    eth0: 192.168.146.140


tomcat1:

    eth0: 192.168.146.138**启用了4个tomcat**





VIP: 192.168.146.200




**# 具体配置**

### lvs master

### 1、安装ipvsadm、keepalived






```
```xml
yum install -y keepalived ipvsadm
```
```









### 2、使用keepalived来管理lvs

脚本：



```
```xml
global_defs
```



```xml
{
```



```xml
router_id master_210
```



```xml
}
```



```xml

```



```xml
vrrp_instance aiyou {
```



```xml
state MASTER
```



```xml
interface eth0
```



```xml
virtual_router_id 100    #这个数值 master和slave必须统一
```



```xml
priority 151     #这个数值决定哪台服务器是master
```



```xml
advert_int 1
```



```xml
authentication {
```



```xml

```

```xml
auth_type PASS
```



```xml

```

```xml
auth_pass 123456
```



```xml

```

```xml
}
```



```xml
virtual_ipaddress {
```



```xml

```

```xml
192.168.146.200
```



```xml

```

```xml
}
```



```xml
}
```



```xml
virtual_server 192.168.146.200 80 {
```



```xml

```

```xml
delay_loop 6
```



```xml

```

```xml
lb_algo wrr
```



```xml

```

```xml
lb_kind DR
```



```xml
#            persistence_timeout 50
```



```xml

```

```xml
protocol TCP
```



```xml
real_server 192.168.146.139 80 {
```



```xml

```

```xml
weight 1
```



```xml

```

```xml
TCP_CHECK {
```



```xml

```

```xml
connect_timeout 3
```



```xml

```

```xml
nb_get_retry 3
```



```xml

```

```xml
delay_before_retry 3
```



```xml

```

```xml
connect_port 80
```



```xml

```

```xml
}
```



```xml

```

```xml
}
```



```xml
real_server 192.168.146.140 80 {
```



```xml

```

```xml
weight 1
```



```xml

```

```xml
TCP_CHECK {
```



```xml

```

```xml
connect_timeout 3
```



```xml

```

```xml
nb_get_retry 3
```



```xml

```

```xml
delay_before_retry 3
```



```xml

```

```xml
connect_port 80
```



```xml

```

```xml
}
```



```xml

```

```xml
}
```



```xml
}
```
```






### lvs slave

### 1、安装ipvsadm、keepalived






```
```xml
yum install -y keepalived ipvsadm
```
```









### 2、使用keepalived来管理lvs



```
```xml
global_defs
```



```xml
{
```



```xml
router_id slave_211
```



```xml
}
```



```xml

```



```xml
vrrp_instance aiyou {
```



```xml
state MASTER
```



```xml
interface eth0
```



```xml
virtual_router_id 100    #这个数值 master和slave必须统一
```



```xml
priority 150     #这个数值决定哪台服务器是master 这里我们比master数值低，所以角色是backup，
```



```xml
advert_int 1
```



```xml
authentication {
```



```xml

```

```xml
auth_type PASS
```



```xml

```

```xml
auth_pass 123456
```



```xml

```

```xml
}
```



```xml
virtual_ipaddress {
```



```xml

```

```xml
192.168.146.200
```



```xml

```

```xml
}
```



```xml
}
```



```xml
virtual_server 192.168.146.200 80 {
```



```xml

```

```xml
delay_loop 6
```



```xml

```

```xml
lb_algo wrr
```



```xml

```

```xml
lb_kind DR
```



```xml
#            persistence_timeout 50
```



```xml

```

```xml
protocol TCP
```



```xml
real_server 192.168.146.139 80 {
```



```xml

```

```xml
weight 1
```



```xml

```

```xml
TCP_CHECK {
```



```xml

```

```xml
connect_timeout 3
```



```xml

```

```xml
nb_get_retry 3
```



```xml

```

```xml
delay_before_retry 3
```



```xml

```

```xml
connect_port 80
```



```xml

```

```xml
}
```



```xml

```

```xml
}
```



```xml
real_server 192.168.146.140 80 {
```



```xml

```

```xml
weight 1
```



```xml

```

```xml
TCP_CHECK {
```



```xml

```

```xml
connect_timeout 3
```



```xml

```

```xml
nb_get_retry 3
```



```xml

```

```xml
delay_before_retry 3
```



```xml

```

```xml
connect_port 80
```



```xml

```

```xml
}
```



```xml

```

```xml
}
```



```xml
}
```
```






### nginx1

### nginx上要跑个脚本



```
```xml
#!/bin/bash
```



```xml
#
```



```xml
# Script to start LVS DR real server.
```



```xml
# description: LVS DR real server
```



```xml
#
```



```xml
.  /etc/rc.d/init.d/functions
```



```xml
VIP=192.168.146.200   #这里根据需要改成自己的VIP地址
```



```xml
host=`/bin/hostname`
```



```xml
case "$1" in
```



```xml
start)
```



```xml

```

```xml
# Start LVS-DR real server on this machine.
```



```xml

```

```xml
/sbin/ifconfig lo down
```



```xml

```

```xml
/sbin/ifconfig lo up
```



```xml

```

```xml
echo 1 > /proc/sys/net/ipv4/conf/lo/arp_ignore
```



```xml

```

```xml
echo 2 > /proc/sys/net/ipv4/conf/lo/arp_announce
```



```xml

```

```xml
echo 1 > /proc/sys/net/ipv4/conf/all/arp_ignore
```



```xml

```

```xml
echo 2 > /proc/sys/net/ipv4/conf/all/arp_announce
```



```xml

```

```xml
/sbin/ifconfig lo:0 $VIP broadcast $VIP netmask 255.255.255.255 up
```



```xml

```

```xml
/sbin/route add -host $VIP dev lo:0
```



```xml
;;
```



```xml
stop)
```



```xml

```

```xml
# Stop LVS-DR real server loopback device(s).
```



```xml

```

```xml
/sbin/ifconfig lo:0 down
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/lo/arp_ignore
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/lo/arp_announce
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/all/arp_ignore
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/all/arp_announce
```



```xml
;;
```



```xml
status)
```



```xml

```

```xml
# Status of LVS-DR real server.
```



```xml

```

```xml
islothere=`/sbin/ifconfig lo:0 | grep $VIP`
```



```xml

```

```xml
isrothere=`netstat -rn | grep "lo:0" | grep $VIP`
```



```xml

```

```xml
if [ ! "$islothere" -o ! "isrothere" ];then
```



```xml

```

```xml
# Either the route or the lo:0 device
```



```xml

```

```xml
# not found.
```



```xml

```

```xml
echo "LVS-DR real server Stopped."
```



```xml

```

```xml
else
```



```xml

```

```xml
echo "LVS-DR real server Running."
```



```xml

```

```xml
fi
```



```xml
;;
```



```xml
*)
```



```xml

```

```xml
# Invalid entry.
```



```xml

```

```xml
echo "$0: Usage: $0 {start|status|stop}"
```



```xml

```

```xml
exit 1
```



```xml
;;
```



```xml
esac
```
```






### nginx2

### nginx上要跑个脚本



```
```xml
#!/bin/bash
```



```xml
#
```



```xml
# Script to start LVS DR real server.
```



```xml
# description: LVS DR real server
```



```xml
#
```



```xml
.  /etc/rc.d/init.d/functions
```



```xml
VIP=192.168.146.200   #这里根据需要改成自己的VIP地址
```



```xml
host=`/bin/hostname`
```



```xml
case "$1" in
```



```xml
start)
```



```xml

```

```xml
# Start LVS-DR real server on this machine.
```



```xml

```

```xml
/sbin/ifconfig lo down
```



```xml

```

```xml
/sbin/ifconfig lo up
```



```xml

```

```xml
echo 1 > /proc/sys/net/ipv4/conf/lo/arp_ignore
```



```xml

```

```xml
echo 2 > /proc/sys/net/ipv4/conf/lo/arp_announce
```



```xml

```

```xml
echo 1 > /proc/sys/net/ipv4/conf/all/arp_ignore
```



```xml

```

```xml
echo 2 > /proc/sys/net/ipv4/conf/all/arp_announce
```



```xml

```

```xml
/sbin/ifconfig lo:0 $VIP broadcast $VIP netmask 255.255.255.255 up
```



```xml

```

```xml
/sbin/route add -host $VIP dev lo:0
```



```xml
;;
```



```xml
stop)
```



```xml

```

```xml
# Stop LVS-DR real server loopback device(s).
```



```xml

```

```xml
/sbin/ifconfig lo:0 down
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/lo/arp_ignore
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/lo/arp_announce
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/all/arp_ignore
```



```xml

```

```xml
echo 0 > /proc/sys/net/ipv4/conf/all/arp_announce
```



```xml
;;
```



```xml
status)
```



```xml

```

```xml
# Status of LVS-DR real server.
```



```xml

```

```xml
islothere=`/sbin/ifconfig lo:0 | grep $VIP`
```



```xml

```

```xml
isrothere=`netstat -rn | grep "lo:0" | grep $VIP`
```



```xml

```

```xml
if [ ! "$islothere" -o ! "isrothere" ];then
```



```xml

```

```xml
# Either the route or the lo:0 device
```



```xml

```

```xml
# not found.
```



```xml

```

```xml
echo "LVS-DR real server Stopped."
```



```xml

```

```xml
else
```



```xml

```

```xml
echo "LVS-DR real server Running."
```



```xml

```

```xml
fi
```



```xml
;;
```



```xml
*)
```



```xml

```

```xml
# Invalid entry.
```



```xml

```

```xml
echo "$0: Usage: $0 {start|status|stop}"
```



```xml

```

```xml
exit 1
```



```xml
;;
```



```xml
esac
```
```






### 后端tomcat安装省略




采用一机多实例模式（非多虚拟主机）




### 测试




### master上验证



```
```xml
[root@lvs1 ~]# ipvsadm -ln
```



```xml
IP Virtual Server version 1.2.1 (size=4096)
```



```xml
Prot LocalAddress:Port Scheduler Flags
```



```xml

```

```xml
-> RemoteAddress:Port           Forward Weight ActiveConn InActConn
```



```xml
TCP  192.168.146.200:80 wrr
```



```xml

```

```xml
-> 192.168.146.139:80           Route   1      0          0
```



```xml

```

```xml
-> 192.168.146.140:80           Route   1      0          0
```



```xml
[root@lvs1 ~]#
```
```






### slave上验证



```
```xml
[root@lvs2 ~]# ipvsadm -ln
```



```xml
IP Virtual Server version 1.2.1 (size=4096)
```



```xml
Prot LocalAddress:Port Scheduler Flags
```



```xml

```

```xml
-> RemoteAddress:Port           Forward Weight ActiveConn InActConn
```



```xml
TCP  192.168.146.200:80 wrr
```



```xml

```

```xml
-> 192.168.146.139:80           Route   1      0          0
```



```xml

```

```xml
-> 192.168.146.140:80           Route   1      0          0
```



```xml
[root@lvs2 ~]#
```
```






参考文章：

[http://www.5ilinux.com/2014/05/lvs_keepalived_nginx_tomcat.html](http://www.5ilinux.com/2014/05/lvs_keepalived_nginx_tomcat.html)



