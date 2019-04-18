# (转载)Linux Out-of-Memory(OOM) Killer - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年07月16日 12:36:11[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2874








Linux有一个特性：OOM Killer，一个保护机制，用于避免在内存不足的时候不至于出现严重问题，把一些无关的进程优先杀掉，即在内存严重不足时，系统为了继续运转，内核会挑选一个进程，将其杀掉，以释放内存，缓解内存不足情况，不过这种保护是有限的，不能完全的保护进程的运行。

    在很多情况下，经常会看到还有剩余内存时，oom-killer依旧把进程杀死了，现象是在/var/log/messages日志文件中有如下信息：

    Out of Memory: Killed process [PID] [process name].

    该问题是low memory耗尽，因为内核使用low memory来跟踪所有的内存分配。

    当low memory耗尽，不管high memory剩多少，oom-killer都会杀死进程，以保持系统的正常运行。

    在32位CPU下寻址范围是有限的，Linux内核定义了下面三个区域：

 # DMA: 0x00000000 - 0x00999999 (0 - 16 MB) 
# LowMem: 0x01000000 - 0x037999999 (16 - 896 MB) - size: 880MB
# HighMem: 0x038000000 - <硬件特定>
    LowMem区（也叫NORMAL ZONE）共880MB，并且是固定不能变的（除非使用hugemem内核），对于高负荷的系统，可能因为LowMem使用不好而触发了OOM Killer机制。因为内存分配是一个连续的区域，在此时，如果LowMem里存在很多碎片或者LowFree太少，此时无法分配到一块连续的内存区域，就触发了OOM Killer。

    查看当前LowFree值：

    cat /proc/meminfo | grep LowFree

    查看LowMem内存碎片：

    cat /proc/buddyinfo

    上面这命令需要在2.6内核才有效。


    有如下方法可以解决该问题：

    1、升级到64位系统，这是最好的方法，因为此时所有的内存都属low memory，如此时提示out of memory，则真的是low memory耗尽，真的OOM了。

    2、如必须使用32位系统，那么可以使用hugemem内核，此时内核会以不同的方式分割low/high memory，而大多数情况下会提供足够多的low memory至high memory的映射，此时很简单的一个修复方法是可以安装hugemem内核包，然后重启。

    3、如果hugemem内核也用不了，那么我们可以尝试将/proc/sys/vm/lower_zone_protection的值设为250或更大，可使用如下命令查看和设置该值：

       cat /proc/sys/vm/lower_zone_protection

       echo 250 > /proc/sys/vm/lower_zone_protection

       或者可以修改/etc/sysctl.conf文件，以便重启后生效，添加的内容如下：

       vm.lower_zone_protection = 250

    4、实在没办法，那么我们把oom-killer关掉，不过该选项可能导致系统挂起，故要看实际情况使用。

       查看当前的oom-killer的状态：cat /proc/sys/vm/oom-kill

       关闭/打开oom-killer：

       echo "0" > /proc/sys/vm/oom-kill

       echo "1" > /proc/sys/vm/oom-kill

       或者直接加到/etc/sysctl.conf文件，内容如下：

       vm.oom-kill = 0

       此时，当进程被oom-killer尝试杀死而没有成功时，会把相关信息记录到/var/log/messages文件中，信息如下：

       "Would have oom-killed but /proc/sys/vm/oom-kill is disabled"

    5、或者不把oom-killer关掉，只针对单一进程处理，把某个进程保护起来，此时，我们可以使用如下命令：

       echo -17 > /proc/[pid]/oom_adj

       /proc/[pid]/oom_adj中oom_adj的取值范围是-17~15，当该值为-17时，系统将不会杀死指定pid的进程，而-16~15则会使得进程的/proc/[pid]/oom_adj值呈指数（K*2^n）形式递增，即它们被杀掉的可能性呈指数递增。针对init（进程号为1）这个进程，无论该值设为多少都不会被杀。

    6、参考资料

[http://www.haw-haw.org/node/626](http://www.haw-haw.org/node/626)

[http://guogoul.com/2010/04/28/oom-killer-swap/](http://guogoul.com/2010/04/28/oom-killer-swap/)

[http://zhwen.org/xlog/?p=430](http://zhwen.org/xlog/?p=430)

[http://www.dbanotes.net/database/linux_outofmemory_oom_killer.html](http://www.dbanotes.net/database/linux_outofmemory_oom_killer.html)

[http://www.sealinger.com/archives/185](http://www.sealinger.com/archives/185)



以上是从网络上查到，结合自己的问题进行下补充：

一开始由于系统配置是2G，而且没有交换分区，所以每天导致out of memory，后来增加了物理内存，并做了交换分区，情况有所改善，但是运行2-3天后还是会出现out of memory的情况，后来分析日志文件messages发现粗体部分，分析是low memory不足导致，后来自己试验用sync ；echo 3 >> /proc/sys/vm/drop_caches可以将内存释放出来，查看/proc/buddyinfo内存释放出来。


Jun 10 13:33:11 xx user.warn kernel: DMA free:3548kB min:68kB low:84kB high:100kB active:0kB inactive:0kB present:16384kB pages_scanned:0 all_unreclaimable? yes

Jun 10 13:33:11 xx user.warn kernel: lowmem_reserve[]: 0 0 880 4080

Jun 10 13:33:11 xx user.warn kernel: DMA32 free:0kB min:0kB low:0kB high:0kB active:0kB inactive:0kB present:0kB pages_scanned:0 all_unreclaimable? no

Jun 10 13:33:11 xx user.warn kernel: lowmem_reserve[]: 0 0 880 4080

**Jun 10 13:33:11 xx user.warn kernel: Normal free:1340kB min:3756kB low:4692kB high:5632kB active:0kB inactive:28kB present:901120kB pages_scanned:2456536 all_unreclaimable? yes**

Jun 10 13:33:11 xx user.warn kernel: lowmem_reserve[]: 0 0 0 25600

Jun 10 13:33:11 xx user.warn kernel: HighMem free:1615600kB min:512kB low:3928kB high:7344kB active:274112kB inactive:24928kB present:3276800kB pages_scanned:0 all_unreclaimable? no

Jun 10 13:33:11 xx user.warn kernel: lowmem_reserve[]: 0 0 0 0

Jun 10 13:33:11 xx user.warn kernel: DMA: 1*4kB 1*8kB 1*16kB 0*32kB 1*64kB 1*128kB 1*256kB 0*512kB 1*1024kB 1*2048kB 0*4096kB = 3548kB

Jun 10 13:33:11 xx user.warn kernel: DMA32: empty

J**un 10 13:33:11 xx user.warn kernel: Normal: 1*4kB 1*8kB 1*16kB 1*32kB 0*64kB 0*128kB 1*256kB 0*512kB 1*1024kB 0*2048kB 0*4096kB = 1340kB**

Jun 10 13:33:11 CHINASOFT user.warn kernel: HighMem: 8192*4kB 7506*8kB 5166*16kB 2932*32kB 1528*64kB 688*128kB 253*256kB 86*512kB 67*1024kB 12*2048kB 234*4096kB = 1615600kB




以上是自己遇到的问题，处理肯定有不对的地方，如果您发现请指正！补充:内核是2.26.16






