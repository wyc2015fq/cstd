# 转载和积累系列 - linux 内核参数调整优化网络 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年05月08日 15:14:38[initphp](https://me.csdn.net/initphp)阅读数：824
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)










Linux系统内核设置优化tcp网络，# vi /etc/sysctl.conf，添加以下内容

net.ipv4.tcp_syncookies = 1

表示开启SYN Cookies。当出现SYN等待队列溢出时，启用cookies来处理，可防范少量SYN攻击，默认为0，表示关闭；

net.ipv4.tcp_tw_reuse = 1

表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；

net.ipv4.tcp_tw_recycle = 1

表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。

net.ipv4.tcp_fin_timeout = 30

表示如果套接字由本端要求关闭，这个参数决定了它保持在FIN-WAIT-2状态的时间。

net.ipv4.tcp_keepalive_time = 1200

表示当keepalive起用的时候，TCP发送keepalive消息的频度。缺省是2小时

net.ipv4.tcp_keepalive_intvl = 30

net.ipv4.tcp_keepalive_probes = 3

上面两行意思是，如果probe 3次(每次30秒)不成功,内核才彻底放弃。

原来的默认值为，显然太大：

tcp_keepalive_time = 7200 seconds (2 hours)

tcp_keepalive_probes = 9

tcp_keepalive_intvl = 75 seconds

net.ipv4.ip_local_port_range = 1024 65000

表示用于向外连接的端口范围。缺省情况下很小：32768到61000，改为1024到65000。

net.ipv4.tcp_max_syn_backlog = 8192

表示SYN队列的长度，默认为1024，加大队列长度为8192，可以容纳更多等待连接的网络连接数。

net.ipv4.netdev_max_backlog = 1000

表示进入包的最大设备队列，默认300，改大

net.core.tcp_max_tw_buckets = 5000

表示系统同时保持TIME_WAIT套接字的最大数量，如果超过这个数字，TIME_WAIT套接字将立刻被清除并打印警告信息。默认为180000，改为 5000。对于Apache、Nginx等服务器，上几行的参数可以很好地减少TIME_WAIT套接字数量，但是对于Squid，效果却不大。此项参数 可以控制TIME_WAIT套接字的最大数量，避免Squid服务器被大量的TIME_WAIT套接字拖死。

另外可以参考优化内核配置：
/proc/sys/net/core/wmem_max 最大socket写buffer,可参考的优化值:873200

/proc/sys/net/core/rmem_max 最大socket读buffer,可参考的优化值:873200

/proc/sys/net/ipv4/tcp_wmem TCP写buffer,可参考的优化值: 8192 436600 873200

/proc/sys/net/ipv4/tcp_rmem TCP读buffer,可参考的优化值: 32768 436600 873200

/proc/sys/net/ipv4/tcp_mem

同样有3个值,意思是:

net.ipv4.tcp_mem[

/proc/sys/net/core/somaxconn

listen()的默认参数,挂起请求的最大数量.默认是128.对繁忙的服务器,增加该值有助于网络性能.

可调整到256.

/proc/sys/net/core/optmem_max

socket buffer的最大初始化值,默认10K.

/proc/sys/net/ipv4/tcp_retries2

TCP失败重传次数,默认值15,意味着重传15次才彻底放弃.可减少到5,以尽早释放内核资源.

还有一个重要参数：net.core.somaxconn

表示socket监听（listen）的backlog上限，是socket的监听队列，当一个请求（request）尚未被处理或建立时，他会进入backlog。而socket server可以一次性处理backlog中的所有请求，处理后的请求不再位于监听队列中。当server处理请求较慢，以至于监听队列被填满后，新来的请求会被拒绝。

该参数默认值128。在高突发的请求中可能会导致链接超时或者触发重传。比如nginx 定义NGX_LISTEN_BACKLOG默认到511, 却由于我们参数未曾优化会限制到128,显然这里限制了nginx的backlog，需要优化
net.core.somaxconn = 32768




