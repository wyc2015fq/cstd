# 蜕变成蝶：Linux设备驱动之DMA - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
DMA概述
DMA是一种无需CPU的参加就可以让外设与系统内存之间进行双向数据传输的硬件机制。它可以使系统CPU从实际的I/O数据传输过程中摆脱出来，大大提高系统的吞吐率，并且在传输期间，CPU还可以并发执行其他任务。
DMA与cache的一致性
cache用作CPU针对内存的缓存，避免CPU每一次都要与相对来说慢点的内存交互数据，从而来提高数据的访问速率，而DMA可以用作内存与外设之间传输数据的方式，数据不需要经过CPU周转。
“假设设备驱动程序把一些数据填充到内存缓冲区中，然后立刻命令硬件设备利用DMA传送方式读取该数据。如果DMA访问这些物理RAM内存单元，而相应的硬件高速缓存行的内容还没有写入RAM中，那么硬件设备所读取的至就是内存缓冲区中的旧值。”
现在有两种方法来处理DMA缓冲区：
一致性DMA映射：
书上讲的比较抽象，通俗地所就是任何对DMA缓冲区的改写都会直接更新到内存中，也称之为“同步的”或者“一致的”。
流式DMA映射：
根据个人的理解，这里的流即输入输出流，我们需要事先指定DMA缓冲区的方向。
启动一次流式DMA数据传输分为如下步骤(本DMA驱动开发介绍仅适合S3C2410处理器类型):
1. 分配DMA缓冲区
在DMA设备不采用S/G（分散/聚集）模式的情况下，必须保证缓冲区是物理上连续的，linux内核有两个函数用来分配连续的内存：kmalloc()和__get_free_pages()。这两个函数都有分配连续内存的最大值，kmalloc以分配字节为单位，最大约为64KB，__get_free_pages()以分配页为单位，最大能分配2^order数目的页，order参数的最大值由include/linux/Mmzone.h文件中的MAX_ORDER宏决定（在默认的2.6.18内核版本中，该宏定义为10。也就是说在理论上__get_free_pages函数一次最多能申请1<<10 * 4KB也就是4MB的连续物理内存，在Xilinx Zynq Linux内核中，该宏定义为11）。
2. 建立流式映射
在对DMA冲区进行读写访问之后，且在启动DMA设备传输之前，启用dma_map_single()函数建立流式DMA映射，这两个函数接受缓冲区的线性地址作为其参数并返回相应的总线地址。
3. 释放流式映射
当DMA传输结束之后我们需要释放该映射，这时调用dma_unmap_single()函数。
注意：
(1). 为了避免高速缓存一致性问题，驱动程序在开始从RAM到设备的DMA数据传输之前，如果有必要，应该调用dma_sync_single_for_device()函数刷新与DMA缓冲区对应的高速缓存行。
(2). 从设备到RAM的一次DMA数据传送完成之前设备驱动程序是不可以访问内存缓冲区的，但如果有必要的话，驱动程序在读缓冲区之前，应该调用dma_sync_single_for_cpu()函数使相应的硬件高速缓存行无效。
(3). 虽然kmalloc底层也是用__get_free_pages实现的，不过kmalloc对应的释放缓冲区函数为kfree，而__get_free_pages对应的释放缓冲区函数为free_pages。具体与__get_free_pages有关系的几个申请与释放函数如下：
申请函数：

C
```
alloc_pages(gfp_mask,order)
//返回第一个所分配页框描述符的地址，或者如果分配失败则返回NULL。
__get_free_pages(gfp_mask,order)
//类似于alloc_pages()，但它返回第一个所分配页的线性地址。如果需要获得线性地址对应的页框号，那么需要调用virt_to_page(addr)宏产生线性地址。
```
释放函数：

C
```
__free_pages(page,order)
//这里主要强调page是要释放缓冲区的线性首地址所在的页框号
free_pages(page,order)
//这个函数类似于__free_pages(page,order)，但是它接收的参数为要释放的第一个页框的线性地址addr
```
DMA驱动主要数据结构：
1）DMA单个内核缓冲区数据结构：

C
```
typedef struct dma_buf_s {
int size;    /* buffer size：缓冲大小 */
dma_addr_t dma_start;    /* starting DMA address ：缓冲区起始物理地址*/
int ref;    /* number of DMA references 缓冲区起始虚拟地址*/
void *id;    /* to identify buffer from outside 标记 */
int write;    /* 1: buf to write , 0: buf to read DMA读还是写*/
struct dma_buf_s *next;    /* next buf to process 指向下一个缓冲区结构*/
} dma_buf_t;
```
2）DMA寄存器数据结构：

C
```
/* DMA control register structure */
typedef struct {
volatile u_long DISRC;/源地址寄存器
volatile u_long DISRCC;//源控制寄存器
volatile u_long DIDST;//目的寄存器
volatile u_long DIDSTC;//目的控制寄存器
volatile u_long DCON;//DMA控制寄存器
volatile u_long DSTAT;//状态寄存器
volatile u_long DCSRC;//当前源
volatile u_long DCDST;//当前目的
volatile u_long DMASKTRIG;//触发掩码寄存器
} dma_regs_t;
```
3）DMA设备数据结构

C
```
/* DMA device structre */
typedef struct {
dma_callback_t callback;//DMA操作完成后的回调函数，在中断处理例程中调用
u_long dst;//目的寄存器内容
u_long src;//源寄存器内容
u_long ctl;//此设备的控制寄存器内容
u_long dst_ctl;//目的控制寄存器内容
u_long src_ctl;//源控制寄存器内容
} dma_device_t;
```
4）DMA通道数据结构

C
```
/* DMA channel structure */
typedef struct {
dmach_t channel;//通道号：可为0，1，2，3
unsigned int in_use;    /* Device is allocated 设备是否已*/
const char *device_id;    /* Device name 设备名*/
dma_buf_t *head;    /* where to insert buffers 该DMA通道缓冲区链表头*/
dma_buf_t *tail;    /* where to remove buffers该DMA通道缓冲区链表尾*/
dma_buf_t *curr;    /* buffer currently DMA'ed该DMA通道缓冲区链表中的当前缓冲区*/
unsigned long queue_count;    /* number of buffers in the queue 链表中缓冲区个数*/
int active;    /* 1 if DMA is actually processing data 该通道是否已经在使用*/
dma_regs_t *regs;    /* points to appropriate DMA registers 该通道使用的DMA控制寄存器*/
int irq;    /* IRQ used by the channel //通道申请的中断号*/
dma_device_t write;    /* to write //执行读操作的DMA设备*/
dma_device_t read;    /* to read 执行写操作的DMA设备*/
} s3c2410_dma_t;
```
DMA驱动主要函数功能分析:
写一个DMA驱动的主要工作包括：DMA通道申请、DMA中断申请、控制寄存器设置、挂入DMA等待队列、清除DMA中断、释放DMA通道.

C
```
int s3c2410_request_dma(const char *device_id, dmach_t channel,
dma_callback_t write_cb, dma_callback_t read_cb) (s3c2410_dma_queue_buffer);
```
函数描述：申请某通道的DMA资源，填充s3c2410_dma_t 数据结构的内容，申请DMA中断。
输入参数：device_id DMA 设备名；channel 通道号；
write_cb DMA写操作完成的回调函数；read_cb DMA读操作完成的回调函数
输出参数：若channel通道已使用，出错返回；否则，返回0

C
```
int s3c2410_dma_queue_buffer(dmach_t channel, void *buf_id,
dma_addr_t data, int size, int write) (s3c2410_dma_stop);
```
函数描述：这是DMA操作最关键的函数，它完成了一系列动作：分配并初始化一个DMA内核缓冲区控制结构，并将它插入DMA等待队列，设置DMA控制寄存器内容，等待DMA操作触发
输入参数： channel 通道号；buf_id,缓冲区标识
dma_addr_t data DMA数据缓冲区起始物理地址；size DMA数据缓冲区大小；write 是写还是读操作
输出参数：操作成功，返回0；否则，返回错误号

C
```
int s3c2410_dma_stop(dmach_t channel)
```
函数描述：停止DMA操作。

C
```
int s3c2410_dma_flush_all(dmach_t channel)
```
函数描述：释放DMA通道所申请的所有内存资源

C
```
void s3c2410_free_dma(dmach_t channel)
```
函数描述：释放DMA通道
因为各函数功能强大，一个完整的DMA驱动程序中一般只需调用以上3个函数即可。可在驱动初始化中调用s3c2410_request_dma，开始DMA传输前调用s3c2410_dma_queue_buffer，释放驱动模块时调用s3c2410_free_dma。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
