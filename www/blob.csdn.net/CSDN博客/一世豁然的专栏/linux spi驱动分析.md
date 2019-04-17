# linux spi驱动分析 - 一世豁然的专栏 - CSDN博客





2015年03月29日 18:38:20[一世豁然](https://me.csdn.net/Explorer_day)阅读数：477标签：[Linux驱动																[SPI](https://so.csdn.net/so/search/s.do?q=SPI&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux驱动&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








本博客转载于：[http://blog.chinaunix.net/uid-23036581-id-2230553.html](http://blog.chinaunix.net/uid-23036581-id-2230553.html)







关于spi的学习，我觉得最好的方法还是看Linux的源代码，主要是driver/spi/spi.c(h)，spidev.c(h)。spi dev的示例可以看看at25.c，spi总线的示例可以看omap_uwire或者spi_s3c24xx.c和spi_s3c24xx_gpio.c。在看这些代码之前，需要对Linux的设备模型有一定的了解。

另外，网上有两篇教程不错，《linux spi子系统驱动分析》以及《linux spi子系统 驱动分析 续》，百度可以直接搜到，这里帖一下我找到的链接，但不清楚是转载的还是原创的。

[http://linux.chinaunix.net/techdoc/net/2007/11/12/972031.shtml](http://linux.chinaunix.net/techdoc/net/2007/11/12/972031.shtml)

[http://www.sudu.cn/info/html/edu/20070101/285153.html](http://www.sudu.cn/info/html/edu/20070101/285153.html)



下面是我整理的关于SPI的一些心得，内核版本2.6.29。

## SPI子系统

spi子系统中，spi设备用struct spi_dev描述，它的驱动程序用struct spi_driver描述。spi总线设备用struct spi_master描述。另外，还有两个重要的全局变量：

struct bus_type spi_bus_type = {

       .name             = "spi",

       .dev_attrs       = spi_dev_attrs,

       .match           = spi_match_device,

       .uevent           = spi_uevent,

       .suspend  = spi_suspend,

       .resume          = spi_resume,

};



static struct class spi_master_class = {

       .name             = "spi_master",

       .owner           = THIS_MODULE,

       .dev_release    = spi_master_release,

};



spi_bus_type对应sys中的spi bus总线，Linux设备模型对这个结构体有详细介绍。



所有spi_master对应的spi总线都属于spi_master_class，也就是说是一个虚拟设备，它的父设备可能是物理设备，比如platform_device等等，s3c2410就是这种情况。

## SPI设备

SPI设备的驱动程序通过spi_register_driver注册进SPI子系统，驱动类型为struct spi_driver。典型例子如at25.c。

static struct spi_driver at25_driver = {

       .driver = {

              .name             = "at25",

              .owner           = THIS_MODULE,

       },

       .probe            = at25_probe,

       .remove          = __devexit_p(at25_remove),

};

因为spi总线不支持SPI设备的自动检测，所以一般在spi的probe函数中不会检测设备是否存在，而是做一些spi设备的初始化工作。



spi驱动中可以调用下列函数进行spi的传输操作：

static inline int spi_write(struct spi_device *spi, const u8 *buf, size_t len);

static inline int spi_read(struct spi_device *spi, u8 *buf, size_t len);

extern int spi_write_then_read(struct spi_device *spi,        const u8 *txbuf, unsigned n_tx,

              u8 *rxbuf, unsigned n_rx);

static inline ssize_t spi_w8r8(struct spi_device *spi, u8 cmd);

static inline ssize_t spi_w8r16(struct spi_device *spi, u8 cmd);



由于spi设备不能被spi总线动态扫描，所以spi子系统使用了另一种方法，就是通过spi_register_board_info函数将spi设备静态得登记到系统中。

int __init spi_register_board_info(struct spi_board_info const *info, unsigned n);

struct spi_board_info {

       char        modalias[32];                // 设备名

       const void      *platform_data;      // 私有数据，会被设置到spi_device.dev.platform_data

       void        *controller_data;           // 私有数据，会被设置到spi_device.controller_data

       int           irq;                              // 中断号

       u32         max_speed_hz;             // 最大速率

       u16         bus_num;                            // 用于关联spi_master

       u16         chip_select;                  // 与片选有关

       u8           mode;                          // spi_device.mode

};

在具体平台的文件中，可以定义struct spi_board_info的结构体，然后通过spi_register_board_info函数保存这些结构体，最后在scan_boardinfo函数中根据这些保存的结构体创建spi设备（spi_new_device）。



spi_new_device用于登记spi设备，这里面又分两步，首先是spi_alloc_device，然后是spi_add_device。

struct spi_device *spi_new_device(struct spi_master *master, struct spi_board_info *chip)

       spi_dev* pdev = spi_alloc(master);

       proxy->chip_select = chip->chip_select;

       proxy->max_speed_hz = chip->max_speed_hz;

       proxy->mode = chip->mode;

       proxy->irq = chip->irq;

       strlcpy(proxy->modalias, chip->modalias, sizeof(proxy->modalias));

       proxy->dev.platform_data = (void *) chip->platform_data;

       proxy->controller_data = chip->controller_data;

       proxy->controller_state = NULL;

       spi_add_device(proxy);



struct spi_device *spi_alloc_device(struct spi_master *master)

struct device * dev = master->dev.parent;

       struct spi_dev * spi = kzalloc(sizeof *spi, GFP_KERNEL);

       spi->master = master;

spi->dev.parent = dev;

       spi->dev.bus = &spi_bus_type;

       spi->dev.release = spidev_release;

       device_initialize(&spi->dev);

这里spi_dev的父设备被指定为master的父设备，而master是spi总线设备，拥有class，是一个虚拟设备。也就是说，spi设备和与之对应的总线设备拥有同一个父设备，这个父设备一般来说是一个物理设备。



int spi_add_device(struct spi_device *spi)

       snprintf(spi->dev.bus_id, sizeof spi->dev.bus_id, "%s.%u", spi->master->dev.bus_id,

                     spi->chip_select);

       status = spi->master->setup(spi);

status = device_add(&spi->dev);

## spi总线

struct spi_master {

       struct device   dev;

 s16   bus_num;                         // 总线号，如果板子上有多个spi总线，靠这个域区分；另外，spi_dev中也有bus_num，spi_dev通过这个域找到它所属的总线。

       u16    num_chipselect;         // 片选号，如果一个spi总线有多个设备，

       /* setup mode and clock, etc (spi driver may call many times) */

       int                  (*setup)(struct spi_device *spi);

       int                  (*transfer)(struct spi_device *spi,       struct spi_message *mesg);

       /* called on release() to free memory provided by spi_master */

       void               (*cleanup)(struct spi_device *spi);

};



**登记spi总线**

struct spi_master *spi_alloc_master(struct device *dev, unsigned size);



int spi_register_master(struct spi_master *master);

scan_boardinfo(master);

spi_register_master中会调用scan_boardinfo。scan_boardinfo中，会扫描前面保存的boardinfo，看新注册的master中的bus_num是否与boardinfo中bus_num匹配，如果匹配，那就调用spi_new_device创建spi设备，并登记到spi子系统中。



**setup函数**

setup函数会做一些初始化工作。比如，根据spi设备的速率，设备paster的位传输定时器；设置spi传输类型；等等。

spi_add_device函数中，会先调用setup函数，然后再调用device_add。这是因为device_add中会调用到driver的probe函数，而probe函数中可能会对spi设备做IO操作。所以spi子系统就先调用setup为可能的IO操作做好准备。

但是，在代码中，setup函数似乎也就只在这一个地方被调用。具体传输过程中切换spi设备时也要做配置工作，但这里的配置工作就由具体传输的实现代码决定了，可以看看spi_bitbang.c中的函数bitbang_work。



**cleanup函数**

cleanup函数会在spidev_release函数中被调用，spidev_release被登记为spi dev的release函数。



**transfer函数**

transfer函数用于spi的IO传输。但是，transfer函数一般不会执行真正的传输操作，而是把要传输的内容放到一个队列里，然后调用一种类似底半部的机制进行真正的传输。这是因为，spi总线一般会连多个spi设备，而spi设备间的访问可能会并发。如果直接在transfer函数中实现传输，那么会产生竞态，spi设备互相间会干扰。

所以，真正的spi传输与具体的spi控制器的实现有关，spi的框架代码中没有涉及。像spi设备的片选、根据具体设备进行时钟调整等等都在实现传输的代码中被调用。



SPI的传输命令都是通过结构体spi_message定义。设备程序调用transfer函数将spi_message交给spi总线驱动，总线驱动再将message传到底半部排队，实现串行化传输。

struct spi_message {

       struct list_head       transfers;

       struct spi_device    *spi;

       unsigned         is_dma_mapped:1;

       void               (*complete)(void *context);

       void               *context;

       unsigned         actual_length;

       int                  status;

       struct list_head       queue;

       void               *state;

};

spi_message中，有一个transfers队列，spi_transfer结构体通过这个队列挂到spi_message中。一个spi_message代表一次传输会话，spi_transfer代表一次单独的IO操作。比如，有些spi设备需要先读后写，那么这个读写过程就是一次spi会话，里面包括两个transfer，一个定义写操作的参数，另一个定义读操作的参数。

## spidev.c

如果不想为自己的SPI设备写驱动，那么可以用Linux自带的spidev.c提供的驱动程序。要使用spidev.c的驱动，只要在登记设备时，把设备名设置成spidev就可以。spidev.c会在device目录下自动为每一个匹配的SPI设备创建设备节点，节点名”spi%d”。之后，用户程序可以通过字符型设备的通用接口控制SPI设备。

需要注意的是，spidev创建的设备在设备模型中属于虚拟设备，它的class是spidev_class。它的父设备是在boardinfo中定义的spi设备。





