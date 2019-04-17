# Linux下ping命令、traceroute命令、tracert命令的使用 - DoubleLi - 博客园






Linux系统的ping命令是常用的网络命令，它通常用来测试与目标主机的连通性，我们经常会说“ping一下某机器，看是不是开着”、不能打开网页时会说“你先ping网关地址192.168.1.1试试”。它通过发送ICMP ECHO_REQUEST数据包到网络主机（send ICMP ECHO_REQUEST to network hosts），并显示响应情况，这样我们就可以根据它输出的信息来确定目标主机是否可访问（但这不是绝对的）。有些服务器为了防止通过ping探测到，通过防火墙设置了禁止ping或者在内核参数中禁止ping，这样就不能通过ping确定该主机是否还处于开启状态。

linux下的ping和windows下的ping稍有区别,linux下ping不会自动终止,需要按ctrl+c终止或者用参数-c指定要求完成的回应次数。

1.命令格式：

ping [参数] [主机名或IP地址]

2.命令功能：

ping命令用于：确定网络和各外部主机的状态；跟踪和隔离硬件和软件问题；测试、评估和管理网络。如果主机正在运行并连在网上，它就对回送信号进行响应。每个回送信号请求包含一个网际协议（IP）和 ICMP 头，后面紧跟一个 tim 结构，以及来填写这个信息包的足够的字节。缺省情况是连续发送回送信号请求直到接收到中断信号（Ctrl-C）。

ping 命令每秒发送一个数据报并且为每个接收到的响应打印一行输出。ping 命令计算信号往返时间和(信息)包丢失情况的统计信息，并且在完成之后显示一个简要总结。ping 命令在程序超时或当接收到 SIGINT 信号时结束。Host 参数或者是一个有效的主机名或者是因特网地址。

3.命令参数：

-d 使用Socket的SO_DEBUG功能。

-f  极限检测。大量且快速地送网络封包给一台机器，看它的回应。

-n 只输出数值。

-q 不显示任何传送封包的信息，只显示最后的结果。

-r 忽略普通的Routing Table，直接将数据包送到远端主机上。通常是查看本机的网络接口是否有问题。

-R 记录路由过程。

-v 详细显示指令的执行过程。

<p>-c 数目：在发送指定数目的包后停止。

-i 秒数：设定间隔几秒送一个网络封包给一台机器，预设值是一秒送一次。

-I 网络界面：使用指定的网络界面送出数据包。

-l 前置载入：设置在送出要求信息之前，先行发出的数据包。

-p 范本样式：设置填满数据包的范本样式。

-s 字节数：指定发送的数据字节数，预设值是56，加上8字节的ICMP头，一共是64ICMP数据字节。

-t 存活数值：设置存活数值TTL的大小。

4.使用实例：

实例1：ping的通的情况

命令：

ping 192.168.120.205

输出：


![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@localhost ~]# ping 192.168.120.205
PING 192.168.120.205 (192.168.120.205) 56(84) bytes of data.
64 bytes from 192.168.120.205: icmp_seq=1 ttl=64 time=0.720 ms
64 bytes from 192.168.120.205: icmp_seq=2 ttl=64 time=0.181 ms
64 bytes from 192.168.120.205: icmp_seq=3 ttl=64 time=0.191 ms
64 bytes from 192.168.120.205: icmp_seq=4 ttl=64 time=0.188 ms
64 bytes from 192.168.120.205: icmp_seq=5 ttl=64 time=0.189 ms

--- 192.168.120.205 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 4000ms
rtt min/avg/max/mdev = 0.181/0.293/0.720/0.214 ms
[root@localhost ~]# 

![复制代码](https://common.cnblogs.com/images/copycode.gif)


说明：



实例2：ping不通的情况

命令：

ping 192.168.120.202

输出：


![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@localhost ~]# ping 192.168.120.202
PING 192.168.120.202 (192.168.120.202) 56(84) bytes of data.
From 192.168.120.204 icmp_seq=1 Destination Host Unreachable
From 192.168.120.204 icmp_seq=2 Destination Host Unreachable
From 192.168.120.204 icmp_seq=3 Destination Host Unreachable
From 192.168.120.204 icmp_seq=4 Destination Host Unreachable
From 192.168.120.204 icmp_seq=5 Destination Host Unreachable
From 192.168.120.204 icmp_seq=6 Destination Host Unreachable

--- 192.168.120.202 ping statistics ---
8 packets transmitted, 0 received, +6 errors, 100% packet loss, time 7005ms
, pipe 4
[root@localhost ~]#

![复制代码](https://common.cnblogs.com/images/copycode.gif)


说明：



实例3：ping网关

命令：

ping -b 192.168.120.1

输出：


![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@localhost ~]# route
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
192.168.120.0   *               255.255.255.0   U     0      0        0 eth0
192.168.0.0     192.168.120.1   255.255.0.0     UG    0      0        0 eth0
10.0.0.0        192.168.120.1   255.0.0.0       UG    0      0        0 eth0
default         192.168.120.240 0.0.0.0         UG    0      0        0 eth0
[root@localhost ~]# ping -b 192.168.120.1
PING 192.168.120.1 (192.168.120.1) 56(84) bytes of data.
64 bytes from 192.168.120.1: icmp_seq=1 ttl=255 time=2.02 ms
64 bytes from 192.168.120.1: icmp_seq=2 ttl=255 time=1.83 ms
64 bytes from 192.168.120.1: icmp_seq=3 ttl=255 time=1.68 ms
64 bytes from 192.168.120.1: icmp_seq=4 ttl=255 time=1.98 ms
64 bytes from 192.168.120.1: icmp_seq=5 ttl=255 time=1.88 ms

--- 192.168.120.1 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 4000ms
rtt min/avg/max/mdev = 1.682/1.880/2.020/0.129 ms

![复制代码](https://common.cnblogs.com/images/copycode.gif)


说明：



实例4：ping指定次数

命令：

ping -c 10 192.168.120.206

输出：


![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@localhost ~]# ping -c 10 192.168.120.206
PING 192.168.120.206 (192.168.120.206) 56(84) bytes of data.
64 bytes from 192.168.120.206: icmp_seq=1 ttl=64 time=1.25 ms
64 bytes from 192.168.120.206: icmp_seq=2 ttl=64 time=0.260 ms
64 bytes from 192.168.120.206: icmp_seq=3 ttl=64 time=0.242 ms
64 bytes from 192.168.120.206: icmp_seq=4 ttl=64 time=0.271 ms
64 bytes from 192.168.120.206: icmp_seq=5 ttl=64 time=0.274 ms
64 bytes from 192.168.120.206: icmp_seq=6 ttl=64 time=0.295 ms
64 bytes from 192.168.120.206: icmp_seq=7 ttl=64 time=0.269 ms
64 bytes from 192.168.120.206: icmp_seq=8 ttl=64 time=0.270 ms
64 bytes from 192.168.120.206: icmp_seq=9 ttl=64 time=0.253 ms
64 bytes from 192.168.120.206: icmp_seq=10 ttl=64 time=0.289 ms

--- 192.168.120.206 ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9000ms
rtt min/avg/max/mdev = 0.242/0.367/1.251/0.295 ms
[root@localhost ~]# 

![复制代码](https://common.cnblogs.com/images/copycode.gif)


说明：



实例5：时间间隔和次数限制的ping

命令：

ping -c 10 -i 0.5 192.168.120.206

输出：


![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@localhost ~]# ping -c 10 -i 0.5 192.168.120.206
PING 192.168.120.206 (192.168.120.206) 56(84) bytes of data.
64 bytes from 192.168.120.206: icmp_seq=1 ttl=64 time=1.24 ms
64 bytes from 192.168.120.206: icmp_seq=2 ttl=64 time=0.235 ms
64 bytes from 192.168.120.206: icmp_seq=3 ttl=64 time=0.244 ms
64 bytes from 192.168.120.206: icmp_seq=4 ttl=64 time=0.300 ms
64 bytes from 192.168.120.206: icmp_seq=5 ttl=64 time=0.255 ms
64 bytes from 192.168.120.206: icmp_seq=6 ttl=64 time=0.264 ms
64 bytes from 192.168.120.206: icmp_seq=7 ttl=64 time=0.263 ms
64 bytes from 192.168.120.206: icmp_seq=8 ttl=64 time=0.331 ms
64 bytes from 192.168.120.206: icmp_seq=9 ttl=64 time=0.247 ms
64 bytes from 192.168.120.206: icmp_seq=10 ttl=64 time=0.244 ms

--- 192.168.120.206 ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 4499ms
rtt min/avg/max/mdev = 0.235/0.362/1.241/0.294 ms
[root@localhost ~]# ping -c 10 -i 0.01 192.168.120.206
PING 192.168.120.206 (192.168.120.206) 56(84) bytes of data.
64 bytes from 192.168.120.206: icmp_seq=1 ttl=64 time=0.244 ms
64 bytes from 192.168.120.206: icmp_seq=2 ttl=64 time=0.195 ms
64 bytes from 192.168.120.206: icmp_seq=3 ttl=64 time=0.219 ms
64 bytes from 192.168.120.206: icmp_seq=4 ttl=64 time=0.204 ms
64 bytes from 192.168.120.206: icmp_seq=5 ttl=64 time=3.56 ms
64 bytes from 192.168.120.206: icmp_seq=6 ttl=64 time=1.93 ms
64 bytes from 192.168.120.206: icmp_seq=7 ttl=64 time=0.193 ms
64 bytes from 192.168.120.206: icmp_seq=8 ttl=64 time=0.193 ms
64 bytes from 192.168.120.206: icmp_seq=9 ttl=64 time=0.202 ms
64 bytes from 192.168.120.206: icmp_seq=10 ttl=64 time=0.211 ms

--- 192.168.120.206 ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 90ms
rtt min/avg/max/mdev = 0.193/0.716/3.564/1.080 ms
[root@localhost ~]# 

![复制代码](https://common.cnblogs.com/images/copycode.gif)




说明：



实例6：通过域名ping公网上的站点

命令：

ping -c 5 www.58.com

输出：


![复制代码](https://common.cnblogs.com/images/copycode.gif)

peida-VirtualBox ~ # ping -c 5 www.58.com
PING www.58.com (211.151.111.30) 56(84) bytes of data.
64 bytes from 211.151.111.30: icmp_req=1 ttl=49 time=14.7 ms
64 bytes from 211.151.111.30: icmp_req=2 ttl=49 time=16.4 ms
64 bytes from 211.151.111.30: icmp_req=3 ttl=49 time=15.2 ms
64 bytes from 211.151.111.30: icmp_req=4 ttl=49 time=14.6 ms
64 bytes from 211.151.111.30: icmp_req=5 ttl=49 time=19.9 ms

--- www.58.com ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 20101ms
rtt min/avg/max/mdev = 14.618/16.192/19.917/1.965 ms
peida-VirtualBox ~ # 

![复制代码](https://common.cnblogs.com/images/copycode.gif)


说明：



实例7：多参数使用

命令：

ping -i 3 -s 1024 -t 255 192.168.120.206

输出：


![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@localhost ~]# ping -i 3 -s 1024 -t 255 192.168.120.206
PING 192.168.120.206 (192.168.120.206) 1024(1052) bytes of data.
1032 bytes from 192.168.120.206: icmp_seq=1 ttl=64 time=1.99 ms
1032 bytes from 192.168.120.206: icmp_seq=2 ttl=64 time=0.694 ms
1032 bytes from 192.168.120.206: icmp_seq=3 ttl=64 time=0.300 ms
1032 bytes from 192.168.120.206: icmp_seq=4 ttl=64 time=0.481 ms
1032 bytes from 192.168.120.206: icmp_seq=5 ttl=64 time=0.415 ms
1032 bytes from 192.168.120.206: icmp_seq=6 ttl=64 time=0.600 ms
1032 bytes from 192.168.120.206: icmp_seq=7 ttl=64 time=0.411 ms
1032 bytes from 192.168.120.206: icmp_seq=8 ttl=64 time=0.281 ms
1032 bytes from 192.168.120.206: icmp_seq=9 ttl=64 time=0.318 ms
1032 bytes from 192.168.120.206: icmp_seq=10 ttl=64 time=0.362 ms
1032 bytes from 192.168.120.206: icmp_seq=11 ttl=64 time=0.408 ms
1032 bytes from 192.168.120.206: icmp_seq=12 ttl=64 time=0.445 ms
1032 bytes from 192.168.120.206: icmp_seq=13 ttl=64 time=0.397 ms
1032 bytes from 192.168.120.206: icmp_seq=14 ttl=64 time=0.406 ms
1032 bytes from 192.168.120.206: icmp_seq=15 ttl=64 time=0.458 ms

--- 192.168.120.206 ping statistics ---
15 packets transmitted, 15 received, 0% packet loss, time 41999ms
rtt min/avg/max/mdev = 0.281/0.531/1.993/0.404 ms
[root@localhost ~]# 

![复制代码](https://common.cnblogs.com/images/copycode.gif)


说明：

-i 3 发送周期为 3秒 -s 设置发送包的大小为1024 -t 设置TTL值为 255













> 
在大多数情况下，作为网络工程技术人员或者系统管理员会在UNIX主机系统下，直接执行命令行： 
**  Traceroute **hostname 
而在Windows系统下是执行Tracert的命令： 
**Tracert** hostname 
比如在北京地区使用windows NT 主机（已经与北京163建立了点对点的连接后） 
使用NT系统中的Tracert命令：(用户可用：开始->运行,输入"command" 调出command窗口使用此命令）

C:\>tracert www.yahoo.com

**参数说明：**

tracert [-d] [-h maximum_hops] [-j computer-list] [-w timeout] target_name

参数 
-d 
指定不对计算机名解析地址。 
-h maximum_hops 
指定查找目标的跳转的最大数目。 
-jcomputer-list 
指定在 computer-list 中松散源路由。 
-w timeout 
等待由 timeout 对每个应答指定的毫秒数。 
target_name 
目标计算机的名称。

**Traceroute的命令参数：**

Traceroute的用法为: Traceroute [options] <IP-address or domain-name> [data size]

[options]的内容有:

[-n]:显示的地址是用数字表示而不是符号

[-v]:长输出

[-p]:UDP端口设置(缺省为33434)

[-q]:设置TTL测试数目(缺省为3)

[-t]:设置测包的服务类型

[data size]:每次测试包的数据字节长度(缺省为38)

**Traceroute的工作原理：**
Traceroute最简单的基本用法是：traceroute hostname

Traceroute程序的设计是利用ICMP及IP header的TTL（Time To Live）栏位（field）。首先，traceroute送出一个TTL是1的IP datagram（其实，每次送出的为3个40字节的包，包括源地址，目的地址和包发出的时间标签）到目的地，当路径上的第一个路由器（router）收到这个datagram时，它将TTL减1。此时，TTL变为0了，所以该路由器会将此datagram丢掉，并送回一个「ICMP time exceeded」消息（包括发IP包的源地址，IP包的所有内容及路由器的IP地址），traceroute 收到这个消息后，便知道这个路由器存在于这个路径上，接着traceroute 再送出另一个TTL是2 的datagram，发现第2 个路由器...... traceroute 每次将送出的datagram的TTL 加1来发现另一个路由器，这个重复的动作一直持续到某个datagram 抵达目的地。当datagram到达目的地后，该主机并不会送回ICMP time exceeded消息，因为它已是目的地了，那么traceroute如何得知目的地到达了呢？

Traceroute在送出UDP datagrams到目的地时，它所选择送达的port number 是一个一般应用程序都不会用的号码（30000 以上），所以当此UDP datagram 到达目的地后该主机会送回一个「ICMP port unreachable」的消息，而当traceroute 收到这个消息时，便知道目的地已经到达了。所以traceroute 在Server端也是没有所谓的Daemon 程式。

Traceroute提取发 ICMP TTL到期消息设备的IP地址并作域名解析。每次 ，Traceroute都打印出一系列数据,包括所经过的路由设备的域名及 IP地址,三个包每次来回所花时间。

Traceroute 有一个固定的时间等待响应(ICMP TTL到期消息)。如果这个时间过了，它将打印出一系列的*号表明：在这个路径上，这个设备不能在给定的时间内发出ICMP TTL到期消息的响应。然后，Traceroute给TTL记数器加1，继续进行。










