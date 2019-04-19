# Davinci及U-boot的一些介绍 - maopig的专栏 - CSDN博客
2011年12月01日 11:36:21[maopig](https://me.csdn.net/maopig)阅读数：2338标签：[flash																[video																[嵌入式操作系统																[平台																[output																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式操作系统&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)
个人分类：[u-boot/驱动开发/linux](https://blog.csdn.net/maopig/article/category/930963)
TI推出的数字多媒体平台DM系列，集成了ARM与DSP双核处理器：DSP处理器运行DSP/BIOS操作系统，负责音视频编解码算法以及其他图形处理算法；ARM处理器运行MontaVista
Linux操作系统，负责设备初始化、用户图形界面管理。ARM处理器端的MontaVista 
Linux 加载启动，需要有Bootloader完成内核由Flash空间加载到DDR空间并启动Linux系统。Bootloader就是在OS内核运行之前运行的一段程序，通过这段引导程序，初始化硬件设备、建立内存空间映射图，使系统的软硬件环境处于一个合适的状态，为OS内核的启动做好准备。目前TI的 DM6446平台采用U-Boot作为Bootloader引导程序。
一、U-Boot简介
U-Boot，全称Universal Boot Loader,是遵循GPL(General Public 
License) 条款的开放源码项目。U-Boot不仅仅支持嵌入式Linux系统的引导，还支持 NetBSD,VxWorks,QNX,RTEMS,artos,LynxOS嵌入式操作系统。U-Boot支持 PowerPC,MIPS,X86,ARM,NIOS,XScale等诸多常用系统的处理器。U-Boot项目的开发目标，即是支持尽可能多的嵌入式处理器和嵌入式操作系统。
U-Boot的特点：
> 开放源码;
> 支持多种嵌入式操作系统内核，如：式LinuxNetBSD,VxWorks,QNX,RTEMS,artos,LynxOS;
> 支持多个处理器系列，如PowerPC,MIPS,X86,ARM,NIOS,XScale;
> 较高的可靠性和稳定性;
> 高度灵活的功能设置，适合U-Boot调试、操作系统不同引导要求等;
> 丰富的设备驱动源码，如串口、以太网、SDRAM、FLASH、EEPROM、RTC、键盘等;
> 较为丰富的开发调试文档与强大的网络技术支持。
二、U-Boot目录结构
> board: 目标板相关文件，主要包含SDRAM,FLASH驱动;
> common:独立于处理器体系结构的通用代码，如内存大小探测与故障检测;
> cpu: 
> 与处理器相关的文件。如mpc8xx子目录下含串口、网口、LCD驱动及中断初始化等文件;
> driver: 通用设备驱动，如CFI FLASH 驱动(目前对INTEL FLASH支持较好);
> doc:U-Boot的说明文档;
> examples:U-Boot下运行的实例程序，如hello_world.c,timer.c;
> include: 
> U-Boot头文件，configs子目录下与目标板相关的配置头文件是移植过程中经常要修改的文件;
> lib_xxx: 
> 处理器体系相关的文件，如lib_ppc,lib_arm目录分别包含与PowerPC,ARM体系结构相关的文件;
> net: 网络功能相关的文件目录，如bootp,nfs,tftp;
> post: 上电自检文件目录，尚待进一步完善;
> rtc: RTC驱动程序;
> tools: 用于创建U-Boot S-RECORD和BIN镜像文件的工具;
U-Boot目前已经支持TI的DM6446平台，在board/davinci目录下为devem目标板相关文件，其中包含了网络、flash等的驱动程序。
Davinci U-Boot系列之二：U-Boot在SEED-DVS6446平台上的应用
SEED-DVS6446平台采用U-Boot-1.2.0版本，针对NAND 
Flash、Net、DDR驱动修改，并提供更多的U-Boot命令支持。下面以SEED-DVS6446平台为例，说明U-Boot在该平台上的常用配置使用。
一、U-Boot启动
1.软件配置
打开串口超级终端(PC超级终端、Hyperterminal、Teraterm),配置相关参数如下：
> 波特率：115200
> 数据位：8
> 奇偶较验：None
> 停止位：1
> 数据流控制：None
2.硬件配置
> DB9串口线一端连接DAVINCI平台串口，另一端连接PC机串口；
> DAVINCI平台的视频采集口连接摄像头或者DVD作为视频输入；
> DAVINCI平台的视频输出口连接显示设备；
> DAVINCI平台的网络接口通过网线连接至路由器等网络接口，或者通过直通线与
PC机网口连接；
> DAVINCI平台采用5V-5A电源供电，启动系统。
此时可以在串口超级终端看到DVS6446平台的一些相关信息，比如ARM,DDR时钟频率，U-Boot版本等等。
二、U-Boot通过TFTP下载内核启动并挂载网络文件系统
TFTP下载内核启动挂载网络文件系统方式，即通过TFTP服务器下载内核启动，文件系统挂载到Linux服务器下的已经搭建好的/opt/nfs文件系统目录下。过程如下：
1.SEED-DVS6446上电启动，显示上述启动信息，当显示"hit any key to stop 
autoboot: 
3"时按下回车键，中断系统自动启动，进行启动参数配置，此时显示如下提示符：SEED-DVS6446_v1.2#: 
;
2.配置启动参数，在提示符下输入以下配置参数：setenv bootargs mem=120M 
console=ttyS0,115200n8 root=/dev/nfs noinitrd rw ip=dhcp nfsroot=nfs 
ip=/opt/nfs video=dm64xxfbutput=pal
3.设置tftp服务器的IP地址和DVS6446的IP地址，在提示符下分别输入如下配置(tftp服务器的IP地址和DVS6446的IP地址，根据实际应用配置)：
setenv serverip _._._._
setenv ipaddr _._._._
4.下载内核并启动(默认用户已经将内核文件uImage复制到tftp服务的根目录下),在提示符下配置下载启动：
tftp 0x80800000 uImage;bootm 0x80800000
5.配置完成系统启动，并自动挂载网络文件系统，启动完毕显示等待登录信息。
三、通过U-Boot配置参数实现VGA视频输出与复合视频输出的配置
SEED-DVS6446支持复合视频与VGA视频输出显示，默认采用复合视频输出，用户可以通过U-Boot中的参数配置实现VGA输出。以TFTP下载内核启动挂载网络文件系统方式为例，详细步骤如下：
1.SEED-DVS6446 U-Boot启动，显示启动信息，当显示"hit any key to stop 
autoboot: 
3"时按下回车键，中断系统自动启动，进行启动参数配置，此时显示如下提示符：SEED-DVS6446_v1.2#: 
;
2.配置启动参数，在提示符下输入以下配置参数：setenv bootargs mem=120M 
console=ttyS0,115200n8 root=/dev/nfs noinitrd rw ip=dhcp nfsroot=nfs 
ip=/opt/nfs video=dm64xxfb format=vgautput=pal
3.设置tftp服务器的IP地址和DVS6446的IP地址，在提示符下分别输入如下配置：
setenv serverip _._._._
setenv ipaddr _._._._
4.下载内核并启动，在提示符下配置下载启动：
tftp 0x80800000 uImage;bootm 0x80800000
5.配置完成系统启动，并自动挂载网络文件系统，将VGA接口同SEED-DVS6446平台的VGA输出口连接，启动完毕即显示等待登录信息，VGA显示器左上角显示Monta
Vista Linux的图标。
四、从CCS直接烧录到DDR情况下，不需要U-Boot，此时cmd的通用脚本文件如下：
-lrts32e.lib
-l..\..\csl\arm\lib\Debug\csl_davinciArm9.lib
-l..\..\lib\davincievmbsl.lib
-stack 0x00000800
-heap 0x00000800
MEMORY
{
IRAM: o = 0x00000000 l = 0x00004000
DRAM: o = 0x00008000 l = 0x00004000
AEMIF: o = 0x02000000 l = 0x02000000
DDR2: o = 0x80000000 l = 0x08000000
}
SECTIONS
{
.bss > DDR2
.cinit > DDR2
.cio > DDR2
.const > DDR2
.stack > DDR2
.sysmem > DDR2
.text > DDR2
.ddr2 > DDR2
}
Davinci U-Boot系列之三：U-Boot常用命令和常用环境变量
U-Boot常用命令
?：
得到所有命令列表或者列出某个命令的帮助。
用法：? [command ...]
说明：列出命令的帮助信息，当不带参数时，列出所有命令及简要说明。
help：
同?
printenv：
打印环境变量。
用法：printenv [name]
说明：name一般是Hiboot环境变量的名字，也可以是用户自定义的变量；当value为空时，删除变量 
"name", 否则设置变量"name",且值为"value".
saveenv：
保存变量。
用法：saveenv
说明：保存变量及其值到flash.
ping：
用于简单判断目标机网络状态或本机网络工作状态。
tftp：
从tftp服务器中下载文件至RAM或者Flash中。
用法：tftp addr file
说明：将file文件下载到地址为addr的RAM或者Flash中。注意：使用tftp时，必须先设置好网络配置，使用setenv配置ipaddr、netmask、serverip参数。
cp：
拷贝内存。
用法：cp[.b,.w,.l]source target count.
说明：把地址为source的内存区域的值，拷贝到地址target的内存区域，区域的大小为count，source和target可以是SDRAM的地址访问，也可以是flash的地址范围，实际拷贝的大小，因命令的不同而不同。cp.b拷贝1xcount
bytes,简单使用cp时，默认为4xcount bytes.
go：
跳转到指定地址，执行代码。
用法：go addr [arg ...]
说明：执行地址addr处的二进制代码，可传递arg参数。
reset：
复位cpy处理器
用法：reset
bootm：
设置运行环境，并开始执行二进制代码。
用法：bootm [addr[arg ...]]
说明：执行addr处的代码，要求二进制代码为mkimage处理过的二进制文件。
erase：
擦除Flash内容.
用法1：erase start end.
说明：擦除地址从start到地址为end区域的内容。
注意：flash的擦除操作必须以块为最小单位，因此地址start必须为某块的其实地址，end地址则为某块的结束地址，如flash的基地址为0x34000000,块大小为0x20000,则操作erase
0x34000000 0x3401FFFF为可操作的，而erase 0x34000003 0x3401FFFF或者erase 
0x34000000 0x3401FF00均不可操作。
用法2：erase N:SF[-SL].
说明：擦除第N块flash的从扇区SF到SL扇区的内容。
用法3：erase bank N.
擦除第N块flash的内容。
用法4：erase all
说明：擦除所有flash的内容。
nand erase：
擦除nand flash内容
用法：nand erase start size
nand write：
用来将内存数据写入nand flash
用法：nand write sram_addr start size
说明：将内存sram_addr处的数据写入nand flash 
start地址开始的空间，写入大小为size.
U-Boot常用环境变量
ipaddr:
设置DVS6446平台的IP地址
说明：setenv ipaddr 192.168.253.53
serverip:
设置服务器ip地址，多用于tftp中
说明：setenv serverip 192.168.253.33
bootargs:
启动OS的启动参数
说明：setenv bootargs mem=120M console=ttys0,115200n8 ...
解释：设置传递参数，如内存大小，控制台，根文件系统等。
bootcmd:
设置自动及执行命令。启动延时根据bootdelay的值，若没有设置bootdelay,默认延时3秒
说明：setenv bootcmd bootm 
0x23450000,即设置启动后自动执行0x23450000处的代码。
bootdelay:
设置自启动延时时间，单位为秒。只有当bootcmd变量被设置后，该变量才有效。改变了值范围为大于等于-1的整数，当设置为-1时，关闭自启动的功能。
说明：setenv bootdelay 4 ; 设置自启动延时4秒
说明：setenv bootdelay -1; 关闭自启动功能
提示：在延时时间内可按任意键切换到命令行模式。注意，在产品开发调试阶段请勿设置延时时间为0，若设置，可以在启动瞬间使用CTRL+C中断程序而进入命令行模式。
netmask:
设置子网掩码。
说明：setenv netmask 255.255.255.0
gatewayip:
设置网关。
说明：setenv gatewayip 192.168.253.1
ethaddr:
设置以太网网卡MAC地址。
说明：setenv ethaddr xx: xx: xx: xx: xx: xx
注意：设置ethaddr后，应运行saveenv,重启才能使MAC地址配置有效。
声明：以上文章来自[http://www.tichinese.com/bbs/viewthread.php?tid=151&extra=page%3D9](http://blog.chinaunix.net/link.php?url=http://www.tichinese.com%2Fbbs%2Fviewthread.php%3Ftid%3D151%26amp%3Bextra%3Dpage%253D9)，此处只作为学习之用
附：
uboot常用命令
显示环境变量： printenv
保存环境变量： saveenv
设置内核文件名： setenv bootfile 
uImage-dm355，其中uImage-dm355为对应的文件名
设置内核启动方式：
1，从网络启动：setenv bootcmd 'tftp;bootm'
2，从FLASH启动
对于DM355: setenv bootcmd 'nboot 0x80700000 0 
0x400000;bootm'
对于DM6446: setenv bootcmd 'bootm 0x2040000'
设置bootargs：
DM355:
文件系统：NFS，文件系统的路径：nfsroot=192.168.1.55:/home/filesys；IP:DHCP；输入：模拟输入；输出NTSC
setenv bootargs console=ttyS0,115200n8 noinitrd rw ip=dhcp 
root=/dev/nfs
nfsroot=192.168.1.55:/home/filesys,nolock mem=100M 
v4l2_video_capture=:device=TVP5146
video=dm64xxfb:output=ntsc
文件系统：NFS，文件系统的路径：nfsroot=192.168.1.55:/home/filesys；IP:DHCP；输入：数字CMOS输入；输出PAL
setenv bootargs console=ttyS0,115200n8 noinitrd rw ip=dhcp 
root=/dev/nfs
nfsroot=192.168.1.55:/home/filesys,nolock mem=100M 
v4l2_video_capture=:device=MT9T001
video=dm64xxfb:output=pal
文件系统：本地FLASH；IP:固定IP；输入：数字CMOS输入；输出PAL
setenv bootargs console=ttyS0,115200n8 noinitrd rw 
rootfstype=yaffs2
ip=192.168.1.11:192.168.1.55:192.168.1.1:255.255.255.0:192.168.1.11::off 
root=/dev/mtdblock3 nolock
mem=90M v4l2_video_capture=:device=MT9T001 
video=dm64xxfb:output=pal
DM6446:
文件系统：NFS，文件系统的路径：nfsroot=192.168.1.55:/home/filesys；IP:DHCP；输入：模拟输入；输出PAL
setenv bootargs console=ttyS0,115200n8 noinitrd rw ip=dhcp 
root=/dev/nfs
nfsroot=192.168.1.55:/home/filesys,nolock mem=38M 
v4l2_video_capture=:device=TVP5146
video=dm64xxfb:output=pal
文件系统：本地硬盘；IP:DHCP；输入：模拟输入；输出PAL
setenv bootargs console=ttyS0,115200n8 noinitrd rw ip=dhcp 
root=/dev/hda1
nfsroot=192.168.1.55:/home/filesys,nolock mem=38M 
v4l2_video_capture=:device=TVP5146
video=dm64xxfb:output=pal
文件系统：NFS，文件系统的路径：nfsroot=192.168.1.55:/home/filesys；IP:固定IP；输入：模拟输入；输出NTSC
setenv bootargs console=ttyS0,115200n8 noinitrd rw 
ip=192.168.1.240::192.168.1.2 root=/dev/nfs
nfsroot=192.168.1.55:/home/filesys, nolock mem=38M 
video=dm64xxfb:output=ntsc v4l2_video_capture=:device=TVP5146
文件系统：本地硬盘；IP:固定IP；输入：模拟输入；输出PAL
setenv bootargs console=ttyS0,115200n8 noinitrd rw 
ip=192.168.1.240::192.168.1.2 root=/dev/hda1
nfsroot=192.168.1.55:/home/filesys,nolock mem=38M 
v4l2_video_capture=:device=TVP5146
video=dm64xxfb:output=pal
烧录内核至FLASH
DM355:
tftp 80700000 uImage-dm355:其中uImage-dm355为对应的文件名
nand erase 400000 200000
nand write 0x80700000 0x400000 0x200000
setenv bootcmd 'nboot 0x80700000 0 0x400000;bootm'
DM6446
tftp
erase 0x2040000 
+0x157fb0(其中0x157fb0为对应内核文件的大小，通过tftp命令可以统计出来)
setenv bootcmd 'bootm 0x2040000'
cp.b 0x80700000 0x2040000 
0x157fb0(其中0x157fb0为对应内核文件的大小，通过tftp命令可以统计出来)
LINUX 常用命令
路径切换命令：
显示当前所在目录路径
pwd
进入/root/encode目录
cd /root/encode
如果目前所在目录是/root
则输入：
cd encode即可进入/root/encode目录
驱动挂载命令：
查看系统上挂载的移动设备
fdisk -l
将USB设备（/dev/sda1）mount到mnt/usb目录下
mount /dev/sda1 /mnt/usb
拷贝命令：
从目录A(/root/encode)拷贝一文件到目录B(mnt/usb)
cp /root/encode/test.mpeg4 mnt/usb
进程终止命令
显示系统进程：ps -a
终止某进程： kill 996(996为目前某进程的PID号）
应用程序执行命令
执行应用程序encode
./encode
后台执行应用程序encode
./encode&(在DM355脱机方式，建议通过此方式执行应用程序，方可以通过kill命令进行进程的终止)
打包命令
tar -zcf A.tar.gz B（其中B为欲打包的目录，A为压缩包的命名）
解包命令
tar -xzf ti-davinci.tar.gz（其中 
ti-davinci.tar.gz欲解包的命名，解压在当前目录
