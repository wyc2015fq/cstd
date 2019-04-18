# centOS7 默认不能联网 - z69183787的专栏 - CSDN博客
2018年02月07日 11:35:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：178
通过VmWare安装 CentOS-7-x86_64-DVD-1511.iso ，安装完成后，连不上网。
需要修改 如下配置：
找到文件 /etc/sysconfig/network-scripts/ifcfg-eno16777736 
内容大致如下：
TYPE=Ethernet
BOOTPROTO=dhcp
DEFROUTE=yes
PEERDNS=yes
PEERROUTES=yes
IPV4_FAILURE_FATAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_PEERDNS=yes
IPV6_PEERROUTES=yes
IPV6_FAILURE_FATAL=no
NAME=eno16777736
UUID=0c12727d-ebdf-4cc4-a9bf-db8ddf083b85
DEVICE=eno16777736
ONBOOT=no
将最下处的配置项ONBOOT的值修改为 yes  ，然后reboot重启，即可。
