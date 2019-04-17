# U-boot mkimage指定Linux内核地址时的两种方式 - DoubleLi - 博客园







uImage的制作是使用的u-boot工具mkimage，build完u-boot后也会将mkimage build出來到/tools目录下，可以直接拿來用，它的作用就是在zImage的前面加上64个字节的头，让u-boot能够识别要加载内核的类型、加载地址等。

基本格式：mkimage -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image

**-A 指定CPU的体系结构：（u-boot支持多种的架构，这里应该是arm）**取值        表示的体系结构
alpha       Alpha 
arm          A RM 
x86          Intel x86
ia64         IA64
mips         MIPS
mips64     MIPS 64 Bit
ppc          PowerPC
s390        IBM S390
sh            SuperH
sparc       SPARC
sparc64   SPARC 64 Bit
m68k       MC68000


**-O 指定操作系统类型，可以取以下值：（u-boot支持多种的OS，这里应该是linux）**openbsd、netbsd、freebsd、4_4bsd、linux、svr4、esix、solaris、irix、sco、dell、ncr、lynxos、vxworks、psos、qnx、u-boot、rtems、artos


**-T 指定映象类型，可以取以下值：（u-boot可以加载不同的映像，这里应该是kernel）**standalone、kernel、ramdisk、multi、firmware、script、filesystem


**-C 指定映象压缩方式，可以取以下值：（除了zImage本身是压缩格式外，u-boot还可以对其再压缩，这里应该是none）**none 不压缩
gzip 用gzip的压缩方式
bzip2 用bzip2的压缩方式


**-a 指定映象在内存中的加载地址，可以于实际加载（如tftp加载）时的地址不相同，也可以相同，详见后面的解释。**

**-e 指定映像运行的入口地址，可以等于加载地址，也可以是加载地址偏移64字节（+0x40）后的地址，详见后面的解释。**

**-n 指定映像名称**

**-d 指定【源文件】和生成的【目标文件名**】****


调整的最多的参数是-a， -e，这两个参数需要与实际加载到内存时的动作保持一致。


1、如果加载时的地址与-a参数指定的不相同，则-e必须等于-a。u-boot会把kernel image去掉不包括64字节的信息头后搬移到-e处，直接启动。要注意两个image的存储空间不要重叠，两者之间至少要留image size的空间。

编译环境：

mkimage -A arm -O linux -T kernel -C none -a 0x70008000 -e 0x70008000 -n 'Linux' -d zImage uImage
        cp uImage /tftpboot/        

目标板环境：

tftp   0x72200000  uImage;   bootm   0x72200000;


2、如果加载时的地址与-a参数指定相同，则-e必须是-a + 0x40的地址。这时u-boot不需要再执行搬移的动作，根据-e的地址直接启动。 

        编译环境：

        mkimage -A arm -O linux -T kernel -C none -a 0x0x70008000 -e 0x70008040 -n 'Linux‘ -d zImage uImage

        cp uImage /tftpboot/           

        目标板环境：


        tftp   0x70008000  uImage;    bootm  0x70008000;













