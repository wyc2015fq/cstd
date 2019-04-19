# Userspace sees three main types of devices - Koma Hub - CSDN博客
2019年02月28日 22:22:12[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：21
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
**1. Character devices is the most common type of devices. Initially for devices implementing streams of bytes, it is now used for a wide range of devices: serial ports, framebuffers, video capture devices, sound devices, input devices, I2C and SPI gateways, etc.**
**2. Block devices for storage devices like hard disks, CD-ROM drives, USB keys, SD/MMC cards, etc.**
**3. Network devices for wired or wireless interfaces, PPP connections and others**
- Network devices are accessed through network-specic APIs and tools (socket API of the standard C library, tools such as
- ifconfig, route, etc.)
- Block and character devices are represented for userspace applications as les than can be manipulated using the traditional le API (open(), read(), write(), close(), etc.)
- Special le types for block and character devices, associating a name with a couple (major, minor)
- The kernel only cares about the (type, major, minor), which is the unique identier of the device
- Special les traditionaly located in /dev, created by mknod, either manually or automatically by udev
**Device drivers must register themselves to the core kernel and implement a set of operations specic to their type:**
- **Character drivers** must instantiate and register a cdev structure and implement file_operations
- **Block drivers** must instantiate and register a gendisk structure and implement block_device_operations and a special make_request function
- **Network drivers** must instantiate and register a net_device structure and implement net_device_ops
In this presentation, we will rst focus on character devices as an example of device drivers.
![](https://img-blog.csdnimg.cn/20190228222042492.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
> 
Thomas Petazzoni
Free Electrons
thomas.petazzoni@free-electrons.com
