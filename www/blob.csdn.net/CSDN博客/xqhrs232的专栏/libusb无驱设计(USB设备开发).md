# libusb无驱设计(USB设备开发) - xqhrs232的专栏 - CSDN博客
2018年01月29日 17:40:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：402
原文地址::[http://blog.csdn.net/zdyueguanyun/article/details/51192939](http://blog.csdn.net/zdyueguanyun/article/details/51192939)
转自：
﻿﻿
[http://blog.csdn.net/saloon_yuan/article/details/7880768](http://blog.csdn.net/saloon_yuan/article/details/7880768)
Linux下的USB驱动开发，主要有内核驱动开发以及基于libusb的无驱设计。libusb是一个开源项目，Linux和Windows下都有相应的版本，本文以一个实际项目的原代码为例，来看看linux系统下怎样利用libusb来访问我们的USB设备。
首先我们简单介绍下libusb的编译和安装（本例OS为CentOS6.0 (内核版本2.6.32-71.el6.i686)。
[官网](http://www.libusb.org/)下载libusb安装包（本项目中下载的版本为libusb-1.0.8.tar），解压后，进入libusb-1.0.8文件夹目录中，依次执行如下指令：
     a:执行./configure来配置安装包。
     b:执行make编译该安装包。
     c:执行make install安装。
libusb安装好之后，就可以调用它的API来访问我们的设备了，先简单介绍下几个主要的API
1：
[cpp][view
 plain](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)[copy](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)
- API_EXPORTED int libusb_init(libusb_context **context)  
该函数进行libusb的初始化，必须最先调用。
libusb_context  *ctx ：可以为NULL。
2：
[cpp][view
 plain](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)[copy](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)
- API_EXPORTED libusb_device_handle *libusb_open_device_with_vid_pid(  
-     libusb_context *ctx, uint16_t vendor_id, uint16_t product_id)  
该函数可直接根据指定的USB设备的Vendor ID 和 Product ID来找到相应的设备并打开它，将设备句柄返回给调用者。
libusb_context  *ctx ：可以为NULL。
uint16_t   vendor ：生产商ID。
uint16_t  product_id： 设备ID。
3：
[cpp][view
 plain](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)[copy](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)
- int libusb_control_transfer(libusb_device_handle *dev_handle,  
-     uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex,  
-     unsigned char *data, uint16_t wLength, unsigned int timeout)  
控制传输操作
libusb_device_handle *dev_handle：  设备句柄。
uint8_t  bmRequestType： 对应Setup Packet包中的bmRequestType字段。
uint8_t  bRequest： 对应Setup Packet包中的bRequest字段。
uint16_t   wValue： 对应Setup Packet包中的wValue字段。
uint16_t wIndex： 对应Setup Packet包中的wIndex字段。
unsigned char  *data： 数据。
uint16_t  wLength： 数据大小。
unsigned int  timeout： 超时阀值。
4：
[cpp][view
 plain](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)[copy](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)
- API_EXPORTED int libusb_bulk_transfer(struct libusb_device_handle *dev_handle,  
-     unsigned char endpoint, unsigned char *data, int length, int *transferred,  
-     unsigned int timeout)  
块传输操作
libusb_device_handle *dev_handle：  设备句柄。
unsigned char endpoint：  端点编号。
unsigned char  *data： 数据。
int length： 数据块长度。
int *transferred：  实际传输数据块大小。
unsigned int timeout：  超时阀值。
5：
[cpp][view
 plain](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)[copy](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)
- API_EXPORTED int libusb_interrupt_transfer(  
- struct libusb_device_handle *dev_handle, unsigned char endpoint,  
-     unsigned char *data, int length, int *transferred, unsigned int timeout)  
中断传输操作
libusb_device_handle *dev_handle：  设备句柄。
unsigned char endpoint：  端点编号。
unsigned char  *data： 数据。
int length： 数据块长度。
int *transferred：  实际传输数据块大小。
unsigned int timeout：  超时阀值。
下面我们看看如何打开设备：
[cpp][view
 plain](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)[copy](http://blog.csdn.net/zdyueguanyun/article/details/51192939#)
- int   rv = 0;  
- 
- rv = libusb_init(NULL);  
- if(rv < 0) {  
-    printf("*** initial USB lib failed! \n");      
- return -1;  
- }  
- 
- //open the usb device
- g_usb_handle = libusb_open_device_with_vid_pid(NULL, USB_VENDOR_ID, USB_PRODUCT_ID);  
- if(g_usb_handle == NULL) {  
-   printf("*** Permission denied or Can not find the USB board (Maybe the USB driver has not been installed correctly), quit!\n");  
- return -1;  
- }  
libusb的使用非常方便，指定ID后，相应设备被打开，得到设备句柄之后，即可调用各种传输操作API对我们的USB设备进行访问了。

