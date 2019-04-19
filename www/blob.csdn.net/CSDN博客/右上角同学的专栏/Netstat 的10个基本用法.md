# Netstat 的10个基本用法 - 右上角同学的专栏 - CSDN博客
2016年07月15日 11:07:04[右上角徐](https://me.csdn.net/u011032983)阅读数：1841
# Netstat 的10个基本用法
####                                                                                                                   本文转自：http://www.linuxidc.com/Linux/2014-01/94644.htm
#### Netstat 简介
Netstat 是一款命令行工具，可用于列出系统上所有的网络套接字连接情况，包括 tcp, udp 以及 unix 套接字，另外它还能列出处于监听状态（即等待接入请求）的套接字。如果你想确认系统上的 Web 服务有没有起来，你可以查看80端口有没有打开。以上功能使 netstat 成为网管和系统管理员的必备利器。在这篇教程中，我会列出几个例子，教大家如何使用 netstat 去查找网络连接信息和系统开启的端口号。
![](http://www.linuxidc.com/upload/2014_01/140103082191361.png)
以下的简单介绍来自 netstat 的 man 手册：
> 
netstat - 打印网络连接、路由表、连接的数据统计、伪装连接以及广播域成员。
#### 1. 列出所有连接
第一个要介绍的，是最简单的命令：列出所有当前的连接。使用 -a 选项即可。
$ netstat -a
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 enlightened:domain      *:*                     LISTEN     
tcp        0      0 localhost:ipp           *:*                     LISTEN     
tcp        0      0 enlightened.local:54750 li240-5.members.li:http ESTABLISHED
tcp        0      0 enlightened.local:49980 del01s07-in-f14.1:https ESTABLISHED
tcp6       0      0 ip6-localhost:ipp       [::]:*                  LISTEN     
udp        0      0 enlightened:domain      *:*                                
udp        0      0 *:bootpc                *:*                                
udp        0      0 enlightened.local:ntp   *:*                                
udp        0      0 localhost:ntp           *:*                                
udp        0      0 *:ntp                   *:*                                
udp        0      0 *:58570                 *:*                                
udp        0      0 *:mdns                  *:*                                
udp        0      0 *:49459                 *:*                                
udp6       0      0 fe80::216:36ff:fef8:ntp [::]:*                             
udp6       0      0 ip6-localhost:ntp       [::]:*                             
udp6       0      0 [::]:ntp                [::]:*                             
udp6       0      0 [::]:mdns               [::]:*                             
udp6       0      0 [::]:63811              [::]:*                             
udp6       0      0 [::]:54952              [::]:*                             
Active UNIX domain sockets (servers and established)
Proto RefCnt Flags       Type       State         I-Node   Path
unix  2      [ ACC ]     STREAM     LISTENING     12403    @/tmp/dbus-IDgfj3UGXX
unix  2      [ ACC ]     STREAM     LISTENING     40202    @/dbus-vfs-daemon/socket-6nUC6CCx
上述命令列出 tcp, udp 和 unix 协议下所有套接字的所有连接。然而这些信息还不够详细，管理员往往需要查看某个协议或端口的具体连接情况。
#### 2. 只列出 TCP 或 UDP 协议的连接
使用 **-t** 选项列出 TCP 协议的连接：
$ netstat -at
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 enlightened:domain      *:*                     LISTEN     
tcp        0      0 localhost:ipp           *:*                     LISTEN     
tcp        0      0 enlightened.local:36310 del01s07-in-f24.1:https ESTABLISHED
tcp        0      0 enlightened.local:45038 a96-17-181-10.depl:http ESTABLISHED
tcp        0      0 enlightened.local:37892 ABTS-North-Static-:http ESTABLISHED
.....
使用 **-u** 选项列出 UDP 协议的连接：
$ netstat -au
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
udp        0      0 *:34660                 *:*                                
udp        0      0 enlightened:domain      *:*                                
udp        0      0 *:bootpc                *:*                                
udp        0      0 enlightened.local:ntp   *:*                                
udp        0      0 localhost:ntp           *:*                                
udp        0      0 *:ntp                   *:*                                
udp6       0      0 fe80::216:36ff:fef8:ntp [::]:*                             
udp6       0      0 ip6-localhost:ntp       [::]:*                             
udp6       0      0 [::]:ntp                [::]:*
上面同时显示了 IPv4 和 IPv6 的连接。
#### 3. 禁用反向域名解析，加快查询速度
默认情况下 netstat 会通过反向域名解析技术查找每个 IP 地址对应的主机名。这会降低查找速度。如果你觉得 IP 地址已经足够，而没有必要知道主机名，就使用 **-n** 选项禁用域名解析功能。
$ netstat -ant
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 127.0.1.1:53            0.0.0.0:*               LISTEN     
tcp        0      0 127.0.0.1:631           0.0.0.0:*               LISTEN     
tcp        0      0 192.168.1.2:49058       173.255.230.5:80        ESTABLISHED
tcp        0      0 192.168.1.2:33324       173.194.36.117:443      ESTABLISHED
tcp6       0      0 ::1:631                 :::*                    LISTEN
上述命令列出所有 TCP 协议的连接，没有使用域名解析技术。So easy ? 非常好。
#### 4. 只列出监听中的连接
任何网络服务的后台进程都会打开一个端口，用于监听接入的请求。这些正在监听的套接字也和连接的套接字一样，也能被 netstat 列出来。使用 **-l** 选项列出正在监听的套接字。
$ netstat -tnl
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 127.0.1.1:53            0.0.0.0:*               LISTEN     
tcp        0      0 127.0.0.1:631           0.0.0.0:*               LISTEN     
tcp6       0      0 ::1:631                 :::*                    LISTEN
现在我们可以看到处于监听状态的 TCP 端口和连接。如果你查看所有监听端口，去掉 **-t** 选项。如果你只想查看 UDP 端口，使用 **-u** 选项，代替 **-t** 选项。
注意：不要使用 **-a** 选项，否则 netstat 会列出所有连接，而不仅仅是监听端口。
相关阅读：
Linux netstat命令 [http://www.linuxidc.com/Linux/2013-06/85528.htm](http://www.linuxidc.com/Linux/2013-06/85528.htm)
Linux netstat命令详解 [http://www.linuxidc.com/Linux/2012-12/75667.htm](http://www.linuxidc.com/Linux/2012-12/75667.htm)
Linux命令：service & netstat  [http://www.linuxidc.com/Linux/2011-12/48395.htm](http://www.linuxidc.com/Linux/2011-12/48395.htm)
Linux下用netstat命令查看网络负载状况的一条语句 [http://www.linuxidc.com/Linux/2011-08/41429.htm](http://www.linuxidc.com/Linux/2011-08/41429.htm)
