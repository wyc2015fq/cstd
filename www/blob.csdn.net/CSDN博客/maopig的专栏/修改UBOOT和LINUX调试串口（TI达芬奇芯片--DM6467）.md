# 修改UBOOT和LINUX调试串口（TI达芬奇芯片--DM6467） - maopig的专栏 - CSDN博客
2011年11月14日 23:42:50[maopig](https://me.csdn.net/maopig)阅读数：7413
Posted on 2011-10-31 10:53 [jamiedu](http://www.cnblogs.com/duzeming/) 阅读(889) [评论(0)](http://www.cnblogs.com/duzeming/archive/2011/10/31/2229905.html#commentform)[编辑](http://www.cnblogs.com/duzeming/archive/2011/10/31/2229905.html#)[收藏](http://www.cnblogs.com/duzeming/archive/2011/10/31/2229905.html#)![](http://www.cnblogs.com/duzeming/aggbug/2229905.html?type=1&webview=1)
1.1 概述
TI针对DM6467提供的UBOOT和内核默认都是串口0作为调试串口输出的，但现在我需要使用DM6467的UART0的modem功能，所以修改代码，改变调试串口为串口2。
需要修改的主要有几部分内容：
1. UBL 代码(这部分代码在刚上电的时候，初始化CPU和拷贝UBOOT到DDR，打印信息只有很少，所以不做修改)。
2. UBOOT代码。
3. linux内核驱动。
      使用的代码及版本请参考:[http://processors.wiki.ti.com/index.php/DaVinci_PSP_03.01_Beta_(r31)_Release_Notes](http://processors.wiki.ti.com/index.php/DaVinci_PSP_03.01_Beta_(r31)_Release_Notes)
2.1 修改UBOOT代码
     因为DM6467的串口是符合TL16C550标准的，所以驱动也是使用16550的驱动，默认情况下，我们只需要提供需要配置的串口的基地址和中断号等资源给16550的驱动就可以了，寄存器的配置不需要我们去关心。
    要用起DM6467的串口有几个地方的配置一定要注意：
　　　　1. 引脚复用寄存器(PINMUX0/1)；
　　　　2. VDD3P3V_PWDN寄存器，需要使能UART的相关引脚(bit4~bit9置零)
　　　　3.CLKCTL，bit24/25置零。
    在UBOOT里涉及到上面几个寄存器的配置的是在dm6467_evm.c的初始化部分
    我的修改如下：
```
1 static void davinci_hd_psc_enable ( void )
 2 {
 3     unsigned int alwaysonpdnum = 0;
 4 
 5     /* Note this function assumes that the Power Domains are alread on */
 6     REG(PSC_ADDR+0xA00+4*14) |= 0x03; /* EMAC */
 7     REG(PSC_ADDR+0xA00+4*15) |= 0x03; /* VDCE */
 8     REG(PSC_ADDR+0xA00+4*16) |= 0x03; /* Video Port */
 9     REG(PSC_ADDR+0xA00+4*17) |= 0x03; /* Video Port */
10     REG(PSC_ADDR+0xA00+4*20) |= 0x03; /* DDR2 */
11     REG(PSC_ADDR+0xA00+4*21) |= 0x03; /* EMIFA */
12     REG(PSC_ADDR+0xA00+4*26) |= 0x03; /* UART0 */
13     REG(PSC_ADDR+0xA00+4*27) |= 0x03; /* UART1 */
14     REG(PSC_ADDR+0xA00+4*28) |= 0x03; /* UART2 */
15     REG(PSC_ADDR+0xA00+4*31) |= 0x03; /* I2C */
16     REG(PSC_ADDR+0xA00+4*33) |= 0x03; /* GPIO */
17     REG(PSC_ADDR+0xA00+4*34) |= 0x03; /* TIMER0 */
18     REG(PSC_ADDR+0xA00+4*35) |= 0x03; /* TIMER1 */
19 
20     /* Set PTCMD.GO to 0x1 to initiate the state transtion for Modules in
21      * the ALWAYSON Power Domain
22 */
23     REG(PSC_PTCMD) = (1<<alwaysonpdnum);
24 
25     /* Wait for PTSTAT.GOSTAT0 to clear to 0x0 */
26     while(! (((REG(PSC_PTSTAT) >> alwaysonpdnum) & 0x00000001) == 0));
27 
28     /* Enable GIO3.3V cells used for EMAC (???) */
29     REG(VDD3P3V_PWDN) = (1<<27);  //disable clkout0
30 
31     /* Select UART function on UART0 */
32     REG(PINMUX0) &= ~(0x0000003f << 18);
34     REG(PINMUX1) = ((1<<4)|(1<<2)|(1<<0)); 
35 
36     /* Enable USB */
37     REG(PINMUX0) &= ~(0x80000000);
38 
39     /* Set the Bus Priority Register to appropriate value */
40     REG(VBPR) = 0x20;
41 }
```
　　接下来还有一个比较重要的地方需要修改，因为DM6467的串口是支持多种模式的，但16550的驱动是默认设备是工作在UART模式的，它没有去配置设备串口的工作模式，所以我们需要去配置一下串口的工作模式。配置DM6467的需要修改寄存器去MDR1。只部分的代码在UBOOT的driver目录下的ns16550.c文件中。
     修改如下：
```
1 void NS16550_init (NS16550_t com_port, int baud_divisor)
 2 {
 3     com_port->ier = 0x00;
 4 #if defined(CFG_DM6467_EVM)
 5         com_port->mdr1 = 0x00;  /* select mode */
 6 #endif
 7 #if defined(CONFIG_OMAP) && !defined(CONFIG_OMAP3_ZOOM2)
 8     com_port->mdr1 = 0x7;    /* mode select reset TL16C750*/
 9 #endif
10     com_port->lcr = UART_LCR_BKSE | UART_LCRVAL;
11     com_port->dll = 0;
12     com_port->dlm = 0;
13     com_port->lcr = UART_LCRVAL;
14     com_port->mcr = UART_MCRVAL;
15     com_port->fcr = UART_FCRVAL;
16     com_port->lcr = UART_LCR_BKSE | UART_LCRVAL;
17     com_port->dll = baud_divisor & 0xff;
18     com_port->dlm = (baud_divisor >> 8) & 0xff;
19     com_port->lcr = UART_LCRVAL;
20 #if defined(CONFIG_OMAP) && !defined(CONFIG_OMAP3_ZOOM2)
21 #if defined(CONFIG_APTIX)
22     com_port->mdr1 = 3;    /* /13 mode so Aptix 6MHz can hit 115200 */
23 #else
24     com_port->mdr1 = 0;    /* /16 is proper to hit 115200 with 48MHz */
25 #endif
26 #endif /* CONFIG_OMAP */
27 }
```
    修改完上面的代码后，就可以在配置文件里添加配置信息，并修改调试调试串口号了。打开/include/configs/davinci_dm6467_evm.c，找到串口配置部分，修改如下：
```
1 /*====================*/
 2 /* Serial Driver info */
 3 /*====================*/
 4 #define CONFIG_SYS_NS16550
 5 #define CONFIG_SYS_NS16550_SERIAL
 6 #define CONFIG_SYS_NS16550_REG_SIZE    4         /* NS16550 register size, byteorder */
 7 //#define CONFIG_SYS_NS16550_COM1        0x01c20000    /* Base address of UART0 */
 8 //#define CONFIG_CONS_INDEX        1        /* use UART0 for console */
 9 //#define CONFIG_SYS_NS16550_COM2        0x01c20400    /* Base address of UART1 */
10 //#define CONFIG_CONS_INDEX        2        /* use UART1 for console */
11 #define CONFIG_SYS_NS16550_COM3        0x01c20800    /* Base address of UART2 */
12 #define CONFIG_CONS_INDEX        3        /* use UART2 for console */
13 #define CONFIG_SYS_NS16550_CLK        24000000    /* Input clock to NS16550 */
14 #define CONFIG_BAUDRATE            115200        /* Default baud rate */
15 #define CONFIG_SYS_BAUDRATE_TABLE    { 9600, 19200, 38400, 57600, 115200 }
```
    重新编译UBOOT，下载测试，这时候UBOOT的打印信息应该从串口2输出了。
    正常情况下可以输出到下面的内容：
```
Starting kernel ...
```
3.1 修改内核调试串口
   3.1.1 内核解压输出信息
        内核解压时使用的串口驱动是独立于UBOOT或内核的串口驱动的，解压时输出的打印信息的相关的代码在/arch/arm/mach_davinci/include/mach/uncompress.h内。这里的串口输出代码很简单，只是往数据寄存器写数据，并没有对串口进行相关的配置，所以这里串口的正常输出需要依赖UBOOT对串口的正确配置。uncompress.h文件修改如下：
```
1 static u32 *get_uart_base(void)
2 {
3     if (__machine_arch_type == MACH_TYPE_DAVINCI_DA830_EVM ||
4         __machine_arch_type == MACH_TYPE_DAVINCI_DA850_EVM)
5         return (u32 *)DA8XX_UART2_BASE;
6     else
7         //return (u32 *)DAVINCI_UART0_BASE;
8         return (u32 *)DAVINCI_UART2_BASE; /* 这里修改为对应的寄存器基地址 */
9 }
```
      编译内核,通过刚才更新的UBOOT下载内核，并启动应该就可以看到熟悉的解压信息了：
```
1 Uncompressing Linux................................................................................................................................................................done, booting the kernel.
```
    3.1.2 修改内核驱动，改变内核调试串口
    因为内核启动时会对上面提到的几个寄存器作修改，所以需要修改/arch/arm/mach_davinci/board_dm646x_evm.c文件来确认2.1提到的寄存器的配置，同时需要使能UART1或UART2的时钟，默认情况下是只使能UART0的时钟。内核串口驱动就不需要修改了，因为也是16550的驱动，这份驱动也没有去配置串口的工作模式，所以UBOOT里配置了寄存器就行了，除非你想确认寄存器的配置，防止启动内核时寄存器被修改了或想控制串口的工作模式。
   使能UART1/2时钟:
```
1 static struct davinci_uart_config uart_config __initdata = {
2 
3     .enabled_uarts = (1 << 0)|(1 << 1)|(1 << 2),
4 
5 };
```
  重新编译内核，启动UBOOT，修改内核启动参数：
```
console=ttyS2,115200n8
```
下载内核，启动～～顺利的话就可以在UART2上输出系统的调试信息了：
```
Uncompressing Linux.......................................................................................................................... done, booting the kernel.
Linux version 2.6.32-rc2-davinci1 (root@ubuntuSv) (gcc version 4.3.3 (Sourcery G++ Lite 2009q1-203) ) #107 PREEMPT Tue Oct 25 08:04:49 UTC 2011
CPU: ARM926EJ-S [41069265] revision 5 (ARMv5TEJ), cr=00053177
CPU: VIVT data cache, VIVT instruction cache
Machine: ZMV-6467
Memory policy: ECC disabled, Data cache writeback
DaVinci dm6467_rev3.x variant 0x1
```
