# 基于libhid/libusb进行usb传输数据 - 深之JohnChen的专栏 - CSDN博客

2019年03月15日 23:57:30[byxdaz](https://me.csdn.net/byxdaz)阅读数：105


一、libhid/libusb介绍

libusb介绍：

libusb 设计了一系列的外部API 为应用程序所调用，通过这些API应用程序可以操作硬件，从libusb的源代码可以看出，这些API 调用了内核的底层接口(ioctl)，和kernel driver中所用到的函数所实现的功能差不多，只是libusb更加接近USB 规范。使得libusb的使用也比开发内核驱动相对容易的多。相关资料见[http://www.libusb.org/](http://www.libusb.org/)。对于开发人员需要仔细阅读下api及其使用的说明文档：[http://libusb.sourceforge.net/api-1.0/](http://libusb.sourceforge.net/api-1.0/)，在文档中详细的说明api具体作用，及调用流程。

Libusb API 函数与功能

了解包含的 API 函数以及结构体相关定义等很简单，直接在安装路径下找到我们编程时需要引用的头文件即可，这里对应的头文件是 usb.h。我们打开 usb.h 可以看到，代码上部分为 usb 协议标准术语的宏定义，中间为相关结构体定义，后半部为我们编程引用的 API 声明，下面根据 API 功能和使用顺序进行分类描述：

核心初始化函数

1. void usb_init(void);

函数功能：libusb 初始化，这一步必须首先执行。

2. int usb_find_busses(void);

函数功能：查找系统上的 usb 总线，任何 usb 设备依附于 usb 总线和主机通信。成功返回 usb 总线数目，失败返回负值。

3. int usb_find_devices(void);

函数功能：查找总线上的 usb 设备，这个函数必须在调用 usb_find_busses() 后使用。成功返回设备数目，失败返回负值。

4. struct usb_bus *usb_get_busses(void);

函数功能：返回全局变量 usb_busses，用于遍历总线链表，查找总线 usb 设备。失败返回 NULL。这四步操作是任何程序必须首先执行的。

设备操作接口

1. usb_dev_handle *usb_open(struct usb_device *dev);

函数功能：打开 usb 设备，引用的 usb_dev_handle 以及 usb_device 结构体是 libusb 最常见的，定义可以参见头文件。函数返回 usb 设备的操作句柄，之后对该 usb 设备的操作都通过该句柄进行；

2. int usb_close(usb_dev_handle *dev);

函数功能：关闭 usb 设备，在所有操作完成后或者程序退出前调用，传入参数即上面提到的句柄，成功返回0；

3. int usb_get_string(usb_dev_handle *dev, int index, int langid, char *buf, size_t buflen);

函数功能：获取设备的字符串描述，通过 index 和 langid 进行索引。返回Unicode字符串到buf中，成功返回实际写入buf 的字节数，失败返回负值。

4. int usb_get_string_simple(usb_dev_handle *dev, int index, char *buf, size_t buflen); 

函数功能：包装了 usb_get_string() 函数，返回第一种语言指定 index 索引的字符串描述，并转换到C风格的 ASCII。成功返回实际写入buf 的字节数，失败返回负值。

5. int usb_get_descriptor_by_endpoint(usb_dev_handle *udev, int ep, unsigned char type, unsigned char index, void *buf, int size);

函数功能：从设备获取描述符，以 type 和 index 索引，以ep标志的控制管道。成功返回实际写入buf 的字节数，失败返回负值。

6. int usb_get_descriptor(usb_dev_handle *udev, unsigned char type, unsigned char index, void *buf, int size);

函数功能：获取设备缺省控制管道的描述符，通过 type 和 index 索引。成功返回实际写入buf 的字节数，失败返回负值。

7. int usb_set_configuration(usb_dev_handle *dev, int configuration);

函数功能：设置当前设备使用的configuration，参数configuration 是设备配置描述符中的 bConfigurationValue。成功返回0，失败返回负值。

8. int usb_claim_interface(usb_dev_handle *dev, int interface);

函数功能： 注册设备与操作系统通信的接口，interface 参数是接口描述符的 bInterfaceNumber 字段。这个函数必须被调用，因为只有注册接口，才能做相应的操作。必须在任何接口相关操作(如 usb_set_altinterface() 、 usb_bulk_write() 等)之前调用。成功返回0，失败返回负值。

9. int usb_release_interface(usb_dev_handle *dev, int interface);

函数功能：注销在 usb_claim_interface 中注册的通讯接口，释放相关资源，与注册函数成对使用。

10. int usb_set_altinterface(usb_dev_handle *dev, int alternate);

函数功能：设定当前接口中的活跃（起作用的）设置，alternate 为接口描述符中 bAlternateSetting 字段。成功返回0，失败返回负值。

11. int usb_resetep(usb_dev_handle *dev, unsigned int ep);

函数功能：复位指定的端点，参数 ep 为端点描述符中的 bEndpointAddress，此函数已被函数 usb_clear_halt 替代。成功返回0，失败返回负值。

12. int usb_clear_halt(usb_dev_handle *dev, unsigned int ep);

函数功能：复位指定的端点，参数 ep 为端点描述符中的 bEndpointAddress。成功返回0，失败返回负值。

13. int usb_reset(usb_dev_handle *dev);

函数功能：重置指定设备，通过发送 RESET 指令过去。返回0成功，负数失败。在执行该函数之后，需要重新枚举设备，且之前的句柄无法再工作。因此现在使用 usb_close 函数来替代。

14. int usb_get_driver_np(usb_dev_handle *dev, int interface, char *name, unsigned int namelen);

函数功能：获取设备接口对应的驱动程序名称，也即遍历 sysfs 系统中驱动与设备的对应关系，查找当前系统中有无该接口驱动，成功驱动名写入到 name 中且函数返回0，失败返回负值。

15. int usb_detach_kernel_driver_np(usb_dev_handle *dev, int interface);

函数功能：从内核中分离该接口驱动程序，也即禁止该驱动对设备起作用。这个步骤不可省略，不然会发现驱动和使用 Libusb 编写的应用程序同时对设备起作用，引起错误。使用时可以先通过 usb_get_driver_np 进行判断，如果有绑定的驱动程序，那么再调用此函数进行分离。成功返回0，失败返回负值。

数据通讯接口

1. int usb_bulk_write(usb_dev_handle *dev, int ep, const char *bytes, int size, int timeout);

函数功能：批量端点写函数，传入指定的端点号，timeout 为超时时间，以 ms 为单位。成功返回实际写入设备的字节数，失败返回负值。

2. int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout); 

函数功能：批量端点读函数，传入指定的端点号，timeout 为超时时间，以 ms 为单位。成功返回实际写入 buf 的字节数，失败返回负值。

3. int usb_interrupt_write(usb_dev_handle *dev, int ep, const char *bytes, int size, int timeout);

函数功能：中断端点写函数，传入指定的端点号，timeout 为超时时间，以 ms 为单位。成功返回实际写入设备的字节数，失败返回负值。

4. int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);

函数功能：中断端点读函数，传入指定的端点号，timeout 为超时时间，以 ms 为单位。成功返回实际写入 buf 的字节数，失败返回负值。

5. int usb_control_msg(usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout);

函数功能：控制请求读写函数，传入参数与 usb 协议保持一致，timeout 为超时时间，以 ms 为单位。成功返回实际读写的字节数，失败返回负值。

辅助函数接口

1. char *usb_strerror(void);

函数功能：根据错误返回值 errno，返回错误码码对应的字符串。

2. void usb_set_debug(int level);

函数功能：设置 libusb 的调试级别，libusb 支持打印调试信息，level 值为指定的调试级别。

3. struct usb_device *usb_device(usb_dev_handle *dev);

函数功能：根据句柄返回对应的 usb_device。

libhid介绍

      libhid是在libusb基础上封装了一层API，简化了数据操作，使得HID设备读写更方便，也更贴近PC端编程习惯。源码下载地址[http://libhid.alioth.debian.org/](http://libhid.alioth.debian.org/)

libusb在windows端的底层驱动是Winusb.sys，在Linux端是ioctl。

二、操作流程

1.libusb操作流程

1.1.初始化

struct usb_bus *bus;

struct usb_device *dev;

usb_init();

usb_find_busses();

usb_find_devices();

1.2.找到设备

for (bus = busses; bus; bus = bus->next)

{        

for (dev = bus->devices; dev; dev = dev->next)

{              

if(dev->descriptor.idVendor==VENDOR_ID&& dev->descriptor.idProduct == PRODUCT_ID)

 {

//找到设备

break;

}

       }

}

1.3.打开设备

dev_handle = usb_open(dev);//打开设备

usb_set_configuration( dev_handle , bConfigureType); //设置设备config类型

usb_claim_interface( dev_handle , 0);//注册与操作系统通信的接口

Note:

usb_claim_interface失败

1)

现象1：函数返回值=-1，获取usb_strerror，为Operation not permitted

原因：是应用程序没有/dev/mnt/usb目录的写权限。

解决方法：一种方法是root登陆;另一种是“chmod o+w -R /dev/bus/usb into /etc/init.d/rc shell script”

2)

现象2：函数返回-2，错误信息为"USB error: could not claim interface 0: No such file or directory"

usb_set_configuration中的参数必须与控制描述符中的bConfigureType一致

1.4.获取报告描述符 

usb_control_msg(dev_handle,

      USB_ENDPOINT_IN+1,

      USB_REQ_GET_DESCRIPTOR,

      (USB_DT_REPORT << 8) + 0, hidif->interface,

      (char*)hidif->hid_parser->ReportDesc, hidif->hid_parser->ReportDescSize,

      USB_TIMEOUT);

报告描述符中得到ReportSize

1.5. 设置/获取HID Report

//get_input_report

len = usb_control_msg(dev_handle,

      USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE,

      HID_REPORT_GET,

      hidif->hid_data->ReportID + (HID_RT_INPUT << 8),

      hidif->interface,

      buffer, REPROTSIZE, USB_TIMEOUT);

//set output report

 int len = usb_control_msg(hidif->dev_handle,

      USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,

      HID_REPORT_SET,

      hidif->hid_data->ReportID + (HID_RT_OUTPUT << 8),

      hidif->interface,

      (char*)buffer, REPROTSIZE, USB_TIMEOUT);

1.6.关闭设备

usb_release_interfaces

usb_close

1.7. debug

lib_usb_setdebug

usb_strerror

2.libhid接口使用说明

2.1初始化

hid_init()//同1.1

hid_new_HIDInterface,准备结构体memory

2.2 查找设备，并打开设备，注册通信接口

hid_force_open();//同1.2，1.3,1.4

2.3 报告操作

hid_get_input_report //同1.5

hid_set_output_report//同1.5

2.4 debug

hid_set_debug(HID_DEBUG_ALL);

hid_set_usb_debug(0);

VC++ HIDAPI实现USB数据读写
[https://blog.csdn.net/lhangtk/article/details/82706242](https://blog.csdn.net/lhangtk/article/details/82706242)

三、libhid/libusb库编译

libusb编译

Linux下libusb编译与生成动态链接库
[https://blog.csdn.net/gd6321374/article/details/79920279](https://blog.csdn.net/gd6321374/article/details/79920279)

Windows下libusb编译
[https://blog.csdn.net/buaalei/article/details/45722859](https://blog.csdn.net/buaalei/article/details/45722859)

HIDAPI库编译

从官网上下载到最新的压缩包hidapi-0.7.0.zip，解压后查看README.txt，里面介绍了三种Windows、Linux、MacOS三种平台下的编译方式：

------

It can be downloaded from github

    git clone git://github.com/signal11/hidapi.git

Build Instructions

-------------------

To build the console test program:

  Windows:

    Build the .sln file in the windows/ directory.

  Linux:

    cd to the linux/ directory and run make.

  Mac OS X:

    cd to the mac/ directory and run make.

1、使用Visual Studio打开hidapi.vcproj，然后编译之。注:因为我使用的是VS2005，由于版本不对应，无法转换hidapi.vcproj到我当前的版本，此时用编辑器打开文件hidapi.vcproj将原本的：

Version="9.00"

 改成8.00.之后就可以打开工程。 

2、编译完成之后，在该目录下会生成一个Debug目录，里面有很多文件，其中我们主要用到hidapi.dll和hidapi.lib两个文件。将hidapi.dll拷贝到C:\Windows\system下（至于拷贝到哪个目录，要根据系统是32位还是64位选择不同的路径。）

之后将hidapi.lib文件拷贝到你的工程目录下，这里我当然是拷贝到我的Qt 工程目录下。当然，也要把hidapi.h文件拷贝到Qt工程目录下，因为hidapi.h包含hidapi这个库中所有函数接口的声明，这样在Qt项目中对设备进行操作才不会调试说找不到hid_xxx(）某某个函数。

3、在Qt工程中要指定lib文件和header文件。修改Qt的工程文件.pro如下：

HEADERS += hidapi.h

LIBS += -L$$_PRO_FILE_PWD_/LIB/ \

            -lhidapi

-L指定当前的路径；_PRO_FILE_PWD_指当前.pro文件所在的目录；-l指定.dll文件，这里是指定名字为hidapi的dll文件。 

上述配置完成之后就可以对USB设备进行操作。具体如何操作可以参见README里面的用例以及hidapi.h的函数说明。

四、常见问题

1. configuration选择,interface认领和endpoint地址

需要先行了解usb设备的配置,接口和端点概念．启用的配置，认领的接口和端点地址存在包含关系.认领某接口需要确定当前选择的配置包含该接口，读写数据时，需要确定当前接口包含这些端点，同时端点类型和地址无误．

2. libusb_  -7 (Operation time out)

同步io操作比较简单，但可能会出现超时. 将超时设置为0，永不超时，可以解决返回超时错误．

3. libusb_claim_interface 函数总是返回 -6 (Resource busy).

解决这个问题,花费的时间最多.即使按照libusb的example 和stackoverflow上提到的,先调用libusb_detach_kernel_driver函数,也不能解决. 

后来查看libusb.h文件,发现有libusb_reset_device函数,尝试在认领接口前,先调用该函数,发现认领接口不再返回错误码. 

经测试,只要调用了libusb_reset_device函数,不需要再调用libusb_detach_kernel_driver方法. 

该方法可以临时解决问题，不确定会不会产生其他问题．

4.libusb_bulk_transfer的timeout参数问题

timeout参数

1）. bulk in：在指定的timeout参数时间内会一直阻塞等待接收，0为一直阻塞等待。

2）. bulk out：timeout设置为0即可

5. libusb不支持Windows端的热插拔事件hotplug，参考[https://github.com/libusb/libusb/issues/86](https://github.com/libusb/libusb/issues/86)

