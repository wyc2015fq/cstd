# Linux 内核运行参数修改——sysctl命令 - z69183787的专栏 - CSDN博客
2018年08月15日 16:51:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：193
[https://blog.csdn.net/u012707739/article/details/78254241](https://blog.csdn.net/u012707739/article/details/78254241)
sysctl命令被用于在内核运行时动态地修改内核的运行参数，可用的内核参数在目录`/proc/sys`中。它包含一些TCP/ip堆栈和虚拟内存系统的高级选项，用sysctl可以读取设置超过五百个系统变量。
### **sysctl命令**
**用法：**
sysctl [options] [variable[=value] …]
**常用选项：**
-n：打印时只打印值，不打印参数名称； 
-e：忽略未知关键字错误； 
-N：打印时只打印参数名称，不打印值； 
-w：设置参数的值（不过好像不加这个选项也可以直接设置）； 
-p：从配置文件“/etc/sysctl.conf”加载内核参数设置； 
-a：打印所有内核参数变量； 
-A：以表格方式打印所有内核参数变量。
### **示例**
**1、 设置socket接收缓冲最大值（rmem_max）为212992字节**
`sudo sysctl -w net.core.rmem_max=212992`- 1
**2、查看socket接收缓冲最大值（rmem_max）**
`sysctl net.core.rmem_max`- 1
sysctl可以设置查看的内核参数都在目录`/proc/sys`中，每个文件都记录了一个参数，sysctl命令中其实也记录了参数的相对路径，只要将 . 换成 / 就是文件路径，比如记录net.core.rmem_max的文件就是`/proc/sys/net/core/rmem_max`，所以也可以直接查看文件内容来读取参数的值
`cat /proc/sys/net/core/rmem_max`- 1
如果不知道参数路径也可以这么做：
`sysctl -a | grep rmem_max`- 1
### **配置sysctl**
编辑此文件`vi /etc/sysctl.conf`
如果该文件为空，则输入以下内容，否则请根据情况自己做调整（复制自[http://man.linuxde.net/sysctl](http://man.linuxde.net/sysctl)）：
```
# Controls source route verification
# Default should work for all interfaces
net.ipv4.conf.default.rp_filter = 1
# net.ipv4.conf.all.rp_filter = 1
# net.ipv4.conf.lo.rp_filter = 1
# net.ipv4.conf.eth0.rp_filter = 1
# Disables IP source routing
# Default should work for all interfaces
net.ipv4.conf.default.accept_source_route = 0
# net.ipv4.conf.all.accept_source_route = 0
# net.ipv4.conf.lo.accept_source_route = 0
# net.ipv4.conf.eth0.accept_source_route = 0
# Controls the System Request debugging functionality of the kernel
kernel.sysrq = 0
# Controls whether core dumps will append the PID to the core filename.
# Useful for debugging multi-threaded applications.
kernel.core_uses_pid = 1
# Increase maximum amount of memory allocated to shm
# Only uncomment if needed!
# kernel.shmmax = 67108864
# Disable ICMP Redirect Acceptance
# Default should work for all interfaces
net.ipv4.conf.default.accept_redirects = 0
# net.ipv4.conf.all.accept_redirects = 0
# net.ipv4.conf.lo.accept_redirects = 0
# net.ipv4.conf.eth0.accept_redirects = 0
# Enable Log Spoofed Packets, Source Routed Packets, Redirect Packets
# Default should work for all interfaces
net.ipv4.conf.default.log_martians = 1
# net.ipv4.conf.all.log_martians = 1
# net.ipv4.conf.lo.log_martians = 1
# net.ipv4.conf.eth0.log_martians = 1
# Decrease the time default value for tcp_fin_timeout connection
net.ipv4.tcp_fin_timeout = 25
# Decrease the time default value for tcp_keepalive_time connection
net.ipv4.tcp_keepalive_time = 1200
# Turn on the tcp_window_scaling
net.ipv4.tcp_window_scaling = 1
# Turn on the tcp_sack
net.ipv4.tcp_sack = 1
# tcp_fack should be on because of sack
net.ipv4.tcp_fack = 1
# Turn on the tcp_timestamps
net.ipv4.tcp_timestamps = 1
# Enable TCP SYN Cookie Protection
net.ipv4.tcp_syncookies = 1
# Enable ignoring broadcasts request
net.ipv4.icmp_echo_ignore_broadcasts = 1
# Enable bad error message Protection
net.ipv4.icmp_ignore_bogus_error_responses = 1
# Make more local ports available
# net.ipv4.ip_local_port_range = 1024 65000
# Set TCP Re-Ordering value in kernel to ‘5′
net.ipv4.tcp_reordering = 5
# Lower syn retry rates
net.ipv4.tcp_synack_retries = 2
net.ipv4.tcp_syn_retries = 3
# Set Max SYN Backlog to ‘2048′
net.ipv4.tcp_max_syn_backlog = 2048
# Various Settings
net.core.netdev_max_backlog = 1024
# Increase the maximum number of skb-heads to be cached
net.core.hot_list_length = 256
# Increase the tcp-time-wait buckets pool size
net.ipv4.tcp_max_tw_buckets = 360000
# This will increase the amount of memory available for socket input/output queues
net.core.rmem_default = 65535
net.core.rmem_max = 8388608
net.ipv4.tcp_rmem = 4096 87380 8388608
net.core.wmem_default = 65535
net.core.wmem_max = 8388608
net.ipv4.tcp_wmem = 4096 65535 8388608
net.ipv4.tcp_mem = 8388608 8388608 8388608
net.core.optmem_max = 40960
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
如果希望屏蔽别人 ping 你的主机，则加入以下代码：
```
# Disable ping requests
net.ipv4.icmp_echo_ignore_all = 1
```
- 1
- 2
编辑完成后，请执行以下命令使变动立即生效：
```
/sbin/sysctl -p
/sbin/sysctl -w net.ipv4.route.flush=1
```
- 1
- 2
### **TCP/IP参数**
所有的TCP/IP参数都位于/proc/sys/net目录下（请注意，对/proc/sys/net目录下内容的修改都是临时的，任何修改在系统重启后都会丢失），例如下面这些重要的参数：
|参数（路径+文件）|描述|
|----|----|
|/proc/sys/net/core/rmem_default|默认的TCP数据接收窗口大小（字节）。|
|/proc/sys/net/core/rmem_max|最大的TCP数据接收窗口（字节）。|
|/proc/sys/net/core/wmem_default|默认的TCP数据发送窗口大小（字节）。|
|/proc/sys/net/core/wmem_max|最大的TCP数据发送窗口（字节）。|
|/proc/sys/net/core/netdev_max_backlog|在每个网络接口接收数据包的速率比内核处理这些包的速率快时，允许送到队列的数据包的最大数目。|
|/proc/sys/net/core/somaxconn|定义了系统中每一个端口最大的监听队列的长度，这是个全局的参数。|
|/proc/sys/net/core/optmem_max|表示每个套接字所允许的最大缓冲区的大小。|
|/proc/sys/net/ipv4/tcp_mem|确定TCP栈应该如何反映内存使用，每个值的单位都是内存页（通常是4KB）。第一个值是内存使用的下限；第二个值是内存压力模式开始对缓冲区使用应用压力的上限；第三个值是内存使用的上限。在这个层次上可以将报文丢弃，从而减少对内存的使用。对于较大的BDP可以增大这些值（注意，其单位是内存页而不是字节）。|
|/proc/sys/net/ipv4/tcp_rmem|为自动调优定义socket使用的内存。第一个值是为socket接收缓冲区分配的最少字节数；第二个值是默认值（该值会被rmem_default覆盖），缓冲区在系统负载不重的情况下可以增长到这个值；第三个值是接收缓冲区空间的最大字节数（该值会被rmem_max覆盖）。|
|/proc/sys/net/ipv4/tcp_wmem|为自动调优定义socket使用的内存。第一个值是为socket发送缓冲区分配的最少字节数；第二个值是默认值（该值会被wmem_default覆盖），缓冲区在系统负载不重的情况下可以增长到这个值；第三个值是发送缓冲区空间的最大字节数（该值会被wmem_max覆盖）。|
|/proc/sys/net/ipv4/tcp_keepalive_time|TCP发送keepalive探测消息的间隔时间（秒），用于确认TCP连接是否有效。|
|/proc/sys/net/ipv4/tcp_keepalive_intvl|探测消息未获得响应时，重发该消息的间隔时间（秒）。|
|/proc/sys/net/ipv4/tcp_keepalive_probes|在认定TCP连接失效之前，最多发送多少个keepalive探测消息。|
|/proc/sys/net/ipv4/tcp_sack|启用有选择的应答（1表示启用），通过有选择地应答乱序接收到的报文来提高性能，让发送者只发送丢失的报文段，（对于广域网通信来说）这个选项应该启用，但是会增加对CPU的占用。|
|/proc/sys/net/ipv4/tcp_fack|启用转发应答，可以进行有选择应答（SACK）从而减少拥塞情况的发生，这个选项也应该启用。|
|/proc/sys/net/ipv4/tcp_timestamps|TCP时间戳（会在TCP包头增加12个字节），以一种比重发超时更精确的方法（参考RFC 1323）来启用对RTT 的计算，为实现更好的性能应该启用这个选项。|
|/proc/sys/net/ipv4/tcp_window_scaling|启用RFC 1323定义的window scaling，要支持超过64KB的TCP窗口，必须启用该值（1表示启用），TCP窗口最大至1GB，TCP连接双方都启用时才生效。|
|/proc/sys/net/ipv4/tcp_syncookies|表示是否打开TCP同步标签（syncookie），内核必须打开了CONFIG_SYN_COOKIES项进行编译，同步标签可以防止一个套接字在有过多试图连接到达时引起过载。|
|/proc/sys/net/ipv4/tcp_tw_reuse|表示是否允许将处于TIME-WAIT状态的socket（TIME-WAIT的端口）用于新的TCP连接 。|
|/proc/sys/net/ipv4/tcp_tw_recycle|能够更快地回收TIME-WAIT套接字。|
|/proc/sys/net/ipv4/tcp_fin_timeout|对于本端断开的socket连接，TCP保持在FIN-WAIT-2状态的时间（秒）。对方可能会断开连接或一直不结束连接或不可预料的进程死亡。|
|/proc/sys/net/ipv4/ip_local_port_range|表示TCP/UDP协议允许使用的本地端口号|
|/proc/sys/net/ipv4/tcp_max_syn_backlog|对于还未获得对方确认的连接请求，可保存在队列中的最大数目。如果服务器经常出现过载，可以尝试增加这个数字。|
|/proc/sys/net/ipv4/tcp_low_latency|允许TCP/IP栈适应在高吞吐量情况下低延时的情况，这个选项应该禁用。|
|/proc/sys/net/ipv4/tcp_westwood|启用发送者端的拥塞控制算法，它可以维护对吞吐量的评估，并试图对带宽的整体利用情况进行优化，对于WAN 通信来说应该启用这个选项。|
|/proc/sys/net/ipv4/tcp_bic|为快速长距离网络启用Binary Increase Congestion，这样可以更好地利用以GB速度进行操作的链接，对于WAN通信应该启用这个选项。|
