# u-boot2010.03 第一阶段--start.S - 一世豁然的专栏 - CSDN博客





2015年06月24日 22:54:59[一世豁然](https://me.csdn.net/Explorer_day)阅读数：514标签：[bootloader																[u-boot](https://so.csdn.net/so/search/s.do?q=u-boot&t=blog)](https://so.csdn.net/so/search/s.do?q=bootloader&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








本博客转载于：[http://blog.csdn.net/yyttiao/article/details/7916495](http://blog.csdn.net/yyttiao/article/details/7916495)




经过前面的配置，编译分析。选择可以开始进入移植篇了。
其实我在想，是先出移植篇呢还是先出分析篇。因为这两者息息相关。整个uboot的启动流程懂了，那么移植也就不那么无从下手了。
我手上的板子是S3C6410的，如果一直对S3C6400 进行分析，那会不会影响到读者的分析。。
最终我决定，分析移植一起进行更新，，一般我会先出分析帖，然后再出教程帖。间隔应该不会特别长时间。
最近我同学说了，说我的帖子不够正式，没有转载说明，没有版权说明，，以后慢慢补上吧。


下面开始 分析篇(一)-----start.S
-----------------------------
使用环境 
PC:                  ubuntu 11.04
kernel:            2.6.32-28-generic
corss:             arm-linux-gcc 4.3.2
arm:                s3c6410
-----------------------------
作者:               LvApp
联系方式:       97164811@qq.com


一切版权均有作者所有，欢迎转载，请指明出处，如何修改请与本人联系，谢谢


今天，我们正式进入了uboot启动代码的分析，从上一次分析的makefile中，关于lds的说明，可以知道为什么要从这个文件开始，要是忘记了，可以去复习下。
[u-boot2010.03 配置编译目标分析](http://blog.csdn.net/yyttiao/article/details/7899008)
[u-boot2010.03 Makefile分析](http://blog.csdn.net/yyttiao/article/details/7899530)


这里就不细说了，arm上电启动执行的第一条语句就是在这个start.S中。
由于代码比较长，依旧采取边帖代码边分析的方式




**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- /*
- *************************************************************************
- *
- * Jump vector table as in table 3.1 in [1]
- *
- *************************************************************************
- */
- 
- 
- .globl _start  
- _start: b   reset  
-     #ifndef CONFIG_NAND_SPL
-     ldr pc, _undefined_instruction  
-     ldr pc, _software_interrupt  
-     ldr pc, _prefetch_abort  
-     ldr pc, _data_abort  
-     ldr pc, _not_used  
-     ldr pc, _irq  
-     ldr pc, _fiq  
- 
- 
- _undefined_instruction:  
-     .word undefined_instruction  
- _software_interrupt:  
-     .word software_interrupt  
- _prefetch_abort:  
-     .word prefetch_abort  
- _data_abort:  
-     .word data_abort  
- _not_used:  
-     .word not_used  
- _irq:  
-     .word irq  
- _fiq:  
-     .word fiq  
- _pad:  
-     .word 0x12345678 /* now 16*4=64 */
- #else
- . = _start + 64  
- #endif

首先进入的就是arm的异常项量表，这对于有过裸机开发的人应该不陌生，如果不了解这个，这里我就不多说，因为这不是这次的主题，可以去查阅相关资料，其实arm的中断入口地址就是在这里指定的
_start: b reset 这就是arm执行的第一条代码，直接通过跳转，进入到了reset地址处



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- /*
-  * the actual reset code
-  */
- reset:  
- /*
-  * set the cpu to SVC32 mode
-  */
-     mrs r0, cpsr            /* 把cpsr 放到r0 中 */
-     bic   r0, r0, #0x3f     /* 清 0x3f 对应位 11 1111 为0 */
-     orr    r0, r0, #0xd3    /* 或上 1011 0011 模式设置为SRC32 禁止中断和快速中断*/
-     msr cpsr, r0  

此处就是arm执行的第二条语句，将arm设置为src模式，并禁止了中断以及快速中断。可以查看s3c6410数据手册，查看cpsr寄存器的作用


下面开始初始化cpu了，这部分是关于协处理器的，我也不是很懂，想具体了解的人，可以去看协处理器手册



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- *************************************************************************  
- *  
- * CPU_init_critical registers  
- *  
- * setup important registers  
- * setup memory timing  
- *  
- *************************************************************************  
- */  
- /*
- * we do sys-critical inits only at reboot,
- * not when booting from ram!
- */
- cpu_init_crit:  
- /*
- * When booting from NAND - it has definitely been a reset, so, no need
- * to flush caches and disable the MMU
- */
- #ifndef CONFIG_NAND_SPL
- /*
- * flush v4 I/D caches
- */
- mov r0, #0  
- mcr p15, 0, r0, c7, c7, 0   /* flush v3/v4 cache */
- mcr p15, 0, r0, c8, c7, 0   /* flush v4 TLB */
- 
- 
- /*
- * disable MMU stuff and caches
- */
- mrc p15, 0, r0, c1, c0, 0  
- bic r0, r0, #0x00002300 @ clear bits 13, 9:8 (--V- --RS)  
- bic r0, r0, #0x00000087 @ clear bits 7, 2:0 (B--- -CAM)  
- orr r0, r0, #0x00000002 @ set bit 2 (A) Align  
- orr r0, r0, #0x00001000 @ set bit 12 (I) I-Cache  
- /* Prepare to disable the MMU */
- adr r1, mmu_disable_phys  
- /* We presume we're within the first 1024 bytes */
- and r1, r1, #0x3fc  
- ldr r2, _TEXT_PHY_BASE  
- ldr r3, =0xfff00000  
- and r2, r2, r3  
- orr r2, r2, r1  
- b   mmu_disable  
- 
- 
- .align 5  
- /* Run in a single cache-line */
- mmu_disable:  
- mcr p15, 0, r0, c1, c0, 0   /* 禁止MMU就是这一句了，没必要判断那么多，其实只要把这句话移到上面，直接禁止了就好了。反正现在就是要关掉对吧 */
- nop  
- nop  
- mov pc, r2  
- #endif
- 
- 
- /* 设置MMC映射地址，如果MMU开启，那么地址将被映射到0x70000000起始位置 0x13 为256MB 查看arm1176jzfs */
- mmu_disable_phys:  
- /* Peri port setup */
- ldr r0, =0x70000000  
- orr r0, r0, #0x13  
- mcr p15,0,r0,c15,c2,4 @ 256M (0x70000000 - 0x7fffffff)  
- 这里就是根据你的外部的内存大小来设置的，我这里是256MB，所以设置为0x13具体的数值根据arm1176jzfs手册可以查看到，s3c6410的核心就是arm1176  
- 
- 
- 下面这个就相对来说比较重要了，很比较复杂。我这里就先留个脚印，回头下一篇细细的分析  
- /*
- * Go setup Memory and board specific bits prior to relocation.
- */
- bl  lowlevel_init    /* go setup pll,mux,memory */
- 注释也说了，是初始化 pll mux memory 之类的，对于cpu的工作频率就是在这里设置的  
- 继续分析  
- after_copy: /* 注意这里噢。。有个after_copy，移植篇的时候，会在这里加上适当的代码 */
- #ifdef CONFIG_ENABLE_MMU
- enable_mmu:  
- /* enable domain access */
- ldr r5, =0x0000ffff  
- mcr p15, 0, r5, c3, c0, 0   /* load domain access register */
- 
- 
- /* Set the TTB register */
- ldr r0, _mmu_table_base  
- ldr r1, =CONFIG_SYS_PHY_UBOOT_BASE  
- ldr r2, =0xfff00000  
- bic r0, r0, r2  
- orr r1, r0, r1  
- mcr p15, 0, r1, c2, c0, 0  
- 
- 
- /* Enable the MMU */
- mrc p15, 0, r0, c1, c0, 0  
- orr r0, r0, #1   /* Set CR_M to enable MMU */
- 
- 
- /* Prepare to enable the MMU */
- adr r1, skip_hw_init  
- and r1, r1, #0x3fc  
- ldr r2, _TEXT_BASE  
- ldr r3, =0xfff00000  
- and r2, r2, r3  
- orr r2, r2, r1  
- b   mmu_enable  
- 
- 
- .align 5  
- /* Run in a single cache-line */
- mmu_enable:  
- 
- 
- mcr p15, 0, r0, c1, c0, 0  
- nop  
- nop  
- mov pc, r2  
- #endif

这段代码主要是是否启动mmu的，一般都不开启，直接跳过就好了。
Ps:看到那个after_copy了没？之前分析的时候，没说过要copy对不对？很奇怪吧。。。其实不然，看了下面的代码，你会发现，下面就剩下设置栈，clear
 bss以及启动第二阶段的部分了
也就是说，uboot 如果是从nand启动的，那么选择的pc指针还在 stepping stone内，而该区域只有4K的大小，有点小了。。。而对于nor flash启动的，以及其它media都有类似的问题，
不是速度慢就是没法直接执行代码，都需要将代码拷贝到ram中去。
移植的一个问题就出来了，在这里 添加拷贝代码。。



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- skip_hw_init:  
- /* Set up the stack  */
- stack_setup:  
- ldr r0, =CONFIG_SYS_UBOOT_BASE  /* base of copy in DRAM  */
- sub r0, r0, #CONFIG_SYS_MALLOC_LEN  /* malloc area */
- sub r0, r0, #CONFIG_SYS_GBL_DATA_SIZE /* bdinfo */
- sub sp, r0, #12  /* leave 3 words for abort-stack */

这一部分是设置栈，而栈设置的位置在哪呢？CONFIG_SYS_UBOOT_BASE 在include/configs/xxx.h中定义，此处我的是s3c6400.h 打开该文件，还是别关了，经常要看这个里面是不是定义了什么宏的



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- /* NAND U-Boot load and start address */
- #define CONFIG_SYS_SDRAM_BASE    0x50000000
- #define CONFIG_SYS_MAPPED_RAM_BASE  CONFIG_SYS_SDRAM_BASE
- #define CONFIG_SYS_UBOOT_BASE    (CONFIG_SYS_MAPPED_RAM_BASE + 0x07e00000)

最终这个CONFIG_SYS_UBOOT_BASE = 0x50000000 + 0x07e00000 为什么后面要加上0x07e00000（126MB）
 其实uboot在将自己拷贝到内存中的时候，会放到内存最大值往下2MB的位置，这里呢，uboot
默认就以为自己是128MB了。往下就是126MB的位置了，该值可以根据实际情况而改变，一般不改也没事的



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- #define CONFIG_ENV_SIZE  0x4000 /* Total Size of Environment Sector */
- #define CONFIG_SYS_MALLOC_LEN    (CONFIG_ENV_SIZE + 1024 * 1024)
- CONFIG_SYS_MALLOC_LEN = 0x4000 + 1024*1024  
- #define CONFIG_SYS_GBL_DATA_SIZE    128 /* size in bytes for initial data */

最终sp = 0x50000000 + 0x07e00000 - (0x4000 + 1024*1024) - 128 - 12



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- clear_bss:  
- ldr r0, _bss_start   /* find start of bss segment */
- ldr r1, _bss_end     /* stop here */
- mov r2, #0   /* clear */
- 
- 
- clbss_l:  
- str r2, [r0]     /* clear loop... */
- add r0, r0, #4  
- cmp r0, r1  
- ble clbss_l  

这一段就是 clear_bss 固定清为0
 由此可知bss也不是天生就是0，也是人为的原因



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7916495#)[copy](http://blog.csdn.net/yyttiao/article/details/7916495#)



- #ifndef CONFIG_NAND_SPL
- ldr pc, _start_armboot  
- 
- 
- _start_armboot:  
- .word start_armboot  
- #else
- b   nand_boot  
- /*  .word nand_boot*/
- #endif

进入最后一部分，就是一个ldr pc,_start_armboot 这个地址最终会保存start_armboot,启动第二阶段


finish.
thanks a lot.







