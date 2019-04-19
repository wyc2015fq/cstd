# 基于MTD的NAND驱动开发(二) - maopig的专栏 - CSDN博客
2012年03月07日 09:15:54[maopig](https://me.csdn.net/maopig)阅读数：570标签：[struct																[硬件驱动																[flash																[table																[module																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=硬件驱动&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
### [基于MTD的NAND驱动开发(二)](http://blog.csdn.net/leibniz_zsu/article/details/4977650)
### [基于MTD的NAND驱动开发(三)](http://blog.csdn.net/leibniz_zsu/article/details/4977853)
### [http://blog.csdn.net/leibniz_zsu/article/details/4977853](http://blog.csdn.net/leibniz_zsu/article/details/4977853)
### [http://blog.csdn.net/leibniz_zsu/article/details/4977869](http://blog.csdn.net/leibniz_zsu/article/details/4977869)
**四、基于MTD的NAND 驱动架构**
**1 、platform_device 和platform_driver 的定义和注册**
对于我们的NAND driver ，以下是一个典型的例子：
```
static
struct platform_driver caorr_nand_driver
= { 
               . driver 
= { 
                                . name 
= " caorr-nand" 
, 
                                . owner 
= THIS_MODULE, 
                } 
, 
                . probe 
= caorr_nand_probe, 
                . 
remove = caorr_nand_remove,
 } ; 
 static int __init caorr_nand_init(
void ) 
 { 
                printk( 
"CAORR NAND Driver, (c) 2008-2009./n" )
; 
                return platform_driver_register(
& caorr_nand_driver)
; 
 } 
 static void __exit caorr_nand_exit(
void ) 
 { 
                platform_driver_unregister( 
& caorr_nand_driver) 
; 
 } 
 module_init( caorr_nand_init)
; 
 module_exit( caorr_nand_exit)
;
```
与大多数嵌入式Linux 驱动一样，NAND 驱动也是从module_init 宏开始。caorr_nand_init 是驱动初始化函数，在此函数中注册platform driver 结构体，platform driver 结构体中自然需要定义probe
 和remove 函数。其实在大多数嵌入式Linux 驱动中，这样的套路基本已经成了一个定式
至于module_init 有什么作用，caorr_nand_probe 又是何时调用的，以及这个driver 是怎么和NAND
 设备联系起来的，就不再多说了，这里只提三点：
A、 以上代码只是向内核注册了NAND 的platform_driver ，即caorr_nand_driver
 ，我们当然还需要一个NAND 的platform_device ，要不然caorr_nand_driver 的probe 函数就永远不会被执行，因为没有device
 需要这个driver 。
B、 向Linux 内核注册NAND 的platform_device
 有两种方式：
其一是直接定义一个NAND 的platform_device 结构体，然后调用platform_device_register 函数注册。作为例子，我们可以这样定义NAND
 的platform_device 结构体：
```
struct platform_device caorr_nand_device
= { 
          . name =
"caorr-nand" ,
          . id =
- 1, 
          . num_resources 
= 0, 
          . resource 
= NULL ,
          . dev =
{ 
              . platform_data 
= & caorr_platform_default_nand,
          } 
 } ; 
 platform_device_register( 
& caorr_nand_device) 
;
```
其中num_resources 和resource 与具体的硬件相关，主要包括一些寄存器地址范围和中断的定义。caorr_platform_default_nand
 待会儿再说。需要注意的是，这个platform_device 中name 的值必须与platform_driver->driver->name 的值完全一致，因为platform_bus_type
 的match 函数是根据这两者的name 值来进行匹配的。
其二是用platform_device_alloc 函数动态分配一个platform_device ，然后再用platform_device_add
 函数把这个platform_device 加入到内核中去。具体不再细说，Linux 内核中有很多例子可以参考。
相对来说，第一种方式更加方便和直观一点，而第二种方式则更加灵活一点。
C、 在加载NAND 驱动时，我们还需要向MTD Core 提供一个信息，那就是NAND
 的分区信息，caorr_platform_default_nand 主要就是起这个作用，更加详细的容后再说。
**2 、MTD 架构的简单描述**
MTD(memory technology device 存储技术设备) 是用于访问memory 设备（ROM
 、flash ）的Linux 的子系统。MTD 的主要目的是为了使新的memory 设备的驱动更加简单，为此它在硬件和上层之间提供了一个抽象的接口。MTD
 的所有源代码在/drivers/mtd 子目录下。MTD 设备可分为四层（从设备节点直到底层硬件驱动），这四层从上到下依次是：设备节点、MTD 设备层、MTD 原始设备层和硬件驱动层。
![](http://blogimg.chinaunix.net/blog/upfile2/090618184650.jpg)
A、Flash硬件驱动层：硬件驱动层负责驱动Flash硬件。
B、MTD原始设备：原始设备层有两部分组成，一部分是MTD原始设备的通用代码，另一部分是各个特定的Flash的数据，例如分区。
用于描述MTD原始设备的数据结构是mtd_info，这其中定义了大量的关于MTD的数据和操作函数。mtd_table（mtdcore.c）则是所有MTD原始设备的列表，mtd_part（mtd_part.c）是用于表示MTD原始设备分区的结构，其中包含了mtd_info，因为每一个分区都是被看成一个MTD原始设备加在mtd_table中的，mtd_part.mtd_info中的大部分数据都从该分区的主分区mtd_part->master中获得。
在drivers/mtd/maps/子目录下存放的是特定的flash的数据，每一个文件都描述了一块板子上的flash。其中调用add_mtd_device()、del_mtd_device()建立/删除 mtd_info结构并将其加入/删除mtd_table（或者调用add_mtd_partition()、del_mtd_partition() （mtdpart.c）建立/删除mtd_part结构并将mtd_part.mtd_info加入/删除mtd_table
 中）。
C、MTD设备层：基于MTD原始设备，linux系统可以定义出MTD的块设备（主设备号31）和字符设备（设备号90）。MTD字符设备的定义在mtdchar.c中实现，通过注册一系列file operation函数（lseek、open、close、read、write）。MTD块设备则是定义了一个描述MTD块设备的结构 mtdblk_dev，并声明了一个名为mtdblks的指针数组，这数组中的每一个mtdblk_dev和mtd_table中的每一个
 mtd_info一一对应。
D、设备节点：通过mknod在/dev子目录下建立MTD字符设备节点（主设备号为90）和MTD块设备节点（主设备号为31），通过访问此设备节点即可访问MTD字符设备和块设备。
E、根文件系统：在Bootloader中将JFFS（或JFFS2）的文件系统映像jffs.image（或jffs2.img）烧到flash的某一个分区中，在/arch/arm/mach-your/arch.c文件的 your_fixup函数中将该分区作为根文件系统挂载。
F、文件系统：内核启动后，通过mount 命令可以将flash中的其余分区作为文件系统挂载到mountpoint上。
以上是从网上找到的一些资料，我只是断断续续地看过一些code，没有系统地研究过，所以这里只能讲一下MTD原始设备层与FLASH硬件驱动之间的交互。
一个MTD原始设备可以通过mtd_part分割成数个MTD原始设备注册进mtd_table，mtd_table中的每个MTD原始设备都可以被注册成一个MTD设备，有两个函数可以完成这个工作，即 add_mtd_device函数和add_mtd_partitions函数。
其中add_mtd_device函数是把整个NAND FLASH注册进MTD Core，而add_mtd_partitions函数则是把NAND FLASH的各个分区分别注册进MTD Core。
add_mtd_partitions函数的原型是：
```
int add_mtd_partitions(
struct mtd_info * master,
```
```
const
struct mtd_partition 
* parts, int nbparts)
;
```
其中master就是这个MTD原始设备，parts即NAND的分区信息，nbparts指有几个分区。那么parts和nbparts怎么来？caorr_platform_default_nand 就是起这个作用了。
```
static
struct mtd_partition caorr_platform_default_nand[
] = 
{ 
    [ 0] 
= { 
               . name 
= "Boot Strap" 
, 
               . offset 
= 0, 
               . size 
= 0x40000, 
    } , 
    [ 1] 
= { 
               . name 
= "Bootloader" 
, 
               . offset 
= MTDPART_OFS_APPEND, 
               . size 
= 0x40000, 
    } , 
    [ 2] 
= { 
               . name 
= "Partition Table" 
, 
               . offset 
= MTDPART_OFS_APPEND, 
               . size 
= 0x40000, 
    } , 
    [ 3] 
= { 
               . name 
= "Linux Kernel" 
, 
               . offset 
= MTDPART_OFS_APPEND, 
               . size 
= 0x500000, 
    } , 
    [ 4] 
= { 
               . name 
= "Rootfs" 
, 
               . offset 
= MTDPART_OFS_APPEND, 
               . size 
= MTDPART_SIZ_FULL, 
    } , 
 } ;
```
其中offset是分区开始的偏移地址，在后4个分区我们设为 MTDPART_OFS_APPEND，表示紧接着上一个分区，MTD Core会自动计算和处理分区地址；size是分区的大小，在最后一个分区我们设为MTDPART_SIZ_FULL，表示这个NADN剩下的所有部分。
这样配置NAND的分区并不是唯一的，需要视具体的系统而定，我们可以在kernel中这样显式的指定，也可以使用bootloader传给内核的参数进行配置。
另外，MTD对NAND芯片的读写主要分三部分：
A、struct mtd_info中的读写函数，如read，write_oob等，这是MTD原始设备层与FLASH硬件层之间的接口；
B、struct nand_ecc_ctrl中的读写函数，如read_page_raw，write_page等，主要用来做一些与ecc有关的操作；
C、struct nand_chip中的读写函数，如read_buf，cmdfunc等，与具体的NAND controller相关，就是这部分函数与硬件交互，通常需要我们自己来实现。(注：这里提到的read，write_oob，cmdfunc等，其实都是些函数指针，所以这里所说的函数，是指这些函数指针所指向的函数，以后本文将不再另做说明。)
值得一提的是，struct nand_chip中的读写函数虽然与具体的NAND controller相关，但是MTD也为我们提供了default的读写函数，如果你的NAND controller比较通用(使用PIO模式)，对NAND芯片的读写与MTD提供的这些函数一致，就不必自己实现这些函数了。
这三部分读写函数是相互配合着完成对NAND芯片的读写的。首先，MTD上层需要读写NAND芯片时，会调用struct mtd_info中的读写函数，接着struct mtd_info中的读写函数就会调用struct nand_chip或struct nand_ecc_ctrl中的读写函数，最后，若调用的是struct nand_ecc_ctrl中的读写函数，那么它又会接着调用struct
 nand_chip中的读写函数。如下图所示：
![](http://blogimg.chinaunix.net/blog/upfile2/090618185723.jpg)
以读NAND芯片为例，讲解一下这三部分读写函数的工作过程。
首先，MTD上层会调用struct mtd_info中的读page函数，即nand_read函数。
接着nand_read函数会调用struct nand_chip中cmdfunc函数，这个cmdfunc函数与具体的NAND controller相关，它的作用是使NAND controller向NAND 芯片发出读命令，NAND芯片收到命令后，就会做好准备等待NAND controller下一步的读取。
接着nand_read函数又会调用struct nand_ecc_ctrl中的read_page函数，而read_page函数又会调用struct nand_chip中read_buf函数，从而真正把NAND芯片中的数据读取到buffer中(所以这个read_buf的意思其实应该是read into buffer，另外，这个buffer是struct mtd_info中的nand_read函数传下来的)。
read_buf函数返回后，read_page函数就会对buffer中的数据做一些处理，比如校验ecc，以及若数据有错，就根据ecc对数据修正之类的，最后read_page函数返回到nand_read函数中。
对NAND芯片的其它操作，如写，擦除等，都与读操作类似。

