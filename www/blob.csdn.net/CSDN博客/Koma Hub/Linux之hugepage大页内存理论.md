# Linux之hugepage大页内存理论 - Koma Hub - CSDN博客
2019年01月17日 22:12:10[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：46
**目录**
[1、Hugepage的引入](#1%E3%80%81Hugepage%E7%9A%84%E5%BC%95%E5%85%A5)
[二、hugepages相关概念](#%E4%BA%8C%E3%80%81hugepages%E7%9B%B8%E5%85%B3%E6%A6%82%E5%BF%B5)
[三、Regular Pages 与 HugePages](#%E4%B8%89%E3%80%81Regular%20Pages%20%E4%B8%8E%20HugePages)
[a、Regular Pages](#a%E3%80%81Regular%20Pages)
[b、Huge Pages](#b%E3%80%81Huge%20Pages)
[四、 hugepage 优点](#%E5%9B%9B%E3%80%81%20hugepage%20%E4%BC%98%E7%82%B9)
[a、Not swappable](#a%E3%80%81Not%20swappable)
[b、Relief of TLB pressure](#b%E3%80%81Relief%20of%20TLB%20pressure)
[c、Decreased page table overhead](#c%E3%80%81Decreased%20page%20table%20overhead)
[d、Eliminated page table lookup overhead](#d%E3%80%81Eliminated%20page%20table%20lookup%20overhead)
[e、Faster overall memory performance](#e%E3%80%81Faster%20overall%20memory%20performance)
[大页内存的优缺点](#%E5%A4%A7%E9%A1%B5%E5%86%85%E5%AD%98%E7%9A%84%E4%BC%98%E7%BC%BA%E7%82%B9)
HugePages是通过使用大页内存来取代传统的4kb内存页面，使得管理虚拟地址数变少，加快了从虚拟地址到物理地址的映射以及通过摒弃内存页面的换入换出以提高内存的整体性能。尤其是对于8GB以上的内存以及较大的[Oracle](https://www.2cto.com/database/Oracle/) SGA size，建议配值并使用HugePage特性。本文基于x86_64 [Linux](https://www.2cto.com/os/linux/)下来描述如何配值 HugePages。
# 1、Hugepage的引入
操作[系统](https://www.2cto.com/os/)对于数据的存取直接从物理内存要比从磁盘读写数据要快的多，但是物理内存是有限的，这样就引出了物理内存与虚拟内存的概念。虚拟内存就是为了满足物理内存的不足而提出的策略，它是利用磁盘空间虚拟出的一块逻辑内存，这部分磁盘空间Windows下称之为虚拟内存，Linux下被称为交换空间(Swap Space)。
对于这个大内存的管理(物理内存+虚拟内存)，大多数操作系统采用了分段或分页的方式进行管理。分段是粗粒度的管理方式，而分页则是细粒度管理方式，分页方式可以避免内存空间的浪费。相应地，也就存在内存的物理地址与虚拟地址的概念。通过前面这两种方式，CPU必须把虚拟地址转换程物理内存地址才能真正访问内存。为了提高这个转换效率，CPU会缓存最近的虚拟内存地址和物理内存地址的映射关系，并保存在一个由CPU维护的映射表中。为了尽量提高内存的访问速度，需要在映射表中保存尽量多的映射关系。
linux的内存管理采取的是分页存取机制，为了保证物理内存能得到充分的利用，内核会按照LRU算法在适当的时候将物理内存中不经常使用的内存页自动交换到虚拟内存中，而将经常使用的信息保留到物理内存。通常情况下，Linux默认情况下每页是4K，这就意味着如果物理内存很大，则映射表的条目将会非常多，会影响CPU的检索效率。因为内存大小是固定的，为了减少映射表的条目，可采取的办法只有增加页的尺寸。因此Hugepage便因此而来。也就是打破传统的小页面的内存管理方式，使用大页面2m,4m,16m等等。如此一来映射条目则明显减少。如果系统有大量的物理内存(大于8G)，则物理32位的操作系统还是64位的，都应该使用Hugepage。
# 二、hugepages相关概念
- Page Table: page table也就是一种用于内存管理的实现方式，用于物理地址到虚拟之间的映射。因此对于内存的访问，先是访问Page Table，然后根据Page Table 中的映射关系，隐式的转移到物理地址来存取数据。
- TLB: Translation Lookaside Buffer (TLB) ，CPU中的一块固定大小的cache，包含了部分page table的映射关系，用于快速实现虚拟地址到物理地址的转换。
- hugetlb: hugetlb 是TLB中指向HugePage的一个entry(通常大于4k或预定义页面大小)。 HugePage 通过hugetlb entries来实现，也可以理解为HugePage 是hugetlb page entry的一个句柄。
- hugetlbfs: 一个类似于tmpfs的新的in-memory filesystem，在2.6内核被提出。
# 三、Regular Pages 与 HugePages
## a、Regular Pages
在下图中有两个不同的进程，两个进程对于内存的访问是首先访问本地的page table，而本地的page table又参照了system-wide table的page(也就是前面描述的TLB)，最终system-wide table中的entry指向了实际的物理地址。图中物理地址page size大小4kb。也可以看到进程1和进程2在system-wide table中都指向了page2，也就是同一个物理地址。Oracle sga中共享内存的使用会出现上述情形。
![](https://img-blog.csdnimg.cn/20190117220420313.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## b、Huge Pages
在下图中，本地的page table 与system page table中都包含了huge page属性。因此page table中的任意一个page可能使用了常规的page，也有可能使用了huge page。同样进程1和进程2都共享了其中的Hpage2。图中的物理内存常规的page size是4kb，huge page size 是4mb。
![](https://img-blog.csdnimg.cn/20190117220440422.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 四、 hugepage 优点
对于较大的系统内存以及sga，使用hugepage可以极大程度的提高Oracle[数据库](https://www.2cto.com/database/)性能。
## a、Not swappable
无需交换。也就是不存在页面由于内存空间不足而存在换入换出的问题
## b、Relief of TLB pressure
减轻TLB的压力，也就是降低了cpu cache可缓存的地址映射压力。由于使用了huge page，相同的内存大小情况下，管理的虚拟地址数量变少。
TLB entry可以包含更多的地址空间，cpu的寻址能力相应的得到了增强。
## c、Decreased page table overhead
降低page table负载，对于普通的page，每个entry需要64bytes进行管理，对于50gb的内存，管理这些entry，需要800mb的大小
(50*1024*1024)kb/4kb*64bytes/1024/1024=800mb。
## d、Eliminated page table lookup overhead
消除page table查找负载
## e、Faster overall memory performance
提高内存的整体性能
# 大页内存的优缺点
有点很明显，大页内存TLB miss 很少，缺页中断也很少，极高的提高了性能，对于那些内存操作非常频繁的业务来说，可以有效的提高性能。
缺点就是，大页内存类似专用内存，会从系统中抠出一块大内存（pagesize*nr_pages），而想要使用这块内存，应用程序必须修改程序，使用上述几种方式来使用大页。 其次，如果程序内存使用较少，却申请了大页，就造成了内存浪费的。
Redhat有另一种便捷的方式，就是 透明大页，就是开启后，任何程序都默认使用2MB的大页，不需要修改程序源码，但是也带来了内存浪费。
原文：[https://www.2cto.com/database/201709/677328.html](https://www.2cto.com/database/201709/677328.html)
原文：[https://blog.csdn.net/mrpre/article/details/83586778](https://blog.csdn.net/mrpre/article/details/83586778)
