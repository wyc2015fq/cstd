# uboot-的start.S详细注解及分析 - 一世豁然的专栏 - CSDN博客





2015年03月13日 10:22:08[一世豁然](https://me.csdn.net/Explorer_day)阅读数：600








本博客转载于：[http://blog.chinaunix.net/uid-22891435-id-380150.html](http://blog.chinaunix.net/uid-22891435-id-380150.html)







**大多数bootloader都分为stage1和stage2两部分，u-boot也不例外。依赖于CPU体系结构的代码（如设备初始化代码等）通常都放在stage1且可以用汇编语言来实现，而stage2则通常用C语言来实现，这样可以实现复杂的功能，而且有更好的可读性和移植性。1、Stage1 start.S代码结构u-boot的stage1代码通常放在start.S文件中，他用汇编语言写成，其主要代码部分如下：（1）定义入口。由于一个可执行的Image必须有一个入口点，并且只能有一个全局入口，通常这个入口放在ROM（Flash）的0x0地址，因此，必须通知编译器以使其知道这个入口，该工作可通过修改连接器脚本来完成。（2）设置异常向量（Exception Vector）。（3）设置CPU的速度、时钟频率及终端控制寄存器。（4）初始化内存控制器。（5）将ROM中的程序复制到RAM中。（6）初始化堆栈。（7）转到RAM中执行，该工作可使用指令ldr pc来完成。2、Stage2 C语言代码部分lib_arm/board.c中的start arm boot是C语言开始的函数也是整个启动代码中C语言的主函数，同时还是整个u-boot（armboot）的主函数，该函数只要完成如下操作：（1）调用一系列的初始化函数。（2）初始化Flash设备。（3）初始化系统内存分配函数。（4）如果目标系统拥有NAND设备，则初始化NAND设备。（5）如果目标系统有显示设备，则初始化该类设备。（6）初始化相关网络设备，填写IP、MAC地址等。（7）进去命令循环（即整个boot的工作循环），接受用户从串口输入的命令，然后进行相应的工作。3、U-Boot的启动顺序(示例，其他u-boot版本类似)cpu/arm920t/start.S**



**@文件包含处理**



**#include <config.h>@由顶层的mkconfig生成，其中只包含了一个文件：configs/<顶层makefile中6个参数的第1个参数>.h#include <version.h>  #include <status_led.h>**



**/* ************************************************************************* * * Jump vector table as in table 3.1 in [1] * ************************************************************************* */**

注：ARM微处理器支持字节（8位）、半字（16位）、字（32位）3种数据类型**@向量跳转表，每条占四个字节（一个字），地址范围为0x0000 0000～@0x0000 0020@ARM体系结构规定在上电复位后的起始位置，必须有8条连续的跳**

**@转指令，通过硬件实现。他们就是异常向量表。ARM在上电复位后，@是从0x00000000开始启动的，其实如果bootloader存在，在执行**

**@下面第一条指令后，就无条件跳转到start_code，下面一部分并没@执行。设置异常向量表的作用是识别bootloader。以后系统每当有@异常出现，则CPU会根据异常号，从内存的0x00000000处开始查表@做相应的处理**

**/********************************************************

**;当一个异常出现以后，ARM会自动执行以下几个步骤:;1.把下一条指令的地址放到连接寄存器LR(通常是R14).---保存位置;2.将相应的CPSR(当前程序状态寄存器)复制到SPSR(备份的程序状态寄存器)中---保存CPSR;3.根据异常类型，强制设置CPSR的运行模式位;4.强制PC(程序计数器)从相关异常向量地址取出下一条指令执行，从而跳转到相应的异常处理程序中*********************************************************/**





**.globl _start  /*系统复位位置,整个程序入口*/@_start是GNU汇编器的默认入口标签，.globl将_start声明为外部程序可访问的标签，.globl是GNU汇编的保留关键字，前面加点是GNU汇编的语法_start: b       start_code   @0x00@ARM上电后执行的第一条指令，也即复位向量，跳转到start_code**

**@reset用b，就是因为reset在MMU建立前后都有可能发生@其他的异常只有在MMU建立之后才会发生ldr pc, _undefined_instruction /*未定义指令异常,0x04*/   ldr pc, _software_interrupt   /*软中断异常,0x08*/   ldr pc, _prefetch_abort    /*内存操作异常,0x0c*/   ldr pc, _data_abort     /*数据异常,0x10*/   ldr pc, _not_used     /*未适用,0x14*/   ldr pc, _irq      /*慢速中断异常,0x18*/   ldr pc, _fiq      /*快速中断异常,0x1c*/**

**@对于ARM数据从内存到CPU之间的移动只能通过L/S指令，如：ldr r0,0x12345678为把0x12345678内存中的数据写到r0中，还有一个就是ldr伪指令，如：ldr r0,=0x12345678为把0x12345678地址写到r0中，mov只能完成寄存器间数据的移动，而且立即数长度限制在8位**



**_undefined_instruction: .word undefined_instruction_software_interrupt: .word software_interrupt_prefetch_abort: .word prefetch_abort_data_abort:  .word data_abort_not_used:  .word not_used_irq:   .word irq_fiq:   .word fiq@.word为GNU ARM汇编特有的伪操作，为分配一段字内存单元（分配的单元为字对齐的），可以使用.word把标志符作为常量使用。如_fiq:.word fiq即把fiq存入内存变量_fiq中，也即是把fiq放到地址_fiq中。**



**.balignl 16,0xdeadbeef@.balignl是.balign的变体**

**@ .align伪操作用于表示对齐方式：通过添加填充字节使当前位置**

**@满足一定的对齐方式。.balign的作用同.align。@ .align {alignment} {,fill} {,max}@其中：alignment用于指定对齐方式，可能的取值为2的次**

**@幂，缺省为4。fill是填充内容，缺省用0填充。max是填充字节@数最大值，如果填充字节数超过max,  就不进行对齐,例如：@  .align 4  /* 指定对齐方式为字对齐 */**

**【参考好野人的窝，于关u-boot中的.balignl 16,0xdeadbeef的理解[http://haoyeren.blog.sohu.com/84511571.html](http://student.csdn.net/link.php?url=http://haoyeren.blog.sohu.com%2F84511571.html)】**







**/* ************************************************************************* * * Startup Code (called from the ARM reset exception vector) * * do important init only if we don't start from memory! * relocate armboot to ram * setup stack * jump to second stage * ***************************************************************************

**@保存变量的数据区,保存一些全局变量,用于BOOT程序从FLASH拷贝@到RAM,或者其它的使用。@还有一些变量的长度是通过连接脚本里得到,实际上由编译器算出**

**@来的**

**_TEXT_BASE: **

**@因为linux开始地址是0x30000000,我这里是64M SDRAM,所以@TEXT_BASE = 0x33F80000 ？？？ .word TEXT_BASE /*uboot映像在SDRAM中的重定位地址*/@TEXT_BASE在开发板相关的目录中的config.mk文档中定义, 他定**

**@义了代码在运行时所在的地址,那么_TEXT_BASE中保存了这个地**

**@址（这个TEXT_BASE怎么来的还不清楚）**





**.globl _armboot_start_armboot_start: .word _start@用_start来初始化_armboot_start。（为什么要这么定义一下还不明白）**





**/* * These are defined in the board-specific linker script. */@下面这些是定义在开发板目录链接脚本中的**



**.globl _bss_start   _bss_start: .word __bss_start@__bss_start定义在和开发板相关的u-boot.lds中，_bss_start保存的是__bss_start标号所在的地址。**



**.globl _bss_end_bss_end: .word _end@同上，这样赋值是因为代码所在地址非编译时的地址，直接取得该标号对应地址。**



**@中断的堆栈设置**



**#ifdef CONFIG_USE_IRQ/* IRQ stack memory (calculated at run-time) */.globl IRQ_STACK_STARTIRQ_STACK_START: .word 0x0badc0de**



**/* IRQ stack memory (calculated at run-time) */.globl FIQ_STACK_STARTFIQ_STACK_START: .word 0x0badc0de#endif**



**/* * the actual start code */@复位后执行程序@真正的初始化从这里开始了。其实在CPU一上电以后就是跳到这里执行的reset: /*  * set the cpu to SVC32 mode  */@更改处理器模式为管理模式@对状态寄存器的修改要按照：读出-修改-写回的顺序来执行@    31 30 29 28 ---   7   6   -   4    3   2   1   0    N  Z  C  V        I   F       M4  M3  M2 M1 M0                                   0   0   0  0   0     User26 模式                                   0   0   0  0   1     FIQ26 模式                                   0   0   0  1   0     IRQ26 模式                                   0   0   0  1   1     SVC26 模式                                   1   0   0  0   0     User 模式                                   1   0   0  0   1     FIQ 模式                                   1   0   0  1   0     IRQ 模式                                   1   0   0  1   1     SVC 模式                                   1   0   1  1   1     ABT 模式                                   1   1   0  1   1     UND 模式                                   1   1   1  1   1     SYS 模式**



** mrs r0,cpsr@将cpsr的值读到r0中 bic r0,r0,#0x1f@清除M0~M4 orr r0,r0,#0xd3@禁止IRQ,FIQ中断，并将处理器置于管理模式 msr cpsr,r0**



**@以下是点灯了，这里应该会牵涉到硬件设置，移植的时候应该可以不要 bl coloured_LED_init bl red_LED_on**



**@针对AT91RM9200进行特殊处理#if defined(CONFIG_AT91RM9200DK) || defined(CONFIG_AT91RM9200EK) /*  * relocate exception table  */ ldr r0, =_start ldr r1, =0x0 mov r2, #16copyex: subs r2, r2, #1@sub带上了s用来更改进位标志，对于sub来说，若发生借位则C标志置0，没有则为1，这跟adds指令相反！要注意。 ldr r3, [r0], #4 str r3, [r1], #4 bne copyex#endif**



**@针对S3C2400和S3C2410进行特殊处理@CONFIG_S3C2400、CONFIG_S3C2410等定义在include/configs/下不同开发板的头文件中#if defined(CONFIG_S3C2400) || defined(CONFIG_S3C2410) /* turn off the watchdog */**



**@关闭看门狗定时器的自动复位功能并屏蔽所有中断，上电后看门狗为开，中断为关# if defined(CONFIG_S3C2400)#  define pWTCON  0x15300000#  define INTMSK  0x14400008 /* Interupt-Controller base addresses */#  define CLKDIVN 0x14800014 /* clock divisor register */#else @s3c2410的配置#  define pWTCON  0x53000000  @pWTCON定义为看门狗控制寄存器的地址（s3c2410和s3c2440相同）#  define INTMSK  0x4A000008 /* Interupt-Controller base addresses */@INTMSK定义为主中断屏蔽寄存器的地址（s3c2410和s3c2440相同）#  define INTSUBMSK  0x4A00001C@INTSUBMSK定义为副中断屏蔽寄存器的地址（s3c2410和s3c2440相同）#  define CLKDIVN  0x4C000014 /* clock divisor register */@CLKDIVN定义为时钟分频控制寄存器的地址（s3c2410和s3c2440相同）# endif@至此寄存器地址设置完毕**



** ldr     r0, =pWTCON mov     r1, #0x0 str     r1, [r0]@对于S3C2440和S3C2410的WTCON寄存器的[0]控制允许或禁止看门狗定时器的复位输出功能，设置为“0”禁止复位功能。**



** /*  * mask all IRQs by setting all bits in the INTMR - default  */ mov r1, #0xffffffff ldr r0, =INTMSK str r1, [r0]# if defined(CONFIG_S3C2410) ldr r1, =0x3ff  @2410好像应该为7ff才对（不理解uboot为何是这个数字） ldr r0, =INTSUBMSK str r1, [r0]# endif@对于S3C2410的INTMSK寄存器的32位和INTSUBMSK寄存器的低11位每一位对应一个中断，相应位置“1”为不响应相应的中断。对于S3C2440的INTSUBMSK有15位可用，所以应该为0x7fff了。**



** /* FCLK:HCLK:PCLK = 1:2:4 */ /* default FCLK is 120 MHz ! */ ldr r0, =CLKDIVN mov r1, #3 str r1, [r0]@时钟分频设置，FCLK为核心提供时钟，HCLK为AHB（ARM920T,内存@控制器，中断控制器，LCD控制器，DMA和主USB模块）提供时钟，@PCLK为APB（看门狗、IIS、I2C、PWM、MMC、ADC、UART、GPIO、@RTC、SPI）提供时钟。分频数一般选择1：4：8，所以HDIVN=2,PDIVN=1，@CLKDIVN=5，这里仅仅是配置了分频寄存器，关于MPLLCON的配置肯@定写在lowlevel_init.S中了@归纳出CLKDIVN的值跟分频的关系：@0x0 = 1:1:1  ,  0x1 = 1:1:2 , 0x2 = 1:2:2  ,  0x3 = 1:2:4,  0x4 = 1:4:4,  0x5 = 1:4:8, 0x6 = 1:3:3,0x7 = 1:3:6@S3C2440的输出时钟计算式为:Mpll=(2*m*Fin)/(p*2^s)S3C2410的输出时钟计算式为:Mpll=(m*Fin)/(p*2^s)m=M(the value for divider M)+8;p=P(the value for divider P)+2M,P,S的选择根据datasheet中PLL VALUE SELECTION TABLE表格进行，我的开发板晶振为16.9344M，所以输出频率选为：399.65M的话M=0x6e,P=3,S=1@s3c2440增加了摄像头,其FCLK、HCLK、PCLK的分频数还受到CAMDIVN[9]（默认为0）,CAMDIVN[8]（默认为0）的影响#endif /* CONFIG_S3C2400 || CONFIG_S3C2410 */**



** /*  * we do sys-critical inits only at reboot,  * not when booting from ram!  */@选择是否初始化CPU#ifndef CONFIG_SKIP_LOWLEVEL_INIT bl cpu_init_crit@执行CPU初始化，BL完成跳转的同时会把后面紧跟的一条指令地址保存到连接寄存器LR（R14）中。以使子程序执行完后正常返回。#endif**



**@调试阶段的代码是直接在RAM中运行的，而最后需要把这些代码@固化到Flash中，因此U-Boot需要自己从Flash转移到@RAM中运行，这也是重定向的目的所在。@通过adr指令得到当前代码的地址信息：如果U-boot是从RAM@开始运行，则从adr,r0,_start得到的地址信息为@r0=_start=_TEXT_BASE=TEXT_BASE=0x33F80000; @如果U-boot从Flash开始运行，即从处理器对应的地址运行，@则r0=0x0000,这时将会执行copy_loop标识的那段代码了。@ _TEXT_BASE 定义在board/smdk2410/config.mk中#ifndef CONFIG_SKIP_RELOCATE_UBOOTrelocate:    /* relocate U-Boot to RAM     */ adr r0, _start  /* r0 <- current position of code   */ ldr r1, _TEXT_BASE  /* test if we run from flash or RAM */ cmp     r0, r1  /* don't reloc during debug         */ beq     stack_setup ldr r2, _armboot_start@_armboot_start为_start地址 ldr r3, _bss_start@_bss_start为数据段地址 sub r2, r3, r2  /* r2 <- size of armboot            */ add r2, r0, r2  /* r2 <- source end address         */**



**copy_loop: ldmia r0!, {r3-r10}  /* copy from source address [r0]    */**

**@从源地址[r0]读取8个字节到寄存器,每读一个就更新一次r0地址@ldmia:r0安字节增长 stmia r1!, {r3-r10}  /* copy to   target address [r1]    */@LDM(STM)用于在寄存器所指的一片连续存储器和寄存器列表的寄存@器间进行数据移动，或是进行压栈和出栈操作。@格式为：LDM(STM){条件}{类型}基址寄存器{！}，寄存器列表{^}@对于类型有以下几种情况： IA 每次传送后地址加1，用于移动数**

**@据块    IB 每次传送前地址加1，用于移动数据块    DA 每次传送后地址减1，用于移动数据块    DB 每次传送前地址减1，用于移动数据块    FD 满递减堆栈，用于操作堆栈（即先移动指针再操作数据，相当于DB）    ED 空递减堆栈，用于操作堆栈（即先操作数据再移动指针，相当于DA）    FA 满递增堆栈，用于操作堆栈（即先移动指针再操作数据，相当于IB）    EA 空递增堆栈，用于操作堆栈（即先操作数据再移动指针，相当于IA）（这里是不是应该要涉及到NAND或者NOR的读写？没有看出来）**



** cmp r0, r2   /* until source end addreee [r2]    */ ble copy_loop#endif /* CONFIG_SKIP_RELOCATE_UBOOT */**



** /* Set up the stack          */@初始化堆栈stack_setup:**

**ldr r0, _TEXT_BASE  /* upper 128 KiB: relocated uboot   */**

**@获取分配区域起始指针，**

**sub r0, r0, #CONFIG_SYS_MALLOC_LEN /* malloc area    */**

**@CFG_MALLOC_LEN=128*1024+CFG_ENV_SIZE=128*1024+0x1@0000=192K**

**sub r0, r0, #CONFIG_SYS_GBL_DATA_SIZE/* bdinfo   */**

**@CFG_GBL_DATA_SIZE    128---size in bytes reserved for initial data用来存储开发板信息#ifdef CONFIG_USE_IRQ**

**@这里如果需要使用IRQ,还有给IRQ保留堆栈空间,一般不使用. sub r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)#endif sub sp, r0, #12  /* leave 3 words for abort-stack    */**



**@该部分将未初始化数据段_bss_start----_bss_end中的数据@清零clear_bss: ldr r0, _bss_start  /* find start of bss segment        */ ldr r1, _bss_end  /* stop here                        */ mov r2, #0x00000000  /* clear                            */**



**clbss_l:str r2, [r0]  /* clear loop...                    */ add r0, r0, #4 cmp r0, r1 ble clbss_l**



**@跳到阶段二C语言中去 ldr pc, _start_armboot**



**_start_armboot: .word start_armboot@start_armboot在/lib_arm/中，到这里因该是第一阶段已经完成了吧，下面就要去C语言中执行第二阶段了吧**



**/* ************************************************************************* * * CPU_init_critical registers * * setup important registers * setup memory timing * ************************************************************************* */@CPU初始化**

**@在“relocate: /* relocate U-Boot to RAM */ ”之前被调用**

**#ifndef CONFIG_SKIP_LOWLEVEL_INITcpu_init_crit:      /*  * flush v4 I/D caches  */@初始化CACHES mov r0, #0 mcr p15, 0, r0, c7, c7, 0 /* flush v3/v4 cache */ mcr p15, 0, r0, c8, c7, 0 /* flush v4 TLB */**



** /*  * disable MMU stuff and caches  */@关闭MMU和CACHES mrc p15, 0, r0, c1, c0, 0 bic r0, r0, #0x00002300 @ clear bits 13, 9:8 (--V- --RS) bic r0, r0, #0x00000087 @ clear bits 7, 2:0 (B--- -CAM) orr r0, r0, #0x00000002 @ set bit 2 (A) Align orr r0, r0, #0x00001000 @ set bit 12 (I) I-Cache mcr p15, 0, r0, c1, c0, 0@对协处理器的操作还是看不懂，暂时先不管吧，有时间研究一下ARM技术手册的协处理器部分。**



** /*  * before relocating, we have to setup RAM timing  * because memory timing is board-dependend, you will  * find a lowlevel_init.S in your board directory.  */@初始化RAM时钟，因为内存是跟开发板密切相关的，所以这部分在/开发板目录/lowlevel_init.S中实现 mov ip, lr@保存LR,以便正常返回,注意前面是通过BL跳到cpu_init_crit来的。@（ARM9有37个寄存器，ARM7有27个）37个寄存器=7个未分组寄存器（R0～R7）+ 2×（5个分组寄存器R8～R12）+6×2（R13=SP，R14=lr 分组寄存器） + 1(R15=PC) +1(CPSR) + 5(SPSR)用途和访问权限：R0～R7:USR(用户模式)、fiq（快速中断模式）、irq（中断模式）、svc（超级用法模式）、abt、undR8～R12：R8_usr～R12_usr（usr，irq，svc，abt，und）         R8_fiq～R12_fiq（fiq） R11=fpR12=IP(从反汇编上看，fp和ip一般用于存放SP的值)R13～R14：R13_usr R14_usr(每种模式都有自己的寄存器)SP ～lr ：R13_fiq R14_fiq          R13_irq R14_irq          R13_svc R14_svc          R13_abt R14_abt          R13_und R14_undR15(PC)：都可以访问（即PC的值为当前指令的地址值加8个字节）R16    ：（(Current Program Status Register，当前程序状态寄存器)）           SPSR _fiq,SPSR_irq,SPSR_abt,SPSR_und(USR模式没有)**



**#if defined(CONFIG_AT91RM9200EK)**



**#else bl lowlevel_init**

**@在重定向代码之前，必须初始化内存时序，因为重定向时需要将@flash中的代码复制到内存中lowlevel_init在@/board/smdk2410/lowlevel_init.S中。               **



**#endif mov lr, ip mov pc, lr@返回到主程序**



**#endif /* CONFIG_SKIP_LOWLEVEL_INIT */**



**/* ************************************************************************* * * Interrupt handling * ************************************************************************* */@这段没有看明白，不过好像跟移植关系不是很大，先放一放。@@ IRQ stack frame.@#define S_FRAME_SIZE 72**



**#define S_OLD_R0 68#define S_PSR  64#define S_PC  60#define S_LR  56#define S_SP  52**



**#define S_IP  48#define S_FP  44#define S_R10  40#define S_R9  36#define S_R8  32#define S_R7  28#define S_R6  24#define S_R5  20#define S_R4  16#define S_R3  12#define S_R2  8#define S_R1  4#define S_R0  0**



**#define MODE_SVC 0x13#define I_BIT  0x80**



**/* * use bad_save_user_regs for abort/prefetch/undef/swi ... * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling */**



** .macro bad_save_user_regs sub sp, sp, #S_FRAME_SIZE stmia sp, {r0 - r12}   @ Calling r0-r12 ldr r2, _armboot_start sub r2, r2, #(CONFIG_STACKSIZE) sub r2, r2, #(CONFIG_SYS_MALLOC_LEN) sub r2, r2, #(CONFIG_SYS_GBL_DATA_SIZE+8)  @ set base 2 words into abort stack ldmia r2, {r2 - r3}   @ get pc, cpsr add r0, sp, #S_FRAME_SIZE  @ restore sp_SVC**



** add r5, sp, #S_SP mov r1, lr stmia r5, {r0 - r3}   @ save sp_SVC, lr_SVC, pc, cpsr mov r0, sp .endm**



** .macro irq_save_user_regs sub sp, sp, #S_FRAME_SIZE stmia sp, {r0 - r12}   @ Calling r0-r12 add     r7, sp, #S_PC stmdb   r7, {sp, lr}^                   @ Calling SP, LR str     lr, [r7, #0]                    @ Save calling PC mrs     r6, spsr str     r6, [r7, #4]                    @ Save CPSR str     r0, [r7, #8]                    @ Save OLD_R0 mov r0, sp .endm**



** .macro irq_restore_user_regs ldmia sp, {r0 - lr}^   @ Calling r0 - lr mov r0, r0 ldr lr, [sp, #S_PC]   @ Get PC add sp, sp, #S_FRAME_SIZE subs pc, lr, #4   @ return & move spsr_svc into cpsr .endm**



** .macro get_bad_stack ldr r13, _armboot_start  @ setup our mode stack sub r13, r13, #(CONFIG_STACKSIZE) sub r13, r13, #(CONFIG_SYS_MALLOC_LEN) sub r13, r13, #(CONFIG_SYS_GBL_DATA_SIZE+8) @ reserved a couple spots in abort stack**



** str lr, [r13]   @ save caller lr / spsr mrs lr, spsr str     lr, [r13, #4]**



** mov r13, #MODE_SVC   @ prepare SVC-Mode @ msr spsr_c, r13 msr spsr, r13 mov lr, pc movs pc, lr .endm**



** .macro get_irq_stack   @ setup IRQ stack ldr sp, IRQ_STACK_START .endm**



** .macro get_fiq_stack   @ setup FIQ stack ldr sp, FIQ_STACK_START .endm**



**/********************************************************* * exception handlers ********************************************************/@异常向量处理@每一个异常向量处其实只放了一条跳转指令（因为每个异常向量只 @有4个字节不能放太多的程序），跳到相应的异常处理程序中。 .align  5undefined_instruction: get_bad_stack bad_save_user_regs bl do_undefined_instruction**



** .align 5software_interrupt: get_bad_stack bad_save_user_regs bl do_software_interrupt**



** .align 5prefetch_abort: get_bad_stack bad_save_user_regs bl do_prefetch_abort**



** .align 5data_abort: get_bad_stack bad_save_user_regs bl do_data_abort**



** .align 5not_used: get_bad_stack bad_save_user_regs bl do_not_used**



**#ifdef CONFIG_USE_IRQ**



** .align 5irq: get_irq_stack irq_save_user_regs bl do_irq irq_restore_user_regs**



** .align 5fiq: get_fiq_stack /* someone ought to write a more effiction fiq_save_user_regs */ irq_save_user_regs bl do_fiq irq_restore_user_regs**



**#else**



** .align 5irq: get_bad_stack bad_save_user_regs bl do_irq**



** .align 5fiq: get_bad_stack bad_save_user_regs bl do_fiq**


**#endif /*CONFIG_USE_IRQ*/@可知start.S的流程为：异常向量——上电复位后进入复位异常向量——跳到启动代码处——设置处理器进入管理模式——关闭看门狗——关闭中断——设置时钟分频——关闭MMU和CACHE——进入lowlever_init.S——检查当前代码所处的位置，如果在FLASH中就将代码搬移到RAM中**



