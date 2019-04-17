# CentOS---网络配置详解 - 在思索中前行！ - CSDN博客





2014年09月17日 20:56:23[_Tham](https://me.csdn.net/txl16211)阅读数：616标签：[Centos																[网络配置](https://so.csdn.net/so/search/s.do?q=网络配置&t=blog)](https://so.csdn.net/so/search/s.do?q=Centos&t=blog)
个人分类：[linux学习																[计算机网络](https://blog.csdn.net/txl16211/article/category/2492201)](https://blog.csdn.net/txl16211/article/category/2231071)







# 一、配置文件详解
在RHEL或者CentOS等Redhat系的Linux系统里，跟网络有关的主要设置文件如下：


/etc/host.conf         配置域名服务客户端的控制文件

/etc/hosts             完成主机名映射为IP地址的功能

/etc/resolv.conf       域名服务客户端的配置文件,用于指定域名服务器的位置

/etc/sysconfig/network 包含了主机最基本的网络信息,用于系统启动.

/etc/sysconfig/network-script/    系统启动时初始化网络的一些信息

/etc/xinetd.conf       定义了由超级进程xinetd启动的网络服务

/etc/networks          完成域名与网络地址的映射

/etc/protocols         设定了主机使用的协议以及各个协议的协议号

/etc/services          设定主机的不同端口的网络服务 


1. /etc/host.conf文件的默认信息如下：


multi on           #允许主机拥有多个IP地址

order hosts,bind   #主机名解析顺序，即本地解析,DNS域名解析的顺序


这个文件一般不需要我们修改，默认的解析顺序是本地解析,DNS服务器解析，也就是说在本系统里对于一个主机名首先进行本地解析，如果本地解析没有，然后进行DNS服务器解析。

2. /etc/hosts文件默认的内容大概如下：


127.0.0.1   butbueatiful   localhost.localdomain localhost

::1             localhost6.localdomain6 localhost6


可见，默认的情况是本机ip和本机一些主机名的对应关系，第一行是ipv4信息，第二行是ipv6信息，如果用不上ipv6本机解析，一般把该行注释掉。

第一行的解析效果是，butbueatiful localhost.localdomain localhost都会被解析成127.0.0.1,我们可以用ping试试。

[root@butbueatiful ~]# ping -c 3 butbueatiful

PING butbueatiful (127.0.0.1) 56(84) bytes of data.

64 bytes from butbueatiful (127.0.0.1): icmp_seq=1 ttl=64 time=0.061 ms

64 bytes from butbueatiful (127.0.0.1): icmp_seq=2 ttl=64 time=0.052 ms

64 bytes from butbueatiful (127.0.0.1): icmp_seq=3 ttl=64 time=0.051 ms


--- butbueatiful ping statistics ---

3 packets transmitted, 3 received, 0% packet loss, time 1999ms

rtt min/avg/max/mdev = 0.051/0.054/0.061/0.009 ms


[root@butbueatiful ~]# ping -c 3 localhost.localdomain

PING butbueatiful (127.0.0.1) 56(84) bytes of data.

64 bytes from butbueatiful (127.0.0.1): icmp_seq=1 ttl=64 time=0.055 ms

64 bytes from butbueatiful (127.0.0.1): icmp_seq=2 ttl=64 time=0.035 ms

64 bytes from butbueatiful (127.0.0.1): icmp_seq=3 ttl=64 time=0.050 ms


--- butbueatiful ping statistics ---

3 packets transmitted, 3 received, 0% packet loss, time 1999ms

rtt min/avg/max/mdev = 0.035/0.046/0.055/0.011 ms


看到上面的结果，你可能会问为什么ping localhost.localdomain的时候，下面显示的是却是butbueatiful，这是因为第一个主机名butbueatiful后面的那些主机名其实都是butbueatiful的主机别名。


如果我们要追加新的本地解析，比如我们希望在我们的机器里把yyyy.com和www.yyyy.com都解析成192.168.0.100，那么就追加如下一句即可：

192.168.0.100 yyyy.com www.yyyy.com


同样，在这里，www.yyyy.com是yyyy.com的主机别名。


如果你仔细一想，会发现，其实这个文件是很危险的，如果有人恶意修改了你这个文件，比如把淘宝的网站域名解析到了他的钓鱼网站，那你就要中招了。

3. /etc/resolv.conf, 指定域名解析的DNS服务器IP等信息, 配置参数一般接触到的有4个：


nameserver    指定DNS服务器的IP地址

domain        定义本地域名信息

search        定义域名的搜索列表

sortlist      对gethostbyname返回的地址进行排序


但是最常用的配置参数是nameserver，其他的可以不设置，这个参数指定了DNS服务器的IP地址，如果设置不正确，就无法进行正常的域名解析。

一般来说，推荐设置2个DNS服务器，比如我们用google的免费DNS服务器，那么该文件的设置内容如下：


nameserver 8.8.8.8

nameserver 8.8.4.4


同样，这个文件也是危险的，如果被人恶意改成了他自己的DNS服务器，他就可以为所欲为的控制你通过域名访问的每个目的地了，这就是常说的DNS劫持。

4. /etc/sysconfig/network, 典型的配置如下：


NETWORKING=yes

NETWORKING_IPV6=no

HOSTNAME=butbueatiful

GATEWAY=192.168.0.1


参数简要解释：


NETWORK          设置网络是否有效，yes有效，no无效

NETWORKING_IPV6  设置ipv6网络是否有效，yes有效，no无效

HOSTNAME         设置服务器的主机名，最好和/etc/hosts里设置一样，否则在使用一些程序的时候会有问题。

GATEWAY          指定默认网关IP

5. ifcfg-ethX, 设置对应网口的IP等信息, 比如第一个网口, 那么就是/etc/sysconfig/network-scripts/ifcfg-eth0，配置例子：


DEVICE="eth0"

BOOTPROTO="static"

BROADCAST="192.168.0.255"

HWADDR="00:16:36:1B:BB:74"

IPADDR="192.168.0.100"

NETMASK="255.255.255.0"

ONBOOT="yes"


参数简要解释：


DEVICE        设备名，不要自己乱改，和文件ifcfg-ethX里的ethX要一致

BROADCAST     广播地址

HWADDR        物理地址，这个你不要乱改

IPADDR        IP地址

NETMASK       子网掩码

ONBOOT        启动或者重启网络时，是否启动该设备，yes是启动，no是不启动

BOOTPROTO     开机协议，最常见的三个参数如下：

              static(静态IP)

              none(不指定,设置固定ip的情况，这个也行，但是如果要设定多网口绑定bond的时候，必须设成none）

              dhcp(动态获得IP相关信息）

6. route-ethX,比如第一个网口eth0的路由信息,那么就是/etc/sysconfig/network-scripts/route-eth0：


比如我们现在有这样一个需求，通过eth0去网络172.17.27.0/24不走默认路由，需要走192.168.0.254，那么我们第一反应，肯定是用route命令追加路由信息：

[root@butbueatiful ~]# route add -net 172.17.27.0 netmask 255.255.255.0 gw 192.168.0.254 dev eth0


可是，你没意识到的是，这样只是动态追加的而已，重启网络后，路由信息就消失了，所以需要设置静态路由，这时候就要设置/etc/sysconfig/network-scripts/route-eth0文件了，如果没有该文件，你就新建一个：

[root@butbueatiful ~]# vi /etc/sysconfig/network-scripts/route-eth0

#追加

172.17.27.0/24via 192.168.0.254


这下即使重启网络，重启系统，该路由也会自动加载，当然了，如果你没有这样的需要，那么这个文件就没必要创建和配置了。


# 二、常用的网络配置


伴随着时间的推移Red Hat公司推出了RHEL6.2，随后CentOS也紧随其后退出了CentOS6.2。新的系统中厂商加入了大量虚拟化及云计算的元素，同时对于细节的改变也不少，这里我们仅对新系统中的网络参数做以详尽说明。


Linux中网络参数大致包含以下内容：


IP地址

子网掩码

网关

DNS服务器

主机名


历来Linux系统中修改这些参数的方式通常有：命令、文件两种。其中通过命令设置可以立即生效但重启后将失效，通过文件修改实现永久生效，但不会立即生效。


首先我们来看看命令的方式：


ifconfig：查看与设置IP地址、子网掩码

hostname：查看与设置主机名

route：     查看与设置路由信息（默认网关等）


通过文件的方式修改：


/etc/sysconfig/network-scripts/ifcfg-设备名（通常为ifcfg-eth0)

/etc/sysconfig/network

/etc/resolv.conf文件：设置DNS服务器


以上种种这些方式可以同时在5.0与6.0系统中实现，但6.0系统后官方文档中描述说：ifconfig与route是非常陈旧的命令，取而代之的是ip命令。


那么我们先看一下老的命令使用方式：

*************************************************************************

ifconfig    接口  选项|地址


# ifconfig  eth0  up          # 开启eth0网卡

# ifconfig  eth0  down        # 关闭eth0网卡

# ifconfig  eth0  -arp        # 关闭eth0网卡arp协议

# ifconfig  eth0  promisc     # 开启eth0网卡的混合模式

# ifconfig  eth0  mtu 1400    # 设置eth0网卡的最大传输单元为1400

# ifconfig  eth0  192.168.0.2/24    # 设置eth0网卡IP地址

# ifconfig  eth0  192.168.0.2  netmask 255.255.255.0    # 功能同上


*************************************************************************

主机名：


# hostname        # 查看主机名

# hostname  butbueatiful.com    # 设置主机名为butbueatiful.com


*************************************************************************

网关设置：


route  add [-net|-host] target [netmask] gw

route  del [-net|-host] target [netmask] gw


# route add  -net 192.168.3.0/24  gw  192.168.0.254    # 设置到192.168.3.0网段的网关为192.168.0.254

# route add  -net 192.168.3.0 netmask 255.255.255.0  gw  192.168.0.254    # 功能同上

# route add  -host 192.168.4.4  gw  192.168.0.254    # 设置到192.168.4.4主机的网关为192.168.0.254

# 

# route del  -net 192.168.3.0/24                        # 删除192.168.3.0网段的网关信息

# route del  -host 192.168.4.4                        # 删除192.168.4.4主机的网关信息

# route add default gw  192.168.0.254                # 设置默认网关为192.168.0.254

# route del default gw  192.168.0.254                # 删除默认网关为192.168.0.254


*************************************************************************


而如今官方不再推荐使用如此陈旧的命令而推荐使用 ip 这个命令，以下我们看看它的用法：


ip  [选项]  操作对象{link|addr|route...}


# ip link show                  # 显示网络接口信息

# ip link set eth0 upi          # 开启网卡

# ip link set eth0 down         # 关闭网卡

# ip link set eth0 promisc on   # 开启网卡的混合模式

# ip link set eth0 promisc offi # 关闭网卡的混个模式

# ip link set eth0 txqueuelen 1200    # 设置网卡队列长度

# ip link set eth0 mtu 1400     # 设置网卡最大传输单元

# ip addr show                  # 显示网卡IP信息

# ip addr add 192.168.0.1/24 dev eth0 # 设置eth0网卡IP地址192.168.0.1

# ip addr del 192.168.0.1/24 dev eth0 # 删除eth0网卡IP地址


# ip route list                 # 查看路由信息

# ip route add 192.168.4.0/24  via  192.168.0.254 dev eth0 # 设置192.168.4.0网段的网关为192.168.0.254,数据走eth0接口

# ip route add default via  192.168.0.254  dev eth0    # 设置默认网关为192.168.0.254

# ip route del 192.168.4.0/24    # 删除192.168.4.0网段的网关

# ip route del default    # 删除默认路由


**************************************************************

接下来再看看通过文件修改网络参数：（CentOS6.2系统为例）


# cat  /etc/sysconfig/network-scripts/ifcfg-eth0    


DEVICE="eth0"              设备名

NM_CONTROLLED="yes"        设备是否被NetworkManager管理

ONBOOT="no"                开机是否启动

HWADDR="00:0C:29:59:E2:D3" 硬件地址（MAC地址）

TYPE=Ethernet              类型

BOOTPROTO=none             启动协议{none|dhcp}

IPADDR=192.168.0.1         IP地址

PREFIX=24                  子网掩码

GATEWAY=192.168.0.254      默认网关

DNS1=202.106.0.20          主DNS

DOMAIN=202.106.46.151      辅助DNS

UUID=5fb06bd0-0bb0-7ffb-45f1-d6edd65f3e03    设备UUID编号


**************************************************************

# cat /etc/sysconfig/network


HOSTNAME=butbueatiful.com    主机名


**************************************************************


注意：在5.0时代DNS服务器写在 /etc/resolv.conf 文件中，但到了6.0时代DNS可以写在/etc/resolv.conf但是此时需要在 /etc/sysconfig/network-scripts/ifcfg-eth0 文件中添加PEERDNS=no 配置，不然每次重启网卡就会重写/etc/resolv.conf文件的内容，当然了也可以直接写在 /etc/sysconfig/network-scripts/ifcfg-eth0
 文件中。



# 后记：

1. 配置/etc/resolv.confg重启丢失解决方法：


一种方法是把 PEERDNS 设置为“no”。


找到网卡配置文件，位置和： /etc/sysconfig/network-scripts/ifcfg-eth 文件中加入PEERDNS 选项。可以是 0, 1, 2等等，代表不同网卡的配置文件。例如，系统上第一张网卡是eth0的话，那它的配置文件就是/etc/sysconfig/network-scripts/ifcfg-eth0 然后在文件中把 PEERDNS 改为 ‘no’. 

例如：


DEVICE=eth0

BOOTPROTO=dhcp

ONBOOT=yes

TYPE=Ethernet

PEERDNS=no


这个选项可令 /etc/resolv.conf 在系统重启后不会被重写。


另一种方法是在这个文件中增加DNS：

如：

DNS1=127.0.0.1

DNS2=8.8.8.8 


2. 安全设置

   我们前面说了/etc/resolv.conf和/etc/hosts被人篡改了的话, 会很危险, 那我们在设置好着2个文件后, 做一下处理, 让这2个文件默认不能直接修改, 即使root也不行, 执行如下命令：

[root@butbueatiful ~]# chattr +i /etc/{resolv.conf,hosts}


如果我们自己想修改的时候，执行：    

[root@butbueatiful ~]# chattr -i /etc/{resolv.conf,hosts}


然后就可以修改了，修改完了别忘记+i。

3. 网络排除思路


检查配置文件是否有错误(书写及语法错误等）

检查本机网络协议是否正确：# ping -c 3 127.0.0.1

检查本机网卡链路是否正确：# ping -c 3 192.168.0.1(本机IP地址)

检查网关是否正确：       # ping -c 3 192.168.0.254(网关IP地址)

检查外部连通性：        # ping -c 3 www.google.com.hk

检查硬件




