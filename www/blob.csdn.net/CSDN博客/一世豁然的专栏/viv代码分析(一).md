# viv代码分析(一) - 一世豁然的专栏 - CSDN博客





2015年06月14日 08:40:59[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1087标签：[vivi																[bootloader](https://so.csdn.net/so/search/s.do?q=bootloader&t=blog)](https://so.csdn.net/so/search/s.do?q=vivi&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)









本博客转载于：[http://www.cnblogs.com/tureno/articles/4445393.html](http://www.cnblogs.com/tureno/articles/4445393.html)





通过vivi研究bootloader有一段时间了，基本是在与之相关的基础方面做工作，还没有真正深入研究vivi。以后的学习重心就要放到研究vivi源代码上面了。我想，真正细致地弄清楚vivi实现的细节，对C语言水平的提高，对ARM体系结构的认识，对S3C2410的熟悉，对嵌入式bootloader相关技术，都能有很大的好处。学习的进度会慢一些，但是务求深入，并且打好相关的基础。



一、写在前面的话



    嵌入式系统软件开发主要包括五个方面：bootloader编写（移植）、驱动程序编写（移植）、操作系统裁减（移植）、文件系统制作、应用程序编写（移植）。嵌入式开发流程我已经熟悉，但是仅限于完成最为基本的工作，大部分是借助网络资料，自己独立解决的问题很有限。学习嵌入式系统已经一年了，算是入门了。然而，入门之后如何继续深入学习嵌入式系统开发，如何提高自身的能力？



    我想，这也许是独立摸索的学习者都会遇到的问题吧。思考之后有所得，核心就是一句话：务实，理论与实践结合！具体说来，就是要不断的认识自己，去了解自己最适合做什么。这是最为重要的，如果不知道做什么，就无法安排学习的重点。嵌入式开发的领域太广，要想在方方面面都深入不太容易（当然牛人除外）。现在对自己的认识如下：本科有硬件、通信背景，但是没有太多机会进行硬件设计。而硬件设计最为重要的就是经验，动手能力，所以不打算把硬件设计作为学习的重点。底层软件开发既需要对硬件熟悉，又需要软件设计能力，正适合我。所以以后的学习，以底层软件开发（bootloader设计、驱动程序设计）为重点，同时也要加强硬件学习。学习有重点，但是嵌入式开发的其他领域也要涉及，了解广博才能更有助于设计。进展慢不要紧，关键是要深入，深入，再深入。真正地去理解这些技术，并且能够熟练的应用。这半年的核心就是bootloader技术研究，打算先看vivi，然后看uboot。手头上的板子有s3c2410、at91rm9200，这些都可以拿来训练，争取能够通过bootloader技术的掌握，同时熟悉了ARM体系结构、ARM汇编、开发工具等等一系列相关内容，总结学习的方法，提高学习能力。



二、准备工作



    在分析vivi源代码的时候，不打算完全按照vivi的代码来进行。我的思路是，以从nand flash启动为主线，分析从上电到引导内核的过程。以自己的理解去实现vivi的源代码，要自己手动编写，即使与vivi的代码相同。只有这样，才能从整体上理解vivi的设计方法，理解vivi各个功能的实现细节。这份文档算是自己的学习笔记。



三、bootloader stage1：【arch/s3c2410/head.S】



    首先解决一个问题，就是为什么使用head.S而不是用head.s？有了GNU AS和GNU GCC的基础，不难理解主要原因就是为了使用C预处理器的**宏替换**和**文件包含**功能（GNU AS的预处理无法完成此项功能）。可以参考前面的总结部分。这样的好处就是可以使用C预处理器的功能来提高ARM汇编的程序设计环境，更加方便。但是因为ARM汇编和C在宏替换的细节上有所不同，为了区分，引入了__ASSEMBLY__这个变量，这是通过Makefile中AFLAGS来引入的（一般在顶层Makefile中定义），具体如下：



|`AFLAGS:=-D__ASSEMBLY__ $(CPPFLAGS)`|
|----|




    在后面的头文件中，会看到很多#ifdef __ASSEMBLY__等的操作，就是用来区分这个细节的。在编译汇编文件时，加入AFLAGS选项，所以__ASSEMBLY__传入，也就是定义了__ASSEMBLY__；在编译C文件时，没有用AFLAGS选项，自然也就没有定义__ASSEMBLY__。由此相应的问题就比较清晰了。这个小技巧也是值得学习和借鉴的。



1 首先关注一下开始的三个头文件。



|```#include"config.h"#include "linkage.h"#include "machine.h"```|
|----|




（1）利用source insight来查看【include/config.h】。



|```#ifndef _CONFIG_H_#define _CONFIG_H_#include "autoconf.h"#endif /* _CONFIG_H_ */```|
|----|




    可见，config.h只是包含一个autoconf.h。而关于autoconf.h的生成，在vivi配置文件分析的时候也解释的很清楚了，在这里就不用再细分析了。需要解释的一点是，如果写一个专用的bootloader，不采用vivi的配置机制，那么配置部分就没有这么复杂了，只需要在include文件夹中包含一个配置头文件即可。现在bootloader的设计有两种趋势，一种是针对特定应用，有特殊要求，也就是“专用”。那么设计时，不需要过多的配置，只需要简单的完成引导内核的功能就可以了。二是普通应用，一般是对基本“通用”的bootloader，比如uboot等，然后根据相应的模版进行移植。这就需要了解uboot等的架构，可以进行定制和功能的增加。uboot完成的不仅仅是一个bootloader的功能，还可以提供调试等功能，所以其角色还包含驻留程序这个功能，也就是uboot真正的角色是monitor。当然，可以不加区分，统称为bootloader。而分析vivi源代码的实现，对这两个方向都有帮助。



（2）【include/linkage.h】就是实现了ENTRY宏的封装。其实这个头文件也仅仅为head.S提供了服务，实际上没有必要写的这么复杂，可以简化一些。比如，我修改了这个头文件，如下：



|```[armlinux@lqminclude]$ cat linkage.h#ifndef _VIVI_LINKAGE_H#define _VIVI_LINKAGE_H#define SYMBOL_NAME(X) X#ifdef __STDC__        #define SYMBOL_NAME_LABEL(X) X##:#else        #define SYMBOL_NAME_LABEL(X) X/**/:#endif#ifdef __ASSEMBLY__#define ALIGN.align 0#define ENTRY(name) /        .globl SYMBOL_NAME(name); /        ALIGN; /        SYMBOL_NAME_LABEL(name)#endif#endif```|
|----|




    在这里，要加强一下C语言宏的设计和分析能力。下面就几个点简单的分析一下，后面专门就C宏部分做个总结。



    关于__STDC__这个宏，是编译器自动添加的，含义就是支持标准C。如果支持标准C，那么##的作用就是“连接”，所以SYMBOL_NAME_LABEL(_start)宏展开为_start:（依我的意思理解##也就相当于分号，表示后面可以继续连接其他的一些定义），如果不支持标准C，则利用了C预处理器对注释的处理方式，就是把/**/替换为一个空格（也就是相当于展开的字符后带个空格），可以测试一下。



    另外，关于ENTRY宏的封装，利用了GNU AS在ARM上的相关特点。首先，利用了分号作为三条语句的连接符(包括最后一个由##展开来的分号)，而分号是GNU AS汇编注释符的一种（另外一种是@）。另外，关于ALIGN为什么用.align 0。这可以参考GNU AS手册，上面讲解的比较清晰，主要是为了兼容ARM本身的编译器。理解了这个也就不难得出ENTRY(_start)宏展开后的形式了。有一个技巧就是可以通过下面的命令来检测宏展开后的结果，比如：



|```[root@lqm vivi_myboard]# gcc-E-D__ASSEMBLY__-I./include arch/s3c2410/head.S >aaa```|
|----|




可以查看aaa文件的显示结果，做了一些注释：



|```# 1"arch/s3c2410/head.S"# 1 "<built-in>"# 1 "<command line>"# 1 "arch/s3c2410/head.S"# 35 "arch/s3c2410/head.S"# 1 "include/config.h" 1# 14 "include/config.h"# 1 "include/autoconf.h" 1# 15 "include/config.h" 2# 36 "arch/s3c2410/head.S" 2# 1 "include/linkage.h" 1# 37 "arch/s3c2410/head.S" 2# 1 "include/machine.h" 1# 19 "include/machine.h"# 1 "include/platform/smdk2410.h" 1# 1 "include/s3c2410.h" 1# 22 "include/s3c2410.h"# 1 "include/hardware.h" 1# 23 "include/s3c2410.h" 2# 1 "include/bitfield.h" 1# 24 "include/s3c2410.h" 2# 3 "include/platform/smdk2410.h" 2# 1 "include/sizes.h" 1# 8 "include/platform/smdk2410.h" 2# 74 "include/platform/smdk2410.h"# 1 "include/architecture.h" 1# 75 "include/platform/smdk2410.h" 2# 20 "include/machine.h" 2# 38 "arch/s3c2410/head.S" 2@ Start of executable code宏定义展开.globl _start;.align 0; _start:.globl ResetEntryPoint;.align 0; ResetEntryPoint:下面是装载中断向量表，ARM规定，在起始必须有8条跳转指令，你可以用b 标号也可以用ldr pc 标号。这样的8条规则的标志被arm定义为bootloader的识别标志，检测到这样的标志后，就可以从该位置启动。这样的做法是因为开始的时候不一定有bootloader，必须有一种识别机制，如果识别到bootloader，那么就从bootloader启动。@@ Exception vector table(physical address= 0x00000000)@@ 0x00: Reset        b Reset@ 0x04: Undefined instruction exceptionUndefEntryPoint:        b HandleUndef@ 0x08: Software interrupt exceptionSWIEntryPoint:        b HandleSWI@ 0x0c: Prefetch Abort(Instruction Fetch MemoryAbort)PrefetchAbortEnteryPoint:        b HandlePrefetchAbort@ 0x10: Data Access Memory AbortDataAbortEntryPoint:        b HandleDataAbort@ 0x14: Not usedNotUsedEntryPoint:        b HandleNotUsed@ 0x18: IRQ(Interrupt Request)exceptionIRQEntryPoint:        b HandleIRQ@ 0x1c: FIQ(Fast Interrupt Request)exceptionFIQEntryPoint:        b HandleFIQ下面是固定位置存放环境变量@@ VIVI magics@@ 0x20: magic number so we can verify that we only put        .long 0@ 0x24:        .long 0@ 0x28: where this vivi was linked, so we can put it in memory in theright place        .long _start   //_start用来指定链接后的起始装载地址装载到内存中的地址@ 0x2C: this contains the platform, cpuand machine id        .long((1<< 24)|(6<< 16)| 193)@ 0x30: vivi capabilities        .long 0@ 0x34:        b SleepRamProc@@ Start VIVI head@Reset: //上电后cpu会从0x0地址读取指令执行，也就是b Reset        @ disable watch dog timer        mov r1, #0x53000000        mov r2, #0x0        str r2, [r1]# 121 "arch/s3c2410/head.S"        @ disable all interrupts        mov r1, #0x4A000000        mov r2, #0xffffffff        str r2, [r1,#0x08]  //0x4A000008为中断屏蔽寄存器，将里面赋全1，表示屏蔽这32个中断源        ldr r2, =0x7ff        str r2, [r1,#0x1C]  //0x4A00001C为中断子屏蔽寄存器，里面低11位也用来表示屏蔽掉这11个中断源        @ initialise system clocks        mov r1, #0x4C000000        mvn r2, #0xff000000//MVN指令可完成从另一个寄存器、被移位的寄存器、或将一个立即数加载到目的寄存器。与MOV指令不同之处是在传送之前按位被取反了，即把一个被取反的值传送到目的寄存器中。也就是r2=0x00ffffff        str r2, [r1, #0x00] //我们可以在程序开头启动MPLL，在设置MPLL的几个寄存器后，需要等待一段 时间(Lock Time)，  MPLL的输出才稳定。在这段时间(Lock Time)内，FCLK停振，CPU停止工作。Lock Time的长短由寄存器LOCKTIME设定，Lock Time之后，MPLL输出正常，CPU工作在新的FCLK下，**前面说过，MPLL启动后需要等待一段时间(Lock Time)，使得其输出稳定。位[23:12]用于UPLL，位[11:0]用于MPLL。本实验使用确省值0x00ffffff。 **        @ldr r2, mpll_50mhz        @str r2, [r1,#0x04]        @ 1:2:4        mov r1, #0x4C000000        mov r2, #0x3        str r2, [r1,#0x14] //0x4C000014为分频寄存器，用于设置FCLK、HCLK、PCLK三者的比例``````bit[2] — — HDIVN1，若为1，则 bit[1:0] 必 须 设 为 0b00 ， 此 时FCLK:HCLK:PCLK=1:1/4:1/4；若为0，三者比例由bit[1:0]确定 bit[1]——HDIVN，0：HCLK=FCLK；1：HCLK=FCLK/2 bit[0]——PDIVN，0：PCLK=HCLK；1：PCLK=HCLK/2  本实验设为0x03，则FCLK:HCLK:PCLK=1:1/2:1/4         mrc p15, 0, r1, c1, c0, 0 @read ctrlregister        orr r1, r1,#0xc0000000 @ Asynchronous        mcr p15, 0, r1, c1, c0, 0 @write ctrlregister````````上面这三句代码的意思是切换模式：If HDIVN = 1, the CPU bus mode has to be changed from thefast busmode to the asynchronous bus mode using following instructions：````````MMU_SetAsyncBusMode mrc  p15, 0, r0, c1, c0, 0 orr  r0, r0, #R1_nF:OR:R1_iA mcr  p15,0, r0, c1, c0, 0  其中的“R1_nF:OR:R1_iA”等于0xc0000000。意思就是说，当HDIVN = 1时，CPU bus mode需要从原来的“fast bus mode”改为“asynchronous bus mode”。 ```````````@ now, CPU clock is 200 Mhz        mov r1, #0x4C000000        ldr r2, mpll_200mhz        str r2, [r1,#0x04] //0x4C000004为MPLLCON寄存器， 对于MPLLCON寄存器，[19:12]为MDIV，[9:4]为PDIV，[1:0]为SDIV。有如下计算公式：  MPLL(FCLK) = (m * Fin)/(p * 2^s) 其中: m = MDIV + 8, p = PDIV + 2 对于本开发板，Fin =  12MHz,MPLLCON设为0x5c0040，可以计算出FCLK=200MHz，再由CLKDIVN的设置可知：HCLK=100MHz，PCLK=50MHz。# 164 "arch/s3c2410/head.S"       ** bl memsetup**        @ Check if this is a wake-up from sleep        ldr r1, PMST_ADDR  //0x560000B4为GSTATUS2寄存器，里面[0:2]三位有效        ldr r0, [r1] //将该寄存器中的值取出来保存到r0中        tst r0, #((1<< 1)) //测试r0的第一位。这位是Power_OFF reset. The reset after the wakeup from Power_OFF mode.The setting is cleared by writing "1" to this bit.TST指令用于把一个寄存器的内容和另一个寄存器的内容或立即数进行按位的与运算，并根据运算结果更新CPSR中条件标志位的值。        **bne WakeupStart**        @ All LED on 这里就需要对GPIO口进行控制        mov r1, #0x56000000         add r1, r1,#0x50 //0x56000050是GPFCON用来配置port F端口        ldr r2,=0x55aa        str r2, [r1,#0x0] //设置为0101010110101010因为每两位用来控制一个引脚，也就是将GPF4-GPF7设置为输出，将GPF0-GPF3设置为中断        mov r2, #0xff        str r2, [r1,#0x8]  //0x56000058为GPFUP为port F的上拉寄存器，全设置为1表示禁止上拉功能        mov r2, #0x00        str r2, [r1,#0x4]  //0x56000054是GPFDAT，总共8位有效，每位控制一个引脚，主要是将GPF4-GPF7数据位全设置为0而这四个引脚是用来控制板子上4个LED，置0则表示亮。# 230 "arch/s3c2410/head.S"        @ set GPIO for UART        mov r1, #0x56000000        add r1, r1,#0x70//0x56000070为GPHCON 用来配置port H 而port H主要就是来控制UART的各个引脚如：UART中接收和发送端口RXDn和TXDn，当然还有自动流控制模式的nRTS0和nCTS0端口。        ldr r2, gpio_con_uart  //我们可以看到 gpio_con_uart: .long vGPHCON  gpio_up_uart: .long vGPHUP 而在platform中的smdk2410.h中定义了这两个值#define vGPHCON   0x0016faaa 表示GPHCON控制11个引脚，如GPH2若设置为10则表示TXD0.类似，具体的查看数据手册#define vGPHUP   0x000007ff //同样将这11位的引脚上拉禁止        str r2, [r1,#0x0]        ldr r2, gpio_up_uart        str r2, [r1,#0x8] //上面也是来配置串口所用到的GPIO口，因为串口的输入输出口都是利用到GPIO       ** bl InitUART**# 259 "arch/s3c2410/head.S"        **bl copy_myself**        @ jump to ram        ldr r1, =on_the_ram        add pc, r1,#0        nop        nop1: b 1b @ infinite loopon_the_ram:# 279 "arch/s3c2410/head.S"        @ get read to call C functions开始调用C函数之前就需要将一些参数准备好，如堆栈要准备好函数调用时需要进出栈        ldr sp, DW_STACK_START @ setup stack pointer        mov fp, #0 @ no previous frame, so fp=0        mov a2, #0 @set argv toNULL        bl main @ call main        mov pc, #0x00000000 @ otherwise, reboot@@ End VIVI head@下面是子例程@@ Wake-up codes@WakeupStart:        @ Clear sleep reset bit        ldr r0, PMST_ADDR        mov r1, #(1<< 1)        str r1, [r0]        @ Release the SDRAM signal protections        ldr r0, PMCTL1_ADDR        ldr r1, [r0]        bic r1, r1,#((1<< 19)| (1 << 18)|(1<< 17))        str r1, [r0]        @ Go...        ldr r0, PMSR0_ADDR @ read a return address        ldr r1, [r0]        mov pc, r1        nop        nop1: b 1b @ infinite loopSleepRamProc:        @ SDRAM is in the self-refresh mode */        ldr r0, REFR_ADDR        ldr r1, [r0]        orr r1, r1,#(1<< 22)        str r1, [r0]        @ wait until SDRAM into self-refresh        mov r1, #161: subs r1, r1,#1        bne 1b        @ Set the SDRAM singal protections        ldr r0, PMCTL1_ADDR        ldr r1, [r0]        orr r1, r1,#((1<< 19)| (1 << 18)|(1<< 17))        str r1, [r0]        ldr r0, PMCTL0_ADDR        ldr r1, [r0]        orr r1, r1,#(1<< 3)        str r1, [r0]1: b 1b# 379 "arch/s3c2410/head.S"**.globl memsetup; .align 0; memsetup:  //对ENTRY(memsetup)宏的展开**        @ initialise the static memory        //同样在这里是对内存控制中用到的13个寄存器进行初始化        @ set memory control registers        mov r1, #0x48000000        adrl r2, mem_cfg_val        add r3, r1,#521: ldr r4, [r2],#4        str r4, [r1],#4        cmp r1, r3        bne 1b        mov pc, lr@@ copy_myself: copy vivi to ram@**下面的将vivi从flash中搬移到sdram中来执行，很重要**copy_myself:        mov r10, lr  //将返回地址保存到r10，为了执行完后返回到主程序        @ reset NAND        mov r1, #0x4E000000        ldr r2, =0xf830 @ initial value 初始化NFCONF寄存器，至于为什么设置为0xf830前面在NAND里面讲过        str r2, [r1,#0x00]        ldr r2, [r1,#0x00]        bic r2, r2,#0x800 @ enable chip也就是相当于NFCONF &= ~0x800 将第15为置为1使能NAND FLASH        str r2, [r1, #0x00]        mov r2, #0xff @RESET command        strb r2, [r1,#0x04] //向NFCMD寄存器中置0xff也就是reset命令        mov r3, #0 @ wait//下面几句是一个延时程序用来等待几秒，等到NAND 准备好1: add r3, r3,#0x1        cmp r3, #0xa        blt 1b2: ldr r2, [r1, #0x10] @ wait ready        tst r2, #0x1//检查NFSTAT寄存器的第0位是否为1也就是是否准备好        beq 2b //没有准备好则继续等待        ldr r2, [r1,#0x00]        orr r2, r2,#0x800 @ disable chip相当于NFCONF |= 0x800 禁止掉NAND FLASH等到要使用FLASH时再使能        str r2, [r1, #0x00]        @ get read to call C functions (for nand_read())        ldr sp, DW_STACK_START @ setup stack pointer //每次需要从汇编调到C函数时 都需要设置好堆栈        mov fp, #0 @ no previous frame, so fp=0        @ copy vivi to RAM 之前都是一些初始化，下面才开始利用C函数来真正开始拷贝        ldr r0, =(0x30000000+ 0x04000000- 0x00100000)//DRAM_BASE + DRAM_SIZE - VIVI_RAM_SIZE,为什么这里不是将vivi拷贝到sdram的起始地址而是拷贝到64MB的sdram的最后1M的区域，可能是这里的sdram采用高端模式，将映象从高地址向低地址存放        mov r1, #0x0//r1则是vivi的起始地址，也就是flash上的0x0地址        mov r2, #0x20000//上面三句都是用来为调用nand_read_ll函数准备好参数，r2表示拷贝大小        bl nand_read_ll //这个c函数在arch/s3c2410/nand_read.c中 nand_read_ll就不具体分析了在nand里面有讲过        tst r0, #0x0  //为什么要比较r0与上0，等于0的话 则去执行ok_nand_read,在这里r0是nand_read_ll函数的返回值，拷贝成功则返回0,所以这就是为什么将r0和0比较        beq ok_nand_read //ok_nand_read子程序用来比较拷贝到sdram最后1MB的vivi和原始的存放在flash上的vivi是否相同，检查拷贝是否成功，vivi在sdram中的位置也就是离0x34000000地址前1MB的位置也就是0x33f00000# 441 "arch/s3c2410/head.S"ok_nand_read:        @ verify        mov r0, #0  //r0这里为flash上vivi的起始地址        ldr r1, =0x33f00000//r1这里为拷贝到sdram上vivi的起始地址        mov r2, #0x400 @ 4 bytes* 1024= 4K-bytes//要比较多少个字节数go_next:        ldr r3, [r0],#4//将r0对应地址的值取出来保存到r3中，然后r0自加4个字节        ldr r4, [r1],#4        teq r3, r4  //测试r3和r4是否相等        bne notmatch //若不相等，则跳到notmarch处        subs r2, r2,#4//将比较总字节数减去4个字节，已经比较了4个字节        beq done_nand_read //若r2为0,则表示已经比较完毕，跳转到done_nand_read处        bne go_next //r2若还不等于0则继续取值比较notmatch:# 469 "arch/s3c2410/head.S"1: b 1bdone_nand_read:        mov pc, r10 //比较完了 就退出子程序，返回主程序执行@ clear memory@ r0: start address@ r1: lengthmem_clear:        mov r2, #0        mov r3, r2        mov r4, r2        mov r5, r2        mov r6, r2        mov r7, r2        mov r8, r2        mov r9, r2clear_loop:        stmia r0!,{r2-r9} //将r2-r9也就是0赋值给从r0为内存起始地址的连续的8*4个字节中        subs r1, r1,#(8* 4) //每次清除32个字节        bne clear_loop`````````````mov pc, lr# 613 "arch/s3c2410/head.S"@ Initialize UART@@ r0 = number of UART portInitUART: //这里也不细讲了，在UART章节中已经详细的讲解了每个寄存器的设置        ldr r1, SerBase        mov r2, #0x0        str r2, [r1,#0x08]        str r2, [r1,#0x0C]        mov r2, #0x3        str r2, [r1,#0x00]        ldr r2, =0x245        str r2, [r1,#0x04]        mov r2, #((50000000/(115200* 16))- 1)        str r2, [r1,#0x28]        mov r3, #100        mov r2, #0x01: sub r3, r3,#0x1        tst r2, r3        bne 1b# 653 "arch/s3c2410/head.S"        mov pc, lr@@ Exception handling functions@HandleUndef:1: b 1b @ infinite loopHandleSWI:1: b 1b @ infinite loopHandlePrefetchAbort:1: b 1b @ infinite loopHandleDataAbort:1: b 1b @ infinite loopHandleIRQ:1: b 1b @ infinite loopHandleFIQ:1: b 1b @ infinite loopHandleNotUsed:1: b 1b @ infinite loop@@ Low Level Debug@# 838 "arch/s3c2410/head.S"@@ Data Area@@ Memory configuration values.align 4mem_cfg_val: //这些变量都是内存控制寄存器的初始值，因为寄存器比较多，所以将初始值制作成表的形式，然后分别读表来初始化各个寄存器        .long 0x22111110        .long 0x00000700        .long 0x00000700        .long 0x00000700        .long 0x00000700        .long 0x00000700        .long 0x00000700        .long 0x00018005        .long 0x00018005        .long 0x008e0459        .long 0xb2        .long 0x30        .long 0x30@ Processor clock values.align 4clock_locktime:        .long 0x00ffffffmpll_50mhz:        .long((0x5c<< 12)|(0x4<< 4)|(0x2))mpll_200mhz:        .long((0x5c<< 12)|(0x4<< 4)|(0x0))clock_clkcon:        .long 0x0000fff8clock_clkdivn:        .long 0x3@ initial values for serialuart_ulcon:        .long 0x3uart_ucon:        .long 0x245uart_ufcon:        .long 0x0uart_umcon:        .long 0x0@ inital values for GPIOgpio_con_uart:        .long 0x0016faaagpio_up_uart:        .long 0x000007ff        .align 2DW_STACK_START:        .word (((((0x30000000+ 0x04000000 - 0x00100000)- 0x00100000)- 0x00004000)-(0x00004000+ 0x00004000+0x00004000))- 0x00008000)+0x00008000-4# 922 "arch/s3c2410/head.S".align 4SerBase:        .long 0x50000000# 935 "arch/s3c2410/head.S".align 4PMCTL0_ADDR:        .long 0x4c00000cPMCTL1_ADDR:        .long 0x56000080PMST_ADDR:        .long 0x560000B4PMSR0_ADDR:        .long 0x560000B8REFR_ADDR:        .long 0x48000024[root@lqm vivi_myboard]#```|
|----|




【include/machine.h】则是利用条件编译来选择适合自己开发板的头文件，如：

#ifdef CONFIG_S3C2410_SMDK

#include "platform/smdk2410.h"

#endif

本开发板的头文件是**【include/platform/smdk2410.h】，主要是一些寄存器的初始值（以v开头）和一些相关的地址等等的定义。一般开发板不同，都是修改此文件相应的部分。这个头文件很重要，因为我们一般要将vivi移植到某个类似于smdk2410的开发板时都只是在这个头文件中修改一些寄存器的初始值和一些寄存器的地址值还有内存分配的地址值等等**



2、关于中断向量表



    开始对中断向量表很疑惑。现在的理解比较清晰了，在硬件实现上，会支持中断机制，这个可以参考微机接口原理部分详细理解。现在的中断机制处理的比较智能，对每一类中断会固定一个中断向量，比如说，发生IRQ中断，中断向量地址为0x00000018（当然，这还与ARM9TDMI core有关，其中一个引脚可以把中断向量表配置为高端启动，或者低端启动。你可以通过CP15的register
 1的bit 13的V bit来设置，可以查看Datasheet TABLE 2-10来解决。），那么PC要执行的指令就是0x00000018。如果在这个地址放上一个跳转指令（只能使用b或者ldr），那么就可以跳到实际功能代码的实现区了。ARM体系结构规定在上电复位后的起始位置，必须有8条连续的跳转指令，这是bootloader的识别入口，通过硬件实现。看一下vivi的中断向量表：



|```@ 0x00        b Reset@ 0x04HandleUndef:        b HandleUndef@ 0x08HandleSWI:        b HandleSWI@ 0x0cHandlePrefetchAbort:        b HandlePrefetchAbort@ 0x10HandleDataAbort:        b HandleDataAbort@ 0x14HandleNotUsed:        b HandleNotUsed@ 0x18HandleIRQ:        b HandleIRQ@ 0x1cHandleFIQ:        b HandleFIQ```|
|----|




    注意，中断向量表可以修改，**也可以通过MMU实现地址重映射，来改变对应的物理介质(在MMU一章中我们提到过由于flash介质上执行速度比较慢，所以我们一般利用MMU将0x0开始的一段代码映射到SDRAM从0x30000000开始的区域，这样代码的执行就比较快)**。如果不对每种中断进行测试，可以采用下面的书写方式。



|```@ 0x00        b Reset@ 0x04HandleUndef:        b .@ 0x08HandleSWI:        b .@ 0x0cHandlePrefetchAbort:        b .@ 0x10HandleDataAbort:        b .@ 0x14HandleNotUsed:        b .@ 0x18HandleIRQ:        b .@ 0x1cHandleFIQ:        b .```|
|----|




    其中，“.”表示当前地址，那么很明显，“b .”，就表示了死循环了。



    如果增加中断处理，则需要考虑使用b还是使用ldr。主要的区别在于b指令跳转范围有限，仅仅是+-32M。所以如果超出32M，那么就要采用ldr了，这也就是为什么下面中断表中reset是采用b跳转指令，而其他中断向量都是采用ldr,基本的模式是：



|```.globl _start_start: b reset        ldr pc, _undefined_instruction        ldr pc, _software_interrupt        ldr pc, _prefetch_abort        ldr pc, _data_abort        ldr pc, _not_used        ldr pc, _irq        ldr pc, _fiq_undefined_instruction: .word undefined_instruction_software_interrupt: .word software_interrupt_prefetch_abort: .word prefetch_abort_data_abort: .word data_abort_not_used: .word not_used_irq: .word irq_fiq: .word fiq````...````/* * exception handlers */        .align  5undefined_instruction:        get_bad_stack        bad_save_user_regs        bl      do_undefined_instruction````...`|
|----|




    这就是uboot采用的方式。这些技术是相当成熟的。现在在加速启动上，就会考虑对中断向量表作出一些相应的处理。比如，如果从nor flash启动，启动后把kernel等加载到sdram里面运行。无疑，sdram里面运行速度比nor flash里面快。但是如果不对中断向量表进行处理，那么发生中断时，首先还是得访问nor
 flash里面的中断向量表。然后跳转到sdram相应的执行部分。所以，提高速度就是要解决不经过nor flash，完全在sdram里面运行。采用的技术手段是通过重定向机制，有些SoC提供了这样的硬件手段，但是有些没有，比如S3C2410就没有。但是S3C2410有MMU，可以通过MMU来改变映射关系实现虽然中断向量还是0x00000018，但是实际的物理介质是sdram，具体的方法可以参考MMU部分的总结来完成。**如果采用nand flash启动的话，就不必作出这样的处理。本质上是因为nand flash并非内存映射，而且中断向量表占用的是开始4K的steppingstone，是sram，其速度比sdram还要快，如果作出上面的处理，速度反而会下降了。**



    中断向量表的出现也是具有历史原因的，是解决特定问题采用的一种技术手段。为了了解中断向量表的一些特殊用途，还应该理解加载域和运行域的不同。这样就能从全局上把握处理的原则，相应的解决机制也比较容易理解了。



3、关于vivi magic number



    接下来，vivi设置了自己的一些magic number。理解什么是magic number，需要通过google来查找。本来wiki上有关于magic number（programming）的解释，不过最近打不开了，找了一个替代网站，网址是[http://www.answers.com/](http://www.answers.com/)，上面的内容是wiki的备份吧（好像可以这样理解）。关于magic
 number（programming）的文章为[http://www.answers.com/topic/magic-number-programming](http://www.answers.com/topic/magic-number-programming)。关于magic number（programming）的总结和解释，专门拿出一篇来总结（基本上是上述英文文章的翻译吧，不过讲解的确实非常清晰，读完之后对magic
 number就很明白了，而且在程序设计中，如何恰当的使用magic number，也会有一定的认识）。



|```@ 0x20: magic number so we can verify that we only put        .long 0@ 0x24:        .long 0@ 0x28: where this vivi was linked, so we can put itin memoryin the right place        .long _start@ 0x2C: this contains the platform, cpuand machine id        .long ARCHITECTURE_MAGIC@ 0x30: vivi capabilities         .long 0```|
|----|




对vivi的这些magic number，虽然设计在这里，不过大部分还是没有使用的。其中0x20和0x24没有使用，在0x2C处，倒是设计了一个magic number，组成的格式如下：bit[31:24]为platform，bit[23:16]为cpu type，bit[15:0]为machine id。关于ARCHITECTURE_MAGIC的定义，在【include/platform/smdk2410.h】，如下：



|```/* * Architecture magic and machine type */#include "architecture.h"#define MACH_TYPE        193    #define ARCHITECTURE_MAGIC    ((ARM_PLATFORM<< 24)| (ARM_S3C2410_CPU<< 16)| /                 MACH_TYPE)```|
|----|




    具体的值的定义则在【include/architecture.h】里面。ARM_PLATFORM为1，ARM_S3C2410_CPU为6，很简单的推理，可以计算出s3c2410的值0x010600c1。不过你可以尝试把此部分去掉，编译仍然没有问题（在我的配置的前提下，因为还没有分析完，所以还不好确定，在vivi的打印信息里面，只有一个MACH_TYPE，实现的手段是通过上述宏，还是通过读取内存提取字段，等分析到那个部分的时候再具体解决，总之，在这里，这个并不成为问题。你可以有多种猜想，也可以按照自己的想法定制，本来magic
 number就是“幻数”，你当然也可以玩一下了。）



4、实际完成的主要任务：



    ·关闭看门狗。上电复位后，看门狗默认是开启的，vivi是不需要使用看门狗的，所以首先要关闭。

    ·关闭所有中断。上电复位后，所有中断默认是关闭的，所以可以不需要代码实现。当然，为了保险和理解上的方便，可以增加此部分代码（vivi就是如此）。

    ·初始化系统时钟。参考clock部分。

    ·内存设置。主要就是完成13个相关寄存器的设置，当然，正常情况下，加入内存检测是必要的，如果内存不可用，或者设置有问题，那么后续工作都是无法完成的。关于内存检测算法，可以参考詹荣开的《嵌入式bootloader技术内幕》，主要分析了blob的内存检测算法，这也具有一定的通用性。vivi采取了类似的算法。这里需要理解的问题就是测试的时候为什么要采用0xaa和0x55。写成二进制就比较清晰了，它们正好是1和0交替，结合sdram的硬件特点，采用这样的值能检测出多种问题。我在用C8051F020写sdram检测时，就是采用了这样的内存检测算法。可以展开来分析研究。


    ·初始化调试指示灯（可选）


    ·初始化UART，作为调试口（可选择，这部分工作是可以放到stage 2来完成的，不过，如果对stage 1来进行调试，可以采用调试灯的方法，也可以初始化UART来完成调试信息的打印，可以参考ARM调试总结部分）。

    ·复制代码到sdram中。

    ·跳转到main，进入stage 2。在这里，詹荣开提到一种弹簧床的技术，实际实现比较简单。在vivi中，可以用如下方法：



|```@ jump to ram        @ a technology about trampoline        ldr r1, =on_the_ram        add pc, r1, #0        nop        nop1:        b 1bon_the_ram:        @ setup by APCS        ldr sp, DW_STACK_START @ setup stack pointer        mov fp, #0 @ no previous frame, so fp=0        mov a2, #0 @ set argv to NULL        bl main @ call main        mov pc, #FLASH_BASE @ otherwise, reboot```|
|----|




    这里有几点需要说明，一是开始的几处nop，实际上是考虑到流水线断流问题而设计的。这部分我也理解的不是太清晰，可以参考毛德操的书来分析。但是可以按照自己想法做简单的修改，也就是不考虑流水线问题，直接采用简单的ldr加载（至于为什么用ldr，而不是使用b，主要还是加载域和运行域的问题，参考前面总结），经过测试也没有问题。二是关于fp等的设置，这里主要是利用了APCS（ARM过程调用标准），具体可以参考相应的标准，同样的道理，如果把这三条去掉，运行也是没有问题的。加入这三条语句和去除这三条语句，在程序设计上，到底有什么不同，可能造成的微影响是什么，还有待研究。三是，如果调用出现问题，直接软复位，这个那个弹簧床技术本质一致，但是处理手段不同。当然，你也可以用b
 on_the_ram。



    具体的每个阶段的工作在前面的基础实验中都做了详细的分析，具体可以参考那些总结部分，就不罗列在这里了。



    至此，bootloader（vivi）第一阶段的分析就完成了。





