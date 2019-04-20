# DSP程序开发与优化经验之四：TMS320C64x+ DSP------Using Cache - leegang12的专栏 - CSDN博客
2014年02月22日 23:03:05[leegang12](https://me.csdn.net/leegang12)阅读数：1279
[转]
一、配置cache
>配置L1 Cache：
CACHE_L1pSetSize(); CACHE_L1dSetSize();
>配置L2 cache：
启动时默认状态下L2 cache被disabled, 全部L2为SRAM。如果启用了DSP/BIOS，则L2 cache被自动使能；否则，可以通过调用CSL命令：CACHE_L2SetSize()来使能L2 cache。
>外部内存的cacheability 
L1D 和 L2可以通过调用CSL命令CACHE_enableCaching(CACHE_MARi)修改对应MAR-bit来控制外部内存段的cacheability; 而对L1P，外部内存总是cacheability的，跟MAR无关。
注意：配置成cache的地址段就不能再放入data and code,因为链接的时候，是不包含cache地址段的，如果要使用L1D SRAM或L1P SRAM，
则应该相应减小cache段大小。
>example for C64x+ Linker Command File
MEMORY
{
L2SRAM : origin = 00800000h length = 001C0000h
CE0 : origin = 80000000h length = 01000000h (DDR第一个16M配置成可cache)
}
SECTIONS
{
.cinit > L2SRAM
.text > L2SRAM
.stack > L2SRAM
.bss > L2SRAM
.const > L2SRAM
.data > L2SRAM
.far > L2SRAM
.switch > L2SRAM
.sysmem > L2SRAM
.tables > L2SRAM
.cio > L2SRAM
.external > CE0
}
>example for C64x+ CSL Command Sequence to Enable Caching
[
#include <csl.h>
#include <csl_cache.h>
...
CACHE_enableCaching(CACHE_CE00);
CACHE_setL2Size(CACHE_256KCACHE);
]
二、cache一致性问题
如果内存是共享的，可以cache访问的，并且被修改了，那么这块内存就存在cache一致性维护的问题。对目前我们简单的编解码器移植来说，存在ARM与DSP的共享，但不存在同时修改的问题，故无需一致性维护。 对C64x+ DSPs来说，其cache 控制器可以根据snoop command以硬件cache一致性协议来自动维护由CPU EDMA/IDMA所访问数据的一致性。当DMA发起读和写命令时就激活了一致性维护机制。当DMA读L2 SRAM cache内存时，数据直接从L1D
 cache提交给DMA，而不会在L2 SRAM更新；当DMA写时，数据直接提交到L1D cache，并且更新L2 SRAM。
>除了一致性操作，对于DMA buffer,最好将其按L2 cacheline对齐，并保证其为cachelines size的整数倍，可以通过以下方式保证：
#pragma DATA_ALIGN(InBuffA, CACHE_L2_LINESIZE)
#pragma DATA_ALIGN(InBuffB, CACHE_L2_LINESIZE)
#pragma DATA_ALIGN(OutBuffA,CACHE_L2_LINESIZE)
#pragma DATA_ALIGN(OutBuffB,CACHE_L2_LINESIZE)
unsigned char InBuffA [N*CACHE_L2_LINESIZE];
unsigned char OutBuffA[N*CACHE_L2_LINESIZE];
unsigned char InBuffB [N*CACHE_L2_LINESIZE];
unsigned char OutBuffB[N*CACHE_L2_LINESIZE];
位置规律
使用Cache是基于时间和空间两种规律，具体说就是CPU读取内存中的某一数据或指令很可能在下一时钟周期或很短的时间内会使用这一指令相邻的指令或数 据（空间），有时候当前使用的指令或数据会重复使用（时间），所以CPU不仅将当前指令读进Cache，而且会将相邻指令一并读入，读入的大小取决于 Cache的大小和Cache line的大小.
L1P是direct-mapped型，16K，512个line,每个line32byte,正好可以长一个取指包，8条指令。
Cache缺失有必然缺失，冲突缺失，容量缺失。
L1D是set-associative型，16K，2way,每way128line,每line64byte,LRU指向最近最少使用的line,没 有write miss,也就是说L1D如果没有要写的数据，通过write buffer（write buffer 4个入口，每个入口64bit)直接写到下一级存储器，如果L1D又要写的数据就直接写,并且dirty bit 置1，表示如果要替换该line的数据要先write back该line的数据到下一级存储器，
DM642有最大可配置为256K的二级Cache,一级cache read miss向二级cache查找，如果没有就有二级cache向外部存储器读，然后传给一级cache，最后送给CPU，如果这期间哪一级line的 dirty bit是1，就要向下一级cache进行write back操作。对于不可cache的外部存储器有CPU直接read.如果CPU对于二级cache write miss,二级cache首先要从外部存储器读入相应数据，CPU直接在二级cache上修改该数据，不会再将数据cache到L1D，因为L1D是
 read-allocate,同样如果二级cache相应line的dirty bit是1就要先write back。
两种情况：1 DMA读L2RAM某一line ，首先L2CACHE会检查该line是否被cache在L1D，如果不是，DMA直接读取L2RAMzhogndeshuju ，如果是会将L1D 中相应line的invalidate置位，并且write back数据到L2RAM中。2 DMA写L2RAM某一line,情况差不多。
对于一般的CPU信号处理加上外围设备的DMA数据读写情况，处理方法是在内存中开辟pingpong缓冲区。inbufa,outbufa, inbufb,outbufb四块内存区，当CPU处理inbufa的数据并将产生的结果放到outbufa,这是外围器件DMA搬移数据到 inbufb,另一外围设备将数据从outbufb搬出；同理当CPU处理inbufb并将产生的结果放到outbufb时，外围设备利用DMA分别从 inbufa和outbufa搬入数据和搬出数据。一般L2RAM DMA实现代码如下：
for (i=0; i<(DATASIZE/BUFSIZE)–2; i+=2)
{
/* –––––––––––––––––––––––––––––––––––––––––––––––––––– */
/* InBuffA –> OutBuffA Processing */
/* –––––––––––––––––––––––––––––––––––––––––––––––––––– */
<DMA_transfer(peripheral, InBuffB, BUFSIZE)>
<DMA_transfer(OutBuffB, peripheral, BUFSIZE)>
process(InBuffA, OutBuffA, BUFSIZE);
/* ––––––––––––––––––––––––––––––––––––––––––––––––––––– */
/* InBuffB –> OutBuffB Processing */
/* ––––––––––––––––––––––––––––––––––––––––––––––––––––– */
<DMA_transfer(peripheral, InBuffA, BUFSIZE)>
<DMA_transfer(OutBuffA, peripheral, BUFSIZE)>
process(InBuffB, OutBuffB, BUFSIZE);
}
上面CACHE的一致性由CPU自动管理，无须程序员设置，而下面要说的片外RAM开辟双缓冲区时要求程序员掌握L2cache 和片外RAM coherence(一致性）以及L2RAM和L1D的一致性，否则程序会出错。
对于C64x系列，无论什么时候当片外设备DMA写片外RAM开辟的inbuf时，都要使用CACHE_invL2(InBuffB, BUFSIZE, CACHE_WAIT)，使L1D当中的inbuffb无效；当片外设备DMA读片外RAM开辟的outbuf时，都要使用CACHE_wbL2 (OutBuffB, BUFSIZE, CACHE_WAIT)，使L1D当中的相应数据write back 到片外RAM .
External Memory DMA Double Buffering Code Example
for (i=0; i<(DATASIZE/BUFSIZE)–2; i+=2)
{
/* ––––––––––––––––––––––––––––––––––––––––––––––––––––– */
/* InBuffA –> OutBuffA Processing */
/* ––––––––––––––––––––––––––––––––––––––––––––––––––––– */
CACHE_wbInvL2(InBuffB, BUFSIZE, CACHE_WAIT);
<DMA_transfer(peripheral, InBuffB, BUFSIZE)>
CACHE_wbL2(OutBuffB, BUFSIZE, CACHE_WAIT);
<DMA_transfer(OutBuffB, peripheral, BUFSIZE)>
process(InBuffA, OutBuffA, BUFSIZE);
/* ––––––––––––––––––––––––––––––––––––––––––––––––––––– */
/* InBuffB –> OutBuffB Processing */
/* ––––––––––––––––––––––––––––––––––––––––––––––––––––– */
CACHE_wbInvL2(InBuffA, BUFSIZE, CACHE_WAIT);
<DMA_transfer(peripheral, InBuffA, BUFSIZE)>
CACHE_wbL2(OutBuffA, BUFSIZE, CACHE_WAIT);
<DMA_transfer(OutBuffA, peripheral, BUFSIZE)>
process(InBuffB, OutBuffB, BUFSIZE);
}
虽然我们可以指定一定大小的buf被write back或者invalidate或者write back+invalidate,但是cache controller是对完整的line操作，这就要求我们在片外RAM开辟内存( 或者数组）作buffer时，尽量使得其大小是CACHE_L2_LINESIZE（128）的整数倍并且CACHE_L2_LINESIZE对齐，为此 我们需要：
#pragma DATA_ALIGN(InBuffA, CACHE_L2_LINESIZE)
#pragma DATA_ALIGN(InBuffB, CACHE_L2_LINESIZE)
#pragma DATA_ALIGN(OutBuffA,CACHE_L2_LINESIZE)
#pragma DATA_ALIGN(OutBuffB,CACHE_L2_LINESIZE)
来对齐buffer。
使用宏#define CACHE_ROUND_TO_LINESIZE(cache,elcnt,elsize) \
((CACHE_#cache#_LINESIZE * \
((elcnt)*(elsize)/CACHE_#cache#_LINESIZE) + 1) / \
(elsize))
来对其内存，使用如下;
unsigned char InBuffA [CACHE_ROUND_TO_LINESIZE(L2, N, sizeof(unsigned char)];
unsigned char OutBuffA[CACHE_ROUND_TO_LINESIZE(L2, N, sizeof(unsigned char)];
unsigned char InBuffB [CACHE_ROUND_TO_LINESIZE(L2, N, sizeof(unsigned char)];
unsigned char OutBuffB[CACHE_ROUND_TO_LINESIZE(L2, N, sizeof(unsigned char)]; 
这样我们得到的数组就是内存对齐并且其大小是CACHE_L2_LINESIZE的整数倍，尽管这样做浪费片外RAM。由于L2CACHE最大256K，所以如果生命的数组大小超过256K，就需要多用几次cache write back和invalidate函数。
如果程序没有配置L2CACHE，也就是说片内256K都作为RAM，那么当利用片外RAM开辟buffer时要考虑的L1D和L1P与片外设备的一致性问题，也有一组相应的write back和invalidate函数对应。
3 基于cache的程序优化
3.1 应用级优化 （application-level optimization)
1) 合理设置cache大小，尽量将DMA用到的buffer开在片内RAM上
2) 将一般性程序代码和数据放到片外RAM，将DSP型代码和数据放到L2RAM。所谓一般性代码是指带有很多条件分支转移的指令，程序执行在空间上有随意 性，不利于流水线的形成，外在片外可以发挥L2CACHE 4 way的优势。DSP型代码是指算法型的代码，放在L2RAM，CPU stall 时间少，可以充分发挥DSP速度快的优势。
3.2 程序级优化（procedural-level optimization)
1)选择合适的数据类型。能用short就不要用int。
2)将同一个函数要处理的数据尽量在内存中连续存放。
3.3避免L1P read miss
这种情况发生在一个循环体中有两个或以上的函数要执行，要利用#pragma DATA_SECTION伪指令将和CMD文件将其在内存中相邻定位，这样不会发生两个程序对应L1P中相同line所造成的冲突缺失。
若果循环体中的两个函数大小超过L1P容量，将这两个函数分别放到两个循环体中。这样做会造成中间数据变量的加大。
3.4避免L1D read miss
利用#pragma DATA_SECTION伪指令将函数要同时处理的数组在内存中相邻存放。最好再用#pragma DATA_MEM_BANK 将数组内存对齐
