# 总线驱动：Bus driver - USB driver for example - Koma Hub - CSDN博客
2019年03月02日 10:44:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：26
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
**Table of Contents**
[Bus driver](#Bus%20driver)
[Adapter, bus and device drivers](#Adapter%2C%20bus%20and%20device%20drivers)
[Example of device driver](#Example%20of%20device%20driver)
[Device identiffers识别器](#Device%20identiffers%E8%AF%86%E5%88%AB%E5%99%A8)
[Instantation of usb_driver](#Instantation%20of%20usb_driver)
[Registration of the driver 登记处](#Registration%20of%20the%20driver%C2%A0%E7%99%BB%E8%AE%B0%E5%A4%84)
[Probe call sequence (1/3)探测调用序列](#Probe%20call%20sequence%20(1%2F3)%E6%8E%A2%E6%B5%8B%E8%B0%83%E7%94%A8%E5%BA%8F%E5%88%97)
[Probe call sequence (2/3)](#Probe%20call%20sequence%20(2%2F3))
[Probe call sequence (3/3)](#Probe%20call%20sequence%20(3%2F3))
[Probe method探针法](#Probe%20method%E6%8E%A2%E9%92%88%E6%B3%95)
[rtl8150 probe](#rtl8150%20probe)
[The model is recursive模型是递归的](#The%20model%20is%20recursive%E6%A8%A1%E5%9E%8B%E6%98%AF%E9%80%92%E5%BD%92%E7%9A%84)
# Bus driver
- Core element of the device model I A single bus driver for each type of bus: USB, PCI, SPI, MMC, I2C, etc.
- This driver is responsibles for I Registering the bus type (bus type structure)
- Allow the registration of adapter/interface drivers (USB controllers, I2C controllers, SPI controllers). These are the hardware devices capable of detecting and providing access to the devices connected to the bus
- Allow the registration of device drivers (USB devices, I2C devices, SPI devices). These are the hardware devices connected to the different buses.
- Matching the device drivers against the detected devices
# Adapter, bus and device drivers
![](https://img-blog.csdnimg.cn/20190302101254583.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# Example of device driver
To illustrate how drivers are implemented to work with the device model, we will use an USB network adapter driver. We will therefore limit ourselves to device drivers and won't cover adapter drivers.
## Device identiffers识别器
- Denes the set of devices that this driver can manage, so that the USB core knows which devices this driver can handle.
- The **MODULE_DEVICE_TABLE **macro allows depmod to extract at compile the relation between device identiers and drivers, so that drivers can be loaded automatically by udev. See **/lib/modules/$(uname -r)/modules**.falias,usbmapg.
```cpp
static struct usb_device_id rtl8150_table[] = {
{USB_DEVICE(VENDOR_ID_REALTEK, PRODUCT_ID_RTL8150)},
{USB_DEVICE(VENDOR_ID_MELCO, PRODUCT_ID_LUAKTX)},
{USB_DEVICE(VENDOR_ID_MICRONET, PRODUCT_ID_SP128AR)},
{USB_DEVICE(VENDOR_ID_LONGSHINE, PRODUCT_ID_LCS8138TX)},
{USB_DEVICE(VENDOR_ID_OQO, PRODUCT_ID_RTL8150)},
{USB_DEVICE(VENDOR_ID_ZYXEL, PRODUCT_ID_PRESTIGE)},
{}
};
MODULE_DEVICE_TABLE(usb, rtl8150_table);
```
## Instantation of usb_driver
- Instantiates the **usb_driver **structure. This structure is a specialization of struct driver dened by the driver model. We have an example of inheritance here.
```cpp
static struct usb_driver rtl8150_driver = {
.name = "rtl8150",
.probe = rtl8150_probe,
.disconnect = rtl8150_disconnect,
.id_table = rtl8150_table,
.suspend = rtl8150_suspend,
.resume = rtl8150_resume
};
```
## Registration of the driver 登记处
When the driver is loaded and unloaded, it simply registers and unregisters itself as an USB device driver.
```cpp
static int __init usb_rtl8150_init(void)
{
    return usb_register(&rtl8150_driver);
}
static void __exit usb_rtl8150_exit(void)
{
    usb_deregister(&rtl8150_driver);
}
module_init(usb_rtl8150_init);
module_exit(usb_rtl8150_exit);
```
## Probe call sequence (1/3)探测调用序列
At boot time, the USB device driver registers itself to the generic BUS infrastructure
在引导时，USB设备驱动程序将自己注册到通用总线基础结构
![](https://img-blog.csdnimg.cn/20190302101930291.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## Probe call sequence (2/3)
When a bus adapter driver detects a device, it noties the generic USB bus infrastructure
当总线适配器驱动程序检测到设备时，它不使用通用的USB总线基础结构.
![](https://img-blog.csdnimg.cn/2019030210220075.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## Probe call sequence (3/3)
The generic USB bus infrastructure knows which driver is capable of handling the detected device. It calls the probe() method of that driver
通用USB总线基础结构知道哪个驱动程序能够处理检测到的设备。它调用该驱动程序的probe（）方法
![](https://img-blog.csdnimg.cn/20190302102357510.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# Probe method探针法
- The probe() method receives as argument a structure describing the device, usually specialized by the bus infrastructure (pci_dev, usb_interface, etc.)
- This function is responsible for I Initializing the device, mapping I/O memory, registering the interrupt handlers. The bus infrastructure provides methods to get the addresses, interrupts numbers and other device-specic information.
- Registering the device to the proper kernel framework, for example the network infrastructure.
- probe（）方法接收描述设备的结构作为参数，通常由总线基础结构（pci_dev、usb_interface等）专门化。
- 此函数负责初始化设备、映射I/O内存、注册中断处理程序。总线基础设施提供了获取地址、中断号码和其他设备特定信息的方法。
- 将设备注册到适当的内核框架，例如网络基础结构。
## rtl8150 probe
```cpp
static int rtl8150_probe(struct usb_interface *intf,
                        const struct usb_device_id *id)
{
    rtl8150_t *dev;
    struct net_device *netdev;
    netdev = alloc_etherdev(sizeof(rtl8150_t));
    dev = netdev_priv(netdev);
    tasklet_init(&dev->tl, rx_fixup, (unsigned long)dev);
    spin_lock_init(&dev->rx_pool_lock);
    netdev->netdev_ops = &rtl8150_netdev_ops;
    alloc_all_urbs(dev);
    usb_set_intfdata(intf, dev);
    SET_NETDEV_DEV(netdev, &intf->dev);
    register_netdev(netdev);
    return 0;
}
```
# The model is recursive模型是递归的
![](https://img-blog.csdnimg.cn/20190302104234469.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
> 
Free Electrons. Kernel, drivers and embedded Linux development, consulting, training and support. http://free-electrons.com
