
# LVS+Keepalived DR模式实现web负载均衡高可用 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月21日 17:40:36[Jlins](https://me.csdn.net/dyllove98)阅读数：2608


一、环境介绍：
lvs_master:192.168.1.18
lvs_backup:192.168.1.19
web1:192.168.1.16
web2:192.168.1.17
vip:192.168.1.20


二、安装：
在两台lvs上安装lvs+keepalived：
yum install lvsadm keepalived -y

在两台web服务器上安装nginx（安装配置略过）

配置keepalived：
主lvs上配置：
vim /etc/keepalived/keepalived.conf
! Configuration File for keepalived
global_defs {
notification_email {
jack_keyang@163.com
}
notification_email_from root@localhost.localdomain
smtp_server 127.0.0.1
smtp_connect_timeout 30
router_id LVS_1
}

vrrp_instance VI_1 {
stateMASTER
interface eth0
virtual_router_id 51
priority100
advert_int 1
authentication {
auth_type PASS
auth_pass 1111
}
virtual_ipaddress {
192.168.1.20
}
}

virtual_server 192.168.1.20 80 {
delay_loop 6
lb_algo rr\#lvs轮询方式
lb_kind DR\#lvs工作方式
nat_mask 255.255.255.255
protocol TCP

real_server 192.168.1.16 80 {
weight 10
TCP_CHECK {
connect_timeout 10
nb_get_retry 3
delay_before_retry 3
connect_port 80
}
}

real_server 192.168.1.17 80 {
weight 10
TCP_CHECK {
connect_timeout 10
nb_get_retry 3
delay_before_retry 3
connect_port 80
}
}
}

备lvs上只需修改红色部分即可：
state BACKUP
priority90

以上部分为lvs机器配置结束
下面为web机器上配置脚本，主要为绑定vip并关闭arp响应
\#!/bin/bash
VIP=192.168.1.20
. /etc/rc.d/init.d/functions
case $1 in
start)
ifconfig lo:0 $VIP netmask 255.255.255.255 broadcast $VIP
route add -host $VIP dev lo:0
echo 1 >/proc/sys/net/ipv4/conf/lo/arp_ignore
echo 2 >/proc/sys/net/ipv4/conf/lo/arp_announce
echo 1 >/proc/sys/net/ipv4/conf/all/arp_ignore
echo 2 >/proc/sys/net/ipv4/conf/all/arp_announce
echo "RealServer Start OK"
;;
stop)
ifconfig lo:0 down
route del $VIP > /dev/null 2>&1
echo 0 >/proc/sys/net/ipv4/conf/lo/arp_ignore
echo 0 >/proc/sys/net/ipv4/conf/lo/arp_announce
echo 0 >/proc/sys/net/ipv4/conf/all/arp_ignore
echo 0 >/proc/sys/net/ipv4/conf/all/arp_announce
echo "RealServer Stoped"
;;
*)
echo "Usage: $0 (start|stop)"
exit 1
esac
exit 0
因为都是使用内网IP，如果想在外网访问只需将VIP的80端口在路由器上映射即可
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

