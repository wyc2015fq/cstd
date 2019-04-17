# ioctl用法详解 (网络) - DoubleLi - 博客园






本函数影响由fd参数引用的一个打开的文件。

#include
#include

int ioctl( int fd, int request, .../* void *arg */ );
返回0：成功    -1：出错

第三个参数总是一个指针，但指针的类型依赖于request参数。
我们可以把和网络相关的请求划分为6类：
套接口操作
文件操作
接口操作
ARP高速缓存操作
路由表操作
流系统
下表列出了网络相关ioctl请求的request参数以及arg地址必须指向的数据类型：

（图1）



套接口操作：
明确用于套接口操作的ioctl请求有三个,它们都要求ioctl的第三个参数是指向某个整数的一个指针。

SIOCATMARK:    如果本套接口的的度指针当前位于带外标记，那就通过由第三个参数指向的整数返回一个非0值；否则返回一个0值。POSIX以函数sockatmark替换本请求。
SIOCGPGRP：       通过第三个参数指向的整数返回本套接口的进程ID或进程组ID，该ID指定针对本套接口的SIGIO或SIGURG信号的接收进程。本请求和fcntl的F_GETOWN命令等效，POSIX标准化的是fcntl函数。
SIOCSPGRP：     把本套接口的进程ID或者进程组ID设置成第三个参数指向的整数，该ID指定针对本套接口的SIGIO或SIGURG信号的接收进程，本请求和fcntl的F_SETOWN命令等效，POSIX标准化的是fcntl操作。

文件操作：
以下5个请求都要求ioctl的第三个参数指向一个整数。

FIONBIO：        根据ioctl的第三个参数指向一个0或非0值分别清除或设置本套接口的非阻塞标志。本请求和O_NONBLOCK文件状态标志等效，而该标志通过fcntl的F_SETFL命令清除或设置。

FIOASYNC：      根据iocl的第三个参数指向一个0值或非0值分别清除或设置针对本套接口的信号驱动异步I/O标志，它决定是否收取针对本套接口的异步I/O信号（SIGIO）。本请求和O_ASYNC文件状态标志等效，而该标志可以通过fcntl的F_SETFL命令清除或设置。

FIONREAD：     通过由ioctl的第三个参数指向的整数返回当前在本套接口接收缓冲区中的字节数。本特性同样适用于文件，管道和终端。

FIOSETOWN：    对于套接口和SIOCSPGRP等效。
FIOGETOWN：    对于套接口和SIOCGPGRP等效。

接口配置：
得到系统中所有接口由SIOCGIFCONF请求完成，该请求使用ifconf结构，ifconf又使用ifreq
结构，如下所示：

Struct ifconf{
    int ifc_len;                 // 缓冲区的大小
    union{
        caddr_t ifcu_buf;        // input from user->kernel
        struct ifreq *ifcu_req;    // return of structures returned
    }ifc_ifcu;
};

#define ifc_buf ifc_ifcu.ifcu_buf    //buffer address
#define ifc_req ifc_ifcu.ifcu_req    //array of structures returned

#define IFNAMSIZ 16

struct ifreq{
    char ifr_name[IFNAMSIZ];           // interface name, e.g., “le0”
    union{
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        short ifru_flags;
        int ifru_metric;
        caddr_t ifru_data;
    }ifr_ifru;
};

#define ifr_addr     ifr_ifru.ifru_addr            // address
#define ifr_dstaddr   ifr_ifru.ifru_dstaddr         // otner end of p-to-p link
#define ifr_broadaddr ifr_ifru.ifru_broadaddr    // broadcast address
#define ifr_flags     ifr_ifru.ifru_flags        // flags
#define ifr_metric    ifr_ifru.ifru_metric      // metric
#define ifr_data      ifr_ifru.ifru_data        // for use by interface

再调用ioctl前我们必须先分撇一个缓冲区和一个ifconf结构，然后才初始化后者。如下图
展示了一个ifconf结构的初始化结构，其中缓冲区的大小为1024，ioctl的第三个参数指向
这样一个ifconf结构。
ifc_len
Ifc_buf
1024
--------------------->缓存


假设内核返回2个ifreq结构，ioctl返回时通过同一个ifconf结构缓冲区填入了那2个ifreq结构，ifconf结构的ifc_len成员也被更新，以反映存放在缓冲区中的信息量
一般来讲ioctl在用户程序中的调用是：
ioctl(int fd,int command, (char*)argstruct)
ioctl调用与网络编程有关（本文只讨论这一点），文件描述符fd实际上是由socket()系统调用返回的。参数command的取值由/usr/include/linux/sockios.h所规定。这些command的由于功能的不同，可分为以下几个小类：
1.改变路由表 (例如 SIOCADDRT, SIOCDELRT), 
2. 读/更新 ARP/RARP 缓存(如：SIOCDARP, SIOCSRARP), 
3. 一般的与网络接口有关的(例如 SIOCGIFNAME, SIOCSIFADDR 等等) 
在Gooodies 目录下有很多样例程序展示了如何使用ioctl。当你看这些程序时，注意参数argstruct是与参数command相关的。例如，与路由表相关的 ioctl使用rtentry这种结构，rtentry定义在/usr/include/linux/route.h（参见例子 adddefault.c）。与ARP有关的ioctl调用使用arpreq结构，arpreq定义在 /usr/include/linux/if_arp.h（参见例子arpread.c）
与网络接口有关的ioctl调用使用的command参数通常看起来像SIOCxIFyyyy的形式，这里x要 么是S（设定set，写write），要么是G（得到get，读read）。在getifinfo.c程序中就使用了这种形式的command参数来读 IP地址，硬件地址，广播地址和得到与网络接口有关的一些标志（flag）。在这些ioctl调用中，第三个参数是ifreq结构，它在 /usr/include/linux/if.h中定义。在某些情况下， ioctrl调用可能会使用到在sockios.h之外的新的定义，例如，WaveLAN无线网络卡会保存有关无线网络信号强度的信息，这对用户的程序可 能有用。但用户怎么得到这种信息呢？我们的第一个本能是在sockios.h中定义新的ioctl命令，例如SIOCGIFWVLNSS（它的英文缩写表 示WaveLAN的信号强度）。但不幸的是，这种命令不是对所有其他的网络接口（例如：loopback环回接口）有意义，而且不应当允许对于 WAVLAN卡以外的网络接口使用ioctl命令。那么，我们需要的是这样一种机制：它能够定义一种与网络接口相关的ioctl命令。幸运的是，在 Linux操作系统中已经为实现这个目的内建了一种挂钩（hook）机制。当你再次看sockios.h文件时，你将发现每一种设备已经预先定义了 SIOCDEVPRIVATE的ioctl命令。而它的实现将留给开发相应驱动程序的人去完成。
通常，一个用户程序使用ioctl (sockid,SIOCDEVPRIVATE,(char*)&ifr)来调用与某种设备（指像WaveLAN那样的特殊设备）相关的 ioctl命令，这里ifr是struct ifreq ifr形式的变量。用户程序应当在ifr.ifr_name中填充与这个设备相关的名字，例如，假设WaveLAN使用的接口号为eth1。一般的，一个 用户程序还需要与内核互相交换ioctl的command参数和结果，这可以通过ifr.ifr_data这个变量来实现，例如，想得到WaveLAN中 表示信号强度的信息时，可以通过返回这个变量来实现。Linux的源代码已经包括了两种设备de4x5和ewrk3，它们定义并且实现了特定的ioctl 调用。这两个设备的源代码在de4x5.h,de4x5.c,ewrk3.h,ewrk3.c中（在 /usr/src/linux/drivers/net/目录中）。这两种设备都定义了它们特有的结构（struct ewrk3_ioctl 和 struct de4x5_ioctl）来方便用户程序和设备驱动之间交换信息。每次调用ioctl前，用户程序应当在相应的结构变量中设定合适的初值，并且将 ifr.ifr_data指向该值。
在我们进一步讨论ewrk3和de4x5的代码前，让我们仔细看看ioctl调用是如何一步步地实现的。所有的和接口相关的ioctl请求 (SIOCxIFyyyy 和 SIOCDEVPRIVATE)将会调用dev_ioctl()（在/usr/src/linux/net/core/dev.c中）。但这只是一个包装 器（wrapper），实际的动作将由dev_ifsioc()（也在dev.c中）来实现。差不多dev_ioctl()这个函数所做的所有工作只是检 查这个调用是否已经有了正当的权限（例如，改变路由表需要有root的权限）。而dev_ifsioc()这个函数首先要做的一些事情包括得到与 ifr.ifr_name相匹配的设备的结构（在/usr/include/linux/netdevice.h中定义）。但这是在实现特定的接口命令 （例如：SIOCGIFADDR）之后。这些特定的接口命令被放置到一个巨大的switch语句之中。其中SIOCDEVPRIVATE命令和其他的在 0x89F0到0x89FF之间的代码将出现在switch语句中的一个分支——default语句中。内核会检查表示设备的结构变量中，是否已经定义了 一个与设备相关的ioctl句柄（handler）。这里的句柄是一个函数指针，它在表示设备的结构变量中do_ioctl部分。如果已经设置了这个句 柄，那么内核将会执行它。
所以，如果要实现一个与设备相关的ioctl命令，所要做的只是编写一个与这个设备相关的ioctl句柄，并且将表示这 个设备的结构变量中do_ioctl部分指向这个句柄。对于ewrk3这个设备，它的句柄是ewrk3_ioctl()（在ewrk3.c里面）并且相应 的表示该设备的结构变量由ewrk3_init()来初始化。在ewrk3_ioctl()的代码中清晰的指出ifr.ifr_data是用作设备驱动程 序和用户程序之间交换信息的。注意，这部分的内存可以双向的交流信息。例如，在ewrk3的驱动程序代码中，if.ifr_data的头两个字节是用来表 示特殊的动作（例如，EWRK3_SET_PROM,EWRK3_CLR_PROM），而这个动作是符合使用者（驱动程序实现了多个与设备相关的、由 SIOCDEVPRIVATE调用的命令）的要求的。另外，ifr.ifr_data中第5个字节指向的缓冲区(buffer)被用来交换其他的信息 （如：当使用EWRK3_SET_HWADDR和EWRK3_GET_HWADDR时为硬件地址）
在你深入ewrk3_ioctl()时，请注意一般情况下一个用户进程不能直接访问内核所在的内存。为此，驱动开发者可以使用两个特殊的函数 memcpy_tofs()和memcpy_fromfs()。内核函数memcpy_tofs(arg1, arg2, arg3) 从地址arg2（用户空间）向地址arg1(内核空间)拷贝arg3个字节。类似的，memcpy_fromfs(arg1,arg2,arg3)从地址 arg2（用户空间）向地址arg1（内核空间）拷贝arg3个字节。在这些调用之前，verify_area()将会检查这个进程是否拥有合适的访问权 限。另外，注意使用printk()函数可以输出debug信息。这个函数与printf()函数类似，但不能处理浮点类型的数。内核代码不能够使用 printf()函数。printk()函数产生的结果将记录在/usr/adm/messages里。如果想知道更多的关于这些函数的或者与它们相关的 信息，可以参考《Linux Kernel Hacker’s Guide》（在Linux文档网站的首页） 这本书中Supporting Functions部分。


使用ioctl与内核交换数据

1. 前言 

使用ioctl系统调用是用户空间向内核交换数据的常用方法之一，从ioctl这个名称上看，本意是针对I/O设备进行的控制操作，但实际并不限制是真正的I/O设备，可以是任何一个内核设备即可。

2. 基本过程

在内核空间中ioctl是很多内核操作结构的一个成员函数，如文件操作结构struct file_operations(include/linux/fs.h)、协议操作结构struct proto_ops(include/linux/net.h)等、tty操作结构struct tty_driver(include/linux/tty_driver.h)等，而这些操作结构分别对应各种内核设备，只要在用户空间打开这些设备， 如I/O设备可用open(2)打开，网络协议可用socket(2)打开等，获取一个文件描述符后，就可以在这个描述符上调用ioctl(2)来向内核 交换数据。

3. ioctl(2)

ioctl(2)函数的基本使用格式为：
int ioctl(int fd, int cmd, void *data)
第一个参数是文件描述符；cmd是操作命令，一般分为GET、SET以及其他类型命令，GET是用户空间进程从内核读数据，SET是用户空间进程向内核写数据，cmd虽然是一个整数，但是有一定的参数格式的，下面再详细说明；第三个参数是数据起始位置指针，
cmd命令参数是个32位整数，分为四部分：
dir(2b) size(14b) type(8b) nr(8b)
详细定义cmd要包括这4个部分时可使用宏_IOC(dir,type,nr,size)来定义，而最简单情况下使用_IO(type, nr)来定义就可以了，这些宏都在include/asm/ioctl.h中定义
本文cmd定义为：
#define NEWCHAR_IOC_MAGIC   'M'
#define NEWCHAR_SET    _IO(NEWCHAR_IOC_MAGIC, 0)
#define NEWCHAR_GET    _IO(NEWCHAR_IOC_MAGIC, 1)
#define NEWCHAR_IOC_MAXNR   1

要定义自己的ioctl操作，可以有两个方式，一种是在现有的内核代码中直接添加相关代码进行支持，比如想通过socket描述符进行 ioctl操作，可在net/ipv4/af_inet.c中的inet_ioctl()函数中添加自己定义的命令和相关的处理函数，重新编译内核即可， 不过这种方法一般不推荐；第二种方法是定义自己的内核设备，通过设备的ioctl()来操作，可以编成模块，这样不影响原有的内核，这是最通常的做法。

4. 内核设备

为进行ioctl操作最通常是使用字符设备来进行，当然定义其他类型的设备也可以。在用户空间，可使用mknod命令建立一个字符类型设备文件，假设该设备的主设备号为123，次设备号为0：
mknode /dev/newchar c 123 0
如果是编程的话，可以用mknode(2)函数来建立设备文件。

建立设备文件后再将该设备的内核模块文件插入内核，就可以使用open(2)打开/dev/newchar文件，然后调用ioctl(2)来传递数据，最后用close(2)关闭设备。而如果内核中还没有插入该设备的模块，open(2)时就会失败。

由于内核内存空间和用户内存空间不同，要将内核数据拷贝到用户空间，要使用专用拷贝函数copy_to_user()；要将用户空间数据拷贝到内核，要使用copy_from_user()。
要最简单实现以上功能，内核模块只需要实现设备的open, ioctl和release三个函数即可，
下面介绍程序片断：
static int newchar_ioctl(struct inode *inode, struct file *filep, 
   unsigned int cmd, unsigned long arg);
static int newchar_open(struct inode *inode, struct file *filep);
static int newchar_release(struct inode *inode, struct file *filep);
// 定义文件操作结构，结构中其他元素为空
struct file_operations newchar_fops = 
{
owner: THIS_MODULE,
ioctl: newchar_ioctl,
open: newchar_open,
release: newchar_release,
};
// 定义要传输的数据块结构
struct newchar{
int a;
int b;
};
#define MAJOR_DEV_NUM 123
#define DEVICE_NAME "newchar"

打开设备，非常简单，就是增加模块计数器，防止在打开设备的情况下删除模块,
当然想搞得复杂的话可进行各种限制检查，如只允许指定的用户打开等：
static int newchar_open(struct inode *inode, struct file *filep)
{
MOD_INC_USE_COUNT;
return 0;
}

关闭设备，也很简单，减模块计数器：
static int newchar_release(struct inode *inode, struct file *filep)
{
MOD_DEC_USE_COUNT;
return 0;
}

进行ioctl调用的基本处理函数
static int newchar_ioctl(struct inode *inode, struct file *filep, 
      unsigned int cmd, unsigned long arg)
{
int ret;
// 首先检查cmd是否合法
if (_IOC_TYPE(cmd) != NEWCHAR_IOC_MAGIC) return -EINVAL;
if (_IOC_NR(cmd) > NEWCHAR_IOC_MAXNR) return -EINVAL;
// 错误情况下的缺省返回值
ret = EINVAL;
switch(cmd)
{
case KNEWCHAR_SET:
// 设置操作，将数据从用户空间拷贝到内核空间
{
   struct newchar nc;
   if(copy_from_user(&nc, (const char*)arg, sizeof(nc)) != 0)
    return -EFAULT;
   ret = do_set_newchar(&nc);
}
break;
case KNEWCHAR_GET:
// GET操作通常会在数据缓冲区中先传递部分初始值作为数据查找条件，获取全部
// 数据后重新写回缓冲区
// 当然也可以根据具体情况什么也不传入直接向内核获取数据
{
   struct newchar nc;
   if(copy_from_user(&nc, (const char*)arg, sizeof(nc)) != 0)
    return -EFAULT;
   ret = do_get_newchar(&nc);
   if(ret == 0){
    if(copy_to_user((unsigned char *)arg, &nc, sizeof(nc))!=0)
     return -EFAULT;
   }
}
break;
}
return ret;
}
模块初始化函数，登记字符设备
static int __init _init(void)
{
int result;
// 登记该字符设备，这是2.4以前的基本方法，到2.6后有了些变化，
// 是使用MKDEV和cdev_init()来进行，本文还是按老方法
result = register_chrdev(MAJOR_DEV_NUM, DEVICE_NAME, &newchar_fops);
if (result < 0) {
printk(KERN_WARNING __FUNCTION__ ": failed register character device for /dev/newchar\n");
return result;
}
return 0;
}

模块退出函数，登出字符设备
static void __exit _cleanup(void)
{
int result;
result = unregister_chrdev(MAJOR_DEV_NUM, DEVICE_NAME);
if (result < 0)
printk(__FUNCTION__ ": failed unregister character device for /dev/newchar\n");
return;
} 
module_init(_init);
module_exit(_cleanup);

5. 结论

用ioctl()在用户空间和内核空间传递数据是最常用方法之一，比较简单方便，而且可以在同一个ioctl中对不同的命令传送不同的数据结构，本文只是为描述方便而在不同命令中使用了相同的数据结构。

from:http://hi.baidu.com/liaolihome/blog/item/e72e012a85412d9e033bf61b.html









