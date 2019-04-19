# uboot传入的linux启动参数解析----命令行 - xqhrs232的专栏 - CSDN博客
2012年02月24日 11:16:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：597
原文地址::[http://blogt.chinaunix.net/link.php?url=http://blog.csdn.net%2Flanmanck%2Farchive%2F2009%2F05%2F15%2F4187389.aspx](http://blogt.chinaunix.net/link.php?url=http://blog.csdn.net%2Flanmanck%2Farchive%2F2009%2F05%2F15%2F4187389.aspx)
Bootloader与内核的交互 
Bootloader与内核的交互是单向的，Bootloader将各类参数传给内核。由于它们不能同时运行，传递办法只有一个：Bootloader将参数放在某个约定的地方之后，再启动内核，内核启动后从这个地方获得参数。
除了约定好参数存放的地址外，还要规定参数的结构。Linux 2.4.x 以后的内核都期望以标记列表(tagged list)的形式来传递启动参数。标记，就是一种数据结构；标记列表，就是挨着存放的多个标记。标记列表以标记ATAG_CORE 开始，以标记ATAG_NONE 结束。标记的数据结构为tag，它由一个tag_header结构和一个联合(union)组成。tag_header结构表示标记的类型及长度，比如是表示内存还是表示命令行参数等。对于不同类型的标记使用不同的联合(union)，比如表示内存时使用tag_mem32，表示命令行时使用
 tag_cmdline。数据结构tag和tag_header定义在Linux内核源码的include/asm/setup.h头文件中： 
struct tag_header { 
u32 size; 
u32 tag; 
}; 
<br>struct tag { 
struct tag_header hdr; 
union { 
struct tag_corecore; 
struct tag_mem32mem; 
struct tag_videotextvideotext; 
struct tag_ramdiskramdisk; 
struct tag_initrdinitrd; 
struct tag_serialnrserialnr; 
struct tag_revisionrevision; 
struct tag_videolfbvideolfb; 
struct tag_cmdlinecmdline; 
<br>/* 
 * Acorn specific
 */
struct tag_acornacorn; 
<br>/* 
 * DC21285 specific
 */
struct tag_memclkmemclk; 
} u; 
}; 
下面以设置内存标记、命令行标记为例说明参数的传递： 
（1）设置标记 ATAG_CORE。 
标记列表以标记 ATAG_CORE开始，假设Bootloader与内核约定的参数存放地址为0x30000100，则可以以如下代码设置标记 ATAG_CORE：
params = (struct tag *) 0x30000100; 
<br>params->hdr.tag = ATAG_CORE; 
params->hdr.size = tag_size (tag_core); 
<br>params->u.core.flags = 0; 
params->u.core.pagesize = 0; 
params->u.core.rootdev = 0; 
<br>params = tag_next (params); 其中，tag_next定义如下，它指向当前标记的末尾： 
#define tag_next(t)((struct tag *)((u32 *)(t) + (t)->hdr.size)) 
（2）设置内存标记。 
假设开发板使用的内存起始地址为0x30000000，大小为0x4000000，则内存标记可以如下设置： 
params->hdr.tag = ATAG_MEM; 
params->hdr.size = tag_size (tag_mem32); 
params->u.mem.start = 0x30000000; 
params->u.mem.size = 0x4000000; 
params = tag_next (params); 
（3）设置命令行标记。 
命令行就是一个字符串，它被用来控制内核的一些行为。比如"root=/dev/mtdblock2 init="/linuxrc" console="ttySAC0""表示根文件系统在MTD2分区上，系统启动后执行的第一个程序为/linuxrc，控制台为ttySAC0(即第一个串口)。
命令行可以在Bootloader中通过命令设置好，然后如下构造标记传给内核： 
char *p = "root=/dev/mtdblock2 init="/linuxrc" console="ttySAC0""; 
params->hdr.tag = ATAG_CMDLINE; 
params->hdr.size = (sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2; 
strcpy (params->u.cmdline.cmdline, p); 
params = tag_next (params); 
（4）设置标记ATAG_NONE。 
标记列表以标记ATAG_NONE结束，如下设置： 
params->hdr.tag = ATAG_NONE; 
params->hdr.size = 0; 
