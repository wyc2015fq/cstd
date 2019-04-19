# ﻿﻿﻿﻿platform_device_系列函数及其设备注册的作用 - maopig的专栏 - CSDN博客
2012年03月30日 10:03:54[maopig](https://me.csdn.net/maopig)阅读数：6811
platform_device_系列函数，实际上是注册了一个叫platform的虚拟总线。使用约定是如果一个不属于任何总线的设备，例如蓝牙，串口等设备，都需要挂在这个虚拟总线上。
driver/base/platform.c
//platform设备声明
struct device platform_bus = {
    .bus_id        = "platform",
};
EXPORT_SYMBOL_GPL(platform_bus);
//platform总线设备声明
struct bus_type platform_bus_type = {
    .name        = "platform",
    .dev_attrs    = platform_dev_attrs,
    .match        = platform_match,
    .uevent        = platform_uevent,
    .suspend    = platform_suspend,
    .suspend_late    = platform_suspend_late,
    .resume_early    = platform_resume_early,
    .resume        = platform_resume,
};
EXPORT_SYMBOL_GPL(platform_bus_type);
int __init platform_bus_init(void)
{
    int error;
    error = device_register(&platform_bus);//注册了"platform"的设备
    if (error)
        return error;
    error = bus_register(&platform_bus_type);//注册了叫"platform"的总线
    if (error)
        device_unregister(&platform_bus);
    return error;
}
//这里在platform总线上挂设备
int platform_device_add(struct platform_device *pdev)
{
    int i, ret = 0;
    if (!pdev)
        return -EINVAL;
    if (!pdev->dev.parent)
        pdev->dev.parent = &platform_bus;//父设备设置为platform_bus
    pdev->dev.bus = &platform_bus_type;//设置挂在platform总线上
    if (pdev->id != -1)
        snprintf(pdev->dev.bus_id, BUS_ID_SIZE, "%s.%d", pdev->name,
             pdev->id);
    else
        strlcpy(pdev->dev.bus_id, pdev->name, BUS_ID_SIZE);
    for (i = 0; i < pdev->num_resources; i++) {
        struct resource *p, *r = &pdev->resource[i];
        if (r->name == NULL)
            r->name = pdev->dev.bus_id;
        p = r->parent;
        if (!p) {
            if (r->flags & IORESOURCE_MEM)
                p = &iomem_resource;
            else if (r->flags & IORESOURCE_IO)
                p = &ioport_resource;
        }
        if (p && insert_resource(p, r)) {
            printk(KERN_ERR
                   "%s: failed to claim resource %d\n",
                   pdev->dev.bus_id, i);
            ret = -EBUSY;
            goto failed;
        }
    }
    pr_debug("Registering platform device '%s'. Parent at %s\n",
         pdev->dev.bus_id, pdev->dev.parent->bus_id);
    ret = device_add(&pdev->dev);
    if (ret == 0)
        return ret;
failed:
    while (--i >= 0)
        if (pdev->resource[i].flags & (IORESOURCE_MEM|IORESOURCE_IO))
            release_resource(&pdev->resource[i]);
    return ret;
}
EXPORT_SYMBOL_GPL(platform_device_add);
//常用的platform_device_register，内部调用了platform_device_add，将设备挂在了platform总线上
/**
* platform_device_register - add a platform-level device
* @pdev: platform device we're adding
*/
int platform_device_register(struct platform_device *pdev)
{
    device_initialize(&pdev->dev);
    return platform_device_add(pdev);
}
EXPORT_SYMBOL_GPL(platform_device_register);
要用注册一个platform驱动的步骤：
1，注册设备platform_device_register
2，注册驱动platform_driver_register
注册时候的两个名字必须一样，才能match上，才能work,例如：
struct platform_device pxa3xx_device_nand = {
    .name        = "pxa3xx-nand",
    .id        = -1,
    .dev        = {
        .dma_mask = &pxa3xx_nand_dma_mask,
        .coherent_dma_mask = DMA_BIT_MASK(32),
    },
    .resource    = pxa3xx_resource_nand,
    .num_resources    = ARRAY_SIZE(pxa3xx_resource_nand),
};
static struct platform_driver pxa3xx_nand_driver = {
    .driver = {
        .name    = "pxa3xx-nand",
    },
    .probe        = pxa3xx_nand_probe,
    .remove        = pxa3xx_nand_remove,
#ifdef CONFIG_PM
    .suspend    = pxa3xx_nand_suspend,
    .resume        = pxa3xx_nand_resume,
#endif
};
而且device注册的时候，可以给driver传参数
struct device {
    struct klist        klist_children;
    struct klist_node    knode_parent;    /* node in sibling list */
    struct klist_node    knode_driver;
    struct klist_node    knode_bus;
    struct device        *parent;
    struct kobject kobj;
    char    bus_id[BUS_ID_SIZE];    /* position on parent bus */
    struct device_type    *type;
    unsigned        is_registered:1;
    unsigned        uevent_suppress:1;
    struct semaphore    sem;    /* semaphore to synchronize calls to
                     * its driver.
                     */
    struct bus_type    *bus;        /* type of bus device is on */
    struct device_driver *driver;    /* which driver has allocated this
                       device */
    void        *driver_data;    /* data private to the driver */
    void        *platform_data;    /* Platform specific data, device
                       core doesn't touch it */
    struct dev_pm_info    power;
#ifdef CONFIG_NUMA
    int        numa_node;    /* NUMA node this device is close to */
#endif
    u64        *dma_mask;    /* dma mask (if dma'able device) */
    u64        coherent_dma_mask;/* Like dma_mask, but for
                         alloc_coherent mappings as
                         not all hardware supports
                         64 bit addresses for consistent
                         allocations such descriptors. */
    struct device_dma_parameters *dma_parms;
    struct list_head    dma_pools;    /* dma pools (if dma'ble) */
    struct dma_coherent_mem    *dma_mem; /* internal for coherent mem
                         override */
    /* arch specific additions */
    struct dev_archdata    archdata;
    spinlock_t        devres_lock;
    struct list_head    devres_head;
    /* class_device migration path */
    struct list_head    node;
    struct class        *class;
    dev_t            devt;    /* dev_t, creates the sysfs "dev" */
    struct attribute_group    **groups;    /* optional groups */
    void    (*release)(struct device *dev);
};
传参数都是通过platform_data传，所以定义为void *
    void        *platform_data;    /* Platform specific data, device
static struct pxa3xx_nand_platform_data XXX_nand_info = {
    .parts            = android_256m_v75_partitions,
    .nr_parts        = ARRAY_SIZE(android_256m_v75_partitions),
};
static void __init XXX_init_nand(void)
{
    pxa3xx_device_nand.dev.platform_data = &XXX_nand_info;
    platform_device_register(&pxa3xx_device_nand);
}
static int __init pxa3xx_nand_probe(struct platform_device *pdev)
{
    struct pxa3xx_nand_platform_data *pdata;
    struct nand_chip *this;
    struct pxa3xx_nand_info *info;
    struct resource *res;
    struct clk *clk = NULL, *smc_clk = NULL;
    int status = -1;
    struct mtd_partition *parts;
    unsigned int data_buf_len;
#ifdef CONFIG_MTD_NAND_PXA3xx_DMA
    unsigned int buf_len;
#endif
    int i, ret = 0;
#ifdef CONFIG_MTD_PARTITIONS
    int err;
#endif
    pdata = pdev->dev.platform_data;
....
....
....
}
下面解释一下pxa_register_device函数
    pxa_set_ohci_info(&XXX_ohci_info);
void __init pxa_set_ohci_info(struct pxaohci_platform_data *info)
{
    pxa_register_device(&pxa27x_device_ohci, info);
}
void __init pxa_register_device(struct platform_device *dev, void *data)
{
    int ret;
    dev->dev.platform_data = data;
    ret = platform_device_register(dev);
    if (ret)
        dev_err(&dev->dev, "unable to register device: %d\n", ret);
}
其实上，也就是给driver传参数，通过dev.platform_data。
到这里，platform_device系列函数，基本算通了，系列函数还有一堆设置的函数，和device_register同级别的那些功能函数，用法基本差不多，只不过都将设备挂在了platform总线上。
platform_device_register向系统注册设备
platform_driver_register向系统注册驱动，过程中在系统寻找注册的设备（根据.name)，找到后运行.probe进行初始化。
***************************************************************
device_register()和platform_device_register()的区别(转载)  
首先看device和platform_device区别
由struct platform_device {
const char * name; //NOTE:此处设备的命名应和相应驱动程序命名一致
u32 id;            //以实现driver binding
struct device dev;
u32 num_resources;
struct resource * resource;
};
可知：platform_device由device派生而来
内核中关于Platform devices的注释
Platform devices
~~~~~~~~~~~~~~~~
Platform devices are devices that typically appear as autonomous
entities in the system. This includes legacy port-based devices and
host bridges to peripheral buses, and most controllers integrated
into system-on-chip platforms. What they usually have in common
is direct addressing from a CPU bus. Rarely, a platform_device will
be connected through a segment of some other kind of bus; but its
registers will still be directly addressable.
Platform devices are given a name, used in driver binding, and a
list of resources such as addresses and IRQs.
个人理解：Platform devices是相对独立的设备，拥有各自独自的资源（addresses and IRQs）
一个Platform devices实例：
static struct platform_device *smdk2410_devices[] __initdata = {
&s3c_device_usb, //片上的各个设备
&s3c_device_lcd, //下面以s3c_device_lcd为例
&s3c_device_wdt,
&s3c_device_i2c,
&s3c_device_iis,
};
/* LCD Controller */
static struct resource s3c_lcd_resource[] = { //LCD的两个资源
[0] = {
.start = S3C2410_PA_LCD,
.end = S3C2410_PA_LCD + S3C2410_SZ_LCD,
.flags = IORESOURCE_MEM,
},
[1] = {
.start = IRQ_LCD,
.end = IRQ_LCD,
.flags = IORESOURCE_IRQ,
}
};
struct platform_device s3c_device_lcd = {//s3c_device_lcd设备
.name = "s3c2410-lcd",
.id = -1,
.num_resources = ARRAY_SIZE(s3c_lcd_resource),
.resource = s3c_lcd_resource,
.dev = { //device实例
.dma_mask = &s3c_device_lcd_dmamask,
.coherent_dma_mask = 0xffffffffUL
}
};
s3c_device_lcd的resource中硬件地址:
#define S3C2410_LCDREG(x) ((x) + S3C2410_VA_LCD)
/* LCD control registers */
#define S3C2410_LCDCON1 S3C2410_LCDREG(0x00)
#define S3C2410_LCDCON2 S3C2410_LCDREG(0x04)
#define S3C2410_LCDCON3 S3C2410_LCDREG(0x08)
#define S3C2410_LCDCON4 S3C2410_LCDREG(0x0C)
#define S3C2410_LCDCON5 S3C2410_LCDREG(0x10)
#define S3C2410_LCDCON1_CLKVAL(x) ((x) << 8)
#define S3C2410_LCDCON1_MMODE (1<<7)
#define S3C2410_LCDCON1_DSCAN4 (0<<5)
#define S3C2410_LCDCON1_STN4 (1<<5)
#define S3C2410_LCDCON1_STN8 (2<<5)
#define S3C2410_LCDCON1_TFT (3<<5)
－－－－－－－－－－－－－－－－－－－－－－－－－－
#define S3C2410_ADDR(x) (0xF0000000 + (x))
/* LCD controller */
#define S3C2410_VA_LCD S3C2410_ADDR(0x00600000)
#define S3C2410_PA_LCD (0x4D000000)
#define S3C2410_SZ_LCD SZ_1M
再分析device_register()和platform_device_register()的实现代码：
device_register()－－－－－－－－－－－－－－－－－－－－－－－－
/**
* device_register - register a device with the system.
* @dev: pointer to the device structure
*
* This happens in two clean steps - initialize the device
* and add it to the system. The two steps can be called
* separately, but this is the easiest and most common.
* I.e. you should only call the two helpers separately if
* have a clearly defined need to use and refcount the device
* before it is added to the hierarchy.
*/
int device_register(struct device *dev)
{
device_initialize(dev); //初始化设备结构
return device_add(dev); //添加设备到设备层
}
platform_device_register()－－－－－－－－－－－－－－－－－－－－
/**
* platform_device_register - add a platform-level device
* @pdev: platform device we're adding
*
*/
int platform_device_register(struct platform_device * pdev)
{
device_initialize(&pdev->dev); //初始化设备结构
return platform_device_add(pdev); //添加一个片上的设备到设备层
}
由以上函数可知：device_register()和platform_device_register()都会首先初始化设备
区别在于第二步：其实platform_device_add()包括device_add(),只不过要先注册resources
platform_device_add()－－－－－－－－－－－－－－－－－－－－－－
/**
* platform_device_add - add a platform device to device hierarchy
* @pdev: platform device we're adding
*
* This is part 2 of platform_device_register(), though may be called
* separately _iff_ pdev was allocated by platform_device_alloc().
*/
int platform_device_add(struct platform_device *pdev)
{
int i, ret = 0;
if (!pdev)
return -EINVAL;
if (!pdev->dev.parent)
pdev->dev.parent = &platform_bus;
pdev->dev.bus = &platform_bus_type;
/*＋＋＋＋＋＋＋＋＋＋＋＋＋＋
The platform_device.dev.bus_id is the canonical name for the devices.
It's built from two components:
* platform_device.name ... which is also used to for driver matching.
* platform_device.id ... the device instance number, or else "-1"
to indicate there's only one.
These are concatenated, so name/id "serial"/0 indicates bus_id "serial.0", and
"serial/3" indicates bus_id "serial.3"; both would use the platform_driver
named "serial". While "my_rtc"/-1 would be bus_id "my_rtc" (no instance id)
and use the platform_driver called "my_rtc".
＋＋＋＋＋＋＋＋＋＋＋＋＋＋*/
if (pdev->id != -1)
snprintf(pdev->dev.bus_id, BUS_ID_SIZE, "%s.%u", pdev->name, pdev->id);
else //"-1" indicate there's only one
strlcpy(pdev->dev.bus_id, pdev->name, BUS_ID_SIZE);
for (i = 0; i < pdev->num_resources; i++) { //遍历设备资源个数，如LCD的两个资源：控制器和IRQ
struct resource *p, *r = &pdev->resource[i];
if (r->name == NULL)
r->name = pdev->dev.bus_id;
p = r->parent;
if (!p) { //resources分为两种IORESOURCE_MEM和IORESOURCE_IO
          //CPU对外设IO端口物理地址的编址方式有两种：I/O映射方式和内存映射方式
if (r->flags & IORESOURCE_MEM)
p = &iomem_resource;
else if (r->flags & IORESOURCE_IO)
p = &ioport_resource;
}
if (p && insert_resource(p, r)) {
printk(KERN_ERR
"%s: failed to claim resource %d/n",
pdev->dev.bus_id, i);
ret = -EBUSY;
goto failed;
}
}
pr_debug("Registering platform device '%s'. Parent at %s/n",
pdev->dev.bus_id, pdev->dev.parent->bus_id);
ret = device_add(&pdev->dev);
if (ret == 0)
return ret;
failed:
while (--i >= 0)
if (pdev->resource[i].flags & (IORESOURCE_MEM|IORESOURCE_IO))
release_resource(&pdev->resource[i]);
return ret;
}
相关参考＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋
device_initialize()－－－－－－－－－－－－－－－－－－
/** </drivers/base/core.c>
* device_initialize - init device structure.
* @dev: device.
*
* This prepares the device for use by other layers,
* including adding it to the device hierarchy.
* It is the first half of device_register(), if called by
* that, though it can also be called separately, so one
* may use @dev's fields (e.g. the refcount).
*/
void device_initialize(struct device *dev)
{
kobj_set_kset_s(dev, devices_subsys);
kobject_init(&dev->kobj);
klist_init(&dev->klist_children, klist_children_get,
klist_children_put);
INIT_LIST_HEAD(&dev->dma_pools);
INIT_LIST_HEAD(&dev->node);
init_MUTEX(&dev->sem);
spin_lock_init(&dev->devres_lock);
INIT_LIST_HEAD(&dev->devres_head);
device_init_wakeup(dev, 0);
set_dev_node(dev, -1);
}
device_add(struct device *dev)－－－－－－－－－－－－－
/**
* device_add - add device to device hierarchy.
* @dev: device.
*
* This is part 2 of device_register(), though may be called
* separately _iff_ device_initialize() has been called separately.
*
* This adds it to the kobject hierarchy via kobject_add(), adds it
* to the global and sibling lists for the device, then
* adds it to the other relevant subsystems of the driver model.
*/
结构体resource－－－－－－－－－－－－－－－－－－－－－－
/* < /include/linux/ioport.h>
* Resources are tree-like, allowing
* nesting etc..
*/
struct resource {
resource_size_t start;
resource_size_t end;
const char *name;
unsigned long flags;
struct resource *parent, *sibling, *child;
};
－－－－－－－－－－－－－－－－－－－－－－－－－－－
原文地址:[http://blog.chinaunix.net/u1/58968/showart_467998.html](http://blog.chinaunix.net/u1/58968/showart_467998.html) ,
在8250.c(driver/serial/8250.c)的初始化函数serial8250_init（）中，给出了一个很简单的例子
static struct platform_device *serial8250_isa_devs;
......
//create a platform_device 
serial8250_isa_devs = platform_device_alloc("serial8250",PLAT8250_DEV_LEGACY);              
  platform_device_add(serial8250_isa_devs);   //add the platform_device to system
platform_driver_register(&serial8250_isa_driver);//then register the platform_driver        
还有另外一个比较类似的比较，就是driver_register和platform_driver_register的比较
platform_driver_register(&xx_driver) 会向系统注册xx_driver这个驱动程序，这个函数会根据 xx_driver中的.name内容，搜索系统注册的device中有没有这个platform_device,如果有，就会执行 platform_driver(也就是xx_driver的类型)中的.probe函数。
对只需要初始化运行一次的函数都加上__init属性，__init 宏告诉编译器如果这个模块被编译到内核则把这个函数放到（.init.text）段，module_exit的参数卸载时同__init类似，如果驱动被编译进内核，则__exit宏会忽略清理函数，因为编译进内核的模块不需要做清理工作，显然__init和__exit对动态加载的模块是无效的，只支持完全编译进内核。
