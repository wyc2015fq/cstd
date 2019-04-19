# 乾坤合一：Linux设备驱动之块设备驱动 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
1. 题外话
在蜕变成蝶的一系列学习当中，我们已经掌握了大部分Linux驱动的知识，在乾坤合一的分享当中，以综合实例为主要讲解，在一个月的蜕茧成蝶的学习探索当中，觉得数据结构，指针，链表等等占据了代码的大部分框架，这些都需要我们平时多看代码，并且在相关知识点的时候需要在电脑上进行操作，这也让自己受益匪浅，笔者在这期间受到了几家IT学院的邀请录制视频，当兼职布道师。但毕竟自己还是个学生，应该潜心学习，争取更好的做一个IT的人才，所以都没有接受，这里很抱歉，并且会更加努力，好好钻研，希望和大家一起共同进步~
2. 块设备与字符设备I/O口操作异同
2.1 块设备只能以块为单位接受输入和返回输出，而字符设备则以字节为单位。大多数设备是字符设备，因为它们不需要缓冲而且不以固定块大小进行操作。
2.2 块设备对于I/O 请求有对应的缓冲区，因此它们可以选择以什么顺序进行响应，字符设备无须缓冲且被直接读写。对于存储设备而言调 读写的顺序作用巨大，因为在读写连续的扇区比分离的扇区更快。
2.3 字符设备只能被顺序读写，而块设备可以随机访问。虽然块设备可随机访问，但是对于磁盘这类机械设备而言，顺序地组织块设备的访问可以提高性能。
3. 块设备驱动结构
3.1 block_device_operations 结构体

C
```
struct block device operations
 
      {
 
        int (*open)(struct inode *, struct file*);  //打开
 
        int (*release)(struct inode *, struct file*);  //释放
　　　　//与字符设备驱动类似，当设备被打开和关闭时将调用它们。
 
        int (*ioctl)(struct inode *,struct file *,unsigned,unsigned long); //ioctl
 
　　　 // ioctl()系统调用的实现，块设备包含大量的标准请求，这些标准请求由Linux 块设备层处理
        long (*unlocked ioctl)(struct file *, unsigned, unsigned long);
 
        long (*compat ioctl)(struct file *, unsigned, unsigned long);
 
         int (*direct access)(struct block device *, sector t, unsigned long*);
 
        int (*media changed)(struct gendisk*);  //介质被改变？
　　　　 //被内核调用来检查是否驱动器中的介质已经改变，如果是，则返回一个非0 值，否则返回0
 
       int (*revalidate disk)(struct gendisk*);  //使介质有效
　　　 　//revalidate_disk()函数被调用来响应一个介质改变，它给驱动一个机会来进行必要的工作以使新介质准备好。
 
       int (*getgeo)(struct block device *, struct hd geometry*);//填充驱动器信息
　　　　//根据驱动器的几何信息填充一个hd_geometry 结构体
 
       struct module *owner; //模块拥有者
　　　　// 一个指向拥有这个结构体的模块的指针，它通常被初始化为THIS_MODULE
 
     };
```
3.2 gendisk 结构体

C
```
struct gendisk
 
      {
 
        int major; /* 主设备号 */
 
              
        int first minor;  /*第1个次设备号*/
 
        int minors; /* 最大的次设备数，如果不能分区，则为1*/
 
             
        char disk name [32]; /* 设备名称 */
                
        struct hd struct **part; /* 磁盘上的分区信息 */
               
        struct block device operations *fops; /*块设备操作结构体*/
                    
        struct request queue *queue;  /*请求队列*/
               
       void *private data;  /*私有数据*/
      
       sector t capacity; /*扇区数，512 字节为1个扇区*/
       
       int flags;
              
       char devfs name[64];
       int number;
 
       struct device *driverfs dev;
       struct kobject kobj;
       
 
       struct timer rand state *random;
       int policy;
       
 
       atomic t sync io; /* RAID */
       unsigned long stamp;
              
       int in flight;
                    
       #ifdef CONFIG SMP
                      
         struct disk stats *dkstats;
       #else
                       
         struct disk stats dkstats;
       #endif
     };
```
3.3 gendisk的操作

C
```
//分配gendisk
struct gendisk *alloc disk (int minors);
 
// 增加gendisk
 void add disk(struct gendisk *gd);
 
// 释放gendisk
 void del gendisk (struct gendisk *gd);
 
//gendisk 引用计数
 
// 设置gendisk 容量
 void set capacity (struct gendisk *disk, sector t size);
```
3.4 request 与bio 结构体
1) 请求
在Linux 块设备驱动中，使用request 结构体来表征等待进行的I/O 请求，request 结构体的主要成员包括(只用于内核块设备层):

C
```
sector t hard sector;　　//第一个尚未传输的扇区
 
unsigned long hard nr sectors;　　//尚待完成的扇区数
 
unsigned int hard cur sectors;　　//当前I/O 操作中待完成的扇区数
```
2) 请求队列
一个块请求队列是一个块I/O 请求的队列，请求队列跟踪的块I/O 请求，它存储用于描述这个设备能够支持的请求的类型信息、它们的最大大小、多少不同的段可进入一个请求、硬件扇区大小、对齐要求等参数，其结果是：如果请求队列被配置正确了，它不会交给该设备一个不能处理的请求。

C
```
//request 队列结构体
 
     struct request queue
 
      {
 
        ...
 
        /* 保护队列结构体的自旋锁 */
 
        spinlock t    queue lock;
 
        spinlock t *queue lock;
 
        /* 队列kobject */
 
        struct kobject kobj;
 
       /* 队列设置 */
 
       unsigned long nr requests; /* 最大的请求数量 */
 
       unsigned int nr congestion on;
 
       unsigned int nr congestion off;
 
       unsigned int nr batching;
 
       unsigned short max sectors;  /* 最大的扇区数 */
 
       unsigned short max hw sectors;
 
       unsigned short max phys segments; /* 最大的段数 */
 
       unsigned short max hw segments;
 
       unsigned short hardsect size;  /* 硬件扇区尺寸 */
 
       unsigned int max segment size;  /* 最大的段尺寸 */
 
       unsigned long seg boundary mask; /* 段边界掩码 */
 
       unsigned int dma alignment;  /* DMA 传送的内存对齐限制 */
 
       struct blk queue tag *queue tags;
 
       atomic t refcnt; /* 引用计数 */
 
       unsigned int in flight;
 
       unsigned int sg timeout;
                    
       unsigned int sg reserved size;
 
       int node;
 
       struct list head drain list;
 
       struct request *flush rq;
 
       unsigned char ordered;
 
     };
```
3) 块I/O
通常一个bio 对应一个I/O 请求，一个请求可以包含多个bio。

C
```
struct bio
      {
         
        sector t bi sector; /* 要传输的第一个扇区 */
         //标识这个 bio  要传送的第一个 （512 字节）扇区。
        struct bio *bi next; /* 下一个bio */
              
        struct block device     *bi bdev;
             
        unsigned long bi flags; /* 状态、命令等 */
                           
        unsigned long bi rw; /* 低位表示READ/WRITE，高位表示优先级*/
 
        unsigned short bi vcnt; /* bio vec 数量 */
                 
        unsigned short bi idx; /* 当前bvl vec 索引 */
   
       /*不相邻的物理段的数目*/
                         
       unsigned short bi phys segments;
       
       /*物理合并和DMA remap合并后不相邻的物理段的数目*/
               
       unsigned short bi hw segments;
 
       unsigned int bi size; /* 以字节为单位所需传输的数据大小 */
 
　　//被传送的数据大小，以字节为单位，驱动中可以使用bio_sectors(bio)宏获得以扇区为单位的大小。
       /* 为了明了最大的hw 尺寸，我们考虑这个bio 中第一个和最后一个虚拟的可合并的段的尺寸 */
               
       unsigned int bi hw front size;
                    
       unsigned int bi hw back size;
                      
       unsigned int bi max vecs; /* 我们能持有的最大bvl vecs 数 */
 
       struct bio vec *bi io vec; /* 实际的vec 列表 */
 
        bio end io t *bi end io;
    
       atomic t bi cnt;
 
       void *bi private;
 
       bio destructor t *bi destructor; /* destructor */
    };
```
3.5  块设备驱动注册与注销
首先注册她们自己到内核，其函数原型如下

C
```
int register blkdev (unsigned int major, const char *name);// major参数是块设备要使用的主设备号，name为设备名
```
与register_blkdev()对应的注销函数是unregister_blkdev()，其原型为：

C
```
int unregister blkdev (unsigned int major, const char *name);
// 传递给register_blkdev() 的参数必须与传递给register_blkdev() 的参数匹配，否则这个函数返回-EINVAL
```
4 Linux 块设备驱动的模块加载与卸载
4.1 需要完成的工作
- 分配、初始化请求队列，绑定请求队列和请求函数。
- 分配、初始化gendisk，给gendisk 的maj or、fops 、queue 等成员赋值，最后添加gendisk。
- 注册块设备驱动。
4.2 块设备驱动的模块加载函数模板 （使用bl k_a llo c_que ue )

C
```
static int    init xxx init (void)
      {
        //分配gendisk
       
        xxx disks = alloc disk (1);
              
        if (!xxx disks)
        {
          goto out;
        }
       
       //块设备驱动注册
                  
       if (register blkdev (XXX MAJOR, "xxx"))
        {
         err =  - EIO;
         goto out;
        }
       
       // “请求队列”分配
           
       xxx queue = blk alloc queue (GFP KERNEL);
            
       if (!xxx queue)
        {
                
         goto out queue;
        }
 
       blk queue make request(xxx queue, &xxx make request); //绑定“制造请求”函数
        
       blk queue hardsect size (xxx queue, xxx blocksize); //硬件扇区尺寸设置
       
       //gendisk初始化
 
  　　  xxx disks->major = XXX MAJOR;
             
        xxx disks->first minor = 0;
      
        xxx disks->fops = &xxx op;
           
        xxx disks->queue = xxx queue;
           
        sprintf(xxx disks->disk name, "xxx%d", i);
           
        set capacity (xxx disks, xxx size); //xxx size 以512bytes 为单位
              
        add disk (xxx disks); //添加gendisk
       
        return 0;
           
        out queue: unregister blkdev (XXX MAJOR, "xxx");
                  
        out: put disk(xxx disks);
           
        blk cleanup queue (xxx queue);
       
        return  - ENOMEM;
 
      }
```
4.3 块设备驱动的模块加载函数模板（使用bl k_ i nit_queue ）

C
```
static int     init xxx init (void)
 
       {
         //块设备驱动注册
               
        if (register blkdev (XXX MAJOR, "xxx"))
         {
 
          err =  - EIO;
 
          goto out;
 
         }
       
     //请求队列初始化
           
        xxx queue = blk init queue (xxx request, xxx lock);
             
        if (!xxx queue)
        {
           
         goto out queue;
 
        }
 
        blk queue hardsect size (xxx queue, xxx blocksize); //硬件扇区尺寸 设置
       
        //gendisk初始化
          
        xxx disks->major = XXX MAJOR;
            
        xxx disks->first minor = 0;
             
        xxx disks->fops = &xxx op;
       
        xxx disks->queue = xxx queue;
                     
        sprintf(xxx disks->disk name, "xxx%d", i);
           
        set capacity (xxx disks, xxx size *2);
           
        add disk (xxx disks); //添加gendisk
       
        return 0;
          
        out queue: unregister blkdev (XXX MAJOR, "xxx");
             
        out: put disk(xxx disks);
           
        blk cleanup queue (xxx queue);
       
        return  - ENOMEM;
 
      }
```
4.4 在块设备的open()函数中赋值private_data

C
```
static int xxx open (struct inode *inode, struct file *filp)
     {
               
       struct xxx dev *dev = inode->i bdev->bd disk->private data;
             
       filp->private data = dev;  //赋值file 的private data
       ...
       return 0;
 
     }
```
5 块设备的I/O请求处理
5.1 使用求情队列
块设备驱动请求函数的原型为：

C
```
void request (request queue t *queue);//请求函数可以在没有完成请求队列中的所有请求的情况下返回，甚至它一个请求不完成都可以返回
```
下面给出了一个更复杂的请求函数，它进行了3 层遍历：遍历请求队 列中的每个请求，遍历请求中的每个bio，遍历bio 中的每个段。请求函数遍历请求、bio 和段如下：

C
```
static void xxx full request (request queue t *q)
    {
     struct request *req;
                    
      int sectors xferred;
            
      struct xxx dev *dev = q->queuedata;
      /* 遍历每个请求 */
         
      while ((req = elv next request(q)) != NULL)
      {
         
        if (!blk fs request (req))
       {
 
      printk (KERN NOTICE "Skip non-fs request\n");
     
         end request (req, 0);
         continue;
       }
              
       sectors xferred = xxx xfer request (dev, req);
        
       if (!end that request first (req, 1, sectors xferred))
       {
           
         blkdev dequeue request (req);
          
         end that request last (req);
       }
      }
   }
   /* 请求处理 */
        
   static int xxx xfer request (struct xxx dev *dev,struct request *req)
   {
     struct bio *bio;
     int nsect = 0;
     /* 遍历请求中的每个bio */
      
     rq for each bio (bio, req)
      {
         
       xxx xfer bio (dev, bio);
                      
       nsect += bio->bi size / KERNEL SECTOR SIZE;
      }
     return nsect;
   }
   /* bio 处理 */
        
   static int xxx xfer bio (struct xxx dev *dev, struct bio *bio)
   {
     int i;
                  
     struct bio vec *bvec;
          
     sector t sector = bio->bi sector;
     
     /* 遍历每一段 */
        
     bio for each segment(bvec, bio, i)
      {
            
       char *buffer =     bio kmap atomic(bio, i, KM USER0);
      
           xxx transfer(dev,  sector,  bio cur sectors(bio),  buffer, bio data dir (bio)  == WRITE);
 
    sector += bio cur sectors(bio);
          bio kunmap atomic (bio, KM USER0);
 
      }
 
     return 0;
 
   }
```
5.2 不适用请求队列
有些设备不需要使用请求队列，其函数原型如下：

C
```
typedef int (make request fn) (request queue t *q, struct bio *bio);
//bio 结构体表示一个或多个要传送的缓冲区
```
在处理处理bio完成后应该使用bio_endio()函数通知处理结束，如下所示：

C
```
void bio endio (struct bio *bio, unsigned int bytes, int error);
//参数bytes 是已经传送的字节数，它可以比这个bio 所代表的字节数少
```
不管对应的I/O 处理成功与否，“制造请求”函数都应该返回0 。如果“制造请求” 函数返回一个非零值，bio 将被再次提交。下面代码所示为一个 “制造请求”函数的例子。

C
```
static int xxx make request (request queue t *q, struct bio *bio)
     {
          
       struct xxx dev *dev = q->queuedata;
       int status;
                     
       status = xxx xfer bio (dev, bio); //处理bio
         
       bio endio (bio, bio->bi size, status); //通告结束
       return 0; 
     }
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
