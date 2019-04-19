# Linux网关配置 - fanyun的博客 - CSDN博客
2016年04月07日 09:31:24[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4840
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)

Linux网关配置：
一、网络拓扑
![gateway](http://www.linuxidc.com/upload/2010_09/100923061034281.jpg)
二、配置网络
1．A,GW1,GW2,B的网络配置
A:
eth0:
IPADDR :192.168.1.2
NETMASK:255.255.255.0
GATEWAY:192.168.1.1
GW1:
eth0:
IPADDR :172.16.113.173
NETMASK:255.255.255.0
GATEWAY:172.16.113.9
eth1:
IPADDR :192.168.1.1
NETMASK:255.255.255.0
B:
eth0:
IPADDR :192.168.0.2
NETMASK:255.255.255.0
GATEWAY:192.168.0.1
GW2:
eth0:
IPADDR :172.16.113.163
NETMASK:255.255.255.0
GATEWAY:172.16.113.9
eth1:
IPADDR :192.168.0.1
NETMASK:255.255.255.0
2．GW1设置
(1)开启路由转发功能
# echo 1 > /proc/sys/net/ipv4/ip_forward
或
#vim /etc/sysctl.conf
net.ipv4.ip_forward = 0 --> net.ipv4.ip_forward = 1
(2)添加到192.168.0.0/24网段的路由
# route add -net 192.168.0.0 netmask 255.255.255.0 gw 172.16.113.163
3. GW2设置
(1)开启路由转发功能
# echo 1 > /proc/sys/net/ipv4/ip_forward
或
#vim /etc/sysctl.conf
net.ipv4.ip_forward = 0 --> net.ipv4.ip_forward = 1
(2)添加到192.168.1.0/24网段的路由
# route add -net 192.168.1.0 netmask 255.255.255.0 gw 172.16.113.173
三、测试网关
若A，B能相互ping通，则两个网关也配置成功了.
