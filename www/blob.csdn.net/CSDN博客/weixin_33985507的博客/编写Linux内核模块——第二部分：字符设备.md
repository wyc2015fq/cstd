# 编写Linux内核模块——第二部分：字符设备 - weixin_33985507的博客 - CSDN博客
2015年11月01日 18:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
【编者的话】字符设备作为Linux设备中的一大类，它提供对按字节访问设备的抽象。用户空间应用程序可以通过标准文件操作来访问设备。本文来自Derek Molloy的博客，介绍了如何字符设备驱动的概念，以及如何编写和测试一个字符设备驱动。\
## 前言
\
本系列文章中，主要描述如何为嵌入式Linux设备编写可加载内核模块（LKM）。这是该系列的第二篇文章，在阅读本文之前，请先阅读[《编写Linux内核模块——第一部分：前言》](%5C)，它讲解了如何构建、加载和卸载可加载内核模块。这些描述不在本文中不再赘述。\
## 字符设备驱动
\
字符设备通常和用户应用程序双向传输数据，它们的行为类似管道和串行接口，即时从字符流中读写字节数据。它们为许多典型的驱动提供了框架，比如那些需要和串行设备、视频捕捉设备和音频设备交互的驱动。字符设备的一种替代是块设备。块设备的行为类似普通文件，它可以允许程序查看缓存数据中的缓冲队列，或是通过读、写、查找等函数进行操作。两种设备类型都可以通过关联到文件系统树上的设备文件进行访问。例如，本文中的代码构建后，可以通过以下方式在Linux系统中创建**/dev/ebbchar**设备文件：
molloyd@beaglebone:~/exploringBB/extras/kernel/ebbchar$ lsmod\Module                  Size  Used by\ebbchar                 2754  0\molloyd@beaglebone:~/exploringBB/extras/kernel/ebbchar$ ls -l /dev/ebb*\crw-rw-rwT 1 root root 240, 0 Apr 11 15:34 /dev/ebbchar\
本文介绍了一个简单的字符设备，可用于用户空间应用程序和运行在内核空间的内核模块之间相互传递消息。在示例中，用C编写的用户空间应用程序发送字符串到内核模块。内核模块响应这条消息，并发回这条消息包含的字母数。然后，本文还将介绍为什么需解决示例中这种实现方式引发的同步问题，并且提供一个使用互斥锁的版本，解决这个同步问题。\
在描述本文驱动的源代码前，需要先讨论一些概念，比如设备驱动的主设备号和次设备号，还有文件操作数据结构。\
### 主设备号和次设备号
\
设备驱动有关联的主设备号和次设备号。例如，/dev/ram0和/dev/null关联了主设备号为1的驱动，而/dev/tty0和/dev/ttyS0关联了主设备号为4的驱动。主设备号用于内核在设备访问时能够识别正确的设备驱动。次设备号的角色和设备相关，它主要使用在驱动中。如果在/dev目录中执行列出文件操作，可以看见每个设备的主/次设备号。比如：
molloyd@beaglebone:/dev$ ls -l\crw-rw---T 1 root i2c      89,   0 Jan  1  2000 i2c-0\brw-rw---T 1 root disk      1,   0 Mar  1 20:46 ram0\brw-rw---T 1 root floppy  179,   0 Mar  1 20:46 mmcblk0\crw-rw-rw- 1 root root      1,   3 Mar  1 20:46 null\crw------- 1 root root      4,   0 Mar  1 20:46 tty0\crw-rw---T 1 root dialout   4,  64 Mar  1 20:46 ttyS0\…\
输出中的第一列为“c”，表示这是一个字符设备，而为“b”表示这是一个块设备。每个设备都有授权访问的用户和组。BeagleBone上的普通用户帐号是这些组中的成员，因此有权限访问i2c-0和ttyS0等设备。
molloyd@beaglebone:/dev$ groups\molloyd dialout cdrom floppy audio video plugdev users i2c spi\
本文开发的设备将在**/dev**目录中以设备文件的形式出现（**/dev/ebbchar**）。\
也可以手工创建一个块设备或者字符设备文件项，然后将它关联到指定设备上（即sudo mknod /dev/test c 92 1），但是这个方式容易出现问题。其中一个问题就是，必须确保使用的设备号（即示例中的92）没有被使用。在BeagleBone可以通过/usr/src/linux-headers-3.8.13-bone70/include/uapi/linux/major.h文件检查所有系统设备的主设备号。然后，使用此方法找到“唯一”的主设备号是不可移植的，因为在其他设备或者其他Linux单板机（发行版）中，主设备号可能冲突。本文的代码自动确认并使用一个合适的主设备号。\
### 文件操作数据结构
\
**file_operations**数据结构定义在/linux/fs.h头文件中，它保存驱动中的函数指针，允许开发者定义文件操作行为。例如，列表1是从/linux/fs.h头文件中摘录的数据结构的片段。本文中的驱动提供了文件操作中的读、写、打开、释放这几个系统调用的实现。如果数据结构中的某些字段不需要实现，只需要简单的将它指向NULL，这样这些字段将不可访问。列表1展示的操作函数的数量看上去是比较吓人的。然而，构建ebbchar内核模块，只需要提供其中四个字段的实现即可。因此，列表1提供了在驱动框架中可以扩展使用的额外函数接口。
 // 注意：__userNote指向用户空间地址。\struct file_operations {\   struct module *owner;                             // 指向拥有该结构的内核模块\   loff_t (*llseek) (struct file *, loff_t, int);    // 修改中间中当前读写的位置\   ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);    // 用于从设备读取数据\   ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);   // 用于向设备发送数据\   ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);  // 异步读\   ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t); // 异步写\   ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);            // 可能异步读\   ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);           // 可能异步写\   int (*iterate) (struct file *, struct dir_context *);                // 当虚拟文件系统（VFS）需要读取文件夹内容的时候调用\   unsigned int (*poll) (struct file *, struct poll_table_struct *);    // 读或写会阻塞？\   long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long); // 由ioctl系统调用使用\   long (*compat_ioctl) (struct file *, unsigned int, unsigned long);   // 由ioctl系统调用使用\   int (*mmap) (struct file *, struct vm_area_struct *);                // 由mmap系统调用使用\   int (*mremap)(struct file *, struct vm_area_struct *);               // 由remap系统调用使用\   int (*open) (struct inode *, struct file *);             // 设备文件初次使用时调用\   int (*flush) (struct file *, fl_owner_t id);             // 进程关闭或者复制文件描述符时调用\   int (*release) (struct inode *, struct file *);          // 当文件结构释放是调用\   int (*fsync) (struct file *, loff_t, loff_t, int datasync);  // 通知设备修改FASYNC标志\   int (*aio_fsync) (struct kiocb *, int datasync);         // 同步通知设备修改FASYNC标志\   int (*fasync) (int, struct file *, int);                 // 异步通知设备修改FASYNC标志\   int (*lock) (struct file *, int, struct file_lock *);    // 用于文件锁的实现\   …\};\
列表1：/linux/fs.h中的文件操作数据结构（片段）\
要了解更多信息，[Kernel.org虚拟文件系统](%5C)为文件操作数据结构提供了优秀的文档。\
## 本次讨论的源码
\
本次讨论的所有代码都在为《Exploring BeagleBone》准备的GitHub仓库上。代码可以在[ExploringBB GitHub仓库内核工程目录中](%5C)公开查看，或者也可以将代码复制到BeagleBone（或者其他Linux设备）：
molloyd@beaglebone:~$ sudo apt-get install git\molloyd@beaglebone:~$ git clone https://github.com/derekmolloy/exploringBB.git\
代码中[/extras/kernel/ebbchar](%5C)目录是本文最重要的资源。为这些示例代码自动生成的Doxygen文档有[HTML格式](%5C)和[PDF格式](%5C)。\
## 设备驱动源码
\
ebbchar设备驱动源码展示在列表2中。和本系列第一篇文章类似，里面有一个init()函数和exit()函数。除此之外，字符设备还需要一些额外的文件操作函数：\
- dev_open()：用空空间每次打开设备的时候调用。 \
- dev_read()：从设备向用户空间发送数据的时候调动。 \
- dev_write()：从用户空间向设备发送数据的时候调用。 \
- dev_release()：用户空间关闭设备的时候调用。
设备驱动有一个类名和设备名。在列表2中，**ebb**（探索BeagleBone，Exploring BeagleBone）作为类名，**ebbchar**作为设备名。这使得设备最终显示在文件系统的**/sys/class/ebb/ebbchar**中。
/**\ * @file   ebbchar.c\ * @author Derek Molloy\ * @date   2015年4月7日\ * @version 0.1\ * @brief   一个介绍性的字符设备驱动，作为Linux可加载内核驱动系列文章第二篇的示例。\ * 该模块映射到/dev/ebbchar文件中，并且提供一个运行于Linux用户空间的C程序，\ * 来和此内核模块进行交互。\ * @see http://www.derekmolloy.ie/ 查看完整描述和补充描述。\ */\\#include \u0026lt;linux/init.h\u0026gt;           // 用于标记函数的宏，如_init、__exit\#include \u0026lt;linux/module.h\u0026gt;         // 将内核模块加载到内核中的核心头文件\#include \u0026lt;linux/device.h\u0026gt;         // 支持内核驱动模型的头文件\#include \u0026lt;linux/kernel.h\u0026gt;         // 包含内核中的类型、宏和函数\#include \u0026lt;linux/fs.h\u0026gt;             // 支持Linux文件系统的头文件\#include \u0026lt;asm/uaccess.h\u0026gt;          // 复制到用户用户空间函数需要的头文件\#define  DEVICE_NAME \"ebbchar\"    ///\u0026lt; 使用此值，设备将会展示在/dev/ebbchar\#define  CLASS_NAME  \"ebb\"        ///\u0026lt; 设备类名，这是一个字符设备驱动\\MODULE_LICENSE(\"GPL\");            ///\u0026lt; 许可类型，这回影响到可用功能\MODULE_AUTHOR(\"Derek Molloy\");    ///\u0026lt; 作者，当使用modinfo命令时可见\MODULE_DESCRIPTION(\"A simple Linux char driver for the BBB\");  ///\u0026lt; 描述，参见modinfo命令\MODULE_VERSION(\"0.1\");            ///\u0026lt; 告知用户的版本号\\static int    majorNumber;                  ///\u0026lt; 保存主设备号，这里自动确定\static char   message[256] = {0};           ///\u0026lt; 用于保存从用户空间传输过来字符串的内存\static short  size_of_message;              ///\u0026lt; 用于记录保存的字符串长度\static int    numberOpens = 0;              ///\u0026lt; 用于保存设备打开次数的计数器\static struct class*  ebbcharClass  = NULL; ///\u0026lt; 设备驱动类结构体指针\static struct device* ebbcharDevice = NULL; ///\u0026lt; 设备驱动设备结构体指针\\// 字符设备操作的函数原型，必须在结构体定义前定义\static int     dev_open(struct inode *, struct file *);\static int     dev_release(struct inode *, struct file *);\static ssize_t dev_read(struct file *, char *, size_t, loff_t *);\static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);\\/** @brief 设备在内核中被表示为文件结构。 /linux/fs.h中定义的file_operations结构体，\ * 它使用C99语法的结构体，列举了文件操作关联的回调函数。\ * 字符设备通常需要实现open、read、write和release函数。\ */\static struct file_operations fops =\{\   .open = dev_open,\   .read = dev_read,\   .write = dev_write,\   .release = dev_release,\};\\/** @brief 可加载内核模块初始化函数\ *  static关键字限制该函数的可见性在该C文件之内。 The __init\ *  __init宏对于内置驱动（非可加载内核模块）来说，只在初始化时调用，在此之后，该函数将被废弃，内存将被回收。\ *  @return 如果成功返回0\ */\static int __init ebbchar_init(void){\   printk(KERN_INFO \"EBBChar: Initializing the EBBChar LKM\\");\\   // 尝试为这个设备动态生成一个主设备号，虽然麻烦一点，但这是值得的\   majorNumber = register_chrdev(0, DEVICE_NAME, \u0026amp;fops);\   if (majorNumber\u0026lt;0){\      printk(KERN_ALERT \"EBBChar failed to register a major number\\");\      return majorNumber;\   }\   printk(KERN_INFO \"EBBChar: registered correctly with major number %d\\
