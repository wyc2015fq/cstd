# DM365 u-boot 启动分析 - maopig的专栏 - CSDN博客
2012年03月05日 22:19:32[maopig](https://me.csdn.net/maopig)阅读数：1748标签：[cmd																[makefile																[linux																[basic																[output																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)
个人分类：[u-boot/驱动开发/linux																[DM368																[u-boot/驱动开发](https://blog.csdn.net/maopig/article/category/1090389)](https://blog.csdn.net/maopig/article/category/919673)](https://blog.csdn.net/maopig/article/category/930963)
### 第一阶段：汇编语言启动先看u-boot/board/davinci/dm365_ipnc/下的文件。u-boot.lds
```
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
        . = 0x00000000;
        . = ALIGN(4);
        .text   :
        {
          cpu/arm926ejs/start.o (.text)
          *(.text)
        }
        . = ALIGN(4);
        .rodata : { *(.rodata) }
        . = ALIGN(4);
        .data : { *(.data) }
        . = ALIGN(4);
        .got : { *(.got) }
        . = .;
        __u_boot_cmd_start = .;
        .u_boot_cmd : { *(.u_boot_cmd) }
        __u_boot_cmd_end = .;
        . = ALIGN(4);
        __bss_start = .;
        .bss (NOLOAD) : { *(.bss) }
        _end = .;
}
```
config.mk
TEXT_BASE = 0x81080000
board_init.s
```
#include <config.h>
.globl  dv_board_init
dv_board_init:
        mov     pc, lr
```
第一个要链接的是cpu/arm926ejs/start.o，那么U-Boot的入口指令一定位于这个程序中。进入/u-boot/cpu/arm926ejs/。
查看start.s。流程为：
reset（复位）——cpu_init_crit（初始化CACHE，关闭MMU）——lowlevel_init（davinci/lowlevel_init.S下，设置pll,mux,memory）——relocate（把U-boot重新定位到RAM）——copy_loop（重新定位代码）——stack_setup（初始化堆栈）——clear_bss（清零bss）——clbss_l（bss段地址空间清零循环）——_start_armboot （start_armboot函数在lib_arm/board.c中实现）
在/u-boot/cpu/arm926ejs/下还有一些文件：
root@ss-desktop:/camera/u-boot/cpu/arm926ejs# ls
at91       cpu.c      da8xx    interrupts.c  omap     versatile
config.mk  cpuinfo.c  davinci  Makefile      start.S
root@ss-desktop:/camera/u-boot/cpu/arm926ejs# ls davinci/
dp83848.c  i2c.c            lxt972.c  nand.c   timer.c
ether.c    lowlevel_init.S  Makefile  reset.S
第二阶段：C语言启动
**lib_arm/board.c**
    start_armboot是U-Boot执行的第一个C语言函数，完成系统初始化工作，进入主循环，处理用户输入的命令。
1、根据如下定义进行一系列初始化；
init_fnc_t *init_sequence[] = {
    cpu_init,        /* basic cpu dependent setup 基本的处理器相关配置--cpu/arm926ejs/cpu.c */
#if defined(CONFIG_SKIP_RELOCATE_UBOOT)
    reloc_init,        /* Set the relocation done flag, must do this AFTER cpu_init(), but as soon as possible--lib_arm/board.c */
#endif
    board_init,        /* basic board dependent setup 基本的板级相关配置--board**/davinci/dm365_ipnc/**dm365_board.c */
    interrupt_init,        /* set up exceptions 初始化中断处理--cpu/arm926ejs/interrupts.c */
    env_init,        /* initialize environment 初始化环境变量--common/cmd_flash.c(??) */
    init_baudrate,        /* initialze baudrate settings 初始化波特率设置--lib_arm/board.c */
    serial_init,        /* serial communications setup 串口通讯设置*/
    console_init_f,        /* stage 1 init of console 控制台初始化阶段1--/common/console.c*/
    display_banner,        /* say that we are here 打印u-boot信息--/lib_arm/board.c */
#if defined(CONFIG_DISPLAY_CPUINFO)
    print_cpuinfo,        /* display cpu info (and speed) */
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
    checkboard,        /* display board info */
#endif
#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
    init_func_i2c,
#endif
    dram_init,        /* configure available RAM banks 配置可用的RAM--board**/davinci/dm365_ipnc/**dm365_board.c
*/
    display_dram_config,   /*显示RAM的配置大小--/lib_arm/board.c
*/
    NULL,
};
2、进行nand初始化
使用driver/mtd/nand.c中的nand_init()
3、设备初始化及控制台初始化
使用common/devices.c中的devices_init()；进行各设备的初始化，包括网络控制台设备的初始化drv_nc_init()；在初始化过程中将dev注册到全局链表devlist中；
使用common/console.c中的console_init_r()；这个函数中可将设备的读写接口挂在控制台的读写接口即标准输入/输出上，以实现对不同设备的适配；具体而言是由console_setfile()将dev挂入stdio_devices[file]数组；
4、各类杂项初始化，包括对eth的配置
misc_init_r()
5、进行ether的MII和PHY进行初始化
使用net/eth.c中的eth_initialize()
6、main_loop   /*总是试图自启动，循环不断执行*/
等待控制台命令，该命令可通过键盘输入或或者通过网络输入，不过对于main_loop命令解析层而言是透明的；
至此启动完成。
第三阶段：启动完全，开始运行
1. main_loop()在common/main.c中
1.1 读命令readline()
由于getc最终会调用设备读接口[stdio_devices[file]->getc ()]，因此此前将控制台适配为哪种设备，则现在就是和哪种设备通信；
1.2 执行命令run_command()
各种命令通过U_BOOT_CMD宏被链接器搜集到.u_boot_cmd所在的__u_boot_cmd_start和 __u_boot_cmd_end区间中；具体参考include/comman.h和board/davinci/dm365/u-boot.lds；
Find_cmd中，从__u_boot_cmd_start开始查找对应命令结构体变量；如果是tftp等网络命令，则执行 do_tftpb()->netboot_common()->NetLoop();
2. netloop()
在for (;;)循环中使用eth_rx()不断接收数据，同时通过一状态机决定是否处理完毕；
3. eth_rx()
这个函数实际上实现了数据由网络到上层协议栈再到应用层解析的整个过程，应用解析后会更改状态机状态；
对于tftp则是在tftp_start时由于利用NetSetHandler()指定了全局量packetHandler为TftpHandler；因此根据如下调用关系eth_rx()->net_receive()->packetHandler()，可知数据被tftphandler 处理并修改Netstate状态值;
4. nand
对于nand的write操作稍需注意，由于nand是按“与”操作才能写入，也就是擦除后，写入前nand中的bit值必须是1。这就意味着所写如果是 1，将不使nand的bit改变。在uboot nand的驱动中对于oob的fill也正是利用了这一点。看似将memory中的oob数据写到了nand oob中,但由于memory中的oob各bit是1，因此对nand oob值不产生影响。
第四阶段：U-boot引导内核
1. go命令的实现
    go命令调用 common/cmd_boot.c下do_go()函数，跳转到某个地址执行。如果在这个地址准备好了自引导的内核映像，就可以启动了。尽管go命令可以带变参，实际使用时一般不用来传递参数。
2. bootm命令的实现
    bootm命令调用 common/cmd_bootm.c下的do_bootm函数，这个函数专门用来引导各种操作系统，可以支持引导Linux、vxworks等操作系统。引导Linux时，调用do_boom_linux()函数。
3. do_bootm_linux函数的实现
do_bootm_linux函数在lib_arm/bootm.c下，是专门引导Linux映像的函数，它还可以处理ramdisk文件系统的映像。
