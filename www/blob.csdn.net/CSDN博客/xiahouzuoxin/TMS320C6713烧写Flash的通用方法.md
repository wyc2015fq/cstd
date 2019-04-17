# TMS320C6713烧写Flash的通用方法 - xiahouzuoxin - CSDN博客





2014年03月30日 23:55:33[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：14459








## 1 你必须知道的TMS320C6000启动过程


这部分内容在我的另一篇博客

[DSP TMS320C6000基础学习（7）—— Bootloader与VectorTable](http://blog.csdn.net/xiahouzuoxin/article/details/9713461)


有提到过，这里重新摘录一遍。

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/reset.jpg)


如上图
- 在Device Reset阶段：设备初始化为默认状态，大部分三态输出都配置为高阻态。
- 在CPU Reset阶段：从RS上升沿处开始（这个时候，HD[4:3]配置启动模式，HD8配置大小端模式，CLKMODE配置输入时钟源，根据HPI_EN配置外设功能），处理器检查启动模式HD[4:3]，启动bootloader程序。

![HD43](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/HD43.png)


从上图可以看出，CE1地址空间必需连接Flash芯片才能使用外部Flash引导模式，在电路设计时要注意。


若HD[4:3]=10（本文的操作环境基于此），EDMA自动将CE1起始位置的1KB代码拷贝到内部程序存储器的0地址，这部分功能是由硬件完成的，称一级引导Bootloader。

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/copy_1KB.jpg)


因此，外部Flash启动的最简单的想法就是：把要运行的程序放到CE1的起始1KB地址空间。这样只要设置HD[4:3]=10就能自启动了。那这么简单，还有讨论本文的必要吗？


呃，如果你的思维还停留在小孩子过家家的程度，唉。。。1KB？1KB才能存多少代码？要是代码量超过1KB呢？这正是本文要探讨的问题的初衷：程序代码>1KB，如何让C6713的程序从外部Flash自启动？


这就涉及另一个Bootloader了，我们称之为二级引导Bootloader（说白了就是一段小程序）。二级Bootloader作用有：（1）在上电复位后将用户的应用程序从Flash拷贝到RAM中执行；（2）跳转到应用程序的入口函数处。


二级Bootloader的执行要由一级Bootloader拷贝到RAM中执行，这就明白了，二级Bootloader必须放在外部Flash的起始的1KB位置处。


我们简要的用个图描述下所谓的二级Bootloader的自启动过程及主要思路。

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/2levelboot.png)


要完成这个过程，
- 首先要编写一段称为2 Level Bootloader的启动代码并烧写到Flash的初始1KB地址处（DSP6713的CE1起始地址为0x90000000），1 Level Bootloader将该代码拷贝到RAM的起始0地址，开始执行。
- 烧写用户程序到0x90000400开始的Flash地址处
- 2 Level Bootloader将0x90000400开始的用户代码拷贝到RAM的0x400地址处
- 2 Level Bootloader调用_c_int00用户入口程序，然后调用main函数开始执行用户代码


关于_c_int00的介绍也请参考[DSP TMS320C6000基础学习（7）—— Bootloader与VectorTable]本文所有操作的前提是您已经配置好了中断向量表（这样在调用_c_int00时才能正确的进入到用户程序）。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md#2-%E7%BC%96%E5%86%99%E4%BA%8C%E7%BA%A7bootloader)2 编写二级Bootloader


先宏定义一下EMIF相关的寄存器，因为我们要读Flash，所以在二级引导程序运行前要配置EMIF寄存器，

;;========c6713_emif.s62========;.title"Flash bootup utility";globalEMIFsymbolsdefinedforthec671xfamily.includeboot_c671x.h62;EMIFRegisterAddressesforc671xfamilyEMIF_GCTL.equ;EMIFglobalcontrolEMIF_CE1.equ;addressofEMIFCE1controlreg.EMIF_CE0.equ;EMIFCE0controlEMIF_CE2.equ;EMIFCE2controlEMIF_CE3.equ;EMIFCE3controlEMIF_SDRAMCTL.equ;EMIFSDRAMcontrolEMIF_SDRAMTIM.equ;EMIFSDRAMtimerEMIF_SDRAMEXT.equ;EMIFSDRAMextension;EMIFRegisterValuesspecificallyforDSKEMIF_GCTL_V.equ;EMIF_CE0_V.equ;EMIFCE0SDRAMEMIF_CE1_V.equ;EMIFCE1Flash-bitEMIF_CE2_V.equ;EMIFCE2Daughtercard-bitasyncEMIF_CE3_V.equ;EMIFCE3Daughtercard-bitasyncEMIF_SDRAMCTL_V.equ;EMIFSDRAMcontrolEMIF_SDRAMTIM_V.equ;SDRAMtiming(refresh)EMIF_SDRAMEXT_V.equ;SDRAMextendedcontrol


宏定义的EMIF寄存器声明为全局符号，.global与C语言中的extern效果一致，声明为外部符号。

;;========boot_c671x.h62========;.if($isdefed("BOOT_C671X_")=);preventmultipleincludesofthisfileBOOT_C671X_.set;EMIFRegisterAddressesforc671xfamily.globalEMIF_GCTL;EMIFglobalcontrol.globalEMIF_CE1;addressofEMIFCE1controlreg..globalEMIF_CE0;EMIFCE0control.globalEMIF_CE2;EMIFCE2control.globalEMIF_CE3;EMIFCE3control.globalEMIF_SDRAMCTL;EMIFSDRAMcontrol.globalEMIF_SDRAMTIM;EMIFSDRAMtimer.globalEMIF_SDRAMEXT;EMIFSDRAMextension;EMIFRegisterValuesforc671xfamily.globalEMIF_GCTL_V;.globalEMIF_CE0_V;EMIFCE0SDRAM.globalEMIF_CE1_V;EMIFCE1Flash-bit.globalEMIF_CE2_V;EMIFCE2Daughtercard-bitasync.globalEMIF_CE3_V;EMIFCE3Daughtercard-bitasync.globalEMIF_SDRAMCTL_V;EMIFSDRAMcontrol.globalEMIF_SDRAMTIM_V;SDRAMtiming(refresh).globalEMIF_SDRAMEXT_V;SDRAMextendedcontrol.endif;ifBOOT_C671X_isnotdefined


下面的代码段名.boot_load，你将在之后的cmd文件中看到它。


代码首先对EMIF进行初始化，然后在copy_section_top中读取用户程序的段信息（段的Flash加载地址，段的RAM运行地址以及段的长度），在copy_loop中执行循环拷贝操作。

;A;;========File:boot_c671x.s62========;.title"Flash bootup utility";globalEMIFsymbolsdefinedforthec671xfamily.includeboot_c671x.h62;Addressofthegeneratedboot-tableuser_size.equuser_ld_start.equuser_rn_start.equ.sect".boot_load".global_boot.ref_c_int00_boot:;************************************************************************;*DEBUGLOOP-COMMENTOUTBFORNORMALOPERATION;************************************************************************zeroB1_myloop:;[!B1]B_myloopnop_myloopend:nop;************************************************************************;*CONFIGUREEMIF;************************************************************************;****************************************************************;*EMIF_GCTL=EMIF_GCTL_V;;****************************************************************mvklEMIF_GCTL,A4||mvklEMIF_GCTL_V,B4mvkhEMIF_GCTL,A4||mvkhEMIF_GCTL_V,B4stwB4,*A4;****************************************************************;*EMIF_CE0=EMIF_CE0_V;****************************************************************mvklEMIF_CE0,A4||mvklEMIF_CE0_V,B4mvkhEMIF_CE0,A4||mvkhEMIF_CE0_V,B4stwB4,*A4;****************************************************************;*EMIF_CE1=EMIF_CE1_V(setupfor-bitasync);****************************************************************mvklEMIF_CE1,A4||mvklEMIF_CE1_V,B4mvkhEMIF_CE1,A4||mvkhEMIF_CE1_V,B4stwB4,*A4;****************************************************************;*EMIF_CE2=EMIF_CE2_V(setupfor-bitasync);****************************************************************mvklEMIF_CE2,A4||mvklEMIF_CE2_V,B4mvkhEMIF_CE2,A4||mvkhEMIF_CE2_V,B4stwB4,*A4;****************************************************************;*EMIF_CE3=EMIF_CE3_V(setupfor-bitasync);****************************************************************||mvklEMIF_CE3,A4||mvklEMIF_CE3_V,B4;mvkhEMIF_CE3,A4||mvkhEMIF_CE3_V,B4stwB4,*A4;****************************************************************;*EMIF_SDRAMCTL=EMIF_SDRAMCTL_V;****************************************************************||mvklEMIF_SDRAMCTL,A4||mvklEMIF_SDRAMCTL_V,B4;mvkhEMIF_SDRAMCTL,A4||mvkhEMIF_SDRAMCTL_V,B4stwB4,*A4;****************************************************************;*EMIF_SDRAMTIM=EMIF_SDRAMTIM_V;****************************************************************||mvklEMIF_SDRAMTIM,A4||mvklEMIF_SDRAMTIM_V,B4;mvkhEMIF_SDRAMTIM,A4||mvkhEMIF_SDRAMTIM_V,B4stwB4,*A4;****************************************************************;*EMIF_SDRAMEXT=EMIF_SDRAMEXT_V;****************************************************************||mvklEMIF_SDRAMEXT,A4||mvklEMIF_SDRAMEXT_V,B4;mvkhEMIF_SDRAMEXT,A4||mvkhEMIF_SDRAMEXT_V,B4stwB4,*A4;****************************************************************************;copysections;****************************************************************************mvklcopyTable,a3;loadtablepointermvkhcopyTable,a3;ldw*a3++,b1;Loadentrypointcopy_section_top:ldw*a3++,b0;bytecountldw*a3++,b4;loadflashstart(load)addressldw*a3++,a4;ramstartaddressnop[!b0]bcopy_done;havewecopiedallsections?nopcopy_loop:ldb*b4++,b5;fetchfromflashsubb0,,b0;decrementcounter[b0]bcopy_loop;setupbranchifnotdone[!b0]bcopy_section_topzeroa1[!b0]and,a3,a1stbb5,*a4++;storetoram[!b0]and-,a3,a5[a1]and,a5,a3;****************************************************************************;jumptoentrypoint;****************************************************************************copy_done:mvkl.S2_c_int00,b0mvkh.S2_c_int00,b0b.S2b0nopcopyTable:;count;flashstart(load)address;ramstart(run)address;;.text.worduser_size.worduser_ld_start.worduser_rn_start;;endoftable.word.word.word


在使用时，我们要对上面程序中的

user_size.equuser_ld_start.equuser_rn_start.equ


进行修改，user_size表示用户程序段的字节大小，我们将在下一节看到可以通过查看*.map文件进行修改；user_ld_start表示用户代码的Flash起始地址（我默认使用0x90000400，一般不改），user_rn_start表示用户代码要存放到RAM的起始地址（从之前的图看，这个我也一般不改）。小程序我一般只修改用户程序段的字节大小。大程序可能要对copyTable（复制表）进行调整。


要满足上面的地址的分布，修改用户应用程序的cmd文件如下：

```
-c
-x
-l rts6700.lib
-heap  100h
-stack 200h
MEMORY
{
    BOOT_RAM   : o=00000000h,l=00000400h
    IRAM       : o=00000400h,l=00040000h
    FLASH_BOOT : o=90000000h,l=00000400h
    FLASH_REST : o=90000400h,l=000FFB00h
}
SECTIONS
{
      .boot_load:> BOOT_RAM

      /* Initialized User code section */
      .text     :> IRAM
      .cinit    :> IRAM

      .vectors  :> IRAM
      .bss      :> IRAM
      .far      :> IRAM
      .stack    :> IRAM
      .const    :> IRAM
      .switch   :> IRAM
      .sysmem   :> IRAM
      .cio      :> IRAM   
}
```


注意其中的.boot_load段，与二级引导程序的.sect ".boot_load"对应。如果用户应用程序定义了其它的段，可对cmd文件做相应修改，但.boot_load:> BOOT_RAM不能改，且不要把其它段放在BOOT_RAM存储区中。


将以上3个汇编文件盒一个cmd文件加到用户程序的工程中重新编译工程。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md#3-%E6%8F%90%E5%8F%96%E8%A6%81%E7%83%A7%E5%86%99%E7%9A%84%E4%BA%8C%E8%BF%9B%E5%88%B6%E6%95%B0%E6%8D%AE)3 提取要烧写的二进制数据


这部分是纯手工活，如果你会使用VIM，那数据处理起来就简单多了。


首先，将Jtag连接上TMS320C6713开发板，下载用户应用程序，使用CCS V3.3的File->Data->Save...功能，将内存中的二进制的代码数据保存到.dat文件。

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/store_dat.png)


Address都是上面的cmd文件设定好的。


要保存的*.dat包括两个文件：一个存放二级Bootloader的机器二进制码（boot.dat），一个存放用户应用程序的二进制码（text.dat）
- boot.dat: Address=0x00000000, Length=0x00000100
- text.dat: Address=0x00000400, Length=?


喔？用户代码的长度怎么知道？还有，不是说一级Bootloader会拷贝1KB长度吗，上面怎么是Length=0x000001000


请到CCS工程的Debug目录下打开.map文件（如下图），详细的解答在下图的注释中。

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/map.png)


上面图中计算烧写长度时/4就是因为CCS中保存*.dat长度的单位为4字节，但要注意，程序的大小可能并不一定是4字节的整数倍，因此在除以4时，“宁可多烧，也不少一个字节”，使用向上取整的运算。

```
Length = ceil( Length(Used in *.map) / 4) );
```


好了？No，别忘了，前一节中我们没办法设定User Code的长度，回头去改，

user_size.equ(这里改为上图中的)user_ld_start.equuser_rn_start.equ


改完后重新编译应用程序的工程，这就好了。给大家看看保存后的boot.dat文件，

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/boot_dat.png)


除了第一行，每行都是一个4字节长度的数。下面要做得是，分别把boot.dat中这些数和text.dat中的这些数放到boot[]和text[]的数组中，并将数组保存在头文件中。


好了，不用讲了，大家各显本事做数据的格式化处理吧，我用我的VIM编辑器，轻松搞定：
- 首先删除头行
- vim命令添加逗号:%s/$/,/g
- 添加数组名


搞定后的结果，如boot.h文件和text.h文件如下：

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/booth.png)![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/texth.png)


好了，到这就差不多了，表示机器码的二进制如今保存在我们的C语言头文件中了，下面就是要把头文件数组中的机器码烧写到Flash的对应的地址空间。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md#4-%E7%83%A7%E5%86%99flash)4 烧写Flash


本文操作环境下使用的Flash型号是AM29LV800BT。


为了烧写，首先你要做的是重新新建一个用于Flash烧写的工程。


烧写Flash的程序网上也有很多讲解，今天把烧写Flash调了出来，就当给大家福利，把我的Flash驱动程序给出来，

/* * FileName : FLASH.h * Author   : xiahouzuoxin * Date     : 2013.09.28 * Version  : v1.0 * Brief    :   */#ifndef _FLASH_H#define _FLASH_H#include "Config.h"#define     FLASH_UL1           0xAA#define     FLASH_UL2           0x55#define     FLASH_UL3           0x80#define     FLASH_UL4           0xAA#define     FLASH_UL5           0x55#define     FLASH_SECTOR_UL6    0x30#define     FLASH_CHIP_UL6      0x10#define     FLASH_PROGRAM       0xA0#define     SECTOR_SIZE         0x0800#define     BLOCK_SIZE          0x8000#define     CHIP_SIZE           0x40000#define     FLASH_FIRST_ADDR    0x90000000#define     FLASH_OFFSET(addr)  (FLASH_FIRST_ADDR+(addr)<<1)  // 16 bit boot mode/* External functions propotype */externuint32_tFlash_Erase(uint32_taddr,uint16_ttype);externvoidFlash_Readm(uint32_taddr,uint16_t*ptr,uint32_tlength);externuint32_tFlash_Reads(uint32_taddr);externvoidFlash_Writem(uint32_taddr,uint16_t*ptr,uint32_tlength);externvoidFlash_Writes(uint32_taddr,uint16_tdata);#endif

/* * FileName : FLASH.c * Author   : xiahouzuoxin * Date     : 2013.09.28 * Version  : v1.0 * Brief    :   */#include "FLASH.h"#include "stdio.h"/* Global vaiables */volatileuint16_t*FLASH_5555=(volatileuint16_t*)(0x90000000+(0x5555<<));volatileuint16_t*FLASH_2AAA=(volatileuint16_t*)(0x90000000+(0x2AAA<<));/*  * @brief  Flash erase function. * @param  addr:  *         type: * @retval  */uint32_tFlash_Erase(uint32_taddr,uint16_ttype){uint32_ti,j;*FLASH_5555=FLASH_UL1;//first*FLASH_2AAA=FLASH_UL2;//second*FLASH_5555=FLASH_UL3;//third*FLASH_5555=FLASH_UL4;*FLASH_2AAA=FLASH_UL5;switch(type){case0x50:      //block erase*(uint16_t*)addr=type;while((*(uint16_t*)addr&0x80)!=0x80);for(i=;i<BLOCK_SIZE;i++){if(*(uint16_t*)(addr+i)!=0xffff){j=;break;}}j=;break;case0x30:      //sector erase*(uint16_t*)addr=type;while((*(uint16_t*)addr&0x80)!=0x80);break;case0x10:      //chip erase*FLASH_5555=type;while((*FLASH_5555&0x80)!=0x80);break;default:break;}return(j);}/*  * @brief  Write a single data. * @param  addr:  *         data: * @retval  */voidFlash_Writes(uint32_taddr,uint16_tdata){uint32_tj=;*FLASH_5555=FLASH_UL1;*FLASH_2AAA=FLASH_UL2;*FLASH_5555=FLASH_PROGRAM;*(uint16_t*)addr=data;j=;while(j<)j++;// with delaywhile(*(uint16_t*)addr!=data);// 校验}/*  * @brief  Write the certain length data. * @param  addr:  *         ptr: *         length: * @retval  */voidFlash_Writem(uint32_taddr,uint16_t*ptr,uint32_tlength){uint32_ti;for(i=;i<length;i++){Flash_Writes(addr+(i<<),*(ptr+i));if(i%0xFF==){printf("\nWrite %d bytes...",i<<);}}}/*  * @brief  Read a single data. * @param  addr:  * @retval  */uint32_tFlash_Reads(uint32_taddr){return(*(uint16_t*)addr);}/*  * @brief  Read the certain length data. * @param  addr:  *         ptr: *         length: * @retval  */voidFlash_Readm(uint32_taddr,uint16_t*ptr,uint32_tlength){uint32_ti;for(i=;i<length;i++){*(ptr+i)=Flash_Reads(addr+*i);}}


我使用的Flash_Writem函数按每次16位（2字节）烧写，主程序main中的烧写代码是

#include <c6x.h>#include <csl.h>#include <stdio.h>#include <stdlib.h>#include "Flash.h"#include "BSP.h"#include "boot.h"#include "text.h"main(){/* Init PLL EMIF ... */BSP_init();/* Fetch data from file *//* Erase flash memory. */Flash_Erase(0x90000000,0x10);printf("\nErase Flash ok.");/* Write flash memory. */Flash_Writem(0x90000000,(uint16_t*)boot,sizeof(boot)<<);printf("\nWrite .boot ok.");Flash_Writem(0x90000000+(0x200<<),(uint16_t*)text,sizeof(text)<<);printf("\nWrite .text ok.");printf("\nBurn to flash ok.");}


注意上面的Flash_Writem调用格式，二级boot_loader被烧写到Flash起始地址为0x90000000的地址空间，用户应用程序被烧写到Flash起始地址为0x90000400的地址空间。


烧写成功，请看下文，

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/Burn.png)


更多烧写内容请参考[2]，里面是我烧写程序用的CCS工程，烧写不同的应用程序只需要替换里面的text.h和boot.h即可。


昨天烧写小段代码测试成功，今天尝试用该方法烧写大段代码，除了改变在2 Level Bootloader的程序中改变use_code_Size外，无任何其它改变，成功烧写300KB以上的代码（在SST39VF1601型号的Flash上做得测试），如下：

![](https://github.com/xiahouzuoxin/notes/raw/master/images/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/test2.png)
- 后注：烧写Flash要注意以上的任何一个细节，某个细节出错都可能造成烧写不成功！在应用程序中切忌对Flash进行擦写操作，莫要覆盖了原保存了烧写程序的Flash区域。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md#5-%E7%83%A7%E5%86%99%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E4%B8%AD%E6%96%AD%E5%90%91%E9%87%8F%E8%A1%A8)5 烧写过程中的中断向量表


就前面的烧写方法中，请思考一个问题，程序是从何时何处跳转到main函数执行的？


如果不烧写Flash，我们都应该知道是在vecs.asm中

```
********************************************************************************
*           Copyright (C) 2003 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... vecs.asm
* DATE CREATED.. 12/06/2000
* LAST MODIFIED. 03/05/2003
********************************************************************************

*------------------------------------------------------------------------------
* Global symbols defined here and exported out of this file
*------------------------------------------------------------------------------
   .global _vectors
   .global _c_int00
   .global _vector1
   .global _vector2
   .global _vector3
   .global _vector4 
   .global _vector5 
   .global _vector6
   .global _vector7
   .global _vector8
   .global _vector9   
   .global _vector10 
   .global _vector11   
   .global _vector12  
   .global _vector13
   .global _vector14   
   .global _vector15   

*------------------------------------------------------------------------------
* Global symbols referenced in this file but defined somewhere else. 
* Remember that your interrupt service routines need to be referenced here.
*------------------------------------------------------------------------------
   .ref _c_int00

*------------------------------------------------------------------------------
* This is a macro that instantiates one entry in the interrupt service table.
*------------------------------------------------------------------------------
VEC_ENTRY .macro addr
    STW   B0,*--B15
    MVKL  addr,B0
    MVKH  addr,B0
    B     B0
    LDW   *B15++,B0
    NOP   2
    NOP   
    NOP   
   .endm


*------------------------------------------------------------------------------
* This is a dummy interrupt service routine used to initialize the IST.
*------------------------------------------------------------------------------
_vec_dummy:
  B    B3
  NOP  5

*------------------------------------------------------------------------------
* This is the actual interrupt service table (IST). It is properly aligned and
* is located in the subsection .text:vecs. This means if you don't explicitly
* specify this section in your linker command file, it will default and link
* into the .text section. Remember to set the ISTP register to point to this
* table.
*------------------------------------------------------------------------------
 ;.sect ".text:vecs"
 .sect ".vectors"   ;
 .align 1024

_vectors:
_vector0:   VEC_ENTRY _c_int00    ;RESET
_vector1:   VEC_ENTRY _vec_dummy  ;NMI
_vector2:   VEC_ENTRY _vec_dummy  ;RSVD
_vector3:   VEC_ENTRY _vec_dummy
_vector4:   VEC_ENTRY _vec_dummy
_vector5:   VEC_ENTRY _vec_dummy
_vector6:   VEC_ENTRY _vec_dummy
_vector7:   VEC_ENTRY _vec_dummy
_vector8:   VEC_ENTRY _vec_dummy    
_vector9:   VEC_ENTRY _vec_dummy
_vector10:  VEC_ENTRY _vec_dummy
_vector11:  VEC_ENTRY _vec_dummy
_vector12:  VEC_ENTRY _vec_dummy
_vector13:  VEC_ENTRY _vec_dummy
_vector14:  VEC_ENTRY _vec_dummy
_vector15:  VEC_ENTRY _vec_dummy

*------------------------------------------------------------------------------


********************************************************************************
* End of vecs.asm
********************************************************************************
```


_vector为中断向量表的首地址标识符，系统复位后默认转入执行复位向量（复位向量始终保存在RAM的0地址处，这也就是为什么之前提到系统硬件复位后从0地址开始执行）。


不烧写Flash，只要在cmd文件中将.vectors段设定在0地址处，然后调用c_int00，跳转到main函数执行。


使用上面的方法烧写Flash，则是在copy table完成之后调用c_int00。两者跳转到main函数的机理是一样的。


但是在烧写Flash的时候，要注意的一个问题就是：中断向量表存放在哪里？


前面烧写Flash的时候，其实有一点没有提到：当系统调用（比如定时器中断），如何才能找到（定时器）中断向量的入口函数？


因此，前面烧写Flash的方法在不做修改的情况下是无法执行中断服务程序的。


修改方法有2，且听一一分解。

#### [](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md#%E6%96%B9%E6%B3%95%E4%B8%80)方法一


在进入main函数之后，重定位中断向量表的位置（关于向量表的重定位参考[DSP TMS320C6000基础学习（7）—— Bootloader与VectorTable](http://blog.csdn.net/xiahouzuoxin/article/details/9713461)）


中断向量表的重定位必须在使用中断之前。

```
extern far void vectors();   /* 声明vectors，因为_vectors定义在汇编文件vecs.asm中 */

IRQ_setVecs(vectors);   /* 重定位中断向量表 */
```

### [](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md#%E6%96%B9%E6%B3%95%E4%BA%8C)方法二


修改汇编文件和cmd文件。基本思路是：把中断向量表保存在0地址处，在向量表之后存储二级Bootloader，通过复位中断跳转到二级Bootloader。
- 先修改cmd文件

```
-c
-x
-l rts6700.lib
-heap  100h
-stack 200h
MEMORY
{
    BOOT_RAM   : o=00000000h,l=00000400h
    IRAM       : o=00000400h,l=00040000h
    FLASH_BOOT : o=90000000h,l=00000400h
    FLASH_REST : o=90000400h,l=000FFB00h
}
SECTIONS
{
      .vectors  :> BOOT_RAM    /* 修改在这里，vectors段定义在vecs.asm中，中断向量表放在RAM 0地址处 */
      .boot_load:> BOOT_RAM

      /* Initialized User code section */
      .text     :> IRAM
      .cinit    :> IRAM

      .vectors  :> IRAM
      .bss      :> IRAM
      .far      :> IRAM
      .stack    :> IRAM
      .const    :> IRAM
      .switch   :> IRAM
      .sysmem   :> IRAM
      .cio      :> IRAM   
}
```
- 修改vecs.asm（只给出了修改部分）

```
...

   .ref _c_int00
   .ref _boot   ; 修改在这里，_boot段为二次引导程序入口，定义在boot_c671x.s62中

...

 .sect ".vectors"   ;
 .align 1024

_vectors:
_vector0:   VEC_ENTRY _boot       ;RESET   修改在这里(将_c_init改成了_boot)，复位后跳转到_boot执行二次引导程序
_vector1:   VEC_ENTRY _vec_dummy  ;NMI
_vector2:   VEC_ENTRY _vec_dummy  ;RSVD
_vector3:   VEC_ENTRY _vec_dummy
_vector4:   VEC_ENTRY _vec_dummy
_vector5:   VEC_ENTRY _vec_dummy
_vector6:   VEC_ENTRY _vec_dummy
_vector7:   VEC_ENTRY _vec_dummy
_vector8:   VEC_ENTRY _vec_dummy    
_vector9:   VEC_ENTRY _vec_dummy
_vector10:  VEC_ENTRY _vec_dummy
_vector11:  VEC_ENTRY _vec_dummy
_vector12:  VEC_ENTRY _vec_dummy
_vector13:  VEC_ENTRY _vec_dummy
_vector14:  VEC_ENTRY _vec_dummy
_vector15:  VEC_ENTRY _vec_dummy
```


两种方法都做过测试，都是可行的！

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md#6-%E5%8F%82%E8%80%83)6 参考


[1] Creating a Second-Level Bootloader for FLASH Bootloading on TMS320C6000 Platform With Code Composer Studio


[2] [My Flash burn CCS project](https://github.com/xiahouzuoxin/notes/blob/master/codes/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/burn_flash)

[](https://github.com/xiahouzuoxin/notes/blob/dev/codes/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95/burn_flash)



