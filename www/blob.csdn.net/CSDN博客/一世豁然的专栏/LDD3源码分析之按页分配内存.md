# LDD3源码分析之按页分配内存 - 一世豁然的专栏 - CSDN博客





2014年12月18日 16:35:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：607








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7417042](http://blog.csdn.net/liuhaoyutz/article/details/7417042) [作者：刘昊昱]





一、scullp编译本文分析LDD3第8章中按页分配内存相关代码,对应源码是examples/scullp目录下的相关文件。这里首先说明一下，scullp的代码有两部分，一是main.c，另一部分是mmap.c，其中mmap.c主要实现了内存映射相关函数。因为本文主要分析按页分配内存，并且mmap.c涉及的内存映射在新的内核中有了很大变化。在本文中，我们不讨论mmap.c，并且为避免编译时出现的错误，编译时也不编译mmap.c。要想在编译scullp模块时不编译mmap.c，只要做如下修改：将Makefile第18行改为： scullp-objs := main.o将main.c的第475行屏蔽掉： //.mmap =      scullp_mmap,这样就不会编译mmap.c了。即使不编译mmap.c，scullp在2.6.32-38-generic-pae内核下编译也会遇到很多问题，但是遇到的问题和解决方法和我在上一篇文章《LDD3源码分析之slab高速缓存》中讨论的是一样的，这里不详细说明了。二、按页分配内存相关函数如果模块需要分配大块的内存，最好按页分配，可使用如下函数：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)

- get_zeroed_page(unsigned int flags);  


分配一页内存，并将内存清零。



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)

- _ _get_free_page(unsigned int flags);  


分配一页内存，但是不将内存清零。



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)

- _ _get_free_pages(unsigned int flags, unsigned int order);  


分配若干页内存，其物理地址连续，返回指向第一个字节的指针，不清零页面。
参数flags和kmalloc函数中的一样。order是要申请页面数的以2为底的对数。释放内存可使用如下函数：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)

- void free_page(unsigned long addr);  
- void free_pages(unsigned long addr, unsigned long order);  


第一个函数是宏，展开后就是对第二个函数的调用。
三、scullp分析scullp模块的代码和上一篇文章分析的scullc基本上一样，和scull也很类似。这里我们只分析最重要的代码。scullp中分配内存的代码在scullp_write函数中：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)

- 243    /* Here's the allocation of a single quantum */
- 244    if (!dptr->data[s_pos]) {  
- 245        dptr->data[s_pos] =  
- 246            (void *)__get_free_pages(GFP_KERNEL, dptr->order);  
- 247        if (!dptr->data[s_pos])  
- 248            goto nomem;  
- 249        memset(dptr->data[s_pos], 0, PAGE_SIZE << dptr->order);  
- 250    }  


scullp释放内存的代码在scullp_trim函数中：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7417042#)

- 493    /* This code frees a whole quantum-set */
- 494    for (i = 0; i < qset; i++)  
- 495        if (dptr->data[i])  
- 496           free_pages((unsigned long)(dptr->data[i]),  
- 497                      dptr->order);  


代码很简单，从用户角度看，使用scullp速度更快了，使用内存效率更高了。测试scullp模块的过程如下图所示：



![](https://img-my.csdn.net/uploads/201203/31/1333196852_5316.png)




