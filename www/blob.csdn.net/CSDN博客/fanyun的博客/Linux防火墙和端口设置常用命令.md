# Linux防火墙和端口设置常用命令 - fanyun的博客 - CSDN博客
2016年04月05日 11:07:54[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：10639
所属专栏：[](https://blog.csdn.net/column/details/.html)[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
一、 
查看端口和防火墙状态：#netstat -lnt |grep 80
1)        开启80端口命令：/sbin/iptables -I INPUT -p tcp --dport 80 -j ACCEPT
2)        保存配置命令：/etc/rc.d/init.d/iptables save
3)        重启服务命令：/etc/rc.d/init.d/iptables restart
4)        查看已经开放的端口： /etc/init.d/iptables status
* 打开指令  
iptables -A INPUT -p tcp -s 192.168.245.223--dport 3306 -j ACCEPT  
iptables -A INPUT -p tcp -s 192.168.245.223--dport 80 -j ACCEPT  
* 关闭指令  
iptables -D INPUT -p tcp -s xxx.xxx.xxx.xxx--dport 3306 -j ACCEPT
/etc/sysconfig/network 包括主机基本网络信息，用于系统启动
/etc/sysconfig/network-script/ 此目录下是系统启动最初始化网络的信息
/etc/sysconfig/network-script/ifcfg-eth0 网络配置信息
/etc/xinetd.conf 定义了由超级进程XINETD启动的网络服务
/etc/protocols 设定了主机使用的协议以及各个协议的协议号
/etc/services 设定了主机的不同端口的网络服务
/etc/sysconfig/iptables 防火墙配置信息
防火墙配置命令
关闭
/etc/rc.d/init.d/iptables stop
开启
/etc/rc.d/init.d/iptables start
查看当前配置：iptables -L
redhat :
chkconfig --level 2345 iptables off
service iptables stop
但是不推荐关闭防火墙
1) 重启后生效
开启： chkconfig iptables on
关闭： chkconfig iptables off
2) 即时生效，重启后失效
开启： service iptables start
关闭： service iptables stop
需要说明的是对于Linux下的其它服务都可以用以上命令执行开启和关闭操作。
在开启了防火墙时，做如下设置，开启相关端口，
修改/etc/sysconfig/iptables 文件，添加以下内容：
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT
将网卡禁用
　　ifconfig eth0 down
将网卡启用
　　ifconfig eth0 up
修改Ip地址
方法一
ifconfig eth0 1.2.3.4 netmask 255.0.0.0 up
重新启动网络
service network restart
方法二：纯修改配置文件修改配置
cd  /etc/sysconfig/network-scripts
vi ifcfg-eth0    #拿我机子举例，我只有一块网卡，就是eth0
==============================   
DEVICE=eth0
BOOTPROTO=static    #这里将auto改为static即自动改为静态
BROADCAST=192.168.2.255  #这里修改为你设置的局域网广播地址（可以不写这行）
HWADDR=00:0C:29:1D:9F:22 #这里是硬件地址（可以不写这行）
IPADDR=192.168.2.173  #这里写上你要设置的IP地址。
NETMASK=255.255.255.0 #掩码
NETWORK=192.168.2.0 #网络号
ONBOOT=yes  #开机即启动网卡。
TYPE=Ethernet #这是类型，当然也可以不写这一行。
=======================
DNS修改
编辑文件vi /etc/resolv.conf 
 /* 使用route 命令配置路由表 */
//添加到主机路由
# route add –host 192.168.168.110 dev eth0:1
# route add –host 192.168.168.119 gw 192.168.168.1
//添加到网络的路由
# route add –net IP netmask MASK eth0
# route add –net IP netmask MASK gw IP
# route add –net IP/24 eth1
//添加默认网关
# route add default gw IP
//删除路由
# route del –host 192.168.168.110 dev eth0:1
/* 常用命令 */
# traceroute www.baidu.com
# ping www.baidu.com
//显示网络接口状态信息
# netstat –I
//显示所有监控的服务器的Socket和正在使用Socket的程序信息
# netstat –lpe
//显示内核路由表信息
# netstat –r
# netstat –nr
//显示TCP/UDP传输协议的连接状态
# netstat –t
# netstat –u
//更改主机名
# hostname myhost
//查看ARP缓存
# arp
//添加
# arp –s IP MAC
//删除
# arp –d IP
 /* 运行级别与网络服务 */
//查看当前运行级别
# runlevel
//运行级别的切换
# init
# telinit
