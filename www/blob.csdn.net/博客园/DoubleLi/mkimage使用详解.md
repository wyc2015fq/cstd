# mkimage使用详解 - DoubleLi - 博客园






uboot源代码的tools/目录下有mkimage工具，这个工具可以用来制作不压缩或者压缩的多种可启动映象文件。

mkimage在制作映象文件的时候，是在原来的可执行映象文件的前面加上一个0x40字节的头，记录参数所指定的信息，这样uboot才能识别这个映象是针对哪个CPU体系结构的，哪个OS的，哪种类型，加载内存中的哪个位置， 入口点在内存的那个位置以及映象名是什么

root@Glym:/tftpboot# ./mkimage
Usage: ./mkimage -l image
-l ==> list image header information
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

-A 指定CPU的体系结构：

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

-O 指定操作系统类型，可以取以下值：
openbsd、netbsd、freebsd、4_4bsd、linux、svr4、esix、solaris、irix、sco、dell、ncr、lynxos、vxworks、psos、qnx、u-boot、rtems、artos

-T 指定映象类型，可以取以下值：
standalone、kernel、ramdisk、multi、firmware、script、filesystem

-C 指定映象压缩方式，可以取以下值：
none 不压缩
gzip 用gzip的压缩方式
bzip2 用bzip2的压缩方式

-a 指定映象在内存中的加载地址，映象下载到内存中时，要按照用mkimage制作映象时，这个参数所指定的地址值来下载

-e 指定映象运行的入口点地址，这个地址就是-a参数指定的值加上0x40（因为前面有个mkimage添加的0x40个字节的头）

-n 指定映象名

-d 指定制作映象的源文件



[U-BOOT下使用bootm引导内核方法](http://www.cnitblog.com/luofuchong/archive/2007/01/12/21834.html)



一、在开始之前先说明一下bootm相关的东西。

1、 首先说明一下，S3C2410架构下的bootm只对sdram中的内核镜像文件进行操作(好像AT91架构提供了一段从flash复制内核镜像的代码， 不过针对s3c2410架构就没有这段代码，虽然可以在u-boot下添加这段代码，不过好像这个用处不大)，所以请确保你的内核镜像下载到sdram 中，或者在bootcmd下把flash中的内核镜像复制到sdram中。

2、-a参数后是内核的运行地址，-e参数后是入口地址。

3、
1）如果我们没用mkimage对内核进行处理的话，那直接把内核下载到0x30008000再运行就行，内核会自解压运行（不过内核运行需要一个tag来传递参数，而这个tag建议是由bootloader提供的，在u-boot下默认是由bootm命令建立的）。

2）如果使用mkimage生成内核镜像文件的话，会在内核的前头加上了64byte的信息，供建立tag之用。bootm命令会首先判断bootm xxxx 这个指定的地址xxxx是否与-a指定的加载地址相同。
(1)如果不同的话会从这个地址开始提取出这个64byte的头部，对其进行分析，然后把去掉头部的内核复制到-a指定的load地址中去运行之
(2)如果相同的话那就让其原封不同的放在那，但-e指定的入口地址会推后64byte，以跳过这64byte的头部。


二、好，接着介绍使用mkimage生成镜像文件并下载运行的方法。

方法一、
1、首先，用u-boot/tools/mkimage这个工具为你的内核加上u-boot引导所需要的文件头，具体做法如下：
[root@localhost tftpboot]#mkimage -n 'linux-2.6.14' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008000 -d zImage zImage.img
Image Name:   linux-2.6.14
Created:      Fri Jan 12 17:14:50 2007
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    1262504 Bytes = 1232.91 kB = 1.20 MB
Load Address: 0x30008000
Entry Point:  0x30008000

这里解释一下参数的意义： 
        -A ==> set architecture to 'arch'
        -O ==> set operating system to 'os'
        -T ==> set image type to 'type'
        -C ==> set compression type 'comp'
        -a ==> set load address to 'addr' (hex)
        -e ==> set entry point to 'ep' (hex)
        -n ==> set image name to 'name'
        -d ==> use image data from 'datafile'
        -x ==> set XIP (execute in place)

2 、下载内核 
U-Boot 1.1.3 (Jan 12 2007 - 16:16:36)

U-Boot code: 33F80000 -> 33F9BAC0  BSS: -> 33F9FBAC
RAM Configuration:
Bank #0: 30000000 64 MB
Nor Flash: 512 kB
Nand Flash:  64 MB
In:    serial
Out:   serial
Err:   serial
Hit any key to stop autoboot:  0
sbc2410=>tftp 0x31000000 zImage.img         
TFTP from server 192.168.1.115; our IP address is 192.168.1.128
Filename 'zImage.img'.
Load address: 0x31000000
Loading: #################################################################
        #################################################################
        #################################################################
        ####################################################
done
Bytes transferred = 1263324 (1346dc hex) 

3.运行 

sbc2410=>bootm 0x31000000
## Booting image at 31000000 ...
  Image Name:   linun-2.6.14 
  Image Type:   ARM Linux Kernel Image (uncompressed)
  Data Size:    1263260 Bytes =  1.2 MB
  Load Address: 30008000
  Entry Point:  30008000
  Verifying Checksum ... OK
OK

Starting kernel ...

Uncompressing Linux.............................................................Linux version 2.6.14 (root@luofuchong) (gcc version 3.4.1) #21 Fri Oct 20 17:206CPU: ARM920Tid(wb) [41129200] revision 0 (ARMv4T)
Machine: SMDK2410
Memory policy: ECC disabled, Data cache writeback
CPU S3C2410A (id 0x32410002)
S3C2410: core 202.800 MHz, memory 101.400 MHz, peripheral 50.700 MHz
S3C2410 Clocks, (c) 2004 Simtec Electronics
CLOCK: Slow mode (1.500 MHz), fast, MPLL on, UPLL on
USB Control, (c) 2006 sbc2410
CPU0: D VIVT write-back cache
CPU0: I cache: 16384 bytes, associativity 64, 32 byte lines, 8 sets
CPU0: D cache: 16384 bytes, associativity 64, 32 byte lines, 8 sets
Built 1 zonelists
Kernel command line: console="ttySAC0" root="/dev/nfs" nfsroot="192".168.1.115:/frien"irq: clearing subpending status 00000002
PID hash table entries: 512 (order: 9, 8192 bytes)
timer tcon="00500000", tcnt a509, tcfg 00000200,00000000, usec 00001e4c
Console: colour dummy device 80x30
Dentry cache hash table entries: 16384 (order: 4, 65536 bytes)
Inode-cache hash table entries: 8192 (order: 3, 32768 bytes)
Memory: 64MB = 64MB total
Memory: 62208KB available (1924K code, 529K data, 108K init)
Mount-cache hash table entries: 512
CPU: Testing write buffer coherency: ok
softlockup thread 0 started up.
NET: Registered protocol family 16
S3C2410: Initialising architecture
SCSI subsystem initialized
usbcore: registered new driver usbfs
usbcore: registered new driver hub
S3C2410 DMA Driver, (c) 2003-2004 Simtec Electronics
DMA channel 0 at c4800000, irq 33
DMA channel 1 at c4800040, irq 34
DMA channel 2 at c4800080, irq 35
DMA channel 3 at c48000c0, irq 36
NetWinder Floating Point Emulator V0.97 (double precision)
devfs: 2004-01-31 Richard Gooch ([rgooch@atnf.csiro.au](mailto:rgooch@atnf.csiro.au))
devfs: devfs_debug: 0x0
devfs: boot_options: 0x1
yaffs Oct 18 2006 12:39:51 Installing.
Console: switching to colour frame buffer device 30x40
fb0: s3c2410fb frame buffer device
fb1: Virtual frame buffer device, using 1024K of video memory
led driver initialized
s3c2410 buttons successfully loaded
s3c2410_serial0 at MMIO 0x50000000 (irq = 70) is a S3C2410
s3c2410_serial1 at MMIO 0x50004000 (irq = 73) is a S3C2410
s3c2410_serial2 at MMIO 0x50008000 (irq = 76) is a S3C2410
io scheduler noop registered
io scheduler anticipatory registered
io scheduler deadline registered
io scheduler cfq registered
RAMDISK driver initialized: 16 RAM disks of 4096K size 1024 blocksize
usbcore: registered new driver ub
Cirrus Logic CS8900A driver for Linux (Modified for SMDK2410)
eth0: CS8900A rev E at 0xe0000300 irq="53", no eeprom , addr: 08: 0:3E:26:0A:5B
S3C24XX NAND Driver, (c) 2004 Simtec Electronics
s3c2410-nand: mapped registers at c4980000
s3c2410-nand: timing: Tacls 10ns, Twrph0 30ns, Twrph1 10ns
NAND device: Manufacturer ID: 0xec, Chip ID: 0x76 (Samsung NAND 64MiB 3,3V 8-bi)Scanning device for bad blocks
Bad eraseblock 1884 at 0x01d70000
Creating 4 MTD partitions on "NAND 64MiB 3,3V 8-bit":
0x00000000-0x00020000 : "vivi"
0x00020000-0x00030000 : "param"
0x00030000-0x00200000 : "kernel"
0x00200000-0x04000000 : "root"
usbmon: debugfs is not available
s3c2410-ohci s3c2410-ohci: S3C24XX OHCI
s3c2410-ohci s3c2410-ohci: new USB bus registered, assigned bus number 1
s3c2410-ohci s3c2410-ohci: irq 42, io mem 0x49000000
hub 1-0:1.0: USB hub found
hub 1-0:1.0: 2 ports detected
Initializing USB Mass Storage driver...
usbcore: registered new driver usb-storage
USB Mass Storage support registered.
usbcore: registered new driver usbmouse
drivers/usb/input/usbmouse.c: v1.6:USB HID Boot Protocol mouse driver
mice: PS/2 mouse device common for all mice
s3c2410 TouchScreen successfully loaded
UDA1341 audio driver initialized
NET: Registered protocol family 2
IP route cache hash table entries: 1024 (order: 0, 4096 bytes)
TCP established hash table entries: 4096 (order: 2, 16384 bytes)
TCP bind hash table entries: 4096 (order: 2, 16384 bytes)
TCP: Hash tables configured (established 4096 bind 4096)
TCP reno registered
TCP bic registered
NET: Registered protocol family 1
IP-Config: Complete:
     device=eth0, addr="192".168.1.128, mask="255".255.255.0, gw="192".168.1.1,
    host="luofuchong", domain=, nis-domain=(none),
    bootserver="192".168.1.1, rootserver="192".168.1.115, rootpath=
Looking up port of RPC 100003/2 on 192.168.1.115
Looking up port of RPC 100005/1 on 192.168.1.115
VFS: Mounted root (nfs filesystem).
Mounted devfs on /dev
Freeing init memory: 108K
init started:  BusyBox v1.1.3 (2006.09.20-14:52+0000) multi-call binary
Starting pid 696, console /dev/tts/0: '/etc/init.d/rcS'

Please press Enter to activate this console.



方法二、
1、首先，用u-boot/tools/mkimage这个工具为你的内核加上u-boot引导所需要的文件头，具体做法如下：
[root@localhost tftpboot]#mkimage -n 'linux-2.6.14' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008040 -d zImage zImage.img
Image Name:   linux-2.6.14
Created:      Fri Jan 12 17:14:50 2007
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    1262504 Bytes = 1232.91 kB = 1.20 MB
Load Address: 0x30008000
Entry Point:  0x30008040




2 、下载内核 
U-Boot 1.1.3 (Jan 12 2007 - 16:16:36)

U-Boot code: 33F80000 -> 33F9BAC0  BSS: -> 33F9FBAC
RAM Configuration:
Bank #0: 30000000 64 MB
Nor Flash: 512 kB
Nand Flash:  64 MB
In:    serial
Out:   serial
Err:   serial
Hit any key to stop autoboot:  0
sbc2410=>tftp 0x30008000 zImage.img         
TFTP from server 192.168.1.115; our IP address is 192.168.1.128
Filename 'zImage.img'.
Load address: 0x30008000
Loading: #################################################################
        #################################################################
        #################################################################
        ####################################################
done
Bytes transferred = 1263324 (1346dc hex) 

3.运行 

sbc2410=>bootm 0x30008000
## Booting image at 30008000 ...
   Image Name:   linux-2.6.14
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1261056 Bytes =  1.2 MB
   Load Address: 30008000
   Entry Point:  30008040
   Verifying Checksum ... OK
   XIP Kernel Image ... OK









