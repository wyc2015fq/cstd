# zImgage,uImage 区别 - xqhrs232的专栏 - CSDN博客
2011年03月24日 11:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：928标签：[linux																[compression																[linux内核																[image																[freebsd																[hex](https://so.csdn.net/so/search/s.do?q=hex&t=blog)](https://so.csdn.net/so/search/s.do?q=freebsd&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=compression&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/lanmanck/archive/2009/06/17/4276738.aspx](http://blog.csdn.net/lanmanck/archive/2009/06/17/4276738.aspx)
对于Linux内核，编译可以生成不同格式的映像文件，例如：
# make zImage
# make uImage
    zImage是ARM Linux常用的一种压缩映像文件，uImage是U-boot专用的映像文件，它是在zImage之前加上一个长度为0x40的“头”，说明这个映像文件的类型、加载位置、生成时间、大小等信息。换句话说，如果直接从uImage的0x40位置开始执行，zImage和uImage没有任何区别。另外，Linux2.4内核不支持uImage，Linux2.6内核加入了很多对嵌入式系统的支持，但是uImage的生成也需要设置。
一、vmlinuz 
vmlinuz是可引导的、压缩的内核。“vm”代表“Virtual Memory”。Linux 支持虚拟内存，不像老的操作系统比如DOS有640KB内存的限制。Linux能够使用硬盘空间作为虚拟内存，因此得名“vm”。
vmlinuz 的建立有两种方式。一是编译内核时通过“make zImage”创建，然后通过:“cp /usr/src/linux-2.4/arch/i386/linux/boot/zImage/boot/vmlinuz”产生。zImage适用于小内核的情况，它的存在是为了向后的兼容性。
二是内核编译时通过命令make bzImage创建，然后通过:“cp/usr/src/linux-2.4/arch/i386/linux/boot/bzImage /boot/vmlinuz”产生。bzImage是压缩的内核映像，需要注意，bzImage不是用bzip2压缩的，bzImage中的bz容易引起误解，bz表示“big zImage”。 bzImage中的b是“big”意思。 zImage(vmlinuz)和bzImage(vmlinuz)都是用gzip压缩的。它们不仅是一个压缩文件，而且在这两个文件的开头部分内嵌有
 gzip解压缩代码。所以你不能用gunzip 或 gzip –dc解包vmlinuz。 
二、initrd-x.x.x.img 
initrd是“initial ramdisk”的简写。initrd一般被用来临时的引导硬件到实际内核vmlinuz能够接管并继续引导的状态。 
initrd 映象文件是使用mkinitrd创建的。mkinitrd实用程序能够创建initrd映象文件。这个命令是RedHat专有的。其它Linux发行版或许有相应的命令。这是个很方便的实用程序。具体情况请看帮助:man mkinitrd下面的命令创建initrd映象文件。
最后生成的内核镜象有两种 zImage 以及 uImage 。其中 zImage 下载到目标板中后，可以直接用 uboot 的命令 go 来进行直接跳转。这时候内核直接解压启动。但是无法挂载文件系统，因为 go 命令没有将内核需要的相关的启动参数传递给内核。传递启动参数我们必须使用命令 bootm 来进行跳转。 Bootm 命令跳转只处理 uImage 的镜象。
uboot 源代码的 tools/ 目录下有 mkimage 工具，这个工具可以用来制作不压缩或者压缩的多种可启动映象文件。 
mkimage 在制作映象文件的时候，是在原来的可执行映象文件的前面加上一个 0x40 字节的头，记录参数所指定的信息，这样 uboot 才能识别这个映象是针对哪个 CPU 体系结构的，哪个 OS 的，哪种类型，加载内存中的哪个位置， 入口点在内存的那个位置以及映象名是什么
用法如下： 
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
-A 指定 CPU 的体系结构： 
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
-O 指定操作系统类型，可以取以下值： 
openbsd 、 netbsd 、 freebsd 、 4_4bsd 、 linux 、 svr4 、 esix 、 solaris 、 irix 、 sco 、 dell 、 ncr 、 lynxos 、 vxworks 、 psos 、 qnx 、 u-boot 、 rtems 、 artos
-T 指定映象类型，可以取以下值： 
standalone 、 kernel 、 ramdisk 、 multi 、 firmware 、 script 、 filesystem 
-C 指定映象压缩方式，可以取以下值： 
none 不压缩 
gzip 用 gzip 的压缩方式 
bzip2 用 bzip2 的压缩方式 
-a 指定映象在内存中的加载地址，映象下载到内存中时，要按照用 mkimage 制作映象时，这个参数所指定的地址值来下载 
-e 指定映象运行的入口点地址，这个地址就是 -a 参数指定的值加上 0x40 （因为前面有个 mkimage 添加的 0x40 个字节的头） 
-n 指定映象名 
-d 指定制作映象的源文件 
我在编译时用到的命令如下： 
# make zImage      // 生成 zImage 镜象 
＃ /usr/local/arm/k9uboot/tools/ mkimage -n 'Linux 2.4.27' -A arm -O linux -T 
kernel -C none -a 0x20007fc0 -e 0x20008000 -d zImage uImage 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/lanmanck/archive/2009/06/17/4276738.aspx](http://blog.csdn.net/lanmanck/archive/2009/06/17/4276738.aspx)

