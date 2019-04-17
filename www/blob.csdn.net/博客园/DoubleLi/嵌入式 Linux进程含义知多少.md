# 嵌入式 Linux进程含义知多少 - DoubleLi - 博客园






理想情况下，您应该明白在您的系统中运行的每一个进程。要获得所有进程的列表，可以执行命令 `[ps](http://www.178-go.com/tags/ps) -ef`（POSIX 风格）或 `ps ax`（BSD 风格）。进程名有方括号的是内核级的进程，执行辅助功能（比如将缓存写入到磁盘）；所有其他进程都是使用者进程。您会注意到，就算是在您新安装的（最小化的）系统中，也会有很多进程在运行。熟悉它们，并把它们记录到文档中。



[kswapd0](http://www.178-go.com/tags/kswapd0),[kjournald](http://www.178-go.com/tags/kjournald),[pdflush](http://www.178-go.com/tags/pdflush)、[kblocked](http://www.178-go.com/tags/kblocked)

### 1.kswapd0


[Linux](http://www.178-go.com/tags/linux) uses kswapd for virtual memory management such that pages that have been recently accessed are kept in memory and less active pages are paged out to disk.
(what is a page?)…Linux uses manages memory in units called pages.
So,the kswapd [process](http://www.178-go.com/tags/process) regularly decreases the ages of unreferenced pages…and at the end they are paged out(moved out) to disk

系统每过一定时间就会唤醒kswapd，看看内存是否紧张，如果不紧张，则睡眠，在kswapd中，有2个阀值,pages_hige和pages_low,当空闲内存页的数量低于pages_low的时候,kswapd进程就会扫描内存并且每次释放出32个free pages,直到free page的数量到达pages_high.

### 2.kjournald

EXT3文件系统的日志进程,具有3种模式:

journal – logs all filesystem data and metadata changes. The slowest of the three ext3 journaling modes, this journaling mode minimizes the chance of losing the changes you have made to any file in an ext3 filesystem.（记录所有文件系统上的元数据改变,最慢的一种模式,）

ordered – only logs changes to filesystem metadata, but flushes file data updates to disk before making changes to associated filesystem metadata. This is the default ext3 journaling mode.(默认使用的模式,只记录文件系统改变的元数据，并在改变之前记录日志)

writeback – only logs changes to filesystem metadata but relies on the standard filesystem write process to write file data changes to disk. This is the fastest ext3 journaling mode.(最快的一种模式,同样只记录修改过的元数据,依赖标准文件系统写进程将数据写到硬盘)

修改模式EXT3的工作模式；

vim /etc/fstab

/dev/hda5      /opt            ext3       data=writeback        1 0
详细介绍:http://www.linuxplanet.com/linuxplanet/reports/4136/5/

### 3、pdflush


pdflush用于将内存中的内容和文件系统进行同步，比如说,当一个文件在内存中进行修改,pdflush负责将它写回硬盘.每当内存中的垃圾页（dirty page）超过10%的时候,pdflush就会将这些页面备份回硬盘.这个比率是可调节的,通过/etc/sysctl.conf中的 vm.dirty_background_ratio项 默认值为10 也可以

cat /proc/sys/vm/dirty_background_ratio 查看当前的值

### 4、kblockd —- 块读写子系统

### 5、[Migration](http://www.178-go.com/tags/migration) 进程迁移

什么是进程迁移？
进程迁移就是将一个进程从当前位置移动到指定的处理器上。它的基本思想是在进程执行过程中移动它，使得它在另一个计算机上继续存取它的所有资源并继续运行，而且不必知道运行进程或任何与其它相互作用的进程的知识就可以启动进程迁移操作，这意味着迁移是透明的。
进程迁移的好处
进程迁移是支持负载平衡和高容错性的一种非常有效的手段。对一系列的负载平衡策略的研究表明进程迁移是实现负载平衡的基础，进程迁移在很多方面具有适用性：

动态负载平衡：将进程迁移到负载轻或空闲的节点上，充分利用可用资源，通过减少节点间负载的差异来全面提高性能。 
容错性和高可用性：某节点出现故障时，通过将进程迁移到其它节点继续恢复运行，这将极大的提高系统的可靠性和可用性。在某些关键性应用中，这一点尤为重要。 
并行文件IO：将进程迁移到文件服务器上进行IO，而不是通过传统的从文件服务器通过网络将数据传输给进程。对于那些需向文件服务器请求大量数据的进程，这将有效的减少了通讯量，极大的提高效率。


充分利用特殊资源：进程可以通过迁移来利用某节点上独特的硬件或软件能力。


内存导引（Memory Ushering）机制：当一个节点耗尽它的主存时，Memory Ushering机制将允许进程迁移到其它拥有空闲内存的节点，而不是让该节点频繁地进行分页或和外存进行交换。这种方式适合于负载较为均衡，但内存使用存在差异或内存物理配置存在差异的系统。 
进程迁移的实现角度
进程迁移的实现复杂性及对OS的依赖性阻碍了进程迁移的广泛使用 ，尤其是对透明的进程迁移实现。根据应用的级别，进程迁移可以作为OS的一部分、用户空间、系统环境的一部分或者成为应用程序的一部分。

用户级迁移：用户级实现较为简单，软件开发和维护也较为容易，因此，现有的很多系统都是采用用户级实现，如Condor和Utopia。但由于在用户级无法获得Kernel的所有状态，因此，对于某类进程，无法进行迁移。另外，由于Kernel空间和User空间之间存在着壁垒，打破这个边界获得 Kernel提供的服务需要巨大的开销。因此，用户级实现效率远远低于内核级实现。 
应用级迁移：应用级迁移实现较为简单，可移植性好，但是需要了解应用程序语义并可能需对应用程序进行修改或重编译，透明性较差，这方面的系统有Freedman、Skordos等。 
内核级迁移：基于内核的实现可以充分利用OS提供的功能，全面的获取进程和OS状态，因此实现效率较高，能够为用户提供很好的透明性。但是由于需要对OS进行修改，实现较为复杂。这方面的典型系统有MOSIX和 Sprite系统。

进程状态
进程迁移的主要工作就在于提取进程状态，然后在目的节点根据进程状态再生该进程。在现实中，一个进程拥有很多状态，并且随着操作系统的演化，进程状态也越来越多样。一般来说，一个进程的状态可以分为以下几类：

进程执行状态（Execution State）：表示当前运行进程的处理器状态，和机器高度相关。包括内核在上下文切换时保存和恢复的信息，如通用和浮点寄存器值、栈指针、条件码等。 
进程控制（Process Control）：操作系统系统用来控制进程的所有信，一般包括进程优先级、进程标识，父进程标识等。一旦系统编排了进程控制信息，进程迁移系统必须冻结该进程的运行。 
进程Memory状态和进程地址空间：包括进程的所有虚存信息，进程数据和进程的堆栈信息等，是进程状态的最主要的一部分。 
进程的消息（Message）状态：包括进程缓冲的消息和连接（Link）的控制信息。进程迁移中通讯连接的保持以及迁移后连接的恢复是进程迁移中一项较有挑战意义的问题。 
文件状态：进程的文件状态包括文件描述符和文件缓冲快。保持文件的Cache一致性和进程间文件同步访问也是进程迁移机制需要着重考虑的。 
由于在同构的环境下（相同或兼容的机器体系结构和指令集以及操作系统）提取和恢复进程状态相对容易，现有的工作大多是以同构环境为前提的。不过，越来越多的人开始研究异构环境下的进程迁移机制，如TUI 系统。





#ps  相关进程  （k开头的基本上都是内核驱动，不建议杀掉）

PID TTY      STAT    TIME COMMAND
1    ?        Ss     0:00 init [3]                 (引导用户空间服务，管理孤儿线程，不能杀)     
2    ?        S      0:00 [migration/0]
3    ?        SN     0:00 [ksoftirqd/0]            (内核调度/管理第0个CPU软中断的守护进程，不能杀)
4    ?        S      0:00 [watchdog/0]             (系统监控应用，能够在系统出现故障时自动重新启动系统。不能杀）
5    ?        S      0:00 [migration/1]            (管理多核心(包括HypterThreading衍生的那个不大管用的、线程在各核心的迁移，不能杀）
6    ?        SN     0:00 [ksoftirqd/1]            (内核调度/管理第1个CPU软中断的守护进程，不能杀)
7    ?        S      0:00 [watchdog/1]             (系统监控应用，能够在系统出现故障时自动重新启动系统。不能杀）
8    ?        S<     0:00 [events/0]               (处理内核事件守护进程，不能杀)
9    ?        S<     0:00 [events/1]               (处理内核事件守护进程，不能杀)
10   ?        S<     0:00 [khelper]                (没查出来，感觉不能杀）

11   ?        S<     0:00 [kthread]                (父内核线程，不能杀）   
15   ?        S<     0:00  \_ [kblockd/0]          (管理磁盘块读写，不能杀)
16   ?        S<     0:00  \_ [kblockd/1]          (管理磁盘块读写，不能杀)
17   ?        S<     0:00  \_ [kacpid]             (内核电源管理，不能杀）
120  ?        S<     0:00  \_ [cqueue/0]           (队列数据结构，不能杀）
121  ?        S<     0:00  \_ [cqueue/1]           (队列数据结构，不能杀）
124  ?        S<     0:00  \_ [khubd]              (内核的usb hub，不能杀）
126  ?        S<     0:00  \_ [kseriod]             内核线程
193  ?        S      0:00  \_ [pdflush]            (pdflush内核线程池是Linux为了回写文件系统数据而创建的进程，不能杀）
194  ?        S      0:00  \_ [pdflush]            (pdflush内核线程池是Linux为了回写文件系统数据而创建的进程，不能杀）

195  ?        S<     0:00  \_ [kswapd0]            (内存回收，确保系统空闲物理内存的数量在一个合适的范围，不能杀）
196  ?        S<     0:00  \_ [aio/0]              (代替用户进程管理io，不能杀）
197  ?        S<     0:00  \_ [aio/1]              (代替用户进程管理io，不能杀）
354  ?        S<     0:00  \_ [kpsmoused]          (内核鼠标支持，可以杀掉）   
387  ?        S<     0:00  \_ [ata/0]              (ata硬盘驱动，不能杀）
388  ?        S<     0:00  \_ [ata/1]              (ata硬盘驱动，不能杀）
389  ?        S<     0:00  \_ [ata_aux]            (ata硬盘驱动，不能杀）
393  ?        S<     0:00  \_ [scsi_eh_0]          (scsi设备，不建议杀）
394  ?        S<     0:00  \_ [scsi_eh_1]          (scsi设备，不建议杀）
395  ?        S<     0:00  \_ [scsi_eh_2]          (scsi设备，不建议杀）
396  ?        S<     0:00  \_ [scsi_eh_3]          (scsi设备，不建议杀）

432  ?        S<     0:00  \_ [kauditd]            (内核审核守护进程，不能杀）
1160 ?        S<     0:00  \_ [hda_codec]
1418 ?        S<     0:00  \_ [kmirrord]           (内核守护进程控制和监视镜像模块，不能杀）
400  ?        S<     0:00  \_ [kjournald]
1442 ?        S<     0:00  \_ [kjournald]
1444 ?        S<     0:00  \_ [kjournald]
1446 ?        S<     0:00  \_ [kjournald]          (kjournald Ext3文件系统的日志管理，通常每个mount_的 Ext3分区会有一个 kjournald看管，各分区的日志
是独立的，不能杀）
466  ?        S<s    0:00 /sbin/udevd -d           (udevd 支持用户态设备操作，不能杀)
1825 ?        Ss     0:00 syslogd -m 0             (syslogd 系统日志进程，不能杀)
1828 ?        Ss     0:00 klogd -x                 (klogd 从内核信息缓冲区获取打印信息,不能杀）
1844 ?        Ss     0:00 irqbalance               (多个CPU之间均衡分配硬件中断，可以关闭，但不建议)

1864 ?        Ss     0:00 /usr/sbin/sshd           (sshd守护进程，不能杀)
1881 ?        Ss     0:00 crond                    (执行定时任务，不能杀)
1888 tty1     Ss+    0:00 /sbin/mingetty tty1      (mingetty 等待用户从tty登录，可以杀）
1892 tty2     Ss+    0:00 /sbin/mingetty tty2      (mingetty 等待用户从tty登录，可以杀)
1893 tty3     Ss+    0:00 /sbin/mingetty tty3      (mingetty 等待用户从tty登录，可以杀)





刚刚开机之后，使用ps查看系统内的内核线程，主要情况如下：   

共发现有232个内核线程存在，它们分别是：

   （1）kthreadd：这种内核线程只有一个，它的作用是管理调度其它的内核线程。它在内核初始化的时候被创建，会循环运行一个叫做kthreadd的函数，该函数的作用是运行kthread_create_list全局链表中维护的kthread。可以调用kthread_create创建一个kthread，它会被加入到kthread_create_list链表中，同时kthread_create会weak up kthreadd_task。kthreadd在执行kthread会调用老的接口——kernel_thread运行一个名叫“kthread”的内核线程去运行创建的kthread，被执行过的kthread会从kthread_create_list链表中删除，并且kthreadd会不断调用scheduler 让出CPU。这个线程不能关闭。

   （2）migration：这种内核线程共有32个，从migration/0到migration/31，每个处理器核对应一个migration内核线程，主要作用是作为相应CPU核的迁移进程，用来执行进程迁移操作，内核中的函数是migration_thread()。属于2.6内核的负载平衡系统，该进程在系统启动时自动加载（每个 cpu 一个），并将自己设为 SCHED_FIFO 的实时进程，然后检查 runqueue::migration_queue 中是否有请求等待处理，如果没有，就在 TASK_INTERRUPTIBLE 中休眠，直至被唤醒后再次检查。migration_queue仅在set_cpu_allowed() 中添加，当进程（比如通过 APM 关闭某 CPU 时）调用set_cpu_allowed()改变当前可用 cpu，从而使某进程不适于继续在当前 cpu 上运行时，就会构造一个迁移请求数据结构 migration_req_t，将其植入进程所在 cpu 就绪队列的migration_queue 中，然后唤醒该就绪队列的迁移 daemon（记录在runqueue::migration_thread 属性中），将该进程迁移到合适的cpu上去在目前的实现中，目的 cpu 的选择和负载无关，而是"any_online_cpu(req->task->cpus_allowed)"，也就是按 CPU 编号顺序的第一个 allowed 的CPU。所以，和 load_balance() 与调度器、负载平衡策略密切相关不同，migration_thread() 应该说仅仅是一个 CPU 绑定以及 CPU 电源管理等功能的一个接口。这个线程是调度系统的重要组成部分，也不能被关闭。

      （3）watchdog：这种内核线程共有32个，从watchdog/0到watchdog/31, 每个处理器核对应一个watchdog 内核线程，watchdog用于监视系统的运行，在系统出现故障时自动重新启动系统，包括一个内核 watchdog module 和一个用户空间的 watchdog 程序。在Linux 内核下, watchdog的基本工作原理是：当watchdog启动后(即/dev/watchdog设备被打开后)，如果在某一设定的时间间隔（1分钟）内/dev/watchdog没有被执行写操作, 硬件watchdog电路或软件定时器就会重新启动系统，每次写操作会导致重新设定定时器。/dev/watchdog是一个主设备号为10， 从设备号130的字符设备节点。 Linux内核不仅为各种不同类型的watchdog硬件电路提供了驱动，还提供了一个基于定时器的纯软件watchdog驱动。如果不需要这种故障处理机制，或者有相应的替代方案，可以在menuconfig的

   Device Drivers —>

      Watchdog Timer Support

处取消watchdog功能。

   （4）events：这种内核线程共有32个，从events/0到events/31, 每个处理器核对应一个 events内核线程。用来处理内核事件很多软硬件事件(比如断电，文件变更)被转换为events，并分发给对相应事件感兴趣的线程进行响应。用来处理内核事件的重要线程，不能被去掉

   （5）khelper：这种内核线程只有一个，主要作用是指定用户空间的程序路径和环境变量, 最终运行指定的user space的程序，属于关键线程，不能关闭

   （6）kblockd：这种内核线程共有32个，从kblockd/0到kblockd/31, 每个处理器核对应一个 kblockd 内核线程。用于管理系统的块设备，它会周期地激活系统内的块设备驱动。如果拥有块设备，那么这些线程就不能被去掉，要是想去掉，需要在.config中直接将CONFIG_BLOCK设成n，同时在menuconfig中取消

    Device Drivers   —>

           Block devices

   （7）kseriod：这种内核线程只有一个，主要作用是管理Serio总线上的设备的各种事件，Serio是一种虚拟总线，是Serial I/O的输写，表示串行的输入输出设备。对应内核中的serio_thread函数，流程大致是这样的：调用serio_get_event()从链表中取出struct serio_event元素，然后对这个元素的事件类型做不同的时候，处理完了之后，调用serio_remove_duplicate_events()在链表中删除相同请求的event。例如：如果要注册新的serio设备，它产生的事件类型是SERIO_REGISTER_PORT，然后流程会转入serio_add_port()。使用Serio总线的主要是标准AT键盘、PS/2鼠标、串口鼠标、Sun键盘，以及一些游戏手柄，不过由于I2C依赖于Serio，所以不关闭I2C就无法关闭Serio，menuconfig中SerialI/O的开关位于

Device Driver  —>

      Inputdevice support

       HardwareI/O ports

           SerialI/O support

   （8）pdflush：这种内核线程共有两个，线程名都是pdflush，主要作用是回写内存中的脏页，回收脏页占据的空间。由于页高速缓存的缓存作用，写操作实际上会被延迟。当页高速缓存中的数据比后台存储的数据更新时，那么该数据就被称做脏数据。在内存中累积起来的脏页最终必须被写回。在以下两种情况发生时，脏页被写回：

1.当空闲内存低于一个特定的阈值时，内核必须将脏页写回磁盘，以便释放内存。

2.当脏页在内存中驻留时间超过一个特定的阈值时，内核必须将超时的脏页写回磁盘，以确保脏页不会无限期地驻留在内存中。

对于第一个目标，pdflush线程在系统中的空闲内存低于一个特定的阈值时，将脏页刷新回磁盘。该后台回写例程的目的在于在可用物理　内存过低时，释放脏页以重新获得内存。特定的内存阈值可以通过dirty_background_ratiosysctl系统调用设置。当空闲内存比阈值：dirty_ background_ratio还低时，内核便会调用函数wakeup_bdflush()唤醒一个pdflush线程，随后pdflush线程进一步 调用函数background_writeout()开始将脏页写回磁盘。函数background_ writeout()需要一个长整型参数，该参数指定试图写回的页面数目。函数background_writeout()会连续地写出数据，直到满足以下两个条件：

1. 已经有指定的最小数目的页被写出到磁盘。

2. 空闲内存数已经回升，超过了阈值dirty_background_ratio。

上述条件确保了pdflush操作可以减轻系统中内存不足的压力。回写操作不会在达到这两个条件前停止，除非pdflush写回了所有的脏页，没有剩下的脏页可再被写回了。

对于第二个目标，pdflush后台例程会被周期性唤醒（和空闲内存是否过低无关），将那些在内存中驻留时间过长的脏页写出，确保内存中不会有长期存在的脏页。如果系统发生崩溃，由于内存处于混乱之中，所以那些在内存中还没来得及写回磁盘 的脏页就会丢失，所以周期性同步页高速缓存和磁盘非常重要。在系统启动时，内核初始化一个定时器，让它周期地唤醒pdflush线程，随后使其运行函数 wb_kupdate()。该函数将把所有驻留时间超过百分之dirty_expire_centisecs秒的脏页写回。然后定时器将再次被初始化为百 分之dirty_expire_ centisecs秒后唤醒pdflush线程。总而言之，pdflush线程周期地被唤醒并且把超过特定期限的脏页写回磁盘。

系统管理员可以在/proc/sys/vm中设置回写相关的参数，也可以通过sysctl系统调用设置它们

        属于核心的内存管理线程，这个线程也不能被关闭

（9）kswapd0：这种内核线程只有一个，主要作用是用来回收内存。在kswapd中，有2个阀值，pages_hige和pages_low。当空闲内存页的数量低于pages_low的时候，kswapd进程就会扫描内存并且每次释放出32个 free pages，直到freepage的数量到达pages_high。具体回收内存有如下原则：

      1. 如果页未经更改就将该页放入空闲队列；

      2. 如果页已经更改并且是可备份回文件系统的，就理解将内存页的内容写回磁盘；

      3. 如果页已经更改但是没有任何磁盘上的备份，就将其写入swap分区。

        同样，属于核心的内存管理线程，这个线程也不能被关闭  

（10）aio：这种内核线程共有32个，从aio/0到aio/31, 每个处理器核对应一个 aio 内核线程, 代替用户进程管理I/O，用以支持用户态的AIO（异步I/O），不应该被关闭。

 （11）unionfs_siod： 这种内核线程共有32个，但是名称都是 unionfs_siod/，每个处理器核对应一个 unionfs_siod 内核线程

   （12）nfsiod：这种内核线程只有一个，主要作用是为nfs提供高效的缓冲机制,从而改善nfs文件系统的性能，如果不需nfs，可以取消这一线程，取消这一线程的方法为menuconfig中取消

     File systems  —>

          Network File Systems

   （13）rpciod：这种内核线程共有32个，从rpciod/0到rpciod/31, 每个处理器核对应一个rpciod内核线程,主要作用是作为远过程调用服务的守护进程，用于从客户端启动I/O服务，通常启动NFS服务时要用到它，想要关闭它，需要在.config中把CONFIG_SUNRPC， CONFIG_SUNRPC_GSS， CONFIG_SUNRPC_XPRT_RDMA的值设成n

   （14）kpsmoused：这种内核线程只有一个，主要作用是支持ps/2接口的鼠标驱动。如要没有鼠标，可以取消，取消方法是menuconfig中取消

DeviceDrivers   —>

      Input device support

       Mice









