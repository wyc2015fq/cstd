# cortex A8处理器启动过程二引导代码BL1 - 一世豁然的专栏 - CSDN博客





2015年04月23日 13:50:59[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1359








本博客转载于：[http://blog.csdn.net/u010216127/article/details/9321489](http://blog.csdn.net/u010216127/article/details/9321489)








BL1相当于u-boot的第一阶段代码，主要完成如下工作：1.初始化硬件：关看门狗、设置串口、SDRAM、初始化Flash;2.重定位，将代码重定位到SDRAM;3.引导u-boot第二阶段代码。其实如果它能引导内核，就相当于一个bootlaoder,这里先实现上面3个功能。代码有点多，我还是贴出来吧，懒得看，需要编译好的源码包留个邮箱我发你。

系统：ubuntu 10.04.4

单板：s5pc100(CES-C100)

编译器：arm-linux-gcc-4.3.2

搭建开发环境详见ubuntu 10.04.4开发环境配置。
一、编写代码

文件start.S:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- .global  
- 
- _start:  
-     /*1. Disable Watchdog */  
-     /*1.关看门狗*/  
-     ldr r0, =0xEA200000  
-     mov r1, #0  
-     str r1, [r0]  
- 
-     bl  clock_init  
-     //bl    test  
-     bl  mp1_x_drive_strength_init  
-     bl  mem_ctrl_asm_init  
-     ldr sp, =0xD0038000  
-     bl  init_uart  
-     bl  nand_init  
-     //bl    test  
-     ldr r0, =0x0  
-     ldr r1, =0x21000000  
-     ldr r2, =bss_start  
-     sub r2, r2, r1  
-     bl  copy_code_to_sdram  
-     //bl    test  
-     //b main  
- 
- clean_bss:  
-     ldr r0, =bss_start  
-     ldr r1, =bss_end  
-     mov r3, #0  
-     cmp r0, r1  
-     ldreq pc, =on_ddr  
- 
- clean_loop:  
-     str r3, [r0], #4  
-     cmp r0, r1  
-     bne clean_loop  
-     ldr pc, =on_ddr  
- 
- on_ddr:  
-     ldr sp, =0x21800000    /* ÖØÐÂ³õÊŒ»¯Õ»£¬ÖžÏòÄÚŽæ */  
-     //bl  test  
-     ldr pc, =main  


文件clock.S



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- .globl clock_init  
- 
- clock_init:  
- 
-     /* 1.设置LOCK_TIME */  
-     ldr r0, =0xe0100000     //CLOCK_POWER_BASE  
-     mov r1, #0xe00  
-     orr r1, r1, #0x10  
-     str r1, [r0, #0x0]      /* APLL_LOCK */  
-     str r1, [r0, #0x4]   /* MPLL_LOCK */  
-     str r1, [r0, #0x8]   /* EPLL_LOCK */  
-     str r1, [r0, #0x0c] //HPLL_LOCK  
- 
- 
- //#define OTHERS        0x7e00f900  
- //  @ set async mode  /* 当CPU时钟 != HCLK时，要设为异步模式 */  
- //  ldr r0, =OTHERS  
- //  ldr r1, [r0]  
- //  bic r1, r1, #0xc0   /* 1100,0000 */       
- //  str r1, [r0]  
- 
- //loop1:                /* 等待，直到CPU进入异步模式 */  
- //  ldr r0, =OTHERS  
- //  ldr r1, [r0]  
- //  and r1, r1, #0xf00                    
- //  cmp r1, #0  
- //  bne loop1         
- 
-     /* SYNC667 */  
-     /* MISC_CON[19] = 0 */  
- 
- //#define ARM_RATIO    0   /* ARMCLK = DOUTAPLL / (ARM_RATIO + 1)    */  
- //#define HCLKX2_RATIO 1   /* HCLKX2 = HCLKX2IN / (HCLKX2_RATIO + 1) */  
- //#define HCLK_RATIO   1   /* HCLK = HCLKX2 / (HCLK_RATIO + 1)       */  
- //#define PCLK_RATIO   3   /* PCLK   = HCLKX2 / (PCLK_RATIO + 1)     */  
- //#define MPLL_RATIO   0   /* DOUTMPLL = MOUTMPLL / (MPLL_RATIO + 1)     */  
- //  ldr r0, =0x7E00F020  /* CLK_DIV0 */  
- //  ldr r1, =(ARM_RATIO) | (MPLL_RATIO << 4) | (HCLK_RATIO << 8) | (HCLKX2_RATIO << 9) | (PCLK_//RATIO << 12)  
- //  str r1, [r0]  
- /* CLK_DIV0 */  
- #define APLL_RATIO  0  
- #define ARM_RATIO   4  
- #define D0_BUS_RATIO    8  
- #define PCLKD0_RATIO    12  
- #define SECSS_RATIO 16  
- 
-     ldr r1, [r0, #0x300]    //CLK_DIV0 Clock divider  
-     ldr r2, =0x3fff  
-     bic r1, r1, r2  
- 
-     ldr r2, =(1<<APLL_RATIO) | (0<<ARM_RATIO) | (4<<D0_BUS_RATIO) | (1<<PCLKD0_RATIO) | (1<<SECSS_RATIO)  
-     orr r1, r1, r2  
-     str r1, [r0, #0x300]    //CLK_DIV0  
- 
-     ldr r2, =((1<<16) | (1<<12) | (1<<8) | (1<<4))  
-     orr r1 ,r1, r2  
-     str r1, [r0, #0x304]    //CLD_DIV1  
-     /* 2.配置时钟 */  
-     /* 2.1 配置APLL */  
-     /* 2.1.1 设置APLL  
-      * 2.1.2 MUXAPLL  
-      * 2.1.3 SYNC667  
-      * 2.1.4 DIVAPLL  
-      */  
- //#define APLL_CON_VAL  ((1<<31) | (266 << 16) | (3 << 8) | (1))  
- //  ldr r0, =0x7E00F00C  
- //  ldr r1, =APLL_CON_VAL  
- //  str r1, [r0]        /* APLL_CON, FOUTAPL = MDIV * Fin / (PDIV*2^SDIV) = 266*12/(3*2^1) = 532MHz  */  
- #define APLL_VAL  ((1<<31) | (417 << 16) | (3 << 8) | (0))      
-     //ldr r0, =0xe0100100   //APLL_CON  
-     ldr r1, =APLL_VAL  
-     str r1, [r0, #0x100]        /* MPLL_CON, FOUTMPL = MDIV * Fin / (PDIV*2^SDIV) = 266*12/(3*2^1) = 532MHz  */  
-     /* 2.2 配置MPLL */  
-     /* 2.2.1 设置MPLL  
-      * 2.2.2 MUXMPLL  
-      * 2.2.3 SYNCMUX  
-      * 2.2.4 SYNC667  
-      * 2.2.5 HCLKX2_RATIO  
-      * 2.2.6 PCLK_RATIO  
-      */  
- //#define MPLL_CON_VAL  ((1<<31) | (266 << 16) | (3 << 8) | (1))  
- //CONFIG_CLK_833_166_66  
- #define MPLL_VAL  ((1<<31) | (89 << 16) | (2 << 8) | (1))  
- #define EPLL_VAL  ((1<<31) | (135 << 16) | (3 << 8) | (3))  
- #define HPLL_VAL  ((1<<31) | (96 << 16) | (6 << 8) | (3))  
- 
-     ldr r1, =MPLL_VAL  
-     str r1, [r0, #0x104]  
- 
-     ldr r1, =EPLL_VAL  
-     str r1, [r0, #0x108]  
- 
-     ldr r1, =HPLL_VAL  
-     str r1, [r0, #0x10c]  
- 
-     /* 3.选择PLL的输出作为时钟源 */  
-     ldr r1, [r0, #0x200]    //CLK_SRC0 0xe0100200  
-     ldr r2, =0x1111  
-     orr r1, r1, r2  
-     str r1, [r0, #0x200]    //FOUT: APLL MPLL EPLL HPLL  
- 
-     mov     r1, #0x10000  
- 1:  subs    r1, r1, #1  
-     bne 1b  
- 
-     mov pc, lr  


文件cpu_init.S



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- //#include "s5pc100.h"  
- /* Port Group MP1_X Drive Strength Control */  
- #define MP1_0DRV_OFFSET         0x03CC  
- #define MP1_1DRV_OFFSET         0x03EC  
- #define MP1_2DRV_OFFSET         0x040C  
- #define MP1_3DRV_OFFSET         0x042C  
- #define MP1_4DRV_OFFSET         0x044C  
- #define MP1_5DRV_OFFSET         0x046C  
- #define MP1_6DRV_OFFSET         0x048C  
- #define MP1_7DRV_OFFSET         0x04AC  
- #define MP1_8DRV_OFFSET         0x04CC  
- 
- /*  
-  * Bus Matrix  
-  */  
- #define ELFIN_MEM_SYS_CFG       0x7e00f120  
- 
- /*  
-  * Memory controller  
-  */  
- #define ELFIN_SROM_BASE         0xE7000000  
- 
- #define SROM_BW_REG         __REG(ELFIN_SROM_BASE+0x0)  
- #define SROM_BC0_REG            __REG(ELFIN_SROM_BASE+0x4)  
- #define SROM_BC1_REG            __REG(ELFIN_SROM_BASE+0x8)  
- #define SROM_BC2_REG            __REG(ELFIN_SROM_BASE+0xC)  
- #define SROM_BC3_REG            __REG(ELFIN_SROM_BASE+0x10)  
- #define SROM_BC4_REG            __REG(ELFIN_SROM_BASE+0x14)  
- #define SROM_BC5_REG            __REG(ELFIN_SROM_BASE+0x18)  
- 
- /*  
-  * SDRAM Controller  
-  */  
- #define APB_DMC_BASE            0xE6000000  
- 
- #define DMC_CONCONTROL          0x00  
- #define DMC_MEMCONTROL          0x04  
- #define DMC_MEMCONFIG0          0x08  
- #define DMC_MEMCONFIG1          0x0C  
- #define DMC_DIRECTCMD           0x10  
- #define DMC_PRECHCONFIG         0x14  
- #define DMC_PHYCONTROL0         0x18  
- #define DMC_PHYCONTROL1         0x1C  
- #define DMC_PHYCONTROL2         0x20  
- #define DMC_PWRDNCONFIG         0x28  
- #define DMC_TIMINGAREF          0x30  
- #define DMC_TIMINGROW           0x34  
- #define DMC_TIMINGDATA          0x38  
- #define DMC_TIMINGPOWER         0x3C  
- #define DMC_PHYSTATUS0          0x40  
- #define DMC_PHYSTATUS1          0x44  
- #define DMC_CHIP0STATUS         0x48  
- #define DMC_CHIP1STATUS         0x4C  
- #define DMC_AREFSTATUS          0x50  
- #define DMC_MRSTATUS            0x54  
- #define DMC_PHYTEST0            0x58  
- #define DMC_PHYTEST1            0x5C  
- #define DMC_QOSCONTROL0         0x60  
- #define DMC_QOSCONFIG0          0x64  
- #define DMC_QOSCONTROL1         0x68  
- #define DMC_QOSCONFIG1          0x6C  
- #define DMC_QOSCONTROL2         0x70  
- #define DMC_QOSCONFIG2          0x74  
- #define DMC_DMC_QOSCONTROL3         0x78  
- #define DMC_QOSCONFIG3          0x7C  
- #define DMC_QOSCONTROL4         0x80  
- #define DMC_QOSCONFIG4          0x84  
- #define DMC_QOSCONTROL5         0x88  
- #define DMC_QOSCONFIG5          0x8C  
- #define DMC_QOSCONTROL6         0x90  
- #define DMC_QOSCONFIG6          0x94  
- #define DMC_QOSCONTROL7         0x98  
- #define DMC_QOSCONFIG7          0x9C  
- 
- /*  
- * Memory Chip direct command  
- */  
- #define PRO_ID_BASE         0xE0000000  
- #define PRO_ID_OFFSET           0x00  
- #define OMR_OFFSET          0x04  
- 
- /*  
-  * GPIO  
-  */  
- #define ELFIN_GPIO_BASE         0xE0300000  
- 
- 
- .globl  mp1_x_drive_strength_init  
- /*  
-  * Init MP1_X Driver Strength for SDRAM  
-  * void mp1_x_drive_strength_init(void)  
-  */  
-  mp1_x_drive_strength_init:  
- 
-     ldr r0, =ELFIN_GPIO_BASE  
-     ldr r1, =0xaaaa  
-     str     r1, [r0, #MP1_0DRV_OFFSET]  
-     str     r1, [r0, #MP1_1DRV_OFFSET]  
-     str     r1, [r0, #MP1_2DRV_OFFSET]  
-     str     r1, [r0, #MP1_3DRV_OFFSET]  
-     str     r1, [r0, #MP1_4DRV_OFFSET]  
-     str     r1, [r0, #MP1_5DRV_OFFSET]  
-     str     r1, [r0, #MP1_6DRV_OFFSET]    
-     str     r1, [r0, #MP1_7DRV_OFFSET]  
-     str     r1, [r0, #MP1_8DRV_OFFSET]    
- 
-     mov pc, lr  
- 
-     .globl mem_ctrl_asm_init  
- mem_ctrl_asm_init:  
- 
-     ldr r0, =APB_DMC_BASE           @APB_DMC_BASE 0xE6000000  
- 
-     ldr r1, =PRO_ID_BASE  
-     ldr r2, [r1, #PRO_ID_OFFSET]  
-     bic r2, #0xfffffdff  
-     mov r2, r2, lsr #9  
-     cmp r2, #0x1  
-     beq onenand_pop  
- 
- single:  
- 
- /************ delay loop *************/  
- 
- #if 0  
-     ldr     r1, =0x10000000  
-     mov         r2, #0  
- loop1:  
- 
-     cmp     r2, r1  
-     addne   r2, r2, #0x1  
-     bne loop1  
- #endif  
- 
- /************ DLL initialization *************/  
- 
-         ldr     r1, =0x6A101000             @ Phycontrol0 DLL parameter setting  
-         str     r1, [r0, #DMC_PHYCONTROL0]  
- 
-         ldr     r1, =0x000084F4                          @Phycontrol1 DLL parameter setting  
-         str     r1, [r0, #DMC_PHYCONTROL1]  
- 
-         ldr     r1, =0x00000000                          @Phycontrol2 DLL parameter setting  
-         str     r1, [r0, #DMC_PHYCONTROL2]  
- 
-         ldr     r1, =0x6A101002                   @DLL on  
-         str     r1, [r0, #DMC_PHYCONTROL0]  
- 
-         ldr     r1, =0x6A101003             @Dll start  
-         str     r1, [r0, #DMC_PHYCONTROL0]  
- 
- 
- 
-     ldr r2, = 0xE6000040        @DMC_PHYSTATUS0  
- 
- loop1:  
- 
-     ldr r1, [r2]                @Check DLL lock  
-     ands    r1, r1, #4  
-     beq loop1  
- 
-     ldr r1, [r2]  
-     mov r1, r1,  LSR #(0x6)  
-     and r1, r1, #(0xff)  
-     mov r1, r1, LSL  #(0x18)  
-     ldr     r2,  = 0xE6000018           @DMC_PHYCONTROL0  
-     ldr r3, [r2]  
-     bic r3,  r3, #(0xff000000)  
-     orr r1, r3, r2  
-     str r1, [r2]  
- 
- 
- 
-         ldr     r1, =0x6A101003         @Force Value locking  
-         str     r1, [r0, #DMC_PHYCONTROL0]  
- 
-         ldr     r1, =0x6A101009         @Dll off  
-         str     r1, [r0, #DMC_PHYCONTROL0]  
- 
- 
- 
- #if 0  
-         ldr     r1, =0x6A101000             @ Phycontrol0 DLL parameter setting  
-         str     r1, [r0, #DMC_PHYCONTROL0]  
- 
-         ldr     r1, =0x00008484                             @Phycontrol1 DLL parameter setting  
-         str     r1, [r0, #DMC_PHYCONTROL1]  
- 
-         ldr     r1, =0x00000000                             @Phycontrol2 DLL parameter setting  
-         str     r1, [r0, #DMC_PHYCONTROL2]  
- 
- #endif  
- /************ DLL initialization - END *************/  
- 
- 
- 
- 
-         ldr     r1, =0x0FF01010                         @auto refresh off  
-         str     r1, [r0, #DMC_CONCONTROL]  
- 
-         ldr     r1, =0x00202400                         @ BL=4 , 1 chip , DDR2  
-         str     r1, [r0, #DMC_MEMCONTROL]  
- 
- #if 1   // add xxs 256MB enable  
-         ldr     r1, =0x20F01323  
-         str     r1, [r0, #DMC_MEMCONFIG0]  
- 
-         ldr     r1, =0x40F00323  
-         str     r1, [r0, #DMC_MEMCONFIG1]  
- #else   // 128MB enable  
-         ldr     r1, =0x20F81313                       
-         str     r1, [r0, #DMC_MEMCONFIG0]  
- 
-         ldr     r1, =0x40F80313  
-         str     r1, [r0, #DMC_MEMCONFIG1]  
- #endif  
- 
-         ldr     r1, =0x20000000       
-         str     r1, [r0, #DMC_PRECHCONFIG]  
- 
- 
-         ldr     r1, =0x00100004         @ PwrdnConfig  
-         str     r1, [r0, #DMC_PWRDNCONFIG]  
- 
- #ifdef  CONFIG_HCLKD0_222  
-     ldr     r1, =0x000006c3                         @7.8us*222MHz=0x6c3, 7.8us*166MHz=1294(0x50E)  
-        str     r1, [r0, #DMC_TIMINGAREF]  
-     /* T-rfc   127.5nS/5ns  64 */  
-        ldr     r1, =0x202332C8                              @TimingRow      @222MHz  
-        str     r1, [r0, #DMC_TIMINGROW]  
- 
-        ldr     r1, =0x24450304                          @CL=5  
-        str     r1, [r0, #DMC_TIMINGDATA]  
- 
-  #else  
- 
- 
-         ldr     r1, =0x0000050E       
-         str     r1, [r0, #DMC_TIMINGAREF]  
- 
-         ldr  r1, =0x16233297                         @TimingRow      @166MHz  
-         str     r1, [r0, #DMC_TIMINGROW]  
- 
- @;      ldr  r1, =0x24250304                         @CL=5  
-         ldr  r1, =0x23230000                         @CL=3  
-         str     r1, [r0, #DMC_TIMINGDATA]  
-  #endif  
- 
-         ldr     r1, =0x07c80232                        @ Timing Power  
-         str     r1, [r0, #DMC_TIMINGPOWER]  
- 
- /* Direct Command for DDR2 */  
-         ldr     r1, =0x07000000                         @chip0 Deselect  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x01000000                        @chip0 PALL  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x00020000                         @chip0 EMRS2  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x00030000                         @chip0 EMRS3  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x00010400                         @chip0 EMRS1 (MEM DLL on = DQS# disable)  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
- @;      ldr     r1, =0x00000552                         @chip0 MRS (MEM DLL reset) CL=5, Burst Length=4  
-         ldr     r1, =0x00000532                         @chip0 MRS (MEM DLL reset) CL=3, Burst Length=4  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x01000000                         @chip0 PALL  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x05000000                         @chip0 REFA  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x05000000                         @chip0 REFA  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
- @;      ldr     r1, =0x00000452                         @chip0 MRS (MEM DLL unreset) , BL=4 , CL=5  
-         ldr     r1, =0x00000432                         @chip0 MRS (MEM DLL unreset) , BL=4 , CL=3   
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
-         ldr     r1, =0x00010780                         @chip0 EMRS1 (OCD default)  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
- 
- //      ldr     r1, =0x00010400                         @chip0 EMRS1 (OCD exit)  
-         ldr     r1, =0x00010402                         @chip0 EMRS1 (OCD exit)  Reduced Strength  
- //      ldr     r1, =0x00010000                         @chip0 EMRS1 (OCD exit)  ODT Disabled  
-         str     r1, [r0, #DMC_DIRECTCMD]  
- 
- 
- /* Direct Command for LPDDR  - END */  
- 
-     ldr     r1, =0x00FF20B0         @ConControl auto refresh on  
-         str     r1, [r0, #DMC_CONCONTROL]  
- 
- #if 0  
-         ldr     r1, =0x001000FF         @ PwrdnConfig  
-         str     r1, [r0, #DMC_PWRDNCONFIG]  
- #endif  
- 
-         ldr     r1, =0x00212413             @ MemControl  
-         str     r1, [r0, #DMC_MEMCONTROL]  
- 
-         b   exit_cpu_init  
- 
- 
- onenand_pop:  
-     ldr r1, =0x50101000             @Phycontrol0 DLL parameter setting  
-     str r1, [r0, #DMC_PHYCONTROL0]  
- 
-     ldr r1, =0x000000F4             @Phycontrol1 DLL parameter setting  
-     str r1, [r0, #DMC_PHYCONTROL1]  
- 
-     ldr r1, =0x00000000             @Phycontrol2 DLL parameter setting  
-     str r1, [r0, #DMC_PHYCONTROL2]  
- 
-     ldr r1, =0x50101002             @Dll on  
-     str r1, [r0, #DMC_PHYCONTROL0]  
- 
-     ldr r1, =0x50101003             @dll start  
-     str r1, [r0, #DMC_PHYCONTROL0]  
- 
-     ldr r1, =0x50101003             @Force Value locking  
-     str r1, [r0, #DMC_PHYCONTROL0]  
- 
-     ldr r1, =0x50101001             @Dll off  
-     str r1, [r0, #DMC_PHYCONTROL0]  
- 
-     ldr r1, =0xFF001010             @auto refresh off  
-     str r1, [r0, #DMC_CONCONTROL]  
- 
-     ldr r1, =0x00212100             @Dll off  
-     str r1, [r0, #DMC_MEMCONTROL]  
- 
- @;  ldr r1, =0x28F80222  
-     ldr r1, =0x28F00222  
-     str r1, [r0, #DMC_MEMCONFIG0]  
- 
-     ldr r1, =0x20F80222  
-     str r1, [r0, #DMC_MEMCONFIG1]  
- 
-     ldr r1, =0x20000000  
-     str r1, [r0, #DMC_PRECHCONFIG]  
- 
-     ldr r1, =0x0000050E  
-     str r1, [r0, #DMC_TIMINGAREF]  
- 
-     ldr r1, =0x0C233287             @TimingRow  @133MHz  
-     str r1, [r0, #DMC_TIMINGROW]  
- 
-     ldr r1, =0x32330303  
-     str r1, [r0, #DMC_TIMINGDATA]  
- 
-     ldr r1, =0x04141433             @Timing Power  
-     str r1, [r0, #DMC_TIMINGPOWER]  
- 
-     ldr r1, =0x07000000             @chip0 Deselect  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x01000000             @chip0 PALL  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x05000000             @chip0 REFA  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x05000000             @chip0 REFA  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x00000032             @chip0 MRS  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x07100000             @chip1 Deselect  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x01100000             @chip1 PALL  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x05100000             @chip1 REFA  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x05100000             @chip1 REFA  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0x00100032             @chip1 MRS  
-     str r1, [r0, #DMC_DIRECTCMD]  
- 
-     ldr r1, =0xFF002030             @ConControl auto refresh on  
-     str r1, [r0, #DMC_CONCONTROL]  
- 
-     ldr r1, =0x00100002             @PwrdnConfig  
-     str r1, [r0, #DMC_PWRDNCONFIG]  
- 
- @;  ldr r1, =0xFF212113             @MemControl  
-     ldr r1, =0xFF212100             @MemControl  
-     str r1, [r0, #DMC_MEMCONTROL]  
- 
-     b   exit_cpu_init  
- 
- exit_cpu_init:  
-     mov pc, lr  


文件uart.c:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- #define ULCON0     (*((volatile unsigned long *)0xEC000000))  
- #define UCON0      (*((volatile unsigned long *)0xEC000004))  
- #define UFCON0     (*((volatile unsigned long *)0xEC000008))  
- #define UMCON0     (*((volatile unsigned long *)0xEC00000C))  
- #define UTRSTAT0   (*((volatile unsigned long *)0xEC000010))  
- #define UFSTAT0    (*((volatile unsigned long *)0xEC000018))  
- #define UTXH0      (*((volatile unsigned char *)0xEC000020))  
- #define URXH0      (*((volatile unsigned char *)0xEC000024))  
- #define UBRDIV0    (*((volatile unsigned long *)0xEC000028))  
- #define UDIVSLOT0  (*((volatile unsigned long *)0xEC00002C))  
- 
- #define GPACON     (*((volatile unsigned long *)0xE0300000))  
- 
- #define ENABLE_FIFO   
- 
- void init_uart(void)  
- {  
-   GPACON &= ~0xffff;  
-   GPACON |= 0x2222;  
- 
-   /* ULCON0 */  
-   ULCON0 = 0x3;  /* 数据位:8, 无较验, 停止位: 1, 8n1 */  
-   UCON0  = 0x5;  /* 使能UART发送、接收 */  
- #ifdef ENABLE_FIFO  
-   UFCON0 = 0x07; /* FIFO enable */  
- #else  
-   UFCON0 = 0x00; /* FIFO disable */  
- #endif  
-   UMCON0 = 0;  
- 
-   /* 波特率 */  
-   /* DIV_VAL = (PCLK / (bps x 16 ) ) - 1   
-    * bps = 115200  
-    * DIV_VAL = (66500000 / (115200 x 16 ) ) - 1   
-    *         = 35.08  
-    */  
-   UBRDIV0   = 35;  
- 
-   /* x/16 = 0.08  
-    * x = 1  
-    */  
-   UDIVSLOT0 = 3;  
- 
-   UTXH0 = 0x4f4f4f4f;  
- }  
- #define ENABLE_FIFO   
- static void delay1(void)  
- {  
-   volatile int i = 10;  
-   while (i--);  
- }  
- 
- unsigned char getc(void)  
- {  
- #ifdef ENABLE_FIFO  
-   while ((UFSTAT0 & (1<<6)) == 0 && (UFSTAT0 & 0x3f) == 0)delay1();  
- #else  
-   while ((UTRSTAT0 & (1<<0)) == 0);  
- #endif  
- 
-   return URXH0;  
- }  
- 
- int getc_nowait(unsigned char *pChar)  
- {  
- #ifdef ENABLE_FIFO  
-   if ((UFSTAT0 & (1<<6)) == 0 && (UFSTAT0 & 0x3f) == 0)  
- #else  
-     if ((UTRSTAT0 & (1<<0)) == 0)  
- #endif  
-       {  
-     return -1;  
-       }  
-     else  
-       {  
-     *pChar = URXH0;  
-     return 0;  
-       }  
- }  
- 
- void putc(char c)  
- {  
- #ifdef ENABLE_FIFO  
-   if(c == '\r')  
-     {  
-       while (UFSTAT0 & (1<<14))delay1();  
-       UTXH0 = '\n';  
-     }  
-   if(c == '\n')  
-     {  
-       while (UFSTAT0 & (1<<14))delay1();  
-       UTXH0 = '\r';  
-     }  
-   //while(!(UTRSTAT0&(1<<1)));  
-   while (UFSTAT0 & (1<<14))delay1();  
- #else  
-   while ((UTRSTAT0 & (1<<2)) == 0);  
- #endif  
-   UTXH0 = c;  
- }  


文件uart.h:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- void init_uart(void);  
- unsigned char getc(void);  
- int getc_nowait(unsigned char *pChar);  
- void putc(char c);  


文件command.c:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- #include "lib.h"  
- #include "nand.h"  
- #include "setup.h"  
- 
- int help(int argc, char * argv[])  
- {  
-     printf("do_command <%s> \n", argv[0]);  
-     printf("help message: \n");  
-     printf("md - memory dispaly \n");  
-     printf("mw - memory write \n");  
-     printf("nand read - nand read sdram_addr nand_addr size\n");  
-     printf("nand write - nand write sdram_addr nand_addr size\n");  
-     printf("bootm - boot zImage\n");  
- 
-     return 0;  
- }  
- 
- int md(int argc, char * argv[])  
- {     
-     unsigned long *p = (unsigned long *)0;  
-     int i, j;  
- 
-     printf("do_command <%s> \n", argv[0]);  
- 
-     if (argc <= 1) {  
-         printf ("Usage:\n%s\n", "md address");  
-         return 1;  
-     }  
- 
-     if (argc >= 2)  
-         p = (unsigned long *)atoi(argv[1]);  
- 
-     for (j = 0; j < 16; j++)  
-     {     
-         printf("%x: ", p);  
-         for (i = 0; i < 4; i++)  
-           printf("%x ", *p++);    
-         puts("\n");  
-     }  
- 
-     return 0;  
- }  
- 
- int mw(int argc, char * argv[])  
- {     
-     unsigned long *p = (unsigned long *)0;  
-     int v = 0;  
- 
-     printf("do_command <%s> \n", argv[0]);  
- 
-     if (argc <= 2) {  
-         printf ("Usage:\n%s\n", "md address data");  
-         return 1;  
-     }  
- 
-     if (argc >= 2)  
-         p = (unsigned long *)atoi(argv[1]);  
- 
-     if (argc >= 3)  
-         v = atoi(argv[2]);  
- 
-     *p = v;  
-     printf("do_command mw finished!\n");  
-     return 0;  
- }  
- int nand(int argc, char *argv[])  
- {  
-   unsigned int nand_addr, sdram_addr;  
-   unsigned int size;  
-   char *cmd;  
-   if (argc < 5)  
-     {  
-       printf("nand read nand_addr sdram_addr size\n");  
-       printf("nand write nand_addr sdram_addr size\n");  
-       return 0;  
-     }  
-   nand_addr = atoi(argv[2]);  
-   sdram_addr = atoi(argv[3]);  
-   size = atoi(argv[4]);  
-   //cmd = argv[1];  
-   //putc(*cmd);  
-   //printf("*cmd = %x\n", *cmd);  
-   //printf("do_command <%s> \n", argv[0]);  
-   //printf("nand <%s> \n", argv[1]);  
-   puts("\n");  
-   printf("nand_addr = %x, sdram_addr = %x, size = %x\n", nand_addr, sdram_addr, size);  
-   if (strcmp(argv[1], "read") == 0)  
-   {  
-     nand_read_dis(nand_addr, sdram_addr, size);  
-     printf("nand <%s> \n", argv[1]);  
-   }  
-   if (strcmp(cmd, "write") == 0)  
-     nand_write_dat(nand_addr, (unsigned char *)sdram_addr, size);  
-   printf("nand %s finished!\n", argv[1]);  
-   return 0;  
- }  
- 
- //const char cmd[] = "root=/dev/nfs nfsroot=192.168.1.104:/work/nfs_root/wy_fs ip=192.168.1.17 cons//ole=ttySAC0";  
- const char cmd[] = "root=/dev/mtdblock2 rw rootfstype=yaffs2 mem=256m init=/init console=ttySAC0,115200 androidboot.console=s3c2410_serial0";  
- 
- void init_tag(int addr)  
- {  
-   struct tag * p;  
-   int i;  
-   p = (struct tag*) addr;  
-   p->hdr.tag  =  ATAG_CORE;  
-   p->hdr.size = tag_size(tag_core);  
-   p->u.core.flags = 0;//1;  
-   p->u.core.pagesize = 0;//4096;  
-   p->u.core.rootdev = 0x00000000;  
-   p = tag_next(p);  
-   p->hdr.tag = ATAG_CMDLINE;  
-   p->hdr.size =  (sizeof (cmd) + sizeof(struct tag_header) + 3) >>2;  
-   for(i=0; i< sizeof (cmd); i++)  
-     p->u.cmdline.cmdline[i] = cmd[i];  
-   p = tag_next(p);  
-   p->hdr.tag = ATAG_MEM;  
-   p->hdr.size = tag_size(tag_mem32);  
-   p->u.mem.start = 0x20000000;  
-   p->u.mem.size = 256*1024*1024;  
-   p = tag_next(p);  
-   p->hdr.tag = ATAG_NONE;  
-   p->hdr.size = 0;  
- }  
- int bootm(int argc, char * argv[])  
- {  
-   int addr = 0x20008000;  
-   void (*fp)(int, int, int);  
-   int taglist_mem_address = 0x20000100;  
-   printf("loading linux from 0x100000 to 0x20008000...\n");  
-   nand_read(0x100000, 0x20008000, 0x200000);  
-   fp = (void (*)(int, int, int))addr;  
-   init_tag(taglist_mem_address);  
-   printf("boot linux ...\n");  
-   fp(0, 1826, taglist_mem_address);  
-   printf("boot linux error!\n");  
-   return 0;  
- }  
- void run_command(int argc, char * argv[])  
- {  
-   switch(argc){  
-   case 0:  
-   case 1:  
-     if (strcmp(argv[0], "help") == 0)  
-     {  
-         help(argc, argv);  
-         return;  
-     }  
- 
-     else if (strcmp(argv[0], "bootm") == 0)  
-       {  
-         bootm(argc, argv);  
-         return;  
-       }  
-     else //(argc >= 1)  
-             printf("Unknown command '%s' - try 'help' \n",argv[0]);  
-   case 2:  
- 
-     if (strcmp(argv[0], "md") == 0)  
-     {  
-         md(argc, argv);  
-         return;  
-     }  
- 
-     else if (strcmp(argv[0], "mw") == 0)  
-     {  
-         mw(argc, argv);  
-         return;  
-     }  
-     else //(argc >= 1)  
-             printf("Unknown command '%s' - try 'help' \n",argv[0]);  
-   default:  
-     /*    if (strcmp(argv[1], "read") == 0)  
-       {  
-         nand(argc, argv);  
-         return;  
-       }  
-     else if (strcmp(argv[1], "write") == 0)  
-       {  
-         nand(argc, argv);  
-         return;  
-       }  
-     */  
-     if (argc == 5)  
-       {  
-       nand(argc, argv);  
-       return;  
-       }  
-     else //(argc >= 1)  
-         printf("Unknown command '%s' - try 'help' \n",argv[0]);  
- 
-     return;  
-   }  
- }  
- 
- 
- 文件command.h:  


void run_command(int argc, char * argv[]);

文件lib.c:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- #include "uart.h"  
- 
- int getchar(void)  
- {  
-   int c;  
-   c = (int)getc();  
-   if (c == '\r')  
-     return '\n';  
-   return c;  
- }  
- 
- 
- void puts(char *str)  
- {  
-   int i = 0;  
-   while (str[i])  
-     {  
-       putc(str[i]);  
-       i++;  
-     }  
- }  
- char * gets(char * s)  
- {  
-   char * p = s;  
-   while ((*p = getchar()) != '\n')  
-     {  
-       if (*p != '\b')  
-     putc(*p++);  
-       else  
-     if (p > s)  
-       {  
-         puts ("\b \b");  
-         p--;  
-       }  
-     }  
-   *p = '\0';  
-   putc('\n');  
-   return s;  
- }  
- 
- void test()  
- {  
-   puts("init: OK\n\r");  
- }  
- void puthex(unsigned int val)  
- {  
-   // 0x1234abcd   
-   int i;  
-   int j;  
- 
-   puts("0x");  
- 
-   for (i = 0; i < 8; i++)  
-     {  
-       j = (val >> ((7-i)*4)) & 0xf;  
-       if ((j >= 0) && (j <= 9))  
-     putc('0' + j);  
-       else  
-     putc('A' + j - 0xa);  
-     }  
- }  
- 
- void putbyte(unsigned char val)  
- {  
-   /* 0x1234abcd */  
-   int i;  
-   int j;  
- 
-   puts("0x");  
- 
-   for (i = 0; i < 2; i++)  
-     {  
-       j = (val >> ((1-i)*4)) & 0xf;  
-       if ((j >= 0) && (j <= 9))  
-     putc('0' + j);  
-       else  
-     putc('A' + j - 0xa);  
-     }  
- }  
- 
- char * itoa(int a, char * buf)  
- 
- {  
-   int num = a;  
-   int i = 0;  
-   int len = 0;  
-   do   
-     {  
-       buf[i++] = num % 10 + '0';  
-       num /= 10;  
-     } while (num);  
- 
-   buf[i] = '\0';  
-   len = i;  
-   for (i = 0; i < len/2; i++)  
-     {  
-       char tmp;  
-       tmp = buf[i];  
-       buf[i] = buf[len-i-1];  
-       buf[len-i-1] = tmp;  
-     }  
-   return buf;  
- }  
- int strlen(char *str)  
- {  
-   int i = 0;  
-   while (str[i])  
-     {  
-       i++;  
-     }  
-   return i;  
- }  
- 
- int strcmp(const char * s1, const char * s2)  
- {  
-   while (*s1 == *s2)  
-     {  
-       //putc(*s1);  
-       if (*s1 == '\0')  
-     {  
-       //s1 -= strlen(s2);  
-       return 0;  
-     }  
-       //putc(*s2);  
-       s1++;  
-       s2++;  
-     }  
-   //putc(*s1);  
-   //putc(*s2);  
-   return *s1 - *s2;  
- }  
- 
-  /*int strcmp(const char * cs,const char * ct)  
- {  
-   register signed char __res;  
- 
-   while (1) {  
-     if ((__res = *cs - *ct++) != 0 || !*cs++)  
-       break;  
-   }  
- 
-   return __res;  
-   }*/  
- int atoi(char * buf)  
- {  
-   int value = 0;  
-   int base = 10;  
-   int i = 0;  
-   if (buf[0] == '0' && buf[1] == 'x')  
-     {  
-       base = 16;  
-       i = 2;  
-     }  
-   // 123 = (1 * 10 + 2) * 10 + 3  
-   // 0x1F = 1 * 16 + F(15)  
-   while (buf[i])  
-     {  
-       int tmp;  
-       if (buf[i] <= '9' && buf[i] >= '0')   
-     tmp = buf[i] - '0';  
-       else  
-     tmp = buf[i] - 'a' + 10;  
-       value = value * base + tmp;  
-       i++;  
-     }  
-   return value;  
- }  
- 
- typedef int * va_list;  
- #define va_start(ap, A)(ap = (int *)&(A) + 1)  
- #define va_arg(ap, T)(*(T *)ap++)  
- #define va_end(ap)((void)0)  
- 
- int printf(const char * format, ...)  
- {  
-   char c;  
-   va_list ap;  
-   va_start(ap, format);  
- 
-   while ((c = *format++) != '\0')  
-     {  
-       switch (c)  
-     {  
-     case '%':  
-       c = *format++;  
- 
-       switch (c)  
-         {  
-           char ch;  
-           char * p;  
-           int a;  
-           char buf[100];  
- 
-         case 'c':  
-           ch = va_arg(ap, int);  
-           putc(ch);  
-           break;  
-         case 's':  
-           p = va_arg(ap, char *);  
-           puts(p);  
-           break;  
-         case 'x':  
-           a = va_arg(ap, int);  
-           puthex(a);  
-           break;  
-         case 'd':  
-           a = va_arg(ap, int);  
-           itoa(a, buf);  
-           puts(buf);  
-           break;          
-         default:  
- 
-           break;  
-         }  
-       break;  
- 
-     default:  
-       putc(c);  
-       break;  
-     }  
-     }  
-   return 0;  
- }  
- 
- void delay(void)  
- 
- {  
-   volatile int i = 0x200000;  
-   while (i--);  
- }  


文件lib.h



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- void delay(void);  
- void puts(char *str);  
- //int puts(const char * s);  
- void test();  
- void puthex(unsigned int val);  
- int getchar(void);  
- char * gets(char * s);  
- //void puthex(int a);  
- void putbyte(unsigned char val);  
- char * itoa(int a, char * buf);  
- int strlen(char *str);  
- int strcmp(const char * s1, const char * s2);  
- int atoi(char * buf);  
- int printf(const char * format, ...);  


文件nand.c:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- //#define MEM_SYS_CFG     (*((volatile unsigned long *)0x7E00F120))  
- #define NFCONF          (*((volatile unsigned long *)0xE7200000))  
- #define NFCONT          (*((volatile unsigned long *)0xE7200004))  
- #define NFCMMD          (*((volatile unsigned long *)0xE7200008))  
- #define NFADDR          (*((volatile unsigned long *)0xE720000C))  
- #define NFDATA          (*((volatile unsigned char *)0xE7200010))  
- #define NFSTAT          (*((volatile unsigned long *)0xE7200028))  
- 
- void nand_read(unsigned int nand_start, unsigned int ddr_start, unsigned int len);  
- 
- void copy_code_to_sdram(unsigned int src, unsigned int dest, unsigned int len)  
- {  
-   //int i = 0;  
-   /*如果是Nor启动*/  
-   // unsigned char *src_start = (unsigned char *)src;  
-   //unsigned char *dest_start = (unsigned char *)dest;  
-   /*if(isBootFromNorFlash())  
-     {  
-       while (i < len)  
-         {  
-           dest_start[i] = src_start[i];  
-           i++;  
-         }  
-     }  
-   else  
-     {  
-       //nand_init();                                                                                  
-       //nand_resd(src, dest, len)                                                                     
-       nand_read(src, dest, len);  
-       }*/  
-   nand_read(src, dest, len);  
- }  
- 
- void nand_select(void)  
- {  
-   NFCONT &= ~(1<<1);  
- }  
- 
- void nand_deselect(void)  
- {  
-   NFCONT |= (1<<1);  
- }  
- 
- 
- void nand_cmd(unsigned char cmd)  
- {  
-   NFCMMD = cmd;  
- }  
- 
- void nand_addr(unsigned char addr)  
- {  
-   NFADDR = addr;  
- }  
- 
- unsigned char nand_get_data(void)  
- {  
-   return NFDATA;  
- }  
- 
- void nand_send_data(unsigned char data)  
- {  
-   NFDATA = data;  
- }  
- 
- void wait_ready(void)  
- {  
-   while ((NFSTAT & 0x1) == 0);  
- }  
- 
- 
- void nand_reset(void)  
- {  
-   /* 选中 */  
-   nand_select();  
- 
-   /* 发出0xff命令 */  
-   nand_cmd(0xff);  
- 
-   /* 等待就绪 */  
-   wait_ready();  
- 
-   /* 取消选中 */  
-   nand_deselect();  
- }  
- 
- /*void clear_bss(void)  
- {  
-   extern int __bss_start, __bss_end;  
-   int *p = &__bss_start;  
- 
-   for (; p < &__bss_end; p++)  
-     *p = 0;  
- }  
- */  
- void nand_init(void)  
- {  
-   /* 让xm0csn2用作nand flash cs0 片选引脚 */  
-   //MEM_SYS_CFG &= ~(1<<1);  
- 
-   /* 设置时间参数 */  
- #define TACLS     7  
- #define TWRPH0    7  
- #define TWRPH1    7  
-   NFCONF &= ~((1<<30) | (7<<12) | (7<<8) | (7<<4));  
-   NFCONF |= ((TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4));  
- 
-   /* 使能nand flash controller */  
-   NFCONT |= 1;  
-   NFCONT &= ~(1<<16); /* 森止soft lock */  
- 
-   nand_reset();  
- }  
- 
- void nand_send_addr(unsigned int addr)  
- {  
- #if 1  
-   unsigned int page   = addr / 2048;  
-   //unsigned int colunm = addr & (2048 - 1);  
-   unsigned int colunm = addr % 2048;  
-   volatile int i;  
-   /* 这两个地址表示从页内哪里开始 */  
-   nand_addr(colunm & 0xff);  
-   for (i = 0; i < 10; i++);  
-   nand_addr((colunm >> 8) & 0xff);  
-   for (i = 0; i < 10; i++);  
-   /* 下面三个地址表示哪一页 */  
-   nand_addr(page & 0xff);  
-   for (i = 0; i < 10; i++);  
-   nand_addr((page >> 8) & 0xff);  
-   for (i = 0; i < 10; i++);  
-   nand_addr((page >> 16) & 0xff);  
-   for (i = 0; i < 10; i++);  
- #else  
-   nand_addr(addr & 0xff);         /* a0~a7 */  
-   nand_addr((addr >> 8) & 0x0f);   /* 程序的角度: a8~a11 */  
- 
-   nand_addr((addr >> 12) & 0xff); /* 程序的角度: a12~a19 */  
-   nand_addr((addr >> 20) & 0xff); /* 程序的角度: a20~a27 */  
-   nand_addr((addr >> 28) & 0x7); /* 程序的角度: a28 ~a30 */  
- 
- #endif  
- }  
- 
- void nand_read(unsigned int nand_start, unsigned int ddr_start, unsigned int len)  
- {  
-   unsigned int addr = nand_start;  
-   int i = nand_start % 2048;  
-   int left = i;  
-   int count = 0;  
-   unsigned char *dest = (unsigned char *)ddr_start;  
-   unsigned char data = 0;  
- 
-   /* 选中芯片 */  
-   nand_select();  
- 
-   while (count < len)  
-     {  
-       /* 发出命令0x00 */  
-       nand_cmd(0x00);  
- 
-       /* 发出地址 */  
-       nand_send_addr(addr);  
- 
-       /* 发出命令0x30 */  
-       nand_cmd(0x30);  
- 
-       /* 等待就绪 */  
-       wait_ready();  
- 
-       /* 读数据 */  
-       //for (; i < (4096-left) && count < len; i++)//从某页的i处开始读  
-       for (; i < (2048-left) && count < len; i++)//从某页的i处开始读  
-     {  
-       data = nand_get_data();  
-       /*(addr<16384)//前4页每次只能写2K  
-         {  
-           if(i<(2048-left))  
-         {  
-           dest[count++] = data;  
-         }  
-         }  
-       else  
-         {  
-           dest[count++] = data;  
-           }*/  
-       dest[count++] = data;  
-       addr++;  
-     }  
- 
-       i = 0;  
-       left = i;  
-     }  
- 
-   /* 取消片选 */  
-   nand_deselect();  
-   // return 0;  
- }  
- 
- void nand_erase_block(unsigned long addr)  
- {  
-   int page = addr / 2048;  
- 
-   nand_select();  
-   nand_cmd(0x60);  
- 
-   nand_addr(page & 0xff);  
-   nand_addr((page >> 8) & 0xff);  
-   nand_addr((page >> 16) & 0xff);  
- 
-   nand_cmd(0xd0);  
-   wait_ready();  
- 
-   nand_deselect();  
- }  
- 
- 
- void nand_write(unsigned int nand_start, unsigned char * buf, unsigned int len)  
- {  
-   unsigned long count = 0;  
-   unsigned long addr  = nand_start;  
-   int i = nand_start % 2048;  
-   //int left = i;  
- 
-   nand_select();  
-   while (count < len)  
-     {  
-       nand_cmd(0x80);  
-       nand_send_addr(addr);  
-       //for (; i < (2048-left) && count < len; i++)  
-       for (; i < 2048 && count < len; i++)  
-     {  
-       /*if(addr<16384)//写前2K  
-         {  
-           if(i<(2048-left))//前2页每页只能写2K  
-         {  
-           nand_send_data(buf[count++]);  
-         }  
-         }  
-       else  
-         {  
-           nand_send_data(buf[count++]);  
-           }*/  
-       nand_send_data(buf[count++]);  
-       addr++;  
-     }  
- 
-       nand_cmd(0x10);  
-       wait_ready();  
-       i = 0;  
-       //left = i;  
-     }  
- 
-   nand_deselect();  
- 
- }  
- 
- void nand_read_dis(unsigned int nand_start, unsigned int ddr_start, unsigned int len)  
- 
- {  
-   int i;  
-   //char buf[100];  
-   unsigned char *buf = (unsigned char *)ddr_start;  
-   //unsigned long addr;  
-   //unsigned long size;  
- 
-   // puts("\n enter the start address: 0x80000");  
- 
-   //scanf("%s", buf);  
- 
-   //addr = strtoul(buf, NULL, 0);  
-   //addr = 0x80000;  
-   //puts("read addr = 0x%x\n\r", addr);  
- 
-   //puts("enter the size: 0x60");  
- 
-   //scanf("%s", buf);  
-   //size = strtoul(buf, NULL, 0);  
-   //size = 0x60;  
- 
-   //if (size > 100)  
-   //{  
-   //  puts("the max size is 100\r");  
-   //  size = 100;  
-   //}  
-   nand_read(nand_start, buf, len);  
- 
-   puts("datas: \n");  
-   for (i = 0; i < len; i++)  
-     {  
-       // printf("%02x ", buf[i]);  
-       putbyte(*buf);  
-       puts("\t");  
-       if ((i+1) % 8 == 0)  
-     {  
-       puts("\n");  
-     }  
-       buf++;  
-     }  
-   puts("\n");  
- }  
- void nand_write_dat(unsigned int nand_start, unsigned char * buf, unsigned int len)  
- {  
-   //char buf[20] = {"abcd1234ABCD"};  
-   //unsigned long addr;  
-   //unsigned long size;  
- 
-   //puts("enter the start address:0x80000 ");  
-   //scanf("%s", buf);  
-   //addr = strtoul(buf, NULL, 0);  
-   //addr = 0x80000;  
-   //puts("enter the string:abcd1234ABCD ");  
-   //scanf("%s", buf);  
-   len = strlen(buf) + 1;  
-   //puts(" size= ");  
-   //puthex(size);  
-   //puts("\r");  
-   nand_write(nand_start, buf, len);  
-   puts("nand write sucessful!\n");  
- }  
- void nand_erase_dat(unsigned long erase_addr, unsigned int len)  
- {  
-   //char buf[100];  
-   //unsigned long addr;  
- 
-   //puts("enter the start address:0x80000 ");  
-   //scanf("%s", buf);  
-   //addr = strtoul(buf, NULL, 0);  
-   //addr = 0x80000;  
-   //puts("erase addr = ");  
-   //puthex(addr);  
-   //puts("\n");  
-       /* 烧写到nand flash block 0 */  
-   if(erase_addr % 0x20000 == 0)  
-     {  
-       for (erase_addr; erase_addr < ((len + 0x1FFFF) & ~0x1FFFF); erase_addr += 0x20000)  
-     {  
-       nand_erase_block(erase_addr);  
-       puts("erase a block sucessful\n");  
-     }  
-     }  
-   else  
-     puts("error erase addr\n");  
-       //nand_erase_block(addr);  
- 
- }  


文件nand.h:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- void nand_read(unsigned int nand_start, unsigned int ddr_start, unsigned int len);  
- void nand_erase_block(unsigned long addr);  
- void nand_write(unsigned int nand_start, unsigned char * buf, unsigned int len);  
- void nand_read_dis(unsigned int nand_start, unsigned int ddr_start, unsigned int len);  
- void nand_write_dat(unsigned int nand_start, unsigned char * buf, unsigned int len);  
- void nand_erase_dat(unsigned long erase_addr, unsigned int len);  
- void copy_code_to_sdram(unsigned int src, unsigned int dest, unsigned int len);  


文件setup.h:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- /*  
-  *  linux/include/asm/setup.h  
-  *  
-  *  Copyright (C) 1997-1999 Russell King  
-  *  
-  * This program is free software; you can redistribute it and/or modify  
-  * it under the terms of the GNU General Public License version 2 as  
-  * published by the Free Software Foundation.  
-  *  
-  *  Structure passed to kernel to tell it about the  
-  *  hardware it's running on.  See linux/Documentation/arm/Setup  
-  *  for more info.  
-  *  
-  * NOTE:  
-  *  This file contains two ways to pass information from the boot  
-  *  loader to the kernel. The old struct param_struct is deprecated,  
-  *  but it will be kept in the kernel for 5 years from now  
-  *  (2001). This will allow boot loaders to convert to the new struct  
-  *  tag way.  
-  */  
- #ifndef __ASMARM_SETUP_H  
- #define __ASMARM_SETUP_H  
- 
- #define u8  unsigned char  
- #define u16 unsigned short  
- #define u32 unsigned long  
- 
- /*  
-  * Usage:  
-  *  - do not go blindly adding fields, add them at the end  
-  *  - when adding fields, don't rely on the address until  
-  *    a patch from me has been released  
-  *  - unused fields should be zero (for future expansion)  
-  *  - this structure is relatively short-lived - only  
-  *    guaranteed to contain useful data in setup_arch()  
-  */  
- #define COMMAND_LINE_SIZE 1024  
- 
- /* This is the old deprecated way to pass parameters to the kernel */  
- struct param_struct {  
-     union {  
-     struct {  
-         unsigned long page_size;        /*  0 */  
-         unsigned long nr_pages;     /*  4 */  
-         unsigned long ramdisk_size;     /*  8 */  
-         unsigned long flags;        /* 12 */  
- #define FLAG_READONLY   1  
- #define FLAG_RDLOAD 4  
- #define FLAG_RDPROMPT   8  
-         unsigned long rootdev;      /* 16 */  
-         unsigned long video_num_cols;   /* 20 */  
-         unsigned long video_num_rows;   /* 24 */  
-         unsigned long video_x;      /* 28 */  
-         unsigned long video_y;      /* 32 */  
-         unsigned long memc_control_reg; /* 36 */  
-         unsigned char sounddefault;     /* 40 */  
-         unsigned char adfsdrives;       /* 41 */  
-         unsigned char bytes_per_char_h; /* 42 */  
-         unsigned char bytes_per_char_v; /* 43 */  
-         unsigned long pages_in_bank[4]; /* 44 */  
-         unsigned long pages_in_vram;    /* 60 */  
-         unsigned long initrd_start;     /* 64 */  
-         unsigned long initrd_size;      /* 68 */  
-         unsigned long rd_start;     /* 72 */  
-         unsigned long system_rev;       /* 76 */  
-         unsigned long system_serial_low;    /* 80 */  
-         unsigned long system_serial_high;   /* 84 */  
-         unsigned long mem_fclk_21285;       /* 88 */  
-     } s;  
-     char unused[256];  
-     } u1;  
-     union {  
-     char paths[8][128];  
-     struct {  
-         unsigned long magic;  
-         char n[1024 - sizeof(unsigned long)];  
-     } s;  
-     } u2;  
-     char commandline[COMMAND_LINE_SIZE];  
- };  
- 
- 
- /*  
-  * The new way of passing information: a list of tagged entries  
-  */  
- 
- /* The list ends with an ATAG_NONE node. */  
- #define ATAG_NONE   0x00000000  
- 
- struct tag_header {  
-     u32 size;  
-     u32 tag;  
- };  
- 
- /* The list must start with an ATAG_CORE node */  
- #define ATAG_CORE   0x54410001  
- 
- struct tag_core {  
-     u32 flags;      /* bit 0 = read-only */  
-     u32 pagesize;  
-     u32 rootdev;  
- };  
- 
- /* it is allowed to have multiple ATAG_MEM nodes */  
- #define ATAG_MEM    0x54410002  
- 
- struct tag_mem32 {  
-     u32 size;  
-     u32 start;  /* physical start address */  
- };  
- 
- /* VGA text type displays */  
- #define ATAG_VIDEOTEXT  0x54410003  
- 
- struct tag_videotext {  
-     u8      x;  
-     u8      y;  
-     u16     video_page;  
-     u8      video_mode;  
-     u8      video_cols;  
-     u16     video_ega_bx;  
-     u8      video_lines;  
-     u8      video_isvga;  
-     u16     video_points;  
- };  
- 
- /* describes how the ramdisk will be used in kernel */  
- #define ATAG_RAMDISK    0x54410004  
- 
- struct tag_ramdisk {  
-     u32 flags;  /* bit 0 = load, bit 1 = prompt */  
-     u32 size;   /* decompressed ramdisk size in _kilo_ bytes */  
-     u32 start;  /* starting block of floppy-based RAM disk image */  
- };  
- 
- /* describes where the compressed ramdisk image lives (virtual address) */  
- /*  
-  * this one accidentally used virtual addresses - as such,  
-  * its depreciated.  
-  */  
- #define ATAG_INITRD 0x54410005  
- 
- /* describes where the compressed ramdisk image lives (physical address) */  
- #define ATAG_INITRD2    0x54420005  
- 
- struct tag_initrd {  
-     u32 start;  /* physical start address */  
-     u32 size;   /* size of compressed ramdisk image in bytes */  
- };  
- 
- /* board serial number. "64 bits should be enough for everybody" */  
- #define ATAG_SERIAL 0x54410006  
- 
- struct tag_serialnr {  
-     u32 low;  
-     u32 high;  
- };  
- 
- /* board revision */  
- #define ATAG_REVISION   0x54410007  
- 
- struct tag_revision {  
-     u32 rev;  
- };  
- 
- /* initial values for vesafb-type framebuffers. see struct screen_info  
-  * in include/linux/tty.h  
-  */  
- #define ATAG_VIDEOLFB   0x54410008  
- 
- struct tag_videolfb {  
-     u16     lfb_width;  
-     u16     lfb_height;  
-     u16     lfb_depth;  
-     u16     lfb_linelength;  
-     u32     lfb_base;  
-     u32     lfb_size;  
-     u8      red_size;  
-     u8      red_pos;  
-     u8      green_size;  
-     u8      green_pos;  
-     u8      blue_size;  
-     u8      blue_pos;  
-     u8      rsvd_size;  
-     u8      rsvd_pos;  
- };  
- 
- /* command line: \0 terminated string */  
- #define ATAG_CMDLINE    0x54410009  
- 
- struct tag_cmdline {  
-     char    cmdline[1]; /* this is the minimum size */  
- };  
- 
- /* acorn RiscPC specific information */  
- #define ATAG_ACORN  0x41000101  
- 
- struct tag_acorn {  
-     u32 memc_control_reg;  
-     u32 vram_pages;  
-     u8 sounddefault;  
-     u8 adfsdrives;  
- };  
- 
- /* footbridge memory clock, see arch/arm/mach-footbridge/arch.c */  
- #define ATAG_MEMCLK 0x41000402  
- 
- struct tag_memclk {  
-     u32 fmemclk;  
- };  
- 
- #define ATAG_MTDPART    0x41001099//add  
- 
- struct tag_mtdpart {             //add  
-   u32 mtd_part_size[3];  
- };  
- 
- struct tag {  
-     struct tag_header hdr;  
-     union {  
-         struct tag_core     core;  
-         struct tag_mem32    mem;  
-         struct tag_videotext    videotext;  
-         struct tag_ramdisk  ramdisk;  
-         struct tag_initrd   initrd;  
-         struct tag_serialnr serialnr;  
-         struct tag_revision revision;  
-         struct tag_videolfb videolfb;  
-         struct tag_cmdline  cmdline;  
- 
-         /*  
-          * Acorn specific  
-          */  
-         struct tag_acorn    acorn;  
- 
-         /*  
-          * DC21285 specific  
-          */  
-         struct tag_memclk   memclk;  
- 
-       struct tag_mtdpart      mtdpart_info;//add  
-     } u;  
- };  
- 
- struct tagtable {  
-     u32 tag;  
-     int (*parse)(const struct tag *);  
- };  
- 
- #define __tag __attribute__((unused, __section__(".taglist")))//add  
- #define __tagtable(tag, fn) \  
-   static struct tagtable __tagtable_##fn __tag = { tag, fn }//add  
- 
- #define tag_member_present(tag,member)              \  
-     ((unsigned long)(&((struct tag *)0L)->member + 1)    \  
-         <= (tag)->hdr.size * 4)  
- 
- #define tag_next(t) ((struct tag *)((u32 *)(t) + (t)->hdr.size))  
- #define tag_size(type)  ((sizeof(struct tag_header) + sizeof(struct type)) >> 2)  
- 
- #define for_each_tag(t,base)        \  
-     for (t = base; t->hdr.size; t = tag_next(t))  
- 
- /*  
-  * Memory map description  
-  */  
- #define NR_BANKS 8  
- 
- struct meminfo {  
-     int nr_banks;  
-     unsigned long end;  
-     struct {  
-         unsigned long start;  
-         unsigned long size;  
-         int           node;  
-     } bank[NR_BANKS];  
- };  
- 
- extern struct meminfo meminfo;  
- 
- #endif  


文件mani.c:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- #include "uart.h"  
- #include "lib.h"  
- #include "command.h"  
- #include "nand.h"  
- 
- #define CFG_PROMPT "WY_BOOT # "/* Monitor Command Prompt*/  
- #define CFG_CBSIZE 256/* Console I/O Buffer Size*/  
- 
- #define GPH1CON (*(volatile unsigned int *)0xe0300C20)  
- #define GPH1DAT (*(volatile unsigned int *)0xe0300c24)  
- void led_water(void);  
- void led_init();  
- 
- 
- //extern void clock_init(void);  
- 
- char *argv[10];  
- int readline (const char *const prompt)  
- {  
-   char console_buffer[CFG_CBSIZE];/* console I/O buffer*/  
-   char *buf = console_buffer;  
-   int argc = 0;  
-   int state = 0;  
-   int i = 0;  
-   puts(prompt);  
-   gets(console_buffer);  
-   while (*buf)  
-     {  
-       if (*buf != ' ' && state == 0)  
-     {  
-       argv[argc++] = buf;  
-       state = 1;  
-     }  
-       if (*buf == ' ' && state == 1)  
-     {  
-       *buf = '\0';  
-       state = 0;  
-     }  
-       buf++;  
-     }  
-   //for(i=0;i<2;i++)  
-   //printf("argv[%d] = %s, argc = %d\n",i,argv[i],argc);  
-   //for(i = 2;i < argc;i++)  
-   //printf("argv[%d] = %x, argc = %d\n",i,atoi(argv[i]),argc);  
- 
-   return argc;  
- }  
- void message(void)  
- {  
-   printf("This bootloader support some command to test peripheral:\n");  
-   printf("Such as: LCD, IIS, BUZZER \n");  
-   printf("Try 'help' to learn them \n");  
- }  
- 
- void nand_write_test(void)  
- {  
-   char buf[20] = {"abcd1234ABCD"};  
-   unsigned long addr;  
-   unsigned long size;  
- 
-   puts("enter the start address:0x20000 ");  
-   //scanf("%s", buf);  
-   //addr = strtoul(buf, NULL, 0);  
-   addr = 0x20000;  
-   puts("enter the string:abcd1234ABCD ");  
-   //scanf("%s", buf);  
-   size = strlen(buf) + 1;  
-   puts(" size= ");  
-   puthex(size);  
-   puts("\r");  
-   nand_write(addr, buf, size);  
- 
- }  
- 
- void nand_read_test(void)  
- 
- {  
-   int i;  
-   char buf[100];  
-   unsigned long addr;  
-   unsigned long size;  
- 
-   puts("\n enter the start address: 0x20000");  
- 
-   //scanf("%s", buf);  
- 
-   //addr = strtoul(buf, NULL, 0);  
-   addr = 0x20000;  
-   //puts("read addr = 0x%x\n\r", addr);  
- 
-   puts("enter the size: 0x60");  
- 
-   //scanf("%s", buf);  
-   //size = strtoul(buf, NULL, 0);  
-   size = 0x60;  
- 
-   if (size > 100)  
-     {  
-       puts("the max size is 100\r");  
-       size = 100;  
-     }  
-   nand_read(addr, buf, size);  
- 
-   puts("datas: \n");  
-   for (i = 0; i < size; i++)  
-     {  
-       // printf("%02x ", buf[i]);  
-       putbyte(buf[i]);  
-       puts("\t");  
-       if ((i+1) % 8 == 0)  
-     {  
-       puts("\n");  
-     }  
-     }  
-   puts("\n");  
- }  
- 
- void nand_erase_test(void)  
- {  
-   //char buf[100];  
-   unsigned long addr;  
- 
-   puts("enter the start address:0x20000 ");  
-   //scanf("%s", buf);  
-   //addr = strtoul(buf, NULL, 0);  
-   addr = 0x20000;  
-   puts("erase addr = ");  
-   puthex(addr);  
-   puts("\n");  
-   nand_erase_block(addr);  
- 
- }  
- 
- void update_program(void)  
- {  
-   unsigned char *buf = (unsigned char *)0x22000000;  
-   //unsigned char *buf = (unsigned char *)0xD0036000;  
-   unsigned long len = 0;  
-   int have_begin = 0;  
-   int nodata_time = 0;  
-   unsigned long erase_addr;  
-   char c;  
-   int i;  
- 
-   /* 读串口获得数据 */  
-   puts("\n\ruse V2.2.exe/gtkterm to send file\r");  
-   while (1)  
-     {  
-       if (getc_nowait(&buf[len]) == 0)  
-     {  
-       have_begin = 1;  
-       nodata_time = 0;  
-       len++;  
-     }  
-       else  
-     {  
-       if (have_begin)  
-         {  
-           nodata_time++;  
-         }  
-     }  
- 
-       if (nodata_time == 1000)  
-     {  
-       break;  
-     }  
-     }  
-   puts("\nhave get data:");  
-   puthex(len);  
-   puts(" bytes\n");  
-   puts("the first 16 bytes data: \n\r");  
-   for (i = 0; i < 16; i++)  
-     {  
-       // put("%02x ", buf[i]);  
-       putbyte(buf[i]);  
-       puts("\t");  
-       if ((i+1) % 8 == 0)  
-     {  
-       puts("\n");  
-     }  
-     }  
-   puts("\n");  
- 
-   puts("Press Y to program the flash: \n\r");  
- 
-   c = getc();  
-   putc(c);  
-   puts("\n");  
-   if (c == 'y' || c == 'Y')  
-     {  
-       /* 烧写到nand flash block 0 */  
-       for (erase_addr = 0; erase_addr < ((len + 0x1FFFF) & ~0x1FFFF); erase_addr += 0x20000)  
-     {  
-       nand_erase_block(erase_addr);  
-     }  
-       nand_write(0, buf, len);  
- 
-       puts("update program successful\n\r");  
-     }  
-   else  
-     {  
-       puts("Cancel program!\n\r");  
-     }  
- }  
- 
- void run_program(void)  
- {  
-   unsigned char *buf = (unsigned char *)0x27e00000;  
-   //unsigned char *buf = (unsigned char *)0xD0036000;  
-   unsigned long len = 0;  
-   int have_begin = 0;  
-   int nodata_time = 0;  
-   void (*theProgram)(void);  
-   int i;  
-   /* 读串口获得数据 */  
-   puts("\n use gtkterm to send file\r");  
-   while (1)  
-     {  
-       if (getc_nowait(&buf[len]) == 0)  
-     {  
-       have_begin = 1;  
-       nodata_time = 0;  
-       len++;  
-     }  
-       else  
-     {  
-       if (have_begin)  
-         {  
-           nodata_time++;  
-         }  
-     }  
- 
-       if (nodata_time == 10000)  
-     {  
-       break;  
-     }  
-     }  
-   //printf("have get %d bytes data\n\r", len);  
-   puts("\n have get data:");  
-   puthex(len);  
-   puts(" bytes\n");  
-   puts("the first 16 bytes data: \n");  
-   for (i = 0; i < 16; i++)  
-     {  
-       // put("%02x ", buf[i]);                                                                       
-       putbyte(buf[i]);  
-       puts("\t");  
-       //putc('\0');  
-     }  
-   puts("\n");  
-   puts("jump to 0x27e00000 to run it\n");  
- 
-   theProgram = (void (*)(void))0x27e00000;  
- 
-   theProgram();  
- }  
- 
- void download_bl2(void)  
- {  
-   unsigned char *buf = (unsigned char *)0x22000000;  
-   //unsigned char *buf = (unsigned char *)0xD0036000;  
-   unsigned long len = 0;  
-   int have_begin = 0;  
-   int nodata_time = 0;  
-   unsigned long erase_addr;  
-   char c;  
-   int i;  
- 
-   /* 读串口获得数据 */  
-   puts("\n\ruse V2.2.exe/gtkterm to send file\r");  
-   while (1)  
-     {  
-       if (getc_nowait(&buf[len]) == 0)  
-     {  
-       have_begin = 1;  
-       nodata_time = 0;  
-       len++;  
-     }  
-       else  
-     {  
-       if (have_begin)  
-         {  
-           nodata_time++;  
-         }  
-     }  
- 
-       if (nodata_time == 1000)  
-     {  
-       break;  
-     }  
-     }  
-   puts("\nhave get data:");  
-   puthex(len);  
-   puts(" bytes\n");  
-   puts("the first 16 bytes data: \n\r");  
-   for (i = 0; i < 16; i++)  
-     {  
-       // put("%02x ", buf[i]);  
-       putbyte(buf[i]);  
-       puts("\t");  
-       if ((i+1) % 8 == 0)  
-     {  
-       puts("\n");  
-     }  
-     }  
-   puts("\n");  
- 
-   puts("Press Y to program the flash: \n\r");  
- 
-   c = getc();  
-   putc(c);  
-   puts("\n");  
-   if (c == 'y' || c == 'Y')  
-     {  
-       /* 烧写到nand flash block 0 */  
-       for (erase_addr = 0x20000; erase_addr < ((len + 0x1FFFF) & ~0x1FFFF); erase_addr += 0x20000)  
-     {  
-       nand_erase_block(erase_addr);  
-     }  
-       nand_write(0x20000, buf, len);  
- 
-       puts("download bl2 program successful\n\r");  
-     }  
-   else  
-     {  
-       puts("Cancel program!\n\r");  
-     }  
- }  
- 
- void load_bl2(void)  
- {  
-   unsigned char *buf = (unsigned char *)0x27e00000;  
-   char c;  
-   //unsigned char *buf = (unsigned char *)0xD0036000;  
-   //unsigned long len = 0;  
-   //int have_begin = 0;  
-   //int nodata_time = 0;  
-   void (*theProgram)(void);  
-   //int i;  
-   /* 读串口获得数据 */  
-   puts("\n start load bl2 code\r");  
-   copy_code_to_sdram(0x20000, 0x27e00000, 0x40000);  
- 
-   puts("Press Y jump to run it \n\r");  
- 
-   c = getc();  
-   putc(c);  
-   puts("\n");  
-   if (c == 'y' || c == 'Y')  
-   {  
-     puts("jump to 0x27e00000 to run it\n");  
-     theProgram = (void (*)(void))0x27e00000;    
-     theProgram();  
-   }  
-   else  
-   {  
-      puts("Cancel run it!\n\r");  
-   }  
- }  
- 
- int main()  
- {  
-   char c;  
-   int argc = 0;  
-   int i = 0;  
-   volatile unsigned long *mem = (volatile unsigned long *)0x22000000;  
-   volatile unsigned long *tmem = (volatile unsigned long *)0x21000010;  
-   volatile unsigned long *tsram = (volatile unsigned long *)0xD0034010;  
-   *mem = 0xDB134653;  
-   led_init();  
-   //clock_init();  
-   //init_uart();  
-   printf("the 2012 hex value:%x\n",2012);  
-   printf("the mem hex value:%x\n",*mem);  
-   *mem = 0x11122212;  
-   printf("\n********************************\r");  
-   printf("                   wy_bootloader\r");  
-   printf("                   vars: %d \r",2012);  
-   printf("********************************\r");  
-   printf("the mem hex value:%x\n",*mem);  
-   printf("[0x21000010] = %x\n",*tmem);  
-   printf("[0xD0034010] = %x\n",*tsram);  
-   while (1)  
-     {  
-       puts("[w] write the nand flash\n");  
-       puts("[r] read the nand flash\n");  
-       puts("[e] erase the nand flash\n");  
-       puts("[g] get file, and write to nand flash 0 block\n");  
-       puts("[x] get file to ddr(0x27e00000), run it\n");  
-       puts("[b] boot the Kernel\n");  
-       puts("[l] load bl2 code to sdram,and run bl2\n");  
-       puts("[d] download bl2 code to nand flash\n");  
-       puts("[s] reset the programe\n");  
-       do {  
-     argc = readline (CFG_PROMPT);  
-     if(argc == 0 && i ==0)  
-       {  
-         message();  
-         i=1;  
-       }  
-     else  
-       run_command(argc, argv);  
-     c = getc();  
-     if (c == '\n' || c == '\r')  
-       {  
-         puts("\n\r");  
-       }  
-     else  
-       {  
-         putc(c);  
-       }  
-     led_water();  
- 
-       } while (c == '\n' || c == '\r');  
- 
-       switch (c)  
-     {  
-     case 'w':  
-     case 'W':  
-       {  
-         nand_write_test();  
-         break;  
-       }  
- 
-     case 'r':  
-     case 'R':  
-       {  
-         nand_read_test();  
-         break;  
-       }  
- 
-     case 'e':  
-     case 'E':  
-       {  
-         nand_erase_test();  
-         break;  
-       }  
- 
-     case 'g':  
-     case 'G':  
-       {  
-         update_program();  
-         break;  
-       }  
- 
-     case 'x':  
-     case 'X':  
-       {  
-         run_program();  
-         break;  
-       }  
- 
-     case 'b':  
-     case 'B':  
-       {  
-         //boot_kernel();  
-         break;  
-       }  
-     case 'l':  
-     case 'L':  
-       {  
-         //boot_kernel();  
-         load_bl2();  
-         break;  
-       }  
-     case 'd':  
-     case 'D':  
-       {  
-         download_bl2();  
-         break;  
-       }  
- 
-     case 's':  
-     case 'S':  
-       {  
-         //volatile unsigned long *mcode = (volatile unsigned long *)0x21000010;  
-         void (*theProgram)(void);  
-         //printf("the 0x21000010 value:%x\n\r",*mcode);  
-         theProgram = (void (*)(void))0xD0034010;  
-         theProgram();  
- 
-         break;  
-       }  
- 
-     }  
-     }  
- 
-   return 0;  
- }  
- void led_init()  
- {  
-   GPH1CON = 0x11110000;  
- 
- }  
- void led_water(void)  
- {  
-   /*  int i;  
-   while (1)  
-   {  
-   GPH1DAT = (i << 4;  
-       i++;  
-       if (i == 16)  
-     i = 0;  
-       delay();  
-       }*/  
-   GPH1DAT = 0x0000;  
-   delay();  
-   GPH1DAT = 0xffff;  
-   delay();  
- }  


文件bootloader.lds:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- SECTIONS {  
-     . = 0x21000010;  
- 
-     .text : {  
-         * (.text)  
-     }  
- 
-     . = ALIGN(4);  
-     .rodata : {  
-         * (.rodata)  
-     }  
- 
-     . = ALIGN(4);  
-     .data : {  
-         * (.data)  
-     }  
- 
-     . = ALIGN(4);  
-     bss_start = .;  
-     .bss  : { *(.bss)  *(COMMON) }  
-     bss_end = .;  
- }  


文件Makefile:



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/9321489#)[copy](http://blog.csdn.net/u010216127/article/details/9321489#)

- boot.bin:start.S clock.S cpu_init.S nand.c main.c uart.c lib.c command.c  
-     arm-linux-gcc -c start.S -o start.o  
-     arm-linux-gcc -c clock.S -o clock.o  
-     arm-linux-gcc -c cpu_init.S -o cpu_init.o   
-     arm-linux-gcc -c main.c -o main.o  
-     arm-linux-gcc -c nand.c -o nand.o  
-     arm-linux-gcc -c uart.c -o uart.o  
-     arm-linux-gcc -c lib.c -o lib.o   
-     arm-linux-gcc -c command.c -o command.o   
-     arm-linux-ld -T bootloader.lds start.o clock.o cpu_init.o nand.o main.o uart.o lib.o command.o -o boot_elf  
-     arm-linux-objcopy -O binary -S boot_elf boot.bin  
- clean:  
-     rm -rf *.o *.bin boot_elf *.dis  


终于贴完了，基本上是按调用的顺序贴的，都是我反复修改、调试后，能用的代码。很多代码都有注释，有问题一起讨论。

二、编译

[change@change:~$](mailto:change@change:%7E$) cd 9_bl1/
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) ls

bootloader.lds  command.c  cpu_init.S  lib.h   Makefile  nand.h        setup.h  uart.c  wy_mktools.exe

clock.S         command.h  lib.c       main.c  nand.c    newtools.exe  start.S  uart.h
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) make

arm-linux-gcc -c start.S -o start.o

arm-linux-gcc -c clock.S -o clock.o

arm-linux-gcc -c cpu_init.S -o cpu_init.o 

arm-linux-gcc -c main.c -o main.o

In file included from main.c:1:

uart.h:5: warning: conflicting types for built-in function 'putc'

In file included from main.c:2:

lib.h:3: warning: conflicting types for built-in function 'puts'

main.c: In function 'readline':

main.c:25: warning: passing argument 1 of 'puts' discards qualifiers from pointer target type

main.c: In function 'nand_read_test':

main.c:102: warning: passing argument 2 of 'nand_read' makes integer from pointer without a cast

arm-linux-gcc -c nand.c -o nand.o

nand.c: In function 'nand_read_dis':

nand.c:286: warning: passing argument 2 of 'nand_read' makes integer from pointer without a cast

nand.c: In function 'nand_write_dat':

nand.c:314: warning: incompatible implicit declaration of built-in function 'strlen'

arm-linux-gcc -c uart.c -o uart.o

uart.c: In function 'init_uart':

uart.c:45: warning: large integer implicitly truncated to unsigned type

uart.c: At top level:

uart.c:82: warning: conflicting types for built-in function 'putc'

arm-linux-gcc -c lib.c -o lib.o

In file included from lib.c:1:

uart.h:5: warning: conflicting types for built-in function 'putc'

lib.c:13: warning: conflicting types for built-in function 'puts'

arm-linux-gcc -c command.c -o command.o

In file included from command.c:1:

lib.h:3: warning: conflicting types for built-in function 'puts'

arm-linux-ld -T bootloader.lds start.o clock.o cpu_init.o nand.o main.o uart.o lib.o command.o -o boot_elf

arm-linux-objcopy -O binary -S boot_elf boot.bin
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) ./newtools boot.bin boots.bin

-bash: ./newtools: No such file or directory
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) newtools boot.bin boots.bin

newtools: command not found
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) sudo chmod 777 -R newtools.exe

[sudo] password for change: 
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) ./newtools boot.bin boots.bin

-bash: ./newtools: No such file or directory
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) newtools boot.bin boots.bin

newtools: command not found
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) ./newtools.exe boot.bin boots.bin
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$) ls

boot.bin        boots.bin  command.c  cpu_init.o  lib.h   main.o    nand.h        setup.h  uart.c  wy_mktools.exe

boot_elf        clock.o    command.h  cpu_init.S  lib.o   Makefile  nand.o        start.o  uart.h

bootloader.lds  clock.S    command.o  lib.c       main.c  nand.c    newtools.exe  start.S  uart.o
[change@change:~/9_bl1$](mailto:change@change:%7E/9_bl1$)

make后生成boot.bin,使用./newtools.exe boot.bin boots.bin，即给boot.bin增加头部。因为BL0代码跳到BL1是会根据头部检验程序的合法性，上一篇也有说明。上面的程序我测试过，基本上都OK，测试留在下篇，引导u-boot第二阶段代码BL2.










