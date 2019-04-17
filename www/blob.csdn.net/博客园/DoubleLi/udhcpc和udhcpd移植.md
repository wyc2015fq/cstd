# udhcpc和udhcpd移植 - DoubleLi - 博客园








**实现DHCP自动获取IP地址**



前提：系统已经实现DNS（即使用ping [www.baidu.com测试时能ping](http://www.baidu.xn--comping-9v3o214cvo0co6ya/)通）。

1．  在内核中添加以下选项：

Networking  --->

   [*] Networking support  

      Networking options  ---> 

          <* > Packet socket                 //添加.配置CONFIG_PACKET

          [ * ]     IP: DHCP support         //添加

          [ * ] Network packet filtering (replaces ipchains)  --->    //添加，后面子选项可不选，配置CONFIG_NETFILTER

说明：若没选<* > Packet socket, [ * ] Network packet filtering (replaces ipchains)  --->选项，在执行udhcpc命令时出现如下错误：

~ # udhcpc

udhcpc (v0.9.9-pre) started

udhcpc[208]: udhcpc (v0.9.9-pre) started



FATAL: couldn't listen on socket, Address family not supported by protocol

udhcpc[208]: FATAL: couldn't listen on socket, Address family not supported by protocol

2．  Busybox中添加以下选项：

Networking Utilities  --->

   udhcp Server/Client  --->

        [] udhcp Server (udhcpd)    //在此不作服务端，故不选。生成udhcpd命令                          

        [*] udhcp Client (udhcpc)       //生成udhcpc命令                               

        [ ] Lease display utility (dumpleases)                        

        [ ]   Log udhcp messages to syslog (instead of stdout)        

        [ ]   Compile udhcp with noisy debugging messages

若busybox没编译相应选项，也可从网上下载相应文件，用arm-linux交叉编译得到udhcpd,udhcpc命令copy到usr/sbin下就可以了。

我从 网上  下的udhcp_0.9.8cvs20050303.orig.tar.gz文件

解压后修改Makefile文件

在19行添加CROSS_COMPILE=arm-linux-  

注释12行的COMBINED_BINARY=1，否则不生成udhcpc命令。

3．  建相关配置文件

从busybox的examples/udhcp/下copy  simple.script文件到开发板/usr/share/udhcpc/下，并重命名为default.script，udhcp_0.9.8cvs20050303.orig.tar.gz中也有这样的文件。

[root@localhost root]# vi usr/share/udhcpc/default.script

#!/bin/sh



# udhcpc script edited by Tim Riker <Tim@Rikers.org>



[ -z "$1" ] && echo "Error: should be called from udhcpc" && exit 1



RESOLV_CONF="/etc/resolv.conf"

[ -n "$broadcast" ] && BROADCAST="broadcast $broadcast"

[ -n "$subnet" ] && NETMASK="netmask $subnet"



case "$1" in

        deconfig)

                /sbin/ifconfig $interface 0.0.0.0

                ;;



        renew|bound)

                /sbin/ifconfig $interface $ip $BROADCAST $NETMASK



                if [ -n "$router" ] ; then

                        echo "deleting routers"

                        while route del default gw 0.0.0.0 dev $interface ; do

                                :

                        done

                        for i in $router ; do

                                route add default gw $i dev $interface

                        done

                fi



                echo -n > $RESOLV_CONF

                [ -n "$domain" ] && echo search $domain >> $RESOLV_CONF

                for i in $dns ; do

                        echo adding dns $i

                        echo nameserver $i >> $RESOLV_CONF

                done

                ;;

esac



exit 0



4．  重启开发板，执行udhcpc就可自动获取IP地址了，以下是执行udhcpc的输出信息：



~ # udhcpc

udhcpc (v0.9.9-pre) started

udhcpc[228]: udhcpc (v0.9.9-pre) started



Sending discover...

udhcpc[228]: Sending discover...



Sending select for 192.168.1.109...

udhcpc[228]: Sending select for 192.168.1.109...



Lease of 192.168.1.109 obtained, lease time 86400

udhcpc[228]: Lease of 192.168.1.109 obtained, lease time 86400



deleting routers

route: SIOC[ADD|DEL]RT: No such process

adding dns 192.168.0.1

~ # ping www.baidu.com

PING www.a.shifen.com (220.181.38.4): 56 data bytes

64 bytes from 220.181.38.4: icmp_seq=0 ttl=52 time=1219.0 ms

[1] + Stopped                    ping www.baidu.com



5．  如果是双网卡必须用参数指明

例：udhcpc  -i  eth1 









