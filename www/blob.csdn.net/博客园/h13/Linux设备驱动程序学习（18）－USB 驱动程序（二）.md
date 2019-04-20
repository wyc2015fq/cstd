# Linux设备驱动程序学习（18）－USB 驱动程序（二） - h13 - 博客园
 USB 设备驱动代码通过urb和所有的 USB 设备通讯。urb用 struct urb 结构描述（include/linux/usb.h ）。
 urb以一种异步的方式同一个特定USB设备的特定端点发送或接受数据。一个 USB 设备驱动可根据驱动的需要，分配多个 urb 给一个端点或重用单个 urb 给多个不同的端点。设备中的每个端点都处理一个 urb 队列, 所以多个 urb 可在队列清空之前被发送到相同的端点。
 一个 urb 的典型生命循环如下:
 （1）被创建；
 （2）被分配给一个特定 USB 设备的特定端点；
 （3）被提交给 USB 核心；
 （4）被 USB 核心提交给特定设备的特定 USB 主机控制器驱动；
 （5）被 USB 主机控制器驱动处理, 并传送到设备；
 （6）以上操作完成后，USB主机控制器驱动通知 USB 设备驱动。
 urb 也可被提交它的驱动在任何时间取消；如果设备被移除，urb 可以被USB核心取消。urb 被动态创建并包含一个内部引用计数，使它们可以在最后一个用户释放它们时被自动释放。
**struct urb**
```
struct urb {
    /* private: usb core and host controller only fields in the urb */
    struct kref kref;        /* URB引用计数 */
    void *hcpriv;            /* host控制器的私有数据 */
    atomic_t use_count;        /* 当前提交计数 */
    atomic_t reject;        /* 提交失败计数 */
    int unlinked;            /* 连接失败代码 */
    /* public: documented fields in the urb that can be used by drivers */
    struct list_head urb_list;    /* list head for use by the urb's
                     * current owner */
    struct list_head anchor_list;    /* the URB may be anchored */
    struct usb_anchor *anchor;
    struct usb_device *dev;     /* 指向这个 urb 要发送的目标 struct usb_device 的指针,这个变量必须在这个 urb 被发送到 USB 核心之前被 USB 驱动初始化.*/
    struct usb_host_endpoint *ep;    /* (internal) pointer to endpoint */
    unsigned int pipe;        /* 这个 urb 所要发送到的特定struct usb_device的端点消息,这个变量必须在这个 urb 被发送到 USB 核心之前被 USB 驱动初始化.必须由下面的函数生成*/
    int status;            /*当 urb开始由 USB 核心处理或处理结束, 这个变量被设置为 urb 的当前状态. USB 驱动可安全访问这个变量的唯一时间是在 urb 结束处理例程函数中. 这个限制是为防止竞态. 对于等时 urb, 在这个变量中成功值(0)只表示这个 urb 是否已被去链. 为获得等时 urb 的详细状态, 应当检查 iso_frame_desc 变量. */
    unsigned int transfer_flags;    /* 传输设置*/
    void *transfer_buffer;        /* 指向用于发送数据到设备(OUT urb)或者从设备接收数据(IN urb)的缓冲区指针。为了主机控制器驱动正确访问这个缓冲, 它必须使用 kmalloc 调用来创建, 不是在堆栈或者静态内存中。 对控制端点, 这个缓冲区用于数据中转*/
    dma_addr_t transfer_dma;    /* 用于以 DMA 方式传送数据到 USB 设备的缓冲区*/
    int transfer_buffer_length;    /* transfer_buffer 或者 transfer_dma 变量指向的缓冲区大小。如果这是 0, 传送缓冲没有被 USB 核心所使用。对于一个 OUT 端点, 如果这个端点大小比这个变量指定的值小, 对这个 USB 设备的传输将被分成更小的块，以正确地传送数据。这种大的传送以连续的 USB 帧进行。在一个 urb 中提交一个大块数据, 并且使 USB 主机控制器去划分为更小的块, 比以连续地顺序发送小缓冲的速度快得多*/
    int actual_length;        /* 当这个 urb 完成后, 该变量被设置为这个 urb (对于 OUT urb)发送或(对于 IN urb)接受数据的真实长度.对于 IN urb, 必须是用此变量而非 transfer_buffer_length , 因为接收的数据可能比整个缓冲小*/
    unsigned char *setup_packet;    /* 指向控制urb的设置数据包指针.它在传送缓冲中的数据之前被传送（用于控制 urb）*/
    dma_addr_t setup_dma;        /* 控制 urb 用于设置数据包的 DMA 缓冲区地址,它在传送普通缓冲区中的数据之前被传送（用于控制 urb）*/
    int start_frame;        /* 设置或返回初始的帧数量（用于等时urb） */
    int number_of_packets;        /* 指定urb所处理的等时传输缓冲区的数量（用于等时urb，在urb被发送到USB核心前，必须设置） */
    int interval;            /*urb 被轮询的时间间隔. 仅对中断或等时 urb 有效. 这个值的单位依据设备速度而不同. 对于低速和高速的设备, 单位是帧, 它等同于毫秒. 对于其他设备, 单位是微帧, 等同于 1/8 毫秒. 在 urb被发送到 USB 核心之前，此值必须设置.*/
    int error_count;        /* 等时urb的错误计数，由USB核心设置 */
    void *context;            /* 指向一个可以被USB驱动模块设置的数据块. 当 urb 被返回到驱动时，可在结束处理例程中使用. */
    usb_complete_t complete;    /* 结束处理例程函数指针, 当 urb 被完全传送或发生错误，它将被 USB 核心调用. 此函数检查这个 urb, 并决定释放它或重新提交给另一个传输中*/
    struct usb_iso_packet_descriptor iso_frame_desc[0];
                    /* （仅用于等时urb）usb_iso_packet_descriptor结构体允许单个urb一次定义许多等时传输，它用于收集每个单独的传输状态*/
    };    
struct usb_iso_packet_descriptor {
    unsigned int offset;        /* 该数据包的数据在传输缓冲区中的偏移量（第一个字节为0） */
    unsigned int length;        /* 该数据包的传输缓冲区大小 */
    unsigned int actual_length;    /* 等时数据包接收到传输缓冲区中的数据长度 */
    int status;            /* 该数据包的单个等时传输状态。它可以把相同的返回值作为主struct urb 结构体的状态变量 */
};
typedef void (*usb_complete_t)(struct urb *);
```
上述结构体中unsigned int pipe;的生成函数(define):
```
static inline unsigned int __create_pipe(struct usb_device *dev,
        unsigned int endpoint)
{
    return (dev->devnum << 8) | (endpoint << 15);
}
/* Create various pipes... */
#define usb_sndctrlpipe(dev,endpoint)    \
    ((PIPE_CONTROL << 30) | __create_pipe(dev, endpoint))
#define usb_rcvctrlpipe(dev,endpoint)    \
    ((PIPE_CONTROL << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)
#define usb_sndisocpipe(dev,endpoint)    \
    ((PIPE_ISOCHRONOUS << 30) | __create_pipe(dev, endpoint))
#define usb_rcvisocpipe(dev,endpoint)    \
    ((PIPE_ISOCHRONOUS << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)
#define usb_sndbulkpipe(dev,endpoint)    \
    ((PIPE_BULK << 30) | __create_pipe(dev, endpoint))
#define usb_rcvbulkpipe(dev,endpoint)    \
    ((PIPE_BULK << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)
#define usb_sndintpipe(dev,endpoint)    \
    ((PIPE_INTERRUPT << 30) | __create_pipe(dev, endpoint))
#define usb_rcvintpipe(dev,endpoint)    \
    ((PIPE_INTERRUPT << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)
//snd:OUT rcv:IN  ctrl:控制  isoc:等时  bulk:批量 int:中断
```
上述结构体中unsigned int transfer_flags;的值域: 
```
/*
 * urb->transfer_flags:
 *
 * Note: URB_DIR_IN/OUT is automatically set in usb_submit_urb().
 */
#define URB_SHORT_NOT_OK    0x0001    /* 置位时，任何在 IN 端点上发生的简短读取, 被 USB 核心当作错误. 仅对从 USB 设备读取的 urb 有用 */
#define URB_ISO_ASAP        0x0002    /* 若为等时 urb , 驱动想调度这个 urb 时，可置位该位, 只要带宽允许且想在此时设置 urb 中的 start_frame 变量. 若没有置位,则驱动必须指定 start_frame 值，且传输如果不能在当
时启动的话，必须能够正确恢复 */
#define URB_NO_TRANSFER_DMA_MAP    0x0004    /* 当 urb 包含要被发送的 DMA 缓冲时，应被置位.USB 核心使用就会使用 transfer_dma 变量指向的缓冲, 而不是被 transfer_buffer 变量指向的缓冲. */
#define URB_NO_SETUP_DMA_MAP    0x0008    /* 和 URB_NO_TRANSFER_DMA_MAP 类似, 这个位用来控制 DMA 缓冲已经建立的 urb. 如果它被置位, USB 核心使用 setup_dma 变量而不是 setup_packet 变量指向的缓冲. */
#define URB_NO_FSBR        0x0020    /* 仅 UHCI USB 主机控制器驱动使用, 并且告诉它不要试图使用前端总线回收( Front Side Bus Reclamation) 逻辑. 这个位通常应不设置, 因为有 UHCI 主机控制器的机器会增加 CPU 负担, 且PCI 总线会忙于等待设置了这个位的 urb */
#define URB_ZERO_PACKET        0x0040    /* 如果置位, 批量 OUT urb 通过发送不包含数据的短包来结束, 这时数据对齐到一个端点数据包边界. 这被一些掉线的 USB 设备需要该位才能正确工作 */
#define URB_NO_INTERRUPT    0x0080    /* 如果置位, 当 urb 结束时硬件可能不产生一个中断. 该位应当小心使用并且只在多个 urb 排队到相同端点时才使用. USB 核心函数使用该位进行 DMA 缓冲传送. */
#define URB_FREE_BUFFER        0x0100    /* Free transfer buffer with the URB */
#define URB_DIR_IN        0x0200    /* Transfer from device to host */
#define URB_DIR_OUT        0
#define URB_DIR_MASK        URB_DIR_IN
```
上述结构体中int status;的常用值(in include/asm-generic/errno.h  and errno_base.h) :
```
// 0     表示 urb 传送成功*/
//以下各个定义在使用时为负值
#define    ENOENT         2    /* urb 被 usb_kill_urb 停止 */
#define    ECONNRESET    104    /* urb 被 usb_unlink_urb 去链, 且 transfer_flags 被设为 URB_ASYNC_UNLINK */
#define    EINPROGRESS    115    /* urb 仍在 USB 主机控制器处理 */
#define    EPROTO        71    /* urb 发生错误: 在传送中发生bitstuff 错误或硬件没有及时收到响应帧 */
#define    EILSEQ        84    /* urb 传送中出现 CRC 较验错 */
#define    EPIPE        32    /* 端点被停止. 若此端点不是控制端点, 则这个错误可通过函数 usb_clear_halt 清除 */
#define    ECOMM        70    /* 数据传输时的接收速度快于写入系统内存的速度. 此错误仅出现在 IN urb */
#define    ENOSR        63    /* 从系统内存中获取数据的速度赶不上USB 数据传送速度，此错误仅出现在 OUT urb. */
#define    EOVERFLOW    75    /* urb 发生"babble"（串扰）错误：端点接受的数据大于端点的最大数据包大小 */
#define    EREMOTEIO    181    /* 当 urb 的 transfer_flags 变量的 URB_SHORT_NOT_OK 标志被设置, urb 请求的数据没有完整地收到 */
#define    ENODEV        19    /* USB 设备从系统中拔出 */
#define    EXDEV        18    /* 仅发生在等时 urb 中, 表示传送部分完成. 为了确定所传输的内容, 驱动必须看单独的帧状态. */
#define    EINVAL        22    /* 如果urb的一个参数设置错误或在提交 urb 给 USB 核心的 usb_submit_urb 调用中, 有不正确的参数，则可能发生次错误 */
#define    ESHUTDOWN    108    /* USB 主机控制器驱动有严重错误，它已被禁止, 或者设备从系统中拔出。且这个urb 在设备被移除后被提交. 它也可能发生在 urb 被提交给设备时，设备的配置已被改变*/
```
**创建和注销 urb**
struct urb 结构不能静态创建,必须使用 usb_alloc_urb 函数创建. 函数原型:
```
struct urb *usb_alloc_urb(int iso_packets, gfp_t mem_flags);
//int iso_packets : urb 包含等时数据包的数目。如果不使用等时urb，则为0
//gfp_t mem_flags : 与传递给 kmalloc 函数调用来从内核分配内存的标志类型相同
```
`//返回值          : 如果成功分配足够内存给 urb , 返回值为指向 urb 的指针. 如果返回值是 NULL, 则在 USB 核心中发生了错误, 且驱动需要进行适当清理`
如果驱动已经对 urb 使用完毕, 必须调用 usb_free_urb 函数，释放urb。函数原型:
```
void usb_free_urb(struct urb *urb);
//struct urb *urb : 要释放的 struct urb 指针
```
根据内核源码，可以通过自己kmalloc一个空间来创建urb，然后必须使用
`void usb_init_urb(struct urb *urb);`
进行初始化后才可以继续使用。 
**其实usb_alloc_urb函数就是这样实现的，所以我当然不推荐这种自找麻烦的做法。 **
**初始化 urb**
```
static inline void usb_fill_int_urb(struct urb *urb,
                 struct usb_device *dev,
                 unsigned int pipe,
                 void *transfer_buffer,
                 int buffer_length,
                 usb_complete_t complete_fn,
                 void *context,
                 int interval);
static inline void usb_fill_bulk_urb(struct urb *urb,
                 struct usb_device *dev,
                 unsigned int pipe,
                 void *transfer_buffer,
                 int buffer_length,
                 usb_complete_t complete_fn,
                 void *context);
static inline void usb_fill_control_urb(struct urb *urb,
                    struct usb_device *dev,
                    unsigned int pipe,
                    unsigned char *setup_packet,
                    void *transfer_buffer,
                    int buffer_length,
                    usb_complete_t complete_fn,
                    void *context);
//struct urb *urb :指向要被初始化的 urb 的指针
//struct usb_device *dev :指向 urb 要发送到的 USB 设备.
//unsigned int pipe : urb 要被发送到的 USB 设备的特定端点. 必须使用前面提过的 usb_******pipe 函数创建
//void *transfer_buffer :指向外发数据或接收数据的缓冲区的指针.注意:不能是静态缓冲,必须使用 kmalloc 来创建.
//int buffer_length :transfer_buffer 指针指向的缓冲区的大小
//usb_complete_t complete :指向 urb 结束处理例程函数指针
//void *context :指向一个小数据块的指针, 被添加到 urb 结构中，以便被结束处理例程函数获取使用.
//int interval :中断 urb 被调度的间隔.
//函数不设置 urb 中的 transfer_flags 变量, 因此对这个成员的修改必须由驱动手动完成
/*等时 urb 没有初始化函数，必须手动初始化，以下为一个例子*/
urb->dev = dev;
urb->context = uvd;
urb->pipe = usb_rcvisocpipe(dev, uvd->video_endp-1);
urb->interval = 1;
urb->transfer_flags = URB_ISO_ASAP;
urb->transfer_buffer = cam->sts_buf[i];
urb->complete = konicawc_isoc_irq;
urb->number_of_packets = FRAMES_PER_DESC;
urb->transfer_buffer_length = FRAMES_PER_DESC;
for (j=0; j < FRAMES_PER_DESC; j++) {
        urb->iso_frame_desc[j].offset = j;
        urb->iso_frame_desc[j].length = 1;
}
```
其实那三个初始化函数只是简单的包装，是inline函数。所以其实和等时的urb手动初始化没什么大的区别。 
**提交 urb**
一旦 urb 被正确地创建并初始化, 它就可以提交给 USB 核心以发送出到 USB 设备. 这通过调用函数 usb_submit_urb 实现:
```
int usb_submit_urb(struct urb *urb, gfp_t mem_flags);
//struct urb *urb :指向被提交的 urb 的指针 
//gfp_t mem_flags :使用传递给 kmalloc 调用同样的参数, 用来告诉 USB 核心如何及时分配内存缓冲
/*因为函数 usb_submit_urb 可被在任何时候被调用(包括从一个中断上下文), mem_flags 变量必须正确设置. 根据 usb_submit_urb 被调用的时间,只有 3 个有效值可用:
GFP_ATOMIC 
只要满足以下条件,就应当使用此值:
1.调用者处于一个 urb 结束处理例程,中断处理例程,底半部,tasklet或者一个定时器回调函数.
2.调用者持有自旋锁或者读写锁. 注意如果正持有一个信号量, 这个值不必要.
3.current->state 不是 TASK_RUNNING. 除非驱动已自己改变 current 状态,否则状态应该一直是 TASK_RUNNING .
GFP_NOIO 
驱动处于块 I/O 处理过程中. 它还应当用在所有的存储类型的错误处理过程中.
GFP_KERNEL 
所有不属于之前提到的其他情况
*/
```
在 urb 被成功提交给 USB 核心之后, 直到结束处理例程函数被调用前,都不能访问 urb 结构的任何成员. 
**urb结束处理例程**
如果 usb_submit_urb 被成功调用, 并把对 urb 的控制权传递给 USB 核心, 函数返回 0; 否则返回一个负的错误代码. 如果函数调用成功, 当 urb 被结束的时候结束处理例程会被调用一次.当这个函数被调用时, USB 核心就完成了这个urb, 并将它的控制权返回给设备驱动.
只有 3 种结束urb并调用结束处理例程的情况:
(1)urb 被成功发送给设备, 且设备返回正确的确认.如果这样, urb 中的status变量被设置为 0.
(2)发生错误, 错误值记录在 urb 结构中的 status 变量.
(3)urb 从 USB 核心unlink. 这发生在要么当驱动通过调用 usb_unlink_urb 或者 usb_kill_urb告知 USB 核心取消一个已提交的 urb,或者在一个 urb 已经被提交给它时设备从系统中去除.
**取消 urb**
使用以下函数停止一个已经提交给 USB 核心的 urb:
```
void usb_kill_urb(struct urb *urb)
int usb_unlink_urb(struct urb *urb);
```
如果调用usb_kill_urb函数,则 urb 的生命周期将被终止. 这通常在设备从系统移除时,在断开回调函数(disconnect callback)中调用.
对一些驱动, 应当调用 usb_unlink_urb 函数来使 USB 核心停止 urb. 这个函数不会等待 urb 完全停止才返回. 这对于在中断处理例程中或者持有一个自旋锁时去停止 urb 是很有用的, 因为等待一个 urb 完全停止需要 USB 核心有使调用进程休眠的能力(wait_event()函数). 
本文出自：[http://blog.chinaunix.net/space.php?uid=20543672&do=blog&id=94356](http://blog.chinaunix.net/space.php?uid=20543672&do=blog&id=94356)

