# Linux内核模块编译 - Orisun - 博客园







# [Linux内核模块编译](https://www.cnblogs.com/zhangchaoyang/articles/1933927.html)





Linux内核模块是一种可被动态加载和卸载的可执行程序。通过内核模块可以扩展内核功能，内核模块通常用于设备驱动、文件系统等。如果没有内核模块，需要向内核添加功能就需要自发代码、重新编译内核、安装新内核等步骤。

内核空间中不止一个程序试图访问驱动程序模块，导致一个内核块在没有被调用完毕之前又被其他程序调用，这称之炒代码的重入。因此在设计驱动模块的数据结构时要考虑数据结构在多线程环境下不被其他线程破坏，对于共享的数据应该采取加锁的方法保护。

应用程序使用虚拟内存，有一个巨大的地址空间。而内核模块可以使用的内在非常小，所以编码时要注意内存的分配和使用。

Printk是内核提供的打印函数，向终端输出打印信息。printf是glibc提供的函数，Linux内核函数是不能依赖于任何程序库的。

1.下载当前版本的Linux源代码

#uname -a

查看当前shell下运行的linux内核版本是多少

![](https://pic002.cnblogs.com/images/2011/103496/2011011215163224.png)

然后去网上[https://launchpad.net/ubuntu/lucid/](https://launchpad.net/ubuntu/lucid/)下一个2.6.32-26版本的内核源代码

或者更简单的方法：

#apt-get install linux-source

直接下载当前运行的linux内核版本的源代码，下载到了/lib/modules/2.6.32-26-generic中，但是我用这种方法得到的biuld目录下没有Makefile

如果当前运行的内核版本与你编译链接的头文件版本不一致，加载内核模块时会遇到insmod: error inserting 'ModuleHello.o'  :-l invalid module format这样的错误

注意我刚装上ubuntu10.04的时候，在/lib/modules/2.6.32-24-generic下没有build文件夹，果断“系统|系统管理|更新管理”更新系统，这时候就有了/lib/modules/2.6.32-27-generic/build/Makefile，重启看看GRUB界面上版本号是多少，再使用uname -a已经是27不是24了。



当然你也可以更换Linux内核版本，让你的linux内核版本去与你的linux源文件版本保持一致。至于如何更换Linux内核版本，请看我的另一篇博客[http://www.cnblogs.com/zhangchaoyang/articles/1915253.html](http://www.cnblogs.com/zhangchaoyang/articles/1915253.html)。



2.编写驱动程序的Makefile文件

在与驱动程序相同的目录下编写驱动程序的Makefile文件

ifneq ($(KERNELRELEASE),) 

 obj-m := ModuleHello.o 

else 

 KERNELDIR ?= /lib/modules/$(shell uname -r) /build

 PWD := $(shell pwd) 

default: 

 $(MAKE) -C $(KERNELDIR) M=$(PWD) modules 

endif

![](https://pic002.cnblogs.com/images/2011/103496/2011011215173958.png)


执行该Makefile文件时将跑到 KERNELDIR下调用make，即/lib/modules/$(shell uname -r) /build下必须有一个Makefiule文件。这里$(shell uname -r) 就等价于2.6.32-26-generic。



网上也有人把KERNELDIR设置为/usr/src/linux-headers-2.6.32-26-generic。

PWD指定要编译的驱动程序源文件所在的目录，$(shell pwd) 表示当前目录

最终要生成的目标是modules

执行$make



3.加载模块
insmod和modeprobe都可以加载一个内核模块。insmod内核模块的符号是否已经在内核中定义，modprobe会检查，并且还检查模块的依赖关系。
sudo insmod ./ModuleHello.ko
用dmesg命令可以看到模块已经成功加载
$dmesg
$dmesg | tail -n 2 tail -n 2表示只看最后2行
也可以用lsmod|grep ModuleHello查看刚才安装的模块，显示的就是.ko前面的名称
cat /proc/devices | grep Hello 查看内核分配的主设备号，这个设备名称是在程序中注册设备时使用的名称，跟上面的模块名称没有必然联系。

4.加载后还可以卸载内核模块
suod rmmod ModuleHello
$dmesg | tail -n 1












