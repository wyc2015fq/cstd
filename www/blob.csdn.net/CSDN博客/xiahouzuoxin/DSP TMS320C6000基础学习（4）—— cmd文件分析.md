# DSP TMS320C6000基础学习（4）—— cmd文件分析 - xiahouzuoxin - CSDN博客





2013年07月26日 17:19:02[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：14598







DSP中的CMD文件是链接命令文件（Linker Command File），以.cmd为后缀。



在分析cmd文件之前，必需先了解

（1）DSP具体芯片的内存映射（Memory Map）

（2）知道点链接的知识，知道C程序中段的概念

（3）知道RAM,Flash等存储模块的区别



======================================================================
***1. coff目标文件***

======================================================================

coff是一种流行的二进制可执行文件格式，在CCS v5中扩展名为.out，可以直接

下载到芯片中。可执行文件包括段头、可执行代码、初始化数据、可重定位信息

和符号表字符串表等信息。

编译器处理段的过程为：

（1）把每个源文件都编译成独立目标文件(.obj)，每个目标文件都有自己的段

（2）链接器将目标文件中相同段名的部分连接在一起，生成最终的coff可执行文件

CCS v5中的Compile Files完成功能（1），Build完成功能（2）。



======================================================================
***2. TMS320C6713内存映射***

======================================================================

![](https://img-blog.csdn.net/20130726171607468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20130726171635468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



======================================================================
***3. 自定义代码段和数据段***

======================================================================

// 将symbol分配到section name指示的数据段

#pragma DATA_SECTION(symbol, "section name");

// 将symbol分配到section name指示的代码段

#pragma CODE_SECTION(symbol, "section name");  

常常结合结构体定义symbol，如下，

volatile struct Symbol symbol;  // Symbol预先定义好的描述特定外设的结构



比如，对于C6713中的Timer0外设，做如下定义，

```cpp
struct Timer0 {
    ...
}
#pragma DATA_SECTION(C6713_Timer0, "C6713_Timer0_cmd");
volatile struct Timer0 C6713_Timer0;
```


"C6713_Timer0_cmd"将在cmd文件中分配空间。




======================================================================
***4. cmd文件***

======================================================================

cmd文件主要用于完成链接的功能，因此可以在cmd文件中使用链接命令，比如：




-stack 0x200    设置栈大小为0x200字节

-heap 0x200     设置堆大小为0x200字节

-l rst67xx.lib  链接rst67xx.lib库




除了链接命令外，cmd 文件还包括MEMORY和SECTOINS两部分，分别用于存储区的划分和段的分配。

MEMORY划分的格式为：



```cpp
L2SRAM    : o = 00000000h l = 00030000h  /* L2 SRAM 192K */
```
o表示起始地址，l表示存储区长度（以字节为单位）




一个简单的例子（TMS320C6713为例，不同芯片不同），外设只添加了Timer0:

```cpp
MEMORY
{
	L2SRAM    : o = 00000000h l = 00030000h  /* L2 SRAM 192K */
	L2CACHE   : o = 00030000h l = 00010000h  /* L2 Cache 64 K */


	/* Peripheral */
	CPU_TIMER0 : o = 01940000h l = 00040000 /* Timer0 */


	EXTERNAL  : o = 80000000h l = 80010000h
}


SECTIONS
{
	/* Allocate program areas */
	.text     > L2SRAM        /* code segment */
	.cinit    > L2SRAM        /* init segment */


	/* Allocate data areas */
	.stack    > L2SRAM
	.far      > L2SRAM
	.switch   > L2SRAM        /* C switch table */
	.tables   > L2SRAM
	.data     > L2SRAM        /* data segment */
	.bss      > L2SRAM        /* data that haven't init */
	.sysmem   > L2SRAM
	.const    > L2SRAM        /* string, const ... */
	.cio      > L2SRAM


	.buffers  > EXTERNAL


	C6713_Timer0_cmd > CPU_TIMER0  /* Timer 0 */
}
```


cmd文件包括2部分 —— MEMORY与SECTIONS

MEMORY完成地址空间的划分；

SECTIONS完成地址空间的分配到具体用途（除了程序中通用段之外还可以有自定义段）。



NOTES: 平时开发时都是将程序下载到RAM空间，当要发布时需要下载到Flash空间，

此处为SRAM的cmd文件，Flash的cmd文件有所不同。





下面是TI公司DSK的cmd，可以直接参考，



```cpp
/****************************************************************************/
/*  C6713.cmd                                                               */
/*  Copyright (c) 2010 Texas Instruments Incorporated                       */
/*																			*/
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an TMS320C6713        */
/*                 device.  Use it as a guideline.  You will want to        */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

-stack 0x2000
-heap 0x8000

MEMORY
{
	IRAM		o = 0x00000000	l = 0x00030000	/* 192kB - Internal RAM */
	L2RAM		o = 0x00030000	l = 0x00010000	/* 64kB - Internal RAM/CACHE */
	EMIFCE0		o = 0x80000000	l = 0x10000000	/* SDRAM in 6713 DSK */
	EMIFCE1		o = 0x90000000	l = 0x10000000	/* Flash/CPLD in 6713 DSK */
	EMIFCE2		o = 0xA0000000	l = 0x10000000	/* Daughterboard in 6713 DSK */
	EMIFCE3		o = 0xB0000000	l = 0x10000000	/* Daughterboard in 6713 DSK */
}

SECTIONS
{
	.text          >  IRAM
	.stack         >  IRAM
	.bss           >  IRAM
	.cio           >  IRAM
	.const         >  IRAM
	.data          >  IRAM
	.switch        >  IRAM
	.sysmem        >  IRAM
	.far           >  IRAM
  .args          >  IRAM
	.ppinfo        >  IRAM
	.ppdata        >  IRAM

  /* COFF sections */
	.pinit         >  IRAM
	.cinit         >  IRAM

  /* EABI sections */
  .binit         >  IRAM
	.init_array    >  IRAM
  .neardata      >  IRAM
	.fardata       >  IRAM
	.rodata        >  IRAM
	.c6xabi.exidx  >  IRAM
	.c6xabi.extab  >  IRAM
}
```





下面再给出一个TMS320F2818的完整cmd文件例子，与6713有所不同，比如16进制格式

表示，MEMORY和SECTIONS书写等。

```cpp
MEMORY
{
PAGE 0 : 
   PRAMH0     : origin = 0x3f8000, length = 0x001000       
         
PAGE 1 : 
   /* SARAM                     */     
   RAMM0    : origin = 0x000000, length = 0x000400
   RAMM1    : origin = 0x000400, length = 0x000400


   /* Peripheral Frame 0:   */
   DEV_EMU    : origin = 0x000880, length = 0x000180
   FLASH_REGS : origin = 0x000A80, length = 0x000060
   CSM        : origin = 0x000AE0, length = 0x000010
   XINTF      : origin = 0x000B20, length = 0x000020
   CPU_TIMER0 : origin = 0x000C00, length = 0x000008
   CPU_TIMER1 : origin = 0x000C08, length = 0x000008		 
   CPU_TIMER2 : origin = 0x000C10, length = 0x000008		 
   PIE_CTRL   : origin = 0x000CE0, length = 0x000020
   PIE_VECT   : origin = 0x000D00, length = 0x000100


   /* Peripheral Frame 1:   */
   ECAN_A     : origin = 0x006000, length = 0x000100
   ECAN_AMBOX : origin = 0x006100, length = 0x000100


   /* Peripheral Frame 2:   */
   SYSTEM     : origin = 0x007010, length = 0x000020
   SPI_A      : origin = 0x007040, length = 0x000010
   SCI_A      : origin = 0x007050, length = 0x000010
   XINTRUPT   : origin = 0x007070, length = 0x000010
   GPIOMUX    : origin = 0x0070C0, length = 0x000020
   GPIODAT    : origin = 0x0070E0, length = 0x000020
   ADC        : origin = 0x007100, length = 0x000020
   EV_A       : origin = 0x007400, length = 0x000040
   EV_B       : origin = 0x007500, length = 0x000040
   SPI_B      : origin = 0x007740, length = 0x000010
   SCI_B      : origin = 0x007750, length = 0x000010
   MCBSP_A    : origin = 0x007800, length = 0x000040


   /* CSM Password Locations */
   CSM_PWL    : origin = 0x3F7FF8, length = 0x000008


   /* SARAM                    */     
   DRAMH0     : origin = 0x3f9000, length = 0x001000         
}
 
SECTIONS
{
   /* Allocate program areas: */
   .reset           : > PRAMH0,      PAGE = 0
   .text            : > PRAMH0,      PAGE = 0
   .cinit           : > PRAMH0,      PAGE = 0


   /* Allocate data areas: */
   .stack           : > RAMM1,       PAGE = 1
   .bss             : > DRAMH0,      PAGE = 1
   .ebss            : > DRAMH0,      PAGE = 1
   .const           : > DRAMH0,      PAGE = 1
   .econst          : > DRAMH0,      PAGE = 1      
   .sysmem          : > DRAMH0,      PAGE = 1
   
   /* Allocate Peripheral Frame 0 Register Structures:   */
   DevEmuRegsFile    : > DEV_EMU,    PAGE = 1
   FlashRegsFile     : > FLASH_REGS, PAGE = 1
   CsmRegsFile       : > CSM,        PAGE = 1
   XintfRegsFile     : > XINTF,      PAGE = 1
   CpuTimer0RegsFile : > CPU_TIMER0, PAGE = 1      
   CpuTimer1RegsFile : > CPU_TIMER1, PAGE = 1      
   CpuTimer2RegsFile : > CPU_TIMER2, PAGE = 1      
   PieCtrlRegsFile   : > PIE_CTRL,   PAGE = 1      
   PieVectTable      : > PIE_VECT,   PAGE = 1


   /* Allocate Peripheral Frame 2 Register Structures:   */
   ECanaRegsFile     : > ECAN_A,      PAGE = 1   
   ECanaMboxesFile   : > ECAN_AMBOX   PAGE = 1


   /* Allocate Peripheral Frame 1 Register Structures:   */
   SysCtrlRegsFile   : > SYSTEM,     PAGE = 1
   SpiaRegsFile      : > SPI_A,      PAGE = 1
   SciaRegsFile      : > SCI_A,      PAGE = 1
   XIntruptRegsFile  : > XINTRUPT,   PAGE = 1
   GpioMuxRegsFile   : > GPIOMUX,    PAGE = 1
   GpioDataRegsFile  : > GPIODAT     PAGE = 1
   AdcRegsFile       : > ADC,        PAGE = 1
   EvaRegsFile       : > EV_A,       PAGE = 1
   EvbRegsFile       : > EV_B,       PAGE = 1
   ScibRegsFile      : > SCI_B,      PAGE = 1
   McbspaRegsFile    : > MCBSP_A,    PAGE = 1


   /* CSM Password Locations */
   CsmPwlFile      : > CSM_PWL,     PAGE = 1
}
```




