# Ubuntu网络配置方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年12月01日 20:03:36[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2142








1)配置IP网关：sudo gedit /etc/network/interfaces 里面添加

auto lo

iface lo inet loopback

auto eth0

iface eth0 inet static

address 192.168.1.5

netmask 255.255.255.0

gateway 192.168.1.1

2)配置DNS： sudo gedit /etc/resolv.conf

nameserver 202.107.117.11



3)主机名：sudo gedit /etc/hostname 里面输入主机名。默认有个主机名，因此这个不是必须的。

4)重启网络：sudo /etc/init.d/networking restart

  也可以重启网卡(重启网卡对别的网卡无影响，更推荐一些)：

       sudo ifconfig eth0 down 

       sudo ifconfig eth0 up




