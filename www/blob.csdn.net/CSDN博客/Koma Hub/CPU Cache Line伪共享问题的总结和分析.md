# CPU Cache Line伪共享问题的总结和分析 - Koma Hub - CSDN博客
2019年04月13日 18:27:54[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：30
个人分类：[x86																[架构																[基础知识																[体系结构](https://blog.csdn.net/Rong_Toa/article/category/7496482)](https://blog.csdn.net/Rong_Toa/article/category/7587699)](https://blog.csdn.net/Rong_Toa/article/category/8742415)](https://blog.csdn.net/Rong_Toa/article/category/8770779)
> 
原文：吴一昊，杨勇 Linux阅码场 
地址：[CPU Cache Line伪共享问题的总结和分析](http://mp.weixin.qq.com/s?__biz=MzAwMDUwNDgxOA==&mid=2652665743&idx=1&sn=18d69bc3dba3e0940bb020c838edfd1b&chksm=810f3112b678b804b9feabcb7908e975fd29d4916ec08c6f33f9d3e2ae0cc96344103db583b5&mpshare=1&scene=1&srcid=#rd)
**Table of Contents**
[1. 关于本文](#1.%20%E5%85%B3%E4%BA%8E%E6%9C%AC%E6%96%87)
[2. 背景知识](#2.%20%E8%83%8C%E6%99%AF%E7%9F%A5%E8%AF%86)
[2.1 存储器层次结构](#2.1%20%E5%AD%98%E5%82%A8%E5%99%A8%E5%B1%82%E6%AC%A1%E7%BB%93%E6%9E%84)
[2.2 多核架构](#2.2%20%E5%A4%9A%E6%A0%B8%E6%9E%B6%E6%9E%84)
[2.3 NUMA 架构](#2.3%20NUMA%20%E6%9E%B6%E6%9E%84)
[Cache](#Cache)
[DRAM](#DRAM)
[Device](#Device)
[2.4 Cache Line](#2.4%20Cache%20Line)
[2.5 Cache 的结构](#2.5%20Cache%20%E7%9A%84%E7%BB%93%E6%9E%84)
[2.6 Cache 一致性](#2.6%20Cache%20%E4%B8%80%E8%87%B4%E6%80%A7)
[2.7 Cache Line 伪共享](#2.7%20Cache%20Line%20%E4%BC%AA%E5%85%B1%E4%BA%AB)
# 1. 关于本文
本文基于 Joe Mario 的一篇博客 改编而成。 
Joe Mario 是 Redhat 公司的 Senior Principal Software Engineer，在系统的性能优化领域颇有建树，他也是本文描述的 `perf c2c` 工具的贡献者之一。 
这篇博客行文比较口语化，且假设读者对 CPU 多核架构，Cache Memory 层次结构，以及 Cache 的一致性协议有所了解。 
故此，笔者决定放弃照翻原文，并且基于原博客文章做了一些扩展，增加了相关背景知识简介。 
本文中若有任何疏漏错误，责任在于编译者。有任何建议和意见，请回复`内核月谈`微信公众号，或通过 oliver.yang at linux.alibaba.com 反馈。
# 2. 背景知识
要搞清楚 Cache Line 伪共享的概念及其性能影响，需要对现代理器架构和硬件实现有一个基本的了解。 
如果读者已经对这些概念已经有所了解，可以跳过本小节，直接了解 perf c2c 发现 Cache Line 伪共享的方法。 
(注：本节中的所有图片，均来自与 Google 图片搜索，版权归原作者所有。）
## 2.1 存储器层次结构
众所周知，现代计算机体系结构，通过存储器层次结构 (Memory Hierarchy) 的设计，使系统在性能，成本和制造工艺之间作出取舍，从而达到一个平衡。 
下图给出了不同层次的硬件访问延迟，可以看到，各个层次硬件访问延迟存在数量级上的差异，越高的性能，往往意味着更高的成本和更小的容量：
![](https://img-blog.csdnimg.cn/20190413181852639.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
通过上图，可以对各级存储器 Cache Miss 带来的性能惩罚有个大致的概念。
## 2.2 多核架构
随着多核架构的普及，对称多处理器 (SMP) 系统成为主流。例如，一个物理 CPU 可以存在多个物理 Core，而每个 Core 又可以存在多个硬件线程。 
x86 以下图为例，1 个 x86 CPU 有 4 个物理 Core，每个 Core 有两个 HT (Hyper Thread)，
![](https://img-blog.csdnimg.cn/20190413181926229.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
从硬件的角度，上图的 L1 和 L2 Cache 都被两个 HT 共享，且在同一个物理 Core。而 L3 Cache 则在物理 CPU 里，被多个 Core 来共享。 
而从 OS 内核角度，每个 HT 都是一个逻辑 CPU，因此，这个处理器在 OS 来看，就是一个 8 个 CPU 的 SMP 系统。
## 2.3 NUMA 架构
一个 SMP 系统，按照其 CPU 和内存的互连方式，可以分为 UMA (均匀内存访问) 和 NUMA (非均匀内存访问) 两种架构。 
其中，在多个物理 CPU 之间保证 Cache 一致性的 NUMA 架构，又被称做 ccNUMA (Cache Coherent NUMA) 架构。
> 
PS：NUMA*（Non Uniform Memory Access Architecture）*技术可以使众多服务器像单一系统那样运转，同时保留小系统便于编程和管理的优点。基于电子商务应用对内存访问提出的更高的要求，NUMA也向复杂的结构设计提出了挑战。**非统一内存访问（NUMA）**是一种用于多处理器的[电脑记忆体](https://baike.baidu.com/item/%E7%94%B5%E8%84%91%E8%AE%B0%E5%BF%86%E4%BD%93)设计，内存访问时间取决于处理器的内存位置。 在NUMA下，处理器访问它自己的本地存储器的速度比非本地存储器（存储器的地方到另一个处理器之间共享的处理器或存储器）快一些。
以 x86 为例，早期的 x86 就是典型的 UMA 架构。例如下图，四路处理器通过** FSB (前端系统总线) **和主板上的内存控制器芯片 (MCH) 相连，DRAM 是以 UMA 方式组织的，延迟并无访问差异，
![](https://img-blog.csdnimg.cn/20190413181955989.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
然而，这种架构带来了严重的**内存总线的性能瓶颈**，影响了 x86 在多路服务器上的可扩展性和性能。
因此，从 Nehalem 架构开始，x86 开始转向 NUMA 架构，内存控制器芯片被集成到处理器内部，多个处理器通过 **QPI 链路**相连，从此 DRAM 有了远近之分。 
而 Sandybridge 架构则更近一步，将片外的 IOH 芯片也集成到了处理器内部，至此，内存控制器和 PCIe Root Complex 全部在处理器内部了。 
下图就是一个典型的 x86 的 NUMA 架构：
![](https://img-blog.csdnimg.cn/20190413182022656.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
由于 NUMA 架构的引入，以下主要部件产生了因物理链路的远近带来的延迟差异：
- 
### Cache
除物理 CPU 有本地的 Cache 的层级结构以外，还存在跨越系统总线 (QPI) 的远程 Cache 命中访问的情况。需要注意的是，远程的 Cache 命中，对发起 Cache 访问的 CPU 来说，还是被记入了 LLC Cache Miss。
- 
### DRAM
在两路及以上的服务器，远程 DRAM 的访问延迟，远远高于本地 DRAM 的访问延迟，有些系统可以达到 2 倍的差异。 
	需要注意的是，即使服务器 BIOS 里关闭了 NUMA 特性，也只是对 OS 内核屏蔽了这个特性，这种延迟差异还是存在的。
- 
### Device
对 CPU 访问设备内存，及设备发起 DMA 内存的读写活动而言，存在本地 Device 和远程 Device 的差别，有显著的延迟访问差异。
因此，对以上 NUMA 系统，一个 NUMA 节点通常可以被认为是一个物理 CPU 加上它本地的 DRAM 和 Device 组成。那么，四路服务器就拥有四个 NUMA 节点。 
如果 BIOS 打开了 NUMA 支持，Linux 内核则会根据 ACPI 提供的表格，针对 NUMA 节点做一系列的 NUMA 亲和性的优化。
在 Linux 上，`numactl --hardware` 可以返回当前系统的 NUMA 节点信息，特别是 CPU 和 NUMA 节点的对应信息。
## 2.4 Cache Line
Cache Line 是 CPU 和主存之间数据传输的最小单位。当一行 Cache Line 被从内存拷贝到 Cache 里，Cache 里会为这个 Cache Line 创建一个条目。 
这个 Cache 条目里既包含了拷贝的内存数据，即 Cache Line，又包含了这行数据在内存里的位置等元数据信息。
由于 Cache 容量远远小于主存，因此，存在多个主存地址可以被映射到同一个 Cache 条目的情况，下图是一个 Cache 和主存映射的概念图：
![](https://img-blog.csdnimg.cn/20190413182107970.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
而这种 Cache 到主存的映射，通常是由内存的虚拟或者物理地址的某几位决定的，取决于 Cache 硬件设计是虚拟地址索引，还是物理地址索引。 
然而，由于索引位一般设计为低地址位，通常在物理页的页内偏移以内，因此，不论是内存虚拟或者物理地址，都可以拿来判断两个内存地址，是否在同一个 Cache Line 里。
Cache Line 的大小和处理器硬件架构有关。在 Linux 上，通过 `getconf` 就可以拿到 CPU 的 Cache Line 的大小，
```
$ getconf.exe -a | grep CACHE
LEVEL1_ICACHE_SIZE                  32768
LEVEL1_ICACHE_ASSOC                 8
LEVEL1_ICACHE_LINESIZE              64
LEVEL1_DCACHE_SIZE                  32768
LEVEL1_DCACHE_ASSOC                 8
LEVEL1_DCACHE_LINESIZE              64
LEVEL2_CACHE_SIZE                   262144
LEVEL2_CACHE_ASSOC                  4
LEVEL2_CACHE_LINESIZE               64
LEVEL3_CACHE_SIZE                   12582912
LEVEL3_CACHE_ASSOC                  16
LEVEL3_CACHE_LINESIZE               64
LEVEL4_CACHE_SIZE                   0
LEVEL4_CACHE_ASSOC                  0
LEVEL4_CACHE_LINESIZE               0
```
## 2.5 Cache 的结构
前面 Linux `getconf` 命令的输出，除了 `*_LINESIZE` 指示了系统的 Cache Line 的大小是 64 字节外，还给出了 Cache 类别，大小。 
其中 `*_ASSOC` 则指示了该 Cache 是几路关联 (Way Associative) 的。
下图很好的说明了 Cache 在 CPU 里的真正的组织结构，
![](https://img-blog.csdnimg.cn/20190413182203685.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
一个主存的物理或者虚拟地址，可以被分成三部分：高地址位当作 Cache 的 Tag，用来比较选中多路 (Way) Cache 中的某一路 (Way)，而低地址位可以做 Index，用来选中某一个 Cache Set。 
在某些架构上，最低的地址位，Block Offset 可以选中在某个 Cache Line 中的某一部份。
因此，Cache Line 的命中，完全依靠地址里的 Tag 和 Index 就可以做到。关于 Cache 结构里的 Way，Set，Tag 的概念，请参考相关文档或者资料。这里就不再赘述。
## 2.6 Cache 一致性
如前所述，在 SMP 系统里，每个 CPU 都有自己本地的 Cache。因此，同一个变量，或者同一行 Cache Line，有在多个处理器的本地 Cache 里存在多份拷贝的可能性，因此就存在数据一致性问题。 
通常，处理器都实现了 Cache 一致性 (Cache Coherence）协议。如历史上 x86 曾实现了 MESI 协议以及 MESIF 协议。
假设两个处理器 A 和 B, 都在各自本地 Cache Line 里有同一个变量的拷贝时，此时该 Cache Line 处于 `Shared` 状态。当处理器 A 在本地修改了变量，除去把本地变量所属的 Cache Line 置为 `Modified` 状态以外， 
还必须在另一个处理器 B 读同一个变量前，对该变量所在的 B 处理器本地 Cache Line 发起 Invaidate 操作，标记 B 处理器的那条 Cache Line 为 `Invalidate` 状态。 
随后，若处理器 B 在对变量做读写操作时，如果遇到这个标记为 `Invalidate` 的状态的 Cache Line，即会引发 Cache Miss， 
从而将内存中最新的数据拷贝到 Cache Line 里，然后处理器 B 再对此 Cache Line 对变量做读写操作。
本文中的 Cache Line 伪共享场景，就基于上述场景来讲解，关于 Cache 一致性协议更多的细节，请参考相关文档。
## 2.7 Cache Line 伪共享
Cache Line 伪共享问题，就是由多个 CPU 上的多个线程同时修改自己的变量引发的。这些变量表面上是不同的变量，但是实际上却存储在同一条 Cache Line 里。 
在这种情况下，由于 Cache 一致性协议，两个处理器都存储有相同的 Cache Line 拷贝的前提下，本地 CPU 变量的修改会导致本地 Cache Line 变成 `Modified` 状态，然后在其它共享此 Cache Line 的 CPU 上， 
引发 Cache Line 的 Invaidate 操作，导致 Cache Line 变为 `Invalidate` 状态，从而使 Cache Line 再次被访问时，发生本地 Cache Miss，从而伤害到应用的性能。 
在此场景下，多个线程在不同的 CPU 上高频反复访问这种 Cache Line 伪共享的变量，则会因 Cache 颠簸引发严重的性能问题。
下图即为两个线程间的 Cache Line 伪共享问题的示意图，
![](https://img-blog.csdnimg.cn/20190413182238609.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
