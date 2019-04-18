# Linux下的C编程实战之五驱动程序开发 - nosmatch的专栏 - CSDN博客
2011年07月26日 23:26:01[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：509标签：[linux																[程序开发																[编程																[c																[struct																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=程序开发&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)

**1.引言**
　　设备驱动程序是操作系统内核和机器硬件之间的接口，它为应用程序屏蔽硬件的细节，一般来说，[**Linux**](http://search.yesky.com/search.do?stype=0&wd=linux)的设备驱动程序需要完成如下功能：
　　（1）初始化设备；
　　（2）提供各类设备服务；
　　（3）负责内核和设备之间的数据交换；
　　（4）检测和处理设备工作过程中出现的错误。
　　妙不可言的是，Linux下的设备驱动程序被组织为一组完成不同任务的函数的集合，通过这些函数使得Windows的设备操作犹如文件一般。在应用程序看来，硬件设备只是一个设备文件，应用程序可以象操作普通文件一样对硬件设备进行操作。本系列文章的第2章文件系统编程中已经看到了这些函数的真面目，它们就是open ()、close ()、read ()、write () 等。
　　Linux主要将设备分为二类：字符设备和块设备（当然网络设备及USB等其它设备的驱动编写方法又稍有不同）。这两类设备的不同点在于：在对字符设备发出读/写请求时，实际的硬件I/O一般就紧接着发生了，而块设备则不然，它利用一块系统内存作缓冲区，当用户进程对设备请求能满足用户的要求，就返回请求的数据，如果不能，就调用请求函数来进行实际的I/O操作。块设备主要针对磁盘等慢速设备。以字符设备的驱动较为简单，因此本章主要阐述字符设备的驱动编写。
**2.驱动模块函数**
　　init 函数用来完成对所控设备的初始化工作，并调用register_chrdev() 函数注册字符设备。假设有一字符设备“exampledev”，则其init 函数为：
void exampledev_init(void)
{
　if (register_chrdev(MAJOR_NUM, " exampledev ", &exampledev_fops))
　　TRACE_TXT("Device exampledev driver registered error");
　else
　　TRACE_TXT("Device exampledev driver registered successfully");
　　…//设备初始化
}
　　其中，register_chrdev函数中的参数MAJOR_NUM为主设备号,“exampledev”为设备名，exampledev_fops为包含基本函数入口点的结构体，类型为file_operations。当执行exampledev_init时，它将调用内核函数register_chrdev，把驱动程序的基本入口点指针存放在内核的字符设备地址表中，在用户进程对该设备执行系统调用时提供入口地址。
　　file_operations结构体定义为：
struct file_operations
{
　int (*lseek)();
　int (*read)();
　int (*write)();
　int (*readdir)();
　int (*select)();
　int (*ioctl)();
　int (*mmap)();
　int (*open)();
　void(*release)();
　int (*fsync)();
　int (*fasync)();
　int (*check_media_change)();
　void(*revalidate)();
};
　　大多数的驱动程序只是利用了其中的一部分，对于驱动程序中无需提供的功能，只需要把相应位置的值设为NULL。对于字符设备来说，要提供的主要入口有：open ()、release ()、read ()、write ()、ioctl ()。
　　open()函数　对设备特殊文件进行open()系统调用时，将调用驱动程序的open () 函数：
int open(struct inode * inode ,struct file * file);
　　其中参数inode为设备特殊文件的inode (索引结点) 结构的指针，参数file是指向这一设备的文件结构的指针。open()的主要任务是确定硬件处在就绪状态、验证次设备号的合法性(次设备号可以用MINOR(inode-> i - rdev) 取得)、控制使用设备的进程数、根据执行情况返回状态码(0表示成功，负数表示存在错误) 等；
　　release()函数　当最后一个打开设备的用户进程执行close ()系统调用时，内核将调用驱动程序的release () 函数：
void release (struct inode * inode ,struct file * file) ;
　　release 函数的主要任务是清理未结束的输入/输出操作、释放资源、用户自定义排他标志的复位等。
read()函数　当对设备特殊文件进行read() 系统调用时，将调用驱动程序read() 函数：
void read(struct inode * inode ,struct file * file ,char * buf ,int count) ;
　　参数buf是指向用户空间缓冲区的指针，由用户进程给出，count 为用户进程要求读取的字节数，也由用户给出。
　　read() 函数的功能就是从硬设备或内核内存中读取或复制count个字节到buf 指定的缓冲区中。在复制数据时要注意，驱动程序运行在内核中，而buf指定的缓冲区在用户内存区中，是不能直接在内核中访问使用的，因此，必须使用特殊的复制函数来完成复制工作，这些函数在<asm/ segment.h>中定义：
void put_user_byte (char data_byte ,char * u_addr) ;
void put_user_word (short data_word ,short * u_addr) ;
void put_user_long(long data_long ,long * u_addr) ;
void memcpy_tofs (void * u_addr ,void * k_addr ,unsigned long cnt) ;
　　参数u_addr为用户空间地址，k_addr 为内核空间地址，cnt为字节数。
　　write( ) 函数　当设备特殊文件进行write () 系统调用时，将调用驱动程序的write () 函数：
void write (struct inode * inode ,struct file * file ,char * buf ,int count) ;
　　write ()的功能是将参数buf 指定的缓冲区中的count 个字节内容复制到硬件或内核内存中，和read() 一样，复制工作也需要由特殊函数来完成：
unsigned char_get_user_byte (char * u_addr) ;
unsigned char_get_user_word (short * u_addr) ;
unsigned char_get_user_long(long * u_addr) ;
unsigned memcpy_fromfs(void * k_addr ,void * u_addr ,unsigned long cnt) ;
　　ioctl() 函数　该函数是特殊的控制函数，可以通过它向设备传递控制信息或从设备取得状态信息，函数原型为：
int ioctl (struct inode * inode ,struct file * file ,unsigned int cmd ,unsigned long arg);
　　参数cmd为设备驱动程序要执行的命令的代码，由用户自定义，参数arg 为相应的命令提供参数，类型可以是整型、指针等。
　　同样，在驱动程序中，这些函数的定义也必须符合命名规则，按照本文约定，设备“exampledev”的驱动程序的这些函数应分别命名为exampledev_open、exampledev_ release、exampledev_read、exampledev_write、exampledev_ioctl，因此设备“exampledev”的基本入口点结构变量exampledev_fops 赋值如下：
struct file_operations exampledev_fops {
　NULL ,
　exampledev_read ,
　exampledev_write ,
　NULL ,
　NULL ,
　exampledev_ioctl ,
　NULL ,
　exampledev_open ,
　exampledev_release ,
　NULL ,
　NULL ,
　NULL ,
　NULL
} ;
