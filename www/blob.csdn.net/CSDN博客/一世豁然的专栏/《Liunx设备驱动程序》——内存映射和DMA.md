# 《Liunx设备驱动程序》——内存映射和DMA - 一世豁然的专栏 - CSDN博客





2014年12月02日 10:36:14[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1415








一、Linux的内核管理

 一）、地址类型

  1、Linux是一个虚拟内存系统，这意味着用户程序所使用的地址与硬件使用的物理地址是不等同的。




  2、虚拟内存引入了一个简介层，它使得许多操作成为可能。

    1）、有了虚拟内存，在系统中运行的程序可以分配弊无利内存更多的内存。

    2）、虚拟地址还能让程序在进程的地址空间内使用更多的技巧，包括将程序的内存映射到设备内存上。




  3、Linux地址类型列表：

    1）、用户虚拟地址。

    2）、物理地址。

    3）、总线地址。

    4）、内核逻辑地址。

    5）、内核虚拟地址。




  4、如果有一个逻辑地址，宏_ _pa()返回对应的物理地址；使用宏_ _va()也能将物理地址逆向映射到逻辑地址，但这只对低端内存页有效。







 二）、物理地址和页

  1、物理地址被分散成离散的单元，称之为页。

    1）、系统内部许多对内存的操作都是基于单元页的。

    2）、每个页大小岁体系结构的不同而不同，但是目前大多数系统都使用每页4096个字节。

    3）、常量PAGE_SIZE给出了在任何指定体系下的页大小。




  2、内存中的地址，无论是虚拟的还是物理的，它们被分为页号和一个页内的偏移量。

    1）、如果忽略了地址偏移量，并将出去偏移量的剩余位移到右端，称该结构为页帧数。移动位以在页帧数和地址空间进行转换是一个常用操作：宏PAGE_SHIFT将告诉程序

              员，必须移动多少位才能完成这个转换。







 三）、高端与低端内存

  1、内核将4GB的虚拟地址空间分割为用户控件和内核空间；在二者的上下文中使用同样的映射，一个典型的分割是将3GB分割给用户空间，1GB分割给内核空间。

    1）、只有内核的低端部分（依赖与硬件和内核的设置。一般为1GB到2GB）拥有逻辑地址；剩余部分是没有的。

    2）、在访问特定的高端内存页前，内核必须建立明确的虚拟映射，是该页可在内核地址空间中被访问。所以，许多内核数据结构必须放置在低端内存中；而高端内存更趋向

               于为用户空间进程页所保留。




  2、高端和低端内存的定义

    1）、低端内存：存在于内核空间上的逻辑地址内存。

    2）、高端内存：是指那些不存在逻辑地址的内存，这是因为它们处于内核虚拟地址之上。







 四）、内存映射和页结构

  1、内核中给您处理内存函数趋向使用所指page结构的指针。该数据结构用来保存内核需要知道的所有内存物理内存信息：对系统中的每个物理页，都有一个page结构相对

        应。下面劫煞牌几个成员：

    1）、atomic_t  count：对页访问计数。当计数值为零时，该页将返回给空闲链表。

    2）、void  *virtual：如果页面被映射，则指向页的内核虚拟地址；如果未被映射则为NULL。

    3）、unsigned  long  flags：描述页状态的一系列标志。




  2、有一些函数和宏用来在page结构指针与虚拟地址之间进行转换。（P413-P414）







 五）、页表

  1、在现在操的系统中，处理器必须使用某种机制，将虚拟地址转换为相应的物理地址，这主公机制被称为页表；他基本上是一个多层树形结构，结构化的数组中包含了虚拟地

        址到物理地址的映射和相关的标志位。







 六）、虚拟内存区

  1、虚拟内存区（VMA）用于管理进程地址空间中不同区域的内核树据结构。一个VMA表示在进程的虚拟内存中的一个同类区域；拥有相同权限标志位和被同样对象（一个文件

        或者交换空间）备份的一个连续的虚拟内存地址范围。




  2、进程的内存映射（至少）包含以下区域：

    1）、程序的可执行代码区域。

    2）、多个数据区，其中包含初始化数据（在开始执行的时候就拥有明确的值）、非初始化数据以及程序堆栈。

    3）、与每个活动的内存映射对应的区域。




  3、/proc/<pid/maps>表示形式：

       每行都是用下面的形式表示的：

       start_end  perm  offset  maior:minor  inode  image

    1）、start和end：该内存区域的起始处和结束处的虚拟地址。

    2）、perm：内存区域读、写和执行权限的位掩码。

    3）、offset：表示内存区域在映射文件中的起始位置。

    4）、major和minor：拥有映射文件的设备的主设备号和次设备号。

    5）、inode：被映射的文件的索引节点号。

    6）、image：被映射文件的名称。




  4、vm_area_struct结构。

    1）、VMA主要成员。（P416-P417）

    2）、vm_operations_struct结构。（P417）








 七）、内存映射处理

  1、内存映射结构负责整合所有其他的数据结构。




  2、在系统中的每个进程都拥有一个struct  mm_struct结构，其中包括虚拟内存区域链表、页表以及其他大量内存管理信息，还包含一个信号灯（mmap_sem）和一个自旋锁

       （page_table_lock）。在结构中能找到该结构的指针；在少数情况下当驱动程序需要访问它时，常用的办法是使用current->mm。










二、mmap设备操作

 一）、相关简介

  1、对于驱动程序来说，内核映射可以提供给用户程序直接访问设备内存的能力。

    1）、映射一个设备意味着将用户空间的一段内存与设备内存关联起来。无论何时当程序分配的地址范围内读写时，实际上访问的就是设备。




  2、mmap的限制

    1）、并不是所有的设备都能进行mmap抽象。

    2）、必须以PAGE_SIZE为单位进行映射。内核只能在页表一级上对虚拟地址进行管理，因此那些被映射的区域必须是PAGE_SIZE的整数倍，并且在物理内存中的起始地址

              也要求是PAGE_SIZE的整数倍。




  3、mmap方法是file_operation结构的一部分，并且执行mmap系统调用时将调出该方法。使用mmap，内核在调用实际函数之前，就能完成大量的工作，因此该方法的原型

        与系统调用有着很大的不同。

    1）、系统调用有着以下声明：

              mmap(caddr_t  addr,  size_t  len,  int  prot,  int  flags,  int  fd,  off_t  offset)

    2）、文件的声明如下：

             int  (*mmap)  (struct  file  *filp,  struct  vm_area_struct  *vma);




  4、有两种建立页表的方法

    1）、使用remap_pfn_range函数一次全部建立。

    2）、通过nopage VMA方法每次建立一个页表。







 二）、使用remap_pfn_range

  1、remap_pfn_range和io_remap_page_range负责为一段物理地址建立新的页表，它们有如下的原型：

        int  remap_pfn_range(struct  vm_area_struct  *vma,  


                                                 unsigned  long  virt_addr,  unsigned  long  pfn);

        int  io_remap_page_range(struct  vm_area_struct  *vma,  


                                                          unsigned  long  virt_addr,  unsigned  long  phys_addr, 


                                                          unsigned  long  size,  pgrot_t  prot);




  2、remap_pfn_range和io_remap_page_range

    1）、remap_pfn_range函数是在pfn指向实际系统RAM的时候使用，而io_remap_page_range是在phys_addr指向I/O内存的时候使用。

    2）、在实际应用中，除了SPARC外，对每个体系架构这两个函数是等价的，而在大多数情况下使用remap_pfn_range函数。对于有可移植性要求的驱动程序，要使用与特

              定情形相符的remap_pfn_range变种。

    3）、另外复杂性也表现在缓存上：对设备内存的引用通常不能被处理器所缓存。







 三）、一个简单的实现。

  1、remap_pfn_range实例。








 四）、为VMA添加操作

  1、针对VMA的open和close操作。当进程打开或者关闭VMA时，会调用这些操作；特别是当fork进程或者创建一个新的对VMA引用时，随时都会调用open函数。




  2、VMA open和close实例。







 五）、使用nopage映射内存

  1、当应用程序要改变一个映射区域所绑定的地址时，会使用mremap系统调用，此时是使用nopage映射的最好时机。如果要支持mremap系统调用，就必须实现nopage函

        数。




  2、nopage函数具有以下原型：

        struct  page  *(*nopage)  (struct  vm_area_struct  *vma,  unsigned  long  abbress,  int  *type);

    1）、当用户要访问VMA中的页时，而该页又不在内存中时，将调用相关的nopage函数。

    2）、addess参数包含了引起错误的虚拟地址，他已经被向下圆整到页的开始位置。

    3）、nopage函数必须定位并返回指向用户所需要页的page结构指针。

    4）、该函数还调用get_page宏，用来增加返回的内存页的使用计数：

           get_page(struct  page  *pageptr);

          I、该步骤对于保证映射页引用计数的正确性是非常必要的。内核为每个内存页都维护了该计数。

    5）、弄page方法还能在type参数所指定的位置中保存错误的类型。




  3、相关代码







 六）、重映射特定的I/O区域

  1、一个典型的驱动程序只映射与其相关的一小段地址，而不是映射全部地址。当应用程序要映射比目标设备可用I/O区域大的内存时，除了计算偏移量，代码还检查参数的合

        法性并报告错误。





  2、注意：用户进程总是使用mremap对映射进行扩展，有可能超过了物理内存区域的尾部。为了防止扩展映射最简单的办法是实现一个简单的nopage方法。它会产生总线信

         号传递给故障进程。







 七）、重新映射RAM

  1、对remap_pfn_range函数的一个限制是：它只能访问保留页和超过物理内存的物理地址。因此，remap_pfn_range不允许重新映射常规地址，这包括调用get_free_page

        函数所获得的地址。相反它能映射内存页。




  2、remap_pfn_range函数无法处理RAM表明：像scull这样基于内存的设备无法简单地实现mmap，因为它的设备内存是通用的RAM，而不是I/O内存。




  3、使用nopage方法重映射RAM：

    1）、将实际的RAM映射到用户空间的方法是：使用vm_ops->nopage一次处理一个页错误。

    2）、*scullp*中mmap实现的设计选择。








 八）、重新映射内核虚拟地址










三、直接执行I/O访问

 一）、相关简介

  1、直接I/O操作的使用情况

    1）、如果需要传输的数据量非常大，直接进行数据传输，而不需要额外的从内核空间拷贝数据操作的参与。

    2）、直接I/O并不能像人们期望的那样，总是能提供性能上的飞跃。设置直接I/O的开销非常巨大，而又没有使用I/O的优势。




  2、在2.6内核中，实现直接I/O的关键是名为get_user_pages的函数，它定义在<linux/mm.h>中，并有一下原型。

        int  get_user_pages(struct  task_struct  *tsk,  struct  mm_struct  *mm,

                                            unsigned  long  start,  int  len,  int  write,  int  force,

                                            struct  page  **page,  struct  vm_area_struct  **vmas);

    1）、get_user_pages函数是一个底层内存管理函数，使用了比较复杂的接口。它还需要在使用前，将mmap为获得地址空间的读取者/写入者信号量设置为读模式。




  3、如果改变了页，则必须使用下面的函数标记出每个被改变的页：

        void  SetPageDirty(struct  page  *page);




  4、不管页是否被改变，它们都必须从页缓存中释放，否额它们会永远存在。

         void  page_cache_release(struct  page  *page);







 二）、异步I/O

  1、异步I/O允许用户控件初始化操作，但不必等待它们完成。这样，当I/O在执行时，应用程序可以进行其他的操作。一个复杂的、高性能的应用程序也能使用异步I/O，让多个

        操作同时进行。




  2、支持异步I/O的驱动程序应该包含<linux/aio.h>。有三个用于实现异步I/O的file_operations方法：

        ssize_t  (*aio_read)  (strcu  kiocb  *iocb,  char  *buffer,  size_t  count,  loff_t  offset);

        ssize_t  (*aio_write)  (strcu  kiocb  *iocb,  char  *buffer,  size_t  count,  loff_t  offset);

        int  (*aio_fsync)  (struct  kiocb  *iocb,  int  datasync);




  3、同步操作会在IOCB中标识，因此，驱动程序应该使用下面的函数进行查询：

        int  is_sync_kiocb(struct  kiocb  *iocb);




  4、当未来某个时刻到来时，驱动程序必须通知内核操作已经完成。

        int  aio_complete(struct  kiocb  *iocb,  long  res,  long  res2);




  5、异步I/O实例。










四、直接内存访问

 一）、相关简介

  1、直接内存访问（DMA），是一种硬件机制，它允许外围设备和主内存之间直接传输它们的I/O数据，而不需要系统处理器的参与。







 二）、DMA数据传输概览

  1、有两种方式引发传输：

    1）、方式一：软件对数据的请求。

    2）、方式二：硬件异步地将数据传递给系统。




  2、方式一数据传输所需要的步骤。（P435）




  3、方式二发生在异步使用DMA时及其数据传输步骤。（P435-P436）




  4、DMA需要设备驱动程序分配一个或多个适合执行DMA的特殊缓冲区——DMA缓冲区。







 三）、分配DMA缓冲区

  1、使用DMA缓冲区的主要问题是：当大于一页时，它们必须占据连续的物理页，这是因为设备使用ISA或者OCI系统总线传输数据，而这两种方式使用的都是物理地址。




  2、虽然可以在系统启动时，也可以在运行时分配DMA缓冲区，但是模块只能在运行时刻分配它们的缓冲区。

    1）、驱动程序作者必须谨慎地为DMA操作分欸正确的内存类型，因为并不是所有的内存区间都适合DMA操作。

    2）、在实际操作中，一些设备和一些系统中的高端内存不能用于DMA，这是因为外围设备不能使用高端内存的地址。




  3、对于有限制的设备：

    1）、应使用GFP_DMA标志调用kmalloc或者get_free_pages从DMA区间分配内存。当设置了该标志，只有使用24位寻址方式的内存才能被分配。

    2）、可以使用通用DMA层来分配缓冲区。




  4、DIY分配

    1）、get_free_pages函数分配的内存有时也会失败。当内核不能返回请求数量的内存，或者需要大于128KB内存的时候，相对于返回“ENOMEM”，另外的一个办法是在引导

              是分配内存，或者为缓冲区保留顶部物理RAM。但引导分配内存对模块来说不可行。

    2）、还有一个和办法就是使用GFP_NOFAIL分配标志来为缓冲区分配内存。但是该方法位内存管理子系统带来了相当大的压力。

    3）、如果需要为DMA分配缓冲区分配一大块内存，最好考虑一下是否有替代方法。如果设备支持分散/聚集I/O，则可以将缓冲区分配成多个小块，设备会很好处理它们，当

              在用户空间中执行了直接I/O的时候，也可以使用分散/聚集I/O。







 四）、通用DMA层

  1、内核提供了一个与总线——体系结构无关的DMA层，它会隐藏大多数问题，强烈建议在编写驱动程序时，为DMA操作使用该层。




  2、处理复杂的硬件

    1）、在执行DMA之前，第一个必须回答的问题是：是否给定的设备在当前主机上具备执行这些操作的能力。处于很多原因，许多设备受限于它们的寻址范围。

    2）、默认情况下，内核假设设备都能在32位地址上执行DMA。如果不是这样，应该调用下面的函数通知内核：

              int  dma_set_mask(struct  device  *dev,  u64  mask);

    3）、如果设备支持常见的32位的DMA操作，则没有必须调用dma_set_mask。




  3、DMA映射

    1）、一个DMA映射是要分配的DMA缓冲区与为该缓冲区生成的、设备可访问地址的组合。

    2）、在某些情况下，为设备设置可用地址需要建立回弹缓冲区。

    3）、DMA映射必须解决缓存一致性的问题。

    4）、DMA映射建立了一个新的结构类型——dma_addr_t来表示总线地址。

    5）、根据DMA缓冲区期望保留的时间长短，PCI代码区分两种类型的DMA映射：

       I、一致性的DMA映射。

       II、流式DMA映射。




  4、建立一致性的DMA映射

    1）、驱动程序可调用dma_alloc_conerent函数建立一致性映射：

            void  *dma_alloc_conerent(struct  device  *dev,  size_t  size,  dma_addr_t  *dma_handle,  int  flags);

    2）、当不再需要缓冲区时，调用dma_free_conherent向系统返回缓冲区：

            dma_free_conherent(struct  device  *dev,  size_t  size,  void  *vaddr,  dma_addr_t  dma_handle);




  5、DMA池

    1）、DMA池是一个生成小型、一致性DMA映射的机制。

    2）、DMA池必须在使用前，调用下面函数创建：

            struct  dma_pool  *dma_pool_create(const  char  *name,  struct  device  *dev,

                                                                                size_t  size,  size_t  align,  size_t  allocation);

    3）、当使用完DMA池后，调用下面的函数释放：

            void  dma_pool_destroy(struct  dma_pool  *pool);

    4）、使用dma_pool_alloc函数处理分配问题：

            void  *dma_pool_alloc(struct  dma_pool_t,  int  mem_flags,  dma_addr_t  *handle);

    5）、使用下面的函数返回不需要的缓冲区：

            void  dma_pool_free(struct  dma_pool  *pool,  void  *vaddr,  dma_addr_t  addr);





  6、建立流式DMA映射

    1）、当建立流式映射时，必须告诉内核数据流动的方向。为此定义了一些符号（dma_data_direction枚举类型）

       I、DMA_TO_DEVICE

            DMA_FROM_DEVICE

       II、DMA_BIDIRECTIONAL

       III、DMA_NONE

    2）、当只有一个缓冲区要传输的时候，使用dma_map_single函数映射它：

          dma_addr_t  dma_map_single(struct  device  *dev,  void  *buffer,  size_t  size,

                                                                     enum  dma_data_direction  direction);

    3）、当传输完毕后，使用dma_unmap_single删除映射：

         dma_unmap_single(struct  device  *dev,  dma_addr_t  dma_addr,  size_t  size,

                                                enum  dma_data_direction  direction);

    4）、几个重要的流式原则。（P443）

    5）、有时候，驱动程序需要不经过撤销映射就访问流式DMA缓冲区的内容，为此内核提供了如下调用：

         void  dma_sync_single_for_cpu(struct  device  *dev,  dma_handle_t  bus_addr,


                                                                     size_t  size,  enum  dma_data_direction  direction);

    6）、在设备访问缓冲区前，应该调用下面的函数将所有权交还给设备：

        void  dma_sync_single_fir_device(struct  device  *dev,  dma_handle_t  bus_addr,


                                                                     size_t  size,  enum  dma_data_direction  direction);

  7、单页流式映射

    1）、  有时候，要为page结构指针指向的缓冲区建立映射，使用下面的函数，建立和撤销使用page结构织针的流式映射：

         dma_addr_t  dma_map_page(struct  device  *dev,  struct  page  *page,  unsigned  long  offset,

                                                                  size_t  size,  enum  dma_data_direction  direction);

         void  dma_unmap_page(struct  device  *dev,  dma_addr_t  dma_address,

                                                       size_t  size,  enum  dma_data_direction  direction);




  8、分散/聚集映射

    1）、分散/聚集映射是一种特殊类型的流式DMA映射。

    2）、映射分散表的第一个步是建立并填充一个描述符被传送缓冲区的scatterlist结构的数组。该结构是与体系结构相关的，并且在头文件<linux/scatterlist.h>中描述。

        I、struct  page  *page;

        II、unsigned  int  length;

              unsigned  int  offset;

    3）、为了映射一个分散/聚集DMA操作，驱动程序应当为传输的每个缓冲区在scatterlist结构对应入口项上设置page、offset和length成员。然后调用：

         int  dma_map_sg(struct  device  *dev,  struct  scatterlist  *sg,  int  nents,

                                           enum  dma_data_direction  direction);

    4）、驱动程序应该传输由dma_map_sg函数返回的每个缓冲区。总线地址和每个缓冲区的长度被保存在scatterlist结构中，但是它们在结构中的位置会随体系结构的不同而

              不同。使用已经定义的两个宏，可用来编写可移植性代码：

         dma_addr_t  sg_dma_address(struct  scatterlist  *sg);

         unsigned  int  sg_dma_len(struct  scatterlist  *sg);

    5）、一旦传输完毕，使用dma_unmap_sg函数解除分散/聚集映射：

         void  dma_unmap_sg(struct  device  *dev,  struct  scatterlist  *list,


                                                   int  nents,  enum  dma_data_direction  direction);

    6）、分散/聚集映射是流式DMA映射，因此适用于流式映射的规则也适用于该种映射。如果必须访问映射的分散/聚集列表，必须首先对其同步：

         void  dma_sync_sg_for_cpu(struct  device  *dev,  struct  scatterlist  *list,

                                                              int  nents,  enum  dma_data_direction  direction);

         void  dma_sync_sg_for_device(struct  device  *dev,  struct  scatterlist  *list,

                                                                   int  nents,  enum  dma_data_direction  direction);




  9、PCI双重地址周期映射

    1）、通常DMA支持层使用32位总线地址，其为设备的DMA掩码所约束。然而PCI总线还支持64位地址模式，即双重地址周期（DAC）。

    2）、为了使用DAC，驱动程序必须包含头文件<linux/pci.h>。还必须设置一个单独的DMA掩码：

         int  pci_dac_set_dma_mask(struct  pci_dev  *pdev,  u64  mask);

    3）、在DAC映射中使用了一个特殊类型(dma64_addr_t)。调用pci_page_to_dma函数可建立一个这样的映射：

         dma64_addr_t  pci_dac_page_to_dma(struct  pci_dev  *pdev,  struct  page  *page,

                                                                                  unsigned  long  offset,   int  direction);

    4）、DAC映射不需要其他另外的资源，因此在使用后，不需要显式释放它。然后像对待其他流式映射一样对待它是必要的。关于缓冲区所有权的规则也使用于它。有一套用

              于同步DMA缓冲区的函数：

        void  pci_dac_dma_sync_single-for_cpu(struct  pci_dev  *pdev,  dma64_addr_t  dma_addr,

                                                                                    size_t  len,  int  direction);

        void  pci_dac_dma_sync_single-for_device(struct  pci_dev  *pdev,  dma64_addr_t  dma_addr,                                                                                   


                                                                                        size_t  len,  int  direction);







 五）、ISA设备的DMA

  1、ISA总线允许两种DMA传输：本地DMA和ISA总线控制DMA。




  2、有三种实体涉及到ISA总线上的DMA数据传输：

    1）、8237DMA控制器（DMAC）。

    2）、外围设备。

    3）、设备驱动程序。




  3、在PC使用的早期DMA控制器能够管理四个“通道”，每个通道都与一套DMA寄存器相关联。

    1）、四个设备可以同时在控制器职工保存它们的DMA信息。通道编号从0-7：通道4在内部用来将从属控制器级联到主控制器上，因此对ISA外围设备来说，通道4不可用。

    2）、可用的通道是从属控制器（8位通道）上的0-3和主控制器（16位通道）上的5-7。




  4、注册DMA

    1）、包含头文件<asm//dma.h>之后，用下面的函数可以获得和释放对DMA通道的所有权：

        int  request_dma(unsigned  int  channel,  const  char  *name);

        void  free_dma(unsigned  int  channel);




  5、与DMA控制器通信

    1）、当read或者write函数被调用时，或者准备异步传输时，驱动程序都要对DMA控制器进行配置。根据驱动程序和其实现配置的策略，这一工作可以在调用open函数，或

              者响应ioctl命令时被执行。

    2）、DMA通道是一个可共享的资源，如果多于一个的处理器要同时对其进行编程，则会产生冲突。因此有一个叫做dma_spin_lock的自旋锁保护控制器。驱动程序不能直接

              处理该自旋锁；但是有两个函数能够对它进行操作：

        unsigned  long  claim_dma_lock();

        void  release_dma_lock(unsigned  long  flags);

    3）、必须被装入控制器的信息包含三个部分：RAM的地址，必须被传输的原子项个数以及传输的方向。为达到此目的，<asm/dma.h>定义了下面的函数：

        void  set_dma_mode(unsigned  int  channel,  char  mode);

        void  set_dma_addr(unsigned  int  channel,  unsigned  int  addr);

        void  set_dma_count(unsigned  int  channel,  unsigned  int  count);

    4）、当处理DMA设备时，还有许多用于管理设备的函数：

        void  disable_dma(unsigned  int  channel);

        void  enable_dma(unsigned  int  channel);

        int  get_dma_residue(unsigned  int  channel);


        int  clear_dma_ff(unsigned  int  channel);




