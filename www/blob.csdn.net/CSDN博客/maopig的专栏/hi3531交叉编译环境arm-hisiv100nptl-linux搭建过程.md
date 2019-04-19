# hi3531交叉编译环境arm-hisiv100nptl-linux搭建过程 - maopig的专栏 - CSDN博客
2017年03月23日 19:35:26[maopig](https://me.csdn.net/maopig)阅读数：2760
http://blog.csdn.net/hemmingway/article/details/38870677
- 
安装SDK
1、Hi3531 SDK包位置
在"Hi3531_V100R001***/01.software/board"目录下，您可以看到一个 Hi3531_SDK_Vx.x.x.x.tgz 的文件，
该文件就是Hi3531的软件开发包。
2、解压缩SDK包
在linux服务器上（或者一台装有linux的PC上，主流的linux发行版本均可以），使用命令：tar -zxf Hi3531_SDK_Vx.x.x.x.tgz ，
解压缩该文件，可以得到一个Hi3531_SDK_Vx.x.x.x目录。
3、展开SDK包内容
返回Hi3531_SDK_Vx.x.x.x目录，运行./sdk.unpack(请用root或sudo权限执行)将会展开SDK包打包压缩存放的内容，请按照提示完成操
作。
如果您需要通过WINDOWS操作系统中转拷贝SDK包，请先运行./sdk.cleanup，收起SDK包的内容，拷贝到新的目录后再展开。
4、在linux服务器上安装交叉编译器
1）安装uclibc交叉编译器（注意，需要有sudo权限或者root权限）：
进入Hi3531_SDK_Vx.x.x.x/osdrv/toolchain/arm-hisiv100-linux目录，运行chmod +x cross.install，然后运行./cross.install
即可。
2) 安装glibc交叉编译器（注意，需要有sudo权限或者root权限）：
进入Hi3531_SDK_Vx.x.x.x/osdrv/toolchain/arm-hisiv200-linux目录，运行chmod +x cross.install，然后运行./cross.install
即可。
3) 执行source /etc/profile， 安装交叉编译器的脚本配置的环境变量就可以生效了，或者请重新登陆也可。
在终端上输入命令 arm-linux 再按Tab键，可以看到下图，说明环境变量设置成功了
ls@ubuntu:/slq/nfs/mpp$ arm-hisiv100nptl-linux-
arm-hisiv100nptl-linux-addr2line arm-hisiv100nptl-linux-gprof
arm-hisiv100nptl-linux-ar arm-hisiv100nptl-linux-ld
arm-hisiv100nptl-linux-as arm-hisiv100nptl-linux-nm
arm-hisiv100nptl-linux-c++ arm-hisiv100nptl-linux-objcopy
arm-hisiv100nptl-linux-c++filt arm-hisiv100nptl-linux-objdump
arm-hisiv100nptl-linux-cpp arm-hisiv100nptl-linux-ranlib
arm-hisiv100nptl-linux-g++ arm-hisiv100nptl-linux-readelf
arm-hisiv100nptl-linux-gcc arm-hisiv100nptl-linux-size
arm-hisiv100nptl-linux-gcc-4.4.1 arm-hisiv100nptl-linux-strings
arm-hisiv100nptl-linux-gccbug arm-hisiv100nptl-linux-strip
arm-hisiv100nptl-linux-gcov 
验证，编译一个hello.c文件
#include <stdio.h>
int main()
{
printf("hello world!\n");
}
使用命令：arm-hisiv100nptl-linux-gcc hello.c -o hello 看是否编译成功
root@ubuntu:/slq/test# ls
hello hello.c
可见成功生成了二进制文件。 
5、编译osdrv
1.osdrv使用说明
本目录设计思路为一套源代码支持两种工具链编译，因此需要通过编译参数指定不同的工具链。其中arm-hisiv100nptl-linux是uclibc工具链，arm-hisiv200-linux是glibc工具链。具体命令如下
(1)编译整个osdrv目录：
make OSDRV_CROSS=arm-hisiv100nptl-linux all
或者
make OSDRV_CROSS=arm-hisiv200-linux all
(2)清除整个osdrv目录的编译文件：
make OSDRV_CROSS=arm-hisiv100nptl-linux clean
或者
make OSDRV_CROSS=arm-hisiv200-linux clean
(3)彻底清除整个osdrv目录的编译文件，除清除编译文件外，还删除已编译好的镜像：
make OSDRV_CROSS=arm-hisiv100nptl-linux distclean
或者
make OSDRV_CROSS=arm-hisiv200-linux distclean
(4)单独编译kernel：
待进入内核源代码目录后，执行以下操作
cp arch/arm/configs/godnet_defconfig .config
make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- menuconfig
make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- uImage
或者
cp arch/arm/configs/godnet_defconfig .config
make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- menuconfig
make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- uImage
(5)单独编译uboot：
待进入boot源代码目录后，执行以下操作
make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- godnet_config
make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux-
或者
make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- godnet_config
make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux-
(6)制作文件系统镜像：
在osdrv/pub/中有已经编译好的文件系统，因此无需再重复编译文件系统，只需要根据单板上flash的规格型号制作文件系统镜像即可。
spi flash使用jffs2格式的镜像，制作jffs2镜像时，需要用到spi flash的块大小。这些信息会在uboot启动时会打印出来。建议使用时先直接运行mkfs.jffs2工具，根据打印信息填写相关参数。下面以块大小为64KB为例：
osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_uclibc -l -e 0x40000 -o osdrv/pub/rootfs_uclibc_256k.jffs2
osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_uclibc -l -e 0x10000 -o osdrv/pub/rootfs_uclibc_64k.jffs2
sudo osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_uclibc -l -e 0x10000 --pad=0x9A0000 -o osdrv/pub/rootfs_uclibc_64k.jffs2
sudo osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_uclibc_slq -l -e 0x10000 --pad=0x9A0000 -o osdrv/pub/rootfs_uclibc_64k_slq.jffs2
sudo osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_custom_slq -l -e 0x10000 --pad=0x100000 -o osdrv/pub/rootfs_custom_64k_slq.jffs2
或者
osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_glibc -l -e 0x40000 -o osdrv/pub/rootfs_glibc_256k.jffs2
nand flash使用yaffs2格式的镜像，制作yaffs2镜像时，需要用到nand flash的pagesize和ecc。这些信息会在uboot启动时会打印出来。建议使用时先直接运行mkyaffs2image工具，根据打印信息填写相关参数。下面以2KB pagesize、1bit ecc为例：
osdrv/pub/bin/pc/mkyaffs2image osdrv/pub/rootfs_uclibc osdrv/pub/rootfs_uclibc_2k_1bit.yaffs2 1 1
或者
osdrv/pub/bin/pc/mkyaffs2image osdrv/pub/rootfs_glibc osdrv/pub/rootfs_glibc_2k_1bit.yaffs2 1 1
(7)单独编译PCIE MCC驱动模块：
首先，先准备好编译模块时需要连接到的内核源代码目录：
cd ./kernel/
tar -xvf linux-3.0.y.tgz
解压内核源代码之后，需要根据具体的芯片类型，先编译内核(以Hi3531为例)，以生成相应的头文件
cd ./linux-3.0.y
cp arch/arm/configs/godnet_defconfig .config
make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- menuconfig
make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- uImage
内核源代码准备完毕，可以进入PCIE MCC驱动模块的源代码目录，编译驱动：
cd ../../drv/pcie_mcc
如果是编译主片的ko，直接使用make命令
make
如果是编译从片的ko文件，请带上参数target=slave
make target=slave
(说明：默认情况下编译驱动的工具链是arm-hisiv100nptl-linux-，如需更改请带上参数 CROSS_COMPILE=arm-hisiv200-linux-)
2. 镜像存放目录说明
编译完的image，rootfs等存放在osdrv/pub目录下
pub
│ rootfs_uclibc.tgz ------------------------------------------ hisiv100nptl编译出的rootfs文件系统
│ rootfs_glibc.tgz ------------------------------------------- hisiv200编译出的rootfs文件系统
│
├─image_glibc ------------------------------------------------ hisiv200编译出的镜像文件
│ uImage ------------------------------------------------- kernel镜像
│ u-boot-hi3531_930MHz.bin ------------------------------- u-boot镜像
│ rootfs_256k.jffs2 -------------------------------------- jffs2 rootfs镜像(对应spi-flash blocksize=256K)
│ rootfs_2k_1bit.yaffs2 ---------------------------------- yaffs2 rootfs镜像(对应nand-flash pagesize=2K ecc=1bit)
│
├─image_uclibc ----------------------------------------------- hisiv100nptl编译出的镜像文件
│ uImage ------------------------------------------------- kernel镜像
│ u-boot-hi3531_930MHz.bin ------------------------------- u-boot镜像
│ rootfs_256k.jffs2 -------------------------------------- jffs2 rootfs镜像(对应spi-flash blocksize=256K)
│ rootfs_2k_1bit.yaffs2 ---------------------------------- yaffs2 rootfs镜像(对应nand-flash pagesize=2K ecc=1bit)
│
└─bin
├─pc
│ mkfs.jffs2
│ mkimage
│ mkfs.cramfs
│ mkyaffs2image
│
├─board_glibc -------------------------------------------- hisiv200编译出的单板用工具以及pcie消息通讯层ko
│ flash_eraseall
│ mcc_usrdev_host.ko
│ flash_erase
│ mcc_usrdev_slv.ko
│ hi35xx_dev_slv.ko
│ nandwrite
│ hi35xx_dev_host.ko
│ mcc_drv_slv.ko
│ mtd_debug
│ flash_info
│ mcc_drv_host.ko
│ boot_device.ko
│ pcit_dma_slv.ko
│ sumtool
│ mtdinfo
│ flashcp
│ nandtest
│ nanddump
│ parted_glibc
│ pcit_dma_host.ko
│ gdb-arm-hisiv200-linux
│
└─board_uclibc ------------------------------------------- hisiv100nptl编译出的单板用工具以及pcie消息通讯层ko
flash_eraseall
mcc_usrdev_host.ko
flash_erase
mcc_usrdev_slv.ko
hi35xx_dev_slv.ko
nandwrite
hi35xx_dev_host.ko
mcc_drv_slv.ko
mtd_debug
flash_info
parted_uclibc
mcc_drv_host.ko
boot_device.ko
pcit_dma_slv.ko
sumtool
mtdinfo
flashcp
nandtest
gdb-arm-hisiv100nptl-linux
nanddump
pcit_dma_host.ko
3.osdrv目录结构说明：
osdrv
├─Makefile ------------------------------ osdrv目录编译脚本
├─busybox ------------------------------- 存放busybox源代码的目录
├─tools --------------------------------- 存放各种工具的目录
│ ├─board_tools ----------------------- 各种单板上使用工具
│ │ ├─reg-tools-1.0.0 --------------- 寄存器读写工具
│ │ ├─mtd-utils --------------------- flash裸读写工具
│ │ ├─udev-100 ---------------------- udev工具集
│ │ ├─gdb --------------------------- gdb工具
│ │ ├─parted ------------------------ 大容量硬盘分区工具
│ │ └─e2fsprogs --------------------- mkfs工具集
│ └─pc_tools -------------------------- 各种pc上使用工具
│ ├─mkfs.cramfs ------------------- cramfs文件系统制作工具
│ ├─mkfs.jffs2 -------------------- jffs2文件系统制作工具
│ ├─mkimage ----------------------- uImage制作工具
│ ├─mkyaffs2image301 -------------- yaffs2文件系统制作工具
│ ├─nand_production --------------- nand flash烧写文件制作工具
│ └─uboot_tools ------------------- uboot镜像制作工具、xls文件及ddr初始化脚本、bootrom工具
├─toolchain ----------------------------- 存放工具链的目录
│ ├─arm-hisiv100nptl-linux ---------------- hisiv100nptl交叉工具链
│ └─arm-hisiv200-linux ---------------- hisiv200交叉工具链
├─pub ----------------------------------- 存放各种镜像的目录
│ ├─image_glibc ----------------------- 基于hisiv100nptl工具链编译，可供FLASH烧写的映像文件，包括uboot、内核、文件系统
│ ├─image_uclibc ---------------------- 基于hisiv200工具链编译，可供FLASH烧写的映像文件，包括uboot、内核、文件系统
│ ├─bin ------------------------------- 各种未放入根文件系统的工具
│ │ ├─pc ---------------------------- 在pc上执行的工具
│ │ ├─board_glibc ------------------- 基于hisiv100nptl工具链编译，在单板上执行的工具
│ │ └─board_uclibc ------------------ 基于hisiv200工具链编译，在单板上执行的工具
│ ├─rootfs_uclibc.tgz ----------------- 基于hisiv100nptl工具链编译的根文件系统
│ └─rootfs_glibc.tgz ------------------ 基于hisiv200工具链编译的根文件系统
├─drv ----------------------------------- 存放外设驱动的目录
│ └─pcie_mcc -------------------------- pcie消息通讯层驱动
├─rootfs_scripts ------------------------ 存放根文件系统制作脚本的目录
├─uboot --------------------------------- 存放uboot源代码的目录
└─kernel -------------------------------- 存放kernel源代码的目录
4.注意事项
(1)使用某一工具链编译后，如果需要更换工具链，请先将原工具链编译文件清除，然后再更换工具链编译。
(2)在windows下复制源码包时，linux下的可执行文件可能变为非可执行文件，导致无法编译使用；u-boot或内核下编译后，会有很多符号链接文件，在windows下复制这些源码包, 会使源码包变的巨大，因为linux下的符号链接文件变为windows下实实在在的文件，因此源码包膨胀。因此使用时请注意不要在windows下复制源代码包。
(3)目前网络为自适应模式，如果要修改为固定某种速率模式，需要修改文件kernel/linux-3.0.y/drivers/net/stmmac/stmmac_mdio.c：
关掉宏定义TNK_HW_PLATFORM_ADJUST，注释掉33行；
选择需要固定配置的速率，例如固定配置为百兆全双工的话，打开46行宏定义PHY_SPEED_100即可。
(4)Hi3531支持硬浮点，文件系统中发布的库都是硬浮点库。因此请用户注意，所有Hi3531板端代码编译时需要在Makefile里面添加以下命令：
CFLAGS += -march=armv7-a -mcpu=cortex-a9 –mfloat-abi=softfp -mfpu=vfpv3-d16
CXXFlAGS +=-march=armv7-a -mcpu=cortex-a9 –mfloat-abi=softfp -mfpu=vfpv3-d16
其中CXXFlAGS中的XX根据用户Makefile中所使用宏的具体名称来确定，e.g:CPPFLAGS。
参考:
http://blog.csdn.net/mybelief321/article/details/9076583
### Ubuntu12.04嵌入式交叉编译环境arm-linu-gcc搭建过程，图解
分类： Qt编程2013-06-12 08:44 2462人阅读 评论(2) 收藏 举报ubuntu12.04arm-linux-gcc
**安装环境**
Linux版本：Ubuntu 12.04 内核版本：Linux 3.5.0 交叉编译器版本：arm-linux-gcc-4.4.3 这个版本的交叉编译器我已经上传到了资源上，可以随便下载，点此下载
**安装前的絮叨**
首先简单介绍一下，所谓的搭建交叉编译环境，即安装、配置交叉编译工具链。在该环境下编译出嵌入式Linux系统所需的操作系统、应用程序等，然后再上传到目标机上。
交叉编译工具链是为了编译、链接、处理和调试[跨平台](http://www.it165.net/pro/)体系结构的程序代码。对于交叉开发的工具链来说，在文件名称上加了一个前缀，用来区别本地的工具链。例如，arm-linux-表示是对arm的交叉编译工具链；arm-linux-gcc表示是使用gcc的编译器。除了体系结构相关的编译选项以外，其使用方法与Linux主机上的gcc相同，所以Linux编程技术对于嵌入式同样适用。不过，并不是任何一个版本拿来都能用，各种软件包往往存在版本匹配问题。例如，编译内核时需要使用arm-linux-gcc-4.3.3版本的交叉编译工具链，而使用arm-linux-gcc-3.4.1的交叉编译工具链，则会导致编译失败。
那么gcc和arm-linux-gcc的区别是什么呢？区别就是gcc是linux下的C语言编译器，编译出来的程序在本地执行，而arm-linux-gcc用来在linux下[跨平台](http://www.it165.net/pro/)的C语言编译器，编译出来的程序在目标机(如ARM平台)上执行，嵌入式开发应使用嵌入式交叉编译工具链。
**安装步骤**
1、将压缩包arm-linux-gcc-4.4.3.tar.gz存放在一个目录下，这个目录就是你等会解压缩的目录，以后这个目录就不能随便删掉了，我的存放路径是 /home/song/software,如下图，记住这个路径，等会还会用到。
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202315349.png)
2、使用tar命令：tar zxvf arm-gcc-4.4.3.tar.gz将software文件夹下的arm-linux-gcc-4.4.3.tar.gz解压缩安装到当前目录下如下图
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202320350.png)
通过下图可以看到解压成功了，并且解压后的文件存放在了/home/song/software/opt/FriendlyARM/toolschain/4.4.3文件夹下，如下图所示，这个存放路径可得记住，如下图
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202325352.png)
3、接下来配置系统环境变量，把交叉编译工具链的路径添加到环境变量PATH中去，这样就可以在任何目录下使用这些工具。记下上一步中的安装路径，使用命令：vim /etc/profile 编辑profile文件，添加环境变量。
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202330354.png)
在profile中最后一行添加： export PATH=$PATH:/home/song/software/opt/FriendlyARM/toolschain/4.4.3/bin 这个路径就是那个bin目录所在的路径，可能你的不一样，按照你实际的目录填就可以了，如下图32行
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202335362.png)
编写完保存就好了
4、使用命令：source /etc/profile 使环境变量生效
5、在终端上输入命令 arm-linux 再按Tab键，可以看到下图，说明环境变量设置成功了
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202336366.png)
6、使用命令：arm-linux-gcc -v 会出现下面的错误提示：/home/song/software/opt/FriendlyARM/toolschain/4.4.3/bin/arm-linux-gcc: 15: exec: /home/song/software/opt/FriendlyARM/toolschain/4.4.3/bin/.arm-none-linux-gnueabi-gcc: not found
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202336367.png)
说是出现这种问题的原因是由于Ubuntu12.04用的是64位的，解决方法就是使用命令：sudo apt-get install ia32-libs 装一些32位的库
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202336368.png)
7、待安装完32位的库之后，再使用命令：arm-linux-gcc -v，这一次就成功了，如下图
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202336369.png)
8、验证，编译一个hello.c文件
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202336370.png)
使用命令：arm-linux-gcc hello.c -o hello 看是否编译成功
![\](http://www.it165.net/uploadfile/files/2014/0618/20140618202336371.png)
可见成功生成了二进制文件。
