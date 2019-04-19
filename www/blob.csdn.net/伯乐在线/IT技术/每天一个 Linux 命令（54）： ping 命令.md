# 每天一个 Linux 命令（54）： ping 命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2013/03/06/2945407.html)
Linux系统的ping命令是常用的网络命令，它通常用来测试与目标主机的连通性，我们经常会说“ping一下某机器，看是不是开着”、不能打开网页时会说“你先ping网关地址192.168.1.1试试”。它通过发送ICMP ECHO_REQUEST数据包到网络主机（send ICMP ECHO_REQUEST to network hosts），并显示响应情况，这样我们就可以根据它输出的信息来确定目标主机是否可访问（但这不是绝对的）。有些服务器为了防止通过ping探测到，通过防火墙设置了禁止ping或者在内核参数中禁止ping，这样就不能通过ping确定该主机是否还处于开启状态。
linux下的ping和windows下的ping稍有区别,linux下ping不会自动终止,需要按ctrl+c终止或者用参数-c指定要求完成的回应次数。
1.命令格式：

Shell
```
ping [参数] [主机名或IP地址]
```
2.命令功能：
ping命令用于：确定网络和各外部主机的状态；跟踪和隔离硬件和软件问题；测试、评估和管理网络。如果主机正在运行并连在网上，它就对回送信号进行响应。每个回送信号请求包含一个网际协议（IP）和 ICMP 头，后面紧跟一个 tim 结构，以及来填写这个信息包的足够的字节。缺省情况是连续发送回送信号请求直到接收到中断信号（Ctrl-C）。
ping 命令每秒发送一个数据报并且为每个接收到的响应打印一行输出。ping 命令计算信号往返时间和(信息)包丢失情况的统计信息，并且在完成之后显示一个简要总结。ping 命令在程序超时或当接收到 SIGINT 信号时结束。Host 参数或者是一个有效的主机名或者是因特网地址。
3.命令参数：

Shell
```
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
```
4.使用实例：
实例1：ping的通的情况
命令：

Shell
```
ping 192.168.120.205
```
输出：
```
Shell
			
			
```
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
```
```
说明：
实例2：ping不通的情况
命令：

Shell
```
ping 192.168.120.202
```
输出：
```
Shell
			
			
```
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
```
```
说明：
实例3：ping网关
命令：

Shell
```
ping -b 192.168.120.1
```
输出：
```
Shell
			
			
```
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
```
```
说明：
实例4：ping指定次数
命令：

Shell
```
ping -c 10 192.168.120.206
```
输出：
```
Shell
			
			
```
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
```
```
说明：
实例5：时间间隔和次数限制的ping
命令：

Shell
```
ping -c 10 -i 0.5 192.168.120.206
```
输出：
```
Shell
			
			
```
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
```
```
说明：
实例6：通过域名ping公网上的站点
命令：

Shell
```
ping -c 5 www.58.com
```
输出：
```
Shell
			
			
```
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
```
```
说明：
实例7：多参数使用
命令：

Shell
```
ping -i 3 -s 1024 -t 255 192.168.120.206
```
输出：
```
Shell
			
			
```
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
```
```
说明：
-i 3 发送周期为 3秒 -s 设置发送包的大小为1024 -t 设置TTL值为 255
### 本系列文章：
[每天一个 Linux 命令（1）：ls命令](http://blog.jobbole.com/108531/)
[每天一个 Linux 命令（2）：cd命令](http://blog.jobbole.com/108805/)
[每天一个 Linux 命令（3）：pwd命令](http://blog.jobbole.com/108855/)
[每天一个 Linux 命令（4）：mkdir命令](http://blog.jobbole.com/108871/)
[每天一个 Linux 命令（5）：rm 命令](http://blog.jobbole.com/108901/)
[每天一个 Linux 命令（6）：rmdir 命令](http://blog.jobbole.com/108925/)
[每天一个 Linux 命令（7）：mv命令](http://blog.jobbole.com/108983/)
[每天一个 Linux 命令（8）：cp 命令](http://blog.jobbole.com/108988/)
[每天一个 Linux 命令（9）：touch 命令](http://blog.jobbole.com/108989/)
[每天一个 Linux 命令（10）：cat 命令](http://blog.jobbole.com/108990/)
[每天一个 Linux 命令（11）：nl 命令](http://blog.jobbole.com/108991/)
[每天一个 Linux 命令（12）：more 命令](http://blog.jobbole.com/108992/)
[每天一个 Linux 命令（13）：less 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（14）：head 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（15）：tail 命令](http://blog.jobbole.com/108995/)
[每天一个 Linux 命令（16）：which命令](http://blog.jobbole.com/109262/)
[每天一个 Linux 命令（17）：whereis 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（18）：locate 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（19）：find 命令概览](http://blog.jobbole.com/109265/)
[每天一个 Linux 命令（20）：find命令之exec](http://blog.jobbole.com/108996/)
[每天一个 Linux 命令（21）：find命令之xargs](http://blog.jobbole.com/108997/)
[每天一个 Linux 命令（22）：find 命令的参数详解](http://blog.jobbole.com/109268/)
[每天一个 Linux 命令（23）：Linux 目录结构](http://blog.jobbole.com/109506/)
[每天一个 Linux 命令（24）：Linux 文件类型与扩展名](http://blog.jobbole.com/109254/)
[每天一个 Linux 命令（25）：Linux 文件属性详解](http://blog.jobbole.com/105269)
[每天一个 Linux 命令（26）：用 SecureCRT 来上传和下载文件](http://blog.jobbole.com/109539)
[每天一个 Linux 命令（27）：linux chmod 命令](http://blog.jobbole.com/109614)
[每天一个 Linux 命令（28）：tar 命令](http://blog.jobbole.com/109674/)
[每天一个 Linux 命令（29）: chgrp 命令](http://blog.jobbole.com/109696/)
[每天一个 Linux 命令（30）: chown 命令](http://blog.jobbole.com/109750/)
[每天一个 Linux 命令（31）: /etc/group 文件详解](http://blog.jobbole.com/109781/)
[每天一个 Linux 命令（32）：gzip 命令](http://blog.jobbole.com/109797/)
[每天一个 Linux 命令（33）：df 命令](http://blog.jobbole.com/109827/)
[每天一个 Linux 命令（34）： du 命令](http://blog.jobbole.com/109846/)
[每天一个 Linux 命令（35）： ln 命令](http://blog.jobbole.com/109869/)
[每天一个 Linux 命令（36）： diff 命令](http://blog.jobbole.com/109875/)
[每天一个 Linux 命令（37）： date 命令](http://blog.jobbole.com/109881/)
[每天一个 Linux 命令（38）： cal 命令](http://blog.jobbole.com/109886/)
[每天一个 Linux 命令（39）： grep 命令](http://blog.jobbole.com/109907/)
[每天一个 Linux 命令（40）： wc 命令](http://blog.jobbole.com/109925/)
[每天一个 Linux 命令（41）： ps 命令](http://blog.jobbole.com/109948/)
[每天一个 Linux 命令（44）： top 命令](http://blog.jobbole.com/109951/)
[每天一个 Linux 命令（45）： free 命令](http://blog.jobbole.com/109965/)
[每天一个 Linux 命令（46）： vmstat 命令](http://blog.jobbole.com/109970/)
[每天一个 Linux 命令（47）： iostat 命令](http://blog.jobbole.com/109980/)
[每天一个 Linux 命令（48）： watch 命令](http://blog.jobbole.com/109994/)
[每天一个 Linux 命令（49）： at 命令](http://blog.jobbole.com/110001/)
[每天一个 Linux 命令（50）： crontab 命令](http://blog.jobbole.com/110005/)
[每天一个 Linux 命令（51）： lsof 命令](http://blog.jobbole.com/110011/)
[每天一个 Linux 命令（52）： ifconfig 命令](http://blog.jobbole.com/110016/)
[每天一个 Linux 命令（53）： route 命令](http://blog.jobbole.com/110019/)
