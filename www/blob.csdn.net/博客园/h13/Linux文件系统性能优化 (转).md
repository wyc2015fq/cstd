# Linux文件系统性能优化 (转) - h13 - 博客园
[http://blog.chinaunix.net/uid-7530389-id-2050116.html](http://blog.chinaunix.net/uid-7530389-id-2050116.html)
由于各种的I/O负载情形各异，Linux系统中文件系统的缺省配置一般来说都比较中庸，强调普遍适用性。然而在特定应用下，这种配置往往在I/O性能方面不能达到最优。因此，如果应用对I/O性能要求较高，除了采用性能更高的硬件（如磁盘、HBA卡、CPU、MEM等）外，我们还可以通过对文件系统进行性能调优，来获得更高的I/O性能提升。总的来说，主要可以从三个方面来做工作：
1、Disk相关参数调优
2、文件系统本身参数调优
3、文件系统挂载(mount)参数调优
当然，负载情况不同，需要结合理论分析与充分的测试和实验来得到合理的参数。下面以SAS(Serial attached SCSI)磁盘上的EXT3文件系统为例，给出Linux文件系统性能优化的一般方法。请根据自身情况作适合调整，不要生搬硬套。
1、Disk相关参数
1.1 Cache mode:启用WCE=1(Write Cache Enable), RCD=0(Read Cache Disable)模式
sdparm -s WCE=1, RCD=0 -S /dev/sdb
1.2 Linux I/O scheduler算法
经过实验，在重负载情形下，deadline调度方式对squidI/O负载具有更好的性能表现。其他三种为noop(fifo), as, cfq，noop多用于SAN/RAID存储系统，as多用于大文件顺序读写，
cfq适于桌面应用。
echo deadline > /sys/block/sdb/queue/scheduler
1.3 deadline调度参数
对于redhat linux建议 read_expire = 1/2 write_expire，对于大量频繁的小文件I/O负载，应当这两者取较小值。更合适的值，需要通过实验测试得到。
echo 500 > /sys/block/sdb/queue/iosched/read_expire
echo 1000 > /sys/block/sdb/queue/iosched/write_expire
1.4 readahead 预读扇区数
预读是提高磁盘性能的有效手段，目前对顺序读比较有效，主要利用数据的局部性特点。比如在我的系统上，通过实验设置通读256块扇区性能较优。
blockdev --setra 256 /dev/sdb
2、EXT3文件系统参数
2.1 block size = 4096 (4KB)
mkfs.ext3 -b指定，大的数据块会浪费一定空间，但会提升I/O性能。EXT3文件系统块大小可以为1KB、2KB、4KB。
2.2 inode size
这是一个逻辑概念，即一个inode所对应的文件相应占用多大物理空间。mkfs.ext3 -i指定，可用文件系统文件大小平均值来设定，可减少磁盘寻址和元数据操作时间。
2.3 reserved block
mkfs.ext3 -m指定，缺省为5%，可调小该值以增大部分可用存储空间。
2.4 disable journal
对数据安全要求不高的应用（如web cache），可以关闭日志功能，以提高I/O性能。
tune2fs -O^has_journal /dev/sdb
3、mount参数
3.1 noatime, nodirtime
访问文件目录，不修改访问文件元信息，对于频繁的小文件负载，可以有效提高性能。
3.2 async
异步I/O方式，提高写性能。
3.3 data=writeback (if journal)
日志模式下，启用写回机制，可提高写性能。数据写入顺序不再保护，可能会造成文件系统数据不一致性，重要数据应用慎用。
3.4 barrier=0 (if journal)
barrier=1，可以保证文件系统在日志数据写入磁盘之后才写commit记录，但影响性能。重要数据应用慎用，有可能造成数据损坏。
4、小结
以/dev/sdb为例，优化操作方法如下，参数请自行调整。
sdparm -s WCE=1, RCD=0 -S /dev/sdb
echo deadline > /sys/block/sdb/queue/scheduler
echo 500 > /sys/block/sdb/queue/iosched/read_expire
echo 1000 > /sys/block/sdb/queue/iosched/write_expire
blockdev --setra 256 /dev/sdb
mkfs.ext3 -b 4096 -i 16384 -m 2 /dev/sdb1
tune2fs -O^has_journal /dev/sdb1
mount /dev/sdb1 /cache1 -o defaults,noatime,nodirtime,async,data=writeback,barrier=0 (if with journal)
mount /dev/sdb1 /cache1 -o defaults,noatime,nodirtime,async (if without journal)
**请大家注意不要模仿，每台服务器的业务不一样，情况不一样，不要在线上的服务器试验。**
性能分析工具
CPU性能分析工具：
vmstat
ps
sar
time
strace
pstree
top
**Memory性能分析工具：**
vmstat
strace
top
ipcs
ipcrm
cat /proc/meminfo
cat /proc/slabinfo
cat /proc/<pid #>/maps
I/O性能分析工具：
vmstat
iostat
repquota
quotacheck
Network性能分析工具：
ifconfig
ethereal
tethereal
iptraf
iwconfig
nfsstat
mrtg
ntop
netstat
cat /proc/sys/net
[**Linux**](http://www.cnblogs.com/hoys/admin/javascript:;)性能调优工具
当通过上述工具及命令，我们发现了应用的性能瓶颈以后，我们可以通过以下工具或者命令来进行性能的调整。
CPU性能调优工具：
nice / renic
sysctl
Memory性能调优工具：
swapon
ulimit
sysctl
I/O性能调优工具：
edquota
quoton
sysctl
boot line:
elevator= <ioscheduler>
Network性能调优工具：
ifconfig
iwconfig
sysctl
CPU性能调整
当一个系统的CPU空闲时间或者等待时间小于5%时，我们就可以认为系统的CPU资源耗尽，我们应该对CPU进行性能调整。
CPU性能调整方法：
编辑/proc/sys/kernel/中的文件，修改内核参数。
#cd /proc/sys/kernel/
# ls /proc/sys/kernel/
acct hotplug panic real-root-dev
cad_pid modprobe panic_on_oops sem
cap-bound msgmax pid_max shmall
core_pattern msgmnb powersave-nap shmmax
core_uses_pid msgmni print-fatal-signals shmmni
ctrl-alt-del ngroups_max printk suid_dumpable
domainname osrelease printk_ratelimit sysrq
exec-shield ostype printk_ratelimit_burst tainted
exec-shield-randomize overflowgid pty threads-max
hostname overflowuid random version
一般可能需要编辑的是pid_max和threads-max，如下：
# sysctl kernel.threads-max
kernel.threads-max = 8192
# sysctl kernel.threads-max=10000
kernel.threads-max = 10000
Memory性能调整
当一个应用系统的内存资源出现下面的情况时，我们认为需要进行Memory性能调整：
页面频繁换进换出；
缺少非活动页。
例如在使用vmstat命令时发现，memory的cache使用率非常低，而swap的si或者so则有比较高的数据值时，应该警惕内存的性能问题。
Memory性能调整方法：
1。关闭非核心的服务进程。
相关的方法请见CPU性能调整部分。
2。修改/proc/sys/vm/下的系统参数。
# ls /proc/sys/vm/
block_dump laptop_mode nr_pdflush_threads
dirty_background_ratio legacy_va_layout overcommit_memory
dirty_expire_centisecs lower_zone_protection overcommit_ratio
dirty_ratio max_map_count page-cluster
dirty_writeback_centisecs min_free_kbytes swappiness
hugetlb_shm_group nr_hugepages vfs_cache_pressure
# sysctl vm.min_free_kbytes
vm.min_free_kbytes = 1024
# sysctl -w vm.min_free_kbytes=2508
vm.min_free_kbytes = 2508
# cat /etc/sysctl.conf
…
vm.min_free_kbytes=2058
…
3。配置系统的swap交换分区等于或者2倍于物理内存。
# free
total used free shared buffers cached
Mem: 987656 970240 17416 0 63324 742400
-/+ buffers/cache: 164516 823140
Swap: 1998840 150272 1848568
I/O性能调整
系统出现以下情况时，我们认为该系统存在I/O性能问题：
系统等待I/O的时间超过50%；
一个设备的平均队列长度大于5。
我们可以通过诸如vmstat等命令，查看CPU的wa等待时间，以得到系统是否存在I/O性能问题的准确信息。
I/O性能调整方法：
1。修改I/O调度算法。
Linux已知的I/O调试算法有4种：
deadline - Deadline I/O scheduler
as - Anticipatory I/O scheduler
cfq - Complete Fair Queuing scheduler
noop - Noop I/O scheduler
可以编辑/etc/yaboot.conf文件修改参数elevator得到。
# vi /etc/yaboot.conf
image=/vmlinuz-2.6.9-11.EL
label=linux
read-only
initrd=/initrd-2.6.9-11.EL.img
root=/dev/VolGroup00/LogVol00
append="elevator=cfq rhgb quiet"
2。文件系统调整。
对于文件系统的调整，有几个公认的准则：
将I/O负载相对平均的分配到所有可用的磁盘上；
选择合适的文件系统，Linux内核支持reiserfs、ext2、ext3、jfs、xfs等文件系统；
# mkfs -t reiserfs -j /dev/sdc1
文件系统即使在建立后，本身也可以通过命令调优；
tune2fs (ext2/ext3)
reiserfstune (reiserfs)
jfs_tune (jfs)
3。文件系统Mount时可加入选项noatime、nodiratime。
# vi /etc/fstab
…
/dev/sdb1 /backup reiserfs acl, user_xattr, noatime, nodiratime 1 1
4。调整块设备的READAHEAD，调大RA值。
[root@overflowuid ~]# blockdev --report
RO RA SSZ BSZ StartSec Size Device
…
rw 256 512 4096 0 71096640 /dev/sdb
rw 256 512 4096 32 71094240 /dev/sdb1
[root@overflowuid ~]# blockdev --setra 2048 /dev/sdb1
[root@overflowuid ~]# blockdev --report
RO RA SSZ BSZ StartSec Size Device
…
rw 2048 512 4096 0 71096640 /dev/sdb
rw 2048 512 4096 32 71094240 /dev/sdb1
Network性能调整
一个应用系统出现如下情况时，我们认为该系统存在网络性能问题：
网络接口的吞吐量小于期望值；
出现大量的丢包现象；
出现大量的冲突现象。
Network性能调整方法：
1。调整网卡的参数。
# ethtool eth0
Settings for eth0:
Supported ports: [ TP ]
Supported link modes: 10baseT/Half 10baseT/Full
100baseT/Half 100baseT/Full
1000baseT/Full
Supports auto-negotiation: Yes
Advertised link modes: 10baseT/Half 10baseT/Full
100baseT/Half 100baseT/Full
1000baseT/Full
Advertised auto-negotiation: Yes
Speed: 100Mb/s
Duplex: Half
Port: Twisted Pair
PHYAD: 0
Transceiver: internal
Auto-negotiation: on
Supports Wake-on: d
Wake-on: d
Current message level: 0x00000007 (7)
Link detected: yes
#ethtool -s eth0 duplex full
#ifconfig eth0 mtu 9000 up
2。增加网络缓冲区和包的队列。
# cat /proc/sys/net/ipv4/tcp_mem
196608 262144 393216
# cat /proc/sys/net/core/rmem_default
135168
# cat /proc/sys/net/core/rmem_max
131071
# cat /proc/sys/net/core/wmem_default
135168
# cat /proc/sys/net/core/wmem_max
131071
# cat /proc/sys/net/core/optmem_max
20480
# cat /proc/sys/net/core/netdev_max_backlog
300
# sysctl net.core.rmem_max
net.core.rmem_max = 131071
# sysctl -w net.core.rmem_max=135168
net.core.rmem_max = 135168
3。调整Webserving。
# sysctl net.ipv4.tcp_tw_reuse
net.ipv4.tcp_tw_reuse = 0
# sysctl -w net.ipv4.tcp_tw_reuse=1
net.ipv4.tcp_tw_reuse = 1
# sysctl net.ipv4.tcp_tw_recycle
net.ipv4.tcp_tw_recycle = 0
# sysctl -w net.ipv4.tcp_tw_recycle=1
net.ipv4.tcp_tw_recycle = 1
一：IO优化
(1)网络IO优化，主要通过调整TCP参数调节
kern.maxproc
#系统可用的最大进程数
kern.ipc.maxsockbuf
##最大套接字缓冲区
kern.maxfiles=65536
##系统中允许的最多文件数量，缺省的是几千个但如果你在运行数据库或大的很吃描述符的进程可以
net.inet.tcp.sendspace=65535
##最大的待发送TCP 数据缓冲区空间，应用程序将数据放到这里就认为发送成功了，系统TCP堆栈保证数据的正常发送
net.inet.tcp.recvspace=65535
##最大的接受TCP缓冲区空间，系统从这里将数据分发给不同的套接字，增大该空间可提高系统瞬间接受数据的能力以提高性能
kern.ipc.somaxconn=8192
##最大的等待连接完成的套接字队列大小，高负载服务器和受到分布式服务阻塞攻击的系统也许会因为这个队列被塞满而不能提供正常服务。默认仅为128，根据机器和实际情况需要改动，太大就浪费了内存
net.inet.udp.recvspace=65535
##最大的接受UDP缓冲区大小
net.inet.udp.maxdgram=57344
##最大的发送UDP数据缓冲区大小
net.local.stream.recvspace=32768
##本地套接字连接的数据接收空间
net.local.stream.sendspace=65535
##本地套接字连接的数据发送空间
net.inet.icmp.drop_redirect=1
net inet.icmp.log_redirect=1‘
net.inet.ip.redirect=0
net.inet6.ip6.redirect=0
##屏蔽ICMP重定向功能
net.inet.icmp.bmcastecho=0
net.inet.icmp.maskrepl=0
##防止广播风暴
net.inet.icmp.icmplim=100
##限制系统发送ICMP速率
net.inet.tcp.always_keepalive=0
##设置为1会帮助系统清除没有正常断开的TCP连接，这增加了一些网络带宽的使用，但是一些死掉的
连接最终能被识别并清除。死的TCP连接是被拨号用户存取的系统的一个特别的问题，因为用户经常
断开modem而不正确的关闭活动的连接。
(2)磁盘IO优化
修改fstab,在加载参数上加入noatime ,禁止加入访问文件时间
控制swappness 参数，尽量减少应用的内存被交换到交换分区中，默认是60
块大小的优化 ,节点块的优化
block size = 4096(4K)
大的数据块会浪费一定的空间，比如此设置会使一个空的文件夹占用4K的空间，不过会提高I/O性能
inode size
这是一个逻辑概念，即一个inode所对应的文件相应占用多大物理空间,mkfs.ext3 -i指定，可用文件系统文件大小平均值来设定，可减少磁盘寻址和元数据操作时间
linux I/O调度方式启用异步方式，提高读写性能
有关IO的几个内核参数：
/proc/sys/vm/dirty_ratio
这个参数控制文件系统的文件系统写缓冲区的大小，单位是百分比，表示系统内存的百分比，表示当写缓冲使用到系统内存多少的时候，开始向磁盘写出数 据。增大之会使用更多系统内存用于磁盘写缓冲，也可以极大提高系统的写性能。但是，当你需要持续、恒定的写入场合时，应该降低其数值，一般启动上缺省是 10
/proc/sys/vm/dirty_expire_centisecs
这个参数声明Linux内核写缓冲区里面的数据多“旧”了之后，pdflush进程就开始考虑写到磁盘中去。单位是 1/100秒。缺省是 30000，也就是 30 秒的数据就算旧了，将会刷新磁盘。对于特别重载的写操作来说，这个值适当缩小也是好的，但也不能缩小太多，因为缩小太多也会导致IO提高太快。建议设置为 1500，也就是15秒算旧。
/proc/sys/vm/dirty_background_ratio
这个参数控制文件系统的pdflush进程，在何时刷新磁盘。单位是百分比，表示系统内存的百分比，意思是当写缓冲使用到系统内存多少的时候， pdflush开始向磁盘写出数据。增大之会使用更多系统内存用于磁盘写缓冲，也可以极大提高系统的写性能。但是，当你需要持续、恒定的写入场合时，应该 降低其数值，一般启动上缺省是 5
/proc/sys/vm/dirty_writeback_centisecs
这个参数控制内核的脏数据刷新进程pdflush的运行间隔。单位是 1/100 秒。缺省数值是500，也就是 5 秒。如果你的系统是持续地写入动作，那么实际上还是降低这个数值比较好，这样可以把尖峰的写操作削平成多次写操作
当然最主要的还是升级硬件或通过做RAID实现
用过的文件系统比较：
1、Reiserfs
　大量小文件访问，衡量指标是IOPS，文件系统性能瓶颈在于 文件元数据操作、目录操作、数据寻址。reiserfs对小文件作了优化，并使用B+ tree组织数据，加速了数据寻址，大大降低了open/create/delete/close等系统调用开销。mount时指定noatime, nodiratime, notail，减少不必要的inode操作，notail关闭tail package功能，以空间换取更高性能。因此，对于随机的小I/O读写，reiserfs是很好的选择。
2、Ext4
　大文件顺序访问，衡量指标是IO吞吐量，文件系统性能瓶颈在 于数据块布局(layout)、数据寻址。Ext4对ext3主要作了两方面的优化:
　一是inode预分配。这使得inode具有很好的局部性特 征，同一目录文件inode尽量放在一起，加速了目录寻址与操作性能。因此在小文件应用方面也具有很好的性能表现。
　二是extent/delay/multi的数据块分配策 略。这些策略使得大文件的数据块保持连续存储在磁盘上，数据寻址次数大大减少，显著提高I/O吞吐量。
因此，对于顺序大I/O读写，EXT4是很好的选择。另外，XFS性能在大文件方面也相当不错。
用到的命令：
检测硬盘的读效率:hdparm -tT /dev/hda。
检测硬盘的写效率：time dd if=/dev/zero of=/media/hda5/tmp/my-file bs=4k count=65536
写入字符到/media/hda5/tmp/my-file文件，bs为块大小，count为快数
系统IO情况：vmstat,如果wa大说明瓶颈在io上。iostat用于监视io情况
二：CPU 编译参数优化
CHOST参数： 对应CPU类型
CFLAGS参数：
-O3：相对于-O、-O1、-O2，-O3开启了所有GCC支持的优化特性；
-msse2：开启CPU所支持的sse2指令集；
-mmmx：参考-msse2
-mfpmath=sse：为指定指令集生成浮点运算特性（优化浮点运算功能）；
-mcpu：指定CPU型号；
-march：指定CPU型号，基本同-mcpu，GCC根据-march设定的CPU型号决定在生成代码时可以使用哪些种类的指令。（注 意！-march参数在某些CPU型号中不可用！）
-pipe：使用管道(pipe)方法替代临时文件方法在编译的不同阶段之间进行通讯。
三：系统优化
（1） 关闭不用的服务
（2） 程序执行效率优化
（3） 充分利用多核的优势
今天跟同事学了一招，可以通过修改swappiness内核参数，降低系统对swap的使用，从而提高系统的性能。
遇到的问题是这样的，新版本产品发布后，每小时对内存的使用会有一个尖峰。虽然这个峰值还远没有到达服务器的物理内存，但确发现内存使用达到峰值时系统开始使用swap。在swap的过程中系统性能会有所下降，表现为较大的服务延迟。对这种情况，可以通过调节swappiness内核参数降低系统对swap的使用，从而避免不必要的swap对性能造成的影响。
[这里](http://www.linuxvox.com/2009/10/what-is-the-linux-kernel-parameter-vm-swappiness/)有一篇关于swappiness内核参数的详细介绍。简单地说这个参数定义了系统对swap的使用倾向，默认值为60，值越大表示越倾向于使用swap。可以设为0，这样做并不会禁止对swap的使用，只是最大限度地降低了使用swap的可能性。通过sysctl -q vm.swappiness可以查看参数的当前设置。修改参数的方法是修改/etc/sysctl.conf文件，加入vm.swappiness=xxx，并重起系统。如果不想重起，可以通过sysctl -p动态加载/etc/sysctl.conf文件，但建议这样做之前先清空swap。

