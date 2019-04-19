# DM365视频处理流程/DM368 NAND Flash启动揭秘 - maopig的专栏 - CSDN博客
2011年12月01日 10:51:44[maopig](https://me.csdn.net/maopig)阅读数：6113
   DM365的视频处理涉及到三个相关处理器，分别是视频采集芯片、ARM处理器和视频图像协处理器（VICP），整个处理流程由ARM核协调。视频处理主要涉及三个处理流程，分别是视频采集、视频编码和对编码后的视频的处理，为了提高性能，通常为每个处理流程提供一个处理线程。
视频采集
  TVP5146将采集到的视频数据转化为数字信号，并将这些数据送入DM365的BT656接口，然后通过Resize得到所需要的分辨率，然后将这些数据写入到指定的内存中，这些内存空间由cmem模块分配。cmem模块用于分配连续的存储空间，连续的存储空间可以提高数据的读写效率。
视频编码
   VICP从指定的存储空间中读入视频数据，并将这些数据编码为指定的视频/图像格式，然后将编码后的数据写入到指定的存储空间，这些存储空间通常也是由cmem模块分配。
编码数据处理
  ARM从指定的内存空间中获取编码后的视频数据，可以将这些编码后的数据保存到本地或远端，也可以使用RTP协议发送到网络。
整个过程由ARM核协调，为了提高效率，大量使用DMA操作和cmem模块分配的连续存储空间。
作者：德州仪器现场技术支持工程师 孟海燕
概要：
本文介绍了DM368 NAND Flash启动的原理，并且以DM368 IPNC参考设计软件为例，介绍软件是如何配合硬件实现启动的。
关键字：NAND Flash启动，RBL，UBL 
芯片上电后是如何启动实现应用功能的？这是许多工程师在看到处理器运行的时候，通常都会问的一个问题。下面我们就以德州仪器的多媒体处理芯片TMS320DM368为例，介绍它的NAND Flash启动原理以及实现。
一．NAND Flash启动原理
德州仪器的多媒体处理芯片TMS320DM368可以实现1080P30 h264的编码，已经广泛的使用在了网络摄像机的应用中。DM368可以支持NOR Flash,
NAND Flash, UART,
SD Card启动等多种启动方式。对于NAND启动，DM365支持的特性如下：
1.   不支持一次性全部固件下载启动。相反的，需要使用从NAND flash把第二级启动代码（UBL）复制到ARM的内存（AIM），将控制转交给用户定义的UBL。
2.   支持最大4KB页大小的NAND。 
3.   支持特殊数字标志的错误检测，在加载UBL的时候会尝试最多24次。例如在NAND的第1个block没有找到特殊数字标志，会到下一个block继续查找，一直到查找到第24个block。
4.   支持30KB大小的UBL（DM365有32KB的内存，其中2KB用作了RBL的堆栈，剩下的空间可以放UBL） 
5.   用户可以选择在RBL执行的时候是否需要支持DMA，I-cache（例如，加载UBL的时候）
6.   使用并且需要4位硬件ECC（支持每512字节需要ECC位数小于或等于4位的NAND Flash）。 
7.   支持需要片选信号在Tr读时间为低电平的NAND Flash。
在网络网络摄像机的应用中为了节约成本，有一些用户使用了NAND Flash启动方式。图1 就是从上电到Linux启动的一个概要的流程图。首先RBL（ROM boot loader）从NAND上读取UBL（user boot loader）并且复制到ARM的内存里面。UBL运行在ARM的内存里，初始化系统，例如初始化DDR。然后UBL从NAND
 Flash里面读取U-Boot的内容并且复制到DDR里运行。DDR里面运行的U-Boot又从NAND Flash里面读取Linux内核代码，并且复制到DDR上，然后启动内核。这样DM365的系统就从上电到完成Linux内核启动，然后就可以运行相应的应用程序了。
![](http://hi.csdn.net/attachment/201112/1/0_1322707767VC61.gif)
图1 NAND Flash启动流程
下面我们会一步一步的介绍从上电到Linux启动是如何实现的。 
首先我们需要提到的一个概念是RBL，也就是ROM Boot Loader （ROM启动代码）。在DM368芯片上有一块ROM的区域（地址从0x00008000到 0x0000 BFFF），这块区域就是存放RBL代码的地方。ROM上的代码是在芯片出厂前就烧写好的，用户是不能修改的。在DM368上，除了AEMIF
 （Nor Flash）启动，其他的启动方式都需要运行RBL。
无论是上电复位，热复位，还是看门狗复位，在复位信号由低到高的时候，DM368芯片会检测BTSEL[2:0]引脚（启动选择引脚）。只要检测到电平不是001，也就是不是AEMIF （NOR Flash）启动，ARM程序就会从ARM的ROM的地址0x00008000地址开始执行。
RBL首先会读取BOOTCFG寄存器里面的BTSEL信息，如果发现BTSEL的状态是000，就会得知配置的是NAND Flash启动，NAND启动模式开始执行。注意：为了保证NAND启动正常运行，需要保证在复位的时候DEEPSLEEPZ/GIO0引脚拉高。在确认启动是NAND后，首先RBL会初始化最高2KB的内存为堆栈并且关闭所以中断。然后RBL会读取NAND的ID信息，然后在RBL的代码里面的NAND ID 列表，从而得知更详细的NAND Flash的信息，例如页（page）大小等，对EMIF做好相应的配置。DM368支持启动的NAND的ID信息可以在参考文档1（ARM子系统用户手册）里面找到。硬件选型时，请务必选择在NAND
 ID列表里面支持的NAND芯片。
接下来，RBL会在NAND Flash的第1块的第0个页开始查找UBL的描述符。如果没有找一个合法的UBL的特殊数字标志，RBL会继续到下一个块的第0个页查找描述符，最多第24个块。RBL会到多个块里面查找描述符是根据NAND Flash本身容易与坏块的特点而设计的。24块应该足以避免NAND Flash坏块的影响。
如RBL在某块里面找到了合法的UBL描述符，这个块号（block number）就会写到ARM内存最后的32位（0x7FFC~0x8000）用于调试时候使用，然后UBL描述符的具体内容将被读取并且处理。UBL描述符告诉RBL关于下载和将控制权交给UBL所需要的信息，具体见表1.
|**第0页地址**|**32位**|**描述**|
|----|----|----|
|0|0xA1AC Edxx|特殊数字标志|
|4|UBL入口地址|用户启动代码的入口地址（绝对地址）|
|8|UBL使用的页数|页数（用户启动diamond的大小）|
|12|UBL开始的块号|开始存放用户启动代码的块号（block number）|
|16|UBL开始的页数|开始存放用户启动代码的页数|
|20|PLL设置-M|PLL设置- 倍率（仅在特殊数字标准表示PLL使能的时候有效）|
|24|PLL设置-N|PLL设置- 分率（仅在特殊数字标准表示PLL使能的时候有效|
|28|快速EMIF设置|快速EMIF设置（仅在特殊数字标准表示快速EMIF启动的时候有效）|
表1 NAND UBL描述符 
一旦用户需要的启动设置配置好，RBL就会从0x0020第地址开始把UBL搬移到ARM内存。在从NAND读取UBL的过程中中，RBL会使用4位的硬件ECC对NAND Flash上的数据进行检错和纠错。如果因为其他原因读失败，复制会立即停止，RBL会在下个块里面继续寻找特殊数字标志。
对于UBL的描述符有几点注意事项：
1.   入口地址必须在0x0020到0x781C之间 
2.   存放UBL的页必须是连续的页，可以分布在多个块内，总共大小必须小于30KB。 
3.   UBL的起始块号（block number）可以是和存放UBL描述符的块号一样。 
4.   如果UBL的起始块号是和存放UBL描述符的块号一样， 那UBL的起始页数一定不可以和UBL描述符存放的页数一样。
但RBL根据UBL描述符里提供的UBL大小信息将UBL全部成功复制到ARM内存后，RBL会跳到UBL起始地址，这样芯片的控制权就交给了UBL，UBL开始在ARM内存里运行了。
也许你会问，既然RBL可以把NAND Flash上的内容复制到ARM内存里运行，为什么我们不直接把U-Boot复制到内存运行？原因是ARM内存太小。一般的U-Boot都是大于100KB，而DM365上可以用于启动的内存只有30KB。也许你又要问了，那为什么不把U-Boot直接复制到DDR上运行，DDR有足够大的空间？这个原因是，芯片上电后并无法知道用户在DM365的DDR2接口上接的DDR信息，RBL也就无法初始化DDR，在RBL运行的阶段DDR是不可用的。这也是为什么UBL里面初始化DDR是它的一项重要任务。
当NAND启动失败的时候，RBL会继续尝试MMC/SD启动方式。如果你系统使用NAND启动，但NAND上的内容损坏了，如果你的板子上有SD卡接口，也可以改变启动方式，那你可以用SD卡先把系统启动起来，然后重新烧写NAND Flash上的内容。这可以作为产品失效后在客户侧的一个补救方法。
二．NAND Flash启动的软件配合实现 
现在我们知道了DM368 NAND Flash启动的原理，下面我们来看看软件是如何根据并配合硬件的要求实现启动的。在DM368 IPNC的软件包里面有一个工具的目录，里面有预先编译好的烧写NAND的CCS的可执行文件， UBL的二进制文件以及相关源码。
刚才在介绍NAND Flash启动原理的时候，我们提到了RBL需要到NAND Flash上面搜索特殊数字标志。这个特殊数字标志就是由烧写NAND的CCS的工程写到Flash上的。在flash_utils_dm36x_1.0.0\flash_utils_dm36x\DM36x\CCS\NANDWriter\src\nandwriter.c里面的LOCAL_writeHeaderAndData()函数就是用来写描述符的。
// Setup header to be written
headerPtr = (Uint32 *) gNandTx;
headerPtr[0] = nandBoot->magicNum; //Magic Number
headerPtr[1] = nandBoot->entryPoint; //Entry Point
headerPtr[2] = nandBoot->numPage; //Number of Pages
#if defined(IPNC_DM365) || defined(IPNC_DM368)
headerPtr[3] = blockNum+3; //Starting Block Number
headerPtr[4] = 0; //Starting Page Number - always start data in page 1 (this header goes in page 0)
对比表1，你可看到headerPtr[3]的内容是用来存放UBL代码的起始块号。这里+3的意思就是UBL是存放在UBL描述符所放块号后面的第三块里面。headerPtr[4] = 0表示是从第0页开始存放。当然这个值用户是可以修改的。只要你烧写UBL代码的位置和描述符里面的起始块/页数一致就可以了。
在IPNC的代码里面UBL的描述符是会从NAND Flash的第1个块开始写，如果块是好的，就放在第1块的第0页。如果第1块是坏的，就会把UBL的描述符写入到下一个块的第0页。IPNC的代码里面没有将UBL描述符可能有的块号从1到24块（这是RBL搜索的范围），它只是从第1块到第3块。如果UBL描述符放在第1块，那如果第4块是好的话，UBL的代码就从第4块的第0页开始放。
#elif defined(IPNC_DM368)
// Defines which NAND blocks the RBL will search in for a UBL image
#define DEVICE_NAND_RBL_SEARCH_START_BLOCK (1)
#define DEVICE_NAND_RBL_SEARCH_END_BLOCK (3)
在nandwriter.c里面你还可以看到UBL的入口地址是固定的0x100。
gNandBoot.entryPoint = 0x0100; // This fixed entry point will work with the UBLs
要了解为什么是0x100，你就必须要看一下UBL的源码。在UBL源码的UBL.cmd文件里面，你可以看到下面的定义，将入口地址放在boot的地方，而boot的运行地址就是0x100。
-e boot //指定入口地址为boot
...
MEMORY
{
...
UBL_I_TEXT (RX) : origin = 0x00000100 length = 0x00004300
...
UBL_F_TEXT (R) : origin = 0x020000E0 length = 0x00004300
...
}
SECTIONS
{
... 
.text : load = UBL_F_TEXT, run = UBL_I_TEXT, LOAD_START(FLASHTEXTStart), LOAD_SIZE(FLASHTEXTSize)
{
*(.boot) 
. = align(4);
*(.text)
. = align(4);
} 
....
}
在UBL的源码boot.c里面有强制把启动的最初代码放在了boot的section里面。
#if defined(__TMS470__)
...
#pragma CODE_SECTION(boot,".boot");
#endif
void boot(void)
{
...
}
这样从cmd的配置以及代码指定代码段，UBL的程序就能确保是从0x100的地址开始运行。
UBL启动U-Boot的过程，借鉴了RBL启动UBL的原理。烧写描述符也是用同样的LOCAL_writeHeaderAndData()函数。在nandwriter.c里面，我们把U-Boot的代码叫做应用代码（APP）。
// Defines which NAND blocks are valid for writing the APP data
#define DEVICE_NAND_UBL_SEARCH_START_BLOCK (8)
#define DEVICE_NAND_UBL_SEARCH_END_BLOCK (10)
下面是IPNC启动后串口最初的打印。
Valid magicnum, 0xA1ACED66, found in block 0x00000008.
DONE
Jumping to entry point at 0x81080000.
我们可以看到UBL是指第8块的地方找到了U-Boot的描述符，这个DEVICE_NAND_UBL_SEARCH_START_BLOCK的定义是一致的。
IPNC代码支持在U-Boot里面更新UBL或者U-Boot自己。下面是烧写ubl和U-Boot在U-Boot下的命令。
烧写ubl：
nand write 0x80700000 0x080000 0x08000
烧写U-Boot：
nand write 0x80700000 0x160000 0x28000
要了解为什么NAND Flash的烧写地址是0x80000和0x160000，这还是需要了解nandwriter.c里面的烧写流程。从前面的内容我们可以得知，nandwriter.c烧写UBL是从1+3=4块开始的，而烧写U-Boot是从8+3=11块。在IPNC上使用的NAND Flash是2K一个页，每个块128KB。所以UBL烧写的地址是128KBx4=0x80000，而烧写U-Boot的地址是128Kx11=0x160000。
所以如果在没有NAND Flash坏块的情况下，nandwriter.c会把UBL的描述符烧写在第1块第0页上，把UBL的代码烧写在第4块第0页上，把U-Boot（APP）的描述符烧写在第8块第0页上，把U-Boot的代码烧写在第11块第0页上。这样芯片在上电确认是NAND Flash启动后，RBL在执行的时候就会找到UBL相应的描述符，把UBL加载的ARM内存里运行。而UBL又找到了U-Boot的描述符，把U-Boot加载到DDR上运行。最后U-Boot加载uImage并启动了Linux，完成了从上电到Linux启动的整个过程。
三．结束语 
每个芯片一般都有多种启动方式，各个芯片的启动方式都有所不同，但又有类似的地方。上面的介绍也可以作为学习其他芯片其他启动方式的一个参考。
最后感谢李斌在本文整理过程中的帮助！
参考文献
1. TMS320DM36x Digital Media System-on-Chip (DMSoC) ARM Subsystem User's Guide Literature Number: SPRUFG5A
2. TMS320DM368 datasheet Literature Number: SPRS668C
