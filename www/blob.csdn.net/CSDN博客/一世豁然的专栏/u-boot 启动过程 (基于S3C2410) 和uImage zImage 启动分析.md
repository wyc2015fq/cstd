# u-boot 启动过程 (基于S3C2410) 和uImage zImage 启动分析 - 一世豁然的专栏 - CSDN博客





2015年05月12日 19:02:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1393








本博客转载于：[http://blog.csdn.net/slj_win/article/details/6952241](http://blog.csdn.net/slj_win/article/details/6952241)




本文以流行的Samsung公司的S3C2410，openmoko平台和u-boot-1.3.2（2008.5 发布）为例，介绍如何在ZIX嵌入式开发环境下探索u-boot启动过程。

虽然u-boot已经广泛应用，由于其相对的复杂性使用户在了解其内部机理和进行u-boot的移植工作时还是会碰到困难。u-boot已有一些分析文档，但多数和真正的代码不能同步或者版本老旧，难以将概念和现实的代码匹配——即硬件板上跑的代码在文档资料中却看不到，更无法紧密的跟踪。本文涉及的代码基于在s3c2410硬件运行的成熟u-boot-1.3.2代码，版本较新，提供的特性非常丰富，而且在forum.linuxbj.com可以自由浏览和下载。此u-boot代表了业界的较高水平，可以直接构建新版的嵌入式产品设计，有较高的应用价值。

u-boot总的启动流程如下
->reset
-> 设置CPU模式
-> 关闭看门狗／中断
-> 设置处理器时钟／片上总线
-> 初始化调试串口
-> MMU／外部总线／SDRAM等初始化
-> rom代码／数据搬移到ram
-> 初始化函数调用栈
-> 初始化外围设备／参数
-> 启动完毕，进入main_loop循环

嵌入式系统离不开bootloader初始化硬件以及引导操作系统。
现在，专用的嵌入式板子运行嵌入式Linux系统已经变得非常流行，u-boot是一种非常适合此类系统的bootloader。

u-boot主要提供以下功能：

设置目标板硬件参数并初始化；
为操作系统传递必要信息；
执行交互式的底层操作；
智能化装载操作系统；
引导和运行的固件程序；
支持大容量存储和USB接口


利用ZIX开发环境，能够通过比较直观的方式观察u-boot内部，而且可以将代码调试和分析同时进行，是一种了解、移植u-boot的强大工具。

使用arm工具链编译u-boot源代码，得到可以烧录的u-boot.bin文件。 
在ZIX开发环境里，可以将u-boot.bin载入s3c2410板运行，并利用gdb调试。

gdb能通过JTAG接口访问硬件，也可以通过TCP/IP访问虚拟硬件。 建立好调试连接，即可通过gdb操纵u-boot启动过程，下面可以跟随代码的执行顺序，了解从上点开始，究竟哪些操作被执行。

s3c2410复位之后，pc指针会指向0x0地址。在u-boot代码中，该0x0地址是一个向量表，
第一条指令跳转branch到复位代码start_code。 位于cpu/arm920t/start.S汇编语言文件第53行:

- .globl _start  _start:  b start_code  

-     ldr pc, _undefined_instruction 

-     ldr pc, _software_interrupt 

-     ldr pc, _prefetch_abort 

-     ldr pc, _data_abort 

-     ldr pc, _not_used 

-     ldr pc, _irq 

-     ldr pc, _fiq
*复制代码*复位指令跳转之后来到第154行，开始执行arm920t处理器的基本初始化。
首先修改当前程序状态寄存器CPSR，使处理器进入Supervisor|32 bit ARM模式，
并关闭ARM9TDMI中断和快速中断，这是通过设置CPSR相应掩码实现的：

- start_code: 

-     /* 

-      * set the cpu to SVC32 mode 

-      */  

-     mrs r0,cpsr 

-     bic r0,r0,#0x1f 

-     orr r0,r0,#0xd3 

-     msr cpsr,r0 
*复制代码*紧接着，将S3C2410特有的WTCON寄存器清零，此举仅为关闭看门狗,代码位置是234行：

-     ldr r0, =pWTCON 

-     mov r1, #0x0 

-     str r1, [r0]
*复制代码*然后在241行，将S3C2410中断控制器INTMSK寄存器置为全1，
INTSUBMSK置为0x7ff，禁止全部中断源。S3C2410手册358页起对此有详细描述：

-     mov r1, #0xffffffff  

-     ldr r0, =INTMSK 

-     str r1, [r0]  

- # if defined(CONFIG_S3C2410) || defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442) || \ 

-   defined(CONFIG_S3C2443)  

-     ldr r1, =INTSUBMSK_val  

-     ldr r0, =INTSUBMSK  

-     str r1, [r0]  

- # endif
*复制代码*接下来在259行，访问arm920t控制寄存器CP15，并置位最高两位［31,30］。
此两位置为0b11后，处理器时钟被设置为异步模式，允许处理器异步访问总线：
[/code]
    mrc p15, 0, r1, c1, c0, 0 
    orr r1, r1, #0xc0000000 
    mcr p15, 0, r1, c1, c0, 0

- 至此arm920t相关的配置完成，后面开始设定S3C2410时钟合成参数。

- 通过设置UPLL，MPLL和CLKDIVN三个寄存器（在S3C2410手册237页起讲述），

- 得到需要的处理器工作频率，分别在308行：

- [code]

-     ldr r0, =UPLLCON  

-     ldr r1, =UPLLCON_val 

-     str r1, [r0] 

- 321行：

- 

-     ldr r1, =MPLLCON_val  

-     str r1, [r0, #-4]    /* MPLLCON */

- 

-     /* FCLK:HCLK:PCLK = 1:2:4 */ 

-     ldr r0, =CLKDIVN 

-     mov r1, #CLKDIVN_val 

-     str r1, [r0]
*复制代码*S3C2410的UART0得到初始化，以便于尽早通过UART0打印信息。
此段代码从332行开始，其中涉及到的寄存器读者可参考S3C2410手册293页起：

-     /* enable uart */

-     ldr r0, =0x4c00000c /* clkcon */ 

-     ldr r1, =0x7fff0 /* all clocks on */ 

-     str r1, [r0] 

-     /* gpio UART0 init */ 

-     ldr r0, =0x56000070 

-     mov r1, #0xaa 

-     str r1, [r0] 

-     /* init uart */ 

-     ldr r0, =0x50000000 

-     mov r1, #0x03 

-     str r1, [r0] 

-     ldr r1, =0x245 

-     str r1, [r0, #0x04] 

-     mov r1, #0x01 

-     str r1, [r0, #0x08] 

-     mov r1, #0x00 

-     str r1, [r0, #0x0c] 

-     mov r1, #0x1a 

-     str r1, [r0, #0x28]
*复制代码*完成UART0设置之后，根据不同的编译时选项和运行时参数，代码会在360行进入相应的分支，分别是

从nand启动，代码执行lowlevel_init，主要是清除cpu cache，以及关闭mmu和i-cache，
并且根据板极硬件配置初始化外部存储器总线和GPIO，最后把代码从nand flash中拷贝到ram中并继续执行。
从nor启动，与第1种情况相比，仅仅把代码拷贝部分简化，将DATA段从flash中拷贝到ram中，其余相同
从ram启动，因为u-boot已经处于配置好的ram中，
所以会跳过所有cache，mmu，sdram，nand和nor相关代码，跳转到done_relocate执行
下面以最复杂的nand启动情况为例分析。首先会跳转到572行执行cpu_init_crit，
通过操作CP15完成flush处理器arm920t的cache和tlb，并关闭mmu和i-cache：

- 
cpu_init_crit:
- 
    /*
- 
     * flush v4 I/D caches 
- 
     */  
- 
    mov r0, #0 
- 
    mcr p15, 0, r0, c7, c7, 0 /* flush v3/v4 cache */ 
- 
    mcr p15, 0, r0, c8, c7, 0 /* flush v4 TLB */   
- 
    /*
- 
     * disable MMU stuff and caches
- 
     */ 
- 
    mrc p15, 0, r0, c1, c0, 0 
- 
    bic r0, r0, #0x00002300     @ clear bits 13, 9:8 (--V- --RS) 
- 
    bic r0, r0, #0x00000087     @ clear bits 7, 2:0 (B--- -CAM) 
- 
    orr r0, r0, #0x00000002     @ set bit 2 (A) Align 
- 
    orr r0, r0, #0x00001000     @ set bit 12 (I) I-Cache 
- 
    mcr p15, 0, r0, c1, c0, 0复制代码然后跳转到board/neo1973/common/lowlevel_init.S文件的139行执行，
进行总线数据宽度、时序、SDRAM控制、GPIO等配置，配置完毕后会返回start.S继续执行。
因为该代码是与板相关，故放在board目录里面。由于代码较多，只粘贴开始部分：



- 
    /* memory control configuration */  
- 
    /* make r0 relative the current location so that it */  
- 
    /* reads SMRDATA out of FLASH rather than memory ! */ 
- 
    adr r0, SMRDATA  
- 
    ldr r1, =BWSCON /* Bus Width Status Controller */ 
- 
    add r2, r0, #13*4复制代码完成板级设置后，在cpu/arm920t/start.S的373行判断代码自身的执行位置。如果从stepping stone内执行，
并且u-boot配置为nand boot模式，则跳转到nand_load拷贝代码：

    ldr r1, =BWSCON /* Z = CPU booted from NAND */ 
    ldr r1, [r1] 
    tst r1, #6 /* BWSCON[2:1] = OM[1:0] */ 
    teqeq r0, #0 /* Z &= running at address 0 */ 
    beq nand_load 
[/code]
在417行是nand_load代码，首先会跳转到614行执行may_resume
以检测系统是从待机模式唤醒还是上电启动。如果唤醒，则会根据之前保存的现场进行相应处理，
本文不做更多叙述；如果是启动，则会返回nand_load继续执行。在nand_load里初始化s3c2410的nand controller，
涉及存储器映射和寄存器NFCONF等，参见S3C2410手册215页起。同样，仅粘贴开始部分的代码：



- 
    mov r1, #S3C2410_NAND_BASE 
- 
    ldr r2, =0xf842 @ initial value enable tacls=3,rph0=6,rph1=0 
- 
    str r2, [r1, #oNFCONF] 
- 
    ldr r2, [r1, #oNFCONF] 
- 
    bic r2, r2, #0x800 @ enable chip复制代码在451行继续根据配置设定栈指针，为后面调用C函数执行拷贝作准备:



- 
    ldr r0, _TEXT_BASE /* upper 128 KiB: relocated uboot */ 
- 
    sub r0, r0, #CFG_MALLOC_LEN /* malloc area */ 
- 
    sub r0, r0, #CFG_GBL_DATA_SIZE /* bdinfo */ 
- 
#ifdef CONFIG_USE_IRQ 
- 
    sub r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ) 
- 
#endif 
- 
    sub sp, r0, #12 /* leave 3 words for abort-stack */复制代码然后在460行，将SDRAM中的目标地址存入r0，0x0地址存入r1，u-boot长度存入r2，
跳入cpu/arm920t/s3c24x0/nand_read.c文件第154行执行nand_read_ll函数，该函数接受前面3个寄存器中的值作为参数，并将返回值放回r0：



- 
    ldr r0, _TEXT_BASE 
- 
    mov r1, #0x0 
- 
    mov r2, #CFG_UBOOT_SIZE 
- 
    bl nand_read_ll复制代码在nand_read_ll函数中实现了nand flash访问代码，并且支持自动跳过坏块的特性，函数循环执行nand页面读取并存入SDRAM，直到u-boot全部拷贝完，并返回0，该C代码留给读者自己阅读。nand_read_ll返回0后，会跳转到ok_nand_read，并482行对拷贝的头4K字节进行校验：



- 
    @ verify 
- 
    mov r0, #0 
- 
    @ldr r1, =0x33f00000 
- 
    ldr r1, _TEXT_BASE 
- 
    mov r2, #0x400     @ 4 bytes * 1024 = 4K-bytes 
- 
go_next: 
- 
    ldr r3, [r0], #4 
- 
    ldr r4, [r1], #4 
- 
    teq r3, r4 
- 
    bne notmatch 
- 
    subs r2, r2, #4 
- 
    beq done_nand_read 
- 
    bne go_next复制代码校验通过后代码506行，在地址为_booted_from_nand的SDRAM位置保存1，以便告诉上层软件本次启动是从nand引导：



- 
done_nand_read: 
- 
    ldr r0, _booted_from_nand 
- 
    mov r1, #1 
- 
    strb r1, [r0]复制代码然后在518行，将中断向量表拷贝到0x0：



- 
    mov r0, #0 
- 
    ldr r1, _TEXT_BASE 
- 
    mov r2, #0x40 
- 
irqvec_cpy_next: 
- 
    ldr r3, [r1], #4 
- 
    str r3, [r0], #4 
- 
    subs r2, r2, #4 
- 
    bne irqvec_cpy_next复制代码在532行，设置栈指针：



- 
    ldr r0, _TEXT_BASE    /* upper 128 KiB: relocated uboot */ 
- 
    sub r0, r0, #CFG_MALLOC_LEN    /* malloc area */ 
- 
    sub r0, r0, #CFG_GBL_DATA_SIZE /* bdinfo */ 
- 
#ifdef CONFIG_USE_IRQ 
- 
    sub r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ) 
- 
#endif 
- 
    sub sp, r0, #12    /* leave 3 words for abort-stack */复制代码在541行，清除bss段并跳转到真正的C函数start_armboot，进入更高级的硬件初始化代码，汇编初始化部分也全部完成使命：



- 
    ldr r0, _bss_start /* find start of bss segment */ 
- 
    ldr r1, _bss_end /* stop here */ 
- 
    mov r2, #0x00000000 /* clear */  
- 
clbss_l:
- 
    str r2, [r0] /* clear loop... */ 
- 
    add r0, r0, #4 
- 
    cmp r0, r1 
- 
    ble clbss_l  
- 
    ldr pc, _start_armboot复制代码start_armboot函数位于lib_arm/board.c文件第277行，首先初始化globel_data类型的变量gd。该变量是一个结构，其成员大多是板子的一些基本设置，如序列号、ip地址、mac地址等(欲知结构的原型可参考include/asm-arm/globel_data.h和include/asm-arm/u-boot.h)：



- 
         gd = (gd_t*)(_armboot_start - CFG_MALLOC_LEN - sizeof(gd_t));
- 
         /* compiler optimization barrier needed for GCC >= 3.4 */
- 
         __asm__ __volatile__("": : :"memory");
- 

- 
         memset ((void*)gd, 0, sizeof (gd_t));
- 
         gd->bd = (bd_t*)((char*)gd - sizeof(bd_t));
- 
         memset (gd->bd, 0, sizeof (bd_t));复制代码然后在一个for循环中从init_sequence地址开始，逐个调用初始化C函数至NULL为止。这些routine函数按调用顺序分别是

cpu_init() ——在common/main.c文件，执行初始化中断栈操作
board_init() ——在board/neo1973/gta01/gta01.c文件中，执行板级初始化，主要是更新GPIO和PLL设置
interrupt_init() ——在/cpu/arm920t/s3c24x0/interrupts.c文件中，执行时钟中断初始化
env_init() ——在common/env_nand.c文件中，设置缺省环境变量
init_baudrate() ——在lib_arm/board.c文件中，将环境变量中的baudrate存入bd_info结构bd
serial_init() ——在common/serial.c文件中，调用驱动中真正的init()初始化串口
console_init_f() ——在common/console.c文件中，更新global_data结构gd的have_console标记为1
display_banner() ——在lib_arm/board.c文件中，打印u-boot banner，输出版本、运行地址等信息。比如在控制台看到的
init_func_i2c() ——在lib_arm/board.c文件中，初始化i2c总线
dram_init() ——在board/neo1973/gta01/gta01.c文件中，填充bd->bi_dram[0]的start和size成员，用来描述u-boot可用的ram
display_dram_config() ——在board/neo1973/gta01/gta01.c文件中，打印当前ram配置。在控制台能够看到相应的 DRAM:  128 MB
利用gdb可以清晰的看到调用过程：



- 
         for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) { 
- 
                  if ((*init_fnc_ptr)() != 0) { 
- 
                           hang (); 
- 
                  } 
- 
         }复制代码接着是一些可选外设的初始化，如显示屏、nor、nand、dataflash、网卡等，此过程执行后全部初始化工作完成。下面仅粘贴nor代码：



- 
#ifndef CFG_NO_FLASH 
- 
         /* configure available FLASH banks */ 
- 
         size = flash_init (); 
- 
         display_flash_config (size); 
- 
#endif /* CFG_NO_FLASH */复制代码之后在457行进入无限循环，调用common/main.c文件的278行main_loop()函数，u-boot完成启动过程。main_loop提供一个交互式命令行，可通过串口或usb控制台操作，也可以进一步引导操作系统：



- 
         for (;;) { 
- 
                  main_loop (); 
- 
         }复制代码本文作者：周明 原载于嵌入式技术博客，请尊重作者劳动，欢迎转载但需注明原文链接！


怎样在ZIX中调试U-Boot和Kernel
由 pyl880121 在 星期一, 09/28/2009 - 22:49 发表。
怎样在ZIX中运用DDD调试并分析U-Boot和Kernel,给出具体步骤,谢谢周老师


请教u-boot 启动logo问题
由 trickeraxun 在 星期四, 12/18/2008 - 17:41 发表。
周老师,能不能讲解下怎样修改u-boot 的启动logo?


官方的u-boot没有logo的功能，ZIX使用的open
由 zhoum 在 星期四, 12/25/2008 - 10:57 发表。
官方的u-boot没有logo的功能，ZIX使用的openmoko加入了这个功能。
在ZIX里面进入 emulator 目录，运行 ./run-qemu
在u-boot的shell里面键入  mtdparts  可以看到分区 No. 3 就是splash，offset 是 0x244000


有一个制作logo的脚本
由 zhoum 在 星期五, 12/19/2008 - 18:23 发表。
有一个制作logo的脚本 splashimg.pl
首先你要有一个png格式的图片，分辨率480x640，比如叫做logo.png
运行  perl splashimg.pl logo.png > logo
得到logo文件，然后使用gzip压缩之
把logo.gz烧写到板上flash的splash分区即可
作者: zorion    时间: 2010-8-22
 00:06:47

（1）zImage自解压
本文以流行的Samsung公司的S3C2410，mini2440平台和linux-2.6.29为例，介绍如何在ZIX嵌入式开发环境下探索linux内核启动过程。

Linux内核启动一般由外部的bootloader引导，也可以在内核头部嵌入一个loader，实际的应用中这两种方式都会经常遇到。所以要了解内核启动最开始的过程，必须对bootloader如何引导内核有所熟悉。下面我们从u-boot加载linux内核的代码开始分析（关于u-boot 自身的启动流程，请参考u-boot 启动过程 —— 基于S3C2410）。

1.处理器内核加载代码

在u-boot的do_bootm_linux函数里，实现了处理器架构相关的linux内核加载代码，特别是tags传递。


该函数中，在(u-boot-1.6)lib_arm/armlinux.c的90行调用了getenv将bootargs环境变量保存在commandline

    char *commandline = getenv ("bootargs");
然后解析uImage文件头，并且按照头中的定义分解和加载uImage。所以这部分代码的运行取决于uImage文件是如何生成的，本文不做过多叙述，可参考另文了解u-boot使用。接下来进行tags设置工作，分别调用了

setup_start_tag()
setup_memory_tag()
setup_commandline_tag()
setup_initrd_tag()
setup_end_tag()
然后对TLB、cache等进行invalid操作，这是通过在lib_arm/armlinux.c的268行调用cleanup_before_linux()(cpu/arm920t/108)实现，然后即可跳入从uImage中分解出来的内核Image或zImage入口

    cleanup_before_linux ();
    theKernel (0, machid, bd->bi_boot_params);
    /* does not return */
    return;
在s3c2410平台上，该入口theKernel一般是物理地址0x30008000。如果我们使用zImage自解压内核映像，对应的代码正是自解压头，位置在内核源码linux-2.6.29的arch/arm/boot/compressed/head.S第 114行的start符号

     start:
                .type   start,#function
                .rept   8
                mov     r0, r0
                .endr

                b       1f
                .word   0x016f2818   @ Magic numbers to help the loader
                .word   start        @ absolute load/run zImage address
                .word   _edata       @ zImage end address
1:              mov     r7, r1       @ save architecture ID
                mov     r8, r2       @ save atags pointer
这也标志着u-boot将系统完全的交给了OS，bootloader生命终止。之后代码在133行会读取cpsr并判断是否处理器处于supervisor模式——从u-boot进入kernel，系统已经处于SVC32模式；而利用angel进入则处于user模式，还需要额外两条指令。之后是再次确认中断关闭，并完成cpsr写入

                mrs     r2, cpsr                @ get current mode
                tst     r2, #3                  @ not user?
                bne     not_angel
                mov     r0, #0x17               @ angel_SWIreason_EnterSVC
                swi     0x123456                @ angel_SWI_ARM
not_angel:
                mrs     r2, cpsr                @ turn off interrupts to
                orr     r2, r2, #0xc0           @ prevent angel from running
                msr     cpsr_c, r2
然后在LC0地址(157行)处将分段信息导入r0-r6、ip、sp等寄存器，并检查代码是否运行在与链接时相同的目标地址(162行)，以决定是否进行处理。由于现在很少有人不使用loader和tags，将zImage烧写到rom直接从0x0位置执行，所以这个处理是必须的（但是zImage的头现在也保留了不用loader也可启动的能力）。arm架构下自解压头一般是链接在0x0地址而被加载到0x30008000运行，所以要修正这个变化。涉及到

r5寄存器存放的zImage基地址
r6和r12（即ip寄存器）存放的got（global offset table）
r2和r3存放的bss段起止地址
sp栈指针地址
很简单，这些寄存器统统被加上一个你也能猜到的偏移地址 0x30008000。该地址是s3c2410相关的，其他的ARM处理器可以参考下表

PXA2xx是0xa0008000
IXP2x00和IXP4xx是0x00008000
Freescale i.MX31/37是0x80008000
TI davinci DM64xx是0x80008000
TI omap系列是0x80008000
AT91RM/SAM92xx系列是0x20008000
Cirrus EP93xx是0x00008000
这些操作发生在代码172行开始的地方，下面只粘贴一部分

                add     r5, r5, r0
                add     r6, r6, r0
                add     ip, ip, r0
后面在211行进行bss段的清零工作

not_relocated:  mov     r0, #0
1:              str     r0, [r2], #4            @ clear bss
                str     r0, [r2], #4
                str     r0, [r2], #4
                str     r0, [r2], #4
                cmp     r2, r3
                blo     1b
然后224行，打开cache，并为后面解压缩设置64KB的临时malloc空间

                bl      cache_on

                mov     r1, sp              @ malloc space above stack
                add     r2, sp, #0x10000    @ 64k max 
接下来238行进行检查，确定内核解压缩后的Image目标地址是否会覆盖到zImage头，如果是则准备将zImage头转移到解压出来的内核后面

                cmp     r4, r2
                bhs     wont_overwrite
                sub     r3, sp, r5              @ > compressed kernel size
                add     r0, r4, r3, lsl #2      @ allow for 4x expansion
                cmp     r0, r5
                bls     wont_overwrite

                mov     r5, r2                  @ decompress after malloc space
                mov     r0, r5
                mov     r3, r7
                bl      decompress_kernel
真实情况——在大多数的应用中，内核编译都会把压缩的zImage和非压缩的Image链接到同样的地址，s3c2410平台下即是0x30008000。这样做的好处是，人们不用关心内核是Image还是zImage，放到这个位置执行就OK，所以在解压缩后zImage头必须为真正的内核让路。

在250行解压完毕，内核长度返回值存放在r0寄存器里。在内核末尾空出128字节的栈空间用，并且使其长度128字节对齐。

                add     r0, r0, #127 + 128      @ alignment + stack
                bic     r0, r0, #127            @ align the kernel length
算出搬移代码的参数：计算内核末尾地址并存放于r1寄存器，需要搬移代码原来地址放在r2，需要搬移的长度放在r3。然后执行搬移，并设置好sp指针指向新的栈（原来的栈也会被内核覆盖掉）

                add     r1, r5, r0              @ end of decompressed kernel
                adr     r2, reloc_start
                ldr     r3, LC1
                add     r3, r2, r3
1:              ldmia   r2!, {r9 - r14}         @ copy relocation code
                stmia   r1!, {r9 - r14}
                ldmia   r2!, {r9 - r14}
                stmia   r1!, {r9 - r14}
作者: zorion    时间: 2010-8-22
 00:08:04

  zImage和uImage的区别 收藏
2月3日
zImage和uImage的区别
　一、vmlinuz
vmlinuz是可引导的、压缩的内核。“vm”代表“Virtual Memory”。Linux 支持虚拟内存，不像老的操作系统比如DOS有640KB内存的限制。Linux能够使用硬盘空间作为虚拟内存，因此得名“vm”。
vmlinuz的建立有两种方式。一是编译内核时通过“make zImage”创建，然后通过:“cp /usr/src/linux-2.4/arch/i386/linux/boot/zImage/boot/vmlinuz”产生。zImage适用于小内核的情况，它的存在是为了向后的兼容性。
二是内核编译时通过命令make bzImage创建，然后通过:“cp/usr/src/linux-2.4/arch/i386/linux/boot/bzImage /boot/vmlinuz”产生。bzImage是压缩的内核映像，需要注意，bzImage不是用bzip2压缩的，bzImage中的bz容易引起误解，bz表示“big zImage”。
 bzImage中的b是“big”意思。 zImage(vmlinuz)和bzImage(vmlinuz)都是用gzip压缩的。它们不仅是一个压缩文件，而且在这两个文件的开头部分内嵌有 gzip解压缩代码。所以你不能用gunzip 或 gzip –dc解包vmlinuz。
二、initrd-x.x.x.img
initrd是“initial ramdisk”的简写。initrd一般被用来临时的引导硬件到实际内核vmlinuz能够接管并继续引导的状态。
initrd映象文件是使用mkinitrd创建的。mkinitrd实用程序能够创建initrd映象文件。这个命令是RedHat专有的。其它Linux发行版或许有相应的命令。这是个很方便的实用程序。具体情况请看帮助:man mkinitrd下面的命令创建initrd映象文件。

最后生成的内核镜象有两种zImage以及uImage。其中zImage下载到目标板中后，可以直接用uboot的命令go来进行直接跳转。这时候内核直接解压启动。但是无法挂载文件系统，因为go命令没有将内核需要的相关的启动参数传递给内核。传递启动参数我们必须使用命令bootm来进行跳转。Bootm命令跳转只处理uImage的镜象。
uboot源代码的tools/目录下有mkimage工具，这个工具可以用来制作不压缩或者压缩的多种可启动映象文件。
mkimage在制作映象文件的时候，是在原来的可执行映象文件的前面加上一个0x40字节的头，记录参数所指定的信息，这样uboot才能识别这个映象是针对哪个CPU体系结构的，哪个OS的，哪种类型，加载内存中的哪个位置， 入口点在内存的那个位置以及映象名是什么
用法如下：
./mkimage -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image
-A ==> set architecture to 'arch'
-O ==> set operating system to 'os'
-T ==> set image type to 'type'
-C ==> set compression type 'comp'
-a ==> set load address to 'addr' (hex)
-e ==> set entry point to 'ep' (hex)
-n ==> set image name to 'name'
-d ==> use image data from 'datafile'
-x ==> set XIP (execute in place)
参数说明：

-A 指定CPU的体系结构：

取值 表示的体系结构
alpha Alpha
arm A RM
x86 Intel x86
ia64 IA64
mips MIPS
mips64 MIPS 64 Bit
ppc PowerPC
s390 IBM S390
sh SuperH
sparc SPARC
sparc64 SPARC 64 Bit
m68k MC68000

-O 指定操作系统类型，可以取以下值：
openbsd、netbsd、freebsd、4_4bsd、linux、svr4、esix、solaris、irix、sco、dell、ncr、lynxos、vxworks、psos、qnx、u-boot、rtems、artos

-T 指定映象类型，可以取以下值：
standalone、kernel、ramdisk、multi、firmware、script、filesystem

-C 指定映象压缩方式，可以取以下值：
none 不压缩
gzip 用gzip的压缩方式
bzip2 用bzip2的压缩方式

-a 指定映象在内存中的加载地址，映象下载到内存中时，要按照用mkimage制作映象时，这个参数所指定的地址值来下载

-e 指定映象运行的入口点地址，这个地址就是-a参数指定的值加上0x40（因为前面有个mkimage添加的0x40个字节的头）

-n 指定映象名

-d 指定制作映象的源文件
我在编译时用到的命令如下：
# make zImage      //生成zImage镜象
＃/usr/local/arm/k9uboot/tools/mkimage -n 'Linux 2.4.27' -A arm -O linux -T
kernel -C none -a 0x20007fc0 -e 0x20008000 -d zImage uImage

内核镜象已经准备好了，这个时候我们就要来准备文件系统了。由于时间缘故，本人暂时采用的是其他人已经好的文件系统k9.img.gz。这个时候我们要做的是，自己写一个简单hello.c的程序，编译通过后加入到该文件系统中，然后下载到目标板中运行。
先编写hello.c；
编译：
#/usr/local/arm/2.95.3/bin/arm-linux-gcc  –o start-hello hello.c
编译后生成可执行文件start-hello
下面我们就必须把该执行文件加入到文件系统中去，步骤如下：
#gunzip  k9.img.gz                     //解压缩
#mount –o loop k9.img  /mnt/new_disk    //挂载
#cp start-hello  /mnt/new_disk           //将文件拷贝到文件系统中
#cd  /mnt/new_disk
#umount /mnt/new_disk                 //卸载
#gzip –c –v9 k9.img > k9.img.gz          //压缩 生成最终的文件系统

下面我们就要下载内核以及准备好文件系统了，这边先说明我的内存分配情况如下：
Flash：
0x10000000 ――― 0x10020000     boot
0x10020000 ――― 0x10040000     uboot
0x10040000 ――― 0x10060000     uboot env
0x10060000 ――― 0x10200000     kernel
0x10200000 ――― 0x11000000     ramdisk

Sdram：
0x20007fc0 ――― 0x20a00000     kernel
0x20a00000 ―――                ramdisk

Loadb  通过串口下载数据到ram中
cp.b    拷贝ram中的数据到flash中。

将kernel以及文件系统ramdisk下载完毕之后，我们还需要设置uboot的环境变量，这样uboot才能够在上电启动的时候启动内核等操作。环境变量设置如下：
Set cpfltoram cp.b 10200000 20a00000 18ffff            //拷贝文件系统到ram中
Set boot bootm 20007fc0                            //启动kernel
Set bootcmd run cpfltoker\;run cpfltoram\;run boot       //uboot复位的执行指令

Set cpfltoker cp.b 10060000 20007fc0 f4fff             //拷贝内核到ram中
Set bootargs root=/dev/ram rw initrd=0x20a00000,4M init=/linuxrc console=ttyS0,11520
0,mem=32m                      //uboot传递给内核的启动参数

[](http://sunhualing.spaces.live.com/blog/cns!7C7EDE32519133D6!165.trak)http://sunhualing.spaces.live.com/blog/cns!7C7EDE32519133D6!165.trak








对于Linux内核，编译可以生成不同格式的映像文件，例如：
# make zImage
# make uImage

    zImage是ARM Linux常用的一种压缩映像文件，uImage是U-boot专用的映像文件，它是在zImage之前加上一个长度为0x40的“头”，说明这个映像文件的类型、加载位置、生成时间、大小等信息。换句话说，如果直接从uImage的0x40位置开始执行，zImage和uImage没有任何区别。另外，Linux2.4内核不支持uImage，Linux2.6内核加入了很多对嵌入式系统的支持，但是uImage的生成也需要设置。
vmlinux  编译出来的最原始的内核文件，未压缩。

zImage   是vmlinux经过gzip压缩后的文件。

bzImage bz表示“big zImage”，不是用bzip2压缩的。两者的不同之处在于，zImage解压缩内核到低端内存(第一个640K)，bzImage解压缩内核到高端内存(1M以上)。如果内核比较小，那么采用zImage或bzImage都行，如果比较大应该用bzImage。

uImage   U-boot专用的映像文件，它是在zImage之前加上一个长度为0x40的tag。

vmlinuz  是bzImage/zImage文件的拷贝或指向bzImage/zImage的链接。

initrd   是“initial ramdisk”的简写。一般被用来临时的引导硬件到实际内核vmlinuz能够接管并继续引导的状态。

vmlinux是内核文件，

zImage是一般情况下默认的压缩内核映像文件，压缩vmlinux，加上一段解压启动代码得到，只能从0X0地址运行。

bzImage在uClinux中很少见到。

uImage是u-boot使用bootm命令引导的Linux压缩内核映像文件格式，
使用工具mkimage对普通的压缩内核映像文件（zImage）加工而得。可以由bootm命令从任意地址解压启动内核。

由于bootloader一般要占用0X0地址，所以，uImage相比zImage的好处就是可以和bootloader共存。

制作uImage的mkimage软件，如果uClinux-dist有的话，一般放在uClinux-dist的tools目录中。作者: zorion    时间: 2010-8-22
 00:13:48

uboot启动zImage(go)和uImage(bootm)分析
时间:2010-01-10 12:18来源:未知 作者:admin 点击:197次
[](http://www.linuxforum.net/forum/showthreaded.php?Cat=&Board=embedded)http://www.linuxforum.net/forum/showthreaded.php?Cat=&Board=embedded

[](http://www.linuxforum.net/forum/showthreaded.php?Cat=&Board=embedded&Number=700811&Search=true&Forum=embedded&Words=uboot%C6%F4%B6%AFzImage&Match=Entire%20Phrase&Searchpage=0&Limit=25&Old=allposts&Main=700811)http://www.linuxforum.net/forum/showthreaded.php?Cat=&Board=embedded&Number=700811&Search=true&Forum=embedded&Words=uboot%C6%F4%B6%AFzImage&Match=Entire%20Phrase&Searchpage=0&Limit=25&Old=allposts&Main=700811
我找了一块龙珠系列mx9328的板子和一个windriver ice，于是想把上面跑起来linux，不过内核启动遇到一些，调试了1个星期，有点收获，准备记录下来，先把之前不明白的zImage和uImage差异写出来。
做个记号
1.之间差异。
bootm
加载linux镜像是加载uIamge，uIamge是由mkimage制作而来，和zIamge的差异是uIamge是zIamge压缩过
的，bootm需要先对uIamge解压，解压地址为内核入口地址。当解压完成时uIamge和zIamge
几乎是相同的，具体差异可以论述。uboot目前只能支持uImage启动，不支持zImage启动
2.bootm解压过程
---------------------------------------------------------------------------
## Booting image at 08808000 ...
Image Name: Linux-2.6.14.7
Image Type: ARM Linux Kernel Image (uncompressed)
Data Size: 989172 Bytes = 966 kB
Load Address: 08008000
Entry Point: 08008000
Verifying Checksum ... OK
OK uboot bootm命令对uIamge的解压操作
---------------------------------------------------------------------------
Starting kernel ... 
传递内核参数将控制权交给arch\arm\boot\compressed]head.S
----------------------------------------------------------------------------
如mx1ADS班子内存的起始地址为0x0800_0000,通过tftp 下载到0x0800_8000 +offset ，offset大于0x80_0000,即tftp 0x0880_8000 然后bootm 0x0880_8000 即
将0x0880_8000处的uIamge解压，解压地址即为mkimage 设置的kernel入口地址0x0800_8000。
2.bootm内核启动分析
通过分析uboot把控制权交给kernel的瞬间，我们可以知道bootm启动linux内核，它究竟做了哪些工作。
具体源码在uboot1.16 lib_arm\Armlinux.c do_bootm_linux()，大家有兴趣可以看下源码，这里不仔细分析了，网上资料超多。
void (*theKernel)(int zero, int arch, uint params) 通过对这个函数指针的操作实现控制权从uboot到linux的交接。
该函数可以等价于set pc=0x0800_8000 ，r0=0 ，r1=0xA0(mx1ads 芯片的id)，r2=0x08000100（bootargs参数基址）。
我通过仿真器，设置了一个指令断点在0x0800_8000,可以看到该瞬间uboot做的操作。 
>BKM>dr //查看当前时刻通用寄存器值
R00 = 00000000 R01 = 000000A0 R02 = 08000100 R03 = 08008000
R04 = 00000000 R05 = 08000124 R06 = 083DC0A9 R07 = 0841BC9C
R08 = 083DBFDC R09 = 083E0260 R10(SL) = 00000000 R11(FP) = 00000002
R12(IP) = 083DBFC0 R13(SP) = 083DBD44 R14(LR) = 08413984 PC = 08008000
CPSR = 600000D3 SPSR = B00000FF

>BKM>di 0x08008000 // 反汇编该地址区域代码
08008000: mov r0,r0
08008004: mov r0,r0
08008008: mov r0,r0
0800800c: mov r0,r0
08008010: mov r0,r0
08008014: mov r0,r0
08008018: mov r0,r0
0800801c: mov r0,r0
08008020: b 0x8008030
08008024: .long 0x16F2818
08008028: andeq r0,r0,r0
0800802c: streqd r1,[pc],-r4
08008030: mov r7,r1
08008034: mov r8,#0
08008038: mrs r2,cpsr
0800803c: tst r2,#3
08008040: bne 0x800804C
08008044: mov r0,#0x17
08008048: swi 0x123456
0800804c: mrs r2,cpsr
08008050: orr r2,r2,#0xC0
08008054: msr cpsr_c,r2
08008058: andeq r0,r0,r0
0800805c: andeq r0,r0,r0
08008060: add r0,pc,#0xC8
08008064: ldmia r0,{r1,r2,r3,r4,r5,r6,r12,r13}
08008068: sub r0,r0,r1
0800806c: beq 0x800809C
>BKM>dml 0x08000100 0x50 //uboot bootargs
08000100: 00000005 54410001 00000000 00000000 ....TA..........
08000110: 00000000 00000004 54410002 04000000 ........TA......
08000120: 08000000 0000000F 54410009 736E6F63 ........TA..snoc
08000130: 3D656C6F 53797474 2C30584D 32353131 =eloSytt,0XM2511
08000140: 386E3030 3D706920 746F6F62 6F722070 8n00=pi toobor p
08000150: 2F3D746F 2F766564 0073666E 00000000 /=to/ved.sfn....
08000160: 00000000 00000000 00000000 00000000 ................
通过仿真器可以很清晰的看到bootm先后完成了2部分工作。大家可以分析bootm源码
1.set bootargs 传递参数
2.set pc=0x0800_8000 ，r0=0 ，r1=0xA0
3.对比zImage 分析
通过tftp 0x0800_8000 命令直接将zImage下载到0x0800_8000地址
此时对0x08008000 反汇编可以得到和arch\arm\boot\compressed\head.S一致的汇编代码，说明这是kernel的入口 
08008000: msr cpsr_c,#0xD3
08008004: bl 0x8008150
08008008: mov r10,r5
0800800c: beq 0x8008148
08008010: bl 0x80081B0
08008014: mov r8,r5
08008018: beq 0x8008148
0800801c: bl 0x80080D8
08008020: ldr r13,[pc,#+4]
08008024: add r14,pc,#0x68
08008028: add pc,r10,#0xC
0800802c: andgt r8,r0,r0,asr r0
08008030: andgt r2,r11,r0
08008034: andgt r2,r11,r0
08008038: andgt r2,r12,r0,lsl #1
0800803c: ldrgth r14,[r13],-r12
08008040: andgt r5,r12,r12,asr #1
08008044: mulgts r12,r12,r0
08008048: andgt r5,r11,r12,asr #0x13
0800804c: ldrgtsh r3,[r11],-r8
08008050: sub r3,pc,#0x28
08008054: ldmia r3!,{r4,r5,r6,r7} 08008058: cmp r4,r5 0800805c: cmpne r5,r6 08008060: ldrne r11,[r4],#+4 08008064: strne r11,[r5],#+4 08008068: bne 0x800805C 0800806c: mov
 r11,#0 08008070: cmp r6,r7

08008054: ldmia r3!,{r4,r5,r6,r7}
08008058: cmp r4,r5
0800805c: cmpne r5,r6
08008060: ldrne r11,[r4],#+4
08008064: strne r11,[r5],#+4
08008068: bne 0x800805C
0800806c: mov r11,#0
08008070: cmp r6,r7
08008074: strcc r11,[r6],#+4
08008078: bcc 0x8008070
0800807c: ldmia r3,{r4,r5,r6,r13}
08008080: str r9,[r4,#+0]
此时用go命令 go 0x08008000，go命令本质就是改变当前pc值，即pc=go 0x08008000
断点位置为 0x08008000 ，使用go 会在 0x08008000处停下来 
>BKM>dr //此时通用寄存器值为
R00 = 00000000 R01 = 083E0264 R02 = 000F0000 R03 = 0000000C
R04 = 08008000 R05 = 08808000 R06 = 41129200 R07 = 083E0264
R08 = 08000000 R09 = 18000000 R10(SL) = 00000000 R11(FP) = 00000020
R12(IP) = 08808354 R13(SP) = 088E9464 R14(LR) = 08808298 PC = 08008000
CPSR = 400000D3 SPSR = B00000FF
//uboot bootargs
08000100: 00000000 00000000 00000000 00000000 ................
08000110: 00000000 00000000 00000000 00000000 ................
08000120: 00000000 00000000 00000000 00000000 ................
08000130: 00000000 00000000 00000000 00000000 ................
08000140: 00000000 00000000 00000000 00000000 ................
08000150: 00000000 00000000 00000000 00000000 ................
08000160: 00000000 00000000 00000000 00000000 ................
08000170: 00000000 00000000 00000000 00000000 ................
08000180: 00000000 00000000 00000000 00000000 ................
08000190: 00000000 00000000 00000000 00000000 ................
080001A0: 00000000 00000000 00000000 00000000 ................
这个时候发现kernel无法正确启动zImage
4,通过仿真器对go命令加以改造
a.将通用寄存器值改成
R00 = 00000000 R01 = 000000A0 R02 = 08000100 R03 = 0000000C
R04 = 08008000 R05 = 08808000 R06 = 41129200 R07 = 083E0264
R08 = 08000000 R09 = 18000000 R10(SL) = 00000000 R11(FP) = 00000020
R12(IP) = 08808354 R13(SP) = 088E9464 R14(LR) = 08808298 PC = 08008000
CPSR = 400000D3 SPSR = B00000FF 
b.通过仿真器修改 0x08000100 地址的值
sml 0x08000100 00000005 54410001 00000000 00000000 00000000 00000004 54410002 04000000 08000000 0000000F 54410009 736E6F63
sml 0x08000130 3D656C6F 53797474 2C30584D 32353131 386E3030 3D706920 746F6F62 6F722070 2F3D746F 2F766564 0073666E 00000000
>BKM>dml 0x08000100 0x50
08000100: 00000005 54410001 00000000 00000000 ....TA..........
08000110: 00000000 00000004 54410002 04000000 ........TA......
08000120: 08000000 0000000F 54410009 736E6F63 ........TA..snoc
08000130: 3D656C6F 53797474 2C30584D 32353131 =eloSytt,0XM2511
08000140: 386E3030 3D706920 746F6F62 6F722070 8n00=pi toobor p
08000150: 2F3D746F 2F766564 0073666E 00000000 /=to/ved.sfn....
08000160: 00000000 00000000 00000000 00000000 ................ 
然后让程序执行，这样通过uboot也可以让zImage得以执行。
可见go和bootm差异就是 go只是改写pc值，而bootm传递r0，r1，r2还有bootargs
作者: zorion    时间: 2010-8-22
 00:35:09

U-BOOT的启动流程及移植[转载]
时间:2010-01-10 09:34来源:未知 作者:admin 点击:181次
摘要：嵌入式系统一般没有通用的bootloader,u-boot是功能强大的bootloader开发软件，但相对也比较复杂。文中对u-b

摘要：嵌入式系统一般没有通用的bootloader,u-boot是功能强大的bootloader开发软件，但相对也比较复杂。文中对u-boot的启动流程作了介绍，详细给出了u-boot在S3C44B0开发板上的移植方法和步骤。
关键词：bootloader；u-boot；嵌入式系统；移植；S3C44B0
１ Bootloader及u-boot简介
Ｂｏｏｔｌｏａｄｅｒ 代码是芯片复位后进入操作系统之前执行的一段代码，主要用于完成由硬件启动到操作系统启动的过渡，从而为操作系统提供基本的运行环境，如初始化ＣＰＵ、 堆栈、存储器系统等。Ｂｏｏｔｌｏａｄｅｒ 代码与ＣＰＵ 芯片的内核结构、具体型号、应用系统的配置及使用的操作系统等因素有关，其功能类似于ＰＣ机的ＢＩＯＳ程序。由于ｂｏｏｔｌｏａｄｅｒ和ＣＰＵ及电路板的配置情况有关，因此不可能有通用的ｂｏｏｔｌｏａｄｅｒ
 ，开发时需要用户根据具体情况进行移植。嵌入式Ｌｉｎｕｘ系统中常用的ｂｏｏｔｌｏａｄｅｒ有ａｒｍｂｏｏｔ、ｒｅｄｂｏｏｔ、ｂｌｏｂ、ｕ－ｂｏｏｔ等，其中ｕ－ｂｏｏｔ是当前比较流行，功能比较强大的ｂｏｏｔｌｏａｄｅｒ，可以支持多种体系结构，但相对也比较复杂。ｂｏｏｔｌｏａｄｅｒ的实现依赖于ＣＰＵ的体系结构，大多数ｂｏｏｔｌｏａｄｅｒ都分为ｓｔａｇｅ １和ｓｔａｇｅ ２两大部分。Ｂｏｏｔｌｏａｄｅｒ的基本原理见参考文献。
ｕ－ｂｏｏｔ是ｓｏｕｒｃｅｆｏｒｇｅ网站上的一个开放源代码的项目。它可对 ＰｏｗｅｒＰＣＭＰＣ５ｘｘ、ＭＰＣ８ｘｘ、ＭＰＣ８２ｘｘ、 ＭＰＣ７ｘｘ、ＭＰＣ７４ｘｘ　、ＡＲＭ（ＡＲＭ７、ＡＲＭ９、ＳｔｒｏｎｇＡＲＭ、Ｘｓｃａｌｅ）、ＭＩＰＳ（４ｋｃ、５ｋｃ）、Ｘ８６等处理器提供支持，支持的嵌入式操作系统有Ｌｉｎｕｘ、Ｖｘ-Ｗｏｒｋｓ、ＮｅｔＢＳＤ、ＱＮＸ、ＲＴＥＭＳ、ＡＲＴＯＳ、ＬｙｎｘＯＳ等，主要用来开发嵌入式系统初始化代码ｂｏｏｔｌｏａｄｅｒ。软件的主站点是ｈｔｔｐ／／ｓｏｕｒｃｅｆｏｒｇｅ．ｎｅｔ／ｐｒｏｊｅｃｔｓ／
 ｕ－ｂｏｏｔ。ｕ－ｂｏｏｔ 最初是由ｄｅｎｘｗｗｗ．ｄｅｎｘ．ｄｅ　的ＰＰＣ-ｂｏｏｔ发展而来的，它对ＰｏｗｅｒＰＣ系列处理器的支持最完善，对Ｌｉｎｕｘ 操作系统的支持最好。源代码开放的ｕ－ｂｏｏｔ软件项目经常更新，是学习硬件底层代码开发的很好样例。

２ u-boot系统启动流程
大多数ｂｏｏｔｌｏａｄｅｒ都分为ｓｔａｇｅ１和ｓｔａｇｅ２两大部分，ｕ－ｂｏｏｔ也不例外。依赖于ＣＰＵ体系结构的代码（如设备初始化代码等）通常都放在ｓｔａｇｅ１且可以用汇编语言来实现，而ｓｔａｇｅ２则通常用Ｃ语言来实现，这样可以实现复杂的功能，而且有更好的可读性和移植性。
２．１ ｓｔａｇｅ１ (ｓｔａｒｔ．ｓ代码结构)
ｕ－ｂｏｏｔ的ｓｔａｇｅ１代码通常放在ｓｔａｒｔ．ｓ文件中，它用汇编语言写成，其主要代码部分如下：
（１） 定义入口 。由于一个可执行的Ｉｍａｇｅ必须有一个入口点，并且只能有一个全局入口，通常这个入口放在ＲＯＭ(Ｆｌａｓｈ)的０ｘ０地址，因此，必须通知编译器以使其知道这个入口，该工作可通过修改连接器脚本来完成。
（２）设置异常向量(Ｅｘｃｅｐｔｉｏｎ Ｖｅｃｔｏｒ)。
（３）设置ＣＰＵ的速度、时钟频率及中断控制寄存器。
（４）初始化内存控制器 。
（５）将ＲＯＭ中的程序复制到ＲＡＭ中。
（６）初始化堆栈 。
（７）转到ＲＡＭ中执行，该工作可使用指令ｌｄｒ ｐｃ来完成。
２．２ ｓｔａｇｅ２Ｃ语言代码部分　
ｌｉｂ ａｒｍ／ｂｏａｒｄ．ｃ中的ｓｔａｒｔ ａｒｍｂｏｏｔ是Ｃ语言开始的函数，也是整个启动代码中Ｃ语言的主函数，同时还是整个ｕ－ｂｏｏｔ（ａｒｍｂｏｏｔ）的主函数，该函数主要完成如下操作：
（１）调用一系列的初始化函数。
（２）初始化Ｆｌａｓｈ设备。
（３）初始化系统内存分配函数。
（４）如果目标系统拥有ＮＡＮＤ设备，则初始化ＮＡＮＤ设备。
（５）如果目标系统有显示设备，则初始化该类设备。
（６）初始化相关网络设备，填写ＩＰ、ＭＡＣ地址等。
（７）进入命令循环（即整个ｂｏｏｔ的工作循环），接受用户从串口输入的命令，然后进行相应的工作。
３ 移植实例
本系统开发板主要由Ｓ３Ｃ４４Ｂ０Ｘ嵌入式微处理器、２ＭＢ的Ｆｌａｓｈ （ＳＳＴ３９ＶＦ１６０）、８ＭＢ的ＳＤＲＡＭ(ＨＹ５７Ｖ６４１６２０)、４个ＬＥＤ以及ＡＲＭ ＪＴＡＧ接口组成。该开发板上与Ｓ３Ｃ４４Ｂ０Ｘ相关部分的功能框图如图１所示。
３．１ ｕ－ｂｏｏｔ文件下载
ｕ－ｂｏｏｔ文件的下载有两种方法,第一种是在Ｌｉｎｕｘ环境下通过ＣＶＳ下载最新的文件，方法是：
＄ｃｖｓ－ｄｐｓｅｒｖｅｒａｎｏｎｙｍｏｕｓ＠ｃｖｓ．ｓｏｕｒｃｅｆｏｒｇｅ． ｎｅｔ／ｃｖｓｒｏｏｔ／ｕ－ｂｏｏｔ ｌｏｇｉｎ
当要求输入匿名登录的密码时，可直接按回车键
＄ｃｖｓ－ｚ６－ｄｐｓｅｒｖｅｒａｎｏｎｙｍｏｕｓ＠ｃｖｓ．ｓｏｕｒｃｅ ｆｏｒｇｅ．ｎｅｔ／ｃｖｓｒｏｏｔ／ｕ－ｂｏｏｔ＼ｃｏ．Ｐ ｍｏｄｕｌｅｎａｍｅ
第二种是通过ｆｔｐ／／ｆｔｐ．ｄｅｎｘ．ｄｅ／ｐｕｂ／ｕ－ｂｏｏｔ／ 下载正式发布的压缩文件。
３．２ ｕ－ｂｏｏｔ文件的结构
初次下载的文件有很多，解压后存放在ｕ－ｂｏｏｔ文件目录下，具体内容已在ｒｅａｄｍｅ文件中做了详细的介绍，其中与移植相关的主要文件夹有：
（１）ＣＰＵ它的每个子文件夹里都有如下文件：
ｍａｋｅｆｉｌｅ
ｃｏｎｆｉｇ．ｍｋ
ｃｐｕ．ｃ 和处理器相关的代码
ｉｎｔｅｒｒｕｐｔｓ．ｃ 中断处理代码
ｓｅｒｉａｌ．ｃ 串口初始化代码
ｓｔａｒｔ．ｓ 全局开始启动代码
（２）ＢＯＡＲＤ它的每个子文件夹里都有如下文件：
ｍａｋｅｆｉｌｅ
ｃｏｎｆｉｇ．ｍｋ
ｓｍｄｋ２４１０．ｃ 和板子相关的代码（以ｓｍｄｋ２４１０为例）
ｆｌａｓｈ．ｃ Ｆｌａｓｈ操作代码
ｍｅｍｓｅｔｕｐ．ｓ 初始化ＳＤＲＡＭ代码
ｕ－ｂｏｏｔ．ｌｄｓ 对应的连接文件
（３）ｌｉｂ ａｒｍ体系结构下的相关实现代码，比如ｍｅｍｃｐｙ等的汇编语言的优化实现。(责任编辑：admin)
３．３ 交叉编译环境的建立 要得到下载到目标板的ｕ－ｂｏｏｔ二进制启动代码，还需要对下载的ｕ－ｂｏｏｔ１．１．１进行编译。ｕ－ｂｏｏｔ的编译一般在Ｌｉｎｕｘ系统下进行，可用ＡＲＭ－ＬＩＮ-ＵＸ－ＧＣＣ进

３．３ 交叉编译环境的建立
要得到下载到目标板的ｕ－ｂｏｏｔ二进制启动代码，还需要对下载的ｕ－ｂｏｏｔ１．１．１进行编译。ｕ－ｂｏｏｔ的编译一般在Ｌｉｎｕｘ系统下进行，可用ＡＲＭ－ＬＩＮ-ＵＸ－ＧＣＣ进行编译。一步一步建立交叉编译环境通常比较复杂，最简单的方法是使用别人编译好的交叉编译工具，方法如下：
（１）在ｈｔｔｐ／／ｈａｎｄｈｅｌｄｓ．ｏｒｇ／ｄｏｗｎｌｏａｄ／ｔｏｏｌｃｈａｉｎ下载 ａｒｍ－Ｌｉｎｕｘ－ｇｃｃ－３．３．２．ｔａｒ．ｂｚ２
（２）以用户名ｒｏｏｔ登录，将ａｒｍ－ｌｉｎｕｘ－ｇｃｃ－３．３．２．ｔａｒ．ｂｚ２ 解压到 ／ｒｏｏｔ目录下
＃ ｔａｒ ｊｘｖｆ ａｒｍ－ｌｉｎｕｘ－ｇｃｃ－３．３．２．ｔａｒ．ｂｚ２
（３）在ｈｔｔｐ／／ｈａｎｄｈｅｌｄｓ．ｏｒｇ／ｄｏｗｎｌｏａｄ／ｔｏｏｌｃｈａｉｎ下载 ａｒｍ－ｌｉｎｕｘ－ｔｏｏｌｃｈａｉｎ－ｐｏｓｔ－２．２．１３．ｔａｒ．ｇｚ 只是用了它的头文件而已，主要来自内核／ｌｉｎｕｘ－ｘ．ｘ／ｉｎｃｌｕｄｅ下　
（４）将ａｒｍ－ｌｉｎｕｘ－ｔｏｏｌｃｈａｉｎ－ｐｏｓｔ－２．２． １３．ｔａｒ．ｇｚ 解压到 ／ｓｋｉｆｆ／ｌｏｃａｌ／ 下
＃ ｔａｒ ｚｘｖｆ ａｒｍ－ｌｉｎｕｘ－ｔｏｏｌｃｈａｉｎ－ｐｏｓｔ－２．２．１３．ｔａｒ．ｇｚ
（５）拷贝头文件到／ｒｏｏｔ／ｕｓｒ／３．３．２／ａｒｍ－ｌｉｎｕｘ／ 下然后删除 ／ｓｋｉｆｆ
＃ ｃｐ －ｄＲ ／ｓｋｉｆｆ／ｌｏｃａｌ／ａｒｍ－ｌｉｎｕｘ／ｉｎｃｌｕｄｅ ／ｒｏｏｔ／ｕｓｒ／３．３．２／ａｒｍ－ｌｉｎｕｘ
＃ ｒｍ －ｆｒ ／ｓｋｉｆｆ
这样就建立了ａｒｍ ｌｉｎｕｘ 交叉编译环境。
（６）增加／ｒｏｏｔ／ｕｓｒ／ｌｏｃａｌ／ａｒｍ／３．３．２／ｂｉｎ到路径环境变量
ｐａｔｈ＝＄ｐａｔｈ：／ｒｏｏｔ／ｕｓｒ／ｌｏｃａｌ／ａｒｍ／３．３．２／ｂｉｎ 可以检查路径变量是否设置正确。＃ ｅｃｈｏ ＄ｐａｔｈ
３．４ 移植的预先编译
移植ｕ－ｂｏｏｔ到新的开发板上仅需要修改与硬件相关的部分即可。主要包括两个层面的移植，第一层是针对ＣＰＵ的移植，第二层是针对ＢＯＡＲＤ的移植。由于ｕ－ｂｏｏｔ １．１．１里面已经包含Ｓ３Ｃ４４Ｂ０的移植，所以笔者对板子ｍｙｂｏａｒｄ的移植主要是针对ＢＯＡＲＤ的移植。移植之前需要仔细阅读ｕ－ｂｏｏｔ目录下的ｒｅａｄｍｅ文件，其中对如何移植做了简要的介绍。为了减少移植的工作量，可以在ｉｎｃｌｕｄｅ／ｃｏｎｆｉｇ目录下选一个和要移植的硬件相似的开发板，笔者选的是ｂ２开发板。具体步骤如下：
（１）ｕ－ｂｏｏｔ １．１．１ 下的ＣＰＵ文件夹里已经包括了Ｓ３Ｃ４４Ｂ０的目录，其下已经有ｓｔａｒｔ．ｓｉｎｔｅｒｒｕｐｔｓ．ｃ以及 ｃｐｕ．ｃｓｅｒｉａｌ．ｃ几个文件，因而不需要建立与ＣＰＵ相关的目录。
（２）在ｂｏａｒｄ目录下创建ｍｙｂｏａｒｄ目录以及ｍｙ-ｂｏａｒｄ．ｃ、ｆｌａｓｈ．ｃ、ｍｅｍｓｅｔｕｐ．ｓ和ｕ－ｂｏｏｔ．ｌｄｓ 等文件。不需要从零开始创建，只需选择一个相似的目录直接复制过来，然后修改文件名及内容即可。笔者在移植ｕ－ｂｏｏｔ过程中选择的是ｕ－ｂｏｏｔ １．１．１／ｂｏａｒｄ／ｄａｖｅ／Ｂ２目录。
（３）在ｉｎｃｌｕｄｅ／ｃｏｎｆｉｇｓ目录下添加ｍｙｂｏａｒｄ．ｈ，在这里可放入全局的宏定义等也不需要从头创建，可以在ｉｎｃｌｕｄｅ／ｃｏｎｆｉｇｓ目录下寻找相似的ＣＰＵ的头文件进行复制，这里笔者用的是Ｂ２．ｈ文件来进行相关的修改。
（４） 对ｕ－ｂｏｏｔ根目录下的ｍａｋｅｆｉｌｅ文件进行修改，加入
ｍｙｂｏａｒｄ_ｃｏｎｆｉｇ : ｕｎｃｏｎｆｉｇ
＠．／ｍｋｃｏｎｆｉｇ ＄(＠:_ｃｏｎｆｉｇ＝）ａｒｍ Ｓ３Ｃ４４Ｂ０ ｍｙｂｏａｒｄ
（５） 修改ｕ－ｂｏｏｔ根目录下的ｍａｋｅｆｉｌｅ文件，加入对板子的申明。然后在ｍａｋｅｆｉｌｅ 中加入ｍｙｂｏａｒｄ、ＬＩＳＴ ＡＲＭ７＝″Ｂ２ ｅｐ７３１２ ｉｍｐａ７ ｍｙｂｏａｒｄ″。
（６）运行ｍａｋｅ ｃｌｏｂｂｅｒ，删除错误的ｄｅｐｅｎｄ文件。
（７）运行ｍａｋｅ ｍｙｂｏａｒｄ ｃｏｎｆｉｇ。
（８）执行到此处即表示整个软件的ｍａｋｅｆｉｌｅ已建立，这时可修改生成的ｍａｋｅｆｉｌｅ中的交叉编译选项，然后打开ｍａｋｅｆｉｌｅ 文件，并找到其中的语句：
ｉｆｅｑ(＄(ＡＲＣＨ),ａｒｍ)
ＣＲＯＳＳ_ＣＯＭＰＩＬＥ＝ａｒｍ－ｌｉｎｕｘ－
ｅｎｄ ｉｆ
接着将其改成
ｉｆｅｑ(＄(ＡＲＣＨ),ａｒｍ)
ＣＲＯＳＳ ＣＯＭＰＩＬＥ＝／ｒｏｏｔ／ｕｓｒ／ｌｏｃａｌ／３．３．２／ｂｉｎ／ａｒｍ－ｌｉｎｕｘ－
ｅｎｄ ｉｆ
这一步和上面的设置环境变量只要有一个就可以了。
执行ｍａｋｅ，报告有一个错误，修改ｍｙｂｏａｒｄ／ｆｌａｓｈ．ｃ中的＃ｉｎｃｌｕｄｅ ″．．／ｃｏｍｍｏｎ／ｆｌａｓｈ．ｃ＂为＂ｕ－ｂｏｏｔ／ｂｏａｒｄ／ｄａｖｅ／ｃｏｍｍｏｎ／ｆｌａｓｈ．ｃ″,重新编译即可通过。
４ 移植时的具体修改要点
若预先编译没有错误就可以开始硬件相关代码的移植，首先必须要对移植的硬件有清楚地了解，如ＣＰＵ、ＣＰＵ的控制寄存器及启动各阶段程序在ＦｌａｓｈＳＤＲＡＭ中的布局等。
笔者在移植过程中先修改／ｉｎｃｌｕｄｅ／ｃｏｎｆｉｇ ／ｍｙ-ｂｏａｒｄ．ｈ头文件中的大部分参数（大部分的宏定义都在这里设置），然后按照ｕ－ｂｏｏｔ的启动流程逐步修改。修改时应熟悉ＡＲＭ汇编语言和Ｃ语言，同时也应对ｕ－ｂｏｏｔ启动流程代码有深入的了解。Ｂ２板的ＣＰＵ频率为７５ＭＨｚ、Ｆｌａｓｈ为４Ｍｂｉｔ、ＳＤＲＡＭ为１６Ｍｂｉｔ、串口波特率为１１５２００ｂｉｔ／ｓ、环境变量放在ＥＥＰＲＯＭ中。根据两个开发板的不同，需要修改的有：ＣＰＵ的频率、Ｆｌａｓｈ和ＳＤＲＡＭ容量的大小、环境变量的位置等。由于参考板已经有了大部分的代码，因此只需要针对ｍｙｂｏａｒｄ进行相应的修改就可以了。与之相关的文件有／ｉｎｃｌｕｄｅ／ｃｏｎｆｉｇ
 ／ｍｙｂｏａｒｄ．ｈ（大部分的宏定义都在这里设置）、／ｂｏａｒｄ／ｍｙｂｏａｒｄ／ｆｌａｓｈ．ｃＦｌａｓｈ的驱动序　、／ｂｏａｒｄ／ｍｙｂｏａｒｄ ／ｍｙｂｏａｒｄ．ｃ（ＳＤＲＡＭ的驱动程序）、／ＣＰＵ／Ｓ３Ｃ４４Ｂ０／ｓｅｒｉａｌ．ｃ（串口的驱动使能部分）等。(责任编辑：admin)
U-BOOT的启动流程及移植[转载](3)
时间:2010-01-10 09:34来源:未知 作者:admin 点击:次
／ｉｎｃｌｕｄｅ／ｃｏｎｆｉｇ ／ｍｙｂｏａｒｄ．ｈ是全局宏定义的地方，主要的修改有： 将＃ｄｅｆｉｎｅ ＣＯＮＦＩＧ Ｓ３Ｃ４４Ｂ０ ＣＬＯＣＫ ＳＰＥＥＤ ７５改为 ＃ｄｅｆｉｎｅ ＣＯＮＦＩＧ Ｓ３Ｃ４

／ｉｎｃｌｕｄｅ／ｃｏｎｆｉｇ ／ｍｙｂｏａｒｄ．ｈ是全局宏定义的地方，主要的修改有：
将＃ｄｅｆｉｎｅ ＣＯＮＦＩＧ Ｓ３Ｃ４４Ｂ０ ＣＬＯＣＫ ＳＰＥＥＤ ７５改为
＃ｄｅｆｉｎｅ ＣＯＮＦＩＧ Ｓ３Ｃ４４Ｂ０ ＣＬＯＣＫ ＳＰＥＥＤ ６４；
将 ＃ｄｅｆｉｎｅ ＰＨＹＳ ＳＤＲＡＭ １ ＳＩＺＥ ０ｘ０１００００００ ／＊ １６ ＭＢ ＊／ 改为
＃ｄｅｆｉｎｅ ＰＨＹＳ ＳＤＲＡＭ １ ＳＩＺＥ ０ｘ００８０００００ ／＊ ８ ＭＢ ＊／；
将 ＃ｄｅｆｉｎｅ ＰＨＹＳ ＦＬＡＳＨ ＳＩＺＥ ０ｘ００４０００００ ／＊ ４ ＭＢ＊改为
＃ｄｅｆｉｎｅ ＰＨＹＳ ＦＬＡＳＨ ＳＩＺＥ ０ｘ００２０００００ ／＊ ２ ＭＢ ＊／；
将 ＃ｄｅｆｉｎｅ ＣＦＧ ＭＡＸ ＦＬＡＳＨ ＳＥＣＴ ２５６ ／＊ ｍａｘ ｎｕｍｂｅｒ ｏｆ ｓｅｃｔｏｒｓ ｏｎ ｏｎｅ ｃｈｉｐ ＊／改为
＃ｄｅｆｉｎｅ ＣＦＧ ＭＡＸ ＦＬＡＳＨ ＳＥＣＴ ３５ ；
将 ＃ｄｅｆｉｎｅ ＣＦＧ ＥＮＶ ＩＳ ＩＮ ＥＥＰＲＯＭ １ ／＊ ｕｓｅ ＥＥＰＲＯＭ ｆｏｒ ｅｎｖｉｒｏｎｍｅｎｔ ｖａｒｓ＊／改为
＃ｄｅｆｉｎｅ ＣＦＧ ＥＮＶ ＩＳ ＩＮ ＦＬＡＳＨ １
其它（如堆栈的大小等）可根据需要修改。
由于Ｆｌａｓｈ、ＳＤＲＡＭ的容量会发生变化，故应对启动阶段程序在Ｆｌａｓｈ、ＳＤＲＡＭ中的位置重新作出安排。笔者将Ｆｌａｓｈ中的ｕ－ｂｏｏｔ代码放在０ｘ０开始的地方，而将复制到ＳＤＲＡＭ中的ｕ－ｂｏｏｔ代码安排在０ｘｃ７０００００开始的地方。
Ｆｌａｓｈ的修改不仅和容量有关，还和具体型号有关，Ｆｌａｓｈ存储器的烧写和擦除一般不具有通用性，应查看厂家的使用说明书，针对不同型号的存储器作出相应的修改。修改过程中，需要了解Ｆｌａｓｈ擦写特定寄存器的写入地址、数据命令以及扇区的大小和位置，以便进行正确的设置。
ＳＤＲＡＭ要修改的地方主要是初始化内存控制器部分，由ｓｔａｒｔ．ｓ文件中的 ｃｐｕ ｉｎｉｔ ｃｒｉｔ完成ＣＰＵ ｃａｃｈｅ的设置，并由 ｂｏａｒｄ／ｍｙｂｏａｒｄ／ｍｅｍｓｅｔｕｐ．ｓ中的ｍｅｍｓｅｔｕｐ完成初始化ＳＤＲＡＭ。Ｓ３Ｃ４４Ｂ０提供有ＳＤＲＡＭ控制器，与一些ＣＰＵ需要ＵＰＭ表编程相比，它只需进行相关寄存器的设置修改即可，因而降低了开发的难度。
串口波特率不需要修改（都是１１５２００ｂｉｔ／ｓ），直接用Ｂ２板的串口驱动即可。串口的设置主要包括初始化串口部分，值得注意的是：串口的波特率与时钟ＭＣＬＫ有很大关系，详见ＣＰＵ用户手册。
配置好以后，便可以重新编译ｕ－ｂｏｏｔ代码。将得到的ｕ－ｂｏｏｔ．ｂｉｎ通过ＪＴＡＧ口下载到目标板后，如果能从串口输出正确的启动信息，就表明移植基本成功。实际过程中会由于考虑不周而需要多次修改。移植成功后，也可以添加一些其它功能（如ＬＣＤ驱动等），在此基础上添加功能相对比较容易。
５ 结束语
ｕ－ｂｏｏｔ是一个功能强大的ｂｏｏｔｌｏａｄｅｒ开发软件，适用的ＣＰＵ平台及支持的嵌入式操作系统很多。本文是笔者在实际开发过程中根据相关资料进行摸索，并在成功移植了ｕ－ｂｏｏｔ的基础上总结出来的。对于不同的ＣＰＵ和开发板，其基本的方法和步骤是相同的，希望能对相关嵌入式系统的设计人员有所帮助。(责任编辑admin)
作者: zorion    时间: 2010-8-23
 21:58:30

U-BOOT的启动流程及移植
文章来源: 文章作者: 发布时间: 2006-11-16   字体: [大 中 小]  

　　3 移植实例

　　本系统开发板主要由S3C44B0X嵌入式微处理器、2MB的Flash (SST39VF160) 、8MB的SDRAM(HY57V641620) 、4个LED以及ARM JTAG接口组成。该开发板上与S3C44B0X相关部分的功能框图如图1所示。

　　3.1 u-boot文件下载

　　u-boot文件的下载有两种方法，第一种是在Linux环境下通过CVS下载最新的文件，方法是:

$ cvs -d :pserver:anonymous@cvs.sourceforge.net:/cvsroot/u-boot login
　　当要求输入匿名登录的密码时，可直接按回车键

$ cvs -z6 -d :pserver:anonymous@cvs.sourceforge.net:/cvsroot/u-boot co -P modulename
　　第二种是通过[](ftp://ftp.denx.de/pub/u-boot/)ftp://ftp.denx.de/pub/u-boot/下载正式发布的压缩文件。

　　3.2 u-boot文件的结构

　　初次下载的文件有很多，解压后存放在u-boot文件目录下，具体内容已在readme文件中做了详细的介绍，其中与移植相关的主要文件夹有:

　　(1) cpu/ 它的每个子文件夹里都有如下文件:

makefile
config.mk
cpu.c 和处理器相关的代码
interrupts.c 中断处理代码
serial.c 串口初始化代码
start.s 全局开始启动代码
　　(2) board/ 它的每个子文件夹里都有如下文件:

makefile
config.mk
smdk2410.c 和板子相关的代码(以smdk2410为例) 
flash.c Flash操作代码
memsetup.s 初始化SDRAM代码
u-boot.lds 对应的连接文件
　　(3) lib_arm/ 体系结构下的相关实现代码，比如memcpy等的汇编语言的优化实现。

　　3.3 交叉编译环境的建立

　　要得到下载到目标板的u-boot二进制启动代码，还需要对下载的u-boot1.1.1进行编译。u-boot的编译一般在Linux系统下进行，可用arm-linux-gcc进行编译。一步一步建立交叉编译环境通常比较复杂，最简单的方法是使用别人编译好的交叉编译工具，方法如下:

　　(1) 在[](http://handhelds.org/download/toolchain)http://handhelds.org/download/toolchain下载arm-linux-gcc-3.3.2.tar.bz2

　　(2) 以用户名root登录，将arm-linux-gcc-3.3.2.tar.bz2 解压到 /root目录下

# tar jxvf arm-linux-gcc-3.3.2.tar.bz2
　　(3) 在[](http://handhelds.org/download/toolchain)http://handhelds.org/download/toolchain下载arm-linux-toolchain-post-2.2.13.tar.gz，只是用了它的头文件而已，主要来自内核/linux-x.x/include下

　　(4) 将arm-linux-toolchain-post-2.2. 13.tar.gz 解压到/skiff/local/ 下

# tar zxvf arm-linux-toolchain-post-2.2.13.tar.gz作者: zorion    时间: 2010-8-23
 22:27:31

引 言
Bootloader(引导装载器)是用于初始化目标板硬件，给嵌入式操作系统提供板上硬件资源信息，并进一步装载、引导嵌入式操作系统运行的固件。在嵌 入式系统开发过程中，很多情况都会涉及底层Bootloader的移植问题， 即使在有些已有Bootloader的参考开发板上也存在这种可能。概括来说， 如下情况会考虑进行Bootloader的移植工作。
① 在自主设计的目标板上，用于引导嵌入式操作系统及其应用。
② 在厂家未提供Bootloader源码的参考板上，遇有
如下情形之一：
a．在实际应用中需要添加或修改一些功能；
b．为了给自行设计主板移植BOotlOade r提供参考，先在参考板上进行移植以积累经验。
    另外，从嵌入式系统实际开发角度讲， 嵌入式操作系统的引导、配置甚至应用程序的运行状况都和bootloader有一定的关联，可以说，掌握Bootloader移植是顺利进行嵌入式系统开发的重要利器。
     与常见的嵌入式操作系统板级支持包B S P相比，Boot loader与底层硬件更为相关， 即每个不同配置的目标板基本都有不同的Bootloader。因为Bootloader往往更依据量体裁衣、定身制作的原则， 以满足要求的最小化代码存放在启动ROM 或Flash中。
     虽然，自行编写Bootloader未尝不可，但从可利用的资源和实际项目开发考虑，采用移植已有的Bootloader源码来解决这一问题更符合大多数项目的开发要求。
1 U-Boot简介
     U—Boot，全称Universal Boot Loader，是遵循GPL条款的开放源码项目，从FADSROM、8xxROM 、PPCBOOT逐步发展演化而来，其源码目录、编译形式与Linux内核很相似。事实上，不少U—Boot源码就是相应Linux内核源程序的简化， 尤其是一些设备的驱动程序， 从U-Boot源码的注释中能体现这一点。但是U-Boot不仅仅支持嵌入式Linux系统的引导，
 当前，它还支持NetBSD。VxWorks、QNX、RTEMS、ARTOS、LynxOS嵌入式操作系统。其目前要支持的目标操作系统包括0P enB S D 、NetBSD、FreeBSD、4 4BSD 、Linux、SVR4、Esix、Solaris、Irix、SCO、Dell、NCR、VxWorks、LynxOS、pSOS、QNX、RTEMS 和ARTOS。这是u-Boot中Universal的一层含义。另外一层含义则是U-Boot除了支持PowerPC系列的处理器外，还能支持 MIPS、x86、ARM
 、Nios、XScale等诸多常用系列的处理器。这两个特点正是U-Boot项目的开发目标，即支持尽可能多的嵌入式处理器和嵌入式操作系统。就目前来 看，U-Boot对PowerPC系列处理器支持最为丰富， 对Linux的支持最完善。其它系列的处理器和操作系统基本是在2002年1 1月PPCBOOT改名为U-Boot 后逐步扩充的。从PPCBOOT向U—Boot的顺利过渡，很大程度上归功于U-Boot的维护人，德国DENX软件工程中心的Wolfgang Denk(以下简称w．D)本人精湛的专业水平和持着不懈的努力。当前，u-BOOt项目在他的领军下，众多有志于开放源码Boot
 loader移植工作的嵌入式开发人员， 正如火如荼地将各个不同系列嵌入式处理器的移植工作不断展开和深入， 以支持更多嵌入式操作系统的装载与引导。
2 U-Boot主要目录结构
board一目标板相关文件，主要包含SDRAM、Flash驱动；
common 一独立于处理器体系结构的通用代码， 如内存大小探测与故障检测；
cpu一与处理器相关的文件， 如mpc8xx子目录下含串口、网口、LCD 驱动及中断初始化等文件；
driver一通用设备驱动，如CFI Flash驱动(目前对Intel Flash支持较好)
doc— U．Boot的说明文档；
examples一可在U—Boot下运行的示例程序；如hello _ world．C，timer．C；
include—— U-Boot头文件，configs子目录下与目标板相关的配置头文件是移植过程中经常要修改的文件；
lib— XXX一处理器体系相关的文件， 如lib—PPC， lib — arm 目录分别包含与PowerPC、ARM体系结构相 关的文件；
net一与网络功能相关的文件目录，如bootp、nfs、 tftp；
post一上电自检文件目录， 尚有待于进一步完善；
rtc— RTC驱动程序；
tools——用于创建U—Boot S—RECORD和BIN镜像文件的工具。
3 U-Boot支持的主要功能
U—Boot可支持的主要功能如表1所列。
系统引导        支持NFS挂载、RAMDISK 系统引导 (压缩或非压缩)形式的根文件系统
支持NFS挂载，从Flash中引导压缩或非压缩系统内核
基本辅助        强大的操作系统接口功能，可灵活设置、传递多个关键参数给操作系统， 适合系统在不同开发阶段的调试要求与产品发布，尤其对Linux支持最为功能强劲
支持目标板环境参数的多种存储方式，如Flash、NVRAM、EEPROM
CRC32校验，可校验Flash中内核、RAMDISK镜像文件是否完好
设备驱动        串口、SDRAM、Flash、以太网、LCD、NVRAM、EEPROM、键盘、USB、PCMCIA、PCI、RTC等驱动支持
上电自检功能        SDRAM、Flash大小自动检测；SDRAM 故障检测；CPU型号
特殊功能        XIP内核引导
4 U-Boot移植过程
① 获得发布的最新版本U—Boot源码，与Linux内核源码类似，也是bzip2的压缩格式。可从U．Boot的官方网站http：／／sourceforge．net／projects／U-Boot上获得。
② 阅读相关文档，主要是U．Boot源码根目录下的README文档和U—Boot官方网站的DULG(The DENX U—
Boot and Linux Guide)文档(http：llwww．denx．de／twiki／bin／view／DULG／Manua1)。尤其是DULG文档，对如何安装建立交叉开发环境和解决U-Boot移植中常见问题，都一一给出了详尽说明。
③ 订阅U—Boot用户邮件列表(http：／／lists．sourceforge．net／lists／listinfo／u—boot— users)。当在移植U-Boot过程中遇有问题，在参考相关文档和搜索u．Boot．U ser邮件档案库(httP：／／SOurceforge．net／mailarchive／forum．php? forum
 — id=l 2898)仍不能解决时，第一时间提交所遇到的问题， 众多U-Boot开发人员会迅速排查问题，而且W．D本人很有可能会直接参与指导。
④ 在建立的开发环境下进行移植工作。绝大多数的开发环境是交叉开发环境。在这方面，DENX和MontaVi sta均提供了完整的开发工具集。
⑤ 在目标板与开发主机间接入硬件调试器。这是进行U-Boot移植应当具备且非常关键的调试工具。因为在整个U—Boot的移植工作中，尤其是初始阶段，硬 件调试器是我们了解目标板真实运行状态的唯一途径。在这方面， W ．D 本人和众多嵌入式开发人员倾向于使用BDI2000。一方面，其价格不如ICE调试器昂贵，同时其可靠性高，功能强大， 完全能胜任移植和调试U—Boot。另外，
 网上也有不少关于BDI2000调试方面的参考文档。
⑥ 如果在参考开发板上移植U—Boot，可能需要移除目标板上已有的Boot loader。可以根据板上Boot loader的说明文档，先着手解决在移除当前Boot loader的情况下，如何进行恢复，以便今后在需要场合能重新装入原先的Boot loader。
5 U-Boot移植方法
当前，对于U．Boot的移植方法，大致分为两种。一种是先用BDI2000创建目标板初始运行环境，将U-Boot镜像
文件u-boot．bin下载到目标板RAM中的指定位置，然后，用BDI2000进行跟踪调试。其好处是， 不用将Uboot镜
像文件烧写到Fla sh中去。但弊端在于， 对移植开发人员的移植调试技能要求较高，BDI2000的配置文件较为复杂。另外一种方法是用BDI2000先将U—Boot镜像文件烧写到Flash中 去，然后利用GDB和BDI2000进行调试。这种方法所用的BDI2000配置文件较为简单，调试过程与U-Boot移植后运行过程相吻合。即U— Boot先从Flash中运行，再重载至RAM
 中相应位置，并从那里正式投入运行。唯一感到有些麻烦的就是需要不断烧写Flash。但考虑到Flash常规擦写次数基本为l 0万次左右，作为移植U-Boot， 不会占用太多的次数，应该不会为Flash烧写有什么担忧。同时，w．D本人也极力推荐使用后一种方法。笔者建议， 除非是U-BOot移植资深人士或有强有力的技术支
持， 建议采用第二种移植方法。
6 U-Boot移植主要修改的文件
从移植U—Boot最小要求，U．Boot能正常启动的角度
出发， 主要考虑修改如下文件。
① <目标板>．h头文件，如include／con gs／RPxlite．h。可以是U-Boot源码中已有的目标板头文件，也可以是新命名的配置头文件；大多数的寄存器参数都是在这一文
件中设置完成的。
② <目标板>．C文件，如board／RPXlite／RPXlite．C。它是SDRAM 的驱动程序，主要完成SDRAM 的UPM 表设
置， 上电初始化。
③ Flash的驱动程序，如board／RPXlite／flash．C或
common／cfi — flash．C。可在参考已有Flash驱动的基础上， 结合目标板Flash数据手册，进行适当修改；
④ 串口驱动，如修改cpu／mpc8xx／seria1．c串口收发器芯片使能部分。
7 U-Boot移植要点 
① BDI2000的配置文件。如果采用第二种移植方法，即先烧入Flash的方法，配置项只需很少几个，就可以 进行U-Boot的烧写与调试了。对PPC 8xx系列的主板，可参考DULG文档中TQM8xx的配置文件进行相应的修改。下面，笔者以美国Embedded Planet公司的RPXlite Dw 板为例，给出在嵌入式Linux交叉开发环境下的BDI2000参考配置文件以作参考。
；bdiGDB configurationfileforRPXliteDW orLITE DW
[INIT】
；init core register
WSPR       149       0x2002000F        ；DER ：set debug enable
                                                                ；register
WSPR       149       0x2002000F        ；DER ：enable SYSIE for BDI 
                                                                ；Flash Program 
WSPR       638        oxFA200000        ；IMMR ：internal memory at
                                                                ；0xA200000
WM             32         oxFA200004          0xFFFFFF89 ；SYPCR
[TARGET]
CPUCLOCK                 4000000          ；the CPU clock rate after processing
                                                              ；the init list 
BDIMODE           AGENT ：the BDI WOrking mode
(LOADONLY | AGENT) 
BREAKMODE HARD                           ；SOFT or HARD，HARD uses PPC
                                                                  ；hardware breakpoints
[HOST]
IP                                        l72．16．115．6
FILE                                   ulmage．litedw
FORMAT BIN 
LOAD M ANUAL                               ；load code MANUAL or AUTO after rese 
DEBUGPORT 2001 
START 0x0100
[FLASH]
CHIPTYPE    AM29BX8                    ；Flash type(AM29F l AM29BX8
                                                            ；AM29BX16 f I28BX8 f 128BX16)
CHIPSIZE   0x400000                    ；The size of one flash chip in bytes
BUSWIDTH 32                               ；The width of the flash memory bus
                                                           ；inbits(8f 16f 32) 
WORKSPACE   0xFA202000         ；RAM buffer for fast flash 
                                                             ；programming
FILE           u-boot．bin                     ；The file to program
FORMAT      BIN      0x00000000
ERASE      0x00000000            BLOCK
ERASE      OxO0008000           BLOCK
ERASE      0x00010000             BL0CK
ERASE      0x000l8000             BLOCK 
[REGS]
DMM 1      OxFA200000
FILE           reg823．def
② U-Boot移植参考板，这是进行U．Boot移植首先要明确的。可以根据目标板上CPU、Flash、SDRAM的情况， 以尽可能相一致为原则，先找出一个与所移植目标板为同一个或同一系列处理器的u．Boot支持板为移植参考板。如RPXlite DW 板可选择U．Boot源码中RPXlite板作为U-Boot移植参考板。对U．Boot移植新手，
 建议依照循序渐进的原则， 目标板文件名暂时先用移植参考板的名称，在逐步熟悉U-BOOt移植基础上， 再考虑给目标板重新命名。在实际移植过程中，可用Linux命令查找移植参考板的特定代码，如grep．r RPXlite．／可确定出在U-Boot中与RPXlite板有关的代码，依此对照目标板实际进行屏蔽或修改。同时， 应不局限于移植参考板中的代码， 要广泛借鉴U．BOOt中已有的代码， 更好地实现一些具体的功能。
③ U-Boot烧写地址。不同目标板，对U-Boot在Flash中存放地址的要求不尽相同。事实上，这是由处理器中断复位向量来决定的， 与主板硬件相关。对MPC8xx主板来讲，就是由硬件配置字(HRCW)决定的。也就是说，U-Boot烧写具体位置是由硬件决定的， 而不是程序设计来选择的。程序中相应U-Boot起始地址必须与硬件所确定的硬件复位向量相吻合，如RPXlite
 DW 板的中断复位向量设置为0x00000100。因此，U．Boot的BIN镜像文件必须烧写到Flash的起始位置。事实上， 大多数的PPC系列的处理器中断复位向量是0x00000100和0xfff00100。这也是一般所说的高位启动和低位启动的Bootloader所在 位置。可通过修改u-BOot源码<目标板>．h头文件中CFG MONITOR BASE和board／<目标板>／config．mk中的TEX T — BASE的设置来与硬件配置相对应。
④ CPU寄存器参数设置。根据处理器系列、类型不同， 寄存器名称与作用有一定差别， 必须根据目标板的实际进行合理配置。一个较为可行和有效的方法是， 借鉴参考移植板的配置，再根据目标板实际，进行合理修改。这是一个较费功夫和考验耐力的过程， 需要仔细对照处理器各寄存器定义、参考设置、目标板实际作出选择并不断测试。MPC 8XX处理器较为关键的寄存器设置为SIUMCR、PLPRCR、SCCR、BRx和ORx。
⑤ 串口调试。能从串口输出信息，即使是乱码，也可以说U．Boot移植取得了实质性突破。依据笔者调试经历， 串口是否有输出，除了与串口驱动相关外，还与Flash相关的寄存器设置有关。因为U-Boot是从Flash中被引导启动的， 如果Flash设置不正确，U．BOot代码读取和执行就会出现一些问题。因此，还需要就Flash的相关寄存器设置进行一些参数调试。同时，
 要注意串口收发芯片相关引脚工作波形。依据笔者调试情况，如果串口无输出或出现乱码， 一种可能就是该芯片损坏或工作不正常。
⑥ 与启动Flash相关的寄存器BR0、OR0的参数设置，应根据目标板Flash的数据手册与BR0和OR0的相关位含义进行合理设置。这不仅关系到Flash能否正常工作， 而且与串口调试有直接的关联。
⑦ 关于CPLD电路。目标板上是否有CPLD电路丝毫不会影响U-B 00t的移植与嵌入式操作系统的正常运行。事实上，CPLD电路是一个集中将板上电路的一些逻辑关系可编程设置的一种实现方法。其本身所起的作用就是实现 一些目标板所需的脉冲信号和电路逻辑， 其功能完全可以用一些逻辑电路与CPU口线来实现。
⑧ SDRAM的驱动。串口能输出以后，U．Boot移植是否顺利基本取决于SDRAM 的驱动是否正确。与串口调试相比，这部分工作更为重要，难度更大。MPC8xx目标板SDRAM 驱动涉及三部分。一是相关寄存器的设置；二是UPM 表；三是SDRAM 上电初始化过程。任何一部分有问题，都会影响U-BOot、嵌入式操作系统甚至应用程序的稳定、可靠运行。所以说，SDRAM
 的驱动不仅关系到U-Boot本身能否正常运行， 而且还与后续部分相关， 是相当关键的部分。
⑨ 补充功能的添加。在获得一个能工作的U-BOot后， 就可以根据目标板和实际开发需要， 添加一些其它功能支持， 如以太网、L CD 、NVRAM 等。与串口和SDRAM 调试相比，在已有基础之上，添加这些功能还是较为容易的。大多只是在参考现有源码的基础上， 进行一些修改和配置。另外， 如果是在自主设计的主板上移植U-Boot， 那
么除了考虑上述软件因素以外， 还需要排查目标板硬件可能存在的问题， 如原理设计、PCB布线、元件好坏。
在移植过程中， 敏锐判断出故障态是硬件还是软件问题， 往往是关系到项目进度甚至移植成败的关键， 相应
难度会增加许多。
结语
       完成一个目标板的移植工作后， 可考虑将移植结果以补丁的形式发送到U-Boot用户邮件列表，尤其是一些参考板的移植结果。这是使用GPL代码并遵循GPL条款的体现。可在阅读 README相关补丁说明的基础上，添加适当的注释，将自己列入光荣榜(CREDITS)。如果愿意承担所移植板的后续更新工作， 可以考虑加入维护人员(MAINTAINERS)开发队伍行列。
       在实际的U-Boot移植中，无法避免地会遇到一些难以预料的问题， 甚至出现倒退， 尤其是U．BOOt移植新手， 更会平添诸多难度。但笔者相信， 在逐步熟悉U．BOOt的移植方法和过程中，随着自身经验的不断积累，加之有众多热衷于开放源码人士的鼎立相助， 坚冰终会消融。




















