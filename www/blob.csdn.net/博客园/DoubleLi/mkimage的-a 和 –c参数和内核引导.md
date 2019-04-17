# mkimage的-a 和 –c参数和内核引导 - DoubleLi - 博客园






## 目录

一、mkimage工具简介
二、-a参数与-e参数和内核引导的关系
三、实例测试
- 3.1 -a参数与-e参数相同，可以将内核下载到SDRAM的任何地址，然后从这启动
- 3.2 -a参数与-e参数不同，只能将内核下载到-a参数后面的地址，然后从这启动；否则无法引导内核
- 3.3 -a参数与-e参数不同，下载地址和-a参数后面的地址不同（无法引导）
- 3.4 覆盖了U-Boot传给内核的参数，会导致这样情形（无法引导）


### 一、mkimage工具简介

U-Boot源代码的tools/目录下有mkimage工具，这个工具可以用来制作不压缩或者压缩的多种可启动映象文件。mkimage在制作映象文件的时候，是在原来的可执行映象文件的前面加上一个0x40字节的头，记录参数所指定的信息，这样U-Boot才能识别这个映象是针对哪个CPU体系结构的，哪个OS的，哪种类型，加载内存中的哪个位置， 入口点在内存的那个位置以及映象名是什么

root@www.linuxidc.com:~/桌面# ./mkimage
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

**-A 指定CPU的体系结构：**

取值                     表示的体系结构
alpha                       Alpha 
arm                           ARM 
x86                        Intel x86
ia64                          IA64
mips                        MIPS
mips64               MIPS 64 Bit
ppc                        PowerPC
s390                     IBM S390
sh                          SuperH
sparc                   SPARC
sparc64          SPARC 64 Bit
m68k                  MC68000

**-O 指定操作系统类型：**

可以取以下值：
openbsd、netbsd、freebsd、4_4bsd、linux、svr4、esix、solaris、irix、sco、dell、ncr、lynxos、vxworks、psos、qnx、u-boot、rtems、artos

**-T 指定映象类型：**

可以取以下值：
standalone、kernel、ramdisk、multi、firmware、script、filesystem

**-C 指定映象压缩方式：**

可以取以下值：
none 不压缩
gzip 用gzip的压缩方式
bzip2 用bzip2的压缩方式

**-a 指定映象在内存中的加载地址**

映象下载到内存中时，要按照用mkimage制作映象时，这个参数所指定的地址值来下载

**-e 指定映象运行的入口点地址**

这个地址就是-a参数指定的值加上0x40（因为前面有个mkimage添加的0x40个字节的头）

**-n 指定映象名**

**-d 指定制作映象的源文件**



### 二、-a参数与-e参数和内核引导的关系






- -a参数后是内核的运行地址，-e参数后是入口地址。
- 如果我们没用mkimage对内核进行处理的话，那直接把内核下载到0x30008000再运行就行，内核会自解压运行（不过内核运行需要一个tag来传递参数，而这个tag建议是由BootLoader提供的，在U-Boot下默认是由bootm命令建立的）。
- 如果使用mkimage生成内核镜像文件的话，会在内核的前头加上了64byte的信息，供建立tag之用。bootm命令会首先判断bootm xxxx 这个指定的地址xxxx是否与-a指定的加载地址相同。如果不同的话会从这个地址开始提取出这个64byte的头部，对其进行分析，然后把去掉头部的内核复制到-a指定的load地址中去运行之。如果相同的话那就让其原封不同的放在那，但-e指定的入口地址会推后64byte，以跳过这64byte的头部。

### 三、实例测试

#### 3.1 -a参数与-e参数相同，可以将内核下载到SDRAM的任何地址，然后从这启动

zImage.img的制作
*****************
root@www.linuxidc.com:~/桌面# ./mkimage -n 'linux-2.6.30' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008000 -d zImage zImage.img
Image Name:   linux-2.6.30
Created:      Sat Mar  3 23:41:49 2012
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    1907440 Bytes = 1862.73 kB = 1.82 MB
Load Address: 0x30008000
Entry Point:  0x30008000
root@www.linuxidc.com:~/桌面# 
Linux引导时的输出
*******************
czu2440 # tftp 0x30000000 zImage.img
TFTP from server 192.168.1.1; our IP address is 192.168.1.5
Filename 'zImage.img'.
Load address: 0x30000000
Loading: #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         ################################################
done
Bytes transferred = 1907504 (1d1b30 hex)
czu2440 # bootm 0x30000000
## Booting image at 30000000 ...
   Image Name:   linux-2.6.30
   Created:      2012-03-03  15:41:49 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1907440 Bytes =  1.8 MB
   Load Address: 30008000
   Entry Point:  30008000
   Verifying Checksum ... OK
OK


Starting kernel ...


Uncompressing Linux.............................................................
............................................................. done, booting the
kernel.

**注意：“Starting kernel ...”是U-Boot在引导内核前输出的信息，后面输出的信息就和U-Boot的没有关系了。**

#### 3.2 -a参数与-e参数不同，只能将内核下载到-a参数后面的地址，然后从这启动；否则无法引导内核

zImage.img的制作
*****************

root@www.linuxidc.com:~/桌面# ./mkimage -n 'linux-2.6.30' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008040 -d zImage zImage.img
Image Name:   linux-2.6.30
Created:      Sun Mar  4 00:18:05 2012
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    1907440 Bytes = 1862.73 kB = 1.82 MB
Load Address: 0x30008000
Entry Point:  0x30008040
root@www.linuxidc.com:~/桌面# 
Linux引导时的输出
*******************
czu2440 # tftp 0x30008000 zImage.img
TFTP from server 192.168.1.1; our IP address is 192.168.1.5
Filename 'zImage.img'.
Load address: 0x30008000
Loading: #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         ################################################
done
Bytes transferred = 1907504 (1d1b30 hex)
czu2440 # bootm 0x30008000
## Booting image at 30008000 ...
   Image Name:   linux-2.6.30
   Created:      2012-03-03  16:18:05 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1907440 Bytes =  1.8 MB
   Load Address: 30008000
   Entry Point:  30008040
   Verifying Checksum ... OK
   XIP Kernel Image ... OK


Starting kernel ...


Uncompressing Linux.............................................................
............................................................. done, booting the
kernel.


#### 3.3 -a参数与-e参数不同，下载地址和-a参数后面的地址不同（无法引导）


czu2440 # tftp 0x30009000 zImage.img
TFTP from server 192.168.1.1; our IP address is 192.168.1.5
Filename 'zImage.img'.
Load address: 0x30009000 
Loading: #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         ################################################
done
Bytes transferred = 1907504 (1d1b30 hex)
czu2440 # bootm 0x30009000 
## Booting image at 0x30009000 ...
   Image Name:   linux-2.6.30
   Created:      2012-03-03  16:18:05 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1907440 Bytes =  1.8 MB
   Load Address: 30008000
   Entry Point:  30008040
   Verifying Checksum ... OK
OK


Starting kernel ...
=============================================================

#### 3.4 覆盖了U-Boot传给内核的参数，会导致这样情形（无法引导）


czu2440 # tftp 0x30000000 zImage.img
TFTP from server 192.168.1.1; our IP address is 192.168.1.5
Filename 'zImage.img'.
Load address: 0x30000000
Loading: #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         ################################################
done
Bytes transferred = 1907504 (1d1b30 hex)
czu2440 # bootm 0x30000000
## Booting image at 30000000 ...
   Image Name:   linux-2.6.30
   Created:      2012-03-03  16:18:05 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1907440 Bytes =  1.8 MB
   Load Address: 30008000
   Entry Point:  30008040
   Verifying Checksum ... OK
OK


Starting kernel ...









