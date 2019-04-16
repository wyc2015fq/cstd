# 转载和积累系列 - CentOS 网络配置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年08月28日 15:59:37[initphp](https://me.csdn.net/initphp)阅读数：823








在win下安装了VM虚拟机，并且安装CentOS系统，配置网络的时候经常用的有两种选择：桥接和NET方式

**桥接：**在虚拟机中使用桥接之后，系统会获得和win物理机同一个网段上，但是会分配不同的IP地址，对外是两个IP。

**NET方式：**net方式是虚拟机和WIN主机共用一个对外的IP地址，对外是一个IP。

VM虚拟机可以选择网络链接的方式：

![](https://img-blog.csdn.net/20140828155006422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




## 配置网络

1. 网络关闭，开启和重启操作


`service network restart|start|stop`

2.修改主机名



打开：vi /etc/sysconfig/network

编辑：



```
NETWORKING=yes　　　　　 #使用网络
HOSTNAME=centos　　　　　#设置主机名称
```


3. 修改网卡配置



网卡配置在/etc/sysconfig/network-scripts/目录下面，选择和网卡名称一样的ifcfg-*文件，然后打开编辑



```
DEVICE=eth0　　　　　　　　　　#对应第一张网卡
　　TYPE=Ethernet
　　ONBOOT=yes　　　　　　　　　　#是否启动时运行
　　NM_CONTROLLED=yes
　　BOOTPROTO=static　　　　　　　#使用静态IP，而不是由DHCP分配IP
　　DEFROUTE=yes
　　IPV4_FAILURE_FATAL=yes
　　IPV6INIT=no
　　NAME="System eth0"　　　　　　#名称
　　HWADDR=00:50:56:94:04:3C　 #必须对应etho是的MAC地址（/etc/udev/rules.d/70-persistent-net.rules）
　　PEERDNS=yes
　　PEERROUTES=yes
　　IPADDR=192.168.1.40　　　　　#指定本机IP地址
　　NETMASK=255.255.255.0　　　 #指定子网掩码
　　GATEWAY=192.168.1.1　　　　 #指定网关
```



ONBOOT是在启动的时候运行网卡，一般情况下需要设置为yes

BOOTPROTO 是设置获取IP地址的方式，如果是static静态方式 就是自己手动设置IP地址，那么就需要设置IPADDR，NETMASK，GATEWAY的值；如果是dhcp则会自动获取IP，但是每次电脑重启后 IP地址可能会变。我是在win下安装虚拟机，所以用**net方式**并且可以设置**static**的IP地址获取方式，这样每次去访问都同一个IP。




4. 修改DNS

可以打开 /etc/resolv.conf 文件，配置DNS服务器


`nameserver 8.8.8.8　　　　　　#GOOGLE的DNS服务器`

5. 查看当前的ip




`ifconfig`

6. 测试是否外网通的




`ping baidu.com`









