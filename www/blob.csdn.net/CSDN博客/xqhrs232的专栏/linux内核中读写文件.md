# linux内核中读写文件 - xqhrs232的专栏 - CSDN博客
2011年10月26日 10:18:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1065标签：[linux内核																[file																[module																[makefile																[linux																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/billowszpt/article/details/6664400](http://blog.csdn.net/billowszpt/article/details/6664400)
相关网页::在linux内核(驱动)中读写文件----[http://freshpassport.blog.51cto.com/2359526/619676](http://freshpassport.blog.51cto.com/2359526/619676)
[http://blog.csdn.net/yihui8/archive/2010/06/26/5696135.aspx](http://blog.chinaunix.net/link.php?url=http://blog.csdn.net%2Fyihui8%2Farchive%2F2010%2F06%2F26%2F5696135.aspx)
之前以为在内核中不能创建文件,其实不是这样子的,只要文件系统跑起来之后就可以像在用户空间一样操作文件.
用户空间                   内核
open()                    sys_open(), filp_open()
close()                   sys_close(), filp_close()
read()                    sys_read(), filp_read()
write()                   sys_write(), filp_write()
----------------------------------------------------
在内核模块中有时会用不了 sys_xxxx, 这时用 filp_xxxx 对应的函数就行了, 在模块中使用 sys_xxxx 还有问题, 还没有找到原因.
在 /tmp 中创建文件 aa
struct file *filep;
filep=filp_open("/tmp/aa",O_CREAT | O_RDWR,0);
if(IS_ERR(filep))
  return -1;
filp_close(filep,0);
return 0;
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
sys_open 的使用方法：
#include <linux/fs.h>
......
 u8  Buff[50];
 int fd;
 memset(Buff, 0x00, sizeof(Buff));
 mm_segment_t old_fs = get_fs();
 set_fs(KERNEL_DS);
 fd = sys_open("/etc/Info", O_RDONLY, 0);
 if(fd>=0)
 {
  sys_read(fd, Buff, 50);
  printk("string: %s\n", Buff);
  sys_close(fd);
 }
 set_fs(old_fs);
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
下面 open 使用的一些参数:
O_ACCMODE <0003>;: 读写文件操作时，用于取出flag的低2位。 
O_RDONLY<00>;: 只读打开 
O_WRONLY<01>;: 只写打开 
O_RDWR<02>;: 读写打开 
O_CREAT<0100>;: 文件不存在则创建，需要mode_t，not fcntl 
O_EXCL<0200>;: 如果同时指定了O_CREAT，而文件已经存在，则出错， not fcntl 
O_NOCTTY<0400>;: 如果pathname指终端设备，则不将此设备分配作为此进程的控制终端。not fcntl O_TRUNC<01000>;: 如果此文件存在，而且为只读或只写成功打开，则将其长度截短为0。not fcntl 
O_APPEND<02000>;: 每次写时都加到文件的尾端 
O_NONBLOCK<04000>;: 如果p a t h n a m e指的是一个F I F O、一个块特殊文件或一个字符特殊文件，则此选择项为此文件的本次打开操作和后续的I / O操作设置非阻塞方式。 
O_NDELAY;; 
O_SYNC<010000>;: 使每次write都等到物理I/O操作完成。 
FASYNC<020000>;: 兼容BSD的fcntl同步操作 
O_DIRECT<040000>;: 直接磁盘操作标识 
O_LARGEFILE<0100000>;: 大文件标识 
O_DIRECTORY<0200000>;: 必须是目录 
O_NOFOLLOW<0400000>;: 不获取连接文件 
O_NOATIME<01000000>;: 暂无 
当新创建一个文件时，需要指定mode 参数，以下说明的格式如宏定义名称<实际常数值>;: 描述。 
S_IRWXU<00700>;：文件拥有者有读写执行权限 
S_IRUSR (S_IREAD)<00400>;：文件拥有者仅有读权限 
S_IWUSR (S_IWRITE)<00200>;：文件拥有者仅有写权限 
S_IXUSR (S_IEXEC)<00100>;：文件拥有者仅有执行权限 
S_IRWXG<00070>;：组用户有读写执行权限 
S_IRGRP<00040>;：组用户仅有读权限 
S_IWGRP<00020>;：组用户仅有写权限 
S_IXGRP<00010>;：组用户仅有执行权限 
S_IRWXO<00007>;：其他用户有读写执行权限 
S_IROTH<00004>;：其他用户仅有读权限 
S_IWOTH<00002>;：其他用户仅有写权限 
S_IXOTH<00001>;：其他用户仅有执行权限
   Linux驱动编程书籍大多数都是介绍怎样用户态下怎么访问硬件设备，由于项目的需要，本人做了内核态下访问设备文件的方法，现在把程序拿出来和大家分享一下，希望对刚入门的朋友有所帮助。
   在我的《内核模块调用驱动》中给出了简单的字符设备文件程序，可以作为本文的驱动对象，在此，我就不多介绍了。调用驱动程序的模块如下：
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include "chardev.h"
MODULE_LICENSE("GPL");
//#define __KERNEL_SYSCALLS__
#define bao "/dev/baovar"
static char buf1[20];
static char buf2[20];
static int __init testmod_init(void)
{
mm_segment_t old_fs;
ssize_t result;
ssize_t ret;
sprintf(buf1,"%s","baoqunmin");
struct file *file=NULL;
file=filp_open(bao,O_RDWR,0);
if(IS_ERR(file)) goto fail0;
old_fs=get_fs();
set_fs(get_ds());
ret=file->f_op->write(file,buf1,sizeof(buf1),&file->f_pos);
result=file->f_op->read(file,buf2,sizeof(buf2),&file->f_pos);
if(result>=0){buf2[20]='\n';printk("buf2-->%s\n",buf2);}
else printk("failed\n");
result=file->f_op->ioctl(file,buf2,sizeof(buf2),&file->f_pos);
result=file->f_op->read(file,buf2,sizeof(buf2),&file->f_pos);
set_fs(old_fs);
   filp_close(file,NULL);
 printk("file loaded\n");
    return 0;
fail0:{filp_close(file,NULL);printk("load failed\n");}
return 1;
}
static void __exit testmod_cleanup(void)
{
    printk("module exit......................................................\n");
}
module_init(testmod_init);
module_exit(testmod_cleanup);
以上是完整的程序，直接可以编译运行。
#include "chardev.h"头文件定义如下，此头文件也必须在驱动中包含！
#include <linux/ioctl.h>
#define BAO_IOCTL 't'
#define IOCTL_READ  _IOR(BAO_IOCTL, 0, int)
#define IOCTL_WRITE  _IOW(BAO_IOCTL, 1, int)
#define BAO_IOCTL_MAXNR 1
以下给出了我的Makefile文件：
CC=gcc
MODCFLAGS := -Wall -DMODULE -D__KERNEL__ -DLINUX -I/usr/src/linux-2.4.20-8/include
test.o :test.c 
 $(CC) $(MODCFLAGS) -c test.c
 echo insmod test.o to turn it on
 echo rmmod test to turn it off
 echo 
1.先加载设备驱动，我的设备文件为bao,
2.再make以上文件
3./sbin/insmod test.o加载模块
4.dmesg 查看运行结果
5./sbin/rmmod test
6.卸载加载的驱动
我在Linux red hat 9.0下成功运行
我实现的只是在模块间对驱动程序的调用，可以说是这是一个最简单的例子。
希望大家能有所收获！
//====================================================
备注:
1>其实这个方法还是在系统跑起来之后才进行操作的!!!
