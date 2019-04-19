# wince bsp中的g_oalAddressTable表----静态映射/动态映射 - xqhrs232的专栏 - CSDN博客
2014年04月08日 15:22:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：845
原文地址::[http://www.cnblogs.com/NorthCan/archive/2011/07/29/2121259.html](http://www.cnblogs.com/NorthCan/archive/2011/07/29/2121259.html)
相关文章
1、wince6.0下物理内存的映射过程分析----[http://download.csdn.net/download/jiwenge9999/3625920](http://download.csdn.net/download/jiwenge9999/3625920)
2、关于的OEMAddressTable/g_oalAddressTable和config.bib的全解析----[http://bbs.csdn.net/topics/290035690](http://bbs.csdn.net/topics/290035690)
3、Wince内存映射----[http://bbs.ednchina.com/BLOG_ARTICLE_2018533.HTM](http://bbs.ednchina.com/BLOG_ARTICLE_2018533.HTM)
* g_oalAddressTable*数组相当于一张从物理地址到虚拟地址的映射表。该数组在\WINCE600\PLATFORM\SMDK6410\SRC\INC\oemaddrtab_cfg.inc文件中。g_oalAddressTable数组的元素类型是一个结构体：
  typedef struct{
    UINT32  CA;        // cached virtual address
    UINT32  PA;        // physical address
    UINT32  size;      // size, in MB bytes
} OAL_ADDRESS_TABLE, *POAL_ADDRESS_TABLE;
CA和PA分别是映射的虚拟地址和物理地址的起始地址，size是以MB为单位的容量大小。下面是飞凌OK6410开发板bsp中g_oalAddressTable表的具体实现：
g_oalAddressTable
DCD     0x80000000, 0x50000000,  256  ; 256 MB DRAM
  DCD     0x90000000, 0x70000000,  4    ; SROM SFR
  DCD     0x90400000, 0x71000000,  4    ; TZIC0
  DCD     0x90800000, 0x72000000,  1    ; FIMG-3DSE SFR           
  DCD     0x90A00000, 0x74000000,  2    ; Indirect Host I/F
  DCD     0x90C00000, 0x74300000,  2    ; USB Host
  DCD     0x90E00000, 0x75000000,  2    ; DMA0
  DCD     0x91000000, 0x76100000,  3    ; 2D Graphics
  DCD     0x91300000, 0x77000000,  3    ; Post Processor
  DCD     0x91600000, 0x78000000,  1    ; Camera I/F
  DCD     0x91700000, 0x78800000,  1    ; JPEG
  DCD     0x91800000, 0x7C000000,  5    ; USB OTG LINK
  DCD     0x91D00000, 0x7D000000,  13   ; D&I(Security Subsystem Config) SFR
  DCD     0x92A00000, 0x7E000000,  1    ; DMC,MFC,WDT,RTC,HIS TX/RX, Keypad, ADC, SYSCON
  DCD     0x92B00000, 0x7F000000,  1    ; TZPC, AC97, I2S, I2C, UART, PWM, IrDA, GPIO, PCM, SPI
DCD     0x93000000, 0x00000000,  16   ; 32 MB SROM(SRAM/ROM) BANK 0
  DCD     0x94000000, 0x18000000,  32   ; 32 MB SROM(SRAM/ROM) BANK 1 = DM9000A
  DCD     0x00000000, 0x00000000,  0    ; end of table
第一行：DCD 0x80000000, 0x50000000,  256  ; 256 MB DRAM
从这一行可以看出，我这块开发板用的是256MB的RAM，为什么物理地址映射的是0x50000000，这可以从S3C6410的数据手册上找到一个“Device Specific Address Space”表：
![](http://pic002.cnblogs.com/images/2011/313067/2011072917065372.jpg)
  从整体上看，g_oalAddressTable表中映射的虚拟地址在0x80000000至0x9FFFFFFF之间，因为这一块虚拟地址在windows ce操作系统中用作（带缓存的）静态映射。256M的物理RAM首先映射到0x80000000虚拟地址上。
  第二行映射的物理地址是0x70000000，从S3C6410的数据手册中同样可以找到这是SROM SFR（特殊功能寄存器）的物理地址。
![](http://pic002.cnblogs.com/images/2011/313067/2011072917073289.jpg)
  RAM从0x80000000虚拟地址开始，映射的容量大小是256M（256*1024*1024=0x1000000），所以这块SROM映射的虚拟地址是紧挨着RAM。
  看懂了前两行，后面的也都没什么问题了，都是一些SFR或者外设的映射。*g_oalAddressTable*表是后续BootLoader和OAL构造一级页表的根据。
//===========================================================================================================================
备注::
1>wince软件结构里对应MMU的是一个名为OEMAddressTable的数据结构（源文件oeminit.asm中），其中建立了物理地址和虚拟地址的静态映射关系，也可以在其中改动系统所能识别物理内存的大小,以支持大内存。
2>如果要在驱动程序中访问设备寄存器，必须建立动态虚拟地址映射，可以调用MmmapIoSpace函数来实现，或者通过VirtualAlloc和VirtualCopy函数来实现。其实MmmapIoSpace内部就调用了后者。
3>在驱动中访问虚拟地址时，必须是非缓存段（位于0xA0000000 到
 0xBFFFFFFF ）

