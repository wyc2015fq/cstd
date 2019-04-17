# 关于 mkimage - DoubleLi - 博客园






在嵌入式系统中，Linux内核和根文件系统一般都与bootloader一起烧写在flash芯片中，系统启动后，bootloader将Linux内核压缩到RAM中，并把压缩的根文件系统复制到RAM中，然后启动内核，由内核解压缩根文件系统，并调用文件系统中的应用程序和脚本文件启动系统。

在烧写Linux内核和根文件系统之前应该对它们进行适当处理，在它们的头部加入一些提供给bootloader的信息字符，以便bootloader能够辨认它们并获取它们的信息，从而正确地将它们解压缩、复制到RAM中的正确位置，并正确启动内核。这个为Linux内核和根文件系统添加头部的工作一般由编译bootloader时生成的一个小应用程序来完成，比如u-boot编译后会在其tools子目录下生成一个叫mkimage的应用程序，而cfe在编译后也会在其cfe/hosttools子目录下生成一个叫mkflashimage的应用程序，它们就是完成上述功能的工具程序，此处我们只介绍u-boot的mkimage。

mkimage命令格式如下：

# mkimage [-x] -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image

其中各字段含义如下：



|**字**段|**含**义|
|----|----|
|-x|指定内核就在Flash中运行，即XIP（execute in place）|
|-T|指定输入文件的类型，如“kernel”、“filesystem”、“firmware”、“ramdisk”|
|-A|指定处理器体系架构|
|-O|指定操作系统类型名字，如“linux”、“vxworks”、“psos”等|
|-C|指定压缩模式，可能设置为“none”、“bzip2”、“gzip”|
|-a|指定程序被解压缩或复制到RAM中的地址|
|-e|指定程序（内核）的入口地址|
|-n|指定image文件的名字|
|-d|指定输入的程序文件，可是有多个文件，如内核或/和根文件系统二进制文件|
|image|输出文件名|


mkimage命令举例如下：

#mkimage -n "My Kernel" -A arm -O linux -T kernel -C gzip –a 0x8000 –e 0x8000 -d kernel.gz kernel.img

以上命令将压缩了的内核二进制文件kernel.gz转换成u-boot能够辨认的二进制文件kernel.img，并指定kernel.img的名字为“My Kernel”，处理器体系架构为arm，操作系统类型为linux，程序类型为操作系统内核，程序由gzip压缩，程序的链接起始地址为0x8000，程序的入口地址为0x8000，注意这两个地址一定要是物理地址而不是对应的虚拟地址。

#mkimage -n "My Rootfs" -A arm -O linux -T ramdisk -C gzip -d ramdisk.gz ramdisk.img

以上命令将压缩了的ramdisk根文件系统二进制文件ramdisk.gz转换成u-boot能够辨认的二进制文件ramdisk.img，并指定ramdisk.img的名字为“My Rootfs”，处理器体系架构为arm，操作系统类型为linux，程序类型为ramdisk，程序由gzip压缩，不需要指定ramdisk的链接起始地址和入口地址。

如果我们将kernel.img和ramdisk.img文件分别写到flash芯片的0xFF000000和0xFF200000位置，系统启动后进入u-boot命令行界面，执行以下u-boot命令：

# bootm 0xFF000000 0xFF200000

那么u-boot将把0xFF000000位置的Linux内核解压缩到RAM中的0x8000位置，再将0xFF200000位置的ramdisk文件系统映像文件刨掉u-boot头部后复制到RAM中的某个位置，然后跳转到内核的入口地址0x8000位置启动内核，同时把板子信息、ramdisk在RAM中的起始地址和结束地址、命令行字符串传给内核，这样Linux开始启动运行。









