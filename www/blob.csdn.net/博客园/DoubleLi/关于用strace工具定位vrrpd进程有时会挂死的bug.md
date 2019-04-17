# 关于用strace工具定位vrrpd进程有时会挂死的bug - DoubleLi - 博客园






> 
只做工作总结备忘之用。

正在烧镜像，稍总结一下进来改bug遇到的问题。






> 
一个项目里要用到L3 switch的nat，vrrp功能，但实地测试中偶然出现write file挂死的情况，但不是必现。交付在即，于是加调试信息，反复跑配置的脚本，定位bug。




一，期初怀疑是vtysh与vrrpd进程通信出现阻塞（现象即是系统挂死）。

> 
(1)因为在跑配置脚本时，出现了enable命令也挂死的情况，所以这么怀疑；

(2)在vrrpd与vtysh命令传输的关键点加打印信息（注意vrrpd不要-d daemon化，用& 来后台），跟踪结果是vtysh    确实把‘enable’命令发了出去，只是vrrpd进程不接收。命令通信框架公用的，基本可以确定没有问题，由      此，把bug收敛到vrrpd进程的问题；

(3)在vtysh进程中ctrl+c信号处理中加进入Linux shell的代码，当vrrpd导致系统挂死（现象是阻塞）时，可以进    入shell来top，top发现，vrrpd进程cpu利用率很高，而且sys：92.0%，所以确定了是vrrpd进程使用了某个系    统调用，而该系统调用接口阻塞，导致了vrrpd阻塞，进而导致了vrrpd进程不接收vtysh进程发多来的命令。

(4)接下来要确定哪个系统调用导致vrrpd阻塞，于是使用了strace来把vrrpd进程拉起来。

   关于strace的用法，网上一大堆，链接：[点击打开链接](http://blog.csdn.net/dianhuiren/article/details/7355974)，并附件一到文章尾部，以防原文删除。

   用的： strace -aef -p /usr/sbin/vrrpd -o /tmp/vrrpdstrace.log

   当复现vrrpd进程阻塞时，查看vrrpdstrace.log文件，分析里边的各个系统调用，确定进程阻塞在了ioctl系    统调用上，根据ioctl的参数，跟踪到内核，最终定位到博通删除mac表项的sdk接口。（该接口前后加打印）

   鉴于继续跟踪太难，bug暂告一段落........




二、跟踪博通sdk接口删除mac表项

> 
(1)过程是曲折和痛苦的，不知道的地方随便加一个打印，然后就刷屏了，但已经明确了肯定是内核处理函数中    有一个没有释放锁而导致的阻塞，接下来就是跟进锁的过程，由于该接口底层用的锁（不清楚什么锁）也不    像用户态的信号量一样有个semid的关键字，。但终于还是找到了一个区分mem的宏来作为关键字，叫L2Xm，

   刷屏总算少一点了。然后就查找内核中使用L2Xm的锁，最后反复跑脚本，确定了可能是_soc_l2x_thread线程    中也使用了该锁。

(2)最后，确定了是_soc_l2x_thread中 LOCK(L2Xm)与UNLOCK(L2Xm)之间的其他锁的阻塞，进而导致了该锁没有

   没有执行不到；进而发现了没有释放锁的地方，问题基本完结。




三、内核kmalloc的内存类型，可能导致内核panic。

> 
(1)在内核接收报文bnet_rx_deferred中报文处理中申请了内存kmalloc，但类型是GFP_KERNEL,，巧合的是，正    好该处理报文中出现了内存泄露，导致时间一长（或者故意打该报文），就会内存不足，一内存不足就会导    致kmalloc出现申请不成功的现象，即不会立即申请的到内存，，内核会在其正在申请的时候把其切换出        去，因为GFP_kernel类型是可睡眠的，非原子的。所以，问题来了，把接收报文中断处理的kmalloc切换了出    去，而接收报文中断处理是不能再切换回来的。即，中断接收报文处理被内核认为应该是原子的，当其中出    现了不是原子的kmalloc（GFP_KERNEL，）时就会panic。

(2)由此，同时发现了内存泄露和GFP_KERNEL两个问题。

(3)关于kmalloc的类型问题，网上也一大堆，链接：[点击打开链接](http://blog.csdn.net/flyingdon/article/details/5107346)，并附录二在文章末尾。 


> 
   完毕。




附件一：strace的用法。



strace命令详解
strace 命令是一种强大的工具，它能够显示所有由用户空间程序发出的系统调用。
　　strace 显示这些调用的参数并返回符号形式的值。strace 从内核接收信息，而且不需要以任何特殊的方式来构建内核。
　　下面记录几个常用 option .
　　1 -f -F选项告诉strace同时跟踪fork和vfork出来的进程
　　2 -o xxx.txt 输出到某个文件。
　　3 -e execve 只记录 execve 这类系统调用
　　---------------------------------------------------
　　进程无法启动，软件运行速度突然变慢，程序的"SegmentFault"等等都是让每个Unix系统用户头痛的问题，
　　本文通过三个实际案例演示如何使用truss、strace和ltrace这三个常用的调试工具来快速诊断软件的"疑难杂症"。


　　truss和strace用来跟踪一个进程的系统调用或信号产生的情况，而 ltrace用来跟踪进程调用库函数的情况。truss是早期为System V R4开发的调试程序，包括Aix、FreeBSD在内的大部分Unix系统都自带了这个工具；
　　而strace最初是为SunOS系统编写的，ltrace最早出现在GNU/DebianLinux中。
　　这两个工具现在也已被移植到了大部分Unix系统中，大多数Linux发行版都自带了strace和ltrace，而FreeBSD也可通过Ports安装它们。

　　你不仅可以从命令行调试一个新开始的程序，也可以把truss、strace或ltrace绑定到一个已有的PID上来调试一个正在运行的程序。三个调试工具的基本使用方法大体相同，下面仅介绍三者共有，而且是最常用的三个命令行参数：

　　-f ：除了跟踪当前进程外，还跟踪其子进程。
　　-o file ：将输出信息写到文件file中，而不是显示到标准错误输出（stderr）。
　　-p pid ：绑定到一个由pid对应的正在运行的进程。此参数常用来调试后台进程。

　　 使用上述三个参数基本上就可以完成大多数调试任务了，下面举几个命令行例子：
　　truss -o ls.truss ls -al： 跟踪ls -al的运行，将输出信息写到文件/tmp/ls.truss中。
　　strace -f -o vim.strace vim： 跟踪vim及其子进程的运行，将输出信息写到文件vim.strace。
　　ltrace -p 234： 跟踪一个pid为234的已经在运行的进程。

　　 三个调试工具的输出结果格式也很相似，以strace为例：

　　brk(0) = 0x8062aa8
　　brk(0x8063000) = 0x8063000
　　mmap2(NULL, 4096, PROT_READ, MAP_PRIVATE, 3, 0x92f) = 0x40016000

　　每一行都是一条系统调用，等号左边是系统调用的函数名及其参数，右边是该调用的返回值。 truss、strace和ltrace的工作原理大同小异，都是使用ptrace系统调用跟踪调试运行中的进程，详细原理不在本文讨论范围内，有兴趣可以参考它们的源代码。
　　举两个实例演示如何利用这三个调试工具诊断软件的"疑难杂症"：

　　案例一：运行clint出现Segment Fault错误

　　操作系统：FreeBSD-5.2.1-release
　　clint是一个C++静态源代码分析工具，通过Ports安装好之后，运行：

　　# clint foo.cpp
　　Segmentation fault (core dumped)
　　 在Unix系统中遇见"Segmentation Fault"就像在MS Windows中弹出"非法操作"对话框一样令人讨厌。OK，我们用truss给clint"把把脉"：

　　# truss -f -o clint.truss clint
　　Segmentation fault (core dumped)
　　# tail clint.truss
　　 739: read(0x6,0x806f000,0x1000) = 4096 (0x1000)
　　 739: fstat(6,0xbfbfe4d0) = 0 (0x0)
　　 739: fcntl(0x6,0x3,0x0) = 4 (0x4)
　　 739: fcntl(0x6,0x4,0x0) = 0 (0x0)
　　 739: close(6) = 0 (0x0)
　　 739: stat("/root/.clint/plugins",0xbfbfe680) ERR#2 'No such file or directory'
　　SIGNAL 11
　　SIGNAL 11
　　Process stopped because of: 16
　　process exit, rval = 139
　　我们用truss跟踪clint的系统调用执行情况，并把结果输出到文件clint.truss，然后用tail查看最后几行。
　　注意看clint执行的最后一条系统调用（倒数第五行）：stat("/root/.clint/plugins",0xbfbfe680) ERR#2 'No such file or directory'，问题就出在这里：clint找不到目录"/root/.clint/plugins"，从而引发了段错误。怎样解决？很简单： mkdir -p /root/.clint/plugins，不过这次运行clint还是会"Segmentation Fault"9。继续用truss跟踪，发现clint还需要这个目录"/root/.clint/plugins/[Python](http://lib.csdn.net/base/11)"，建好这个目录后 clint终于能够正常运行了。

　　案例二：vim启动速度明显变慢

　　操作系统：FreeBSD-5.2.1-release
　　vim版本为6.2.154，从命令行运行vim后，要等待近半分钟才能进入编辑界面，而且没有任何错误输出。仔细检查了.vimrc和所有的vim脚本都没有错误配置，在网上也找不到类似问题的解决办法，难不成要hacking source code？没有必要，用truss就能找到问题所在：

　　# truss -f -D -o vim.truss vim

　　这里-D参数的作用是：在每行输出前加上相对时间戳，即每执行一条系统调用所耗费的时间。我们只要关注哪些系统调用耗费的时间比较长就可以了，用less仔细查看输出文件vim.truss，很快就找到了疑点：

　　735: 0.000021511 socket(0x2,0x1,0x0) = 4 (0x4)
　　735: 0.000014248 setsockopt(0x4,0x6,0x1,0xbfbfe3c8,0x4) = 0 (0x0)
　　735: 0.000013688 setsockopt(0x4,0xffff,0x8,0xbfbfe2ec,0x4) = 0 (0x0)
　　735: 0.000203657 connect(0x4,{ AF_INET 10.57.18.27:6000 },16) ERR#61 'Connection refused'
　　735: 0.000017042 close(4) = 0 (0x0)
　　735: 1.009366553 nanosleep(0xbfbfe468,0xbfbfe460) = 0 (0x0)
　　735: 0.000019556 socket(0x2,0x1,0x0) = 4 (0x4)
　　735: 0.000013409 setsockopt(0x4,0x6,0x1,0xbfbfe3c8,0x4) = 0 (0x0)
　　735: 0.000013130 setsockopt(0x4,0xffff,0x8,0xbfbfe2ec,0x4) = 0 (0x0)
　　735: 0.000272102 connect(0x4,{ AF_INET 10.57.18.27:6000 },16) ERR#61 'Connection refused'
　　735: 0.000015924 close(4) = 0 (0x0)
　　735: 1.009338338 nanosleep(0xbfbfe468,0xbfbfe460) = 0 (0x0)

　　vim试图连接10.57.18.27这台主机的6000端口（第四行的connect（）），连接失败后，睡眠一秒钟继续重试（第6行的 nanosleep（））。以上片断循环出现了十几次，每次都要耗费一秒多钟的时间，这就是vim明显变慢的原因。可是，你肯定会纳闷："vim怎么会无缘无故连接其它计算机的6000端口呢？"。问得好，那么请你回想一下6000是什么服务的端口？没错，就是X Server。看来vim是要把输出定向到一个远程X Server，那么Shell中肯定定义了DISPLAY变量，查看.cshrc，果然有这么一行：setenv DISPLAY ${REMOTEHOST}:0，把它注释掉，再重新登录，问题就解决了。


　　案例三：用调试工具掌握软件的工作原理

　　操作系统：Red Hat Linux 9.0
　　用调试工具实时跟踪软件的运行情况不仅是诊断软件"疑难杂症"的有效的手段，也可帮助我们理清软件的"脉络"，即快速掌握软件的运行流程和工作原理，不失为一种学习源代码的辅助方法。下面这个案例展现了如何使用strace通过跟踪别的软件来"触发灵感"，从而解决软件开发中的难题的。
　　大家都知道，在进程内打开一个文件，都有唯一一个文件描述符（fd：file descriptor）与这个文件对应。而本人在开发一个软件过程中遇到这样一个问题：
　　已知一个fd，如何获取这个fd所对应文件的完整路径？不管是Linux、FreeBSD或是其它Unix系统都没有提供这样的API，怎么办呢？我们换个角度思考：Unix下有没有什么软件可以获取进程打开了哪些文件？如果你经验足够丰富，很容易想到lsof，使用它既可以知道进程打开了哪些文件，也可以了解一个文件被哪个进程打开。好，我们用一个小程序来试验一下lsof，看它是如何获取进程打开了哪些文件。lsof： 显示进程打开的文件。

　　/* testlsof.c */
　　#include #include #include #include #include
　　int main(void)
　　{
　　 open("/tmp/foo", O_CREAT|O_RDONLY); /* 打开文件/tmp/foo */
　　 sleep(1200); /* 睡眠1200秒，以便进行后续操作 */
　　 return 0;
　　}

　　将testlsof放入后台运行，其pid为3125。命令lsof -p 3125查看进程3125打开了哪些文件，我们用strace跟踪lsof的运行，输出结果保存在lsof.strace中：

　　# gcc testlsof.c -o testlsof
　　# ./testlsof &
　　[1] 3125
　　# strace -o lsof.strace lsof -p 3125

　　我们以"/tmp/foo"为关键字搜索输出文件lsof.strace，结果只有一条：


　　# grep '/tmp/foo' lsof.strace
　　readlink("/proc/3125/fd/3", "/tmp/foo", 4096) = 8

　　原来lsof巧妙的利用了/proc/nnnn/fd/目录（nnnn为pid）：Linux内核会为每一个进程在/proc/建立一个以其pid为名的目录用来保存进程的相关信息，而其子目录fd保存的是该进程打开的所有文件的fd。目标离我们很近了。好，我们到/proc/3125/fd/看个究竟：

　　# cd /proc/3125/fd/
　　# ls -l
　　total 0
　　lrwx------ 1 root root 64 Nov 5 09:50 0 -> /dev/pts/0
　　lrwx------ 1 root root 64 Nov 5 09:50 1 -> /dev/pts/0
　　lrwx------ 1 root root 64 Nov 5 09:50 2 -> /dev/pts/0
　　lr-x------ 1 root root 64 Nov 5 09:50 3 -> /tmp/foo
　　# readlink /proc/3125/fd/3
　　/tmp/foo

　　答案已经很明显了：/proc/nnnn/fd/目录下的每一个fd文件都是符号链接，而此链接就指向被该进程打开的一个文件。我们只要用readlink()系统调用就可以获取某个fd对应的文件了，代码如下：


　　#include #include #include #include #include #include
　　int get_pathname_from_fd(int fd, char pathname[], int n)
　　{
　　 char buf[1024];
　　 pid_t pid;
　　 bzero(buf, 1024);
　　 pid = getpid();
　　 snprintf(buf, 1024, "/proc/%i/fd/%i", pid, fd);
　　 return readlink(buf, pathname, n);
　　}
　　int main(void)
　　{
　　 int fd;
　　 char pathname[4096];
　　 bzero(pathname, 4096);
　　 fd = open("/tmp/foo", O_CREAT|O_RDONLY);
　　 get_pathname_from_fd(fd, pathname, 4096);
　　 printf("fd=%d; pathname=%sn", fd, pathname);
　　 return 0;
　　}

　　出于安全方面的考虑，在FreeBSD 5 之后系统默认已经不再自动装载proc文件系统，因此，要想使用truss或strace跟踪程序，你必须手工装载proc文件系统：mount -t procfs proc /proc；或者在/etc/fstab中加上一行：

　　proc /proc procfs rw 0 0




# 附录二：kmalloc的类型用法



malloc内存分配和malloc相似，除非被阻塞否则他执行的速度非常快，而且不对获得空间清零。

Flags参数

#include<linux/slab.h>

Void *kmalloc(size_t size, int flags);

第一个参数是要分配的块的大小，第二个参数是分配标志（flags），他提供了多种kmalloc的行为。

最常用的GFP_KERNEL,他表示内存分配（最终总是调用get_free_pages来实现实际的分配，这就是，这就是GFP前缀的由来）是代表运行在内核空间的进程执行的。使用GFP_KERNEL容许kmalloc在分配空闲内存时候如果内存不足容许把当前进程睡眠以等待。因此这时分配函数必须是可重入的。如果在进程上下文之外如：中断处理程序、tasklet以及内核定时器中这种情况下current进程不该睡眠，驱动程序该使用GFP_ATOMIC.

GFP_ATOMIC

用来从中断处理和进程上下文之外的其他代码中分配内存. 从不睡眠.

GFP_KERNEL

内核内存的正常分配. 可能睡眠.

GFP_USER

用来为用户空间页来分配内存; 它可能睡眠.

GFP_HIGHUSER

如同 GFP_USER, 但是从高端内存分配, 如果有. 高端内存在下一个子节描述.

GFP_NOIO

GFP_NOFS

这个标志功能如同 GFP_KERNEL, 但是它们增加限制到内核能做的来满足请求. 一个 GFP_NOFS 分配不允许进行任何文件系统调用, 而 GFP_NOIO 根本不允许任何 I/O 初始化. 它们主要地用在文件系统和虚拟内存代码, 那里允许一个分配睡眠, 但是递归的文件系统调用会是一个坏注意.

上面列出的这些分配标志可以是下列标志的相或来作为参数, 这些标志改变这些分配如何进行:

__GFP_DMA

这个标志要求分配在能够 DMA 的内存区. 确切的含义是平台依赖的并且在下面章节来解释.

__GFP_HIGHMEM

这个标志指示分配的内存可以位于高端内存.

__GFP_COLD

正常地, 内存分配器尽力返回"缓冲热"的页 -- 可能在处理器缓冲中找到的页. 相反, 这个标志请求一个"冷"页, 它在一段时间没被使用. 它对分配页作 DMA 读是有用的, 此时在处理器缓冲中出现是无用的. 一个完整的对如何分配 DMA 缓存的讨论看"直接内存存取"一节在第 1 章.

__GFP_NOWARN

这个很少用到的标志阻止内核来发出警告(使用 printk ), 当一个分配无法满足.

__GFP_HIGH

这个标志标识了一个高优先级请求, 它被允许来消耗甚至被内核保留给紧急状况的最后的内存页.

__GFP_REPEAT

__GFP_NOFAIL

__GFP_NORETRY

这些标志修改分配器如何动作, 当它有困难满足一个分配. __GFP_REPEAT 意思是" 更尽力些尝试" 通过重复尝试 -- 但是分配可能仍然失败. __GFP_NOFAIL 标志告诉分配器不要失败; 它尽最大努力来满足要求. 使用 __GFP_NOFAIL 是强烈不推荐的; 可能从不会有有效的理由在一个设备驱动中使用它. 最后, __GFP_NORETRY 告知分配器立即放弃如果得不到请求的内存.



2.内存区段

__GFP_DMA和__GFP_HIGHMEM的使用与平台相关，Linux把内存分成3个区段：可用于DMA的内存、常规内存、以及高端内存。X86平台上ISA设备DMA区段是内存的前16MB，而PCI设备无此限制。

内存区后面的机制在 mm/page_alloc.c 中实现, 而内存区的初始化在平台特定的文件中, 常常在 arch 目录树的 mm/init.c。

Linux 处理内存分配通过创建一套固定大小的内存对象池. 分配请求被这样来处理, 进入一个持有足够大的对象的池子并且将整个内存块递交给请求者. 驱动开发者应当记住的一件事情是, 内核只能分配某些预定义的, 固定大小的字节数组.

如果你请求一个任意数量内存, 你可能得到稍微多于你请求的, 至多是 2 倍数量. 同样, 程序员应当记住 kmalloc 能够处理的最小分配是 32 或者 64 字节, 依赖系统的体系所使用的页大小. kmalloc 能够分配的内存块的大小有一个上限. 这个限制随着体系和内核配置选项而变化. 如果你的代码是要完全可移植, 它不能指望可以分配任何大于 128 KB. 如果你需要多于几个 KB, 但是, 有个比 kmalloc 更好的方法来获得内存





在设备驱动程序或者内核模块中动态开辟内存，不是用malloc，而是kmalloc ,vmalloc，或者用get_free_pages直接申请页。释放内存用的是kfree,vfree，或free_pages. kmalloc函数返回的是虚拟地址(线性地址). kmalloc特殊之处在于它分配的内存是物理上连续的,这对于要进行DMA的设备十分重要. 而用vmalloc分配的内存只是线性地址连续,物理地址不一定连续,不能直接用于DMA.

　　注意kmalloc最大只能开辟128k-16，16个字节是被页描述符结构占用了。kmalloc用法参见khg.

　　内存映射的I/O口，寄存器或者是硬件设备的RAM(如显存)一般占用F0000000以上的地址空间。在驱动程序中不能直接访问，要通过kernel函数vremap获得重新映射以后的地址。

　　另外，很多硬件需要一块比较大的连续内存用作DMA传送。这块内存需要一直驻留在内存，不能被交换到文件中去。但是kmalloc最多只能开辟大小为32XPAGE_SIZE的内存,一般的PAGE_SIZE=4kB,也就是128kB的大小的内存。









