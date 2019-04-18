# MySQL系列：innodb源码分析之内存管理 - gauss的专栏 - CSDN博客
2015年05月10日 09:44:41[gauss](https://me.csdn.net/mathlmx)阅读数：281
个人分类：[数据库](https://blog.csdn.net/mathlmx/article/category/2641909)
                
在innodb中实现了自己的内存池系统和内存堆分配系统，在innodb的内存管理系统中，大致分为三个部分：基础的内存块分配管理、内存伙伴分配器和内存堆分配器。innodb定义和实现内存池的主要目的是提供内存的使用率和效率，防止内存碎片和内存分配跟踪和调试。我们先来看看他们的关系和结构。
以下是它的关系结构图：
![](https://img-blog.csdn.net/20141110193642750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图中的：
ut_mem_block块是基础内存管理
Buddy allocator是内存伙伴分配器
mem_heap是内存堆分配器
# 1.基础内存管理
innodb中的内存分配和内存释放是通过统一的结构进行管理，具体的实现在ut0mem.h和ut0mem.c当中，其中最重要的就是对malloc和free的封装。通过一个链表结构体来管理已经分配的内存，结构体如下：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/40985363#)[copy](http://blog.csdn.net/yuanrxdu/article/details/40985363#)
- typedef ut_mem_block_struct  
- {  
-            ulint        size;                                   /*这个被分配block的内存大小*/
-            ulint        magic_n;                                /*节点魔法字，用于校验所用*/
-            UT_LIST_NODE_T(ut_mem_block_t) mem_block_list;       /*block list node,指定prev node和next node*/
-  };  
关于block的list定义是个全局的变量，UT_LIST_BASE_NODE_T(ut_mem_block_t) ut_mem_block_list;所有分配的block都会加入到这个list当中。在ut_malloc_low函数分配内存的时候会将分配的block加入到list当中，在ut_free的时候会所释放的内存所在的block从list当中删除。除了这两个函数以外，innodb还提供ut_free_all_mem函数来释放所有分配的block和统计分配内存的总数ut_total_allocated_memory功能。
 基础内存管理的方法如下：
        ut_malloc_low                    分配一个n长度的内存块，并将分配的块记录到ut_mem_block_list当中.
        ut_malloc                            与ut_malloc_low功能相同，但是会用0初始化所分配的内存。
        ut_free                                释放一个分配的内存块，并将其从ut_mem_block_list当中删除。
        ut_free_all_mem                 释放ut_mem_block_list所有的内存块并清空ut_mem_block_list
以上函数是支持多线程并发操作的，也就是说是线程安全的。
innodb这样做的目的是保证所有malloc出去的内存都在 ut_mem_block_list当中，以便管理。
    基础内存管理的结构如下：
![](https://img-blog.csdn.net/20141110194128921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 2.伙伴分配器
 innodb的伙伴分配器是基于ut_malloc_low函数之上的内存管理器，在创建伙伴分配器时，innodb会一下用ut_malloc_low开辟一个很大的内存块，然后用伙伴分配来分配这个块的内存使用。innodb的伙伴分配器是基于2的基数为基础的管理方式，其buddy alloc pool的定义如下：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/40985363#)[copy](http://blog.csdn.net/yuanrxdu/article/details/40985363#)
- struct mem_pool_struct  
-  {     
-      byte*               buf;                          /*整体内存的句柄*/
-      ulint                size;                        /*整体内存大小*/
-      ulint                reserved;                    /*当前分配出去的总内存大小*/
-      mutex             mutex;                          /*多线程互斥量*/
-      UT_LIST_BASE_NODE_T(mem_area_t) free_list[64];    /*area_t链表数组,每个数组单元能管理2的i次方内存块列表，i是数组的下标*/
-  }；  
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/40985363#)[copy](http://blog.csdn.net/yuanrxdu/article/details/40985363#)
- struct mem_area_struct  
-  {  
-       ulint size_and_free;                                        /*area的内存大小（一定是2的次方），最后一个bit表示是否已经释放*/
-       UT_LIST_NODE_T(mem_area_t) free_list;       /*area链表的上下area,因为buddy area是会分裂的，有可能多个*/
-   };  
mem_area_t是一个buddy的内存区域，也就是mem_area_struct。以下是一个32位机器管理1024字节内存块的buddy list分布：
![](https://img-blog.csdn.net/20141110194621453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
每一个area是有mem_area_t头和可分配的内存（memory_buffer）确定的，memory_buffer的长度不小于mem_area_t头的长度，在32位机器上mem_area_t的头应该是16个字节（8字节对齐）。
## 2.1mem_area_t的分裂
  在内存分配的过程中，有可能会造成mem_area_t的分裂，还是以上面的例子来说，加入我们要分配一个200字节的内存，这时候伙伴分配器的分配流程是这样的：
        1.找到一个离200+sizeof(mem_area_t)最近的2的i次方的数（256），确定i = 8，
        2.在free_list[i]的列表中查找是否有空闲的node,如果有，将node职位no free.如果没有，对i + 1层执行查找是否有可用的内存，
        3.在上面的例子中，i+1=9,free_list是空的，继续在i+2层找，一次类推，直到找到有node的层，也就是i = 10;
        4.首先对10层进行分裂，分裂成两512大小的第9层节点，并从10删除area，在第9层加入2个512的node.
        5.然后在对第9层的第一个节点进行分裂，分裂两个大小为256的第8层节点，并从第九层删除，在第8层加入2个节点。
        6.将第一个256大小的area分配给对应的操作者，并置为no free标识。
    以下是分配了一个200字节的内存池结构:
![](https://img-blog.csdn.net/20141110194702871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果分配出去后的area_t会从free_list[i]链表中删除，也就是说在buddy上将是记录的。
## 2.2mem_area_t的合并
如果200字节分配出去后，使用完毕会归还给buddy allocator,还是拿上面的例子来说，就会发生area合并的情况,步骤如下：
    1.使用者归还伙伴分配的内存，首先会根据area_t的信息去找到自己的buddy,也就是第8层另外一个没有被分配的area.
    2.找到buddy area后，判断buddy area是否是释放状态，如果是，触发合并，将自己和buddy area从第8层删除，合并成一个512大小的第9层area,
    3.在重复1 ~ 2步，又会将自己和第九层另外一个buddy area合并成一个1024大小的第10层area.
## 2.3buddy allocator的接口函数:
    mem_pool_create                构建一个buddy allocator
    mem_area_alloc                   用buddy allocator分配一块内存
    mem_area_free                    将一块内存归还给buddy allocator
    mem_pool_get_reserved      获得buddy allocator已经使用的内存大小
# 3内存分配堆(memory heap)
innodb中的内存管理最终的体现形式是mem_heap_t内存分配与管理，所有关于内存分配的操作都会调用mem_heap的API方法，mem_heap_t的结构定义如下：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/40985363#)[copy](http://blog.csdn.net/yuanrxdu/article/details/40985363#)
- struct mem_block_info_struct  
- {  
-      ulint   magic_n;         /*魔法字*/
- char    file_name[8];    /*分配内存的文件*/
-      ulint   line;            /*分配内存的文件所在行*/
-      ulint   len;             /*block的长度*/
-      ulint   type;            /*依赖的底层分配类型，有DYNAMIC、BUFFER、BTR_SEARCH三种类型*/
-      ibool   init_block;     /*是否是外部分配的内存块*/
- 
-      ulint   free;           /*被占用的空间大小*/
-      ulint   start;          /*可分配内存的起始位置*/
-      byte*   free_block;     /*备用block,仅仅在BTR_SEARCH方式可用*/
- 
-      UT_LIST_BASE_NODE_T(mem_block_t)   base;  
-      UT_LIST_NODE_T(mem_block_t)     list;  
- };  
**备注：mem_block_info_struct/mem_block_info_t/mem_block_t/mem_heap_t是等价**
mem_heap_t的内存结构如下：
![](https://img-blog.csdn.net/20141110195309206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
关于mem_heap_t的几个要点：
    1.一个mem_block_t最小空间不小于64字节，标准的大小是8KB,在非MEM_HEAP_BUFFER模式下分配的空间不大于page size - 200（page size一般为16KB）
    2.mem_heap_t有三种类型，分别是DYNAMIC、BUFFER、BTR_SEARCH，在DYNAMIC模式下都是基于buddy allocator进行mem_block_t分配的，在BTR_SEARCH模式下，使用free_block来作为内存分配，在BUFFER模式下比较复杂，如果分配的内存大小< page size的一半时，使用buddy alloc,否则使用buf_frame的内存分配方式（这个是属于buf0buf.XX里面的方式，还未开始分析）。
    3.mem_heap_t在分配新的mem_block_t的时候一定是分配一个heap最后节点大小的两倍，如果分配的大小超过MEM_MAX_ALLOC_IN_BUF（相当于一个page size）的时候，heap 类型判断，在不是DYNAMIC模式下，最大就是一个MEM_MAX_ALLOC_IN_BUF大小。如果其他模式下就是设置成MEM_BLOCK_STANDARD_SIZE标准大小，在这些限制外，如果需要分配的内存大于这些限制，以分配内存大小为准进行mem_block_t分配。分配好的mem_block_t总是加入到heap
 base list的最后，也就是heap堆栈的顶端。
    4.mem_heap_t在释放mem_block_t时候总是从顶端开始释放，直到不能释放为止（mem_block_t没有被使用者归还）。在mem_block_t释放的时候也是需要参考DYNAMIC、BUFFER、BTR_SEARCH类型进行相对于的归还规则（和2要点是相对应的）。
**mem_heap_t函数方法说明:**
mem_heap_create                                        用DYNAMIC模式创建一个mem_heap_t
mem_heap_create_in_buffer                        用BUFFER模式创建一个mem_heap_t
mem_heap_create_in_btr_search                 用BTR_SEARCH模式创建一个mem_heap_t
mem_heap_free                                            释放mem_heap_t对象
mem_alloc                                                    创建在MEM_HEAP_DYNAMIC模式下，并分配一块指定大小的内存（在这种方式下mem_heap_t只会有一个mem_block_t）
mem_free                                                      归还mem_heap_t分配的内存，并释放mem_heap_t
mem_heap_alloc                                           在指定的mem_heap_t上分配一块内存
mem_heap_get_heap_top                            获得heap顶端块可使用内存的地址
mem_heap_empty                                        清空指定的mem_heap_t
mem_heap_get_top                                     获得heap顶部的指定n大小的mem_block_t指针
mem_heap_free_top                                    释放heap顶部N大小的mem_block_t块
# 4总结
innodb提供内存池和heap分配方式来统一管理内存，最主要的目的是提高内存的率。在MySQL-5.6的版本中，innodb提供两种选择，一种是使用innodb提供的内存池管理内存，还有一种是提供系统的malloc和free来作为内存管理。MySQL默认的是系统管理内存方式，一些有经验的DBA会使用系统的管理内存方式+TMalloc来做内存优化，借助TMalloc高效的内存管理方式实现MySQL的性能提升。
