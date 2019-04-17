# LDD3源码分析之vmalloc - 一世豁然的专栏 - CSDN博客





2014年12月18日 16:37:54[一世豁然](https://me.csdn.net/Explorer_day)阅读数：781








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7417174](http://blog.csdn.net/liuhaoyutz/article/details/7417174) [作者：刘昊昱]





一、scullv编译本文分析LDD3第8章中与vmalloc函数相关代码,对应源码是examples/scullv目录下的相关文件。这里首先说明一下，scullv的代码有两部分，一是main.c，另一部分是mmap.c，其中mmap.c主要实现了内存映射相关函数。因为本文主要分析vmalloc的用法，并且mmap.c涉及的内存映射在新的内核中有了很大变化。在本文中，我们不讨论mmap.c，并且为避免编译时出现的错误，编译时也不编译mmap.c。要想在编译scullv模块时不编译mmap.c，只要做如下修改：将Makefile第18行改为： scullp-objs := main.o将main.c的第475行屏蔽掉： //.mmap =      scullp_mmap,这样就不会编译mmap.c了。即使不编译mmap.c，scullv在2.6.32-38-generic-pae内核下编译也会遇到很多问题，但是遇到的问题和解决方法和我在《LDD3源码分析之slab高速缓存》中讨论的是一样的，这里不详细说明了。二、vmalloc及其相关函数vmalloc函数分配的内存在虚拟地址空间是连续的，在物理地址上可能不连续。我们在这里介绍vmalloc函数的原因是，它是Linux内存分配机制的基础。但是，在大多数情况下，我们并不鼓励使用vmalloc，通过vmalloc函数获得的内存使用效率不高，而且在某些体系架构上，用于vmalloc的地址空间总量相对较小。只要可能，应该直接和单个页面打交道，而不是使用vmalloc。vmalloc函数原型及相关函数如下所示：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417174#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417174#)

- void *vmalloc(unsigned long size);  
- void vfree(void * addr);  
- void *ioremap(unsigned long offset, unsigned long size);  
- void iounmap(void * addr);  


需要强调的是，由kmalloc和__get_free_pages返回的内存地址也是虚拟地址，其实际值仍然需要由MMU处理才能转换为物理内存地址。vmalloc在如何使用硬件上没有区别，区别在于内核如何执行分配任务上。
kmalloc和__get_free_pages返回的虚拟地址范围和物理内存是一一对应的，可能会有基于常量PAGE_OFFSET的一个偏移。这两个函数不需要为该地址段修改页表。但是，vmalloc和ioremap使用的地址范围是完全虚拟的，每次分配都要通过对页表的适当设置来建立(虚拟)内存区域。与vmalloc一样，ioremap也建立新的页表，但是和vmalloc不同的是，ioremap并不实际分配内存。ioremap的返回值是一个特殊的虚拟地址，可以用来访问指定的物理内存区域，这个虚拟地址最后要调用iounmap来释放掉。三、scullv分析scullv模块的代码和上一篇文章分析的scullp基本上一样，和scull也很类似。这里我们只分析最重要的代码。scullv中分配内存的代码在scullv_write函数中：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417174#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417174#)

- 244    /* Allocate a quantum using virtual addresses */
- 245    if (!dptr->data[s_pos]) {  
- 246        dptr->data[s_pos] = (void *)vmalloc(PAGE_SIZE << dptr->order);  
- 247        if (!dptr->data[s_pos])  
- 248            goto nomem;  
- 249        memset(dptr->data[s_pos], 0, PAGE_SIZE << dptr->order);  
- 250    }  


scullv中释放内存的代码在scullv_trim函数中：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417174#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417174#)

- 493    /* Release the quantum-set */
- 494    for (i = 0; i < qset; i++)  
- 495        if (dptr->data[i])  
- 496            vfree(dptr->data[i]);  


测试vmalloc模块的过程如下图所示：






