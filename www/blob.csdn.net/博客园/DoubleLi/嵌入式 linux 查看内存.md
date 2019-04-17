# 嵌入式  linux 查看内存 - DoubleLi - 博客园






在Windows系统中查看[内存](http://product.it168.com/list/b/0205_1.shtml)的使用情况很简单，想必大家都已经耳熟能详了，那么在linux系统如何查看内存使用情况呢？下面和大家分享在Linux下查看内存使用情况的free命令：

　　[root@scs-2 tmp]# free

　　total used free shared buffers cached

　　Mem: 3266180 3250004 16176 0 110652 2668236

　　-/+ buffers/cache: 471116 2795064

　　Swap: 2048276 80160 1968116

　　下面是对这些数值的解释：

　　total:总计物理内存的大小。

　　used:已使用多大。

　　free:可用有多少。

　　Shared:多个进程共享的内存总额。

　　Buffers/cached:磁盘缓存的大小。

　　第三行(-/+ buffers/cached):

　　used:已使用多大。

　　free:可用有多少。

　　第四行就不多解释了。

　　区别：第二行(mem)的used/free与第三行(-/+ buffers/cache) used/free的区别。 这两个的区别在于使用的角度来看，第一行是从OS的角度来看，因为对于OS，buffers/cached 都是属于被使用，所以他的可用内存是16176KB,已用内存是3250004KB,其中包括，内核（OS）使用+Application(X, oracle,etc)使用的+buffers+cached.

　　第三行所指的是从应用程序角度来看，对于应用程序来说，buffers/cached 是等于可用的，因为buffer/cached是为了提高文件读取的性能，当应用程序需在用到内存的时候，buffer/cached会很快地被回收。

　　所以从应用程序的角度来说，可用内存=系统free memory+buffers+cached。

　　如上例：

　　2795064=16176+110652+2668236

　　接下来解释什么时候内存会被交换，以及按什么方交换。 当可用内存少于额定值的时候，就会开会进行交换。

　　如何看额定值：

　　cat /proc/meminfo

　　[root@scs-2 tmp]# cat /proc/meminfo

　　MemTotal: 3266180 kB

　　MemFree: 17456 kB

　　Buffers: 111328 kB

　　Cached: 2664024 kB

　　SwapCached: 0 kB

　　Active: 467236 kB

　　Inactive: 2644928 kB

　　HighTotal: 0 kB

　　HighFree: 0 kB

　　LowTotal: 3266180 kB

　　LowFree: 17456 kB

　　SwapTotal: 2048276 kB

　　SwapFree: 1968116 kB

　　Dirty: 8 kB

　　Writeback: 0 kB

　　Mapped: 345360 kB

　　Slab: 112344 kB

　　Committed_AS: 535292 kB

　　PageTables: 2340 kB

　　VmallocTotal: 536870911 kB

　　VmallocUsed: 272696 kB

　　VmallocChunk: 536598175 kB

　　HugePages_Total: 0

　　HugePages_Free: 0

　　Hugepagesize: 2048 kB

　　用free -m查看的结果：

　　[root@scs-2 tmp]# free -m

　　total used free shared buffers cached

　　Mem: 3189 3173 16 0 107 2605

　　-/+ buffers/cache: 460 2729

　　Swap: 2000 78 1921

　　查看/proc/kcore文件的大小（内存镜像）：

　　[root@scs-2 tmp]# ll -h /proc/kcore

　　-r——– 1 root root 4.1G Jun 12 12:04 /proc/kcore

　　备注：

　　占用内存的测量

　　测量一个进程占用了多少内存，linux为我们提供了一个很方便的方法，/proc目录为我们提供了所有的信息，实际上top等工具也通过这里来获取相应的信息。

　　/proc/meminfo 机器的内存使用信息

　　/proc/pid/maps pid为进程号，显示当前进程所占用的虚拟地址。

　　/proc/pid/statm 进程所占用的内存

　　[root@localhost ~]# cat /proc/self/statm

　　654 57 44 0 0 334 0

　　输出解释

[CPU](http://product.it168.com/list/b/0217_1.shtml) 以及CPU0。。。的每行的每个参数意思（以第一行为例）为：

　　参数 解释 /proc//status

　　Size (pages) 任务虚拟地址空间的大小 VmSize/4

　　Resident(pages) 应用程序正在使用的物理内存的大小 VmRSS/4

　　Shared(pages) 共享页数 0

　　Trs(pages) 程序所拥有的可执行虚拟内存的大小 VmExe/4

　　Lrs(pages) 被映像到任务的虚拟内存空间的库的大小 VmLib/4

　　Drs(pages) 程序数据段和用户态的栈的大小 （VmData+ VmStk ）4

　　dt(pages) 04

　　查看机器可用内存

　　/proc/28248/>free

　　total used free shared buffers cached

　　Mem: 1023788 926400 97388 0 134668 503688

　　-/+ buffers/cache: 288044 735744

　　Swap: 1959920 89608 1870312

　　我们通过free命令查看机器空闲内存时，会发现free的值很小。这主要是因为，在linux中有这么一种思想，内存不用白不用，因此它尽可能的cache和buffer一些数据，以方便下次使用。但实际上这些内存也是可以立刻拿来使用的。

　　所以 空闲内存=free+buffers+cached=total-used

$ free -m
             total       used       free     shared    buffers     cached
Mem:         1002        769        232          0         62        421
-/+ buffers/cache:        286        715
Swap:         1153          0       1153

第一部分Mem行:
total 内存总数: 1002M
used 已经使用的内存数: 769M
free 空闲的内存数: 232M
shared 当前已经废弃不用，总是0
buffers Buffer 缓存内存数: 62M
cached Page 缓存内存数:421M

关系：total(1002M) = used(769M) + free(232M)

第二部分(-/+ buffers/cache):
(-buffers/cache) used内存数：286M (指的第一部分Mem行中的used - buffers - cached)
(+buffers/cache) free内存数: 715M (指的第一部分Mem行中的free + buffers + cached)

可见-buffers/cache反映的是被程序实实在在吃掉的内存，而+buffers/cache反映的是可以挪用的内存总数。(从这里我们可以看出，实际上 ：可用内存＝第一部分Mem行中的free + buffers + cached,并不是只有free部分)


第三部分是指交换分区, 我想不讲大家都明白.

我想大家看了上面,还是很晕.第一部分(Mem)与第二部分(-/+ buffers/cache)的结果中有关used和free为什么这么奇怪.
其实我们可以从二个方面来解释.
对操作系统来讲是Mem的参数.buffers/cached 都是属于被使用,所以它认为free只有232.
对应用程序来讲是(-/+ buffers/cach).buffers/cached 是等同可用的，因为buffer/cached是为了提高程序执行的性能，当程序使用内存时，buffer/cached会很快地被使用。

所以,以应用来看看,以(-/+ buffers/cache)的free和used为主.所以我们看这个就好了.另外告诉大家一些常识.Linux为了提高磁盘和内存存取效率, Linux做了很多精心的设计, 除了对dentry进行缓存(用于VFS,加速文件路径名到inode的转换), 还采取了两种主要Cache方式：Buffer Cache和Page Cache。前者针对磁盘块的读写，后者针对文件inode的读写。这些Cache能有效缩短了 I/O系统调用(比如read,write,getdents)的时间。

记住内存是拿来用的,不是拿来看的.不象windows,无论你的真实物理内存有多少,他都要拿硬盘交换文件来读.这也就是windows为什么常 常提示虚拟空间不足的原因.你们想想,多无聊,在内存还有大部分的时候,拿出一部分硬盘空间来充当内存.硬盘怎么会快过内存.所以我们看linux,只要 不用swap的交换空间,就不用担心自己的内存太少.如果常常swap用很多,可能你就要考虑加物理内存了.这也是linux看内存是否够用的标准哦.


补充：cat /proc/meminfo

可以查看内存的详细使用情况,free的结果由此文件生成。









