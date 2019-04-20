# Linux kernel中的annotation(转) - h13 - 博客园
转自：[http://blog.yaabou.com/?p=82](http://blog.yaabou.com/?p=82)
__user表示是一个用户空间的指针，所以kernel不可能直接使用。
#ifdef __CHECKER__
# define __user __attribute__((noderef, address_space(1)))
# define __kernel /* default address space */
#else
# define __user
# define __kernel
#endif
noderef告诉编译器，不应该解除该指针的引用，因为在当前地址空间中它是没有意义的。
这里的CHECKER表示是否使用了Sprase（就是一种静态分析工具，用来分析内核源码中的BUG）。是不是想研究一下了？呵呵。可以参见[http://sparse.wiki.kernel.org/index.php/Main_Page](http://sparse.wiki.kernel.org/index.php/Main_Page)
所以对于这种变量，在kernel中使用要用到copy_to_user和copy_from_user。
__iomem是2.6.9中加入的特性。是用来个表示指会指向一个I/O的内存空间。主要是为了driver的通用性考虑。由于不同的CPU体系结构对I/O空间的表示可能不同。当使用__iomem时，compiler会忽略对变量的检查（因为用的是void __iomem）。但sparse会对它进行检查，当__iomem的指针和正常的指针混用时，就会发出一些warnings。
下面还有一些为个变量新加入的函数:
    unsigned int ioread8(void __iomem *addr);
    unsigned int ioread16(void __iomem *addr);
    unsigned int ioread32(void __iomem *addr);
    void iowrite8(u8 value, void __iomem *addr);
    void iowrite16(u16 value, void __iomem *addr);
    void iowrite32(u32 value, void __iomem *addr);
可以看一下[http://lwn.net/Articles/102240/](http://lwn.net/Articles/102240/)
和[http://lwn.net/Articles/102232/](http://lwn.net/Articles/102232/)
