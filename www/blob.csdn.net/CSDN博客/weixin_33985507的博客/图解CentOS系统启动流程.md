# 图解CentOS系统启动流程 - weixin_33985507的博客 - CSDN博客
2017年03月01日 10:06:02[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
![0BO5CjeoruC](http://si1.go2yd.com/get-image/0BO5CjeoruC)
作者 |Linux运维之路    来源 | 51CTO博客
当我们按下开机键后，系统背后的秘密我们是否了解呢？这里，我带大家探索一下linux系统开机背后的秘密。
![0BO5CiJ4FJQ](http://si1.go2yd.com/get-image/0BO5CiJ4FJQ)
1.加电自检
主板在接通电源后，系统首先由POST程序来对内部各个设备进行检查，自检中如发现有错误，将按两种情况处理：对于严重故障(致命性故障)则停机，此时由于各种初始化操作还没完成，不能给出任何提示或信号；对于非严重故障则给出提示或声音报警信号，等待用户处理。
2.BIOS启动引导阶段
自检通过之后，按次序查找各引导设备，第一个有引导程序的设备即为本次启动要用到的设备。一般电脑是按"F2"或"Del"键等等进入BIOS，在里面我们可以调整硬盘的启动顺序。
找到第一个有引导程序的设备后，BIOS会找到设备中的MBR，读取其中的Bootloader。
3.GRUP引导阶段
这时候就可以看到我们的Centos系统了，但是一般都不需要操作，会自动启动系统。其实我们也可以自己手动启动系统的。
3.1 手动在grub命令行接口启动系统：
        grub> root (hd#,#)
        grub> kernel /vmlinuz-VERSION-RELEASE ro root=/dev/DEVICE 
        grub> initrd /initramfs-VERSION-RELEASE.img
grub> boot
3.2 grub的命令行接口：
          e: 编辑模式，用于编辑菜单；
      c: 命令模式，交互式接口；
help: 获取帮助列表
help KEYWORD: 详细帮助信息
find (hd#,#)/PATH/TO/SOMEFILE：
root (hd#,#) 设定grup的根设备
kernel /PATH/TO/KERNEL_FILE: 设定本次启动时用到的内核文件；额外还可以添加许多内核支持使用的cmdline参数；
例如：init=/path/to/init, selinux=0
initrd /PATH/TO/INITRAMFS_FILE: 设定为选定的内核提供额外文件的ramdisk；
boot: 引导启动选定的内核；
3.3 配置文件：/boot/grub/grub.conf
配置项：
default=#: 设定默认启动的菜单项；落单项(title)编号从0开始；
timeout=#：指定菜单项等待选项选择的时长；
splashimage=(hd#,#)/PATH/TO/XPM_PIC_FILE：指明菜单背景图片文件路径；
hiddenmenu：隐藏菜单；
password [--md5] STRING: 菜单编辑认证；
title TITLE：定义菜单项“标题”, 可出现多次；用来引导多个内核或者操作系统
    root (hd#,#)：grub查找stage2及kernel文件所在设备分区；为grub的“根”; 
kernel /PATH/TO/VMLINUZ_FILE [PARAMETERS]：启动的内核
initrd /PATH/TO/INITRAMFS_FILE: 内核匹配的ramfs文件；
password [--md5] STRING: 启动选定的内核或操作系统时进行认证
4.加载内核阶段
内核会自身初始化：
探测可识别到的所有硬件设备；
加载硬件驱动程序；（有可能会借助于ramdisk加载驱动）
以只读方式挂载根文件系统；
运行用户空间的第一个应用程序：/sbin/init
5.init初始化阶段
内核加载完毕，会运行init程序，init进程起来后，系统启动的控制权移交给init进程
/sbin/init进程是所有进程的父进程，当init起来之后，它首先会读取配置文件/etc/inittab，进行以下工作：
1.执行系统初始化脚本(/etc/rc.d/rc.sysinit)，对系统进行基本的配置，以读写方式挂载根文件系统及其它文件系统，到此系统基本算运行起来了，后面需要进行运行级别的确定及相应服务的启动；
2.确定启动后进入的运行级别；
3.执行/etc/rc.d/rc，该文件定义了服务启动的顺序是先K后S，而具体的每个运行级别的服务状态是放在/etc/rc.d/rcn.d（n=0~6）目录下，所有的文件均链接至/etc/init.d下的相应文件。
4.有关key sequence的设置5.有关UPS的脚本定义6.启动虚拟终端/sbin/mingetty 7.在运行级别5上运行X
配置文件：/etc/inittab 
    每行定义一种action以及与之对应的process
    id:runlevels:action:process 
        id：一个任务的标识符；
runlevels：在哪些级别启动此任务；#，###，可以为空，表示所有级别；
action：在什么条件下启动此任务；
process：任务；（脚本或程序）
   action：
wait：等待切换至此任务所在的级别时执行一次；
respawn：一旦此任务终止，就自动重新启动之；
initdefault：设定默认运行级别；此时，process省略；
sysinit：设定系统初始化方式，此处一般为指定/etc/rc.d/rc.sysinit脚本；
    K*：要停止的服务；K##*，优先级，数字越小，越是优先关闭；依赖的服务先关闭，而后关闭被依赖的；
S*：要启动的服务；S##*，优先级，数字越小，越是优先启动；被依赖的服务先启动，而依赖的服务后启动；
chkconfig命令：管控/etc/init.d/每个服务脚本在各级别下的启动或关闭状态；
    查看：chkconfig  --list   [name]
    添加：chkconfig  --add  name
删除：chkconfig  --del  name
修改指定的链接类型：
chkconfig  [--level  LEVELS]  name  <on|off|reset>
--level LEVELS：指定要控制的级别；默认为2345；
init命令：
    级别切换：init #
    级别查看：who -r
注：
正常级别下，最后启动的一个服务S99local没有链接至/etc/init.d下的某脚本，而是链接至了/etc/rc.d/rc.local （/etc/rc.local）脚本；因此，不便或不需写为服务脚本的程序期望能开机自动运行时，直接放置于此脚本文件中即可。
文章来源：马哥教育 
官方微信：马哥linux运维 
技术交流群：485374463
