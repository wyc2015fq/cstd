# 《Linux设备驱动程序》——块设备驱动程序 - 一世豁然的专栏 - CSDN博客





2014年12月05日 11:29:40[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1802标签：[Linux驱动																[kernel](https://so.csdn.net/so/search/s.do?q=kernel&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux驱动&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








                
一、概论

  1、一个块设备驱动程序主要同通过传输固定大小的随机数据来访问设备。Linux内核视块设备为与字符设备相异的基本设备类型，因此块设备驱动程序有自己完成特定任务的

      接 口。


  2、高效的块设备驱动程序在功能上是严格要求的，并不仅仅体现在用户应用程序的都、写操作中。





  3、块驱动程序是在核心内存与其他存储介质之间的管道，因此它可以认为是虚拟内存子系统的组成部分。





  4、对块设备分层的设计，其着眼点是性能。Linux块设备驱动程序接口使得块设备可以发挥其最大的功效，但是其复杂程度有时编程者必须面对的一个问题。





  5、术语


    1）、一个数据块指的是固定大小的数据，而大小的值由内核确定。数据块的大小通常是4096个字节，但是可以根据体系结构和所使用的文件系统进行改变。


    2）、与数据块对应的是扇区，它是由底层硬件决定大小的一个块。内核所处理的扇区大小是512字节。


    3）、无论何时内核为用户提供了一个扇区编号，该扇区的大小就是512字节。如果要是用不同的扇区硬件大小，用户必须对内核的扇区数进行相应的修改。





二、注册


 一）、注册块设备驱动程序


  1、对大多数块设备驱动程序来说，第一步是向内核注册自己。


        int  register_blkdev(unsigned  int  majir,  const  char  *name);


  2、与其对应的注销块设备驱动程序的函数是：


        int  unregister_blkdev(unsigned  int  majir,  const  char  *name);





 二）、注册磁盘


  1、块设备操作


    1）、块设备使用block_device_operations结构告诉系统对它们的操作接口。（P460）


    2）、在此结构中，没有负责读和写函数。在块设备的I/O系统中，这些操作由request函数处理。


  2、gendisk结构


    1）、内核使用gendisk结构来表示一个独立的磁盘设备。实际上，内核还使用gendisk结构表示分区。


    2）、gendisk结构中的许多成员必须由驱动程序初始化。（P461）


    3）、gendisk结构是一个动态分配的结构，它需要一些内核的特殊处理来进行初始化；驱动程序不能自己动态分配该结构：


              struct  gendisk  *alloc_disk(int  minors);


    4）、当不需要一个磁盘时，调用下面的函数卸载磁盘：


              void  del_gendisk(struct  fendisk  *gd);


    5）、gendisk是一个引用计数结构。get_disk和put_disk函数负责处理引用计数，但是驱动程序不能直接使用这两个函数。通常对del_gendisk的调用会删除gendisk中的最


               终计数，但是没有机制能保证其肯定发生。


    6）、分配一个gendisk结构并不能使磁盘对系统可用。为达到这个目的，必须初始化结构并调用add_disk：


              void  add_disk(struct  gendisk  *gd);





 三）、sbull中的初始化（书P462-P464 sbull 模块的代码）





 四）、对扇区带小奥的说明


  1、内核认为每个磁盘都是512字节大小的扇区所组成的线形数组。但并不是所有的硬件都使用这个扇区大小。让一个具有不同的扇区大小的设备运行起来并不是特别困难，只


        是仔细关系许多细节。


  2、所有操作的第一步就是通知内核设备所支持的扇区大小。硬件扇区大小作为一个参数放在请求队列中，而不是放在fendisk结构中。当分配队列之后，立即调用


        blk_queue_hardsect_size设置扇区大小：


        blk_queue_hardsect_size(dev->queue,  hardsect_size);


  3、所有的I/O请求都将定位在硬件扇区的开始位置，并且每个请求的大小都将是扇区大小的整数倍。必须记住，内核总是认为扇区大小是512字节，因此必须将所有的扇区数进


        行转换。










三、块设备操作


 一）、open和release函数


  1、sbull模块中的open函数。


  2、sbull模块中的release函数。





 二）、对可移动介质的支持


  1、block_disk_change结构中包含了两个函数用以支持移动介质。如果是为非移动设备编写驱动程序，则可以忽略这两个函数。





  2、（从check_disk_change函数中）调用media_changed函数以检查介质是否被改变；如果被改变该函数将返回非零值。


    1）、sbull中的change实现。





  3、在介质改变后将调用revalideate函数；为了让驱动程序能操作新的介质，该函数要完成所有必须的工作。调用revalideate函数后，内核将试着重新读取设备的分区表。


    1）、sbull中的revalideate函数实现。





 三）、ioctl函数（书P466代码）










四、请求处理

 一）、request函数介绍

  1、块设备驱动程序的request函数有以下原型：

        void  request(request_queue_t  *queue);

    1）、当内核需要驱动程序处理读取、写入以及其他对设备的操作时，就会调用该函数。

    2）、在其返回前，request函数不必完成所有在队列中的请求；实际上，对大多数真实设备而言，它可能没有完成任何请求。然而它必须启动对请求的响应，并且保证所有的

              请求最终被驱动程序所处理。




  2、每个设备都有一个请求队列。当请求队列生成的时候，request函数就与该队列绑定在一起。







 二）、一个简单的request函数

  1、sbull中sbull_request函数。

    1）、内核提供了函数elv_next_request用来获得队列中第一个未完成的请求；当没有请求需要处理时，该函数就返回NULL。注意elv_next_request并不从队列中删除请求。

              如果不加以干涉而两次调用该函数，则两次都返回相同的request结构。

    2）、block_fs_request调用告诉用户该请求是否是一个文件系统请求——移动块数据的请求。如果不是文件系统请求，则将其传递给end_request:

              void  end_request(struct  request  *req,  int  succeeded);

    3）、当处理非文件系统请求时，传递0表示不能成功地完成该请求，否则调用sbull_transfer对数据进行实际上的移动。该函数使用了request结构中的诸多成员。




  2、request结构中的成员：

    1）、sector_t  sector：在设备上开始扇区的索引号。

    2）、unsigned  long  nr_sectors：需要传递扇区（512字节）数。

    3）、char  *buffer：要传递或者接收数据的缓冲区指针。

    4）、rq_data_dir(struct  request  *req)：此宏从request中得到传输方向。




  3、sbull_transfe函数代码及其解析。







 三）、请求队列

  1、从简单的直觉上讲，一个块设备请求队列可以这样描述：包含块设备I/O请求的序列。

    1）、请求队列跟踪未完成的块设备的I/O请求。

    2）、请求队列还实现了插件接口，使得多个I/O调度器的使用称为可能。




  2、请求队列拥有request_queue和request_queue_t结构类型。早<linux/blkdev.h>中定义了该结构及操作该结构的函数。




  3、队列的创建与删除

    1）、创建和初始化请求队列的函数是：

              request_queue_t  *blk_init_queue(request_fn_proc,  spinlock_t  *lock);

    2）、为了把请求队列返回给系统，需要调用以下函数


              void  blk_cleanup_queue(reque_queue_t  *);




  4、队列函数

    1）、返回队列中下一个要处理的请求的函数是：

              struct  requeuest  *elv_next_request(request_queue_t  *queue);

    2）、将请求从队列中实际删除，使用以下函数：

              void  blkdev_dequeue_request(struct  request  *req);

    3）、如果出于某些原因要将拿出队列的请求在返回给队列，使用下面的函数：

             void  elv_requeue_request(request_queue_t  *queue,  struct  request  *req);




  5、队列控制函数(P473-P474)








 四）、请求过程剖析

  1、每个request结构都表示了一个块设备的I/O请求。从本质上讲，一个request结构是作为一个bio结构的链表而实现的。当然是依靠一些管理信息来组合的，这样保证在执行

        请求的时候，驱动程序能知道执行的位置。




  1、bio结构

    1）、当内核以文件系统、虚拟内存子系统或者系统调用的形式决定从块I/O设备输入、输出块数据时它将再结合一个bio结构，用来描述这个操作。

    2）、bio结构被传递给I/O代码，代码会把它合并到一个已经存在的request结构中，或者根据需要，在创建一个新的request结构。

    3）、bio结构包含了驱动程序执行请求的全部信息，而不必于初始化这个请求的用户空间的进程相关联。

    4）、bio结构成员。(P475)




  3、为了让内核开发者能在未来修改bio结构，而又不用重新编写驱动程序代码，并不推荐直接使用bi_io_vec。因此在使用bio结构的时候，提供了一套宏来简化工作过程。在

        使用bio来操作每个段的开始阶段，他只是简单地在bi_io_vec数组中遍历每个没有被处理的入口，使用下面的宏：

       int  segno;

       struct  bio_vec  *bvec;

       bio_for_each_segment(bvec,  bio,  segno) {

              /*使用该段进行一定的操作*/

       }；




  4、如果需要直接访问页，需要首先保证正确的内核虚拟地址是存在的。为达此目的，可以使用：

       char  *__bio_kmap_atomic(struct  bio  *bio,  int  i,  enum  km_type  type);

       void  __bio_kunmap_atomic(char  *buffer,  enum  km_type  type);




  5、为了追踪正在处理的请求的当前状态，块设备层也在bio结构中维护了一系列指针。提供了一些宏可访问当前状态：

    1）、struct  page  *bio_page(struct  bio  *bio);

           返回下指向一个传输页的page结构指针。

    2）、int  bio_offset(struct  bio  *bio);

           返回在页中被传输数据的偏移量。

    3）、int  bio_cur_sectors(struct  bio  *bio);

           返回要在当前页中传输的扇区数量。

    4）、char  *bio_data(struct  bio  *bio);

           返回指向被传输数据的内核逻辑地址。

    5）、char  *bio_kmap_irq(struct  bio  *bio,  unsigned  long  *flags);

              void  bio_kunmap_irq(char  *buffer,  unsigned  long  *flags);




  6、request结构成员（P477-P478）




  7、屏障请求

    1）、2.6内核提供了屏障来解决无限制重新组合面临的问题。

    2）、如果驱动程序要实现屏障请求，所要做的第一步是将这一特性通知块设备层。屏障操作是另外一个请求队列；用下面的函数设置：

           void  blk_queue_ordered(request_queue_t  *queue,  int  flags);

    3）、屏障的请求实现只是检查request结构中的相关标志，为此，内核提供了一个宏完成这个工作：

           int  blk_barrier_rq(struct  request  *req);




  8、不可重试请求

    1）、如果驱动程序要重试一个失败的请求，首先它要调用：

           int  blk_noretry_request(struct  request  *req);








 五）、请求完成函数

  1、当设备完成一个I/O请求的部分或者全部的扇区时，它必须调用下面的函数通知块设备子系统：

        int  end_that_request_first(struct  request  *req,  int  sucess,  int  count);




  2、end_that_request_first的返回值表明该请求中的所有扇区是否被传输。如果返回值是0表示所以得扇区都被传输了，该请求执行完毕。此时必须调用

        blkdev_dequeue_request函数删除请求（如果还没有做这步），并把其传递给：

        end_that_request_last(struct  request  *req);




  3、使用bio

    1）、sbull_full_request函数。

    2）、sbull_xfer_request函数。

    3）、sbull_xfer_bio函数。




  4、块设备请求和DMA

    1）、如果读者的设备可以完成“分散/聚集”I/O，有一个更简单的实现方式。函数：

              int  blk_rq_map_sg(request_queue_t  *queue,  struct  request  *req,  struct  scatterlist  *list);

    2）、如果不想让blk_rq_map_sg合并相邻的段，可以使用下面的函数改变这个默认的行为：

              clear_bit(QUEUE_FLAGS_CLUSTER,  &queue->queue_flags);




  5、不使用请求队列

    1）、为了能使用“无序列”模式，驱动程序必须提供一个“构造请求”的函数，而不是一个请求处理函数。下面式构造请求处理的原型。

              typedef  int  (make_request_fn)  (request_queue_t  *q,  struct  bio  *bio);

    2）、直接进行传输是利用前面介绍过的方法，通过bio传输。由于没有request结构进行操作，因此函数应该够调用bio_endio,告诉bio结构的创建者请求的完成情况：

              void  bio_endio(struct  bio  *bio,  unsigned  int  byte,  int  error);

    3）、另外还必须告诉设备子系统，驱动程序使用定制的make_request函数。为做到这点，必须使用下面的函数分配一个请求队列：

              request_queue_t  *blk_alloc_queue(int  flags);

    4）、一旦拥有了队列，将blk_alloc_queue与make_request函数传递给下列函数：

              void  blk_queue_make_request(request_queue_t  *queue,  make_requeue_fn  *func);










五、其他一些细节

 一）、命令预处理

  1、块设备层为驱动程序返回elv_next_request前，提供了检查和预处理请求的机制，该机制允许驱动程序预先建立驱动器命令，决定是否处理该请求，还是以其他方式的处

        理。




  2、如果想使用这个功能，要按照下面原型捡来命令处理函数：

        type  int  (prep_rq_fn)  (request_queue_t  *queue,  struct  request  *req);




  3、为了让块设备层调用预处理函数，将其传递给：

        void  blk_queue_prep_rq(request_queue_t  *queue,  prep_rq_fn  *func);







 二）、标记命令列

  1、同时拥有多个活动请求的硬件通常支持某种形式的标记命令队列（TCQ）。TCQ只是为每个请求添加一个整数（标记）技术，这样当驱动器完成它们中的一个请求后，它

         就可以告诉驱动程序完成的是那个。




  2、为了能让驱动器支持标记命令队列，必须在初始化的时候调用下列的函数告诉内核：

        int  blk_queue_init_tags(request_queue_t  *queue,  int  depth,  struct  blk_queue_tags  *tags);




  3、如果设备能处理的标记数量发生了变化，可以用下面的函数通知内核：

        int  blk_queue_resize_tags(request_queue_t  *queue,  int  new_depth);




  4、使用一下函数将一个标记与一个请求相关联，调用该函数时必须锁住队列锁：

        int  blk_queue_start_tag(request_queue_t  *queue,  struct  request  *req);




  5、当一个指定请求的全部数据传输完毕后，驱动程序使用下面的函数返回标记：

        void  blk_queue_end_tag(request_queue_t  *queue,  struct  request  *req);




  6、如果要为一个指定的标记找到相应的请求，使用blk_queue_find_tag函数：

       struct  requst  *blk_queue_find_tag(request_queue_t  *queue,  int  tag);




  7、如果发生了错误，驱动程序可能不得不重新置位，或者执行其他一些操作对其进行控制的设备强制纠错。在这种情况下，任何未完成标记的命令都不能被执行。块设备层提

        供了一个函数，可以在这种情况下帮助重复：

       void  blk_queue_invalidata_tags(request_queue_t  *queue);





