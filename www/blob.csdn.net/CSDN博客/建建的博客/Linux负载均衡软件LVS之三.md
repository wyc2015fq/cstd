# Linux负载均衡软件LVS之三 - 建建的博客 - CSDN博客
2017年03月20日 15:25:49[纪建](https://me.csdn.net/u013898698)阅读数：230
LVS集群有DR、TUN、NAT三种配置模式，可以对www服务、FTP服务、MAIL服务等做负载均衡，下面通过搭建www服务的负载均衡实例，讲述基于DR模式的LVS集群配置。
一、 Director Server的配置
在Director Server上配置LVS负载均衡集群，有两种方法：
 通过ipvsadm命令行进行配置
 通过Redhat提供的工具piranha来配置LVS
1、通过ipvsadm命令行方式配置LVS
安装IPVS后，就可以配置LVS集群了，首先在Director Server上绑定一个虚拟IP（也叫VIP），此IP用于对外提供服务，执行如下命令：
[root@localhost ~]#ifconfig eth0:0 192.168.60.200 broadcast 192.168.60.200 \
 >netmask 255.255.255.255 up
此处在eth0设备上绑定了一个虚拟设备eth0:0，同时设置了一个虚拟IP是192.168.60.200，也就是上面我们规划的IP地址，然后指定广播地址也为192.168.60.200，需要特别注意的是，这里的子网掩码为255.255.255.255。
然后给设备eth0:0指定一条路由，执行如下指令：
[root@localhost ~]#route add -host 192.168.60.200 dev eth0:0 
接着启用系统的包转发功能，从而使系统充当路由器，执行如下指令：
[root@localhost ~]#echo "1" >/proc/sys/net/ipv4/ip_forward
指令中，参数值为1时启用ip转发，为0时禁止ip转发。其实在DR模式中，开启系统的包转发功能不是必须的，而在NAT模式下此操作是必须的。
然后开始配置ipvs，执行如下操作：
[root@localhost ~]#ipvsadm -C
[root@localhost ~]#ipvsadm -A -t 192.168.60.200:80 -s rr -p 600
[root@localhost ~]#ipvsadm -a -t 192.168.60.200:80 -r 192.168.60.132:80 -g
[root@localhost ~]#ipvsadm -a -t 192.168.60.200:80 -r 192.168.60.144:80 -g
上面操作中，第一行是清除内核虚拟服务器列表中的所有记录，第二行是添加一条新的虚拟IP记录。这个新的IP是192.168.60.200，同时指定持续服务时间为600秒。第三、四行是在新加虚拟IP记录中添加两条新的Real Server记录，并且指定LVS 的工作模式为直接路由模式。
最后，启动LVS服务，执行如下操作：
[root@localhost ~]#ipvsadm
这样，LVS在Director Server上的配置就完成了。
为了管理和配置的方便，可以将上面的操作写出一个脚本文件，脚本内容如下：
#!/bin/bash
VIP=192.168.60.200
RIP1=192.168.60.132
RIP2=192.168.60.144
GW=192.168.60.1
# set the Virtual IP Address
/sbin/ifconfig eth0:0 $VIP broadcast $VIP netmask 255.255.255.255 up
/sbin/route add -host $VIP dev eth0:0 
echo "1" >/proc/sys/net/ipv4/ip_forward
#Clear IPVS table
/sbin/ipvsadm -C
#set LVS
/sbin/ipvsadm -A -t $VIP:80 -s rr -p 600
/sbin/ipvsadm -a -t $VIP:80 -r $RIP1:80 -g
/sbin/ipvsadm -a -t $VIP:80 -r $RIP2:80 -g
#Run LVS
/sbin/ipvsadm
#end
也可以写成可启动与停止的服务脚本，脚本内容如下：
#!/bin/sh
# description: Start LVS of Director server
VIP=192.168.60.200
RIP1=192.168.60.132
RIP2=192.168.60.144
./etc/rc.d/init.d/functions
case "$1" in
    start)
        echo " start LVS of Director Server"
# set the Virtual  IP Address and sysctl parameter
 /sbin/ifconfig eth0:0 $VIP broadcast $VIP netmask 255.255.255.255 up
       echo "1" >/proc/sys/net/ipv4/ip_forward 
#Clear IPVS table
       /sbin/ipvsadm -C
#set LVS
/sbin/ipvsadm -A -t $VIP:80 -s rr -p 600
/sbin/ipvsadm -a -t $VIP:80 -r $RIP1:80 -g
/sbin/ipvsadm -a -t $VIP:80 -r $RIP2:80 -g
#Run LVS
      /sbin/ipvsadm
       ;;
    stop)
        echo "close LVS Directorserver"
        echo "0" >/proc/sys/net/ipv4/ip_forward
        /sbin/ipvsadm -C
        /sbin/ifconfig eth0:0 down
        ;;
    *)
        echo "Usage: $0 {start|stop}"
        exit 1
esac
将此脚本命名为lvsDR文件，然后把文件放到/etc/init.d下，执行：
[root@localhost ~]#chomd 755 /etc/init.d/lvsDR
最后可以通过下面命令启动或停止LVS服务：
service lvsDR {start|stop}
到此为止，命令行方式配置Director Server完毕。
2、通过Redhat提供的工具piranha来配置LVS
 Piranha是REDHAT提供的一个基于Web的LVS配置软件，可以省去手工配置LVS的繁琐工作，同时，也可单独提供cluster功能，例如，可以通过Piranha激活Director Server的后备主机，也就是配置Director Server的双机热备功能。
 Piranha工具的安装非常简单，下载Piranha的rpm包，进行安装即可：
[root@localhost ~]#rpm –ivh piranha-0.8.2-1.i386.rpm
Piranha安装完毕后,会产生/etc/sysconfig/ha/lvs.cf文件，默认此文件是空的，可以通过Piranha提供的web界面配置此文件，也可以直接手动编辑此文件，编辑好的lvs.cf文件内容类似如下，注意，“#”号后面的内容为注释。
[root@localhost ~]# more /etc/sysconfig/ha/lvs.cf
serial_no = 18              #序号。
primary = 192.168.60.56     #指定主Director Server的真实IP地址，是相对与有备用的Director Server而言的，也就是给Director Server做HA Cluster。
service = lvs               #指定双机的服务名。
backup_active = 0        #是否激活备用Director Server。“0”表示不激活，“1”表示激活。
backup = 0.0.0.0             #这里指定备用Director Server的真实IP地址，如果没有备用Director Server，可以用“0.0.0.0”代替。
heartbeat = 0             #是否开启心跳，1表示开启，0表示不开启。
heartbeat_port = 539      #指定心跳的UDP通信端口。
keepalive = 5               #心跳间隔时间，单位是秒。
deadtime = 10                #如果主Director Server在deadtime(秒)后没有响应，那么备份Director
Server就会接管主Director Server的服务。
network = direct            #指定LVS的工作模式，direct表示DR模式，nat表示NAT模式，tunnel表示TUNL模式。
debug_level = NONE          #定义debug调试信息级别。
virtual [www.gaojf.com](http://www.gaojf.com/){      #指定虚拟服务的名称。
active = 1        #是否激活此服务。
address = 192.168.60.200 eth0:0  #虚拟服务绑定的虚拟IP以及网络设备名。
port = 80                   #虚拟服务的端口。
send = "GET / HTTP/1.0\r\n\r\n"  #给real server发送的验证字符串。
expect = "HTTP"             #服务器正常运行时应该返回的文本应答信息，用来判断real server是否工作正常。
use_regex = 0               # expect选项中是否使用正则表达式，0表示不使用，1表示使用。
load_monitor = none         #LVS中的Director Server能够使用 rup 或 ruptime 来监视各个real server的负载状态。该选项有3个可选值，rup、ruptime和none，如果选择rup，每个real server就必须运行rstatd服务。如果选择了ruptime，每个real server就必须运行 rwhod 服务。
scheduler = rr              #指定LVS的调度算法。
protocol = tcp              #虚拟服务使用的协议类型。
timeout = 6                 #real server失效后从lvs路由列表中移除失效real server所必须经过的时间，以秒为单位。
reentry = 15                #某个real server被移除后，重新加入lvs路由列表中所必须经过的时间，以秒为单位。
quiesce_server = 0          #如果此选项为1.那么当某个新的节点加入集群时，最少连接数会被重设
为零，因此LVS会发送大量请求到此服务节点，造成新的节点服务阻塞，
建议设置为0。
server RS1 {                #指定real server服务名。
address = 192.168.60.132    #指定real server的IP地址。
active = 1                  #是否激活此real server服务。
weight = 1                   #指定此real server的权值，是个整数值，权值是相对于所有real server节点而言的，权值高的real server处理负载的性能相对较强。
}
server RS2 {
address = 192.168.60.144
active = 1
weight = 1
}
}
编辑完成,然后启动pulse服务,即启动lvs服务
[root@localhost ~]#service pulse start
同理，此种方式下也要启用系统的包转发功能：
[root@localhost ~]#echo "1" >/proc/sys/net/ipv4/ip_forward
到此为止，Piranha工具方式配置Director Server完毕。
二、Real server 的配置
在lvs的DR和TUn模式下，用户的访问请求到达真实服务器后，是直接返回给用户的，而不再经过前端的Director Server，因此，就需要在每个Real server节点上增加虚拟的VIP地址，这样数据才能直接返回给用户，增加VIP地址的操作可以通过创建脚本的方式来实现，创建文件/etc/init.d/lvsrs，脚本内容如下：
#!/bin/bash
VIP=192.168.60.200
/sbin/ifconfig lo:0 $VIP broadcast $VIP netmask 255.255.255.255 up
/sbin/route add -host $VIP dev lo:0
echo “1″ >/proc/sys/net/ipv4/conf/lo/arp_ignore
echo “2″ >/proc/sys/net/ipv4/conf/lo/arp_announce
echo “1″ >/proc/sys/net/ipv4/conf/all/arp_ignore
echo “2″ >/proc/sys/net/ipv4/conf/all/arp_announce
sysctl -p
#end
此操作是在回环设备上绑定了一个虚拟IP地址，并设定其子网掩码为255.255.255.255，与Director Server上的虚拟IP保持互通，然后禁止了本机的ARP请求。
上面脚本也可以写成可启动与停止的服务脚本，内容如下：
[root@localhost ~]#more /etc/init.d/lvsrs
#!/bin/bash
#description : Start Real Server 
VIP=192.168.60.200
./etc/rc.d/init.d/functions
case "$1" in
    start)
        echo " Start LVS  of  Real Server"
 /sbin/ifconfig lo:0 $VIP broadcast $VIP netmask 255.255.255.255 up
        echo "1" >/proc/sys/net/ipv4/conf/lo/arp_ignore
        echo "2" >/proc/sys/net/ipv4/conf/lo/arp_announce
        echo "1" >/proc/sys/net/ipv4/conf/all/arp_ignore
        echo "2" >/proc/sys/net/ipv4/conf/all/arp_announce 
        ;;
    stop)
        /sbin/ifconfig lo:0 down
        echo "close LVS Director server"
        echo "0" >/proc/sys/net/ipv4/conf/lo/arp_ignore
        echo "0" >/proc/sys/net/ipv4/conf/lo/arp_announce
        echo "0" >/proc/sys/net/ipv4/conf/all/arp_ignore
        echo "0" >/proc/sys/net/ipv4/conf/all/arp_announce
        ;;
    *)
        echo "Usage: $0 {start|stop}"
        exit 1
esac
然后，修改lvsrs有可执行权限：
[root@localhost ~]#chomd 755 /etc/init.d/lvsrs
最后，可以通过下面命令启动或关闭lvsrs
service lvsrs {start|stop}
由于虚拟ip，也就是上面的VIP地址，是Director Server和所有的Real server共享的，如果有ARP请求VIP地址时，Director Server与所有Real server都做应答的话，就出现问题了，因此，需要禁止Real server响应ARP请求。而lvsrs脚本的作用就是使Real Server不响应arp请求。
对于2.4以下低版本，在Real Server上要安装ARP hidden内核补丁,幸运的是，在2.6内核中通过调整内核参数即可忽略ARP广播回应。
三、在Director上配置冗余策略 Ldirectord
Ldirectord的作用是监测Real Server，当Real Server失效时，把它从虚拟服务器列表中删除，恢复时重新添加到列表，在前面文章介绍heartbeat时默认已经安装了Ldirectord，因此。这里只需进行配置即可。Ldirectord的配置文件是/etc/ha.d/ldirectord.cf。
下面是需要配置的选项，注意，“#”号后面的内容为注释：
# Global Directives
checktimeout=20           #判定real server出错的时间间隔。
checkinterval=10          #指定ldirectord在两次检查之间的间隔时间。
fallback=127.0.0.1:80     #当所有的real server节点不能工作时，web服务重定向的地址。
autoreload=yes            #是否自动重载配置文件，选yes时，配置文件发生变化，自动载入配置信息。
logfile="/var/log/ldirectord.log"   #设定ldirectord日志输出文件路径。
quiescent=no              #当选择no时，如果一个节点在checktimeout设置的时间周期内没有响应，ldirectord将会从LVS的路由表中直接移除real server，此时，将中断现有的客户端连接，并使LVS丢掉所有的连接跟踪记录和持续连接模板，如果选择为yes，当某个real server失效时，ldirectord将失效节点的权值设置为0，新的连接将不能到达，但是并不从LVS路由表中清除此节点，同时，连接跟踪记录和程序连接模板仍然保留在Director上。
注意：以上几行为ldirectord.cf文件的“全局”设置，它们可以应用到下面多个虚拟主机，下面是每个虚拟主机的配置。
# Sample for an http virtual service
virtual=192.168.60.200:80        #指定虚拟的IP地址和端口号，注意，在virtual行后面的行必
须缩进4个空格或以一个tab字符进行标记。
real=192.168.60.132:80 gate      #指定Real Server服务器地址和端口，同时设定LVS工作模式，
用gate表示DR模式，ipip表示TUNL模式，masq表示NAT模式。
real=192.168.60.144:80 gate
fallback=127.0.0.1:80 gate       
service=http                     #指定服务的类型，这里是对http服务做负载均衡。
request="index.html"             #ldirectord将根据指定的Real Server地址，结合该选项给出
的请求路径，发送访问请求，检查Real Server上的服务是否正
常运行，确保这里给出的页面地址是可访问的，不然ldirectord
会误认为此节点已经失效，发生错误监控现象。
receive="ok"  #指定检测的关键字，这里的设置表示测试访问index.html页面是否包含ok关键字，如果有则说明该节点正常，否则认定为故障。
scheduler=rr                     #指定调度算法，这里是rr（轮叫）算法。
protocol=tcp                     #指定协议的类型，LVS支持TCP和UDP协议。
checktype=negotiate              #指定Ldirectord的检测类型，默认为negotiate。
checkport=80                     #指定监控的端口号。
virtualhost=www.gaojf.com        #虚拟服务器的名称，随便指定。
配置完毕，就可以执行如下命令启动或关闭ldirectord服务
/etc/init.d/ldirectord {start|stop}
Ldirectord的核心作用是监控Real Server节点状态，同时，它还能调用ipvsadm自动创建LVS路由表，这点从ldirectord.cf中可以看出，
这里需要说明的是，ldirectord和Piranha都具有监控Real Server的功能，如果要通过ldirectord监控节点状态，只需启动ldirectord服务，整个集群系统就可以运行起来，而无需执行上面我们配置的LVS脚本，因为ldirectord会自动调用ipvsadm创建LVS路由表，而我们上面讲述的利用ipvsadm命令行方式配置LVS，是为了让读者能更深入的了解ipvsadm的实现细节和实现机制。
如果是通过Piranha工具配置LVS，就无需使用ldirectord，Piranha工具对应的系统进程是pluse，此进程也会自动调用ipvsadm创建LVS路由表，同时会利用自身的nanny守护进程监控real server的状态！
