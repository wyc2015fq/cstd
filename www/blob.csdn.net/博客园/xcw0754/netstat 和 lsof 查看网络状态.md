# netstat 和 lsof 查看网络状态 - xcw0754 - 博客园
# [netstat 和 lsof 查看网络状态](https://www.cnblogs.com/xcw0754/p/8732538.html)
`netstat`和`lsof`都是linux下的工具，可以用于查看系统的网络状态。
### netstat
netstat可以打印 网络连接，路由表，接口统计数据，还有多播和masquerade连接相关的东西(不熟悉，就不写了)。
大体上的用法如下
```
$netstat  [-o] [-p] [-v] [-c] [-t]  [-u]  [-w]  [-l]  [-a]  [-n]  [-N]  [address_family_options]  
            [--numeric-hosts] [--numeric-ports] [--numeric-users]  [--extend|-e[--extend|-e]]
$netstat  {-r}  [-v]   [-n]  [-c]  [address_family_options]   
            [--extend|-e[--extend|-e]]   [--numeric-hosts]   [--numeric-ports] [--numeric-users]
$netstat  {-i}  [-a]   [-v]  [-p]  [-n]  [-c]  
            [--numeric-hosts] [--numeric-ports] [--numeric-users] [--extend|-e[--extend|-e]]
$netstat {-s} [-t] [-u] [-w]
################### 下面的暂时用不到，暂时不了解 ###################
netstat {-g} [--numeric|-n] [--numeric-hosts] [--numeric-ports] [--numeric-users] [--continuous|-c]
netstat {-M} [--extend|-e] [--numeric|-n] [--numeric-hosts] [--numeric-ports] [--numeric-users] [--continuous|-c]
netstat {--version|-V}
netstat {--help|-h}
```
首个参数表示功能，比如`{-r}`是路由表相关，`{-i}`是接口相关(也就是lo，eth0这些)，`{-s}`是统计数据相关，第一种用法没有指定功能，默认打印出socket列表，如果没有指定协议族，就会都打印。
`address_family_options` 表示协议族，可以是[-4] [-6] [--protocol={inet,unix,ipx,ax25,netrom,ddp}[,...]] [--unix|-x] [--inet|--ip] [--ax25] [--ipx] [--netrom] [--ddp]中的各种组合。
`-v` 罗嗦模式
`-W` (不了解)
`-n` 不要解析地址、端口、用户等名称，这样打印会快很多
`--numeric-hosts` 包含在`-n`中
`--numeric-ports` 包含在`-n`中
`--numeric-users` 包含在`-n`中
`-A  family` 指定协议族(也许叫低层协议更合适？)，`family`可以是 inet, unix, ipx, ax25, netrom, ddp等，组合时用逗号隔开即可，效果同`--inet`，`--unix(即-x)`，`--ipx`，`--ax25`，`--netrom`，`--ddp`一样，其中inet就是raw、tcp、udp的组合。
`-c` 连续打印
`-e` 打印更多信息(列)
`-ee` 打印更更多信息(列)
`-o` Include information related to networking timers
`-p` 显示pid和程序名
`-l` 只显示listening的socket
`-a` 显示listening和非listening的socket
`-F` Print routing information from the FIB. (这是默认的参数，可不用)
`-C` Print routing information from the route cache. (route cache 不了解)
`-s` 统计数据，用来观察网络情况，主要是IP，ICMP，TCP，UDP的收发情况
举一些例子
- 
`$netstat -n -l -4`
查看本机在监听哪些端口，指明ipv4协议，不会显示"活跃的UNIX域套接字"
- 
`$netstat -n -l -x`
查看本机有哪些活跃的UNIX域套接字在监听着，也是不解析名字
- 
`$netstat  -n -4 -p`
打印连接所属的程序和pid
- 
`$netstat -ree`
详细打印路由表信息
- 
`$netstat -sw`
简单打印IP和ICMP的统计数据
下面是更详细的一些信息
- `$netstat -r`
显示路由表，Flag的意思如下
```
A Receive all multicast at this interface.
B OK broadcast.
D Debugging ON. 
M Promiscuous Mode.
O No ARP at this interface.
P P2P connection at this interface. 
R Interface is running. 
U Interface is up. 
G Not a direct entry.
```
- Internet连接套接字的两个列的意义
```
Recv-Q    --   The count of bytes not copied by the user program connected to this socket.
Send-Q    --   The count of bytes not acknowledged by the remote host.
```
- Internet连接套接字的`STATE`列是如下之一（也就是tcp的3次握手和4次挥手的状态）
```
ESTABLISHED
SYN_SENT
SYN_RECV
FIN_WAIT1
FIN_WAIT2
TIME_WAIT
CLOSE
CLOSE_WAIT
LAST_ACK
LISTEN
CLOSING
UNKNOWN
```
- UNIX域套接字的`STATE`列信息的意义
```
FREE   -- The socket is not allocated
LISTENING    --    The  socket  is  listening  for a connection request.  Such sockets are only included in the output if you specify the --listening (-l) or --all (-a) option.
CONNECTING   --   The socket is about to establish a connection.
CONNECTED    --  The socket is connected.
DISCONNECTING   --  The socket is disconnecting.
(empty)     --      The socket is not connected to another one.
UNKNOWN   --  This state should never happen.
```
- UNIX域套接字的`type`列信息的意义
```
SOCK_DGRAM    --  The socket is used in Datagram (connectionless) mode.
SOCK_STREAM    -- This is a stream (connection) socket.
SOCK_RAW    --  The socket is used as a raw socket.
SOCK_RDM    --  This one serves reliably-delivered messages.
SOCK_SEQPACKET    --  This is a sequential packet socket.
SOCK_PACKET    --  Raw interface access socket.
UNKNOWN    --  Who ever knows what the future will bring us - just fill in here :-)
```
- 
UNIX域套接字的`RefCnt`列信息的意义
`RefCnt` -- 引用次数 (如 attached processes via this socket).`
- `netstat`其实就是查看下面这些文件的信息再整理打印出来
```
/etc/services -- The services translation file
/proc -- Mount point for the proc filesystem, which gives access to kernel status information via the following files.
/proc/net/dev -- device information
/proc/net/raw -- raw socket information
/proc/net/tcp -- TCP socket information
/proc/net/udp -- UDP socket information
/proc/net/igmp -- IGMP multicast information
/proc/net/unix -- Unix domain socket information
/proc/net/ipx -- IPX socket information
/proc/net/ax25 -- AX25 socket information
/proc/net/appletalk -- DDP (appletalk) socket information
/proc/net/nr -- NET/ROM socket information
/proc/net/route -- IP routing information
/proc/net/ax25_route -- AX25 routing information
/proc/net/ipx_route -- IPX routing information
/proc/net/nr_nodes -- NET/ROM nodelist
/proc/net/nr_neigh -- NET/ROM neighbours
/proc/net/ip_masquerade -- masqueraded connections
/proc/net/snmp -- statistics
```
### lsof
没时间了，待续～

