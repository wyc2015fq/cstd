# Linux设备驱动程序学习——USB 驱动程序（三） - h13 - 博客园
（本部分的一些示例源码来自drivers/usb/usb-skeleton.c，它是Linux内核为我们提供的最基础的USB驱动程序，USB骨架程序）
驱动程序把驱动对象注册到 USB 子系统中，之后使用供应商(idVendor)和设备(idProduct)标识来判断对应的硬件是否已经安装.
**驱动的设备支持列表**
struct usb_device_id 结构提供了这个驱动支持的不同类型 USB 设备的列表. USB 核心通过此列表用来决定设备对应的驱动,热插拔脚本也通过此列表来决定当特定设备被插入系统时,应该自动加载的驱动.
```
struct usb_device_id {
    /* 确定设备信息去和结构体中哪几个字段匹配来判断驱动的适用性 */
    __u16        match_flags;
    /* Used for product specific matches; range is inclusive */
    __u16        idVendor;    //USB设备的制造商ID，须向[www.usb.org](http://www.cnblogs.cc2/link.php?url=http://www.usb.org%2F)申请
    __u16        idProduct;    //USB设备的产品ID，有制造商自定
    __u16        bcdDevice_lo;    /* USB设备的产品版本号最低值*/
    __u16        bcdDevice_hi;    /* 和最高值，以BCD码来表示。*/
    /* 分别定义设备的类,子类和协议,他们由 USB 论坛分配并定义在 USB 规范中. 这些值指定这个设备的行为, 包括设备上所有的接口 */
    __u8        bDeviceClass;    
    __u8        bDeviceSubClass;
    __u8        bDeviceProtocol;
    /* 分别定义单个接口的类,子类和协议,他们由 USB 论坛分配并定义在 USB 规范中 */
    __u8        bInterfaceClass;
    __u8        bInterfaceSubClass;
    __u8        bInterfaceProtocol;
    /* 这个值不用来匹配驱动的, 驱动用它来在 USB 驱动的探测回调函数中区分不同的设备 */
    kernel_ulong_t    driver_info;
};
//以上结构体中__u16 match_flags;所使用的define:
//include/linux/mod_devicetable.h
/* Some useful macros to use to create struct usb_device_id */
#define USB_DEVICE_ID_MATCH_VENDOR        0x0001
#define USB_DEVICE_ID_MATCH_PRODUCT        0x0002
#define USB_DEVICE_ID_MATCH_DEV_LO        0x0004
#define USB_DEVICE_ID_MATCH_DEV_HI        0x0008
#define USB_DEVICE_ID_MATCH_DEV_CLASS        0x0010
#define USB_DEVICE_ID_MATCH_DEV_SUBCLASS    0x0020
#define USB_DEVICE_ID_MATCH_DEV_PROTOCOL    0x0040
#define USB_DEVICE_ID_MATCH_INT_CLASS        0x0080
#define USB_DEVICE_ID_MATCH_INT_SUBCLASS    0x0100
#define USB_DEVICE_ID_MATCH_INT_PROTOCOL    0x0200
//include/linux/usb.h
#define USB_DEVICE_ID_MATCH_DEVICE \
        (USB_DEVICE_ID_MATCH_VENDOR | USB_DEVICE_ID_MATCH_PRODUCT)
#define USB_DEVICE_ID_MATCH_DEV_RANGE \
        (USB_DEVICE_ID_MATCH_DEV_LO | USB_DEVICE_ID_MATCH_DEV_HI)
#define USB_DEVICE_ID_MATCH_DEVICE_AND_VERSION \
        (USB_DEVICE_ID_MATCH_DEVICE | USB_DEVICE_ID_MATCH_DEV_RANGE)
#define USB_DEVICE_ID_MATCH_DEV_INFO \
        (USB_DEVICE_ID_MATCH_DEV_CLASS | \
        USB_DEVICE_ID_MATCH_DEV_SUBCLASS | \
        USB_DEVICE_ID_MATCH_DEV_PROTOCOL)
#define USB_DEVICE_ID_MATCH_INT_INFO \
        (USB_DEVICE_ID_MATCH_INT_CLASS | \
        USB_DEVICE_ID_MATCH_INT_SUBCLASS | \
        USB_DEVICE_ID_MATCH_INT_PROTOCOL)
//这个结构体一般不用手动赋值，以下的宏可以实现赋值:
/**
 * USB_DEVICE - macro used to describe a specific usb device
 * @vend: the 16 bit USB Vendor ID
 * @prod: the 16 bit USB Product ID
 *
 * This macro is used to create a struct usb_device_id that matches a
 * specific device.
 */
//仅和指定的制造商和产品ID匹配，用于需要特定驱动的设备
#define USB_DEVICE(vend,prod) \
    .match_flags = USB_DEVICE_ID_MATCH_DEVICE, \
    .idVendor = (vend), \
    .idProduct = (prod)
/**
 * USB_DEVICE_VER - describe a specific usb device with a version range
 * @vend: the 16 bit USB Vendor ID
 * @prod: the 16 bit USB Product ID
 * @lo: the bcdDevice_lo value
 * @hi: the bcdDevice_hi value
 *
 * This macro is used to create a struct usb_device_id that matches a
 * specific device, with a version range.
 */
//仅和某版本范围内的指定的制造商和产品ID匹配
#define USB_DEVICE_VER(vend, prod, lo, hi) \
    .match_flags = USB_DEVICE_ID_MATCH_DEVICE_AND_VERSION, \
    .idVendor = (vend), \
    .idProduct = (prod), \
    .bcdDevice_lo = (lo), \
    .bcdDevice_hi = (hi)
/**
 * USB_DEVICE_INTERFACE_PROTOCOL - describe a usb device with a specific interface protocol
 * @vend: the 16 bit USB Vendor ID
 * @prod: the 16 bit USB Product ID
 * @pr: bInterfaceProtocol value
 *
 * This macro is used to create a struct usb_device_id that matches a
 * specific interface protocol of devices.
 */
//仅和指定的接口协议、制造商和产品ID匹配
#define USB_DEVICE_INTERFACE_PROTOCOL(vend, prod, pr) \
    .match_flags = USB_DEVICE_ID_MATCH_DEVICE | \
         USB_DEVICE_ID_MATCH_INT_PROTOCOL, \
    .idVendor = (vend), \
    .idProduct = (prod), \
    .bInterfaceProtocol = (pr)
/**
 * USB_DEVICE_INFO - macro used to describe a class of usb devices
 * @cl: bDeviceClass value
 * @sc: bDeviceSubClass value
 * @pr: bDeviceProtocol value
 *
 * This macro is used to create a struct usb_device_id that matches a
 * specific class of devices.
 */
//仅和指定的设备类型相匹配
#define USB_DEVICE_INFO(cl, sc, pr) \
    .match_flags = USB_DEVICE_ID_MATCH_DEV_INFO, \
    .bDeviceClass = (cl), \
    .bDeviceSubClass = (sc), \
    .bDeviceProtocol = (pr)
/**
 * USB_INTERFACE_INFO - macro used to describe a class of usb interfaces
 * @cl: bInterfaceClass value
 * @sc: bInterfaceSubClass value
 * @pr: bInterfaceProtocol value
 *
 * This macro is used to create a struct usb_device_id that matches a
 * specific class of interfaces.
 */
//仅和指定的接口类型相匹配
#define USB_INTERFACE_INFO(cl, sc, pr) \
    .match_flags = USB_DEVICE_ID_MATCH_INT_INFO, \
    .bInterfaceClass = (cl), \
    .bInterfaceSubClass = (sc), \
    .bInterfaceProtocol = (pr)
/**
 * USB_DEVICE_AND_INTERFACE_INFO - describe a specific usb device with a class of usb interfaces
 * @vend: the 16 bit USB Vendor ID
 * @prod: the 16 bit USB Product ID
 * @cl: bInterfaceClass value
 * @sc: bInterfaceSubClass value
 * @pr: bInterfaceProtocol value
 *
 * This macro is used to create a struct usb_device_id that matches a
 * specific device with a specific class of interfaces.
 *
 * This is especially useful when explicitly matching devices that have
 * vendor specific bDeviceClass values, but standards-compliant interfaces.
 */
//仅和指定的制造商、产品ID和接口类型相匹配
#define USB_DEVICE_AND_INTERFACE_INFO(vend, prod, cl, sc, pr) \
    .match_flags = USB_DEVICE_ID_MATCH_INT_INFO \
        | USB_DEVICE_ID_MATCH_DEVICE, \
    .idVendor = (vend), \
    .idProduct = (prod), \
    .bInterfaceClass = (cl), \
    .bInterfaceSubClass = (sc), \
    .bInterfaceProtocol = (pr)
/* ----------------------------------------------------------------------- */
```
对于一个只为一个供应商的一个 USB 设备的简单 USB 设备驱动, 其 struct usb_device_id 可定义如下:
```
/* Define these values to match your devices */
#define USB_SKEL_VENDOR_ID    0xfff0
#define USB_SKEL_PRODUCT_ID    0xfff0
/* table of devices that work with this driver */
static struct usb_device_id skel_table [] = {
    { USB_DEVICE(USB_SKEL_VENDOR_ID, USB_SKEL_PRODUCT_ID) },
    { }                    /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, skel_table);
```
MODULE_DEVICE_TABLE 宏是必需的，它允许用户空间工具判断该驱动可控制什么设备. 对于 USB 驱动, 这个宏中的第一个值必须是 usb .
如果你需要这个驱动被系统中每个 USB 设备调用, 创建仅需设置 driver_info 成员: 
```
static struct usb_device_id usb_ids[] = {
 {.driver_info = 42},
    {} 
};
```
**注册USB驱动程序**
所有 USB 驱动都必须创建的主要结构是 struct usb_driver. 这个结构必须被 USB 驱动程序手动填充并且包含多个回调函数和变量, 并向 USB 核心描述 USB 驱动程序:

```
struct usb_driver {
    const char *name;    
    /*指向驱动程序名字的指针. 它必须在内核所有的 USB 驱动中是唯一的(通常被设为和驱动模块名相同).当驱动在内核中运行时,会出现在/sys/bus/usb/drivers目录中 */
    int (*probe) (struct usb_interface *intf,
         const struct usb_device_id *id);
    /*指向 USB 驱动中探测函数指针. 当USB 核心认为它有一个本驱动可处理的 struct usb_interface时此函数将被调用. USB 核心用来做判断的 struct usb_device_id 指针也被传递给此函数.如果这个 USB 驱动确认传递给它的 struct usb_interface, 它应当正确地初始化设备并返回 0. 如果驱动没有确认这个设备或发生错误,则返回负错误值 */
    void (*disconnect) (struct usb_interface *intf);
    /*指向 USB 驱动的断开函数指针.当 struct usb_interface 从系统中清除或驱动 USB 核心卸载时,函数将被 USB 核心调用*/
    int (*ioctl) (struct usb_interface *intf, unsigned int code,
            void *buf);
    /*指向 USB 驱动的 ioctl 函数指针. 若此函数存在, 在用户空间程序对usbfs 文件系统关联的设备调用 ioctl 时,此函数将被调用. 实际上,当前只有 USB 集线器驱动使用这个 ioctl*/
    int (*suspend) (struct usb_interface *intf, pm_message_t message);
    /*指向 USB 驱动中挂起函数的指针*/
    int (*resume) (struct usb_interface *intf);
    /*指向 USB 驱动中恢复函数的指针*/
    int (*reset_resume)(struct usb_interface *intf);
    /*要复位一个已经被挂起的USB设备时调用此函数*/
    int (*pre_reset)(struct usb_interface *intf);
    /*在设备被复位之前由usb_reset_composite_device()调用*/
    int (*post_reset)(struct usb_interface *intf);
    /*在设备被复位之后由usb_reset_composite_device()调用*/
    const struct usb_device_id *id_table;
    /*指向 struct usb_device_id 表的指针*/
    struct usb_dynids dynids;
    struct usbdrv_wrap drvwrap;
    /*是struct device_driver driver的再包装,struct device_driver 包含 struct module *owner;*/
    unsigned int no_dynamic_id:1;
    unsigned int supports_autosuspend:1;
    unsigned int soft_unbind:1;
};
#define    to_usb_driver(d) container_of(d, struct usb_driver, drvwrap.driver)
```
创建一个简单的 struct usb_driver 结构, 只有 4 个成员需要初始化:
```
static struct usb_driver skel_driver = {
 .name = "skeleton",
 .id_table = skel_table,
 .probe = skel_probe,
 .disconnect = skel_disconnect, 
};
```
```
//向 USB 核心注册 struct usb_driver 
static int __init usb_skel_init(void)
{
        int result;
        /* register this driver with the USB subsystem */
        result = usb_register(&skel_driver);
        if (result)
                err("usb_register failed. Error number %d", result);
        return result;
}
/*当 USB 驱动被卸载, struct usb_driver 需要从内核注销(代码如下). 当以下调用发生, 当前绑定到这个驱动的任何 USB 接口将会断开, 并调用断开函数*/
static void __exit usb_skel_exit(void)
{
        /* deregister this driver with the USB subsystem */
        usb_deregister(&skel_driver);
}
```
**探测和断开的细节**
在 struct usb_driver 结构中, 有 2 个 USB 核心在适当的时候调用的函数：
(1)当设备安装时, 如果 USB 核心认为这个驱动可以处理，则调用探测（probe）函数，探测函数检查传递给它的设备信息, 并判断驱动是否真正合适这个设备.
(2)由于某些原因，设备被移除或驱动不再控制设备时，调用断开（disconnect）函数，做适当清理.
探测和断开回调函数都在 USB 集线器内核线程上下文中被调用, 因此它们休眠是合法的. 为了缩短 USB 探测时间，大部分工作尽可能在设备打开时完成.这是因为 USB 核心是在一个线程中处理 USB 设备的添加和移除, 因此任何慢设备驱动都可能使 USB 设备探测时间变长。
**   探测函数分析**
在探测回调函数中, USB 驱动应当初始化它可能用来管理 USB 设备的所有本地结构并保存所有需要的设备信息到本地结构, 因为在此时做这些通常更容易.为了和设备通讯，USB 驱动通常要探测设备的端点地址和缓冲大小. 以下是usb-skeleton的probe函数中的探测代码：
```
/* set up the endpoint information */
    /* use only the first bulk-in and bulk-out endpoints */
    iface_desc = interface->cur_altsetting;        //从输入的interface中提取当前接口的端点总数
    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {        
    /*轮询所有端点*/
        endpoint = &iface_desc->endpoint[i].desc;    //获得端点的数据结构指针
        if (!dev->bulk_in_endpointAddr &&
         usb_endpoint_is_bulk_in(endpoint)) {    //如果是批量输入端点，
            /* we found a bulk in endpoint */
            buffer_size = le16_to_cpu(endpoint->wMaxPacketSize);
            dev->bulk_in_size = buffer_size;    //获得端点大小
            dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;    //获得端点地址
            dev->bulk_in_buffer = kmalloc(buffer_size, GFP_KERNEL);    //为此端点创建缓冲区
            if (!dev->bulk_in_buffer) {
                err("Could not allocate bulk_in_buffer");
                goto error;
            }
        }
        if (!dev->bulk_out_endpointAddr &&
         usb_endpoint_is_bulk_out(endpoint)) {    如果是批量输出端点
            /* we found a bulk out endpoint */
            dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;    //获得端点地址
        }
    }
    if (!(dev->bulk_in_endpointAddr && dev->bulk_out_endpointAddr)) {    //如果不是这两种端点，报错
        err("Could not find both bulk-in and bulk-out endpoints");
        goto error;
    }
//这里端点判断的函数给我们的编程带来了方便：
/*-------------------------------------------------------------------------*/
/**
 * usb_endpoint_num - get the endpoint's number
 * @epd: endpoint to be checked
 *
 * Returns @epd's number: 0 to 15.
 */
static inline int usb_endpoint_num(const struct usb_endpoint_descriptor *epd)
{
    return epd->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
}
/**
 * usb_endpoint_type - get the endpoint's transfer type
 * @epd: endpoint to be checked
 *
 * Returns one of USB_ENDPOINT_XFER_{CONTROL, ISOC, BULK, INT} according
 * to @epd's transfer type.
 */
static inline int usb_endpoint_type(const struct usb_endpoint_descriptor *epd)
{
    return epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;
}
/**
 * usb_endpoint_dir_in - check if the endpoint has IN direction
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type IN, otherwise it returns false.
 */
static inline int usb_endpoint_dir_in(const struct usb_endpoint_descriptor *epd)
{
    return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN);
}
/**
 * usb_endpoint_dir_out - check if the endpoint has OUT direction
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type OUT, otherwise it returns false.
 */
static inline int usb_endpoint_dir_out(
                const struct usb_endpoint_descriptor *epd)
{
    return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT);
}
/**
 * usb_endpoint_xfer_bulk - check if the endpoint has bulk transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type bulk, otherwise it returns false.
 */
static inline int usb_endpoint_xfer_bulk(
                const struct usb_endpoint_descriptor *epd)
{
    return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
        USB_ENDPOINT_XFER_BULK);
}
/**
 * usb_endpoint_xfer_control - check if the endpoint has control transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type control, otherwise it returns false.
 */
static inline int usb_endpoint_xfer_control(
                const struct usb_endpoint_descriptor *epd)
{
    return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
        USB_ENDPOINT_XFER_CONTROL);
}
/**
 * usb_endpoint_xfer_int - check if the endpoint has interrupt transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type interrupt, otherwise it returns
 * false.
 */
static inline int usb_endpoint_xfer_int(
                const struct usb_endpoint_descriptor *epd)
{
    return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
        USB_ENDPOINT_XFER_INT);
}
/**
 * usb_endpoint_xfer_isoc - check if the endpoint has isochronous transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type isochronous, otherwise it returns
 * false.
 */
static inline int usb_endpoint_xfer_isoc(
                const struct usb_endpoint_descriptor *epd)
{
    return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
        USB_ENDPOINT_XFER_ISOC);
}
/**
 * usb_endpoint_is_bulk_in - check if the endpoint is bulk IN
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has bulk transfer type and IN direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_bulk_in(
                const struct usb_endpoint_descriptor *epd)
{
    return (usb_endpoint_xfer_bulk(epd) && usb_endpoint_dir_in(epd));
}
/**
 * usb_endpoint_is_bulk_out - check if the endpoint is bulk OUT
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has bulk transfer type and OUT direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_bulk_out(
                const struct usb_endpoint_descriptor *epd)
{
    return (usb_endpoint_xfer_bulk(epd) && usb_endpoint_dir_out(epd));
}
/**
 * usb_endpoint_is_int_in - check if the endpoint is interrupt IN
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has interrupt transfer type and IN direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_int_in(
                const struct usb_endpoint_descriptor *epd)
{
    return (usb_endpoint_xfer_int(epd) && usb_endpoint_dir_in(epd));
}
/**
 * usb_endpoint_is_int_out - check if the endpoint is interrupt OUT
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has interrupt transfer type and OUT direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_int_out(
                const struct usb_endpoint_descriptor *epd)
{
    return (usb_endpoint_xfer_int(epd) && usb_endpoint_dir_out(epd));
}
/**
 * usb_endpoint_is_isoc_in - check if the endpoint is isochronous IN
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has isochronous transfer type and IN direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_isoc_in(
                const struct usb_endpoint_descriptor *epd)
{
    return (usb_endpoint_xfer_isoc(epd) && usb_endpoint_dir_in(epd));
}
/**
 * usb_endpoint_is_isoc_out - check if the endpoint is isochronous OUT
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has isochronous transfer type and OUT direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_isoc_out(
                const struct usb_endpoint_descriptor *epd)
{
    return (usb_endpoint_xfer_isoc(epd) && usb_endpoint_dir_out(epd));
}
/*-------------------------------------------------------------------------*/
```
在设备注册之后，USB 驱动的后续操作都是通过struct usb_interface 获得设备的端点信息，所以要使用 usb_set_intfdata 将前面获得的端点信息保存到struct usb_interface 下的struct device 中的void *driver_data;中，以方便以后的操作。在usb-skeleton的probe函数中的代码： 
```
/* save our data pointer in this interface device */
    usb_set_intfdata(interface, dev);
```
之后在USB的驱动程序中的打开函数和断开函数中调用usb_get_intfdata来获取端点数据。由于这 2 个函数, USB 驱动不需要为系统中所有当前的设备各保持一个静态指针数组来保存单个设备结构. 对设备信息的非直接引用使得任何 USB 驱动都支持不限数量的设备.
若这个 USB 驱动没有和另一种处理用户和设备交互的子系统(如 input, tty, video......)关联, 驱动可使用 USB 主设备号，以便在用户空间使用传统的字符驱动接口. 为此, USB 驱动必须在探测函数中调用 usb_register_dev 函数, 以注册一个设备到 USB 核心. 在usb-skeleton的probe函数中的代码： 
```
/* we can register the device now, as it is ready */
    retval = usb_register_dev(interface, &skel_class);
    if (retval) {
        /* something prevented us from registering this driver */
        err("Not able to get a minor for this device.");
        usb_set_intfdata(interface, NULL);
        goto error;
    }
//其中使用到的 struct usb_class_driver 结构体如下：
/**
 * struct usb_class_driver - identifies a USB driver that wants to use the USB major number
 * @name: the usb class device name for this driver. Will show up in sysfs.
 * @fops: pointer to the struct file_operations of this driver.
 * @minor_base: the start of the minor range for this driver.
 *
 * This structure is used for the usb_register_dev() and
 * usb_unregister_dev() functions, to consolidate a number of the
 * parameters used for them.
 */
struct usb_class_driver {
    char *name;    //sysfs 用来描述设备的名字
    const struct file_operations *fops;    // struct file_operations 结构指针, 驱动定义来注册为字符设备
    int minor_base;
    /*给这个驱动安排的次设备号的起始. 所有和这个驱动相关的设备被创建为从这个值开始的唯一的, 递增的次设备号. 只有 16 个设备被允许在任何时刻和这个驱动关联, 除非 CONFIG_USB_DYNAMIC_MINORS 配置选项被打开. 如
果这样, 忽略这个变量, 并且这个设备的所有的次设备号会以先来先服务的方式分配. 建议打开了这个选项的系统使用类似 udev 的程序来产生系统中的设备节点, 因为一个静态的 /dev 树不会正确工作.*/
};
//而在usb-skeleton的设置如下：
static const struct file_operations skel_fops = {
    .owner =    THIS_MODULE,
    .read =        skel_read,
    .write =    skel_write,
    .open =        skel_open,
    .release =    skel_release,
    .flush =    skel_flush,
};
/*
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with the driver core
 */
static struct usb_class_driver skel_class = {
    .name =        "skel%d",
    .fops =        &skel_fops,
    .minor_base =    USB_SKEL_MINOR_BASE,
};
```
当 USB 设备断开, 所有关联到这个设备的资源都应被释放,如果已在探测函数中调用 usb_register_dev 分配了 USB 设备的次设备号, 必须调用函数 usb_deregister_dev 来将次设备号还回 USB 核心.在断开函数中, 需要从接口获取之前调用 usb_set_intfdata 所设置的数据，然后设置struct usb_interface 结构指针为 NULL，以防止错误的访问.而在usb-skeleton的源码如下： 
```
static void skel_disconnect(struct usb_interface *interface)
{
    struct usb_skel *dev;
    int minor = interface->minor;
    dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);
    /* give back our minor */
    usb_deregister_dev(interface, &skel_class);
    /* prevent more I/O from starting */
    mutex_lock(&dev->io_mutex);
    dev->interface = NULL;
    mutex_unlock(&dev->io_mutex);
    usb_kill_anchored_urbs(&dev->submitted);
    /* decrement our usage count */
    kref_put(&dev->kref, skel_delete);
    info("USB Skeleton #%d now disconnected", minor);
}
```
当一个 USB 设备调用 disconnect 函数时, 所有当前正被传送的 urb 可自动被 USB 核心取消, 不必显式调用 usb_kill_urb. 在USB设备被断开之后，如果驱动试图调用 usb_submit_urb 提交urb , 将会失败, 错误值为-EPIPE.
**提交和控制 urb 的过程**
以usb-skeleton源码中的写函数为例： 
```
static ssize_t skel_write(struct file *file, const char *user_buffer, size_t count, loff_t *ppos)
{
    struct usb_skel *dev;
    int retval = 0;
    struct urb *urb = NULL;
    char *buf = NULL;
    size_t writesize = min(count, (size_t)MAX_TRANSFER);
    dev = (struct usb_skel *)file->private_data;
    /* verify that we actually have some data to write */
    if (count == 0)
        goto exit;
    /* limit the number of URBs in flight to stop a user from using up all RAM */
    if (down_interruptible(&dev->limit_sem)) {
        retval = -ERESTARTSYS;
        goto exit;
    }
    spin_lock_irq(&dev->err_lock);
    if ((retval = dev->errors) < 0) {
        /* any error is reported once */
        dev->errors = 0;
        /* to preserve notifications about reset */
        retval = (retval == -EPIPE) ? retval : -EIO;
    }
    spin_unlock_irq(&dev->err_lock);
    if (retval < 0)
        goto error;
    /* create a urb, and a buffer for it, and copy the data to the urb */
    /*当驱动有数据发送到 USB 设备，首先分配一个 urb */
    urb = usb_alloc_urb(0, GFP_KERNEL);
    if (!urb) {
        retval = -ENOMEM;
        goto error;
    }
    /*以最有效的方式是创建一个 DMA 缓冲区来发送数据到设备, 并拷贝数据到缓冲区*/
    buf = usb_buffer_alloc(dev->udev, writesize, GFP_KERNEL, &urb->transfer_dma);
    if (!buf) {
        retval = -ENOMEM;
        goto error;
    }
    if (copy_from_user(buf, user_buffer, writesize)) {
        retval = -EFAULT;
        goto error;
    }
    /* this lock makes sure we don't submit URBs to gone devices */
    mutex_lock(&dev->io_mutex);
    if (!dev->interface) {        /* disconnect() was called */
        mutex_unlock(&dev->io_mutex);
        retval = -ENODEV;
        goto error;
    }
    /* initialize the urb properly */
    /*在将urb提交给 USB 核心之前，正确初始化 urb */
    usb_fill_bulk_urb(urb, dev->udev,
             usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
             buf, writesize, skel_write_bulk_callback, dev);
    urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
    usb_anchor_urb(urb, &dev->submitted);
    /* send the data out the bulk port */
    /*提交 urb 给 USB 核心, 由它将 urb 传递给设备*/
    retval = usb_submit_urb(urb, GFP_KERNEL);
    mutex_unlock(&dev->io_mutex);
    if (retval) {
        err("%s - failed submitting write urb, error %d", __func__, retval);
        goto error_unanchor;
    }
    /* release our reference to this urb, the USB core will eventually free it entirely */
    usb_free_urb(urb);
    return writesize;
error_unanchor:
    usb_unanchor_urb(urb);
error:
    if (urb) {
        usb_buffer_free(dev->udev, writesize, buf, urb->transfer_dma);
        usb_free_urb(urb);
    }
    up(&dev->limit_sem);
exit:
    return retval;
}
//当urb被成功传递到 USB 设备(或者在传输中发生了错误), urb 回调函数将被 USB 核心调用.也就是上面初始化 urb 中的 skel_write_bulk_callback
static void skel_write_bulk_callback(struct urb *urb)
{
    struct usb_skel *dev;
    dev = urb->context;
    /* sync/async unlink faults aren't errors */
    /*检查 urb 的状态，判断这个 urb 是否成功完成传输*/
    if (urb->status) {
        if(!(urb->status == -ENOENT ||
         urb->status == -ECONNRESET ||
         urb->status == -ESHUTDOWN))
            err("%s - nonzero write bulk status received: %d",
             __func__, urb->status);
        spin_lock(&dev->err_lock);
        dev->errors = urb->status;
        spin_unlock(&dev->err_lock);
    }
    /* free up our allocated buffer */
    /*释放分配给这个 urb 的缓冲区.*/
    usb_buffer_free(urb->dev, urb->transfer_buffer_length,
            urb->transfer_buffer, urb->transfer_dma);
    up(&dev->limit_sem);
}
```
urb 回调函数是在中断上下文运行, 因此它不应做任何内存分配, 持有任何信号量, 或任何可导致进程休眠的事情. 如果从回调中提交 urb 并需要分配新内存块, 需使用 GFP_ATOMIC 标志来告知 USB 核心不要休眠.
**使用简单的函数接口(urb函数的包装)**
有时只是要发送或接受一些简单的 USB 数据,可以使用简单的函数接口: 
```
int usb_bulk_msg(struct usb_device *usb_dev, unsigned int pipe,
         void *data, int len, int *actual_length, int timeout)
/*创建批量 urb 并发送到指定的设备, 接着在返回之前等待完成.*/
//struct usb_device *usb_dev :目标 USB 设备指针
//unsigned int pipe :目标 USB 设备的特定端点. 必须使用特定的宏创建.
//void *data :如果是 OUT 端点, 指向要发送到设备的数据的指针. 如果是 IN 端点, 这是从设备读取的数据的缓冲区指针.
//int len : data 参数指向的缓冲的长度
//int *actual_length :指向函数放置真实字节数的指针,根据端点方向,这些字节要么是被发送到设备的,要么是从设备中读取的.
//int timeout :时钟嘀哒数, 应等待的时间. 如果为 0, 函数永远等待操作完成.
//成功返回0,actual_length 参数包含被传送或从设备中读取的字节数.否则返回负的错误值.
int usb_control_msg(struct usb_device *dev, unsigned int pipe, __u8 request,
         __u8 requesttype, __u16 value, __u16 index, void *data,
         __u16 size, int timeout)
/*创建控制 urb 并发送到指定的设备, 接着在返回之前等待完成.*/
//struct usb_device *usb_dev :目标 USB 设备指针
//unsigned int pipe :目标 USB 设备的特定端点. 必须使用特定的宏创建.
//__u8 request :控制消息的 USB 请求值.
//__u8 requesttype :控制消息的 USB 请求类型.
//__u16 value :控制消息的 USB 消息值.
//__u16 index :控制消息的 USB 消息索引值.
//void *data :如果是 OUT 端点, 指向要发送到设备的数据的指针. 如果是 IN 端点, 这是从设备读取的数据的缓冲区指针.
//__u16 size : data 参数指向的缓冲的长度
//int timeout :时钟嘀哒数, 应等待的时间. 如果为 0, 函数永远等待操作完成.
//成功返回被传送到或从设备读取的字节数.否则返回负的错误值.
int usb_interrupt_msg(struct usb_device *usb_dev, unsigned int pipe, void *data, int len, int *actual_length, int timeout)
/*创建中断 urb 并发送到指定的设备, 接着在返回之前等待完成.其实就是usb_bulk_msg的包装,所有参数和usb_bulk_msg一样使用*/
```
以上的函数不能在中断上下文或持有自旋锁时调用. 这些函数不能被取消, 所以小心使用; 确保驱动的 disconnect 函数了解足够的信息, 在允许它自己从内存被卸载之前等待调用结束.
** 其他 USB 函数**
USB 核心中的一些辅助函数用来从所有的 USB 设备中获取标准信息. 这些函数不能在中断上下文或者持有自旋锁时调用,因为他们内部都是使用上面介绍的简单的接口函数.这里就不一一介绍了,包括《LDD3》介绍的这些函数，在/drivers/usb/core/message.c都有。
本文出自：[http://blog.chinaunix.net/space.php?uid=20543672&do=blog&id=94357](http://blog.chinaunix.net/space.php?uid=20543672&do=blog&id=94357)
