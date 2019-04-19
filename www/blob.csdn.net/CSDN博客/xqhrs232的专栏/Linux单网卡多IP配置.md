# Linux单网卡多IP配置 - xqhrs232的专栏 - CSDN博客
2018年04月25日 17:54:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：177
原文地址::[http://blog.chinaunix.net/uid-354915-id-3995445.html](http://blog.chinaunix.net/uid-354915-id-3995445.html)
相关文章
1、linux下一块网卡设置多个ip地址----[http://blog.51cto.com/9280078/1845626](http://blog.51cto.com/9280078/1845626)
一、仅一个网卡的情况下，可以让该机器可以通过多个IP被访问，或隐藏常用IP，让他人访问其临时IP。
1.如果临时性的增加一个IP（重启机器或network服务后，丢失），可以使用ifconfig命令
1）先查看目前的网卡信息
[root@test network-scripts]#ifconfig
eth0Link encap:EthernetHWaddr 00:0C:29:13:94:EB
inet addr:192.168.1.88Bcast:192.168.1.255Mask:255.255.255.0
inet6 addr: fe80::20c:29ff:fe13:94eb/64 Scope:Link
UP BROADCAST RUNNING MULTICASTMTU:1500Metric:1
RX packets:3412 errors:0 dropped:0 overruns:0 frame:0
TX packets:1544 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:1000
RX bytes:437408 (427.1 KiB)TX bytes:189062 (184.6 KiB)
Base address:0x2040 Memory:e8920000-e8940000
loLink encap:Local Loopback
inet addr:127.0.0.1Mask:255.0.0.0
inet6 addr: ::1/128 Scope:Host
UP LOOPBACK RUNNINGMTU:16436Metric:1
RX packets:44 errors:0 dropped:0 overruns:0 frame:0
TX packets:44 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:0
RX bytes:4546 (4.4 KiB)TX bytes:4546 (4.4 KiB)
表明现在机器上只有一个网卡，端口为eth0
2）新增一个虚拟端口，并配置IP地址
[root@test network-scripts]#ifconfig eth0:1 172.16.1.222 netmask 255.255.255.0 up
up表示当即生效，另外，如果想关闭个端口。可以ifconfig eth0:1 down
执行命令后，ifconfig多出一个端口信息
eth0:1Link encap:EthernetHWaddr 00:0C:29:13:94:EB
inet addr:172.16.1.119Bcast:172.16.1.255Mask:255.255.255.0
UP BROADCAST RUNNING MULTICASTMTU:1500Metric:1
Base address:0x2040 Memory:e8920000-e8940000
且能够ping通新增的IP
[root@test network-scripts]#ping 172.16.1.222
PING 172.16.1.222 (172.16.1.222) 56(84) bytes of data.
64 bytes from 172.16.1.222: icmp_seq=0 ttl=64 time=3.29 ms
查看当前路由
[root@test ~]#netstat -rn
Kernel IP routing table
DestinationGatewayGenmaskFlagsMSS Windowirtt Iface
192.168.1.00.0.0.0255.255.255.0U0 00 eth0
172.16.1.00.0.0.0255.255.255.0U0 00 eth0#增加的
0.0.0.0192.168.1.10.0.0.0UG0 00 eth0
3）设置路由
对应新IP，新增一个网段，使这个网段能够访问
route add -net 172.16.1.0 netmask 255.255.255.0 gw 172.16.1.254 eth0:1
查看当前路由
[root@test ~]#netstat -rn
Kernel IP routing table
DestinationGatewayGenmaskFlagsMSS Windowirtt Iface
192.168.1.00.0.0.0255.255.255.0U0 00 eth0
172.16.1.0172.16.1.254255.255.255.0UG0 00 eth0#增加的
172.16.1.00.0.0.0255.255.255.0U0 00 eth0
0.0.0.0192.168.1.10.0.0.0UG0 00 eth0
此时ping 172.16.1.118这台机器，ping通，表示临时新增IP完成
[root@test ~]#ping 172.16.1.118
PING 172.16.1.118 (172.16.1.118) 56(84) bytes of data.
64 bytes from 172.16.1.118: icmp_seq=0 ttl=64 time=0.147 ms
注：这是临时使用的办法，如重启network或重启机器。则新增的IP丢失
2、永久性新增一个IP
1）仿照/etc/sysconfig/network-scripts/ifcfg-eth0文件，增加一个新增虚拟端口的文件
如ifcfg-eth0:1
cp /etc/sysconfig/network-scripts/ifcfg-eth0 /etc/sysconfig/network-scripts/ifcfg-eth0:1
vi /etc/sysconfig/network-scripts/ifcfg-eth0:1
修改成
DEVICE=eth0:1
#BOOTPROTO=dhcp
BOOTPROTO=static
HWADDR=00:0C:29:13:94:EB
ONBOOT=yes
IPADDR=172.16.1.119
NETMASK=255.255.255.0
TYPE=Ethernet
GATEWAY=172.16.1.254
或直接在ifcfg-eth0增加：
IPADDR_1=172.16.1.119/24
LABEL_1='1'
2）永久性增加对应的路由
[root@test sysconfig]#vi /etc/sysconfig/static-routes
增加一条路由
any net 172.16.1.0 gw 172.16.1.254 netmask 255.255.255.0
[root@test ~]#vi /etc/sysconfig/network#这个操作如果没做，也能连接上去，暂不知道影响什么的
增加一条
GATEWAY=172.16.1.254
3）service network restart
