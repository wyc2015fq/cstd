# 第51章 设置FLASH的读写保护及解除—零死角玩转STM32-F429系列 - xqhrs232的专栏 - CSDN博客
2018年01月25日 22:32:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：648
原文地址::[https://www.cnblogs.com/firege/p/5806164.html](https://www.cnblogs.com/firege/p/5806164.html)
相关文章
1、用J-Flash去掉STM32写保护----[http://download.csdn.net/download/liuhuan307056001/9643702](http://download.csdn.net/download/liuhuan307056001/9643702)
2、[如何解锁Flash读写保护](http://forum.eepw.com.cn/thread/251415/1)----[http://forum.eepw.com.cn/thread/251415/1](http://forum.eepw.com.cn/thread/251415/1)
3、STM32读保护功能和清除读保护功能设置----[https://wenku.baidu.com/view/e086f59dc8d376eeafaa3116.html](https://wenku.baidu.com/view/e086f59dc8d376eeafaa3116.html)
4、分享STM32代码读保护个人解决方案----[http://www.51hei.com/mcu/2767.html](http://www.51hei.com/mcu/2767.html)
5、STM32破解有多难？----[http://bbs.ic37.com/bbsview-29900.htm](http://bbs.ic37.com/bbsview-29900.htm)
## 第51章     设置FLASH的读写保护及解除
全套200集视频教程和1000页PDF教程请到秉火论坛下载：[www.firebbs.cn](http://www.firebbs.cn/)
野火视频教程优酷观看网址：http://i.youku.com/firege
本章参考资料：《STM32F4xx 中文参考手册》、《STM32F4xx规格书》、库说明文档《stm32f4xx_dsp_stdperiph_lib_um.chm》以及《Proprietary
 code read-out protection on microcontrollers》。
### 51.1 选项字节与读写保护
在实际发布的产品中，在STM32芯片的内部FLASH存储了控制程序，如果不作任何保护措施的话，可以使用下载器直接把内部FLASH的内容读取回来，得到bin或hex文件格式的代码拷贝，别有用心的厂商即可利用该代码文件山寨产品。为此，STM32芯片提供了多种方式保护内部FLASH的程序不被非法读取，但在默认情况下该保护功能是不开启的，若要开启该功能，需要改写内部FLASH选项字节(Option
 Bytes)中的配置。
#### 51.1.1 选项字节的内容
选项字节是一段特殊的FLASH空间，STM32芯片会根据它的内容进行读写保护、复位电压等配置，选项字节的构成见表 511。
表 511 选项字节的构成
|地址|[63:16]|[15:0]|
|----|----|----|
|0x1FFF C000|保留|ROP 和用户选项字节 (RDP & USER)|
|0x1FFF C008|保留|扇区 0 到 11 的写保护 nWRP 位|
|0x1FFE C000|保留|保留|
|0x1FFE C008|保留|扇区 12 到 23 的写保护 nWRP 位|
选项字节具体的数据位配置说明见表 512。
表 512 选项字节具体的数据位配置说明
位 11:0
nWRPi：
0：开启扇区 i 的写保护。
1：关闭扇区 i 的写保护。
我们主要讲解选项字节配置中的RDP位和PCROP位，它们分别用于配置读保护级别及代码读出保护。
#### 51.1.2 RDP读保护级别
修改选项字节的RDP位的值可设置内部FLASH为以下保护级别：
    0xAA：级别0，无保护
这是STM32的默认保护级别，它没有任何读保护，读取内部FLASH及"备份SRAM"的内容都没有任何限制。(注意这里说的"备份SRAM"是指STM32备份域的SRAM空间，不是指主SRAM，下同)
    其它值：级别1，使能读保护
把RDP配置成除0xAA或0xCC外的任意数值，都会使能级别1的读保护。在这种保护下，若使用调试功能(使用下载器、仿真器)或者从内部SRAM自举时都不能对内部FLASH及备份SRAM作任何访问(读写、擦除都被禁止)；而如果STM32是从内部FLASH自举时，它允许对内部FLASH及备份SRAM的任意访问。
也就是说，在级别1模式下，任何尝试从外部访问内部FLASH内容的操作都被禁止，例如无法通过下载器读取它的内容，或编写一个从内部SRAM启动的程序，若该程序读取内部FLASH，会被禁止。而如果是芯片自己访问内部FLASH，是完全没有问题的，例如前面的"读写内部FLASH"实验中的代码自己擦写内部FLASH空间的内容，即使处于级别1的读保护，也能正常擦写。
当芯片处于级别1的时候，可以把选项字节的RDP位重新设置为0xAA，恢复级别0。在恢复到级别0前，芯片会自动擦除内部FLASH及备份SRAM的内容，即降级后原内部FLASH的代码会丢失。在级别1时使用SRAM自举的程序也可以访问选项字节进行修改，所以如果原内部FLASH的代码没有解除读保护的操作时，可以给它加载一个SRAM自举的程序进行保护降级，后面我们将会进行这样的实验。
    0xCC：级别2，禁止调试
把RDP配置成0xCC值时，会进入最高级别的读保护，且设置后无法再降级，它会永久禁止用于调试的JTAG接口(相当于熔断)。在该级别中，除了具有级别1的所有保护功能外，进一步禁止了从SRAM或系统存储器的自举(即平时使用的串口ISP下载功能也失效)，JTAG调试相关的功能被禁止，选项字节也不能被修改。它仅支持从内部FLASH自举时对内部FLASH及SRAM的访问(读写、擦除)。
由于设置了级别2后无法降级，也无法通过JTAG、串口ISP等方式更新程序，所以使用这个级别的保护时一般会在程序中预留"后门"以更新应用程序，若程序中没有预留后门，芯片就无法再更新应用程序了。所谓的"后门"是一种IAP程序(In
 Application Program)，它通过某个通讯接口获取将要更新的程序内容，然后利用内部FLASH擦写操作把这些内容烧录到自己的内部FLASH中，实现应用程序的更新。
不同级别下的访问限制见图 511。
![](https://images2015.cnblogs.com/blog/896704/201608/896704-20160825114003788-482301197.png)
图 511 不同级别下的访问限制
不同保护级别之间的状态转换见图 512。
![](https://images2015.cnblogs.com/blog/896704/201608/896704-20160825114005804-601812417.png)
图 512 不同级别间的状态转换
#### 51.1.3 PCROP代码读出保护
在STM32F42xx及STM32F43xx系列的芯片中，除了可使用RDP对整片FLASH进行读保护外，还有一个专用的代码读出保护功能（Proprietary
 code readout protection，下面简称PCROP），它可以为内部FLASH的某几个指定扇区提供保护功能，所以它可以用于保护一些IP代码，方便提供给另一方进行二次开发，见图 513。
![](https://images2015.cnblogs.com/blog/896704/201608/896704-20160825114007647-232931013.jpg)
图 513 PCROP保护功能
当SPMOD位设置为0时(默认值)，nWRPi位用于指定要进行写保护的扇区，这可以防止错误的指针操作导致FLASH 内容的改变，若扇区被写保护，通过调试器也无法擦除该扇区的内容；当SPMOD位设置为1时，nWRPi位用于指定要进行PCROP保护的扇区。其中PCROP功能可以防止指定扇区的FLASH内容被读出，而写保护仅可以防止误写操作，不能被防止读出。
当要关闭PCROP功能时，必须要使芯片从读保护级别1降为级别0，同时对SPMOD位置0，才能正常关闭；若芯片原来的读保护为级别0，且使能了PCROP保护，要关闭PCROP时也要先把读保护级别设置为级别1，再在降级的同时设置SPMOD为0。
### 51.2 修改选项字节的过程
修改选项字节的内容可修改各种配置，但是，当应用程序运行时，无法直接通过选项字节的地址改写它们的内容，例如，接使用指针操作地址0x1FFFC0000的修改是无效的。要改写其内容时必须设置寄存器FLASH_OPTCR及FLASH_OPTCR1中的对应数据位，寄存器的与选项字节对应位置见图 514及图 515，详细说明请查阅《STM32参考手册》。
![](https://images2015.cnblogs.com/blog/896704/201608/896704-20160825114008335-154067280.png)
图 514 FLASH_OPTCR寄存器说明(nWRP表示0-11扇区)
![](https://images2015.cnblogs.com/blog/896704/201608/896704-20160825114009038-868891941.png)
图 515 FLASH_OPTCR1寄存器说明(nWRP表示12-23扇区)
默认情况下，FLASH_OPTCR寄存器中的第0位OPTLOCK值为1，它表示选项字节被上锁，需要解锁后才能进行修改，当寄存器的值设置完成后，对FLASH_OPTCR寄存器中的第1位OPTSTRT值设置为1，硬件就会擦除选项字节扇区的内容，并把FLASH_OPTCR/1寄存器中包含的值写入到选项字节。
所以，修改选项字节的配置步骤如下：
(1)    解锁，在 Flash 选项密钥寄存器 (FLASH_OPTKEYR) 中写入 OPTKEY1 = 0x0819 2A3B；接着在 Flash 选项密钥寄存器 (FLASH_OPTKEYR) 中写入 OPTKEY2 = 0x4C5D 6E7F。
(2)    检查 FLASH_SR 寄存器中的 BSY 位，以确认当前未执行其它Flash 操作。
(3)    在 FLASH_OPTCR 和/或 FLASH_OPTCR1 寄存器中写入选项字节值。
(4)    将 FLASH_OPTCR 寄存器中的选项启动位 (OPTSTRT) 置 1。
(5)    等待 BSY 位清零，即写入完成。
### 51.3 操作选项字节的库函数
为简化编程，STM32标准库提供了一些库函数，它们封装了修改选项字节时操作寄存器的过程。
##### 1.    选项字节解锁、上锁函数
对选项字节解锁、上锁的函数见代码清单 511。
代码清单 511选项字节解锁、上锁
1
2 #define FLASH_OPT_KEY1
 ((uint32_t)0x08192A3B)
3 #define FLASH_OPT_KEY2
 ((uint32_t)0x4C5D6E7F)
4
5 /**
6 * @brief Unlocks
 the FLASH Option Control Registers access.
7 * @param None
8 * @retval None
9 */
10 void FLASH_OB_Unlock(void)
11 {
12 if((FLASH->OPTCR
 & FLASH_OPTCR_OPTLOCK) != RESET)
13 {
14 /*
 Authorizes the Option Byte register programming */
15 FLASH->OPTKEYR
 = FLASH_OPT_KEY1;
16 FLASH->OPTKEYR
 = FLASH_OPT_KEY2;
17 }
18 }
19
20 /**
21 * @brief Locks
 the FLASH Option Control Registers access.
22 * @param None
23 * @retval None
24 */
25 void FLASH_OB_Lock(void)
26 {
27 /* Set the
 OPTLOCK Bit to lock the FLASH Option Byte Registers access */
28 FLASH->OPTCR
 |= FLASH_OPTCR_OPTLOCK;
29 }
解锁的时候，它对FLASH_OPTCR寄存器写入两个解锁参数，上锁的时候，对FLASH_
 OPTCR寄存器的FLASH_OPTCR_OPTLOCK位置1。
##### 2.    设置读保护级别
解锁后设置选项字节寄存器的RDP位可调用FLASH_OB_RDPConfig完成，见代码清单 512。
代码清单 512 设置读保护级别
1 /**
2 * @brief Sets
 the read protection level.
3 * @param OB_RDP:
 specifies the read protection level.
4 * This parameter
 can be one of the following values:
5 * @arg OB_RDP_Level_0:
 No protection
6 * @arg OB_RDP_Level_1:
 Read protection of the memory
7 * @arg OB_RDP_Level_2:
 Full chip protection
8 *
9 *
 /!\ Warning /!\ When enabling OB_RDP level 2 it's no more possible to go back to level 1 or 0
10 *
11 * @retval None
12 */
13 void FLASH_OB_RDPConfig(uint8_t OB_RDP)
14 {
15 FLASH_Status
 status = FLASH_COMPLETE;
16
17 /*
 Check the parameters */
18 assert_param(IS_OB_RDP(OB_RDP));
19
20 status = FLASH_WaitForLastOperation();
21
22 if(status
 == FLASH_COMPLETE)
23 {
24 *(__IO uint8_t*)OPTCR_BYTE1_ADDRESS
 = OB_RDP;
25
26 }
27 }
该函数根据输入参数设置RDP寄存器位为相应的级别，其注释警告了若配置成OB_RDP_Level_2会无法恢复。类似地，配置其它选项时也有相应的库函数，如FLASH_OB_PCROP1Config、FLASH_OB_WRP1Config分别用于设置要进行PCROP保护或WRP保护(写保护)的扇区。
##### 3.    写入选项字节
调用上一步骤中的函数配置寄存器后，还要调用代码清单 513中的FLASH_OB_Launch函数把寄存器的内容写入到选项字节中。
代码清单 513 写入选项字节
1 /**
2 * @brief Launch
 the option byte loading.
3 * @param None
4*
 @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PROGRAM,
5 * FLASH_ERROR_WRP,
 FLASH_ERROR_OPERATION or FLASH_COMPLETE.
6 */
7 FLASH_Status
 FLASH_OB_Launch(void)
8 {
9 FLASH_Status
 status = FLASH_COMPLETE;
10
11 /*
 Set the OPTSTRT bit in OPTCR register */
12 *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS
 |= FLASH_OPTCR_OPTSTRT;
13
14 /*
 Wait for last operation to be completed */
15 status = FLASH_WaitForLastOperation();
16
17 return status;
18 }
该函数设置FLASH_OPTCR_OPTSTRT位后调用了FLASH_WaitForLastOperation函数等待写入完成，并返回写入状态，若操作正常，它会返回FLASH_COMPLETE。
### 51.4 实验：设置读写保护及解除
在本实验中我们将以实例讲解如何修改选项字节的配置，更改读保护级别、设置PCROP或写保护，最后把选项字节恢复默认值。
本实验要进行的操作比较特殊，在开发和调试的过程中都是在SRAM上进行的（使用SRAM启动方式）。例如，直接使用FLASH版本的程序进行调试时，如果该程序在运行后对扇区进行了写保护而没有解除的操作或者该解除操作不正常，此时将无法再给芯片的内部FLASH下载新程序，最终还是要使用SRAM自举的方式进行解除操作。所以在本实验中为便于修改选项字节的参数，我们统一使用SRAM版本的程序进行开发和学习，当SRAM版本调试正常后再改为FLASH版本。
关于在SRAM中调试代码的相关配置，请参考前面的章节。
注意：
若您在学习的过程中想亲自修改代码进行测试，请注意备份原工程代码。当芯片的FLASH被保护导致无法下载程序到FLASH时，可以下载本工程到芯片，并使用SRAM启动运行，即可恢复芯片至默认配置。但如果修改了读保护为级别2，采用任何方法都无法恢复！(除了这个配置，其它选项都可以大胆地修改测试。)
#### 51.4.1 硬件设计
本实验在SRAM中调试代码，因此把BOOT0和BOOT1引脚都使用跳线帽连接到3.3V，使芯片从SRAM中启动。
#### 51.4.2 软件设计
本实验的工程名称为"设置读写保护与解除"，学习时请打开该工程配合阅读，它是从"RAM调试—多彩流水灯"工程改写而来的。为了方便展示及移植，我们把操作内部FLASH相关的代码都编写到"internalFlash_reset.c"及"internalFlash_reset.h"文件中，这些文件是我们自己编写的，不属于标准库的内容，可根据您的喜好命名文件。
##### 1.    主要实验
(1)    学习配置扇区写保护；
(2)    学习配置PCROP保护；
(3)    学习配置读保护级别；
(4)    学习如何恢复选项字节到默认配置；
##### 2.    代码分析
###### 配置扇区写保护
我们先以代码清单 514中的设置与解除写保护过程来学习如何配置选项字节。
代码清单 514 配置扇区写保护
1
2 #define FLASH_WRP_SECTORS
 (OB_WRP_Sector_0|OB_WRP_Sector_1)
3 __IO uint32_t SectorsWRPStatus
 = 0xFFF;
4
5 /**
6 * @brief WriteProtect_Test,普通的写保护配置
7 *
 @param 运行本函数后会给扇区FLASH_WRP_SECTORS进行写保护，再重复一次会进行解写保护
8 * @retval None
9 */
10 void WriteProtect_Test(void)
11 {
12 FLASH_Status
 status = FLASH_COMPLETE;
13 {
14 /* 获取扇区的写保护状态 */
15 SectorsWRPStatus
 = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
16
17 if (SectorsWRPStatus
 == 0x00)
18 {
19 /* 扇区已被写保护，执行解保护过程*/
20
21 /* 使能访问OPTCR寄存器 */
22 FLASH_OB_Unlock();
23
24 /* 设置对应的nWRP位，解除写保护 */
25 FLASH_OB_WRPConfig(FLASH_WRP_SECTORS,
 DISABLE);
26 status=FLASH_OB_Launch();
27 /* 开始对选项字节进行编程 */
28 if (status
 != FLASH_COMPLETE)
29 {
30 FLASH_ERROR("对选项字节编程出错，解除写保护失败，status
 = %x",status);
31 /*
 User can add here some code to deal with this error */
32 while (1)
33 {
34 }
35 }
36 /* 禁止访问OPTCR寄存器 */
37 FLASH_OB_Lock();
38
39 /* 获取扇区的写保护状态 */
40 SectorsWRPStatus
 = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
41
42 /* 检查是否配置成功 */
43 if (SectorsWRPStatus
 == FLASH_WRP_SECTORS)
44 {
45 FLASH_INFO("解除写保护成功！");
46 }
47 else
48 {
49 FLASH_ERROR("未解除写保护！");
50 }
51 }
52 else
53 { /* 若扇区未被写保护，开启写保护配置 */
54
55 /* 使能访问OPTCR寄存器 */
56 FLASH_OB_Unlock();
57
58 /*使能 FLASH_WRP_SECTORS 扇区写保护 */
59 FLASH_OB_WRPConfig(FLASH_WRP_SECTORS,
 ENABLE);
60
61 status=FLASH_OB_Launch();
62 /* 开始对选项字节进行编程 */
63 if (status
 != FLASH_COMPLETE)
64 {
65 FLASH_ERROR("对选项字节编程出错，设置写保护失败，status
 = %x",status);
66 while (1)
67 {
68 }
69 }
70 /* 禁止访问OPTCR寄存器 */
71 FLASH_OB_Lock();
72
73 /* 获取扇区的写保护状态 */
74 SectorsWRPStatus
 = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
75
76 /* 检查是否配置成功 */
77 if (SectorsWRPStatus
 == 0x00)
78 {
79 FLASH_INFO("设置写保护成功！");
80 }
81 else
82 {
83 FLASH_ERROR("设置写保护失败！");
84 }
85 }
86 }
87 }
本函数分成了两个部分，它根据目标扇区的状态进行操作，若原来扇区为非保护状态时就进行写保护，若为保护状态就解除保护。其主要操作过程如下：
    调用FLASH_OB_GetWRP函数获取目标扇区的保护状态若扇区被写保护，则开始解除保护过程，否则开始设置写保护过程；
    调用FLASH_OB_Unlock解锁选项字节的编程；
    调用FLASH_OB_WRPConfig函数配置目标扇区关闭或打开写保护；
    调用FLASH_OB_Launch函数把寄存器的配置写入到选项字节；
    调用FLASH_OB_GetWRP函数检查是否配置成功；
    调用FLASH_OB_Lock禁止修改选项字节。
###### 配置PCROP保护
配置PCROP保护的过程与配置写保护过程稍有区别，见代码清单 515。
代码清单 515 配置PCROP保护(internalFlash_reset.c文件)
1
2 /**
3 *
 @brief SetPCROP,设置PCROP位，用于测试解锁
4 *
 @note 使用有问题的串口ISP下载软件，可能会导致PCROP位置1，
5 导致无法给芯片下载程序到FLASH，本函数用于把PCROP位置1，
6 模拟出无法下载程序到FLASH的环境，以便用于解锁的程序调试。
7 若不了解PCROP位的作用，请不要执行此函数！！
8 * @param None
9 * @retval None
10 */
11 void SetPCROP(void)
12 {
13
14 FLASH_Status
 status = FLASH_COMPLETE;
15
17
18 FLASH_INFO();
19 FLASH_INFO("正在设置PCROP保护，请耐心等待...");
20 //选项字节解锁
21 FLASH_OB_Unlock();
22
23 //设置为PCROP模式
24 FLASH_OB_PCROPSelectionConfig(OB_PcROP_Enable);
25 //设置扇区0进行PCROP保护
26 FLASH_OB_PCROPConfig(OB_PCROP_Sector_10,ENABLE);
27 //把寄存器设置写入到选项字节
28 status =FLASH_OB_Launch();
29
30 if (status
 != FLASH_COMPLETE)
31 {
32 FLASH_INFO("设置PCROP失败！");
33 }
34 else
35 {
36 FLASH_INFO("设置PCROP成功！");
37
38 }
39 //选项字节上锁
40 FLASH_OB_Lock();
41 }
该代码在解锁选项字节后，调用FLASH_OB_PCROPSelectionConfig函数把SPMOD寄存器位配置为PCROP模式，接着调用FLASH_OB_PCROPConfig函数配置目标保护扇区。
###### 恢复选项字节为默认值
当芯片被设置为读写保护或PCROP保护时，这时给芯片的内部FLASH下载程序时，可能会出现图 516和图 517的擦除FLASH失败的错误提示。
![](https://images2015.cnblogs.com/blog/896704/201608/896704-20160825114009929-931103573.jpg)
图 516 擦除失败提示
![](https://images2015.cnblogs.com/blog/896704/201608/896704-20160825114010882-1328979274.jpg)
图 517 擦除进度条卡在开始状态
只要不是把读保护配置成了级别2保护，都可以使用SRAM启动运行代码清单 516中的函数恢复选项字节为默认状态，使得FLASH下载能正常进行。
代码清单 516 恢复选项字节为默认值
1 // @brief OPTCR
 register byte 0 (Bits[7:0]) base address
2 #define OPTCR_BYTE0_ADDRESS
 ((uint32_t)0x40023C14)
3 //@brief OPTCR
 register byte 1 (Bits[15:8]) base address
4 #define OPTCR_BYTE1_ADDRESS
 ((uint32_t)0x40023C15)
5 //@brief OPTCR
 register byte 2 (Bits[23:16]) base address
6 #define OPTCR_BYTE2_ADDRESS
 ((uint32_t)0x40023C16)
7 // @brief OPTCR
 register byte 3 (Bits[31:24]) base address
8 #define OPTCR_BYTE3_ADDRESS
 ((uint32_t)0x40023C17)
9 // @brief OPTCR1
 register byte 0 (Bits[7:0]) base address
10 #define OPTCR1_BYTE2_ADDRESS
 ((uint32_t)0x40023C1A)
11
12 /**
13 *
 @brief InternalFlash_Reset,恢复内部FLASH的默认配置
14 * @param None
15 * @retval None
16 */
17 int InternalFlash_Reset(void)
18 {
19 FLASH_Status
 status = FLASH_COMPLETE;
20
21 /* 使能访问选项字节寄存器 */
22 FLASH_OB_Unlock();
23
24 /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
25 /* 清除各种FLASH的标志位 */
26 FLASH_ClearFlag(FLASH_FLAG_EOP
 | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
27 FLASH_FLAG_PGAERR
 | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
28
29 FLASH_INFO("\r\n");
30 FLASH_INFO("正在准备恢复的条件，请耐心等待...");
31
32 //确保把读保护级别设置为LEVEL1，以便恢复PCROP寄存器位
33 //PCROP寄存器位从设置为0时，必须是读保护级别由LEVEL1转为LEVEL0时才有效，
34 //否则修改无效
35 FLASH_OB_RDPConfig(OB_RDP_Level_1);
36
37 status=FLASH_OB_Launch();
38
39 status = FLASH_WaitForLastOperation();
40
41 //设置为LEVEL0并恢复PCROP位
42
43 FLASH_INFO("\r\n");
44 FLASH_INFO("正在擦除内部FLASH的内容，请耐心等待...");
45
46 //关闭PCROP模式
47 FLASH_OB_PCROPSelectionConfig(OB_PcROP_Disable);
48 FLASH_OB_RDPConfig(OB_RDP_Level_0);
49
50 status =FLASH_OB_Launch();
51
52 //设置其它位为默认值
53 (*(__IO uint32_t *)(OPTCR_BYTE0_ADDRESS))=0x0FFFAAE9;
54 (*(__IO uint16_t
 *)(OPTCR1_BYTE2_ADDRESS))=0x0FFF;
55 status =FLASH_OB_Launch();
56 if (status
 != FLASH_COMPLETE)
57 {
58 FLASH_ERROR("恢复选项字节默认值失败，错误代码：status=%X",status);
59 }
60 else
61 {
62 FLASH_INFO("恢复选项字节默认值成功！");
63 }
64 //禁止访问
65 FLASH_OB_Lock();
66
67 return status;
68 }
这个函数进行了如下操作：
    调用FLASH_OB_Unlock解锁选项字节的编程；
    调用FLASH_ClearFlag函数清除所有FLASH异常状态标志；
    调用FLASH_OB_RDPConfig函数设置为读保护级别1，以便后面能正常关闭PCROP模式；
    调用FLASH_OB_Launch写入选项字节并等待读保护级别设置完毕；
    调用FLASH_OB_PCROPSelectionConfig函数关闭PCROP模式；
    调用FLASH_OB_RDPConfig函数把读保护级别降为0；
    调用FLASH_OB_Launch定稿选项字节并等待降级完毕，由于这个过程需要擦除内部FLASH的内容，等待的时间会比较长；
    直接操作寄存器，使用"(*(__IO uint32_t *)(OPTCR_BYTE0_ADDRESS))=0x0FFFAAE9;"和"(*(__IO uint16_t *)(OPTCR1_BYTE2_ADDRESS))=0x0FFF;"语句把OPTCR及OPTCR1寄存器与选项字节相关的位都恢复默认值；
    调用FLASH_OB_Launch函数等待上述配置被写入到选项字节；
    恢复选项字节为默认值操作完毕。
###### main函数
最后来看看本实验的main函数，见。代码清单 517。
代码清单 517 main函数
1 /**
2 * @brief 主函数
3 * @param 无
4 * @retval 无
5 */
6 int main(void)
7 {
8 /*
 LED 端口初始化 */
9 LED_GPIO_Config();
10 Debug_USART_Config();
11 LED_BLUE;
12
13 FLASH_INFO("本程序将会被下载到STM32的内部SRAM运行。");
14 FLASH_INFO("下载程序前，请确认把实验板的BOOT0和BOOT1引脚都接到3.3V电源处！！");
15
16 FLASH_INFO("\r\n");
17 FLASH_INFO("----这是一个STM32芯片内部FLASH解锁程序----");
18 FLASH_INFO("程序会把芯片的内部FLASH选项字节恢复为默认值");
19
20
21 #if
 0//工程调试、演示时使用，正常解除时不需要运行此函数
22 SetPCROP(); //修改PCROP位，仿真芯片被锁无法下载程序到内部FLASH的环境
23 #endif
24
25 #if
 0//工程调试、演示时使用，正常解除时不需要运行此函数
26 WriteProtect_Test(); //修改写保护位，仿真芯片扇区被设置成写保护的的环境
27 #endif
28
30
31 /*恢复选项字节到默认值，解除保护*/
32 if(InternalFlash_Reset()==FLASH_COMPLETE)
33 {
34 FLASH_INFO("选项字节恢复成功，请把BOOT0和BOOT1引脚都连接到GND，");
35 FLASH_INFO("然后随便找一个普通的程序，下载程序到芯片的内部FLASH进行测试");
36 LED_GREEN;
37 }
38 else
39 {
40 FLASH_INFO("选项字节恢复成功失败，请复位重试");
41 LED_RED;
42 }
43
45
46 while (1);
47 }
在main函数中，主要是调用了InternalFlash_Reset函数把选项字节恢复成默认值，程序默认时没有调用SetPCROP和WriteProtect_Test函数设置写保护，若您想观察实验现象，可修改条件编译的宏，使它加入到编译中。
##### 3.    下载测试
把开发板的BOOT0和BOOT1引脚都使用跳线帽连接到3.3V电源处，使它以SRAM方式启动，然后用USB线连接开发板"USB TO UART"接口跟电脑，在电脑端打开串口调试助手，把编译好的程序下载到开发板并复位运行，在串口调试助手可看到调试信息。程序运行后，请耐心等待至开发板亮绿灯或串口调试信息提示恢复完毕再给开发板断电，否则由于恢复过程被中断，芯片内部FLASH会处于保护状态。
芯片内部FLASH处于保护状态时，可重新下载本程序到开发板以SRAM运行恢复默认配置。
### 51.5 每课一问
1.    分别设置芯片为读保护级别1、写保护及PCROP保护，然后给芯片的内部FLASH下载程序，观察实验现象。
开源共享，共同进步。
