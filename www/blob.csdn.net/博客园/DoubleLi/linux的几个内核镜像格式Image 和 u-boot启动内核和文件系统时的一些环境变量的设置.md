# linux的几个内核镜像格式Image 和 u-boot启动内核和文件系统时的一些环境变量的设置 - DoubleLi - 博客园






关于编译powerpc linux的几个Image
参考原文 http://blog.sina.com.cn/s/blog_86a30b0c0100wfzt.html

![](http://image73.360doc.com/DownloadImg/2014/06/0816/42418398_3.jpg)转载▼




PowerPC架构 Linux和ARM,X86等平台有些差异，PowerPC平台HW参数不是通过命令行方式传递到Linux，而是通过传递Device tree文件的方式传递参数，所以PowerPC平台Linux需要编译dtb和uImage才能正常加载，另外PowerPC架构linux还提供simpleImage的方式加载，也就是将dtb与uImage或将dtb,uImage与ramdisk.image.gz多个文件整合生产单个simpleImage文件，这种simpleImage文件加载方式适合于没有bootloader支持的情况下，通过JTAG将simpleImage直接加载到target board内存中运行，对于调试非常方便，下面说说这几种Image文件的编译:


1，dtb:



dtb文件由dts生成，对于任何一个PowerPC处理器板，都要有对应的dts文件，dts文件主要是对目标板的HW参数进行描述，比如我的目标板是evm440(Powerpc440)，我编写了一个evm440.dts文件放到内核arch/powerpc/boot/dts/目录下.

编译evm440.dtb文件命令如下:

$ make ARCH=powerpc CROSS_COMPILE=/toolchain/freescale-2010.09/bin/powerpc-linux-gnu- evm440.dtb

编译成功后会在arch/powerpc/boot/下生成evm440.dtb文件,这就是我们需要的dtb文件.



2. uImage:

这个Image应该是地球都知道的，这里就不多说了，编译命令如下:


$ make ARCH=powerpc CROSS_COMPILE=/toolchain/freescale-2010.09/bin/powerpc-linux-gnu- uImage





编译成功后会在arch/powerpc/boot/下生成uImage文件,这就是我们需要的内核Image文件.



3. simpleImage.xxxx和simpleImage.initrd.xxxx:

这里的xxxx是特定目标板名字，我的目标板是evm440，也就是simpleImage.evm440和simpleImage.initrd.evm440。

simpleImage.evm440是包括evm440.dtb与uImage以及simpleboot的单一Image，可以不需要bootloader支持加载.


simpleImage.initrd.evm440是包括evm440.dtb，ramdisk.image.gz与uImage以及simpleboot的单一Image，也是可以不需要bootloader支持加载.

编译命令如下:




$ make ARCH=powerpc CROSS_COMPILE=/usr/toolchain/freescale-2010.09/bin/powerpc-linux-gnu- simpleImage.evm440



$ make ARCH=powerpc CROSS_COMPILE=/usr/toolchain/freescale-2010.09/bin/powerpc-linux-gnu- simpleImage.initrd.evm440



编译成功后会在arch/powerpc/boot/下生成:

simpleImage.evm440

simpleImage.evm440.elf

simpleImage.initrd.evm440

simpleImage.initrd.evm440.elf

这些命令格式，另外包括cuImage，在Document/powerpc/bootwrape文件中有说明。

附
1. 在修改内核配置时，要在arch/powerpc/boot/dts/目录下使用新的设备树文件（通常参考相似的设备树文件），如p2020stm1.dts；
2. 修改扳级文件，在arch/powerpc/platform/85xx/目录下（通常参考相似文件），如 cp mpc85xx_rdb.c p2020_stm1.c,并修改好这个目录下的Makefile 和Kconfig文件；
3. 内核根目录下.config可以参考arch/powepc/configs/目录下的配置，如mpc85xx_smp_defconfig；
4. 编译 make cuImage.p2020stm1。 cuImage 对应命令格式， p2020stm1 对应设备树文件。为了使命令中的p2020stm1能找到对应的CPU，还需要修改文件arch/powerpc/boot/wraper文件，添加"|*-p2020*"内容。
194  *-mpc85*|*    ..... 
195  platformo=$object/cuboot-85xx.o 

经过这四个过程基本OK

[转]Linux内核zImage\Image\uImage之区别及uImage的制作
http://zh5202.blog.163.com/blog/static/178025195201210535458755/

一、Image、zImage和uImage的区别

内核编译（make）之后会生成两个文件，一个是Image，一个是zImage，其中Image为内核映像文件，而zImage为内核的一种映像压缩文件，Image大约为4M，而zImage不到2M。uImage是uboot专用的映像文件，它是在zImage之前加上一个长度为64字节的“头”，说明这个内核的版本、加载位置、生成时间、大小等信息；其0x40之后与zImage没什么区别。uImage的64字节的头结构如下：

typedef struct image_header{

     uint32_tih_magic;

     uint32_tih_hcrc;

     uint32_tih_time;

     uint32_tih_size;

     uint32_tih_load;

     uint32_tih_ep;

     uint32_tih_dcrc;

     uint32_tih_os;

     uint32_tih_arch;

     uint32_tih_type;

     uint32_tih_comp;

     uint32_tih_name[IH_NMLEN];

}image_header_t;

       所以uImage和zImage都是压缩后的内核映像，而uImage是uboot专用的映像文件，是用mkimage工具根据zImage制作而来的。



二、uImage的制作

(编译uboot源码，编译成功后载uboot/tools目录下会生成mkimgage工具，将mkimage工具拷贝到/usr/bin/目录下即可。)

mkimage工具

内核编译完后zImage在内核根目录下，mkimage工具在u-boot根目录下的tools文件夹中，将zImage和mkimage放在同一个目录下，执行：

./mkimage -A arm -O linux -T kernel -C none -a 30008000 -e 30008000 -n linux-2.6.26 -d zImage uImage2.6.26

就将zImage转换成uImage了。

【mkimage的命令参数：】

-A :==>set architecture to 'arch’       ：用于指定CPU类型，比如arm、x86、mips、ppc

-O :==>set operating system to 'os’     ：用于指定操作系统，比如freebsd、linux、vxworks

-T :==>set image type to 'type’        ：用于指定image类型，比如kernel、ramdisk、filesystem

-C :==>set compression type 'comp’    ：用于指定压缩类型：none、gzip、bzip2

-a :==>set load address to 'addr’(hex)   ：用于指定uImage的加载地址

-e :==>set entry point to 'ep’(hex)      ：用于指定内核的入口地址，一般是：uImage的载入地址+0x40（信息头的大小）

-n :==>set image name to 'name’       ：用于指定uImage在头结构中的命名

-d :==>use image data from 'datafile’    ：用于指定无头信息的zImage文件名

-x :==>set XIP(execute in place)        ：用于设置执行位置

如对于ARM Linux内核映像常用选项为：

-A arm          :==> 架构是arm

-O linux         :==> 操作系统是linux

-T kernel         :==> 类型是kernel

-C none/bzip/gzip  :==> 压缩类型

-a 0x40008000    :==> uImage的载入地址（hex），通常为0xX0008000

-e 0x40008040    :==> 内核的入口地址（hex），XX为0x40或者0x00

-n linux-xxx      :==> uImage的名字，任意

-d namexxx       :==> 无头信息的zImage文件名，为源内核文件

uImagexxx       :==> 加了头信息之后的zImage文件名，任意

【例子：】

./mkimage -A arm -O linux -T kernel -C none -a 30008000 -e 30008040 -d zImage uImage



【注意：】

       -a和-e后面跟的分别是image的载入地址和内核的入口地址，两者可以一样，也可以不一样，依据如下情况而定：

当-a后面指定的地址和bootm xxxx后面的地址一样时，-e后面的地址必须要比-a后面的地址多0x40，也就是映像头的大小64个字节。因为当他们地址一样时，uboot是不会搬运映像的；

当-a后面指定的地址和bootm xxxx后面的地址不一样时，uboot会将bootm xxxx地址处的映像文件搬运到-a指定的地址处，此时，-e和-a必须要一样，因为映像头并没有搬运过去，载入地址就是内核的入口地址。需要注意的是，因为uboot要重新搬运内核映像，所以要注意bootm xxxx的地址和-a之间的地址不要导致复制时的覆盖。

P2020项目中，还看见看了使用这命令制作根文件系统镜像
mkimage -A ppc -O linux -T ramdisk -C gzip -n ${rooot_ver} -d rootfs.img.gz rootfs.uboot 




## [转]uImage、zImage、bzImage、vlinzx区别http://hi.baidu.com/cbncb/item/389b155c62acd316abf6d748

在网络中，不少服务器采用的是Linux系统。为了进一步提高服务器的性能，可能需要根 据特定的硬件及需求重新编译Linux内核。编译Linux 内核，需要根据规定的步骤进行，编译内核过程中涉及到几个重要的文件。比如对于RedHat Linux，在/boot目录下有一些与Linux内核有关的文件 .
　　编译过RedHat Linux内核的人对其中的System.map、vmlinuz、initrd-2.4.7-10.img印象可能比较深刻，因为编译内核过程中涉及到这些文件的建立等操作。那么这几个文件是怎么产生的?又有什么作用呢?



对于Linux内核，编译可以生成不同格式的映像文件，例如：
# make zImage
# make uImage
# make bzImage
zImage是ARM Linux常用的一种压缩映像文件不能超过512KB，bzImage 即bigzImage ，二者的内核都是gzip压缩的
uImage是U-boot专用的映像文件，它是在zImage之前加上一个长度为0x40的“头”，说明这个映像文件的类型、加载位置、生成时间、大小等信息。换句话说，如果直接从uImage的0x40位置开始执行，zImage和uImage没有任何区别。另外，Linux2.4内核不支持uImage，Linux2.6内核加入了很多对嵌入式系统的支持，但是uImage的生成也需要设置。

　　一、vmlinuz

　　vmlinuz是可引导的、压缩的内核。“vm”代表“Virtual Memory”。Linux 支持虚拟内存，不像老的操作系统比如DOS有640KB内存的限制。Linux能够使用硬盘空间作为虚拟内存，因此得名“vm”。vmlinuz是可执行 的Linux内核，它位于/boot/vmlinuz，它一般是一个软链接，比如图中是vmlinuz-2.4.7-10的软链接。

vmlinuz的建立有两种方式。

一是编译内核时通过“make zImage”创建，手动拷贝到/boot目录下面。zImage适用于小内核的情况，它的存在是为了向后的兼容性。

　　二是内核编译时通过命令make bzImage创建，然后手动拷贝至/boot目录下。bzImage是压缩的内核映像，需要注意，bzImage不是用bzip2压缩的,bz表示“big zImage”。 bzImage中的b是“big”意思。 zImage(vmlinuz)和bzImage(vmlinuz)都是用gzip压缩的。它们不仅是一个压缩文件，而且在这两个文件的开头部分内嵌有gzip解压缩代码。所以你不能用gunzip或gzip –dc解包vmlinuz。

内核文件中包含一个微型的gzip用于解压缩内核并引导它。两者的不同之处在于，老的zImage解压缩内核到低端内存(第一个 640K)，bzImage解压缩内核到高端内存(1M以上)。如果内核比较小，那么可以采用zImage或bzImage之一，两种方式引导的系统运行 时是相同的。大的内核采用bzImage，不能采用zImage。

vmlinux是未压缩的内核，vmlinuz是vmlinux的压缩文件。

　　二、initrd-x.x.x.img

　　initrd是“initial ramdisk”的简写。initrd一般被用来临时的引导硬件到实际内核vmlinuz能够接管并继续引导的状态。图中的initrd-2.4.7-10.img主要是用于加载ext3等文件系统及scsi设备的驱动。

　　比如，使用的是scsi硬盘，而内核vmlinuz中并没有这个scsi硬件的驱动，scsi模块是存储在根文件系统的/lib/modules下的，那么在装入scsi模块之前，内核不能加载根文件系统。为了解决这个问题，可以引导一个能够读实际内核的initrd内核并用initrd修正 scsi引导问题。initrd-2.4.7-10.img是用gzip压缩的文件，initrd实现加载一些模块和安装文件系统等功能。

initrd映象文件是使用mkinitrd创建的。mkinitrd实用程序能够创建initrd映象文件。这个命令是RedHat专有的(这也是为什么，在Linux内核包里/Documentation/Changes里面没有提到要将mkinitrd升级）。其它Linux发行版或许有相应的命令。这是个很方便的实用程序。具体情况请看帮助:man mkinitrd下面的命令创建initrd映象文件。

initrd映象文件是使用mkinitrd创建的。mkinitrd实用程序能够创建initrd映象文件。这个命令是RedHat专有的。其它Linux发行版或许有相应的命令。这是个很方便的实用程序。具体情况请看帮助:man mkinitrd下面的命令创建initrd映象文件。



 　　三、uImage文件

vmlinux是内核文件，zImage是一般情况下默认的压缩内核映像文件，压缩vmlinux，加上一段解压启动代码得到。而uImage是u-boot使用bootm命令引导的Linux压缩内核映像文件格式,是使用工具mkimage对普通的压缩内核映像文件（zImage）加工而得。它是uboot专用的映像文件，它是在zImage之前加上一个长度为 64字节的“头”，说明这个内核的版本、加载位置、生成时间、大小等信息；其0x40之后与zImage没区别。

由于bootloader一般要占用0X0地址，所以，uImage相比zImage的好处就是可以和bootloader共存。

其实就是一个自动跟手动的区别,有了uImage头部的描述,u-boot就知道对应Image的信息,如果没有头部则需要自己手动去搞那些参数。

如何生成uImage文件？首先 在uboot的/tools目录下寻找mkimage文件，把其copy到系统/usr/local/bin目录下，这样就完成制作工具。然后在内核目录下运行make uImage，如果成功，便可以在arch/arm/boot/目录下发现uImage文件，其大小比zImage多64个字节。

此外，平时调试用uImage，不用去管调整了哪些东西；zImage则是一切OK后直接烧0X0。开机就运行。



### u-boot启动内核和文件系统时的一些环境变量的设置http://wk2325272.blog.163.com/blog/static/1017654482011101731148777/

1．设置MAC 地址：

U-Boot>setenv ethaddr 3e:36:65:ba:6f:be

2．设置IP 地址：

U-Boot>setenv ipaddr 192.168.0.2（好像不行，有新进展在贴出来）



3.设置装载内核和文件系统的信息：


setenv bootcmd cp.b 0x00030000 0x21500000 0x00200000\;cp.b 0x00300000 0x21100000 0x00400000\;bootm 0x21500000   // 0x00030000表示内核在NAND FLASH中的起始地址，0x21500000表示装载到SDRAM中的位置，        //0x00200000表示内核文件的大小

4.设置读内核和文件系统的信息：



set ker nand read 21500000 30000 200000   //200000为内核大小

set fs nand read 21100000 300000 400000   //400000为fs大小

5.设置启动信息：

set bootcmd run boot

set boot run fs\;run ker\;bootm 21500000

set bootargs mem=64M console=ttySAC0,115200 initrd=0x21100000,4M root=/dev/ram0 rw

6.保存环境变量：

saveenv

注意：3到6从u-boot启动内核和根文件下系统时必须要设置的环境变量。















