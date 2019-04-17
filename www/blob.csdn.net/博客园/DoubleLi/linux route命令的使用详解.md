# linux route命令的使用详解 - DoubleLi - 博客园






route命令用于显示和操作IP路由表。要实现两个不同的子网之间的通信，需要一台连接两个网络的路由器，或者同时位于两个网络的网关来实现。在Linux系统中，设置路由通常是 为了解决以下问题：该Linux系统在一个局域网中，局域网中有一个网关，能够让机器访问Internet，那么就需要将这台机器的IP地址设置为 Linux机器的默认路由。要注意的是，直接在命令行下执行route命令来添加路由，不会永久保存，当网卡重启或者机器重启之后，该路由就失效了；要想永久保存，有如下方法：



1.在/etc/rc.local里添加
2.在/etc/sysconfig/network里添加到末尾
3./etc/sysconfig/static-router :
any net x.x.x.x/24 gw y.y.y.y


格式：route 
格式：/sbin/route 
用于打印路由表（display the current routing table）。

在非root用户使用时需要使用完整路径执行route命令。



## 命令参数

[root@linux ~]# route [-nee]
[root@linux ~]# route add [-net|-host] [网域或主机] netmask [mask] [gw|dev]
[root@linux ~]# route del [-net|-host] [网域或主机] netmask [mask] [gw|dev]
观察的参数：
   -n  ：不要使用通讯协定或主机名称，直接使用 IP 或 port number；
   -ee ：使用更详细的资讯来显示
增加 (add) 与删除 (del) 路由的相关参数：
   -net    ：表示后面接的路由为一个网域；
   -host   ：表示后面接的为连接到单部主机的路由；
   netmask ：与网域有关，可以设定 netmask 决定网域的大小；
   gw      ：gateway 的简写，后续接的是 IP 的数值喔，与 dev 不同；
   dev     ：如果只是要指定由那一块网路卡连线出去，则使用这个设定，后面接 eth0 等



格式：route -n
格式：/sbin/route -n
用于打印路由表，加上-n参数就是在输出的信息中不打印主机名而直接打印ip地址。

格式：route add default gw {IP-ADDRESS} {INTERFACE-NAME}
用于设置默认路由，其中，
参数{IP-ADDRESS): 用于指定路由器（网关）的IP地址；
参数{INTERFACE-NAME}: 用于指定接口名称，如eth0。使用/sbin/ifconfig -a可以显示所有接口信息。

例:route add default gw mango

格式：route add -net {NETWORK-ADDRESS} netmask {NETMASK} dev {INTERFACE-NAME}
添加到指定网络的路由规则，其中
参数{NETWORK-ADDRESS}: 用于指定网络地址
参数{NETMASK}: 用于指定子网掩码
参数{INTERFACE-NAME}: 用于指定接口名称，如eth0。

例1:route add -net 192.56.76.0 netmask 255.255.255.0 dev eth0
例2:route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0

格式：route add -net {NETWORK-ADDRESS} netmask {NETMASK} reject
设置到指定网络为不可达，避免在连接到这个网络的地址时程序过长时间的等待，直接就知道该网络不可达。

例:route add -net 10.0.0.0 netmask 255.0.0.0 reject

格式：route del -net {NETWORK-ADDRESS} netmask {NETMASK} dev {INTERFACE-NAME}
格式：route del -net {NETWORK-ADDRESS} netmask {NETMASK} reject
用于删除路由设置。参数指定的方式与route add相似。



## 输出详解

route命令输出的路由表字段含义如下：
    Destination 目标
          The destination network or destination host. 目标网络或目标主机。

    Gateway 网关
          The gateway address or '*' if none set. 网关地址，如果没有就显示星号。

    Genmask 网络掩码
          The  netmask  for  the  destination net; '255.255.255.255' for a
          host destination and '0.0.0.0' for the default route.



     Flags：总共有多个旗标，代表的意义如下：                        

         o U (route is up)：该路由是启动的；                       

         o H (target is a host)：目标是一部主机 (IP) 而非网域；                       

         o G (use gateway)：需要透过外部的主机 (gateway) 来转递封包；                       

         o R (reinstate route for dynamic routing)：使用动态路由时，恢复路由资讯的旗标；                       

         o D (dynamically installed by daemon or redirect)：已经由服务或转 port 功能设定为动态路由                       

         o M (modified from routing daemon or redirect)：路由已经被修改了；                       

         o !  (reject route)：这个路由将不会被接受(用来抵挡不安全的网域！)

         o A (installed by addrconf)



         o C (cache entry)

    Metric 距离、跳数。暂无用。
          The 'distance' to the target (usually counted in  hops).  It  is
          not  used  by  recent kernels, but may be needed by routing dae-
          mons.

    Ref   不用管，恒为0。
          Number of references to this route. (Not used in the Linux  ker-nel.)

    Use    该路由被使用的次数，可以粗略估计通向指定网络地址的网络流量。
          Count  of lookups for the route.  Depending on the use of -F and
          -C this will be either route cache misses (-F) or hits (-C).

    Iface 接口，即eth0,eth0等网络接口名
          Interface to which packets for this route will be sent.





## 范例一

单纯的观察路由状态


[root@linux ~]# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
192.168.10.0    0.0.0.0         255.255.255.0   U     0      0        0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U     0      0        0 eth0
0.0.0.0         192.168.10.30   0.0.0.0         UG    0      0        0 eth0
[root@linux ~]# route
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
192.168.10.0    *               255.255.255.0   U     0      0        0 eth0
169.254.0.0     *               255.255.0.0     U     0      0        0 eth0
default         server.cluster     0.0.0.0         UG    0      0        0 eth0



此外，观察一下上面的路由排列顺序喔，依序是由小网域(192.168.10.0/24 是 Class C)，逐渐到大网域(169.254.0.0/16 Class B) 最后则是预设路由 (0.0.0.0/0.0.0.0)。然后当我们要判断某个网路封包应该如何传送的时候，该封包会经由这个路由的过程来判断喔！举例来说，我上头仅有三个路由，若我有一个传往 192.168.10.20 的封包要传递，那首先会找 192.168.10.0/24 这个网域的路由，找到了！所以直接由 eth0 传送出去；如果是传送到 Yahoo 的主机呢？ Yahoo 的主机 IP 是 202.43.195.52，我通过判断 

1)不是 192.168.10.0/24，
      2)不是 169.254.0.0/16 结果到达 

3)0/0  时， OK ！传出去了，透过  eth0  将封包传给  192.168.10.30 那部  gateway  主机啊！所以说，路由是有顺序的。因此当你重复设定多个同样的路由时，例如在你的主机上的两张网路卡设定为相同网域的  IP  时，会出现什么情况？会出现如下的情况： 
Kernel  IP  routing  table 
Destination          Gateway                  Genmask                  Flags  Metric  Ref        Use  Iface 
192.168.10.0        0.0.0.0                  255.255.255.0      U          0            0                0  eth0 
192.168.10.0        0.0.0.0                  255.255.255.0      U          0            0                0  eth1 
也就是说，由于路由是依照顺序来排列与传送的，所以不论封包是由那个介面  (eth0, eth1)  所接收，都会由上述的  eth0  传送出去，所以，在一部主机上面设定两个相同网域的  IP  本身没有什么意义！有点多此一举就是了。除非是类似虚拟主机  (Xen, VMware 等软体 )  所架设的多主机时，才会有这个必要～



## 范例二

[root@linux ~]# route del -net 169.254.0.0 netmask 255.255.0.0 dev eth0
# 上面这个动作可以删除掉 169.254.0.0/16 这个网域！
# 请注意，在删除的时候，需要将路由表上面出现的资讯都写入
# 包括  netmask , dev 等等参数喔！注意注意
[root@linux ~]# route add -net 192.168.100.0 netmask 255.255.255.0 dev eth0
# 透过 route add 来增加一个路由！请注意，这个路由必须要能够与你互通。


# 举例来说，如果我下达底下的指令就会显示错误：
 route add -net 192.168.200.0 netmask 255.255.255.0 gw 192.168.200.254
# 因为我的环境内仅有 192.168.10.100 这个 IP ，所以不能与 192.168.200.254这个网段直接使用 MAC 互通！这样说，可以理解喔！？
[root@linux ~]# route add default gw 192.168.10.30
# 增加预设路由的方法！请注意，只要有一个预设路由就够了喔！
# 在这个地方如果您随便设定后，记得使用底下的指令重新设定你的网路
  /etc/init.d/network restart
     如果是要进行路由的删除与增加，那就得要参考上面的例子了，其实，使用 man route 里面的资料就很丰富了！仔细查阅一下囉！你只要记得，当出现『SIOCADDRT: Network is unreachable』这个错误时，肯定是由于 gw 后面接的 IP 无法直接与您的网域沟通 (Gateway 并不在你的网域内)。





## WINDOWS下的route命令



简单的的操作如下，

查看路由状态：routeprint

只查看ipv4（ipv6）路由状态：route print-4(-6)

添加路由：route add 目的网络 mask 子网掩码 网关——重启机器或网卡失效

route add 192.168.20.0 mask 255.255.255.0192.168.10.1

添加永久：route -p add 目的网络 mask子网掩码网关

route -p add 192.168.20.0 mask 255.255.255.0192.168.10.1

删除路由：route delete 目的网络 mask 子网掩码

route delete 192.168.20.0 mask255.255.255.0









