# 在Linux下写一个简单的驱动程序 - Koma Hub - CSDN博客
2019年01月27日 18:32:14[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：151
个人分类：[Linux																[Linux kernel																[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)](https://blog.csdn.net/Rong_Toa/article/category/7348890)](https://blog.csdn.net/Rong_Toa/article/category/7156254)
**目录**
[一个简单的驱动](#%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E9%A9%B1%E5%8A%A8)
[编写驱动程序](#%E7%BC%96%E5%86%99%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F)
[驱动程序的Makefile](#%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F%E7%9A%84Makefile)
[驱动程序的测试程序](#%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F%E7%9A%84%E6%B5%8B%E8%AF%95%E7%A8%8B%E5%BA%8F)
[驱动程序的编译](#%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F%E7%9A%84%E7%BC%96%E8%AF%91)
[加载驱动](#%E5%8A%A0%E8%BD%BD%E9%A9%B1%E5%8A%A8)
[init函数](#init%E5%87%BD%E6%95%B0)
[创建节点](#%E5%88%9B%E5%BB%BA%E8%8A%82%E7%82%B9)
[PS：/proc/devices 与 /dev的区别](#PS%EF%BC%9A%2Fproc%2Fdevices%20%E4%B8%8E%20%2Fdev%E7%9A%84%E5%8C%BA%E5%88%AB)
[运行上层应用程序](#%E8%BF%90%E8%A1%8C%E4%B8%8A%E5%B1%82%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F)
[编译：](#%E7%BC%96%E8%AF%91%EF%BC%9A)
[运行](#%E8%BF%90%E8%A1%8C)
[卸载驱动](#%E5%8D%B8%E8%BD%BD%E9%A9%B1%E5%8A%A8)
[彻底删除设备文件](#%E5%BD%BB%E5%BA%95%E5%88%A0%E9%99%A4%E8%AE%BE%E5%A4%87%E6%96%87%E4%BB%B6)
[驱动模板](#%E9%A9%B1%E5%8A%A8%E6%A8%A1%E6%9D%BF)
[头文件](#%E5%A4%B4%E6%96%87%E4%BB%B6)
[初始化函数](#%E5%88%9D%E5%A7%8B%E5%8C%96%E5%87%BD%E6%95%B0)
[退出函数](#%E9%80%80%E5%87%BA%E5%87%BD%E6%95%B0)
[版权信息](#%E7%89%88%E6%9D%83%E4%BF%A1%E6%81%AF)
[功能函数](#%E5%8A%9F%E8%83%BD%E5%87%BD%E6%95%B0)
[从上层应用到底层驱动的执行过程](#%E4%BB%8E%E4%B8%8A%E5%B1%82%E5%BA%94%E7%94%A8%E5%88%B0%E5%BA%95%E5%B1%82%E9%A9%B1%E5%8A%A8%E7%9A%84%E6%89%A7%E8%A1%8C%E8%BF%87%E7%A8%8B)
[linux系统的分层结构](#linux%E7%B3%BB%E7%BB%9F%E7%9A%84%E5%88%86%E5%B1%82%E7%BB%93%E6%9E%84)
[从上层应用到底层驱动的执行过程](#%E4%BB%8E%E4%B8%8A%E5%B1%82%E5%BA%94%E7%94%A8%E5%88%B0%E5%BA%95%E5%B1%82%E9%A9%B1%E5%8A%A8%E7%9A%84%E6%89%A7%E8%A1%8C%E8%BF%87%E7%A8%8B)
[驱动程序的执行特点](#%C2%A0%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F%E7%9A%84%E6%89%A7%E8%A1%8C%E7%89%B9%E7%82%B9)
# 一个简单的驱动
## 编写驱动程序
```cpp
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#define    HELLO_MAJOR     231
#define    DEVICE_NAME     "HelloModule"
static int hello_open(struct inode *inode, struct file *file){
    printk(KERN_EMERG "hello open. \nSome time you think you're ok\n"\
                            "but actually you're not ok.\n");
    return 0;
}
static int hello_write(struct file *file, const char __user * buf, size_t count, loff_t *ppos){
    printk(KERN_EMERG "hello write. \nSome time you think you're fine\n"\
                            "but actually you're not fine.\n");
    return 0;
}
static struct file_operations hello_flops = {
    .owner  =   THIS_MODULE,
    .open   =   hello_open,     
    .write  =   hello_write,
};
static int __init hello_init(void){
    int ret;
    
    ret = register_chrdev(HELLO_MAJOR,DEVICE_NAME, &hello_flops);
    if (ret < 0) {
      printk(KERN_EMERG DEVICE_NAME " can't register major number.\n");
      return ret;
    }
    printk(KERN_EMERG DEVICE_NAME " initialized.\n");
    return 0;
}
static void __exit hello_exit(void){
    unregister_chrdev(HELLO_MAJOR, DEVICE_NAME);
    printk(KERN_EMERG DEVICE_NAME " removed.\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
```
## 驱动程序的Makefile
```cpp
ifneq ($(KERNELRELEASE),)
MODULE_NAME = hellomodule
$(MODULE_NAME)-objs := hello.o
obj-m := $(MODULE_NAME).o
else
KERNEL_DIR = /lib/modules/`uname -r`/build
MODULEDIR := $(shell pwd)
.PHONY: modules
default: modules
modules:
	make -C $(KERNEL_DIR) M=$(MODULEDIR) modules
clean distclean:
	rm -f *.o *.mod.c .*.*.cmd *.ko
	rm -rf .tmp_versions
endif
```
## 驱动程序的测试程序
```cpp
#include <fcntl.h>
#include <stdio.h>
int main(void)
{
    int fd;
    int val = 1;
    fd = open("/dev/hellodev", O_RDWR);
    if(fd < 0){
        printf("can't open!\n");
    }
    write(fd, &val, 4);
    return 0;
}
```
## 驱动程序的编译
```
# ls
hello.c  hellotest.c  Makefile
# make 
make -C /lib/modules/`uname -r`/build M=/home/rongtao/driver/test2 modules
make[1]: 进入目录“/usr/src/kernels/3.10.0-693.el7.x86_64”
  CC [M]  /home/rongtao/driver/test2/hello.o
/home/rongtao/driver/test2/hello.c:25:5: 警告：从不兼容的指针类型初始化 [默认启用]
     .write  =   hello_write,
     ^
/home/rongtao/driver/test2/hello.c:25:5: 警告：(在‘hello_flops.write’的初始化附近) [默认启用]
  LD [M]  /home/rongtao/driver/test2/hellomodule.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /home/rongtao/driver/test2/hellomodule.mod.o
  LD [M]  /home/rongtao/driver/test2/hellomodule.ko
make[1]: 离开目录“/usr/src/kernels/3.10.0-693.el7.x86_64”
# ls
hello.c         hellomodule.mod.c  hellomodule.o  hellotest.c  modules.order
hellomodule.ko  hellomodule.mod.o  hello.o        Makefile     Module.symvers
```
编译可以不在root权限下运行。一下内容需在root用户执行。
## 加载驱动
```
# insmod hellomodule.ko 
insmod: ERROR: could not insert module hellomodule.ko: File exists
# rmmod hellomodule
Message from syslogd@toa at Jan 27 18:11:39 ...
 kernel:HelloModule removed.
# insmod hellomodule.ko 
Message from syslogd@toa at Jan 27 18:11:42 ...
 kernel:HelloModule initialized.
```
这里因为我上一次insmod的驱动没有被删除“rmmod”，所以会出现如上所示的ERROR信息。
同样，insmod加载驱动的时候，会调用函数hello_init()，打印的调试信息如下。
### init函数
```cpp
printk(KERN_EMERG DEVICE_NAME " initialized.\n");
```
对应insmod中的：
```cpp
kernel:HelloModule initialized.
```
## 创建节点
加载驱动后，在/proc/devices中会出现一行：
```
# grep Hello /proc/devices 
231 HelloModule
```
虽然已经加载了驱动hellomodule.ko，而且在/proc/devices文件中也看到了已经加载的模块HelloModule，但是这个模块仍然不能被使用，因为在设备目录/dev目录下还没有它对应的设备文件。所以，需要创建一个设备节点。
```
# mknod /dev/hellodev c 231 0
# ll /dev/hellodev 
crw-r--r--. 1 root root 231, 0 1月  27 18:16 /dev/hellodev
```
在/proc/devices中看到HelloModule模块的主设备号为231，创建节点的时候就是将设备文件/dev/hellodev与主设备号建立连接。这样在应用程序操作文件/dev/hellodev的时候，就会定位到模块HelloModule。
## PS：/proc/devices 与 /dev的区别
- /proc/devices中的设备是驱动程序生成的，它可产生一个major供mknod作为参数。这个文件中的内容显示的是当前挂载在系统的模块。当加载驱动HelloModule的时候，并没有生成一个对应的设备文件来对这个设备进行抽象封装，以供上层应用访问。
- /dev下的设备是通过mknod加上去的，用户通过此设备名来访问驱动。我以为可以将/dev下的文件看做是硬件模块的一个抽象封装，Linux下所有的设备都以文件的形式进行封装。
## 运行上层应用程序
### 编译：
`# gcc hellotest.c -o hellotest`
### 运行
```
# ./hellotest 
Message from syslogd@toa at Jan 27 18:19:30 ...
 kernel:hello open. #012Some time you think you're ok#012but actually you're not ok.
Message from syslogd@toa at Jan 27 18:19:30 ...
 kernel:hello write. #012Some time you think you're fine#012but actually you're not fine.
```
## 卸载驱动
```
# rmmod hellomodule
Message from syslogd@toa at Jan 27 18:20:09 ...
 kernel:HelloModule removed.
```
### 彻底删除设备文件
```
# rm /dev/hellodev 
rm：是否删除字符特殊文件 "/dev/hellodev"？y
```
# 驱动模板
从宏观上把握了驱动程序的框架，然后再从细节上完善驱动的功能，这是开发驱动程序的一般步骤。驱动模板必备要素有头文件、初始化函数、退出函数、版权信息，常用的扩展要素是增加一些功能函数完善底层驱动的功能
## 头文件
```cpp
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
```
- init.h 　　　　定义了驱动的初始化和退出相关的函数
- kernel.h 　　  定义了经常用到的函数原型及宏定义
- module.h 　　定义了内核模块相关的函数、变量及宏
## 初始化函数
```cpp
static int __init hello_init(void){
    int ret;
    ret = register_chrdev(HELLO_MAJOR,DEVICE_NAME,&hello_flops);
    if (ret < 0) {
          printk(KERN_EMERG DEVICE_NAME " can't register major number.\n");
          return ret;
    }
    printk(KERN_EMERG DEVICE_NAME " initialized.\n");
    return 0;
}
module_init(hello_init);
```
       当加载驱动到内核的时候，这个初始化函数就会被自动执行。
　　初始化函数顾名思义是用来初始化模块的，常用的功能是通过register_chrdev来注册函数。内核分配了一块内存（数组）专门用来存放字符设备的函数集，register_chrdev函数会在这个数组的HELLO_MAJOR位置将hello_flops中的内容进行填充，也就是将HelloModule的功能函数地址注册到设备管理内存集中。
　　形象的比喻好像是操作系统提供了很多的衣服架，注册设备就好像是把一个衣服挂到某一个衣服架上。衣服上有许多口袋，就好像每一个模块有许多功能程序接口。显然，如果想使用设备的某个功能，就可以先找到对应的衣服架，然后找到相应的口袋，去调用对应的函数，执行动作。
![](https://img-blog.csdnimg.cn/20190127182444419.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## 退出函数
```cpp
static void __exit hello_exit(void){
    unregister_chrdev(HELLO_MAJOR, DEVICE_NAME);
    printk(KERN_EMERG DEVICE_NAME " removed.\n");
}
module_exit(hello_exit);
```
　　当卸载驱动的时候，退出函数便会自动执行，完成一系列清理工作。
　　在加载驱动的时候，我们向设备管理内存集中注册了该模块的相关功能函数。当卸载驱动的时候，就有必要将这个模块占用的内存空间清空。这样当其他的设备注册的时候便有更多的空间可以选择。
　　形象的比喻是， 当卸载驱动的时候，就是把衣服从衣服架上取下来，这样衣服架就腾空了。
## 版权信息
```cpp
MODULE_LICENSE("GPL");
```
　　Linux内核是按照GPL发布的，同样Linux的驱动程序也要提供版权信息，否则当加载到内核中系统会给出警告信息。
## 功能函数
```cpp
static int hello_open(struct inode *inode, struct file *file){
    printk(KERN_EMERG "hello open. \nSome time you think you're ok\n"\
                            "but actually you're not ok.\n");
    return 0;
}
static int hello_write(struct file *file, const char __user * buf, size_t count, loff_t *ppos){
    printk(KERN_EMERG "hello write. \nSome time you think you're fine\n"\
                            "but actually you're not fine.\n");
    return 0;
}
static struct file_operations hello_flops = {
    .owner  =   THIS_MODULE,
    .open   =   hello_open,     
    .write  =   hello_write,
};
```
　　功能函数虽然不是一个驱动模板所必须的，但是一个有实际意义的驱动程序一定包含功能函数。功能函数实际上定义了这个驱动程序为用户提供了哪些功能，也就是用户可以对一个硬件设备可以进行哪些操作。
　　常见的功能函数有xxx_open()、xxx_write()、xxx_read()、xxx_ioctl()、xxx_llseek()等。当上层应用调用open()、write()、read()、ioctl()、llseek()等这些函数的时候，经过层层调用最后到达底层，调用相应的功能函数。结构体file_operations中的成员定义了很多函数，实际应用可以只对其部分成员赋值，其定义如下。
```cpp
struct file_operations {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    int (*readdir) (struct file *, void *, filldir_t);
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
    int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    int (*mmap) (struct file *, struct vm_area_struct *);
    int (*open) (struct inode *, struct file *);
    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *);
    int (*fsync) (struct file *, struct dentry *, int datasync);
    int (*aio_fsync) (struct kiocb *, int datasync);
    int (*fasync) (int, struct file *, int);
    int (*lock) (struct file *, int, struct file_lock *);
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
    int (*check_flags)(int);
    int (*flock) (struct file *, int, struct file_lock *);
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **);
};
```
# 从上层应用到底层驱动的执行过程
## linux系统的分层结构
![](https://img-blog.csdnimg.cn/20190127182919121.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
Linux系统的分层结构为：应用层 ----> 库 ----> 内核 ----> 驱动程序 ----> 硬件设备。
## **从上层应用到底层驱动的执行过程**
　　以“open("/dev/hellodev", O_RDWR)”函数的执行过程为例来说明。
- （1）应用程序使用库提供的open函数打开代表hellodev的设备文件。
- （2）库根据open函数传入的参数执行swi指令，这条指令会引起CPU异常，从而进入内核。
- （3）内核的异常处理函数根据这些参数找到相应的驱动程序。
- （4）执行相应的驱动程序。
- （5）返回一个文件句柄给库，进而返回给应用程序。
## ** 驱动程序的执行特点**
　　与应用程序不同，驱动程序从不主动运行，它是被动的：根据应用程序的要求进行初始化，根据应用程序的要求进行读写。驱动程序加载进内核，只是告诉内核“我在这里，我能做这些工作”，至于这些工作何时开始，则取决于应用程序。
　　驱动程序运行于“内核空间”，它是系统“信任”的一部分，驱动程序的错误有可能导致整个系统的崩溃。
**参考资料：**
- [linux驱动开发框架](http://www.cnblogs.com/hnrainll/archive/2011/06/14/2080963.html)
- 《嵌入式Linux应用开发完全手册》
原文：[https://www.cnblogs.com/amanlikethis/p/4914510.html](https://www.cnblogs.com/amanlikethis/p/4914510.html)
非常感谢原文作者的贡献，讲解的非常清晰通俗易懂。
