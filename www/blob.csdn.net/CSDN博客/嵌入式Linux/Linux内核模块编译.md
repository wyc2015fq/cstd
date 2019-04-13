
# Linux内核模块编译 - 嵌入式Linux - CSDN博客

2018年07月25日 19:12:52[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1112所属专栏：[微信公众号:嵌入式linux](https://blog.csdn.net/column/details/26885.html)



Linux内核模块是一种可被动态加载和卸载的可执行程序。通过内核模块可以扩展内核功能，内核模块通常用于设备驱动、文件系统等。如果没有内核模块，需要向内核添加功能就需要自发代码、重新编译内核、安装新内核等步骤。
内核空间中不止一个程序试图访问驱动程序模块，导致一个内核块在没有被调用完毕之前又被其他程序调用，这称之炒代码的重入。因此在设计驱动模块的数据结构时要考虑数据结构在多线程环境下不被其他线程破坏，对于共享的数据应该采取加锁的方法保护。
可以把内核比喻成一个很长的火车，每个车厢就是一个内核模块，内核在运行这个火车就会一直在开动，但是我们想在火车开动的情况下增加新的车厢，这个时候就需要insmod，意思就是往这个长长的火车车厢增加一个内核模块。
下载当前版本的Linux源代码
\#uname -a
![](https://img-blog.csdn.net/20180725190732944?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
用apt-cache search linux-source命令对应的linux内核版本
用apt-get install linux-source-4.4.0下载对应的linux内核源码
下载的源码在/usr/src/
tar -jxv -f linux-source-4.4.0.tar.bz2 -C /data/weiqifa/
把源码解压到我的用户目录下
![](https://img-blog.csdn.net/20180725190733141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、编写模块的Makefile和内核模块代码
`ifneq ($(KERNELRELEASE),)  
  
obj-m :=hello.o  
  
else  
KDIR:= /lib/modules/4.4.0-62-generic/build/  
  
all:  
    make -C $(KDIR) M=$(PWD) modules   
clean:  
    rm -f *.ko *.o *.mod.o *.mod.c .symvers  
  
endif

hello.c代码
#include <linux/module.h>  
#include <linux/init.h>  
  
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("DAVID");  
MODULE_DESCRIPTION("Hello world module");  
  
static int __init hello_init(void)  
{  
    printk(KERN_ERR "hello world!\n");  
    return 0;  
}  
static void __exit hello_exit(void)  
{  
    printk(KERN_EMERG "hello exit!\n");  
}  
  
module_init(hello_init);  
module_exit(hello_exit);`3、编译
![](https://img-blog.csdn.net/20180725190733127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
编译后生成如下文件
![](https://img-blog.csdn.net/20180725190733154?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4、执行命令
![](https://img-blog.csdn.net/20180725190733115?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5、日志打印
输入dmesg可以看到内核日志
![](https://img-blog.csdn.net/20180725190733311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
喜欢可以关注微信公众号：嵌入式Linux


