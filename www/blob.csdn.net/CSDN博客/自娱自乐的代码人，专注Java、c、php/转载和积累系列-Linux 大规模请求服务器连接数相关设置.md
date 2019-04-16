# 转载和积累系列 - Linux 大规模请求服务器连接数相关设置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年05月27日 16:32:56[initphp](https://me.csdn.net/initphp)阅读数：899








**【原创】Linux 大规模请求服务器连接数相关设置**


作者：heiyeluren




一般一个大规模Linux服务器请求数可能是几十万上百万的情况，需要足够的连接数来使用，所以务必进行相应的设置。




默认的Linux服务器文件描述符等打开最大是1024，用ulimit -a 查看：




[viewuser@~]$ ulimit -a
core file size          (blocks, -c) 0   #coredump 文件大小
data seg size           (kbytes, -d) unlimited

scheduling priority             (-e) 0

file size               (blocks, -f) unlimited

pending signals                 (-i) 255622

max locked memory       (kbytes, -l) 64

max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024   #打开文件数量，root账户无限制
pipe size            (512 bytes, -p) 8

POSIX message queues     (bytes, -q) 819200

real-time priority              (-r) 0

stack size              (kbytes, -s) 8192

cpu time               (seconds, -t) unlimited
max user processes              (-u) 4096   #root用户本项是无限
virtual memory          (kbytes, -v) unlimited

file locks                      (-x) unlimited




如果超过了连接数量，可以在 /var/log/message 里面看到类似：

May 14 16:13:52 hostname kernel: nf_conntrack: table full, dropping packet




的信息，基本可以判定是fd不够用引起的。（服务器受到攻击也会有这个信息）










设置要求：假设我们要设置为200W最大打开文件描述符




**1. 修改 nr_open 限制** （用途：能够配置nofile最大数）

cat /proc/sys/fs/nr_open 

Linux 内核 2.6.25 以前，在内核里面宏定义是1024*1024，最大只能是100w（1048576），所以不要设置更大的值，如果Linux内核大于
 2.6.25 则可以设置更大值。

设置方法：

su**do** bash -c 'echo 2000000 > /proc/sys/fs/nr_open'
注意：只有修改了 nr_open 限制，才能修改下面的限制。（如果 nr_open 的默认现有值如果高于我们的200w，那么可以不用修改）





**2. 打开文件描述符限制**：修改 limits.conf 的nofile软硬打开文件限制（用途：tcp连接数）




(1) 临时生效




如果想针对当前登陆session临时生效文件描述符修改，可以直接使用 ulimit 命令：

ulimit -SHn 2000000  



再执行相应的程序就能够正常使用不会超过限制，但是重启服务器会失效。

如果想一直生效，可以把这个内容保存到启动里面，同步到 ： /etc/rc.local 文件

sudo echo "ulimit -SHn2000000" >> /etc/rc.local



注意：如果需要让 /etc/rc.local 下次启动生效，务必记得有该文件必须有执行权限：sudo chmod +x /etc/rc.local




下次启动会自动执行这句，也是可以正常使用的。




(2) 永久生效



文件位置：/etc/security/limits.conf 

查找 nofile ，如果没有，则在自己最后加上：

2.6.25 及以前内核设置为100W：

|*        soft     nofile  1000000  *        hard    nofile  1000000|
|----|




2.6.25 以后版本内核可以设置为200W：

|*        soft     nofile  2000000  *        hard    nofile  2000000|
|----|





设置后保存本文件。（本操作必须重启才生效，如果无法重启，会无法生效，不确定是否使用 /sbin/sysctl -p 是否可以直接生效）



说明：如果需要 limits.conf生效，有部分需要加载/lib/security/pam_limits.so才能生效（默认情况一般不关心），如果需要关注，则需要在 /etc/pam.d/login 在末尾追加 session required /lib/security/pam_limits.so ，但是目前新版内核应该都没问题问题，可以忽略。




**3. 打开进程限制**：修改
 limits.conf 中的nproc限制 （用途：进程数）




说明：如果你对进程总数量没有特殊要求，可以不修改本选项，如果你是一个高性能多进程的server，需要很多进程来处理，那么可以修改本选项。

ulimit -a 里可以看到 max user processes 如果值是比较大的，可以不用设置 nproc 项。

配置文件：/etc/security/limits.d/20-nproc.conf
  （RHEL 7/CentOS 7）

|*          soft    nproc  4096root     soft    nproc  unlimited|
|----|





就是root无限（实际root用户限制是：255622），其他非root用户是4096个进程。




说明：

硬限制表明soft限制中所能设定的最大值。 soft限制指的是当前系统生效的设置值。 hard限制值可以被普通用户降低。但是不能增加。 soft限制不能设置的比hard限制更高。 只有root用户才能够增加hard限制值。

当增加文件限制描述，可以简单的把当前值双倍。 例子如下， 如果你要提高默认值1024， 最好提高到2048， 如果还要继续增加， 就需要设置成4096。




**4. 修改 file-max 选项** （用途：可分配文件句柄数目）


file-max 价值：指定了可以分配的文件句柄的最大数目（可以使用 /proc/sys/fs/file-nr 文件查看到当前已经使用的文件句柄和总句柄数。）



(1) 临时生效：

文件路径：/proc/sys/fs/file-max
cat /proc/sys/fs/file-max
3252210如果要修改，直接覆盖文件：（比如改成200w）
sudo echo 2000000 > /proc/sys/fs/file-max注意：如果你想每次启动都自动执行上面的命令，可以在系统启动配置文件/etc/rc.local里面添加一句命令：（跟永久生效差不多）echo 2000000 > /proc/sys/fs/file-max或者直接Shell全搞定：echo "echo 2000000 > /proc/sys/fs/file-max" >> /etc/rc.local注意：如果需要让 /etc/rc.local 下次启动生效，务必记得有该文件必须有执行权限：sudo chmod +x /etc/rc.local(2) 永久生效：修改配置文件，文件位置：/etc/sysctl.conf
打开配置文件到最末尾，如果配置文件里没有则可以直接添加：sudo echo "fs.file-max = 2000000" >>/etc/sysctl.conf
配置文件生效：sudo /sbin/sysctl -p






**5. 修改TCP等相关选项**



配置文件：/etc/sysctl.conf

修改选项：
|net.core.somaxconn = 2048   net.core.rmem_default = 262144  net.core.wmem_default = 262144  net.core.rmem_max = 16777216  net.core.wmem_max = 16777216  net.core.netdev_max_backlog = 20000  net.ipv4.tcp_rmem = 4096 4096 16777216  net.ipv4.tcp_wmem = 4096 4096 16777216  net.ipv4.tcp_mem = 786432 2097152 3145728  net.ipv4.tcp_max_syn_backlog = 16384  net.ipv4.tcp_fin_timeout = 30net.ipv4.tcp_keepalive_time = 300net.ipv4.tcp_max_tw_buckets = 5000net.ipv4.tcp_tw_reuse = 1  net.ipv4.tcp_tw_recycle = 1  net.ipv4.tcp_syncookies = 1net.ipv4.tcp_max_orphans = 131072net.netfilter.nf_conntrack_max = 6553500net.netfilter.nf_conntrack_tcp_timeout_established = 1200|
|----|

配置文件生效：sudo
 /sbin/sysctl -p



以上选项也可以直接给 /proc/sys/net/ 目录下面按照各个选项可以直接使用类似于 echo VALUE > /proc/sys/net/core/wmem_max 来直接修改内存临时值生效。




主要看这几项：

-  net.ipv4.tcp_rmem 用来配置读缓冲的大小，三个值，第一个是这个读缓冲的最小值，第三个是最大值，中间的是默认值。我们可以在程序中修改读缓冲的大小，但是不能超过最小与最大。为了使每个socket所使用的内存数最小，我这里设置默认值为4096。
  
- net.ipv4.tcp_wmem 用来配置写缓冲的大小。读缓冲与写缓冲在大小，直接影响到socket在内核中内存的占用。
  
- net.ipv4.tcp_mem 则是配置tcp的内存大小，其单位是页，而不是字节。当超过第二个值时，TCP进入 pressure模式，此时TCP尝试稳定其内存的使用，当小于第一个值时，就退出pressure模式。当内存占用超过第三个值时，TCP就拒绝分配
 socket了，查看dmesg，会打出很多的日志“TCP: too many of orphaned sockets”。
- net.ipv4.tcp_max_orphans 这个值也要设置一下，这个值表示系统所能处理不属于任何进程的 socket数量，当我们需要快速建立大量连接时，就需要关注下这个值了。当不属于任何进程的socket的数量大于这个值时，dmesg就会看
 到”too many of orphaned sockets”。
- net.ipv4.tcp_syncookies = 1表示开启SYN Cookies。当出现SYN等待队列溢出时，启用cookies来处理，可防范少量SYN攻击，默认为0，表示关闭；
- net.ipv4.tcp_tw_reuse = 1表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；
- net.ipv4.tcp_tw_recycle = 1表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。
- net.ipv4.tcp_fin_timeout修改系統默认的TIMEOUT时间
- net.ipv4.tcp_max_syn_backlog 进入SYN包的最大请求队列.默认1024.对重负载服务器,增加该值显然有好处.可调整到16384.
- net.ipv4.tcp_keepalive_time = 300 表示当keepalive起用的时候，TCP发送keepalive消息的频度。缺省是2小时，改为300秒。
- net.ipv4.tcp_max_tw_buckets = 5000 表示系统同时保持TIME_WAIT套接字的最大数量，如果超过这个数字，TIME_WAIT套接字将立刻被清除并打印警告信息。默认为180000，改为5000。





注意：如果是客户端程序，为了更好的访问server程序不是卡在端口分配上，建议把客户端的端口（port_range）范围开大一些：

修改文件：/etc/sysctl.conf

|net.ipv4.ip_local_port_range = 1024 65535|
|----|



配置生效：sudo /sbin/sysctl
 -p

如果是客户端，其他文件打开限制等可以参考上面的来设置。







**6. 其他一些配置**




(1) 打开core文件


如果为了观察程序是否正常，出现问题后生成相应映像文件，可以开启coredump相关的操作，可以打开：（非必须，如果线上环境，担心影响稳定性，可以考虑不开启）

配置文件：/etc/security/limits.conf

修改配置文件：

增加： 

|*        soft     core   102400*        hard    core   2048003|
|----|





建议设置为无限大小：

|*        soft     core   unlimited*        hard    core   unlimited|
|----|


然后重启机器生效（不确定是否可以使用 /sbin/sysctl -p 生效），使用： ulimit -a 或 ulimit
 -c 查看结果，后续如果程序出现栈溢出等都会生成coredump文件，方便用gdb等追查问题原因。










参考URL：
[http://www.tuicool.com/articles/r6NvQr](http://www.tuicool.com/articles/r6NvQr)
[http://blog.yufeng.info/archives/2568#more-2568](http://blog.yufeng.info/archives/2568#more-2568)
[http://heylinux.com/archives/2938.html](http://heylinux.com/archives/2938.html)
[http://club.1688.com/article/8964198.html](http://club.1688.com/article/8964198.html)
[http://blog.csdn.net/qxk1989/article/details/7822792](http://blog.csdn.net/qxk1989/article/details/7822792)
[http://blog.csdn.net/unix21/article/details/8743537](http://blog.csdn.net/unix21/article/details/8743537)
[http://blog.csdn.net/force_eagle/article/details/6725243](http://blog.csdn.net/force_eagle/article/details/6725243)
[http://blog.csdn.net/guichenglin/article/details/7753047](http://blog.csdn.net/guichenglin/article/details/7753047)
[http://blog.csdn.net/wenchao126/article/details/7528543](http://blog.csdn.net/wenchao126/article/details/7528543)
[http://blog.csdn.net/zhangxinrun/article/details/7621028](http://blog.csdn.net/zhangxinrun/article/details/7621028)


