# 从ARM VIVT看linux的cache 处理 - h13 - 博客园
从 ARM VIVT 看 cache
请访问google 文档. 
[http://docs.google.com/Doc?id=dcbsxfpf_282csrs2pfn](http://pk.linuxpk.com/link.php?url=http://docs.google.com%2FDoc%3Fid%3Ddcbsxfpf_282csrs2pfn)
do_wp_page 的VIVT 考虑
在下面的函数中(write protect detected), 为什么需要**flush_cache_page,和**** update_mmu_cache**? 原因在于在vivt 的情况下, 如果a 进程写入cache line 1,  b进程试图读取自己的 cache line 2,   并且b进程是cow(一种情况), 那么在copy这个页面的时候就需要flush 用户a存在于cache line1 内的数据, 以保证b进程获取正确数据.  
实际上**flush_cache_page**(); 除了刷新用户a的页面外,还考虑到**vipt_aliasing.  我们后面再讨论**.
关于这个问题的一个讨论:
[http://search.luky.org/linux-kernel.2000/msg03711.html](http://search.luky.org/linux-kernel.2000/msg03711.html)
[http://lists.arm.linux.org.uk/lurker/message/20040705.161647.21042a82.html](http://lists.arm.linux.org.uk/lurker/message/20040705.161647.21042a82.html)
static int do_wp_page(struct mm_struct *mm, struct vm_area_struct *vma,
        unsigned long address, pte_t *page_table, pmd_t *pmd,
        spinlock_t *ptl, pte_t orig_pte)
{
    ....
    old_page = vm_normal_page(vma, address, orig_pte);
    if (!old_page) {
          if ((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
                     (VM_WRITE|VM_SHARED))
            goto reuse;
        goto gotten;
    }
    /*
     * Take out anonymous pages first, anonymous shared vmas are
     * not dirty accountable.
     */
    if (PageAnon(old_page)) {
        if (trylock_page(old_page)) {
reuse = can_share_swap_page(old_page);
            unlock_page(old_page);
        }
    } else if (unlikely((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
                    (VM_WRITE|VM_SHARED))) {
        ......
        dirty_page = old_page;
        get_page(dirty_page);
reuse = 1;
    }
    if (reuse) {
reuse:
**flush_cache_page**(vma, address, pte_pfn(orig_pte));
        entry = pte_mkyoung(orig_pte);
        entry = maybe_mkwrite(pte_mkdirty(entry), vma);
        if (ptep_set_access_flags(vma, address, page_table, entry,1))
**update_mmu_cache**(vma, address, entry);
        ret |= VM_FAULT_WRITE;
        goto unlock;
    }
Cache 的基本知识
首先推荐 wikipedia, 本文有半数来自这篇文章:
[http://en.wikipedia.org/wiki/CPU_cache#Associativity](http://en.wikipedia.org/wiki/CPU_cache#Associativity)
Arm4, Arm5: VIVT
Arm6, Arm7:  I-cache VIPT, Dcache VIPT or  PIPT.
用两个例子和图形分析下啥是index，啥是tag：
K8 4k 2-way cache, cache line 64B
![](http://docs.google.com/File?id=dcbsxfpf_284dvtqvqgc_b)
0.  共4k， cache line 长度是64B, 共64个cache line
1. byte select :  64B, 2^6, 即 [0-5] bit 选择一个cache line内的byte
2. 64 个cache line, 需要2^6个index去索引,6-11bit, 但是2way的含义在于一个index选择2个cache line(数据可以存储于这2个cache line的任一个), 即只要2^6/2,  5bit 作index 就够了, [10-6] bit 就是k8 cache 的index
3.剩下的[31:11] bit 用于确定这2个cache line是否包含了要存取的数据
-----------------------------------
4. 上图是2way cache 实现原理的一种示意图.  左边的tag ram存储的是cache 的tag 即地址的[31:11]bit, 当再cache中查找的时候,用index [10:6]bit 同时和tag ram中的way0, way0 相比较, 如果有一个相同就代表命中了.  我们就知道命中的是那个way的cache.
5. 左边存储cache的具体数据, 却并不是以cache line的形式来存储的,而是每个way的data SRAM 各存储了一个word, 总共有512个entry, 所以这些数据的index 是[10:2]bit.
6. 结合第4步骤选出来的way number, 就知道到底命中了那个way的数据
这种实现, cache line 的index (way 选择), 和date的'index ' 分别具有不同的bit数目. n-way的cache意味着命中cache的时候需要同时和n个 cache index 进行比较. (肯定是同时,如果比两次还不如用1:1的cache呢).
如果n比较大, 用类似CAM的实现方式显然比较靠普, 就下面要说的ARM920T的I-cache.
ARM920T (SAM SUNG 2410a) I-cache :16k 64-way  cache,  32B cache line 
![](http://pk.linuxpk.com/image/zoom.gif)![](http://docs.google.com/File?id=dcbsxfpf_283dzdnkjff_b)
0.  共16k， cache line 长度是32B, 共512个cache line
1. byte select :  64B, 2^6, 即 [0-4] bit 选择一个cache line内的byte
2. 512 个cache line, 需要2^9个index去索引, 64 way就是一个index选择64个cacheline, 即只要2^9/64,  3bit 作index 就够了, 据就是[7-5] bit是ARM920T I-cache的index (这里把cache index 叫做seg)
3.剩下的[31:8] bit 用于确定某个way内的64个cache line是否包含了要存取的数据
-----------------------------------
4. 上图是用CAM 实现的64 way cache的一种示意图.  0-7 这7个平面是7个CAM内存组. CAM中存的是cache TAG即地址的[31:8]bit,当再cache中查找的时候,用cache index [7-5]bit 同时和CAM 中的tag相比较, 如果命中就直接获取到了一个完整的cache line.
5. 0-4 bit 可以选中这个cache line 中的byte, 如果是word 操作, [4:2]bit就是word index
n-way的cache意味着命中cache的时候需要同时和n个 cache index 进行比较. 这次是用CAM.
VIVT VIPT
VIVT:　意思是cache的index和tag都是虚拟地址. 这个好处就是读取cache的时候无须TLB的介入, 速度比较快. 
VIPT:   这个就是说, cache 和TLB 可以同时工作, 比不上VIVT 的cache 速度, 但是比PIPT 要好. 
Cache Alias  
Virtual index cache 的速度是快了, 但是会带来其他的问题: 一个物理地址的内容可以出现在多个cache line中, 这就需要更多的cache flush操作. 反而影响了速度. 这就是cache alias. 
随着技术进步, TLB查找速度提高了, 在cache 用index查找cache的过程中TLB可以完成虚拟地址到物理地址的转换工作, 在cache比较tag的时候物理地址已经可以使用了, 就是说采用physical tag可以和cache并行工作, virtual tag已经不怎么使用了, 特别是在容量比较大的cache中(比如l2 cache).  只有小,并且延迟很小的cache 还或许采用virtual tag.  
alias就是同一个物理地址被映射到两个或多个相同或者不同的虚拟地址的时候,cache 中存在多于一个的cache line 包含这个物理地址的数据, 具体来讲, alias 可以通过如下几种方式产生:
(强烈推荐 [http://mail-index.netbsd.org/port-sh3/2006/09/07/0000.html](http://mail-index.netbsd.org/port-sh3/2006/09/07/0000.html)  )
 一个PA 被映射到不同的虚拟地址, 如linux下, 内核和用户访问页面的虚拟地址可能不同. 这些虚拟地址有个特点,因为映射到同pa , 其低几位bit 必然相同. 
PA--> VA1  |31                                           12 |   PA's low 12 bit  |   
      VA2  |VA2  ie, 31-11                               12 |   PA's low 12 bit  |   
这种情况下是否有alias, 取决于index采用哪几个bit: 
cache index:                                        |n                  m| word|byte|
1. 如果bit [n..m]中包含了Vitual index 的几个比特,那么因为VA1/VA2不同, 就会有两个cache line 可以包含这个地址的数据, 这一点对于VIVT,和VIPT 都相同.  
2. 如果 bit [n..m] 完全落在了PA的 低几个bit, 对于direct map的cache, 不会有alias出现, 但是对于n-way的情况, 就要看是PT还是VT了. 如果是VIVT, 因为 VT 不同,那么alias不可避免. 如果VIPT, 则因为Pysical的高几位地址也相同, alias就不会出现了. 
-----------以下内容有点疑问
许多cpu可以通过cache miss 的时候检查所可能的cache line, 来防止cache alias的出现. cache miss的时候, 搜索所有可能位置, 通过对比物理地址是否相同(*不确信)来消除alias.   这种消除可以简化, 就是上面的[n...m]完全落在page index之外,比如上面的PA's low 12 bit时, 这种情况只有一个可能的位置来存储一个PA的VA映射, 对于PT, 在这种情况下,进行hit处理的时候已经进行了这种alias的检查, miss的时候就没有问题了.
page color
配备有特别大容量的physical index的cache(一般是2级缓存)会引起一个问题: 应用程序无法控制自己的物理内存在cache中的位置,即无法控制cache 冲突,因为一般是os决定一个虚拟页面映射到那个物理页面. 
比如有一个1M的的Physical Index, direct map的二级缓存,如果是4k页面,那cache里就可以同时容纳256个物理页面, 为了区分这些页面, 给他们编上号码, 0-255, 称为page的color, 不同color的page在这种二级cache里不会有冲突. 
一个比较直接的办法是在给虚拟页面映射物理页面的时候保证他们的color相同,这样应用程序可以按照自己虚拟地址的color来安排cache的访问模式, 这样可以避免confilict miss. 应用程序可以安排在一段时间内只访问1m的数据来避免capacity miss. 
术语:
snag : 给定的时间内,拥有不同虚拟color的页面可能拥有相同的物理color,从而引起冲突. 
[birthday paradox](http://en.wikipedia.org/wiki/Birthday_paradox) : 如果OS总是随机的给虚拟页面映射物理页面,那么很可能许多页面拥有相同的物理color,从而引起冲突. (birthday paradox: 随机选取的一些 人中,可能会有几对相同生日的人.)
给不同的虚拟color映射到不同的物理color的技术就是page color. 比较简单的实现是让他们的虚拟color和物理color相同.
如果OS保证一个物理页面只会映射到一个color的虚拟页面, cpu就可以使用Virtual index而无需考虑aliasing问题.早期的SPARC 和 RS/6000 就采用了这种设计. 
virtual hints
CPU 如Pentium 4 (Willamette and Northwood cores), 采用了vhint 来代替vtag来做way select, 他是4way的,只用2bit(从vitual tag hash 或者其他方式获得), 这样就不用采用TCAM了, 并在way 内存储physical tag.  
和vhints类似有的cpu同时采用VT和PT,  比如early SPARC, VT用来作way select, 用PT做miss detection. 
这里顺便提下, 和cache alias相类似的问题是 Ambiguity: 不同的物理地址映射到相同的VA. 这种情况下在linux内, 只有不同进程的用户空间的页面才可能(fix me). 这种情况会造成同一个cache line在不同的进程中代表不同的数据, 切换进程的时候看似invalid user space 的cache必须进行.  实际上这个也要看是VIVT还是VIPT. 
Process 1:   |31    VA                   20| phy address low bit|    =>   PA1        | phy address low bit|
Process 2:   |31    VA                   20| phy address low bit|    =>   PA2        | phy address low bit|
注：两个进程的虚拟地址完全相同, 就是说phy address low bit 也相同. 
====================
地址切换:
1. 在VIPT 的时候, 或者要像i386那样切换整个pgd, 或者像mips, 有ASID作为区分. 这样TLB 不会有Ambiguity, 同时VIPT 的cache 要比较phy address 来确定 cache hit. 故不用flush cache. 
2. VIVT 的时候就就比较麻烦, 尽管MMU 不会给出错误的PA, 但是因为是vitual tag 也相同, 就会命中上一个进程的对应于不同物理地址cache了, flush cache 就必须进行了.  具体的例子, 如 arm/mm/proc-arm926.S
=====================
TLB 撤销: exit_mm
还有就是撤销TLB的时候,  撤销的时候,VIVT 仍然需要把cache flush, 否则到心的mm后就会有Ambiguity错误了, 见 do_exit -> exit_mm->mmput->exit_mmap->flush_cache_mm. 
ARM cache的一些讨论
仍然有许多关于arm cache问题不清晰: 如2.6的宏cache_is_vivt: 如果编译内核的时候没有明确给出cache type的信息: **CONFIG_CPU_CACHE_VIPT 或者 ****CONFIG_CPU_CACHE_VIVT, 那么****cache_is_vivt 是可靠的吗. 下面的一段注释没有在2.6的内核代码中出现. **或许这是一个经验公式,呵呵,需要列出所有arm cpu以及其cache 种类并根据ctype的类型来 hash出这个判断方式.(fix me).
#define __cacheid_present(val)            (val != read_cpuid(CPUID_ID))
#define __cacheid_type_v7(val)            ((val & (7 << 29)) == (4 << 29))
#define __cacheid_vivt_prev7(val)        ((val & (15 << 25)) != (14 << 25))
#define __cacheid_vipt_prev7(val)        ((val & (15 << 25)) == (14 << 25))
#define __cacheid_vipt_nonaliasing_prev7(val)    ((val & (15 << 25 | 1 << 23)) == (14 << 25))
#define __cacheid_vipt_aliasing_prev7(val)    ((val & (15 << 25 | 1 << 23)) == (14 << 25 | 1 << 23))
#define __cacheid_vivt(val)            (__cacheid_type_v7(val) ? 0 : __cacheid_vivt_prev7(val))
#define __cacheid_vipt(val)            (__cacheid_type_v7(val) ? 1 : __cacheid_vipt_prev7(val))
#define __cacheid_vipt_nonaliasing(val)        (__cacheid_type_v7(val) ? 1 : __cacheid_vipt_nonaliasing_prev7(val))
#define __cacheid_vipt_aliasing(val)        (__cacheid_type_v7(val) ? 0 : __cacheid_vipt_aliasing_prev7(val))
#define __cacheid_vivt_asid_tagged_instr(val)    (__cacheid_type_v7(val) ? ((val & (3 << 14)) == (1 << 14)) : 0)
#if defined(**CONFIG_CPU_CACHE_VIVT**) && !defined(**CONFIG_CPU_CACHE_VIPT**)
#define cache_is_vivt()            1
#define cache_is_vipt()            0
#define cache_is_vipt_nonaliasing()    0
#define cache_is_vipt_aliasing()    0
#define icache_is_vivt_asid_tagged()    0
#elif defined(**CONFIG_CPU_CACHE_VIPT**)
#define cache_is_vivt()            0
#define cache_is_vipt()            1
...
#else ** /*没有明确配置cache type*/**
/*
 * VIVT or VIPT caches.  Note that this is unreliable since ARM926
 * and V6 CPUs satisfy the "(val & (15 << 25)) == (14 << 25)" test.
 * There's no way to tell from the CacheType register what type (!)
 * the cache is.
 */    
#define** cache_is_vivt**()                            \
    ({                                \
        unsigned int __val = read_cpuid(CPUID_CACHETYPE);    \
        (!__cacheid_present(__val)) || __cacheid_vivt(__val);    \
    })
#endif
