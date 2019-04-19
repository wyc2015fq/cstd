# [置顶] 中文翻译 for S5PV210_iROM - xqhrs232的专栏 - CSDN博客
2014年08月12日 14:19:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：788
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/a627088424/article/details/9149671](http://blog.csdn.net/a627088424/article/details/9149671)
相关文章
1、s5pv210 的bootloader是怎么个分配法----[http://bbs.csdn.net/topics/390535472?page=1#post-397965496](http://bbs.csdn.net/topics/390535472?page=1#post-397965496)
2、S5PV210 Bootloader的问题----[http://bbs.csdn.net/topics/370048762](http://bbs.csdn.net/topics/370048762)
3、[S5PV210的BL1应用](http://blog.csdn.net/kangear/article/details/9002561)----[http://blog.csdn.net/kangear/article/details/9002561](http://blog.csdn.net/kangear/article/details/9002561)
4、[S5PV210的IRAM应用](http://blog.csdn.net/kangear/article/details/8993247)----[http://blog.csdn.net/kangear/article/details/8993247](http://blog.csdn.net/kangear/article/details/8993247)
5、s5pv210启动过程_百度文章----[http://wenku.baidu.com/link?url=MKBWFYBgywiSQPskaMu8pOnoCiUmXq-al0yQVyqaxDqcMPDHtzWs-NZlGxNiC445aCEZgbuttzxcEIqMPFKSidvHObFShXxhS3eI-PtTFwu](http://wenku.baidu.com/link?url=MKBWFYBgywiSQPskaMu8pOnoCiUmXq-al0yQVyqaxDqcMPDHtzWs-NZlGxNiC445aCEZgbuttzxcEIqMPFKSidvHObFShXxhS3eI-PtTFwu)
#  应用笔记
                                                                                                   (Internal ROM Booting)

                                                                                                                              S5PV210
                                                                                                                 精简指令集微处理器

Preliminary product information describe products that are in development,
for which full characterization data and associated errata are not yet available.
Specifications and information herein are subject to change without notice.
Confidential Proprietary of Samsung Electronics Co., Ltd
Copyright © 2008 Samsung Electronics, Inc. All Rights Reserved
## 使用IROM来引导，需要先懂一些硬件特性：
·S5PV210处理器特性
·64KB 内部ROM（iROM）
·96KB 内部SRAM
·通用SDRAM和其控制器
·4/8位高速SD/MMC控制器
  4-位SD/4-位MMC/4或8-位eMMC
·Nand Flash控制器
·OneNand控制器(AUDI)
·eSSD控制器
·UART/USB控制器
## 支持：
·OneNand启动（Mux/Demux）
·Nand 启动（所有的nand启动都支持8位的H/W ECC校检，但是只是4KB 5cycle类型的 nand支持16位的ECC。）
·MMC启动（兼容eMMC）
·eSSD启动
·UART/USB启动
·支持 安全模式启动
  -除了UART/USB模式的启动，其他模式都要被核实Bootloader的完整性。
  -为了支持安全启动模式，有一个所谓的Security Key value应该写入S5PV210
   如果没有写入这个值，就没有所谓的安全启动模式。
·支持两种启动（Second boot support）
  -当第一种（1st）启动模式失败时，SD/MMC将尝试通过它的通道2启动 with 4-bit data

## 与线路相关的部分：
1.使用OM针脚选择启动设备
2.V210所有的存储类的启动设备都有一个SD/MMC通道2 作为second启动
3.Xm0CSn4/NFCSn2/ONANDXL_CSn0信号用来 for OneNand启动
  （BL1代码的最开始应该包含二进制的检验数据checksum data）
4.Xm0CSn2/NFCSn0信号用来 for Nand 启动
5.SD/MMC/eMMC启动（通道0用来first boot，通道2用来second boot）
6.UART的通道2端口用来 for UART 启动
![](https://img-blog.csdn.net/20130622102339703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Internal SRAM中的分配给BL1的大小为16KB，给BL2的大小为80KB（16+80=96KB）
1.iROM做最初的启动：初始化系统时钟，特定设备的控制器和booting设备。
2.iROM中的启动代码能加载bootloader到SRAM。这一步被加载的bootloader称作BL1
3.接着BL1执行：BL1将会加载SRAM中的bootloader(称作BL2)，接着BL1将核实BL2以判断是否为安全模式启动。
4.接着BL2执行：BL2初始化DRAM控制器然后加载OS到SARM中。
5.最后，跳到OS的开头地址。   给操作系统提供了良好的运行环境。

## iROM启动步骤：
1.关看门狗
2.初始化指令缓存单元
3.初始化栈区域
4.初始化堆区域
5.初始化块设备拷贝功能
6.初始化PLL并设置系统时钟
7.拷贝BL1到internal SRAM
8.核实BL1中的checksum
   如果checksum失败了，iROM将尝试second boot（SD/MMC 通道2）。
9.检查是不是安全模式启动
   如果S5PV210中写入了security key value，将会是安全模式启动
   如果是安全模式启动，核实BL1的完整性
10.跳转到BL1的起始地址
![](https://img-blog.csdn.net/20130622102418531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130622102448500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Figure 2. V210 boot-up diagram

iROM 2nd boot-up 步骤 when 1st boot fail
![](https://img-blog.csdn.net/20130622102545671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果是在安全启动模式下，BL1完整性核实应该被添加在checknum之后。
【劝告】三星建议2nd，UART和USB启动仅仅用来调试用。

## UART启动模式
----S5PV210 iROM支持UART下载。UART下载的检查是通过发送标志位给DNW（没有选择信号for UART下载）。为了避免UART超时，用户应该在板子上电之前就先设置好DNW。也就是说，BL1下载程序启动后，iROM的UART下载就就绪了。
![](https://img-blog.csdn.net/20130622102606328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
UART启动需要配置OM针脚，请参考Boot configuration

## USB启动模式
----如果UART超时了，iROM尝试USB启动模式。为了避免USB判断（negotiation）超时，用户需要先通过USB将PC与目标板连接。如果USB正确连接上了，用户就能通过USB下载BL1了。
![](https://img-blog.csdn.net/20130622102624281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130622102649328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130622102700156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
通过UART/USB模式启动的话，BL1是不需要头信息的，此时BL1的基地址是0xd002_0000.
其它情况，BL1都需要有头信息并且此时的基地址应该为0xd0020010.
![](https://img-blog.csdn.net/20130622102726296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130622102738890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 全局变量
如果用MMC启动，MMC存储卡的信息需要写入到特殊区域。参考下表
![](https://img-blog.csdn.net/20130622102759781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 设备拷贝函数：
S5PV210内部ROM中有块设备拷贝函数，因此开发者不需要自己实现设备拷贝函数，这些函数能将任何设备中的数据拷贝到SDRAM中。当internal ROM启动程序完毕后就可以使用这些函数了。
|地址|函数名|功能|
|----|----|----|
|0xD0037F90|NF8_ReadPage_Adv|先进的NF8_ReadPage函数                                            (8位ECC校验)                                  只针对：2048, 4096 Page  8bits-bus的Nand）|
|0xD0037F94|NF16_ReadPage_Adv|先进的NF16_ReadPage函数                                            (4位ECC校验)                                               （只针对：2048 page size, 5 cycle address, 16bits-bus的Nand）|
|0xD0037F98|CopySDMMCtoMem|将SD/MMC中的数据拷贝到SDRAM. 在IROM启动完成后就可以使用了。|
|0xD0037F9C|CopyMMC4_3toMem|将eMMC中的数据拷贝到SDRAM. 在IROM启动完成后就可以使用了。|
|0xD0037FA0|CopyOND_ReadMultiPages|将OneNand中的数据拷贝到SDRAM. 在IROM启动完成后就可以使用了。(normal speed copy)|
|0xD0037FA4|CopyOND_ReadMultiPages_Adv|将OneNand中的数据拷贝到SDRAM. 在IROM启动完成后就可以使用了。(fast speed copy)|
|0xD0037FA8|Copy_eSSDtoMem|将eSSD中的数据拷贝到SDRAM. 在IROM启动完成后就可以使用了。(CPUPIO mode)|
|0xD0037FAC|Copy_eSSDtoMem_Adv|将eSSD中的数据拷贝到SDRAM. 在IROM启动完成后就可以使用了。(UDMA mode)|
|0xD0037FB0|NF8_ReadPage_Adv128p|先进的NF8_ReadPage函数(8位ECC校验)只针对：128 page per block with 2K page size的Nand。|
·先进的Nand Flash块拷贝函数(8-Bit ECC Check)
/**
*这个函数能能拷贝页的一块到目的存储器（仅8位ECC校验）
*@参数 uint32 block：源的块地址
*@参数 uint32 page： 源的页地址
*@参数 uint8 *buffer:目标缓存的指针
*@返回 int32-成功 or 失败
*/
#define NF8_ReadPage_Adv (a,b,c) (((int(*)(uint32, uint32, uint8*))(*((uint32 *) 0xD0037F90)))(a,b,c))
。。。。。。。。
更多的函数介绍请查阅官方文档，这里不再介绍

## 启动块分配指导：
1.SD/MMC/eSSD设备块分配
![](https://img-blog.csdn.net/20130622102817218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上面的指导只是一个标本，但是有一条是强制的
------ 第一块不应该被使用（Reserved）
2.eMMC
![](https://img-blog.csdn.net/20130622102828531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.OneNAND/NAND
![](https://img-blog.csdn.net/20130622102838875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
【劝告】如果是NAND启动，ECC数据应该像如下存放
![](https://img-blog.csdn.net/20130622102859093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Note1）8位ECC校检时，ECC数据大小为13Byte
Note2）16位ECC校检时，ECC数据大小为26Byte
  如果16位ECC校检，对于每个NAND Flash  ECC的空间大小是可变的，因此需要检验NAND Flash的数据表。
## 启动代码的头信息：
BL1必须有头数据。iROM中的代码会将头数据拷贝到internal SRAM中。头数据含有2个信息，一个是BL1的大小，另一个是BL1的checknum数据。
![](https://img-blog.csdn.net/20130622102933531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当加载BL1时，iROM会利用头数据去检查BL1的大小，并且拷贝BL1到internal SRAM中。拷贝完后，iROM总计刚才拷贝过来的BL1的数据，并与BL1的头信息中checknum进行比较。如果成功通过，BL1将开始执行。要不然iROM将尝试第二种启动（Second boot）

## 制作checknum的代码：
for(count=0;count< dataLength;count+=1)
{
            buffer = (*(volatile u8*)(uBlAddr+count));
            checkSum = checkSum + buffer;
}
---count   unsigned int类型的变量
---dataLength   unsigned int类型的变量
---buffer    unsigned short类型的变量，它被用来从BL1中读取1byte数据
---checknum unsigned int类型的变量，它包含BL1代码的总和。
## 时钟配置：
V210只能使用24MHz的外部晶振。
![](https://img-blog.csdn.net/20130622103004531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130622103017765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130622103029156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 启动配置：
OM[5:0]针脚应该是VDDSYS或GND两者之一（直连）。目的是为了当进入睡眠模式时尽可能的减少漏电流。但是如果你一定要去做选择， 你应该添加100K以上的上拉和下拉电阻。
![](https://img-blog.csdn.net/20130622103051656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130622103103406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 错误处理：
当iROM启动发生错误时，XPWMTOUT0针脚将转换以指示错误类型。
[例子：OneNAND checknum失败]
如果在OneNAND启动模式中checknum失败时。XPWMTOUT针脚40:60[高电平：低电平]
![](https://img-blog.csdn.net/20130622103127875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
所有情况的指示如下表：
![](https://img-blog.csdn.net/20130622103141890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 硬件指导：
eMMC启动模式中电源连接参考
eMMC能支持MMC 4.3 spec。在启动模式功能上与MMC 4.3 和 MMC 4.2有区别。
![](https://img-blog.csdn.net/20130622103154890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYyNzA4ODQyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
