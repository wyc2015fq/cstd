# Linux下USB烧写uImage kernel - DoubleLi - 博客园






Linux下USB烧写uImage kernel



1.启动开发板，进入u-boot：（如果开发板中没有[系统](http://www.2cto.com/os/)，可以通过用SD卡方式启动开发板进入）



U-Boot 2011.06 (Mar 19 2012 - 08:45:34) for MINI6410



CPU: S3C6400@667MHz

Fclk = 667MHz, Hclk = 133MHz, Pclk = 66MHz (ASYNC Mode)

Board: MINI6410

DRAM: 256 MiB

NAND: 512 MiB

MMC: SAMSUNG SD/MMC: Host controler CH0

SD card is SD_VERSION_2

SD/MMC size : 3911680 block * 512B = 1910MB

Video: no Video params found, try bootargs~~ 640x480x24 31kHz 59Hz

[petworm] video_hw_init: <640, 480, 640, 480>

Error: Bad gzipped data

Error: no valid bmp or bmp.gz image at 57500000

In: serial

Out: serial

Err: serial

Net: dm9000

Hit any key to stop autoboot: 3



2.按任意键进入命令行状态后，运行dnw命令，文件存放地址作为参数传入（十六进制）：



MINI6410 # dnw 50008000

Insert a OTG cable into the connector!



3.插入USB电缆，连接主机和开发板，minicom会出现如下提示：



USB Cable is Connected

Now, Waiting for DNW to transmit data



4.另开一个终端，加载secbulk驱动：



sudo insmod secbulk



5.建立secbulk驱动设备节点(通过试验，可跳过此步)：



sudo mknod /dev/secbulk0 c 102 0



6.运行dnw程序，将欲传送的uImage的带路径的文件名作为参数传入:



sudo ./dnw uImage_a70 #(我们选择当前目录下的uImage_a70文件)



回车后传输开始，直至传送完毕。Minicom端提示如下：



Now, Waiting for DNW to transmit data

Download Done!! Download Address: 0x50008000, Download Filesize:0x40de8

Checksum is being calculated.

Checksum O.K.



主机端提示如下：



liwei@liwei:~/Desktop/dnw_linux/dnw$ sudo ./dnw uImage_a70

file name : uImage_a70

file size : 3763296 bytes

Writing data...

100% 3763306 bytes OK



此时，文件传送完毕，且校验正确。这时我们就可以用u-boot命令来将文件写入相应

的nandflash地址中：



MINI6410 # nand erase 80000 400000

NAND erase: device 0 offset 0x80000, size 0x400000

Erasing at 0x460000 -- 100% complete.

OK

MINI6410 # nand write 50008000 80000 400000

NAND write: device 0 offset 0x80000, size 0x400000

4194304 bytes written: OK



7.将S2调整到NAND启动位置，重启开发板后从NandFlash读取内核到内存，运行结

果如下：



U-Boot 2011.06 (Mar 16 2012 - 13:58:27) for MINI6410



CPU: S3C6400@667MHz

Fclk = 667MHz, Hclk = 133MHz, Pclk = 66MHz (ASYNC Mode)

Board: MINI6410

DRAM: 256 MiB

NAND: 512 MiB

MMC: SAMSUNG SD/MMC: Host controler CH0

No SD/MMC card detected!

No MMC card for env!

Using default environment

Video: no Video params found, try bootargs~~ 640x480x24 31kHz 59Hz

[petworm] video_hw_init: <640, 480, 640, 480>

No SD/MMC card detected!

Video: No MMC card for background image!

In: serial

Out: serial

Err: serial

Net: dm9000

Hit any key to stop autoboot: 0

MINI6410 # nand read 50008000 80000 400000

NAND read: device 0 offset 0x80000, size 0x400000

4194304 bytes read: OK



8.从内存中启动内核：



MINI6410 # bootm 50008000

## Booting kernel from Legacy Image at 50008000 ...

Image Name: petworm

Image Type: ARM [Linux](http://www.2cto.com/os/linux/) Kernel Image (uncompressed)

Data Size: 3763232 Bytes = 3.6 MiB

Load Address: 50008000

Entry Point: 50008040

Verifying Checksum ... OK

XIP Kernel Image ... OK

OK



Starting kernel ...



Uncompressing Linux... done, booting the kernel.

Initializing cgroup subsys cpu

Linux version 2.6.38-FriendlyARM (root@jensen) (gcc version 4.5.1 (ctng-1.8.1-F1

CPU: ARMv6-compatible processor [410fb766] revision 6 (ARMv7), cr=00c5387f

CPU: VIPT nonaliasing data cache, VIPT nonaliasing instruction cache

Machine: MINI6410

。。。 。。。

至此，Linux下用USB电缆烧写uImage成功完成！



附：

我们可以通过使用u-boot自带的mkimage命令来将zImage转换为uImage文件，命令如下：

mkimage -A arm -O linux -T kernel -C none -a 0x50008000 -e

0x50008040 -n "image-name" -d zImage_a70 uImage_a70

此命令将zImage_a70文件转换为uImage_a70文件，镜像的名称为image-name。









