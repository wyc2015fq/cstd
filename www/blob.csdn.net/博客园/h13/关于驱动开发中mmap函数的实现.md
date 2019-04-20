# 关于驱动开发中mmap函数的实现 - h13 - 博客园
在编写设备驱动程序的时候，如果要想把设备内存映射到用户空间，那需要我们实现mmap，通过看ldd3上面的介绍，对实现mmap有了一点了解．书上介绍主要是利用
int remap_pfn_range(struct vm_area_struct *vma, unsigned long virt_addr,unsigned long pfn, unsigned long size,pgprot_t prot) 函数或者
int io_remap_page_range(struct vm_area_struct *vma, unsigned long virt_addr, unsigned long phys_addr, unsigned long size, pgprot_t prot)函数来实现，它们负责建立新的页表．这两个函数的区别是第一个函数是在参数pfn指向实际系统ＲＡＭ的时候使用，而第二个函数是在phys_addr指向I/O内存的时候使用．对于ＡＲＭ平台来说，系统内存和端口应该是统一编址的，所以两个函数是等价的．
　　还有另外一个函数就是nopage函数，是ＶＭＡ结构体中可以填充的一个函数，在虚拟页没有所对应的物理页的时候，会调用此函数，来分配一个物理页给虚拟页．
　　int remap_page_range(unsigned long from, unsigned long phys_addr, unsigned long size, pgprot_t prot)
其中from是映射开始的虚拟地址。这个函数为虚拟地址空间from和from+size之间的范围构造页表；
phys_addr是虚拟地址应该映射到的物理地址；
size是被映射区域的大小；prot是保护标志。
remap_page_range的处理过程是对from到form+size之间的每一个页面，查找它所在的页目录和页表（必要时建立页表），清除页表项旧的内容，重新填写它的物理地址与保护域。
remap_page_range可以对多个连续的物理页面进行处理。<<Linux设备驱动程序>>指出，remap_page_range只能给予对保留的页和物理内存之上的物理地址的访问，当对非保留的页使用remap_page_range时，缺省的nopage处理控制映射被访问的虚地址处的零页。所以在分配内存后，就要对所分配的内存置保留位，它是通过函数mem_map_reserve实现的，它就是对相应物理页面置
PG_reserved标志位。
remap_pfn_range 通过你的页帧号来建立页表, 并映射到用户空间!
一般情况是你的驱动分配一块内存,然后在驱动的mmap中使用这块内存的物理地址转成页帧号, 再调用remap_pfn_range!
假
设1你是通过kmalloc(),get_free_pages()等分配的，这种内存页是不能通过remap_pfn_range()映射出去的，要对
每个页面调用SetPageReserverd()标记为"保留"才可以，virt_to_phys()函数只是得到其物理地
址，remap_pfn_range()中的第三个参数是要求物理页便的"帧"号，即pfn,所以你的phys还要">> PAGE_SHIFT"操作
假设2你是通过vmalloc分配得来的，同上，不同的是要用vmalloc_to_pfn
3，用kmalloc,get_free_pages，vmalloc分配的物理内存页面最好还是不要用remap_pfn_page方法，建议使用VMA的nopage方法
4，对于这样的设备内存，最好对调用pgprot_nocached(vma->vm_page_prot)后传给remap_pfn_range，防止处理器缓存
