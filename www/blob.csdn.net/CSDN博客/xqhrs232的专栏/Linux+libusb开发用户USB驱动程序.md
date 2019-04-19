# Linux+libusb开发用户USB驱动程序 - xqhrs232的专栏 - CSDN博客
2017年09月19日 10:26:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：745
原文地址::[http://blog.sina.com.cn/s/blog_8a84e6d301015uh7.html](http://blog.sina.com.cn/s/blog_8a84e6d301015uh7.html)
相关文章
1、[转: 嵌入式linux下usb驱动开发方法--看完少走弯路](http://blog.csdn.net/jimmy_1986/article/details/5838297)----[http://blog.csdn.net/jimmy_1986/article/details/5838297](http://blog.csdn.net/jimmy_1986/article/details/5838297)
2、linux下USB通信程序----[http://download.csdn.net/download/yamazikai/3801764](http://download.csdn.net/download/yamazikai/3801764)
3、linux下基于python的usb通信程序开发----[http://www.doc88.com/p-7018333656127.html](http://www.doc88.com/p-7018333656127.html)
前几天心血来潮，在网上买了游戏摇杆和按键，他们通过usb口与电脑连接，于是乎我就想针对此手柄与按键开发自己的usb驱动，目的是为了以后玩的happy，哈哈。
    libusb是用户端驱动程序封装库，是USB主机对USB设备进行操作的函数集合，有了它我们针对某型号USB设备就不用去修改比较繁琐的linux内核驱动了，方便了对设备的使用与调试。我电脑的编程环境是ubuntu12.04+geany(一款linux下的集成开发工具，个人感觉挺好用的)。接下来分享一下代码：
#include <stdio.h>
#include <string.h>
#include <usb.h>
#define IdVendor 0x0079       //这是我设备的厂商号
#define IdProduct 0x0006      //这是我设备的产品号，当你的usb设备接入电脑时，使用lsusb -v命令来查看设备信息，请看下面第三张图片中的0079:0006
//#define PrintDev
char enp_num[8],buf8[8],is_change=0;
//端点描述符
static void print_endpoint(struct usb_endpoint_descriptor *endpoint)
{
  printf("      bEndpointAddress: xh\n", endpoint->bEndpointAddress);
  printf("      bmAttributes:     xh\n", endpoint->bmAttributes);
  printf("      wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
  printf("      bInterval:        %d\n", endpoint->bInterval);
  printf("      bRefresh:         %d\n", endpoint->bRefresh);
  printf("      bSynchAddress:    %d\n", endpoint->bSynchAddress);
}
static void print_altsetting(struct usb_interface_descriptor *interface)
{
  int i;
  printf("    bInterfaceNumber:   %d\n", interface->bInterfaceNumber);
  printf("    bAlternateSetting:  %d\n", interface->bAlternateSetting);
  printf("    bNumEndpoints:      %d\n", interface->bNumEndpoints);
  printf("    bInterfaceClass:    %d\n", interface->bInterfaceClass);
  printf("    bInterfaceSubClass: %d\n", interface->bInterfaceSubClass);
  printf("    bInterfaceProtocol: %d\n", interface->bInterfaceProtocol);
  printf("    iInterface:         %d\n", interface->iInterface);
  for (i = 0; i < interface->bNumEndpoints; i++)
    print_endpoint(&interface->endpoint[i]);
}
//显示设备所拥有的所有接口的描述符
static void print_interface(struct usb_interface *interface)
{
  int i;
  for (i = 0; i < interface->num_altsetting; i++)
    print_altsetting(&interface->altsetting[i]);
}
//配置描述符
static void print_configuration(struct usb_config_descriptor *config)
{
  int i;
  printf("  wTotalLength:         %d\n", config->wTotalLength);
  printf("  bNumInterfaces:       %d\n", config->bNumInterfaces);
  printf("  bConfigurationValue:  %d\n", config->bConfigurationValue);
  printf("  iConfiguration:       %d\n", config->iConfiguration);
  printf("  bmAttributes:         xh\n", config->bmAttributes);
  printf("  MaxPower:             %d\n", config->MaxPower);
  for (i = 0; i < config->bNumInterfaces; i++)
    print_interface(&config->interface[i]);
}
static void print_device(struct usb_device *dev)
{
    //读取描述符
    int i;
    char description[256];
    snprintf(description, sizeof(description), "X-X",dev->descriptor.idVendor, dev->descriptor.idProduct);
    printf("Dev #%d: %s\n", dev->devnum,description);
    for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
      print_configuration(&dev->config[i]);
}
//因为我的游戏设备属于HID类型，所以他与电脑通过中断端点进行数据传输。当摇杆位置变化或者按键按下时，读取设备中断端点值
static int read_interupt(struct usb_device *dev)
{
    usb_dev_handle *udev;
    char buf[256];
    int ret,i;
    udev=usb_open(dev);
    //#ifdef PrintDev
        //print_device(dev);
    //#endif
    //使用libusb驱动前必须使接口脱离linux内核驱动
    usb_detach_kernel_driver_np(udev,0);
