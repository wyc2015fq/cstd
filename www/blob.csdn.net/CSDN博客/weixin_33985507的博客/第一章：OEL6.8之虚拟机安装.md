# 第一章：OEL6.8之虚拟机安装 - weixin_33985507的博客 - CSDN博客
2018年12月16日 14:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

## 一、在   Windows 上安装  VMware Workstation  
 具体安装请参考《[VMware Workstation 15 Pro 永久激活密钥 下载](https://www.cnblogs.com/ios9/p/10108288.html)》
## 二、创建虚拟机
### 1：选择“新建虚拟机“
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216150146748-1507544341.png)
### 2：选择“典型”
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216150151421-777306932.png)
### 3：选择“稍后安装操作系统“
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216150440235-1733462855.png)
### 4：选择“Linux Oracle Enterprise Linux 64“
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216150443051-1353750509.png)
### 5：命名，目录位置
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216150443880-2122788218.png)
### 6：选择“将磁盘拆分成多个文件”，分配 60G,不会立即占用
按实际需要设置容量大小
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216151500448-1458876355.png)
### 7：硬件资源分配 2G 内存，CPU *2(管理物理机的资源情况)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216151508217-224412105.png)
### 8：选择光驱镜像路径“”
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216151510989-200867025.png)
### 9：NAT:虚拟网卡，主机模式，桥接模式（任一模式）；
## 三、安装linux操作系统
### 1：开启虚拟机

### 2：选择安装”Install or upgrade as existing system”
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216151828718-1788494104.png)
### 3：选择跳过“skip”
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216151943303-1787813434.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216151943915-682481125.png)
### 4：下一步
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216152926496-414869246.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216152928910-840165196.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216152933942-717994042.png)
### 5：“Basic Storage Device“/ next
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216152938555-1120828352.png)
### 6：选择“Yes,discard any data”
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216152944119-1945303496.png)
### 7：Hostname（主机名）：自己命名
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216152945225-1399284761.png)
### 8：配置网络：后面在配置也行
### 9：选择地区“Asia/Shanghai”
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216152946978-1633290858.png)
### 10：设置root用户密码：
密码强度：按实际情况而定。
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216153351916-2009164043.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216153354291-1688514467.png)
### 11：格式磁盘挂载
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154238920-1977334396.png)
注：我设置的硬盘容量为1T 。如果是设置为 60G的硬盘  容量的。提示信息如下下图：
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154241090-243796786.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154242678-1880243782.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154244036-235710235.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154244671-346567080.png)
###  12：启动引导“/dev/sda”
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154246235-1946250945.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154247482-868355414.png)
### 13：最小建议：Desktop,Oracle Linux Server
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154253204-2079213668.png)
### 14：等待安装，然后重启
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154259645-107793776.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154300715-896798926.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154301895-607361795.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216154307502-766439126.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216155705907-1238865853.png)
### 15：licence information  同意
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216161056424-1568551852.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216161109747-1760923912.png)
### 13：No,I prefer to register at a alter time
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216161111757-1377807232.png)
### 14：稍后注册
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216161121805-1202141282.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162035134-346795286.png)
### 15：用户稍后创建
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162039237-839241876.png)

### 16：设置 NTP 时间
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162041788-1149146627.png)
### 17：是否启用 kdump，重启完成后，完成安装
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162048218-1557820878.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162053993-579608149.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162055936-757638423.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162056584-749948595.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216162208267-881410910.png)
### 四：设置静态ip
注：可以参考此为：《[centos 7.3 设置静态IP](https://www.cnblogs.com/ios9/p/8029936.html)》
### 1：登陆进行linux系统后，开启网络连接，并查看ip地址。并测试是否在宿主机可以ping通该虚拟机ip
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164326245-1938146028.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164328008-988281470.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164331379-1588595713.png)
### 2：SecureCRT 进行shell连接
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164612214-839682964.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164612970-127918493.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164613746-1724036931.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164614482-959484793.png)
![image](https://img2018.cnblogs.com/blog/672567/201812/672567-20181216164617331-1457101699.png)
### 3：修改静态ip
```
# 查看ip信息
[root@oracle network-scripts]# ifconfig
eth0      Link encap:Ethernet  HWaddr 00:0C:29:31:35:56
          inet addr:192.168.1.102  Bcast:192.168.1.255  Mask:255.255.255.0
          inet6 addr: fe80::20c:29ff:fe31:3556/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:124262 errors:0 dropped:0 overruns:0 frame:0
          TX packets:62101 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:184251906 (175.7 MiB)  TX bytes:4234465 (4.0 MiB)
lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:56 errors:0 dropped:0 overruns:0 frame:0
          TX packets:56 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:4160 (4.0 KiB)  TX bytes:4160 (4.0 KiB)
#查看 网卡信息
[root@oracle ~]# cd /etc/sysconfig/network-scripts/
[root@oracle network-scripts]# pwd
/etc/sysconfig/network-scripts
[root@oracle network-scripts]# ls
ifcfg-eth0   ifdown-eth   ifdown-isdn    ifdown-sit     ifup-bnep  ifup-ipv6   ifup-post    ifup-tunnel       network-functions
ifcfg-lo     ifdown-ib    ifdown-post    ifdown-tunnel  ifup-eth   ifup-isdn   ifup-ppp     ifup-wireless     network-functions-ipv6
ifdown       ifdown-ippp  ifdown-ppp     ifup           ifup-ib    ifup-plip   ifup-routes  init.ipv6-global
ifdown-bnep  ifdown-ipv6  ifdown-routes  ifup-aliases   ifup-ippp  ifup-plusb  ifup-sit     net.hotplug
[root@oracle network-scripts]# more ifcfg-eth0 
DEVICE=eth0
HWADDR=00:0C:29:31:35:56
TYPE=Ethernet
UUID=91467e4a-f858-460e-8671-6e248dde9b91
ONBOOT=no
NM_CONTROLLED=yes
BOOTPROTO=dhcp
#把网卡的ip信息改为静态ip
[root@oracle network-scripts]# more ifcfg-eth0 
DEVICE=eth0
HWADDR=00:0C:29:31:35:56
#TYPE=Ethernet
UUID=91467e4a-f858-460e-8671-6e248dde9b91
#ONBOOT=no
NM_CONTROLLED=yes
#BOOTPROTO=dhcp
TYPE="Ethernet"
BOOTPROTO="static"
DEFROUTE="yes"
PEERDNS="yes"
PEERROUTES="yes"
IPV4_FAILURE_FATAL="no"
IPV6INIT="yes"
IPV6_AUTOCONF="yes"
IPV6_DEFROUTE="yes"
IPV6_PEERDNS="yes"
IPV6_PEERROUTES="yes"
IPV6_FAILURE_FATAL="no"
IPV6_ADDR_GEN_MODE="stable-privacy"
NAME="eth0"
ONBOOT="yes"
IPADDR="192.168.1.102"
GATEWAY="192.168.1.1"
#修改完后，重启网卡
[root@oracle network-scripts]#  service network restart
Shutting down interface eth0:  Device state: 3 (disconnected)
[  OK  ]
Shutting down loopback interface:  [  OK  ]
Bringing up loopback interface:  [  OK  ]
Bringing up interface eth0:  Active connection state: activating
Active connection path: /org/freedesktop/NetworkManager/ActiveConnection/4
state: activated
Connection activated
[  OK  ]
[root@oracle network-scripts]#
```



-==================================================================
