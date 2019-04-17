# 嵌入式系统烧写uboot/bootloader/kernel的一般方法 - DoubleLi - 博客园






嵌入式系统烧写uboot/bootloader/kernel的一般方法



本文介绍了在嵌入式系统中烧写uboot/bootloader/kernel 的一般方法，以及如果uboot或者内核出现错误，  www.2cto.com  

引导失败等情况时如何重新烧写uboot/kernel.



烧写uboot/kernel的方式一般有两种，一种是在linux本身（shell中），直接将uboot/kernel的影像文件（二进制文件）使用操作烧写flash的命令烧写到flash特定分区上，



另一种是在uboot界面上，通过配置serverip,ipaddr，使用tftp命令[下载](http://www.2cto.com/soft)uboot/kernel到flash上



例子如下：



//一，在linux的shell中烧写uboot/kernel



mtd write命令是自定义的烧写flash命令。



//烧写uboot ,/dev/mtd0 是bootloader 的特定分区

 mtd write u-boot.bin /dev/mtd0 



//烧写kernel， mtd1 是kernel的特定分区

mtd write vmlinux.elf /dev/mtd1



//重启

reboot 



//二，在uboot界面中烧写uboot/kernel，以kernel为例子



//在bootloader 界面中



//本机uboot 的ip地址

setenv ipaddr 10.10.167.10



//服务器ip地址

setenv serverip 10.10.167.249



//下载linux内核vmlinux52

tftpboot 0 vmlinux52



// 烧写到flash 上。

// 0x20000000 是vmlinux52的存贮位置。 

// 0x1dd00000 是[系统](http://www.2cto.com/os/)内核的起始位置（每个设备位置不同）， 

// +0x600000是文件长度

upgrade 0x20000000 0x1dd00000 +0x600000



// 重启linux



bootoctliux



*************************************** **



如果因为uboot或者内核有错误而导致启动失败，出现卡死状态等，那么需要判断问题是出在uboot阶段还是linux阶段，这可以通过观察启动过程中的debug信息来判断，



如果uboot出现问题是比较麻烦的，因为这需要有一个装有正确的uboot/bootloader的flash芯片作为引导，



将系统重新引导至uboot界面中，之后再重新烧写uboot到系统本身的flash中，进而烧写内核（如有必要）。 



一，重新烧写uboot



首先需要一个特定的PCLL（flash芯片），这个PCLL上固化了一个正确的bootloader， 通过这个bootloader可以成功进入bootloader界面， 在此条件下重新烧写uboot/bootloader。



使用此PLCC的步骤是：



在系统下电情况下， 使用跳线/拨码开关等将此系统的启动方式变从外部flash（即PLCC）引导，



之后将系统上电， 这时会从ＰＬＣＣ 启动。



启动成功后，进入uboot界面，  即命令行显示Dream 5220# #（类似命令行提示符）



此时将版卡的启动方式变为 从 nor flash 启动。  之后重新烧写uboot到此nor flash 上。  



再用 kermit  来操作。



Kermit是一个串口服务工具，类似与minicom ，功能比minicom 多。



//kermit/usb0.cfg  是kermit 的配置文件， 在其中可以配置 /dev/ttyUSB0 等路径

kermit -y kermit/usb0.cfg



-y kermit/usb0.cfg   指定配置文件。



该文件内容如下：

/*



set line /dev/ttyUSB0                                                                                                                

set speed 115200

set carrier-watch off 

set hardshake none

set flow-control none

robust

set file type bin 

set file name lit 

set rec pack 1000

set send pack 1000

set window 5



*/

将此文件保存在主目录下， 命名为：kermit/usb0.cfg（可自定义）



执行后进入kermit 界面， 按 c 



进入 串口界面（就是设备的界面）



切换回kermit 界面快捷键：Ctrl + \  , 之后再按 c  。



执行：

Dream 5220# # detect  



显示：

Checking for NOR flash...

Flash detect 33554432Mb



这说明flash状态良好。



//让串口进入接受状态。

Dream 5220# # loadb 



进入kermit 界面： ctrl + \  按 c



执行：

send  uboot.bin



就是将当前目录下的uboot.bin 文件发送给串口。



完成后，进入串口界面。



执行：

Dream 5220# # upgrade 0x20000000 0x1dc00000 +0xe0000



0x20000000 是 kermit 执行 send  后 将文件send 到的默认地址。

再将文件写到nor flash 的首地址：0x1dc00000  +0xe0000为写的长度。



执行：reset。让系统重启。



/*



至此，新的uboot烧写完成，reset后， 如果还是不成功的话，那么问题肯定出在linux的的启动阶段或者新的uboot还是有问题，这可以通过启动过程中的debug信息看出来



一，如果内核有问题，导致linux没有进入shell界面，要重新进入Uboot环境，就是在reset 之后，一直按 ctrl + c  ，这样在uboot启动完成后， 不会进入kernel的启动阶段，而是停留在uboot的命令行中，



在这个uboot 环境下，烧写新内核， 配置ipaddr,serverip等，同最开始的例子中操作是一样的。



//本机uboot 的ip地址

setenv ipaddr 10.10.167.10



//服务器ip地址

setenv serverip 10.10.167.249



//下载linux内核vmlinux52

// 在服务器的tftpboot目录下存在相应文件，搭建到tftp服务环境

tftpboot 0 vmlinux52



// 烧写到flash 上。

// 0x20000000 是vmlinux52的存贮位置。 

// 0x1dd00000 是系统内核的起始位置（每个设备位置不同）， 

// +0x600000是文件长度

upgrade 0x20000000 0x1dd00000 +0x600000



在reset 之前，一般还要设置一下环境变量。



比如



setenv linux_args mtdparts=octeon_nor0:1m(BOOT),7m(LINUX),21m(CONFIG),3m(MD)



setenv bootcmd 'bootoctlinux 0x1dd00000 coremask=0x3 mem=700'



//保存

saveenv



bootoctliux



二 ，如果新烧写的uboot本身有问题，那么修改uboot代码，编译通过后， 使用PLCC重复上述烧写过程。









