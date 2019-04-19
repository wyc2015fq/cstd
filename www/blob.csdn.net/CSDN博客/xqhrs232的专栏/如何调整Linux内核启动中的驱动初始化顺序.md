# 如何调整Linux内核启动中的驱动初始化顺序 - xqhrs232的专栏 - CSDN博客
2010年12月24日 18:16:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：734标签：[linux内核																[module																[c																[alignment																[struct																[audio](https://so.csdn.net/so/search/s.do?q=audio&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=alignment&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://hi.baidu.com/serial_story/blog/item/d40d49c6907212129c163ded.html](http://hi.baidu.com/serial_story/blog/item/d40d49c6907212129c163ded.html)
**如何调整Linux内核启动中的驱动初始化顺序**
【问题】
此处我要实现的是将芯片的ID用于网卡MAC地址，网卡驱动是enc28j60_init。
但是，读取芯片ID的函数，在as352x_afe_init模块中，所以要先初始化as352x_afe_init。
此处，内核编译完之后，在生成的system.map中可以看到，
enc28j60_init在as352x_afe_init之前，所以，无法去读芯片ID。
所以我们的目标是，将as352x_afe_init驱动初始化放到enc28j60_init之前，
然后才能读取芯片ID，才能用于网卡初始化的时候的，将芯片ID设置成网卡MAC地址。
【解决过程】
【1】
最简单想到的，是内核里面的
arch/arm/mach-as352x/core.c
中，去改devices设备列表中的顺序。
enc28j60_init对应的是ssp_device，因为网卡初始化用到的是SPI驱动去进行和通讯的。
as352x_afe_init对应的是afe_device。
原先是：
static struct platform_device *devices[] =
{
&uart_device,
&nand_device,
&afe_device,
&audio_device,
&usb_device,
&as352xkbd_device,
&ssp_device,
};
把afe改到最前面：
static struct platform_device *devices[] =
{
&afe_device,
&uart_device,
&nand_device,
&audio_device,
&usb_device,
&as352xkbd_device,
&ssp_device,
};
但是，实际结果是，没有任何影响，连systemp.map生成的，那么模块初始化顺序，都没有任何变化。
也就说明，想要实现驱动加载顺序的改变，改core.c里面的设备列表顺序是没有用的。
【2】
在网上看到很多帖子，主要就是这几个：
怎么确定驱动加载顺序
[http://blog.chinaunix.net/u2/72751/showart_1074704.html](http://blog.chinaunix.net/u2/72751/showart_1074704.html)
Linux内核驱动程序初始化顺序的调整
[http://www.itjj.net/tech/OS/Linux/2006-12-21/74501.html](http://www.itjj.net/tech/OS/Linux/2006-12-21/74501.html)
内核启动时,设备及驱动初始化的实现 
[http://linux.chinaunix.net/bbs/archiver/tid-1109340.html](http://linux.chinaunix.net/bbs/archiver/tid-1109340.html)
其说明的也很清楚了，就是：
Linux内核为不同驱动的加载顺序对应不同的优先级，定义了一些宏：
include/linux/init.h
#define pure_initcall(fn)   __define_initcall("0",fn,1)
#define core_initcall(fn)   __define_initcall("1",fn,1)
#define core_initcall_sync(fn)   __define_initcall("1s",fn,1s)
#define postcore_initcall(fn)   __define_initcall("2",fn,2)
#define postcore_initcall_sync(fn) __define_initcall("2s",fn,2s)
#define arch_initcall(fn)   __define_initcall("3",fn,3)
#define arch_initcall_sync(fn)   __define_initcall("3s",fn,3s)
#define subsys_initcall(fn)   __define_initcall("4",fn,4)
#define subsys_initcall_sync(fn) __define_initcall("4s",fn,4s)
#define fs_initcall(fn)    __define_initcall("5",fn,5)
#define fs_initcall_sync(fn)   __define_initcall("5s",fn,5s)
#define rootfs_initcall(fn)   __define_initcall("rootfs",fn,rootfs)
#define device_initcall(fn)   __define_initcall("6",fn,6)
#define device_initcall_sync(fn) __define_initcall("6s",fn,6s)
#define late_initcall(fn)   __define_initcall("7",fn,7)
#define late_initcall_sync(fn)   __define_initcall("7s",fn,7s)
#define __initcall(fn) device_initcall(fn)
把自己的驱动的函数名用这些宏去定义之后，
就会对应不同的加载时候的优先级。
其中，我们写驱动中所用到的module_init对应的是
#define module_init(x) __initcall(x);
而
#define __initcall(fn) device_initcall(fn)
所以，驱动对应的加载的优先级为6
在上面的不同的优先级中，
数字越小，优先级越高。
同一等级的优先级的驱动，加载顺序是链接过程决定的，结果是不确定的，我们无法去手动设置谁先谁后。
不同等级的驱动加载的顺序是先优先级高，后优先级低，这是可以确定的。
所以，像我们之前在驱动中用：
module_init(i2c_dev_init);
module_init(as352x_afe_init);
module_init(as352x_afe_i2c_init);
module_init(enc28j60_init);
所以，大家都是同一个优先级去初始化，
最后这些驱动加载的顺序，可以查看在根目录下，
生成的system.map：
。。。
c00197d8 t __initcall_alignment_init5
。。。。。
c00197f4 t __initcall_default_rootfsrootfs
c00197f8 t __initcall_timer_init_sysfs6
c00197fc t __initcall_clock_dev_init6
。。。
c00198d8 t __initcall_loop_init6
c00198dc t __initcall_net_olddevs_init6
c00198e0 t __initcall_loopback_init6
c00198e4 t __initcall_enc28j60_init6
。。。
c0019900 t __initcall_as352x_spi_init6
c0019904 t __initcall_spidev_init6
。。。
c0019920 t __initcall_i2c_dev_init6
c0019924 t __initcall_as352x_afe_i2c_init6
c0019928 t __initcall_as352x_afe_init6
。。。
c0019970 t __initcall_random32_reseed7
c0019974 t __initcall_seqgen_init7
c0019978 t __initcall_rtc_hctosys7
c001997c T __con_initcall_start
c001997c t __initcall_con_init
c001997c T __initcall_end
。。。
此处就是由于
c0019920 t __initcall_i2c_dev_init6
c0019924 t __initcall_as352x_afe_i2c_init6
c0019928 t __initcall_as352x_afe_init6
在
c00198e4 t __initcall_enc28j60_init6
之前，所以我这里才要去改。。。
知道原理，能想到的，就是
要么把
as352x_afe_init
改到
enc28j60_init
之前一级，即优先级为5。
即在驱动中，调用：
fs_initcall(as352x_afe_init)；
要么把
enc28j60_init
改到
as352x_afe_init
之后，即优先级为7
即在驱动中，调用：
late_initcall(enc28j60_init)；
但是，此处麻烦就麻烦在，
如果把
as352x_afe_init
改到
enc28j60_init
之前一级，
发现后面网卡初始化enc28j60_init中，虽然读取芯片ID对了，
但是后面的IP-auto configure 有问题。
所以放弃。
如果把
enc28j60_init
改到
as352x_afe_init
之后，
但是，从system.map中看到的是，优先级为7的驱动中，明显有几个驱动，
也是和网卡初始化相关的，所以，这样改，尝试后，还是失败了。
所以，没法简单的通过调整现有的驱动的顺序，去实现顺序的调整。
最后，被逼无奈，想到了一个可以实现我们需求的办法，
那就是，单独定义一个优先级，把afe相关的初始化都放到那里面去，
这样，就可以保证，其他没什么相关的冲突了。
最后证实，这样是可以实现目的的。
具体添加一个新的优先级的步骤如下：
1.定义新的优先级
include/linux/init.h中：
#define pure_initcall(fn)   __define_initcall("0",fn,1)
#define core_initcall(fn)   __define_initcall("1",fn,1)
#define core_initcall_sync(fn)   __define_initcall("1s",fn,1s)
#define postcore_initcall(fn)   __define_initcall("2",fn,2)
#define postcore_initcall_sync(fn) __define_initcall("2s",fn,2s)
#define arch_initcall(fn)   __define_initcall("3",fn,3)
#define arch_initcall_sync(fn)   __define_initcall("3s",fn,3s)
#define subsys_initcall(fn)   __define_initcall("4",fn,4)
#define subsys_initcall_sync(fn) __define_initcall("4s",fn,4s)
#define fs_initcall(fn)    __define_initcall("5",fn,5)
#define fs_initcall_sync(fn)   __define_initcall("5s",fn,5s)
#define rootfs_initcall(fn)   __define_initcall("rootfs",fn,rootfs)
#if 1
#define prev_device_initcall(fn)   __define_initcall("6",fn,6)
#define prev_device_initcall_sync(fn) __define_initcall("6s",fn,6s)
#define device_initcall(fn)   __define_initcall("7",fn,7)
#define device_initcall_sync(fn) __define_initcall("7s",fn,7s)
#define late_initcall(fn)   __define_initcall("8",fn,8)
#define late_initcall_sync(fn)   __define_initcall("8s",fn,8s)
#else
#define device_initcall(fn)   __define_initcall("6",fn,6)
#define device_initcall_sync(fn) __define_initcall("6s",fn,6s)
#define late_initcall(fn)   __define_initcall("7",fn,7)
#define late_initcall_sync(fn)   __define_initcall("7s",fn,7s)
#endif
2.用对应新的宏，定义我们的驱动：
prev_device_initcall(i2c_dev_init);
prev_device_initcall(as352x_afe_i2c_init);
prev_device_initcall(as352x_afe_init);
做到这里，本以为可以了，但是编译后，在system.map中，发现之前优先级为7的那几个函数，
被放到system.map最后了，而不是预想的，
在优先级7之后，在
c001997c T __con_initcall_start
c001997c t __initcall_con_init
c001997c T __initcall_end
之前。
最后，发现时没有把对应的链接文件中的宏加进去：
3.
include/asm-generic/vmlinux.lds.h
#if 1
#define INITCALLS        /
*(.initcall0.init)       /
*(.initcall0s.init)       /
*(.initcall1.init)       /
*(.initcall1s.init)       /
*(.initcall2.init)       /
*(.initcall2s.init)       /
*(.initcall3.init)       /
*(.initcall3s.init)       /
*(.initcall4.init)       /
*(.initcall4s.init)       /
*(.initcall5.init)       /
*(.initcall5s.init)       /
*(.initcallrootfs.init)       /
*(.initcall6.init)       /
*(.initcall6s.init)       /
*(.initcall7.init)       /
*(.initcall7s.init)       /
*(.initcall8.init)       /
*(.initcall8s.init)
#else
#define INITCALLS        /
*(.initcall0.init)       /
*(.initcall0s.init)       /
*(.initcall1.init)       /
*(.initcall1s.init)       /
*(.initcall2.init)       /
*(.initcall2s.init)       /
*(.initcall3.init)       /
*(.initcall3s.init)       /
*(.initcall4.init)       /
*(.initcall4s.init)       /
*(.initcall5.init)       /
*(.initcall5s.init)       /
*(.initcallrootfs.init)       /
*(.initcall6.init)       /
*(.initcall6s.init)       /
*(.initcall7.init)       /
*(.initcall7s.init)
#endif
最后，再重新编译，就可以实现我们要的，
和afe相关的驱动初始化，都在网卡enc28j60_init之前了。
也就可以在网卡里面读芯片ID了。
当然，对应编译生成的system.map文件中，
对应的通过module_init定义的驱动，优先级也都变成7了。
而late_initcall对应优先级8了。
注：当前开发板arm的板子，所以，对应的load 脚本在：
linux-2.6.28.4/arch/arm/kernel/vmlinux.lds
看起来，应该是这个文件：
linux-2.6.28.4/arch/arm/kernel/vmlinux.lds.S
生成上面那个脚本的。
vmlinux.lds中的这一行：
__initcall_start = .;
   *(.initcallearly.init) __early_initcall_end = .; *(.initcall0.init) *(.initcall0s.init) *(.initcall1.init) *(.initcall1s.init) *(.initcall2.init) *(.initcall2s.init) *(.initcall3.init) *(.initcall3s.init) *(.initcall4.init) *(.initcall4s.init) *(.initcall5.init)
 *(.initcall5s.init) *(.initcallrootfs.init) *(.initcall6.init) *(.initcall6s.init) *(.initcall7.init) *(.initcall7s.init)
就是将之前那些对应的init类型的函数，展开，放到这对应的位置。
【3】
不过，最后的最后，竟然发现网卡还是工作不正常，结果第二天，无意间发现是网卡地址设置导致网卡工作不正常的。
也就是说，实际是直接将afe设置到原先的优先级5就可以的，而不用这么麻烦去改系统的东西的。。。
不过，至少这也是一种办法，虽然不是那么的好。。。

