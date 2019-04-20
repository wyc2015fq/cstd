# DMA及cache一致性的学习心得 --dma_alloc_writecombine - h13 - 博客园
**来源：[http://xmxohy.blog.163.com/blog/static/53469584201082734630713/](http://xmxohy.blog.163.com/blog/static/53469584201082734630713/)**
先理解cache的作用
CPU在访问内存时，首先判断所要访问的内容是否在Cache中，如果在，就称为“命中（hit）”，此时CPU直接从Cache中调用该内容；否则，就 称为“ 不命中”，CPU只好去内存中调用所需的子程序或指令了。CPU不但可以直接从Cache中读出内容，也可以直接往其中写入内容。由于Cache的存取速 率相当快，使得CPU的利用率大大提高，进而使整个系统的性能得以提升。
Cache的一致性就是直Cache中的数据，与对应的内存中的数据是一致的。
DMA是直接操作总线地址的，这里先当作物理地址来看待吧（系统总线地址和物理地址只是观察内存的角度不同）。如果cache缓存的内存区域不包括DMA分配到的区域，那么就没有一致性的问题。但是如果cache缓存包括了DMA目的地址的话，会出现什么什么问题呢？
问题出在，经过DMA操作，cache缓存对应的内存数据已经被修改了，而CPU本身不知道（DMA传输是不通过CPU的），它仍然认为cache中的数 据就是内存中的数据，以后访问Cache映射的内存时，它仍然使用旧的Cache数据。这样就发生Cache与内存的数据“不一致性”错误。
题外话：好像2.6.29内核中，6410的总线地址和物理地址是一样的，因为我在查看vir_to_bus函数的时候，发现在/arch/arm/linux/asm/memory.h中这样定义： 
#ifndef __virt_to_bus
#define __virt_to_bus    __virt_to_phys
#define __bus_to_virt    __phys_to_virt
#endif
而且用source Insight搜索了一遍，没有发现6410相关的代码中，重新定义__vit_to_bus，因此擅自认为2.6内核中，6410的总线地址就是物理地址。希望高手指点。
顺便提一下，总线地址是从设备角度上看到的内存，物理地址是CPU的角度看到的未经过转换的内存（经过转换的是虚拟地址）
由上面可以看出，DMA如果使用cache，那么一定要考虑cache的一致性。解决DMA导致的一致性的方法最简单的就是禁止DMA目标地址范围内的cache功能。但是这样就会牺牲性能。
因此在DMA是否使用cache的问题上，可以根据DMA缓冲区期望保留的的时间长短来决策。DAM的映射就分为：一致性DMA映射和流式DMA映射。
一致性DMA映射申请的缓存区能够使用cache，并且保持cache一致性。一致性映射具有很长的生命周期，在这段时间内占用的映射寄存器，即使不使用也不会释放。生命周期为该驱动的生命周期
流式DMA映射实现比较复杂，因为没具体了解，就不说明了。只知道种方式的生命周期比较短，而且禁用cache。一些硬件对流式映射有优化。建立流式DMA映射，需要告诉内核数据的流动方向。
因为LCD随时都在使用，因此在Frame buffer驱动中，使用一致性DMA映射
上面的代码中用到 dma_alloc_writecombine函数，另外还有一个一致性DMA映射函数dma_alloc_coherent
两者的区别在于：
查看两者的源代码 
[cpp] [view plain](http://blog.csdn.net/beyondioi/article/details/6991575)[copy](http://blog.csdn.net/beyondioi/article/details/6991575)
http://static.blog.csdn.net/scripts/ZeroClipboard/ZeroClipboard.swf
- /*
- * Allocate DMA-coherent memory space and return both the kernel remapped
- * virtual and bus address for that space.
- */
- void *  
- dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *handle, gfp_t gfp)  
- {  
- void *memory;  
- 
- if (dma_alloc_from_coherent(dev, size, handle, &memory))  
- return memory;  
- 
- if (arch_is_coherent()) {  
- void *virt;  
- 
- virt = kmalloc(size, gfp);  
- if (!virt)  
- return NULL;  
- *handle =  virt_to_dma(dev, virt);  
- 
- return virt;  
- }  
- 
- return __dma_alloc(dev, size, handle, gfp,  
- pgprot_noncached(pgprot_kernel));  
- }  
- 
- /*
- * Allocate a writecombining region, in much the same way as
- * dma_alloc_coherent above.
- */
- void *  
- dma_alloc_writecombine(struct device *dev, size_t size, dma_addr_t *handle, gfp_t gfp)  
- {  
- return __dma_alloc(dev, size, handle, gfp,  
- pgprot_writecombine(pgprot_kernel));  
- }  
- 
- 
- #define pgprot_noncached(prot)  __pgprot(pgprot_val(prot) &~(L_PTE_CACHEABLE | L_PTE_BUFFERABLE))
- #define pgprot_writecombine(prot) __pgprot(pgprot_val(prot) &~L_PTE_CACHEABLE)
再结合网上的资料（不过我感觉那文章写的有些问题，我修改了一下），由上面代码可以看出，两个函数都调用了__dma_alloc函数，区别只在于最后一个参数。
dma_alloc_coherent 在 arm 平台上会禁止页表项中的 C （Cacheable） 域以及 B (Bufferable)域。而 dma_alloc_writecombine 只禁止 C （Cacheable） 域.
 C 代表是否使用高速缓冲存储器， 而 B 代表是否使用写缓冲区。
这样，dma_alloc_writecombine 分配出来的内存不使用缓存，但是会使用写缓冲区。而 dma_alloc_coherent  则二者都不使用。
C B 位的具体含义
0 0 无cache，无写缓冲；任何对memory的读写都反映到总线上。对 memory 的操作过程中CPU需要等待。
0 1 无cache，有写缓冲；读操作直接反映到总线上；写操作，CPU将数据写入到写缓冲后继续运行，由写缓冲进行写回操作。
1 0 有cache，写通模式；读操作首先考虑cache hit；写操作时直接将数据写入写缓冲，如果同时出现cache hit，那么也更新cache。
1 1 有cache，写回模式；读操作首先考虑cache hit；写操作也首先考虑cache hit。
这样，两者的区别就很清楚了。 
A = dma_alloc_writecombine(struct device *dev, size_t size,dma_addr_t *handle, gfp_t gfp);
含义：
A          : 内存的虚拟起始地址，在内核要用此地址来操作所分配的内存
dev      : 可以平台初始化里指定，主要是用到dma_mask之类参数，可参考framebuffer
size      : 实际分配大小，传入dma_map_size即可
handle: 返回的内存物理地址，dma就可以用。
A和hanle是一一对应的，A是虚拟地址，而handle是总线地址。对任意一个操作都将改变写缓冲区内容。
