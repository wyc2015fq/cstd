# MySQL 主主复制 + LVS + Keepalived 实现 MySQL 高可用性 - 文章 - 伯乐在线
原文出处： [bestvivi](http://bestvivi.com/2015/09/09/MySQL%E4%B8%BB%E4%B8%BB%E5%A4%8D%E5%88%B6+LVS+Keepalived%E5%AE%9E%E7%8E%B0MySQL%E9%AB%98%E5%8F%AF%E7%94%A8%E6%80%A7)
[MySQL复制](http://blog.jobbole.com/100393/)能够保证数据的冗余的同时可以做读写分离来分担系统压力，如果是主主复制还可以很好的避免主节点的单点故障。但是MySQL主主复制存在一些问题无法满足我们的实际需要：未提供统一访问入口来实现负载均衡，如果其中master宕掉的话需要手动切换到另外一个master，而不能自动进行切换。
这篇文章下面要介绍如何通过LVS+Keepalived的方式来是实现MySQL的高可用性，同时解决以上问题。
## Keepalived和LVS介绍
Keepalived是一个基于[VRRP](http://datatracker.ietf.org/wg/vrrp/documents/)（虚拟路由冗余协议）可用来实现服务高可用性的软件方案，避免出现单点故障。Keepalived一般用来实现轻量级高可用性，且不需要共享存储，一般用于两个节点之间，常见有LVS+Keepalived、Nginx+Keepalived组合。
[LVS](http://www.linux-vs.org/index.html)(Linux Virtual Server)是一个高可用性虚拟的服务器集群系统。本项目在1998年5月由章文嵩博士成立，是中国国内最早出现的自由软件项目之一。
LVS主要用于多服务器的负载均衡，作用于网络层。LVS构建的服务器集群系统中，前端的负载均衡层被称为Director Server；后端提供服务的服务器组层被称为Real Server。通过下图可以大致了解LVS的基础架构。
![](/image?src=http://blogyangwei.qiniudn.com/mysql_lvs_keep01.png&objectId=1190000003727841&token=3063043e901139661c1c21a3c3c114b2)
LVS有三种工作模式，分别是DR（Direct Routing 直接路由）、TUN(Tunneling IP隧道）、NAT（Network Address Translation 网络地址转换）。其中TUN模式能够支持更多的Real Server，但需要所有服务器支持IP隧道协议；DR也可以支持相当的Real Server，但需要保证Director Server虚拟网卡与物理网卡在同一网段；NAT扩展性有限，无法支持更多的Real Server，因为所有的请求包和应答包都需要Director Server进行解析再生，影响效率。 同时，LVS负载均衡有10中调度算法，分别是rr、wrr、lc、wlc、lblc、lblcr、dh、sh、sed、nq
详细的LVS说明请参见 [传送门](http://bestvivi.com)
本文中将利用LVS实现MySQL的读写负载均衡，Keepalived避免节点出现单点故障。
## LVS+Keepalived配置
#### 环境准备
LVS1：192.168.1.2
LVS2：192.168.1.11
MySQL Server1：192.168.1.5
MySQL Server2：192.168.1.6
VIP：192.168.1.100
OS: CentOS 6.4
![](/image?src=http://blogyangwei.qiniudn.com/mysql_lvs_keep04.png&objectId=1190000003727841&token=23c48d1855dd8e3459c41bc266214cae)
**Keepalive安装**
[keepalived下载地址](http://www.keepalived.org/software/)
需要安装以下软件包


```
# yum install -y kernel-devel openssl openssl-devel
```
解压keepalived到/usr/local/并进入目录执行配置编译


```
# ./configure --prefix=/usr/local/keepalived --with-kernel-dir=/usr/src/kernels/2.6.32-431.5.1.el6.x86_64/
Keepalived configuration
------------------------
Keepalived version       : 1.2.13
Compiler                 : gcc
Compiler flags           : -g -O2
Extra Lib                : -lssl -lcrypto -lcrypt 
Use IPVS Framework       : Yes
IPVS sync daemon support : Yes
IPVS use libnl           : No
fwmark socket support    : Yes
Use VRRP Framework       : Yes
Use VRRP VMAC            : Yes
SNMP support             : No
SHA1 support             : No
Use Debug flags          : No
# make
```
默认情况下keepalived启动时会去/etc/keepalived目录下找配置文件，将需要的配置文件拷贝到指定位置


```
# cp /usr/local/keepalived/etc/rc.d/init.d/keepalived /etc/rc.d/init.d/
# cp /usr/local/keepalived/etc/sysconfig/keepalived /etc/sysconfig/
# cp /usr/local/keepalived/etc/keepalived/keepalived.conf /etc/keepalived/
# cp /usr/local/keepalived/sbin/keepalived /usr/sbin/
# chkconfig mysqld on
# chkconfig keepalived on
```
**LVS安装**
[ipvsadm下载地址](http://www.linuxvirtualserver.org/software/kernel-2.6/ipvsadm-1.26.tar.gz)
需要安装以下软件包


```
# yum install -y libnl* popt*
```
查看是否加载lvs模块


```
# modprobe -l |grep ipvs
```
解压安装


```
# ln -s /usr/src/kernels/2.6.32-431.5.1.el6.x86_64/ /usr/src/linux
# tar -zxvf ipvsadm-1.26.tar.gz
# make
```
LVS安装完成，查看当前LVS集群


```
# ipvsadm -L -n
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port Scheduler Flags
-> RemoteAddress:Port           Forward Weight ActiveConn InActConn
```
#### LVS+Keepalived配置
**搭建MySQL主主复制**
这里不再赘述，请参考[MySQL复制](http://blog.jobbole.com/100393/)
**配置Keepalived**
下面是LVS1节点（Keepalived主节点）上的Keepalived配置，LVS2类似


```
# vim /etc/keepalived/keepalived.conf    
! Configuration File for keepalived
global_defs {
   router_id LVS1
}
vrrp_instance VI_1 {
    state MASTER #指定instance初始状态，实际根据优先级决定.backup节点不一样
    interface eth0 #虚拟IP所在网
    virtual_router_id 51 #VRID，相同VRID为一个组，决定多播MAC地址
    priority 100 #优先级，另一台改为90.backup节点不一样
    advert_int 1  #检查间隔
    authentication {
        auth_type PASS  #认证方式，可以是pass或ha
        auth_pass 1111  #认证密码
    }
    virtual_ipaddress {
        192.168.1.100  #VIP
    }
}
virtual_server 192.168.1.100 3306 {
    delay_loop 6  #服务轮询的时间间隔
    lb_algo wrr  #加权轮询调度，LVS调度算法 rr|wrr|lc|wlc|lblc|sh|sh
    lb_kind DR   #LVS集群模式 NAT|DR|TUN，其中DR模式要求负载均衡器网卡必须有一块与物理网卡在同一个网段
    #nat_mask 255.255.255.0
    persistence_timeout 50  #会话保持时间
    protocol TCP  #健康检查协议
     ## Real Server设置，3306就是MySQL连接端口
    real_server 192.168.1.5 3306 {
        weight 3  ##权重
        TCP_CHECK {
            connect_timeout 3
            nb_get_retry 3
            delay_before_retry 3
            connect_port 3306
        }
    }
    real_server 192.168.1.6 3306 {
        weight 3
        TCP_CHECK {
            connect_timeout 3
            nb_get_retry 3
            delay_before_retry 3
            connect_port 3306
        }
    }
}
```
**配置LVS**
编写LVS启动脚本/etc/init.d/realserver


```
#!/bin/sh
VIP=192.168.1.100
. /etc/rc.d/init.d/functions
case "$1" in
# 禁用本地的ARP请求、绑定本地回环地址
start)
    /sbin/ifconfig lo down
    /sbin/ifconfig lo up
    echo "1" >/proc/sys/net/ipv4/conf/lo/arp_ignore
    echo "2" >/proc/sys/net/ipv4/conf/lo/arp_announce
    echo "1" >/proc/sys/net/ipv4/conf/all/arp_ignore
    echo "2" >/proc/sys/net/ipv4/conf/all/arp_announce
    /sbin/sysctl -p >/dev/null 2>&1
    /sbin/ifconfig lo:0 $VIP netmask 255.255.255.255 up #在回环地址上绑定VIP，设定掩码，与Direct Server（自身）上的IP保持通信
    /sbin/route add -host $VIP dev lo:0
    echo "LVS-DR real server starts successfully.n"
    ;;
stop)
    /sbin/ifconfig lo:0 down
    /sbin/route del $VIP >/dev/null 2>&1
    echo "1" >/proc/sys/net/ipv4/conf/lo/arp_ignore
    echo "2" >/proc/sys/net/ipv4/conf/lo/arp_announce
    echo "1" >/proc/sys/net/ipv4/conf/all/arp_ignore
    echo "2" >/proc/sys/net/ipv4/conf/all/arp_announce
echo "LVS-DR real server stopped.n"
    ;;
status)
    isLoOn=`/sbin/ifconfig lo:0 | grep "$VIP"`
    isRoOn=`/bin/netstat -rn | grep "$VIP"`
    if [ "$isLoON" == "" -a "$isRoOn" == "" ]; then
        echo "LVS-DR real server has run yet."
    else
        echo "LVS-DR real server is running."
    fi
    exit 3
    ;;
*)
    echo "Usage: $0 {start|stop|status}"
    exit 1
esac
exit 0
```
将lvs脚本加入开机自启动


```
# chmod +x /etc/init.d/realserver
# echo "/etc/init.d/realserver" >> /etc/rc.d/rc.local
```
分别启动LVS和keepalived


```
# service realserver start
# service keepalived start
```
注意此时网卡的变化，可以看到虚拟网卡已经分配到了realserver上。
此时查看LVS集群状态，可以看到集群下有两个Real Server，调度算法，权重等信息。ActiveConn代表当前Real Server的活跃连接数


```
# ipvsadm -ln
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port Scheduler Flags
  -> RemoteAddress:Port           Forward Weight ActiveConn InActConn
TCP  192.168.1.100:3306 wrr persistent 50
  -> 192.168.1.5:3306             Route   3      4          1         
  -> 192.168.1.6:3306             Route   3      0          2
```
此时LVS+Keepalived+MySQL主主复制已经搭建完成。
#### 测试验证
**功能性验证**
关闭MySQL Server2


```
# service mysqld stop
```
在LVS1查看/var/log/messages中关于keepalived日志，LVS1检测到了MySQL Server2宕机，同时LVS集群自动剔除了故障节点


```
Sep  9 13:50:53 192.168.1.2 Keepalived_healthcheckers[18797]: TCP connection to [192.168.1.6]:3306 failed !!!
Sep  9 13:50:53 192.168.1.2 Keepalived_healthcheckers[18797]: Removing service [192.168.1.6]:3306 from VS [192.168.1.100]:3306
```
从新启动MySQL Server2后自动将故障节点自动加入LVS集群


```
Sep  9 13:51:41 192.168.1.2 Keepalived_healthcheckers[18797]: TCP connection to [192.168.1.6]:3306 success.
Sep  9 13:51:41 192.168.1.2 Keepalived_healthcheckers[18797]: Adding service [192.168.1.6]:3306 to VS [192.168.1.100]:3306
```
关闭LVS1上的Keepalived（模拟宕机操作），查看LVS1上的日志，可以看到Keepalived移出了LVS1上的VIP


```
Sep  9 14:01:27 192.168.1.2 Keepalived[18796]: Stopping Keepalived v1.2.13 (09/09,2014)
Sep  9 14:01:27 192.168.1.2 Keepalived_healthcheckers[18797]: Removing service [192.168.1.5]:3306 from VS [192.168.1.100]:3306
Sep  9 14:01:27 192.168.1.2 Keepalived_healthcheckers[18797]: Removing service [192.168.1.6]:3306 from VS [192.168.1.100]:3306
Sep  9 14:01:27 192.168.1.2 Keepalived_vrrp[18799]: VRRP_Instance(VI_1) sending 0 priority
Sep  9 14:01:27 192.168.1.2 Keepalived_vrrp[18799]: VRRP_Instance(VI_1) removing protocol VIPs.
```
同时查看LVS2上日志，可以看到LVS2成为了Master，并接管了VIP


```
Sep  9 14:11:24 192.168.1.11 Keepalived_vrrp[7457]: VRRP_Instance(VI_1) Transition to MASTER STATE
Sep  9 14:11:25 192.168.1.11 Keepalived_vrrp[7457]: VRRP_Instance(VI_1) Entering MASTER STATE
Sep  9 14:11:25 192.168.1.11 Keepalived_vrrp[7457]: VRRP_Instance(VI_1) setting protocol VIPs.
Sep  9 14:11:25 192.168.1.11 Keepalived_vrrp[7457]: VRRP_Instance(VI_1) Sending gratuitous ARPs on eth0 for 192.168.1.100
Sep  9 14:11:25 192.168.1.11 Keepalived_healthcheckers[7456]: Netlink reflector reports IP 192.168.1.100 added
Sep  9 14:11:25 192.168.1.11 avahi-daemon[1407]: Registering new address record for 192.168.1.100 on eth0.IPv4.
Sep  9 14:11:30 192.168.1.11 Keepalived_vrrp[7457]: VRRP_Instance(VI_1) Sending gratuitous ARPs on eth0 for 192.168.1.100
```
在LVS2上查看LVS集群状态，一切正常。


```
# ipvsadm -ln
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port Scheduler Flags
  -> RemoteAddress:Port           Forward Weight ActiveConn InActConn
TCP  192.168.1.100:3306 wrr persistent 50
  -> 192.168.1.5:3306             Route   3      2          0         
  -> 192.168.1.6:3306             Route   3      1          0
```
## 总结
- MySQL主主复制是集群的基础，组成Server Array，其中每个节点作为Real Server。
- LVS服务器提供了负载均衡的作用，将用户请求分发到Real Server，一台Real Server故障并不会影响整个集群。
- Keepalived搭建主备LVS服务器，避免了LVS服务器的单点故障，出现故障时可以自动切换到正常的节点。
