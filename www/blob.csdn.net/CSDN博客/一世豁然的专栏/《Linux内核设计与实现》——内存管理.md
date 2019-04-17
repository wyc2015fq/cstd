# 《Linux内核设计与实现》——内存管理 - 一世豁然的专栏 - CSDN博客





2014年11月22日 10:14:21[一世豁然](https://me.csdn.net/Explorer_day)阅读数：815








一、页

  1、内核把物理页作为内存管理的基本单位。

    1）、内存管理单元通常以页为单位进行处理。正因为如此，MMU以页（page）大小为单位来管理系统中的页表。

    2）、从虚拟内存的角度上看，页就是最小单位。

    3）、大多数32位体系结构支持4KB的页，而64位体系结构一般会支持8KB的页。




  2、内核用struct  page结构表示系统中的每个物理页，该结构位于<linux/mm_types.h>中：

        struct  page {

            unsigned  long  flags;

            atomic_t   _count;

            atomic_t  _mapcount;

            unsigned  long  private;

            struct  address_space  *mapping;

            pgoff_t  index;

            struct  list_head  lru;

            void  *virtual;

        };

    1）、page结构与物理页相关，因为内核需要知道一个页是否空闲。如果页已经被分配，内核还需要知道谁拥有这个页。







二、区

  1、内核把页页划分为不同的区（zone），内核使用区对具有相似特性的页进行分组。




  2、Linux主要使用四种区：

    1）、ZONE_DMA——这个区包含的页能用来执行DMA操作。

    2）、ZINE_DMA32——和ZONE_DMA类似，该区包含的页面可用来执行DMA操作；而和ZONE_DMA不同之处在于，这些页面只能被32位设备访问。在某些体系结构中，该

              区将比ZONE_DMA更大。

    3）、ZONE_NORMAL——这个区包含的都是能正常映射的页。

    4）、ZONE_HIGHEM——这个区包含“高端内存”，其中的页并不能永久地映射到内核地址空间。




  3、区的实际使用和分布是与体系结构有关的。

    1）、X86-32上的区（P188  表12-1）




  4、Linux把系统的页划分为区，形成不同的内存池，这样就可以根据用途进行分配。注意，区的划分没有任何物理意义，这只不过是内核为了管理页而采取的一种逻辑上的分

        组。




  5、某些分配可能需要从特定的区中获取页，而另外一些分配则可以从多个区中获取页。不是所有的体系结构都定义了全部区。




  6、每个区都用struct  zone表示，在<linux/mmzone>中定义：

        struct  zone {

            unsigned  long  watermark[NR_WMARK];

            unsigned  long  lowmem_reserve[MAX_NR_ZONES];

            struct  per_cpu_pageset  pageset[NR_CPUS];

            spinlock_t  lock;

            struct  free_area  free_area[MAX_ORDER];

            spinlock_t  lru_lock;

            struct  zone_lru {

                 struct  list_head  list;

                 unsigned  long  nr_saved_scan;

             }  lru[NR_LRU_LISTS];

            struct  zone_reclaim_stat  reclaim_stat;

            unsigned  long  pages_scanned;

            unsigned  long  flags;

            atomic_long_t  am_stat[NR_VM_ZONE_STAT_ITEMS];

            int  prev_priority;

            unsigned  int  inaction_ratio;

            wait_queue_head_t  *wait_table;

            unsigned  long  wait_table_hash_nr_entries;

            unsigned  long  wait_table_bits;

            struct  pglist_data  *zone_pgdat;

            unsigned  long  zone_start_pfn;

            unsigned  long  spanned_pages;

            unsigned  long  present_pages;

            const  char  *name;

         };

    1）、lock域是一个自旋锁，它防止该结构被并发访问。

    2）、watermark数组持有该区的最小值、最低和最高水位值。

    3）、name域是一个以NULL结束的字符串表示这个区的名字。










三、获得页

 一）、相关简介

  1、内恶化提供了一种请求内存的底层机制，并提供了对它进行访问的几个接口。所有这些接口都以页为单位分配内存，定义在<linux/gfp.h>中。最核心函数是：

        struct  page  *  alloc_pages(gfp_t  gfp_mask,  unsigned  int  order);




  2、把指定的页转换成它的逻辑地址：

        void  *  page_address(struct  page  *page);




  3、无需用到struct  page结构的函数

        unsigned  long  __get_free_pages(gfp_t  gfp_mask,  unsigned  int  order);




  4、其他函数

        struct  page  *  alloc_page(gfp_t  gfp_mask);

        unsigned  long  __get_free_page(gfp_t  gfp_mask);







 二）、获得填充为0的页

  1、需要返回的页的内容全为0：

        unsigned  long  get_zeroed_page(unsigned  int  gfp_mask);




  2、低级页分配方法（P190  表12-2）







 三）、释放页

  1、当不需要页时可以用下面的函数释放它们：

        void   __free_pages(struct  page  *page,   unsigned  int  order);

        void  free_pages(unsigned  long  addr,  unsigned  int  order);

        void  free_page(unsigned  long  addr);










四、kmalloc()

 一）、相关简介

  1、kmalloc()函数在<linux/slab.h>中声明：

        void  *  malloc(size_t  size,  gfp_t  flags);







 二）、gfp_mask标志

  1、gfp_mask标志可分为三类

    1）、行为修饰符：表示内核应当如何分配所需要的内存。在某些特定情况下，只能使用某些特定的方法分配内存。

    2）、区修饰符：表示从哪儿分配内存。

    3）、类型标志组合了行为修饰符和区修饰符，将各种可能用到的组合归纳为不同的类型，简化了修饰符的使用；如此，只需指定一个类型标志就行。




  2、行为修饰符（P192-P193  表12-3）




  3、区修饰符（P193  表12-4）



  4、不能给__get_free_pages()或kalloc()指定ZONE_HIGHEM,因为这两个函数返回的都是逻辑地址，而不是page结构，这两个函数分配的内存当前可能还没有映射到内核的

         虚拟地址空间，因此，肯恩根本没有逻辑地址。    




  5、只有alloc_pages()才能分配高端内存。




  6、类型标志

    1）、类型标志（P194  表12-5）

    2）、在每种类型标志后隐含的修饰符列表（P194  表12-6）

    3）、什么时候用那种标志（P195  表12-7）







 三）、kfree










五、vmalloc(）

  1、vmalloc(0函数的工作方式类似于kmalloc()。只不过前者分配的内存虚拟地址是连续的，而物理地址则无须连续。大多数内核代码用kmalloc()来获取内存，而不是

        vmalloc()。




  2、vmalloc()函数声明在<linux/vmalloc.h>中，定义在<mm/vmalloc.c>中：

        void  *  vmalloc(unsigned  long  size);




  3、释放通uovmalloc分配的内存：

        void  vfree(const  void  *addr);







六、slab层

 一）、相关简介

  1、slab分配器扮演了通用数据结构缓存层的角色。




  2、Linux的slab在设计和实现是考虑的规则。







 二）、slab层的设计

  1、slab层把不同的对象划分为所谓高速缓存组，其中每个高速缓存组都存放不同类型的对象，每种对象类型对应一个高速缓存。




  2、高速缓存又被划分为slab。slab由一个或多个物理上的连续的页组成，一般情况下，slab也就仅仅由一页组成。没和高速缓存可以由多个slab组成。




  3、没个slab都包含一些对象的成员，这里的对象指的是被缓存的数据结构。每个slab处于三种状态之一：满、部分满或空。




  4、高速缓存、slab以及对象之间的关系（P198  图12-1）。




  5、每个高速缓存都使用kmem_cache接口来表示。这个结构包含三个链表：slabs_full、slabs_partial和slabs_empty，均存放在描述符kmem_list3结构内，该结构在

        mm/slab.c中定义。这些链表包含告诉缓存中的所有slab。slab描述符struct  slab用来描述每个slab：

        struct  slab {

           struct  list_head  list;

           unsigned  long  colouroff;

           void  *s_mem;

           unsigned  int  inuse;

           kmem_bufctl_t  free;

        };











 三）、slab分配器的接口

  1、一个新的高速缓存通过以下的函数创建：

        struct  kmem_cache_create(const  char  *name,  sezi_t  size,

                                                             size_t  align,  unsigned  long  flags,

                                                             void  (*ctor) (noid  *));

    1）、第一个参数是一个字符串，存放着高速缓存的名字。

    2）、第二个参数是高速缓存中每个元素的大小。

    3）、第三个参数是slab内第一个对象，它用来确保在页内进行特定的对齐。

    4）、flags参数是可选的设置项，用来控制高速缓存的行为。

    5）、最后一个参数ctor是高速缓存的构造函数。




  2、要撤销一个高速缓存：

        int  kmem_cache_destroy(struct  kmem_cache  *cachep);

    1）、调用该函数之前应该满足以下条件：

        I、高速缓存中的所有的slab都必须为空。

       II、在调用kmem_eache_destroy()过程中不再访问这个高速缓存。




  3、从缓存中分配

    1）、创建一个高速缓存之后，就可以通过下列函数获取对象：

               void  *  kmem_cache_alloc(struct  kmem_cache  *cachep,  gfp_t  flags);

    2）、最后释放一个对象，并把它返回给原先的slab，可以用下列函数：

              void  kmem_cache_free(struct  kmem_cache  *cachep,  void  *objp);




  4、slab分配器实例










七、在栈上的静态分配

  1、单页内核栈

    1）、当激活这个选项时，每个进程的内核只有一页那么大，根据体系结构的不同，或为4KB，或为8KB。




  2、中断栈

    1）、中断栈为每个进程提供了一个用于中断处理程序的栈。有了这个选项，中断处理程序不用再和被中断进程共享一个内存栈，它们可以使用自己的栈。




  3、进行动态分配内存是一个明智的选择。







八、高端内存的映射

  1、永久映射

    1）、要映射一个给定的page结构到内呢地址空间，可以使用下列函数：

              void  *kmap(struct  page  *page);

     2）、当不再需要高端内存时，应该解除映射：

               void  kunmap(struct  page  *page);




  2、临时映射

    1）、可以通过以下函数建立一个临时映射：

              void  *kmap_atomic(struct  page  *page,  enum  km_type  type);

    2）、通过下列函数取消映射：

              void  kunmap_atomic(void  *kxaddr,  enum  km_type  type);







九、每个CPU的分配

  1、一般来说，每个CPU的数据存放在一个数组中。数组中的每一项都对应着系统上一个存在的处理器。

        unsigned  long  my_percpu[NR_CPUS];




  2、按如下方式访问：

        int  cpu;

        cpu = get_cpu();

        my_percpu[cpu]++;

        printk("my_percpu  on  cpu=%d  is  %lu\n",  cpu,  my_percpu[cpu]);










十、新的每个CPU接口

 一）、编译时的每个CPU数据

  1、在编译时定义每个CPU变量易如反掌：

        DEFINE_PER_CPU(type,  name);

    1）、在别处声明变量，以防止编译时警告：

        DECLARE_PER_CPU(type,  name);




  2、可以利用get_cpu_var()和put_cpu_var()例程操作变量。调用get_cpu_var()返回当前处理器上的指定变量，同时它将禁止抢占；另一方面put_cpu_var()将相应的重新激活抢

        占。 




  3、也可以用per_cpu()获得别的处理器上的每个CPU数据。







 二）、运行时的每个CPU数据


  1、内核实现每个CPU数据的动态分配方法类似于kmalloc()。该例程为系统上的每个处理器创建所需的内存实例

        void  *alloc_percpu(type);

        void  *__alloc_percpu(size_t  size,  size_t  align);

        void  free_percpu(const  void  *);







十一、使用每个CPU数据的原因

  1、减少了数据的锁定。



  2、使用每个CPU数据可以大大减少缓存失效。







十二、分配函数的选择

  1、如果需要连续的物理页，就可以使用某个低级页分配器或kmalloc()。




  2、如果想从高端内存进行分配，就是用alloc_pages()。




  3、如果不需要物理上连续的页，二仅仅需要虚拟地址上的页，就可以使用vmalloc()。




  4、如果要创建和撤销很多大的数据结构，那么考虑建立slab高速缓存。




