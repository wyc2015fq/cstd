# 云服务器 IPV6的配置 - qq_42302962的博客 - CSDN博客
2018年05月25日 15:05:55[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：524
相信大部分的做IOS开发的朋友，在App发布的时候都是需要将服务器做到支持IPV6的。
我今天就在做[NtyTcp](http://www.ntytcp.com/) DNS异步解析的时候，需要一个测试环境，所以搭建了一个IPV6的环境。
把过程列举出来。
一。主机内核需要支持IPv6
修改 /etc/sysctl.conf
net.ipv6.conf.all.disable_ipv6 = 0
net.ipv6.conf.default.disable_ipv6 = 0
net.ipv6.conf.lo.disable_ipv6 = 0
二。IPv6的Tunnel
因为国内大部分网络不支持IPv4，所以需要通过一个IPv6-IPv4 Tunnel
有耐心的朋友 可以自己使用openvpn自己搭建 Tunnel
推荐一个靠谱的 [https://tunnelbroker.net/](https://tunnelbroker.net/) ，注册创建通道ok，适合个人用户玩玩。
注册，添加tunnel以后，会有相关设置。本地 ipv6 本机ipv4 地址，远程ipv6，远程ipv4，以及网关ipv6.
配置的你的阿里云服务器。
三。配置系统 ipv6 虚拟网卡设备
这里有两种方式来添加虚拟网卡设备
1. /etc/network/interfaces，添加一下
![image.png](http://s1.51cto.com/images/20180420/1524237075181015.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2. 就是写一个shell脚本，给一个N年以前部署的案例。
# modprobe ipv6
# ip tunnel add cernet-ipv6 mode sit remote 222.28.155.24 local 112.93.116.189 ttl 255
# ip link set cernet-ipv6 up
# ip -6 addr add 2001:da8:20d:400::705d:74bd dev cernet-ipv6
# ip -6 route add ::/0 dev cernet-ipv6    
