# PF_NETLINK应用实例NETLINK_KOBJECT_UEVENT具体实现－－udev实现原理 - h13 - 博客园
     相对于linux来说，udev还是一个新事物。然而，尽管它03年才出现，尽管它很低调(J)，但它无疑已经成为linux下不可或缺的组件了。udev是什么？它是如何实现的？最近研究Linux设备管理时，花了一些时间去研究udev的实现。 
　　udev是什么？u 是指user space，dev是指device，udev是用户空间的设备驱动程序吗？最初我也这样认为，调试内核空间的程序要比调试用户空间的程序复杂得多，内核空间的程序的BUG所引起的后果也严重得多，device driver是内核空间中所占比较最大的代码，如果把这些device driver中硬件无关的代码，从内核空间移动到用户空间，自然是一个不错的想法。 
　　但我的想法并不正确，udev的文档是这样说的， 
　　1. dynamic replacement for /dev。作为devfs的替代者，传统的devfs不能动态分配major和minor的值，而major和minor非常有限，很快就会用完了。 udev能够像DHCP动态分配IP地址一样去动态分配major和minor。 
　　2. device naming。提供设备命名持久化的机制。传统设备命名方式不具直观性，像/dev/hda1这样的名字肯定没有boot_disk这样的名字直观。udev能够像DNS解析域名一样去给设备指定一个有意义的名称。 
　　3. API to access info about current system devices 。提供了一组易用的API去操作sysfs，避免重复实现同样的代码，这没有什么好说的。 
　　我们知道，用户空间的程序与设备通信的方法，主要有以下几种方式， 
　　1. 通过ioperm获取操作IO端口的权限，然后用inb/inw/ inl/ outb/outw/outl等函数，避开设备驱动程序，直接去操作IO端口。（没有用过） 
　　2. 用ioctl函数去操作/dev目录下对应的设备，这是设备驱动程序提供的接口。像键盘、鼠标和触摸屏等输入设备一般都是这样做的。 
　　3. 用write/read/mmap去操作/dev目录下对应的设备，这也是设备驱动程序提供的接口。像framebuffer等都是这样做的。 
　　上面的方法在大多数情况下，都可以正常工作，但是对于热插拨(hotplug)的设备，比如像U盘，就有点困难了，因为你不知道：什么时候设备插上了，什么时候设备拔掉了。这就是所谓的hotplug问题了。 
　　处理hotplug传统的方法是，在内核中执行一个称为hotplug的程序，相关参数通过环境变量传递过来，再由hotplug通知其它关注 hotplug事件的应用程序。这样做不但效率低下，而且感觉也不那么优雅。新的方法是采用NETLINK实现的，这是一种特殊类型的socket，专门用于内核空间与用户空间的异步通信。下面的这个简单的例子，可以监听来自内核hotplug的事件。 
```
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <ctype.h>  
#include <sys/un.h>  
#include <sys/ioctl.h>  
#include <sys/socket.h>  
#include <linux/types.h>  
#include <linux/netlink.h>  
#include <errno.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
#define UEVENT_BUFFER_SIZE 2048  
static int init_hotplug_sock()  
{  
  const int buffersize = 1024;  
  int ret;  
  struct sockaddr_nl snl;  
  bzero(&snl, sizeof(struct sockaddr_nl));  
  snl.nl_family = AF_NETLINK;  
  snl.nl_pid = getpid();  
  snl.nl_groups = 1;  
  int s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);  
  if (s == -1)   
  {  
  perror("socket");  
  return -1;  
  }  
  setsockopt(s, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));  
  ret = bind(s, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));  
  if (ret < 0)   
  {  
  perror("bind");  
  close(s);  
  return -1;  
  }  
  return s;  
}  
int main(int argc, char* argv[])  
{  
  int hotplug_sock = init_hotplug_sock();  
  while(1)  
  {  
  /* Netlink message buffer */  
  char buf[UEVENT_BUFFER_SIZE * 2] = {0};  
  recv(hotplug_sock, &buf, sizeof(buf), 0);  
printf("%s\n", buf);  
  /* USB 设备的插拔会出现字符信息，通过比较不同的信息确定特定设备的插拔，在这添加比较代码 */
  }
  return 0;  
}
```
　　编译： 
　　gcc -g hotplug.c -o hotplug_monitor 
　　运行后插/拔U盘，可以看到： 
```
add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1 
　　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/usbdev2.2_ep00 
　　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0 
　　add@/class/scsi_host/host2 
　　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep81 
　　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep02 
　　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep83 
　　add@/class/usb_device/usbdev2.2 
　　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/host2/target2:0:0/2:0:0:0 
　　add@/class/scsi_disk/2:0:0:0 
　　add@/block/sda 
　　add@/block/sda/sda1 
　　add@/class/scsi_device/2:0:0:0 
　　add@/class/scsi_generic/sg0 
　　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep81 
　　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep02 
　　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep83 
　　remove@/class/scsi_generic/sg0 
　　remove@/class/scsi_device/2:0:0:0 
　　remove@/class/scsi_disk/2:0:0:0 
　　remove@/block/sda/sda1 
　　remove@/block/sda 
　　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/host2/target2:0:0/2:0:0:0 
　　remove@/class/scsi_host/host2 
　　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0 
　　remove@/class/usb_device/usbdev2.2 
　　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/usbdev2.2_ep00 
　　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1
```
　　udev的主体部分在udevd.c文件中，它主要监控来自4个文件描述符的事件/消息，并做出处理： 
　　1. 来自客户端的控制消息。这通常由udevcontrol命令通过地址为/org/kernel/udev/udevd的本地socket，向udevd发送的控制消息。其中消息类型有： 
　　l UDEVD_CTRL_STOP_EXEC_QUEUE 停止处理消息队列。 
　　l UDEVD_CTRL_START_EXEC_QUEUE 开始处理消息队列。 
　　l UDEVD_CTRL_SET_LOG_LEVEL 设置LOG的级别。 
　　l UDEVD_CTRL_SET_MAX_CHILDS 设置最大子进程数限制。好像没有用。 
　　l UDEVD_CTRL_SET_MAX_CHILDS_RUNNING 设置最大运行子进程数限制(遍历proc目录下所有进程，根据session的值判断)。 
　　l UDEVD_CTRL_RELOAD_RULES 重新加载配置文件。 
　　2. 来自内核的hotplug事件。如果有事件来源于hotplug，它读取该事件，创建一个udevd_uevent_msg对象，记录当前的消息序列号，设置消息的状态为EVENT_QUEUED,然后并放入running_list和exec_list两个队列中，稍后再进行处理。 
　　3. 来自signal handler中的事件。signal handler是异步执行的，即使有signal产生，主进程的select并不会唤醒，为了唤醒主进程的select，它建立了一个管道，在 signal handler中，向该管道写入长度为1个子节的数据，这样就可以唤醒主进程的select了。 
　　4. 来自配置文件变化的事件。udev通过文件系统inotify功能，监控其配置文件目录/etc/udev/rules.d，一旦该目录中文件有变化，它就重新加载配置文件。 
　　其中最主要的事件，当然是来自内核的hotplug事件，如何处理这些事件是udev的关键。udev本身并不知道如何处理这些事件，也没有必要知道，因为它只实现机制，而不实现策略。事件的处理是由配置文件决定的，这些配置文件即所谓的rule。 
　　关于rule的编写方法可以参考《writing_udev_rules》，udev_rules.c实现了对规则的解析。 
　　在规则中，可以让外部应用程序处理某个事件，这有两种方式，一种是直接执行命令，通常是让modprobe去加载驱动程序，或者让mount去加载分区。另外一种是通过本地socket发送消息给某个应用程序。 
　　在udevd.c:udev_event_process函数中，我们可以看到，如果RUN参数以”socket:”开头则认为是发到socket，否则认为是执行指定的程序。 
　　下面的规则是执行指定程序： 
　　60-pcmcia.rules: RUN+="/sbin/modprobe pcmcia" 
　　下面的规则是通过socket发送消息： 
　　90-hal.rules:RUN+="socket:/org/freedesktop/hal/udev_event" 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/ken_GL/archive/2010/12/02/6050168.aspx](http://blog.csdn.net/ken_GL/archive/2010/12/02/6050168.aspx)
