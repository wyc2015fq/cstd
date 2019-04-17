# 嵌入式 uboot以及kernel添加看门狗临时记录(个人记录未整理乱) - DoubleLi - 博客园






Uboot_Kernerl_Add_Watch_Dog：



U-Boot 2010.06 (Nov 01 2013 - 15:28:44)


DRAM:  128 MiB
Check spi flash controller v350... Found
Spi(cs1) ID: 0xEF 0x40 0x18 0x00 0x00 0x00
Spi(cs1): Block:64KB Chip:16MB Name:"W25Q128B"
*** Warning - bad CRC, using default environment


In:    serial
Out:   serial
Err:   serial
Hit any key to stop autoboot:  0 
16384 KiB hi_sfc at 0:0 is now current device


## Booting kernel from Legacy Image at 82000000 ...
   Image Name:   Linux-3.0.8
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    2735184 Bytes = 2.6 MiB
   Load Address: 80008000
   Entry Point:  80008000
   Loading Kernel Image ... OK
OK


Starting kernel ...


Uncompressing Linux... done, booting the kernel.
Linux version 3.0.8 (root@u12d32) (gcc version 4.4.1 (Hisilicon_v100(gcc4.4-290+uclibc_0.9.32.1+eabi+linuxpthread)) ) #17 Tue Mar 25 16:34:29 CST 2014
CPU: ARM926EJ-S [41069265] revision 5 (ARMv5TEJ), cr=00053177
CPU: VIVT data cache, VIVT instruction cache
Machine: hi3518
Memory policy: ECC disabled, Data cache writeback
AXI bus clock 200000000.
Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 16256
Kernel command line: mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(config),3072k(kernel),11264k(rootfs)
PID hash table entries: 256 (order: -2, 1024 bytes)
Dentry cache hash table entries: 8192 (order: 3, 32768 bytes)
Inode-cache hash table entries: 4096 (order: 2, 16384 bytes)
Memory: 64MB = 64MB total
Memory: 59748k/59748k available, 5788k reserved, 0K highmem
Virtual kernel memory layout:
    vector  : 0xffff0000 - 0xffff1000   (   4 kB)
    fixmap  : 0xfff00000 - 0xfffe0000   ( 896 kB)
    DMA     : 0xffc00000 - 0xffe00000   (   2 MB)
    vmalloc : 0xc4800000 - 0xfe000000   ( 920 MB)
    lowmem  : 0xc0000000 - 0xc4000000   (  64 MB)
    modules : 0xbf000000 - 0xc0000000   (  16 MB)
      .init : 0xc0008000 - 0xc0029000   ( 132 kB)
      .text : 0xc0029000 - 0xc04cf000   (4760 kB)
      .data : 0xc04d0000 - 0xc04f35a0   ( 142 kB)
       .bss : 0xc04f35c4 - 0xc0510e78   ( 119 kB)
SLUB: Genslabs=13, HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
NR_IRQS:32 nr_irqs:32 32
sched_clock: 32 bits at 100MHz, resolution 10ns, wraps every 42949ms
Console: colour dummy device 80x30
Calibrating delay loop... 218.72 BogoMIPS (lpj=1093632)
pid_max: default: 32768 minimum: 301
Mount-cache hash table entries: 512
CPU: Testing write buffer coherency: ok
devtmpfs: initialized
NET: Registered protocol family 16
Serial: AMBA PL011 UART driver
uart:0: ttyAMA0 at MMIO 0x20080000 (irq = 5) is a PL011 rev2
console [ttyAMA0] enabled
uart:1: ttyAMA1 at MMIO 0x20090000 (irq = 5) is a PL011 rev2
bio: create slab <bio-0> at 0
SCSI subsystem initialized
usbcore: registered new interface driver usbfs
usbcore: registered new interface driver hub
usbcore: registered new device driver usb
Switching to clocksource timer1
NET: Registered protocol family 2
IP route cache hash table entries: 1024 (order: 0, 4096 bytes)
TCP established hash table entries: 2048 (order: 2, 16384 bytes)
TCP bind hash table entries: 2048 (order: 1, 8192 bytes)
TCP: Hash tables configured (established 2048 bind 2048)
TCP reno registered
UDP hash table entries: 256 (order: 0, 4096 bytes)
UDP-Lite hash table entries: 256 (order: 0, 4096 bytes)
NET: Registered protocol family 1
RPC: Registered named UNIX socket transport module.
RPC: Registered udp transport module.
RPC: Registered tcp transport module.
RPC: Registered tcp NFSv4.1 backchannel transport module.
NetWinder Floating Point Emulator V0.97 (double precision)
VFS: Disk quotas dquot_6.5.2
Dquot-cache hash table entries: 1024 (order 0, 4096 bytes)
JFFS2 version 2.2. (NAND) 漏 2001-2006 Red Hat, Inc.
fuse init (API version 7.16)
msgmni has been set to 116
Block layer SCSI generic (bsg) driver version 0.4 loaded (major 253)
io scheduler noop registered
io scheduler deadline registered (default)
io scheduler cfq registered
brd: module loaded
loop: module loaded
Spi id table Version 1.22
Spi(cs1) ID: 0xEF 0x40 0x18 0x00 0x00 0x00
SPI FLASH start_up_mode is 3 Bytes
Spi(cs1): 
Block:64KB 
Chip:16MB 
Name:"W25Q128B"
spi size: 0x16777216
chip num: 1
5 cmdlinepart partitions found on MTD device hi_sfc
Creating 5 MTD partitions on "hi_sfc":
0x000000000000-0x000000040000 : "boot"
0x000000040000-0x000000060000 : "env"
0x000000060000-0x000000080000 : "config"
0x000000080000-0x000000380000 : "kernel"
0x000000380000-0x000000e80000 : "rootfs"
Fixed MDIO Bus: probed
himii: probed
PPP generic driver version 2.4.2
PPP Deflate Compression module registered
PPP BSD Compression module registered
PPP MPPE Compression module registered
NET: Registered protocol family 24
ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
hiusb-ehci hiusb-ehci.0: HIUSB EHCI
hiusb-ehci hiusb-ehci.0: new USB bus registered, assigned bus number 1
hiusb-ehci hiusb-ehci.0: irq 15, io mem 0x100b0000
hiusb-ehci hiusb-ehci.0: USB 0.0 started, EHCI 1.00
hub 1-0:1.0: USB hub found
hub 1-0:1.0: 1 port detected
ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
hiusb-ohci hiusb-ohci.0: HIUSB OHCI
hiusb-ohci hiusb-ohci.0: new USB bus registered, assigned bus number 2
hiusb-ohci hiusb-ohci.0: irq 16, io mem 0x100a0000
hub 2-0:1.0: USB hub found
hub 2-0:1.0: 1 port detected
usbcore: registered new interface driver cdc_acm
cdc_acm: USB Abstract Control Model driver for USB modems and ISDN adapters
usbcore: registered new interface driver cdc_wdm
Initializing USB Mass Storage driver...
usbcore: registered new interface driver usb-storage
USB Mass Storage support registered.
usbcore: registered new interface driver ums-alauda
usbcore: registered new interface driver ums-datafab
usbcore: registered new interface driver ums-freecom
usbcore: registered new interface driver ums-isd200
usbcore: registered new interface driver ums-jumpshot
usbcore: registered new interface driver ums-sddr09
usbcore: registered new interface driver ums-sddr55
usbcore: registered new interface driver mdc800
mdc800: v0.7.5 (30/10/2000):USB Driver for Mustek MDC800 Digital Camera
usbcore: registered new interface driver usbserial
usbserial: USB Serial Driver core
USB Serial support registered for GSM modem (1-port)
usbcore: registered new interface driver option
option: v0.7.2:USB Driver for GSM modems
mousedev: PS/2 mouse device common for all mice
usbcore: registered new interface driver usbhid
usbhid: USB HID core driver
TCP cubic registered
Initializing XFRM netlink socket
NET: Registered protocol family 10
NET: Registered protocol family 17
NET: Registered protocol family 15
lib80211: common routines for IEEE802.11 drivers
Registering the dns_resolver key type
registered taskstats version 1
drivers/rtc/hctosys.c: unable to open rtc device (rtc0)
鴘sb 1-1: new high speed USB device number 2 using hiusb-ehci
VFS: Mounted root (jffs2 filesystem) on device 31:4.
devtmpfs: mounted
Freeing init memory: 132K


            _ _ _ _ _ _ _ _ _ _ _ _
            \  _  _   _  _ _ ___
            / /__/ \ |_/
           / __   /  -  _ ___
          / /  / /  / /
  _ _ _ _/ /  /  \_/  \_ ______
___________\___\__________________


[RCS]: /etc/init.d/S00devs
mknod: /dev/console: File exists
mknod: /dev/ttyAMA0: File exists
mknod: /dev/ttyAMA1: File exists
mknod: /dev/null: File exists
[RCS]: /etc/init.d/S01udev
udevd (600): /proc/600/oom_adj is deprecated, please use /proc/600/oom_score_adj instead.
[RCS]: /etc/init.d/S80network
/
Auto login as root ...
/opt/joseph
Jan  1 08:00:03 login[606]: root login on 'ttyS000'
Welcome to HiLinux.
Congratulate joseph_power_init ok  !
Congratulate joseph_led_init ok  !
None of nfsroot found in cmdline.
[root@njqy]#


1、P447(硬件设计用户指南)

系统初始化配置
系统上电复位后WatchDog计数器处于停止计数状态，在系统初始化过程中需要将
WatchDog初始化并启动其运行。WatchDog的初始化过程如下：
1.  写寄存器WDG_LOAD，设定计数初值。
2.  写寄存器WDG_CONTROL，打开中断屏蔽并启动WatchDog计数。
3.  写寄存器WDG_LOCK，给WatchDog上锁，防止软件错误修改WatchDog的配置。
----结束
中断处理过程
收到WatchDog发出的中断后，应及时清除其中断状态，并使其载入计数初值重新开始
计数。WatchDog中断处理的过程如下所示：
1.  向WDG_LOCK写0x1ACC_E551，为WatchDog开锁。
2.  写寄存器WDG_INTCLR，清除WatchDog的中断状态，同时也使WatchDog自动载入
计数初值重新开始计数。
3.  向寄存器WDG_LOCK写入0x1ACC_E551以外的任何值，给WatchDog上锁。
----结束 
Hi3518 HD IP Camera SoC 
用户指南  3 系统
文档版本04 (2013-09-25) 
海思专有和保密信息
版权所有? 深圳市海思半导体有限公司
3-176
关闭WatchDog 
向寄存器WDG_CONTROL[inten]控制位写入0或1控制WatchDog的状态：
z  0：关闭WatchDog；
z  1：打开WatchDog。

表3-33 WatchDog寄存器概览（基址是0x2004_0000）
偏移地址  名称  描述  页码
0x0000 WDG_LOAD  计数初值寄存器  3-174
0x0004 WDG_VALUE  计数器当前值寄存器  3-175
0x0008 WDG_CONTROL  控制寄存器  3-175
0x000C WDG_INTCLR  中断清除寄存器  3-176
0x0010 WDG_RIS  原始中断寄存器  3-176
0x0014 WDG_MIS  屏蔽后中断寄存器  3-176
0x0018～
0x0BFC 
RESERVED  保留  - 
0x0C00 WDG_LOCK  LOCK寄存器  3-177

Examples:

1、
uboot中默认看门狗策略由两个宏CONFIG_HW_WATCHDOG和CONFIG_WATCHDOG来使能。


此策略是在代码执行中的不同阶段，添加喂狗代码。

这种喂狗方法会使代码很乱，uboot中到处都充斥的喂狗代码。另外这种方法对代码执行时间是敏感的，如果有段代码执行时间很长（如搬运code），则需要添加喂狗代码，很繁。

uboot的默认策略比较适合外部看门狗。

我们现在用的是CPU内部看门狗，直接无视上述两宏。我们要在watchdog interrupt中喂狗，即只在watchdog interrupt handler中喂狗，比较简便
2、
在include/watchdog.h文件中有宏定义 
#ifdef CONFIG_HW_WATCHDOG 
    #if defined(__ASSEMBLY__) 
        #define WATCHDOG_RESET bl hw_watchdog_reset 
    #else 
        extern void hw_watchdog_reset(void); 
        #define WATCHDOG_RESET hw_watchdog_reset 
    #endif 
所以，需要打开CONFIG_HW_WATCHDOG开关以及实现hw_watchdog_reset喂狗函数。
3、


Hi3518C_Current_Uboot_Env :




joseph # printenv
bootargs=mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(config),3072k(kernel),11264k(rootfs)
bootcmd=sf probe 0;sf read 0x82000000 0x80000 0x300000;bootm 0x82000000
bootdelay=1
baudrate=115200
ethaddr=00:00:23:34:45:66
ipaddr=192.168.1.10
serverip=192.168.1.2
netmask=255.255.255.0
bootfile="uImage"
stdin=serial
stdout=serial
stderr=serial
verify=n
ver=U-Boot 2010.06 (Nov 01 2013 - 15:28:44)

Environment size: 459/262140 bytes



















#define GPIO_LIGHT_DAT (IO_ADDRESS(0x201A0000 + 0x03FC))
#define GPIO_LIGHT_DIR (IO_ADDRESS(0x201A0000 + 0x0400))
#define GPIO_LIGHT_MUX (IO_ADDRESS(0x200F0000 + 0x009C))




#define GPIO_1_1_DAT (IO_ADDRESS(0x201503FC))//red
#define GPIO_1_1_DIR (IO_ADDRESS(0x20150400))
#define GPIO_1_1_MUX (IO_ADDRESS(0x200F0004))


#define GPIO_6_0_DAT (IO_ADDRESS(0x201A03FC))//blue
#define GPIO_6_0_DIR   (IO_ADDRESS(0x201A0400))
#define GPIO_6_0_MUX   (IO_ADDRESS(0x200F0080))










/*alter by kj*/
/*
Author : kj
Time : 2014-04-19
Function : 
control gpio led
Instruction :
1 - red on
2 - red off
3 - blue on
4 - blue off
5 - light on
6 - light off


writel(0x2, REG_BASE_SCTL+REG_SC_SYSRES);
 */
static void joseph_set_gpio_hi3518c(int led_status)
{
switch(led_status)
{
case 1:
{
printk("\nThe uboot set red led on and blue led off begin alter by kj !!!\n");

writel(0x00000002,GPIO_1_1_DIR);
writel(0x00000000,GPIO_1_1_MUX);
writel(0x00000002,GPIO_1_1_DAT);// 1 - on ;0 - off


writel(0x00000001,GPIO_6_0_DIR);
writel(0x00000000,GPIO_6_0_MUX);
writel(0x00000000,GPIO_6_0_DAT);// blue off


break;  
}
case 2:
{
printk("\nThe uboot  red off light begin alter by kj !!!\n");

writel(0x00000002,GPIO_1_1_DIR);
writel(0x00000000,GPIO_1_1_MUX);
writel(0x00000000,GPIO_1_1_DAT);// red off


break;  
}
case 3:
{


printk("\nThe uboot set bule on begin alter by kj !!!\n");

writel(0x00000001,GPIO_6_0_DIR);
writel(0x00000000,GPIO_6_0_MUX);
writel(0x00000001,GPIO_6_0_DAT);//bule on
break;  
}
case 4:
{


printk("\nThe uboot set blue off begin alter by kj !!!\n");

writel(0x00000001,GPIO_6_0_DIR);
writel(0x00000000,GPIO_6_0_MUX);
writel(0x00000000,GPIO_6_0_DAT);//blue off
break;  
}
case 5:
{
printk("\nThe uboot set light on begin alter by kj !!!\n");

writel(0x00000080,GPIO_LIGHT_DIR);
writel(0x00000000,GPIO_LIGHT_MUX);
writel(0x00000000,GPIO_LIGHT_DAT);//light on

break;  
}
case 6:
{


printk("\nThe uboot set light off begin alter by kj !!!\n");

writel(0x00000080,GPIO_LIGHT_DIR);
writel(0x00000001,GPIO_LIGHT_MUX);
writel(0x00000000,GPIO_LIGHT_DAT);//light off


break;  
}
default:
break;
}






}








/*====================================Hi3518c start.S Begin 2014-04-20=============================================*/
/*
 *  armboot - Startup Code for ARM926EJS CPU-core
 *
 *  Copyright (c) 2003  Texas Instruments
 *
 *  ----- Adapted for OMAP1610 OMAP730 from ARM925t code ------
 *
 *  Copyright (c) 2001 Marius Gr?ger <mag@sysgo.de>
 *  Copyright (c) 2002 Alex Z?pke <azu@sysgo.de>
 *  Copyright (c) 2002 Gary Jennejohn <garyj@denx.de>
 *  Copyright (c) 2003 Richard Woodruff <r-woodruff2@ti.com>
 *  Copyright (c) 2003 Kshitij <kshitij@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */




#include <config.h>
#include <version.h>


/*
 *************************************************************************
 *
 * Jump vector table as in table 3.1 in [1]
 *
 *************************************************************************
 */




.globl _start //汇编程序都要提供一个_start符号并且用.globl声明
_start: b reset //B或BL指令引起处理器转移到“子程序名”处开始执行 复位
ldr     pc, _undefined_instruction 
ldr     pc, _software_interrupt
ldr     pc, _prefetch_abort
ldr     pc, _data_abort
ldr     pc, _not_used
ldr     pc, _irq
ldr     pc, _fiq


_undefined_instruction: .word undefined_instruction
_software_interrupt: .word software_interrupt
_prefetch_abort: .word prefetch_abort
_data_abort: .word data_abort
_not_used: .word not_used
_irq: .word irq
_fiq: .word fiq
_pad: .word 0x12345678 /* now 16*4=64 */


/*.fill 
     语法：.fill repeat, size, value
     含义是反复拷贝 size个字节，重复 repeat 次，
         其中 size 和 value 是可选的，默认值分别为 1 和 0.
*/
__blank_zone_start:
.fill 1024*4,1,0 //给某个具体的寄存器里填数
__blank_zone_end:


.globl _blank_zone_start
_blank_zone_start:
.word __blank_zone_start




.globl _blank_zone_end
_blank_zone_end:
.word __blank_zone_end


.balignl 16,0xdeadbeef
/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */


_TEXT_BASE:
.word TEXT_BASE


.globl _armboot_start
_armboot_start:
.word _start


/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
.word __bss_start


.globl _bss_end
_bss_end:
.word _end


#ifdef CONFIG_USE_IRQ
/* IRQ stack memory (calculated at run-time) */
.globl IRQ_STACK_START
IRQ_STACK_START:
.word 0x0badc0de


/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
.word 0x0badc0de
#endif




_clr_remap_spi_entry:
    .word   SF_TEXT_ADRS + do_clr_remap - TEXT_BASE
_clr_remap_nand_entry:
    .word   NAND_TEXT_ADRS + do_clr_remap - TEXT_BASE


/*
 * the actual reset code
 */


reset:
/*
* set the cpu to SVC32 mode
*/
mrs r0,cpsr //将状态寄存器的内容传送至通用寄存器,将CPSR中的内容传送至R0
bic r0,r0,#0x1f //位清除指令 将R0最低5位清零，其余位不变 工作模式位清零
orr r0,r0,#0xd3 //工作模式位设置为“10011”（管理模式），并将中断禁止位和快中断禁止位置1 "1101 0011" 指令用于在两个操作数上进行逻辑或运算，并把结果放置到目的寄存器中
msr cpsr,r0 //将通用寄存器的内容传送至状态寄存器,将中的内容R0传送至CPSR


/*
* we do sys-critical inits only at reboot,
* not when booting from ram!
*/


/*
* flush v4 I/D caches
*/
mov r0, #0 //置零ro通用寄存器
mcr p15, 0, r0, c7, c7, 0
/* flush v3/v4 cache */ //向c7写入0将使ICache与DCache无效 "0"表示省略opcode_2 MCR{<cond>} p15, 0, <Rd>, <CRn>, <CRm>{,<opcode_2>}
mcr p15, 0, r0, c8, c7, 0
/* flush v4 TLB */ //MCR{条件} 协处理器编码，协处理器操作码1，源寄存器，目的寄存器1，目的寄存器2，协处理器操作码2


/*
* disable MMU stuff and caches
*/
mrc p15, 0, r0, c1, c0, 0
bic r0, r0, #0x00002300
/* clear bits 13, 9:8 (--V- --RS) */
bic r0, r0, #0x00000087
/* clear bits 7, 2:0 (B--- -CAM) */
orr r0, r0, #0x00000002
/* set bit 2 (A) Align */
mcr p15, 0, r0, c1, c0, 0 //保存r0到控制寄存器


mov r0, pc, lsr#24 //LSL、LSR、ASR、ROR 寄存器移位
cmp r0, #0x0
bne do_clr_remap //检测是否需要跳转，PC的高八位如果不为0（已经在ram中运行了）则跳转 不等于则调转


check_start_mode:
ldr r0, =REG_BASE_SCTL
ldr r0, [r0, #REG_SYSSTAT]
mov r6, r0, lsr#5
and r6, #0x1


/* reg[0x2005008c:5]:
* 0: start from spi
* 1: start from nand
*/


cmp r6, #BOOT_FROM_SPI
ldreq   pc, _clr_remap_spi_entry


ldr pc, _clr_remap_nand_entry
@b .
/* bug here */


/*
LDR和STR用来存取内存,关于"索引偏移"，你是不是指pre-indexed addressing和post-indexed addressingpre-indexed addressing是指地址经过运算不写回基址寄存器post-indexed addressing则回写到基址寄存器比如pre-indexed addressing:mov r1,#0STR r0, [r1, #0x10]       ;r1+0x10这个是所用的实际地址值，但是不回写入r1,在此句之后，r1=0post-indexed addressing:STR r0, [r1]， #0x10       ;r1+0x10这个是所用的实际地址值，这个值回写入r1，此句之后，r1=0x10
*/
do_clr_remap:
ldr     r4, =REG_BASE_SCTL //用来从存储器（确切地说是地址空间）中装载数据到通用寄存器 系统控制器寄存器 0x20050000 写地址
ldr r0, [r4, #REG_SC_CTRL] //加载32位的立即数或一个地址值到指定寄存器 不回写 其实是r4+#0x0是实际地址值


/* reg[0x20050000:8]:
* 0: keep remap
* 1: clear remap 重映射
*/
@Set clear remap bit.
orr r0, #(1<<8) //第八位置1
str r0, [r4, #REG_SC_CTRL] //不回写 @表示注释


@Setup TCM (ENABLED, 2KB) // TCM时钟门控使能
ldr     r0, =( 1 | (MEM_CONF_ITCM_SIZE<<2) | MEM_BASE_ITCM)
mcr     p15, 0, r0, c9, c1, 1


@enable I-Cache now
mrc p15, 0, r0, c1, c0, 0
orr r0, r0, #0x00001000
/* set bit 12 (I) I-Cache */
mcr p15, 0, r0, c1, c0, 0


@Check if I'm running in ddr //代码内存运行测试
mov r0, pc, lsr#28
cmp r0, #8
bleq    relocate //小于等于跳转


ldr     r0, _blank_zone_start
ldr     r1, _TEXT_BASE //代码段
sub     r0, r0, r1 //减法 sub a,b (a-b)
adrl    r1, _start //将相对于程序或相对于寄存器的地址载入寄存器中 adrl宽
add     r0, r0, r1 //加法
mov     r1, #0          /* flags: 0->normal 1->pm */
bl      init_registers //初始化寄存器


#ifndef CONFIG_SKIP_RELOCATE_UBOOT
relocate:
@copy arm exception table in 0 address
adrl r0, _start
mov r1, #0
mov r2, #0x100
/* copy arm Exception table to 0 addr */
add     r2, r0, r2
copy_exception_table:
ldmia   r0!, {r3 - r10}
stmia   r1!, {r3 - r10}
cmp     r0, r2
ble     copy_exception_table


@relocate U-Boot to RAM
adrl r0, _start
/* r0 <- current position of code   */
ldr r1, _TEXT_BASE
/* test if we run from flash or RAM */
cmp     r0, r1                  /* don't reloc during debug         */
beq     stack_setup
ldr r2, _armboot_start
ldr r3, _bss_start
sub r2, r3, r2
/* r2 <- size of armboot            */
add r2, r0, r2
/* r2 <- source end address         */


copy_loop:
ldmia r0!, {r3-r10}
/* copy from source address [r0]    */
stmia r1!, {r3-r10}
/* copy to   target address [r1]    */
cmp r0, r2
/* until source end addreee [r2]    */
ble copy_loop
#endif /* CONFIG_SKIP_RELOCATE_UBOOT */


/* Set up the stack
   */
stack_setup:
ldr r0, _TEXT_BASE
/* upper 128 KiB: relocated uboot   */
sub r0, r0, #CONFIG_SYS_MALLOC_LEN
/* malloc area    */
sub r0, r0, #CONFIG_SYS_GBL_DATA_SIZE /* bdinfo
   */
#ifdef CONFIG_USE_IRQ
sub r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
#endif
sub sp, r0, #12
/* leave 3 words for abort-stack    */
bic sp, sp, #7
/*8-byte alignment for ABI compliance*/


clear_bss:
ldr r0, _bss_start
/* find start of bss segment        */
ldr r1, _bss_end
/* stop here                        */
mov r2, #0x00000000
/* clear                            */


clbss_l:str r2, [r0]
/* clear loop...                    */
add r0, r0, #4
cmp r0, r1
ble clbss_l


ldr pc, _start_armboot


_start_armboot:
.word start_armboot




/*
 *************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************
 */
#ifndef CONFIG_SKIP_LOWLEVEL_INIT
cpu_init_crit:
/*
* flush v4 I/D caches
*/
mov r0, #0
mcr p15, 0, r0, c7, c7, 0
/* flush v3/v4 cache */
mcr p15, 0, r0, c8, c7, 0
/* flush v4 TLB */


/*
* disable MMU stuff and caches
*/
mrc p15, 0, r0, c1, c0, 0
bic r0, r0, #0x00002300
/* clear bits 13, 9:8 (--V- --RS) */
bic r0, r0, #0x00000087
/* clear bits 7, 2:0 (B--- -CAM) */
orr r0, r0, #0x00000002
/* set bit 2 (A) Align */
orr r0, r0, #0x00001000
/* set bit 12 (I) I-Cache */
mcr p15, 0, r0, c1, c0, 0


/*
* Go setup Memory and board specific bits prior to relocation.
*/
mov ip, lr
/* perserve link reg across call */
@bl lowlevel_init
/* go setup pll,mux,memory */
mov lr, ip
/* restore link */
mov pc, lr
/* back to my caller */
#endif /* CONFIG_SKIP_LOWLEVEL_INIT */


#ifndef CONFIG_PRELOADER
/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */


@
@ IRQ stack frame.
@
#define S_FRAME_SIZE 72


#define S_OLD_R0 68
#define S_PSR 64
#define S_PC 60
#define S_LR 56
#define S_SP 52


#define S_IP 48
#define S_FP 44
#define S_R10 40
#define S_R9 36
#define S_R8 32
#define S_R7 28
#define S_R6 24
#define S_R5 20
#define S_R4 16
#define S_R3 12
#define S_R2 8
#define S_R1 4
#define S_R0 0


#define MODE_SVC 0x13
#define I_BIT 0x80


/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */


.macro  bad_save_user_regs
@ carve out a frame on current user stack
sub sp, sp, #S_FRAME_SIZE
stmia sp, {r0 - r12}
@ Save user registers (now in svc mode) r0-r12


ldr r2, _armboot_start
sub r2, r2, #(CONFIG_STACKSIZE+CONFIG_SYS_MALLOC_LEN)
@ set base 2 words into abort stack
sub r2, r2, #(CONFIG_SYS_GBL_DATA_SIZE+8)
@ get values for "aborted" pc and cpsr (into parm regs)
ldmia r2, {r2 - r3}
add r0, sp, #S_FRAME_SIZE
@ grab pointer to old stack
add r5, sp, #S_SP
mov r1, lr
stmia r5, {r0 - r3}
@ save sp_SVC, lr_SVC, pc, cpsr
mov r0, sp
@ save current stack into r0 (param register)
.endm


.macro  irq_save_user_regs
sub sp, sp, #S_FRAME_SIZE
stmia sp, {r0 - r12}
@ Calling r0-r12
@ !!!! R8 NEEDS to be saved !!!! a reserved stack spot would be good.
add r8, sp, #S_PC
stmdb r8, {sp, lr}^
@ Calling SP, LR
str lr, [r8, #0]
@ Save calling PC
mrs r6, spsr
str r6, [r8, #4]
@ Save CPSR
str r0, [r8, #8]
@ Save OLD_R0
mov r0, sp
.endm


.macro  irq_restore_user_regs
ldmia sp, {r0 - lr}^
@ Calling r0 - lr
mov r0, r0
ldr lr, [sp, #S_PC]
@ Get PC
add sp, sp, #S_FRAME_SIZE
subs pc, lr, #4
@ return & move spsr_svc into cpsr
.endm


.macro get_bad_stack
ldr r13, _armboot_start
@ setup our mode stack
sub r13, r13, #(CONFIG_STACKSIZE+CONFIG_SYS_MALLOC_LEN)
@ reserved a couple spots in abort stack
sub r13, r13, #(CONFIG_SYS_GBL_DATA_SIZE+8)


str lr, [r13]
@ save caller lr in position 0 of saved stack
mrs lr, spsr
@ get the spsr
str lr, [r13, #4]
@ save spsr in position 1 of saved stack
mov r13, #MODE_SVC
@ prepare SVC-Mode
@ msr spsr_c, r13
msr spsr, r13
@ switch modes, make sure moves will execute
mov lr, pc
@ capture return pc
movs pc, lr
@ jump to next instruction & switch modes.
.endm


.macro get_irq_stack
@ setup IRQ stack
ldr sp, IRQ_STACK_START
.endm


.macro get_fiq_stack
@ setup FIQ stack
ldr sp, FIQ_STACK_START
.endm
#endif /* CONFIG_PRELOADER */


/*
 * exception handlers
 */
#ifdef CONFIG_PRELOADER
.align  5 //加上.align汇编语句后，指令就对齐
do_hang:
ldr sp, _TEXT_BASE
/* switch to abort stack */
1:
bl 1b
/* hang and never return */
#else /* !CONFIG_PRELOADER */
.align  5
undefined_instruction:
get_bad_stack
bad_save_user_regs
bl do_undefined_instruction


.align  5
software_interrupt:
get_bad_stack
bad_save_user_regs
bl do_software_interrupt


.align  5
prefetch_abort:
get_bad_stack
bad_save_user_regs
bl do_prefetch_abort


.align  5
data_abort:
get_bad_stack
bad_save_user_regs
bl do_data_abort


.align  5
not_used:
get_bad_stack
bad_save_user_regs
bl do_not_used


#ifdef CONFIG_USE_IRQ


.align  5
irq:
get_irq_stack
irq_save_user_regs
bl do_irq
irq_restore_user_regs


.align  5
fiq:
get_fiq_stack
/* someone ought to write a more effiction fiq_save_user_regs */
irq_save_user_regs
bl do_fiq
irq_restore_user_regs


#else


.align  5
irq:
get_bad_stack
bad_save_user_regs
bl do_irq


.align  5
fiq:
get_bad_stack
bad_save_user_regs
bl do_fiq


#endif
#endif /* CONFIG_PRELOADER */
#include "lowlevel_init.S"
/*====================================Hi3518c start.S End=============================================*/






/*====================================Hi3518c lowlevelinit.s Begin=====================================*/
.text
.align  2
.global  init_registers
.type init_registers, %function
init_registers:
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
@ link register save eliminated.
ldr r2, [r0, #0]
cmp r2, #0
mov r6, #1
bne .L43
.L2:
ldr r3, [r0, #4]
cmp r3, #0
bne .L43
ldr r3, [r0, #8]
cmp r3, #0
ldrne ip, [r0, #12]
bne .L3
ldr ip, [r0, #12]
cmp ip, #0
beq .L6
.L3:
cmp r1, #0
beq .L7
.L45:
tst ip, #2
beq .L8
and r4, ip, #248
mov r4, r4, lsr #3
add r4, r4, #1
cmp r4, #32
movne r4, r6, asl r4
andne ip, ip, #63488
ldr r5, [r2, #0]
movne ip, ip, lsr #11
subne r4, r4, #1
bicne r5, r5, r4, asl ip
ldrne r4, [r0, #4]
ldreq ip, [r0, #4]
orrne ip, r5, r4, asl ip
cmp r3, #0
str ip, [r2, #0]
beq .L11
.L29:
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
subs r3, r3, #1
bne .L29
.L11:
add r0, r0, #16
.L46:
ldr r2, [r0, #0]
cmp r2, #0
beq .L2
.L43:
cmp r1, #0
ldr r3, [r0, #8]
ldr ip, [r0, #12]
bne .L45
.L7:
tst ip, #4
beq .L18
and r4, ip, #248
mov r4, r4, lsr #3
add r4, r4, #1
cmp r4, #32
movne r4, r6, asl r4
andne ip, ip, #63488
ldr r5, [r2, #0]
movne ip, ip, lsr #11
subne r4, r4, #1
bicne r5, r5, r4, asl ip
ldrne r4, [r0, #4]
ldreq ip, [r0, #4]
orrne ip, r5, r4, asl ip
cmp r3, #0
str ip, [r2, #0]
beq .L11
.L32:
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
subs r3, r3, #1
bne .L32
add r0, r0, #16
b .L46
.L8:
tst ip, #131072
bne .L13
cmp r3, #0
beq .L11
.L31:
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
subs r3, r3, #1
bne .L31
add r0, r0, #16
b .L46
.L18:
tst ip, #262144
bne .L22
cmp r3, #0
beq .L11
.L34:
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
subs r3, r3, #1
bne .L34
add r0, r0, #16
b .L46
.L13:
and r4, ip, #16252928
mov r4, r4, lsr #19
add r4, r4, #1
mov r7, r6, asl r4
ldr r5, [r0, #4]
mov r8, ip, lsr #27
sub r7, r7, #1
.L16:
ldr ip, [r2, #0]
cmp r4, #32
andne ip, r7, ip, lsr r8
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
cmp r5, ip
bne .L16
cmp r3, #0
beq .L11
.L30:
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
subs r3, r3, #1
bne .L30
add r0, r0, #16
b .L46
.L22:
and r4, ip, #16252928
mov r4, r4, lsr #19
add r4, r4, #1
mov r8, r6, asl r4
ldr r5, [r0, #4]
sub r8, r8, #1
mov r7, ip, lsr #27
.L25:
ldr ip, [r2, #0]
cmp r4, #32
andne ip, r8, ip, lsr r7
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
cmp r5, ip
bne .L25
cmp r3, #0
beq .L11
.L33:
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
subs r3, r3, #1
bne .L33
add r0, r0, #16
b .L46
.L6:
@ 38 "lowlevel_init.c" 1
nop
@ 0 "" 2
bx lr
/*====================================Hi3518c lowlevelinit.s End=====================================*/


1.U-boot启动过程：

1）/uboot/cpu/arm926ejs/start.S文件是Uboot的入口程序。 
2）/uboot/lib_arm/board.c Uboot执行的第一个C函数，完成系统的初始化。

3）init_sequence[] 是基本的初始化函数指针。 
4）void start_armboot(void) 数序执行init_sequence[]数组中的初始化函数。

我把U-boot的运行过程简化描述如下： 
check board->dram_init->flash init->nand init->env relocate->ip,mac获取->device init->网卡初始化->进入main_loop函数，等待串口输入（无输入则执行bootcmd命令）。

2.U-boot和内核的主要关系式内核启动过程中参数的传递。

U-boot会给Linux Kernel传递很多参数，如：串口，RAM，videofb等。而内核也会读取和处理这些参数。两者之间通过struct tag来传递参数。U-boot把要传递给kernel的东西保存在struct tag数据结构中，启动kernel时，把这个结构体的物理地址传给kernel；Linux kernel通过这个地址，用parse_tags分析出传递过来的参数。 
1、u-boot给kernel传参数： 
在uboot/common/cmd_bootm.c文件中，bootm命令对应的do_bootm函数，当分析uImage中信息发现OS是Linux时，调用. /lib_arm/bootm.c文件中的do_bootm_linux函数来启动Linux kernel。 
2、内核读取U-boot传递的相关参数： 
对于Linux Kernel配合ARM平台启动时，先执行arch/arm/kernel/head.S，这个文件会调用arch/arm/kernel/head-common.S中的函数，在最后调用start_kernel。


start.s-----start_armboot()------main_loop()--------do_bootm()------do_bootm_linux()
1，start.s主要完成的是CPU的基本设置，比如设置位SVC模式，关闭watchog,关闭中断，关闭MMU（哥当时就笑了， 
      当然，这也提醒和我一样广大的同学们要在学习中注重基础，为什么要关MMu？ 因为uboot中所有对地址的操是使用物理地址，在的实地址，不存在虚拟地址，因此MMU必须关闭。晓得了不），还有设置堆栈，最后将剩下的uboot  搬到SDRAM（有人说是将全部的uboot代码搬过去，有　的又说是只讲  stage2的代码搬过去，到底是哪个不清楚，不过这个问题没必要纠结，没什么影响）。
2，start_armboot()主要完成的是对板子的外设的初始化，串口啊，ＬＣＤ啊，ＮＡＮＤ啊，设置一下环境变量啊，这个函数中重点有３个结构体，ｇｄ，ｂｄ，ｓｅｑｕｅｎｃｅ，搞清楚就ｏｋ了
3，ｍａｉｎｌｏｏｐ简单的说就是在板子上电延时的那会，如果有按键按下，则进入交互模式，就是一直解析你输入的命令，如果没有按下，则继续往下走，启动ｋｅｒｎｅｌ去
4，ｄｏｂｏｏｔｍ主要是解析ｋｅｒｎｅｌ的头，制作ｕｂｏｏｔ可以启动的ｋｅｒｎｅｌ时，需要在前面加头，头的内容包括有什么类型的映像，什么类型的操作系统，还有怎么压缩的等，最后还完成一个重要的工作就是将ｋｅｒｎｅｌ搬到ＳＤＲＡＭ中，接着调用do_bootm_linux()
5，do_bootm_linux()做的就比较简单了，设置一下启动参数，然后直接启动内核，ｏｖｅｒ






Hi3518c：
Uboot：
U-Boot 2010.06 (Nov 01 2013 - 15:28:44)
DRAM:  128 MiB
Check spi flash controller v350... Found
Spi(cs1) ID: 0xEF 0x40 0x18 0x00 0x00 0x00
Spi(cs1): Block:64KB Chip:16MB Name:"W25Q128B"
*** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial
Hit any key to stop autoboot:  0 
16384 KiB hi_sfc at 0:0 is now current device
start.s
1、./include/configs/hi3518c.h:86:#define SF_TEXT_ADRS
(SFC_MEM_BASE)
2、指针寄存器包括堆栈寄存器SP(stack pointer)和基数指针寄存器BP(base pointer)
3、跳转到第二阶段代码入口
    ldr  pc, _start_armboot
_start_armboot:  .word start_armboot
4、./arch/arm/lib/board.c:383:void start_armboot (void)
{
U-Boot使用了一个结构体gd_t来存储全局数据区的数据
结构体bd_t在include/asm-arm.u/u-boot.h中定义
U-Boot使用一个数组init_sequence来存储对于大多数开发板都要执行的初始化函数的函数指针

 init_fnc_t *init_sequence[] = {
#if defined(CONFIG_ARCH_CPU_INIT)
arch_cpu_init,
/* basic arch cpu dependent setup */
#endif
timer_init,  /* initialize timer before usb init */
board_init,  /* basic board dependent setup */
#if defined(CONFIG_USE_IRQ)
interrupt_init,
/* set up exceptions */
#endif
// timer_init,
/* initialize timer */
#ifdef CONFIG_FSL_ESDHC
get_clocks,
#endif
env_init,  /* initialize environment */
init_baudrate,
/* initialze baudrate settings */
serial_init,  /* serial communications setup */
console_init_f,
/* stage 1 init of console */
display_banner,
/* say that we are here */
#if defined(CONFIG_DISPLAY_CPUINFO)
print_cpuinfo,
/* display cpu info (and speed) */
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
checkboard,  /* display board info */
#endif
#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
init_func_i2c,
#endif
dram_init,  /* configure available RAM banks */
#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
arm_pci_init,
#endif
display_dram_config,
NULL,
};
  } 
5、 display_flash_config (flash_init ());调用spiflash驱动进行flash ID 以及type初始化以及识别
6、进入倒计时等候用户终止uboot进入命令行：
{
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
s = getenv ("bootdelay");
bootdelay = s ? (int)simple_strtol(s, NULL, 10) : CONFIG_BOOTDELAY;

debug ("### main_loop entered: bootdelay=%d\n\n", bootdelay);

# ifdef CONFIG_BOOT_RETRY_TIME
init_cmd_timeout ();
# endif  /* CONFIG_BOOT_RETRY_TIME */

#ifdef CONFIG_POST
if (gd->flags & GD_FLG_POSTFAIL) {
s = getenv("failbootcmd");
}
else
#endif /* CONFIG_POST */
#ifdef CONFIG_BOOTCOUNT_LIMIT
if (bootlimit && (bootcount > bootlimit)) {
printf ("Warning: Bootlimit (%u) exceeded. Using altbootcmd.\n",
       (unsigned)bootlimit);
s = getenv ("altbootcmd");
}
else
#endif /* CONFIG_BOOTCOUNT_LIMIT */
s = getenv ("bootcmd");

debug ("### main_loop: bootcmd=\"%s\"\n", s ? s : "<UNDEFINED>");

if (bootdelay >= 0 && s && !abortboot (bootdelay)) {
# ifdef CONFIG_AUTOBOOT_KEYED
int prev = disable_ctrlc(1);
/* disable Control C checking */
# endif

# ifndef CONFIG_SYS_HUSH_PARSER
run_command (s, 0);
# else
parse_string_outer(s, FLAG_PARSE_SEMICOLON |
   FLAG_EXIT_FROM_LOOP);
# endif

# ifdef CONFIG_AUTOBOOT_KEYED
disable_ctrlc(prev);
/* restore Control C checking */
# endif
}


} 


7、Uboot被abort的时候需要在uboot等待用户命令的时候喂狗：
{
location:/common/console.c
int fgetc(int file)
{
if (file < MAX_FILES) {
#if defined(CONFIG_CONSOLE_MUX)
/*
* Effectively poll for input wherever it may be available.
*/
for (;;) {
/*
* Upper layer may have already called tstc() so
* check for that first.
*/
if (tstcdev != NULL)
return console_getc(file);
console_tstc(file);
#ifdef CONFIG_WATCHDOG
/*
* If the watchdog must be rate-limited then it should
* already be handled in board-specific code.
*/
udelay(1);
#endif
}
#else
return console_getc(file);
#endif
}

return -1;
}
}


注意：在烧写文件系统或者内核的时候以及keimit的时候进行喂狗


## Booting kernel from Legacy Image at 82000000 ...
   Image Name:   Linux-3.0.8
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    2735184 Bytes = 2.6 MiB
   Load Address: 80008000
   Entry Point:  80008000
   Loading Kernel Image ... OK
OK


Starting kernel ...


Uncompressing Linux... done, booting the kernel.
Linux version 3.0.8 (root@u12d32) (gcc version 4.4.1 (Hisilicon_v100(gcc4.4-290+uclibc_0.9.32.1+eabi+linuxpthread)) ) #17 Tue Mar 25 16:34:29 CST 2014
CPU: ARM926EJ-S [41069265] revision 5 (ARMv5TEJ), cr=00053177
CPU: VIVT data cache, VIVT instruction cache
Machine: hi3518
Memory policy: ECC disabled, Data cache writeback
AXI bus clock 200000000.
Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 16256
Kernel command line: mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(config),3072k(kernel),11264k(rootfs)
PID hash table entries: 256 (order: -2, 1024 bytes)
Dentry cache hash table entries: 8192 (order: 3, 32768 bytes)
Inode-cache hash table entries: 4096 (order: 2, 16384 bytes)
Memory: 64MB = 64MB total
Memory: 59748k/59748k available, 5788k reserved, 0K highmem
8、uboot没有被abort的时候进入bootm命令
{
1、uboot首先把内核镜像拷贝到内存地址为0x80000000的地方，然后执行bootm 0x80000000命令

bootm命令来自 common/cmd_bootm.c文件，完成的功能有

》CRC校验image head struct的64个字节的正确性

》根据镜像头中指定的压缩类型（比如gzip），把kernel解压到指定的位置hdr->ih_load（0x80800000）

》调用来自lib_sh/sh_linux.c中的函数do_bootm_linux( . . . )，启动内核



2、接下来，分析do_bootm_linux函数的执行流程

传给kernel的命令行参数地址是宏定义 #define COMMAND_LINE （（char *）（param+0x100））

do_bootm_linux

{

》从参数去中获取环境变量bootargs的值

char *commandline = getenv("bootargs");

》从image head struct中获取kernel的入口地址

void (*theKernel)(void); // 函数指针

theKernel = (void (*) (void)) ntohl (hdr->ih_ep);

》从image head struct中获取kernel的指定存储参数的地址

ulong param;

param = ntohl (hdr->ih_load);

》把commandline拷贝到约定好的COMMAND_LINE地址

strcpy (COMMAND_LINE, commandline);



》最后，执行kernel的入口函数，把控制权交给kernel

/* now, finally, we pass control to the kernel  itself ... */

theKernel();

}
load内核然后解压：
ret = bootm_load_os(images.os, &load_end, 1);
{ 
/*
* Compression Types
*/
#define IH_COMP_NONE
0 /*  No
Compression Used */
#define IH_COMP_GZIP
1 /* gzip
Compression Used */
#define IH_COMP_BZIP2
2 /* bzip2 Compression Used
*/
#define IH_COMP_LZMA
3 /* lzma  Compression Used
*/
#define IH_COMP_LZO
4 /* lzo   Compression Used
*/

#define IH_MAGIC
0x27051956 /* Image Magic Number
*/
#define IH_NMLEN
32 /* Image Name Length
*/

static int bootm_load_os(image_info_t os, ulong *load_end, int boot_progress)
{
uint8_t comp = os.comp;
ulong load = os.load;
ulong blob_start = os.start;
ulong blob_end = os.end;
ulong image_start = os.image_start;
ulong image_len = os.image_len;
uint unc_len = CONFIG_SYS_BOOTM_LEN;
ulong image_end;

const char *type_name = genimg_get_type_name (os.type);
int boot_sp;

__asm__ __volatile__(
"mov    %0, sp\n"
:"=r"(boot_sp)
:
:"cc"
);

/* Check whether kernel zImage overwrite uboot,
* which will lead to kernel boot fail. */
image_end = load + image_len;
/* leave at most 32KByte for move image stack */
boot_sp -= BOOTM_STACK_GUARD;
if( !((load > _bss_end) || (image_end < boot_sp)) ) {
printf("\nkernel image will overwrite uboot! kernel boot fail!\n");
return BOOTM_ERR_RESET;
}

switch (comp) {
case IH_COMP_NONE:
if (load == blob_start || load == image_start) {
printf ("   XIP %s ... ", type_name);
} else {
printf ("   Loading %s ... ", type_name);
memmove_wd ((void *)load, (void *)image_start,
image_len, CHUNKSZ);
}
*load_end = load + image_len;
puts("OK\n");
break;
#ifdef CONFIG_GZIP
case IH_COMP_GZIP:
printf ("   Uncompressing %s ... ", type_name);
if (gunzip ((void *)load, unc_len,
(uchar *)image_start, &image_len) != 0) {
puts ("GUNZIP: uncompress, out-of-mem or overwrite error "
"- must RESET board to recover\n");
if (boot_progress)
show_boot_progress (-6);
return BOOTM_ERR_RESET;
}

*load_end = load + image_len;
break;
#endif /* CONFIG_GZIP */
#ifdef CONFIG_BZIP2
case IH_COMP_BZIP2:
printf ("   Uncompressing %s ... ", type_name);
/*
* If we've got less than 4 MB of malloc() space,
* use slower decompression algorithm which requires
* at most 2300 KB of memory.
*/
int i = BZ2_bzBuffToBuffDecompress ((char*)load,
&unc_len, (char *)image_start, image_len,
CONFIG_SYS_MALLOC_LEN < (4096 * 1024), 0);
if (i != BZ_OK) {
printf ("BUNZIP2: uncompress or overwrite error %d "
"- must RESET board to recover\n", i);
if (boot_progress)
show_boot_progress (-6);
return BOOTM_ERR_RESET;
}

*load_end = load + unc_len;
break;
#endif /* CONFIG_BZIP2 */
#ifdef CONFIG_LZMA
case IH_COMP_LZMA:
printf ("   Uncompressing %s ... ", type_name);

int ret = lzmaBuffToBuffDecompress(
(unsigned char *)load, &unc_len,
(unsigned char *)image_start, image_len);
if (ret != SZ_OK) {
printf ("LZMA: uncompress or overwrite error %d "
"- must RESET board to recover\n", ret);
show_boot_progress (-6);
return BOOTM_ERR_RESET;
}
*load_end = load + unc_len;
break;
#endif /* CONFIG_LZMA */
#ifdef CONFIG_LZO
case IH_COMP_LZO:
printf ("   Uncompressing %s ... ", type_name);

int ret = lzop_decompress((const unsigned char *)image_start,
 image_len, (unsigned char *)load,
 &unc_len);
if (ret != LZO_E_OK) {
printf ("LZO: uncompress or overwrite error %d "
     "- must RESET board to recover\n", ret);
if (boot_progress)
show_boot_progress (-6);
return BOOTM_ERR_RESET;
}

*load_end = load + unc_len;
break;
#endif /* CONFIG_LZO */
default:
printf ("Unimplemented compression type %d\n", comp);
return BOOTM_ERR_UNIMPLEMENTED;
}
puts ("OK\n");
debug ("   kernel loaded at 0x%08lx, end = 0x%08lx\n", load, *load_end);
if (boot_progress)
show_boot_progress (7);

if ((load < blob_end) && (*load_end > blob_start)) {
debug ("images.os.start = 0x%lX, images.os.end = 0x%lx\n", blob_start, blob_end);
debug ("images.os.load = 0x%lx, load_end = 0x%lx\n", load, *load_end);

return BOOTM_ERR_OVERLAP;
}

return 0;
}


}
}
9、uboot调用do_bootm_linux引导内核启动：
{


int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
{
bd_t *bd = gd->bd;
char *s;
int machid = bd->bi_arch_number;
void (*theKernel)(int zero, int arch, uint params);

#ifdef CONFIG_CMDLINE_TAG
char *commandline = getenv ("bootargs");
#endif

if ((flag != 0) && (flag != BOOTM_STATE_OS_GO))
return 1;

theKernel = (void (*)(int, int, uint))images->ep;

s = getenv ("machid");
if (s) {
machid = simple_strtoul (s, NULL, 16);
printf ("Using machid 0x%x from environment\n", machid);
}

show_boot_progress (15);

debug ("## Transferring control to Linux (at address %08lx) ...\n",
      (ulong) theKernel);

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
   defined (CONFIG_CMDLINE_TAG) || \
   defined (CONFIG_INITRD_TAG) || \
   defined (CONFIG_SERIAL_TAG) || \
   defined (CONFIG_REVISION_TAG)
setup_start_tag (bd);
#ifdef CONFIG_SERIAL_TAG
setup_serial_tag (¶ms);
#endif
#ifdef CONFIG_REVISION_TAG
setup_revision_tag (¶ms);
#endif
#ifdef CONFIG_SETUP_MEMORY_TAGS
setup_memory_tags (bd);
#endif
#ifdef CONFIG_CMDLINE_TAG
setup_commandline_tag (bd, commandline);
#endif
#ifdef CONFIG_INITRD_TAG
if (images->rd_start && images->rd_end)
setup_initrd_tag (bd, images->rd_start, images->rd_end);
#endif
#if defined(CONFIG_ETHMDIO_INF)
setup_eth_mdiointf_tag(bd, getenv("mdio_intf"));
#endif
#if defined(CONFIG_ETHADDR_TAG)
setup_ethaddr_tag(bd, getenv("ethaddr"));
#endif
#if defined(CONFIG_NANDID_TAG)
setup_nandid_tag(bd);
#endif
#if defined(CONFIG_SPIID_TAG)
setup_spiid_tag(bd);
#endif
setup_end_tag (bd);
#endif

/* we assume that the kernel is in place */
printf ("\nStarting kernel ...\n\n");

#ifdef CONFIG_USB_DEVICE
{
extern void udc_disconnect (void);
udc_disconnect ();
}
#endif

cleanup_before_linux ();


printf ("\n Init cpu done ................... alter by kj !!!\n\n");

theKernel (0, machid, bd->bi_boot_params);
/* does not return */

return 1;
}


}




/*===========================================hi3518c start kernel begin =============================================*/
进入内核：


一、创建si工程，按前面讲的
 二、内核：
  A、处理u-boot传入的参数，从arch/arm/kernel/head.s开始跟踪进来
   0、判断是否支持这个cpu
   1、判断是否支持这个单板，uboot启动内核时传人的机器ID（theKernel (0, bd->bi_arch_number, bd->bi_boot_params);）
     在arch/arm/kernel/head.s下： 
     bl __lookup_machine_type  @ r5=machinfo
     进入__lookup_machine_type由此引出了 
     在arch/arm/kernel/vmlinux.lds    //链接脚本
       __arch_info_begin = .;
        *(.arch.info.init)
       __arch_info_end = .;
     在内核里面搜索：
     book@book-desktop:/work/system/linux-2.6.22.6$ grep ".arch.info.init" * -nR

     在arch.h里面有：
     #define MACHINE_START(_type,_name)   \
     static const struct machine_desc __mach_desc_##_type \
      __used       \
      __attribute__((__section__(".arch.info.init"))) = { \
      .nr  = MACH_TYPE_##_type,  \
      .name  = _name,

     #define MACHINE_END    \
     };

     MACHINE_START(S3C2440, "SMDK2440")
      /* Maintainer: Ben Dooks <ben@fluff.org> */
      .phys_io = S3C2410_PA_UART,
      .io_pg_offst = (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
      .boot_params = S3C2410_SDRAM_PA + 0x100,

      .init_irq = s3c24xx_init_irq,
      .map_io  = smdk2440_map_io,
      .init_machine = smdk2440_machine_init,
      .timer  = &s3c24xx_timer,
     MACHINE_END

     展开宏

     static const struct machine_desc __mach_desc_S3C2440 \
      __used       \
      __attribute__((__section__(".arch.info.init"))) = { \
      .nr  = MACH_TYPE_S3C2440,  \
      .name  = "SMDK2440",
      /* Maintainer: Ben Dooks <ben@fluff.org> */
      .phys_io = S3C2410_PA_UART,
      .io_pg_offst = (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
      .boot_params = S3C2410_SDRAM_PA + 0x100,

      .init_irq = s3c24xx_init_irq,
      .map_io  = smdk2440_map_io,
      .init_machine = smdk2440_machine_init,
      .timer  = &s3c24xx_timer,
     }; 
   2、建立页表 
   3、使能MMU
   4、跳到我们内核的第一个c函数：start_kernel，这个函数会处理（theKernel (0, bd->bi_arch_number, bd->bi_boot_params);）里面的启动参数

  B、挂接根文件系统
  C、最终目的：运行应用程序



内核的启动流程：
 arch/arm/kernel/head.s

start_kernel
  setup_arch(&command_line);         //解析uboot传入的参数
  setup_command_line(command_line);  //解析uboot传入的参数
  parse_early_param
   do_early_param
    从__setup_start到__setup_end，调用early函数
  unknown_bootoption
   obsolete_checksetup
    从__setup_start到__setup_end，调用非early函数
  rest_init();
   kernel_init
    prepare_namespace();
     mount_root();//挂接根文件系统
    init_post();
     //执行应用程序

 三、Uboot传入的命令行参数的处理
 1、说明：对于下面两个函数只不过是把Uboot传入的命令行参数记录下来而已
 setup_arch(&command_line);         //解析uboot传入的参数
 setup_command_line(command_line);  //解析uboot传入的参数

我们在prepare_namespace();这个函数的时候它肯定得先确定你挂接的是哪一个文件系统：命令行参数root=/dev/mtdblock3指定
 在这个函数里面可以看到ROOT_DEV = name_to_dev_t(root_device_name);
 由于if (saved_root_name[0]) {
   root_device_name = saved_root_name;
   if (!strncmp(root_device_name, "mtd", 3)) {
    mount_block_root(root_device_name, root_mountflags);
    goto out;
   }
   ROOT_DEV = name_to_dev_t(root_device_name);
   if (strncmp(root_device_name, "/dev/", 5) == 0)
    root_device_name += 5;
  }
 在SI里面全局查找saved_root_name在哪里定义，可以搜索到一个函数和一个宏：
 static int __init root_dev_setup(char *line)
 {
  strlcpy(saved_root_name, line, sizeof(saved_root_name));
  return 1;
 }
 __setup("root=", root_dev_setup);
 由这个宏我们可以大胆的估计，在解析命令行的时候发现了root=***，然后以它找到root_dev_setup这个函数，然后调用这个
 函数，这个函数把root=***后面的***保存到变量saved_root_name里面去。

2、下面再分析__setup("root=", root_dev_setup);怎么执行：
 在SI里面全局查找__setup在哪里定义，技巧先查找，再在结果里面搜索define
 #define __setup(str, fn)     \
  __setup_param(str, fn, fn, 0)
 再搜索__setup_param
 #define __setup_param(str, unique_id, fn, early)   \
  static char __setup_str_##unique_id[] __initdata = str; \
  static struct obs_kernel_param __setup_##unique_id \
   __attribute_used__    \
   __attribute__((__section__(".init.setup"))) \
   __attribute__((aligned((sizeof(long))))) \
   = { __setup_str_##unique_id, fn, early }

 我们把__setup("root=", root_dev_setup)展开，可以分析这个就是定义了如同上面的结构体，这个结构体里面有3个重要的成员
 ①名字"root=" ②函数root_dev_setup③early
 而且__setup("root=", root_dev_setup)定义的结构体有一个特殊的属性：__attribute__((__section__(".init.setup")))，显然
 ".init.setup"在链接脚本里面
 __setup_start = .;
    *(.init.setup)
   __setup_end = .;

搜索一下__setup_start就可以知道命令行是怎么调用的了，在两个函数里面被调用：
 ①函数obsolete_checksetup
 ②函数do_early_param，这个应该就是对应上面的__setup_str_##unique_id, fn, early 里面的early了。
 再找一下do_early_param是谁调用：
 parse_early_param
 由于我们传入的参数early是0.很显然我们用的是①函数obsolete_checksetup
 如下调用过程：
 parse_early_param
   do_early_param
    从__setup_start到__setup_end，调用early函数

①函数obsolete_checksetup它被unknown_bootoption调用，搜索这个函数得：它被start_kernel函数调用。
  parse_args("Booting kernel", static_command_line, __start___param,
      __stop___param - __start___param,
      &unknown_bootoption);
 如下调用过程：
 unknown_bootoption
   obsolete_checksetup
    从__setup_start到__setup_end，调用非early函数

四、分区表
 如果启动参数不是下面这个，则要设置启动参数：
 set bootargs noinitrd root=/dev/mtdblock3 rootfstype=jffs2 init=/linuxrc console=ttySAC0

启动linux后，可以看到启动时会显示分区表：
 Creating 4 MTD partitions on "NAND 256MiB 3,3V 8-bit":
 0x00000000-0x00040000 : "bootloader"
 0x00040000-0x00060000 : "params"
 0x00060000-0x00260000 : "kernel"
 0x00260000-0x10000000 : "root"

book@book-desktop:/work/system/linux-2.6.22.6$ grep ".arch.info.init" * -nR
 分区应该在arch/arm/plat-s3c24xx/common-smdk.c里面

/* NAND parititon from 2.4.18-swl5 */

static struct mtd_partition smdk_default_nand_part[] = {
  [0] = {
         .name   = "bootloader",
         .size   = 0x00040000,
   .offset = 0,
  },
  [1] = {
         .name   = "params",
         .offset = MTDPART_OFS_APPEND,
         .size   = 0x00020000,
  },
  [2] = {
         .name   = "kernel",
         .offset = MTDPART_OFS_APPEND,
         .size   = 0x00200000,
  },
  [3] = {
         .name   = "root",
         .offset = MTDPART_OFS_APPEND,
         .size   = MTDPART_SIZ_FULL,
  }
 };




内核启动流程：
{
内核的启动流程：
arch/arm/kernel/head.s

start_kernel
setup_arch(&command_line);         //解析uboot传入的参数
setup_command_line(command_line);  //解析uboot传入的参数
parse_early_param
do_early_param
从__setup_start到__setup_end，调用early函数
unknown_bootoption
obsolete_checksetup
从__setup_start到__setup_end，调用非early函数
rest_init();
kernel_init
prepare_namespace();
mount_root();//挂接根文件系统
init_post();
//执行应用程序


}
1、./arch/arm/boot/compressed/misc.c:193: putstr(" done, booting the kernel.\n");


分析开始：


1、
head.S中得_start 标号开始分析。(有些指令不影响初始化，暂时略去不分析) 
代码位置在/arch/arm/boot/compressed/head.S中
{
/*
*  linux/arch/arm/boot/compressed/head.S
*
*  Copyright (C) 1996-2002 Russell King
*  Copyright (C) 2004 Hyok S. Choi (MPU support)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*/
#include <linux/linkage.h>

/*
* Debugging stuff
*
* Note that these macros must not contain any code which is not
* 100% relocatable.  Any attempt to do so will result in a crash.
* Please select one of the following when turning on debugging.
*/
#ifdef DEBUG

#if defined(CONFIG_DEBUG_ICEDCC)

#if defined(CONFIG_CPU_V6) || defined(CONFIG_CPU_V6K) || defined(CONFIG_CPU_V7)
.macro  loadsp, rb, tmp
.endm
.macro  writeb, ch, rb
mcr p14, 0, \ch, c0, c5, 0
.endm
#elif defined(CONFIG_CPU_XSCALE)
.macro  loadsp, rb, tmp
.endm
.macro  writeb, ch, rb
mcr p14, 0, \ch, c8, c0, 0
.endm
#else
.macro  loadsp, rb, tmp
.endm
.macro  writeb, ch, rb
mcr p14, 0, \ch, c1, c0, 0
.endm
#endif

#else

#include <mach/debug-macro.S>

.macro  writeb, ch, rb
senduart \ch, \rb
.endm

#if defined(CONFIG_ARCH_SA1100)
.macro  loadsp, rb, tmp
mov \rb, #0x80000000
@ physical base address
#ifdef CONFIG_DEBUG_LL_SER3
add \rb, \rb, #0x00050000
@ Ser3
#else
add \rb, \rb, #0x00010000
@ Ser1
#endif
.endm
#elif defined(CONFIG_ARCH_S3C2410)
.macro loadsp, rb, tmp
mov \rb, #0x50000000
add \rb, \rb, #0x4000 * CONFIG_S3C_LOWLEVEL_UART_PORT
.endm
#else
.macro  loadsp, rb, tmp
addruart \rb, \tmp
.endm
#endif
#endif
#endif

.macro  kputc,val
mov r0, \val
bl putc
.endm

.macro  kphex,val,len
mov r0, \val
mov r1, #\len
bl phex
.endm

.macro  debug_reloc_start
#ifdef DEBUG
kputc #'\n'
kphex r6, 8
/* processor id */
kputc #':'
kphex r7, 8
/* architecture id */
#ifdef CONFIG_CPU_CP15
kputc #':'
mrc p15, 0, r0, c1, c0
kphex r0, 8
/* control reg */
#endif
kputc #'\n'
kphex r5, 8
/* decompressed kernel start */
kputc #'-'
kphex r9, 8
/* decompressed kernel end  */
kputc #'>'
kphex r4, 8
/* kernel execution address */
kputc #'\n'
#endif
.endm

.macro  debug_reloc_end
#ifdef DEBUG
kphex r5, 8
/* end of kernel */
kputc #'\n'
mov r0, r4
bl memdump
/* dump 256 bytes at start of kernel */
#endif
.endm

.section ".start", #alloc, #execinstr
/*
* sort out different calling conventions
*/
.align
.arm @ Always enter in ARM state
start:
.type start,#function /*uboot跳转到内核后执行的第一条代码*/
.rept 7 /*重复定义7次下面的指令，也就是空出中断向量表的位置*/
mov r0, r0 /*就是nop指令*/
.endr
  ARM(  mov r0, r0  )
  ARM(  b 1f ) @ 跳转到后面的标号1处
THUMB(  adr r12, BSYM(1f)
)
THUMB(  bx r12  )

.word 0x016f2818
@ Magic numbers to help the loader @ 辅助引导程序的幻数，用来判断镜像是否是zImage
.word start
@ absolute load/run zImage address 加载运行zImage的绝对地址，start表示赋的初值
.word _edata
@ zImage end address zImage结尾地址，_edata是在vmlinux.lds.S中定义的，表示init,text,data三个段的结束位置
THUMB(  .thumb )
1: mov
r7, r1 @ save architecture ID  /*标号1处*/ ID 保存体系结构ID 用r1保存
mov r8, r2
@ save atags pointer 保存r2寄存器 参数列表，r0始终为0

#ifndef __ARM_ARCH_2__
/*
* Booting from Angel - need to enter SVC mode and disable
* FIQs/IRQs (numeric definitions from angel arm.h source).
* We only do this if we were in user mode on entry.
*/
mrs r2, cpsr
@ get current mode 得到当前模式
tst r2, #3
@ not user? 实际上是相与,判断是否处于用户模式
bne not_angel   @ 如果不是处于用户模式，就跳转到not_angel标号处 不相等跳转 /*如果是普通用户模式，则通过软中断进入超级用户权限模式*/
mov r0, #0x17
@ angel_SWIreason_EnterSVC ，向SWI中传递参数
ARM( swi
0x123456 )
@ angel_SWI_ARM angel_SWI_ARM 这个是让用户空间进入SVC空间 SWI软中断
THUMB(  svc 0xab  ) @ angel_SWI_THUMB 
not_angel:
mrs r2, cpsr
@ turn off interrupts to  /*表示非用户模式，可以直接关闭中断*/
orr r2, r2, #0xc0
@ prevent angel from running 关闭中断
msr cpsr_c, r2 @ 把r2的值从新写回到cpsr中
#else
teqp pc, #0x0c000003
@ turn off interrupts
#endif

/*
* Note that some cache flushing and other stuff may
* be needed here - is there an Angel SWI call for this?
*/

/*
* some architecture specific code can be inserted
* by the linker here, but it should preserve r7, r8, and r9.
*/

.text

#ifdef CONFIG_AUTO_ZRELADDR
@ determine final kernel image address
mov r4, pc
and r4, r4, #0xf8000000
add r4, r4, #TEXT_OFFSET
#else
ldr r4, =zreladdr
#endif

bl cache_on

restart:  adr r0, LC0 /*把地址表的起始地址放入r0中*/
ldmia r0, {r1, r2, r3, r6, r10, r11, r12} /*加载地址表中的所有地址到相应的寄存器*/@r0是运行时地址，而r1则是链接时地址，而它们两都是表示LC0表的起始位置，这样他们两的差则是运行和链接的偏移量，纠正了这个偏移量才可以运行与”地址相关的代码“
ldr sp, [r0, #28]

/*
* We might be running at a different address.  We need
* to fix up various pointers.
*/
sub r0, r0, r1
@ calculate the delta offset 计算偏移量，并放入r0中
add r6, r6, r0
@ _edata
add r10, r10, r0
@ inflated kernel size location

/*
* The kernel build system appends the size of the
* decompressed kernel at the end of the compressed data
* in little-endian form.
*/
ldrb r9, [r10, #0]
ldrb lr, [r10, #1]
orr r9, r9, lr, lsl #8
ldrb lr, [r10, #2]
ldrb r10, [r10, #3]
orr r9, r9, lr, lsl #16
orr r9, r9, r10, lsl #24

#ifndef CONFIG_ZBOOT_ROM
/* malloc space is above the relocated stack (64k max) */
add sp, sp, r0
add r10, sp, #0x10000
#else
/*
* With ZBOOT_ROM the bss/stack is non relocatable,
* but someone could still run this code from RAM,
* in which case our reference is _edata.
*/
mov r10, r6
#endif

/*
* Check to see if we will overwrite ourselves.
*   r4  = final kernel address
*   r9  = size of decompressed image
*   r10 = end of this image, including  bss/stack/malloc space if non XIP
* We basically want:
*   r4 - 16k page directory >= r10 -> OK
*   r4 + image length <= current position (pc) -> OK
*/
@vmlinux 的起始地址大于 zImage 运行时所需的最大地址（ r2 ） , 那么直接将 zImage 解压到 vmlinux 的目标地址
add r10, r10, #16384
cmp r4, r10
bhs wont_overwrite  /*如果r4大于或等于r10的话*/
add r10, r4, r9
  ARM(  cmp r10, pc  )
THUMB(  mov lr, pc  )
THUMB(  cmp r10, lr  )
bls wont_overwrite

/*
* Relocate ourselves past the end of the decompressed kernel.
*   r6  = _edata
*   r10 = end of the decompressed kernel
* Because we always copy ahead, we need to do it from the end and go
* backward in case the source and destination overlap.
*/
/*
* Bump to the next 256-byte boundary with the size of
* the relocation code added. This avoids overwriting
* ourself when the offset is small.
*/
add r10, r10, #((reloc_code_end - restart + 256) & ~255)
bic r10, r10, #255

/* Get start of code we want to copy and align it down. */
adr r5, restart
bic r5, r5, #31

sub r9, r6, r5
@ size to copy
add r9, r9, #31
@ rounded up to a multiple
bic r9, r9, #31
@ ... of 32 bytes
add r6, r9, r5
add r9, r9, r10

1: ldmdb
r6!, {r0 - r3, r10 - r12, lr}
cmp r6, r5
stmdb r9!, {r0 - r3, r10 - r12, lr}
bhi 1b

/* Preserve offset to relocated code. */
sub r6, r9, r6

#ifndef CONFIG_ZBOOT_ROM
/* cache_clean_flush may use the stack, so relocate it */
add sp, sp, r6
#endif

bl cache_clean_flush

adr r0, BSYM(restart)
add r0, r0, r6
mov pc, r0

wont_overwrite:
/*
* If delta is zero, we are running at the address we were linked at.
*   r0  = delta
*   r2  = BSS start
*   r3  = BSS end
*   r4  = kernel execution address
*   r7  = architecture ID
*   r8  = atags pointer
*   r11 = GOT start
*   r12 = GOT end
*   sp  = stack pointer
*/
teq r0, #0
beq not_relocated  @ 如果为0，则不用重定位了，直接跳转到标号not_relocated处执行
add r11, r11, r0
add r12, r12, r0

#ifndef CONFIG_ZBOOT_ROM
/*
* If we're running fully PIC === CONFIG_ZBOOT_ROM = n,
* we need to fix up pointers into the BSS region.
* Note that the stack pointer has already been fixed up.
*/
add r2, r2, r0
add r3, r3, r0

/*
* Relocate all entries in the GOT table.
*/
1: ldr
r1, [r11, #0] @ relocate entries in the GOT
add r1, r1, r0
@ table.  This fixes up the
str r1, [r11], #4
@ C references.
cmp r11, r12
blo 1b
#else

/*
* Relocate entries in the GOT table.  We only relocate
* the entries that are outside the (relocated) BSS region.
*/
1: ldr
r1, [r11, #0] @ relocate entries in the GOT
cmp r1, r2
@ entry < bss_start ||
cmphs r3, r1
@ _end < entry
addlo r1, r1, r0
@ table.  This fixes up the
str r1, [r11], #4
@ C references.
cmp r11, r12
blo 1b
#endif

not_relocated:
mov r0, #0
1: str
r0, [r2], #4 @ clear bss
str r0, [r2], #4
str r0, [r2], #4
str r0, [r2], #4
cmp r2, r3
blo 1b

/*
* The C runtime environment should now be setup sufficiently.
* Set up some pointers, and start decompressing.
*   r4  = kernel execution address
*   r7  = architecture ID
*   r8  = atags pointer
*/
@ 这里的 r1,r2 之间的空间为解压缩内核程序所使用，也是传递给 decompress_kernel 的第二和第三的参数
mov r0, r4
mov r1, sp
@ malloc space above stack
add r2, sp, #0x10000
@ 64k max
mov r3, r7
bl decompress_kernel
bl cache_clean_flush
bl cache_off
mov r0, #0
@ must be zero
mov r1, r7
@ restore architecture number
mov r2, r8
@ restore atags pointer
mov pc, r4
@ call kernel   //@ 这里就是最终我们从zImage跳转到Image的伟大一跳了，跳之前准备好r0,r1,r2

/*读入地址表。因为我们的代码可以在任何地址执行，也就是位置无关代码（PIC），所以我们需要加上一个偏移量。下面有每一个列表项的具体意义。

LC0是表的首项，它本身就是在此head.s中定义的

.type LC0, #object

LC0: .word LC0 @ r1 LC0表的起始位置

.word __bss_start @ r2 bss段的起始地址在vmlinux.lds.S中定义

.word _end @ r3 zImage（bss）连接的结束地址在vmlinux.lds.S中定义

.word zreladdr @ r4 zImage的连接地址，我们在arch/arm/mach-s3c2410/makefile.boot中定义的

.word _start @ r5 zImage的基地址，bootp/init.S中的_start函数，主要起传递参数作用

.word _got_start @ r6 GOT（全局偏移表）起始地址，_got_start是在compressed/vmlinux.lds.in中定义的

.word _got_end @ ip GOT结束地址

.word user_stack+4096 @ sp 用户栈底 user_stack是紧跟在bss段的后面的，在compressed/vmlinux.lds.in中定义的

@ 在本head.S的末尾定义了zImag的临时栈空间，在这里分配了4K的空间用来做堆栈。

.section ".stack", "w"

user_stack: .space 4096

GOT表的初值是连接器指定的，当时程序并不知道代码在哪个地址执行。如果当前运行的地址已经和表上的地址不一样，还要修正GOT表。
*/


.align  2
.type LC0, #object
LC0: .word
LC0 @ r1
.word __bss_start
@ r2
.word _end
@ r3
.word _edata
@ r6
.word input_data_end - 4
@ r10 (inflated size location)
.word _got_start
@ r11
.word _got_end
@ ip
.word .L_user_stack_end
@ sp
.size LC0, . - LC0

#ifdef CONFIG_ARCH_RPC
.globl  params
params:  ldr r0, =0x10000100
@ params_phys for RPC
mov pc, lr
.ltorg
.align
#endif

/*
* Turn on the cache.  We need to setup some page tables so that we
* can have both the I and D caches on.
*
* We place the page tables 16k down from the kernel execution address,
* and we hope that nothing else is using it.  If we're using it, we
* will go pop!
*
* On entry,
*  r4 = kernel execution address
*  r7 = architecture number
*  r8 = atags pointer
* On exit,
*  r0, r1, r2, r3, r9, r10, r12 corrupted
* This routine must preserve:
*  r4, r7, r8
*/
.align  5
cache_on:  mov r3, #8  @ cache_on function
b call_cache_fn

/*
* Initialize the highest priority protection region, PR7
* to cover all 32bit address and cacheable and bufferable.
*/
__armv4_mpu_cache_on:
mov r0, #0x3f
@ 4G, the whole
mcr p15, 0, r0, c6, c7, 0
@ PR7 Area Setting
mcr p15, 0, r0, c6, c7, 1

mov r0, #0x80
@ PR7
mcr p15, 0, r0, c2, c0, 0
@ D-cache on
mcr p15, 0, r0, c2, c0, 1
@ I-cache on
mcr p15, 0, r0, c3, c0, 0
@ write-buffer on

mov r0, #0xc000
mcr p15, 0, r0, c5, c0, 1
@ I-access permission
mcr p15, 0, r0, c5, c0, 0
@ D-access permission

mov r0, #0
mcr p15, 0, r0, c7, c10, 4
@ drain write buffer
mcr p15, 0, r0, c7, c5, 0
@ flush(inval) I-Cache
mcr p15, 0, r0, c7, c6, 0
@ flush(inval) D-Cache
mrc p15, 0, r0, c1, c0, 0
@ read control reg
@ ...I .... ..D. WC.M
orr r0, r0, #0x002d
@ .... .... ..1. 11.1
orr r0, r0, #0x1000
@ ...1 .... .... ....

mcr p15, 0, r0, c1, c0, 0
@ write control reg

mov r0, #0
mcr p15, 0, r0, c7, c5, 0
@ flush(inval) I-Cache
mcr p15, 0, r0, c7, c6, 0
@ flush(inval) D-Cache
mov pc, lr

__armv3_mpu_cache_on:
mov r0, #0x3f
@ 4G, the whole
mcr p15, 0, r0, c6, c7, 0
@ PR7 Area Setting

mov r0, #0x80
@ PR7
mcr p15, 0, r0, c2, c0, 0
@ cache on
mcr p15, 0, r0, c3, c0, 0
@ write-buffer on

mov r0, #0xc000
mcr p15, 0, r0, c5, c0, 0
@ access permission

mov r0, #0
mcr p15, 0, r0, c7, c0, 0
@ invalidate whole cache v3
/*
* ?? ARMv3 MMU does not allow reading the control register,
* does this really work on ARMv3 MPU?
*/
mrc p15, 0, r0, c1, c0, 0
@ read control reg
@ .... .... .... WC.M
orr r0, r0, #0x000d
@ .... .... .... 11.1
/* ?? this overwrites the value constructed above? */
mov r0, #0
mcr p15, 0, r0, c1, c0, 0
@ write control reg

/* ?? invalidate for the second time? */
mcr p15, 0, r0, c7, c0, 0
@ invalidate whole cache v3
mov pc, lr

__setup_mmu:  sub r3, r4, #16384
@ Page directory size
bic r3, r3, #0xff
@ Align the pointer
bic r3, r3, #0x3f00
/*
* Initialise the page tables, turning on the cacheable and bufferable
* bits for the RAM area only.
*/
mov r0, r3
mov r9, r0, lsr #18
mov r9, r9, lsl #18
@ start of RAM
add r10, r9, #0x10000000
@ a reasonable RAM size
mov r1, #0x12
orr r1, r1, #3 << 10
add r2, r3, #16384
1: cmp
r1, r9 @ if virt > start of RAM
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
orrhs r1, r1, #0x08
@ set cacheable
#else
orrhs r1, r1, #0x0c
@ set cacheable, bufferable
#endif
cmp r1, r10
@ if virt > end of RAM
bichs r1, r1, #0x0c
@ clear cacheable, bufferable
str r1, [r0], #4
@ 1:1 mapping
add r1, r1, #1048576
teq r0, r2
bne 1b
/*
* If ever we are running from Flash, then we surely want the cache
* to be enabled also for our execution instance...  We map 2MB of it
* so there is no map overlap problem for up to 1 MB compressed kernel.
* If the execution is in RAM then we would only be duplicating the above.
*/
mov r1, #0x1e
orr r1, r1, #3 << 10
mov r2, pc
mov r2, r2, lsr #20
orr r1, r1, r2, lsl #20
add r0, r3, r2, lsl #2
str r1, [r0], #4
add r1, r1, #1048576
str r1, [r0]
mov pc, lr
ENDPROC(__setup_mmu)

__arm926ejs_mmu_cache_on:
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
mov r0, #4
@ put dcache in WT mode
mcr p15, 7, r0, c15, c0, 0
#endif

__armv4_mmu_cache_on:
mov r12, lr
#ifdef CONFIG_MMU
bl __setup_mmu
mov r0, #0
mcr p15, 0, r0, c7, c10, 4
@ drain write buffer
mcr p15, 0, r0, c8, c7, 0
@ flush I,D TLBs
mrc p15, 0, r0, c1, c0, 0
@ read control reg
orr r0, r0, #0x5000
@ I-cache enable, RR cache replacement
orr r0, r0, #0x0030
#ifdef CONFIG_CPU_ENDIAN_BE8
orr r0, r0, #1 << 25
@ big-endian page tables
#endif
bl __common_mmu_cache_on
mov r0, #0
mcr p15, 0, r0, c8, c7, 0
@ flush I,D TLBs
#endif
mov pc, r12

__armv7_mmu_cache_on:
mov r12, lr
#ifdef CONFIG_MMU
mrc p15, 0, r11, c0, c1, 4
@ read ID_MMFR0
tst r11, #0xf
@ VMSA
blne __setup_mmu
mov r0, #0
mcr p15, 0, r0, c7, c10, 4
@ drain write buffer
tst r11, #0xf
@ VMSA
mcrne p15, 0, r0, c8, c7, 0
@ flush I,D TLBs
#endif
mrc p15, 0, r0, c1, c0, 0
@ read control reg
orr r0, r0, #0x5000
@ I-cache enable, RR cache replacement
orr r0, r0, #0x003c
@ write buffer
#ifdef CONFIG_MMU
#ifdef CONFIG_CPU_ENDIAN_BE8
orr r0, r0, #1 << 25
@ big-endian page tables
#endif
orrne r0, r0, #1
@ MMU enabled
movne r1, #-1
mcrne p15, 0, r3, c2, c0, 0
@ load page table pointer
mcrne p15, 0, r1, c3, c0, 0
@ load domain access control
#endif
mcr p15, 0, r0, c1, c0, 0
@ load control register
mrc p15, 0, r0, c1, c0, 0
@ and read it back
mov r0, #0
mcr p15, 0, r0, c7, c5, 4
@ ISB
mov pc, r12

__fa526_cache_on:
mov r12, lr
bl __setup_mmu
mov r0, #0
mcr p15, 0, r0, c7, c7, 0
@ Invalidate whole cache
mcr p15, 0, r0, c7, c10, 4
@ drain write buffer
mcr p15, 0, r0, c8, c7, 0
@ flush UTLB
mrc p15, 0, r0, c1, c0, 0
@ read control reg
orr r0, r0, #0x1000
@ I-cache enable
bl __common_mmu_cache_on
mov r0, #0
mcr p15, 0, r0, c8, c7, 0
@ flush UTLB
mov pc, r12

__arm6_mmu_cache_on:
mov r12, lr
bl __setup_mmu
mov r0, #0
mcr p15, 0, r0, c7, c0, 0
@ invalidate whole cache v3
mcr p15, 0, r0, c5, c0, 0
@ invalidate whole TLB v3
mov r0, #0x30
bl __common_mmu_cache_on
mov r0, #0
mcr p15, 0, r0, c5, c0, 0
@ invalidate whole TLB v3
mov pc, r12

__common_mmu_cache_on:
#ifndef CONFIG_THUMB2_KERNEL
#ifndef DEBUG
orr r0, r0, #0x000d
@ Write buffer, mmu
#endif
mov r1, #-1
mcr p15, 0, r3, c2, c0, 0
@ load page table pointer
mcr p15, 0, r1, c3, c0, 0
@ load domain access control
b 1f
.align  5 @ cache line aligned
1: mcr
p15, 0, r0, c1, c0, 0 @ load control register
mrc p15, 0, r0, c1, c0, 0
@ and read it back to
sub pc, lr, r0, lsr #32
@ properly flush pipeline
#endif

#define PROC_ENTRY_SIZE (4*5)

/*
* Here follow the relocatable cache support functions for the
* various processors.  This is a generic hook for locating an
* entry and jumping to an instruction at the specified offset
* from the start of the block.  Please note this is all position
* independent code.
*
*  r1  = corrupted
*  r2  = corrupted
*  r3  = block offset
*  r9  = corrupted
*  r12 = corrupted
*/

call_cache_fn:
adr r12, proc_types
#ifdef CONFIG_CPU_CP15
mrc p15, 0, r9, c0, c0
@ get processor ID
#else
ldr r9, =CONFIG_PROCESSOR_ID
#endif
1: ldr
r1, [r12, #0] @ get value
ldr r2, [r12, #4]
@ get mask
eor r1, r1, r9
@ (real ^ match)
tst r1, r2
@       & mask
ARM( addeq
pc, r12, r3 ) @ call cache function
THUMB(  addeq r12, r3  )
THUMB(  moveq pc, r12  ) @ call cache function
add r12, r12, #PROC_ENTRY_SIZE
b 1b

/*
* Table for cache operations.  This is basically:
*   - CPU ID match
*   - CPU ID mask
*   - 'cache on' method instruction
*   - 'cache off' method instruction
*   - 'cache flush' method instruction
*
* We match an entry using: ((real_id ^ match) & mask) == 0
*
* Writethrough caches generally only need 'on' and 'off'
* methods.  Writeback caches _must_ have the flush method
* defined.
*/
.align  2
.type proc_types,#object
proc_types:
.word 0x41560600
@ ARM6/610
.word 0xffffffe0
W(b) __arm6_mmu_cache_off
@ works, but slow
W(b) __arm6_mmu_cache_off
mov pc, lr
THUMB(  nop )
@ b
__arm6_mmu_cache_on @ untested
@ b
__arm6_mmu_cache_off
@ b
__armv3_mmu_cache_flush

#if !defined(CONFIG_CPU_V7)
/* This collides with some V7 IDs, preventing correct detection */
.word 0x00000000
@ old ARM ID
.word 0x0000f000
mov pc, lr
THUMB(  nop )
mov pc, lr
THUMB(  nop )
mov pc, lr
THUMB(  nop )
#endif

.word 0x41007000
@ ARM7/710
.word 0xfff8fe00
W(b) __arm7_mmu_cache_off
W(b) __arm7_mmu_cache_off
mov pc, lr
THUMB(  nop )

.word 0x41807200
@ ARM720T (writethrough)
.word 0xffffff00
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
mov pc, lr
THUMB(  nop )

.word 0x41007400
@ ARM74x
.word 0xff00ff00
W(b) __armv3_mpu_cache_on
W(b) __armv3_mpu_cache_off
W(b) __armv3_mpu_cache_flush

.word 0x41009400
@ ARM94x
.word 0xff00ff00
W(b) __armv4_mpu_cache_on
W(b) __armv4_mpu_cache_off
W(b) __armv4_mpu_cache_flush

.word 0x41069260
@ ARM926EJ-S (v5TEJ)
.word 0xff0ffff0
W(b) __arm926ejs_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv5tej_mmu_cache_flush

.word 0x00007000
@ ARM7 IDs
.word 0x0000f000
mov pc, lr
THUMB(  nop )
mov pc, lr
THUMB(  nop )
mov pc, lr
THUMB(  nop )

@ Everything from here on will be the new ID system.

.word 0x4401a100
@ sa110 / sa1100
.word 0xffffffe0
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv4_mmu_cache_flush

.word 0x6901b110
@ sa1110
.word 0xfffffff0
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv4_mmu_cache_flush

.word 0x56056900
.word 0xffffff00
@ PXA9xx
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv4_mmu_cache_flush

.word 0x56158000
@ PXA168
.word 0xfffff000
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv5tej_mmu_cache_flush

.word 0x56050000
@ Feroceon
.word 0xff0f0000
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv5tej_mmu_cache_flush

#ifdef CONFIG_CPU_FEROCEON_OLD_ID
/* this conflicts with the standard ARMv5TE entry */
.long 0x41009260
@ Old Feroceon
.long 0xff00fff0
b __armv4_mmu_cache_on
b __armv4_mmu_cache_off
b __armv5tej_mmu_cache_flush
#endif

.word 0x66015261
@ FA526
.word 0xff01fff1
W(b) __fa526_cache_on
W(b) __armv4_mmu_cache_off
W(b) __fa526_cache_flush

@ These match on the architecture ID

.word 0x00020000
@ ARMv4T
.word 0x000f0000
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv4_mmu_cache_flush

.word 0x00050000
@ ARMv5TE
.word 0x000f0000
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv4_mmu_cache_flush

.word 0x00060000
@ ARMv5TEJ
.word 0x000f0000
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv5tej_mmu_cache_flush

.word 0x0007b000
@ ARMv6
.word 0x000ff000
W(b) __armv4_mmu_cache_on
W(b) __armv4_mmu_cache_off
W(b) __armv6_mmu_cache_flush

.word 0x000f0000
@ new CPU Id
.word 0x000f0000
W(b) __armv7_mmu_cache_on
W(b) __armv7_mmu_cache_off
W(b) __armv7_mmu_cache_flush

.word 0
@ unrecognised type
.word 0
mov pc, lr
THUMB(  nop )
mov pc, lr
THUMB(  nop )
mov pc, lr
THUMB(  nop )

.size proc_types, . - proc_types

/*
* If you get a "non-constant expression in ".if" statement"
* error from the assembler on this line, check that you have
* not accidentally written a "b" instruction where you should
* have written W(b).
*/
.if (. - proc_types) % PROC_ENTRY_SIZE != 0
.error "The size of one or more proc_types entries is wrong."
.endif

/*
* Turn off the Cache and MMU.  ARMv3 does not support
* reading the control register, but ARMv4 does.
*
* On exit,
*  r0, r1, r2, r3, r9, r12 corrupted
* This routine must preserve:
*  r4, r7, r8
*/
.align  5
cache_off:  mov r3, #12  @ cache_off function
b call_cache_fn

__armv4_mpu_cache_off:
mrc p15, 0, r0, c1, c0
bic r0, r0, #0x000d
mcr p15, 0, r0, c1, c0
@ turn MPU and cache off
mov r0, #0
mcr p15, 0, r0, c7, c10, 4
@ drain write buffer
mcr p15, 0, r0, c7, c6, 0
@ flush D-Cache
mcr p15, 0, r0, c7, c5, 0
@ flush I-Cache
mov pc, lr

__armv3_mpu_cache_off:
mrc p15, 0, r0, c1, c0
bic r0, r0, #0x000d
mcr p15, 0, r0, c1, c0, 0
@ turn MPU and cache off
mov r0, #0
mcr p15, 0, r0, c7, c0, 0
@ invalidate whole cache v3
mov pc, lr

__armv4_mmu_cache_off:
#ifdef CONFIG_MMU
mrc p15, 0, r0, c1, c0
bic r0, r0, #0x000d
mcr p15, 0, r0, c1, c0
@ turn MMU and cache off
mov r0, #0
mcr p15, 0, r0, c7, c7
@ invalidate whole cache v4
mcr p15, 0, r0, c8, c7
@ invalidate whole TLB v4
#endif
mov pc, lr

__armv7_mmu_cache_off:
mrc p15, 0, r0, c1, c0
#ifdef CONFIG_MMU
bic r0, r0, #0x000d
#else
bic r0, r0, #0x000c
#endif
mcr p15, 0, r0, c1, c0
@ turn MMU and cache off
mov r12, lr
bl __armv7_mmu_cache_flush
mov r0, #0
#ifdef CONFIG_MMU
mcr p15, 0, r0, c8, c7, 0
@ invalidate whole TLB
#endif
mcr p15, 0, r0, c7, c5, 6
@ invalidate BTC
mcr p15, 0, r0, c7, c10, 4
@ DSB
mcr p15, 0, r0, c7, c5, 4
@ ISB
mov pc, r12

__arm6_mmu_cache_off:
mov r0, #0x00000030
@ ARM6 control reg.
b __armv3_mmu_cache_off

__arm7_mmu_cache_off:
mov r0, #0x00000070
@ ARM7 control reg.
b __armv3_mmu_cache_off

__armv3_mmu_cache_off:
mcr p15, 0, r0, c1, c0, 0
@ turn MMU and cache off
mov r0, #0
mcr p15, 0, r0, c7, c0, 0
@ invalidate whole cache v3
mcr p15, 0, r0, c5, c0, 0
@ invalidate whole TLB v3
mov pc, lr

/*
* Clean and flush the cache to maintain consistency.
*
* On exit,
*  r1, r2, r3, r9, r10, r11, r12 corrupted
* This routine must preserve:
*  r4, r6, r7, r8
*/
.align  5
cache_clean_flush:
mov r3, #16
b call_cache_fn

__armv4_mpu_cache_flush:
mov r2, #1
mov r3, #0
mcr p15, 0, ip, c7, c6, 0
@ invalidate D cache
mov r1, #7 << 5
@ 8 segments
1: orr
r3, r1, #63 << 26 @ 64 entries
2: mcr
p15, 0, r3, c7, c14, 2 @ clean & invalidate D index
subs r3, r3, #1 << 26
bcs 2b
@ entries 63 to 0
subs r1, r1, #1 << 5
bcs 1b
@ segments 7 to 0

teq r2, #0
mcrne p15, 0, ip, c7, c5, 0
@ invalidate I cache
mcr p15, 0, ip, c7, c10, 4
@ drain WB
mov pc, lr

__fa526_cache_flush:
mov r1, #0
mcr p15, 0, r1, c7, c14, 0
@ clean and invalidate D cache
mcr p15, 0, r1, c7, c5, 0
@ flush I cache
mcr p15, 0, r1, c7, c10, 4
@ drain WB
mov pc, lr

__armv6_mmu_cache_flush:
mov r1, #0
mcr p15, 0, r1, c7, c14, 0
@ clean+invalidate D
mcr p15, 0, r1, c7, c5, 0
@ invalidate I+BTB
mcr p15, 0, r1, c7, c15, 0
@ clean+invalidate unified
mcr p15, 0, r1, c7, c10, 4
@ drain WB
mov pc, lr

__armv7_mmu_cache_flush:
mrc p15, 0, r10, c0, c1, 5
@ read ID_MMFR1
tst r10, #0xf << 16
@ hierarchical cache (ARMv7)
mov r10, #0
beq hierarchical
mcr p15, 0, r10, c7, c14, 0
@ clean+invalidate D
b iflush
hierarchical:
mcr p15, 0, r10, c7, c10, 5
@ DMB
stmfd sp!, {r0-r7, r9-r11}
mrc p15, 1, r0, c0, c0, 1
@ read clidr
ands r3, r0, #0x7000000
@ extract loc from clidr
mov r3, r3, lsr #23
@ left align loc bit field
beq finished
@ if loc is 0, then no need to clean
mov r10, #0
@ start clean at cache level 0
loop1:
add r2, r10, r10, lsr #1
@ work out 3x current cache level
mov r1, r0, lsr r2
@ extract cache type bits from clidr
and r1, r1, #7
@ mask of the bits for current cache only
cmp r1, #2
@ see what cache we have at this level
blt skip
@ skip if no cache, or just i-cache
mcr p15, 2, r10, c0, c0, 0
@ select current cache level in cssr
mcr p15, 0, r10, c7, c5, 4
@ isb to sych the new cssr&csidr
mrc p15, 1, r1, c0, c0, 0
@ read the new csidr
and r2, r1, #7
@ extract the length of the cache lines
add r2, r2, #4
@ add 4 (line length offset)
ldr r4, =0x3ff
ands r4, r4, r1, lsr #3
@ find maximum number on the way size
clz r5, r4
@ find bit position of way size increment
ldr r7, =0x7fff
ands r7, r7, r1, lsr #13
@ extract max number of the index size
loop2:
mov r9, r4
@ create working copy of max way size
loop3:
ARM( orr
r11, r10, r9, lsl r5 ) @ factor way and cache number into r11
ARM( orr
r11, r11, r7, lsl r2 ) @ factor index number into r11
THUMB(  lsl r6, r9, r5
)
THUMB(  orr r11, r10, r6
) @ factor way and cache number into r11
THUMB(  lsl r6, r7, r2
)
THUMB(  orr r11, r11, r6
) @ factor index number into r11
mcr p15, 0, r11, c7, c14, 2
@ clean & invalidate by set/way
subs r9, r9, #1
@ decrement the way
bge loop3
subs r7, r7, #1
@ decrement the index
bge loop2
skip:
add r10, r10, #2
@ increment cache number
cmp r3, r10
bgt loop1
finished:
ldmfd sp!, {r0-r7, r9-r11}
mov r10, #0
@ swith back to cache level 0
mcr p15, 2, r10, c0, c0, 0
@ select current cache level in cssr
iflush:
mcr p15, 0, r10, c7, c10, 4
@ DSB
mcr p15, 0, r10, c7, c5, 0
@ invalidate I+BTB
mcr p15, 0, r10, c7, c10, 4
@ DSB
mcr p15, 0, r10, c7, c5, 4
@ ISB
mov pc, lr

__armv5tej_mmu_cache_flush:
1: mrc
p15, 0, r15, c7, c14, 3 @ test,clean,invalidate D cache
bne 1b
mcr p15, 0, r0, c7, c5, 0
@ flush I cache
mcr p15, 0, r0, c7, c10, 4
@ drain WB
mov pc, lr

__armv4_mmu_cache_flush:
mov r2, #64*1024
@ default: 32K dcache size (*2)
mov r11, #32
@ default: 32 byte line size
mrc p15, 0, r3, c0, c0, 1
@ read cache type
teq r3, r9
@ cache ID register present?
beq no_cache_id
mov r1, r3, lsr #18
and r1, r1, #7
mov r2, #1024
mov r2, r2, lsl r1
@ base dcache size *2
tst r3, #1 << 14
@ test M bit
addne r2, r2, r2, lsr #1
@ +1/2 size if M == 1
mov r3, r3, lsr #12
and r3, r3, #3
mov r11, #8
mov r11, r11, lsl r3
@ cache line size in bytes
no_cache_id:
mov r1, pc
bic r1, r1, #63
@ align to longest cache line
add r2, r1, r2
1:
ARM( ldr
r3, [r1], r11 ) @ s/w flush D cache
THUMB(  ldr     r3, [r1] ) @ s/w flush D cache
THUMB(  add     r1, r1, r11 )
teq r1, r2
bne 1b

mcr p15, 0, r1, c7, c5, 0
@ flush I cache
mcr p15, 0, r1, c7, c6, 0
@ flush D cache
mcr p15, 0, r1, c7, c10, 4
@ drain WB
mov pc, lr

__armv3_mmu_cache_flush:
__armv3_mpu_cache_flush:
mov r1, #0
mcr p15, 0, r1, c7, c0, 0
@ invalidate whole cache v3
mov pc, lr

/*
* Various debugging routines for printing hex characters and
* memory, which again must be relocatable.
*/
#ifdef DEBUG
.align  2
.type phexbuf,#object
phexbuf:  .space 12
.size phexbuf, . - phexbuf

@ phex corrupts {r0, r1, r2, r3}
phex: adr
r3, phexbuf
mov r2, #0
strb r2, [r3, r1]
1: subs
r1, r1, #1
movmi r0, r3
bmi puts
and r2, r0, #15
mov r0, r0, lsr #4
cmp r2, #10
addge r2, r2, #7
add r2, r2, #'0'
strb r2, [r3, r1]
b 1b

@ puts corrupts {r0, r1, r2, r3}
puts: loadsp
r3, r1
1: ldrb
r2, [r0], #1
teq r2, #0
moveq pc, lr
2: writeb
r2, r3
mov r1, #0x00020000
3: subs
r1, r1, #1
bne 3b
teq r2, #'\n'
moveq r2, #'\r'
beq 2b
teq r0, #0
bne 1b
mov pc, lr
@ putc corrupts {r0, r1, r2, r3}
putc:
mov r2, r0
mov r0, #0
loadsp  r3, r1
b 2b

@ memdump corrupts {r0, r1, r2, r3, r10, r11, r12, lr}
memdump:  mov r12, r0
mov r10, lr
mov r11, #0
2: mov
r0, r11, lsl #2
add r0, r0, r12
mov r1, #8
bl phex
mov r0, #':'
bl putc
1: mov
r0, #' '
bl putc
ldr r0, [r12, r11, lsl #2]
mov r1, #8
bl phex
and r0, r11, #7
teq r0, #3
moveq r0, #' '
bleq putc
and r0, r11, #7
add r11, r11, #1
teq r0, #7
bne 1b
mov r0, #'\n'
bl putc
cmp r11, #64
blt 2b
mov pc, r10
#endif

.ltorg
reloc_code_end:

.align
.section ".stack", "aw", %nobits
.L_user_stack:
.space 4096
.L_user_stack_end:


}
./arch/arm/boot/compressed/misc.c:174:decompress_kernel(unsigned long output_start, unsigned long free_mem_ptr_p,


2、进入内核的第二阶段：
内核启动第二阶段主要完成的工作有，cpu ID检查，machine ID(也就是开发板ID)检查，创建初始化页表，设置C代码运行环境，跳转到内核第一个真正的C函数startkernel开始执行
这一阶段涉及到两个重要的结构体：

(1) 一个是struct proc_info_list 主要描述CPU相关的信息，定义在文件arch\arm\include\asm\procinfo.h中，与其相关的函数及变量在文件arch/arm/mm/proc_arm920.S中被定义和赋值。

(2) 另一个结构体是描述开发板或者说机器信息的结构体struct machine_desc，定义在\arch\arm\include\asm\mach\arch.h文件中，其函数的定义和变量的赋值在板极相关文件arch/arm/mach-s3c2410/mach-smdk2410.c中实现，这也是内核移植非常重要的一个文件。

该阶段一般由前面的解压缩代码调用,进入该阶段要求：

MMU = off, D-cache = off, I-cache = dont care,r0 = 0, r1 = machine id.

所有的机器ID列表保存在arch/arm/tools/mach-types 文件中，在编译时会将这些机器ID按照统一的格式链接到基本内核映像文件vmlinux的__arch_info_begin和__arch_info_end之间的段中。存储格式定义在include/asm-arm/mach/arch.h文件中的结构体struct machine_desc {}。这两个结构体的内容最终会被连接到基本内核映像vmlinux中的两个段内，分别是*(.proc.info.init)和*(.arch.info.init)，可以参考下面的连接脚本。

链接脚本：arch/arm/kernel/vmlinux.lds
{
链接脚本：arch/arm/kernel/vmlinux.lds

*****************************链接脚本**************************************

SECTIONS

{

. = TEXTADDR;

.init : { /* 初始化代码段*/

_stext = .;

_sinittext = .;

*(.init.text)

_einittext = .;

__proc_info_begin = .;

*(.proc.info.init)

__proc_info_end = .;

__arch_info_begin = .;

*(.arch.info.init)

__arch_info_end = .;

__tagtable_begin = .;

*(.taglist.init)

__tagtable_end = .;

. = ALIGN(16);

__setup_start = .;

*(.init.setup)

__setup_end = .;

__early_begin = .;

*(.early_param.init)

__early_end = .;

__initcall_start = .;

*(.initcall1.init)

*(.initcall2.init)

*(.initcall3.init)

*(.initcall4.init)

*(.initcall5.init)

*(.initcall6.init)

*(.initcall7.init)

__initcall_end = .;

__con_initcall_start = .;

*(.con_initcall.init)

__con_initcall_end = .;

__security_initcall_start = .;

*(.security_initcall.init)

__security_initcall_end = .;

. = ALIGN(32);

__initramfs_start = .;

usr/built-in.o(.init.ramfs)

__initramfs_end = .;

. = ALIGN(64);

__per_cpu_start = .;

*(.data.percpu)

__per_cpu_end = .;

#ifndef CONFIG_XIP_KERNEL

__init_begin = _stext;

*(.init.data)

. = ALIGN(4096);

__init_end = .;

#endif

}

*****************************链接脚本**************************************

下面开始代码\arch\arm\kernel\head.S的注释：


}


done, booting the kernel.
Linux version 3.0.8 (root@u12d32) (gcc version 4.4.1 (Hisilicon_v100(gcc4.4-290+uclibc_0.9.32.1+eabi+linuxpthread)) ) #17 Tue Mar 25 16:34:29 CST 2014
CPU: ARM926EJ-S [41069265] revision 5 (ARMv5TEJ), cr=00053177
CPU: VIVT data cache, VIVT instruction cache
Machine: hi3518
Memory policy: ECC disabled, Data cache writeback 
AXI bus clock 200000000. //./arch/arm/mach-hi3518/core.c +307
Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 16256 //vim ./mm/page_alloc.c +3307
Kernel command line: mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(config),3072k(kernel),11264k(rootfs)
PID hash table entries: 256 (order: -2, 1024 bytes) //./mm/page_alloc.c:5435:
Dentry cache hash table entries: 8192 (order: 3, 32768 bytes)
Inode-cache hash table entries: 4096 (order: 2, 16384 bytes) //./mm/page_alloc.c +5435
Memory: 64MB = 64MB total  //./arch/arm/mm/init.c +613:
printk(KERN_INFO "Memory:");./arch/arm/mm/init.c +623
Memory: 59748k/59748k available, 5788k reserved, 0K highmem
Virtual kernel memory layout: //./arch/arm/mm/init.c +633
    vector  : 0xffff0000 - 0xffff1000   (   4 kB)
    fixmap  : 0xfff00000 - 0xfffe0000   ( 896 kB)
    DMA     : 0xffc00000 - 0xffe00000   (   2 MB)
    vmalloc : 0xc4800000 - 0xfe000000   ( 920 MB)
    lowmem  : 0xc0000000 - 0xc4000000   (  64 MB)
    modules : 0xbf000000 - 0xc0000000   (  16 MB)
      .init : 0xc0008000 - 0xc0029000   ( 132 kB)
      .text : 0xc0029000 - 0xc04cf000   (4760 kB)
      .data : 0xc04d0000 - 0xc04f35a0   ( 142 kB)
       .bss : 0xc04f35c4 - 0xc0510e78   ( 119 kB)
SLUB: Genslabs=13, HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1 //./mm/slub.c +3327
NR_IRQS:32 nr_irqs:32 32 //./kernel/irq/irqdesc.c +218 
sched_clock: 32 bits at 100MHz, resolution 10ns, wraps every 42949ms ./arch/arm/kernel/sched_clock.c +55
Console: colour dummy device 80x30 //./drivers/tty/vt/vt.c +2933
Calibrating delay loop... 218.72 BogoMIPS (lpj=1093632) //./arch/frv/kernel/setup.c +714
pid_max: default: 32768 minimum: 301 //./kernel/pid.c +561
Mount-cache hash table entries: 512 //./fs/namespace.c +2688
CPU: Testing write buffer coherency: ok //./arch/arm/mm/fault-armv.c +ss240
devtmpfs: initialized //./drivers/base/devtmpfs.c +394
NET: Registered protocol family 16 //./net/socket.c +2480
Serial: AMBA PL011 UART driver //./drivers/tty/serial/amba-pl011.c +2051
uart:0: ttyAMA0 at MMIO 0x20080000 (irq = 5) is a PL011 rev2 //./arch/arm/mach-hi3518/core.c +477
console [ttyAMA0] enabled
uart:1: ttyAMA1 at MMIO 0x20090000 (irq = 5) is a PL011 rev2
bio: create slab <bio-0> at 0 //./fs/bio.c +114
SCSI subsystem initialized //./drivers/scsi/scsi.c +1332
usbcore: registered new interface driver usbfs //./drivers/usb/core/driver.c +879
usbcore: registered new interface driver hub
usbcore: registered new device driver usb
Switching to clocksource timer1 //./kernel/time/clocksource.c +577
NET: Registered protocol family 2 //./net/socket.c +2480
IP route cache hash table entries: 1024 (order: 0, 4096 bytes) // ./mm/page_alloc.c +5435
TCP established hash table entries: 2048 (order: 2, 16384 bytes)
TCP bind hash table entries: 2048 (order: 1, 8192 bytes)
TCP: Hash tables configured (established 2048 bind 2048)
TCP reno registered
UDP hash table entries: 256 (order: 0, 4096 bytes)
UDP-Lite hash table entries: 256 (order: 0, 4096 bytes)
NET: Registered protocol family 1
RPC: Registered named UNIX socket transport module.
RPC: Registered udp transport module.
RPC: Registered tcp transport module.
RPC: Registered tcp NFSv4.1 backchannel transport module.
NetWinder Floating Point Emulator V0.97 (double precision) //./arch/arm/nwfpe/fpmodule.c +101
VFS: Disk quotas dquot_6.5.2
Dquot-cache hash table entries: 1024 (order 0, 4096 bytes)
JFFS2 version 2.2. (NAND) 漏 2001-2006 Red Hat, Inc.
fuse init (API version 7.16)
msgmni has been set to 116
Block layer SCSI generic (bsg) driver version 0.4 loaded (major 253)
io scheduler noop registered
io scheduler deadline registered (default)
io scheduler cfq registered
brd: module loaded
loop: module loaded
Spi id table Version 1.22
Spi(cs1) ID: 0xEF 0x40 0x18 0x00 0x00 0x00
SPI FLASH start_up_mode is 3 Bytes
Spi(cs1): 
Block:64KB 
Chip:16MB 
Name:"W25Q128B"
spi size: 0x16777216
chip num: 1
5 cmdlinepart partitions found on MTD device hi_sfc
Creating 5 MTD partitions on "hi_sfc":
0x000000000000-0x000000040000 : "boot"
0x000000040000-0x000000060000 : "env"
0x000000060000-0x000000080000 : "config"
0x000000080000-0x000000380000 : "kernel"
0x000000380000-0x000000e80000 : "rootfs"
Fixed MDIO Bus: probed
himii: probed
PPP generic driver version 2.4.2
PPP Deflate Compression module registered
PPP BSD Compression module registered
PPP MPPE Compression module registered
NET: Registered protocol family 24
ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
hiusb-ehci hiusb-ehci.0: HIUSB EHCI
hiusb-ehci hiusb-ehci.0: new USB bus registered, assigned bus number 1
hiusb-ehci hiusb-ehci.0: irq 15, io mem 0x100b0000
hiusb-ehci hiusb-ehci.0: USB 0.0 started, EHCI 1.00
hub 1-0:1.0: USB hub found
hub 1-0:1.0: 1 port detected
ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
hiusb-ohci hiusb-ohci.0: HIUSB OHCI
hiusb-ohci hiusb-ohci.0: new USB bus registered, assigned bus number 2
hiusb-ohci hiusb-ohci.0: irq 16, io mem 0x100a0000
hub 2-0:1.0: USB hub found
hub 2-0:1.0: 1 port detected
usbcore: registered new interface driver cdc_acm
cdc_acm: USB Abstract Control Model driver for USB modems and ISDN adapters
usbcore: registered new interface driver cdc_wdm
Initializing USB Mass Storage driver...
usbcore: registered new interface driver usb-storage
USB Mass Storage support registered.
usbcore: registered new interface driver ums-alauda
usbcore: registered new interface driver ums-datafab
usbcore: registered new interface driver ums-freecom
usbcore: registered new interface driver ums-isd200
usbcore: registered new interface driver ums-jumpshot
usbcore: registered new interface driver ums-sddr09
usbcore: registered new interface driver ums-sddr55
usbcore: registered new interface driver mdc800
mdc800: v0.7.5 (30/10/2000):USB Driver for Mustek MDC800 Digital Camera
usbcore: registered new interface driver usbserial
usbserial: USB Serial Driver core
USB Serial support registered for GSM modem (1-port)
usbcore: registered new interface driver option
option: v0.7.2:USB Driver for GSM modems
mousedev: PS/2 mouse device common for all mice
usbcore: registered new interface driver usbhid
usbhid: USB HID core driver
TCP cubic registered
Initializing XFRM netlink socket
NET: Registered protocol family 10
NET: Registered protocol family 17
NET: Registered protocol family 15
lib80211: common routines for IEEE802.11 drivers
Registering the dns_resolver key type
registered taskstats version 1
drivers/rtc/hctosys.c: unable to open rtc device (rtc0)
鴘sb 1-1: new high speed USB device number 2 using hiusb-ehci
VFS: Mounted root (jffs2 filesystem) on device 31:4.
devtmpfs: mounted
Freeing init memory: 132K


            _ _ _ _ _ _ _ _ _ _ _ _
            \  _  _   _  _ _ ___
            / /__/ \ |_/
           / __   /  -  _ ___
          / /  / /  / /
  _ _ _ _/ /  /  \_/  \_ ______
___________\___\__________________












/*===========================================hi3518c start kernel end =============================================*/


1、第一句话的打印信息为：locationi:/init/Version.c
const char linux_banner[] =
"Linux version " UTS_RELEASE " (" LINUX_COMPILE_BY "@"
LINUX_COMPILE_HOST ") (" LINUX_COMPILER ") " UTS_VERSION "\n";
2、


内核初始化后跳转到start_kernel(）
，start_kernel(）最后调用rest_init();驱动中初始化内容在rest_init()中调用
init/main.c中
static noinline void __init_refok rest_init(void)
    __releases(kernel_lock)
{
    int pid;


    kernel_thread(kernel_init, NULL, CLONE_FS | CLONE_SIGHAND);  ／／加载
    numa_default_policy();
    pid = kernel_thread(kthreadd, NULL, CLONE_FS | CLONE_FILES);
    kthreadd_task = find_task_by_pid_ns(pid, &init_pid_ns);
    unlock_kernel();


    /*
     * The boot idle thread must execute schedule()
     * at least once to get things moving:
     */
    init_idle_bootup_task(current);
    rcu_scheduler_starting();
    preempt_enable_no_resched();
    schedule();
    preempt_disable();


    /* Call into cpu_idle with preempt disabled */
    cpu_idle();
}


kernel_init在
static int __init kernel_init(void * unused)
{
    lock_kernel();


    /*
     * init can allocate pages on any node
     */
    set_mems_allowed(node_possible_map);
    /*
     * init can run on any cpu.
     */
    set_cpus_allowed_ptr(current, cpu_all_mask);
    /*
     * Tell the world that we're going to be the grim
     * reaper of innocent orphaned children.
     *
     * We don't want people to have to make incorrect
     * assumptions about where in the task array this
     * can be found.
     */
    init_pid_ns.child_reaper = current;


    cad_pid = task_pid(current);


    smp_prepare_cpus(setup_max_cpus);


    do_pre_smp_initcalls();
    start_boot_trace();


    smp_init();
    sched_init_smp();


    do_basic_setup();  ／／在此基本设置


    /*
     * check if there is an early userspace init.  If yes, let it do all
     * the work
     */


    if (!ramdisk_execute_command)
        ramdisk_execute_command = "/init";


    if (sys_access((const char __user *) ramdisk_execute_command, 0) != 0) {
        ramdisk_execute_command = NULL;
        prepare_namespace();
    }


    /*
     * Ok, we have completed the initial bootup, and
     * we're essentially up and running. Get rid of the
     * initmem segments and start the user-mode stuff..
     */


    init_post();
    return 0;
}


 do_basic_setup(); 
static void __init do_basic_setup(void)
{
    rcu_init_sched(); /* needed by module_init stage. */
    init_workqueues();
    cpuset_init_smp();
    usermodehelper_init();
    driver_init();／／驱动初始化
    init_irq_proc();
    do_ctors();
    do_initcalls();  ／／调用
}


关于调用顺序知识
在2.6内核中，initcall.init区段又分成7个子区段，分别是 
.initcall1.init 
.initcall2.init 
.initcall3.init 
.initcall4.init 
.initcall5.init 
.initcall6.init 
.initcall7.init 


当需要把函数fn放到.initcall1.init区段时，只要声明 
core_initcall(fn); 
即可。 


其他的各个区段的定义方法分别是： 
core_initcall(fn) --->.initcall1.init 
postcore_initcall(fn) --->.initcall2.init 
arch_initcall(fn) --->.initcall3.init 
subsys_initcall(fn) --->.initcall4.init 
fs_initcall(fn) --->.initcall5.init 
device_initcall(fn) --->.initcall6.init 
late_initcall(fn) --->.initcall7.init 


而与2.4兼容的initcall(fn)则等价于device_initcall(fn)。 


各个子区段之间的顺序是确定的，即先调用.initcall1.init中的函数指针 
再调用.initcall2.init中的函数指针，等等。 
而在每个子区段中的函数指针的顺序是和链接顺序相关的，是不确定的。 


在内核中，不同的init函数被放在不同的子区段中，因此也就决定了它们的调用顺序。 
这样也就解决了一些init函数之间必须保证一定的调用顺序的问题。 
＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊
MACHINE_START(MX28EVK, "Freescale MX28EVK board")
    .phys_io    = 0x80000000,
    .io_pg_offst    = ((0xf0000000) >> 18) & 0xfffc,
    .boot_params    = 0x40000100,
    .fixup        = fixup_board,
    .map_io        = mx28_map_io,
    .init_irq    = mx28_irq_init,
    .init_machine    = mx28evk_init_machine,
    .timer        = &mx28_timer.timer,
MACHINE_END
由上发现，MACHINE_START主要是定义了"struct machine_desc"的类型，放在 section(".arch.info.init")，是初始化数据，Kernel 起来之后将被丢弃。

各个成员函数在不同时期被调用：
1. .init_machine 在 arch/arm/kernel/setup.c 中被 customize_machine 调用，放在 arch_initcall() 段里面，会自动按顺序被调用。

2. init_irq在start_kernel() --> init_IRQ() --> init_arch_irq() 被调用
3. map_io 在 setup_arch() --> paging_init() --> devicemaps_init()被调用

其他主要都在 setup_arch() 中用到。





*************************************
static void __init do_initcalls(void)
{
    initcall_t *call;


    for (call = __early_initcall_end; call < __initcall_end; call++)
        do_one_initcall(*call);


    /* Make sure there is no pending stuff from the initcall sequence */
    flush_scheduled_work();
}
 这个__initcall_start是在文件        arch/xxx/kernel/vmlinux.lds.S (其中的xxx 是你的体系结构的名称,例如i386)这个文件是内核ld的时候使用的.其中定义了各个sectioin,看看就明白了。在这个文件中有个.initcall.init, 代码如下: 




./init/main.c:821 driver_init()




./drivers/usb/host/hiusb-3518.c
./drivers/usb/host/.hiusb-3518.o.cmd
./drivers/usb/host/hiusb-3518.o
./drivers/mtd/devices/hisfc350/hisfc350_3518.c
./drivers/mtd/nand/hinfc301/hinfc301_3518.c
./drivers/net/hieth-sf/sys-3518.c
./drivers/mmc/host/himciv100/himci_3518.c
./drivers/cpufreq/hi3518-cpufreq.c
./drivers/cpufreq/.hi3518-cpufreq.o.cmd
./drivers/cpufreq/hi3518-cpufreq.o
./include/config/hi3518
./include/config/mach/hi3518.h
./include/config/arch/hi3518.h
./config_hi3518c
./arch/arm/mach-hi3518


./mm/.highmem.o.cmd
./mm/highmem.o
./mm/highmem.c
./kernel/power/hibernate.c
./kernel/irq/generic-chip.c
./kernel/irq/dummychip.c
./kernel/irq/.dummychip.o.cmd
./kernel/irq/chip.o
./kernel/irq/dummychip.o
./kernel/irq/chip.c
./kernel/irq/.chip.o.cmd
./kernel/stop_machine.c
./net/dccp/ccids/lib/packet_history.h
./net/dccp/ccids/lib/packet_history.c
./net/802/hippi.c
./net/ipv4/tcp_highspeed.c
./net/bluetooth/hidp
./net/bluetooth/hidp/hidp.h
./drivers/input/serio/hil_mlc.c
./drivers/input/touchscreen/hampshire.c
./drivers/input/touchscreen/touchit213.c
./drivers/input/keyboard/hilkbd.c
./drivers/input/keyboard/hil_kbd.c
./drivers/tty/vt/defkeymap.c_shipped
./drivers/sh/intc/chip.c
./drivers/hid
./drivers/hid/hid-ntrig.c
./drivers/hid/hid-belkin.c
./drivers/hid/hid-roccat-kovaplus.h
./drivers/hid/hid-lgff.c
./drivers/hid/hid-ezkey.c
./drivers/hid/hid-emsff.c
./drivers/hid/hid-waltop.c
./drivers/hid/hid-roccat-koneplus.h
./drivers/hid/hid-debug.o
./drivers/hid/hid-dr.c
./drivers/hid/hid-roccat-kovaplus.c
./drivers/hid/hid-elecom.c
./drivers/hid/.hid-core.o.cmd
./drivers/hid/hid-roccat-kone.c
./drivers/hid/hid-samsung.c
./drivers/hid/hid-microsoft.c
./drivers/hid/hid-lcpower.c
./drivers/hid/hid-petalynx.c
./drivers/hid/hid-roccat.c
./drivers/hid/hidraw.c
./drivers/hid/hid-magicmouse.c
./drivers/hid/hid-roccat-pyra.c
./drivers/hid/hid-ortek.c
./drivers/hid/.hid-input.o.cmd
./drivers/hid/hid-lg3ff.c
./drivers/hid/hid-kye.c
./drivers/hid/.hid.o.cmd
./drivers/hid/hid-input.o
./drivers/hid/hid-apple.c
./drivers/hid/hid-monterey.c
./drivers/hid/hid-kensington.c
./drivers/hid/hid-ids.h
./drivers/hid/hid-gaff.c
./drivers/hid/hid-tmff.c
./drivers/hid/hid-input.c
./drivers/hid/hid-zydacron.c
./drivers/hid/hid-uclogic.c
./drivers/hid/usbhid
./drivers/hid/usbhid/.hid-core.o.cmd
./drivers/hid/usbhid/hid-pidff.c
./drivers/hid/usbhid/hid-quirks.c
./drivers/hid/usbhid/usbhid.h
./drivers/hid/usbhid/hid-core.o
./drivers/hid/usbhid/hiddev.c
./drivers/hid/usbhid/hid-quirks.o
./drivers/hid/usbhid/.usbhid.o.cmd
./drivers/hid/usbhid/.hid-quirks.o.cmd
./drivers/hid/usbhid/hid-core.c
./drivers/hid/usbhid/usbhid.o
./drivers/hid/hid-cherry.c
./drivers/hid/hid-sjoy.c
./drivers/hid/hid-chicony.c
./drivers/hid/hid-core.o
./drivers/hid/hid-roccat-arvo.c
./drivers/hid/hid-prodikeys.c
./drivers/hid/hid-sunplus.c
./drivers/hid/hid-roccat-pyra.h
./drivers/hid/hid-roccat-koneplus.c
./drivers/hid/hid-wacom.c
./drivers/hid/hid-roccat-common.h
./drivers/hid/hid-axff.c
./drivers/hid/hid-a4tech.c
./drivers/hid/hid-twinhan.c
./drivers/hid/hid.o
./drivers/hid/hid-lg.h
./drivers/hid/hid-cypress.c
./drivers/hid/hid-gyration.c
./drivers/hid/hid-quanta.c
./drivers/hid/hid-sony.c
./drivers/hid/hid-debug.c
./drivers/hid/hid-roccat-common.c
./drivers/hid/.hid-debug.o.cmd
./drivers/hid/hid-roccat-kone.h
./drivers/hid/hid-topseed.c
./drivers/hid/hid-keytouch.c
./drivers/hid/hid-pl.c
./drivers/hid/hid-picolcd.c
./drivers/hid/hid-lg2ff.c
./drivers/hid/hid-core.c
./drivers/hid/hid-zpff.c
./drivers/hid/hid-multitouch.c
./drivers/hid/hid-lg.c
./drivers/hid/hid-roccat-arvo.h
./drivers/hid/hid-lg4ff.c
./drivers/usb/host/hiusb-ohci.c
./drivers/usb/host/hiusb.h
./drivers/usb/host/hiusb-godarm.c
./drivers/usb/host/hiusb-godbox.c
./drivers/usb/host/hiusb-3518.c
./drivers/usb/host/Kconfig.hiusb
./drivers/usb/host/hiusb-godbox_v1.c
./drivers/usb/host/hiusb-phoenix.c
./drivers/usb/host/hiusb-godnet.c
./drivers/usb/host/hiusb-ehci.c
./drivers/usb/host/hiusb-hi3520d.c
./drivers/usb/host/.hiusb-3518.o.cmd
./drivers/usb/host/hiusb-godeyes.c
./drivers/usb/host/hiusb-3518.o
./drivers/usb/serial/ChangeLog.history
./drivers/usb/serial/whiteheat.h
./drivers/usb/serial/whiteheat.c
./drivers/usb/core/otg_whitelist.h
./drivers/usb/gadget/hid.c
./drivers/usb/gadget/gadget_chips.h
./drivers/usb/gadget/f_hid.c
./drivers/staging/msm/lcdc_toshiba_wvga_pt.c
./drivers/staging/msm/mddi_toshiba.c
./drivers/staging/msm/mddi_toshiba_wvga_pt.c
./drivers/staging/msm/mddi_toshiba.h
./drivers/staging/msm/mddi_toshiba_vga.c
./drivers/staging/brcm80211/include/sbchipc.h
./drivers/staging/brcm80211/brcmsmac/wlc_phy_shim.c
./drivers/staging/brcm80211/brcmsmac/wlc_phy_shim.h
./drivers/staging/brcm80211/brcmfmac/bcmchip.h
./drivers/staging/phison
./drivers/staging/phison/phison.c
./drivers/staging/rts_pstor/rtsx_chip.h
./drivers/staging/rts_pstor/rtsx_chip.c
./drivers/staging/ath6kl/hif
./drivers/staging/ath6kl/hif/common/hif_sdio_common.h
./drivers/staging/ath6kl/hif/sdio/linux_sdio/src/hif_scatter.c
./drivers/staging/ath6kl/hif/sdio/linux_sdio/src/hif.c
./drivers/staging/ath6kl/hif/sdio/linux_sdio/include/hif_internal.h
./drivers/staging/ath6kl/include/hif.h
./drivers/pci/hipcie
./drivers/mtd/chips
./drivers/mtd/chips/chipreg.o
./drivers/mtd/chips/chipreg.c
./drivers/mtd/chips/.chipreg.o.cmd
./drivers/mtd/devices/hisfc300new
./drivers/mtd/devices/hisfc300new/hisfc300new_spi_ids.c
./drivers/mtd/devices/hisfc300new/hisfc300new_godcube.c
./drivers/mtd/devices/hisfc300new/hisfc300new_godarm.c
./drivers/mtd/devices/hisfc300new/hisfc300new.h
./drivers/mtd/devices/hisfc300new/hisfc300new_godnet.c
./drivers/mtd/devices/hisfc300new/hisfc300new.c
./drivers/mtd/devices/hisfc300new/hisfc300new_spi_s25fl256s.c
./drivers/mtd/devices/hisfc300new/hisfc300new_spi_general.c
./drivers/mtd/devices/hisfc300new/hisfc300new_spi_n25q256a.c
./drivers/mtd/devices/hisfc350
./drivers/mtd/devices/hisfc350/hisfc350_spi_mx25l25635e.c
./drivers/mtd/devices/hisfc350/hisfc350.h
./drivers/mtd/devices/hisfc350/hisfc350_spi_ids.o
./drivers/mtd/devices/hisfc350/hisfc350.c
./drivers/mtd/devices/hisfc350/hisfc350_spi_w25q256fv.c
./drivers/mtd/devices/hisfc350/hisfc350_hi3520d.c
./drivers/mtd/devices/hisfc350/hisfc350_spi_ids.c
./drivers/mtd/devices/hisfc350/hisfc350_spi_s25fl256s.c
./drivers/mtd/devices/hisfc350/hisfc350_spi_general.c
./drivers/mtd/devices/hisfc350/.hisfc350.o.cmd
./drivers/mtd/devices/hisfc350/.hisfc350_spi_ids.o.cmd
./drivers/mtd/devices/hisfc350/hisfc350_3518.c
./drivers/mtd/devices/hisfc350/hisfc350_phoenix.c
./drivers/mtd/devices/hisfc350/hisfc350_spi_en25q64.c
./drivers/mtd/devices/hisfc350/hisfc350_spi_n25q256a.c
./drivers/mtd/devices/hisfc350/hisfc350.o
./drivers/mtd/devices/hisfc300
./drivers/mtd/devices/hisfc300/hisfc300.c
./drivers/mtd/devices/hisfc300/hisfc300_spi_ids.c
./drivers/mtd/devices/hisfc300/hisfc300_godnet.c
./drivers/mtd/devices/hisfc300/hisfc300_godbox.c
./drivers/mtd/devices/hisfc300/hisfc300.h
./drivers/mtd/devices/hisfc300/hisfc300_godeyes.c
./drivers/mtd/devices/hisfc300/hisfc300_godcube.c
./drivers/mtd/nand/diskonchip.c
./drivers/mtd/nand/hinfc301
./drivers/mtd/nand/hinfc301/hinfc301dbg.c
./drivers/mtd/nand/hinfc301/hinfc301_phoenix.c
./drivers/mtd/nand/hinfc301/hinfc301.c
./drivers/mtd/nand/hinfc301/hinfc301_godbox.c
./drivers/mtd/nand/hinfc301/hinfc301_3518.c
./drivers/mtd/nand/hinfc301/hinfc301.h
./drivers/mtd/nand/hinfc301/hinfc301_godnet.c
./drivers/mtd/nand/hinfc301/hinfc301_godeyes.c
./drivers/mtd/nand/hinfc301/hinfc301_godarm.c
./drivers/mtd/nand/hinfc300
./drivers/mtd/nand/hinfc300/hinfc300dbg.c
./drivers/mtd/nand/hinfc300/hinfc300.h
./drivers/mtd/nand/hinfc300/hinfc300.c
./drivers/s390/cio/qdio_thinint.c
./drivers/platform/x86/toshiba_bluetooth.c
./drivers/platform/x86/toshiba_acpi.c
./drivers/platform/x86/thinkpad_acpi.c
./drivers/i2c/busses/i2c-highlander.c
./drivers/net/via-rhine.c
./drivers/net/wan/wanxlfw.inc_shipped
./drivers/net/wireless/ath/ath9k/hif_usb.c
./drivers/net/wireless/ath/ath9k/hif_usb.h
./drivers/net/wireless/bcm4329/include/sbchipc.h
./drivers/net/wireless/zd1211rw/zd_chip.h
./drivers/net/wireless/zd1211rw/zd_chip.c
./drivers/net/wireless/bcmdhd/include/sbchipc.h
./drivers/net/hamachi.c
./drivers/net/hieth-sf
./drivers/net/hieth-sf/hieth.h
./drivers/net/hieth-sf/hieth.o
./drivers/net/hieth-sf/sys-hi3520d.c
./drivers/net/hieth-sf/.hieth.o.cmd
./drivers/net/higmacv300
./drivers/net/higmacv300/higmacv300_timer.h
./drivers/net/higmacv300/higmacv300_timer.c
./drivers/net/higmacv300/higmacv300_godarm.h
./drivers/net/higmacv300/higmacv300.h
./drivers/net/higmacv300/higmacv300_regmap.h
./drivers/net/higmacv300/higmacv300_ethtool.c
./drivers/net/higmacv300/higmacv300_proc.c
./drivers/net/higmacv300/higmacv300_proc.h
./drivers/net/higmacv300/higmacv300_main.c
./drivers/net/higmacv300/higmacv300_godarm.c
./drivers/net/higmacv300/higmacv300_mdio.c
./drivers/ata/ahci_sys_hi3520d_defconfig.c
./drivers/macintosh/adbhid.c
./drivers/macintosh/mac_hid.c
./drivers/char/toshiba.c
./drivers/ssb/driver_chipcommon_pmu.c
./drivers/ssb/driver_chipcommon.c
./drivers/mmc/host/sh_mobile_sdhi.c
./drivers/mmc/host/himciv100
./drivers/mmc/host/himciv100/.himci.o.cmd
./drivers/mmc/host/himciv100/hi_mci.h
./drivers/mmc/host/himciv100/hi_mci_reg.h
./drivers/mmc/host/himciv100/himci_godnet.c
./drivers/mmc/host/himciv100/himci_godarm.c
./drivers/mmc/host/himciv100/himci.c
./drivers/mmc/host/himciv100/himci_godeyes.c
./drivers/mmc/host/himciv100/himci.o
./drivers/mmc/host/himciv100/himci_phoenix.c
./drivers/mmc/host/himciv100/himci_godbox.c
./drivers/mmc/host/himciv100/himci_3518.c
./drivers/cpufreq/hi3518-cpufreq.c
./drivers/cpufreq/.hi3518-cpufreq.o.cmd
./drivers/cpufreq/hi3518-cpufreq.o
./drivers/cpufreq/Kconfig.hifreq
./drivers/media/dvb/mantis/mantis_hif.c
./drivers/media/dvb/mantis/mantis_hif.h
./drivers/media/video/omap3isp/isphist.h
./drivers/media/video/omap3isp/isphist.c
./drivers/media/video/pwc/philips.txt
./drivers/bcma/driver_chipcommon_pmu.c
./drivers/bcma/driver_chipcommon.c
./drivers/message/fusion/lsi/mpi_history.txt
./drivers/video/via/chip.h
./drivers/video/hitfb.c
./drivers/video/msm/mddi_client_toshiba.c
./drivers/video/omap2/displays/panel-lgphilips-lb035q02.c
./drivers/video/chipsfb.c
./drivers/scsi/pm8001/pm8001_chips.h
./drivers/scsi/aic7xxx/aic7xxx_reg.h_shipped
./drivers/scsi/aic7xxx/aic79xx_reg.h_shipped
./drivers/scsi/aic7xxx/aic7xxx_seq.h_shipped
./drivers/scsi/aic7xxx/aic7xxx_reg_print.c_shipped
./drivers/scsi/aic7xxx/aic79xx_reg_print.c_shipped
./drivers/scsi/aic7xxx/aic79xx_seq.h_shipped
./drivers/scsi/sym53c8xx_2/sym_hipd.h
./drivers/scsi/sym53c8xx_2/sym_hipd.c
./drivers/scsi/53c700_d.h_shipped
./drivers/scsi/mvsas/mv_chips.h
./drivers/infiniband/hw/ehca/hipz_fns_core.h
./drivers/infiniband/hw/ehca/hipz_fns.h
./drivers/infiniband/hw/ehca/hipz_hw.h
./drivers/infiniband/hw/ipath/ipath_init_chip.c
./drivers/isdn/hisax
./drivers/isdn/hisax/hisax.h
./drivers/isdn/hisax/hisax_debug.h
./drivers/isdn/hisax/hisax_isac.h
./drivers/isdn/hisax/hisax_fcpcipnp.c
./drivers/isdn/hisax/hisax_if.h
./drivers/isdn/hisax/hisax_fcpcipnp.h
./drivers/isdn/hisax/hisax_cfg.h
./drivers/isdn/hisax/hisax_isac.c
./drivers/isdn/hisax/saphir.c
./drivers/crypto/hifn_795x.c
./firmware/whiteheat_loader.HEX
./firmware/whiteheat.HEX
./firmware/whiteheat_loader_debug.HEX
./sound/usb/6fire/chip.h
./sound/usb/6fire/chip.c
./sound/pci/ice1712/prodigy_hifi.h
./sound/pci/ice1712/prodigy_hifi.c
./sound/soc/mid-x86/mfld_machine.c
./tools/perf/Documentation/perf-archive.txt
./tools/perf/perf-archive.sh
./tools/perf/util/hist.h
./tools/perf/util/ui/browsers/hists.c
./tools/perf/util/hist.c
./include/linux/hid-debug.h
./include/linux/hidraw.h
./include/linux/usb/g_hid.h
./include/linux/hiddev.h
./include/linux/mtd/flashchip.h
./include/linux/clockchips.h
./include/linux/regulator/machine.h
./include/linux/hid.h
./include/linux/chio.h
./include/linux/hippidevice.h
./include/linux/hil.h
./include/linux/ssb/ssb_driver_chipcommon.h
./include/linux/mmc/sh_mobile_sdhi.h
./include/linux/toshiba.h
./include/linux/hid-roccat.h
./include/linux/if_hippi.h
./include/linux/highmem.h
./include/linux/bcma/bcma_driver_chipcommon.h
./include/linux/hil_mlc.h
./include/linux/unaligned/le_byteshift.h
./include/linux/unaligned/be_byteshift.h
./include/linux/highuid.h
./include/linux/stop_machine.h
./include/media/v4l2-chip-ident.h
./include/config/hiusb
./include/config/log/buf/shift.h
./include/config/hid
./include/config/usb/hid.h
./include/config/himci
./include/config/himciv100.h
./include/config/mtd/hisfc350.h
./include/config/hid.h
./include/config/hisfc350
./include/config/hisfc350/chip
./include/config/hisfc350/enable/chipselect
./include/config/hi3518
./include/config/hieth
./include/config/arm/l1/cache/shift.h
./include/config/mach/hi3518.h
./include/config/arch/hi3518.h
./config_hi3518c
./Documentation/sh/new-machine.txt
./Documentation/hid
./Documentation/hid/hidraw.txt
./Documentation/hid/hiddev.txt
./Documentation/usb/gadget_hid.txt
./Documentation/power/regulator/machine.txt
./Documentation/DocBook/v4l/vidioc-dbg-g-chip-ident.xml
./Documentation/sound/alsa/soc/machine.txt
./Documentation/sound/alsa/Audiophile-Usb.txt
./Documentation/highuid.txt
./Documentation/laptops/thinkpad-acpi.txt
./Documentation/timers/highres.txt
./Documentation/networking/tcp-thin.txt
./Documentation/vm/highmem.txt
./Documentation/x86/x86_64/machinecheck
./Documentation/arm/SA1100/GraphicsClient
./Documentation/arm/SA1100/GraphicsMaster
./Documentation/filesystems/caching
./Documentation/ABI/testing/sysfs-driver-hid-prodikeys
./Documentation/ABI/testing/sysfs-driver-hid-roccat-pyra
./Documentation/ABI/testing/sysfs-driver-hid-picolcd
./Documentation/ABI/testing/sysfs-driver-hid-roccat-kone
./Documentation/ABI/testing/sysfs-driver-hid
./Documentation/ABI/testing/sysfs-driver-hid-roccat-kovaplus
./Documentation/ABI/testing/sysfs-driver-hid-roccat-arvo
./Documentation/ABI/testing/sysfs-driver-hid-roccat-koneplus
./Documentation/ABI/obsolete/sysfs-driver-hid-roccat-koneplus
./Documentation/block/switching-sched.txt
./fs/fscache/histogram.c
./scripts/kconfig/zconf.tab.c_shipped
./scripts/kconfig/zconf.hash.c_shipped
./scripts/kconfig/lex.zconf.c_shipped
./scripts/genksyms/keywords.c_shipped
./scripts/genksyms/parse.h_shipped
./scripts/genksyms/parse.c_shipped
./scripts/genksyms/lex.c_shipped
./scripts/dtc/dtc-parser.tab.h_shipped
./scripts/dtc/dtc-parser.tab.c_shipped
./scripts/dtc/dtc-lexer.lex.c_shipped
./arch/ia64/kernel/machine_kexec.c
./arch/sh/boards/mach-highlander
./arch/sh/kernel/machine_kexec.c
./arch/sh/include/mach-common/mach/highlander.h
./arch/sh/include/asm/sfp-machine.h
./arch/sh/cchips
./arch/sh/lib/ashiftrt.S
./arch/blackfin/kernel/irqchip.c
./arch/blackfin/lib/umulsi3_highpart.S
./arch/blackfin/lib/smulsi3_highpart.S
./arch/s390/kernel/machine_kexec.c
./arch/s390/include/asm/sfp-machine.h
./arch/s390/include/asm/schid.h
./arch/frv/mm/highmem.c
./arch/frv/include/asm/highmem.h
./arch/unicore32/kernel/hibernate.c
./arch/unicore32/kernel/hibernate_asm.S
./arch/avr32/mach-at32ap/include/mach/chip.h
./arch/xtensa/include/asm/highmem.h
./arch/mn10300/include/asm/highmem.h
./arch/x86/mm/highmem_32.c
./arch/x86/kernel/machine_kexec_32.c
./arch/x86/kernel/machine_kexec_64.c
./arch/x86/kernel/cpu/mcheck/winchip.c
./arch/x86/power/hibernate_asm_64.S
./arch/x86/power/hibernate_64.c
./arch/x86/power/hibernate_asm_32.S
./arch/x86/power/hibernate_32.c
./arch/x86/include/asm/visws/lithium.h
./arch/x86/include/asm/highmem.h
./arch/um/sys-ppc/miscthings.c
./arch/alpha/kernel/sys_rawhide.c
./arch/alpha/include/asm/sfp-machine.h
./arch/powerpc/platforms/cell/spufs/spu_save_dump.h_shipped
./arch/powerpc/platforms/cell/spufs/spu_restore_dump.h_shipped
./arch/powerpc/mm/highmem.c
./arch/powerpc/kernel/machine_kexec_32.c
./arch/powerpc/kernel/machine_kexec_64.c
./arch/powerpc/kernel/machine_kexec.c
./arch/powerpc/include/asm/sfp-machine.h
./arch/powerpc/include/asm/code-patching.h
./arch/powerpc/include/asm/highmem.h
./arch/powerpc/lib/code-patching.c
./arch/arm/mach-pxa/himalaya.c
./arch/arm/mm/highmem.c
./arch/arm/mm/cache-hil2v200.c
./arch/arm/mm/cache-hil2v100.c
./arch/arm/mm/cache-hil2v200.h
./arch/arm/mm/cache-hil2v100.h
./arch/arm/kernel/machine_kexec.c
./arch/arm/mach-hi3518
./arch/arm/configs/hi3516c_full_defconfig
./arch/arm/configs/hi3518a_full_defconfig
./arch/arm/configs/hi3518c_full_defconfig
./arch/arm/configs/hi3518a_mini_defconfig
./arch/arm/configs/hi3518c_mini_defconfig
./arch/arm/configs/hi3516c_mini_defconfig
./arch/arm/configs/hi3520d_full_defconfig
./arch/arm/configs/hi3520d_mini_defconfig
./arch/arm/configs/hi3716m_defconfig
./arch/arm/include/asm/highmem.h
./arch/arm/mach-hi3520d
./arch/arm/mach-u300/dummyspichip.c
./arch/arm/mach-msm/board-sapphire.c
./arch/arm/mach-msm/board-mahimahi.c
./arch/arm/mach-bcmring/csp/chipc
./arch/arm/mach-bcmring/csp/chipc/chipcHw_str.c
./arch/arm/mach-bcmring/csp/chipc/chipcHw_reset.c
./arch/arm/mach-bcmring/csp/chipc/chipcHw_init.c
./arch/arm/mach-bcmring/csp/chipc/chipcHw.c
./arch/arm/mach-bcmring/include/mach/csp/chipcHw_def.h
./arch/arm/mach-bcmring/include/mach/csp/chipcHw_reg.h
./arch/arm/mach-bcmring/include/mach/csp/chipcHw_inline.h
./arch/arm/plat-spear/include/plat/shirq.h
./arch/arm/plat-spear/shirq.c
./arch/arm/mach-godbox/hi_pm_sleep.S
./arch/arm/mach-godbox/hipm.c
./arch/tile/mm/highmem.c
./arch/tile/kernel/machine_kexec.c
./arch/tile/include/hv/drv_mshim_intf.h
./arch/tile/include/asm/highmem.h
./arch/tile/include/arch/chip.h
./arch/tile/include/arch/chip_tilepro.h
./arch/tile/include/arch/chip_tilegx.h
./arch/tile/include/arch/chip_tile64.h
./arch/mips/mm/highmem.c
./arch/mips/kernel/mips_machine.c
./arch/mips/kernel/machine_kexec.c
./arch/mips/power/hibernate.S
./arch/mips/include/asm/mips_machine.h
./arch/mips/include/asm/fw/arc/hinv.h
./arch/mips/include/asm/highmem.h
./arch/mips/include/asm/mach-loongson/machine.h
./arch/sparc/mm/highmem.c
./arch/sparc/kernel/pci_schizo.c
./arch/sparc/include/asm/sfp-machine.h
./arch/sparc/include/asm/sfp-machine_32.h
./arch/sparc/include/asm/highmem.h
./arch/sparc/include/asm/machines.h
./arch/sparc/include/asm/sfp-machine_64.h
./arch/m68k/amiga/chipram.c
./arch/m68k/include/asm/machines.h
./samples/hidraw
./samples/hidraw/hid-example.c








1、arch/arm/mach-spear6xx/include/mach/hardware.h :
/* Vitual to physical translation of statically mapped space */
#define IO_ADDRESS(x)
(x | 0xF0000000)


2、arch/arm/mach-spear3xx/include/mach/hardware.h ：
/* Vitual to physical translation of statically mapped space */
#define IO_ADDRESS(x)
(x | 0xF0000000)


3、U8500 mach/hardware.h
/* this macro is used in assembly, so no cast */
#define IO_ADDRESS(x)           \
(((x) & 0x0fffffff) + (((x) >> 4) & 0x0f000000) + U8500_IO_VIRTUAL)


4、mach/io.h
#define IO_ADDRESS(x) ((x) >= GODNET_IOCH1_PHYS ? (x) + IO_OFFSET_HIGH \
: (x) + IO_OFFSET_MID)


5、3520d
#define IO_ADDRESS(x) ((x) >= HI3520D_IOCH1_PHYS ? (x) + IO_OFFSET_HIGH \
: (x) + IO_OFFSET_MID)


6、./arch/arm/mach-hi3518/include/mach/io.h










/*========================================kj timer begin=====================================*/
kj_add_timer()
{
init_timer(&kj_timer1);

kj_timer.expires = jiffies + 100;
kj_timer.data = 1;
kj_timer.function = ledON;

add_timer(&kj_timer);
}


static void ledON(unsigned long data)
{
  int i = 100;
  //  kj add function




  kj_timer.data = data_kj+1;
kj_timer.expires = jiffies + i;
kj_timer.function = ledOFF;
add_timer(&kj_timer);
}




static void ledOFF(unsigned long data)
{
  int i = 300;
  //  kj add function




  kj_timer.data = data_kj+1;
kj_timer.expires = jiffies + i;
kj_timer.function = ledON;
add_timer(&kj_timer);
}


static void del_timer_kj(struct timer_list * timer)
{
del_timer(timer);
}
/*=========================================kj timer end====================================*/
#define GPIO_LIGHT_DAT (IO_ADDRESS(0x201A0000 + 0x03FC))
#define GPIO_LIGHT_DIR (IO_ADDRESS(0x201A0000 + 0x0400))
#define GPIO_LIGHT_MUX (IO_ADDRESS(0x200F0000 + 0x009C))




#define GPIO_1_1_DAT (IO_ADDRESS(0x201503FC))//red
#define GPIO_1_1_DIR (IO_ADDRESS(0x20150400))
#define GPIO_1_1_MUX (IO_ADDRESS(0x200F0004))


#define GPIO_6_0_DAT (IO_ADDRESS(0x201A03FC))//blue
#define GPIO_6_0_DIR   (IO_ADDRESS(0x201A0400))
#define GPIO_6_0_MUX   (IO_ADDRESS(0x200F0080))










/*alter by kj*/
/*
Author : kj
Time : 2014-04-19
Function : 
control gpio led
Instruction :
1 - red on
2 - red off
3 - blue on
4 - blue off
5 - light on
6 - light off


writel(0x2, REG_BASE_SCTL+REG_SC_SYSRES);
 */
static void joseph_set_gpio_hi3518c(int led_status)
{
switch(led_status)
{
case 1:
{
printk("\nThe uboot set red led on and blue led off begin alter by kj !!!\n");

writel(0x00000002,GPIO_1_1_DIR);
writel(0x00000000,GPIO_1_1_MUX);
writel(0x00000002,GPIO_1_1_DAT);// 1 - on ;0 - off


writel(0x00000001,GPIO_6_0_DIR);
writel(0x00000000,GPIO_6_0_MUX);
writel(0x00000000,GPIO_6_0_DAT);// blue off


break;  
}
case 2:
{
printk("\nThe uboot  red off light begin alter by kj !!!\n");

writel(0x00000002,GPIO_1_1_DIR);
writel(0x00000000,GPIO_1_1_MUX);
writel(0x00000000,GPIO_1_1_DAT);// red off


break;  
}
case 3:
{


printk("\nThe uboot set bule on begin alter by kj !!!\n");

writel(0x00000001,GPIO_6_0_DIR);
writel(0x00000000,GPIO_6_0_MUX);
writel(0x00000001,GPIO_6_0_DAT);//bule on
break;  
}
case 4:
{


printk("\nThe uboot set blue off begin alter by kj !!!\n");

writel(0x00000001,GPIO_6_0_DIR);
writel(0x00000000,GPIO_6_0_MUX);
writel(0x00000000,GPIO_6_0_DAT);//blue off
break;  
}
case 5:
{
printk("\nThe uboot set light on begin alter by kj !!!\n");

writel(0x00000080,GPIO_LIGHT_DIR);
writel(0x00000000,GPIO_LIGHT_MUX);
writel(0x00000000,GPIO_LIGHT_DAT);//light on

break;  
}
case 6:
{


printk("\nThe uboot set light off begin alter by kj !!!\n");

writel(0x00000080,GPIO_LIGHT_DIR);
writel(0x00000001,GPIO_LIGHT_MUX);
writel(0x00000000,GPIO_LIGHT_DAT);//light off


break;  
}
default:
break;
}






}












/*========================================joseph hi3518c feed wdg begin========================================*/


#define HISILICON_SCTL_BASE 0x20050000
/* define watchdog IO */
#define JOSEPH_HIWDT_BASE      0x20040000
#define JOSEPH_HIWDT_REG(x)    (JOSEPH_HIWDT_BASE + (x))


#define JOSEPH_HIWDT_LOAD      0x000
#define JOSEPH_HIWDT_VALUE     0x004
#define JOSEPH_HIWDT_CTRL      0x008
#define JOSEPH_HIWDT_INTCLR    0x00C
#define JOSEPH_HIWDT_RIS       0x010
#define JOSEPH_HIWDT_MIS       0x014
#define JOSEPH_HIWDT_LOCK      0xC00


#define JOSEPH_HIWDT_UNLOCK_VAL    0x1ACCE551


/* module param */
#define HIDOG_TIMER_MARGIN    1 //alter by kj
static DEFINE_SPINLOCK(hidog_lock);
static int cur_margin = HIDOG_TIMER_MARGIN;


#ifndef MHZ
#define MHZ (1000*1000)
#endif


static unsigned long rate = 3*MHZ;


static inline void hidog_set_timeout(unsigned int nr)
{
    unsigned long cnt = (~0x0)/rate;        /* max cnt */
    unsigned long flags; 


    spin_lock_irqsave(&hidog_lock, flags); 


    if( nr==0 || nr>cnt)
        cnt=~0x0; 
    else
        cnt = nr*rate;
    /* unlock watchdog registers */ 
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK); 
    hiwdt_writel(cnt, HIWDT_LOAD); 
    hiwdt_writel(cnt, HIWDT_VALUE); 
    /* lock watchdog registers */ 
    hiwdt_writel(0, HIWDT_LOCK); 
    spin_unlock_irqrestore(&hidog_lock, flags); 
};


static void joseph_start_wdg_reg(void)
{
    unsigned long flags;
    unsigned long t;

    spin_lock_irqsave(&hidog_lock, flags);
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    hiwdt_writel(0x00, HIWDT_CTRL);
    hiwdt_writel(0x00, HIWDT_INTCLR);
    hiwdt_writel(0x03, HIWDT_CTRL);
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    /* enable watchdog clock --- set the frequency to 3MHz */
    t = readl(reg_ctl_base_va);
    writel(t & ~0x00800000, reg_ctl_base_va);
    spin_unlock_irqrestore(&hidog_lock, flags);

}


























/*========================================joseph hi3518c feed wdg end========================================*/









