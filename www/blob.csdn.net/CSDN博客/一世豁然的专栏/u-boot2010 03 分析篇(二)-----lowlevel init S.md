# u-boot2010.03 分析篇(二)-----lowlevel.init.S - 一世豁然的专栏 - CSDN博客





2015年06月24日 22:57:40[一世豁然](https://me.csdn.net/Explorer_day)阅读数：541标签：[bootloader																[u-boot](https://so.csdn.net/so/search/s.do?q=u-boot&t=blog)](https://so.csdn.net/so/search/s.do?q=bootloader&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








本博客转载于：[http://blog.csdn.net/yyttiao/article/details/7933684](http://blog.csdn.net/yyttiao/article/details/7933684)




分析篇(二)-----lowlevel.init.S

-----------------------------

使用环境 

PC:     ubuntu 11.04

kernel: 2.6.32-28-generic

corss:  arm-linux-gcc 4.3.2

arm:    s3c6410

-----------------------------

作者: LvApp

联系方式: 97164811@qq.com


一切版权均有作者所有，欢迎转载，请指明出处，如何修改请与本人联系，谢谢


今天主要分析上次说的lowlevel_init.S 这个文件board/samsung/smdk6400/lowlevel_init.S

这个文件内的**lowlevel_init** 就是系统初始化的时候被调用的，在这里面完成很多事情，主要为：

1：led设置

2：禁止看门狗

3：禁止所有的中断

4：设置系统时钟

5：设置nand相关的寄存器

6：设置内存blank相关寄存器

等


下面直接看代码：




**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7933684#)[copy](http://blog.csdn.net/yyttiao/article/details/7933684#)



- lowlevel_init:  
- mov r12, lr  
- /* 对LED的设置，这里可以根据自己的板子外接的led进行配置，或者直接无视也行 */
- /* LED on only #8 */
- ldr r0, =ELFIN_GPIO_BASE  
- ldr r1, =0x55540000  
- str r1, [r0, #GPNCON_OFFSET]  
- 
- ldr r1, =0x55555555  
- str r1, [r0, #GPNPUD_OFFSET]  
- 
- ldr r1, =0xf000  
- str r1, [r0, #GPNDAT_OFFSET]  
- 
- /* Disable Watchdog */
- ldr r0, =0x7e000000  @0x7e004000  
- orr r0, r0, #0x4000  
- mov r1, #0  
- str r1, [r0]  
- /* 此处可见，将0x7E004000 地址处写入了0x00这个值 */

参看 WTCON(0x7E004000) 寄存器可知，第五位是启动和禁止看门狗的位，0是禁止。这里干脆直接往这个寄存器写0，这样就肯定可以禁止看门狗了
有图可知，该位默认是为1的。也就是说，看门狗默认是启动的，如果不禁止，那么有可能导致系统不停的复位，以至于无法启动，所以在uboot启动阶段，
禁止看门狗，而且看门狗需要放到相对比较靠前的位置，可以让系统复位之后，可以保证能够执行到该指定去关闭看门狗，否则在关闭之前系统就复位了。
![](https://img-my.csdn.net/uploads/201209/02/1346522963_5905.png)
这里开始就是跟中断相关了



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7933684#)[copy](http://blog.csdn.net/yyttiao/article/details/7933684#)



- /* External interrupt pending clear */
- ldr r0, =(ELFIN_GPIO_BASE+EINTPEND_OFFSET)  /*EINTPEND*/
- ldr r1, [r0]  
- str r1, [r0]  
- 
- ldr r0, =ELFIN_VIC0_BASE_ADDR   @0x71200000  
- ldr r1, =ELFIN_VIC1_BASE_ADDR   @0x71300000  
- 
- /* Disable all interrupts (VIC0 and VIC1) */
- mvn r3, #0x0  
- str r3, [r0, #oINTMSK]  
- str r3, [r1, #oINTMSK]  
- 
- /* Set all interrupts as IRQ */
- mov r3, #0x0  
- str r3, [r0, #oINTMOD]  
- str r3, [r1, #oINTMOD]  
- 
- /* Pending Interrupt Clear */
- mov r3, #0x0  
- str r3, [r0, #oVECTADDR]  
- str r3, [r1, #oVECTADDR]  
- 
- /* init system clock */
- /* 
- * 系统时钟的配置相对比较重要，s3c6410可以跑到500Mhz+ 呢。所以必须得设置好了
- * 设置过程有点长，一会在下面进行分析
- */
- bl system_clock_init  
- 
- #ifndef CONFIG_NAND_SPL
- /* for UART */
- bl uart_asm_init  
- #endif
- 
- #ifdef CONFIG_BOOT_NAND
- /* simple init for NAND */
- /* 
- * 如果是从nand启动的uboot，那就离不开nand_copy代码了，要使用nand 那就需要对nand进行初始化。
- * 也是在下面讲解
- */
- bl nand_asm_init  
- #endif
- /*
- * 内存初始化
- */
- /* Memory subsystem address 0x7e00f120 */
- ldr r0, =ELFIN_MEM_SYS_CFG  
- 
- /* Xm0CSn2 = NFCON CS0, Xm0CSn3 = NFCON CS1 */
- mov r1, #S3C64XX_MEM_SYS_CFG_NAND  
- str r1, [r0]  
- /* 进入内存配置 cpu_init.S */
- bl  mem_ctrl_asm_init  
- 
- /* Wakeup support. Don't know if it's going to be used, untested. */
- ldr r0, =(ELFIN_CLOCK_POWER_BASE + RST_STAT_OFFSET)  
- ldr r1, [r0]  
- bic r1, r1, #0xfffffff7  
- cmp r1, #0x8  
- beq wakeup_reset  
- 
- 1:  
- mov lr, r12  
- mov pc, lr  

至此，整个初始化就完成了。可以看到，这个函数内，完成的任务非常艰巨，可谓是功不可没呀~~下面我们仔细分析下，都具体完成了些什么内容。


system_clock_init:
对于s3c6410时钟频率如何计算,其寄存器的介绍,总线的介绍,请先参阅下面这个链接
S3C6410 系统时钟介绍 [http://blog.csdn.net/yyttiao/article/details/7933646](http://blog.csdn.net/yyttiao/article/details/7933646)
有了上面这个帖子的介绍之后,我们开始分析代码...这里会发现特别简单...
摘取了关键部分代码



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7933684#)[copy](http://blog.csdn.net/yyttiao/article/details/7933684#)



- /* 设置系统时钟的来源，此处为MPLL 默认也是该值 */
- ldr r1, [r0, #OTHERS_OFFSET]  
- bic r1, r1, #0x40    /* syncmux = 0 moutmpll */
- /* sync mux 的时钟来自mpll 倍频器，为1 就是来自 内核频率(典型值533mhz) */
- str r1, [r0, #OTHERS_OFFSET]  
- #endif
- /* 设置PLL锁住时间，默认为最长时间，该值表示在改变频率期间，CPU终止脉冲输出的时间 */
- mov r1, #0xff00  
- orr r1, r1, #0xff  
- str r1, [r0, #APLL_LOCK_OFFSET]  
- str r1, [r0, #MPLL_LOCK_OFFSET]  
- 
- /* Set Clock Divider */
- /* 设置分频比 */
- ldr r1, [r0, #CLK_DIV0_OFFSET]  
- bic r1, r1, #0x30000  
- bic r1, r1, #0xff00  
- bic r1, r1, #0xff  
- 
- /* CLK_DIV_VAL 这是一个宏,进入该宏,会得到设置好的值.直接写入PLL就好了 */
- ldr r2, =CLK_DIV_VAL     /* 设置时钟分频比，根据configs/smdk6410.h设置，改变include/arch-s3c64xx/s3c6410.h内对分频比的设置 */
- 
- orr r1, r1, r2  
- str r1, [r0, #CLK_DIV0_OFFSET]  
- 
- /* 设置APLL的分频比以及使能APLL */
- /* 公式FOUT = M x Fin / ( P x 2 x s ) */
- ldr r1, =APLL_VAL  
- str r1, [r0, #APLL_CON_OFFSET]  
- 
- /* 设置MPLL的分频比以及使能MPLL */
- ldr r1, =MPLL_VAL  
- str r1, [r0, #MPLL_CON_OFFSET]  
- 
- /* FOUT of EPLL is 96MHz */
- /* Fout = (M + K / 2^16) * Fin / (P x 2^s) */
- ldr r1, =0x200203  
- str r1, [r0, #EPLL_CON0_OFFSET]  
- ldr r1, =0x0  
- str r1, [r0, #EPLL_CON1_OFFSET]  
- 
- /* APLL, MPLL, EPLL select to Fout */
- /* 选择时钟源，是经过PLL倍频还是直接外部引脚 */
- ldr r1, [r0, #CLK_SRC_OFFSET]  
- orr r1, r1, #0x7  
- str r1, [r0, #CLK_SRC_OFFSET]  
- 
- /* wait at least 200us to stablize all clock */
- mov r1, #0x10000  
- 1:  subs    r1, r1, #1  
- bne 1b  

关于系统时钟频率,到这就介绍完了...还有不明白了,请留言...我会及时回复的.....


**nand_asm_init:**
我们经常会将uboot放到nand中,因为nor比较贵..所以常用的就是nand了.在使用nand搬移uboot的时候,就得设置nand了..
对于nand只要操作寄存器就好了.因为芯片给我们做好了硬件控制器.而nand访问其实是有时间的,不同的nand芯片,所需要的
总线访问时间是不同的,但是现在我们还不知道外围的设备是什么型号,所以,比较麻烦,当然我们自己用,肯定知道芯片的时间了
但是为了更好的支持,一般在第一阶段,会把nand的总线访问时间,设置为最大时间,这样就比较通用了.在第二阶段的时候,再
去判断芯片的类型,设置比较合理的总线访问时间...这样会比较好...
代码比较简单...先看个图...这里不详细介绍nand的硬件框架图..以后会补上.跟上面介绍时钟一样...
在NAND中有一个寄存器叫做 NCONF 下面是该寄存器中,对时序设置的几个配置,第一阶段都将这些值设置为最高.
![](https://img-my.csdn.net/uploads/201209/02/1346522848_4743.png)



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7933684#)[copy](http://blog.csdn.net/yyttiao/article/details/7933684#)



- /*
-  * NAND Interface init for SMDK6400
-  */
- nand_asm_init:  
- ldr r0, =ELFIN_NAND_BASE  
- /* 此处由于是第一次获取，读取到的应该是复位值 0x0000100X
-  * 此处设置了nand时序线上的延时时间，采用最大值 TACLS TWRPH0 TWRPH1 均为7
-  * 
-  * TACLS = HCLK * TACLS 
-  * TWRPH0 = HCLK * (TWRPH0 + 1)
-  * TWRPH1 = HCLK * (TWRPH1 + 1)
-  */
- ldr r1, [r0, #NFCONF_OFFSET]  
- orr r1, r1, #0x70  
- orr r1, r1, #0x7700  
- str r1, [r0, #NFCONF_OFFSET]  
- 
- /* 开始nand控制器，禁止2个NAND芯片，未使能 */
- ldr r1, [r0, #NFCONT_OFFSET]  
- orr r1, r1, #0x07  
- str r1, [r0, #NFCONT_OFFSET]  
- 
- mov pc, lr  
- /*
- * NAND Interface init for SMDK6400
- */
- nand_asm_init:  
- ldr r0, =ELFIN_NAND_BASE  
- /* 此处由于是第一次获取，读取到的应该是复位值 0x0000100X
-  * 此处设置了nand时序线上的延时时间，采用最大值 TACLS TWRPH0 TWRPH1 均为7
-  * 
-  * TACLS = HCLK * TACLS 
-  * TWRPH0 = HCLK * (TWRPH0 + 1)
-  * TWRPH1 = HCLK * (TWRPH1 + 1)
-  */
- ldr r1, [r0, #NFCONF_OFFSET]  
- orr r1, r1, #0x70  
- orr r1, r1, #0x7700  
- str r1, [r0, #NFCONF_OFFSET]  
- 
- /* 开始nand控制器，禁止2个NAND芯片，未使能 */
- ldr r1, [r0, #NFCONT_OFFSET]  
- orr r1, r1, #0x07  
- str r1, [r0, #NFCONT_OFFSET]  
- 
- mov pc, lr  




对于nand的详细介绍,会在移植帖中讲解..这里只需要简单的知道下就好了..第一阶段对nand的操作,只有copy了...

我们把复杂的处理,都放到第二阶段,到时候会跟大家讲解mtd管理的流程...



Finish!

Thanks a lot;




