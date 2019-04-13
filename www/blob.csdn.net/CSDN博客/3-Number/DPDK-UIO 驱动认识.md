
# DPDK-UIO 驱动认识 - 3-Number - CSDN博客


2017年11月06日 14:52:02[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：3113


# 0x01 缘由
前期学习了一些DPDK的demo，了解了一些基础DPDK的应用。对dpdk的一些底层原理没做过多分析，今天主要学习用户态驱动程序的实现情况。
# 0x02 用户态驱动程序UIO
**UIO（Userspace I/O）**是运行在用户空间的I/O技术。Linux系统中一般的驱动设备都是运行在内核空间，而在用户空间用应用程序调用即可。
UIO的内核部分和用户空间的工作
**内核空间**
UIO的少量运行在内核空间的驱动所做的工作有哪些呢?
（1）分配和记录设备需要的资源和注册uio设备
在设备的探测函数中：
-使能PCI 设备
-申请资源
-读取并记录配置信息
-注册uio设备// uio_register_device()
// uio_8139d_pci_probe & uio_8139d_handler
（2）必须在内核空间实现的小部分中断应答函数
**用户空间的关键操作**
（1）关键操作
（2）响应硬件中断
![](https://img-blog.csdn.net/20171106145017326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**有什么优势？**
1、用户空间驱动程序的优点
1、可以和整个C库链接。
2、在驱动中可以使用浮点数，在某些特殊的硬件中，可能需要使用浮点数，而linux内核并不提供浮点数的支持。如果能在用户态实现驱动，就可以轻松解决这一问题。
3、驱动问题不会导致整个系统挂起。内核态驱动的一些错误常常导致整个系统挂起。
4、用户态的驱动调试方便。
5、可以给出封闭源码的驱动程序，不必采用GPL，更为灵活。
我的理解为将以前内核模块驱动所需要做的工作移到了用户态空间操作。dpdk实现一个uio驱动，然后实现e1000e驱动的相关操作。
# 0x03 源码简单分析
与其他内核PCI模块开发一样代码结构，UIO驱动实现部分：
关键数据结构：
```python
//dpdk定义的uio pci设备描述结构
struct rte_uio_pci_dev {
    struct uio_info info; //uio 通用结构
    struct pci_dev *pdev;  //pci设备描述结构
    enum rte_intr_mode mode; //中断模式
};
struct uio_info {
    struct uio_device    *uio_dev; //uio设备属于
    const char        *name; //名称
    const char        *version; //版本号
    struct uio_mem        mem[MAX_UIO_MAPS];//可映射的内存区域列表，size == 0表示列表结束
    struct uio_port        port[MAX_UIO_PORT_REGIONS]; //网口区域列表
    long            irq; //UIO_IRQ_CUSTOM 中断号
    unsigned long        irq_flags; //请求中断号的标志
    void            *priv;  //可选的私有数据
    irqreturn_t (*handler)(int irq, struct uio_info *dev_info); //中断信息处理
    int (*mmap)(struct uio_info *info, struct vm_area_struct *vma);//内存映射操作
    int (*open)(struct uio_info *info, struct inode *inode); //打开
    int (*release)(struct uio_info *info, struct inode *inode); //释放
    int (*irqcontrol)(struct uio_info *info, s32 irq_on); //中断控制操作 关闭/打开 当向/dev/uioX中写入值时
};
关键处理函数：
static int __init
igbuio_pci_init_module(void)
{
    int ret;
    ret = igbuio_config_intr_mode(intr_mode); //内核insmod时带的参数，中断模式
    if (ret < 0)
        return ret;
    return pci_register_driver(&igbuio_pci_driver);//注册PCI设备，实际调用pci_module_init。
}
关键的pci驱动操作函数，主要是探测和删除
static struct pci_driver igbuio_pci_driver = {
    .name = "igb_uio", //名称
    .id_table = NULL,
    .probe = igbuio_pci_probe, //探测回调函数
    .remove = igbuio_pci_remove,//删除回调函数
};
关键看下igbuio_pci_probe：
//根据内核版本不同，返回类型不同
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
static int __devinit
#else
static int
#endif
igbuio_pci_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
    struct rte_uio_pci_dev *udev;
    struct msix_entry msix_entry;
    int err;
    //分配内核空间内存，rte_uio_pci_dev一个设备类型大小
    udev = kzalloc(sizeof(struct rte_uio_pci_dev), GFP_KERNEL);
    if (!udev)
        return -ENOMEM;
    /*
     * 使能设备: 调用更底层的PCI代码使能设备的内存和I/O区域
     */
    err = pci_enable_device(dev);
    if (err != 0) {
        dev_err(&dev->dev, "Cannot enable PCI device\n");
        goto fail_free;
    }
    /*
        预留PCI设备的i/o或内存区域，pci_request_regions这个函数封装了一些PCI驱动相关的内存操作，不深入理解；
     */
    err = pci_request_regions(dev, "igb_uio");
    if (err != 0) {
        dev_err(&dev->dev, "Cannot request regions\n");
        goto fail_disable;
    }
    /* 将设备设置层DMA总线主模式 */
    pci_set_master(dev);
    /* 重新映射I/O内存，同样详细的封装不做具体理解 */
    err = igbuio_setup_bars(dev, &udev->info);
    if (err != 0)
        goto fail_release_iomem;
    /* 设定 64-bit DMA mask 若函数返回成功，可以在位于该函数所带参数范围内的任意地址进行DMA操作。*/
    err = pci_set_dma_mask(dev,  DMA_BIT_MASK(64));
    if (err != 0) {
        dev_err(&dev->dev, "Cannot set DMA mask\n");
        goto fail_release_iomem;
    }
    //内存范围一致性的处理
    err = pci_set_consistent_dma_mask(dev, DMA_BIT_MASK(64));
    if (err != 0) {
        dev_err(&dev->dev, "Cannot set consistent DMA mask\n");
        goto fail_release_iomem;
    }
    /* 填充uio信息 */
    udev->info.name = "igb_uio";
    udev->info.version = "0.1";
    udev->info.handler = igbuio_pci_irqhandler;
    udev->info.irqcontrol = igbuio_pci_irqcontrol;
#ifdef CONFIG_XEN_DOM0
    /* check if the driver run on Xen Dom0 */
    if (xen_initial_domain())
        udev->info.mmap = igbuio_dom0_pci_mmap;
#endif
    udev->info.priv = udev;
    udev->pdev = dev;
    switch (igbuio_intr_mode_preferred) {
    case RTE_INTR_MODE_MSIX:
        /* Only 1 msi-x vector needed */
        msix_entry.entry = 0;
        if (pci_enable_msix(dev, &msix_entry, 1) == 0) {
            dev_dbg(&dev->dev, "using MSI-X");
            udev->info.irq = msix_entry.vector;
            udev->mode = RTE_INTR_MODE_MSIX;
            break;
        }
        /* fall back to INTX */
    case RTE_INTR_MODE_LEGACY:
        if (pci_intx_mask_supported(dev)) {
            dev_dbg(&dev->dev, "using INTX");
            udev->info.irq_flags = IRQF_SHARED;
            udev->info.irq = dev->irq;
            udev->mode = RTE_INTR_MODE_LEGACY;
            break;
        }
        dev_notice(&dev->dev, "PCI INTX mask not supported\n");
        /* fall back to no IRQ */
    case RTE_INTR_MODE_NONE:
        udev->mode = RTE_INTR_MODE_NONE;
        udev->info.irq = 0;
        break;
    default:
        dev_err(&dev->dev, "invalid IRQ mode %u",
            igbuio_intr_mode_preferred);
        err = -EINVAL;
        goto fail_release_iomem;
    }
    //用特定属性创建sysfs节点组
    err = sysfs_create_group(&dev->dev.kobj, &dev_attr_grp);
    if (err != 0)
        goto fail_release_iomem;
    /* 注册uio设备 */
    err = uio_register_device(&dev->dev, &udev->info);
    if (err != 0)
        goto fail_remove_group;
    pci_set_drvdata(dev, udev);
    dev_info(&dev->dev, "uio device registered with irq %lx\n",
         udev->info.irq);
    return 0;
fail_remove_group:
    sysfs_remove_group(&dev->dev.kobj, &dev_attr_grp);
fail_release_iomem:
    igbuio_pci_release_iomem(&udev->info);
    if (udev->mode == RTE_INTR_MODE_MSIX)
        pci_disable_msix(udev->pdev);
    pci_release_regions(dev);
fail_disable:
    pci_disable_device(dev);
fail_free:
    kfree(udev);
    return err;
}
```

# 0x04 DPDK应用层实现
转载：[http://www.cnblogs.com/MerlinJ/p/4108021.html](http://www.cnblogs.com/MerlinJ/p/4108021.html)
本文基于DPDK-1.8.0分析。
网卡驱动模型一般包含三层，即，PCI总线设备、网卡设备以及网卡设备的私有数据结构，即将设备的共性一层层的抽象，PCI总线设备包含网卡设备，网卡设备又包含其私有数据结构。在DPDK中，首先会注册设备驱动，然后查找当前系统有哪些PCI设备，并通过PCI_ID为PCI设备找到对应的驱动，最后调用驱动初始化设备。
一、网卡驱动注册
以e1000网卡驱动为例说明。
在1.8.0版本中，网卡驱动的注册使用了一种奇技淫巧的方法，使用GCC attribute扩展属性的constructor属性，使得网卡驱动的注册在程序MAIN函数之前就执行了。
![](http://common.cnblogs.com/images/copycode.gif)
staticstructrte_driver pmd_igb_drv ={
.type=PMD_PDEV,
.init=rte_igb_pmd_init,
};

staticstructrte_driver pmd_igbvf_drv ={
.type=PMD_PDEV,
.init=rte_igbvf_pmd_init,
};

PMD_REGISTER_DRIVER(pmd_igb_drv);
PMD_REGISTER_DRIVER(pmd_igbvf_drv);
![](http://common.cnblogs.com/images/copycode.gif)
其中PMD_REGISTER_DRIVER()宏的定义如下：
\#definePMD_REGISTER_DRIVER(d)\
voiddevinitfn_ \#\#d(void);\
void__attribute__((constructor, used)) devinitfn_ \#\#d(void)\
{\
rte_eal_driver_register(&d);\
}

使用attribute的constructor属性，在MAIN函数执行前，就执行rte_eal_driver_register()函数，将pmd_igb_drv驱动挂到全局dev_driver_list链表上。
二、扫描当前系统有哪些PCI设备
调用rte_eal_init()--->rte_eal_pci_init()函数，查找当前系统中有哪些网卡，分别是什么类型，并将它们挂到全局链表pci_device_list上。
1、首先初始化全局链表pci_driver_list、pci_device_list。用于挂载PCI驱动及PCI设备。
2、pci_scan()通过读取/sys/bus/pci/devices/目录下的信息，扫描当前系统的PCI设备，并初始化，并按照PCI地址从大到小的顺序挂在到pci_debice_list上。
![](http://common.cnblogs.com/images/copycode.gif)
int
rte_eal_pci_init(void)
{
TAILQ_INIT(&pci_driver_list);
TAILQ_INIT(&pci_device_list);
pci_res_list=RTE_TAILQ_RESERVE_BY_IDX(RTE_TAILQ_PCI,
mapped_pci_res_list);

/*for debug purposes, PCI can be disabled*/
if(internal_config.no_pci)
return0;

if(pci_scan() <0) {
RTE_LOG(ERR, EAL,"%s(): Cannot scan PCI bus\n",
 __func__);
return-1;
}
\#ifdef VFIO_PRESENT
pci_vfio_enable();

if(pci_vfio_is_enabled()) {

/*if we are primary process, create a thread to communicate with
* secondary processes. the thread will use a socket to wait for
* requests from secondary process to send open file descriptors,
* because VFIO does not allow multiple open descriptors on a group or
* VFIO container.
*/
if(internal_config.process_type == RTE_PROC_PRIMARY &&
pci_vfio_mp_sync_setup()<0)
return-1;
}
\#endif
return0;
}
![](http://common.cnblogs.com/images/copycode.gif)
pcai_scan()通过读取/sys/bus/pci/devices/目录下相关PCI设备的如下文件，获取对应的信息，初始化struct rte_pci_device数据结构，并将其按照PCI地址从大到小的顺序挂到pci_device_list链表上。
![](http://common.cnblogs.com/images/copycode.gif)
structrte_pci_device {
TAILQ_ENTRY(rte_pci_device) next;/**< Next probed
 PCI device.*/
structrte_pci_addr addr;/**<
 PCI location.*/
structrte_pci_id id;/**< PCI
 ID.*/
structrte_pci_resource mem_resource[PCI_MAX_RESOURCE];/**<
 PCI Memory Resource*/
structrte_intr_handle intr_handle;/**<
 Interrupt handle*/
conststructrte_pci_driver *driver;/**<
 Associated driver*/
uint16_t max_vfs;/**< sriov enable if not zero*/
intnuma_node;/**< NUMA node
 connection*/
structrte_devargs *devargs;/**<
 Device user arguments*/
};
![](http://common.cnblogs.com/images/copycode.gif)
![](http://common.cnblogs.com/images/copycode.gif)
root@Ubuntu:~\# ls -ltr /sys/bus/pci/devices/0000\:00\:09.0/
total0
-rw-r--r--1root root4096Nov1512:18uevent
lrwxrwxrwx1root root0Nov1512:18subsystem -> ../../../bus/pci
-r--r--r--1root root4096Nov1512:19class
-r--r--r--1root root4096Nov1512:19vendor
-r--r--r--1root root4096Nov1512:19device
-rw-r--r--1root root256Nov1512:19config
-r--r--r--1root root4096Nov1512:19local_cpus
-r--r--r--1root root4096Nov1512:19irq
-r--r--r--1root root4096Nov1512:20resource
drwxr-xr-x2root root0Nov1512:20power
-r--r--r--1root root4096Nov1914:33subsystem_vendor
-r--r--r--1root root4096Nov1914:33subsystem_device
-r--r--r--1root root4096Nov1914:33numa_node
-rw-------1root root8Nov1914:58resource2
-rw-------1root root131072Nov1914:58resource0
--w-------1root root4096Nov1914:58reset
--w--w----1root root4096Nov1914:58rescan
--w--w----1root root4096Nov1914:58remove
-rw-r--r--1root root4096Nov1914:58msi_bus
-r--r--r--1root root4096Nov1914:58modalias
-r--r--r--1root root4096Nov1914:58local_cpulist
-rw-------1root root4096Nov1914:58enable
-r--r--r--1root root4096Nov1914:58dma_mask_bits
-r--r--r--1root root4096Nov1914:58consistent_dma_mask_bits
-rw-r--r--1root root4096Nov1914:58broken_parity_status
drwxr-xr-x3root root0Nov1915:31uio
lrwxrwxrwx1root root0Nov1915:31driver -> ../../../bus/pci/drivers/igb_uio
-rw-r--r--1root root4096Nov1915:32max_vfs
![](http://common.cnblogs.com/images/copycode.gif)
目录名：就是PCI设备的地址，记录在struct rte_pci_addr数据结构中。
vendor文件：获取PCI_ID.vendor_id。
device文件：获取PCI_ID.device_id。
subsystem_vendor文件：获取PCI_ID.subsystem_vendor_id。
subsystem_device文件：获取PCI_ID.subsystem_device_id。
numa_node文件：获取PCI设备属于哪个CPU socket。
resource文件：获取PCI设备的在地址总线上的物理地址，以及物理地址空间的大小，记录在struct rte_pci_resouce数据结构中。
三、PCI驱动注册
调用rte_eal_init()--->rte_eal_dev_init()函数，遍历dev_driver_list链表，执行网卡驱动对应的init的回调函数，注册PCI驱动。
![](http://common.cnblogs.com/images/copycode.gif)
/*Once the vdevs are initalized, start calling all the pdev drivers*/
TAILQ_FOREACH(driver,&dev_driver_list, next) {
if(driver->type !=PMD_PDEV)
continue;
/*PDEV drivers don't get passed any parameters*/
driver->init(NULL, NULL);
}
![](http://common.cnblogs.com/images/copycode.gif)
以e1000网卡为例，执行的init回调函数就是rte_igb_pmd_init()函数。
staticint
rte_igb_pmd_init(constchar*name __rte_unused,constchar*params__rte_unused)
{
rte_eth_driver_register(&rte_igb_pmd);
return0;
}

rte_eth_driver_register()主要是指定PCI设备的初始化函数为rte_eth_dev_init()，以及注册PCI驱动，将PCI驱动挂到pci_driver_list全局链表上。
void
rte_eth_driver_register(structeth_driver *eth_drv)
{
eth_drv->pci_drv.devinit =rte_eth_dev_init;
rte_eal_pci_register(&eth_drv->pci_drv);
}

其中，rte_igb_pmd数据结构如下，指定e1000网卡的初始化函数是eth_igb_dev_init()。
![](http://common.cnblogs.com/images/copycode.gif)
staticstructeth_driver rte_igb_pmd ={
{
.name="rte_igb_pmd",
.id_table=pci_id_igb_map,
.drv_flags= RTE_PCI_DRV_NEED_MAPPING |RTE_PCI_DRV_INTR_LSC,
},
.eth_dev_init=eth_igb_dev_init,
.dev_private_size=sizeof(structe1000_adapter),
};
![](http://common.cnblogs.com/images/copycode.gif)
四、网卡初始化
调用rte_eal_init()--->rte_eal_pci_probe()函数，遍历pci_device_list和pci_driver_list链表，根据PCI_ID，将pci_device与pci_driver绑定，并调用pci_driver的init回调函数rte_eth_dev_init()，初始化PCI设备。
![](http://images.cnitblog.com/blog/681791/201411/201337002346881.jpg)
在rte_eal_pci_probe_one_driver()函数中，
1、首先通过比对PCI_ID的vendor_id、device_id、subsystem_vendor_id、subsystem_device_id四个字段判断pci设备和pci驱动是否匹配。
2、PCI设备和PCI驱动匹配后，调用pci_map_device()函数为该PCI设备创建map resource。具体如下：
> a、首先读取/sys/bus/pci/devices/PCI设备目录下的uio目录，获取uio设备的ID，该ID就是uio目录名最后几位的数字。当igb_uio模块与网卡设备绑定的时候，会在/sys/bus/pci/devices/对应的PCI设备目录下创建uio目录。

> 如果启动参数中指定了OPT_CREATE_UIO_DEV_NUM，会在/dev目录下创建对应uio设备的设备文件。
![](http://common.cnblogs.com/images/copycode.gif)
> root@Ubuntu:~\# ls -ltr /sys/bus/pci/devices/
> 0000
> \:
> 00
> \:
> 09.0
> /uio/

> total
> 0

> drwxr
> -xr-x
> 5
> root root
> 0
> Nov
> 19
> 15
> :
> 31
> uio0

> root@Ubuntu:
> ~\# ls -ltr /sys/bus/pci/devices/
> 0000
> \:
> 00
> \:0a.
> 0
> /uio/

> total
> 0

> drwxr
> -xr-x
> 5
> root root
> 0
> Nov
> 19
> 15
> :
> 31
> uio1

> root@Ubuntu:
> ~\#

![](http://common.cnblogs.com/images/copycode.gif)

> b、初始化PCI设备的中断句柄。

> rte_pci_device->intr_handler.fd = open(“/dev/uioID”, O_RDWR);
> /*
> ID为0或1,即uio0或uio1
> */

> rte_pci_device
> ->intr_handler.type = RTE_INTR_HANDLER_UIO;


> c、读取/sys/bus/pci/devices/0000\:00\:09.0/uio/uio0/maps/map0/目录下的文件，获取UIO设备的map resource。并将其记录在struct pci_map数据结构中。

> struct
> pci_map {

> void
> *
> addr;

> uint64_t offset;

> uint64_t size;

> uint64_t phaddr;

> };

![](http://common.cnblogs.com/images/copycode.gif)
> root@Ubuntu:~\# ls -ltr /sys/bus/pci/devices/
> 0000
> \:
> 00
> \:
> 09.0
> /uio/uio0/maps/

> total
> 0

> drwxr
> -xr-x
> 2
> root root
> 0
> Nov
> 19
> 15
> :
> 32
> map0

> root@Ubuntu:
> ~\# ls -ltr /sys/bus/pci/devices/
> 0000
> \:
> 00
> \:
> 09.0
> /uio/uio0/maps/map0/

> total
> 0

> -r--r--r--
> 1
> root root
> 4096
> Nov
> 19
> 15
> :
> 32
> size

> -r--r--r--
> 1
> root root
> 4096
> Nov
> 19
> 15
> :
> 32
> offset

> -r--r--r--
> 1
> root root
> 4096
> Nov
> 19
> 15
> :
> 32
> addr

> -r--r--r--
> 1
> root root
> 4096
> Nov
> 19
> 15
> :
> 34
> name

> root@Ubuntu:
> ~\#

![](http://common.cnblogs.com/images/copycode.gif)

> d、检查PCI设备和UIO设备在内存总线上的物理地址是否一致。如果一致，对/dev/uioID文件mmap一段内存空间，并将其记录在pci_map->addr和rte_pci_device->mem_resource[].addr中。

> root@Ubuntu:~\# cat /sys/bus/pci/devices/
> 0000
> \:
> 00
> \:
> 09.0
> /uio/uio0/maps/map0/
> addr

> 0xf0440000

> root@Ubuntu:
> ~\#

![](http://common.cnblogs.com/images/copycode.gif)
> root@Ubuntu:~\# cat /sys/bus/pci/devices/
> 0000
> \:
> 00
> \:
> 09.0
> /
> resource

> 0x00000000f0440000
> 0x00000000f045ffff
> 0x0000000000040200
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x000000000000d248
> 0x000000000000d24f
> 0x0000000000040101
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000
> 0x0000000000000000

> root@Ubuntu:
> ~\#

![](http://common.cnblogs.com/images/copycode.gif)

> e、将所有UIO设备的resource信息都记录在struct mapped_pci_resource数据结构中，并挂到全局链表pci_res_list上。
![](http://common.cnblogs.com/images/copycode.gif)
> struct
> mapped_pci_resource {

> TAILQ_ENTRY(uio_resource) next;


> struct
> rte_pci_addr pci_addr;

> char
> path[PATH_MAX];

> size_t nb_maps;

> struct
> uio_map maps[PCI_MAX_RESOURCE];

> };

![](http://common.cnblogs.com/images/copycode.gif)
3、调用rte_eth_dev_init()初始化PCI设备。
> a、首先，调用rte_eth_dev_allocate()在全局数组rte_eth_devices[]中分配一个网卡设备。并在全局数组rte_eth_dev_data[]中为网卡设备的数据域分配空间。

> eth_dev = &
> rte_eth_devices[nb_ports];

> eth_dev
> ->data = &rte_eth_dev_data[nb_ports];


> 并调用rte_zmalloc()为网卡设备的私有数据结构分配空间。

> rte_eth_dev->rte_eth_dev_data->dev_private = rte_zmalloc(
> sizeof
> (
> struct
> e1000_adapter));

> b、调用eth_igb_dev_init()初始化网卡设备。首先设置网卡设备的操作函数集，以及收包、发包函数。

> eth_dev->dev_ops = &
> eth_igb_ops;

> eth_dev
> ->rx_pkt_burst = &
> eth_igb_recv_pkts;

> eth_dev
> ->tx_pkt_burst = &eth_igb_xmit_pkts;


> 初始化网卡设备的硬件相关数据结构struct e1000_hw，包括设备ID、硬件操作函数集、在内存地址总线上映射的地址、MAC地址等等。

> c、注册中断处理函数。

> rte_intr_callback_register(&(pci_dev->
> intr_handle),

> eth_igb_interrupt_handler, (
> void
> *)eth_dev);

五、设备与驱动相互映射关系图
![](http://images.cnitblog.com/blog/681791/201411/201337014847166.jpg)
# 0x05 总结
造轮子去重写一个类似的DPDK uio驱动层，显然这样是没啥意义的，学习的目的还是多了解底层实现的一些东西，方便以后工作问题的处理。


