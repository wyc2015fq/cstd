# Linux UDP严重丢包问题的解决 - xqhrs232的专栏 - CSDN博客
2018年08月03日 14:48:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：119
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[https://blog.csdn.net/u013267687/article/details/52594126](https://blog.csdn.net/u013267687/article/details/52594126)
测试系统在Linux上的性能发现丢包率极为严重，发210000条数据，丢包达110000之巨，丢包率超过50%。同等情形下Windows上测试，仅丢几条数据。形势严峻，必须解决。考虑可能是因为协议栈Buffer太低所致，于是先看看默认情况：
sysctl -a |grep net.core
发现
net.core.rmem_max = 131071
net.core.rmem_default = 112640
修改吧，变大一点，变成10M，然后reboot（应该重启某个服务即可）
然后查网卡收包情况：
netstat -su
结果如下：
Udp:
97690 packets received
112310 packets to unknown port received.
0 packet receive errors
20 packets sent
发现数据在网卡就丢了，判断可能是防火墙引起的，于是执行命令：
iptables -L
结果如下：
Chain INPUT (policy ACCEPT)
target prot opt source destination
Chain FORWARD (policy ACCEPT)
target prot opt source destination
Chain OUTPUT (policy ACCEPT)
target prot opt source destination
证明iptables启动，于是停止防火墙：
service iptables stop
这个命令即时生效，开启防火墙的命令：
service iptables start
如果要彻底关闭防火墙，则需要重启后生效
开启： chkconfig iptables on
关闭： chkconfig iptables off
在开启了防火墙时，做如下设置，开启相关端口，
修改/etc/sysconfig/iptables 文件，添加以下内容：
-A RH-Firewall-1-INPUT -m state –state NEW -m tcp -p tcp –dport 80 -j ACCEPT
-A RH-Firewall-1-INPUT -m state –state NEW -m tcp -p tcp –dport 22 -j ACCEPT
重新测试，没丢一条数据。
linux 内核参数调整说明
所有的TCP/IP调优参数都位于/proc/sys/net/目录。例如, 下面是最重要的一些调优参数, 后面是它们的含义:
1. /proc/sys/net/core/rmem_max — 最大的TCP数据接收缓冲。
2. /proc/sys/net/core/wmem_max — 最大的TCP数据发送缓冲。
3. /proc/sys/net/ipv4/tcp_timestamps — 时间戳在(请参考RFC 1323)TCP的包头增加12个字节。
4. /proc/sys/net/ipv4/tcp_sack — 有选择的应答。
5. /proc/sys/net/ipv4/tcp_window_scaling — 支持更大的TCP窗口. 如果TCP窗口最大超过65535(64KB), 必须设置该数值为1。
6. rmem_default — 默认的接收窗口大小。
7. rmem_max — 接收窗口的最大大小。
8. wmem_default — 默认的发送窗口大小。
9. wmem_max — 发送窗口的最大大小。
/proc目录下的所有内容都是临时性的, 所以重启动系统后任何修改都会丢失。
建议在系统启动时自动修改TCP/IP参数:
把下面代码增加到/etc/rc.local文件, 然后保存文件, 系统重新引导的时候会自动修改下面的TCP/IP参数:
echo 256960 > /proc/sys/net/core/rmem_default
echo 256960 > /proc/sys/net/core/rmem_max
echo 256960 > /proc/sys/net/core/wmem_default
echo 256960 > /proc/sys/net/core/wmem_max
echo 0 > /proc/sys/net/ipv4/tcp_timestamps
echo 1 > /proc/sys/net/ipv4/tcp_sack
echo 1 > /proc/sys/net/ipv4/tcp_window_scaling
TCP/IP参数都是自解释的, TCP窗口大小设置为256960, 禁止TCP的时间戳(取消在每个数据包的头中增加12字节), 支持更大的TCP窗口和TCP有选择的应答。
上面数值的设定是根据互连网连接和最大带宽/延迟率来决定。
注: 上面实例中的数值可以实际应用, 但它只包含了一部分参数。
另外一个方法: 使用 /etc/sysctl.conf 在系统启动时把参数配置成您所设置的值:
net.core.rmem_default = 256960
net.core.rmem_max = 256960
net.core.wmem_default = 256960
net.core.wmem_max = 256960
net.ipv4.tcp_timestamps = 0
net.ipv4.tcp_sack =1
net.ipv4.tcp_window_scaling = 1
设置Linux内核参数
配置 Linux 内核参数(2种方法),修改后不用重启动更新: /sbin/sysctl -p
第一种:打开/etc/sysctl.conf 复制如下内容
kernel.shmall = 2097152
kernel.shmmax = 2147483648
kernel.shmmni = 4096
kernel.sem = 250 32000 100 128
fs.file-max = 65536
net.ipv4.ip_local_port_range = 1024 65000
net.core.rmem_default=262144
net.core.wmem_default=262144
net.core.rmem_max=262144
net.core.wmem_max=262144
第二种:打开终端
cat >> /etc/sysctl.conf<
kernel.shmall = 2097152
kernel.shmmax = 2147483648
kernel.shmmni = 4096
kernel.sem = 250 32000 100 128
fs.file-max = 65536
net.ipv4.ip_local_port_range = 1024 65000
net.core.rmem_default=262144
net.core.wmem_default=262144
net.core.rmem_max=262144
net.core.wmem_max=262144
EOF
这里，对每个参数值做个简要的解释和说明。
（1）shmmax：该参数定义了共享内存段的最大尺寸（以字节为单位）。缺省为32M，对于oracle来说，该缺省值太低了，通常将其设置为2G。
（2）shmmni：这个内核参数用于设置系统范围内共享内存段的最大数量。该参数的默认值是 4096 。通常不需要更改。
（3）shmall：该参数表示系统一次可以使用的共享内存总量（以页为单位）。缺省值就是2097152，通常不需要修改。
（4）sem：该参数表示设置的信号量。
（5）file-max：该参数表示文件句柄的最大数量。文件句柄设置表示在linux系统中可以打开的文件数量。
修改好内核以后，执行下面的命令使新的配置生效。
[root @linux1 /root]# /sbin/sysctl -p
以 root 用户身份运行以下命令来验证您的设置：
/sbin/sysctl -a | grep shm
/sbin/sysctl -a | grep sem
/sbin/sysctl -a | grep file-max
/sbin/sysctl -a | grep ip_local_port_range
例如：
# /sbin/sysctl -a | grep shm
kernel.shmmni = 4096
kernel.shmall = 2097152
kernel.shmmax = 2147483648
kernel.shm-use-bigpages = 0
# /sbin/sysctl -a | grep sem
kernel.sem = 250 32000 100 128
# /sbin/sysctl -a | grep file-max
fs.file-max = 65536
# /sbin/sysctl -a | grep ip_local_port_range
net.ipv4.ip_local_port_range = 1024 65000
如果系统的参数设置的比上述参数值小，则编辑 /etc/sysctl.conf 文件，添加或更改这些参数。完成后，运行以下命令激活更改：
/sbin/sysctl -p
PS：通常用默认参数就可以了吧.
