# platform_device和platform_driver - h13 - 博客园
首先介绍一下注册一个驱动的步骤：
1、定义一个platform_driver结构
2、初始化这个结构，指定其probe、remove等函数，并初始化其中的driver变量
3、实现其probe、remove等函数
看platform_driver结构，定义于include/linux/platform_device.h文件中：
```
struct platform_driver {
    int (*probe)(struct platform_device *);
    int (*remove)(struct platform_device *);
    void (*shutdown)(struct platform_device *);
    int (*suspend)(struct platform_device *, pm_message_t state);
    int (*suspend_late)(struct platform_device *, pm_message_t state);
    int (*resume_early)(struct platform_device *);
    int (*resume)(struct platform_device *);
    struct device_driver driver;
};
```
    可见，它包含了设备操作的几个功能函数，同样重要的是，它还包含了一个device_driver结构。刚才提到了驱动程序中需要初始化这个变量。下面看一下这个变量的定义，位于include/linux/device.h中：
```
struct device_driver {
    const char        * name;
    struct bus_type        * bus;
    struct kobject        kobj;
    struct klist        klist_devices;
    struct klist_node    knode_bus;
    struct module        * owner;
    const char         * mod_name;  /* used for built-in modules */
    struct module_kobject    * mkobj;
    int    (*probe)    (struct device * dev);
    int    (*remove)    (struct device * dev);
    void    (*shutdown)    (struct device * dev);
    int    (*suspend)    (struct device * dev, pm_message_t state);
    int    (*resume)    (struct device * dev);
};
```
    需要注意这两个变量：name和owner。那么的作用主要是为了和相关的platform_device关联起来，owner的作用是说明模块的所有者，驱动程序中一般初始化为THIS_MODULE。
    下面是一个platform_driver的初始化实例：
```
static struct platform_driver s3c2410iis_driver = { 
  .probe = s3c2410iis_probe, 
  .remove = s3c2410iis_remove,
  .driver = {
    .name = "s3c2410-iis",
    .owner = THIS_MODULE,
  },
};
```
    上面的初始化是一个音频驱动的实例。注意其中的driver这个结构体，只初始化了其name和owner两个量。接着看一下和driver相关的另一个结构，定义如下：
```
struct platform_device {
    const char    * name;
    int        id;
    struct device    dev;
    u32        num_resources;
    struct resource    * resource;
};
```
   该结构中也有一个name变量。platform_driver从字面上来看就知道是设备驱动。设备驱动是为谁服务的呢？当然是设备了。platform_device就描述了设备对象。下面是一个具体的实例： 
```
struct platform_device s3c_device_iis = {
    .name         = "s3c2410-iis",
    .id         = -1,
    .num_resources     = ARRAY_SIZE(s3c_iis_resource),
    .resource     = s3c_iis_resource,
    .dev = {
        .dma_mask = &s3c_device_iis_dmamask,
        .coherent_dma_mask = 0xffffffffUL
    }
};
```
    它的name变量和刚才上面的platform_driver的name变量是一致的，内核正是通过这个一致性来为驱动程序找到资源，即platform_device中的resource。这个结构的定义如下，位于include/linux/ioport.h中： 
```
struct resource {
    resource_size_t start;
    resource_size_t end;
    const char *name;
    unsigned long flags;
    struct resource *parent, *sibling, *child;
};
```
    下面是一个具体的实例：
```
static struct resource s3c_iis_resource[] = {
    [0] = {
        .start = S3C24XX_PA_IIS,
        .end = S3C24XX_PA_IIS + S3C24XX_SZ_IIS -1,
        .flags = IORESOURCE_MEM,
    }
};
```
    这个结构的作用就是告诉驱动程序设备的起始地址和终止地址和设备的端口类型。这里的地址指的是物理地址。
    另外还需要注意platform_device中的device结构，它详细描述了设备的情况，定义如下：
```
struct device {
    struct klist        klist_children;
    struct klist_node    knode_parent;        /* node in sibling list */
    struct klist_node    knode_driver;
    struct klist_node    knode_bus;
    struct device        *parent;
    struct kobject kobj;
    char    bus_id[BUS_ID_SIZE];    /* position on parent bus */
    struct device_type    *type;
    unsigned        is_registered:1;
    unsigned        uevent_suppress:1;
    struct semaphore    sem;    /* semaphore to synchronize calls to
                     * its driver.
                     */
    struct bus_type    * bus;        /* type of bus device is on */
    struct device_driver *driver;    /* which driver has allocated this
                     device */
    void        *driver_data;    /* data private to the driver */
    void        *platform_data;    /* Platform specific data, device
                     core doesn't touch it */
    struct dev_pm_info    power;
#ifdef CONFIG_NUMA
    int        numa_node;    /* NUMA node this device is close to */
#endif
    u64        *dma_mask;    /* dma mask (if dma'able device) */
    u64        coherent_dma_mask;/* Like dma_mask, but for
                     alloc_coherent mappings as
                     not all hardware supports
                     64 bit addresses for consistent
                     allocations such descriptors. */
    struct list_head    dma_pools;    /* dma pools (if dma'ble) */
    struct dma_coherent_mem    *dma_mem; /* internal for coherent mem
                     override */
    /* arch specific additions */
    struct dev_archdata    archdata;
    spinlock_t        devres_lock;
    struct list_head    devres_head;
    /* class_device migration path */
    struct list_head    node;
    struct class        *class;
    dev_t            devt;        /* dev_t, creates the sysfs "dev" */
    struct attribute_group    **groups;    /* optional groups */
    void    (*release)(struct device * dev);
};
```
    上面把驱动程序中涉及到的主要结构都介绍了，下面主要说一下驱动程序中怎样对这个结构进行处理，以使驱动程序能运行。
    相信大家都知道module_init（）这个宏。驱动模块加载的时候会调用这个宏。它接收一个函数为参数，作为它的参数的函数将会对上面提到的platform_driver进行处理。看一个实例：假如这里module_init要接收的参数为s3c2410_uda1341_init这个函数，下面是这个函数的定义：
```
static int __init s3c2410_uda1341_init(void) { 
 memzero(&input_stream, sizeof(audio_stream_t)); 
 memzero(&output_stream, sizeof(audio_stream_t)); 
 return platform_driver_register(&s3c2410iis_driver); 
}
```
注意函数体的最后一行，它调用的是platform_driver_register这个函数。这个函数定义于driver/base/platform.c中，原型如下：
int platform_driver_register(struct platform_driver *drv)
它的功能就是为上面提到的plarform_driver中的driver这个结构中的probe、remove这些变量指定功能函数。
    到目前为止，内核就已经知道了有这么一个驱动模块。内核启动的时候，就会调用与该驱动相关的probe函数。我们来看一下probe函数实现了什么功能。
    probe函数的原型为
    int xxx_probe(struct platform_device *pdev)
    即它的返回类型为int，接收一个platform_device类型的指针作为参数。返回类型就是我们熟悉的错误代码了，而接收的这个参数呢，我们上面已经说过，驱动程序为设备服务，就需要知道设备的信息。而这个参数，就包含了与设备相关的信息。
    probe函数接收到plarform_device这个参数后，就需要从中提取出需要的信息。它一般会通过调用内核提供的platform_get_resource和platform_get_irq等函数来获得相关信息。如通过platform_get_resource获得设备的起始地址后，可以对其进行request_mem_region和ioremap等操作，以便应用程序对其进行操作。通过platform_get_irq得到设备的中断号以后，就可以调用request_irq函数来向系统申请中断。这些操作在设备驱动程序中一般都要完成。
    在完成了上面这些工作和一些其他必须的初始化操作后，就可以向系统注册我们在/dev目录下能看在的设备文件了。举一个例子，在音频芯片的驱动中，就可以调用register_sound_dsp来注册一个dsp设备文件，lcd的驱动中就可以调用register_framebuffer来注册fb设备文件。这个工作完成以后，系统中就有我们需要的设备文件了。而和设备文件相关的操作都是通过一个file_operations 来实现的。在调用register_sound_dsp等函数的时候，就需要传递一个file_operations 类型的指针。这个指针就提供了可以供用户空间调用的write、read等函数。file_operations结构的定义位于include/linux/fs.h中，列出如下：
```
struct file_operations {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    int (*readdir) (struct file *, void *, filldir_t);
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
    int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    int (*mmap) (struct file *, struct vm_area_struct *);
    int (*open) (struct inode *, struct file *);
    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *);
    int (*fsync) (struct file *, struct dentry *, int datasync);
    int (*aio_fsync) (struct kiocb *, int datasync);
    int (*fasync) (int, struct file *, int);
    int (*lock) (struct file *, int, struct file_lock *);
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
    int (*check_flags)(int);
    int (*dir_notify)(struct file *filp, unsigned long arg);
    int (*flock) (struct file *, int, struct file_lock *);
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **);
};
```
    到目前为止，probe函数的功能就完成了。
    当用户打开一个设备，并调用其read、write等函数的时候，就可以通过上面的file_operations来找到相关的函数。所以，用户驱动程序还需要实现这些函数，具体实现和相关的设备有密切的关系，这里就不再介绍了。
**原文地址**[http://blog.chinaunix.net/u1/57747/showart_1073860.html](http://blog.chinaunix.net/u1/57747/showart_1073860.html)

