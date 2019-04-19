# CentOS自动打开网络连接 - Koma Hub - CSDN博客
2018年10月13日 13:39:56[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：151
CentOS开机网络默认的关闭的，查看网卡：
```
$ ifconfig
ens33: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.52.128  netmask 255.255.255.0  broadcast 192.168.52.255
        inet6 fe80::25c5:e1ad:cf86:88fc  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:a5:d3:82  txqueuelen 1000  (Ethernet)
        RX packets 342  bytes 29392 (28.7 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 210  bytes 30952 (30.2 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```
找到对应的配置文件：
```
$ sudo more /etc/sysconfig/network-scripts/ifcfg-ens33
[sudo] Toa 的密码：
TYPE=Ethernet
PROXY_METHOD=none
BROWSER_ONLY=no
BOOTPROTO=dhcp
DEFROUTE=yes
IPV4_FAILURE_FATAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_FAILURE_FATAL=no
IPV6_ADDR_GEN_MODE=stable-privacy
NAME=ens33
UUID=a80268f2-6e1b-41f2-8e45-f197b703f2ce
DEVICE=ens33
ONBOOT=no
ZONE=public
```
可以看到他的ONBOOT是no，将其改成yes（需要管理员权限）
```
$ sudo vim /etc/sysconfig/network-scripts/ifcfg-ens33
[Toa@toa ~]$ sudo more /etc/sysconfig/network-scripts/ifcfg-ens33
TYPE=Ethernet
PROXY_METHOD=none
BROWSER_ONLY=no
BOOTPROTO=dhcp
DEFROUTE=yes
IPV4_FAILURE_FATAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_FAILURE_FATAL=no
IPV6_ADDR_GEN_MODE=stable-privacy
NAME=ens33
UUID=a80268f2-6e1b-41f2-8e45-f197b703f2ce
DEVICE=ens33
ONBOOT=yes
ZONE=public
```
下次开机就会直接打开下图位置处的联网了
![](https://img-blog.csdn.net/20181013133920114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
