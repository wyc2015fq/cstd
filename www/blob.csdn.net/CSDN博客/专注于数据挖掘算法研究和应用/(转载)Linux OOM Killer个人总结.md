# (转载)Linux OOM Killer个人总结 - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年07月16日 12:37:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：14195








Linux下面有个特性叫OOM killer（Out Of Memory killer），这个东西会在系统内存耗尽的情况下跳出来，选择性的干掉一些进程以求释放一些内存。典型的情况是：某天机器突然登不上了能ping通，但是ssh死活连不了。原因是sshd进程被OOM killer干掉了。重启机器后查看系统日志会发现Out of Memory: Killed process ×××等。

下面介绍Linux下面的OOM killer到底是什么样一个机制呢，它在什么时候会跳出来，又会选择那些进程下手呢。

1、什么时候跳出来

先看第一个问题，它什么时候会跳出来。是不是malloc返回NULL的时候跳出来呢？不是的，malloc的manpage里有下面一段话：

By default, Linux follows an optimistic memory allocation strategy. This means that when malloc() returns non-NULL there is no guarantee that the memory really is available. This is a really bad bug. In case it turns out that
 the system is out of memory, one or more processes will be killed by the infamous OOM killer. In case Linux is employed under circumstances where it would be less desirable to suddenly lose some randomly picked processes, and moreover the kernel version is
 sufficiently recent, one can switch off this overcommitting behavior using a command like:

# echo 2 > /proc/sys/vm/overcommit_memory

上面这段话告诉我们，Linux中malloc返回非空指针，并不一定意味着指向的内存就是可用的，Linux下允许程序申请比系统可用内存更多的内存，这个特性叫Overcommit。这样做是出于优化系统考虑，因为不是所有的程序申请了内存就立刻使用的，当你使用的时候说不定系统已经回收了一些资源了。不幸的是，当你用到这个Overcommit给你的内存的时候，系统还没有资源的话，OOM killer就跳出来了。

Linux下有3种Overcommit的策略（参考内核文档：vm/overcommit-accounting），可以在/proc/sys/vm/overcommit_memory配置（取0,1和2三个值，默认是0）。

（1）0：启发式策略，比较严重的Overcommit将不能得逞，比如你突然申请了128TB的内存。而轻微的overcommit将被允许。另外，root能Overcommit的值比普通用户要稍微多些。

（2）1：永远允许overcommit，这种策略适合那些不能承受内存分配失败的应用，比如某些科学计算应用。

（3）2：永远禁止overcommit，在这个情况下，系统所能分配的内存不会超过swap+RAM*系数（/proc/sys/vm/overcmmit_ratio，默认50%，你可以调整），如果这么多资源已经用光，那么后面任何尝试申请内存的行为都会返回错误，这通常意味着此时没法运行任何新程序。

补充（待考证）：在这篇文章：Memory overcommit in Linux中，作者提到，实际上启发策略只有在启用了SMACK或者SELinux模块时才会起作用，其他情况下等于永远允许策略。

2、跳出来之后选择进程的策略

只要存在overcommit，就可能会有OOM killer跳出来。那么OOM killer跳出来之后选目标的策略又是什么呢？我们期望的是：没用的且耗内存多的程序被枪。

Linux下这个选择策略也一直在不断的演化。作为用户，我们可以通过设置一些值来影响OOM killer做出决策。Linux下每个进程都有个OOM权重，在/proc/<pid>/oom_adj里面，取值是-17到+15，取值越高，越容易被干掉。

最终OOM killer是通过/proc/<pid>/oom_score这个值来决定哪个进程被干掉的。这个值是系统综合进程的内存消耗量、CPU时间(utime + stime)、存活时间(uptime - start time)和oom_adj计算出的，消耗内存越多分越高，存活时间越长分越低。总之，总的策略是：损失最少的工作，释放最大的内存同时不伤及无辜的用了很大内存的进程，并且杀掉的进程数尽量少。

另外，Linux在计算进程的内存消耗的时候，会将子进程所耗内存的一半同时算到父进程中。这样，那些子进程比较多的进程就要小心了。当然还有其他的策略，大家可以参考文章：Taming the OOM killer和When Linux Runs Out of Memory。



在 32 位CPU 架构下寻址是有限制的。Linux 内核定义了三个区域：

-------------------------------------------------------------------------------

# DMA: 0x00000000 - 
0x00999999 (0 - 16 MB)                                    

# LowMem: 0x01000000 - 0x037999999 (16 - 896 MB) - size: 880MB  


# HighMem: 0x038000000 - <硬件特定>                                           


-------------------------------------------------------------------------------

LowMem 区 (也叫 NORMAL ZONE ) 一共 880 MB，而且不能改变(除非用 hugemem 内核)。对于高负载的系统，就可能因为 LowMem 利用不好而引发 OOM Killer 。一个可能原因是 LowFree 太少了，另外一个原因是 LowMem 里都是碎片，请求不到连续的内存区域【根据我遇到的一个案例，一个猜想是有些应用一次性请求比较大的内存，恰恰又是 880M 之内的，空闲的(LowFree)不够大，就会触发 OOM
 Killer 出来干活】。

Tip: OOM Killer 的关闭与激活方式：

# echo "0" > /proc/sys/vm/oom-kill  

# echo "1" > /proc/sys/vm/oom-kill 


检查当前 LowFree 的值：

# cat /proc/meminfo |grep LowFree  


检查LowMem内存碎片：

# cat /proc/buddyinfo               


据说使用 SysRq 的方式更好，不过 Hang 的时候再用吧。根据一些文档描述，OOM Killer 在 2.4 与 2.6 上表现是不一样的。2.4 的版本中是把新进来(新申请内存)的进程杀掉。而 2.6 上是杀掉占用内存最厉害的进程(这是很危险的，很容易导致系统应用瘫痪)。

对于 RHEL 4 ，新增了一个参数： vm.lower_zone_protection 。这个参数默认的单位为 MB，默认 0 的时候，LowMem 为 16MB。建议设置 vm.lower_zone_protection = 200 甚至更大以避免 LowMem 区域的碎片，是绝对能解决这个问题的(这参数就是解决这个问题出来的)。

对于某个用户页面的请求可以首先从“普通”区域中来满足（ZONE_NORMAL）；如果失败，就从ZONE_HIGHMEM开始尝试； 如果这也失败了，就从ZONE_DMA开始尝试。这种分配的区域列表依次包括ZONE_NORMAL、ZONE_HIGHMEM和ZONE_DMA区域。另一方面，对于 DMA 页的请求可能只能从 DMA 区域中得到满足，因此这种请求的区域列表就只包含 DMA 区域。

检查当前 low&hight 内存 的值 

# egrep 'High|Low' /proc/meminfo

或者

# free –lm                     


检查LowMem内存碎片

# cat /proc/buddyinfo           


内存物理上使用基本没有所谓的碎片问题，读写内存上任何一个字节的速度都是固定的。不存在硬盘的机械寻道慢的问题。 buddy system分配内存的时候并不能保证物理地址连续的，尤其是非2的整数次冥的页面数量的时候，需要mmu将这些物理地址不连续的内存块在线性空间上拼接起来。

Node   0就是有几个内存结点 如果不是NUMA那就是只有一个了


zone   内存区域 


DMA  内存开始的16 MB                                     

HighMem 所有大于4G的内存 ,                               

Normal 介于两者之间的内村                                


后面11个数字是以4,8,16,32,64,128,256,512,1024,2048,4096kb为单位的所有可分配内存（可以连续分配的内存地址空间）。当low 内存不足时，不管high 内存有多大，oom-killer都会开始结束一些进程保持系统运行。oom的一个原因就是虽然看着有内存，但是无法分配，所以Kill占用内存最大的程序。那都是进程级的，有问题自己解决，用内存池也行。系统都是大块分配给进程并回收，都是整页的，不管多离散，在虚存空间都能映射成连续的，所以没有碎片问题。

**如何保留低端内存:**

在 X86 高内存设备中，当用户进程使用 mlock() 在常规区域分配大量内存时，可重新使用的 lowmem 内存可能会不足，而一些系统呼叫将失败并显示“EAGAIN” 等错误。在 RHEL 5.x (X86) 中，最终用户可以使用 lowmem_reserve_ratio 控制保留的 lowmem。

详细信息

# cat /proc/sys/vm/lowmem_reserve_ratio                 


256 256 32 

DMA Normal HighMem

在常规区域中，将保留 256 页（默认）。要在常规区域中保留 512 页：

# echo "256 512 32" > /proc/sys/vm/lowmem_reserve_ratio


# cat /proc/sys/vm/lowmem_reserve_ratio                


256 512 32

要设置永久值，请编辑 /etc/sysctl.conf 并添加以下内容：

vm.lowmem_reserve_ratio = 256 512 32                   


# sysctl –p                                          


# cat /proc/sys/vm/lowmem_reserve_ratio    


256 512 32                                             




