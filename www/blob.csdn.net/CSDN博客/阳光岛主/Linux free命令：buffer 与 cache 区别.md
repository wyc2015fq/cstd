
# Linux free命令：buffer 与 cache 区别 - 阳光岛主 - CSDN博客

2018年01月20日 12:04:44[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2832


Linux free命令查询剩余可用内存的最常用命令，其中 buffer 与 cache 有何区别呢？
米扑博客，专门总结了一篇博客《[Linux free命令：buffer 与 cache 区别](https://blog.mimvp.com/article/16951.html)》，分享到CSDN
更多经典技术博客，请见我的米扑博客：[https://blog.mimvp.com](https://blog.mimvp.com)

**free 命令**
free 命令相对于top 提供了更简洁的查看系统内存使用情况
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|homer@homer-pc:~$|free|--help
|Usage:
|free|[options]
|Options:
|-b, --bytes         show output|in|bytes
|-k, --kilo          show output|in|kilobytes
|-m, --mega          show output|in|megabytes
|-g, --giga          show output|in|gigabytes
|--tera          show output|in|terabytes
|-h, --human         show human-readable output
|--si            use powers of 1000 not 1024
|-l, --lohi          show detailed low and high memory statistics
|-o, --old           use old|format|(without -/+buffers|/cache|line)
|-t, --total         show total|for|RAM + swap
|-s N, --seconds N   repeat printing every N seconds
|-c N, --count N     repeat printing N|times|,|then|exit
|--help     display this help and|exit
|-V, --version  output version information and|exit
|

**free 实例**
|1
|2
|3
|4
|5
|6
|homer@homer-pc:~$
|homer@homer-pc:~$|free
|total       used|free|shared    buffers     cached
|Mem:       3779056    3008500     770556     169980     140868    1025484
|-/+ buffers|/cache|:    1842148    1936908
|Swap:      4095992          0    4095992
|

Mem：表示物理内存统计
-/+ buffers/cached：表示物理内存的缓存统计
Swap：表示硬盘上交换分区的使用情况
系统的总物理内存：3779056Kb（free 默认单位为 kb），但系统当前真正可用的内存并不是第一行free 标记的 770556Kb，它仅代表未被分配的内存。
使用total1、used1、free1（Mem）；used2、free2（-/+buffers/cache） 等名称来代表上面统计数据的各值，1、2 分别代表第一行和第二行的数据。
**Mem**
total1：表示物理内存总量
used1：表示总计分配给缓存（包含buffers 与cache ）使用的数量，但其中可能部分缓存并未实际使用
free1：未被分配的内存
shared1：共享内存，一般系统不会用到，这里也不讨论
buffers1：系统分配但未被使用的buffers 数量
cached1：系统分配但未被使用的cache 数量
**-/+buffers/cache**
used2：实际使用的buffers 与cache 总量，也是实际使用的内存总量。
free2：未被使用的buffers 与cache 和未被分配的内存之和，这就是系统当前实际可用内存。
可以整理出如下等式：
total1 = used1 + free1
total1 = used2 + free2
used1 =buffers1 + cached1+ used2
free2 =buffers1 + cached1+ free1
**buffer 与 cache 区别**
A buffer is something that has yet tobe "written" to disk.
A cache is something that has been"read" from the diskand stored for later use.
**buffers**就是存放要输出到disk（块设备）的数据，缓冲满了一次写，提高io性能（内存 -> 磁盘）
**cached**就是存放从disk上读出的数据，常用的缓存起来，减少io（磁盘 -> 内存）
buffer 和 cache，两者都是RAM中的数据。
简单来说，buffer是即将要被写入磁盘的，cache是被从磁盘中读出来的
**1） buffer 缓冲**
buffer是用于存储速度不同步的设备或优先级不同的设备之间传输数据的区域。
缓冲（buffers）是根据磁盘的读写设计的，把分散的写操作集中进行，减少磁盘碎片和硬盘的反复寻道，从而提高系统性能。
**2） cache 缓存**
cache经常被用在磁盘的I/O请求上，如果有多个进程都要访问某个文件，于是该文件便被做成cache以方便下次被访问，这样可提供系统性能。
缓存（cached）是把读取过的数据保存起来，重新读取时若命中（找到需要的数据）就不要去读硬盘了，若没有命中就读硬盘。其中的数据会根据读取频率进行组织，把最频繁读取的内容放在最容易找到的位置，把不再读的内容不断往后排，直至从中删除。
**free 与 top 关系**
**top**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|homer@homer-pc:~$|top|||head
|top|- 13:46:27 up  1:32,  0|users|,  load average: 0.42, 0.60, 0.65
|Tasks: 227 total,   1 running, 226 sleeping,   0 stopped,   0 zombie
|%Cpu(s): 12.6 us,  3.9 sy,  0.1 ni, 78.4|id|,  3.1 wa,  0.0 hi,  1.8 si,  0.0 st
|KiB Mem:   3779056 total,  3203612 used,   575444|free|,   142436 buffers
|KiB Swap:  4095992 total,        0 used,  4095992|free|.  1031324 cached Mem
|PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
|2714 homer     20   0 1214744 170404  70136 S   6.4  4.5   5:15.92 chrome
|1 root      20   0   34004   3320   1488 S   0.0  0.1   0:01.74 init
|2 root      20   0       0      0      0 S   0.0  0.0   0:00.00 kthreadd
|

**free**
|1
|2
|3
|4
|5
|homer@homer-pc:~$|free
|total       used|free|shared    buffers     cached
|Mem:       3779056    3203336     575720     172416     142436    1031324
|-/+ buffers|/cache|:    2029576    1749480
|Swap:      4095992          0    4095992
|

抽取上面 top 和 free 的mem 做对比，如下：
KiB Mem:3779056 total,  3203612 used,   575444 free,   142436 buffers
KiB Swap:  4095992 total,        0 used,  4095992 free.1031324 cached Mem
top 蓝色部分的数值，正好与free的第一行数据近似一致：
total       used       free     shared    buffers     cached
Mem:3779056    3203336     575720172416142436    1031324
但是top命令，无法给出free 第二行的 -/+ buffers/cache 数据（即物理内存的实际使用与可用的内存，上面分别为 2029576Kb 和 1749480Kb），因此得出结论：
top命令不能完全反映出物理内存的实际使用量，推荐用free查看物理内存的实际使用量
另外，推荐一款软件**htop**，帮助进一步验证实际物理内存使用情况
**htop 安装：**
sudo apt-get -y install htop
**htop 查看：**
![linux-free-command-difference-between-buffer-and-cache-12](https://blog.mimvp.com/wp-content/uploads/2017/11/linux-free-command-difference-between-buffer-and-cache-12.png)
上图 Mem：2125/3690MB，表示实际物理内存使用了 2125MB，总物理内存为 3690MB，与free命令的第二行的 -/+ buffers/cache 数据近似一致。

**参考推荐**：
[Linux top 命令详解](https://blog.mimvp.com/article/11229.html)（推荐）
[Linux sar 系统性能分析命令详解](https://blog.mimvp.com/article/16228.html)（推荐）
[Perf — Linux下的系统性能调优工具](https://blog.mimvp.com/article/15661.html)


