# 嵌入式 hi3518c裸板uboot烧写、kernel烧写、fs烧写小结 - DoubleLi - 博客园






1、在uboot中我可以添加自己的命令，添加的方法是找到一个uboot的命令，然后模仿着去增加属于自己的命令代码以及实现函数就可以

2、记住在使用printf进行调试的时候，在遇到指针或者字符串的时候最好使用“%x”，以为我不知道指针或者字符串中是否包含不可见字符，如果有不可见字符会导致错误，而且错误不好查找

3、对于uboot中的环境变量，其实是放在uboot里面的，也就是环境变量占用的是uboot的空间，如果不需要去实时修改环境变量的值那么就可以不用env这个分区；但是如果需要修改环境变量，那么首先需要知道saveenv的起始地址和大小，来防止env的保存会覆盖flash中其他的有用数据。具体运行saveenv就可以看到起始地址和env大小

4、有时候在烧写完成kernel的时候直接第一次reset是可以启动的，但是当断电然后再一次上电的时候可能就会出现错误，例如：

Resetting CPU ...

这就有可能是内核不完整了，说明前面的操作可能覆盖了内核的部分内容导致的

5、对flash（spi）串行flash进行分区的时候需要特别注意：

   1）：uboot对flash分区可能存在明显的代码分区列表可以直接进行修改

   2）：uboot中没有这个明显的代码的时候，可能是在uboot的env中传过去的

setenv bootargs mem=64Mconsole=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(config),3072k(kernel),11264k(rootfs)

具体是哪种方式可以去uboot源码中进行查找函数的入口参数的传入点

6、分区的具体信息需要根据uboot的大小（每一分区的大小最小是一个扇区，包括kernel和rootfs）

0x000000000000-0x000000040000 :"boot"
0x000000040000-0x000000060000 : "env"
0x000000060000-0x000000080000 : "conf"
0x000000080000-0x000000380000 : "kernel"
0x000000380000-0x000000e80000 : "rootfs"

7、uboot对flash分区具有最大上限一般为（32）



在使用jlink烧写之前必须对cpu进行初始化，然后进去normal模式，接着进行内存初始化，然后才能烧写

hi3518c初始化命令：


setmem 0x20030000 0x11000000 32
setmem 0x20030004 0x0068306E 32
setmem 0x20030008 0x12000000 32
setmem 0x2003000c 0x007C2063 32
setmem 0x20030010 0x19000000 32
setmem 0x20030014 0x00682064 32
setmem 0x20030020 0x1B000000 32
setmem 0x20030024 0x007C40E1 32
setmem 0x20030028 0x00000010 32
setmem 0x2011001c 0x80000600 32



setmem 0x20030028 0x00000410 32
setmem 0x2011001c 0x80000600 32




setmem 0x20050014 0x0fff8000 32
setmem 0x2011001c 0x80000600 32






setmem 0x20050000 0x00000214 32
setmem 0x2011001c 0x80000600 32



setmem 0x2011001c 0x80000600 32
setmem 0x20110020 0x00000785 32
setmem 0x20110058 0x3240a000 32
setmem 0x20110010 0x00000001 32
setmem 0x2011002c 0x00000122 32
setmem 0x20110040 0x80000000 32
setmem 0x20110050 0x62330a08 32
setmem 0x20110054 0xff626720 32
setmem 0x2011005c 0xffcff5f2 32
setmem 0x201100ac 0x03000001 32
setmem 0x2011001c 0x80000600 32



setmem 0x201100f4 0x00000001 32
setmem 0x20110004 0x00000000 32
setmem 0x2011001c 0x80000600 32





setmem 0x20120404 0x80000000 32
setmem 0x2011001c 0x80000600 32


setmem 0x20120408 0x01842200 32
setmem 0x20120418 0x00220055 32
setmem 0x2012041c 0x048b0d41 32
setmem 0x20120420 0x03233881 32
setmem 0x2012042c 0xf200001e 32
setmem 0x20120434 0x288f6693 32
setmem 0x20120438 0x0040009b 32
setmem 0x2012043c 0x1001aa00 32
setmem 0x20120584 0x0000006d 32
setmem 0x20120440 0x00001730 32
setmem 0x20120444 0x00000006 32
setmem 0x20120448 0x00000008 32
setmem 0x2011001c 0x80000600 32




setmem 0x201205c0 0x00000e81 32
setmem 0x20120600 0x00000e81 32
setmem 0x2011001c 0x80000600 32



setmem 0x20120404 0x000000ff 32
setmem 0x2011001c 0x80000600 32


setmem 0x20110058 0x3240a062 32
setmem 0x20120408 0x01842200 32



注：其中每一空白行的前一行setmem 0x2011001c 0x8000060032是为了实现wait操作



1、在裸板上使用jlink进行烧写uboot的需要知道uboot在内存中的运行地址也就是代码段的入口，可以去uboot源码下找到uboot.map进行查找text入口。

2、然后在串口中进行loadb 0x82000000（uboot中也是可以找到定义的），然后sfwrite到flash但是需要先擦除后烧写

3、有时候在烧写uboot之后saveenv后reset找不到内核，并提示wrong format image这可能是内核的头没了，原因是saveenv的时候覆盖了内核的文件，所以需要去uboot中找到类似下面的代码：


#defineCFG_ENV_ADDR   0x80000

#defineCFG_ENV_SIZE   0x2000

然后对flash分区进行重新计算

4、对flash分区3.0内核和uboot 2006中是使用环境变量进行划分的，类似代码如下：

setenv bootargs 'mem=64M console=ttyAMA0,115200root=/dev/mtdblock4 rootfstype=jffs2mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),2944k(os),12800k(rootfs)'
注：speed 1000 后使用ＡＸＤ进行cpu和ram 初始化

sf probe 0;mw.b 0x82000000 ff 0x80000;

烧录uboot：
loadb 0x82000000
 sf probe 0;sf erase 0x0 0x80000;sf write0x82000000 0x0 0x80000



烧录内核:
    sf probe0;mw.b 82000000 ff 300000;loadb 0x82000000;sf erase c0000 300000;sfwrite 82000000 c0000 300000
  //mw.b表示按字节显示  mw: 对RAM中的内容写操作，mw 32000000 ff 10000(把内存0x32000000开始的0x10000字节设为0xFF)


烧录文件系统:
    sf probe 0;mw.b 82000000 ff C80000 ;loadb 0x82000000;sf erase 3c0000 C80000;sf write 82000000 3c0000 C80000


官方uboot_env:

bootargs=mem=64M console=ttyAMA0,115200
bootcmd=bootm 0x82000000
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
ver=U-Boot 2010.06 (Dec 12 2012 - 13:16:58)

Environment size: 294/262140 bytes



hi3518_qy;

sf probe 0;mw.b 0x82000000 ff 0x80000;

烧录uboot：
loadb 0x82000000
 sf probe 0;sf erase 0x0 0x80000;sf write0x82000000 0x0 0x80000

烧录内核:
    sf probe0;mw.b 82000000 ff 300000;loadb 0x82000000;sf erase c0000 300000;sfwrite 82000000 c0000 300000
  //mw.b表示按字节显示  mw: 对RAM中的内容写操作，mw 32000000 ff 10000(把内存0x32000000开始的0x10000字节设为0xFF)

烧录文件系统:
    sf probe 0;mw.b 82000000 ff C80000 ;loadb 0x82000000;sf erase 3c0000 C80000;sf write 82000000 3c0000 C80000

setenv bootargs 'mem=64M console=ttyAMA0,115200root=/dev/mtdblock4 rootfstype=jffs2mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),2944k(os),12800k(rootfs)'


内核：
setenv bootcmd sf probe 0\;sf read 0x82000000 0xc00000x300000\;bootm 0x82000000

剩余：
256k未使用









