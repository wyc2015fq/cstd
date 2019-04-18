# Linux常用的网络命令 - gauss的专栏 - CSDN博客
2015年07月04日 00:00:05[gauss](https://me.csdn.net/mathlmx)阅读数：321

Linux下网卡命名规律：eth0，eth1。第一块以太网卡，第二块。lo为环回接口，它的IP地址固定为127.0.0.1，掩码8位。它代表你的机器本身。
1、ifconfig是查看网卡的信息 
ifconfig [Interface] 
    Interface是可选项，如果不加此项，则显示系统中所有网卡的信息。如果添加此选项则显示所指定的网卡信息. 
例如：ifconfig eth0 
eth0 Link encap:Etherne
 HWaddr 00:0C:29:F3:3B:F2
            inet addr:192.168.0.10 Bcast:192.168.0.255 Mask:255.255.255.0
            UP BROADCAST RUNNING MULTICAST MTU:1500 Metric:1
            RX packets:78 errors:0 dropped:0 overruns:0 frame:0
            TX packets:104 errors:0 dropped:0 overruns:0 carrier:0
            collisions:0 txqueuelen:100
            RX bytes:11679 (11.4 Kb)
            TX bytes:14077 (13.7 Kb)
            Interrupt:10 Base address:0x1080
可以看到:
第一行：连接类型：Ethernet（以太网）HWaddr（硬件mac地址） 
第二行：网卡的IP地址、子网、掩码 
第三行：UP（代表网卡开启状态）RUNNING（代表网卡的网线被接上）MULTICAST（支持组播）MTU:1500（最大传输单元）：1500字节 
第四、五行：接收、发送数据包情况统计 
第七行：接收、发送数据字节数统计信息。 
（1） 配置eth0的IP地址， 同时激活该设备。 
#ifconfig eth0 192.168.1.1 0 netmask 255.255.255.0 up 
（2） 配置eth0别名设备eth0:1的IP地址，并添加路由。 
#ifconfig eth0:1 192.168.1.3 
#route add –host 192.168.1.3 dev eth0:1 
（3） 激活设备。 
#ifconfig eth0 up 
（4） 禁用设备。 
#ifconfig eth0 down 
（5） 查看指定的网络接口的配置。 
#ifconfig eth0 
（6） 查看所有的网络接口配置。 
#ifconfig
查看网路接口信息 
      1．了解linux中的网络接口设备 
$/sbin/ifconfig 查看所有活动网络接口信息，其中包括一个lo环回端口。
2．查看指定网络接口的信息
$/sbin/ifconfig <网络接口名称> 查看指定接口信息。
3．查看系统中所有网络接口的信息
$/sbin/ifconfig –a 查看所有接口信息，包括非活动状态接口。
注：普通用户查看网络接口信息时，需要在命令前加“/sbin”，而管理员却不用。这是由于命令搜索路径的原因，可以通过“$echo $PATH”查看。 
2、 route命令来配置并查看内核路由表的配置情况 
例如： 
（1） 添加到主机的路由。 
#route add –host 192.168.1.2 dev eth0:0 
#route add –host 10.20.30.148 gw 10.20.30.40 
（2） 添加到网络的路由。 
#route add –net 10.20.30.40 netmask 255.255.255.248 eth0 
#route add –net 10.20.30.48 netmask 255.255.255.248 gw 10.20.30.41 
#route add –net 192.168.1.0/24 eth1 
（3） 添加默认网关。 
#route add default gw 192.168.1.1 
（4） 查看内核路由表的配置。 
#route 
（5）删除路由。 
#route del –host 192.168.1.2 dev eth0:0 
#route del –host 10.20.30.148 gw 10.20.30.40 
#route del –net 10.20.30.40 netmask 255.255.255.248 eth0 
#route del –net 10.20.30.48 netmask 255.255.255.248 gw 10.20.30.41 
#route del –net 192.168.1.0/24 eth1 
#route del default gw 192.168.1.1 
对于1和2两点可使用下面的语句实现： 
Ifconfig eth0 172.16.19.71 netmask 255.255.255.0 
Route 0.0.0.0 gw 172.16.19.254 
Service network restart 
3、 traceroute命令显示数据包到达目的主机所经过的路由 
例如： 
#traceroute [www.sina.com.cn](http://www.sina.com.cn/)
4、 ping命令来测试网络的连通性 
例如： 
#ping [www.sina.com.cn](http://www.sina.com.cn/)
#ping –c 4 192.168.1.12 
5、 netstat命令来显示网络状态信息 
        主要用途有：察看网络的连接状态（仅对TCP有效，对UDP无效）、检查接口的配置信息、检查路由表、取得统
计信息。不带参数时表示显示获得的TCP、UDP端口状态，因为UDP为无连接的协议，所以状态对其无意义。常见的
状态有：ESTABLISHED、 LISTENING、TIME-WAIT，分别表示处于连接状态、等待连接、关闭连接。netstat程序有
几个参数选项，用户可以用来查看网络上不同方面的信息，其参数说明如下：
-a：选项为显示所有配置的接口
-i：选项为显示接口统计信息
-n：选项为以数字形式显示IP地址
-r：选项显示内核路由表
-s: 选项表示计数器的值。 
例如： 
（1） 显示网络接口状态信息。 
#netstat –i 
（2） 显示所有监控中的服务器的Socket和正使用Socket的程序信息。 
#netstat –lpe 
（3） 显示内核路由表信息。 
#netstat –r 
#netstat –nr 
（4） 显示TCP/UDP传输协议的连接状态。 
#netstat –t 
#netstat –u 
6、 hostname更改主机名 
例如； 
#hostname myhost 
7、 arp命令来配置并查看arp缓存 
例如： 
（1） 查看arp缓存。 
#arp 
（2） 添加一个IP地址和MAC地址的对应记录。 
#arp –s 192.168.33.15 00:60:08:27:CE:B2 
（3） 删除一个IP地址和MAC地址的对应缓存记录。 
#arp –d192.168.33.15 
    8. ifup命令用于启动指定的非活动网卡设备 
        该命令与ifconfig up命令相似。Ifdown命令用于停止指定的活动网卡设备，该命令与ifconfig down命令功能相 
似。这两个命令的格式如下：
ifup 网卡设备名
ifdown   网卡设备名
9. nslookup查询DNS数据库的用户级命令
exit 命令：退出nslookup命令。
      lserver命令：查找指定的域名服务器并将其设置为 默认的域名服务器。
      server命令：和lserver相似，解析另一台域名服务 器并将其设置为默认的服务器，不同在于本命令使 用当前的默 
认服务器来解析。
      set命令：修改工作参数，它也是nslookup中最常用 的命令，其可使用的工作参数如nslookup程序的set命令中的工作参数.
工作参数说明:            set class   设置查询的类型，一般情况下为I nternet。
                                        set de bug   设置调试模式/set nodebug:设置为非调试模式
                                        set d2    设置详细调试模式。/set nod2：设置为非详细    调试模式。
set [no]defname   设置为defname，会在所有没有加“o“的名字后    自动加上默认的域名。它是默认的方式。
set domain   设置默认的域名。
set port    DNS的默认端口为53，用本命令可以设置其    他端口，一般用于调试。
set q(querytype)   改变查询的信息的类型。默认的类型为A纪录
set recurse   设置查询类型为递归；
set retry    设置重试的次数。默认的值为4。
set root    设置默认的根服务器。
set timeout   设置等待应答的限制时间（单位为秒），超 出即为超时，如果还可以重试，就会将长超    时值加倍，重 
新查询。默认的超时值为5秒。 
10. tcpdump命令用于监视TCP/IP连接并直接读取数据链路层的数据包的头部信息 
        用户可以指定哪些数据包被监视、哪些控制要显示格式。例如要监视所有Ethernet上来往的通信，执行下述命 
令：
#   tcpdump -i eth0
实际上，tcpdump命令的格式为：
tcpdump [ -选项] [ -c 数量 ] [ -F 文件名 ][ -i 网络接口 ] [ -r 文件 
名] [ -s snaplen ]
　　　　　　　　　　[ -T 类型 ] [ -w 文件名 ] [表达式 ] 
tcpdump命令中的选项参数说明如xia所示。Tcpdump命令中的表达式是一个正则表达式，tcpdump利用它作为过滤报 
文的条件，如果一个报文满足表达式的条件，则这个报文将会被捕获。如果没有给出任何条件，则网络上所有的信 
息包将会被截获。 
tcpdump命令的选项介绍: 参数选项   说明 
        -a 将网络地址和广播地址转变成名字；
        -d 将匹配信息包的代码以人们能够理解的汇编格式给出；
        -dd 将匹配信息包的代码以c语言程序段的格式给出；
        -ddd 将匹配信息包的代码以十进制的形式给出；
         -e 在输出行打印出数据链路层的头部信息；
        -f 将外部的Internet地址以数字的形式打印出来；
        -l 使标准输出变为缓冲行形式；
        -n 不把网络地址转换成名字；
        -t 在输出的每一行不打印时间戳；
        -v 输出一个稍微详细的信息，例如在ip包中可以包括ttl和服务类型的信息；
        -vv 输出详细的报文信息；
        -c 在收到指定的包的数目后，tcpdump就会停止；
        -F 从指定的文件中读取表达式,忽略其它的表达式；
        -I 指定监听的网络接口；
        -r 从指定的文件中读取包(这些包一般通过-w选项产生)；
        - w 直接将包写入文件中，并不分析和打印出来；
        -T 将监听到的包直接解释为指定的类型的报文，常见的类型有 
        rpc （远程过程调用）和snmp（简单网络管理协议；） 
11. 网络配置文件及程序 
1、网络设置/etc/sysconfig/network   该文件用来指定服务器上的网络配置信息，包含了控制和网络有关的文件和守护 
程序的行为的参数。 
        /etc/rc.d/init.d/network restart 
2、端口号和服务器名之间的映射/etc/services 
3、配置名字解析器/etc/host.conf 
4、配置DNS客户/etc/resolv.conf 
5、使用netconfig程序配置网络
