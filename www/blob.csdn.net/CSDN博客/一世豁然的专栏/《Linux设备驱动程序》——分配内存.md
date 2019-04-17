# 《Linux设备驱动程序》——分配内存 - 一世豁然的专栏 - CSDN博客





2014年11月17日 11:13:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1476








一、kmalloc函数的内幕

 一）、相关介绍

  1、kmalloc内存分配引擎是一个非常强大的工具。除非被堵塞，否者这函数可以运行的很快，而且不对所获取的内存空间清零。也就是说，分配给它的区域仍然保持原有的数

        据。




  2、kmalloc分配的区域在物理内存中也是连续的。




  3、函数原型

      #include<linux/slab.h>

      void  *kmalloc(size_t  size,  int  flags);








 二）、flags参数

  1、flags参数为要分配的块的大小。




  2、flags参数是分配标识，它能够以多种方式控制kmalloc的行为。




  3、分配标志

    1）、GFP_ATOMIC。

    2）、GFP_KERNEL。

    3）、GFP_USER。

    4）、GFP_HIGHUSER。

    5）、GFP_NOIO。

              GFP_NOFS。




  4、以上的分配标识可以与下面的标识“或”起来使用

    1）、_ _GFP_DMA。

    2）、_ _GFP_HIGHMEM。

    3）、_ _GFP_COLD。

    4）、_ _GFP_NOWARN。

    5）、_ _GFP_HIGH。

    6）、_ _GFP_REPEAT。

              _ _GFP_NOFAIL。

              _ _GFP_NORETRY。










 三）、内存区段

  1、Linux内核把内存分为三个区段：可用于DMA的内存，常规内存以及高端内存。




  2、DMA的内存

    1）、可用于DMA的内存指存在于特别地址范围内的内存。外设可利用这些内存执行DMA访问。在大多数健全的系统上，所有内存都位于这一区段。




  3、常规内存

    1）、通常的内存分配都发生在常规内存区，但通过设备相关标志也可以请求在其他区段中分配。





  4、高端内存


    1）、高端内存是32位平台为了访问（相对）大量的内存而存在的一种机制。




  5、当一个新页面为满足kmalloc的要求被分配时，内存会创建一个内存区段的列表以提供搜索：

    1）、如果指定了_ _GFP_DMA标志，则只有DMA区段会被搜索。

    2）、如果低地址段上没有可用内存，分配就会失败。

    3）、如果没有指定特定的标识，则常规区段和DMA区段都会被搜索。

    4）、如果设置了_ _GFP_HIGHMEM标志，则所有的区段都会被搜索以获取一个空闲页。







 四）、size参数

  1、内存负责管理系统物理内存，物理内存只能按页面进行分配。




  2、Linux处理内存分配方法是，创建一系列的内存对象池，每个池中的内存块大小是固定一致的。处理分配请求时，就直接在包含有足够大的内存块的池中传递一个整块给请

        求者。




  3、驱动程序开发人员应该记住一点，就是只能分配一些预定义的，固定大小的字节数组。程序员应该记住，kmalloc能处理的最小的内存块是32位或者64位，到底哪个则取决

        于当前体系结构使用的页面大小。




  4、对kmalloc能够分配的内存块大小，存在一个上限，这个限制随着体系架构的不同以及内核配置选项的不同而变化。如果我们希望代码具有完整的可移植性，则不应该分配

        大于128KB的内存。










二、后备高速缓存

 一）、后备高速缓存


 1、驱动程序通常不会涉及使用后备高速缓存的内存的行为，但也有例外，如Linux2.6内核中的USB和SCSI驱动程序。




 2、Linxu内核的高速缓存管理有时称为“slab分配器”。因此，相关的函数和类型都在<linux/slab.h>中声明。slab分配器的实现的高速内存具有kmem_cache_t类型。可通过调

       用kmem_cache_create创建：

       kmem_cache_t  *kmem_cache_create(const  char  *name,  size_t  size

                                                                                size_t  offset;

                                                                                 unsigned  long  flags,

                                                                                 void  (*constructtor)(void  *,  kmem_cache_t  *,  unsigned  long  flags),

                                                                                 void  (*destructor)(void  *,  kmem_cache_t  *,  unsigned  long  flags));

    1）、该函数创建一个新的高速缓存对象，其中可以容纳任一数目的内存区域，这些区域的大小都相同，由size参数指定。

    2）、参数name与此告诉缓存相关联，其功能是保存一些信息以便追踪问题，它通常被设置为将要高速缓存的结构类型的名字，高速缓存保留指向该名称的指针，而不是复

               制其内容。

    3）、offset参数是页面中第一个对象的偏移量，它可以用来确保对已分配的对象进行某种特殊的对齐，但是最常用的就是0，表示使用默认值。

    4）、flags参数指控如何完成分配，是一个位掩码，可取值如下：

              I、SLAB_NO_REAP。

              II、SLAB_HWCACHE_ALIGN。

              III、SLAB_CACHE_DMA。

    5）、constructtor和destructor参数是可选的参数；前者用于初始化新分配的对象，而后者用于“清除”对象——在内存空间被整个释放给系统。




  3、为了简便起见，程序员可用同一个函数同时作为constructtor和destructor使用；当调用的是一个constructtor函数的时候，slab分配器总是传递

         SLAB_CTOR_CONSTRUCTOR标志。




  4、一旦某个对象的告诉缓存被创建，就可以调用kmem_cache_alloc从中分配内存对象：

        void  *kmem_cache_alloc(kmem_cache_t  *cache,  int  flags);




  5、释放一个内存对象时使用kmem_cache_free:

         void  kmem_cache_free(kmem_cache_t  *cache,  const  void  *obj);




  6、释放高速缓存：

         int  kmem_cache_destroy(kmem_cache_t  *cache);







 二）、基于slab高速缓存的scull：scullc







 三）、内存池

  1、为了确保内核中中有些地方的内核分配绝对不失败，内核开发者建议了一种称为“内存池”的抽象。内存池其实就是某种形式的后备高速缓存，它试图，时钟保存空闲的内

        存，以便在紧急状态下使用。




  2、内存池对象的类型为mempool_t，可使用mempool_create来建立内存池对象：

        mempool_t  *mempool_create(int  min_tr,  mempool_alloc_t  *alloc_fn,

                                                                  mempoll_free_t  *free_t  *free_fn,  void  *pool_data);

    1）、min_tr参数表示的是内存池应始终保持的已分配对象的最少数目。

    2）、对象的实际分配和释放由alloc_fn和free_fn函数处理，其原型如下：

      typedef  void  *(mempool_alloc_t)(int  gfp_mask,  void  *pool_data);

      typedef  void  *(mempool_free_t)(void  element,  void  *pool_data);

    3）、pool_data参数被传入alloc_fn和free_fn。




  3、内核中有两个函数（mempoll_alloc_slab和mempool_free_slab），它们的原型和上述内存池原型匹配，并利用kmem  alloc_fn和kmem  free_fn处理内存和释放。




  4、可以利用下面的函数来调整mempool的大小：

        int  mempool_resize(mempool_t  *pool,  int  new_min_nr,  int  gfp_mask);

    1）、如果对该函数的调用成功，将把内存池的大小调整为至少有new_min_tr个顶分配对象。




  5、如果不再需要内存池，可使用下面的函数将其返回给系统：

        void  mempool_destroy(mempool_t  *pool);

    1）、在销毁mempool之前，必须将所有已分配的对象返回到内存池中，否则会导致内核ops。




  6、mempool会分配一些内存块，空闲且不会真正得到利用。










三、get_free_page和相关函数

 一）、叶分配技术

  1、分配页面可使用下面的函数：

    1）、get_zeroed_page(unsigned  int  flags);

    2）、_ _get_free_page(unsigned  int  flags);

    3）、_ _get_free_pages(unisgned  int  flags,  unisgned  int  order);

       I、flags参数的作用和kmalloc中的一样。

       II、参数表示要申请或释放的页面数的以2为底的对数。




  2、释放页面。第一个函数是一个宏，展开后就是对第二个函数的调用：

    1）、void  free_page(unsigned  long  addr);

    2）、void  free_pages(unsigned  long  addr,  unsigned  long  order);

       I、如果试图释放和先前分配书怒不等的页面，内存映射关系就会被破坏，随后系统就会出错。




  3、只要符合和kmolloc同样的规则，get_free_pages和其他函数可以在任何时间调用。







 二）、使用整页的csull：csullp







 三）、alloc_pages接口

  1、struct  page是内核用来描述单个内存页的数据结构。




  2、Linux页分配器的核心代码是成为alloc_pages_node的函数：

        struct  page  *alloc_pages_node(int  nid,  unsigned  int  flags,  unsigned  int  order);

        此函数有两个变种，大多数情况下使用这两个宏：

        struct  page  *alloc_pages(unsigned  int  flags,  unsigned  int  order);

        struct  page  *alloc_page(unsigned  int  flags);





  3、释放由上述函数释放的页面：

        void  _ _free_page(struct  page  *page);

        void  _ _free_pages(struct  page  *page,  unsigned  int  order);

        void  free_hot_page(struct  page  *page);

        void  free_cold_page(struct  page  *page);











四、vmalloc及其函数  

 一）、vmalloc内存分配函数

  1、vmalloc和拿书分配虚拟地址空间的连续区域，尽管这段区域在物理上可能是不连续的，内核却认为它们在地址上是连续。vmalloc在错误时，返回0(NULL)，成功时返回一

        个指针。




  2、在多数大多数情况下不鼓励使用vmalloc，通过vmalloc获得的内存使用起来效率不高，而且在某些体系结构架构上，用于vmalloc的地址空间总量相对较小。




  3、vmalloc函数的原型及其相关函数：

        void  *vmalloc(unsigned  long  size);

        void  vfree(void  *addr);

        void  *ioremap(unsigned  long  offset,  unsigned  long  size);

        void  iounmap(void  *addr);




  4、要强调的是，由kmalloc和_ _get_free_pages返回的内存地址也是虚拟地址，其实际值仍然需要由MMU处理才能转换为物理内存地址。vmalloc在如何使用硬件上没有区

        别，区别在于内核如何执行分配上。




  5、用vmalloc分配得到的地址是不能在微处理器之外使用。但驱动程序需要真正的物理地址时，就不能使用vmalloc。使用vmalloc的函数的正确场合是在分配一大块连续的，

        只在软件中存在的，用于缓冲的内存区域的时候。







 二）、使用虚拟地址的scull：scullv










五、per-CPU

  1、当建立一个per-CPU变量时，系统中的每个处理器都会拥有该变量的特有副本。对per-CPU变量的访问（几乎）不需要锁定，因为每个处理器在其自己的副本工作。per-

        CPU变量还可以保存在对应处理器的高速缓存中，这样就可以在频繁更新时获得更好的性能。




  2、要编译和创建一个per-CPU变量，可使用如下宏：

        DEFINE_PER_CPU(type,  name);

        如果该变量是一个数组，需在type中包含数组的维数。




  3、应该显式地调用get_cpu_var宏访问某给定的当前处理器副本，结束后调用put_cou_var。对put_cou_var的调用将返回当前处理器变量版本的lvalue值，并禁止抢占。




  4、使用PER-CPU宏访问其他处理器的变量副本。




  5、动态分配per-CPU变量也是可能的：

        void  8alloc_percpu(type);

        void  * _ _alloc_percpu(void  *per_cpu_var,  int  cpu_id);




  6、Per-CPU变量可以导出给模块，但是必须使用上述宏的特殊版本：

        EXPORT_PER_CPU_SYMBOL(per-cpu-var);

        EXPORT_PER_CPU_SYMBO_GPL(per_cpu_var);

        要在模块中访问这样一个变量，则应将其声明如下：

        DECLARE_PER_CPU(type,  name);











六、获得更大的缓冲区

  1、如果的确需要连续的大块内存用作缓冲区，就最好在系统引导期间通过请求内存来分配。

    1）、在引导时就进行分配是获得大量连续内存页面的惟一方法。

    2）、在引导时分配缓冲区有点“脏”，因为它通过保留私有内存池而跳过了内核的内存管理策略。这种技术比较粗暴也很不灵活，但也是最容易失败。

    3）、对普通用户来说引导时的分配不是一个切实可用的选项。




  2、通过下面函数之一则可完成引导时的内存分配：

        #include<linux/bootmem>

        void  *alloc_bootmem(unsigned  long  size);

        void  *alloc_bootmem_low(unsigned  long  size);

        void  *alloc_bootmem_pages(unsigned  long  size);

        void  *alloc_bootmem_low_pages(unsigned  long  size);




  3、释放引导时分配的内存：

        void  free_bootmem(unsigned  long  addr,  unsigned  long  size);




