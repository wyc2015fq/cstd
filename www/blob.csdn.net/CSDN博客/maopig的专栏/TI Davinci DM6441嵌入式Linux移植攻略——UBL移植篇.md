# TI Davinci DM6441嵌入式Linux移植攻略——UBL移植篇 - maopig的专栏 - CSDN博客
2012年12月15日 17:11:37[maopig](https://me.csdn.net/maopig)阅读数：997
目录[(?)](http://blog.csdn.net/dashon2011/article/details/7333305#)[[+]](http://blog.csdn.net/dashon2011/article/details/7333305#)
- [一DM6441的Boot过程简介](http://blog.csdn.net/dashon2011/article/details/7333305#t0)
- [二DM6441的UBL移植](http://blog.csdn.net/dashon2011/article/details/7333305#t1)- [CCS文件夹](http://blog.csdn.net/dashon2011/article/details/7333305#t2)
- [Common文件夹](http://blog.csdn.net/dashon2011/article/details/7333305#t3)
- [GNU文件夹](http://blog.csdn.net/dashon2011/article/details/7333305#t4)- [移植DDR2](http://blog.csdn.net/dashon2011/article/details/7333305#t5)
- [移植Nand Flash](http://blog.csdn.net/dashon2011/article/details/7333305#t6)
- [其它](http://blog.csdn.net/dashon2011/article/details/7333305#t7)
声明：本文参考网友zjb_integrated的文章《[TI Davinci DM6446开发攻略——UBL移植](http://zjbintsystem.blog.51cto.com/964211/279709)》和《[DAVINCI
 DM365-DM368开发攻略——U-BOOT-2010.12及UBL的移植](http://zjbintsystem.blog.51cto.com/964211/713240)》，内容有增删，特此声明！另有系列网文《[DM365的UBL源码分析](http://www.61ic.com/Article/DaVinci/TMS320DM3x/201112/39729.html)》（作者不详）对UBL源码的分析比较透彻，有兴趣的也可以看看。
# **一、DM6441的Boot过程简介**
在搭好 TI Davinci DM6441的开发环境后，今天起进入Linux的移植阶段。UBL的移植，相对于UBOOT移植、KERNEL移植、ROOTFS移植、设备驱动及DSP开发来说，还是比较简单的。先从DAVINCI的启动说起，了解UBL在DAVIN系统中的位置和作用。对于固件程序烧写在NAND FLASH 的Davinci dm644x嵌入式系统， 上电启动的过程如下：
![](http://hi.csdn.net/attachment/201203/8/0_1331194670uX6L.gif)
RBL（ARM ROM Boot Loader）在芯片出厂的时候就已经烧写到ROM里了，这不需要大家关心。上电后，RBL会自动从EMIFA EM_CS2 memory space (0x0200 0000)执行指令，这个地址就是NAND FLASH 或NOR FLASH的片选起始地址。当你的系统设置为NAND BOOT的时候，UBL（User Boot Loader）是必不可少的，否则RBL不能直接把UBOOT给BOOT起来，因为RBL只支持14K
 NAND FLASH 的 BOOT程序，而UBOOT编译出来后的bin文件一般都大于80K，特别是版本越高，UBOOT的代码越大，所以这时候就需要写一个UBL。
UBL 从NAND FLASH 读取UBOOT，然后把UBOOT COPY 到 DDR2(RAM)的相关地址上，然后把UBOOT 给BOOT 起来。根据TI DAVIN RBL的规定，不同型号的NAND FLASH，UBL保存的地址是不同的，512字节PAGE 的NAND（即SMALL PAGE），保存的地址是：0x00004000；2048字节PAGE的NAND （即LARGE PAGE）保存的地址是：0x20000。
至于如何通过仿真器烧写UBL或通过UART BOOT烧写UBL，在U-BOOT移植的文章再详细介绍。
PS：不要混淆UBL和U-Boot！
# 二、DM6441的UBL移植
其实UBL和U-Boot的联系非常密切，放在这里单独来写，是因为本人也是个菜鸟，很多东西也不太懂，可以边写边加深理解。
nand-boot和nor-boot其实在移植的层面来看，都差不多。根据目标板的实际情况，重点放在nand-boot。这个UBL的版本是V1.50(当然你可以用更高的V2.00，导师提供给我的也是2.00的，但我的开发环境里是1.50的)，在root/dvsdk_2_00_00_22/PSP_02_00_00_140/board_utilites/dm644x下（后面的U-Boot也在该目录），有个文件DM644x_FlashAndBootUtils_1_50.tar.gz，为了方便，把它复制到/home/usrname并解压出来:
host $ cp DM644x_FlashAndBootUtils_1_50.tar.gz /home/usrname
host $ tar zxvf DM644x_FlashAndBootUtils_1_50.tar.gz
host $ cd DM644x_FlashAndBootUtils_1_50
UBL就是在Common和DM644x下。
Common目录里有非常多东西，包括UBL的驱动源码、工具、脚本等等。我们主要关注arch，drivers，src，ubl，这几个文件夹打开看看就明白什么意思了。UBL的main（）函数在/Common/ubl/src/ubl.c里。
DM644x下有CCS、Common，GNU三个文件夹：
## CCS文件夹：
这里边的程序需要在TI CCS下编译，通过仿真器和JTAG在DM36X的板子上调试和烧写NAND FLASH或NOR FLASH，有烧写Writer的应用程序，用CCS打开工程文件，会连接到/dvsdk_2_00_00_22/PSP_02_00_00_140/board_utilites/Common/drivers里。暂且先不管。
## Common文件夹：
Common里有核心的文件device.c和device_nand.c。device.c是最重要的文件，这里初始化很多系统的东西，见DEVICE_init（）：
1、屏蔽所有中断；
2、清除中断标志；
3、DEVICE_PSCInit（），Power and Sleep Controller；
4、DEVICE_pinmuxControl（），主芯片管脚复用的设置，DM644X的管脚复用很多，很复杂，一共5个PINMUX寄存器需配置；
5、DEVICE_PLL1Init（），PPL1配置，使用不同的频率的DM644x，这些值都不同，不过TI已经提供参数参考，DM6441一般使用Uint32 PLL1_Mult = 19;  // DSP=513 MHz
6、DEVICE_PLL2Init（），PPL2的配置，使用同上，不同频率的值不同；使用不同型号的DDR，要设置不同的参数，即时序参数等，这是关键的地方。
7、DEVICE_DDR2Init（），DDR2的配置，市场上不同的DDR2内存芯片需要不同的参数配置，就在这个函数内。
8、DEVICE_EMIFInit（），这个针对NAND FLASH接口或NOR FLASH接口的访问时序配置；
9、DEVICE_UART0Init（），串口UART0的配置，这个就是我们调试DM644X串口的设置，我们使用UART0来调试LINUX，这里配置不好，后面的开发就无法调试了。
10、DEVICE_TIMER0Init（），定时器TIMER0的设置；
11、DEVICE_I2C0Init（）的设置；
## GNU文件夹：
这个就是在LINUX环境下编译UBL的环境，修改dvsdk_2_00_00_22/PSP_02_00_00_140/board_utilites/dm644x/GNU/ubl下的makefile，把与nor相关的命令全注释掉，只保留与nand相关的：
all:
$(MAKE) -C build TYPE=nand
#$(MAKE) -C build TYPE=nor
clean:
$(MAKE) -C build TYPE=nand clean
#$(MAKE) -C build TYPE=nor clean
%::
$(MAKE) -C build TYPE=nand $@
#$(MAKE) -C build TYPE=nor $@
然后是make clean和 make生成ubl_DM644x_nand.bin的文件。
另外，在其它目录还有一些重要的文件：
nand.c及nand.h (DM644x_FlashAndBootUtils_1_50/Common/drivers)
主要移植就是定义好UBOOT在NAND的存储地址，不同型号的NAND FLASH ，比如SMALL PAGE（512字节）和LARGE PAGE（2048字节）这些都要修改除非你的NAND的类型和TI EVM 兼容。
nandboot.c (DM644x_FlashAndBootUtils_1_50/Common/ubl/src)
主要任务就是如何把u-boot.bin或带有头的u-boot.img正确COPY到DDR里，这里最容易出问题，编译出来的U-BOOT文件一般带有Valid magic number（MAGIC_NUMBER_VALID），入口地址entrypaoit，这些信息不对都使UBOOT 运行不起来，建议看一下或COPY UBOOT的image.h。
好了，介绍到这里，我们已知道UBL的移植就是对device.c、device.h、device_nand.c、device_nand.h、nand.c、nand.h、nandboot.c及相应的makefile文件进行相应的修改。相信已可以根据自己目标板的实际情况进行相应移植了。下面正式开始对板子进行移植：
首先说下板子的配置：
CPU:DM6441
DDR2: 三星的K4T1G164QE-HCLE6，CL=3，16bit，8bank， PageSize=512word
Nand Flash:ST的NAND512-A，512Mbit容量，BusWidth=8，PageSize=512Bytes，BlockSize=16K
DDR2和Nand Flash都与Ti的评估板不一样，所以下面的重点是对DDR2和Nand Flash的移植。
### 1、移植DDR2
1）在device.c中第106至123行是对DDR2参数的配置，根据数据手册修改为：
    // For K4T1G164QE @ 162 MHz,modified by Dashon 2012.3.16
static const Uint8 DDR_NM = 1;//16bit,modified by Dashon 2012.3.16
    static const Uint8 DDR_CL = 3;
    static const Uint8 DDR_IBANK = 3;
 static const Uint8 DDR_PAGESIZE = 1;//512,modified by Dashon 2012.3.16
    static const Uint8 DDR_T_RFC = 20;
    static const Uint8 DDR_T_RP = 2;
    static const Uint8 DDR_T_RCD = 2;
    static const Uint8 DDR_T_WR = 2;
    static const Uint8 DDR_T_RAS = 6;
    static const Uint8 DDR_T_RC = 8;
    static const Uint8 DDR_T_RRD = 2;
    static const Uint8 DDR_T_WTR = 1;
    static const Uint8 DDR_T_XSNR = 22;
    static const Uint8 DDR_T_XSRD = 199;
    static const Uint8 DDR_T_RTP = 1;
    static const Uint8 DDR_T_CKE = 2;
    static const Uint16 DDR_RR = 1053;
    static const Uint8 DDR_READ_Latency = 5; 
DDR2时钟采用默认的162MHz，不必修改PLL2的配置。其他参数基本与MT47H64M16BT一致，也不作修改。
2）在device.c中的Uint32 DEVICE_DDR2Init()函数中第432至441行，修改寄存器SBCR的值：
  // modified by Dashon 2012.3.16
  // For K4T1G164QE @ 162 MHz
  // Setup the read latency (CAS Latency + 3 = 6 (but write 6-1=5))
  DDR->DDRPHYCR = (0x50006400) | DDR_READ_Latency;
  // Set TIMUNLOCK bit, CAS LAtency 3, 8 banks, 512-word page size 
  //DDR->SDBCR = 0x0013C631;
  DDR->SDBCR =  0x0013c000 |
                (DDR_NM << 14)   |
                (DDR_CL << 9)    |
                (DDR_IBANK << 4) |
                (DDR_PAGESIZE <<0);
### 2、移植Nand Flash
1）在device.h中第109行，修改BusWidth为8：
#define DEVICE_BOOTCFG_EMIFWIDTH_MASK  (0x00000000)//BusWidth=8,modified by Dashon,2012.3.16
2）在device_nand.c中const NAND_CHIP_InfoObj DEVICE_NAND_CHIP_infoTable[]的定义中增加NAND512-A的信息：
  { 0x79,   8192,       32,     512+16},// 128 MB
 { 0xFF,   32768,      32,    512+16},// 512 MB,devID尚未确定,added by Dashon,2012.3.18
  { 0xA1,   1024,       64,     2048+64},// 128 MB - Big Block
第一个数字是devID，我暂时还没确定NAND1G-B的devID，在进行调试nand flash读取设备ID才能确定，这里先用0xFF代替。
### 3、其它
由于板子没有I2C，需在device.c的Uint32 DEVICE_init()函数屏蔽掉I2C的初始化：
  // I2C0 Setup
  //if (status == E_PASS) status |= DEVICE_I2C0Init();//modified by Dashon,2012.3.16
