# u-boot2010.03 移植篇(二)-----修改start.S,支持nand启动 - 一世豁然的专栏 - CSDN博客





2015年06月24日 23:00:06[一世豁然](https://me.csdn.net/Explorer_day)阅读数：990标签：[bootloader																[u-boot](https://so.csdn.net/so/search/s.do?q=u-boot&t=blog)](https://so.csdn.net/so/search/s.do?q=bootloader&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








本博客转载于：[http://blog.csdn.net/yyttiao/article/details/7961381](http://blog.csdn.net/yyttiao/article/details/7961381)




下面开始 移植篇(二)-----修改start.S,支持nand启动

----------------------------------------------------------

使用环境 

PC:     ubuntu 11.04

kernel: 2.6.32-28-generic

corss:  arm-linux-gcc 4.3.2

arm:    s3c6410

uboot:  uboot-2010-03

----------------------------------------------------------

作者:    LvApp

联系方式: 97164811@qq.com


一切版权均有作者所有，欢迎转载，请指明出处，如何修改请与本人联系，谢谢



uboot的第一阶段,其实做的事情也是比较多的,,但是一般来说,重点就是配置各种硬件环境,来保证第二阶段能正常启动.

而该部分根据不同的硬件也是不同的..像我的,s3c6410有2片dram,和一片nand..所以我为了保证我的代码能正常执行,

我必须得把代码搬移到内存里面去.不然代码在nand中也没法执行...但是为什么代码在nand中不能执行呢?其中一个比较

重要的原因就是nand并不是挂在CPU总线上面的,而是采用专门的硬件处理单元来控制的...nand控制器.

在你不添加任何代码的情况下,是不能控制外围的nand芯片的...但是怎么办.cpu内根本没有代码,怎么才能读取nand中的

代码到内存中去呢?没错-----s3c6410在启动的时候会帮我们以nand的最基本的时序.搬移nand中的前8K代码,到stepping

这样就可以执行了.而uboot的代码远比这个大..以至于我们得紧靠这部分代码,来完成剩下的代码部分的搬移....


今天的重点就是这个了.主要是来完成nand中剩余部分的uboot搬移到ram中,以便uboot能正常启动起来....好了.直接开始..


首先,你得有**smdk6410_config** 编译目标,不然怎么开始.....此处省略N字...请看我的移植篇第一篇...


下面开始.打开**start.S** (cpu/arm1176/start.S)和**smdk6410.h** (include/configs/smdk6410.h)

我们得一边参看头文件中的配置,一边修改代码,来达到我们的目的..

在**start.S** 中,最开始就是一段啥?异常向量表,此处忽略,请参考分析篇中对start.S的分析


前面一部分是MMU啥的...直接招待**after_copy:**这个标签

什么?之前没有任何copy的东西???怎么有**after_copy**一说呢?

没错..所以我们得把**copy**代码添加在这里..从**nand**中把数据搬移出来,放到这之前完成,这样才对得起这个**after_copy**的标签

说干就干,这里我是调用的C代码,你也可以用汇编来完成的,只要功能一样就行了
![](https://img-my.csdn.net/uploads/201209/09/1347198146_5455.png)

这里我们得向我们的配置头文件内添加一个宏,就是 **#define CONFIG_NAND_BOOT**

接着,我们要去添加我们要调用的**copy_from_nand**





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- *   
-  * copy U-Boot to SDRAM and jump to ram (from NAND or OneNAND)   
-  * r0: size to be compared   
-  * Load 1'st 2blocks to RAM because U-boot's size is larger than 1block(128k) size   
-  */////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
- .globl copy_from_nand   
- 
- copy_from_nand:   
-     mov r10, lr                     /* save return address */
- 
-     mov r9, r0   
- /* get ready to call C functions */
-     ldr sp, _TEXT_PHY_BASE          /* setup temp stack pointer */
-     sub sp, sp, #12   
-     mov fp, #0                      /* no previous frame, so fp=0 */
-     mov r9, #0x1000   
-     bl  copy_uboot_to_ram           /* 此函数需要添加，稍后说明。 */
- 3:  tst r0, #0x0                      
-     bne copy_failed                 /* 判断返回值是否失败 */
-     ldr r0, =0x0c000000             /* 判断内容是否一致 */
-     ldr r1, _TEXT_PHY_BASE   
- 1:  ldr r3, [r0], #4   
-     ldr r4, [r1], #4   
-     teq r3, r4   
-     bne compare_failed              /* not matched */
-     subs r9, r9, #4   
-     bne 1b   
- 4:  mov lr, r10                     /* all is OK */
-     mov pc, lr   
- 
- copy_failed:   
-     nop                             /* copy from nand failed */
-     b copy_failed   
- 
- compare_failed:   
-     nop                             /* compare failed */
-     b compare_failed  


在这段代码里面,就是调整了下sp,为了能调用c函数copy_uboot_to_ram,那么重点就是这个函数了..这个函数在nand_cp.c中.


<Ps:这个错误由以为网友提出,在此表示感谢....希望更多的网友能够支持.谢谢>

这个文件比较大,分析篇我会重点分析,究竟nand是如何搬移数据的.这里我们只要复制下好了...



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- #include <common.h>
- #ifdef CONFIG_S3C64XX
- #include <asm/io.h>
- #include <linux/mtd/nand.h>
- #include <asm/arch/s3c6400.h>
- 
- staticint nandll_read_page(uchar * buf, ulong addr, int large_block)  
- {  
- int i;  
- int page_size = 512;  
- 
- if (large_block == 1)  
-         page_size = 2048;  
- if (large_block == 2)  
-         page_size = 4096;  
- 
-     NAND_ENABLE_CE();  
- 
-     NFCMD_REG = NAND_CMD_READ0;  
- 
- /* Write Address */
-     NFADDR_REG = 0;  
- 
- if (large_block)  
-         NFADDR_REG = 0;  
- 
-     NFADDR_REG = (addr) & 0xff;  
-     NFADDR_REG = (addr >> 8) & 0xff;  
-     NFADDR_REG = (addr >> 16) & 0xff;  
- 
- if (large_block)  
-         NFCMD_REG = NAND_CMD_READSTART;  
- 
-     NF_TRANSRnB();  
- 
- /* for compatibility(2460). u32 cannot be used. by scsuh */
- for (i = 0; i < page_size; i++) {  
-         *buf++ = NFDATA8_REG;  
-     }  
- 
-     NAND_DISABLE_CE();  
- return 0;  
- }  
- 
- staticint nandll_read_blocks(ulong dst_addr, ulong size, int large_block)  
- {  
-     uchar *buf = (uchar *) dst_addr;  
- int i;  
-     uint page_shift = 9;  
- 
- if (large_block == 1)  
-         page_shift = 11;  
- 
- /* Read pages */
- if (large_block == 2)  
-         page_shift = 12;  
- 
- if (large_block == 2) {  
- /* Read pages */
- for (i = 0; i < 4; i++, buf += (1 << (page_shift - 1))) {  
-             nandll_read_page(buf, i, large_block);  
-         }  
- /* Read pages */
- for (i = 4; i < (0x60000 >> page_shift);i++, buf += (1 << page_shift)) {  
-             nandll_read_page(buf, i, large_block);  
-         }  
-     } else {  
- for (i = 0; i < (0x60000 >> page_shift);i++, buf += (1 << page_shift)) {  
-             nandll_read_page(buf, i, large_block);  
-         }  
-     }  
- 
- return 0;  
- }  
- 
- int copy_uboot_to_ram(void)  
- {  
- int large_block = 0;  
- int i;  
-     vu_char id;  
- 
-     NAND_ENABLE_CE();  
-     NFCMD_REG = NAND_CMD_READID;  
-     NFADDR_REG = 0x00;  
- 
- /* wait for a while */
- for (i = 0; i < 200; i++) ;  
-         id = NFDATA8_REG;  
-     id = NFDATA8_REG;  
- 
- if (id > 0x80)  
-         large_block = 1;  
- if (id == 0xd5)  
-         large_block = 2;  
- 
- /* read NAND Block.
-      * 128KB ->240KB because of U-Boot size increase. by scsuh
-      * So, read 0x3c000 bytes not 0x20000(128KB).
-      */
- return nandll_read_blocks(CONFIG_SYS_PHY_UBOOT_BASE, 0x60000,large_block);  
- }  
- 
- #endif

然后修改Makefile 把我们的nand_cp.c添加到uboot中去编译
![](https://img-my.csdn.net/uploads/201209/09/1347198172_2648.png)
最后make下.你会发现.编译好了....编译器记得重新make
 smdk6410_config下.因为我们修改了头文件,需要重新生成.mk文件

今天的任务基本完成了,今天比较轻松,接着我们稍微修改下smdk6410.h中的配置.
主要是改几个名字,把什么6400的改成6410就好了..
没错,你只要查找替换就行了...因为我们现在编译的已经是6410了.不是6400了...

修改完之后,有一个地方得稍微改下.
cpu/arm1176/s3c64xx/Makefile
![](https://img-my.csdn.net/uploads/201209/09/1347198200_1229.png)
把6400改成64XX因为,我们的配置文件里面已经没有6400了....
这个请查看头文件中的配置定义...
![](https://img-my.csdn.net/uploads/201209/09/1347198216_3735.png)




继续修改此篇:

到此眼看就已经结束了...好了,make吧....啥有一个错误,天哪....





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- arm-linux-gcc -E -g  -Os   -fno-common -ffixed-r8 -msoft-float  -fno-common -ffixed-r8 -msoft-float  -D__KERNEL__ -DTEXT_BASE=0   
- -I/media/LvApp/u-boot-2010.03/include -fno-builtin -ffreestanding -nostdinc -isystem   
- /usr/local/arm/4.4.1/bin/../lib/gcc/arm-none-linux-gnueabi/4.4.1/include -pipe  -DCONFIG_ARM -D__ARM__   
- -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv5t -march=armv5t  -ansi -D__ASSEMBLY__   
- -P - </media/LvApp/u-boot-2010.03/nand_spl/board/samsung/smdk6410/u-boot.lds >/media/LvApp/u-boot-2010.03/nand_spl/u-boot.lds  
- cd /media/LvApp/u-boot-2010.03/nand_spl/board/samsung/smdk6410 && arm-linux-ld -Bstatic -T   
- /media/LvApp/u-boot-2010.03/nand_spl/u-boot.lds -Ttext 0  start.o cpu_init.o lowlevel_init.o nand_boot.o nand_ecc.o s3c64xx.o \  
-         -Map /media/LvApp/u-boot-2010.03/nand_spl/u-boot-spl.map \  
-         -o /media/LvApp/u-boot-2010.03/nand_spl/u-boot-spl  
- start.o: In function `copy_from_nand':  
- /media/LvApp/u-boot-2010.03/nand_spl/board/samsung/smdk6410/start.S:302: undefined reference to `copy_uboot_to_ram'  
- make[1]: *** [/media/LvApp/u-boot-2010.03/nand_spl/u-boot-spl] Error 1  
- make[1]: Leaving directory `/media/LvApp/u-boot-2010.03/nand_spl/board/samsung/smdk6410'  
- make: *** [nand_spl] Error 2  

别急别急,,,小错而已..是吧....我们仔细查看,这是一个链接错误,为嘛会链接出错了..在此看到有这一行


start.o cpu_init.o lowlevel_init.o nand_boot.o nand_ecc.o s3c64xx.o

看到这一行,内心就暗自偷笑下吧..找到问题所在了...为啥?copy_uboot_to_ram这个函数不就是在start.o中内调用的嘛...所以这里没有nand_cp.o被链接进来,铁定

链接错误的...那要怎么改..直接定位到这个错误的目录下u-boot-2010.03/nand_spl/board/samsung/smdk6410修改该目录下Makefile 添加如下所示:





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- COBJS   = nand_boot.o nand_ecc.o s3c64xx.o nand_cp.o 后面得加上nand_cp.o  




接着我们得模仿后面的写法,为nand_cp.o添加目标生成方法





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- # from SoC directory
- $(obj)cpu_init.S:  
-     @rm -f $@  
-     @ln -s $(TOPDIR)/cpu/arm1176/s3c64xx/cpu_init.S $@  
- 
- 
- $(obj)nand_cp.c:  
-     @rm -f $@  
-     @ln -s $(TOPDIR)/cpu/arm1176/nand_cp.c $@  


至此,,编译应该就没有问题啦....





难不成这就行了???你下载进去试试不就知道了...

不行?运行不了?为什么呢....首先我们分析下,启动的过程...arm上电之后开始0地址指令..0地址的指令(此处假设时钟是nand启动,,这里就是讨论这个问题)是由arm自主加载的..从nand

的前8k,那么前8K的代码也就是0---0x2000 的这一段了..在start.S 这个文件,我们都知道是第一条指令的位置,那么这个文件就必须要被拷贝到0地址对应起来咯...这样才能被正常执行.

这些是不是都同意呢?不同意....咱们私下讨论.联系我...同意的,咱们继续.往下分析

既然知道了start.s为开始的地方,那么这里面也是第一部分的引导代码,而大家都知道第一部分的代码其实不全的,,,启动部分只有一下部分被arm搬移,剩余的大部分其实还在nand中..

那么既然不全,也就是跳转啊,函数调用啊...都必须在0---0x2000 的位置内才能到访问到.而且还不能是跟符号相关的跳转.因为符号跳转是根据链接地址跳转的..链接地址大家应该都知道

是被链接到一个很高的内存地址处去了...0x5e000啥的那个地址...

继续啊...不能符号跳转,就必须是相对跳转了...只要保证第一部分中所有的函数调用,符号跳转都在0---0x2000中,那就没问题,正常执行....为此,我们看一下u-boot.map这个文件.发现一个问题





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- Linker script and memory map  
- 
- 
-                 0x00000000                . = 0x0  
-                 0x00000000                . = ALIGN (0x4)  
- 
- 
- .text           0x57e00000    0x262a4  
-  cpu/arm1176/start.o(.text)  
-  .text          0x57e00000      0x440 cpu/arm1176/start.o  
-                 0x57e00000                _start  
-                 0x57e00040                _end_vect  
-                 0x57e00048                _armboot_start  
-                 0x57e0004c                _bss_start  
-                 0x57e00050                _bss_end  
-                 0x57e0010c                copy_from_nand  
-  cpu/arm1176/s3c64xx/cpu_init.o(.text)  
-  .text          0x57e00440      0x124 cpu/arm1176/s3c64xx/cpu_init.o  
-                 0x57e00440                mem_ctrl_asm_init  

我们只看一小部分了....在这里我们看到_start符号地址正好是那个指定的链接地址..也就是代码的起始符号...地址很高..无所谓啦...我们看看start.S中都有哪些符号的调用吧..这里直接看这个符号吧






**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- .text          0x57e1c268      0x1fc board/samsung/smdk6410/libsmdk6410.a(lowlevel_init.o)  
-                 0x57e1c26c                lowlevel_init  

这个符号其实在strat.S中是要使用的,,那么在仔细一看这个地址挺高的,,,好像不在前8k中是吧...那么在执行的时候就找不到这个符号的代码了...


问题就是在这,,得确保第一部分需要用到的代码能被arm加载进前8k就好了....那就得需要告诉连接器,,你...就你..帮我把这个链接到前面去.别放后面...不然打你...

链接器就会帮你乖乖的链接上去了...至于链接怎么听话,那就得使用链接听得懂的话了...链接脚本....使用哪个链接脚本..自己看我前面的分析文档吧....





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- cpu/arm1176/u-boot.lds  
- SECTIONS  
- {  
-     . = 0x00000000;  
- 
- 
-     . = ALIGN(4);  
-     .text :  
-     {  
-         cpu/arm1176/start.o (.text)  
-         board/samsung/smdk6410/lowlevel_init.o (.text)  
-         cpu/arm1176/s3c64xx/cpu_init.o (.text)  
-         cpu/arm1176/nand_cp.o   (.text)  
-         *(.text)  
-     }  
- 只列出了被修改的部分...  


还有另外一个文件






**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- board/samsung/smdk6410/u-boot-nand.lds  
- SECTIONS  
- {  
-     . = 0x00000000;  
- 
- 
-     . = ALIGN(4);  
-     .text      :  
-     {  
-       cpu/arm1176/start.o   (.text)  
-       cpu/arm1176/s3c64xx/cpu_init.o    (.text)  
-       board/samsung/smdk6410/lowlevel_init.o (.text)  
-           cpu/arm1176/nand_cp.o   (.text)  
-       *(.text)  
-     }  


至此...最后看一下修改之后的u-boot.map






**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7961381#)[copy](http://blog.csdn.net/yyttiao/article/details/7961381#)



- Linker script and memory map  
- 
- 
-                 0x00000000                . = 0x0  
-                 0x00000000                . = ALIGN (0x4)  
- 
- 
- .text           0x57e00000    0x262a4  
-  cpu/arm1176/start.o(.text)  
-  .text          0x57e00000      0x440 cpu/arm1176/start.o  
-                 0x57e00000                _start  
-                 0x57e00040                _end_vect  
-                 0x57e00048                _armboot_start  
-                 0x57e0004c                _bss_start  
-                 0x57e00050                _bss_end  
-                 0x57e0010c                copy_from_nand  
-  cpu/arm1176/s3c64xx/cpu_init.o(.text)  
-  .text          0x57e00440      0x124 cpu/arm1176/s3c64xx/cpu_init.o  
-                 0x57e00440                mem_ctrl_asm_init  
-  board/samsung/smdk6410/lowlevel_init.o(.text)  
-  .text          0x57e00564      0x1fc board/samsung/smdk6410/lowlevel_init.o  
-                 0x57e00568                lowlevel_init  
-  cpu/arm1176/nand_cp.o(.text)  
-  .text          0x57e00760      0x1b4 cpu/arm1176/nand_cp.o  
-                 0x57e00814                copy_uboot_to_ram  


好了.都到前面去了..真听话.....完了..就到这吧....




完了..今天就说这么多..特别简单..
Finish!
Thanks a lot~





