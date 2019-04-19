# 移植Linux-3.0.8到s5pv210 - xqhrs232的专栏 - CSDN博客
2017年03月18日 23:53:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：668
原文地址::[http://www.51hei.com/bbs/dpj-37249-1.html](http://www.51hei.com/bbs/dpj-37249-1.html)
一、 基本内核移植
1. 解压内核源码包
tar jxvf android_kernel_2.6.35_smdkv210.tar.bz2
cd android-kernel-samsung-dev/
2. 修改Makefile中的体系结构ARCH和交叉编译器前缀CROSS_COMPILE
vim Makefile
修改191和192行为如下：
ARCH            ?= arm
CROSS_COMPILE   ?= /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-
3. 使用SMDKV210的缺省配置文件
make  smdkv210_android_defconfig
4. 配置内核，修改串口
make menuconfig
修改底层消息和底层调试串行端口为UART0：
System Type  --->
(0) S3C UART to use for low-level messages
Kernel hacking  --->
(0) S3C UART to use for low-level debug
5. 确定机器码
vim arch/arm/tools/mach-types
由于友善提供的内核时mini210的，第一步先修改u-boot for tiny210的机器码，使之与内核机器码匹配。
从图上可以看出，mini210评估板的机器码是3466（16进制是0xd8a）：
![](http://c.51hei.com/a/a/b/57141754832131.jpg)
修改include/configs/tiny210.h
#define MACH_TYPE_TINY210 3466
6. 确定内核的加载地址和参数地址
vim arch/arm/mach-s5pv210/Makefile.boot
![](http://c.51hei.com/a/a/b/57141754858531.jpg)
可以看出，内核的加载地址和参数地址分别为0x20008000和0x20000100，bootloader启动内核前应该将内核拷贝到0x20008000，并将参数放到0x20000100。
7. 编译内核
make uImage -j 2
-j 2指定了编译时的线程数，使用2个线程可加快编译的速度，编译完成后可以看到：
OBJCOPY arch/arm/boot/Image
  Kernel: arch/arm/boot/Image is ready
  AS      arch/arm/boot/compressed/head.o
  GZIP    arch/arm/boot/compressed/piggy.gzip
  AS      arch/arm/boot/compressed/piggy.gzip.o
  CC      arch/arm/boot/compressed/misc.o
  CC      arch/arm/boot/compressed/decompress.o
  SHIPPED arch/arm/boot/compressed/lib1funcs.S
  AS      arch/arm/boot/compressed/lib1funcs.o
  LD      arch/arm/boot/compressed/vmlinux
  OBJCOPY arch/arm/boot/zImage
  Kernel: arch/arm/boot/zImage is ready
  UIMAGE  arch/arm/boot/uImage
Image Name:   Linux-2.6.35.7
Created:      Mon Mar  4 17:18:54 2013
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    2667648 Bytes = 2605.12 kB = 2.54 MB
Load Address: 20008000
Entry Point:  20008000
  Image arch/arm/boot/uImage is ready
可以看出，内核的加载地址的确是0x20008000，进入点地址也是0x20008000，u-boot支持的Linux内核映像uImage也可用了，它在arch/arm/boot目录。
8. 设置u-boot的环境变量
现在我们需要在u-boot下设置一些环境变量，其中bootargs是引导参数行，machid是机器码，0xd8a即3466，即mini210评估板，最后输入saveenv保存环境变量到Nand
 Flash：
mini210# setenv bootargs 'console=ttySAC0,115200 root=/dev/mtdblock4 rootfstype=yaffs rw'
mini210# setenv machid 0xd8a
mini210# saveenv
Saving Environment to SMDK bootable device...
Erasing Nand...
Writing to Nand...
Saved enviroment variables
在u-boot下输入bdinfo可以查看一些开发板的信息，可以看到boot_params引导参数的地址是0x20000100，这个地址和arch/arm/mach-s5pv210/Makefile.boot中的params_phys-y的值是一致的。内存DRAM的起始地址是0x20000000，大小是0x20000000，共521M。arch_number是机器码为d8a。
![](http://c.51hei.com/a/a/b/57141764816012.jpg)
9. 下载内核
![](http://c.51hei.com/a/a/b/57141764884814.jpg)
10. 引导内核
![](http://c.51hei.com/a/a/b/5714176481630.jpg)
![](http://c.51hei.com/a/a/b/5714176485134.jpg)
中间省略N多信息。。。。。。。。。。。。。。。
![](http://c.51hei.com/a/a/b/5714176482325.jpg)
出现一连串英文就Ok了吧，由于没有添加文件系统，所以到最后提示重启
![](http://c.51hei.com/a/a/b/57141774843874.jpg)
完成！！
11、uboot自动引导内核
    下面是自引导的实现：
    u-boot# nand write 0x21000000 0x600000 0x496d58
u-boot# setenv bootcmd nand read 0x21000000 0x600000 0x496d58\;bootm 0x21000000
u-boot# saveenv
然后重启板子就Ok
移植问题总结：
用uboot下载并尝试运行，结果是悲剧的，在uboot打印”Starting kernel...“之后就看不到任何输出了，显然，有两种原因：
 
     a. 编译的内核类型错误，比如您的uboot使用的是uImage，而您编译的是zImage。
              b. 移植的uboot存在问题，没能正确的拷贝内核到正确的内存地址并启动。
              c. 内核存在某些配置，我们没有配置。
                经过查看内核配置项知道，内核默认采用的UART1打印调试信息，因此，执行：
[html] view
 plaincopy
- make menuconfig  
        在出来的对话中依次选择System type => (1) S3C UART to use for low-level messages，在编辑框中将1改为0，然后保存配置并再次编译内核，直接执行make即可，无需make clean。
编译完内核之后再次下载并尝试运行，这时，可以看到内核打印的信息了，如果您不幸只看到“Uncompressing Linux... done, booting the kernel.”就没有任何输出了，那么请检查uboot传入的机器码跟内核机器码是否匹配，如果不匹配请修正，然后重新编译运行内核或者uboot，如果修正之后仍然看不到其他输出，那么请检查uboot的bootargs参数，bootargs中必须配置console=ttySAC0，否则也看不到打印信息。设置bootargs无需修改源码，只需在uboot运行期间添加环境变量bootargs即可，例子如上。
