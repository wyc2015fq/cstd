# Centos开机连接两个网卡问题解决 - BlueSky - CSDN博客
2015年12月24日 20:13:12[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1609
个人分类：[Linux技巧](https://blog.csdn.net/ASIA_kobe/article/category/5981797)
症状：      /etc/sysconfig/network-scripts/中只有ifcfg-lo文件，而没有ifcfg-eth0。  终端中执行命令ifconfig显示的信息为ifcfg-lo的信息。执行命令ifconfig eth0时，才可以显示出eth0对应的信息。使用命令ifconfig eth0 192.168.2.XX可以正常设置ip。但每次重启后必须再次手动执行if....命令设置ip地址。上网查了一下好多人都说是没有装网卡驱动的问题。其实并不是这样。 
 ======================================================================  解决方案：         1.在/etc/sysconfig/network-scripts/目中新建一个ifcfg-eth0文件，并正确配置信息，ip,network,hwaddr等等，DEVICE的值为eth0.        2.ifcfg-eth0中 ONBOOT=yes  按常理说,这样设置是可以自己激活的.  但如果你发现你的设置并没有成功激活网卡，那么修改下列文件 
 vi /etc/rc.d/rc.loacl  最后加入  ifup eth0。  重启电脑后执行命令ifconfig这时候就会显示出eth0和lo共2部分信息。其他电脑ping这台电脑也可以ping通。     
其中 ifcfg-em1添加信息为
### 2、配置外网网卡的网关 
修改网关的配置文件
[root@centos]# vi /etc/sysconfig/network
修改以下内容
NETWORKING=yes(表示系统是否使用网络，一般设置为yes。如果设为no，则不能使用网络，而且很多系统服务程序将无法启动)
HOSTNAME=centos(设置本机的主机名，这里设置的主机名要和/etc/hosts中设置的主机名对应)
GATEWAY=192.168.1.1(**注意：这里需要配置为外网网卡的网关**)
### 3、配置IP地址
修改对应网卡的IP地址的配置文件
# vi /etc/sysconfig/network-scripts/ifcfg-eth0
修改以下内容
DEVICE=eth0 #描述网卡对应的设备别名，例如ifcfg-eth0的文件中它为eth0，Dell服务器的一般为：em1、em2
BOOTPROTO=static #设置网卡获得ip地址的方式，可能的选项为static，dhcp或bootp，分别对应静态指定的 ip地址，通过dhcp协议获得的ip地址，通过bootp协议获得的ip地址
BROADCAST=192.168.0.255 #对应的子网广播地址
HWADDR=00:07:E9:05:E8:B4 #对应的网卡物理地址
IPADDR=12.168.1.2 #如果设置网卡获得 ip地址的方式为静态指定，此字段就指定了网卡对应的ip地址
**GATEWAY=#注意：内网网卡这里必须为空，否则两个网卡同时启用后上不了外网**
IPV6INIT=no
IPV6_AUTOCONF=no
NETMASK=255.255.255.0 #网卡对应的网络掩码
NETWORK=192.168.1.0 #网卡对应的网络地址
ONBOOT=yes #系统启动时是否设置此网络接口，设置为yes时，系统启动时激活此设备
### 4、重新启动网络配置
# service network restart 
或
# /etc/init.d/network restart
