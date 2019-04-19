# Linux驱动模块初始教程：一步一步，从helloworld到insmod->printk!!! - xqhrs232的专栏 - CSDN博客
2016年07月12日 23:16:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：945
原文地址::[http://blog.csdn.net/fareast8612/article/details/7470386](http://blog.csdn.net/fareast8612/article/details/7470386)
相关文章
1、insmod: error inserting 'cmos_driver.ko' : -1 File exists 和 insmod: error inserting './cmos_driver.ko': -1 Unknown symbol in module----[http://www.eefocus.com/jefby1990/blog/13-04/292699_423f7.html](http://www.eefocus.com/jefby1990/blog/13-04/292699_423f7.html)
2、[编译一个自己的内核模块，并加载进内核](http://blog.chinaunix.net/uid-26905027-id-3772490.html) ----[http://blog.chinaunix.net/uid-26905027-id-3772490.html](http://blog.chinaunix.net/uid-26905027-id-3772490.html)
转自：[http://hi.baidu.com/yangfanxing__/blog/item/61febf09242195c23bc7638f.html](http://hi.baidu.com/yangfanxing__/blog/item/61febf09242195c23bc7638f.html)
【0】笔者的配置环境
XP->VMWare 7.1->Ubuntu 9.04
【1】有必要查询下Linux内核
# uname -r
2.6.28-11-generic
# ls /usr/src/
linux-headers-2.6.28-11 linux-headers-2.6.28-11-generic
由此可见内核版本和内核头文件版本是一致的，都是2.6.28-11。（如果不一致的话在insmod一步必定出错：
Error inserting './hello.ko': -1 Invalid module format
网上有纠正这个错误的方法，但是感觉是在投机——躲避内核的版本检查；笔者在安装Ubuntu 8.04的时候出现过header头文件和内核版本不匹配的问题，后来通过重装Ubuntu为9.04解决之）。
【2】编写hello.c
新建自己的工作目录，如：
# mkdir /home/wk/hello
编写hello.c
# cd /home/wk/hello
# gedit hello.c
加入以下内容：
//Begin---hello.c
#include</usr/src/linux-headers-2.6.28-11/include/linux/init.h>
#include</usr/src/linux-headers-2.6.28-11/include/linux/module.h>
MODULE_LICENSE("GPL");
//printk(KERN_ALERT "Begin\n");
static int hello_init(void)
{
printk(KERN_ALERT "Hello World!\n");
return 0;
}
static void hello_exit(void)
{
printk(KERN_ALERT "Good bye, ubuntu\n");
// return 0;
}
module_init(hello_init);
module_exit(hello_exit);
//End---hello.c
注意第一行
#include</usr/src/linux-headers-2.6.28-11/include/linux/init.h>
位置要正确；或者你只要写成
#include<linux/init.h>
保存退出（Ctrl+Q）。
【3】编写Makefile
# cd /home/wk/hello
# gedit Makefile
注意大小写。
#Begin---Makefile
KERNELDIR=/lib/modules/2.6.28-11-generic/build
PWD:=$(shell pwd)
INSTALLDIR=/home/wk/hello/install
obj-m:= hello.o
modules:
$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
modules_install:
cp hello.ko $(INSTALLDIR)
clean:
rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions
.PHONY: modules modules_install clean
#End---Makefile
网上有Makefile的详解，就不冗述了。
保存退出。
【4】开始make
# make
输出类似以下消息：
make -C /lib/modules/2.6.28-11-generic/build M=/home/wk/hello modules
make[1]: 正在进入目录 `/usr/src/linux-headers-2.6.28-11-generic'
Building modules, stage 2.
MODPOST 1 modules
make[1]:正在离开目录 `/usr/src/linux-headers-2.6.28-11-generic'
则编译成功,用
# ls
查看应该可以看到生成一堆文件，如：
hello.c   hello.mod.c install    Makefile1       Module.symvers
hello.c~ hello.mod.o Makefile   Module.markers
hello.ko hello.o      Makefile~ modules.order
【5】安装模块：
# insmod hello.ko
或者
# insmod ./hello.ko
一个意思。
这一步完了没反应。接着再试一次：
# insmod hello.ko
提示错误：
insmod: error inserting 'hello.ko': -1 File exists
说明hello模块是安装成功的。
用
# lsmod
也能查看到hello模块。
防止打印太多，用：
# lsmod | head -5
只显示前5行等。如：
Module                  Size Used by
hello                   9344 0 
binfmt_misc            16776 1 
bridge                 56340 0 
stp                    10500 1 bridge
看到hello模块了吧~~~
可是为什么不打印呢？先不说这个。。。
【6】卸载模块
# rmmod hello.ko
再用
# lsmod | head -5
发现hello模块已经不存在了。
【7】关于printk
现在谈谈这个问题：insmod的时候，printk问什么没有在终端中显示Hello World呢？因为hello.c中明明是这么写的：
static int hello_init(void)
{
printk(KERN_ALERT "Hello World!\n");
return 0;
}
答案只有一句：printk不显示在超级终端上！靠！为什么？我也不知道。。。
【8】printk调试信息打印的补救方法
用命令：
# dmesg | tail -8
这个命令格式类似于lsmod（只显示最后8行）。
[   95.586960] Good bye, ubuntu
[   96.483964] Hello World!
[   97.031787] Good bye, ubuntu
[   97.594151] Hello World!
[   98.109896] Good bye, ubuntu
[   98.615569] Hello World!
[   99.098943] Good bye, ubuntu
[ 1893.976170] Hello World!
以上的信息显示：笔者在95.586960做了rmmod hello.ko，在96.483964做了insmod hello.ko……
这里[]中的数字表示执行模块安装/卸载聚开机的时间，如96.483964 S（约开机一个半小时以后）。
【9】那么这些信息都存在哪儿呢？
都说默认是在/val/log/messages目录下；可是有时候你看了，没有；
那么请看看另一个目录：
# cd /etc
# ls
你会找到包含syslog的文件，如：
syslog.conf
那么就是它了。
# cat syslog.conf | head -20
同理，只显示前20行的东东；看到类似以下的东东：
# /etc/syslog.conf Configuration file for syslogd.
#
#    For more information see syslog.conf(5)
#    manpage.
#
# First some standard logfiles. Log by facility.
#
auth,authpriv.*    /var/log/auth.log
*.*;auth,authpriv.none   -/var/log/syslog
#cron.*     /var/log/cron.log
daemon.*    -/var/log/daemon.log
kern.*     -/var/log/kern.log
lpr.*     -/var/log/lpr.log
mail.*     -/var/log/mail.log
user.*     -/var/log/user.log
#
# Logging for the mail system. Split it up so that
...
看到kern.*了吧~看到/var/log/kern.log了吧！我猜消息就在这底下。
于是，
# cat /var/log/kern.log | tail -10
显示：
Aug 4 23:40:53 ubuntu kernel: [   38.848634] eth0: no IPv6 routers present
Aug 4 23:41:47 ubuntu kernel: [   92.893834] Hello World!
Aug 4 23:41:50 ubuntu kernel: [   95.586960] Good bye, ubuntu
Aug 4 23:41:51 ubuntu kernel: [   96.483964] Hello World!
Aug 4 23:41:51 ubuntu kernel: [   97.031787] Good bye, ubuntu
Aug 4 23:41:52 ubuntu kernel: [   97.594151] Hello World!
Aug 4 23:41:52 ubuntu kernel: [   98.109896] Good bye, ubuntu
Aug 4 23:41:53 ubuntu kernel: [   98.615569] Hello World!
Aug 4 23:41:53 ubuntu kernel: [   99.098943] Good bye, ubuntu
Aug 5 00:11:48 ubuntu kernel: [ 1893.976170] Hello World!
看到了吧！跟先前的dmesg显示的内容相似而更完整了！
【好的，我说完了！】
