# haproxy+keepalived实现高可用负载均衡 - 深之JohnChen的专栏 - CSDN博客

2014年03月16日 15:15:12[byxdaz](https://me.csdn.net/byxdaz)阅读数：2518标签：[负载均衡](https://so.csdn.net/so/search/s.do?q=负载均衡&t=blog)
个人分类：[WEB编程																[软件架构与设计](https://blog.csdn.net/byxdaz/article/category/573934)](https://blog.csdn.net/byxdaz/article/category/479812)


                
软件负载均衡一般通过两种方式来实现：基于操作系统的软负载实现和基于第三方应用的软负载实现。LVS就是基于Linux操作系统实现的一种软负载，HAProxy就是开源的并且基于第三应用实现的软负载。

HAProxy相比LVS的使用要简单很多，功能方面也很丰富。当 前，HAProxy支持两种主要的代理模式:"tcp"也即4层（大多用于邮件服务器、内部协议通信服务器等），和7层（HTTP）。在4层模式 下，HAProxy仅在客户端和服务器之间转发双向流量。7层模式下，HAProxy会分析协议，并且能通过允许、拒绝、交换、增加、修改或者删除请求 (request)或者回应(response)里指定内容来控制协议，这种操作要基于特定规则。

我现在用HAProxy主要在于它有以下优点，这里我总结下：

一、免费开源，稳定性也是非常好，这个可通过我做的一些小项目可以看出来，单Haproxy也跑得不错，稳定性可以与LVS相媲美；

二、根据官方文档，HAProxy可以跑满10Gbps-New benchmark of HAProxy at 10 Gbps using Myricom's 10GbE NICs (Myri-10G PCI-Express)，这个作为软件级负载均衡，也是比较惊人的；

三、HAProxy可以作为MySQL、邮件或其它的非web的负载均衡，我们常用于它作为MySQL(读)负载均衡；

四、自带强大的监控服务器状态的页面，实际环境中我们结合Nagios进行邮件或短信报警，这个也是我非常喜欢它的原因之一；

五、HAProxy支持虚拟主机。

===================================================================================

在做反向代理服务器的负载均衡时，我们通常会使用nginx的均衡配置。其实，haproxy的负载均衡也是属于这一类的。那么关于这方面的配置过程我们现在来进行一下讲解。首先，对haproxy进行一个简单的介绍，之后就是安装和配置环节了。

HAProxy介绍

反向代理服务器,支持双机热备支持虚拟主机,但其配置简单,拥有非常不错的服务器健康检查功能,当其代理的后端服务器出现故障, HAProxy会自动将该服务器摘除,故障恢复后再自动将该服务器加入｡新的1.3引入了frontend,backend；frontend根据任意 HTTP请求头内容做规则匹配,然后把请求定向到相关的backend.

http://blog.liuts.com/post/223/ （搭建四层负载均衡器）

http://rfyimcool.blog.51cto.com/1030776/413187 （搭建七层负载均衡器）

===================================================================================

keepalived简介　　

http://www.keepalived.org

keepalived是一个类似于layer3, 4 & 5交换机制的软件，也就是我们平时说的第3层、第4层和第5层交换。Keepalived的作用是检测web服务器的状态，如果有一台web服务器死机，或工作出现故障，Keepalived将检测到，并将有故障的web服务器从系统中剔除，当web服务器工作正常后Keepalived自动将web服务器加入到服务器群中，这些工作全部自动完成，不需要人工干涉，需要人工做的只是修复故障的web服务器。

类似的HA工具还有heatbeat、drbd等，heatbeat、drbd配置都较为复杂。

keepalived理论工作原理

keepalived可提供vrrp以及health-check功能，可以只用它提供双机浮动的vip（vrrp虚拟路由功能），这样可以简单实现一个双机热备高可用功能。

keepalived是一个类似于layer3, 4 & 5交换机制的软件，也就是我们平时说的第3层、第4层和第5层交换。Keepalived的作用是检测web 服务器的状态。 Layer3,4&5工作在IP/TCP协议栈的IP层，TCP层，及应用层,原理分别如下：

　　Layer3：Keepalived使用Layer3的方式工作式时，Keepalived会定期向服务器群中的服务器

　　发送一个ICMP的数据包（既我们平时用的Ping程序）,如果发现某台服务的IP地址没有激活，Keepalived便报告这台服务器失效，并将它从服务器群中剔除，这种情况的典型例子是某台服务器被非法关机。Layer3的方式是以服务器的IP地址是否有效作为服务器工作正常与否的标准。在本文中将采用这种方式。

　　Layer4:如果您理解了Layer3的方式，Layer4就容易了。Layer4主要以TCP端口的状态来决定服务器工作正常与否。如web server的服务端口一般是80，如果Keepalived检测到80端口没有启动，则Keepalived将把这台服务器从服务器群中剔除。

　　Layer5：Layer5就是工作在具体的应用层了，比Layer3,Layer4要复杂一点，在网络上占用的带宽也要大一些。Keepalived将根据用户的设定检查服务器程序的运行是否正常，如果与用户的设定不相符，则Keepalived将把服务器从服务器群中剔除。

vip即虚拟ip，是附在主机网卡上的，即对主机网卡进行虚拟，此IP仍然是占用了此网段的某个IP。

keepalived作用

　　随着你的网站业务量的增长你网站的服务器压力越来越大？需要负载均衡方案！商业的硬件如F5又太贵，你们又是创业型互联公司如何有效节约成本，节省不必要的浪费？同时实现商业硬件一样的高性能高可用的功能？有什么好的负载均衡可伸张可扩展的方案吗？答案是肯定的！有！我们利用 LVS+Keepalived基于完整开源软件的架构可以为你提供一个负载均衡及高可用的服务器。

　　LVS+Keepalived 介绍

　　LVS

　　LVS是Linux Virtual Server的简写，意即Linux虚拟服务器，是一个虚拟的服务器集群系统。本项目在1998年5月由章文嵩博士成立，是中国国内最早出现的自由软件项目之一.目前有三种IP负载均衡技术（VS/NAT、VS/TUN和VS/DR）八种调度算法（rr,wrr,lc,wlc,lblc,lblcr,dh,sh）。

　　Keepalvied

　　Keepalived在这里主要用作RealServer的健康状态检查以及LoadBalance主机和BackUP主机之间failover的实现。keepalived简介　　keepalived是一个类似于layer3, 4 & 5交换机制的软件，也就是我们平时说的第3层、第4层和第5层交换。Keepalived的作用是检测web服务器的状态，如果有一台web服务器死机，或工作出现故障，Keepalived将检测到，并将有故障的web服务器从系统中剔除，当web服务器工作正常后Keepalived自动将web服务器加入到服务器群中，这些工作全部自动完成，不需要人工干涉，需要人工做的只是修复故障的web服务器。

===================================================================================

Keepalived介绍

Keepalived是一个基于VRRP协议来实现的WEB 服务高可用方案，可以利用其来避免单点故障。一个WEB服务至少会有2台服务器运行Keepalived，一台为主服务器（MASTER），一台为备份服务器（BACKUP），但是对外表现为一个虚拟IP，主服务器会发送特定的消息给备份服务器，当备份服务器收不到这个消息的时候，即主服务器宕机的时候，备份服务器就会接管虚拟IP，继续提供服务，从而保证了高可用性。1 +-------------VIP(192.168.0.7)------------------+

2 | | | 

3 | | | 

4 server(MASTER) <----keepalived----> server(BACKUP)

5 (192.168.0.1) (192.168.0.2)

keepalived是VRRP的完美实现，因此在介绍keepalived之前，先介绍一下VRRP的原理。

VRRP协议简介

在现实的网络环境中，两台需要通信的主机大多数情况下并没有直接的物理连接。对于这样的情况，它们之间路由怎样选择？主机如何选定到达目的主机的下一跳路由，这个问题通常的解决方法有二种：

· 在主机上使用动态路由协议(RIP、OSPF等)

· 在主机上配置静态路由

很明显，在主机上配置路态路由是非常不切实际的，因为管理、维护成本以及是否支持等诸多问题。配置静态路由就变得十分流行，但路由器(或者说默认网关default gateway)却经常成为单点。

VRRP的目的就是为了解决静态路由单点故障问题。

VRRP通过一竞选(election)协议来动态的将路由任务交给LAN中虚拟路由器中的某台VRRP路由器。

工作机制

在一个VRRP虚拟路由器中，有多台物理的VRRP路由器，但是这多台的物理的机器并不能同时工作，而是由一台称为MASTER的负责路由工作，其它的都是BACKUP，MASTER并非一成不变，VRRP让每个VRRP路由器参与竞选，最终获胜的就是MASTER。MASTER拥有一些特权，比如 拥有虚拟路由器的IP地址，我们的主机就是用这个IP地址作为静态路由的。拥有特权的MASTER要负责转发发送给网关地址的包和响应ARP请求。

VRRP通过竞选协议来实现虚拟路由器的功能，所有的协议报文都是通过IP多播(multicast)包(多播地址 224.0.0.18)形式发送的。虚拟路由器由VRID(范围0-255)和一组IP地址组成，对外表现为一个周知的MAC地址。所以，在一个虚拟路由 器中，不管谁是MASTER，对外都是相同的MAC和IP(称之为VIP)。客户端主机并不需要因为MASTER的改变而修改自己的路由配置，对他们来 说，这种主从的切换是透明的。

在一个虚拟路由器中，只有作为MASTER的VRRP路由器会一直发送VRRP广告包(VRRPAdvertisement message)，BACKUP不会抢占MASTER，除非它的优先级(priority)更高。当MASTER不可用时(BACKUP收不到广告包)， 多台BACKUP中优先级最高的这台会被抢占为MASTER。这种抢占是非常快速的(<1s)，以保证服务的连续性。

由于安全性考虑，VRRP包使用了加密协议进行加密。

==========================================

vrrp简介

随着Internet的迅猛发展，基于网络的应用逐渐增多。这就对网络的可靠性提出了越来越高的要求。斥资对所有网络设备进行更新当然是一种很好的可靠性解决方案；但本着保护现有投资的角度考虑，可以采用廉价冗余的思路，在可靠性和经济性方面找到平衡点。

虚拟路由冗余协议就是一种很好的解决方案。在该协议中，对共享多存取访问介质（如以太网）上终端IP设备的默认网关(Default Gateway)进行冗余备份，从而在其中一台路由设备宕机时，备份路由设备及时接管转发工作，向用户提供透明的切换，提高了网络服务质量。

一、协议概述 

在基于TCP/IP协议的网络中，为了保证不直接物理连接的设备之间的通信，必须指定路由。目前常用的指定路由的方法有两种：一种是通过路由协议（比如：内部路由协议RIP和OSPF）动态学习；另一种是静态配置。在每一个终端都运行动态路由协议是不现实的，大多客户端操作系统平台都不支持动态路由协议，即使支持也受到管理开销、收敛度、安全性等许多问题的限制。因此普遍采用对终端IP设备静态路由配置，一般是给终端设备指定一个或者多个默认网关(Default Gateway)。静态路由的方法简化了网络管理的复杂度和减轻了终端设备的通信开销，但是它仍然有一个缺点：如果作为默认网关的路由器损坏，所有使用该网关为下一跳主机的通信必然要中断。即便配置了多个默认网关，如不重新启动终端设备，也不能切换到新的网关。采用虚拟路由冗余协议
 (Virtual Router Redundancy Protocol，简称VRRP)可以很好的避免静态指定网关的缺陷。 

在VRRP协议中，有两组重要的概念：VRRP路由器和虚拟路由器，主控路由器和备份路由器。VRRP路由器是指运行VRRP的路由器，是物理实体，虚拟路由器是指VRRP协议创建的，是逻辑概念。一组VRRP路由器协同工作，共同构成一台虚拟路由器。该虚拟路由器对外表现为一个具有唯一固定IP地址和MAC地址的逻辑路由器。处于同一个VRRP组中的路由器具有两种互斥的角色：主控路由器和备份路由器，一个VRRP组中有且只有一台处于主控角色的路由器，可以有一个或者多个处于备份角色的路由器。VRRP协议使用选择策略从路由器组中选出一台作为主控，负责ARP相应和转发IP数据包，组中的其它路由器作为备份的角色处于待命状态。当由于某种原因主控路由器发生故障时，备份路由器能在几秒钟的时延后升级为主路由器。由于此切换非常迅速而且不用改变IP地址和MAC地址，故对终端使用者系统是透明的。

二、工作原理 

一个VRRP路由器有唯一的标识：VRID，范围为0—255。该路由器对外表现为唯一的虚拟MAC地址，地址的格式为00-00-5E-00-01-[VRID]。主控路由器负责对ARP请求用该MAC地址做应答。这样，无论如何切换，保证给终端设备的是唯一一致的IP和MAC地址，减少了切换对终端设备的影响。

VRRP控制报文只有一种：VRRP通告(advertisement)。它使用IP多播数据包进行封装，组地址为224.0.0.18，发布范围只限于同一局域网内。这保证了VRID在不同网络中可以重复使用。为了减少网络带宽消耗只有主控路由器才可以周期性的发送VRRP通告报文。备份路由器在连续三个通告间隔内收不到VRRP或收到优先级为0的通告后启动新的一轮VRRP选举。

在VRRP路由器组中，按优先级选举主控路由器，VRRP协议中优先级范围是0—255。若VRRP路由器的IP地址和虚拟路由器的接口IP地址相同，则称该虚拟路由器作VRRP组中的IP地址所有者；IP地址所有者自动具有最高优先级：255。优先级0一般用在IP地址所有者主动放弃主控者角色时使用。可配置的优先级范围为1—254。优先级的配置原则可以依据链路的速度和成本、路由器性能和可靠性以及其它管理策略设定。主控路由器的选举中，高优先级的虚拟路由器获胜，因此，如果在VRRP组中有IP地址所有者，则它总是作为主控路由的角色出现。对于相同优先级的候选路由器，按照IP地址大小顺序选举。VRRP还提供了优先级抢占策略，如果配置了该策略，高优先级的备份路由器便会剥夺当前低优先级的主控路由器而成为新的主控路由器。

为了保证VRRP协议的安全性，提供了两种安全认证措施：明文认证和IP头认证。明文认证方式要求：在加入一个VRRP路由器组时，必须同时提供相同的VRID和明文密码。适合于避免在局域网内的配置错误，但不能防止通过网络监听方式获得密码。IP头认证的方式提供了更高的安全性，能够防止报文重放和修改等攻击。

三、 应用实例 

最典型的VRRP应用：RTA、RTB组成一个VRRP路由器组，假设RTB的处理能力高于RTA，则将RTB配置成IP地址所有者，H1、H2、H3的默认网关设定为RTB。则RTB成为主控路由器，负责ICMP重定向、ARP应答和IP报文的转发；一旦RTB失败，RTA立即启动切换，成为主控，从而保证了对客户透明的安全切换。

在VRRP应用中，RTA在线时RTB只是作为后备，不参与转发工作，闲置了路由器RTA和链路L1。通过合理的网络设计，可以到达备份和负载分担双重效果。让RTA、RTB同时属于互为备份的两个VRRP组：在组1中RTA为IP地址所有者；组2中RTB为IP地址所有者。将H1的默认网关设定为RTA；H2、H3的默认网关设定为RTB。这样，既分担了设备负载和网络流量，又提高了网络可靠性。

VRRP协议的工作机理与CISCO公司的HSRP（Hot Standby Routing Protocol）有许多相似之处。但二者主要的区别是在CISCO的HSRP中，需要单独配置一个IP地址作为虚拟路由器对外体现的地址，这个地址不能是组中任何一个成员的接口地址。

使用VRRP协议，不用改造目前的网络结构，最大限度保护了当前投资，只需最少的管理费用，却大大提升了网络性能，具有重大的应用价值。

===================================================================================

keepalive的简单应用——管理VIP的飘动 

from:http://www.cnblogs.com/killkill/archive/2010/12/31/1922360.html

VIP的飘动可以为我们解决很多问题，以前我试过使用ifup/ifdown的方式控制网卡的up/down来实现，这种方式有个小问题，就是每次VIP 飘动之后都要等上几十秒才能生效，感觉时间比较长，而且还要配合一些逻辑脚本才能很好地工作，有没有更好的方法呢？当然有，这就是本文的主角—— keepalived。

安装很简单：1 tar zxvf keepalived-1.1.20.tar.gz 

2 cd keepalived-1.1.20

3 ./configure --prefix=/

4 make

5 make install

修改一下 /etc/keepalived/keepalived.conf 这个配置文件就可以用了，以下是我的环境，192.168.10.141和192.168.10.142是两个VIP，可以在两台服务器之间飘动：

主机的配置：01 global_defs {

02 notification_email {

03 failover@firewall.loc

04 }

05 notification_email_from Alexandre.Cassen@firewall.loc

06 smtp_server 192.168.0.48

07 smtp_connect_timeout 10

08 router_id nginx

09 }

10 

11 vrrp_instance VI_141 {

12 state BACKUP

13 interface eth0

14 virtual_router_id 141

15 priority 50

16 advert_int 1

17 authentication {

18 auth_type PASS

19 auth_pass 141

20 }

21 virtual_ipaddress {

22 192.168.10.141/26 dev eth0

23 }

24 }

25 

26 vrrp_instance VI_142 {

27 state BACKUP

28 interface eth0

29 virtual_router_id 142

30 priority 100

31 advert_int 1

32 authentication {

33 auth_type PASS

34 auth_pass 142

35 }

36 virtual_ipaddress {

37 192.168.10.142/26 dev eth0

38 }

39 }

备机的配置：01 global_defs {

02 notification_email {

03 failover@firewall.loc

04 }

05 notification_email_from Alexandre.Cassen@firewall.loc

06 smtp_server 10.168.0.48

07 smtp_connect_timeout 10

08 router_id nginx

09 }

10 

11 vrrp_instance VI_141 {

12 state BACKUP

13 interface eth0

14 virtual_router_id 141

15 priority 100

16 advert_int 1

17 authentication {

18 auth_type PASS

19 auth_pass 141

20 }

21 virtual_ipaddress {

22 192.168.10.141/26 dev eth0

23 }

24 }

25 

26 vrrp_instance VI_142 {

27 state BACKUP

28 interface eth0

29 virtual_router_id 142

30 priority 50

31 advert_int 1

32 authentication {

33 auth_type PASS

34 auth_pass 142

35 }

36 virtual_ipaddress {

37 192.168.10.142/26 dev eth0

38 }

39 }

乍一看，主机和备机的配置文件是一样的，仔细看一下priority的值，使用以下命令即可将keepalived加入linux的服务中：1 chkconfig --add keepalived ;

通过启、停keepalived这个服务即可观察到VIP的飘动，至于为什么VIP飘动后可以很快地生效，还有待研究。

===================================================================================

haproxy+keepalived实现高可用负载均衡

我的环境：

haproxy keepalived 主：192.168.1.192

haproxy keepalived 备：192.168.1.193

vip：192.168.1.200

web：192.168.1.187:80 192.168.1.187:8000

一：安装过程，在192.168.1.192上：

keepalived的安装:

#tar -zxvf keepalived-1.1.17.tar.gz

#ln -s /usr/src/kernels/2.6.18-128.el5-i686/ /usr/src/linux

#cd keepalived-1.1.17

#./configure --prefix=/ --mandir=/usr/local/share/man/ --with-kernel-dir=/usr/src/kernels/2.6.18-128.el5-i686/

#make && make install

#cd /etc/keepalived/

#mv keepalived.conf keepalived.conf.default

#vi keepalived.conf

! Configuration File for keepalived

vrrp_script chk_http_port {

script "/etc/keepalived/check_haproxy.sh"

interval 2

weight 2

global_defs {

router_id LVS_DEVEL

}

vrrp_instance VI_1 {

state MASTER #192.168.1.193上改为BACKUP

interface eth0

virtual_router_id 51 

priority 150 #192.168.1.193上改为120

advert_int 1

authentication {

auth_type PASS

auth_pass 1111

}

track_script {

chk_http_port

}

virtual_ipaddress {

192.168.1.200 

}

}

}

#vi /etc/keepalived/check_haproxy.sh

#!/bin/bash

A=`ps -C haproxy --no-header |wc -l`

if [ $A -eq 0 ];then

/usr/local/haproxy/sbin/haproxy -f /usr/local/haproxy/conf/haproxy.cfg

sleep 3

if [ `ps -C haproxy --no-header |wc -l` -eq 0 ];then

/etc/init.d/keepalived stop

fi

fi

#chmod 755 /etc/keepalived/check_haproxy.sh

haproxy的安装(主备都一样)：

#tar -zxvf haproxy-1.4.9.tar.gz

#cd haproxy-1.4.9

#make TARGET=linux26 PREFIX=/usr/local/haproxy install

#cd /usr/local/haproxy/

#mkdir conf logs

#cd conf

#vi haproxy.cfg

global

log 127.0.0.1 local3 info

maxconn 4096

user nobody

group nobody

daemon

nbproc 1

pidfile /usr/local/haproxy/logs/haproxy.pid

defaults

maxconn 2000

contimeout 5000

clitimeout 30000

srvtimeout 30000

mode http

log global

log 127.0.0.1 local3 info

stats uri /admin?stats

option forwardfor

frontend http_server

bind :80

log global

default_backend info_cache

acl test hdr_dom(host) -i test.domain.com

use_backend cache_test if test

backend info_cache

#balance roundrobin

balance source

option httpchk HEAD /haproxy.txt HTTP/1.1\r\nHost:192.168.1.187

server inst2 192.168.1.187:80 check inter 5000 fall 3

backend cache_test

balance roundrobin

#balance source

option httpchk HEAD /haproxy.txt HTTP/1.1\r\nHost:test.domain.com

server inst1 192.168.1.187:8000 check inter 5000 fall 3

二：再两台机器上都分别启动：

/etc/init.d/keepalived start （这条命令会自动把haproxy启动）

三：测试：

1.再两台机器上分别执行ip add

主: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000

link/ether 00:0c:29:98:cd:c0 brd ff:ff:ff:ff:ff:ff

inet 192.168.1.192/24 brd 192.168.1.255 scope global eth0

inet 192.168.1.200/32 scope global eth0

inet6 fe80::20c:29ff:fe98:cdc0/64 scope link

valid_lft forever preferred_lft forever

备: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000

link/ether 00:0c:29:a6:0c:7e brd ff:ff:ff:ff:ff:ff

inet 192.168.1.193/24 brd 255.255.255.254 scope global eth0

inet6 fe80::20c:29ff:fea6:c7e/64 scope link

valid_lft forever preferred_lft forever

2.停掉主上的haproxy，3秒后keepalived会自动将其再次启动

3.停掉主的keepalived，备机马上接管服务

备: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000

link/ether 00:0c:29:a6:0c:7e brd ff:ff:ff:ff:ff:ff

inet 192.168.1.193/24 brd 255.255.255.254 scope global eth0

inet 192.168.1.200/32 scope global eth0

inet6 fe80::20c:29ff:fea6:c7e/64 scope link

valid_lft forever preferred_lft forever

4.更改hosts

192.168.1.200 test.com

192.168.1.200 test.domain.com

通过IE测试，可以发现

test.com的请求发向了192.168.1.187:80

test.domain.com的请求发向了192.168.1.187:8000

