# RamDisk块设备驱动实例开发讲解一 - h13 - 博客园
[http://armboard.taobao.com/](http://armboard.taobao.com/)
[http://blog.chinaunix.net/uid-13245160-id-84366.html](http://blog.chinaunix.net/uid-13245160-id-84366.html)
RamDisk块设备驱动实例开发讲解一 (2010-12-03 16:28)
一、开发环境
主  机：VMWare--Fedora 9 
开发板：Mini2440--64MB Nand, Kernel:2.6.30.4 
编译器：arm-linux-gcc-4.3.2
二、块设备基本概念
扇区(Sectors)：任何块设备硬件对数据处理的基本单位。通常，1个扇区的大小为512byte。 
块(Blocks)：由Linux制定对内核或文件系统等数据处理的基本单位。通常，1个块由1个或多个扇区组成。 
段(Segments)：由若干个相邻的块组成。是Linux内存管理机制中一个内存页或者内存页的一部分。 
    页、段、块、扇区之间的关系图如下
综合上描述：块设备驱动是基于扇区(sector)来访问底层物理磁盘，基于块(block)来访问上层文件系统。扇区一般是2的n次方大小，典型为512B，内核也要求块是2的n次方大小，且块大小通常为扇区大小的整数倍，并且块大小要小于页面大小，典型大小为512B、1K或4K。 
三、块设备在Linux中的结构
1, 首先我们来看一下，块设备在整个Linux中应用的总体结构，如图：
2, 块设备驱动层(Block Device Driver)在总体结构中扮演的角色。
  从上图可以看出，块设备的应用在Linux中是一个完整的子系统。
  首先，我们先看一下，块设备驱动是以何种方式对块设备进行访问的。在Linux中，驱动对块设备的输入或输出(I/O)操作，都会向块设备发出一个请求，在驱动中用request结构体描述。但对于一些磁盘设备而言请求的速度很慢，这时候内核就提供一种队列的机制把这些I/O请求添加到队列中(即：请求队列)，在驱动中用request_queue结构体描述。在向块设备提交这些请求前内核会先执行请求的合并和排序预操作，以提高访问的效率，然后再由内核中的I/O调度程序子系统(即：上图中的I/O调度层)来负责提交I/O请求，I/O调度程序将磁盘资源分配给系统中所有挂起的块I/O请求，其工作是管理块设备的请求队列，决定队列中的请求的排列顺序以及什么时候派发请求到设备，关于更多详细的I/O调度知识这里就不深加研究了。
  其次，块设备驱动又是怎样维持一个I/O请求在上层文件系统与底层物理磁盘之间的关系呢？这就是上图中通用块层(Generic Block Layer)要做的事情了。在通用块层中，通常用一个bio结构体来对应一个I/O请求，它代表了正在活动的以段(Segment)链表形式组织的块IO操作，对于它所需要的所有段又用bio_vec结构体表示。
  再次，块设备驱动又是怎样对底层物理磁盘进行反问的呢？上面讲的都是对上层的访问对上层的关系。Linux提供了一个gendisk数据结构体，用他来表示一个独立的磁盘设备或分区。在gendisk中有一个类似字符设备中file_operations的硬件操作结构指针，他就是block_device_operations结构体，他的作用相信大家已经很清楚了。
具体描述上面中讲到的维持各层关系的数据结构体(这里只列出了较常用的一些成员)
request与request_queue结构体，定义在/include/linux/blkdev.h中： struct request
{
    struct list_head queuelist;     /*链表结构*/
    struct request_queue *q;        /*请求队列*/
    sector_t sector;                /*要传送的下一个扇区*/
    sector_t hard_sector;           /*要完成的下一个扇区*/
    unsigned long nr_sectors;       /*要传送的扇区数目*/
    unsigned long hard_nr_sectors;  /*要完成的扇区数目*/
    unsigned int current_nr_sectors;/*当前要传送的扇区数目*/
    unsigned int hard_cur_sectors;  /*当前要完成的扇区数目*/
    struct bio *bio;                /*请求的bio结构体的链表*/
    struct bio *biotail;            /*请求的bio结构体的链表尾*/
    void *elevator_private;
    void *elevator_private2;
    struct gendisk *rq_disk;
    unsigned long start_time;
    unsigned short nr_phys_segments;/*请求在物理内存中占据不连续段的数目*/
    unsigned short ioprio;
    char *buffer;                   /*传送的缓冲区*/
    int tag;
    int errors;
    int ref_count;                  /*引用计数*/
    .
    .
    .
};
  struct request_queue
{
    .
    .
    .
    struct list_head queue_head;
    unsigned long nr_requests;       /*最大的请求数目*/
    unsigned int nr_congestion_on;
    unsigned int nr_congestion_off;
    unsigned int nr_batching;
    unsigned int max_sectors;        /*最大的扇区数目*/
    unsigned int max_hw_sectors;
    unsigned short max_phys_segments;/*最大的段数目*/
    unsigned short max_hw_segments;
    unsigned short hardsect_size;    /*扇区尺寸大小*/
    unsigned int max_segment_size;   /*最大的段尺寸大小*/
    unsigned long seg_boundary_mask; /*段边界掩码*/
    void *dma_drain_buffer;
    unsigned int dma_drain_size;
    unsigned int dma_pad_mask;
    unsigned int dma_alignment;      /*DMA传输内存对齐*/
    struct blk_queue_tag *queue_tags;
    struct list_head tag_busy_list;
    unsigned int nr_sorted;
    unsigned int in_flight;
    unsigned int rq_timeout;
    struct timer_list timeout;
    struct list_head timeout_list;
    .
    .
    .
};
bio与bio_vec结构体，定义在/include/linux/bio.h中： struct bio 
{
    sector_t bi_sector;            /*要传送的第一个扇区*/
    struct bio *bi_next;           /*下一个bio*/
    struct block_device *bi_bdev;
    unsigned long bi_flags;        /*状态、命令等*/
    unsigned long bi_rw;           /*低位表示READ/WRITE，高位表示优先级*/
    unsigned short bi_vcnt;        /*bio_vec的数量*/
    unsigned short bi_idx;         /*当前bvl_vec的索引*/
    unsigned int bi_phys_segments; /*不相邻物理段的数目*/
    unsigned int bi_size;          /*以字节为单位所需传送的数据大小*/
    unsigned int bi_seg_front_size;/*为了明确硬件尺寸，需要考虑bio中第一个和最后一个虚拟的可合并段的尺寸大小*/
    unsigned int bi_seg_back_size;
    unsigned int bi_max_vecs;      /*支持最大bvl_vecs的数量*/
    struct bio_vec *bi_io_vec;     /*vec列表*/
    .
    .
    .
};
struct bio_vec 
{
    struct page *bv_page;   /*页指针*/
    unsigned int bv_len;    /*传输的字节数*/
    unsigned int bv_offset; /*偏移位置*/
};
上面的这些结构体都是对上层的支持，那么对硬件底层的支持比较重要的结构体是gendisk，定义在/include/linux/genhd.h中： struct gendisk 
{
    int major;            /*主设备号*/
    int first_minor;      /*第一个次设备号*/
    int minors;           /*最大的次设备号，如果不能分区则为1*/
    char disk_name[DISK_NAME_LEN];  /*设备名称*/
    struct disk_part_tbl *part_tbl; /*磁盘上的分区表信息*/
    struct hd_struct part0;
    struct block_device_operations *fops;/*块设备对底层硬件的操作结构体指针*/
    struct request_queue *queue; /*请求队列*/
    void *private_data;          /*私有数据*/
    .
    .
    .
};
那么这些结构体之间的关系图如下：
块设备驱动的I/O请求处理的两种方式:
块设备驱动的I/O请求处理有两种方式，分别是使用请求队列和不使用请求队列。那么这两种方式有什么不同呢？在第2点中已讲到使用请求队列有助于提高系统的性能，但对于一些完全可随机访问的块设备(如：Ram盘等)使用请求队列并不能获得多大的益处，这时候，通用块层提供了一种无队列的操作模式，使用这种模式，驱动必须提供一个制造请求函数。我们还是用代码来区别它们吧。
使用请求队列： static int __int ramdisk_init(void)
{
    /*块设备驱动注册*/
    register_blkdev(RAMDISK_MAJOR, RAMDISK_NAME);
    /*使用请求队列的方式*/
    ramdisk_queue = blk_init_queue(ramdisk_do_request, NULL);
    /*分配gendisk*/
    .........
    /*初始化gendisk*/
    .........
    /*添加gendisk到系统中*/
    .........
}
/*请求处理函数，请求队列的处理流程如下：
 *首先：从请求队列中拿出一条请求
 *其次：判断这一条请求的方向，是向设备写还是读，然后将数据装入缓冲区
 *最后：通知请求完成*/
static void ramdisk_do_request(struct request_queue_t *queue)
{
    struct request *req;
    /*使用循环一条请求一条请求的来处理，elv_next_request函数是遍历队列中的每一条请求*/
    while(req = elv_next_request(queue) != NULL)
    {
        /*判断要传输数据的总长度大小是否超过范围*/
        if ((req->sector + req->current_nr_sectors) << 9 > RAMDISK_SIZE)
        {
            /*如果超过范围就直接报告请求失败*/
            end_request(req, 0);
            continue;
        }
        /*判断请求处理的方向*/
        switch (rq_data_dir(req)) 
        {
            case READ:
                memcpy(req->buffer, disk_data + (req->sector << 9), req->current_nr_sectors << 9);
                end_request(req, 1);/*报告请求处理成功*/
                break;
            case WRITE:
                memcpy(disk_data + (req->sector << 9), req->buffer, req->current_nr_sectors << 9);
                end_request(req, 1);/*报告请求处理成功*/
                break;
            default:
                break;
        }
    }
}
不使用请求队列，制造请求函数： static int __int ramdisk_init(void)
{
    /*块设备驱动注册*/
    ramdisk_major = register_blkdev(RAMDISK_MAJOR, RAMDISK_NAME);
    /*使用制造请求的方式,先分配ramdisk_queue*/
    ramdisk_queue = blk_alloc_queue(GFP_KERNEL);
    /*再绑定请求制造函数*/
    blk_queue_make_request(ramdisk_queue, &ramdisk_make_request);
    /*分配gendisk*/
    .........
    /*初始化gendisk*/
    .........
    /*添加gendisk到系统中*/
    .........
}
/*绑定请求制造函数。注意：第一个参数仍然是请求队列，但在这里实际不包含任何请求。
所以这里要处理的重点对象的bio中的每个bio_vec，他表示一个或多个要传送的缓冲区。*/
static int ramdisk_make_request(struct request_queue_t *queue, struct bio *bio)
{
    int i;
    struct bio_vec *bvec;
    void *disk_mem;
    void *bvec_mem;
    /*在遍历段之前先判断要传输数据的总长度大小是否超过范围*/
    if((bio->bi_sector << 9) + bio->bi_size > RAMDISK_SIZE)
    {
        /*如果超出范围就通知这个bio处理失败*/
        bio_endio(bio, 0, -EIO);
        return 0;
    }
    /*获得这个bio请求在块设备内存中的起始位置*/
    disk_mem = disk_data + (bio->bi_sector << 9);
    /*开始遍历这个bio中的每个bio_vec*/
    bio_for_each_segment(bvec, bio, i) 
    {
        /*因bio_vec中的内存地址是使用page *描述的，故在高端内存中需要用kmap进行映射后才能访问，
        再加上在bio_vec中的偏移位置，才是在高端物理内存中的实际位置*/
        bvec_mem = kmap(bvec->bv_page) + bvec->bv_offset;
        /*判断bio请求处理的方向*/
        switch(bio_data_dir(bio))
        {
            case READ:
            case READA:
                memcpy(bvec_mem, disk_mem, bvec-> bv_len);
                break;
            case WRITE : 
                memcpy(disk_mem, bvec_mem, bvec-> bv_len);
                break;
            default : 
                kunmap(bvec->bv_page);
        }
        /*处理完每一个bio_vec都应把kmap映射的地址取消掉*/
        kunmap(bvec->bv_page);
        /*累加当前bio_vec中的内存长度，以确定下一个bio_vec在块设备内存中的位置*/
        disk_mem += bvec->bv_len;
    }
    /*bio中所有的bio_vec处理完后报告处理结束*/
    bio_endio(bio, bio->bi_size, 0);
    return 0;
}
四、块设备驱动(RamDisk)实现步骤详解
    其实从上面的结构体关系图就可以看出这就是块设备驱动程序的整体的结构了，当然这只是较简单的块设备驱动了，现在我们就即将要做的就是实现一个简单的RamDisk块设备驱动了。其实告诉大家我这里为什么只实现一个简单的块设备驱动，因为我是要为以后的MMC/SD卡驱动、Nand flash驱动等做一些前提准备的。好了，还是先了解一下什么是RamDisk吧。
    RamDisk是将Ram中的一部分内存空间模拟成一个磁盘设备，以块设备的访问方式来访问这一片内存，达到数据存取的目的。RamDisk设备在Linux设备文件系统中对应的设备文件节点一般为：/dev/ram%d。
建立驱动代码文件my2440_ramdisk.c，实现驱动模块的加载和卸载，步骤如下：
加载部分：分配请求队列及绑定请求制造函数 -> 分配及初始化gendisk -> 添加gendisk -> 注册块设备驱动。
卸载部分：清除请求队列 -> 删除gendisk -> 注销块设备驱动。 #include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#define RAMDISK_MAJOR    0        /*主设备号设置0让内核动态产生一个主设备号*/
#define RAMDISK_NAME    "my2440_ramdisk"    /*设备名称*/
#define RAMDISK_SIZE    (4 * 1024 * 1024)   /*虚拟磁盘的大小,共4M*/
static int ramdisk_major = RAMDISK_MAJOR;   /*用来保存动态分配的主设备号*/
static struct class *ramdisk_class;         /*定义一个设备类,好在/dev下动态生成设备节点*/
static struct gendisk *my2440_ramdiak;      /*定义一个gendisk结构体用来表示一个磁盘设备*/
static struct request_queue *ramdisk_queue; /*定义磁盘设备的请求队列*/
unsigned char *disk_data;/*定义一个指针来表示ramdisk块设备在内存中的域*/
/*块设备驱动操作结构体，其实不需要做什么操作，这里就设置为空*/
static struct block_device_operations ramdisk_fops = 
{
    .owner    = THIS_MODULE,
};
static int __init ramdisk_init(void)
{
    int ret;
    /*块设备驱动注册, 注意这个块设备驱动的注册在2.6内核中是可选的，
    该函数由内核提供。这里使用是为了获得一个动态生成的主设备号*/
    ramdisk_major = register_blkdev(RAMDISK_MAJOR, RAMDISK_NAME);
    if(ramdisk_major <= 0)
    {
        return ramdisk_major;
    }
    /*动态创建一个设备节点，跟字符型设备一样*/
    ramdisk_class = class_create(THIS_MODULE, RAMDISK_NAME);
    if(IS_ERR(ramdisk_class))
    {
        ret = -1;
        goto err_class;
    }
    device_create(ramdisk_class, NULL, MKDEV(ramdisk_major, 0), NULL, RAMDISK_NAME);
    /*RamDisk属真正随机访问的设备，因此不使用请求队列的处理方式，而使用制造请求的方式*/
    ramdisk_queue = blk_alloc_queue(GFP_KERNEL);/*分配ramdisk_queue*/
    if(!ramdisk_queue)
    {
        ret = -ENOMEM;
        goto err_queue;
    }
    blk_queue_make_request(ramdisk_queue, &ramdisk_make_request);/*绑定请求制造函数*/
    /*分配gendisk，该函数由内核提供，参数为磁盘设备的次设备号数量(或者磁盘的分区数量)
    注意一个分区就代表一个次设备，这里指定数量后以后就不能被修改了*/
    my2440_ramdiak = alloc_disk(1);
    if(!my2440_ramdiak)
    {
        ret = -ENOMEM;
        goto err_alloc;
    }
    /*初始化gendisk*/
    my2440_ramdiak->major = ramdisk_major;    /*这里指定的主设备号就是在上面动态获取的主设备号*/
    my2440_ramdiak->first_minor    = 0;       /*指定第一个次设备号为0*/
    my2440_ramdiak->fops = &ramdisk_fops;     /*指定块设备驱动对底层硬件操作的结构体指针，定义在后面来讲*/
    my2440_ramdiak->queue = ramdisk_queue;    /*指定初始化好的请求队列*/
    sprintf(my2440_ramdiak->disk_name, RAMDISK_NAME);/*指定磁盘设备的名称*/
    /*设置磁盘设备的容量大小,该函数由内核提供。
    注意该函数是以512字节为1个扇区单位进行处理的，因为内核要求如此*/
    set_capacity(my2440_ramdiak, RAMDISK_SIZE >> 9);/*右移9位就是除以512*/
    /*添加gendisk到系统中, 该函数由内核提供*/
    add_disk(my2440_ramdiak);
    return 0;
/*错误处理*/
err_class:
    unregister_blkdev(ramdisk_major, RAMDISK_NAME);
err_queue:
    device_destroy(ramdisk_class, MKDEV(ramdisk_major, 0));
    class_destroy(ramdisk_class);
err_alloc:
    blk_cleanup_queue(ramdisk_queue);
    return ret;
}
static void __exit ramdisk_exit(void)
{    
    /*删除磁盘设备*/
    del_gendisk(my2440_ramdiak);
    put_disk(my2440_ramdiak);
    /*清除请求队列*/
    blk_cleanup_queue(ramdisk_queue);
    /*清除设备类*/
    device_destroy(ramdisk_class, MKDEV(ramdisk_major, 0));
    class_destroy(ramdisk_class);
    /*注销块设备*/
    unregister_blkdev(ramdisk_major, RAMDISK_NAME);
}
module_init(ramdisk_init);
module_exit(ramdisk_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huang Gang");
MODULE_DESCRIPTION("My2440 RamDisk Driver");
RamDisk属真正随机访问的设备，因此没有使用请求队列的处理方式，而是使用制造请求的方式。制造请求处理函数实现如下： /*绑定请求制造函数。注意：第一个参数仍然是请求队列，但在这里实际不包含任何请求。
所以这里要处理的重点对象的bio中的每个bio_vec，他表示一个或多个要传送的缓冲区。*/
static int ramdisk_make_request(struct request_queue_t *queue, struct bio *bio)
{
    int i;
    struct bio_vec *bvec;
    void *disk_mem;
    void *bvec_mem;
    /*在遍历段之前先判断要传输数据的总长度大小是否超过范围*/
    if((bio->bi_sector << 9) + bio->bi_size > RAMDISK_SIZE)
    {
        /*如果超出范围就通知这个bio处理失败*/
        bio_io_error(bio);
        return 0;
    }
    /*获得这个bio请求在块设备内存中的起始位置*/
    disk_mem = disk_data + (bio->bi_sector << 9);
    /*开始遍历这个bio中的每个bio_vec*/
    bio_for_each_segment(bvec, bio, i) 
    {
        /*因bio_vec中的内存地址是使用page *描述的，故在高端内存中需要用kmap进行映射后才能访问，
        再加上在bio_vec中的偏移位置，才是在高端物理内存中的实际位置*/
        bvec_mem = kmap(bvec->bv_page) + bvec->bv_offset;
        /*判断bio请求处理的方向*/
        switch(bio_data_dir(bio))
        {
            case READ:
            case READA:
                memcpy(bvec_mem, disk_mem, bvec->bv_len);
                break;
            case WRITE : 
                memcpy(disk_mem, bvec_mem, bvec->bv_len);
                break;
            default : 
                kunmap(bvec->bv_page);
        }
        /*处理完每一个bio_vec都应把kmap映射的地址取消掉*/
        kunmap(bvec->bv_page);
        /*累加当前bio_vec中的内存长度，以确定下一个bio_vec在块设备内存中的位置*/
        disk_mem += bvec->bv_len;
    }
    /*bio中所有的bio_vec处理完后报告处理结束*/
    bio_endio(bio, 0);
    return 0;
}

