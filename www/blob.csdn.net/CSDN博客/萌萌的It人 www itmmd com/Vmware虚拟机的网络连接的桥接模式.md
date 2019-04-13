
# Vmware虚拟机的网络连接的桥接模式 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月15日 22:26:41[Jlins](https://me.csdn.net/dyllove98)阅读数：1310


VMware虚拟机安装完以后会提供三种网络模式，对应网络连接名称分别是
Bridged(桥接模式)             VMnet0
NAT(网络地址转换模式)      VMnet8
Host-only(主机模式)           VMnet1
桥接模式是为虚拟机提供的操作系统和宿主系统提供真实的局域环境，可以理解为运行在同一台计算机的两个独立的操作系统，虚拟机主机是局域网的另外一台机器而已。
![wKiom1RnWE_ie50zAAGLwO84Gzk221.jpg](http://s3.51cto.com/wyfs02/M00/53/72/wKiom1RnWE_ie50zAAGLwO84Gzk221.jpg)
宿主系统
|1
|2
|3
|IPv4 地址 . . . . . . . . . . . . : 192.168.2.52
|子网掩码  . . . . . . . . . . . . : 255.255.255.0
|默认网关. . . . . . . . . . . . . : 192.168.2.1
|

虚拟机系统
|1
|2
|3
|4
|5
|6
|DEVICE=eth0
|ONBOOT=|yes
|BOOTPROTO=static
|IPADDR=192.168.2.99
|NETMASK=255.255.255.0
|GATEWAY=192.168.2.1
|

把它们当成局域网的两台不同的设备配置即可，当ping出现destination host unreachable的时候，请检查虚机上述网络连接VMnet0 是否选择了网络适配器
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

